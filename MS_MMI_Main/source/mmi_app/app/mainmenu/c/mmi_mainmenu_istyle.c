/*****************************************************************************
** File Name:      mmi_mainmenu_istyle.c                                     *
** Author:                                                                   *
** Date:           10/25/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu             Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif


#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_ISTYLE_SUPPORT

#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_data.h"
#include "mmitheme_mainmenu.h"
#include "mmitheme_mainmenu.h"
#include "mmi_gui_trc.h"
#include "mmi_mainmenu_item_func.h"

#include "mmi_app_menu_trc.h"
#include "guimenu.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_modu_main.h"
#include "mmidisplay_data.h"
#include "guistring.h"
#include "mmi_image.h"
#include "ui_layer.h"
#include "mmi_link.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmi_mainmenu_export.h"
#include "mmk_tp.h"
#include "mmiudisk_export.h"
#include "mmisearch_internal.h"
#include "mmitheme_special.h"
#include "mmi_menutable.h"
#include "guiimg.h"
#include "mmiidle_export.h"
#include "img_dec_interface.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#include "mmisd_export.h"
#endif
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
#include "mmi3d_export.h"
#endif
#include "guibutton.h"
#include "graphics_draw.h"
#include "guilabel.h"
#include "guitext.h"
#include "guictrl_api.h"
#include "mmipub.h"
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmitheme_mainmenu.h"
#include "mmisearch_export.h"

#ifdef UI_SUPPORT_U_MAINMENU
#include "mmi_mainmenu_istyle_u_se.h"
#endif

#include "mmi_mainmenu_istyle.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define FIND_NONE (0xffffffff)

//菜单文件夹默认group_id"-2"

#define MAINMENU_FILEFOLD_NAME_SIDE_MARGIN  5
#define MAINMENU_FILEFOLD_NAME_RECT_HEIGHT  25
#define MAINMENU_FILEFOLD_NAME_FONT         20

#define MENU_SHAKE_ANGLE             6

#define MAINMENU_CLICK_ALPHA_MASK    125    //alpha mask for istyle click icon
#define MAINMENU_SEARCH_MARGIN      5       //margin between statusbar and search page
#define MAINMENU_SEARCH_HORIZON_MARGIN      10//margin between search page and screen
#define MAINMENU_SEARCH_EDIT_MARGIN     25//margin between search page and screen
#define MAINMENU_SEARCH_HORIZON_MOVE_SPACING 20

typedef uint32 ISTYLEMENU_CHILD_FLAG;
#define ISTYLE_MENU_CHILD_CONTROL_EDIT       0x00000001
#define ISTYLE_MENU_CHILD_CONTROL_LIST       0x00000002
#define ISTYLE_MENU_CHILD_CONTROL_BUTTON     0x00000004

#define ISTYLE_MENU_CHILD_CONTROL_NONE      0
#define ISTYLE_MENU_CHILD_CONTROL_EDIT_TEXT (ISTYLE_MENU_CHILD_CONTROL_EDIT|ISTYLE_MENU_CHILD_CONTROL_BUTTON)
#define ISTYLE_MENU_CHILD_CONTROL_ALL       (ISTYLE_MENU_CHILD_CONTROL_EDIT|ISTYLE_MENU_CHILD_CONTROL_LIST|ISTYLE_MENU_CHILD_CONTROL_BUTTON)


#define ISTYLE_MENU_ZOOM_OUT                  120


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL const uint32 istyle_bar[] =
{
    ID_COMMUNICATION_DIAL,
    ID_COMMUNICATION_MESSAGE,
#ifdef  BROWSER_SUPPORT
    ID_COMMUNICATION_BROWSER,
#endif
    ID_COMMUNICATION_PB,
};

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

//temp struct define
#define ITEM_FROM_PAGE (1)
#define ITEM_FROM_FOLDER (2)
#define ITEM_FROM_BAR (4)

struct menu_item_flag_t
{
    uint32 group_id;
    uint32 menu_id;
    uint32 flag;
    union
    {
        struct
        {
            uint32 page;
            uint32 item;
        } page, bar;

        struct
        {
            uint32 folder;
            uint32 item;
        } folder;
    } pos;
};




/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL uint8 s_shake_num = 0;
LOCAL GUI_RECT_T s_left_direct_move_rect = {0};
LOCAL GUI_RECT_T s_right_direct_move_rect = {0};
LOCAL wchar            *s_search_word_ptr = PNULL;      //record search history when slide out search page

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL void *IStyleInit(
    MMI_HANDLE_T ctrl_handle
);

LOCAL BOOLEAN IStyleTerm(
    void *pm_data_ptr
);


LOCAL void IStyleStartShakeIconTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleStopShakeIconTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL MMI_RESULT_E IStyleHandle(
    void *pm_data_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

LOCAL BOOLEAN GetNvPageInfo(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL BOOLEAN GetNvBarInfo(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL BOOLEAN GetNvFolderInfo(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void SaveNv(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void SetDefaultPage(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void SetDefaultBar(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void IStyleInitDetail(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL uint32 FindItemFlagIndex(struct menu_item_flag_t *items_flag_ptr, uint32 item_cnt, uint32 menu_id);

LOCAL BOOLEAN DeleteItemFromPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 page_index,
    uint32 item_index
);

LOCAL BOOLEAN DeletePage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 page_index
);

LOCAL BOOLEAN InsertItemToPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 page_index,
    uint32 item_index
);

LOCAL BOOLEAN AppendItemToPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 page_index
);

LOCAL BOOLEAN AddItemToPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr
);

LOCAL BOOLEAN DeleteItemFromBar(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 item_index
);

LOCAL BOOLEAN InsertItemToBar(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 item_index
);

LOCAL BOOLEAN DeleteItemFromFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 folder_index,
    uint32 item_index
);

LOCAL BOOLEAN DeleteFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 folder_index
);

LOCAL BOOLEAN InsertItemToFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 folder_index,
    uint32 item_index
);

LOCAL BOOLEAN AppendItemToFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 folder_index
);

LOCAL uint32 FindFolderIndex(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr, uint32 menu_id);

LOCAL BOOLEAN FindItemFromFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 menu_id,
    uint32 *folder_idx_ptr,
    uint32 *item_idx_ptr
);

LOCAL BOOLEAN FindItemFromPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 menu_id,
    uint32 *page_idx_ptr,
    uint32 *item_idx_ptr
);

LOCAL void VerifyItems(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void VerifyPages(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void VerifyFolders(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);


LOCAL BOOLEAN IStyleGetInfo(
    MMI_HANDLE_T          win_handle,
    GUIMENU_ISTYLE_DATA_T *menu_info_ptr);

LOCAL void GetIconTextRect(
    GUI_RECT_T  item_rect,
    GUI_RECT_T *icon_rect,
    GUI_RECT_T *text_rect
);

LOCAL void IStyleDrawPageLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    UILAYER_HANDLE_T layer_handle,
    int16            offset_x,
    BOOLEAN          is_need_bar
);

LOCAL GUI_RECT_T IStyleGetRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr);

LOCAL void IStyleCreatePageLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr);

LOCAL void IStyleReleasePageLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr);

LOCAL void IStyleDrawPageLayers(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    int16          offset_x
);

LOCAL void MoveBarLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    int16          offset_x
);

LOCAL void MovePageLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T   *point_ptr
);

LOCAL BOOLEAN GetFolderItemRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    uint16           index,
    GUI_RECT_T       *rect_ptr
);

LOCAL BOOLEAN GetCurItemRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_RECT_T       *rect_ptr
);

LOCAL void SetMoveIconLayerPosition(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void CreateMoveIconLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleReleaseMoveIconLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void ClearCurItemRect(
    MMIMAINMENU_ISTYLE_DATA_T  *menu_ctrl_ptr
);

LOCAL void IStyleInitMoveLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleAppendLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO lcd_dev_info
);

LOCAL BOOLEAN IStyleMoveInit(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN IStyleConfirmItem(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
);

LOCAL BOOLEAN IStyleResetBarIndexRect(
    GUIMENU_ISTYLE_DATA_T *menu_info_ptr,
    uint16 item_num
);

LOCAL void IStyleResetAutoSwitchPageRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN CreateIStyleMenuItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_RECT_T layer_rect,
    GUIMENU_ITEM_BUF_T *item_buf_ptr
);

LOCAL void DestoryIStyleItemLayer(
    GUIMENU_ITEM_BUF_T *item_buf_ptr
);

LOCAL BOOLEAN SetIStyleMoveItemInfo(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN ResetIStyleMoveItemInfo(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void DestroyIStyle(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleModifyRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleDraw(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr
);

LOCAL void IStyleDrawBar(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    uint16             cur_page_index,
    GUI_LCD_DEV_INFO   lcd_dev_info,
    BOOLEAN            is_draw_all
);

LOCAL void OpenFolderCallBack(uint32 menu_ctrl_ptr, uint32 dummy);

BOOLEAN IStyleGetItemData(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 menu_id,
    uint32 group_id,
    CTRLMAINMENU_ITEM_T *item_ptr
);

LOCAL GUI_LCD_DEV_INFO IStyleDrawItem(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dst_dev_ptr,
    GUI_RECT_T *rect_ptr,
    uint16 page_index,
    uint16 item_index,
    GUIMENU_ITEM_POSITION_E position
);

LOCAL void IStyleDrawAllItems(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    uint16              page_index,
    GUI_LCD_DEV_INFO   lcd_dev_info,
    int16 offset_x,
    BOOLEAN is_need_icon,
    BOOLEAN is_need_bar
);

LOCAL void GetBarItemRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_RECT_T       rect_ptr[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX],
    uint32           num
);

LOCAL void ShakeItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T          *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO  *in_ptr,
    GUI_LCD_DEV_INFO        *out_ptr,
    uint16                  index,
    BOOLEAN                 is_bar_item
);

LOCAL void InvertBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T          *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO  *in_ptr,
    const GUI_LCD_DEV_INFO  *out_ptr
);

LOCAL void DisplayIStyleBarItem(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO   lcd_dev_info,
    BOOLEAN is_need_shake
);

LOCAL void BltSlideItemLayerToMenuLayerEx(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    GUI_LCD_DEV_INFO lcd_dev_info,
    int16            offset_x,
    BOOLEAN          is_need_bar,
    BOOLEAN           is_need_icon
);

LOCAL void IStyleBltSlideItemLayerToMenuLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    GUI_LCD_DEV_INFO lcd_dev_info,
    int16            offset_x,
    BOOLEAN          is_need_bar
);

LOCAL GUI_LCD_DEV_INFO GetCurrentPageLyaerInfo(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void StopItemExchangeTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void StartItemExchangeTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL MMI_RESULT_E IStyleHandleTimer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

LOCAL MMI_RESULT_E IStyleNotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
);

LOCAL BOOLEAN IStyleHandleCancelKey(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
);

LOCAL BOOLEAN HandleIStyleAutoMove(
    MMIMAINMENU_ISTYLE_DATA_T       *menu_ctrl_ptr,
    CONTINUE_MOVE_TYPE_E continue_move_type,
    BOOLEAN is_add_page
);

LOCAL void AutoSlidePage(
    MMIMAINMENU_ISTYLE_DATA_T      *menu_ctrl_ptr,
    uint16               next_page_index,
    CONTINUE_MOVE_TYPE_E continue_move_type
);

LOCAL BOOLEAN IStyleAddNewPage(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void ClickEffectOnIcon(
    MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr
);

LOCAL void IStyleHandleTpDown(
    GUI_POINT_T       *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr
);

LOCAL BOOLEAN IStyleHandleAutoSwitchPage(
    GUI_POINT_T *point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL GUI_RECT_T GetIStyleMoveLayerRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN HandleIStyleMove(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr
);

LOCAL BOOLEAN IsPointInIconRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T    point,
    GUI_RECT_T     rect
);

LOCAL void HandleItemExchange(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleHandleTpMove(
    GUI_POINT_T *point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN SetSlideCurPage(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void HandleSlideOver(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);

LOCAL BOOLEAN HandleIStylePage0TpUp(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void RecoverPageSlideStateAfterTpTinyMove(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);

LOCAL BOOLEAN IStyleRunItem(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL BOOLEAN IStyleHandleTpUp(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleCreateTitleBarLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleReleaseBarBgLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleCreateBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL GUI_LCD_DEV_INFO GetIStyleBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleReleaseBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleDrawShaking(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

#ifdef DYNAMIC_MAINMENU_SUPPORT
LOCAL void IStyleReload(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void HandleDeleteTpUpMsg(
    GUI_POINT_T *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IsInDyItemRect(
    GUI_POINT_T *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMI_MENU_ID_T *menu_id_ptr,
    BOOLEAN *is_pressed_ptr
);

LOCAL MMI_RESULT_E HandleQueryWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);

LOCAL void DeleteDynamicItem(void);
#endif
LOCAL uint32 GenMenuId(void);

LOCAL void CreateFilefoldEditNameBox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void CreateFilefoldTextNameBox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleDestroyFilefoldNameBox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleGetFilefoldNameFromEditbox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN GetEditRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_RECT_T       *rect_ptr
);

LOCAL BOOLEAN IStyleGetFolderRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_RECT_T *rect_ptr
);

LOCAL BOOLEAN IStyleCloseFilefolder(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
);

LOCAL void IStyleUpdateFolderThumbnail(
    MMIMAINMENU_ISTYLE_DATA_T       *menu_ctrl_ptr,
    uint16                page_index,
    uint16                item_index,
    BOOLEAN               is_bar_item
);

LOCAL BOOLEAN IsCurrentIndexFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32         page_index,
    uint16         item_index,
    BOOLEAN     is_bar_item
);

/* 临时文件夹只显示*/
LOCAL BOOLEAN CreateIStyleFilefoldTemp(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN DestroyIStyleFilefoldTemp(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleCreateFolderLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr);

LOCAL void IStyleReleaseFolderLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr);

LOCAL void BltItemLayerToFolderLayer(
    MMIMAINMENU_ISTYLE_DATA_T           *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO          lcd_dev_info
);

LOCAL void DisplayFolderBg(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO lcd_dev_info
);

LOCAL void IStyleOpenFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL BOOLEAN  AddItemToFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    BOOLEAN  *is_filefold_first_create
);

LOCAL uint32 AddItemNodeToFilefoldNode(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint16        item_index ,
    BOOLEAN       is_bar_item
);

LOCAL BOOLEAN RemoveItemFromFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint16 *dest_page_index_ptr
);

LOCAL BOOLEAN Destroy1ItemFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void HandleSearchSlideOver(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);

LOCAL GUI_RECT_T GetSearchPageRect(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

LOCAL void IStyleInitSearchPageParam(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO lcd_dev_info
);

LOCAL void DisplaySearchBg(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO    lcd_dev_info
);

LOCAL void DisplaySearchPageLayer(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    BOOLEAN             only_refresh_list,
    BOOLEAN             only_refresh_editbox,
    BOOLEAN             refresh_all,
    GUI_LCD_DEV_INFO    lcd_dev_info
);

LOCAL MMI_RESULT_E HandleIStyleSearchDeleteButtonCallBack(void);

LOCAL void SetCtrlLcdDevInfoInSearchPage(GUI_LCD_DEV_INFO lcd_dev_info);

LOCAL void SetCtrlVisibleStateInSearchPage(ISTYLEMENU_CHILD_FLAG flag);

LOCAL void RemoveSearchLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void RecoverSearchLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void CreateSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void CreateSearchPageLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
);

LOCAL void IStyleBltSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO const *lcd_dev_ptr,
    int16            offset_x
);

LOCAL void ReleaseSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    BOOLEAN           free_searchwordptr
);

LOCAL BOOLEAN IsListResponseTpMSGInSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T       point
);

LOCAL BOOLEAN HandleHorizontalMoveInSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO const *lcd_dev_ptr,
    int16            offset_x
);

LOCAL void ResetSearchPageLayerPosition(MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr);

LOCAL BOOLEAN HandleListVerticalMoveInSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr,
    GUI_POINT_T     point
);

LOCAL void GetSearchCtrlHandle(
    MMI_HANDLE_T *edit_handle_ptr,
    MMI_HANDLE_T *list_handle_ptr,
    MMI_HANDLE_T *delete_handle_ptr
);

LOCAL  MMI_RESULT_E SearchPageHookFunc(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

#ifdef UI_SUPPORT_U_MAINMENU

LOCAL BOOLEAN U_SetPageInfo(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint16         page_index,
    void           *page_info_ptr
);
#endif

LOCAL void StartAutoMoveTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);
LOCAL void StopAutoMoveTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);
LOCAL void StartRedrawTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);
LOCAL void StopRedrawTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);
LOCAL void StartMoveStartTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);
LOCAL void StopMoveStartTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Matrix Init
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL void *IStyleInit(
    MMI_HANDLE_T ctrl_handle
)
{
    MMIMAINMENU_ISTYLE_DATA_T *istyle_data = PNULL;

    istyle_data = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ISTYLE_DATA_T));

    if (PNULL == istyle_data)
    {
        SCI_TRACE_LOW("IStyleInit Faild");
        return PNULL;
    }

    SCI_MEMSET(istyle_data, 0, sizeof(MMIMAINMENU_ISTYLE_DATA_T));

    istyle_data->ctrl_handle = ctrl_handle;
    istyle_data->win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);
    IStyleGetInfo(istyle_data->win_handle, &istyle_data->default_data);

    MMITHEME_GetMainMenuTheme(&istyle_data->theme_data);

    istyle_data->theme_data.is_used_slide = TRUE;

    //init page manager data
    IStyleInitDetail(istyle_data);

#ifdef UI_SUPPORT_U_MAINMENU
    istyle_data->U_SetPageInfo = U_SetPageInfo;
#endif

    return (void *)istyle_data;
}

/*****************************************************************************/
//  Description : Matrix Term
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN IStyleTerm(
    void *pm_data_ptr
)
{
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)pm_data_ptr;

    if (PNULL == pm_data_ptr)
    {
        SCI_TRACE_LOW("IStyleTerm Faild");
        return FALSE;
    }

    DestroyIStyle(menu_ctrl_ptr);

    SCI_FREE(pm_data_ptr);

    return TRUE;
}


/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleStartShakeIconTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->shake_icon_timer_id)
    {
        menu_ctrl_ptr->shake_icon_timer_id = MMK_CreateWinTimer(
                                                 menu_ctrl_ptr->ctrl_handle,
                                                 menu_ctrl_ptr->theme_data.icon_shake_period,
                                                 FALSE);
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleStopShakeIconTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->shake_icon_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->shake_icon_timer_id);
        menu_ctrl_ptr->shake_icon_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IStyleHandle(
    void *pm_data_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)pm_data_ptr;
    GUI_POINT_T tp_point = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
        case MSG_CTL_OPEN:
            break;

        case MSG_CTL_PAINT:
            if ((0 != menu_ctrl_ptr->tp_timer_id
                 || 0 != menu_ctrl_ptr->redrew_timer_id
                 || menu_ctrl_ptr->is_move_state)
                && !menu_ctrl_ptr->is_search_entry)
            {
                break;
            }

            IStyleDraw(menu_ctrl_ptr);
            break;

        case MSG_CTL_GET_FOCUS:
            if (menu_ctrl_ptr->is_move_state)
            {
                IStyleStartShakeIconTimer(menu_ctrl_ptr);
            }

            UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);
            IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->titleidx_layer_handle);
            IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->bar_item_layer);
            IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->page_layer_handle[0]);

            if (menu_ctrl_ptr->is_move_state)
            {
                IStyleConfirmItem(menu_ctrl_ptr, MSG_APP_CANCEL);
                menu_ctrl_ptr->is_move_state = FALSE;
            }

            break;

        case MSG_CTL_GET_ACTIVE:
            if (menu_ctrl_ptr->is_move_state)
            {
                IStyleStartShakeIconTimer(menu_ctrl_ptr);
            }

            break;

        case MSG_CTL_LOSE_FOCUS:

            menu_ctrl_ptr->is_move_state = FALSE;
            menu_ctrl_ptr->is_moving = FALSE;
            StopAutoMoveTimer(menu_ctrl_ptr);
            StopRedrawTimer(menu_ctrl_ptr);
            StopMoveStartTimer(menu_ctrl_ptr);

            break;

        case MSG_APP_OK:
            break;

        case MSG_APP_WEB:
            break;

        case MSG_APP_RED:
            if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
            {
                if (!IStyleHandleCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL))
                {
                    result = MMI_RESULT_FALSE;
                }
            }

            break;

        case MSG_NOTIFY_CANCEL:
        case MSG_APP_CANCEL:
            if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
            {
                IStyleHandleCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            }

            break;

        case MSG_TP_PRESS_DOWN:
            //set tp press point
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            IStyleHandleTpDown(&tp_point, menu_ctrl_ptr);
            break;

        case MSG_TP_PRESS_UP:
			{
			BOOLEAN mainmenu_istyle_is_move = FALSE;			
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
			mainmenu_istyle_is_move = menu_ctrl_ptr->is_move_state;
            if (IStyleHandleTpUp(&tp_point, menu_ctrl_ptr) && !mainmenu_istyle_is_move)
            {
                IStyleRunItem(menu_ctrl_ptr);
            }
			}
            break;

        case MSG_TP_PRESS_MOVE:
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            IStyleHandleTpMove(&tp_point, menu_ctrl_ptr);
            break;

        case MSG_TP_PRESS_LONG:
        case MSG_TP_PRESS_SHORT:
            if (menu_ctrl_ptr->is_item_pressed && menu_ctrl_ptr->is_move_state
                && abs(menu_ctrl_ptr->slide_start_point.x - MMK_GET_TP_X(param)) <= MMIMAINMENU_MOVE_SPACING)
            {
                IStyleInitMoveLayer(menu_ctrl_ptr);
            }
            
			break;

        case MSG_NOTIFY_PENOK:
            if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
            {
                IStyleHandleCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            }

            break;


        case MSG_TIMER:
            IStyleHandleTimer(menu_ctrl_ptr, msg_id, param);
            break;

        case MSG_CTRLMAINMENU_SET_MOVE_STATE:
            IStyleMoveInit(menu_ctrl_ptr);
            break;

        case MSG_CTRLMAINMENU_SET_RECT:
            IStyleModifyRect(menu_ctrl_ptr);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}



//
//nv
//
LOCAL BOOLEAN GetNvPageInfo(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MN_RETURN_RESULT_E  nv_result = MN_RETURN_FAILURE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    MMINV_READ(MMINV_MAINMENU_ISTYLE_PAGES, &(menu_ctrl_ptr->page_info), nv_result);


    return (MN_RETURN_SUCCESS == nv_result);
}


LOCAL BOOLEAN GetNvBarInfo(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MN_RETURN_RESULT_E  nv_result = MN_RETURN_FAILURE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    MMINV_READ(MMINV_MAINMENU_ISTYLE_BAR, &(menu_ctrl_ptr->bar_info), nv_result);


    return (MN_RETURN_SUCCESS == nv_result);
}


LOCAL BOOLEAN GetNvFolderInfo(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MN_RETURN_RESULT_E  nv_result = MN_RETURN_FAILURE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    MMINV_READ(MMINV_MAINMENU_ISTYLE_FOLDERS, &(menu_ctrl_ptr->folder_info), nv_result);


    return (MN_RETURN_SUCCESS == nv_result);
}


LOCAL void SaveNv(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MMINV_WRITE(MMINV_MAINMENU_ISTYLE_PAGES, &(menu_ctrl_ptr->page_info));
    MMINV_WRITE(MMINV_MAINMENU_ISTYLE_BAR, &(menu_ctrl_ptr->bar_info));
    MMINV_WRITE(MMINV_MAINMENU_ISTYLE_FOLDERS, &(menu_ctrl_ptr->folder_info));

    return;
}


LOCAL void SetDefaultPage(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = MMIMAINMENU_GetItemList();
    MMI_LINK_NODE_T *item_node_ptr = PNULL;

    item_node_ptr = item_list_ptr->list_head_ptr;

    do
    {
        MMIMAINMENU_ITEM_INFO_T *cur_item = PNULL;

        if (PNULL == item_node_ptr)
        {
            break;
        }

        cur_item = (MMIMAINMENU_ITEM_INFO_T *)item_node_ptr->data;
        item_node_ptr = item_node_ptr->next_ptr;

		if (PNULL != cur_item)
		{        
			if (ID_MAINMENU_STYLE_SET == cur_item->menu_id
			|| 0 == cur_item->menu_id
#ifdef QBTHEME_SUPPORT
			|| ID_MAINMENU_QBTHEMEAPP == cur_item->menu_id
			|| ID_MAINMENU_QBTHEMEMORE == cur_item->menu_id
#endif
#ifndef UI_SUPPORT_U_MAINMENU
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
			|| ID_GOMENU_CHANGE_STYLE == cur_item->menu_id
#endif
#endif
			)
			{
				continue;
			}
			
			AddItemToPage(menu_ctrl_ptr, cur_item);
		}


    }
    while (item_node_ptr != item_list_ptr->list_head_ptr);

    return;
}


LOCAL void SetDefaultBar(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    uint32 item_idx = 0;
    uint32 bar_size = ARR_SIZE(istyle_bar);

    menu_ctrl_ptr->bar_info.cnt = bar_size;

    for (item_idx = 0; item_idx < bar_size; item_idx++)
    {
        menu_ctrl_ptr->bar_info.item[item_idx].group_id = 0;
        menu_ctrl_ptr->bar_info.item[item_idx].menu_id = istyle_bar[item_idx];
    }

    return;
}



/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleInitDetail(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN is_page_nv_got = FALSE;
    BOOLEAN is_bar_nv_got = FALSE;
    BOOLEAN is_folder_nv_got = FALSE;

    is_page_nv_got = GetNvPageInfo(menu_ctrl_ptr);

    if (!is_page_nv_got)
    {
        //use default
        //do nothing, when verify items, will insert all items to page
        //SetDefaultPage(menu_ctrl_ptr);
        SCI_TRACE_LOW("IStyle, none of Page info get from nv...");
    }

    is_bar_nv_got = GetNvBarInfo(menu_ctrl_ptr);

    if (!is_bar_nv_got)
    {
        //use default
        SetDefaultBar(menu_ctrl_ptr);
    }

    is_folder_nv_got = GetNvFolderInfo(menu_ctrl_ptr);

    if (!is_folder_nv_got)
    {
        //do nothing...
        SCI_TRACE_LOW("IStyle, none of folder info get from nv...");
    }

    //check - verify the items, for the new items, delete items, or repeat items
    VerifyItems(menu_ctrl_ptr);
    VerifyPages(menu_ctrl_ptr);
    VerifyFolders(menu_ctrl_ptr);

    IStyleResetBarIndexRect(&menu_ctrl_ptr->default_data, menu_ctrl_ptr->page_info.cnt);

    // 创建item的显示层信息
    IStyleCreatePageLayer(menu_ctrl_ptr);
    // 创建bar显示层
    IStyleCreateTitleBarLayer(menu_ctrl_ptr);
    // 创建bar item 显示层
    IStyleCreateBarItemLayer(menu_ctrl_ptr);
    // 设置自动滑动的区域
    IStyleResetAutoSwitchPageRect(menu_ctrl_ptr);

    menu_ctrl_ptr->theme_data.is_used_org = TRUE;

#ifdef UI_SUPPORT_U_MAINMENU
    MMIISTYLE_U_InitUMainmenuEnv(menu_ctrl_ptr->ctrl_handle);
#endif

    MMITHEME_InitMenuFolderCache();

    return;
}


LOCAL uint32 FindItemFlagIndex(struct menu_item_flag_t *items_flag_ptr, uint32 item_cnt, uint32 menu_id)
{
    uint32 index = FIND_NONE;
    uint32 i = 0;

    for (i = 0; i < item_cnt; i++)
    {
        if (items_flag_ptr[i].menu_id == menu_id)
        {
            index = i;
            break;
        }
    }

    return index;
}



//
//page
//
LOCAL BOOLEAN DeleteItemFromPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 page_index,
    uint32 item_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (page_index >= menu_ctrl_ptr->page_info.cnt)
    {
        return FALSE;
    }

    //if cnt is 0, also return false
    if (item_index >= menu_ctrl_ptr->page_info.page[page_index].cnt)
    {
        return FALSE;
    }

    move_size = menu_ctrl_ptr->page_info.page[page_index].cnt - item_index - 1;
    menu_ctrl_ptr->page_info.page[page_index].cnt -= 1;

    if (move_size > 0)
    {
        move_size *= sizeof(MMIMAINMENU_ITEM_INFO_T);

        memmove(&menu_ctrl_ptr->page_info.page[page_index].item[item_index],
                &menu_ctrl_ptr->page_info.page[page_index].item[item_index + 1],
                move_size);
    }

    return TRUE;
}


LOCAL BOOLEAN DeletePage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 page_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (page_index >= menu_ctrl_ptr->page_info.cnt)
    {
        return FALSE;
    }

    move_size = menu_ctrl_ptr->page_info.cnt - page_index - 1;
    menu_ctrl_ptr->page_info.cnt -= 1;

    if (move_size > 0)
    {
        move_size *= sizeof(ISTYLE_PAGE_INFO_T);
        memmove(&menu_ctrl_ptr->page_info.page[page_index],
                &menu_ctrl_ptr->page_info.page[page_index + 1],
                move_size);
    }

    return TRUE;
}






//insert to the front of item_index
LOCAL BOOLEAN InsertItemToPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 page_index,
    uint32 item_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    if (page_index >= menu_ctrl_ptr->page_info.cnt
        || menu_ctrl_ptr->page_info.page[page_index].cnt >= ISTYLE_PAGE_ITEM_CNT_MAX)
    {
        return FALSE;
    }

    if (item_index > menu_ctrl_ptr->page_info.page[page_index].cnt)
    {
        item_index = menu_ctrl_ptr->page_info.page[page_index].cnt;
    }

    //move
    move_size = menu_ctrl_ptr->page_info.page[page_index].cnt - item_index;

    if (move_size > 0)
    {
        move_size *= sizeof(MMIMAINMENU_ITEM_INFO_T);
        memmove(&menu_ctrl_ptr->page_info.page[page_index].item[item_index + 1],
                &menu_ctrl_ptr->page_info.page[page_index].item[item_index],
                move_size);
    }

    //add
    menu_ctrl_ptr->page_info.page[page_index].item[item_index] = *item_ptr;
    menu_ctrl_ptr->page_info.page[page_index].cnt += 1;

    return TRUE;
}


//append to tail
LOCAL BOOLEAN AppendItemToPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 page_index
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    return InsertItemToPage(menu_ctrl_ptr, item_ptr, page_index, menu_ctrl_ptr->page_info.page[page_index].cnt);
}



//add to page
//append to tail of the last page or find a space from the first page
//if no space, return false
LOCAL BOOLEAN AddItemToPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr
)
{
    uint32 page_to_append = FIND_NONE;

    if (PNULL == menu_ctrl_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->page_info.cnt == 0)
    {
        menu_ctrl_ptr->page_info.cnt = 1;
        page_to_append = 0;
    }
    else if (menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->page_info.cnt - 1].cnt < ISTYLE_PAGE_ITEM_CNT_MAX)
    {
        page_to_append = menu_ctrl_ptr->page_info.cnt - 1;
    }
    else if (menu_ctrl_ptr->page_info.cnt < ISTYLE_PAGE_CNT_MAX)
    {
        page_to_append = menu_ctrl_ptr->page_info.cnt;
        menu_ctrl_ptr->page_info.cnt += 1;
    }
    else
    {
        uint32 page_index = 0;

        for (page_index = 0; page_index < menu_ctrl_ptr->page_info.cnt - 1; page_index++)
        {
            if (menu_ctrl_ptr->page_info.page[page_index].cnt < ISTYLE_PAGE_ITEM_CNT_MAX)
            {
                page_to_append = page_index;
                break;
            }
        }
    }

    if (FIND_NONE == page_to_append)
    {
        return FALSE;
    }


    return AppendItemToPage(menu_ctrl_ptr, item_ptr, page_to_append);
}


//
//bar
//
LOCAL BOOLEAN DeleteItemFromBar(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 item_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    //if cnt is 0, also return false
    if (item_index >= menu_ctrl_ptr->bar_info.cnt)
    {
        return FALSE;
    }

    move_size = (menu_ctrl_ptr->bar_info.cnt - item_index - 1);
    menu_ctrl_ptr->bar_info.cnt -= 1;

    if (move_size > 0)
    {
        move_size *= sizeof(MMIMAINMENU_ITEM_INFO_T);
        memmove(&menu_ctrl_ptr->bar_info.item[item_index],
                &menu_ctrl_ptr->bar_info.item[item_index + 1],
                move_size);
    }

    return TRUE;
}


//insert to the front of item_index
LOCAL BOOLEAN InsertItemToBar(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 item_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->bar_info.cnt >= ISTYLE_BAR_ITEM_CNT_MAX
        || item_index > menu_ctrl_ptr->bar_info.cnt)
    {
        return FALSE;
    }

    //move
    move_size = menu_ctrl_ptr->bar_info.cnt - item_index;

    if (move_size > 0)
    {
        move_size *= sizeof(MMIMAINMENU_ITEM_INFO_T);
        memmove(&menu_ctrl_ptr->bar_info.item[item_index + 1],
                &menu_ctrl_ptr->bar_info.item[item_index],
                move_size);
    }

    //add
    menu_ctrl_ptr->bar_info.item[item_index] = *item_ptr;
    menu_ctrl_ptr->bar_info.cnt += 1;

    return TRUE;
}


//
//folder
//
LOCAL BOOLEAN DeleteItemFromFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 folder_index,
    uint32 item_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (folder_index >= menu_ctrl_ptr->folder_info.cnt)
    {
        return FALSE;
    }

    //if cnt is 0, also return false
    if (item_index >= menu_ctrl_ptr->folder_info.folder[folder_index].cnt)
    {
        return FALSE;
    }

    move_size = menu_ctrl_ptr->folder_info.folder[folder_index].cnt - item_index - 1;
    menu_ctrl_ptr->folder_info.folder[folder_index].cnt -= 1;

    if (move_size > 0)
    {
        move_size *= sizeof(MMIMAINMENU_ITEM_INFO_T);
        memmove(&menu_ctrl_ptr->folder_info.folder[folder_index].item[item_index],
                &menu_ctrl_ptr->folder_info.folder[folder_index].item[item_index + 1],
                move_size);
    }

    return TRUE;
}


LOCAL BOOLEAN DeleteFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 folder_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (folder_index >= menu_ctrl_ptr->folder_info.cnt)
    {
        return FALSE;
    }

    move_size = menu_ctrl_ptr->folder_info.cnt - folder_index - 1;
    menu_ctrl_ptr->folder_info.cnt -= 1;
#if 0
    if (move_size > 0)
    {
        move_size *= sizeof(ISTYLE_FOLDER_INFO_T);
        memmove(&menu_ctrl_ptr->folder_info.folder[folder_index],
                &menu_ctrl_ptr->folder_info.folder[folder_index - 1],
                move_size);
    }
#else
    if (move_size > 0)
    {
        move_size *= sizeof(ISTYLE_FOLDER_INFO_T);
		{
			memmove(&menu_ctrl_ptr->folder_info.folder[folder_index],
				&menu_ctrl_ptr->folder_info.folder[folder_index + 1],
				move_size);
		}
    }
#endif
    return TRUE;
}




//insert to the front of item_index
LOCAL BOOLEAN InsertItemToFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 folder_index,
    uint32 item_index
)
{
    int32 move_size = 0;

    if (PNULL == menu_ctrl_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    if (folder_index >= menu_ctrl_ptr->folder_info.cnt
        || menu_ctrl_ptr->folder_info.folder[folder_index].cnt >= ISTYLE_FOLDER_ITEM_CNT_MAX
        || item_index > menu_ctrl_ptr->folder_info.folder[folder_index].cnt)
    {
        return FALSE;
    }

    //move
    move_size = menu_ctrl_ptr->folder_info.folder[folder_index].cnt - item_index;

    if (move_size > 0)
    {
        move_size *= sizeof(MMIMAINMENU_ITEM_INFO_T);
        memmove(&menu_ctrl_ptr->folder_info.folder[folder_index].item[item_index + 1],
                &menu_ctrl_ptr->folder_info.folder[folder_index].item[item_index],
                move_size);
    }

    //add
    menu_ctrl_ptr->folder_info.folder[folder_index].item[item_index] = *item_ptr;
    menu_ctrl_ptr->folder_info.folder[folder_index].cnt += 1;

    return TRUE;
}


//append to tail
LOCAL BOOLEAN AppendItemToFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 folder_index
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    return InsertItemToFolder(menu_ctrl_ptr, item_ptr, folder_index, menu_ctrl_ptr->folder_info.folder[folder_index].cnt);
}


//find folder in folders info
LOCAL uint32 FindFolderIndex(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr, uint32 menu_id)
{
    uint32 folder_idx = 0;
    BOOLEAN is_found = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FIND_NONE;
    }

    for (folder_idx = 0; folder_idx < menu_ctrl_ptr->folder_info.cnt; folder_idx++)
    {
        if (menu_id == menu_ctrl_ptr->folder_info.folder[folder_idx].menu_id)
        {
            is_found = TRUE;
            break;
        }
    }

    return is_found ? folder_idx : (FIND_NONE);
}


//find item in folder
LOCAL BOOLEAN FindItemFromFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 menu_id,
    uint32 *folder_idx_ptr,
    uint32 *item_idx_ptr
)
{
    uint32 folder_idx = 0;
    uint32 item_idx = 0;
    BOOLEAN is_item_find = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    for (folder_idx = 0; folder_idx < menu_ctrl_ptr->folder_info.cnt; folder_idx++)
    {
        for (item_idx = 0; item_idx < menu_ctrl_ptr->folder_info.folder[folder_idx].cnt; item_idx++)
        {
            if (menu_id == menu_ctrl_ptr->folder_info.folder[folder_idx].item[item_idx].menu_id)
            {
                if (PNULL != folder_idx_ptr) *folder_idx_ptr = folder_idx;
                if (PNULL != item_idx_ptr) *item_idx_ptr = item_idx;
                break;
            }
        }
    }

    return is_item_find;
}


//find item in page
LOCAL BOOLEAN FindItemFromPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 menu_id,
    uint32 *page_idx_ptr,
    uint32 *item_idx_ptr
)
{
    uint32 page_idx = 0;
    uint32 item_idx = 0;
    BOOLEAN is_item_find = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    for (page_idx = 0; page_idx < menu_ctrl_ptr->page_info.cnt; page_idx++)
    {
        for (item_idx = 0; item_idx < menu_ctrl_ptr->page_info.page[page_idx].cnt; item_idx++)
        {
            if (menu_id == menu_ctrl_ptr->page_info.page[page_idx].item[item_idx].menu_id)
            {
                if (PNULL != page_idx_ptr) *page_idx_ptr = page_idx;
                if (PNULL != item_idx_ptr) *item_idx_ptr = item_idx;
                is_item_find = TRUE;
                break;
            }
        }
    }

    return is_item_find;
}

//check - verify the items, for the new items, delete items, or repeat items
LOCAL void VerifyItems(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = MMIMAINMENU_GetItemList();
    struct menu_item_flag_t *items_flag_ptr = PNULL;
    MMI_LINK_NODE_T *item_node_ptr = PNULL;
    uint32 item_cnt = 0;
    int32 flag_idx = 0;
    uint32 page_idx = 0;
    uint32 folder_idx = 0;
    uint32 item_idx = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (PNULL == item_list_ptr || 0 == item_list_ptr->item_count || PNULL == item_list_ptr->list_head_ptr)
    {
        return;
    }

    //0 create the item flags
    item_cnt = item_list_ptr->item_count + menu_ctrl_ptr->folder_info.cnt;
    items_flag_ptr = SCI_ALLOC_APP(sizeof(struct menu_item_flag_t) * item_cnt);

    item_node_ptr = item_list_ptr->list_head_ptr;

    item_idx = 0;

    //menu item
    do
    {
        MMIMAINMENU_ITEM_INFO_T *cur_item = PNULL;

        if (PNULL == item_node_ptr)
        {
            break;
        }

        cur_item = (MMIMAINMENU_ITEM_INFO_T *)item_node_ptr->data;
        item_node_ptr = item_node_ptr->next_ptr;
		
		if (PNULL != cur_item)
		{        
			if (0 == cur_item->menu_id
		|| ID_MAINMENU_STYLE_SET == cur_item->menu_id
#ifdef QBTHEME_SUPPORT
			|| ID_MAINMENU_QBTHEMEAPP == cur_item->menu_id
			|| ID_MAINMENU_QBTHEMEMORE == cur_item->menu_id
#endif
#ifndef UI_SUPPORT_U_MAINMENU
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
			|| ID_GOMENU_CHANGE_STYLE == cur_item->menu_id
#endif
#endif
			)
			{
				continue;
			}
			
			items_flag_ptr[item_idx].group_id = cur_item->group_id;
			items_flag_ptr[item_idx].menu_id = cur_item->menu_id;
			items_flag_ptr[item_idx].flag = 0;
			
			item_idx++;
		}

    }
    while (item_node_ptr != item_list_ptr->list_head_ptr);

	//去掉后面的闲置的，因为中间去掉一些
    item_cnt = item_idx + menu_ctrl_ptr->folder_info.cnt;

    //folder
    for (folder_idx = 0; folder_idx < menu_ctrl_ptr->folder_info.cnt; folder_idx++)
    {
        items_flag_ptr[item_idx].group_id = MAINMENU_FILEFOLD_DEFAULT_GROUP_ID;
        items_flag_ptr[item_idx].menu_id = menu_ctrl_ptr->folder_info.folder[folder_idx].menu_id;
        items_flag_ptr[item_idx].flag = 0;

        item_idx++;
    }


    //1 page - menu_ctrl_ptr->page_info
    for (page_idx = 0; page_idx < menu_ctrl_ptr->page_info.cnt; page_idx++)
    {
        for (item_idx = 0; item_idx < menu_ctrl_ptr->page_info.page[page_idx].cnt; item_idx++)
        {
            flag_idx = FindItemFlagIndex(items_flag_ptr, item_cnt, menu_ctrl_ptr->page_info.page[page_idx].item[item_idx].menu_id);

            if (FIND_NONE == flag_idx)
            {
                //not exist in system
                //delete item from page
                DeleteItemFromPage(menu_ctrl_ptr, page_idx, item_idx);
                item_idx--;
            }
            else
            {
                if (items_flag_ptr[flag_idx].flag & ITEM_FROM_PAGE)
                {
                    //repeat in page, keep the first
                    //delete item from page
                    DeleteItemFromPage(menu_ctrl_ptr, page_idx, item_idx);
                    item_idx--;
                }
                else
                {
                    items_flag_ptr[flag_idx].flag |= ITEM_FROM_PAGE;
                    items_flag_ptr[flag_idx].pos.page.page = page_idx;
                    items_flag_ptr[flag_idx].pos.page.item = item_idx;
                }
            }
        }
    }

    //2 bar - menu_ctrl_ptr->bar_info
    for (item_idx = 0; item_idx < menu_ctrl_ptr->bar_info.cnt; item_idx++)
    {
        flag_idx = FindItemFlagIndex(items_flag_ptr, item_cnt, menu_ctrl_ptr->bar_info.item[item_idx].menu_id);

        if (FIND_NONE == flag_idx)
        {
            //not exist in system
            //delete item from page
            DeleteItemFromBar(menu_ctrl_ptr, item_idx);
            item_idx--;
        }
        else
        {
            if (items_flag_ptr[flag_idx].flag & ITEM_FROM_BAR)
            {
                //repeat in page, keep the first
                //delete item from page
                DeleteItemFromBar(menu_ctrl_ptr, item_idx);
                item_idx--;
            }
            else
            {
                items_flag_ptr[flag_idx].flag |= ITEM_FROM_BAR;
                items_flag_ptr[flag_idx].pos.bar.item = item_idx;
            }
        }
    }

    //3 folder - menu_ctrl_ptr->folder_info
    for (folder_idx = 0; folder_idx < menu_ctrl_ptr->folder_info.cnt; folder_idx++)
    {
        for (item_idx = 0; item_idx < menu_ctrl_ptr->folder_info.folder[folder_idx].cnt; item_idx++)
        {
            flag_idx = FindItemFlagIndex(items_flag_ptr, item_cnt, menu_ctrl_ptr->folder_info.folder[folder_idx].item[item_idx].menu_id);

            if (FIND_NONE == flag_idx)
            {
                //not exist in system
                //delete item from folder
                DeleteItemFromFolder(menu_ctrl_ptr, folder_idx, item_idx);
                item_idx--;
            }
            else
            {
                if (items_flag_ptr[flag_idx].flag & ITEM_FROM_FOLDER)
                {
                    //repeat in folder, keep the first
                    //delete item from folder
                    DeleteItemFromFolder(menu_ctrl_ptr, folder_idx, item_idx);
                    item_idx--;
                }
                else
                {
                    items_flag_ptr[flag_idx].flag |= ITEM_FROM_FOLDER;
                    items_flag_ptr[flag_idx].pos.folder.folder = folder_idx;
                    items_flag_ptr[flag_idx].pos.folder.item = item_idx;
                }
            }
        }
    }

    //4 verify
    for (item_idx = 0; item_idx < item_cnt; item_idx++)
    {
        BOOLEAN is_item_find = FALSE;
        uint32 del_folder_idx = 0;
        uint32 del_page_idx = 0;
        uint32 del_item_idx = 0;


        switch (items_flag_ptr[item_idx].flag)
        {
            case 0:
                //not in menu; add to page
            {
                MMIMAINMENU_ITEM_INFO_T item = {0};

                item.group_id = items_flag_ptr[item_idx].group_id;
                item.menu_id = items_flag_ptr[item_idx].menu_id;
                AddItemToPage(menu_ctrl_ptr, &item);
            }
            break;

            case (ITEM_FROM_BAR|ITEM_FROM_FOLDER):
                //repeat in all bar, folder
                //delete from folder
                is_item_find = FindItemFromFolder(menu_ctrl_ptr, items_flag_ptr[item_idx].menu_id, &del_folder_idx, &del_item_idx);
                //SCI_ASSERT(is_item_find);
                //SCI_ASSERT(del_folder_idx == items_flag_ptr[item_idx].pos.folder.folder);
				if (is_item_find && (del_folder_idx == items_flag_ptr[item_idx].pos.folder.folder))
				{                
					DeleteItemFromFolder(menu_ctrl_ptr, del_folder_idx, del_item_idx);
				}
                break;

            case (ITEM_FROM_BAR|ITEM_FROM_PAGE):

                //repeat in barand page
            case (ITEM_FROM_FOLDER|ITEM_FROM_PAGE):
                //repeat in folder and page
                //delete from page
                is_item_find = FindItemFromPage(menu_ctrl_ptr, items_flag_ptr[item_idx].menu_id, &del_page_idx, &del_item_idx);
                //SCI_ASSERT(is_item_find);
                //SCI_ASSERT(del_page_idx == items_flag_ptr[item_idx].pos.page.page);
				if (is_item_find && (del_page_idx == items_flag_ptr[item_idx].pos.page.page))
				{               
					DeleteItemFromPage(menu_ctrl_ptr, del_page_idx, del_item_idx);
				}
 
                break;

            case (ITEM_FROM_BAR|ITEM_FROM_FOLDER|ITEM_FROM_PAGE):
                //repeat in all bar, folder and page
                //delete from folder; delete from page
                is_item_find = FindItemFromFolder(menu_ctrl_ptr, items_flag_ptr[item_idx].menu_id, &del_folder_idx, &del_item_idx);
                //SCI_ASSERT(is_item_find);
                //SCI_ASSERT(del_folder_idx == items_flag_ptr[item_idx].pos.folder.folder);
				if (is_item_find && (del_folder_idx == items_flag_ptr[item_idx].pos.folder.folder))
				{
					DeleteItemFromFolder(menu_ctrl_ptr, del_folder_idx, del_item_idx);
				}
                is_item_find = FindItemFromPage(menu_ctrl_ptr, items_flag_ptr[item_idx].menu_id, &del_page_idx, &del_item_idx);
                //SCI_ASSERT(is_item_find);
                //SCI_ASSERT(del_page_idx == items_flag_ptr[item_idx].pos.page.page);
				if (is_item_find && (del_page_idx == items_flag_ptr[item_idx].pos.page.page))
				{
					DeleteItemFromPage(menu_ctrl_ptr, del_page_idx, del_item_idx);
				}                
                break;

            case ITEM_FROM_PAGE:
            case ITEM_FROM_FOLDER:
            case ITEM_FROM_BAR:

                //ok, do nothing
            default:
                break;
        }
    }

    SCI_FREE(items_flag_ptr);

    return;
}


//delete empty page
LOCAL void VerifyPages(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    int32 page_idx = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    for (page_idx = menu_ctrl_ptr->page_info.cnt - 1; page_idx >= 0; page_idx--)
    {
        if (menu_ctrl_ptr->page_info.page[page_idx].cnt == 0)
        {
            DeletePage(menu_ctrl_ptr, page_idx);
        }
    }

    if (menu_ctrl_ptr->cur_page_index >= menu_ctrl_ptr->page_info.cnt)
    {
        menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->page_info.cnt - 1;
    }

    return;
}


//delete empty folder; delete "1 item" folder, and move the item to page
LOCAL void VerifyFolders(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    int32 folder_idx = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    for (folder_idx = menu_ctrl_ptr->folder_info.cnt - 1; folder_idx >= 0; folder_idx--)
    {
        BOOLEAN is_folder_find = FALSE;
        uint32 page_idx = 0;
        uint32 item_idx = 0;

        if (menu_ctrl_ptr->folder_info.folder[folder_idx].cnt > 1)
        {
            continue;
        }

        is_folder_find = FindItemFromPage(menu_ctrl_ptr,
                                          menu_ctrl_ptr->folder_info.folder[folder_idx].menu_id,
                                          &page_idx,
                                          &item_idx);

        if (!is_folder_find)
        {
            DeleteFolder(menu_ctrl_ptr, folder_idx);
            continue;
        }

        if (menu_ctrl_ptr->folder_info.folder[folder_idx].cnt == 1)
        {
            MMIMAINMENU_ITEM_INFO_T item = menu_ctrl_ptr->folder_info.folder[folder_idx].item[0];

            //delete folder
            DeleteItemFromPage(menu_ctrl_ptr, page_idx, item_idx);
            DeleteFolder(menu_ctrl_ptr, folder_idx);
            //insert item to page
            InsertItemToPage(menu_ctrl_ptr, &item, page_idx, item_idx);
        }
        else //=0
        {
            //delete folder
            DeleteItemFromPage(menu_ctrl_ptr, page_idx, item_idx);
            DeleteFolder(menu_ctrl_ptr, folder_idx);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleGetInfo(
    MMI_HANDLE_T          win_handle,
    GUIMENU_ISTYLE_DATA_T *menu_info_ptr //in/out:
)
{
    GUI_RECT_T menu_rect = MMITHEME_GetClientRectEx(win_handle);
    GUI_RECT_T bar_rect  = {0};
    GUI_RECT_T item_rect[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};
    uint16     item_height = (menu_rect.bottom - menu_rect.top + 1 - MMITHEME_ISTYLEMENU_INDEX_HEIGHT) / (MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM + 1);
    uint16     margin = (menu_rect.right - menu_rect.left + 1 - MENUTEXT_WIDTH * MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM) / (MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM + 1) / 2;
    uint32     i = 0;

    if (PNULL == menu_info_ptr)
    {
        return FALSE;
    }

    menu_info_ptr->icon_mask_alpha    = MMI_MASK_ALPHA;
    menu_info_ptr->bg_mask_alpha    = MMI_MASK_ALPHA / 2;
    menu_info_ptr->zoom_out           = ISTYLE_MENU_ZOOM_OUT;
    menu_info_ptr->item_font          = SONG_FONT_14;
    menu_info_ptr->bar_image_id       = IMAGE_ISTYLEMENU_BAR_BG;
    menu_info_ptr->bar_unsel_bg_image = IMAGE_SLIDE_MAINMENU_TITLE_OFF;
    menu_info_ptr->bar_sel_bg_image   = IMAGE_SLIDE_MAINMENU_TITLE_ON;
    menu_info_ptr->folder_image_id    = IMAGE_ISTYLEMENU_FOLDER_BG;
    menu_info_ptr->folder_bg_id       = IMAGE_ISTYLEMENU_FOLDER_BG;

    menu_info_ptr->page_row_num = MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM;
    menu_info_ptr->page_col_num = MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM;
    menu_info_ptr->page_num     = MMITHEME_MENUMAIN_SLIDE_PAGE_NUM;

    menu_info_ptr->item_width = MENUICON_WIDTH;
    menu_info_ptr->item_height = MENUICON_WIDTH;

    bar_rect = menu_rect;
    bar_rect.top = bar_rect.bottom - item_height - MMITHEME_ISTYLEMENU_INDEX_HEIGHT + 1;

    menu_rect.bottom = bar_rect.top - 1;

    menu_info_ptr->bar_rect = bar_rect;

    menu_rect.left += margin;
    menu_rect.right -= margin;

    GUI_CreateMatrixRect(&menu_rect,
                         0,
                         0,
                         0,
                         0,
                         MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM ,
                         MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM,
                         item_rect
                        );

    for (i = 0; i < MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM; i++)
    {
        item_rect[i] = GUI_GetCenterRect(item_rect[i], MENUTEXT_WIDTH, MENUICON_HEIGHT + MENUTEXT_HEIGHT + MENUNUMICON_HEIGHT / 3);
    }

    SCI_MEMCPY(menu_info_ptr->item_rect, item_rect, sizeof(menu_info_ptr->item_rect));

    bar_rect.top += MMITHEME_ISTYLEMENU_INDEX_HEIGHT;
    bar_rect.left += margin;
    bar_rect.right -= margin;

    GUI_CreateMatrixRect(&bar_rect,
                         0,
                         0,
                         0,
                         0,
                         1,
                         MMITHEME_ISTYLEMENU_BAR_ITEM_MAX,
                         item_rect
                        );

    for (i = 0; i < MMITHEME_ISTYLEMENU_BAR_ITEM_MAX; i++)
    {
        item_rect[i] = GUI_GetCenterRect(item_rect[i], MENUTEXT_WIDTH, MENUICON_HEIGHT + MENUTEXT_HEIGHT + MENUNUMICON_HEIGHT / 3);
    }

    SCI_MEMCPY(menu_info_ptr->bar_item_rect, item_rect, sizeof(menu_info_ptr->bar_item_rect));


    return TRUE;
}

/*****************************************************************************/
// Description : get icon text rect
// Global resource dependence :
//  Author:James.Zhang
// Note:
/*****************************************************************************/
LOCAL void GetIconTextRect(
    GUI_RECT_T  item_rect,
    GUI_RECT_T *icon_rect,
    GUI_RECT_T *text_rect
)
{
    if (PNULL != icon_rect)
    {
        *icon_rect = item_rect;
        icon_rect->top = item_rect.top + MENUNUMICON_HEIGHT / 3;
        icon_rect->bottom = icon_rect->top + MENUICON_HEIGHT - 1;
        *icon_rect = GUI_GetCenterRect(*icon_rect, MENUICON_WIDTH , MENUICON_HEIGHT);
    }

    if (PNULL != text_rect)
    {
        *text_rect = item_rect;
        text_rect->top = item_rect.top + MENUICON_HEIGHT + MENUNUMICON_HEIGHT / 3;
    }
}


/*****************************************************************************/
//  Description : draw page layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleDrawPageLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,  //in
    uint16           page_index,
    UILAYER_HANDLE_T layer_handle,
    int16            offset_x,
    BOOLEAN          is_need_bar
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    lcd_dev_info.block_id = layer_handle;

    // 画层
    if (0 != layer_handle && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        IStyleBltSlideItemLayerToMenuLayer(menu_ctrl_ptr, page_index, lcd_dev_info, offset_x, is_need_bar);
    }
    else
    {
        IStyleDrawAllItems(menu_ctrl_ptr, page_index, lcd_dev_info, 0, TRUE, TRUE);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_2095_112_2_18_2_33_48_135, (uint8 *)"d", layer_handle);
    }

    return;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T IStyleGetRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    GUI_RECT_T          client_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    //get selected bar display rect
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    client_rect = MMITHEME_GetClientRectEx(menu_ctrl_ptr->win_handle);

    client_rect.bottom = menu_ctrl_ptr->default_data.bar_rect.top - 1;

    return client_rect;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleCreatePageLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          page_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    page_rect = IStyleGetRect(menu_ctrl_ptr);

    menu_ctrl_ptr->page_layer_handle[0].lcd_id = 0;
    menu_ctrl_ptr->page_layer_handle[0].block_id = UILAYER_NULL_HANDLE;

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = page_rect.left;
    create_info.offset_y = page_rect.top;
    create_info.width = (page_rect.right + 1 - page_rect.left);
    create_info.height = (page_rect.bottom + 1 - page_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_FPS_ACCELERATE;

    UILAYER_CreateLayer(
        &create_info,
        &menu_ctrl_ptr->page_layer_handle[0]
    );

    // 设置color key
    UILAYER_SetLayerColorKey(&menu_ctrl_ptr->page_layer_handle[0], TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleReleasePageLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->page_layer_handle[0]);   /*lint !e506 !e774*/
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->page_layer_handle[1]);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : draw page layers
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleDrawPageLayers(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    int16          offset_x
)
{
    uint16              layer_width     = 0;
    uint16              layer_height    = 0;
    BOOLEAN             is_need_bar     = menu_ctrl_ptr->is_move_state;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    //get selected bar display rect
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    if (is_need_bar)
    {
        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);
    }
    else
    {
        GUI_RECT_T page_rect = {0};

        page_rect.right = layer_width - 1;
        page_rect.bottom = menu_ctrl_ptr->default_data.bar_rect.top - 1;

        UILAYER_ClearRect(&menu_ctrl_ptr->page_layer_handle[0], page_rect);
    }


    //向右移动
    if (offset_x > 0)
    {
        if (0 == menu_ctrl_ptr->cur_page_index)
        {
            IStyleBltSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, offset_x - layer_width);

            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x, is_need_bar);
        }
        else
        {
            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x, is_need_bar);

            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index - 1, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x - layer_width, FALSE);
        }
    }
    //向左移动
    else if (offset_x < 0)
    {
        if (menu_ctrl_ptr->is_search_display)
        {
            if (!menu_ctrl_ptr->is_search_entry)
            {
                //先向右移动出现搜索页，再向左，越过slide_start_point
                ReleaseSearchPage(menu_ctrl_ptr, FALSE);

                IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x, is_need_bar);

                IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index + 1, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x + layer_width, FALSE);
            }
            else
            {
                //move from search page
                IStyleBltSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, offset_x);

                IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x + layer_width, is_need_bar);
            }

            return ;
        }

        if (menu_ctrl_ptr->page_info.cnt - 1 == menu_ctrl_ptr->cur_page_index)
        {
            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x, is_need_bar);
        }
        else
        {
            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x, is_need_bar);

            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index + 1, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x + layer_width, FALSE);
        }
    }
    else
    {
        if (!(menu_ctrl_ptr->is_search_entry && menu_ctrl_ptr->is_search_display))
        {
            IStyleDrawPageLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0].block_id, offset_x, is_need_bar);
        }
    }
}

/*****************************************************************************/
//  Description : draw page layers
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MoveBarLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    int16          offset_x
)
{
    uint16              layer_width     = 0;
    uint16              layer_height    = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    //get selected bar display rect
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    //向右移动
    if (offset_x > 0)
    {
        if (0 == menu_ctrl_ptr->cur_page_index)
        {
            IStyleBltSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, offset_x - layer_width);
        }
    }
    //向左移动
    else if (offset_x < 0)
    {
        if (menu_ctrl_ptr->is_search_display)
        {
            if (!menu_ctrl_ptr->is_search_entry)
            {
                //先向右移动出现搜索页，再向左，越过slide_start_point
                ReleaseSearchPage(menu_ctrl_ptr, FALSE);
            }
            else
            {
                //move from search page
                IStyleBltSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, offset_x);
            }

            return ;
        }
    }
}

/*****************************************************************************/
//  Description : redraw page layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MovePageLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,  //in
    GUI_POINT_T   *point_ptr
)
{
    if ((PNULL == menu_ctrl_ptr) || (PNULL == point_ptr))
    {
        return;
    }

#ifdef UI_SUPPORT_U_MAINMENU

    if (!(menu_ctrl_ptr->is_cur_pagesearch && menu_ctrl_ptr->slide_start_point.x - point_ptr->x  <= MAINMENU_SEARCH_HORIZON_MOVE_SPACING))
    {
        MMIISTYLE_U_Display(menu_ctrl_ptr, point_ptr);
    }

    if (HandleHorizontalMoveInSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, point_ptr->x - menu_ctrl_ptr->slide_start_point.x))
    {
        MoveBarLayer(menu_ctrl_ptr, point_ptr->x - menu_ctrl_ptr->slide_start_point.x);
    }

#else

    if (HandleHorizontalMoveInSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, point_ptr->x - menu_ctrl_ptr->slide_start_point.x))
    {
        IStyleDrawPageLayers(menu_ctrl_ptr, point_ptr->x - menu_ctrl_ptr->slide_start_point.x);
    }

#endif

}

/*****************************************************************************/
//  Description : get cur item rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFolderItemRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,  //in
    uint16           index,           //in
    GUI_RECT_T       *rect_ptr        //out
)
{
    BOOLEAN result = FALSE;

    if (menu_ctrl_ptr->is_folder_open)
    {
        if ((index < GUIMENU_FILEFOLD_MAX_ITEM_NUM) && ((index + MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM) < MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM))
        {
            *rect_ptr = menu_ctrl_ptr->default_data.item_rect[index + MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM];
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get cur item rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCurItemRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,  //in
    GUI_RECT_T       *rect_ptr        //out
)
{
    BOOLEAN result = FALSE;

    if (PNULL == rect_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        result = GetFolderItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, rect_ptr);
    }
    else
    {
        if (menu_ctrl_ptr->is_bar_item)
        {
            uint32 item_num = menu_ctrl_ptr->bar_info.cnt;

            if (menu_ctrl_ptr->cur_item_index < item_num)
            {
                GUI_RECT_T bar_item_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};

                GetBarItemRect(menu_ctrl_ptr, bar_item_rect, item_num);

                *rect_ptr = bar_item_rect[menu_ctrl_ptr->cur_item_index];

                result = TRUE;
            }
        }
        else
        {
            if (menu_ctrl_ptr->cur_item_index < menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].cnt)
            {
                *rect_ptr = menu_ctrl_ptr->default_data.item_rect[menu_ctrl_ptr->cur_item_index];
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set move layer position
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetMoveIconLayerPosition(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (menu_ctrl_ptr->is_moving)
    {
        GUI_RECT_T tmp_rect = GetIStyleMoveLayerRect(menu_ctrl_ptr);

        IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->icon_move_layer_handle);

        UILAYER_SetLayerPosition(&menu_ctrl_ptr->icon_move_layer_handle, tmp_rect.left, tmp_rect.top);
    }

    return;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveIconLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T cur_rect = {0};

    if (!GetCurItemRect(menu_ctrl_ptr, &cur_rect))
    {
        return;
    }

    if (0 == menu_ctrl_ptr->icon_move_layer_handle.block_id
        || UILAYER_NULL_HANDLE == menu_ctrl_ptr->icon_move_layer_handle.block_id)
    {
        UILAYER_INFO_T      layer_in    = {0};
        UILAYER_INFO_T      layer_out   = {0};
        SCALE_IMAGE_IN_T    scale_in    = {0};
        SCALE_IMAGE_OUT_T   scale_out   = {0};
        UILAYER_CREATE_T    create_info = {0};
        int16               width       = (cur_rect.right + 1 - cur_rect.left) * menu_ctrl_ptr->default_data.zoom_out / 100;
        int16               height      = (cur_rect.bottom + 1 - cur_rect.top) * menu_ctrl_ptr->default_data.zoom_out / 100;
        GUI_LCD_DEV_INFO    layer_handle = {0};
        GUIMENU_ITEM_POSITION_E position = GUIMENU_ITEM_IN_PAGE;
		uint16 page_index = menu_ctrl_ptr->cur_page_index;


        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.width = (int16)width;
        create_info.height = (int16)height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_FPS_ACCELERATE;

        UILAYER_CreateLayer(
            &create_info,
            &menu_ctrl_ptr->icon_move_layer_handle
        );

        SetMoveIconLayerPosition(menu_ctrl_ptr);

        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->icon_move_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&menu_ctrl_ptr->icon_move_layer_handle);

        if (menu_ctrl_ptr->is_folder_open)
        {
            position = GUIMENU_ITEM_IN_FOLDER;
			page_index = menu_ctrl_ptr->open_folder_index;
        }
        else if (menu_ctrl_ptr->is_bar_item)
        {
            position = GUIMENU_ITEM_IN_BAR;
        }

        layer_handle = IStyleDrawItem(menu_ctrl_ptr, PNULL, PNULL, page_index, menu_ctrl_ptr->cur_item_index, position);

        UILAYER_GetLayerInfo(&layer_handle, &layer_in);
        UILAYER_GetLayerInfo(&menu_ctrl_ptr->icon_move_layer_handle, &layer_out);

        // src data info
        scale_in.src_format           = IMGREF_FORMAT_ARGB888;
        scale_in.src_chn.chn0.ptr     = UILAYER_GetLayerBufferPtr(&layer_handle);

        if (PNULL == scale_in.src_chn.chn0.ptr)
        {
            return;
        }

        scale_in.src_chn.chn0.size    = layer_in.layer_height * layer_in.layer_width * sizeof(uint32);
        scale_in.src_size.w           = layer_in.layer_width;
        scale_in.src_size.h           = layer_in.layer_height;
        scale_in.src_trim_rect.w      = layer_in.layer_width;
        scale_in.src_trim_rect.h      = layer_in.layer_height;        // dest data info

        scale_in.target_format        = IMGREF_FORMAT_ARGB888;
        scale_in.target_buf.size      = layer_out.layer_height * layer_out.layer_width * sizeof(uint32) + 255;
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);
        scale_in.target_size.w        = layer_out.layer_width;
        scale_in.target_size.h        = layer_out.layer_height;


        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in, &scale_out))
        {
            uint32  src_height     = MIN(layer_out.layer_height, scale_out.output_size.h);
            uint32  src_width      = MIN(layer_out.layer_width, scale_out.output_size.w);
            UILAYER_COPYBUF_PARAM_T     copy_param = {0};
            UILAYER_COPYBUF_T           copy_buf = {0};

            copy_param.rect = UILAYER_GetLayerRect(&menu_ctrl_ptr->icon_move_layer_handle);
            copy_param.rect.right = copy_param.rect.left + (int16)src_width - 1;
            copy_param.rect.bottom = copy_param.rect.top + (int16)src_height - 1;

            copy_buf.buf_ptr = (uint8 *)scale_out.output_chn.chn0.ptr;
            copy_buf.data_type = DATA_TYPE_ARGB888;
            copy_buf.width = scale_out.output_size.w;
            copy_buf.height = scale_out.output_size.h;

            UILAYER_CopyBufferToLayer(&menu_ctrl_ptr->icon_move_layer_handle, &copy_param, &copy_buf);

            UILAYER_WeakLayerAlpha(&menu_ctrl_ptr->icon_move_layer_handle,
                                   menu_ctrl_ptr->default_data.icon_mask_alpha);
        }

        SCI_FREE(scale_in.target_buf.ptr);
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void IStyleReleaseMoveIconLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->icon_move_layer_handle);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : Clear Icon Rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ClearCurItemRect(
    MMIMAINMENU_ISTYLE_DATA_T  *menu_ctrl_ptr
)
{
    GUI_RECT_T       cur_rect     = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (!GetCurItemRect(menu_ctrl_ptr, &cur_rect))
    {
        return;
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        lcd_dev_info = menu_ctrl_ptr->page_layer_handle[1];
    }
    else
    {
        lcd_dev_info = GetCurrentPageLyaerInfo(menu_ctrl_ptr);
#ifdef UI_SUPPORT_U_MAINMENU

        if (UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0])
            && DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType()
            && DATA_TYPE_PMARGB888 == UILAYER_GetLayerColorType(&menu_ctrl_ptr->page_layer_handle[0]))
        {
            //ARGB888 TO PAF888
            UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);

            UILAYER_Clear(&lcd_dev_info);
            IStyleDrawAllItems(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, lcd_dev_info, 0, TRUE, FALSE);
        }

#endif
    }

    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        MMITHEME_ClearRect(&lcd_dev_info, &cur_rect);
    }

    return;
}

/*****************************************************************************/
//  Description : init move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void IStyleInitMoveLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr  //in
)
{
    menu_ctrl_ptr->is_moving = TRUE;

    ClearCurItemRect(menu_ctrl_ptr);

    // 新增页
    if (IStyleAddNewPage(menu_ctrl_ptr) && !(menu_ctrl_ptr->is_folder_open))
    {
        IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        CreateMoveIconLayer(menu_ctrl_ptr);
    }

    // 记录移动的信息
    SetIStyleMoveItemInfo(menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void IStyleAppendLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO lcd_dev_info
)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    return;
}

/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleMoveInit(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL == menu_ctrl_ptr || menu_ctrl_ptr->is_move_state) // 记录当前和起始位置
    {
        return FALSE;
    }

    menu_ctrl_ptr->is_move_state = TRUE;

    if (UILAYER_IsMultiLayerEnable())
    {
        if (menu_ctrl_ptr->is_item_pressed)
        {
            IStyleInitMoveLayer(menu_ctrl_ptr);
        }
    }
    else
    {
        menu_ctrl_ptr->is_move_state = FALSE;
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        IStyleDestroyFilefoldNameBox(menu_ctrl_ptr);

        CreateFilefoldEditNameBox(menu_ctrl_ptr);
    }

    IStyleStartShakeIconTimer(menu_ctrl_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : 确认移动
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleConfirmItem(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
)
{
    BOOLEAN result = FALSE;

#ifdef DYNAMIC_MAINMENU_SUPPORT
    BOOLEAN is_move_state = FALSE;
#endif

#ifdef DYNAMIC_MAINMENU_SUPPORT
    is_move_state = menu_ctrl_ptr->is_move_state;
#endif

    if (menu_ctrl_ptr->is_move_state)
    {
        if (menu_ctrl_ptr->is_folder_open)
        {
            IStyleGetFilefoldNameFromEditbox(menu_ctrl_ptr);
            IStyleDestroyFilefoldNameBox(menu_ctrl_ptr);

            UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[1]);

            CreateFilefoldTextNameBox(menu_ctrl_ptr);
        }

        //check - verify the items, for the new items, delete items, or repeat items
		//VerifyFolders(menu_ctrl_ptr);
        VerifyItems(menu_ctrl_ptr);
        VerifyPages(menu_ctrl_ptr);
        VerifyFolders(menu_ctrl_ptr);

        SaveNv(menu_ctrl_ptr);

        // 释放层
        IStyleReleaseMoveIconLayer(menu_ctrl_ptr);

        // 设置状态
        menu_ctrl_ptr->is_move_state = FALSE;
        menu_ctrl_ptr->is_moving = FALSE;
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMIAPISETVWP_Resume();
#endif
#ifdef DYNAMIC_MAINMENU_SUPPORT

        //if (MMIMENU_DyGetItemCount() > 0)
        {
            //由于图层上面画了"x"要重新画一次
            IStyleReload(menu_ctrl_ptr);
            is_move_state = TRUE;
        }

#endif

        IStyleDraw(menu_ctrl_ptr);

        result = TRUE;

        if (MSG_APP_RED != msg_id)
        {
            return result;
        }
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        result = TRUE;

#ifdef DYNAMIC_MAINMENU_SUPPORT

        if (MMIMENU_DyGetItemCount() > 0 && is_move_state)
        {
            //由于图层上面画了"x"要重新画一次
            IStyleReload(menu_ctrl_ptr);
        }

#endif
        IStyleCloseFilefolder(menu_ctrl_ptr);

        return result;
    }
	
    if (!result)
    {
        //notify parent control or parent window
        IStyleNotifyParentMsg(menu_ctrl_ptr->ctrl_handle, msg_id);
    }
    return result;
}

/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleResetBarIndexRect(
    GUIMENU_ISTYLE_DATA_T *menu_info_ptr, //in/out:
    uint16 item_num
)
{
    uint16     margin     = 0;
    GUI_RECT_T total_rect = {0};

	if (PNULL == menu_info_ptr)
    {
    	return FALSE;
	}

	margin     = (menu_info_ptr->bar_rect.right - menu_info_ptr->bar_rect.left + 1 - MMITHEME_ISTYLEMENU_INDEX_HEIGHT * (item_num + 1)) / 2;
	total_rect = menu_info_ptr->bar_rect;
	
	total_rect.left += margin;
    total_rect.right -= margin;
    total_rect.bottom = total_rect.top + MMITHEME_ISTYLEMENU_INDEX_HEIGHT - 1;
   
    GUI_CreateMatrixRect(&total_rect,
                         0,
                         0,
                         0,
                         0,
                         1,
                         item_num + 1, //add search page indication
                         menu_info_ptr->bar_index_rect
                        );

    return TRUE;
}

/*****************************************************************************/
//  Description : reset
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleResetAutoSwitchPageRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    uint16 lcd_width = 0; // lcd宽度
    uint16 lcd_height = 0; // lcd高度
    uint16 move_space = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUI_RECT_T menu_rect = {0};

    CTRLMAINMENU_GetRect(menu_ctrl_ptr->ctrl_handle, &menu_rect);

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    move_space = MIN(lcd_height, lcd_width);

    s_left_direct_move_rect = menu_rect;
    s_left_direct_move_rect.left = 0;
    s_left_direct_move_rect.right = move_space / 10;
    s_left_direct_move_rect.bottom = menu_ctrl_ptr->default_data.bar_rect.top - 1;

    s_right_direct_move_rect = menu_rect;
    s_right_direct_move_rect.right = lcd_width - 1;
    s_right_direct_move_rect.left = s_right_direct_move_rect.right - move_space / 10;
    s_right_direct_move_rect.bottom = menu_ctrl_ptr->default_data.bar_rect.top - 1;

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateIStyleMenuItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_RECT_T layer_rect,
    GUIMENU_ITEM_BUF_T *item_buf_ptr
)
{
    BOOLEAN             result = FALSE;
    UILAYER_RESULT_E    ret = UILAYER_RESULT_ERROR;
    GUI_RECT_T          icon_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    //SCI_ASSERT(PNULL != item_buf_ptr);  /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return result;
    }

    if (0 != item_buf_ptr->icon_layer.block_id
        && UILAYER_NULL_HANDLE != item_buf_ptr->icon_layer.block_id)
    {
        return result;
    }

    //get selected bar display rect
    icon_rect = layer_rect;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = icon_rect.left;
    create_info.offset_y = icon_rect.top;
    create_info.width = (icon_rect.right + 1 - icon_rect.left);
    create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    //create_info.format = UILAYER_MEM_DOUBLE_COPY;

    ret = UILAYER_CreateLayer(&create_info, &item_buf_ptr->icon_layer);
    UILAYER_SetLayerColorKey(&item_buf_ptr->icon_layer, TRUE, UILAYER_TRANSPARENT_COLOR);

    item_buf_ptr->width = layer_rect.right - layer_rect.left + 1;
    item_buf_ptr->height = layer_rect.bottom - layer_rect.top + 1;

    if (UILAYER_RESULT_SUCCESS == ret)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DestoryIStyleItemLayer(
    GUIMENU_ITEM_BUF_T *item_buf_ptr
)
{
    if (PNULL != item_buf_ptr)
    {
        UILAYER_RELEASELAYER(&item_buf_ptr->icon_layer);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetIStyleMoveItemInfo(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->is_move_state) // 记录当前和起始位置
    {
        result = TRUE;
        menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index;
        menu_ctrl_ptr->tmp_cur_page_index = menu_ctrl_ptr->cur_page_index;
        menu_ctrl_ptr->tmp_is_bar_item  = menu_ctrl_ptr->is_bar_item;
    }

    return result;
}

/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetIStyleMoveItemInfo(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->is_move_state) // 记录当前和起始位置
    {
        result = TRUE;
        menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->tmp_cur_item_index;
        menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->tmp_cur_page_index;
        menu_ctrl_ptr->is_bar_item = menu_ctrl_ptr->tmp_is_bar_item;
    }

    return result;
}

/*****************************************************************************/
// Description : 销毁单，释放内存
// Global resource dependence :
//  Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DestroyIStyle(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    menu_ctrl_ptr->is_move_state = FALSE;

    if (MMIMENU_DyCheckIsSyn())//check信息有变化
    {
        IStyleReload(menu_ctrl_ptr);
    }
	
    // 保存当前页
    SaveNv(menu_ctrl_ptr);

    // 释放
    IStyleReleasePageLayer(menu_ctrl_ptr);

    // 释放
    IStyleReleaseBarBgLayer(menu_ctrl_ptr);

    // title图片显示位置
    IStyleReleaseBarItemLayer(menu_ctrl_ptr);

    //释放搜索页
    ReleaseSearchPage(menu_ctrl_ptr, TRUE);

    MMITHEME_DestroyMenuFolderCache();

#ifdef UI_SUPPORT_U_MAINMENU
    MMIISTYLE_U_UninitUMainmenuEnv();
    MMIISTYLE_U_DistoryPageLayer();
#endif

    return;
}

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleModifyRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
)
{
    //set icon menu current index
    IStyleGetInfo(menu_ctrl_ptr->win_handle, &menu_ctrl_ptr->default_data);
    IStyleResetBarIndexRect(&menu_ctrl_ptr->default_data, menu_ctrl_ptr->bar_info.cnt);

    // title图片显示位置
    IStyleReleaseBarBgLayer(menu_ctrl_ptr);
    IStyleCreateTitleBarLayer(menu_ctrl_ptr);

    // title图片显示位置
    IStyleReleaseBarItemLayer(menu_ctrl_ptr);
    IStyleCreateBarItemLayer(menu_ctrl_ptr);

    // 需要重新设置menu显示的区域和层
    IStyleReleasePageLayer(menu_ctrl_ptr);
    IStyleCreatePageLayer(menu_ctrl_ptr);

    // 自动切换的区域需要重新设置
    IStyleResetAutoSwitchPageRect(menu_ctrl_ptr);

    StopAutoMoveTimer(menu_ctrl_ptr);

    return;
}

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IStyleDraw(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);

#ifdef DYNAMIC_MAINMENU_SUPPORT

    if (menu_ctrl_ptr->is_move_state && MMIAPIUDISK_UdiskIsRun())
    {
        if (menu_ctrl_ptr->is_folder_open)
        {
            IStyleDestroyFilefoldNameBox(menu_ctrl_ptr);

            UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[1]);
        }

        // 释放层
        IStyleReleaseMoveIconLayer(menu_ctrl_ptr);

        // 设置状态
        menu_ctrl_ptr->is_move_state = FALSE;
        menu_ctrl_ptr->is_moving = FALSE;
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        StopMoveStartTimer(menu_ctrl_ptr);
        StopRedrawTimer(menu_ctrl_ptr);
    }

    if (MMIMENU_DyCheckIsSyn())//check信息有变化
    {
        IStyleReload(menu_ctrl_ptr);
    }

#endif

    IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->titleidx_layer_handle);
    IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->bar_item_layer);
    IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->page_layer_handle[0]);
    IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, TRUE);

    // 设置合并的层，这里只需要把第一个page的层合并即可
    if (UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0]))
    {
        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);
    }

    if (UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0])) // cp buffer
    {
        if (!menu_ctrl_ptr->is_search_display)
        {
            IStyleBltSlideItemLayerToMenuLayer(
                menu_ctrl_ptr,
                menu_ctrl_ptr->cur_page_index,
                menu_ctrl_ptr->page_layer_handle[0],
                0,
                TRUE);
        }
        else
        {
            BltSlideItemLayerToMenuLayerEx(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0], 0, TRUE, FALSE);
        }
    }
    else // 直接画
    {
        if (menu_ctrl_ptr->is_search_display)
        {
            IStyleDrawAllItems(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, lcd_dev_info, 0, FALSE, TRUE);
        }
        else
        {
            IStyleDrawAllItems(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, lcd_dev_info, 0, TRUE, TRUE);
        }
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        UILAYER_WeakLayerAlpha(&menu_ctrl_ptr->page_layer_handle[0],
                               menu_ctrl_ptr->default_data.bg_mask_alpha);

        DisplayFolderBg(menu_ctrl_ptr, menu_ctrl_ptr->page_layer_handle[0]);

        IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->page_layer_handle[1]);

        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[1]);
        BltItemLayerToFolderLayer(menu_ctrl_ptr, menu_ctrl_ptr->page_layer_handle[1]);
    }

    if (menu_ctrl_ptr->is_search_entry)
    {
        IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_layer_handle);

        if (UILAYER_IsLayerActive(&menu_ctrl_ptr->tmp_layer_handle))
        {
            DisplaySearchBg(menu_ctrl_ptr, menu_ctrl_ptr->tmp_layer_handle);
        }
        else
        {
            SetCtrlLcdDevInfoInSearchPage(lcd_dev_info);

            DisplaySearchBg(menu_ctrl_ptr, lcd_dev_info);

            SetCtrlLcdDevInfoInSearchPage(menu_ctrl_ptr->tmp_layer_handle);
        }
    }

#ifdef UI_SUPPORT_U_MAINMENU
    MMIISTYLE_U_SetPageInfo(menu_ctrl_ptr, FALSE);
#endif
    return;
}

#ifdef SEARCH_SUPPORT
#define ISTYLE_BAR_RECT_START 1
#else
#define ISTYLE_BAR_RECT_START 0
#endif
/*****************************************************************************/
//  Description : display bar
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void IStyleDrawBar(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    uint16             cur_page_index,
    GUI_LCD_DEV_INFO   lcd_dev_info,
    BOOLEAN            is_draw_all
)
{
    uint16          i = 0;
    uint16          image_width = 0;
    uint16          image_height = 0;
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      bar_rect = {0};
    GUI_RECT_T      disp_rect = {0};
    MMI_IMAGE_ID_T  title_img_id = IMAGE_NULL;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    if (is_draw_all)
    {
        UILAYER_Clear(&lcd_dev_info);

        bar_rect = menu_ctrl_ptr->default_data.bar_rect;

        GUIRES_GetImgWidthHeight(&image_width, &image_height,
                                 menu_ctrl_ptr->default_data.bar_image_id, menu_ctrl_ptr->win_handle);

        bar_rect.top = bar_rect.bottom - image_height + 1;

        //display title image
        GUIRES_DisplayImg(PNULL,
                          &bar_rect,
                          PNULL,
                          menu_ctrl_ptr->win_handle,
                          menu_ctrl_ptr->default_data.bar_image_id,
                          &lcd_dev_info);

        UILAYER_WeakLayerAlpha(&lcd_dev_info,
                               menu_ctrl_ptr->default_data.icon_mask_alpha);
    }
    else
    {
        bar_rect = menu_ctrl_ptr->default_data.bar_rect;
        bar_rect.bottom = bar_rect.top + MMITHEME_ISTYLEMENU_INDEX_HEIGHT - 1;

        UILAYER_ClearRect(&lcd_dev_info, bar_rect);
    }

#ifdef SEARCH_SUPPORT
    //display search indication
    disp_rect = menu_ctrl_ptr->default_data.bar_index_rect[0];

    if (! menu_ctrl_ptr->is_search_entry)
    {
        title_img_id = IMAGE_ISTYLEMENU_SEARCH_OFF;
    }
    else
    {
        title_img_id = IMAGE_ISTYLEMENU_SEARCH_ON;
    }

    GUIRES_GetImgWidthHeight(&image_width, &image_height, title_img_id, menu_ctrl_ptr->win_handle);

    disp_rect = GUI_GetCenterRectExt(disp_rect, image_width, image_height);
    image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1);

    //display title image
    GUIRES_DisplayImg(PNULL,
                      &disp_rect,
                      &image_rect,
                      menu_ctrl_ptr->win_handle,
                      title_img_id,
                      &lcd_dev_info);
#endif

    for (i = 0; i < menu_ctrl_ptr->page_info.cnt; i++)
    {
        disp_rect = menu_ctrl_ptr->default_data.bar_index_rect[i + ISTYLE_BAR_RECT_START];

        //get title image id
        if (i == cur_page_index)
        {
            title_img_id = menu_ctrl_ptr->default_data.bar_sel_bg_image;

            //handle first page in search status
            if (menu_ctrl_ptr->is_search_entry)
            {
                title_img_id = menu_ctrl_ptr->default_data.bar_unsel_bg_image;
            }
        }
        else
        {
            title_img_id = menu_ctrl_ptr->default_data.bar_unsel_bg_image;
        }

        GUIRES_GetImgWidthHeight(&image_width, &image_height, title_img_id, menu_ctrl_ptr->win_handle);

        disp_rect = GUI_GetCenterRectExt(disp_rect, image_width, image_height);
        image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1);

        //display title image
        GUIRES_DisplayImg(PNULL,
                          &disp_rect,
                          &image_rect,
                          menu_ctrl_ptr->win_handle,
                          title_img_id,
                          &lcd_dev_info);
    }

    IMG_EnableTransparentColor(FALSE);

    return;
}


LOCAL void OpenFolderCallBack(uint32 menu_ctrl_ptr, uint32 dummy)
{
    IStyleOpenFolder((MMIMAINMENU_ISTYLE_DATA_T *)menu_ctrl_ptr);

    return;
}


/*****************************************************************************/
// Description:
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
BOOLEAN IStyleGetItemData(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32 menu_id,
    uint32 group_id,
    CTRLMAINMENU_ITEM_T *item_ptr
)
{
    BOOLEAN is_data_got = FALSE;

    if (PNULL == menu_ctrl_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    if (group_id == MAINMENU_FILEFOLD_DEFAULT_GROUP_ID)
    {
        int32 i = FindFolderIndex(menu_ctrl_ptr, menu_id);

        if (i != FIND_NONE)
        {
            is_data_got = TRUE;
            item_ptr->menu_id = menu_id;
            item_ptr->group_id = group_id;

            item_ptr->text.type = CTRL_TEXT_BUFFER;

            item_ptr->text.data.str_info.wstr_ptr = menu_ctrl_ptr->folder_info.folder[i].text;
            item_ptr->text.data.str_info.wstr_len = menu_ctrl_ptr->folder_info.folder[i].text_len;

            item_ptr->call_back_start = OpenFolderCallBack;
            item_ptr->param_data1 = (uint32)menu_ctrl_ptr;

            //delete call back...
        }
    }
    else
    {
        is_data_got = MMIMAINMENU_GetItemData(menu_id, group_id, item_ptr);
    }


    return is_data_got;
}



/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO IStyleDrawItem(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dst_dev_ptr,
    GUI_RECT_T *rect_ptr,
    uint16 page_index,
    uint16 item_index,
    GUIMENU_ITEM_POSITION_E position
)
{
    GUI_LCD_DEV_INFO item_layer = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
    CTRLMAINMENU_ITEM_T item_info = {0};
    GUIMENU_DISPLAY_ITEM_PARAM_T param = {0};
    BOOLEAN is_item_find = FALSE;
    uint32 group_id = 0;
    uint32 menu_id = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return item_layer;
    }


    switch (position)
    {
        case GUIMENU_ITEM_IN_PAGE:

            if (page_index >= menu_ctrl_ptr->page_info.cnt
                || item_index >= menu_ctrl_ptr->page_info.page[page_index].cnt)
            {
                break;
            }

            is_item_find = TRUE;
            group_id = menu_ctrl_ptr->page_info.page[page_index].item[item_index].group_id;
            menu_id = menu_ctrl_ptr->page_info.page[page_index].item[item_index].menu_id;

            break;

        case GUIMENU_ITEM_IN_BAR:

            if (item_index >= menu_ctrl_ptr->bar_info.cnt)
            {
                break;
            }

            is_item_find = TRUE;
            group_id = menu_ctrl_ptr->bar_info.item[item_index].group_id;
            menu_id = menu_ctrl_ptr->bar_info.item[item_index].menu_id;

            break;

        case GUIMENU_ITEM_IN_FOLDER:

            if (page_index >= menu_ctrl_ptr->folder_info.cnt
                || item_index >= menu_ctrl_ptr->folder_info.folder[page_index].cnt)
            {
                break;
            }

            is_item_find = TRUE;
            group_id = menu_ctrl_ptr->folder_info.folder[page_index].item[item_index].group_id;
            menu_id = menu_ctrl_ptr->folder_info.folder[page_index].item[item_index].menu_id;

            break;

        default:
            break;
    }

    if (!is_item_find)
    {
        return item_layer;
    }

    is_item_find = IStyleGetItemData(menu_ctrl_ptr, menu_id, group_id, &item_info);

    if (!is_item_find)
    {
        return item_layer;
    }

    if (MAINMENU_FILEFOLD_DEFAULT_GROUP_ID == item_info.group_id)
    {
        uint32 folder_idx = 0;
        uint32 item_idx = 0;

        folder_idx = FindFolderIndex(menu_ctrl_ptr, menu_id);

        param.menu_item_num = menu_ctrl_ptr->folder_info.folder[folder_idx].cnt;

        for (item_idx = 0; item_idx < menu_ctrl_ptr->folder_info.folder[folder_idx].cnt; item_idx++)
        {
            param.group_id[item_idx] = menu_ctrl_ptr->folder_info.folder[folder_idx].item[item_idx].group_id;
            param.menu_id[item_idx] = menu_ctrl_ptr->folder_info.folder[folder_idx].item[item_idx].menu_id;
        }

        param.format |= GUIMENU_DISPLAY_ITEM_FORMAT_FOLDER_THUMBNAIL;
    }
#ifdef DYNAMIC_MAINMENU_SUPPORT
    else if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == item_info.group_id)
    {
        if (menu_ctrl_ptr->is_move_state && !MMIAPIUDISK_UdiskIsRun())
        {
            param.format |= GUIMENU_DISPLAY_ITEM_FORMAT_DELETE_ICON;
        }
    }
#endif

    if (PNULL == dst_dev_ptr || PNULL == rect_ptr)
    {
        item_layer = MMITHEME_DrawMainmenuItem(menu_ctrl_ptr->win_handle, &item_info, &param);
    }
    else
    {
        if (DATA_TYPE_RGB565 == UILAYER_GetLayerColorType(dst_dev_ptr))
        {
            item_layer = MMITHEME_DrawMainmenuItem(menu_ctrl_ptr->win_handle, &item_info, &param);

            UILAYER_SetLayerPosition(&item_layer, rect_ptr->left, rect_ptr->top);
            UILAYER_BltLayerToLayer(dst_dev_ptr, &item_layer, rect_ptr, TRUE);
        }
        else
        {
            param.format |= GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER;
            param.dst_layer = *dst_dev_ptr;
            param.dst_rect  = *rect_ptr;

            item_layer = MMITHEME_DrawMainmenuItem(menu_ctrl_ptr->win_handle, &item_info, &param);
        }
    }

    return item_layer;
}



/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void IStyleDrawAllItems(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    uint16              page_index,
    GUI_LCD_DEV_INFO   lcd_dev_info,
    int16 offset_x,
    BOOLEAN is_need_icon,
    BOOLEAN is_need_bar
)
{
    uint16              i = 0;
    uint16              item_num = 0;
    GUI_RECT_T          bar_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (is_need_icon)
    {
        for (i = 0; i < menu_ctrl_ptr->page_info.page[page_index].cnt; i++)
        {
            IStyleDrawItem(menu_ctrl_ptr, &lcd_dev_info,
                           &menu_ctrl_ptr->default_data.item_rect[i], page_index, i, GUIMENU_ITEM_IN_PAGE);
        }
    }

    if (is_need_bar)
    {
        GUI_LCD_DEV_INFO item_layer = {0};

        item_num = menu_ctrl_ptr->bar_info.cnt;

        GetBarItemRect(menu_ctrl_ptr, bar_rect, item_num);

        item_layer = GetIStyleBarItemLayer(menu_ctrl_ptr);

        for (i = 0; i < item_num; i++)
        {
            IStyleDrawItem(menu_ctrl_ptr, &item_layer, &bar_rect[i], 0, i, GUIMENU_ITEM_IN_BAR);
        }
    }
}

/*****************************************************************************/
//  Description : get pubwin button rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void GetBarItemRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_RECT_T       rect_ptr[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX],
    uint32           num
)
{
    GUI_RECT_T item_rect = menu_ctrl_ptr->default_data.bar_item_rect[0];
    GUI_RECT_T bar_rect = menu_ctrl_ptr->default_data.bar_rect;
    uint32     i = 0;
    uint16     width = item_rect.right - item_rect.left + 1;
    uint16     height = item_rect.bottom - item_rect.top + 1;
    uint16     margin_w = 0;

    if (3 == num)
    {
        margin_w = (bar_rect.right - bar_rect.left) / 8;
    }

    switch (num)
    {
        case 1:
            rect_ptr[0] = GUI_GetCenterRect(bar_rect, width, height);
            break;

        case 2:
            rect_ptr[0] = menu_ctrl_ptr->default_data.bar_item_rect[1];
            rect_ptr[1] = menu_ctrl_ptr->default_data.bar_item_rect[2];
            break;

        case 3:
        case 4:
            for (i = 0; i < num; i++)
            {
                rect_ptr[i] = menu_ctrl_ptr->default_data.bar_item_rect[i];

                rect_ptr[i].left  += margin_w;
                rect_ptr[i].right += margin_w;
            }

            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : rotate item layer
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ShakeItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T          *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO  *in_ptr,
    GUI_LCD_DEV_INFO        *out_ptr,
    uint16                  index,
    BOOLEAN                 is_bar_item
)
{
    GUI_LCD_DEV_INFO    out_dev     = {0};
    UILAYER_INFO_T      layer_in    = {0};
    UILAYER_INFO_T      layer_out   = {0};
    ROTATE_ARBITRARY_IN_T  rot_in   = {0};
    ROTATE_ARBITRARY_OUT_T rot_out  = {0};
    int32                  angle    = (s_shake_num + index) % MENU_SHAKE_ANGLE - MENU_SHAKE_ANGLE / 2;

    if (PNULL == menu_ctrl_ptr || PNULL == in_ptr || PNULL == out_ptr)
    {
        return;
    }

    if (angle < 0)
    {
        angle += 360;
    }
    else if (0 == angle)
    {
        angle++;
    }

    out_dev = *out_ptr;

    UILAYER_GetLayerInfo(in_ptr, &layer_in);
    UILAYER_GetLayerInfo(&out_dev, &layer_out);
    /* 设置参数*/
    rot_in.angle = angle;
    rot_in.src_format = IMGREF_FORMAT_ARGB888;
    rot_in.src_size.w = layer_in.layer_width;
    rot_in.src_size.h = layer_in.layer_height;
    rot_in.src_center.x = layer_in.layer_width / 2;
    rot_in.src_center.y = layer_in.layer_height / 2;
    rot_in.src_chn.chn0.ptr = (void *)UILAYER_GetLayerBufferPtr(in_ptr);
    rot_in.src_chn.chn0.size = layer_in.layer_width * layer_in.layer_height * sizeof(uint32);

    rot_in.target_format = IMGREF_FORMAT_ARGB888;
    rot_in.target_size.w = layer_out.layer_width;
    rot_in.target_size.h = layer_out.layer_height;
    rot_in.target_center.x = layer_out.layer_width / 2;
    rot_in.target_center.y = layer_out.layer_height / 2;
    rot_in.target_chn.chn0.ptr = (void *)UILAYER_GetLayerBufferPtr(&out_dev);
    rot_in.target_chn.chn0.size = layer_out.layer_width * layer_out.layer_height * sizeof(uint32);

    if (PNULL == rot_in.src_chn.chn0.ptr
        || PNULL == rot_in.target_chn.chn0.ptr)
    {
        return;
    }

    UILAYER_Clear(&out_dev);

    if (SCI_SUCCESS == GRAPH_RotateArbitrary(&rot_in, &rot_out))
    {
        *out_ptr = out_dev;
    }
}

/*****************************************************************************/
//  Description : rotate item layer
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void InvertBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T          *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO  *in_ptr,
    const GUI_LCD_DEV_INFO  *out_ptr
)
{
    UILAYER_INFO_T         layer_in  = {0};
    UILAYER_INFO_T         layer_out = {0};
    uint32                 width     = 0;
    uint32                 height    = 0;
    uint32                *src_ptr   = PNULL;
    uint32                *dst_ptr   = PNULL;
    uint32                 i         = 0;
    GUI_RECT_T             item_rect = {0};

    if (PNULL == in_ptr
        || PNULL == out_ptr)
    {
        return;
    }

    UILAYER_GetLayerInfo(in_ptr, &layer_in);
    UILAYER_GetLayerInfo(out_ptr, &layer_out);

    width = MIN(layer_in.layer_width, layer_out.layer_width);
    height = MIN(layer_in.layer_height, layer_out.layer_height / 2);
    height = MIN(height, MENUICON_HEIGHT);

    src_ptr = (uint32 *)UILAYER_GetLayerBufferPtr(in_ptr);
    dst_ptr = (uint32 *)UILAYER_GetLayerBufferPtr(out_ptr);

    if (PNULL == src_ptr
        || PNULL == dst_ptr)
    {
        return;
    }

    UILAYER_Clear(out_ptr);
    dst_ptr += layer_out.mem_width * (height * 2 - 1);

    for (i = 0; i < height; i++)
    {
        SCI_MEMCPY(dst_ptr, src_ptr, width * sizeof(uint32));
        src_ptr += layer_in.mem_width;
        dst_ptr -= layer_out.mem_width;
    }

    item_rect        = menu_ctrl_ptr->default_data.item_rect[0];
    item_rect.right  -= item_rect.left;
    item_rect.left   = 0;
    item_rect.bottom -= item_rect.top;
    item_rect.top    = 0;

    UILAYER_WeakLayerAlpha(out_ptr,
                           menu_ctrl_ptr->default_data.bg_mask_alpha);

    UILAYER_SetLayerPosition(in_ptr, 0, 0);
    UILAYER_SetLayerPosition(out_ptr, 0, 0);

    UILAYER_BltLayerToLayer(out_ptr, in_ptr, &item_rect, TRUE);
}

/*****************************************************************************/
//  Description : display bar
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayIStyleBarItem(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO   lcd_dev_info,
    BOOLEAN is_need_shake
)
{
    //倒影区域
    GUI_RECT_T rotation_rect = {0, 0, MMITHEME_MENU_ITEM_WIDTH - 1, MENUICON_HEIGHT * 2 - 1};
    uint16          i = 0;
    uint16          item_num = 0;
    GUIMENU_ITEM_BUF_T  inverted_bar_item_layer = {0};
    GUIMENU_ITEM_BUF_T  shake_bar_item_layer = {0};
    BOOLEAN             is_getshakelayer = FALSE;
    BOOLEAN             is_getinvertlayer = FALSE;
    GUI_RECT_T          bar_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};
    GUI_LCD_DEV_INFO    src_layer = {0};
    GUI_RECT_T          disp_rect = {0};

    inverted_bar_item_layer.icon_layer.block_id = UILAYER_NULL_HANDLE;
    shake_bar_item_layer.icon_layer.block_id = UILAYER_NULL_HANDLE;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    UILAYER_Clear(&lcd_dev_info);

    is_getinvertlayer = FALSE;
    is_getshakelayer = FALSE;

    CreateIStyleMenuItemLayer(menu_ctrl_ptr, rotation_rect, &inverted_bar_item_layer);
    is_getinvertlayer = UILAYER_IsLayerActive(&inverted_bar_item_layer.icon_layer);

    if (is_need_shake && is_getinvertlayer)
    {
        CreateIStyleMenuItemLayer(menu_ctrl_ptr, rotation_rect, &shake_bar_item_layer);
        is_getshakelayer = UILAYER_IsLayerActive(&shake_bar_item_layer.icon_layer);
    }

    item_num = menu_ctrl_ptr->bar_info.cnt;

    GetBarItemRect(menu_ctrl_ptr, bar_rect, item_num);

    for (i = 0; i < item_num; i++)
    {
        //不画current item
        if (i == menu_ctrl_ptr->tmp_cur_item_index
            && menu_ctrl_ptr->is_moving
            && menu_ctrl_ptr->tmp_is_bar_item)
        {
            continue;
        }

        src_layer = IStyleDrawItem(menu_ctrl_ptr, PNULL, PNULL, 0, i, GUIMENU_ITEM_IN_BAR);

        if (is_getinvertlayer)
        {
            InvertBarItemLayer(menu_ctrl_ptr, &src_layer, &inverted_bar_item_layer.icon_layer);
        }

        disp_rect = bar_rect[i];

        if (is_getinvertlayer)
        {
            if (is_need_shake && is_getshakelayer)
            {
                ShakeItemLayer(menu_ctrl_ptr, &inverted_bar_item_layer.icon_layer, &shake_bar_item_layer.icon_layer, i, TRUE);
                src_layer = shake_bar_item_layer.icon_layer;
            }
            else
            {
                src_layer = inverted_bar_item_layer.icon_layer;
            }
        }

        UILAYER_SetLayerPosition(&src_layer, disp_rect.left, disp_rect.top);

        disp_rect.bottom = disp_rect.top + MENUICON_HEIGHT * 2 - 1;

        UILAYER_BltLayerToLayer(&lcd_dev_info, &src_layer, &disp_rect, FALSE);
    }

    IMG_EnableTransparentColor(FALSE);

    DestoryIStyleItemLayer(&inverted_bar_item_layer);
    DestoryIStyleItemLayer(&shake_bar_item_layer);

    return;
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void BltSlideItemLayerToMenuLayerEx(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    GUI_LCD_DEV_INFO lcd_dev_info,
    int16            offset_x,
    BOOLEAN          is_need_bar,
    BOOLEAN           is_need_icon
)
{
    uint16              i = 0;
    uint16              page_item_num = 0;
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          page_rect = {0};
    GUI_RECT_T          clip_rect = {0};
    uint16              lcd_width       = 0; // lcd宽度
    uint16              lcd_height      = 0; // lcd高度
    GUI_LCD_DEV_INFO    layer_handle    = {0};

    GUIMENU_ITEM_BUF_T  shake_item_layer = {0};    //抖动的临时buffer
    BOOLEAN             is_need_shake = FALSE;
    BOOLEAN             is_getshakelayer = FALSE;


    shake_item_layer.icon_layer.block_id = UILAYER_NULL_HANDLE;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    page_rect.right = lcd_width - 1;
    page_rect.bottom = lcd_height - 1;

    IMG_EnableTransparentColor(TRUE);

    is_need_shake = (BOOLEAN)(menu_ctrl_ptr->is_move_state
                              && !menu_ctrl_ptr->is_moving //抖动时拖动item, 性能不足
                              && !(menu_ctrl_ptr->is_folder_open)
                              && !menu_ctrl_ptr->is_tp_pressed //滑动时不再抖动
                              && 0 == offset_x
                             );

    if (is_need_icon)
    {
        if (is_need_shake)
        {
            //create shake layer
            GUI_RECT_T shake_rect   = {0};    //shake layer rect

            shake_rect.right = MMITHEME_MENU_ITEM_WIDTH - 1;
            shake_rect.bottom = MMITHEME_MENU_ITEM_HEIGHT - 1;

            CreateIStyleMenuItemLayer(menu_ctrl_ptr, shake_rect, &shake_item_layer);

            is_getshakelayer = UILAYER_IsLayerActive(&shake_item_layer.icon_layer);
        }

        page_item_num = menu_ctrl_ptr->page_info.page[page_index].cnt;

        for (i = 0; i < page_item_num; i++)
        {
            //不画current item
            if (i == menu_ctrl_ptr->tmp_cur_item_index
                && menu_ctrl_ptr->is_moving
                && !(menu_ctrl_ptr->is_folder_open)
                && page_index == menu_ctrl_ptr->tmp_cur_page_index
                && !menu_ctrl_ptr->tmp_is_bar_item)
            {
                continue;
            }

            disp_rect = menu_ctrl_ptr->default_data.item_rect[i];

            disp_rect.left += (offset_x);
            disp_rect.right += (offset_x);

            //避免冗余刷新,提高性能
            if (!GUI_IntersectRect(&clip_rect, page_rect, disp_rect))
            {
                continue;
            }

            if (is_need_shake && is_getshakelayer)
            {
                layer_handle = IStyleDrawItem(menu_ctrl_ptr, PNULL, PNULL, page_index, i, GUIMENU_ITEM_IN_PAGE);

                ShakeItemLayer(menu_ctrl_ptr, &layer_handle, &shake_item_layer.icon_layer, i, FALSE);
                UILAYER_SetLayerPosition(&shake_item_layer.icon_layer, disp_rect.left, disp_rect.top);
                UILAYER_BltLayerToLayer(&lcd_dev_info, &shake_item_layer.icon_layer, &disp_rect, FALSE);
// 				SCI_SLEEP(300);
// 				MMITHEME_UpdateRect();
//                 SCI_SLEEP(300);
            }
            else
            {
                IStyleDrawItem(menu_ctrl_ptr, &lcd_dev_info, &disp_rect, page_index, i, GUIMENU_ITEM_IN_PAGE);
            }

		}

    }

    if (is_need_bar)
    {
        GUI_LCD_DEV_INFO item_layer = {0};

        item_layer = GetIStyleBarItemLayer(menu_ctrl_ptr);

        DisplayIStyleBarItem(menu_ctrl_ptr, item_layer, is_need_shake);
    }

    DestoryIStyleItemLayer(&shake_item_layer);

    IMG_EnableTransparentColor(FALSE);



}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleBltSlideItemLayerToMenuLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    GUI_LCD_DEV_INFO lcd_dev_info,
    int16            offset_x,
    BOOLEAN          is_need_bar
)
{
    if (menu_ctrl_ptr->is_search_entry)
    {
        int16           layer_x_position = 0;
        int16           layer_y_position = 0;

        UILAYER_GetLayerPosition(&menu_ctrl_ptr->tmp_layer_handle, &layer_x_position, &layer_y_position);

        //click dock icon in search page
        if (0 == layer_x_position)
        {
            return ;
        }
    }

    BltSlideItemLayerToMenuLayerEx(menu_ctrl_ptr, page_index, lcd_dev_info, offset_x, is_need_bar, TRUE);
}

/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence :
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetCurrentPageLyaerInfo(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    if (UILAYER_IsMultiLayerEnable()
        && UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0]))
    {
        lcd_dev_info = menu_ctrl_ptr->page_layer_handle[0];
    }

    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : stop item timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StopItemExchangeTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->user_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->user_timer_id);
        menu_ctrl_ptr->user_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StartItemExchangeTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->user_timer_id)
    {
        menu_ctrl_ptr->user_timer_id = MMK_CreateWinTimer(
                                           menu_ctrl_ptr->ctrl_handle,
                                           menu_ctrl_ptr->theme_data.item_exchange_period,
                                           FALSE);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IStyleHandleTimer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,  //事件类型
    DPARAM           param          //附带的处理参数
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if (menu_ctrl_ptr->tp_timer_id == *(uint8 *)param)
    {
        switch (menu_ctrl_ptr->continue_move_type)
        {
            case CONTINUE_MOVE_LEFT:
                if (menu_ctrl_ptr->cur_page_index > 0)
                {
                    HandleIStyleAutoMove(menu_ctrl_ptr, menu_ctrl_ptr->continue_move_type, TRUE);
                }

                break;

            case CONTINUE_MOVE_RIGHT:
                if (menu_ctrl_ptr->cur_page_index < menu_ctrl_ptr->page_info.cnt - 1)
                {
                    HandleIStyleAutoMove(menu_ctrl_ptr, menu_ctrl_ptr->continue_move_type, TRUE);
                }

                break;

            default:
                result = MMI_RESULT_FALSE;
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_6201_112_2_18_2_33_56_137, (uint8 *)"");
                break;
        }

        if (!result)
        {
            menu_ctrl_ptr->continue_move_type = CONTINUE_MOVE_INVALID;
            StopAutoMoveTimer(menu_ctrl_ptr);
        }
    }
    else if (menu_ctrl_ptr->redrew_timer_id == *(uint8 *)param)
    {
        GUI_POINT_T  point = {0};
        MMI_TP_STATUS_E state = MMI_TP_NONE;

        if (!menu_ctrl_ptr->is_moving)
        {
            MMK_GetLogicTPMsg(&state, &point);

            StopRedrawTimer(menu_ctrl_ptr);

            StartRedrawTimer(menu_ctrl_ptr);

            if (abs(menu_ctrl_ptr->slide_pre_point.x - point.x) > MMIMAINMENU_MOVE_SPACING)
            {
                UILAYER_SetDirectDraw(TRUE);
                UILAYER_SetMainLayerImmutable(TRUE);

                //设置双buffer切换，不使用copy
                UILAYER_SwitchDoubleMem(&menu_ctrl_ptr->page_layer_handle[0], TRUE);

                MovePageLayer(menu_ctrl_ptr, &point);
                menu_ctrl_ptr->slide_pre_point.x = point.x;
                menu_ctrl_ptr->slide_pre_point.y = point.y;

                StopMoveStartTimer(menu_ctrl_ptr);
            }
        }
    }
    else if (menu_ctrl_ptr->user_timer_id == *(uint8 *)param)
    {
        HandleItemExchange(menu_ctrl_ptr);
    }
    else if (menu_ctrl_ptr->shake_icon_timer_id == *(uint8 *)param)
    {
        IStyleDrawShaking(menu_ctrl_ptr);
    }
    else if (menu_ctrl_ptr->start_move_timer_id == *(uint8 *)param)
    {
        GUIMENU_MOVE_STATE_T move_state = GUIMENU_STATE_MOVE_NONE;
        StopMoveStartTimer(menu_ctrl_ptr);
        move_state = GUIMENU_STATE_MOVE_SET;
        MMITHEME_Vibrate(MMITHEME_MENUMAIN_ICON_VIBRATE); // 震动一次
        MMK_SendMsg(menu_ctrl_ptr->win_handle, MSG_NOTIFY_MENU_SET_STATUS, (DPARAM)&move_state);
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IStyleNotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    MMI_MESSAGE_ID_E    notify_msg = MSG_NOTIFY_START;

    switch (msg_id)
    {
        case MSG_APP_WEB:
            notify_msg = MSG_NOTIFY_MIDSK;
            break;

        case MSG_APP_OK:
            notify_msg = MSG_NOTIFY_OK;
            break;

        case MSG_TP_PRESS_UP:
            notify_msg = MSG_NOTIFY_PENOK;
            break;

        case MSG_APP_CANCEL:
            notify_msg = MSG_NOTIFY_CANCEL;
            break;

        default:
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUIMENU_2297_112_2_18_3_20_22_238, (uint8 *)"d", msg_id);
            break;
    }

    if (MSG_NOTIFY_START != notify_msg)
    {
        //notify parent control
        result = GUICTRL_PostNotify(ctrl_handle, notify_msg);
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleHandleCancelKey(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
)
{
#if 0
    BOOLEAN is_handled = FALSE;

    if (menu_ctrl_ptr->is_move_state)
    {
		
        VerifyItems(menu_ctrl_ptr);
        VerifyPages(menu_ctrl_ptr);
        VerifyFolders(menu_ctrl_ptr);
        SaveNv(menu_ctrl_ptr);

        // IStyleInitDetail(menu_ctrl_ptr);

        // 释放层
        IStyleReleaseMoveIconLayer(menu_ctrl_ptr);
        // 设置状态
        menu_ctrl_ptr->is_move_state = FALSE;
        menu_ctrl_ptr->is_moving = FALSE;

        IStyleDraw(menu_ctrl_ptr);

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMIAPISETVWP_Resume();
#endif
        is_handled = TRUE;
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_6281_112_2_18_2_33_56_138, (uint8 *)"");
    }

    if (!is_handled)
    {
        //notify parent control or parent window
        IStyleNotifyParentMsg(menu_ctrl_ptr->ctrl_handle, msg_id);
    }

    return is_handled;
#else
    return IStyleConfirmItem(menu_ctrl_ptr, msg_id);
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleIStyleAutoMove(
    MMIMAINMENU_ISTYLE_DATA_T       *menu_ctrl_ptr,
    CONTINUE_MOVE_TYPE_E continue_move_type,
    BOOLEAN is_add_page
)
{
    BOOLEAN result = TRUE;
    uint16  next_page_index = 0;
    uint16  total_page_num  = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    // item总数
    total_page_num = menu_ctrl_ptr->page_info.cnt;

    // 记录当前页
    next_page_index = menu_ctrl_ptr->cur_page_index;

    switch (continue_move_type)
    {
        case CONTINUE_MOVE_LEFT:
            if (0 == next_page_index) // 当前页为第一页，下一页为最后一页
            {
                next_page_index = (uint16)(total_page_num - 1);
            }
            else
            {
                next_page_index--; // 计算下页
            }

            break;

        case CONTINUE_MOVE_RIGHT:
            if (total_page_num - 1 == next_page_index) // 如果是最后一页
            {
                next_page_index = 0; // 下一页为首页
            }
            else
            {
                next_page_index++; // 下页累加
            }

            if (is_add_page && next_page_index == menu_ctrl_ptr->page_info.cnt - 1)
            {
                // 当移动到最后一页的时候，增加下一页
                if (IStyleAddNewPage(menu_ctrl_ptr))
                {
                    IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
                }
            }

            break;

        default:
            result = FALSE;
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_6348_112_2_18_2_33_57_139, (uint8 *)"d", continue_move_type);
            break;
    }

    //switch current slide page
    if (result) // title处于focus状态
    {
        if ((menu_ctrl_ptr->is_temp_filefold_create)
            && (IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index, FALSE)))
        {
            if (DestroyIStyleFilefoldTemp(menu_ctrl_ptr))
            {
                menu_ctrl_ptr->temp_filefold_index = -1;
                menu_ctrl_ptr->is_temp_filefold_create = FALSE;
            }
        }

        AutoSlidePage(menu_ctrl_ptr, next_page_index, continue_move_type);

        menu_ctrl_ptr->cur_page_index = next_page_index;
        menu_ctrl_ptr->is_bar_item = FALSE;

        menu_ctrl_ptr->is_empty_space = TRUE;

        if (HandleIStyleMove(menu_ctrl_ptr))
        {
            menu_ctrl_ptr->tmp_cur_page_index = menu_ctrl_ptr->cur_page_index;
            menu_ctrl_ptr->tmp_is_bar_item = FALSE;
        }

        if (!menu_ctrl_ptr->is_moving)
        {
            menu_ctrl_ptr->cur_item_index = 0;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : auto slide page layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void AutoSlidePage(
    MMIMAINMENU_ISTYLE_DATA_T      *menu_ctrl_ptr,
    uint16               next_page_index,
    CONTINUE_MOVE_TYPE_E continue_move_type
)
{
    int16               move_space      = 0; // 位移
    uint16              i               = 0; //
    uint16              frame_num       = 10; // 移动频率
    uint16              lcd_width       = 0; // lcd宽度
    uint16              lcd_height      = 0; // lcd高度
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    if (menu_ctrl_ptr->cur_page_index == next_page_index
        || 0 == menu_ctrl_ptr->page_info.cnt)
    {
        return;
    }

    if (CONTINUE_MOVE_RIGHT == continue_move_type) // 从右向左滑动
    {
        move_space = -lcd_width;
    }
    else if (CONTINUE_MOVE_LEFT == continue_move_type)// 从左向右滑动
    {
        move_space = lcd_width;
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_6424_112_2_18_2_33_57_140, (uint8 *)"d", continue_move_type);
        return;
    }

    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;

    SetMoveIconLayerPosition(menu_ctrl_ptr);

    UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);

    for (i = 5; i <= frame_num; i++)
    {
        int32 offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, i);

        IStyleDrawPageLayers(menu_ctrl_ptr, offset);

        MMITHEME_UpdateRect();
    }

    IStyleDrawBar(menu_ctrl_ptr, next_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);

    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

    return;
}

/*****************************************************************************/
//  Description : handle page touch panel press down msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleAddNewPage(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->page_info.cnt >= ISTYLE_PAGE_CNT_MAX)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->page_info.cnt - 1].cnt == 0)
    {
        return FALSE;
    }

    menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->page_info.cnt].cnt = 0;
    menu_ctrl_ptr->page_info.cnt += 1;

    IStyleResetBarIndexRect(&menu_ctrl_ptr->default_data, menu_ctrl_ptr->page_info.cnt);

    return TRUE;
}

/*****************************************************************************/
//  Description : Create click effect on icon
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void ClickEffectOnIcon(
    MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr
)
{
    BOOLEAN result = TRUE;

#ifdef UI_SUPPORT_U_MAINMENU
    result = FALSE;
#endif

    if (!result || (menu_ctrl_ptr->is_search_entry && !menu_ctrl_ptr->is_bar_item))/*lint !e774*/
    {
        return ;
    }

    if (FALSE == menu_ctrl_ptr->is_move_state)
    {
        GUI_RECT_T          layer_rect = {0};
        GUI_LCD_DEV_INFO    tmp_lcd_dev_info = {0};

        if (menu_ctrl_ptr->is_bar_item)
        {
            GUI_RECT_T bar_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};
            uint16      item_num = 0;

            item_num = menu_ctrl_ptr->bar_info.cnt;

            GetBarItemRect(menu_ctrl_ptr, bar_rect, item_num);

            layer_rect = bar_rect[menu_ctrl_ptr->cur_item_index];

            tmp_lcd_dev_info = IStyleDrawItem(menu_ctrl_ptr, PNULL, &layer_rect, 0, menu_ctrl_ptr->cur_item_index, GUIMENU_ITEM_IN_BAR);
        }
        else if (menu_ctrl_ptr->is_item_pressed)
        {
            if (!(menu_ctrl_ptr->is_folder_open))
            {
                layer_rect = menu_ctrl_ptr->default_data.item_rect[menu_ctrl_ptr->cur_item_index];

                tmp_lcd_dev_info = IStyleDrawItem(menu_ctrl_ptr, PNULL, &layer_rect, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index, GUIMENU_ITEM_IN_PAGE);
            }
            else
            {
                GetFolderItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &layer_rect);

                tmp_lcd_dev_info = IStyleDrawItem(menu_ctrl_ptr, PNULL, &layer_rect, menu_ctrl_ptr->open_folder_index, menu_ctrl_ptr->cur_item_index, GUIMENU_ITEM_IN_FOLDER);
            }
        }

        UILAYER_SetLayerPosition(&tmp_lcd_dev_info, layer_rect.left, layer_rect.top);

        UILAYER_WeakLayerAlpha(&tmp_lcd_dev_info, MAINMENU_CLICK_ALPHA_MASK);

        if (!(menu_ctrl_ptr->is_folder_open))
        {
            UILAYER_BltLayerToLayer(&menu_ctrl_ptr->page_layer_handle[0], &tmp_lcd_dev_info, &layer_rect, FALSE);
        }
        else
        {
            UILAYER_BltLayerToLayer(&menu_ctrl_ptr->page_layer_handle[1], &tmp_lcd_dev_info, &layer_rect, FALSE);
        }
    }
}


/*****************************************************************************/
//  Description : handle page touch panel press down msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleHandleTpDown(
    GUI_POINT_T       *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr
)
{
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          item_rect = {0};
    GUI_RECT_T          folder_rect = {0};
    uint16              item_index = 0;

    if ((PNULL == tp_point_ptr) || (PNULL == menu_ctrl_ptr))
    {
        return;
    }

    SCI_TRACE_LOW("mmimenu_istyle.c: HandleIStyleTpDown tick %d", SCI_GetTickCount());

    menu_ctrl_ptr->is_tp_pressed = TRUE;
    menu_ctrl_ptr->is_bar_item = FALSE;

    // 增加速度点
    MMK_AddVelocityItem(tp_point_ptr->x, tp_point_ptr->y);

    // 停止timer
    StopMoveStartTimer(menu_ctrl_ptr);

    // 首先设置页面的滑动相关数据
    // 记录起始点
    // 记录前一个点
    menu_ctrl_ptr->slide_start_point = menu_ctrl_ptr->slide_pre_point = menu_ctrl_ptr->pre_tp_point = *tp_point_ptr;

    if (!(menu_ctrl_ptr->is_folder_open))
    {
        if (! menu_ctrl_ptr->is_search_entry)
        {
            //if press item icon
            for (item_index = 0; item_index < menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].cnt; item_index++)
            {
                item_rect = menu_ctrl_ptr->default_data.item_rect[item_index];

                //if press item
                if (GUI_PointIsInRect(*tp_point_ptr, item_rect))
                {
                    disp_rect = item_rect;

                    menu_ctrl_ptr->tmp_width = tp_point_ptr->x - disp_rect.left;
                    menu_ctrl_ptr->tmp_height = tp_point_ptr->y - disp_rect.top;

                    menu_ctrl_ptr->is_item_pressed = TRUE;
                    menu_ctrl_ptr->is_bar_item = FALSE;
                    menu_ctrl_ptr->cur_item_index = item_index;
                    menu_ctrl_ptr->open_folder_index = item_index;
                    break;
                }
            }
        }

        if (!menu_ctrl_ptr->is_item_pressed)
        {
            GUI_RECT_T bar_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};
            uint32 total_num = menu_ctrl_ptr->bar_info.cnt;

            GetBarItemRect(menu_ctrl_ptr, bar_rect, total_num);

            //if press item icon
            for (item_index = 0; item_index < total_num; item_index++)
            {
                item_rect = bar_rect[item_index];

                //if press item
                if (GUI_PointIsInRect(*tp_point_ptr, item_rect))
                {
                    disp_rect = item_rect;

                    menu_ctrl_ptr->tmp_width = tp_point_ptr->x - disp_rect.left;
                    menu_ctrl_ptr->tmp_height = tp_point_ptr->y - disp_rect.top;

                    menu_ctrl_ptr->is_item_pressed = TRUE;
                    menu_ctrl_ptr->is_bar_item = TRUE;

                    menu_ctrl_ptr->cur_item_index = item_index;
                    menu_ctrl_ptr->open_folder_index = item_index;
                    break;
                }
            }
        }
    }
    else
    {
        for (item_index = 0; item_index < menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt; item_index++)
        {
            GetFolderItemRect(menu_ctrl_ptr, item_index, &item_rect);

            //if press item
            if (GUI_PointIsInRect(*tp_point_ptr, item_rect))
            {
                disp_rect = item_rect;

                menu_ctrl_ptr->tmp_width = tp_point_ptr->x - disp_rect.left;
                menu_ctrl_ptr->tmp_height = tp_point_ptr->y - disp_rect.top;

                menu_ctrl_ptr->is_item_pressed = TRUE;
                menu_ctrl_ptr->is_bar_item = FALSE;
                menu_ctrl_ptr->cur_item_index = item_index;

                break;
            }
        }
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        IStyleGetFolderRect(menu_ctrl_ptr, &folder_rect);

        if (GUI_PointIsInRect(*tp_point_ptr, folder_rect))
        {
            menu_ctrl_ptr->is_press_in_folder_rect = TRUE;
        }
    }

    if (menu_ctrl_ptr->is_item_pressed)
    {
        menu_ctrl_ptr->is_press_without_moving = TRUE;

        // 不是移动图标状态,启动Timer
        if (menu_ctrl_ptr->theme_data.is_used_org && !menu_ctrl_ptr->is_move_state
            && !menu_ctrl_ptr->is_search_entry)
        {
            StartMoveStartTimer(menu_ctrl_ptr);
        }

        //icon in clicked effect
        ClickEffectOnIcon(menu_ctrl_ptr);
    }

    if (!GUI_PointIsInRect(menu_ctrl_ptr->slide_start_point, menu_ctrl_ptr->default_data.bar_rect)
        && !(menu_ctrl_ptr->is_folder_open)
       )
    {
        // 不是移动图标状态,启动Timer
        StartRedrawTimer(menu_ctrl_ptr);
    }

#ifdef UI_SUPPORT_U_MAINMENU
    MMIISTYLE_U_GenerateRandomStyle(menu_ctrl_ptr);
#endif

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleHandleAutoSwitchPage(
    GUI_POINT_T *point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = TRUE;

    if (
        (PNULL == point_ptr) ||
        (PNULL == menu_ctrl_ptr)
    )
    {
        return FALSE;
    }

    if (!(menu_ctrl_ptr->is_folder_open))
    {
        if (GUI_PointIsInRect(*point_ptr, s_left_direct_move_rect))
        {
            menu_ctrl_ptr->continue_move_type = CONTINUE_MOVE_LEFT;

            if (0 == menu_ctrl_ptr->tp_timer_id)
            {
                if (menu_ctrl_ptr->cur_page_index > 0) // 当前页不是第一页
                {
                    StartAutoMoveTimer(menu_ctrl_ptr);
                }
            }
        }
        else if (GUI_PointIsInRect(*point_ptr, s_right_direct_move_rect))
        {
            menu_ctrl_ptr->continue_move_type = CONTINUE_MOVE_RIGHT;

            if (0 == menu_ctrl_ptr->tp_timer_id)
            {
                if (menu_ctrl_ptr->page_info.cnt - 1 > menu_ctrl_ptr->cur_page_index)// 当前页不是最后一页
                {
                    StartAutoMoveTimer(menu_ctrl_ptr);
                }
            }
        }
        else
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    if (!result)
    {
        menu_ctrl_ptr->continue_move_type = CONTINUE_MOVE_INVALID;
        StopAutoMoveTimer(menu_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetIStyleMoveLayerRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    GUI_RECT_T tmp_rect = {0};
    GUI_POINT_T point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    int16 tmp_width  = (int16)(menu_ctrl_ptr->tmp_width * menu_ctrl_ptr->default_data.zoom_out / 100);
    int16 tmp_height = (int16)(menu_ctrl_ptr->tmp_height * menu_ctrl_ptr->default_data.zoom_out / 100);
    int16 width      = (int16)(menu_ctrl_ptr->default_data.item_width * menu_ctrl_ptr->default_data.zoom_out / 100);
    int16 height     = (int16)(menu_ctrl_ptr->default_data.item_height * menu_ctrl_ptr->default_data.zoom_out / 100);

    MMK_GetLogicTPMsg(&state, &point);

    tmp_rect.left = MAX(point.x - tmp_width, 0);
    tmp_rect.top = MAX(point.y - tmp_height, 0);
    tmp_rect.right = tmp_rect.left + width - 1;
    tmp_rect.bottom = tmp_rect.top + height - 1;

    return tmp_rect;
}

/*****************************************************************************/
//  Description : handle icon menu ok
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleIStyleMove(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (!menu_ctrl_ptr->is_moving || !menu_ctrl_ptr->is_move_state)
    {
        return FALSE;
    }

    do
    {
        MMIMAINMENU_ITEM_INFO_T item = {0};

        if (menu_ctrl_ptr->is_folder_open)
        {
            if (menu_ctrl_ptr->cur_item_index == menu_ctrl_ptr->tmp_cur_item_index)
            {
                break;
            }

            item = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item[menu_ctrl_ptr->tmp_cur_item_index];

            DeleteItemFromFolder(menu_ctrl_ptr, menu_ctrl_ptr->open_folder_index, menu_ctrl_ptr->tmp_cur_item_index);
            InsertItemToFolder(menu_ctrl_ptr, &item, menu_ctrl_ptr->open_folder_index, menu_ctrl_ptr->cur_item_index);

            result = TRUE;

            break;
        }

        if (menu_ctrl_ptr->tmp_is_bar_item ==  menu_ctrl_ptr->is_bar_item
            && menu_ctrl_ptr->tmp_cur_page_index == menu_ctrl_ptr->cur_page_index
            && menu_ctrl_ptr->tmp_cur_item_index == menu_ctrl_ptr->cur_item_index)
        {
            break;
        }

        if (menu_ctrl_ptr->is_bar_item)
        {
            if (menu_ctrl_ptr->bar_info.cnt >= ISTYLE_BAR_ITEM_CNT_MAX && !menu_ctrl_ptr->tmp_is_bar_item)
            {
                break;
            }
        }
        else
        {
            if (menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].cnt >= ISTYLE_PAGE_ITEM_CNT_MAX
                && menu_ctrl_ptr->tmp_cur_page_index != menu_ctrl_ptr->cur_page_index)
            {
                break;
            }
        }

        if (menu_ctrl_ptr->tmp_is_bar_item)
        {
            //item原属于bar
            item = menu_ctrl_ptr->bar_info.item[menu_ctrl_ptr->tmp_cur_item_index];
            DeleteItemFromBar(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index);
        }
        else
        {
            //item原属于宫格
            item = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->tmp_cur_page_index].item[menu_ctrl_ptr->tmp_cur_item_index];
            DeleteItemFromPage(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_page_index, menu_ctrl_ptr->tmp_cur_item_index);
        }

        if (menu_ctrl_ptr->is_bar_item)
        {
            InsertItemToBar(menu_ctrl_ptr, &item, menu_ctrl_ptr->cur_item_index);
        }
        else
        {
            InsertItemToPage(menu_ctrl_ptr, &item, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index);
        }

        result = TRUE;
    }
    while (0);

    if (result)
    {
        menu_ctrl_ptr->tmp_cur_page_index = menu_ctrl_ptr->cur_page_index;
        menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index;
        IStyleDraw(menu_ctrl_ptr);
    }

    return result;
}


/*****************************************************************************/
//  Description : is point in icon rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPointInIconRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T    point,
    GUI_RECT_T     rect
)

{
    GUI_RECT_T icon_rect = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    GetIconTextRect(rect, &icon_rect, PNULL);

    return GUI_PointIsInRect(point, icon_rect);
}



/*****************************************************************************/
//  Description : handle item exchange
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void HandleItemExchange(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    int32       i = 0;
    uint16      item_index = 0;
    uint16      page_item_num = 0;
    int16       center_x = 0;
    GUI_RECT_T  item_rect = {0};
    BOOLEAN     is_bar_item = FALSE;
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    BOOLEAN     is_bar_item_tmp = FALSE;
    uint16      item_index_tmp = 0;
    BOOLEAN     is_find = FALSE;
    BOOLEAN     is_find_in_bar = FALSE;
    BOOLEAN     is_in_self_rect = FALSE;
    GUI_RECT_T       folder_rect = {0};

    //1 stop timer
    StopItemExchangeTimer(menu_ctrl_ptr);

    if (PNULL == menu_ctrl_ptr || !menu_ctrl_ptr->is_move_state)
    {
        return;
    }

    //2 get system parameter
    MMK_GetLogicTPMsg(&state, &point);

    //3
    if (menu_ctrl_ptr->is_folder_open)
    {
        //get item total number
        page_item_num = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt;

        item_index = page_item_num;

        for (i = 0; i < page_item_num; i++)
        {
            GetFolderItemRect(menu_ctrl_ptr, i, &item_rect);

            if (GUI_PointIsInRect(point, item_rect))
            {
                is_find = IsPointInIconRect(menu_ctrl_ptr, point, item_rect);
                item_index = i;
                break;
            }
        }
    }
    else
    {
        if (GUI_PointIsInRect(point, menu_ctrl_ptr->default_data.bar_rect))
        {
            //若临时文件夹创建在非bar区域，item移到bar上，会导致temp_filefold_index找不到临时文件夹，销毁不掉
            if (!menu_ctrl_ptr->is_bar_item
                && (menu_ctrl_ptr->is_temp_filefold_create)
                && (IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index, FALSE))
               )
            {

                if (DestroyIStyleFilefoldTemp(menu_ctrl_ptr))
                {
                    menu_ctrl_ptr->temp_filefold_index = -1;
                    menu_ctrl_ptr->is_temp_filefold_create = FALSE;
                }
            }

            //get item total number
            page_item_num = menu_ctrl_ptr->bar_info.cnt;

            item_index = page_item_num;

            if (0 != page_item_num)
            {
                GUI_RECT_T bar_item_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};

                GetBarItemRect(menu_ctrl_ptr, bar_item_rect, page_item_num);

                //bar内部
                if (menu_ctrl_ptr->is_bar_item)
                {
                    //向右
                    if (point.x > menu_ctrl_ptr->slide_start_point.x)
                    {
                        for (i = page_item_num - 1; i >= 0; i--)
                        {
                            item_rect = bar_item_rect[i];

                            if (point.x > item_rect.left)
                            {
                                item_index = i;

                                is_find_in_bar = (point.x > (item_rect.left + (item_rect.right - item_rect.left) / 4)
                                                  && point.x < (item_rect.left + (item_rect.right - item_rect.left) * 3 / 4));

                                break;
                            }
                        }
                    }
                    //向左
                    else
                    {
                        for (i = 0; i < page_item_num; i++)
                        {
                            item_rect = bar_item_rect[i];

                            if (point.x < item_rect.right)
                            {
                                item_index = i;

                                is_find_in_bar = (point.x < (item_rect.right - (item_rect.right - item_rect.left) / 4)
                                                  && point.x > (item_rect.right - (item_rect.right - item_rect.left) * 3 / 4));

                                break;
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < page_item_num; i++)
                    {
                        item_rect = bar_item_rect[i];

                        center_x = (item_rect.left + item_rect.right) / 2;

                        if (point.x < center_x)
                        {
                            item_index = i;

                            is_find_in_bar = (point.x > (center_x - (item_rect.right - item_rect.left) / 4)
                                              && point.x < (center_x + (item_rect.right - item_rect.left) * 1 / 4));

                            break;
                        }
                    }
                }
            }

            is_bar_item = TRUE;
        }
        else
        {
            //get item total number
            page_item_num = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].cnt;

            item_index = page_item_num;

            for (i = 0; i < page_item_num; i++)
            {
                item_rect = menu_ctrl_ptr->default_data.item_rect[i];

                if (GUI_PointIsInRect(point, item_rect))
                {
                    is_find = IsPointInIconRect(menu_ctrl_ptr, point, item_rect);
                    item_index = i;
                    break;
                }
            }

            item_rect = menu_ctrl_ptr->default_data.item_rect[menu_ctrl_ptr->cur_item_index];

            is_in_self_rect = GUI_PointIsInRect(point, item_rect);
        }
    }

    if (item_index != menu_ctrl_ptr->cur_item_index
        && menu_ctrl_ptr->is_folder_open
        && menu_ctrl_ptr->is_moving
        && is_find)
    {
        menu_ctrl_ptr->cur_item_index = item_index;

        if (HandleIStyleMove(menu_ctrl_ptr))
        {
            menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index;

        }
    }
    else if (item_index != menu_ctrl_ptr->cur_item_index
             || is_bar_item != menu_ctrl_ptr->is_bar_item
             || menu_ctrl_ptr->is_temp_filefold_create)
    {
        // save state
        is_bar_item_tmp = menu_ctrl_ptr->is_bar_item;
        item_index_tmp = menu_ctrl_ptr->cur_item_index;

        //set current item index
        menu_ctrl_ptr->cur_item_index = item_index;
        menu_ctrl_ptr->is_bar_item = is_bar_item;

        if (item_index >= page_item_num)
        {
            if (is_bar_item_tmp != menu_ctrl_ptr->is_bar_item)
            {
                menu_ctrl_ptr->is_empty_space = TRUE;
            }
            else
            {
                if (0 == page_item_num)
                {
                    menu_ctrl_ptr->cur_item_index = 0;
                }
                else
                {
                    menu_ctrl_ptr->cur_item_index = page_item_num - 1;
                }
            }
        }

        if (!(menu_ctrl_ptr->is_folder_open)//文件夹内不再创建文件夹
            && menu_ctrl_ptr->folder_info.cnt < ISTYLE_FOLDER_CNT_MAX
            && (is_find || is_find_in_bar)
            && (!menu_ctrl_ptr->is_temp_filefold_create)
            && (!IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_page_index, menu_ctrl_ptr->tmp_cur_item_index, menu_ctrl_ptr->tmp_is_bar_item))
           )
        {

            /* 建立临时文件夹*/
            if (!IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr->is_bar_item)
                && !is_find_in_bar)
            {
                if (CreateIStyleFilefoldTemp(menu_ctrl_ptr))
                {
                    menu_ctrl_ptr->temp_filefold_index = menu_ctrl_ptr->cur_item_index;
                    menu_ctrl_ptr->is_temp_filefold_create = TRUE;
                }
            }

            // recover state
            menu_ctrl_ptr->cur_item_index = item_index_tmp;
            menu_ctrl_ptr->is_bar_item = is_bar_item_tmp;
        }
        else if ((menu_ctrl_ptr->is_temp_filefold_create)
                 && (!IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_page_index, menu_ctrl_ptr->tmp_cur_item_index, menu_ctrl_ptr->tmp_is_bar_item))
                 && (IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index, menu_ctrl_ptr->is_bar_item))
                )
        {
            if ((!is_find) || is_in_self_rect)
            {
                if (DestroyIStyleFilefoldTemp(menu_ctrl_ptr))
                {
                    menu_ctrl_ptr->temp_filefold_index = -1;
                    menu_ctrl_ptr->is_temp_filefold_create = FALSE;
                }
            }
            else
            {
                if (menu_ctrl_ptr->cur_item_index != menu_ctrl_ptr->temp_filefold_index)
                {
                    if (DestroyIStyleFilefoldTemp(menu_ctrl_ptr))
                    {
                        menu_ctrl_ptr->temp_filefold_index = -1;
                        menu_ctrl_ptr->is_temp_filefold_create = FALSE;

                        /* 建立临时文件夹*/
                        if (!IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr->is_bar_item)
                            && !is_find_in_bar)
                        {
                            if (CreateIStyleFilefoldTemp(menu_ctrl_ptr))
                            {
                                menu_ctrl_ptr->temp_filefold_index = menu_ctrl_ptr->cur_item_index;
                                menu_ctrl_ptr->is_temp_filefold_create = TRUE;
                            }
                        }
                    }
                }
            }

            // recover state
            menu_ctrl_ptr->cur_item_index = item_index_tmp;
            menu_ctrl_ptr->is_bar_item = is_bar_item_tmp;

        }
        else  if (menu_ctrl_ptr->is_moving)
        {
            if (HandleIStyleMove(menu_ctrl_ptr))
            {
                menu_ctrl_ptr->tmp_cur_page_index = menu_ctrl_ptr->cur_page_index;
                menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index;
                menu_ctrl_ptr->tmp_is_bar_item = is_bar_item;
            }
        }
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        IStyleGetFolderRect(menu_ctrl_ptr, &folder_rect);

        if (!GUI_PointIsInRect(point, folder_rect))
        {
            uint16 dest_page_index = menu_ctrl_ptr->open_folder_page_index;

            if (menu_ctrl_ptr->is_moving)
            {
                if (!RemoveItemFromFilefold(menu_ctrl_ptr, &dest_page_index))
                {
                    dest_page_index = menu_ctrl_ptr->cur_page_index;
                }
            }

            if (menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt <= 1)
            {
                Destroy1ItemFilefold(menu_ctrl_ptr);
            }
            else
            {
                IStyleCloseFilefolder(menu_ctrl_ptr);
            }

            //current page is full
            if (dest_page_index != menu_ctrl_ptr->cur_page_index)
            {
                menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->tmp_cur_page_index = dest_page_index;
            }
        }
    }

    return;
}


/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleHandleTpMove(
    GUI_POINT_T *point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if ((PNULL == point_ptr) || (PNULL == menu_ctrl_ptr))
    {
        return;
    }

    // 增加速度点
    MMK_AddVelocityItem(point_ptr->x, point_ptr->y);

    StopItemExchangeTimer(menu_ctrl_ptr);
    SetMoveIconLayerPosition(menu_ctrl_ptr);

    menu_ctrl_ptr->pre_tp_point.x = point_ptr->x;
    menu_ctrl_ptr->pre_tp_point.y = point_ptr->y;

    if (!(menu_ctrl_ptr->is_folder_open))
    {
        // 当move消息来时，停止timer
        if (abs(point_ptr->x - menu_ctrl_ptr->slide_start_point.x) > MMIMAINMENU_MOVE_SPACING
            || !menu_ctrl_ptr->is_item_pressed)
        {
            if (!GUI_PointIsInRect(menu_ctrl_ptr->slide_start_point, menu_ctrl_ptr->default_data.bar_rect))
            {
                if (!menu_ctrl_ptr->is_item_pressed || !menu_ctrl_ptr->is_move_state) // move icon状态下，不设置slide状态
                {
                    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
                }

                StopMoveStartTimer(menu_ctrl_ptr);
            }
        }
    }

    // 处理slide效果
    if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
    {
        return;
    }

    // 处理auto move
    if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving
        && IStyleHandleAutoSwitchPage(point_ptr, menu_ctrl_ptr))
    {
        return;
    }

    if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)  // 处理移动状态
    {
        StartItemExchangeTimer(menu_ctrl_ptr);
    }

    menu_ctrl_ptr->is_press_without_moving = FALSE;

    return;
}


/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSlideCurPage(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
)
{
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    if (0 == menu_ctrl_ptr->page_info.cnt)
    {
        return FALSE;
    }

    //get selected bar display rect
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    if (tp_point_ptr->x < menu_ctrl_ptr->slide_start_point.x) // 向左移动
    {
        // 判断是否最后一页
        if (menu_ctrl_ptr->slide_start_point.x - tp_point_ptr->x >= ((int32)(layer_width) * 1 / 4)
            || menu_ctrl_ptr->fling_velocity < -FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index < menu_ctrl_ptr->page_info.cnt - 1) // 不是最后一页
            {
                menu_ctrl_ptr->cur_page_index++; // 当前页索引号增加
            }
        }
    }
    else // 向右移动
    {
        // 判断是否第一页
        if (tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x >= (int32)layer_width * 1 / 4
            || menu_ctrl_ptr->fling_velocity > FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index != 0)
            {
                menu_ctrl_ptr->cur_page_index--;
            }
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideOver(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)
{
#ifdef UI_SUPPORT_U_MAINMENU
    MMIISTYLE_U_HandleUMenuSlideOverX(menu_ctrl_ptr, tp_point_ptr, FALSE);
    IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
#else
    int32   offset_x        = 0;
    int16   move_space      = 0; // 位移
    int16   lcd_pos         = 0; // lcd位置
    uint16  frame_num       = 8; // 移动频率
    uint16  cur_frame       = 1; // 计数
    uint16  pre_page_index  = 0; // 记录移动之前的页
    uint16  lcd_width       = 0; // lcd宽度
    uint16  lcd_height      = 0; // lcd高度
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    move_space = tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x;

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    pre_page_index = menu_ctrl_ptr->cur_page_index;

    // 设置下一页
    SetSlideCurPage(tp_point_ptr, menu_ctrl_ptr);

    if (pre_page_index == menu_ctrl_ptr->cur_page_index) // 当前页没变化
    {
        menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->tmp_cur_item_index; // 没有换页，缺省不变

        if (move_space > 0)  // 说明向右移动了当前页
        {
            lcd_pos = -((int16)lcd_width - 1);
        }
        else if (move_space < 0)   // 向左移动当前页
        {
            lcd_pos = ((int16)lcd_width - 1);
        }
    }
    else  // 换页了
    {
        menu_ctrl_ptr->cur_item_index = 0; // 换页之后缺省为0

        if (move_space > 0)   // 说明向右移动了当前页
        {
            lcd_pos = ((int16)lcd_width - 1);
        }
        else if (move_space < 0)  // 向左移动当前页
        {
            lcd_pos = -((int16)lcd_width - 1);
        }
    }

    if (0 != move_space)
    {
        for (; cur_frame <= frame_num; cur_frame++)
        {
            if (menu_ctrl_ptr->cur_page_index == pre_page_index) // 没换页
            {
                offset_x = move_space - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);
            }
            else // 换页了
            {
                offset_x = -(lcd_pos - move_space) + MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, lcd_pos - move_space, frame_num, cur_frame);
            }

            if (cur_frame < frame_num)
            {
                UILAYER_SetDirectDraw(TRUE);
                UILAYER_SetMainLayerImmutable(TRUE);
            }

            IStyleDrawPageLayers(menu_ctrl_ptr, offset_x);

            MMITHEME_UpdateRect();
        }

        IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
    }
    else
    {
        IStyleDrawPageLayers(menu_ctrl_ptr, offset_x);
    }

#endif
}

/*****************************************************************************/
//  Description : handle entry operation in page 0
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleEntrySearchPage(MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr)
{
    MMI_HANDLE_T    editbox_handle = 0;
    MMI_HANDLE_T    list_handle = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    GetSearchCtrlHandle(&editbox_handle, &list_handle, PNULL);

    menu_ctrl_ptr->is_search_entry = TRUE;

    SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_EDIT_TEXT);

    ResetSearchPageLayerPosition(menu_ctrl_ptr);

    //destroy cache and video wallpaper before construct im
    MMITHEME_DestroyMenuItemCache();
    MMITHEME_DestroyMenuFolderCache();

    RemoveSearchLayer(menu_ctrl_ptr);

    MMK_SetWinHookFunc(menu_ctrl_ptr->win_handle, SearchPageHookFunc);

#ifdef SEARCH_SUPPORT
    GUILIST_SetForbidTip(list_handle, TRUE);

    MMIAPISEARCH_SetSearchWinHandle(menu_ctrl_ptr->win_handle);

    if (PNULL != s_search_word_ptr)
    {
        MMI_STRING_T    keyword = {0};

        keyword.wstr_ptr = s_search_word_ptr;
        keyword.wstr_len = MMIAPICOM_Wstrlen(s_search_word_ptr);

        GUIEDIT_SetString(editbox_handle, keyword.wstr_ptr, keyword.wstr_len);

        SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_ALL);
    }

#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : handle tp up in page 0
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:tp up in page 0 may enter search page
/*****************************************************************************/
LOCAL BOOLEAN HandleIStylePage0TpUp(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
)
{
    MMI_HANDLE_T    editbox_handle = 0;

    if (PNULL == tp_point_ptr || PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    GetSearchCtrlHandle(&editbox_handle, PNULL, PNULL);

    if (!menu_ctrl_ptr->is_search_entry)
    {
        if (!menu_ctrl_ptr->is_search_display
            || tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x < 0) // 前一点与当前点之差太小的时候，这里为负值，此时应该释放搜索页
        {
            if (menu_ctrl_ptr->is_search_display && tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x < 0) // 前一点与当前点之差太小的时候，这里为负值，此时应该释放搜索页
            {
                ReleaseSearchPage(menu_ctrl_ptr, FALSE);
            }

            //normal tp up
            HandleSlideOver(menu_ctrl_ptr, tp_point_ptr);
        }
        else
        {
            uint16              layer_width = 0;
            uint16              layer_height = 0;
            GUI_LCD_DEV_INFO    lcd_dev_info = {0};

            CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

            //get selected bar display rect
            GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

            if ((tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x >= (int32)layer_width * 1 / 4
                 || menu_ctrl_ptr->fling_velocity > FLING_MIN_VELOCITY)
                && !menu_ctrl_ptr->is_move_state
                && UILAYER_IsLayerActive(&menu_ctrl_ptr->tmp_layer_handle))
            {
                menu_ctrl_ptr->is_search_entry = TRUE;
                HandleSearchSlideOver(menu_ctrl_ptr, tp_point_ptr);

                SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_EDIT_TEXT);

                HandleEntrySearchPage(menu_ctrl_ptr);
            }
            else
            {
                HandleSearchSlideOver(menu_ctrl_ptr, tp_point_ptr);

                ReleaseSearchPage(menu_ctrl_ptr, FALSE);
            }
        }
    }
    else
    {
        //tp up in search page
        uint16              layer_width = 0;
        uint16              layer_height = 0;
        GUI_LCD_DEV_INFO    lcd_dev_info = {0};

        CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

        //get selected bar display rect
        GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

        if (menu_ctrl_ptr->slide_start_point.x - tp_point_ptr->x >= (int32)layer_width * 1 / 4
            || (menu_ctrl_ptr->fling_velocity > FLING_MIN_VELOCITY && menu_ctrl_ptr->slide_start_point.x > tp_point_ptr->x)
            ||  ! UILAYER_IsLayerActive(&menu_ctrl_ptr->tmp_layer_handle))
        {
            menu_ctrl_ptr->is_search_entry = FALSE;

            if (! UILAYER_IsLayerActive(&menu_ctrl_ptr->tmp_layer_handle)
                && tp_point_ptr->x > menu_ctrl_ptr->slide_start_point.x)
            {
                //向右滑动抬起直接退出，模拟滑动效果
                tp_point_ptr->x = menu_ctrl_ptr->slide_start_point.x - 1 ;
            }

            HandleSearchSlideOver(menu_ctrl_ptr, tp_point_ptr);

            ReleaseSearchPage(menu_ctrl_ptr, FALSE);
        }
        else
        {
            MMI_STRING_T    keyword_str = {0};

            if (tp_point_ptr->x < menu_ctrl_ptr->slide_start_point.x)
            {
                HandleSearchSlideOver(menu_ctrl_ptr, tp_point_ptr);
            }

            ResetSearchPageLayerPosition(menu_ctrl_ptr);

            SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_EDIT_TEXT);

            GUIEDIT_GetString(editbox_handle, &keyword_str);

            if (0 < keyword_str.wstr_len && PNULL != keyword_str.wstr_ptr)
            {
                SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_ALL);
            }

            if (menu_ctrl_ptr->page_layer_handle[0].block_id != menu_ctrl_ptr->tmp_layer_handle.block_id)
            {
                RemoveSearchLayer(menu_ctrl_ptr);
            }
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : recover page state after short move(not receive tp_move msg)
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void RecoverPageSlideStateAfterTpTinyMove(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)
{
    if (PNULL == menu_ctrl_ptr || PNULL == tp_point_ptr)
    {
        return ;
    }

    if (menu_ctrl_ptr->is_search_display && !menu_ctrl_ptr->is_search_entry)
    {
        ReleaseSearchPage(menu_ctrl_ptr, FALSE);
    }

    if (!(menu_ctrl_ptr->is_folder_open)
        && ! menu_ctrl_ptr->is_search_entry
        && abs(menu_ctrl_ptr->slide_start_point.x - tp_point_ptr->x) > MMIMAINMENU_MOVE_SPACING
        && !menu_ctrl_ptr->is_moving
        && !menu_ctrl_ptr->is_press_without_moving
        && !GUI_PointIsInRect(menu_ctrl_ptr->slide_start_point, menu_ctrl_ptr->default_data.bar_rect))
    {
        HandleSlideOver(menu_ctrl_ptr, tp_point_ptr);
    }
}



LOCAL BOOLEAN IStyleRunItem(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    MMIMAINMENU_ITEM_INFO_T *item_ptr = {0};
    CTRLMAINMENU_ITEM_T item_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        item_ptr = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item + menu_ctrl_ptr->cur_item_index;
    }
    else if (menu_ctrl_ptr->is_bar_item)
    {
        item_ptr = menu_ctrl_ptr->bar_info.item + menu_ctrl_ptr->cur_item_index;
    }
    else
    {
        item_ptr = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item + menu_ctrl_ptr->cur_item_index;
    }

    IStyleGetItemData(menu_ctrl_ptr, item_ptr->menu_id, item_ptr->group_id, &item_info);

    if (
#ifdef DYNAMIC_MAINMENU_SUPPORT    
    !MMIMENU_DyRunItem(item_ptr->group_id,item_ptr->menu_id) && 
#endif
    PNULL != item_info.call_back_start)
    {
        item_info.call_back_start(item_info.param_data1, item_info.param_data2);
    }

    return TRUE;
}


/*****************************************************************************/
//  Description : handle slide menu touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleHandleTpUp(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
)
{
    BOOLEAN             result = FALSE;
    float               fling_velocity = 0;
    BOOLEAN             is_open_folder = FALSE;
    GUI_RECT_T          folder_rect = {0};
    GUI_RECT_T          edit_rect = {0};
    BOOLEAN             is_add_item = FALSE;
    BOOLEAN             is_filefold_first_create = FALSE;

    if (PNULL == tp_point_ptr || PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("mmimenu_istyle.c: IStyleHandleTpUp tick %d", SCI_GetTickCount());

    //恢复双buffer copy
    UILAYER_SwitchDoubleMem(&menu_ctrl_ptr->page_layer_handle[0], FALSE);

#ifdef DYNAMIC_MAINMENU_SUPPORT

    if (!MMIAPIUDISK_UdiskIsRun())
    {
        HandleDeleteTpUpMsg(tp_point_ptr, menu_ctrl_ptr);
    }

#endif

    menu_ctrl_ptr->is_tp_pressed = FALSE;

    if (menu_ctrl_ptr->is_move_state)
    {
        is_add_item = AddItemToFilefold(menu_ctrl_ptr, &is_filefold_first_create);

        if (is_add_item)
        {
            IStyleUpdateFolderThumbnail(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr->is_bar_item);
        }
    }

    menu_ctrl_ptr->is_temp_filefold_create = FALSE;
    menu_ctrl_ptr->temp_filefold_index = -1;

    // 增加速度点
    MMK_AddVelocityItem(tp_point_ptr->x, tp_point_ptr->y);

    MMK_ComputeCurrentVelocity(&fling_velocity, PNULL, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);

    menu_ctrl_ptr->fling_velocity = fling_velocity;

    StopAutoMoveTimer(menu_ctrl_ptr);
    StopMoveStartTimer(menu_ctrl_ptr);
    StopRedrawTimer(menu_ctrl_ptr);

    StopItemExchangeTimer(menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_moving)
    {
        if (menu_ctrl_ptr->cur_page_index == menu_ctrl_ptr->page_info.cnt - 1)
        {
            if (IStyleAddNewPage(menu_ctrl_ptr))
            {
                IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
            }
        }
    }

    result = menu_ctrl_ptr->is_item_pressed;
    menu_ctrl_ptr->is_item_pressed = FALSE;

    if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state)
    {
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

        if (0 == menu_ctrl_ptr->cur_page_index)
        {
            HandleIStylePage0TpUp(tp_point_ptr, menu_ctrl_ptr);
        }
        else
        {
            HandleSlideOver(menu_ctrl_ptr, tp_point_ptr);
        }

#ifdef UI_SUPPORT_U_MAINMENU
        MMIISTYLE_U_SetPageInfo(menu_ctrl_ptr, FALSE);
#endif
        menu_ctrl_ptr->is_cur_pagesearch = menu_ctrl_ptr->is_search_display;

        return FALSE;
    }
    else
    {
        //处理烂屏
        RecoverPageSlideStateAfterTpTinyMove(menu_ctrl_ptr, tp_point_ptr);
    }

    if (result && !menu_ctrl_ptr->is_moving && !(menu_ctrl_ptr->is_folder_open)
        && IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr->is_bar_item))
    {
        is_open_folder = TRUE;

        if (menu_ctrl_ptr->is_search_entry)
        {
            is_open_folder = FALSE;

            BltSlideItemLayerToMenuLayerEx(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->page_layer_handle[0], 0, TRUE, FALSE);
        }
    }

    if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
    {
        menu_ctrl_ptr->is_empty_space = FALSE;
        menu_ctrl_ptr->is_moving = FALSE; // 移动结束

        ResetIStyleMoveItemInfo(menu_ctrl_ptr);

        // 释放层
        IStyleReleaseMoveIconLayer(menu_ctrl_ptr);
    }

    menu_ctrl_ptr->is_cur_pagesearch = menu_ctrl_ptr->is_search_display;

    if ((is_open_folder && (!is_add_item) && (menu_ctrl_ptr->is_press_without_moving))
        || (is_add_item && is_filefold_first_create))
    {
        //open folder here
        IStyleOpenFolder(menu_ctrl_ptr);
        return FALSE;
    }

    if (menu_ctrl_ptr->is_folder_open)
    {
        IStyleGetFolderRect(menu_ctrl_ptr, &folder_rect);

        if (!GUI_PointIsInRect(*tp_point_ptr, folder_rect) && !menu_ctrl_ptr->is_press_in_folder_rect)
        {
            IStyleCloseFilefolder(menu_ctrl_ptr);
        }

        if (menu_ctrl_ptr->is_move_state)
        {
            GetEditRect(menu_ctrl_ptr, &edit_rect);

            if (GUI_PointIsInRect(*tp_point_ptr, edit_rect))
            {
                GUIAPICTRL_SetState(menu_ctrl_ptr->ctrl_foldername_handle, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
                MMK_SetActiveCtrl(menu_ctrl_ptr->ctrl_foldername_handle, TRUE);
            }
        }
    }

    menu_ctrl_ptr->is_press_without_moving = FALSE;
    menu_ctrl_ptr->is_press_in_folder_rect = FALSE;

#ifdef UI_SUPPORT_U_MAINMENU

    if (!(menu_ctrl_ptr->is_folder_open))
    {
        MMIISTYLE_U_SetPageInfo(menu_ctrl_ptr, FALSE);
    }

#endif
	
    //SaveNv(menu_ctrl_ptr);
    return result;
}

/*****************************************************************************/
//  Description : create bar layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void IStyleCreateTitleBarLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          icon_rect = menu_ctrl_ptr->default_data.bar_rect;
    UILAYER_CREATE_T    create_info = {0};

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = icon_rect.left;
    create_info.offset_y = icon_rect.top;
    create_info.width = (icon_rect.right + 1 - icon_rect.left);
    create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

    UILAYER_CreateLayer(&create_info, &(menu_ctrl_ptr->titleidx_layer_handle));

    UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->titleidx_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(&(menu_ctrl_ptr->titleidx_layer_handle));

}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleReleaseBarBgLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->titleidx_layer_handle);   /*lint !e506 !e774*/
    }

    return;
}

/*****************************************************************************/
//  Description : create bar layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void IStyleCreateBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          icon_rect = menu_ctrl_ptr->default_data.bar_rect;
    UILAYER_CREATE_T    create_info = {0};

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = icon_rect.left;
    create_info.offset_y = icon_rect.top;
    create_info.width = (icon_rect.right + 1 - icon_rect.left);
    create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

    UILAYER_CreateLayer(
        &create_info,
        &(menu_ctrl_ptr->bar_item_layer)
    );

    UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->bar_item_layer), TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(&(menu_ctrl_ptr->bar_item_layer));

}

/*****************************************************************************/
//  Description : create bar layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetIStyleBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (UILAYER_IsLayerActive(&menu_ctrl_ptr->bar_item_layer))
    {
        lcd_dev_info = menu_ctrl_ptr->bar_item_layer;
    }

    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleReleaseBarItemLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->bar_item_layer);   /*lint !e506 !e774*/
    }

    return;
}


/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note: 处理抖动timer
/*****************************************************************************/
LOCAL void IStyleDrawShaking(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    IStyleStopShakeIconTimer(menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_move_state)
    {
        IStyleStartShakeIconTimer(menu_ctrl_ptr);
    }

    s_shake_num += MENU_SHAKE_ANGLE / 2;

    if (menu_ctrl_ptr->is_tp_pressed)
    {
        return;
    }

    // 设置合并的层，这里只需要把第一个page的层合并即可
    if (menu_ctrl_ptr->is_move_state || menu_ctrl_ptr->is_moving)
    {
        SetMoveIconLayerPosition(menu_ctrl_ptr);

        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);

        if (menu_ctrl_ptr->is_folder_open)
        {
            lcd_dev_info = menu_ctrl_ptr->page_layer_handle[1];

            UILAYER_Clear(&lcd_dev_info);
            BltItemLayerToFolderLayer(menu_ctrl_ptr, lcd_dev_info);

            MMK_SetCtrlLcdDevInfo(menu_ctrl_ptr->ctrl_foldername_handle, &lcd_dev_info);
            GUIAPICTRL_HandleEvent(menu_ctrl_ptr->ctrl_foldername_handle, MSG_CTL_PAINT, 0);
        }
        else
        {
            lcd_dev_info = GetCurrentPageLyaerInfo(menu_ctrl_ptr);

            UILAYER_Clear(&lcd_dev_info);

            IStyleBltSlideItemLayerToMenuLayer(
                menu_ctrl_ptr,
                menu_ctrl_ptr->cur_page_index,
                lcd_dev_info,
                0,
                TRUE);
            IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
        }
    }

    return;
}


#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 重新加载主菜单
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void IStyleReload(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    BOOLEAN is_move_state = FALSE;
    BOOLEAN is_open_bar = FALSE;
    uint16 cur_item_idx = 0;
    uint16 cur_page_idx = 0;
    MMI_MENU_ID_T menu_id = 0;
    BOOLEAN is_folder_open = FALSE;

    is_move_state = menu_ctrl_ptr->is_move_state;
    cur_page_idx = menu_ctrl_ptr->cur_page_index;
    cur_item_idx = menu_ctrl_ptr->open_folder_index;
    is_open_bar = menu_ctrl_ptr->is_folder_open_from_bar;

    if (menu_ctrl_ptr->is_folder_open)
    {
        menu_id = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].menu_id;
        is_folder_open = TRUE;
        IStyleReleaseFolderLayer(menu_ctrl_ptr);
        IStyleDestroyFilefoldNameBox(menu_ctrl_ptr);
        menu_ctrl_ptr->is_folder_open = FALSE;
    }

    menu_ctrl_ptr->is_move_state = FALSE;

    // 释放
    IStyleResetBarIndexRect(&menu_ctrl_ptr->default_data, menu_ctrl_ptr->page_info.cnt);

    // 释放
    IStyleReleasePageLayer(menu_ctrl_ptr);

    // 释放
    IStyleReleaseBarBgLayer(menu_ctrl_ptr);

    // title图片显示位置
    IStyleReleaseBarItemLayer(menu_ctrl_ptr);

    IStyleInitDetail(menu_ctrl_ptr);

    menu_ctrl_ptr->is_move_state = is_move_state;

    //处理文件夹打开的情况，此处打开的文件夹可能已经被销毁。
    //如果有打开文件夹
    if (is_folder_open)
    {
        menu_ctrl_ptr->cur_page_index = cur_page_idx;
        menu_ctrl_ptr->cur_item_index = cur_item_idx;
        menu_ctrl_ptr->is_folder_open_from_bar = is_open_bar;

        IStyleOpenFolder(menu_ctrl_ptr);
    }
}

/*****************************************************************************/
// Description : 处理tpup消息
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void HandleDeleteTpUpMsg(
    GUI_POINT_T *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN is_pressed = FALSE;
    MMI_WIN_ID_T query_win_id = MAIN_MAINMENU_ISTYLE_QUERY_WIN_ID;
    MMI_MENU_ID_T menu_id = 0;

    if ((PNULL == tp_point_ptr) || (PNULL == menu_ctrl_ptr))
    {
        return;
    }

    IsInDyItemRect(tp_point_ptr, menu_ctrl_ptr, &menu_id, &is_pressed);

    if (is_pressed)
    {
        menu_ctrl_ptr->delete_menu_id = menu_id;
        MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY,
                                    IMAGE_PUBWIN_QUERY,
                                    &query_win_id,
                                    HandleQueryWinMsg);
    }

    return;
}

/*****************************************************************************/
// Description : 判断tp是否在动态item区域内,返回item所在区域类型
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void IsInDyItemRect(
    GUI_POINT_T *tp_point_ptr,
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    MMI_MENU_ID_T *menu_id_ptr,
    BOOLEAN *is_pressed_ptr
)
{
    uint16 img_height = 0;
    uint16 img_width = 0;
    GUI_RECT_T delete_icon_rect = {0};

    if ((PNULL == tp_point_ptr) || (PNULL == menu_ctrl_ptr) || (PNULL == menu_id_ptr))
    {
        return;
    }

    GUIRES_GetImgWidthHeight(&img_width, &img_height,
                             IMAGE_ISTYLEMENU_DELETE_ICON,
                             menu_ctrl_ptr->win_handle);

    if (!menu_ctrl_ptr->is_move_state || !menu_ctrl_ptr->is_tp_pressed
        || menu_ctrl_ptr->is_moving)

    {
        return;
    }

    //点击文件夹内部item
    if (menu_ctrl_ptr->is_folder_open)
    {
        uint16 j = 0;

        for (j = 0; j < menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt; j++)
        {
            MMIMAINMENU_ITEM_INFO_T *cur_item_info_ptr = &(menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item[j]);
            GUI_RECT_T disp_rect = {0};

            if (PNULL == cur_item_info_ptr
                || DYNAMIC_MAINMENU_DEFAULT_GROUP_ID != cur_item_info_ptr->group_id)
            {
                continue;
            }

            // 获取图层位置
            GetFolderItemRect(menu_ctrl_ptr, j, &disp_rect);

            delete_icon_rect.top = disp_rect.top;
            delete_icon_rect.left = disp_rect.left;
            delete_icon_rect.right = disp_rect.left + img_width;
            delete_icon_rect.bottom = disp_rect.top + img_height;

            //点击的删除区域
            if (GUI_PointIsInRect(*tp_point_ptr, delete_icon_rect))
            {
                *is_pressed_ptr = TRUE;
                *menu_id_ptr = cur_item_info_ptr->menu_id;
                break;
            }
        }
    }
    else//非文件夹item
    {
        uint16 i = 0;

        if (!GUI_PointIsInRect(*tp_point_ptr, menu_ctrl_ptr->default_data.bar_rect))
        {
            delete_icon_rect =
                menu_ctrl_ptr->default_data.item_rect[menu_ctrl_ptr->cur_item_index];

            delete_icon_rect.right = delete_icon_rect.left + img_width;
            delete_icon_rect.bottom = delete_icon_rect.top + img_height;

            //点击的删除区域
            if (GUI_PointIsInRect(*tp_point_ptr, delete_icon_rect)) //屏幕中普通item
            {
                if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item[menu_ctrl_ptr->cur_item_index].group_id)
                {
                    *is_pressed_ptr = TRUE;
                    *menu_id_ptr = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item[menu_ctrl_ptr->cur_item_index].menu_id;
                    return;
                }
            }
        }
        else
        {
            uint16 item_cnt = menu_ctrl_ptr->bar_info.cnt;

            if (item_cnt == 0)
            {
                return;
            }

            //bar上的item
            for (i = 0; i < item_cnt; i++)
            {
                MMIMAINMENU_ITEM_INFO_T item_info = menu_ctrl_ptr->bar_info.item[i];

                if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID != item_info.group_id)
                {
                    continue;
                }

                delete_icon_rect =
                    menu_ctrl_ptr->default_data.bar_item_rect[menu_ctrl_ptr->cur_item_index];

                delete_icon_rect.right = delete_icon_rect.left + img_width;
                delete_icon_rect.bottom = delete_icon_rect.top + img_height;

                //点击的删除区域
                if (GUI_PointIsInRect(*tp_point_ptr, delete_icon_rect))
                {
                    *is_pressed_ptr = TRUE;
                    *menu_id_ptr = item_info.menu_id;
                    return;
                }
            }
        }
    }

    return;
}
/*****************************************************************************/
//  Description : to handle the asp message list of delete query window
//  Global resource dependence :
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T  query_win_id = MAIN_MAINMENU_ISTYLE_QUERY_WIN_ID;

    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:

            MMIPUB_CloseQuerytWin(&query_win_id);
#if defined DYNAMIC_MAINMENU_SUPPORT
			DeleteDynamicItem();
#endif
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}
/*****************************************************************************/
//  Description : 删除动态菜单项
//  Global resource dependence : 
//  Author: jin.wang  add by guodong.liu
//  Note:
/*****************************************************************************/
LOCAL void DeleteDynamicItem(void)
{
    MMIMAINMENU_ISTYLE_DATA_T*menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T*)GUIMAINMENU_GetPM(MAINMENUWIN_MENU_CTRL_ID);

    if (PNULL != menu_ctrl_ptr)
    {
        CTRLMAINMENU_ITEM_T menuitem_info = {0};

        MMIMAINMENU_GetItemData(
            menu_ctrl_ptr->delete_menu_id,
            DYNAMIC_MAINMENU_DEFAULT_GROUP_ID,
            &menuitem_info);

        //判断此要删除的item信息是否正确
        if (0 != menuitem_info.group_id
                && PNULL != menuitem_info.call_back_delete)
        {
            GUIMAINMENU_DY_ITEM_T item_info = {0};
            BOOLEAN is_info_get = FALSE;
            BOOLEAN is_delete = FALSE;

            is_info_get = MMIMENU_DySlideGetRunInfo(menu_ctrl_ptr->delete_menu_id, &item_info);

            if (is_info_get
                    && PNULL != item_info.ori_data_ptr
                    && MMIMENU_DyIsRunInfoValid(&(item_info.ori_data_ptr->dynamic_menu_info)))
            {
                //运行删除应用的回调
                is_delete = (menuitem_info.call_back_delete)(
                                (uint32)item_info.ori_data_ptr->dynamic_menu_info.param1ptr,
                                (uint32)item_info.ori_data_ptr->dynamic_menu_info.param2ptr
                            );
            }

            MMIMENU_DyFreeParmInfo(&item_info);

            //if (is_delete)
            {
                menu_ctrl_ptr->is_delete_state = TRUE;
                MMIMENU_DyDeleteItemEx(&menuitem_info);
            }
        }
    }
}
#endif  // end of DYNAMIC_MAINMENU_SUPPORT


LOCAL uint32 GenMenuId(void)
{
    return SCI_GetTickCount();
}

/*****************************************************************************/
//  Description : 创建文件夹的编辑框
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL void CreateFilefoldEditNameBox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    GUIEDIT_INIT_DATA_T     edit_init = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUI_BOTH_RECT_T         edit_rect = {0};
    GUI_BOTH_RECT_T         client_rect = {0};
    MMI_HANDLE_T ctrl_handle = 0;
    MMI_STRING_T text = {0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_BG_T        edit_bg = {GUI_BG_COLOR, GUI_SHAPE_SROUNDED_RECT, IMAGE_NULL, MMI_WHITE_COLOR, FALSE};
    GUI_BORDER_T    edit_border = {0, MMI_BACKGROUND_COLOR, GUI_BORDER_NOT_DISPLAY};
    GUI_COLOR_T     edit_font_color = MMI_BACKGROUND_COLOR;
    IGUICTRL_T     *edit_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr || !(menu_ctrl_ptr->is_folder_open))
    {
        return;
    }

    if (menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len)
    {
        text.wstr_ptr = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text;
        text.wstr_len = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len;
    }
    else
    {
        //get item string
        MMI_GetLabelTextByLang(TXT_FILEFOLD, &text);
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    client_rect.v_rect.top =  menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM].top - MAINMENU_FILEFOLD_NAME_RECT_HEIGHT;

    edit_rect.v_rect.left = MAINMENU_FILEFOLD_NAME_SIDE_MARGIN;
    edit_rect.v_rect.top = client_rect.v_rect.top;
    edit_rect.v_rect.right = lcd_width - MAINMENU_FILEFOLD_NAME_SIDE_MARGIN;
    edit_rect.v_rect.bottom = edit_rect.v_rect.top + MAINMENU_FILEFOLD_NAME_RECT_HEIGHT;

    edit_init.both_rect = edit_rect;

    edit_init.type        = GUIEDIT_TYPE_TEXT;
    edit_init.str_max_len = GUIMENU_DYMAINMENU_STR_MAX_NUM;

    //create phone number edit control
    create.guid              = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = menu_ctrl_ptr->win_handle;
    create.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
    create.init_data_ptr     = &edit_init;

    edit_ptr = MMK_CreateControl(&create);

    if (PNULL != edit_ptr)
    {
        ctrl_handle = GUICTRL_GetCtrlHandle(edit_ptr);

        //set edit font
        GUIEDIT_SetFont(ctrl_handle, MAINMENU_FILEFOLD_NAME_FONT);
        GUIEDIT_SetStyle(ctrl_handle, GUIEDIT_STYLE_SINGLE);

        //set border & font color & bg
        GUIEDIT_SetBg(ctrl_handle, &edit_bg);
        GUIEDIT_SetBorder(ctrl_handle, &edit_border);
        GUIEDIT_SetFontColor(ctrl_handle, edit_font_color);
        GUIEDIT_SetString(ctrl_handle, text.wstr_ptr, text.wstr_len);

        MMK_SetCtrlLcdDevInfo(ctrl_handle, &menu_ctrl_ptr->page_layer_handle[1]);
        GUIAPICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        GUIAPICTRL_HandleEvent(ctrl_handle, MSG_CTL_PAINT, 0);

        menu_ctrl_ptr->ctrl_foldername_handle = ctrl_handle;
    }
}

/*****************************************************************************/
//  Description : 创建文件夹的名字显示
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL void CreateFilefoldTextNameBox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    GUI_BOTH_RECT_T   text_rect = {0};
    MMI_HANDLE_T ctrl_handle = 0;
    MMI_STRING_T text = {0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_CONTROL_CREATE_T create = {0};
    GUITEXT_INIT_DATA_T  init_data = {0};
    GUI_BG_T             bg = {0};
    GUI_FONT_T font = MAINMENU_FILEFOLD_NAME_FONT;
    GUI_COLOR_T color = MMI_WHITE_COLOR;
    IGUICTRL_T *text_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr || !(menu_ctrl_ptr->is_folder_open))
    {
        return;
    }

    if (menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len)
    {
        text.wstr_ptr = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text;
        text.wstr_len = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len;
    }
    else
    {
        //get item string
        MMI_GetLabelTextByLang(TXT_FILEFOLD, &text);
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    text_rect.v_rect.left = MAINMENU_FILEFOLD_NAME_SIDE_MARGIN;
    text_rect.v_rect.top = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM].top - MAINMENU_FILEFOLD_NAME_RECT_HEIGHT;
    text_rect.v_rect.right = lcd_width - MAINMENU_FILEFOLD_NAME_SIDE_MARGIN;
    text_rect.v_rect.bottom = text_rect.v_rect.top + MAINMENU_FILEFOLD_NAME_RECT_HEIGHT;
    init_data.both_rect = text_rect;

    //create.ctrl_id = text_ctrl_id;
    create.guid    = SPRD_GUI_TEXTBOX_ID;
    create.parent_win_handle = menu_ctrl_ptr->win_handle;
    create.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
    create.init_data_ptr = &init_data;

    text_ptr = MMK_CreateControl(&create);

    if (PNULL != text_ptr)
    {
        ctrl_handle = GUICTRL_GetCtrlHandle(text_ptr);

        GUITEXT_SetBg(ctrl_handle, &bg);
        GUITEXT_IsDisplayPrg(FALSE, ctrl_handle);
        GUITEXT_SetFont(ctrl_handle, &font, &color);
        GUITEXT_SetMargin(ctrl_handle, MAINMENU_FILEFOLD_NAME_SIDE_MARGIN);
        GUITEXT_SetLineSpace(ctrl_handle, 0);
        GUITEXT_SetString(ctrl_handle, text.wstr_ptr, text.wstr_len, FALSE);

        MMK_SetCtrlLcdDevInfo(ctrl_handle, &menu_ctrl_ptr->page_layer_handle[1]);
        GUIAPICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        GUIAPICTRL_HandleEvent(ctrl_handle, MSG_CTL_PAINT, 0);

        menu_ctrl_ptr->ctrl_foldername_handle = ctrl_handle;
    }
}

/*****************************************************************************/
//  Description : 销毁文件夹的名字框
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL void IStyleDestroyFilefoldNameBox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    MMK_DestroyControl(menu_ctrl_ptr->ctrl_foldername_handle);
    menu_ctrl_ptr->ctrl_foldername_handle = 0;
}

/*****************************************************************************/
//  Description : 保存文件夹的名字
//  Global resource dependence : none
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL void IStyleGetFilefoldNameFromEditbox(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    MMI_STRING_T  str_info = {0};

    if (0 == menu_ctrl_ptr->ctrl_foldername_handle)
    {
        return;
    }

    if (!(menu_ctrl_ptr->is_folder_open))
    {
        return;
    }

    GUIEDIT_GetString(menu_ctrl_ptr->ctrl_foldername_handle, &str_info);

    menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len = MIN(str_info.wstr_len, GUIMENU_DYMAINMENU_STR_MAX_NUM);
    MMIAPICOM_Wstrncpy(
        menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text,
        str_info.wstr_ptr,
        menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len);
    menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text[menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].text_len] = 0;

    return;
}
/*****************************************************************************/
//  Description : 获得编辑框的区域
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetEditRect(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,  //in
    GUI_RECT_T       *rect_ptr        //out
)
{
    GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_foldername_handle, rect_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : get  filefold rect
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleGetFolderRect(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr, //in
    GUI_RECT_T *rect_ptr //out
)
{
    uint32 item_count = 0;

    if (PNULL == menu_ctrl_ptr || PNULL == rect_ptr || !(menu_ctrl_ptr->is_folder_open))
    {
        return FALSE;
    }

    item_count = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt;

    // 获得menu区域
    CTRLMAINMENU_GetRect(menu_ctrl_ptr->ctrl_handle, rect_ptr);

    rect_ptr->top = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM].top - 35;

    if (item_count <= MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM)//一行
    {
        rect_ptr->bottom = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM].bottom;/*lint !e415*/
    }
    else if (item_count <= MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM * 2)//两行
    {
        rect_ptr->bottom = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM * 2].bottom;/*lint !e415*/
    }
    else//三行
    {
        rect_ptr->bottom = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM * 3].bottom;/*lint !e415*/
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : close  filefold
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IStyleCloseFilefolder(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (!menu_ctrl_ptr->is_folder_open)
    {
        return FALSE;
    }

    IStyleReleaseFolderLayer(menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_move_state)
    {
        IStyleGetFilefoldNameFromEditbox(menu_ctrl_ptr);
    }

    IStyleDestroyFilefoldNameBox(menu_ctrl_ptr);

    IStyleUpdateFolderThumbnail(menu_ctrl_ptr, menu_ctrl_ptr->open_folder_page_index, menu_ctrl_ptr->open_folder_item_index, menu_ctrl_ptr->is_folder_open_from_bar);
    
	menu_ctrl_ptr->is_folder_open = FALSE;

    IStyleDraw(menu_ctrl_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : update file folder Thumbnail
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL void IStyleUpdateFolderThumbnail(
    MMIMAINMENU_ISTYLE_DATA_T       *menu_ctrl_ptr,
    uint16                page_index,
    uint16                item_index,
    BOOLEAN               is_bar_item
)
{
    MMIMAINMENU_ITEM_INFO_T item_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (is_bar_item)
    {
        item_info = menu_ctrl_ptr->bar_info.item[item_index];
    }
    else
    {
        item_info = menu_ctrl_ptr->page_info.page[page_index].item[item_index];
    }

    MMITHEME_DeleteMenuItem(item_info.menu_id);

    return;
}


LOCAL BOOLEAN IsCurrentIndexFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint32         page_index,
    uint16         item_index,
    BOOLEAN     is_bar_item
)
{
    MMIMAINMENU_ITEM_INFO_T item_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);
	if (PNULL != menu_ctrl_ptr)
	{    
		if (is_bar_item)
		{
			item_info = menu_ctrl_ptr->bar_info.item[item_index];
		}
		else
		{
			item_info = menu_ctrl_ptr->page_info.page[page_index].item[item_index];
		}
		
		return (MAINMENU_FILEFOLD_DEFAULT_GROUP_ID == item_info.group_id);
	}
	return FALSE;
}


/* 临时文件夹只显示*/
LOCAL BOOLEAN CreateIStyleFilefoldTemp(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    uint32 new_folder_idx = 0;
    ISTYLE_FOLDER_INFO_T *new_folder = PNULL;
    MMIMAINMENU_ITEM_INFO_T item_folder = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->folder_info.cnt >= ISTYLE_MAX_FILEFOLD_NUM)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->tmp_cur_item_index == menu_ctrl_ptr->cur_item_index
        && menu_ctrl_ptr->tmp_cur_page_index == menu_ctrl_ptr->cur_page_index)
    {
        return FALSE;
    }

    new_folder_idx = menu_ctrl_ptr->folder_info.cnt;
    new_folder = &(menu_ctrl_ptr->folder_info.folder[new_folder_idx]);
    menu_ctrl_ptr->folder_info.cnt += 1;

    new_folder->cnt = 0;
    new_folder->menu_id = GenMenuId();
    new_folder->text_len = 0;

    //cur item: 1 add item to folder, 2 delete from page
    AppendItemToFolder(menu_ctrl_ptr,
                       &(menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item[menu_ctrl_ptr->cur_item_index]),
                       new_folder_idx);

    DeleteItemFromPage(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index);

    //new folder: insert folder to page
    item_folder.group_id = MAINMENU_FILEFOLD_DEFAULT_GROUP_ID;
    item_folder.menu_id = new_folder->menu_id;
    InsertItemToPage(menu_ctrl_ptr, &item_folder, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index);
#if 0
    //tmp item: 1 insert to folder, 2 delete item from page
    AppendItemToFolder(menu_ctrl_ptr,
                       &(menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->tmp_cur_page_index].item[menu_ctrl_ptr->tmp_cur_item_index]),
                       new_folder_idx);

    DeleteItemFromPage(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_page_index, menu_ctrl_ptr->tmp_cur_item_index);
#endif
    //draw
    IStyleDrawItem(
        menu_ctrl_ptr,
        &menu_ctrl_ptr->page_layer_handle[0],
        &menu_ctrl_ptr->default_data.item_rect[menu_ctrl_ptr->cur_item_index],
        menu_ctrl_ptr->cur_page_index,
        menu_ctrl_ptr->cur_item_index,
        GUIMENU_ITEM_IN_PAGE
    );

    return TRUE;
}


LOCAL BOOLEAN DestroyIStyleFilefoldTemp(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    uint32 folder_idx = 0;

    if (PNULL == menu_ctrl_ptr || -1 == menu_ctrl_ptr->temp_filefold_index)
    {
        return FALSE;
    }


    folder_idx = FindFolderIndex(
                     menu_ctrl_ptr,
                     menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item[menu_ctrl_ptr->temp_filefold_index].menu_id
                 );

    if (FIND_NONE == folder_idx)
    {
        return FALSE;
    }

    // delete folder item from page
    DeleteItemFromPage(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index);

    InsertItemToPage(menu_ctrl_ptr, &(menu_ctrl_ptr->folder_info.folder[folder_idx].item[0]), menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index);

    // delete folder
    DeleteFolder(menu_ctrl_ptr, folder_idx);

    //draw
    IStyleDrawItem(menu_ctrl_ptr,
                   &menu_ctrl_ptr->page_layer_handle[0],
                   &menu_ctrl_ptr->default_data.item_rect[menu_ctrl_ptr->temp_filefold_index],
                   menu_ctrl_ptr->cur_page_index,
                   menu_ctrl_ptr->temp_filefold_index,
                   GUIMENU_ITEM_IN_PAGE);

    return TRUE;
}

/*****************************************************************************/
//  Description : create folder layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void IStyleCreateFolderLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          page_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    IStyleGetFolderRect(menu_ctrl_ptr, &page_rect);

    menu_ctrl_ptr->page_layer_handle[1].lcd_id = 0;
    menu_ctrl_ptr->page_layer_handle[1].block_id = UILAYER_NULL_HANDLE;

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = page_rect.left;
    create_info.offset_y = page_rect.top;
    create_info.width = (page_rect.right + 1 - page_rect.left);
    create_info.height = (page_rect.bottom + 1 - page_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;

    UILAYER_CreateLayer(
        &create_info,
        &menu_ctrl_ptr->page_layer_handle[1]
    );

    // 设置color key
    UILAYER_SetLayerColorKey(&menu_ctrl_ptr->page_layer_handle[1], TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[1]);
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void IStyleReleaseFolderLayer(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->page_layer_handle[1]);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void BltItemLayerToFolderLayer(
    MMIMAINMENU_ISTYLE_DATA_T           *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO          lcd_dev_info
)
{
    uint16              i = 0;
    uint16              page_item_num = 0;
    GUI_RECT_T          disp_rect = {0};
    GUIMENU_ITEM_BUF_T  shake_item_layer = {0};    //抖动的临时buffer
    GUI_RECT_T shake_rect   = {0};    //shake layer rect
    BOOLEAN is_need_shake = FALSE;
    BOOLEAN is_get_shake_layer = FALSE;

    shake_item_layer.icon_layer.block_id = UILAYER_NULL_HANDLE;

    if (PNULL == menu_ctrl_ptr || !(menu_ctrl_ptr->is_folder_open))
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    page_item_num = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt;

    is_need_shake = (BOOLEAN)(menu_ctrl_ptr->is_move_state && !menu_ctrl_ptr->is_moving);  //抖动时拖动item, 性能不足

    if (is_need_shake)
    {
        shake_rect.right = MMITHEME_MENU_ITEM_WIDTH - 1;
        shake_rect.bottom = MMITHEME_MENU_ITEM_HEIGHT - 1;

        CreateIStyleMenuItemLayer(menu_ctrl_ptr, shake_rect, &shake_item_layer);

        is_get_shake_layer = UILAYER_IsLayerActive(&shake_item_layer.icon_layer);
    }

    for (i = 0; i < page_item_num; i++)
    {
        //不画current item
        if (i == menu_ctrl_ptr->tmp_cur_item_index
            && menu_ctrl_ptr->is_moving)
        {
            continue;
        }

        GetFolderItemRect(menu_ctrl_ptr, i, &disp_rect);

        if (is_get_shake_layer)
        {
            GUI_LCD_DEV_INFO layer_handle = IStyleDrawItem(menu_ctrl_ptr, PNULL, PNULL, menu_ctrl_ptr->open_folder_index, i, GUIMENU_ITEM_IN_FOLDER);

            ShakeItemLayer(menu_ctrl_ptr, &layer_handle, &shake_item_layer.icon_layer, i, FALSE);
            UILAYER_SetLayerPosition(&shake_item_layer.icon_layer, disp_rect.left, disp_rect.top);
            UILAYER_BltLayerToLayer(&lcd_dev_info, &shake_item_layer.icon_layer, &disp_rect, FALSE);
        }
        else
        {
            IStyleDrawItem(menu_ctrl_ptr, &lcd_dev_info, &disp_rect, menu_ctrl_ptr->open_folder_index, i, GUIMENU_ITEM_IN_FOLDER);
        }
    }

    DestoryIStyleItemLayer(&shake_item_layer);

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayFolderBg(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO lcd_dev_info
)
{
    uint16              page_item_num = 0;
    GUI_RECT_T          disp_rect = {0};

    if (PNULL == menu_ctrl_ptr
        || !(menu_ctrl_ptr->is_folder_open))
    {
        return;
    }

    CTRLMAINMENU_GetRect(menu_ctrl_ptr->ctrl_handle, &disp_rect);

    page_item_num = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].cnt;

    disp_rect.top = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM].top - MAINMENU_FILEFOLD_NAME_RECT_HEIGHT;

    //一行
    if (page_item_num <= MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM)
    {
        disp_rect.bottom = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM].bottom;/*lint !e415*/
    }
    //两行
    else if (page_item_num <= MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM * 2)
    {
        disp_rect.bottom = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM * 2].bottom;/*lint !e415*/
    }
    //三行
    else
    {
        disp_rect.bottom = menu_ctrl_ptr->default_data.item_rect[MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM * 3].bottom;/*lint !e415*/
    }

    IMG_EnableTransparentColor(TRUE);

    disp_rect.top -= MENUTEXT_HEIGHT;
    disp_rect.bottom += MENUTEXT_HEIGHT;

    //display icon
    GUIRES_DisplayImg(PNULL,
                      &disp_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      menu_ctrl_ptr->default_data.folder_bg_id,
                      &lcd_dev_info);

    IMG_EnableTransparentColor(FALSE);
}

LOCAL void IStyleOpenFolder(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    MMIMAINMENU_ITEM_INFO_T *folder_item_ptr = PNULL;
    uint32 folder_index = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (menu_ctrl_ptr->is_bar_item)
    {
        folder_item_ptr = &(menu_ctrl_ptr->bar_info.item[menu_ctrl_ptr->cur_item_index]);
    }
    else
    {
        folder_item_ptr = &(menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item[menu_ctrl_ptr->cur_item_index]);
    }

    folder_index = FindFolderIndex(menu_ctrl_ptr, folder_item_ptr->menu_id);

    if (FIND_NONE == folder_index)
    {
        return;
    }

    menu_ctrl_ptr->is_folder_open = TRUE;
    menu_ctrl_ptr->is_folder_open_from_bar = menu_ctrl_ptr->is_bar_item;
    menu_ctrl_ptr->open_folder_page_index = menu_ctrl_ptr->cur_page_index;
    menu_ctrl_ptr->open_folder_item_index = menu_ctrl_ptr->cur_item_index;
    menu_ctrl_ptr->open_folder_index = folder_index;

    IStyleCreateFolderLayer(menu_ctrl_ptr);

    IStyleDraw(menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_move_state)
    {
        CreateFilefoldEditNameBox(menu_ctrl_ptr);
    }
    else
    {
        CreateFilefoldTextNameBox(menu_ctrl_ptr);
    }

#ifdef UI_SUPPORT_U_MAINMENU
    MMIISTYLE_U_ReleaseAllItemLayer();
#endif
}

/*****************************************************************************/
//  Description : move menu item into file folder
//  Global resource dependence :
//  Note:  hermann liu
/*****************************************************************************/
LOCAL BOOLEAN  AddItemToFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    BOOLEAN  *is_filefold_first_create
)
{
    BOOLEAN       result = FALSE;
    int32       i = 0;
    uint16      item_index = 0;
    uint16      page_item_num = 0;
    int16       center_x = 0;
    GUI_RECT_T  item_rect = {0};
    BOOLEAN     is_bar_item = FALSE;
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    BOOLEAN     is_find = FALSE;
    BOOLEAN     is_find_in_bar = FALSE;

    if (menu_ctrl_ptr->is_folder_open)
    {
        return FALSE;
    }

    if (!menu_ctrl_ptr->is_move_state)
    {
        return FALSE;
    }

    MMK_GetLogicTPMsg(&state, &point);

    if (GUI_PointIsInRect(point, menu_ctrl_ptr->default_data.bar_rect))
    {
        //get item total number
        page_item_num = menu_ctrl_ptr->bar_info.cnt;

        item_index = page_item_num;

        if (0 != page_item_num)
        {
            GUI_RECT_T bar_item_rect[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = {0};

            GetBarItemRect(menu_ctrl_ptr, bar_item_rect, page_item_num);

            //bar内部
            if (menu_ctrl_ptr->is_bar_item)
            {
                //向右
                if (point.x > menu_ctrl_ptr->slide_start_point.x)
                {
                    for (i = page_item_num - 1; i >= 0; i--)
                    {
                        item_rect = bar_item_rect[i];

                        if (point.x > item_rect.left)
                        {
                            if (point.x > (item_rect.left + (item_rect.right - item_rect.left) / 4)
                                && point.x < (item_rect.left + (item_rect.right - item_rect.left) * 3 / 4)
                               )
                            {
                                is_find_in_bar = TRUE;
                            }

                            item_index = i;
                            break;
                        }
                    }
                }
                //向左
                else
                {
                    for (i = 0; i < page_item_num; i++)
                    {
                        item_rect = bar_item_rect[i];

                        if (point.x < item_rect.right)
                        {
                            item_index = i;

                            if (point.x < (item_rect.right - (item_rect.right - item_rect.left) / 4)
                                && point.x > (item_rect.right - (item_rect.right - item_rect.left) * 3 / 4)
                               )
                            {
                                is_find_in_bar = TRUE;
                            }

                            break;
                        }
                    }
                }
            }
            else
            {
                for (i = 0; i < page_item_num; i++)
                {
                    item_rect = bar_item_rect[i];

                    center_x = (item_rect.left + item_rect.right) / 2;

                    if (point.x < center_x)
                    {
                        item_index = i;

                        if (point.x > (center_x - (item_rect.right - item_rect.left) / 4)
                            && point.x < (center_x + (item_rect.right - item_rect.left) * 1 / 4)
                           )
                        {
                            is_find_in_bar = TRUE;
                        }

                        break;
                    }
                }
            }
        }

        is_bar_item = TRUE;
    }
    else
    {
        //get item total number
        page_item_num = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].cnt;

        item_index = page_item_num;

        for (i = 0; i < page_item_num; i++)
        {
            item_rect = menu_ctrl_ptr->default_data.item_rect[i];

            if (GUI_PointIsInRect(point, item_rect))
            {
                is_find = TRUE;
                item_index = i;
                break;
            }
        }
    }

    if ((item_index != menu_ctrl_ptr->cur_item_index || is_bar_item != menu_ctrl_ptr->tmp_is_bar_item)
        && (is_find || is_find_in_bar)
        && menu_ctrl_ptr->is_moving
       )
    {

        if ((!IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_page_index, menu_ctrl_ptr->tmp_cur_item_index, menu_ctrl_ptr->tmp_is_bar_item))
            && (IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, item_index, is_bar_item))
           )
        {
            uint32 new_folder_idx = 0;

            if ((menu_ctrl_ptr->is_temp_filefold_create)
                && menu_ctrl_ptr->temp_filefold_index != item_index
                && (IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index, FALSE))
               )
            {
                DestroyIStyleFilefoldTemp(menu_ctrl_ptr);
            }

            new_folder_idx = AddItemNodeToFilefoldNode(menu_ctrl_ptr, item_index, is_bar_item);

            if (FIND_NONE == new_folder_idx)
            {
                result = FALSE;
            }
            else
            {
                if (2 == menu_ctrl_ptr->folder_info.folder[new_folder_idx].cnt)
                {
                    *is_filefold_first_create = TRUE;
                }

                menu_ctrl_ptr->tmp_cur_page_index = menu_ctrl_ptr->cur_page_index;

                if (menu_ctrl_ptr->tmp_is_bar_item != is_bar_item)
                {
                    menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index = item_index;
                }
                else
                {
                    if (menu_ctrl_ptr->tmp_cur_item_index > item_index)
                    {
                        menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index = item_index;
                    }
                    else
                    {
                        menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index = item_index - 1;
                    }
                }

                menu_ctrl_ptr->tmp_is_bar_item = menu_ctrl_ptr->is_bar_item = is_bar_item;
                result = TRUE;
            }
        }
        else
        {
            result = FALSE;
        }
    }

    if (!result)
    {
        if ((menu_ctrl_ptr->is_temp_filefold_create)
            && (IsCurrentIndexFilefold(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->temp_filefold_index, FALSE))
           )
        {
            DestroyIStyleFilefoldTemp(menu_ctrl_ptr);
        }
    }

    return result;
}


LOCAL uint32 AddItemNodeToFilefoldNode(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint16        item_index,     //2 当前的index
    BOOLEAN       is_bar_item
)
{
    MMIMAINMENU_ITEM_INFO_T folder_item = {0};
    uint32 folder_idx = 0;
    MMIMAINMENU_ITEM_INFO_T item_tmp = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return PNULL;
    }

    if (!menu_ctrl_ptr->is_move_state)
    {
        return PNULL;
    }

    //cur - 1 find folder, 2 test folder - can insert?
    if (is_bar_item)
    {
        folder_item = menu_ctrl_ptr->bar_info.item[item_index];
    }
    else
    {
        folder_item = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->cur_page_index].item[item_index];
    }

    folder_idx = FindFolderIndex(menu_ctrl_ptr, folder_item.menu_id);

    if (FIND_NONE == folder_idx
        || menu_ctrl_ptr->folder_info.folder[folder_idx].cnt >= ISTYLE_FOLDER_ITEM_CNT_MAX)
    {
        return PNULL;
    }

    //tmp - get item, and delete item
    if (menu_ctrl_ptr->tmp_is_bar_item)
    {
        item_tmp = menu_ctrl_ptr->bar_info.item[menu_ctrl_ptr->tmp_cur_item_index];
        DeleteItemFromBar(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index);
    }
    else
    {
        item_tmp = menu_ctrl_ptr->page_info.page[menu_ctrl_ptr->tmp_cur_page_index].item[menu_ctrl_ptr->tmp_cur_item_index];
        DeleteItemFromPage(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_page_index, menu_ctrl_ptr->tmp_cur_item_index);
    }

    //cur - 1 add item to folder
    AppendItemToFolder(menu_ctrl_ptr, &item_tmp, folder_idx);

    return folder_idx;
}

/*****************************************************************************/
//  Description : remove item from the folder which is opened.
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveItemFromFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint16 *dest_page_index_ptr
)
{
    BOOLEAN is_inserted = FALSE;
    uint16 page_index = 0;

    if (PNULL == menu_ctrl_ptr || PNULL == dest_page_index_ptr)
    {
        return FALSE;
    }

    if (!menu_ctrl_ptr->is_moving || !menu_ctrl_ptr->is_move_state || !menu_ctrl_ptr->is_folder_open)
    {
        return FALSE;
    }

    page_index = *dest_page_index_ptr;

    do
    {
        if (page_index >= menu_ctrl_ptr->page_info.page[page_index].cnt)
        {
            is_inserted = AddItemToPage(
                              menu_ctrl_ptr,
                              &(menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item[menu_ctrl_ptr->tmp_cur_item_index])
                          );

			if (is_inserted)
			{
				FindItemFromPage(menu_ctrl_ptr,
					menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item[menu_ctrl_ptr->tmp_cur_item_index].menu_id,
					dest_page_index_ptr, PNULL);
			}

			break;
        }

        if (menu_ctrl_ptr->page_info.page[page_index].cnt < ISTYLE_PAGE_ITEM_CNT_MAX)
        {
            is_inserted = AppendItemToPage(
                              menu_ctrl_ptr,
                              &(menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item[menu_ctrl_ptr->tmp_cur_item_index]),
                              page_index
                          );
			break;
        }

		page_index++;
    }
    while (page_index != *dest_page_index_ptr);

    if (!is_inserted)
    {
        return FALSE;
    }

    DeleteItemFromFolder(menu_ctrl_ptr, menu_ctrl_ptr->open_folder_index, menu_ctrl_ptr->tmp_cur_item_index);

    return TRUE;
}



/*****************************************************************************/
//  Description : 当 文件夹里还有一个item时销毁文件夹.
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Destroy1ItemFilefold(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    MMIMAINMENU_ITEM_INFO_T item = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    IStyleReleaseFolderLayer(menu_ctrl_ptr);

    IStyleDestroyFilefoldNameBox(menu_ctrl_ptr);

    item = menu_ctrl_ptr->folder_info.folder[menu_ctrl_ptr->open_folder_index].item[0];

    if (menu_ctrl_ptr->is_folder_open_from_bar)
    {
        DeleteItemFromBar(menu_ctrl_ptr, menu_ctrl_ptr->open_folder_item_index);
        InsertItemToBar(menu_ctrl_ptr, &item, menu_ctrl_ptr->open_folder_item_index);
    }
    else
    {
        DeleteItemFromPage(menu_ctrl_ptr, menu_ctrl_ptr->open_folder_page_index, menu_ctrl_ptr->open_folder_item_index);
        InsertItemToPage(menu_ctrl_ptr, &item, menu_ctrl_ptr->open_folder_page_index, menu_ctrl_ptr->open_folder_item_index);
    }

    menu_ctrl_ptr->is_folder_open = FALSE;
    DeleteFolder(menu_ctrl_ptr, menu_ctrl_ptr->open_folder_index);
    IStyleDraw(menu_ctrl_ptr);

    return TRUE;

}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void HandleSearchSlideOver(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)
{
    int32   offset_x        = 0;
    int16   move_space      = 0; // 位移
    int16   lcd_pos         = 0; // lcd位置
    uint16  frame_num       = 8; // 移动频率
    uint16  cur_frame       = 1; // 计数
    //uint16  pre_page_index  = 0; // 记录移动之前的页
    uint16  lcd_width       = 0; // lcd宽度
    uint16  lcd_height      = 0; // lcd高度
#ifdef UI_SUPPORT_U_MAINMENU
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    BOOLEAN is_change_page  = FALSE;
    GUI_POINT_T tmp_point   = {0};
#endif
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    RecoverSearchLayer(menu_ctrl_ptr);

    move_space = tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x;

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    lcd_pos = ((int16)lcd_width - 1);

    if (0 != move_space)
    {
        for (; cur_frame <= frame_num; cur_frame++)
        {
            if (move_space > 0)
            {
                if (menu_ctrl_ptr->is_search_entry) // 下一页 menu_ctrl_ptr->slide_start_point.x + lcd_pos
                {
                    offset_x = move_space + MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, lcd_pos - move_space, frame_num, cur_frame);
#ifdef UI_SUPPORT_U_MAINMENU
                    direction = GO_MOVE_RIGHT;
                    is_change_page = TRUE;
                    tmp_point.x = menu_ctrl_ptr->slide_start_point.x + (offset_x);
#endif
                }
                else // 恢复到 menu_ctrl_ptr->slide_start_point.x
                {
                    offset_x = move_space - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);
#ifdef UI_SUPPORT_U_MAINMENU
                    direction = GO_MOVE_RIGHT;
                    tmp_point.x = tp_point_ptr->x + (offset_x - move_space);
#endif
                }
            }
            else
            {
                if (!menu_ctrl_ptr->is_search_entry) // 下一页 menu_ctrl_ptr->slide_start_point.x - lcd_pos
                {
                    offset_x = (lcd_pos + move_space) - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, lcd_pos + move_space, frame_num, cur_frame);
#ifdef UI_SUPPORT_U_MAINMENU
                    direction = GO_MOVE_LEFT;
                    is_change_page = TRUE;
                    tmp_point.x = -lcd_pos + menu_ctrl_ptr->slide_start_point.x + offset_x;
#endif
                }
                else // 恢复到 menu_ctrl_ptr->slide_start_point.x
                {
                    offset_x = lcd_pos + move_space - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);
#ifdef UI_SUPPORT_U_MAINMENU
                    direction = GO_MOVE_LEFT;
                    tmp_point.x = menu_ctrl_ptr->slide_start_point.x + move_space - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);
#endif
                }
            }

            if (cur_frame < frame_num)
            {
                UILAYER_SetDirectDraw(TRUE);
                UILAYER_SetMainLayerImmutable(TRUE);
            }

            //count = count + frame_num - i;
#ifdef UI_SUPPORT_U_MAINMENU
            MoveBarLayer(menu_ctrl_ptr, offset_x);

            MMIISTYLE_U_Display(menu_ctrl_ptr, &tmp_point);

            if (cur_frame == frame_num) // 最后一针，恢复
            {
                MMIISTYLE_U_DrawUMenuLastFrame(menu_ctrl_ptr, direction, is_change_page);
            }

#else
            IStyleDrawPageLayers(menu_ctrl_ptr, offset_x);
#endif
            MMITHEME_UpdateRect();
        }

        IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);
    }
    else
    {
        IStyleDrawPageLayers(menu_ctrl_ptr, offset_x);
    }
}

/*****************************************************************************/
//  Description : get search page rect
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSearchPageRect(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    GUI_BOTH_RECT_T search_both_rect = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return search_both_rect.v_rect;
    }

    search_both_rect = MMITHEME_GetWinClientBothRect(menu_ctrl_ptr->win_handle);

    search_both_rect.v_rect.top += MAINMENU_SEARCH_MARGIN;
    search_both_rect.v_rect.bottom = menu_ctrl_ptr->default_data.bar_rect.top - 1;

    return search_both_rect.v_rect;
}

/*****************************************************************************/
//  Description : init search page form param
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void IStyleInitSearchPageParam(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO lcd_dev_info
)
{
    uint16                      bg_height = 0;

    MMI_CONTROL_CREATE_T        editbox_create = {0};
    MMI_CTRL_ID_T               editbox_ctrl_id = MMISEARCH_EDITBOX_ID;
    GUIEDIT_INIT_DATA_T         editbox_init_data = {0};
    GUI_BOTH_RECT_T             editbox_both_rect = {0};
    GUI_BG_T                    edit_bg = {0};

    MMI_CONTROL_CREATE_T        deletebutton_create = {0};
    MMI_CTRL_ID_T               deletebutton_ctrl_id = MMISEARCH_ISTYLE_DELETEBUTTON_ID;
    GUIBUTTON_INIT_DATA_T       deletebutton_init_data = {0};
    GUI_BG_T                    deletebutton_bg = {0};
    uint32                      deletebutton_bgimg = IMAGE_ISTYLEMENU_SEARCHPAGE_DELETEBUTTON;
    GUI_BOTH_RECT_T             deletebutton_both_rect = {0};
    uint16                      deletebutton_width = 0;

    MMI_CONTROL_CREATE_T        list_create = {0};
    GUILIST_INIT_DATA_T         list_init_data = {0};
    MMI_CTRL_ID_T               list_ctrl_id = MMISEARCH_LIST_ID;
    IGUICTRL_T                 *list_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr || 0 != menu_ctrl_ptr->cur_page_index)
    {
        return;
    }

    GUIRES_GetImgWidthHeight(PNULL, &bg_height, IMAGE_ISTYLEMENU_SEARCHPAGE_BG, menu_ctrl_ptr->win_handle);

    //create delete button
    GUIRES_GetImgWidthHeight(&deletebutton_width, PNULL, deletebutton_bgimg, menu_ctrl_ptr->win_handle);

    deletebutton_init_data.bg.bg_type = GUI_BG_IMG;
    deletebutton_init_data.bg.img_id = deletebutton_bgimg;

    deletebutton_both_rect.v_rect = GetSearchPageRect(menu_ctrl_ptr);
    deletebutton_both_rect.v_rect.right -= MAINMENU_SEARCH_MARGIN + MAINMENU_SEARCH_HORIZON_MARGIN;
    deletebutton_both_rect.v_rect.left = deletebutton_both_rect.v_rect.right - deletebutton_width + 1;
    deletebutton_both_rect.v_rect.bottom = deletebutton_both_rect.v_rect.top + bg_height;
    //deletebutton_both_rect.h_rect = deletebutton_init_data.both_rect.v_rect;
    deletebutton_init_data.both_rect = deletebutton_both_rect;

    deletebutton_create.ctrl_id = deletebutton_ctrl_id;
    deletebutton_create.guid = SPRD_GUI_BUTTON_ID;
    deletebutton_create.init_data_ptr = &deletebutton_init_data;
    deletebutton_create.parent_win_handle = menu_ctrl_ptr->win_handle;
    deletebutton_create.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
    deletebutton_create.state = GUICTRL_STATE_DISABLE_ACTIVE;
    MMK_CreateControl(&deletebutton_create);

    GUIBUTTON_SetCallBackFunc(deletebutton_ctrl_id, HandleIStyleSearchDeleteButtonCallBack);

    deletebutton_bg.bg_type = GUI_BG_IMG;
    deletebutton_bg.img_id = deletebutton_bgimg;
    GUIBUTTON_SetBg(deletebutton_ctrl_id, &deletebutton_bg);
    GUIBUTTON_SetPressedBg(deletebutton_ctrl_id, &deletebutton_bg);

    GUIBUTTON_PermitBorder(deletebutton_ctrl_id, FALSE);
    GUIBUTTON_SetRunSheen(deletebutton_ctrl_id, FALSE);

    //create edit-box ctrl
    editbox_both_rect.v_rect = GetSearchPageRect(menu_ctrl_ptr);
    editbox_both_rect.v_rect.top += 2;
    editbox_both_rect.v_rect.bottom = editbox_both_rect.v_rect.top + bg_height;
    editbox_both_rect.v_rect.left = MAINMENU_SEARCH_EDIT_MARGIN;
    editbox_both_rect.v_rect.right = deletebutton_both_rect.v_rect.left - 1;
    //editbox_both_rect.h_rect = editbox_both_rect.v_rect;

    editbox_init_data.both_rect  = editbox_both_rect;
    editbox_init_data.is_rect_adaptive = TRUE;
    editbox_init_data.type = GUIEDIT_TYPE_TEXT;
    editbox_init_data.str_max_len = MMISEARCH_MAX_KEYWORD_LENS;

    editbox_create.init_data_ptr = &editbox_init_data;
    editbox_create.ctrl_id = editbox_ctrl_id;
    editbox_create.guid = SPRD_GUI_EDITBOX_ID;
    editbox_create.parent_win_handle = menu_ctrl_ptr->win_handle;
    editbox_create.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
    MMK_CreateControl(&editbox_create);

    GUIEDIT_PermitBorder(editbox_ctrl_id, FALSE);

    GUIEDIT_SetStyle(editbox_ctrl_id, GUIEDIT_STYLE_SINGLE);

    if (PNULL == s_search_word_ptr)
    {
        GUIEDIT_SetDefaultStringById(editbox_ctrl_id, STXT_SEARCH);
    }
    else
    {
        GUIEDIT_SetString(editbox_ctrl_id, s_search_word_ptr, MMIAPICOM_Wstrlen(s_search_word_ptr));
    }

    //GUIEDIT_SetFont(editbox_ctrl_id,CAF_FONT_25);
    GUIEDIT_SetBg(editbox_ctrl_id, &edit_bg);

    //create list child ctrl
    list_init_data.is_dynamic_item_data = TRUE;
    list_init_data.max_item_num = MMISEARCH_MATCHED_NUM_MAX + 1;
    list_init_data.type = GUILIST_TEXTLIST_E;
    list_init_data.both_rect.v_rect = GetSearchPageRect(menu_ctrl_ptr);
    list_init_data.both_rect.v_rect.top = editbox_init_data.both_rect.v_rect.bottom + MAINMENU_SEARCH_MARGIN;
    list_init_data.both_rect.v_rect.left += MAINMENU_SEARCH_MARGIN;
    list_init_data.both_rect.v_rect.right -= MAINMENU_SEARCH_MARGIN;

    list_create.ctrl_id = list_ctrl_id;
    list_create.guid = SPRD_GUI_LIST_ID;
    list_create.init_data_ptr = &list_init_data;
    list_create.parent_win_handle = menu_ctrl_ptr->win_handle;
    MMK_CreateControl(&list_create);

    SetCtrlLcdDevInfoInSearchPage(lcd_dev_info);

    list_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(list_ctrl_id);

    if (PNULL != list_ptr)
    {
        IGUICTRL_SetState(list_ptr, GUICTRL_STATE_INVISIBLE, TRUE);
    }

    GUILIST_SetMaxItem(list_ctrl_id, MMISEARCH_MATCHED_NUM_MAX + 1, TRUE);
}

/*****************************************************************************/
//  Description : display search bg
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void DisplaySearchBg(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO    lcd_dev_info
)
{
    GUI_RECT_T      bg_rect = {0};
    uint16          bg_height = 0;

    if (PNULL == menu_ctrl_ptr || 0 != menu_ctrl_ptr->cur_page_index)
    {
        return ;
    }

    menu_ctrl_ptr->is_search_display = TRUE;

    //display bg
    GUIRES_GetImgWidthHeight(PNULL, &bg_height, IMAGE_ISTYLEMENU_SEARCHPAGE_BG, menu_ctrl_ptr->win_handle);

    bg_rect = GetSearchPageRect(menu_ctrl_ptr);
    bg_rect.bottom = bg_rect.top + bg_height;
    bg_rect.left = MAINMENU_SEARCH_MARGIN;
    bg_rect.right -= MAINMENU_SEARCH_MARGIN;

    UILAYER_ClearRect(&lcd_dev_info, bg_rect);

    GUIRES_DisplayImg(PNULL,
                      &bg_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      IMAGE_ISTYLEMENU_SEARCHPAGE_BG,
                      &lcd_dev_info);
}

/*****************************************************************************/
//  Description : display search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void DisplaySearchPageLayer(
    MMIMAINMENU_ISTYLE_DATA_T     *menu_ctrl_ptr,
    BOOLEAN             only_refresh_list, //只刷list区域
    BOOLEAN             only_refresh_editbox,//只刷editbox
    BOOLEAN             refresh_all,//全部刷新
    GUI_LCD_DEV_INFO    lcd_dev_info
)
{
    MMI_HANDLE_T    editbox_handle = 0;
    MMI_HANDLE_T    deletebutton_handle = 0;
    MMI_HANDLE_T    list_handle = 0;
    GUI_RECT_T      bg_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    uint16          bg_height = 0;

    if (PNULL == menu_ctrl_ptr || 0 != menu_ctrl_ptr->cur_page_index)
    {
        return ;
    }

    GetSearchCtrlHandle(&editbox_handle, &list_handle, &deletebutton_handle);

    menu_ctrl_ptr->is_search_display = TRUE;

    //display bg
    GUIRES_GetImgWidthHeight(PNULL, &bg_height, IMAGE_ISTYLEMENU_SEARCHPAGE_BG, menu_ctrl_ptr->win_handle);

    bg_rect = GetSearchPageRect(menu_ctrl_ptr);
    bg_rect.bottom = bg_rect.top + bg_height;
    bg_rect.left = MAINMENU_SEARCH_MARGIN;
    bg_rect.right -= MAINMENU_SEARCH_MARGIN;

    if (refresh_all)
    {
        cross_rect = GetSearchPageRect(menu_ctrl_ptr);
    }
    else if (only_refresh_editbox)
    {
        uint16 bg_width = 0;

        GUIRES_GetImgWidthHeight(&bg_width, PNULL, IMAGE_ISTYLEMENU_SEARCHPAGE_DELETEBUTTON, menu_ctrl_ptr->win_handle);

        cross_rect = bg_rect ;

        cross_rect.right -= bg_width + MAINMENU_SEARCH_HORIZON_MARGIN;
    }
    else if (only_refresh_list)
    {
        cross_rect = GetSearchPageRect(menu_ctrl_ptr);
        cross_rect.top = bg_rect.bottom + MAINMENU_SEARCH_MARGIN;
        cross_rect.left = MAINMENU_SEARCH_MARGIN;
        cross_rect.right -= MAINMENU_SEARCH_MARGIN;
    }

    UILAYER_ClearRect(&menu_ctrl_ptr->tmp_layer_handle, cross_rect);

    cross_rect = GUI_CreateRect(0, 0, cross_rect.right - cross_rect.left, cross_rect.bottom - cross_rect.top);

    if (only_refresh_editbox || refresh_all)
    {
        GUIRES_DisplayImg(PNULL,
                          &bg_rect,
                          &cross_rect,
                          menu_ctrl_ptr->win_handle,
                          IMAGE_ISTYLEMENU_SEARCHPAGE_BG,
                          &lcd_dev_info);
    }

    if (only_refresh_list || refresh_all)
    {
        GUIAPICTRL_HandleEvent(list_handle, MSG_CTL_PAINT, PNULL);
    }

    if (refresh_all)
    {
        GUIAPICTRL_HandleEvent(editbox_handle, MSG_CTL_PAINT, PNULL);
    }

    if (refresh_all)
    {
        GUIAPICTRL_HandleEvent(deletebutton_handle, MSG_CTL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//  Description : handle delete button call back in search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIStyleSearchDeleteButtonCallBack(void)
{
    MMI_STRING_T    keyword = {0};

    MMI_HANDLE_T    editbox_handle = 0;

    GetSearchCtrlHandle(&editbox_handle, PNULL, PNULL);

    GUIEDIT_GetString(editbox_handle, &keyword);

#ifdef SEARCH_SUPPORT
    MMIAPISEARCH_IstyleStop();
#endif

    if (0 < keyword.wstr_len && PNULL != keyword.wstr_ptr)
    {
        GUIEDIT_ClearAllStr(editbox_handle);

        MMK_SetActiveCtrl(editbox_handle, TRUE);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : set ctrl lcd dev info in search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlLcdDevInfoInSearchPage(GUI_LCD_DEV_INFO lcd_dev_info)
{
    MMI_HANDLE_T    editbox_handle = 0;
    MMI_HANDLE_T    deletebutton_handle = 0;
    MMI_HANDLE_T    list_handle = 0;

    GetSearchCtrlHandle(&editbox_handle, &list_handle, &deletebutton_handle);

    MMK_SetCtrlLcdDevInfo(editbox_handle, &lcd_dev_info);
    MMK_SetCtrlLcdDevInfo(list_handle, &lcd_dev_info);
    MMK_SetCtrlLcdDevInfo(deletebutton_handle, &lcd_dev_info);
}

/*****************************************************************************/
//  Description : set ctrl visible state in search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlVisibleStateInSearchPage(ISTYLEMENU_CHILD_FLAG flag)
{
    MMI_HANDLE_T    edit_handle = 0;
    MMI_HANDLE_T    list_handle = 0;
    MMI_HANDLE_T    delete_handle = 0;

    IGUICTRL_T     *edit_ptr = PNULL;
    IGUICTRL_T     *list_ptr = PNULL;
    IGUICTRL_T     *delete_ptr = PNULL;

    GetSearchCtrlHandle(&edit_handle, &list_handle, &delete_handle);

    edit_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(edit_handle);
    list_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(list_handle);
    delete_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(delete_handle);

    if (PNULL != edit_ptr)
    {
        IGUICTRL_SetState(edit_ptr, GUICTRL_STATE_INVISIBLE, (BOOLEAN)(!(flag & ISTYLE_MENU_CHILD_CONTROL_EDIT)));
    }

    if (PNULL != list_ptr)
    {
        IGUICTRL_SetState(list_ptr, GUICTRL_STATE_INVISIBLE, (BOOLEAN)(!(flag & ISTYLE_MENU_CHILD_CONTROL_LIST)));
    }

    if (PNULL != delete_ptr)
    {
        IGUICTRL_SetState(delete_ptr, GUICTRL_STATE_INVISIBLE, (BOOLEAN)(!(flag & ISTYLE_MENU_CHILD_CONTROL_BUTTON)));
    }
}

/*****************************************************************************/
//  Description : recreate search layer in search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:remove search layer
/*****************************************************************************/
LOCAL void RemoveSearchLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    GUI_RECT_T      search_rect = GetSearchPageRect(menu_ctrl_ptr);
    MMI_HANDLE_T    edit_handle = 0;

    UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);

    //cut search page layer to accelerate flush speed
    UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);
    UILAYER_BltLayerToLayer(&menu_ctrl_ptr->page_layer_handle[0], &menu_ctrl_ptr->tmp_layer_handle, &search_rect, FALSE);

    if (menu_ctrl_ptr->tmp_layer_handle.block_id != menu_ctrl_ptr->page_layer_handle[0].block_id)
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->tmp_layer_handle);/*lint !e506 !e774*/
    }

    menu_ctrl_ptr->tmp_layer_handle.block_id = menu_ctrl_ptr->page_layer_handle[0].block_id;

    SetCtrlLcdDevInfoInSearchPage(menu_ctrl_ptr->tmp_layer_handle);

    GetSearchCtrlHandle(&edit_handle, PNULL, PNULL);

    MMK_SetActiveCtrl(edit_handle, FALSE);
}

/*****************************************************************************/
//  Description : recreate search layer in search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:恢复搜索页
/*****************************************************************************/
LOCAL void RecoverSearchLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (menu_ctrl_ptr->page_layer_handle[0].block_id == menu_ctrl_ptr->tmp_layer_handle.block_id)
    {
#ifndef UI_SUPPORT_U_MAINMENU
        GUI_RECT_T search_rect = GetSearchPageRect(menu_ctrl_ptr);
#endif
        UILAYER_SetLayerMemFormat(&menu_ctrl_ptr->page_layer_handle[0], UILAYER_MEM_FPS_ACCELERATE);

#ifdef UI_SUPPORT_U_MAINMENU
        //cut memory for search page layer
        MMIISTYLE_U_ReleaseAllItemLayer();
#endif
        menu_ctrl_ptr->tmp_layer_handle.block_id = UILAYER_NULL_HANDLE;

        CreateSearchPageLayer(menu_ctrl_ptr);

        IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_layer_handle);

        SetCtrlLcdDevInfoInSearchPage(menu_ctrl_ptr->tmp_layer_handle);

#ifdef UI_SUPPORT_U_MAINMENU
        //recover GO info
		MMIISTYLE_U_SetPageInfo(menu_ctrl_ptr,FALSE);

        SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_EDIT_TEXT);

        DisplaySearchPageLayer(menu_ctrl_ptr, TRUE, TRUE, TRUE, menu_ctrl_ptr->tmp_layer_handle);
#else
        UILAYER_BltLayerToLayer(&menu_ctrl_ptr->tmp_layer_handle, &menu_ctrl_ptr->page_layer_handle[0], &search_rect, FALSE);

        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);

        BltSlideItemLayerToMenuLayerEx(menu_ctrl_ptr, 0, menu_ctrl_ptr->page_layer_handle[0], 0, TRUE, FALSE);
#endif
    }
}

/*****************************************************************************/
//  Description : create search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void CreateSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (!menu_ctrl_ptr->is_search_display)
    {
        CreateSearchPageLayer(menu_ctrl_ptr);

        IStyleInitSearchPageParam(menu_ctrl_ptr, menu_ctrl_ptr->tmp_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create search page layer
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void CreateSearchPageLayer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    GUI_RECT_T              search_rect = {0};
    UILAYER_CREATE_T        create_info = {0};

    if (PNULL == menu_ctrl_ptr || 0 != menu_ctrl_ptr->cur_page_index)
    {
        return ;
    }

    BLKMEM_GarbageClean();

    if (0 == menu_ctrl_ptr->tmp_layer_handle.block_id
        || UILAYER_NULL_HANDLE == menu_ctrl_ptr->tmp_layer_handle.block_id)
    {
        //get search page display rect
        search_rect = GetSearchPageRect(menu_ctrl_ptr);

        // create search page layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = search_rect.left;
        create_info.offset_y = search_rect.top;
        create_info.width = (search_rect.right + 1 - search_rect.left);
        create_info.height = (search_rect.bottom + 1 - search_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_IMMUTABLE;

        UILAYER_CreateLayer(
            &create_info,
            &menu_ctrl_ptr->tmp_layer_handle
        );

        // set color key
        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->tmp_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(&menu_ctrl_ptr->tmp_layer_handle);
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_12142_112_2_18_2_34_10_143, (uint8 *)"d", menu_ctrl_ptr->tmp_layer_handle.block_id);
    }
}

/*****************************************************************************/
//  Description : blt search layer to page layer
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void IStyleBltSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO const *lcd_dev_ptr,
    int16            offset_x
)
{
    int16       layer_x_position = 0;
    int16       layer_y_position = 0;

    if (PNULL == menu_ctrl_ptr || !menu_ctrl_ptr->is_search_display || 0 != menu_ctrl_ptr->cur_page_index)
    {
        return;
    }

    RecoverSearchLayer(menu_ctrl_ptr);

    UILAYER_GetLayerPosition(lcd_dev_ptr, &layer_x_position, &layer_y_position);

    UILAYER_SetLayerPosition(lcd_dev_ptr, offset_x, layer_y_position);

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_12186_112_2_18_2_34_10_144, (uint8 *)"d", offset_x);
}

/*****************************************************************************/
//  Description : release search page in left
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    BOOLEAN           free_searchwordptr
)
{
    MMI_HANDLE_T    edit_handle = 0;
    MMI_HANDLE_T    list_handle = 0;
    MMI_HANDLE_T    delete_handle = 0;
    MMI_STRING_T    keyword = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (PNULL != s_search_word_ptr)
    {
        SCI_FREE(s_search_word_ptr);
    }

    GetSearchCtrlHandle(&edit_handle, &list_handle, &delete_handle);

    //save search history to s_search_word_ptr
    GUIEDIT_GetString(edit_handle, &keyword);

    if (0 < keyword.wstr_len && PNULL != keyword.wstr_ptr && !free_searchwordptr)
    {
        s_search_word_ptr = SCI_ALLOC_APPZ((keyword.wstr_len + 1) * sizeof(wchar));

        if (PNULL != s_search_word_ptr)
        {
            SCI_MEMCPY(s_search_word_ptr, keyword.wstr_ptr, keyword.wstr_len * sizeof(wchar));
        }
    }

    MMK_DestroyControl(edit_handle);
    MMK_DestroyControl(list_handle);
    MMK_DestroyControl(delete_handle);

    if (menu_ctrl_ptr->tmp_layer_handle.block_id != menu_ctrl_ptr->page_layer_handle[0].block_id)
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->tmp_layer_handle);/*lint !e506 !e774*/
    }
    else
    {
        menu_ctrl_ptr->tmp_layer_handle.block_id = UILAYER_NULL_HANDLE;
    }

    menu_ctrl_ptr->is_search_display = FALSE;

    menu_ctrl_ptr->is_search_entry = FALSE;
    MMIAPISET_SetIsNeedValidatePrivacy(TRUE);
#ifdef SEARCH_SUPPORT
    MMIAPISEARCH_IstyleExit();
#endif

    MMK_SetWinHookFunc(menu_ctrl_ptr->win_handle, PNULL);
}

/*****************************************************************************/
//  Description : weather hided list response tp msg
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:list隐藏时判断是否响应tp消息
/*****************************************************************************/
LOCAL BOOLEAN IsListResponseTpMSGInSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T       point
)
{
    GUI_RECT_T              search_rect = {0};
    GUI_RECT_T              list_rect = {0};
    MMI_HANDLE_T            list_handle = 0;
    BOOLEAN                 result = FALSE;

    GUI_RECT_T              im_rect = {0};
    MMI_HANDLE_T            im_handle = 0;
    MMI_HANDLE_T            win_handle   = 0;
    MMI_HANDLE_T            ctrl_handle = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return result;
    }

    GetSearchCtrlHandle(PNULL, &list_handle, PNULL);

    search_rect = GetSearchPageRect(menu_ctrl_ptr);

    im_handle = MMK_GetCtrlHandleByGuid(menu_ctrl_ptr->win_handle, SPRD_GUI_IM_ID);

    list_rect = search_rect;

    if (0 != im_handle)
    {
        GUIIM_GetImRect(im_handle, &im_rect);

        list_rect.bottom = MIN(im_rect.top, search_rect.bottom);
    }

    MMK_GetHandleByPos(point, TRUE, &win_handle, &ctrl_handle, 0);

    if (GUI_PointIsInRect(point, list_rect)
        && ctrl_handle != MMK_GetCtrlHandleByGuid(win_handle, SPRD_GUI_IM_ID))
    {
        IGUICTRL_T *list_ptr = PNULL;
        list_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(list_handle);

        if (PNULL != list_ptr && GUICTRL_GetState(list_ptr, GUICTRL_STATE_INVISIBLE))
        {
            result = TRUE;
        }
    }

    return  result;
}

/*****************************************************************************/
//  Description : handle horizontal move in search page
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:set ctrl property when horizon move
/*****************************************************************************/
LOCAL BOOLEAN HandleHorizontalMoveInSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO const *lcd_dev_ptr,
    int16            offset_x
)
{
#ifdef SEARCH_SUPPORT
    MMI_RESULT_E    result = TRUE;
    MMI_HANDLE_T    edit_handle = 0;
    MMI_HANDLE_T    list_handle = 0;

    if (PNULL == menu_ctrl_ptr || 0 != menu_ctrl_ptr->cur_page_index)
    {
        return  TRUE;
    }

    if (! menu_ctrl_ptr->is_search_display && !menu_ctrl_ptr->is_search_entry && offset_x < 0)
    {
        return TRUE;
    }

    if (menu_ctrl_ptr->is_search_display && menu_ctrl_ptr->is_search_entry && 0 < offset_x)
    {
        ResetSearchPageLayerPosition(menu_ctrl_ptr);

        IStyleBltSearchPage(menu_ctrl_ptr, &menu_ctrl_ptr->tmp_layer_handle, 0);

        return FALSE;
    }

    //not redraw when move short
    if (menu_ctrl_ptr->is_search_display && menu_ctrl_ptr->is_search_entry
        && MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && offset_x > -MAINMENU_SEARCH_HORIZON_MOVE_SPACING)
    {
        return FALSE;
    }

    if (!menu_ctrl_ptr->is_search_display)
    {
        CreateSearchPage(menu_ctrl_ptr);

        DisplaySearchPageLayer(menu_ctrl_ptr, TRUE, TRUE, TRUE, menu_ctrl_ptr->tmp_layer_handle);

        IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_layer_handle);
    }

    RecoverSearchLayer(menu_ctrl_ptr);

    edit_handle = MMK_GetCtrlHandleByWin(menu_ctrl_ptr->win_handle, MMISEARCH_EDITBOX_ID);
    list_handle = MMK_GetCtrlHandleByWin(menu_ctrl_ptr->win_handle, MMISEARCH_LIST_ID);

    if (0 != offset_x && !menu_ctrl_ptr->is_search_listslide)
    {
        GUI_RECT_T      im_rect = {0};
        MMI_HANDLE_T    im_handle = 0;
        IGUICTRL_T       *im_ptr = PNULL;

        im_handle = MMK_GetCtrlHandleByGuid(menu_ctrl_ptr->win_handle, SPRD_GUI_IM_ID);
        im_ptr = MMK_GetCtrlPtr(im_handle);

        if (PNULL != im_ptr)
        {
            IGUICTRL_GetRect(im_ptr, &im_rect);

            MMK_SetActiveCtrl(edit_handle, FALSE);
            MMK_WinInactiveCtrl(menu_ctrl_ptr->win_handle, FALSE);

            MMK_SetActiveCtrl(menu_ctrl_ptr->ctrl_handle, FALSE);

            //clear im rect
            UILAYER_ClearRect(lcd_dev_ptr, im_rect);
        }

        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
    }

    SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_NONE);

    MMITHEME_InitMenuItemCache();

    MMITHEME_InitMenuFolderCache();

    return result;
#else
    return TRUE;
#endif
}

/*****************************************************************************/
//  Description : reset search page layer
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:reset search layer left position to zero
/*****************************************************************************/
LOCAL void ResetSearchPageLayerPosition(MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr)
{
    int16           layer_x_position = 0;
    int16           layer_y_position = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return ;
    }

    UILAYER_GetLayerPosition(&menu_ctrl_ptr->tmp_layer_handle, &layer_x_position, &layer_y_position);

    if (0 != layer_x_position)
    {
        UILAYER_SetLayerPosition(&menu_ctrl_ptr->tmp_layer_handle, 0, layer_y_position);
    }
}

/*****************************************************************************/
//  Description : check is list horizon move
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleListVerticalMoveInSearchPage(
    MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr,
    GUI_POINT_T     point
)
{
    BOOLEAN is_verticalmove = FALSE;

    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state)
    {
        is_verticalmove = IS_TP_MOVE_Y(point.y, menu_ctrl_ptr->slide_start_point.y);

        if (is_verticalmove)
        {
            StopRedrawTimer(menu_ctrl_ptr);

            menu_ctrl_ptr->is_search_listslide = TRUE;
        }
        else if (IS_TP_MOVE_X(point.x, menu_ctrl_ptr->slide_start_point.x))
        {
            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;

            SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_NONE);

            is_verticalmove = FALSE;
        }
    }

    return  is_verticalmove;
}

/*****************************************************************************/
//  Description : get search ctrl handle
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL void GetSearchCtrlHandle(
    MMI_HANDLE_T *edit_handle_ptr,
    MMI_HANDLE_T *list_handle_ptr,
    MMI_HANDLE_T *delete_handle_ptr
)
{
    if (PNULL != edit_handle_ptr)
    {
        *edit_handle_ptr = MMK_GetCtrlHandleByWin(MAIN_IDLE_WIN_ID, MMISEARCH_EDITBOX_ID);
    }

    if (PNULL != list_handle_ptr)
    {
        *list_handle_ptr = MMK_GetCtrlHandleByWin(MAIN_IDLE_WIN_ID, MMISEARCH_LIST_ID);
    }

    if (PNULL != delete_handle_ptr)
    {
        *delete_handle_ptr = MMK_GetCtrlHandleByWin(MAIN_IDLE_WIN_ID, MMISEARCH_ISTYLE_DELETEBUTTON_ID);
    }
}

/*****************************************************************************/
//  Description : hook function for handle i-style search
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E SearchPageHookFunc(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMI_HANDLE_T    list_handle = 0;
    MMI_HANDLE_T    edit_handle = 0;
    MMI_HANDLE_T    win_handle   = 0;
    MMI_HANDLE_T    ctrl_handle = 0;
    GUI_POINT_T     point = {0};
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr = PNULL;

    ctrl_handle = MMK_GetCtrlHandleByGuid(win_id, SPRD_GUI_MAINMENU_ID);
    menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)CTRLMAINMENU_GetPM(ctrl_handle);

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_12445_112_2_18_2_34_11_148, (uint8 *)"d", msg_id);

    if (PNULL == menu_ctrl_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ISTYLE_12449_112_2_18_2_34_11_149, (uint8 *)"");

        return result;
    }

    GetSearchCtrlHandle(&edit_handle, &list_handle, PNULL);

    switch (msg_id)
    {
        case MSG_TP_PRESS_DOWN:
        {
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            MMK_GetHandleByPos(point, TRUE, &win_handle, &ctrl_handle, 0);

            if (list_handle == ctrl_handle || IsListResponseTpMSGInSearchPage(menu_ctrl_ptr, point))
            {
                // 增加速度点
                MMK_AddVelocityItem(point.x, point.y);

                // 记录前一个点 记录起始点
                menu_ctrl_ptr->slide_start_point = menu_ctrl_ptr->slide_pre_point = menu_ctrl_ptr->pre_tp_point = point;

                StartRedrawTimer(menu_ctrl_ptr);

                menu_ctrl_ptr->is_search_listslide = FALSE; //mark this to distinguish list slide vertical and menu slide horizontal
            }
        }
        break;

        case MSG_TP_PRESS_MOVE:
        {
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            MMK_GetHandleByPos(point, TRUE, &win_handle, &ctrl_handle, 0);

            if (list_handle == ctrl_handle)
            {
                if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state)
                {
                    result = MMI_RESULT_TRUE;
                }
                else
                {
                    if (!menu_ctrl_ptr->is_search_listslide)
                    {
                        if (! HandleListVerticalMoveInSearchPage(menu_ctrl_ptr, point))
                        {
                            SetCtrlVisibleStateInSearchPage(ISTYLE_MENU_CHILD_CONTROL_NONE);

                            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;

                            result = MMI_RESULT_TRUE;
                        }
                    }
                }
            }
        }
        break;

        case MSG_TP_PRESS_UP:
        {
            IGUICTRL_T     *edit_ptr =  PNULL;
            edit_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(edit_handle);

            if (PNULL != edit_ptr)
            {
                IGUICTRL_SetState(edit_ptr, GUICTRL_STATE_INVISIBLE, FALSE);
            }

            StopRedrawTimer(menu_ctrl_ptr);

            if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state)
            {
                MMK_SendMsg(menu_ctrl_ptr->ctrl_handle, msg_id, param);

                result = MMI_RESULT_TRUE;

                return result;
            }
        }
        break;

        case MSG_TP_PRESS_SHORT:
        case MSG_TP_PRESS_DOUBLE:
            result = MMI_RESULT_TRUE;
            break;

        case MSG_GET_FOCUS:
            MMK_SetActiveCtrl(edit_handle, FALSE);
            break;

        case MSG_FULL_PAINT:
        {
            //destroy cache to cut down memory
            MMITHEME_DestroyMenuItemCache();
            MMITHEME_DestroyMenuFolderCache();
        }
        break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_KEYUP_CANCEL:
        {
            //only im response cancel key
            MMI_HANDLE_T    im_handle = 0;
            IGUICTRL_T       *im_ptr = PNULL;

            result = MMI_RESULT_TRUE;

            im_handle = MMK_GetCtrlHandleByGuid(menu_ctrl_ptr->win_handle, SPRD_GUI_IM_ID);
            im_ptr = MMK_GetCtrlPtr(im_handle);

            if (PNULL != im_ptr
                && im_handle == MMK_GetActiveCtrl(win_id))
            {
                //allow dispatch to im
                result = MMI_RESULT_FALSE;
            }
        }
        break;

        default:
            break;
    }

    return  result;
}

/*****************************************************************************/
//  Description : handle i-style menu search msg
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIISTYLESEARCH_HandleSearchWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMI_HANDLE_T    edit_handle = 0;
    MMI_HANDLE_T    list_handle = 0;
    MMI_HANDLE_T    delete_handle = 0;
    MMIMAINMENU_ISTYLE_DATA_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)CTRLMAINMENU_GetPM(MMK_GetCtrlHandleByGuid(win_id, SPRD_GUI_MAINMENU_ID));

    if (menu_ctrl_ptr == PNULL || !(menu_ctrl_ptr->is_search_entry && menu_ctrl_ptr->is_search_display))
    {
        return result;
    }

    GetSearchCtrlHandle(&edit_handle, &list_handle, &delete_handle);

    switch (msg_id)
    {
#ifdef  SEARCH_SUPPORT

        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T    keyword_str = {0};
            IGUICTRL_T     *list_ptr = PNULL;

            list_ptr = (IGUICTRL_T *)MMK_GetCtrlPtr(list_handle);

            GUIEDIT_GetString(edit_handle, &keyword_str);

            GUILIST_RemoveAllItems(list_handle);

            GUILIST_SetForbidTip(list_handle, TRUE);

            MMIAPISEARCH_IstyleStop();

            if (0 != keyword_str.wstr_len && PNULL != keyword_str.wstr_ptr)
            {
                MMIAPISEARCH_IstyleEntry();

                MMIAPISEARCH_SetSearchWinHandle(win_id);

                if (PNULL != list_ptr)
                {
                    IGUICTRL_SetState(list_ptr, GUICTRL_STATE_INVISIBLE, FALSE);
                }

                MMISEARCH_EntrySearching(&keyword_str);
            }
            else
            {
                if (PNULL != list_ptr)
                {
                    IGUICTRL_SetState(list_ptr, GUICTRL_STATE_INVISIBLE, TRUE);
                }

                //list already draw on layer,need to clear layer
                DisplaySearchPageLayer(menu_ctrl_ptr, TRUE, FALSE, FALSE, menu_ctrl_ptr->tmp_layer_handle);
            }

            result = MMI_RESULT_TRUE;
        }
        break;

        case MSG_NOTIFY_LIST_NEED_ITEM_CONTENT:
        case MSG_NOTIFY_LIST_NEED_ITEM_DATA:
        case MSG_SEARCH_LIST_APPEND_MATCH_ITEM:
        case MSG_SEARCH_LIST_REAPPEND_MATCH_ITEM:
        case MSG_SEARCH_START_SEARCH:
        case MSG_SEARCH_FILE_START_UPDATE_TIMER:
        case MSG_SEARCH_FILE_LOAD_ALL_LIST_FINISH:
        case MSG_TIMER:
            result = MMIAPISEARCH_HandleControlMsg(win_id, msg_id, param);
            break;
#endif  //end SEARCH_SUPPORT

        case MSG_NOTIFY_PENOK:
        {
            MMI_NOTIFY_T *notify_ptr = PNULL;

            notify_ptr = (MMI_NOTIFY_T *)param;

            if (PNULL != notify_ptr)
            {
#ifdef  SEARCH_SUPPORT

                if (list_handle == notify_ptr->src_handle)
                {
                    result = MMIAPISEARCH_HandleControlMsg(win_id, msg_id, param);
                }

#endif  //end SEARCH_SUPPORT

                if (! menu_ctrl_ptr->is_bar_item || notify_ptr->src_handle != menu_ctrl_ptr->ctrl_handle)
                {
                    result = MMI_RESULT_TRUE;
                }
            }
        }
        break;

        case MSG_NOTIFY_UPDATE:

        {
            IGUICTRL_T *deletebutton_ptr = PNULL;
            GUI_RECT_T  deletebutton_rect = {0};

            deletebutton_ptr = MMK_GetCtrlPtr(delete_handle);

            if (PNULL != deletebutton_ptr && PNULL != param)
            {
                IGUICTRL_GetRect(deletebutton_ptr, &deletebutton_rect);

                if (GUI_RectIsCovered(deletebutton_rect, *(GUI_RECT_T *)param))
                {
                    //delete button do not need update bg
                    break;
                }
            }

            //update bg when im commit
            DisplaySearchPageLayer(menu_ctrl_ptr, FALSE, TRUE, FALSE, menu_ctrl_ptr->tmp_layer_handle);
        }

        break;

        case MSG_NOTIFY_RESIZE:
        {
            // clear im rect when hide im
            GUI_RECT_T  im_rect = *(GUI_RECT_T *)param;

            if (GUI_IsRectEmpty(im_rect))
            {
                im_rect = menu_ctrl_ptr->default_data.bar_rect;
                GUILIST_SetNeedPrgbar(list_handle, TRUE);
            }
            else
            {
                GUIAPICTRL_HandleEvent(MMK_GetCtrlHandleByGuid(win_id, SPRD_GUI_IM_ID), MSG_CTL_PAINT, PNULL);
                GUILIST_SetNeedPrgbar(list_handle, FALSE);
            }

            UILAYER_ClearRect(&menu_ctrl_ptr->tmp_layer_handle, im_rect);

            DisplaySearchPageLayer(menu_ctrl_ptr, TRUE, FALSE, FALSE, menu_ctrl_ptr->tmp_layer_handle);

            BltSlideItemLayerToMenuLayerEx(menu_ctrl_ptr, 0, menu_ctrl_ptr->page_layer_handle[0], 0, TRUE, FALSE);
        }
        break;

        default:
            break;
    }

    return  result;
}

/*****************************************************************************/
//  Description : is in istyle search or not
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMENU_IsInIsyleSearch(void)
{
    BOOLEAN         result = FALSE;
    MMIMAINMENU_ISTYLE_DATA_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)CTRLMAINMENU_GetPM(MMK_GetCtrlHandleByGuid(MAIN_IDLE_WIN_ID, SPRD_GUI_MAINMENU_ID));

    if (PNULL != menu_ctrl_ptr)
    {
        result = menu_ctrl_ptr->is_search_entry;
    }

    return result;
}

/*****************************************************************************/
//  Description : release search page to save mem
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ReleaseIstyleSearch(void)
{
    MMIMAINMENU_ISTYLE_DATA_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)CTRLMAINMENU_GetPM(MMK_GetCtrlHandleByGuid(MAIN_IDLE_WIN_ID, SPRD_GUI_MENU_ID));

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->is_search_entry)
    {
        ReleaseSearchPage(menu_ctrl_ptr, FALSE);
    }
}

/*****************************************************************************/
//  Description : change page when click home key
//  Global resource dependence :
//  Author: Xinhe.Yan
//  Note:处理按中间键换页
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIMENU_HandleKeySlide(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

    switch (msg_id)
    {
        case MSG_APP_RED:
            MMK_NeedKbdClkNotifyOnce();
            break;

        case MSG_KEY_CLICK:
        {
            uint8 key_code = *(uint8 *)param;
            SCI_TRACE_LOW("@MSG_KEY_CLICK, key = 0x%x", key_code);

            if (KEY_RED == key_code)
            {
                MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr = PNULL;
                GUI_POINT_T     tp_point = {0};

                menu_ctrl_ptr = (MMIMAINMENU_ISTYLE_DATA_T *)CTRLMAINMENU_GetPM(MMK_GetCtrlHandleByGuid(win_id, SPRD_GUI_MAINMENU_ID));

                if (PNULL != menu_ctrl_ptr)
                {
                    if (menu_ctrl_ptr->is_moving || menu_ctrl_ptr->is_move_state || menu_ctrl_ptr->is_tp_pressed
                        || menu_ctrl_ptr->is_folder_open)
                    {
                        return result;
                    }

                    if (0 == menu_ctrl_ptr->cur_page_index)
                    {
                        //已经在搜索页
                        if (menu_ctrl_ptr->is_search_entry)
                        {
                            MMK_WinInactiveCtrl(menu_ctrl_ptr->win_handle, FALSE);

                            RecoverSearchLayer(menu_ctrl_ptr);

                            tp_point.x = menu_ctrl_ptr->slide_start_point.x - 1;

                            menu_ctrl_ptr->is_search_entry = FALSE;

                            HandleSearchSlideOver(menu_ctrl_ptr, &tp_point);

                            menu_ctrl_ptr->is_cur_pagesearch = FALSE;

                            ReleaseSearchPage(menu_ctrl_ptr, FALSE);
                        }
                        else
                        {
                            CreateSearchPage(menu_ctrl_ptr);

                            if (!UILAYER_IsLayerActive(&menu_ctrl_ptr->tmp_layer_handle))
                            {
                                return result;
                            }

                            DisplaySearchPageLayer(menu_ctrl_ptr, TRUE, TRUE, TRUE, menu_ctrl_ptr->tmp_layer_handle);

                            IStyleAppendLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_layer_handle);

                            menu_ctrl_ptr->slide_start_point.x = 0;
                            tp_point.x = menu_ctrl_ptr->slide_start_point.x + MMIMAINMENU_MOVE_SPACING;
                            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;

                            menu_ctrl_ptr->is_search_entry = TRUE;
                            HandleSearchSlideOver(menu_ctrl_ptr, &tp_point);

                            menu_ctrl_ptr->is_cur_pagesearch = TRUE;

                            HandleEntrySearchPage(menu_ctrl_ptr);
                        }
                    }
                    else
                    {
                        int32  offset_x        = 0;
                        int32   move_space      = 0; // 位移
                        uint16  frame_num       = 5; // 移动频率
                        uint16  cur_frame       = 1; // 计数
                        uint16  lcd_width       = 0; // lcd宽度
                        uint16  lcd_height      = 0; // lcd高度
                        GUI_LCD_DEV_INFO    lcd_dev_info = {0};

                        CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

#ifdef UI_SUPPORT_U_MAINMENU
                        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0], FALSE, FALSE);
#endif

                        GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

                        move_space = menu_ctrl_ptr->cur_page_index * lcd_width;

                        for (cur_frame = 1 ;  cur_frame <= frame_num ; cur_frame++)
                        {
                            if (cur_frame < frame_num)
                            {
                                UILAYER_SetDirectDraw(TRUE);
                                UILAYER_SetMainLayerImmutable(TRUE);
                            }

                            //平均分
                            offset_x = (menu_ctrl_ptr->cur_page_index) * lcd_width - (move_space - move_space / frame_num * cur_frame) ;

                            if (offset_x >= lcd_width)
                            {
                                menu_ctrl_ptr->cur_page_index = (move_space - move_space / frame_num * cur_frame) / lcd_width ;

                                offset_x = offset_x % lcd_width;
                            }

                            IStyleDrawPageLayers(menu_ctrl_ptr, offset_x);

                            MMITHEME_UpdateRect();
                        }

                        menu_ctrl_ptr->cur_page_index = 0;

                        IStyleDrawBar(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle, FALSE);

#ifdef UI_SUPPORT_U_MAINMENU
                        MMIISTYLE_U_SetPageInfo(menu_ctrl_ptr, FALSE);
#endif
                    }

                    result = MMI_RESULT_TRUE;
                }
            }
        }
        break;

        case MSG_KEY_DBLCLK:
        {
            uint8 key_code = *(uint8 *)param;
            SCI_TRACE_LOW("@MSG_KEY_DBLCLK, key = 0x%x", key_code);
        }
        break;

        default:
            break;
    }

    return result;
}

#ifdef UI_SUPPORT_U_MAINMENU

/*****************************************************************************/
//  Description : 获得页面数据
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN U_SetPageInfo(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
    uint16         page_index,
    void           *page_info_ptr
)
{
    uint16 i            = 0;
    uint16 item_num     = 0;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T icon_rect = {0};
    GUI_RECT_T menu_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint16 page_max_item_num = ISTYLE_PAGE_ITEM_CNT_MAX;
    U_MAINMENU_PAGE_INFO_T *umenu_page_info_ptr = (U_MAINMENU_PAGE_INFO_T *)page_info_ptr;

    if (PNULL == menu_ctrl_ptr || PNULL == page_info_ptr || page_index >= menu_ctrl_ptr->page_info.cnt)
    {
        return FALSE;
    }

    if (PNULL == umenu_page_info_ptr->page_info.icon_info)
    {
        return FALSE;
    }

    umenu_page_info_ptr->page_info.icon_num = page_max_item_num;

    umenu_page_info_ptr->page_info.total_col_num = menu_ctrl_ptr->default_data.page_col_num;
    umenu_page_info_ptr->page_info.total_row_num = menu_ctrl_ptr->default_data.page_row_num;

    lcd_dev_info = umenu_page_info_ptr->lcd_dev_info;

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcInputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(&lcd_dev_info, FALSE, FALSE);
    }

    item_num = menu_ctrl_ptr->page_info.page[page_index].cnt;

    if (0 == item_num)
    {
        return TRUE; //如果是零页则不用进行初始化
    }

    menu_rect = IStyleGetRect(menu_ctrl_ptr);

    icon_rect.left = 0;
    icon_rect.top = 0;
    icon_rect.right = icon_rect.left + MMITHEME_MENU_ITEM_WIDTH;
    icon_rect.bottom = icon_rect.top + MMITHEME_MENU_ITEM_HEIGHT;
    MMIISTYLE_U_ResetIstyleItemLayerPtr(item_num, umenu_page_info_ptr);

    for (i = 0; i < item_num; i++)
    {
        item_rect = menu_ctrl_ptr->default_data.item_rect[i];

        IStyleDrawItem(menu_ctrl_ptr, &lcd_dev_info, &icon_rect, page_index, i, GUIMENU_ITEM_IN_PAGE);

        icon_rect.top    += MMITHEME_MENU_ITEM_HEIGHT;
        icon_rect.bottom += MMITHEME_MENU_ITEM_HEIGHT;

        item_rect = GUI_GetCenterRect(item_rect, MMITHEME_MENU_ITEM_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

        umenu_page_info_ptr->page_info.icon_info[i].icon_offset.x = item_rect.left - menu_rect.left;
        umenu_page_info_ptr->page_info.icon_info[i].icon_offset.y = item_rect.top - menu_rect.top;
        umenu_page_info_ptr->page_info.icon_info[i].icon_size.w = MMITHEME_MENU_ITEM_WIDTH;
        umenu_page_info_ptr->page_info.icon_info[i].icon_size.h = MMITHEME_MENU_ITEM_HEIGHT;
        umenu_page_info_ptr->page_info.icon_info[i].item_col_num = i % menu_ctrl_ptr->default_data.page_col_num;
        umenu_page_info_ptr->page_info.icon_info[i].item_row_num = i / menu_ctrl_ptr->default_data.page_col_num;
    }

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcInputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(&lcd_dev_info, TRUE, TRUE);
    }

    return TRUE;
}
#endif //UI_SUPPORT_U_MAINMENU

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StartMoveStartTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->start_move_timer_id)
    {
        menu_ctrl_ptr->start_move_timer_id = MMK_CreateWinTimer(
                                                 menu_ctrl_ptr->ctrl_handle,
                                                 menu_ctrl_ptr->theme_data.icon_move_start_period,
                                                 FALSE);
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StopMoveStartTimer(
    MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->start_move_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->start_move_timer_id);
        menu_ctrl_ptr->start_move_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StartAutoMoveTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->tp_timer_id)
    {
        menu_ctrl_ptr->tp_timer_id = MMK_CreateWinTimer(
                                         menu_ctrl_ptr->ctrl_handle,
                                         menu_ctrl_ptr->theme_data.auto_move_period,
                                         TRUE);
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StopAutoMoveTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->tp_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->tp_timer_id);
        menu_ctrl_ptr->tp_timer_id = 0;
        menu_ctrl_ptr->continue_move_type = CONTINUE_MOVE_INVALID;
    }
}


/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StartRedrawTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->redrew_timer_id)
    {
        menu_ctrl_ptr->redrew_timer_id = MMK_CreateWinTimer(
                                             menu_ctrl_ptr->ctrl_handle,
                                             MMITHEME_MENU_REDRAW_TIME,
                                             FALSE);
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void StopRedrawTimer(MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->redrew_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->redrew_timer_id);
        menu_ctrl_ptr->redrew_timer_id = 0;
    }
}


/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IStyleRegister(CTRLMAINMENU_PM_APIS_T *apis)
{
    if (PNULL == apis)
    {
        return FALSE;
    }

    apis->Initialize = IStyleInit;
    apis->Destory = IStyleTerm;
    apis->Handle = IStyleHandle;

    return TRUE;
}


#endif // MMI_ISTYLE_SUPPORT
#endif // MMI_PDA_SUPPORT
#endif //TOUCH_PANEL_SUPPORT 

