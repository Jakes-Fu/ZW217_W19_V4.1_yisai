/*****************************************************************************
** File Name:      mmipic_edit_wintab.c                                                  *
** Author:                                                                   *
** Date:           01/10/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pic editor                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2008       Vincent.shang       Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_pic_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_EDITOR_SUPPORT
#include "os_api.h"
#include "mmipic_edit.h"
#include "mmipic_menutable.h"
#include "mmifmm_export.h"
#include "guiiconlist.h"
#include "mmipic_id.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "guiiconlist.h"
#include "guibutton.h"
#include "mmifmm_id.h"
#include "mmipic_image.h"
#include "mmi_appmsg.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmitheme_anim.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "mmiacc_event.h"
#include "mmischedule_export.h"
#include "guidropdownlist.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_mainmenu_export.h"
#include "mmisd_export.h"
#include "guiedit.h"
#include "mmipicview_export.h"
#include "mmi_default.h"
#include "guires.h"
#include "guiform.h"
#include "guiownerdraw.h"
#include "mmipic_text.h"
#include "guisetlist.h"
#ifdef PIC_VIEWER_SUPPORT
#include  "mmipicview_id.h"
#include  "mmipicview_internal.h"
#endif

#include "guiiconlist.h"
#include "guitab.h"
#include "mmitheme_iconlist.h"
//#include "UI_layer.h"
#include "guiownerdraw.h"
#include "guictrl_api.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPIC_COLOR_NUM 35
#define MMIPIC_COLOR_NUM_EACH_LINE  5
#define MMIPIC_FRAME_NUM    3

#if defined MMI_LOW_MEMORY_PIC_EDIT
#define MMIPIC_PASTE_IMAGE_NUM   4
#else
#if defined MMI_PIC_PASTE_IMAGE_FULL
#define MMIPIC_PASTE_IMAGE_NUM   30
#else
#define MMIPIC_PASTE_IMAGE_NUM   9
#endif //MMI_PIC_PASTE_IMAGE_FULL
#endif //MMI_LOW_MEMORY_RESOURCE

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

//warning
//LOCAL GUIANIM_MINIATURE_FULL_PATH_T   s_mmipic_miniature_full_name = {0};

LOCAL uint16  s_icon_index=0; 


LOCAL const uint32 s_char_edit_win_active_attr[] = 
{
    MMIPIC_NAME_LABEL_CTRL_ID,       //name content
        MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID,       //date content  
        0,       //time
        0,       //ring
        MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID 
};

LOCAL GUI_COLOR_T s_char_color[MMIPIC_COLOR_NUM] = {
0xFFFF,/*255,255,255*/    0xC618,/*194,194,194*/    0x8C51,/*137,137,137*/    0x31A6,/*54,54,54*/    0x0000,/*0,0,0*/      
0xFFF3,/*255,255,153*/    0xFFE0,/*255,255,0*/      0xF4A3,/*247,148,29*/     0xF324,/*242,101,34*/  0xA201,/*160,65,13*/  
0xFDF8,/*255,188,192*/    0xF36F,/*242,109,125*/    0xF800,/*255,0,0*/        0x9841,/*157,10,14*/   0x3000,/*51,0,0*/     
0xFDFB,/*251,190,216*/    0xE811,/*236,0,140*/      0x980B,/*153,0,93*/       0xF81F,/*255,0,255*/   0x3009,/*50,0,75*/    
0x07FF,/*0,255,255*/      0x057D,/*0,173,239*/      0x0250,/*0,74,128*/       0x001F,/*0,0,255*/     0x000C,/*0,0,102*/    
0x9FE0,/*153,255,0*/      0x07E0,/*0,255,0*/        0x052A,/*0,166,80*/       0x0180,/*0,51,0*/      0x2992,/*46,48,146*/  
0xC4ED,/*198,156,109*/    0x6180,/*102,51,0*/        0x2080,/*38,19,0*/        0x9B2C,/*153,102,102*/ 0xFE79/*255,204,204*/

#if 0
WAIT_TEXT_BACKGROUND_COLOR,
        MMI_DARK_BLUE_COLOR,
        MMI_BLUE_COLOR,
        MMI_DARK_GREEN_COLOR,
        MMI_DARK_CYAN_COLOR,
        MMI_GREEN_COLOR,
        MMI_CYAN_COLOR,     
        MMI_DARK_RED_COLOR,
        MMI_DIAL_GRAY_COLOR, //MMI_DARK_MAGENTA_COLOR,
        MMI_DARK_YELLOW_COLOR,
        MMI_DARK_GRAY_COLOR,
        MMI_GRAY_COLOR,
        MMI_RED_COLOR,
        MMI_DARK_WHITE_COLOR, //MMI_MAGENTA_COLOR,
        MMI_YELLOW_COLOR,
        MMI_WHITE_COLOR,
        MMI_BLACK_COLOR,
        MMI_PURPLE_COLOR,
#endif        
} ;
LOCAL BOOLEAN s_is_changed = FALSE;
LOCAL BOOLEAN s_is_saved = FALSE;
LOCAL uint32 s_operation_times = 0;

LOCAL MMIPIC_PROGRESSBAR_RECT_T  s_bright_bar_rect = {0};
LOCAL MMIPIC_PROGRESSBAR_T       s_bright_bar = {0};
//LOCAL MMIPIC_PROGRESSBAR_RECT_T  s_hue_bar_rect = {0};
//LOCAL MMIPIC_PROGRESSBAR_T       s_hue_bar = {0};
LOCAL MMIPIC_PROGRESSBAR_RECT_T  s_color_bar_rect = {0};
LOCAL MMIPIC_PROGRESSBAR_T       s_color_bar = {0};

LOCAL BOOLEAN           is_pic_edited = FALSE;
LOCAL MMI_MESSAGE_ID_E  s_pre_tp_msg = 0; 

LOCAL BOOLEAN           s_is_tp_progress_glide = FALSE;
//LOCAL GUI_LCD_DEV_INFO      s_icon_layer_handle = {0};
//LOCAL MMIPIC_ADJUST_TYPE_E  s_adjust_type = MMIPIC_BRI_TYPE;
#define ICON_COR_CYAN       0x07FF
#define ICON_COR_RED        0xF800
#define ICON_COR_MAGENTA    0xF81F
#define ICON_COR_GREEN      0x07E0
#define ICON_COR_YELLOW     0xFFE0
#define ICON_COR_BLUE       0x001F


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
static PIC_CHAR_PARAM_T s_char_info={0};

//pic select folder
static wchar* s_selected_folder_ptr = PNULL;
wchar* g_random_name_ptr = PNULL;

extern uint16 g_random_name_len;
extern BOOLEAN g_can_be_default_saved;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

// /*****************************************************************************/
// //  Description : init PROGRESSBAR_T structure variable
// //  Global resource dependence : 
// //  Author: robert.wang
// //  Note: 
// /*****************************************************************************/
// LOCAL void Init_PROGRESSBAR_T(MMIPIC_PROGRESSBAR_T *bar);
/*****************************************************************************/
//  Description : get progressbar status for tp
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL int8 GetProgressBarStatusForTP(GUI_POINT_T point , //int
                                           MMIPIC_PROGRESSBAR_T bar, //in
                                           MMIPIC_PROGRESSBAR_RECT_T bar_rect, //in
                                           RECT_INDEX_E *rect_index, //out
                                           int16        *grade_level    //out
                                           );
                                           
/*****************************************************************************/
//  Description : handle pic editor main win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditorMainWinMsg (
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM      param
                                            );

// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : handle main menu window  msg
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleEditorMenuWinMsg (
//                                             MMI_WIN_ID_T        win_id,
//                                             MMI_MESSAGE_ID_E    msg_id, 
//                                             DPARAM      param
//                                             );
// #endif
/*****************************************************************************/
//  Description : handle save as window msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePicSaveAsWinMsg(
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM       param
                                           );
/*****************************************************************************/
//  Description : handle zoom win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleZoomWinMsg (
                                      MMI_WIN_ID_T      win_id,
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM            param
                                      );

/*****************************************************************************/
//  Description : handle rotate win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRotateWinMsg (
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM          param
                                        );

/*****************************************************************************/
//  Description : handle paste pic preview win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePastePreviewWinMsg (
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id, 
                                              DPARAM            param
                                              );
/*****************************************************************************/
//  Description : handle paste pic edit win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePasteEditWinMsg (
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : handle add character win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCharacterFormWinMsg (
                                              MMI_WIN_ID_T      win_id,
                                              MMI_MESSAGE_ID_E  msg_id, 
                                              DPARAM            param
                                              );

/*****************************************************************************/
//  Description : handle character preview window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleCharacterPreviewWinMsg (
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id, 
                                                  DPARAM            param
                                                  );

#ifdef PIC_EDIT_FRAME_SUPPORT
/*****************************************************************************/
//  Description : handle frame window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFrameWinMsg (
                                       MMI_WIN_ID_T     win_id,
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       );
#endif

// /*****************************************************************************/
// //  Description : handle resize pic menu window message
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleResizeMenuWinMsg (
//                                             MMI_WIN_ID_T        win_id,
//                                             MMI_MESSAGE_ID_E    msg_id, 
//                                             DPARAM          param
//                                             );
// 
// 
// /*****************************************************************************/
// //  Description : handle resized pic preview window message
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleResizePreviewWinMsg (
//                                                MMI_WIN_ID_T     win_id,
//                                                MMI_MESSAGE_ID_E msg_id, 
//                                                DPARAM           param
//                                                );

/*****************************************************************************/
//  Description : handle RGB window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleRGBWinMsg (
                                     MMI_WIN_ID_T       win_id,
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM         param
                                     );

/*****************************************************************************/
//  Description : handle adjust bright and constract window msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdjustBrightAndConstractWinMsg (
                                                          MMI_WIN_ID_T      win_id,
                                                          MMI_MESSAGE_ID_E  msg_id, 
                                                          DPARAM            param
                                                          );
#ifndef PIC_EDIT_U200_SUPPORT
/*****************************************************************************/
//  Description : handle adjust saturation and hue window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdjustSaturationAndHueWinMsg (
                                                        MMI_WIN_ID_T        win_id,
                                                        MMI_MESSAGE_ID_E    msg_id, 
                                                        DPARAM          param
                                                        );
#endif
/*****************************************************************************/
//  Description : handle effect window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEffectWinMsg (
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM          param
                                        );
/*****************************************************************************/
//  Description : handle add text editor window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddCharTextEditorWinMsg(
                                                 MMI_WIN_ID_T       win_id,
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM         param
                                                 );
/*****************************************************************************/
//  Description : handle character color window message
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCharColorWinMsg(
                                         MMI_WIN_ID_T       win_id,
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM         param
                                         );

/*****************************************************************************/
//  Description : set win title
//  Global resource dependence : 
//  Author:vincent.shang
//  Note: 
/*****************************************************************************/
LOCAL void SetFileNameWinTitle(
                               MMI_WIN_ID_T      win_id, 
                               const wchar       *file_name_ptr,
                               uint16            file_name_len,
                               //BOOLEAN           is_ucs2,
                               BOOLEAN           is_refresh
                               );
                               
/*****************************************************************************/
// 	Description : owner draw function for color and frame color window 
//	Global resource dependence : 
//  Author: robert.wang
//	Note: 
/*****************************************************************************/
LOCAL void AddCharColorDrawRect(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
// 	Description : owner draw function for color and frame color window 
//	Global resource dependence : 
//  Author: robert.wang
//	Note: 
/*****************************************************************************/
LOCAL void AddCharFrameColorDrawRect(GUIOWNDRAW_INFO_T *owner_draw_ptr);

LOCAL MMI_RESULT_E HandlePicEditorExitAlertWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

//[START]pic select folder
LOCAL uint32 MMIPIC_GetFilePathLen( uint16 * pWString );
//[END]pic select folder
LOCAL void MMIPic_OpenSelectFolderWin(void);

/*****************************************************************************/
// 	Description : change state and  record changed times
//	Global resource dependence : 
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void RecordChangedTimes(void);

/*****************************************************************************/
// 	Description : display char on pic
//	Global resource dependence : 
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void CalCharPosition(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description :  set param of form for adjust windows
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetMainWinFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T form_id, MMI_CTRL_ID_T first_child_id);

/*****************************************************************************/
// 	Description : open main menu icon tab ctrl
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T img_id, uint16 icon_num, MMI_IMAGE_ID_T bg_img);

/*****************************************************************************/
// 	Description :  set param of form
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T child_ctrl_id, 
                               MMI_TEXT_ID_T* text_id, MMI_IMAGE_ID_T img_id,
                               uint16 child_ctrl_num, MMI_IMAGE_ID_T bg_img);

#ifdef PIC_EDIT_U200_SUPPORT
/*****************************************************************************/
// 	Description : set title context && rect && bg_img
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetPicEditTitle(MMI_WIN_ID_T win_id, MMI_STRING_T *str_ptr, MMI_IMAGE_ID_T bg_img);

/*****************************************************************************/
// 	Description :  set the rect of buttons on the top of window
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetCancelOkButtonParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctr1_id, MMI_CTRL_ID_T ctr2_id, BOOLEAN is_main_win);

/*****************************************************************************/
// 	Description :  set the param of buttons at bottom of window
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetEditOptionButtonParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_TEXT_ID_T* text_id,
                                       uint16 ctrl_num, MMI_IMAGE_ID_T bg_img);

/*****************************************************************************/
// 	Description :  the handle of pubwin of charater size
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCharSizeWinMsg(MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param );

#endif //PIC_EDIT_U200_SUPPORT
/*****************************************************************************/
// 	Description :  save the edit of image
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SaveEditImg(void);

/*****************************************************************************/
// 	Description :  save the edit of image
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveMsg(MMI_WIN_ID_T win_id, 
                                 MMI_MESSAGE_ID_E msg_id, 
                                 DPARAM param );
                                 
/*****************************************************************************/
// 	Description :  Save Img File
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SaveImgFile(MMI_WIN_ID_T win_id, BOOLEAN is_overwrite);

/*****************************************************************************/
// 	Description : display the default img for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayBackgroudImg(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : display the focus border for form's child
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayFormBorder(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
// 	Description : set the main form buttons grayed
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetMainFormGrayed(BOOLEAN is_grayed);

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
// the window for  pic editor main window
WINDOW_TABLE(MMIPIC_EDITOR_MAIN_WIN_TAB) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleEditorMainWinMsg),    
        WIN_ID( MMIPIC_EDIT_MAIN_WIN_ID),
        WIN_HIDE_STATUS,
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_MAIN_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_MAIN_SAVE_CTRL_ID),
#endif
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIPIC_EDIT_MAINMENU_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAG_PS_ICON_ROTATE, MMIPIC_EDIT_MAINMENU_RAOTATE_ID, MMIPIC_EDIT_MAINMENU_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAG_PS_ICON_ADJUST, MMIPIC_EDIT_MAINMENU_ADJUST_ID, MMIPIC_EDIT_MAINMENU_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAG_PS_ICON_EFFECT, MMIPIC_EDIT_MAINMENU_EFFECT_ID, MMIPIC_EDIT_MAINMENU_FORM_ID),
#ifdef PIC_EDIT_FRAME_SUPPORT
            CHILD_BUTTON_CTRL(TRUE, IMAG_PS_ICON_FRAME, MMIPIC_EDIT_MAINMENU_FRAME_ID, MMIPIC_EDIT_MAINMENU_FORM_ID),
#endif
            CHILD_BUTTON_CTRL(TRUE, IMAG_PS_ICON_TEXT, MMIPIC_EDIT_MAINMENU_TEXT_ID, MMIPIC_EDIT_MAINMENU_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAG_PS_ICON_CLIP, MMIPIC_EDIT_MAINMENU_CLIP_ID, MMIPIC_EDIT_MAINMENU_FORM_ID),
        END_WIN 
};

// //the window for  pic editor menu window
// WINDOW_TABLE(MMIPIC_EDIT_MENU_WIN_TAB) = 
// {
//     WIN_FUNC( (uint32)HandleEditorMenuWinMsg),
//     WIN_ID(MMIPIC_EDIT_MENU_WIN_ID),
//     WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
//     WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
//     CREATE_POPMENU_CTRL(MENU_PIC_EDIT_FUNC_OPT,MMIPIC_FUNC_MENU_CTRL_ID),
//     WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
//     END_WIN
// };

//rename win
WINDOW_TABLE(MMIPIC_SAVE_AS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePicSaveAsWinMsg ),    
    WIN_ID(MMIPIC_SAVE_AS_WIN_ID),
    WIN_TITLE(TXT_COMMON_SAVE_AS),
#ifdef PIC_EDIT_U200_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    CREATE_EDIT_TEXT_CTRL(0,MMIPIC_SAVE_AS_EDITBOX_CTRL_ID),
    END_WIN
};

// the window for  pic editor zoom window
WINDOW_TABLE(MMIPIC_ZOOM_WIN_TAB) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleZoomWinMsg),    
        WIN_ID( MMIPIC_EDIT_ZOOM_WIN_ID),
        WIN_STYLE( WS_DISABLE_COMMON_BG),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                       
        END_WIN
};

// #ifndef PIC_EDIT_U200_SUPPORT
// // the window for  pic editor rotate window
// WINDOW_TABLE(MMIPIC_ROTATE_WIN_TAB) = 
// {
//     //CLEAR_LCD,
//         //WIN_PRIO( WIN_ONE_LEVEL ),
//         WIN_FUNC( (uint32)HandleRotateWinMsg),    
//         WIN_ID( MMIPIC_EDIT_ROTATE_WIN_ID),
//         WIN_STYLE( WS_DISABLE_COMMON_BG),
//         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//         WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),        
//         END_WIN
// };
// 
// // the window for  pic editor paste preview window
// WINDOW_TABLE(MMIPIC_PASTE_EDIT_WIN_TAB) = 
// {
//     //CLEAR_LCD,
//         //WIN_PRIO( WIN_ONE_LEVEL ),
//         WIN_FUNC( (uint32)HandlePasteEditWinMsg),    
//         WIN_ID( MMIPIC_PASTE_EDIT_WIN_ID),
//         WIN_STYLE( WS_DISABLE_COMMON_BG),
//         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//         WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
//         CREATE_ICONLIST_CTRL(MMIPIC_VIEWPIC_ICONLIST_CTRL_ID),
//         END_WIN
// };
// 
// // the window for  pic editor paste edit window
// WINDOW_TABLE(MMIPIC_PASTE_PREVIEW_WIN_TAB) = 
// {
//     //CLEAR_LCD,
//         //WIN_PRIO( WIN_ONE_LEVEL ),
//         WIN_FUNC( (uint32)HandlePastePreviewWinMsg),    
//         WIN_ID(MMIPIC_PASTE_PREVIEW_WIN_ID),
//         WIN_STYLE( WS_DISABLE_COMMON_BG),
//         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//         WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
//         END_WIN
// };
// 
// #else

WINDOW_TABLE(MMIPIC_ROTATE_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandleRotateWinMsg),    
        WIN_ID( MMIPIC_EDIT_ROTATE_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
#endif
        END_WIN
};

//剪贴画预览窗口(用来显示图片与剪贴画重叠界面)
WINDOW_TABLE(MMIPIC_PASTE_PREVIEW_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandlePastePreviewWinMsg),    
        WIN_ID(MMIPIC_PASTE_PREVIEW_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
#endif
        END_WIN
};

//剪贴画icon选择窗口//注，与原有的不同
WINDOW_TABLE(MMIPIC_PASTE_EDIT_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandlePasteEditWinMsg),    
        WIN_ID( MMIPIC_PASTE_EDIT_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_EDIT_CANCEL_CTRL_ID),
#endif        
        CREATE_ICONLIST_CTRL(MMIPIC_VIEWPIC_ICONLIST_CTRL_ID),
        END_WIN
};

// #endif

#ifndef PIC_EDIT_U200_SUPPORT
// the window for  pic editor add character window
WINDOW_TABLE( MMIPIC_ADD_CHARACTER_WIN_TAB ) = 
{
        WIN_ID(MMIPIC_ADD_CHARACTER_WIN_ID),
        WIN_FUNC((uint32)HandleCharacterFormWinMsg),
		WIN_TITLE(TXT_PIC_ADD_CHAR),        
		WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPIC_CHARACTER_FORM_CTRL1_ID),
        
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPIC_CHARACTER_NAME_FORM_CTRL_ID,MMIPIC_CHARACTER_FORM_CTRL1_ID),
                //name
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPIC_NAME_TITLE_LABEL_CTRL_ID,MMIPIC_CHARACTER_NAME_FORM_CTRL_ID),
                //name content
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIPIC_NAME_LABEL_CTRL_ID,MMIPIC_CHARACTER_NAME_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPIC_CHARACTER_SIZE_FORM_CTRL_ID,MMIPIC_CHARACTER_FORM_CTRL1_ID),
                //size title
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPIC_SIZE_TITLE_LABEL_CTRL_ID,MMIPIC_CHARACTER_SIZE_FORM_CTRL_ID),
                //size
                CHILD_SETLIST_CTRL(TRUE,MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID,MMIPIC_CHARACTER_SIZE_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPIC_CHARACTER_FONT_COLOR_FORM_CTRL_ID,MMIPIC_CHARACTER_FORM_CTRL1_ID),
                //font color title
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPIC_COLOR_TITLE_LABEL_CTRL_ID,MMIPIC_CHARACTER_FONT_COLOR_FORM_CTRL_ID),
                //font color  
                CHILD_OWNDRAW_CTRL(TRUE,MMIPIC_COLOR_LABEL_CTRL_ID,MMIPIC_CHARACTER_FONT_COLOR_FORM_CTRL_ID, AddCharColorDrawRect),/*lint !e64*/

            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPIC_CHARACTER_FRAME_COLOR_FORM_CTRL_ID,MMIPIC_CHARACTER_FORM_CTRL1_ID),
                //frame color title
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPIC_FRAME_COLOR_TITLE_LABEL_CTRL_ID,MMIPIC_CHARACTER_FRAME_COLOR_FORM_CTRL_ID),
                //frame color 
                CHILD_OWNDRAW_CTRL(TRUE,MMIPIC_FRAME_COLOR_LABEL_CTRL_ID,MMIPIC_CHARACTER_FRAME_COLOR_FORM_CTRL_ID, AddCharFrameColorDrawRect),/*lint !e64*/

            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPIC_CHARACTER_DIRECTION_COLOR_FORM_CTRL_ID,MMIPIC_CHARACTER_FORM_CTRL1_ID),
                //direction title
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPIC_DIRECTION_TITLE_LABEL_CTRL_ID,MMIPIC_CHARACTER_DIRECTION_COLOR_FORM_CTRL_ID),
                //direction 
                CHILD_SETLIST_CTRL(TRUE,MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID,MMIPIC_CHARACTER_DIRECTION_COLOR_FORM_CTRL_ID),

        END_WIN
};

// 输入编辑文字的颜色
WINDOW_TABLE(MMPIC_CHAR_COLOR_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleCharColorWinMsg),    
        WIN_ID( MMIPIC_CHAR_COLOR_WIN_ID ), 
        WIN_STYLE( WS_DISABLE_COMMON_BG),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        END_WIN
}; 

// 输入编辑文字的内容
WINDOW_TABLE(MMPIC_ADD_CHAR_EDITTEXT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleAddCharTextEditorWinMsg),    
    WIN_ID( MMIPIC_EDITTEXT_WIN_ID ), 
    WIN_TITLE(TXT_PIC_INPUT_CHAR),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    CREATE_EDIT_TEXT_CTRL(MMIPIC_CHAR_TEXT_MAX_LEN,MMIPIC_TEXTEDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
    END_WIN
}; 

// the window for  pic editor character preview window
WINDOW_TABLE(MMIPIC_CHARACTER_PREVIEW_WIN_TAB) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleCharacterPreviewWinMsg),    
        WIN_ID(MMIPIC_CHARACTER_PREVIEW_WIN_ID),
        WIN_STYLE( WS_DISABLE_COMMON_BG),
        WIN_HIDE_STATUS,
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        END_WIN
};

#else

//编辑文字的大小
WINDOW_TABLE(MMPIC_CHAR_SIZE_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleCharSizeWinMsg),    
    WIN_ID( MMIPIC_CHAR_SIZE_WIN_ID), 
    WIN_TITLE(TXT_COMMON_FONT_SIZE),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPIC_CHAR_SIZE_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
}; 

// 输入编辑文字的颜色
WINDOW_TABLE(MMPIC_CHAR_COLOR_WIN_TAB ) = 
{
        WIN_FUNC((uint32)HandleCharColorWinMsg),    
        WIN_ID( MMIPIC_CHAR_COLOR_WIN_ID ), 
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_EDIT_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_EDIT_OK_CTRL_ID),
        END_WIN
}; 

// 输入编辑文字的内容
WINDOW_TABLE(MMPIC_ADD_CHAR_EDITTEXT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleAddCharTextEditorWinMsg),    
    WIN_ID( MMIPIC_EDITTEXT_WIN_ID ), 
    WIN_TITLE(TXT_COMMON_CONTENTS),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    CREATE_EDIT_TEXT_CTRL(MMIPIC_CHAR_TEXT_MAX_LEN, MMIPIC_TEXTEDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    END_WIN
}; 

// the window for  pic editor character preview window
WINDOW_TABLE(MMIPIC_CHARACTER_PREVIEW_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandleCharacterPreviewWinMsg),    
        WIN_ID(MMIPIC_CHARACTER_PREVIEW_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIPIC_CHAR_PREVIEW_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, PNULL, MMIPIC_CHAR_COLOR_CTRL_ID, MMIPIC_CHAR_PREVIEW_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, PNULL, MMIPIC_CHAR_SIZE_CTRL_ID, MMIPIC_CHAR_PREVIEW_FORM_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
        END_WIN
};

#endif

#ifdef PIC_EDIT_FRAME_SUPPORT
WINDOW_TABLE(MMIPIC_FRAME_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandleFrameWinMsg),    
        WIN_ID(MMIPIC_EDIT_FRAME_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
#endif
        CREATE_ICONLIST_CTRL(MMIPIC_EDIT_FRAME_ICONLIST_CTRL_ID),
        END_WIN
};
#endif//PIC_EDIT_FRAME_SUPPORT

// #ifndef PIC_EDIT_U200_SUPPORT
// // the window for  pic editor resize menu window
// WINDOW_TABLE(MMIPIC_RESIZE_MENU_WIN_TAB) = 
// {
//     //CLEAR_LCD,
//     //WIN_PRIO( WIN_ONE_LEVEL ),
//         WIN_FUNC( (uint32)HandleResizeMenuWinMsg),    
//         WIN_ID(MMIPIC_RESIZE_MENU_WIN_ID),
//         WIN_TITLE(TXT_PIC_ADJUST_SIZE),
//         WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
//         
//         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
// 
//         CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPIC_RESIZE_LISTBOX_CTRL_ID),
//         END_WIN
// };
// 
// // the window for  pic editor resize menu window
// WINDOW_TABLE(MMIPIC_RESIZE_PREVIEW_WIN_TAB) = 
// {
//     //CLEAR_LCD,
//         //WIN_PRIO( WIN_ONE_LEVEL ),
//         WIN_FUNC( (uint32)HandleResizePreviewWinMsg),    
//         WIN_ID(MMIPIC_RESIZE_PREVIEW_WIN_ID),
//         WIN_STYLE( WS_DISABLE_COMMON_BG),
//         WIN_TITLE(TXT_NULL),
//         WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//         WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
//         END_WIN
// };
// #endif

// the window for  pic editor RGB window
WINDOW_TABLE(MMIPIC_RGB_WIN_TAB) = 
{
        WIN_FUNC((uint32)HandleRGBWinMsg),    
        WIN_ID( MMIPIC_EDIT_RGB_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
#endif
        CREATE_OWNDRAW_CTRL(MMIPIC_ADJUST_COL_GROUP_ID, PNULL),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIPIC_ADJUST_COL_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIPIC_ADJUST_COL_BRI_BUTTON_ID, MMIPIC_ADJUST_COL_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIPIC_ADJUST_COL_COL_BUTTON_ID, MMIPIC_ADJUST_COL_FORM_ID),
        END_WIN
};

// the window for  pic editor adjustment Bright And Constract window
WINDOW_TABLE(MMIPIC_ADJUS_BRI_CON_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandleAdjustBrightAndConstractWinMsg),    
        WIN_ID( MMIPIC_EDIT_ADJUST_BRI_CON_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
#endif
        CREATE_OWNDRAW_CTRL(MMIPIC_ADJUST_BRI_GROUP_ID, PNULL),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIPIC_ADJUST_BRI_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIPIC_ADJUST_BRI_BRI_BUTTON_ID, MMIPIC_ADJUST_BRI_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIPIC_ADJUST_BRI_COL_BUTTON_ID, MMIPIC_ADJUST_BRI_FORM_ID),
        END_WIN
};

// the window for  pic editor effect window
WINDOW_TABLE(MMIPIC_EFFECT_WIN_TAB) = 
{
        WIN_FUNC( (uint32)HandleEffectWinMsg),    
        WIN_ID( MMIPIC_EDIT_EFFECT_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
#ifndef PIC_EDIT_U200_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_PIC_UNDO),
#else
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_CANCEL_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, MMIPIC_OK_CTRL_ID),
#endif
        CREATE_ICONLIST_CTRL(MMIPIC_EDIT_EFFECT_ICONLIST_CTRL_ID),
        END_WIN
};

//save edit window
WINDOW_TABLE(MMIPIC_SAVE_TAB) = 
{
    WIN_FUNC((uint32)HandleSaveMsg),    
    WIN_ID(MMIPIC_EDIT_SAVE_WIN_ID),
    WIN_TITLE(STXT_SAVE),
    CREATE_EDIT_TEXT_CTRL(MMIFILE_FILE_NAME_MAX_LEN + 1,MMIPIC_SAVE_EDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init pic module
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_InitModule(void)
{
    MMIPIC_RegMenuGroup();
}
/*****************************************************************************/
//  Description : open pic editor 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_OpenPicEditorMainWin(
                                           const wchar    *full_path_ptr,     //in
                                           uint16         full_path_len      //in:字节数
                                           )
{
    BOOLEAN result = FALSE;
    PIC_EDIT_FILE_INFO_T*     file_info_ptr = NULL;
    BOOLEAN alloc_ret = FALSE;

    
    do
    {
        file_info_ptr =SCI_ALLOCA(sizeof(PIC_EDIT_FILE_INFO_T));
        if (PNULL == file_info_ptr)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] full_path_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_953_112_2_18_2_45_56_54,(uint8*)"");
            break;
        }
        SCI_MEMSET(file_info_ptr, 0, sizeof(PIC_EDIT_FILE_INFO_T));    
        
        s_selected_folder_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
        if (PNULL == s_selected_folder_ptr)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] s_selected_folder_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_961_112_2_18_2_45_56_55,(uint8*)"");
            break;
        }
        SCI_MEMSET(s_selected_folder_ptr,0,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
        
        g_random_name_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
        if (PNULL == g_random_name_ptr)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] g_random_name_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_969_112_2_18_2_45_56_56,(uint8*)"");
            break;
        }
        SCI_MEMSET(g_random_name_ptr,0,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
        g_random_name_len = 0;

        file_info_ptr->full_path_wstr_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
        if (PNULL == file_info_ptr->full_path_wstr_ptr)
        {
            //MMIPIC_ASSERT_WARNING:"[MMIPIC] file_info_ptr->full_path_wstr_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_978_112_2_18_2_45_56_57,(uint8*)"");
            break;
        }
        SCI_MEMSET(file_info_ptr->full_path_wstr_ptr,0,(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));

        MMI_WSTRNCPY(file_info_ptr->full_path_wstr_ptr,sizeof(wchar)*(full_path_len+1),
            full_path_ptr,full_path_len,
            full_path_len);
        file_info_ptr->full_path_wstr_len = full_path_len;

    	alloc_ret = TRUE;
        result = MMK_CreateWin((uint32*)MMIPIC_EDITOR_MAIN_WIN_TAB, (ADD_DATA)file_info_ptr);
    }while(0);

    if (!alloc_ret)
    {
        if (PNULL != file_info_ptr)
        {
            if (PNULL != file_info_ptr->full_path_wstr_ptr)
            {
                SCI_FREE(file_info_ptr->full_path_wstr_ptr);
                file_info_ptr->full_path_wstr_ptr = PNULL;
            }
            
            SCI_FREE(file_info_ptr);
            file_info_ptr = PNULL;

        }
        
        if (PNULL != s_selected_folder_ptr)
        {
            SCI_FREE(s_selected_folder_ptr);
            s_selected_folder_ptr = PNULL;
        }
        
        if (PNULL != g_random_name_ptr)
        {
            SCI_FREE(g_random_name_ptr);
            g_random_name_ptr = PNULL;
        }

       MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);				

    }

    return result;
}
// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : init PROGRESSBAR_T structure variable
// //  Global resource dependence : 
// //  Author: robert.wang
// //  Note: 
// /*****************************************************************************/
// LOCAL void Init_PROGRESSBAR_T(MMIPIC_PROGRESSBAR_T *bar)
// {
//     int16 i = 0;
//     
//     SCI_MEMSET(bar, 0, sizeof(*bar));
//     bar->img_bg_id = IMAGE_NULL;
//     for (i = 0; i < GROUP_MAX; i++)
//     {
//         bar->group[i].item[0] = TXT_NULL;
//         bar->group[i].item[1] = TXT_NULL;
//     }
//     
// }
// /*****************************************************************************/
// //  Description : get progressbar status for tp
// //  Global resource dependence : 
// //  Author: robert.wang
// //  Note: 
// /*****************************************************************************/
// LOCAL int8 GetProgressBarStatusForTP(GUI_POINT_T point , //int
//                                            MMIPIC_PROGRESSBAR_T bar, //in
//                                            MMIPIC_PROGRESSBAR_RECT_T bar_rect, //in
//                                            RECT_INDEX_E *rect_index, //out
//                                            int16        *grade_level    //out
//                                            )
// {
//     GUI_RECT_T  full_rect = {0};
//     GUI_RECT_T  up_rect = {0};
//     GUI_RECT_T  down_rect = {0};            
//     GUI_RECT_T  grade_rect = {0};
//     RECT_INDEX_E index = RECT_INDEX_MAX;
//     
//     uint16 total = 0;   
//     int16 i = 0;
//     uint8  new_adjust = 0;
//     int16  grade = 0;
//     int16  unit_width = 0; 
// 
//     int16  fill_color_width = 0;
// 
//     fill_color_width = MMIPIC_GetFillColorRectWidth();
//     new_adjust = bar.focus_item;
//     total = bar.total;    
//     for(i = 0; i < total; i++)
//     {
//         unit_width = fill_color_width / bar.group[i].total_level;
//         grade = bar.group[i].grade;
// 
//         //检测是否点击了DOWN 区域
//         down_rect = bar_rect.group[i].item[DOWN_RECT_INDEX];
//         if (GUI_PointIsInRect(point, down_rect))
//         {
//             index = DOWN_RECT_INDEX;
//         }
//         //检测是否点击了UP区域
//         up_rect = bar_rect.group[i].item[UP_RECT_INDEX];
//         if (GUI_PointIsInRect(point, up_rect))
//         {
//             index = UP_RECT_INDEX;
//         }
//         //检测是否点击率设置了颜色值区域
//         grade_rect = bar_rect.group[i].item[GRADE_RECT_INDEX];
//         if (GUI_PointIsInRect(point, grade_rect))
//         {
//             if ( 0 < (point.x - grade_rect.left) && (point.x - grade_rect.left) < 4)
//             {
//                 grade = 0;
//             }
//             else
//             {
//                 grade = (point.x - grade_rect.left) / unit_width + 1;
//             }
// 
//             if (MMIPIC_GRADE_MIN <= grade && grade <= MMIPIC_GRADE_MAX)
//             {
//                 index = GRADE_RECT_INDEX;
//             }
//         }
// 
//         //检查触笔选中了哪个状态条
//         full_rect = bar_rect.group[i].item[FULL_RECT_INDEX];                
//         if (GUI_PointIsInRect(point, full_rect))
//         {
//             new_adjust = i;
//             break;
//         }
// 
// 
//     }
// 
//     *rect_index = index;
//     *grade_level = grade;
// 
//     SCI_TRACE_LOW("[MMIPIC] GetProgressBarStateForTP rect_index = %d, grade_level = %d, new_adjust = %",
//                 *rect_index, *grade_level, new_adjust);
//     return new_adjust;
// }
// #else
/*****************************************************************************/
//  Description : check the grade of point for progress line
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL int16 GetProgressGradeForPoint(GUI_POINT_T point , uint16 focus_item)
{
    GUI_RECT_T  grade_rect = s_bright_bar_rect.group[focus_item].item[GRADE_RECT_INDEX];
    int16       grade = 0;
    int16       fill_color_width = grade_rect.right - grade_rect.left + 1;
    uint8       total_level = s_bright_bar.group[focus_item].total_level;
    int16       unit_width = fill_color_width / total_level;

    if((point.x <= grade_rect.left) || 
        (0 < (point.x - grade_rect.left) && (point.x - grade_rect.left) < (unit_width / 2 -1)))
    {
        grade = 0;
    }
    else if(point.x >= grade_rect.right)
    {
        grade = total_level;
    }
    else
    {
        grade = (point.x - grade_rect.left) / unit_width + 1;
    }

    return grade;
}

/*****************************************************************************/
//  Description : get progressbar status for tp
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL int8 GetProgressBarStatusForTP(GUI_POINT_T point , //int
                                           MMIPIC_PROGRESSBAR_T bar, //in
                                           MMIPIC_PROGRESSBAR_RECT_T bar_rect, //in
                                           RECT_INDEX_E *rect_index, //out
                                           int16        *grade_level    //out
                                           )
{
    GUI_RECT_T  grade_rect = {0};
    GUI_RECT_T  full_rect  = {0};
    RECT_INDEX_E index = RECT_INDEX_MAX;
    
    uint16 total = 0;   
    int16 i = 0;
    uint8  new_adjust = 0;
    int16  grade = 0;
    int16  unit_width = 0; 

    int16  fill_color_width = 0;

    new_adjust = bar.focus_item;
    total = bar.total;    
    for(i = 0; i < total; i++)
    {
        grade_rect  = bar_rect.group[i].item[GRADE_RECT_INDEX];
        full_rect   = bar_rect.group[i].item[FULL_RECT_INDEX];
        fill_color_width = grade_rect.right - grade_rect.left + 1;
        unit_width = fill_color_width / bar.group[i].total_level;
        grade = bar.group[i].grade;

        //检测是否点击率设置了颜色值区域
        if(GUI_PointIsInRect(point, grade_rect))
        {
            grade = GetProgressGradeForPoint(point, i);
            
            if (MMIPIC_GRADE_MIN <= grade && grade <= MMIPIC_GRADE_MAX)
            {
                index = GRADE_RECT_INDEX;
            }
        }
        
        if (GUI_PointIsInRect(point, full_rect))
        {
            new_adjust = i;
            break;
        }
    }

    *rect_index = index;
    *grade_level = grade;

    //SCI_TRACE_LOW:"[MMIPIC] GetProgressBarStateForTP rect_index = %d, grade_level = %d, new_adjust = %"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1214_112_2_18_2_45_57_58,(uint8*)"dd",*rect_index, *grade_level, new_adjust);
    return new_adjust;
}

/*****************************************************************************/
//  Description : init character func
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL void AddCharacterInfoInit()
{
    GUI_RECT_T        display_rect=MMITHEME_GetClientRectEx(MMIPIC_EDIT_MAIN_WIN_ID);
    s_char_info.color_index=4;//MMI_BLACK_COLOR;
    s_char_info.frame_color_index = 4;//MMI_BLACK_COLOR;
    s_char_info.size_type = MMI_PIC_CHAR_FONT_LARGE; //modified by robert.wang for cr145791
    s_char_info.direction_type=MMI_PIC_DIRECTION_HORIZONTAL;
    s_char_info.box=display_rect;
    s_char_info.space_two_chars=1;
    s_char_info.space_two_lines=2;

    //added, @robert.wang, 09-10-22, cr157728
    //clean content
     SCI_MEMSET(s_char_info.content,0x00,sizeof(s_char_info.content));
     s_char_info.length = 0;
    //end cr157728
}

/*****************************************************************************/
//  Description : handle pic editor main win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditorMainWinMsg (
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static PIC_EDIT_FILE_INFO_T * s_file_info_ptr=PNULL;
    static BOOLEAN s_is_decoded =FALSE;
    MMI_WIN_ID_T    alert_win_id = MMIPIC_WAIT_PUBWIN_WIN_ID;
    MMI_WIN_ID_T    nosupport_win_id = MMIPIC_NOSUPPORT_PUBWIN_WIN_ID;
    MMI_WIN_ID_T not_save_alert_id = MMIPIC_NOSAVE_ALERT_WIN_ID;
    uint32       waiting_time_out = 0;
    MMI_PIC_FILE_GET_ERR_E getfile_err_type = MMI_PIC_FILE_GET_ERR_NONE;
    MMI_PIC_ERROR_TYPE_E  pic_result = MMI_PIC_ERR_EXIT;  //added, @robert.wang, 09-10-26, cr158188
    
#ifdef PIC_EDIT_U200_SUPPORT
    uint16          pic_index   = 0;
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIICONLIST_DISP_T  icon_text = {0};
    MMI_PIC_ADJUSTMENT_E adjustment=MMI_PIC_ADJUSTMENT_BRIGHTNESS;
    GUI_BG_T        button_bg = {0};
    MMI_STRING_T    title_txt = {0};
    
#endif

    s_file_info_ptr = (PIC_EDIT_FILE_INFO_T*)MMK_GetWinAddDataPtr(win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: MSG_OPEN_WINDOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1274_112_2_18_2_45_57_59,(uint8*)"");
        MMIPIC_InitProcessParam();
#ifdef PIC_EDIT_U200_SUPPORT
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_MAIN_TITLE_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_MAIN_SAVE_CTRL_ID, MMIPIC_MAIN_CANCEL_CTRL_ID, TRUE);
#endif   
        SetMainWinFormParam(win_id, MMIPIC_EDIT_MAINMENU_FORM_ID, MMIPIC_EDIT_MAINMENU_RAOTATE_ID);
        
        s_operation_times = 0;
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);

            getfile_err_type = MMIPIC_GetCommonFileDataBuf(s_file_info_ptr);
            //allow picture over 200k
            if((MMI_PIC_FILE_GET_ERR_NONE != getfile_err_type) && (MMI_PIC_FILE_OVERSIZE !=getfile_err_type) )
            {
                MMK_CloseWin(MMIFMM_PICTURE_PREVIEW_WIN_ID);            
                if(MMI_PIC_FILE_ASY_READ_ERR ==getfile_err_type)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_READ_FAIL,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);											
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_NOT_SUPPORTED_SIZE,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);											
                }
                MMK_CloseWin(win_id);
            }
            else
            {
                //init
                MMIPIC_PicEditorInit(s_file_info_ptr);
                AddCharacterInfoInit();
                
                MMK_CloseWin(MMIFMM_PICTURE_PREVIEW_WIN_ID);   
                //SCI_TRACE_LOW:"HandleEditorMainWinMsg: enter pic editor  "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1307_112_2_18_2_45_57_60,(uint8*)"");

                if(!s_is_changed)
                {
#ifndef PIC_EDIT_U200_SUPPORT
                    GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, FALSE);
#else
                    GUIBUTTON_SetTextId(MMIPIC_MAIN_CANCEL_CTRL_ID, STXT_RETURN);
#endif
                }
                MMK_SetAtvCtrl(win_id, MMIPIC_EDIT_MAINMENU_FORM_ID);
				SetMainFormGrayed(TRUE);
            }
        break;
        
    case MSG_FULL_PAINT:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: MSG_FULL_PAINT s_is_decoded = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1322_112_2_18_2_45_57_61,(uint8*)"d", s_is_decoded);
        //if(TRUE==s_is_decoded)
        {
            MMIPIC_DisplayImage();    
#ifdef PIC_EDIT_U200_SUPPORT
            SetPicEditTitle(win_id, &title_txt, IMAG_PS_MAIN_TITLE_BG);
#endif
        }
        break;

    case MSG_PIC_GET_DATA_CNF:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: get file date success then begin decode jpg image "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1333_112_2_18_2_45_57_62,(uint8*)"");
        if(MMIPIC_DecodeJpgImage(s_file_info_ptr))
        {
            //alloc undo buffer,init
            if (!MMIPIC_AllocUndoBuf())
            {
                MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);				
                MMK_CloseWin(win_id);
            }
            else
            {
                s_is_decoded=TRUE;
            }
        }
        else
        {
            s_is_decoded=FALSE;
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_NO_SUPPORT,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_PIC_DISPLAY_IND:
        if(MMK_IsOpenWin(alert_win_id))
        {
            MMK_CloseWin(alert_win_id);
        }
        
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg:  decode jpg image success "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1361_112_2_18_2_45_57_63,(uint8*)"");
        
        if (!MMIPIC_MallocMemForPicProcess())
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);				
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPIC_DisplayImage();
			SetMainFormGrayed(FALSE);
        }
        break;
        
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: MSG_GET_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1375_112_2_18_2_45_57_64,(uint8*)"");
        s_is_decoded=TRUE;
        //AppendIconLayer(s_icon_layer_handle);
//         GUIICONLIST_SetCurIconIndex(0, iconctrl_id); //设置当前icon的index值

        //重新设置cancel控件的现实图片，没有图片没有改变时为return，否则为cancel
        if(s_is_changed)
        {
#ifndef PIC_EDIT_U200_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, TXT_PIC_UNDO, FALSE);
#else
            GUIBUTTON_SetTextId(MMIPIC_MAIN_CANCEL_CTRL_ID, TXT_PIC_UNDO);
#endif
        }
        else
        {
#ifndef PIC_EDIT_U200_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, FALSE);
#else
            GUIBUTTON_SetTextId(MMIPIC_MAIN_CANCEL_CTRL_ID, STXT_RETURN);
#endif
        }
        break;
        
    case MSG_LOSE_FOCUS:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: MSG_LOSE_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1400_112_2_18_2_45_57_65,(uint8*)"");
        break;
        
    case MSG_TIMER:
        break;
        
// #ifndef PIC_EDIT_U200_SUPPORT
//     case MSG_APP_MENU:
//     case MSG_APP_OK:
//         MMK_CreateWin((uint32 *)MMIPIC_EDIT_MENU_WIN_TAB,PNULL);
//         break;
// #else

    case MSG_APP_OK:         
    case MSG_CTL_OK:
#ifndef PIC_EDIT_U200_SUPPORT
        SaveEditImg();
        break;
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        switch (MMK_GetActiveCtrlId(win_id))
        {
        case MMIPIC_EDIT_MAINMENU_RAOTATE_ID:
            MMK_CreateWin((uint32*)MMIPIC_ROTATE_WIN_TAB, PNULL);
            break;
            
        case MMIPIC_EDIT_MAINMENU_ADJUST_ID:
            MMK_CreateWin((uint32*)MMIPIC_ADJUS_BRI_CON_WIN_TAB, (ADD_DATA)MMI_PIC_ADJUSTMENT_BRIGHTNESS);
            break;
                        
        case MMIPIC_EDIT_MAINMENU_EFFECT_ID:
            MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, PNULL);
            break;
#ifdef PIC_EDIT_FRAME_SUPPORT    
        case MMIPIC_EDIT_MAINMENU_FRAME_ID:
            MMK_CreateWin((uint32*)MMIPIC_FRAME_WIN_TAB, PNULL);
            break;
#endif            
        case MMIPIC_EDIT_MAINMENU_CLIP_ID:
            MMK_CreateWin((uint32*) MMIPIC_PASTE_PREVIEW_WIN_TAB, PNULL);
            break;
            
        case MMIPIC_EDIT_MAINMENU_TEXT_ID:
#ifndef PIC_EDIT_U200_SUPPORT
            MMK_CreateWin((uint32*) MMIPIC_ADD_CHARACTER_WIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32*)MMIPIC_CHARACTER_PREVIEW_WIN_TAB, PNULL);
#endif
            break;
        default:
            break;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:   
        if(s_is_changed)
        {
            //撤销
            MMIPIC_Undo();
            MMIPIC_UndoFileInfo();
            s_is_changed=FALSE;
#ifndef PIC_EDIT_U200_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, FALSE);
#else
            GUIBUTTON_SetTextId(MMIPIC_MAIN_CANCEL_CTRL_ID, STXT_RETURN);
#endif            
            if(s_operation_times > 0)
            {
                s_operation_times --;
            }
            
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        else//没有修改或者撤销过都只能作为return,因为只能撤销一次操作
        {
            if(s_operation_times > 0)
            {
                MMIPUB_OpenAlertWinByTextId(&waiting_time_out, 
                    TXT_PIC_NOSAVE,
                    TXT_NULL,
                    IMAGE_PUBWIN_WARNING,
                    &not_save_alert_id,
                    PNULL,
                    MMIPUB_SOFTKEY_OKCANCEL,
                    HandlePicEditorExitAlertWin);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifndef PIC_EDIT_U200_SUPPORT
        MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
#else
        if ((PNULL != param))
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            case MMIPIC_MAIN_SAVE_CTRL_ID:
                //if(s_is_changed)
                {
                    //打开编辑名称窗口
                    SaveEditImg();
                }
                break;
            case MMIPIC_MAIN_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
                
            default:
                MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
                break;
            }
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_PAINT:
       // MMIPIC_DisplayImage();
        break;
                
    case MSG_APP_RED:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: MSG_APP_RED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1527_112_2_18_2_45_57_66,(uint8*)"");
        MMK_ReturnIdleWin();
        break;
	
    case MSG_PROMPTWIN_OK: 
        MMIPUB_CloseQuerytWin(PNULL);

        if(s_is_changed)
        {
            //打开编辑名称窗口
            SaveEditImg();
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL: //MSG_ACC_MSGBOX_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_APP_UP:         
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
//         SCI_TRACE_LOW("HandleEditorMainWinMsg: MSG_APP_DIRECTION");
//         MMIPIC_MoveDisplayImage(msg_id);
//         MMK_SendMsg(win_id, msg_id, PNULL);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CLOSE_WINDOW:
        //SCI_TRACE_LOW:"HandleEditorMainWinMsg: MSG_CLOSE_WINDOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1561_112_2_18_2_45_57_67,(uint8*)"");

        //SCI_TRACE_LOW:"Editor CloseWin s_operation_times = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1563_112_2_18_2_45_57_68,(uint8*)"d",s_operation_times);
        //不可撤销，则return(目前只能撤销一次)
//         if (s_is_changed)
//         {
//             SaveEditImg();
//         }
        
        //图片改变才需要更新picview
        if( s_is_saved )
        {
#ifdef PIC_VIEWER_SUPPORT        
            //MMIAPIPICVIEW_ReloadFile();
            if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
            {
                PIC_EDIT_FILE_INFO_T  param_to_save_as_win={0};
                uint16 param_index=0;
                FILEARRAY_DATA_T file_info_ptr={0};
                wchar   abs_preview_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
                uint16   abs_preview_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
                wchar   abs_saved_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
                uint16   abs_saved_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
                BOOLEAN  is_same_path=FALSE;
                MMIAPIPICVIEW_ReloadFile(); 
#ifndef MMI_PICVIEW_MINI_SUPPORT
                // ??è?μ±?°?ˉààμ????t?
                MMIAPIPICVIEW_GetCurPicFileInfo(MMIAPIPICVIEW_IsPicArrayFromArrayLib(),&file_info_ptr);
#endif
                MMIAPIFMM_SplitFullPathExt(file_info_ptr.filename,MMIAPICOM_Wstrlen(file_info_ptr.filename),abs_preview_path, &abs_preview_path_length, PNULL, PNULL, PNULL, PNULL);
                
                
                //??è?±à?-oóí????ù′?′￠μ????t?D
                MMIPIC_GetFileInfo(&param_to_save_as_win);
                MMIAPIFMM_SplitFullPathExt(param_to_save_as_win.full_path_wstr_ptr,MMIAPICOM_Wstrlen(param_to_save_as_win.full_path_wstr_ptr),abs_saved_path, &abs_saved_path_length, PNULL, PNULL, PNULL, PNULL);
                
                is_same_path=MMIAPICOM_Wstrncmp(abs_preview_path, abs_saved_path, MMIAPICOM_Wstrlen(abs_preview_path));
                
                if(!is_same_path)
                {
//                     if(MMIAPIPICVIEW_IsPicArrayFromArrayLib())
                    {
                        MMK_PostMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL,PNULL);
                    }
//                     else
//                     {
//                         MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),     //in, filearray?μ,
//                             param_to_save_as_win.full_path_wstr_ptr);//in,ìí?óμ????tè??·?
//                         MMK_PostMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_UPDATE_LIST, (DPARAM)&param_index,sizeof(uint16));
//                         
//                     }
                }
                
                
            }
            else
#endif
            {
                MMIAPIPICVIEW_ReloadFile(); 
            }
        }
        else//add for NEWMS00108303
        {
            MMIAPIPICVIEW_GetAndResetEditPicIndex();
        }

        if(PNULL!=s_file_info_ptr)
        {
            if (PNULL != s_file_info_ptr->full_path_wstr_ptr)
            {
                SCI_FREE(s_file_info_ptr->full_path_wstr_ptr);
                s_file_info_ptr->full_path_wstr_ptr = PNULL;
            }
            SCI_FREE(s_file_info_ptr);
            s_file_info_ptr = PNULL;
        }
        if(PNULL != s_selected_folder_ptr)
        {
            SCI_FREE(s_selected_folder_ptr);
            s_selected_folder_ptr = PNULL;
        }
        MMIPIC_PicEditorExit();
        //enter piceditor disable 3D;exit piceditor enable 3D
        //MMI_Enable3DMMI(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_PIC_EDITOR);
#endif
        s_is_decoded=FALSE;
        s_is_changed=FALSE;
        s_is_saved = FALSE;
        g_can_be_default_saved = TRUE;
        if(PNULL != g_random_name_ptr)
        {
            SCI_FREE(g_random_name_ptr);
            g_random_name_ptr = PNULL;
        }
        s_operation_times = 0;

//        DestroyIconLayer(&s_icon_layer_handle);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : hand main menu win msg
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// 
// LOCAL MMI_RESULT_E  HandleEditorMenuWinMsg (
//                                             MMI_WIN_ID_T        win_id,
//                                             MMI_MESSAGE_ID_E    msg_id, 
//                                             DPARAM      param
//                                             )
// {
//     MMI_RESULT_E    result = MMI_RESULT_TRUE;
//     MMI_MENU_GROUP_ID_T     group_id = 0; 
//     MMI_MENU_ID_T           menu_id  = 0;
//     MMI_PIC_EFFECT_E  effect=MMI_PIC_EFFECT_GRAY;
//     MMI_PIC_ADJUSTMENT_E adjustment=MMI_PIC_ADJUSTMENT_BRIGHTNESS;
//     MMI_PIC_ERROR_TYPE_E  pic_result = MMI_PIC_ERR_EXIT; //added, @robert.wang, 09-10-26, cr158188
// 
//     switch(msg_id)
//     {
//     case MSG_OPEN_WINDOW:
// 
//         if(!s_is_changed)
//         {
//             GUIMENU_SetItemGrayed(MMIPIC_FUNC_MENU_CTRL_ID,MENU_PIC_EDIT_FUNC_OPT,ID_PIC_EDIT_UNDO,TRUE);
//         }
// 
//         if(!MMIPIC_IfBmpCanBePasted())
//         {
//             GUIMENU_SetItemGrayed(MMIPIC_FUNC_MENU_CTRL_ID,MENU_PIC_EDIT_INSERT_OPT,ID_PIC_EDIT_PASTE,TRUE);
//         }
//         
//         if(!MMIPIC_IfBmpCanBeAddCharacter())
//         {
//             GUIMENU_SetItemGrayed(MMIPIC_FUNC_MENU_CTRL_ID,MENU_PIC_EDIT_INSERT_OPT,ID_PIC_EDIT_CHARACTER,TRUE);
//         }
//         
//         // 设置参数
//         GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
//         MMK_SetAtvCtrl(win_id,MMIPIC_FUNC_MENU_CTRL_ID);
//         break;
//     case MSG_FULL_PAINT:
//         
//         break;
//         
//     case MSG_GET_FOCUS:
//         
//         break;
//         
//     case MSG_TIMER:
//         
//         break;
//         
//     case MSG_CTL_PENOK:
//     case MSG_APP_OK:
//     case MSG_CTL_OK:
//     case MSG_APP_WEB:
//     case MSG_CTL_MIDSK:
//         GUIMENU_GetId(MMIPIC_FUNC_MENU_CTRL_ID, &group_id, &menu_id);
//         switch (menu_id){
//         case ID_PIC_EDIT_SAVE:
//             //modified, @robert.wang, 09-10-26, cr158188
//             pic_result = MMIPIC_Save(TRUE);
//             switch(pic_result)
//             {
//             case MMI_PIC_SUCCESS:
//                 s_operation_times = 0;
//                 s_is_saved = TRUE;
//                 MMIPIC_ShowMsg(pic_result);
//                 break;
//             case MMI_PIC_ERR_NO_SPACE:
//             case MMI_PIC_ERR_LONG_FILENAME:
//             case MMI_PIC_ERR_INVALID_NAME:
//             case MMI_PIC_ERR_QUERY_OVERWRITE:
//                 MMIPIC_ShowMsg(pic_result);
//                 break;
//             default:
//                 MMIPIC_ShowMsg(pic_result);
//                 MMIPIC_ExitFromPicEditor();
//                 break;
//             }
//             //end cr158200
// 
//             break;
//             
//         case ID_PIC_EDIT_SAVEAS:                    
//             MMIPic_OpenSelectFolderWin();
//             break;
//             
//         case ID_PIC_EDIT_UNDO:
//             MMIPIC_Undo();
//             MMIPIC_UndoFileInfo();
//             s_is_changed=FALSE;
//             if(s_operation_times > 0)
//             {
//                 s_operation_times --;
//             }
//             break;
// 
//         case ID_PIC_EDIT_ROTATE:
//             MMK_CreateWin((uint32*) MMIPIC_ROTATE_WIN_TAB, PNULL);
//             break;
//             
//             //insert    
//         case ID_PIC_EDIT_PASTE:
//             MMK_CreateWin((uint32*) MMIPIC_PASTE_PREVIEW_WIN_TAB, PNULL);
//             break;
//         case ID_PIC_EDIT_CHARACTER:
//             MMK_CreateWin((uint32*) MMIPIC_ADD_CHARACTER_WIN_TAB, PNULL);
//             break;
// 
// #ifdef PIC_EDIT_FRAME_SUPPORT    
//         case ID_PIC_EDIT_FRAME:
//             MMK_CreateWin((uint32*)MMIPIC_FRAME_WIN_TAB, PNULL);
//             break;
// #endif
//             
//             //adjust
//         case ID_PIC_EDIT_SIZE:
//             MMK_CreatePubListWin((uint32*)MMIPIC_RESIZE_MENU_WIN_TAB, PNULL);
//             break;
//         case ID_PIC_EDIT_COLOR:
//             MMK_CreateWin((uint32*)MMIPIC_RGB_WIN_TAB, PNULL);
//             break;             
//         case ID_PIC_EDIT_BRIGHTNESS:
//             adjustment=MMI_PIC_ADJUSTMENT_BRIGHTNESS;
//             SCI_TRACE_LOW("[MMIPIC] ajustment = %d",adjustment);
//             MMK_CreateWin((uint32*)MMIPIC_ADJUS_BRI_CON_WIN_TAB, (ADD_DATA)adjustment);
//             break;
//         case ID_PIC_EDIT_CONSTRACT:
//             adjustment=MMI_PIC_ADJUSTMENT_CONSTRACT;
//             SCI_TRACE_LOW("[MMIPIC] ajustment = %d",adjustment);
//             MMK_CreateWin((uint32*)MMIPIC_ADJUS_BRI_CON_WIN_TAB, (ADD_DATA)adjustment);
//             break;             
//             
//         case ID_PIC_EDIT_SATURATION:
//             adjustment=MMI_PIC_ADJUSTMENT_SATURATION;
//             MMK_CreateWin((uint32*)MMIPIC_ADJUST_SAT_HUE_WIN_TAB, (ADD_DATA)adjustment);
//             break;
//         case ID_PIC_EDIT_HUE:
//             adjustment=MMI_PIC_ADJUSTMENT_HUE;
//             MMK_CreateWin((uint32*)MMIPIC_ADJUST_SAT_HUE_WIN_TAB, (ADD_DATA)adjustment);
//             break;
//             
//             //effort
//         case ID_PIC_EDIT_GRAY:
//             effect=MMI_PIC_EFFECT_GRAY;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_SEPIA:
//             effect=MMI_PIC_EFFECT_SEPIA;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_SMOOTH:
//             effect=MMI_PIC_EFFECT_SMOOTH;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_EMBOSS:
//             effect=MMI_PIC_EFFECT_EMBOSS;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_SHARPEN:
//             effect=MMI_PIC_EFFECT_SHARPEN;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_DIFFUSION:
//             effect=MMI_PIC_EFFECT_DIFFUSION;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_EXPOSURE:
//             effect=MMI_PIC_EFFECT_EXPOSURE;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_NEGATIVE_RGB:
//             effect=MMI_PIC_EFFECT_NEGATIVE_RGB;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//         case ID_PIC_EDIT_DENOISE:
//             effect=MMI_PIC_EFFECT_DENOISE;
//             MMK_CreateWin((uint32*)MMIPIC_EFFECT_WIN_TAB, (ADD_DATA)effect);
//             break;
//             
//             
//         default:
//             result = MMI_RESULT_FALSE;
//             break;
//         }
//         
//         MMK_CloseWin(win_id);           
//         break;
//         
//         case MSG_APP_CANCEL:
//         case MSG_CTL_CANCEL:
//         case MSG_LOSE_FOCUS:
//             MMK_CloseWin(win_id);
//             break;
//             
//         case MSG_APP_LEFT:
//             
//             break;
//             
//         case MSG_APP_RIGHT:
//             
//             break;
//             
//         case MSG_TP_PRESS_DOWN:
//             
//             break;
//             
//         case MSG_CLOSE_WINDOW:
//             
//             break;
//             
//         default:
//             result = MMI_RESULT_FALSE;
//             break;
//     }
//     return (result);
// }
// #endif

/*****************************************************************************/
//  Description : get file name and suffix
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
void MMIPIC_GetNameAndSuffix(
                             uint16          *name_suffix_ptr,   //文件名
                             uint16          name_suffix_len,    //文件名长度
                             uint16    *name_ptr,          //没有后缀名的文件名
                             uint16    *suffix_ptr         //后缀名
                             )
{
    int32              i = 0;
    
    if (PNULL == name_suffix_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetNameAndSuffix name_suffix_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1905_112_2_18_2_45_58_69,(uint8*)"");
        return ;
    }
    
    if (0 == name_suffix_len)
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetNameAndSuffix name_suffix_len is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1911_112_2_18_2_45_58_70,(uint8*)"");
        return ;
    }


    if ((PNULL == name_ptr)
        ||(PNULL == suffix_ptr)
       )
    {
        //SCI_TRACE_LOW:"[MMIPIC] MMIPIC_GetNameAndSuffix name_ptr or suffix_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_1920_112_2_18_2_45_58_71,(uint8*)"");
        return ;
    }
    
    for (i=(name_suffix_len-1); i>=0; i--)
    {
        if (MMIPIC_DOT == name_suffix_ptr[i])
        {
            break;
        }
    }
    if (i < 0)
    {
        i=name_suffix_len;
    }
    
    MMI_WSTRNCPY(name_ptr,i,
        name_suffix_ptr,i,
        i);	
    
	
    MMI_WSTRNCPY(suffix_ptr,(name_suffix_len-i),
        (name_suffix_ptr+i),(name_suffix_len-i),
        (name_suffix_len-i));  	
    
}

/*****************************************************************************/
//  Description : save as a file and show msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL  MMI_PIC_ERROR_TYPE_E SaveAsFileAndShowMsg(const wchar *full_path_ptr,     //in
                                                          uint16     full_path_len       //in, 双字节为单位
                                                          )
{
    MMI_PIC_ERROR_TYPE_E pic_result = MMI_PIC_ERR_EXIT;
    
    pic_result = MMIPIC_SaveAs(full_path_ptr, full_path_len);
    switch(pic_result)
    {
    case MMI_PIC_SUCCESS:
       //change the status while picture is changed,
       //or waiting for directly copy  file
       //see HandleCopyFileWaitWinMsg()
        if (MMIPIC_IsPictureChanged())  
        {
            s_operation_times = 0;
            g_can_be_default_saved = TRUE;
            s_is_saved = TRUE;
            //MMIPIC_ShowMsg(pic_result);
        }
        break;
    case MMI_PIC_ERR_NO_SPACE:
    case MMI_PIC_ERR_LONG_FILENAME:
    case MMI_PIC_ERR_INVALID_NAME:
    case MMI_PIC_ERR_QUERY_OVERWRITE:
        MMIPIC_ShowMsg(pic_result);
        break;
    default:
        MMIPIC_ShowMsg(pic_result);
        MMIPIC_ExitFromPicEditor();
        break;
    }

    return pic_result;         
}
/*****************************************************************************/
//  Description : handle save as windows msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePicSaveAsWinMsg(
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM       param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint16                              max_ucs2_len = 0;
    PIC_EDIT_FILE_INFO_T   file_info = {0};//full path info
    wchar       image_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
	
    uint16      image_name_len = 0;
    MMI_CTRL_ID_T ctrl_id=MMIPIC_SAVE_AS_EDITBOX_CTRL_ID;
    MMI_STRING_T      name_str = {0};
    wchar       not_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
	
    uint16      not_suffix_name_len = 0;
    
    BOOLEAN     disk_result = FALSE;
    //BOOLEAN     is_udisk = FALSE;
    
    static uint16        s_ucs2_suffix[MMIPIC_SUFFIX_MAX_LEN+1] = {0};
    static uint16      s_ucs2_suffix_len=0;
    static uint16      s_ucs2_suffix_without_dot[MMIPIC_SUFFIX_MAX_LEN+1] = {0};
    static uint16      s_ucs2_suffix_len_without_dot=0;
    static uint16       s_device_name[MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
    static uint16       s_device_len=0;     
    
    static uint16       s_dir_name[MMIFILE_DIR_NAME_MAX_LEN+1] = {0};
    static uint16       s_dir_len=0;            
    
    static uint16       s_new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    static uint16       s_new_path_len = MMIFILE_FULL_PATH_MAX_LEN ;
    
    uint16       new_name_with_suffix[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16       new_name_with_suffix_len = 0;
    uint32      free_high_word = 0;
    uint32      free_low_word = 0;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    static uint16 selected_folder_len = 0;	
    
    MMI_PIC_ERROR_TYPE_E  saveas_ret = MMI_PIC_ERR_EXIT;
    //FILE_DEV_E_T          file_dev = FS_UDISK;
    BOOLEAN               is_check_memory_success = FALSE;

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    	//set left softkey
        
	GUIEDIT_SetSoftkey(ctrl_id,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
		
        SCI_MEMSET(s_new_full_path, 0, (sizeof(wchar) * MMIFILE_FULL_PATH_MAX_LEN));
        SCI_MEMSET(s_ucs2_suffix_without_dot, 0, (sizeof(wchar) * MMIPIC_SUFFIX_MAX_LEN));
        SCI_MEMSET(s_ucs2_suffix, 0, (sizeof(wchar) * MMIPIC_SUFFIX_MAX_LEN));
        SCI_MEMSET(s_dir_name, 0, (sizeof(wchar) * MMIFILE_DIR_NAME_MAX_LEN));
        SCI_MEMSET(s_device_name, 0, (sizeof(wchar) * MMIFILE_DEVICE_NAME_MAX_LEN));
        MMIPIC_GetFileInfo(&file_info);
        selected_folder_len = MMIPIC_GetFilePathLen(s_selected_folder_ptr);	

        MMIAPIFMM_SplitFullPath(file_info.full_path_wstr_ptr,file_info.full_path_wstr_len,
			s_device_name,&s_device_len,s_dir_name,&s_dir_len,
			(uint16 *)image_name,&image_name_len);		
		
        MMIPIC_GetNameAndSuffix((uint16 *)image_name,image_name_len,
			(uint16 *)not_suffix_name,s_ucs2_suffix);

		s_ucs2_suffix_len_without_dot = MMIPIC_SUFFIX_MAX_LEN -1;
        MMIAPIFMM_SplitFileName(image_name,image_name_len,
		                             not_suffix_name,&not_suffix_name_len,
		                             s_ucs2_suffix_without_dot,&s_ucs2_suffix_len_without_dot);

//fullpath  d           :   \      abc         \             auto              .          jpg		
//            --                     ---                       ----                        ---
//      s_device_len         s_dir_len        not_suffix_name_len       s_ucs2_suffix_len_without_dot

        s_ucs2_suffix_len = s_ucs2_suffix_len_without_dot + 1;    //the interface MMIAPIFMM_SplitFileName() can only get the suffix length without ".",so need add 1
        if( MMIFILE_FULL_PATH_MAX_LEN > ( selected_folder_len + 1 + s_ucs2_suffix_len ) )
        {
            max_ucs2_len = MMIFILE_FULL_PATH_MAX_LEN - selected_folder_len - 1 - s_ucs2_suffix_len;
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MAX_DEPTH);                   
            MMK_CloseWin(win_id);
	     return result;
        }
		
		GUIEDIT_SetTextMaxLen(ctrl_id, max_ucs2_len, max_ucs2_len);
		
        GUIEDIT_SetString(ctrl_id, not_suffix_name, image_name_len - s_ucs2_suffix_len);

        
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id,ctrl_id);		
        break;

    case MSG_FULL_PAINT:
        
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef PIC_EDIT_U200_SUPPORT
        if(PNULL != param)
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
        }
#endif //PIC_EDIT_U200_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id, &name_str);
        if (MMIAPICOM_FileNameIsValid(&name_str))
        {
            MMI_WSTRNCPY(new_name_with_suffix,name_str.wstr_len,
                name_str.wstr_ptr,name_str.wstr_len,
                name_str.wstr_len);				
            
            MMI_WSTRNCPY(new_name_with_suffix+name_str.wstr_len,s_ucs2_suffix_len,
                s_ucs2_suffix,s_ucs2_suffix_len,
                s_ucs2_suffix_len);								
            new_name_with_suffix_len=name_str.wstr_len+s_ucs2_suffix_len;            

            s_new_path_len = selected_folder_len+1+new_name_with_suffix_len;	            

            if (s_new_path_len <= MMIFILE_FULL_PATH_MAX_LEN)
            {
            
                MMI_WSTRNCPY(s_new_full_path,selected_folder_len,
                    s_selected_folder_ptr,selected_folder_len,
                    selected_folder_len);
                
                s_new_full_path[selected_folder_len] = MMIFILE_SLASH;	    	
                
                MMI_WSTRNCPY(s_new_full_path+selected_folder_len + 1,new_name_with_suffix_len,
                    new_name_with_suffix,new_name_with_suffix_len,
                    new_name_with_suffix_len);		
            
                device_type = MMIAPIFMM_GetDeviceTypeByPath(s_new_full_path, MMIFILE_DEVICE_LEN);
                if(device_type >= MMI_DEVICE_NUM || !MMIAPIFMM_GetDeviceTypeStatus(device_type))
                {
                    device_type = MMIAPIFMM_GetDefaultDisk();
                }

                MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(device_type),
					                       MMIAPIFMM_GetDevicePathLen(device_type),
										   &free_high_word,
										   &free_low_word);
                if (MMIPIC_IsPictureChanged())
                {
                    if ((0 < free_high_word) || 
                    (MMIPIC_GetEditBufferSize() < free_low_word))
                    {
                        disk_result = TRUE;
                    }
                }
                else 
                {
                    PIC_EDIT_FILE_INFO_T        edit_file_info = {0};
                    uint32 edit_file_size=0;

                    MMIPIC_GetFileInfo(&edit_file_info);

                    MMIAPIFMM_GetFileInfo(edit_file_info.full_path_wstr_ptr, edit_file_info.full_path_wstr_len, &edit_file_size, NULL, NULL);

                    if ((0 < free_high_word) || 
                    (edit_file_size <= free_low_word))
                    {
                        disk_result = TRUE;
                    }
                    
                 //   SCI_TRACE_LOW("HandlePicSaveAsWinMsg, edit_file_size=%d",edit_file_size);
                }
                
               // SCI_TRACE_LOW("HandlePicSaveAsWinMsg, disk_result=%d,free_low_word=%d",disk_result,free_low_word);

                if(disk_result)
                {
                    if(!MMIAPIFMM_IsFileExist(s_new_full_path,s_new_path_len))
                    {
                        saveas_ret = SaveAsFileAndShowMsg(s_new_full_path,s_new_path_len);
                        if (MMI_PIC_SUCCESS != saveas_ret)
                        {
                            return result;
                        }
                        MMK_CloseWin(win_id);
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_FILE_EXIST_OVERWRITE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    }
                }
                else
                {
                    is_check_memory_success = MMIAPIFMM_CheckMemoryCard(device_type);
                    //除磁盘格式错误外的
                    if(is_check_memory_success)
                    {
                        MMIAPIFMM_PromptFileErrorType(PNULL,SFS_ERROR_NO_SPACE, device_type);
                    }
                }          
            }
            else
            {
                MMIPIC_ShowMsg(MMI_PIC_ERR_LONG_FILENAME);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);//TXT_DC_NAMEINVALID
        }
        
        break;
        
    case MSG_APP_CANCEL:
//for keypad right softkey and red key is pressed.Both the keys cause the MSG_CTL_CANCEL message,because exit from the input control        
    case MSG_CTL_CANCEL:        
        MMK_CloseWin(win_id);
        
        break;
    case MSG_PROMPTWIN_OK: 
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(PNULL);
        
        saveas_ret = SaveAsFileAndShowMsg(s_new_full_path,s_new_path_len);   
        if (MMI_PIC_SUCCESS != saveas_ret)
        {
            return result;
        }
        break;
        
    case  MSG_PROMPTWIN_CANCEL: 
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_APP_LEFT:
        
        break;
        
    case MSG_APP_RIGHT:
        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
    
}
/*****************************************************************************/
//  Description : hand zoom win
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleZoomWinMsg (
                                      MMI_WIN_ID_T      win_id,
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM            param
                                      )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 设置参数
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, 0, STXT_RETURN, FALSE);
        break;
    case MSG_FULL_PAINT:
        //MMIPIC_DisplayZoomPreviewImage();
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
    case MSG_APP_OK:
        
        
        
        break;
        
    case MSG_APP_CANCEL:
        
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_APP_LEFT:
        
        break;
        
    case MSG_APP_RIGHT:
        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle rotate win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRotateWinMsg (
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM          param
                                        )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    MMI_WIN_ID_T error_alert_id = MMIPIC_ERROR_WIN_ID;
    BOOLEAN is_in_rect = FALSE;
    
#ifdef PIC_EDIT_U200_SUPPORT
    MMI_STRING_T        title_txt = {0};
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPIC_CopyOrgImgDataToOutputImgData();
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_ROTATE_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_ROTATE_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
#else
        s_icon_index=0;
#endif
        if( !MMIPIC_AppendRotatePreviewImageData() )
        {
	        MMIPIC_FreeRotatedPreviewImage();
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        break;
        
    case MSG_FULL_PAINT:
        MMIPIC_DisplayAllRotatedPreviewImage(s_icon_index);
#ifdef PIC_EDIT_U200_SUPPORT        
        MMI_GetLabelTextByLang(TXT_PIC_ROTATE_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_ROTATE_BG);
#endif
    break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
        
#ifdef PIC_EDIT_U200_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            case MMIPIC_OK_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
                
            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMIPIC_OnlySaveOrgImgToUndoBuf();//保存undo buf
        if(!MMIPIC_GetRotatedImageData(s_icon_index))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        
        MMIPIC_CopyOutputImgDataToOrgImgData();
        MMIPIC_CopyOutputRectToOrgRect();
        RecordChangedTimes();
        MMK_CloseWin(win_id); 
        break;
                
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_APP_UP:
        if(MMI_ROTATION_DEGREE_90 == s_icon_index)
        {
            s_icon_index = MMI_ROTATION_DEGREE_270;
        }
        else
        {
            if(MMI_ROTATION_DEGREE_180 == s_icon_index)
            {
                s_icon_index = MMI_ROTATION_DEGREE_MIRROR;
            }
            else
            {
                s_icon_index=s_icon_index-2;
            }
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_APP_DOWN:
        if(MMI_ROTATION_DEGREE_270 == s_icon_index)
        {
            s_icon_index = MMI_ROTATION_DEGREE_90;
        }
        else
        {
            if(MMI_ROTATION_DEGREE_MIRROR == s_icon_index)
            {
                s_icon_index = MMI_ROTATION_DEGREE_180;
            }
            else
            {
                s_icon_index=s_icon_index+2;
            }
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_APP_LEFT:
        if(MMI_ROTATION_DEGREE_90 == s_icon_index)
        {
            s_icon_index = MMI_ROTATION_DEGREE_MIRROR;
        }
        else
        {
            s_icon_index--;
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_APP_RIGHT:
        if(MMI_ROTATION_DEGREE_MIRROR == s_icon_index)
        {
            s_icon_index = MMI_ROTATION_DEGREE_90;
        }
        else
        {
            s_icon_index++;
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(MMIPIC_HandleRotatedPreviewTpDown(point,&is_in_rect))
            {
                RecordChangedTimes();
                MMK_CloseWin(win_id);
            }
            else if(is_in_rect)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
                MMIPIC_ExitFromPicEditor(); 
                return (result);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        MMIPIC_FreeRotatedPreviewImage();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle paste image preview win msg
//  Global resource dependence : 
//  Author:vincent.shang
//  Note:  yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePasteEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E                    result      = MMI_RESULT_TRUE;
    uint16                          pic_index   = 0;
    MMI_CTRL_ID_T                   ctrl_id     = MMIPIC_VIEWPIC_ICONLIST_CTRL_ID;
    uint32                          img_id      = 0;
    GUIANIM_DATA_INFO_T             data_info   = {0};
    GUIICONLIST_DISP_T              icon_list   = {0};
    int32                           ret_value   = 0;
    uint16                          icon_width  = 0;
    uint16                          icon_height = 0;
    MMI_IMAGE_ID_T                  first_img   = IMAGE_PIC_ANGEL;
#ifdef PIC_EDIT_U200_SUPPORT    
    MMI_STRING_T                    title_txt   = {0};
    GUI_RECT_T                      icon_rect   = MMITHEME_GetClientRectEx(win_id);
#endif
    GUI_BG_T                        bg_info     = {0};
	GUIIMG_BITMAP_T					output_img_info = {0};
    MMI_WIN_ID_T                    nosupport_win_id = MMIPIC_NOSUPPORT_PUBWIN_WIN_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:        
        DisplayBackgroudImg(win_id);
#ifndef PIC_EDIT_U200_SUPPORT
        GUIICONLIST_SetDispCustomerSoftkey(ctrl_id, TRUE);
#else
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_CLIP_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_EDIT_OK_CTRL_ID, MMIPIC_EDIT_CANCEL_CTRL_ID, FALSE);
        
        icon_rect.top += MMIPICEDIT_TITLE_HEIGHT;
        GUIICONLIST_SetRect(ctrl_id, &icon_rect);
#endif  
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id  = IMAG_PS_BG;
        GUIICONLIST_SetBg(ctrl_id, &bg_info);
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, first_img, win_id);
		MMIPIC_GetBitmapInfo(&output_img_info);
        if((icon_width > output_img_info.img_width) || (icon_height > output_img_info.img_height))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_TOO_SMALL_SIZE,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);											
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIPIC_PASTE_PREVIEW_WIN_ID);
        }
        icon_width  = MAX(icon_width, MMITHEME_ICON_SHOW_WIDTH);
        icon_height = MAX(icon_height, MMITHEME_ICON_SHOW_HEIGHT);
        GUIICONLIST_SetIconWidthHeight(ctrl_id, icon_width, icon_height);
        GUIICONLIST_SetTotalIcon(ctrl_id, MMIPIC_PASTE_IMAGE_NUM);
        GUIICONLIST_SetCurIconIndex(0,ctrl_id);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        DisplayBackgroudImg(win_id);
#ifdef PIC_EDIT_U200_SUPPORT    
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_CLIP_BG);
#endif
        break;
#ifndef PIC_EDIT_U200_SUPPORT    
    case MSG_CTL_ICONLIST_APPEND_TEXT:
        //get pic index and item index
        pic_index = *((uint16 *)param);

        //set icon list       
        MMI_GetLabelTextByLang(TXT_PIC_CLIP, &icon_list.name_str);
        
        icon_list.softkey_id[0] = STXT_SELECT;
        icon_list.softkey_id[1] = TXT_NULL;
        icon_list.softkey_id[2] = STXT_RETURN;

        //append icon
        GUIICONLIST_AppendText(pic_index,ctrl_id,&icon_list);
        break;
#endif
    case MSG_CTL_ICONLIST_APPEND_ICON:
        //get pic index and item index
        pic_index = *((uint16 *)param);

        data_info.img_id = first_img + pic_index;
        
        //append icon
        GUIICONLIST_AppendIcon(pic_index,ctrl_id,&data_info,NULL);
        break;
        
#ifdef PIC_EDIT_U200_SUPPORT    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            case MMIPIC_EDIT_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
                
            case MMIPIC_EDIT_OK_CTRL_ID:
            case MMIPIC_VIEWPIC_ICONLIST_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;

            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if(!is_pic_edited)
        {
            is_pic_edited = TRUE;
        }
        pic_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
        img_id    = first_img + pic_index;
        ret_value = MMIPIC_GetCurrentPasteImageData(img_id, win_id);
        if (ret_value)
        {
            //打开图像浏览窗口
            MMK_CreateWin((uint32*)MMIPIC_PASTE_PREVIEW_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
            MMIPIC_SetOSDImagePosition();
        }
        break;
                
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(!is_pic_edited)
        {
            MMK_CloseWin(MMIPIC_PASTE_PREVIEW_WIN_ID);
        }
        MMK_CloseWin( win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        
        break;      
    
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : hand paste edit win msg
//  Global resource dependence : 
//  Author: vincent.shang
//  Note: yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePastePreviewWinMsg(
                                             MMI_WIN_ID_T     win_id,
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM           param
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    error_alert_id = MMIPIC_ERROR_WIN_ID;    
#ifdef PIC_EDIT_U200_SUPPORT    
    MMI_STRING_T    title_txt = {0};
    uint16          pic_index = 0;
    GUIANIM_DATA_INFO_T data_info = {0};
    //static BOOLEAN  is_clip_added = FALSE;
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPIC_CopyOrgImgDataToOutputImgData();
        MMIPIC_SetOSDImagePosition();
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_CLIP_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_CLIP_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
#endif
        is_pic_edited   = FALSE;
        MMK_CreateWin((uint32*)MMIPIC_PASTE_EDIT_WIN_TAB, PNULL);
        break;

    case MSG_FULL_PAINT:        
        MMIPIC_DisplayOutputImage(win_id, msg_id);//display  source image]
        MMIPIC_DisplayOSDImage();//display osd image 
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_CLIP_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_CLIP_BG);
#endif
        break;

    case MSG_GET_FOCUS:   
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_UpdateFileName(win_id);
#endif
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_TIMER:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIPIC_OnlySaveOrgImgToUndoBuf(); //added, @robert.wang, 09-10-15, cr155834

        if(!MMIPIC_MergeImage())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }

        MMIPIC_CopyOutputImgDataToOrgImgData();//added, @robert.wang, 09-10-26, cr158180
        RecordChangedTimes();
        MMK_CloseWin(win_id);
        break;
#ifdef PIC_EDIT_U200_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            case MMIPIC_OK_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
                
            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
                
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:        
    case MSG_APP_RIGHT:
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_MoveOSDImage(msg_id);
#endif        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            MMIPIC_TouchPenMoveOSDImage(point,msg_id);
            s_pre_tp_msg = MSG_TP_PRESS_MOVE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point1 = {0};
            point1.x = MMK_GET_TP_X(param);
            point1.y = MMK_GET_TP_Y(param);
            MMIPIC_TouchPenMoveOSDImage(point1,msg_id);
            s_pre_tp_msg = MSG_TP_PRESS_DOWN;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point2 = {0};
            point2.x = MMK_GET_TP_X(param);
            point2.y = MMK_GET_TP_Y(param);
            
            if((MMIPIC_IfClickClipRect(point2)) && (MSG_TP_PRESS_DOWN == s_pre_tp_msg))
            {
                MMK_CreateWin((uint32*)MMIPIC_PASTE_EDIT_WIN_TAB, PNULL);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CLOSE_WINDOW:
        MMIPIC_FreeOsdBuf();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : Handle AddChar Win Web Key
//  Global resource dependence : 
//  Author: vincent.shang
//  Note:   
/*****************************************************************************/
LOCAL void HandleAddCharWinWebKey(
                                  MMI_WIN_ID_T      win_id, 
                                  MMISCH_DETAIL_WIN_PARAM_T   *win_param)
{
    MMI_CTRL_ID_T       ctrl_id = MMK_GetActiveCtrlId(win_id);
    MMIPIC_CHAR_ITEM_E  color_tab = MMIPIC_CHAR_WORD_COLOR;
    
    switch(ctrl_id)
    {
    case MMIPIC_NAME_LABEL_CTRL_ID:
        MMK_CreateWin((uint32 *)MMPIC_ADD_CHAR_EDITTEXT_WIN_TAB, (ADD_DATA)win_param);
        break;
        
    case MMIPIC_COLOR_LABEL_CTRL_ID:
    case MMIPIC_FRAME_COLOR_LABEL_CTRL_ID:
        if (MMIPIC_COLOR_LABEL_CTRL_ID == ctrl_id)
        {
            color_tab = MMIPIC_CHAR_WORD_COLOR;
        }
        else
        {
            color_tab = MMIPIC_CHAR_FRAME_COLOR;
        }
        MMK_CreateWin((uint32 *)MMPIC_CHAR_COLOR_WIN_TAB, (ADD_DATA)color_tab);
        break;

    case MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID:
        break;
    case MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID:
        break;
        
    default:
        break;
    }
    
}

/*****************************************************************************/
//  Description : 设置下拉列表的数据
//  Global resource dependence : 
//  Author:vincent.shang
//  Note:
//
//  Modifier: robert.wang   
//  Date:     2009-7-24
//  Note:     only two fonts , large or little
//            cr145791    
/*****************************************************************************/
LOCAL void SetCharacterDropDownList(void)
{
    MMI_PIC_CHAR_FONT_E          index1 = s_char_info.size_type;
    MMI_PIC_DIRECTION_E          index2 = s_char_info.direction_type;
    
    
    //初始化下拉列表
    
    //size
    GUISETLIST_AddItemById(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID, TXT_COMM_FONT_BIG);
    GUISETLIST_AddItemById(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID, TXT_COMMON_MID);
    GUISETLIST_AddItemById(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID, TXT_FONT_SMALL);
    GUISETLIST_SetTitleTextId(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID, TXT_COMMON_FONT_SIZE);
    GUISETLIST_SetCurIndex(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID, (uint32)index1);
    
    //direction
    GUISETLIST_AddItemById(MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID, TXT_PIC_HORIZONTAL);
    GUISETLIST_AddItemById(MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID, TXT_PIC_VERTICAL);
    GUISETLIST_SetTitleTextId(MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID, TXT_PIC_CHAR_DIRECTION);
    GUISETLIST_SetCurIndex(MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID, (uint32)index2);
}
/*****************************************************************************/
//  Description : get validate rect for outline border 
//  Global resource dependence : 
//  Author: vincent.shang
//  Note:   
/*****************************************************************************/
LOCAL void EditWinGetFocusOutsideRect(
                                      GUI_RECT_T *rect_ptr,
                                      int16 interval
                                      )
{
    if (PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPIC] EditWinGetFocusOutsideRect rect_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_2911_112_2_18_2_46_0_72,(uint8*)"");
        return;
    }
    
    (*rect_ptr).left=(*rect_ptr).left-interval;
    (*rect_ptr).top=(*rect_ptr).top-interval;
    (*rect_ptr).right=(*rect_ptr).right+interval;
    (*rect_ptr).bottom=(*rect_ptr).bottom+interval;
}
/*****************************************************************************/
//  Description : Draw Focus Rect 
//  Global resource dependence : 
//  Author: vincent
//  Note:   
/*****************************************************************************/
PUBLIC void MMIPIC_EditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color,
                                int16 interval
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    EditWinGetFocusOutsideRect(&rect,interval);
    
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);    
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);    
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);

    if (interval >= 3)
    {
        LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top + 1, rect.right, color);
        LCD_DrawVLine(&lcd_dev_info, rect.left + 1, rect.top, rect.bottom, color);
        LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom - 1, rect.right, color);
        LCD_DrawVLine(&lcd_dev_info, rect.right - 1, rect.top, rect.bottom, color);
    }
}

/*****************************************************************************/
//  Description : set charcater detail info
//  Global resource dependence : 
//  Author:  vincent.shang
//  Note:   
/*****************************************************************************/
LOCAL void SetCharacterWinInfo(void)
{
    MMI_STRING_T        name_str  = {0};   
    
    name_str.wstr_ptr = s_char_info.content;    
    name_str.wstr_len = s_char_info.length;    
    GUILABEL_SetText(MMIPIC_NAME_LABEL_CTRL_ID, &name_str, FALSE);
}

/*****************************************************************************/
//  Description : Handle character edit WinTPDown
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void HandleCharacterEditWinTPDown(
                                        MMI_WIN_ID_T     win_id, 
                                        GUI_POINT_T      point
                                        )
{
    MMI_CTRL_ID_T       ctrl_id = 0;
    MMIPIC_CHAR_ITEM_E  color_tab =  MMIPIC_CHAR_WORD_COLOR;
    
    ctrl_id = MMK_GetActiveCtrlId(win_id);

    switch(ctrl_id)
    {
    case MMIPIC_NAME_LABEL_CTRL_ID:
        MMK_CreateWin((uint32 *)MMPIC_ADD_CHAR_EDITTEXT_WIN_TAB, PNULL);
        break;
    
    case MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID:
        break;
    
    case MMIPIC_COLOR_LABEL_CTRL_ID:
        color_tab = MMIPIC_CHAR_WORD_COLOR;
        MMK_CreateWin((uint32 *)MMPIC_CHAR_COLOR_WIN_TAB, (ADD_DATA)color_tab);
        break;
    
    case MMIPIC_FRAME_COLOR_LABEL_CTRL_ID:
        color_tab = MMIPIC_CHAR_FRAME_COLOR;
        MMK_CreateWin((uint32 *)MMPIC_CHAR_COLOR_WIN_TAB, (ADD_DATA)color_tab);
        break;    
        
    case MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID:
        break;
        
    default:
        break;
    }    
}

/*****************************************************************************/
// 	Description : set character form param
//	Global resource dependence : 
//  Author: robert.wang
//	Note: 
/*****************************************************************************/
LOCAL void SetCharactorFormParam(MMI_WIN_ID_T      win_id)
{
    GUIFORM_SetStyle(MMIPIC_CHARACTER_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIPIC_CHARACTER_SIZE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIPIC_CHARACTER_FONT_COLOR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIPIC_CHARACTER_FRAME_COLOR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIPIC_CHARACTER_DIRECTION_COLOR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMK_SetAtvCtrl(win_id, MMIPIC_NAME_LABEL_CTRL_ID);
#ifndef PIC_EDIT_U200_SUPPORT
    GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_EDIT, STXT_RETURN, FALSE);
#endif

    GUILABEL_SetTextById(MMIPIC_NAME_TITLE_LABEL_CTRL_ID, TXT_COMMON_CONTENTS,FALSE);
    GUILABEL_SetTextById(MMIPIC_SIZE_TITLE_LABEL_CTRL_ID, TXT_COMMON_FONT_SIZE,FALSE);
    GUILABEL_SetTextById(MMIPIC_COLOR_TITLE_LABEL_CTRL_ID, TXT_COMMON_FONT_COLOR,FALSE);
    GUILABEL_SetTextById(MMIPIC_FRAME_COLOR_TITLE_LABEL_CTRL_ID, TXT_PIC_CHAR_FRAME_COLOR,FALSE);
    GUILABEL_SetTextById(MMIPIC_DIRECTION_TITLE_LABEL_CTRL_ID, TXT_PIC_CHAR_DIRECTION,FALSE);

    SetCharacterDropDownList();
}

/*****************************************************************************/
//  Description : handle add character win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCharacterFormWinMsg (
                                                        MMI_WIN_ID_T      win_id,
                                                        MMI_MESSAGE_ID_E  msg_id, 
                                                        DPARAM            param
                                                        )
{
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
    GUI_RECT_T        display_rect = MMITHEME_GetClientRectEx(MMIPIC_ADD_CHARACTER_WIN_ID);
    GUI_FONT_T         font = MMI_DEFAULT_SMALL_FONT;
    GUI_LCD_DEV_INFO   lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};  
    
    int16                   font_size = MMI_DEFAULT_SMALL_FONT_SIZE;
    MMI_WIN_ID_T            nosupport_win_id = MMIPIC_NOSUPPORT_PUBWIN_WIN_ID;
    int16                   pic_height = 0;
    int16                   pic_width = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        SetCharactorFormParam(win_id);
        DisplayBackgroudImg(win_id);
        GUIFORM_SetType(MMIPIC_CHARACTER_FORM_CTRL1_ID, GUIFORM_TYPE_TP);
        GUISETLIST_SetFontAlign(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID, ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID, ALIGN_LVMIDDLE);
        break;
        
    case MSG_CTL_SETLIST_SWITCH:       
        s_char_info.size_type = (MMI_PIC_CHAR_FONT_E)GUISETLIST_GetCurIndex(MMIPIC_SIZE_DROPDOWNLIST_CTRL_ID);
        s_char_info.direction_type= (MMI_PIC_DIRECTION_E)GUISETLIST_GetCurIndex(MMIPIC_DIRECTION_DROPDOWNLIST_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        DisplayBackgroudImg(win_id);
        SetCharacterWinInfo();    
        break;
    case MSG_GET_FOCUS:
    
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        if(MMI_PIC_CHAR_FONT_LARGE ==s_char_info.size_type)
        {
            font=MMI_PIC_CHAR_LARGE_FONT;            
            font_size=MMI_PIC_CHAR_LARGE_FONT_SIZE; //added by robert.wang for cr147445
        }
        else if(MMI_PIC_CHAR_FONT_NORMAL == s_char_info.size_type)
    	{
            font=MMI_PIC_CHAR_NORMAL_FONT;
            font_size=MMI_PIC_CHAR_NORMAL_FONT_SIZE;//added by robert.wang for cr147445
    	}
		else
        {
            font=MMI_PIC_CHAR_SMALL_FONT;
            font_size=MMI_PIC_CHAR_SMALL_FONT_SIZE;//added by robert.wang for cr147445
        }
        s_char_info.font=font;
        
        MMIPIC_GetPictureHeightAndWidth(&pic_height,&pic_width);

        if((font_size > pic_height)||(font_size > pic_width))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_TOO_SMALL,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);											
        }
        else
        {          
            MMK_CreateWin((uint32 *)MMIPIC_CHARACTER_PREVIEW_WIN_TAB, PNULL);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_SK_SKB_ICON_PRESSED:
    case MSG_CTL_MIDSK:        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef PIC_EDIT_U200_SUPPORT
        if ((PNULL != param) && (MSG_CTL_PENOK == msg_id))
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
                break;
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
            }
        }
#endif //PIC_EDIT_U200_SUPPORT
        HandleAddCharWinWebKey(win_id, param);        
        break;
        
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        break;
        
    case MSG_CLOSE_WINDOW:
        s_char_info.box = MMITHEME_GetClientRectEx(MMIPIC_ADD_CHARACTER_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle character color win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCharColorWinMsg(
                                         MMI_WIN_ID_T       win_id,
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM         param
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static MMIPIC_CHAR_ITEM_E s_color_item=0;/*lint !e64*/
    GUI_RECT_T        client_rect=MMITHEME_GetClientRectEx(MMIPIC_CHAR_COLOR_WIN_ID);
    GUI_LCD_DEV_INFO    lcd_dev_info= {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_TEXT_ID_T           txt_id[2]= {TXT_COMMON_FONT_COLOR, TXT_PIC_CHAR_FRAME_COLOR};
    int32 i=0;
    int32  j=0;
    int32 k=0;
    static GUI_RECT_T   s_rect[MMIPIC_COLOR_NUM]={0};
    
    static uint32 s_color_index=0;

    uint16  client_width = 0;
    uint16  client_height = 0;
    uint16  color_width = 0;
    uint16  color_height = 0;
    uint16  color_block_ver = 0;
    uint16  color_block_hor = 0;
    uint16  line_space = 6;
    uint16  line_count = 0; //total lines
    int16   interval=3;

    int16   client_rect_top = 0;
    GUI_COLOR_T line_color = MMI_LIGHT_BLUE_COLOR;

    BOOLEAN is_close_win = FALSE;
    MMI_STRING_T    title_txt = {0};
#ifndef PIC_EDIT_U200_SUPPORT
    client_rect_top = client_rect.top;
#else
    client_rect_top = client_rect.top + MMIPICEDIT_TITLE_HEIGHT;
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        DisplayBackgroudImg(win_id);
#ifdef PIC_EDIT_U200_SUPPORT
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_TEXT_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_EDIT_OK_CTRL_ID, MMIPIC_EDIT_CANCEL_CTRL_ID, FALSE);
#endif
        s_color_item = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
#ifndef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(txt_id[s_color_item-2], &title_txt); 
        SetFileNameWinTitle(win_id,title_txt.wstr_ptr,title_txt.wstr_len,FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
#endif
        break;

    case MSG_FULL_PAINT:    
        DisplayBackgroudImg(win_id);

        //color blocks are displayed in the center of client rect
        //get total lines
        line_count = (MMIPIC_COLOR_NUM + MMIPIC_COLOR_NUM_EACH_LINE - 1)/ MMIPIC_COLOR_NUM_EACH_LINE;
        
        client_width = MMITHEME_GetClientRectWidth();
#ifndef PIC_EDIT_U200_SUPPORT
        client_height = MMITHEME_GetClientRectHeightEx(MMIPIC_CHAR_COLOR_WIN_ID);
#else
        client_height = MMITHEME_GetClientRectHeightEx(MMIPIC_CHAR_COLOR_WIN_ID) - MMIPICEDIT_TITLE_HEIGHT;
#endif
        //calculate the color rect width and height
        color_width = (client_width - (MMIPIC_COLOR_NUM_EACH_LINE + 1) * line_space)/MMIPIC_COLOR_NUM_EACH_LINE;
        color_height = (client_height - (line_count + 1) * line_space) / line_count;
        
        color_block_hor = color_width + line_space;
        color_block_ver = color_height + line_space;
        
        for(i=0;i<MMIPIC_COLOR_NUM;i++)
        {
            j=i%MMIPIC_COLOR_NUM_EACH_LINE;
            k=i/MMIPIC_COLOR_NUM_EACH_LINE;
            s_rect[i].left = line_space + j * color_block_hor;
            s_rect[i].top = client_rect_top + line_space + color_block_ver * k;
            s_rect[i].right = s_rect[i].left + color_width - 1;
            s_rect[i].bottom = s_rect[i].top + color_height - 1;
            
            LCD_FillRect(&lcd_dev_info,s_rect[i],s_char_color[i]);  
        }        
        
        if(MMIPIC_CHAR_WORD_COLOR == s_color_item)
        {
            s_color_index=s_char_info.color_index;
        }
        else
        {
            s_color_index=s_char_info.frame_color_index;
        }        
        MMIPIC_EditWinDrawFocusRect(s_rect[s_color_index], line_color,interval); 
        
#ifdef PIC_EDIT_U200_SUPPORT
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_TEXT_BG);
#endif
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:		
    case MSG_CTL_OK:        
        if(MMIPIC_CHAR_WORD_COLOR == s_color_item)
        {
            s_char_info.color_index=s_color_index;
        }
        else
        {
            s_char_info.frame_color_index=s_color_index;
        }    
        RecordChangedTimes();
        MMK_CloseWin(win_id);
        CalCharPosition(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef PIC_EDIT_U200_SUPPORT
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            case MMIPIC_EDIT_OK_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
                
            case MMIPIC_EDIT_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
            default:
                break;
            }
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_LEFT:
        {
            GUI_RECT_T  rect_temp=s_rect[s_color_index];
            rect_temp.left=rect_temp.left-interval;
            rect_temp.top=rect_temp.top-interval;
            rect_temp.right=rect_temp.right+interval;
            rect_temp.bottom=rect_temp.bottom+interval;
            
            GUIRES_DisplayImg(PNULL,
                &rect_temp,
                &rect_temp,
                win_id,
                IMAG_PS_BG,
                &lcd_dev_info);
            LCD_FillRect(&lcd_dev_info,s_rect[s_color_index],s_char_color[s_color_index]);          
            if(0==s_color_index)
            {
                s_color_index=MMIPIC_COLOR_NUM;
            }               
            s_color_index--;
            MMIPIC_EditWinDrawFocusRect(s_rect[s_color_index], line_color,interval);
            
        }
        break;
        
    case MSG_APP_RIGHT:
        {
            GUI_RECT_T  rect_temp=s_rect[s_color_index];
            rect_temp.left=rect_temp.left-interval;
            rect_temp.top=rect_temp.top-interval;
            rect_temp.right=rect_temp.right+interval;
            rect_temp.bottom=rect_temp.bottom+interval;
            
            GUIRES_DisplayImg(PNULL,
                &rect_temp,
                &rect_temp,
                win_id,
                IMAG_PS_BG,
                &lcd_dev_info);
            LCD_FillRect(&lcd_dev_info,s_rect[s_color_index],s_char_color[s_color_index]);
            s_color_index++;            
            if(MMIPIC_COLOR_NUM<=s_color_index)
            {
                s_color_index=0;
            }
            MMIPIC_EditWinDrawFocusRect(s_rect[s_color_index], line_color,interval);
        }
        break;
        
    case MSG_APP_UP:
        {
            GUI_RECT_T  rect_temp=s_rect[s_color_index];
            rect_temp.left=rect_temp.left-interval;
            rect_temp.top=rect_temp.top-interval;
            rect_temp.right=rect_temp.right+interval;
            rect_temp.bottom=rect_temp.bottom+interval;
            
            GUIRES_DisplayImg(PNULL,
                &rect_temp,
                &rect_temp,
                win_id,
                IMAG_PS_BG,
                &lcd_dev_info);
            LCD_FillRect(&lcd_dev_info,s_rect[s_color_index],s_char_color[s_color_index]);
            if(s_color_index<=(MMIPIC_COLOR_NUM_EACH_LINE-1))
            {
                s_color_index=((MMIPIC_COLOR_NUM/MMIPIC_COLOR_NUM_EACH_LINE)-1)*MMIPIC_COLOR_NUM_EACH_LINE+s_color_index;
            }
            else
            {
                s_color_index=s_color_index-MMIPIC_COLOR_NUM_EACH_LINE;
            }
            MMIPIC_EditWinDrawFocusRect(s_rect[s_color_index], line_color,interval);
        }
        break;
    case MSG_APP_DOWN:
        {
            GUI_RECT_T  rect_temp=s_rect[s_color_index];
            rect_temp.left=rect_temp.left-interval;
            rect_temp.top=rect_temp.top-interval;
            rect_temp.right=rect_temp.right+interval;
            rect_temp.bottom=rect_temp.bottom+interval;
            
            GUIRES_DisplayImg(PNULL,
                &rect_temp,
                &rect_temp,
                win_id,
                IMAG_PS_BG,
                &lcd_dev_info);
            LCD_FillRect(&lcd_dev_info,s_rect[s_color_index],s_char_color[s_color_index]);
            s_color_index=s_color_index+MMIPIC_COLOR_NUM_EACH_LINE;
            if(s_color_index>=((MMIPIC_COLOR_NUM/MMIPIC_COLOR_NUM_EACH_LINE)*MMIPIC_COLOR_NUM_EACH_LINE))
            {
                s_color_index=s_color_index-(MMIPIC_COLOR_NUM/MMIPIC_COLOR_NUM_EACH_LINE)*MMIPIC_COLOR_NUM_EACH_LINE;
            }
            MMIPIC_EditWinDrawFocusRect(s_rect[s_color_index], line_color,interval);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            GUI_RECT_T  rect_temp=s_rect[s_color_index];
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
            rect_temp.left=rect_temp.left-interval;
            rect_temp.top=rect_temp.top-interval;
            rect_temp.right=rect_temp.right+interval;
            rect_temp.bottom=rect_temp.bottom+interval;
            
            GUIRES_DisplayImg(PNULL,
                &rect_temp,
                &rect_temp,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
            LCD_FillRect(&lcd_dev_info,s_rect[s_color_index],s_char_color[s_color_index]);
            
            
            for(i=0;i<MMIPIC_COLOR_NUM;i++)
            {
                if(GUI_PointIsInRect(point,s_rect[i]))
                {
                    if(MMIPIC_CHAR_WORD_COLOR == s_color_item)
                    {
                        s_char_info.color_index=i;
                    }
                    else
                    {
                        s_char_info.frame_color_index=i;
                    }
                    s_color_index=i;
                    
                    is_close_win = TRUE;
                    break;
                }
            }
            MMIPIC_EditWinDrawFocusRect(s_rect[s_color_index], line_color,interval);

            if (is_close_win)
            {
                MMK_CloseWin(win_id);
                CalCharPosition(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
            }
        }
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
    
}

/*****************************************************************************/
//  Description : hand add character text win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddCharTextEditorWinMsg(
                                                 MMI_WIN_ID_T       win_id,
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM         param
                                                 )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_STRING_T        str = {0}; 
    MMI_CTRL_ID_T       current_ctrl_id = MMIPIC_TEXTEDIT_CTRL_ID;
    
#ifdef PIC_EDIT_U200_SUPPORT
    MMI_STRING_T        title_txt = {0};
    GUI_RECT_T          rect_ptr  = MMITHEME_GetClientRectEx(win_id);
    uint16              font_size = MMI_DEFAULT_SMALL_FONT_SIZE;
    int16               pic_height = 0;
    int16               pic_width  = 0;
    MMI_WIN_ID_T        nosupport_win_id = MMIPIC_NOSUPPORT_PUBWIN_WIN_ID;
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        DisplayBackgroudImg(win_id);
#ifdef PIC_EDIT_U200_SUPPORT
        GUIEDIT_SetRect(current_ctrl_id, &rect_ptr);
#endif
        GUIEDIT_SetString(
            current_ctrl_id,
            s_char_info.content,
            s_char_info.length
            );
        
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        DisplayBackgroudImg(win_id);
        break;
        
    case MSG_CTL_OK:  
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        if(!is_pic_edited)
        {
            is_pic_edited = TRUE;
        }
        // get string info
        GUIEDIT_GetString(current_ctrl_id, &str);
        s_char_info.length = str.wstr_len;
		
        SCI_MEMSET(s_char_info.content, 0, (MMIPIC_CHAR_TEXT_MAX_LEN * sizeof(wchar)));
        MMI_WSTRNCPY(
            s_char_info.content, MMIPIC_CHAR_TEXT_MAX_LEN,
            str.wstr_ptr, str.wstr_len,
            str.wstr_len);
            
#ifdef PIC_EDIT_U200_SUPPORT
        switch(s_char_info.size_type)
        {
        case MMI_PIC_CHAR_FONT_LARGE:
            s_char_info.font=MMI_PIC_CHAR_LARGE_FONT;            
            font_size=MMI_PIC_CHAR_LARGE_FONT_SIZE;
            break;

        case MMI_PIC_CHAR_FONT_NORMAL:
            s_char_info.font=MMI_PIC_CHAR_NORMAL_FONT;            
            font_size=MMI_PIC_CHAR_NORMAL_FONT_SIZE;
            break;

        default:
            s_char_info.font=MMI_PIC_CHAR_SMALL_FONT;            
            font_size=MMI_PIC_CHAR_SMALL_FONT_SIZE; 
            break;
        }
                
        MMIPIC_GetPictureHeightAndWidth(&pic_height,&pic_width);
        if((font_size > pic_height)||(font_size > pic_width))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_TOO_SMALL,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {          
            MMK_CreateWin((uint32 *)MMIPIC_CHARACTER_PREVIEW_WIN_TAB, PNULL);
        }
#endif
        MMK_CloseWin(win_id);
        CalCharPosition(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef PIC_EDIT_U200_SUPPORT
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            //case MMIPIC_EDIT_OK_CTRL_ID:
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
                
            //case MMIPIC_EDIT_CANCEL_CTRL_ID:
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
            default:
                break;
            }
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(!is_pic_edited)
        {
            MMK_CloseWin(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
        }
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : handle  character preview win msg
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: move character on image
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCharacterPreviewWinMsg (
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id, 
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
#ifdef PIC_EDIT_U200_SUPPORT
    MMI_CTRL_ID_T   ctrl_ptr[2] = {MMIPIC_CHAR_COLOR_CTRL_ID, MMIPIC_CHAR_SIZE_CTRL_ID};
    MMI_TEXT_ID_T   txt_ptr[2]  = {TXT_COMMON_FONT_COLOR, TXT_COMMON_FONT_SIZE};
    MMI_STRING_T    title_txt   = {0};
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:  
        MMIPIC_ResetPictureDisplayRect(); 
        
        s_char_info.box = MMIPIC_GetRelativeRect(win_id);
        is_pic_edited = FALSE;
#ifndef PIC_EDIT_U200_SUPPORT
        CalCharPosition(win_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
//        MMK_CreatePubEditWin((uint32*)MMIPIC_ADD_CHARACTER_WIN_TAB, PNULL);
#else    
        MMI_GetLabelTextByLang(TXT_PIC_ADD_CHAR, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_TEXT_BG);
        SetFormParam(win_id, MMIPIC_CHAR_PREVIEW_FORM_ID, MMIPIC_CHAR_COLOR_CTRL_ID, txt_ptr, PNULL, 2, IMAG_PS_TEXT_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
        MMK_CreatePubEditWin((uint32*)MMPIC_ADD_CHAR_EDITTEXT_WIN_TAB, PNULL);
#endif
        break;
        
    case MSG_FULL_PAINT:
        MMIPIC_DisplayOutputImage(win_id, msg_id);
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_ADD_CHAR, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_TEXT_BG);
#endif
        break;
        
    case MSG_GET_FOCUS:
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_UpdateFileName(win_id);
#endif
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_TIMER:
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_SaveToUndoBuf();
        RecordChangedTimes();
        MMK_CloseWin(MMIPIC_ADD_CHARACTER_WIN_ID);
        MMK_CloseWin(win_id);
#else

        switch(MMK_GetActiveCtrlId(win_id))
        {
        case MMIPIC_CHAR_SIZE_CTRL_ID:
            //MMK_CloseWin(win_id);
            MMK_CreatePubListWin((uint32*)MMPIC_CHAR_SIZE_WIN_TAB, PNULL);
            break;
        
        case MMIPIC_CHAR_COLOR_CTRL_ID:
            //MMK_CloseWin(win_id);
            MMK_CreateWin((uint32*)MMPIC_CHAR_COLOR_WIN_TAB, (ADD_DATA)MMIPIC_CHAR_WORD_COLOR);
            break;
        default:
            break;
        }
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifndef PIC_EDIT_U200_SUPPORT
        MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
#else
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
            case MMIPIC_OK_CTRL_ID:
                MMIPIC_SaveToUndoBuf();
                RecordChangedTimes();
                MMK_CloseWin(win_id);
                break;

            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
            default:
                MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
                break;
            }
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_MoveText(&s_char_info.box,                           //the fixed display area
            s_char_info.content,                        //string to display
            s_char_info.length,                         //length of string to display
            s_char_info.space_two_chars,                //space between two chars.
            s_char_color[s_char_info.color_index],      //string color
            s_char_info.font,                           //font pointer  
            s_char_info.space_two_lines,                //space between 2 lines     
            s_char_color[s_char_info.frame_color_index],//font edge color/font frame color
            s_char_info.direction_type,
            msg_id,
            s_char_info.char_string_width,
            s_char_info.char_string_heigh
            );
#endif
        break;
                
     //add for CR251783
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            MMIPIC_TouchPenMoveChar(&s_char_info,s_char_color,point,msg_id);
#ifdef PIC_EDIT_U200_SUPPORT
            s_pre_tp_msg = MSG_TP_PRESS_MOVE;
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point1 = {0};
            point1.x = MMK_GET_TP_X(param);
            point1.y = MMK_GET_TP_Y(param);
            MMIPIC_TouchPenMoveChar(&s_char_info,s_char_color,point1,msg_id);
#ifdef PIC_EDIT_U200_SUPPORT
            s_pre_tp_msg = MSG_TP_PRESS_DOWN;
#endif
        }
        break;    
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef PIC_EDIT_U200_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point2 = {0};
            point2.x = MMK_GET_TP_X(param);
            point2.y = MMK_GET_TP_Y(param);
            
            if((MMIPIC_IfClickCharRect(win_id, &s_char_info, point2)) && (MSG_TP_PRESS_DOWN == s_pre_tp_msg))
            {
                MMK_CreatePubEditWin((uint32*)MMPIC_ADD_CHAR_EDITTEXT_WIN_TAB, PNULL);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 
     //add for CR251783
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : set win title
//  Global resource dependence : 
//  Author:vincent.shang
//  Note: 
/*****************************************************************************/
LOCAL void SetFileNameWinTitle(
                               MMI_WIN_ID_T      win_id, 
                               const wchar       *file_name_ptr,
                               uint16            file_name_len,
                               //BOOLEAN         is_ucs2,
                               BOOLEAN           is_refresh
                               )
{
    MMI_STRING_T                name_str = {0};
    uint16                      title_max_len = 0;

    if (PNULL == file_name_ptr)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC] SetFileNameWinTitle file name = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_3807_112_2_18_2_46_2_73,(uint8*)"");
        return ;
    }
    if (0 == file_name_len)
    {
        //MMIPIC_ASSERT_WARNING:"[MMIPIC] SetFileNameWinTitle name len = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_3812_112_2_18_2_46_2_74,(uint8*)"");
        return ;
    }

    title_max_len = MMITHEME_GetWinTitleMaxLen();
    
    name_str.wstr_ptr  = (wchar*)file_name_ptr;
  
    name_str.wstr_len   = file_name_len;
	
    if(title_max_len < name_str.wstr_len)		
    {
        name_str.wstr_len = title_max_len;
		
        MMI_WSTRNCPY(&name_str.wstr_ptr[title_max_len/sizeof(wchar)-MMIPIC_ELLIPSIS_3_LEN], 
                                   MMIPIC_ELLIPSIS_3_LEN,
                                   MMIPIC_ELLIPSIS_3, 
                                   MMIPIC_ELLIPSIS_3_LEN, 
                                   MMIPIC_ELLIPSIS_3_LEN);		
    }
    
    GUIWIN_SetTitleText(win_id, name_str.wstr_ptr, name_str.wstr_len, is_refresh);
#ifdef PIC_EDIT_U200_SUPPORT
    GUIWIN_SetTitleTextAlign(win_id, ALIGN_HMIDDLE);
#endif
}

// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : init arrow buttons
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL void InitArrowBtns(MMIPIC_MENU_ID_E item,
//                             MMI_WIN_ID_T win_id
//                             )
// {
//     MMI_CTRL_ID_T ctrl_id = 0;
// 
//     int32 left = 0;
//     int32 top = 0; 
//     GUI_RECT_T rect = {0};
//     
//     uint16 width = 0;
//     uint16 height = 0;
//     
//     GUI_BG_T bg_info = {0}; 
//     MMI_IMAGE_ID_T img_id = IMAGE_NULL;
//     MMI_IMAGE_ID_T focus_img_id = IMAGE_NULL;
// 
//     GUI_RECT_T client_rect = MMITHEME_GetClientRectEx(win_id);
//     uint16 screen_width = 0;
//     uint16 screen_height = 0;
// 
//     GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);
//     
//     img_id = IMAGE_LIST_TITLE_ARROW_LEFT_UP;
//     GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
// 
//     //left arrow
//     ctrl_id = MMIPIC_LEFT_ARROW_CTRL_ID;
// 
//     top = client_rect.top + MMIPIC_ARROW_TOP_SPACE;
//     left = (screen_width - MMIPIC_ARROW_SPACE - 2 * width) / 2;
// 
//     rect.left = left;
//     rect.right = rect.left + width - 1;
//     rect.top = top;
//     rect.bottom = rect.top + height - 1;
// 
//     GUIBUTTON_SetRect(ctrl_id, &rect);
//     
//     img_id = IMAGE_LIST_TITLE_ARROW_LEFT_UP;
//     focus_img_id = IMAGE_LIST_TITLE_ARROW_LEFT_DOWN;
// 
//     bg_info.bg_type = GUI_BG_IMG;
//     bg_info.img_id = img_id;
//     GUIBUTTON_SetBg(ctrl_id, &bg_info);  
//     
//     bg_info.bg_type = GUI_BG_IMG;
//     bg_info.img_id = focus_img_id;
//     GUIBUTTON_SetPressedBg(ctrl_id, &bg_info);  
// 
//     GUIBUTTON_SetRunSheen(ctrl_id, FALSE);
//     //right arrow
//     ctrl_id = MMIPIC_RIGHT_ARROW_CTRL_ID;
//     
//     rect.left = rect.right + MMIPIC_ARROW_SPACE;
//     rect.right = rect.left + width - 1;
// 
//     GUIBUTTON_SetRect(ctrl_id, &rect);
//     
//     img_id = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;
//     focus_img_id = IMAGE_LIST_TITLE_ARROW_RIGHT_DOWN;
// 
//     bg_info.bg_type = GUI_BG_IMG;
//     bg_info.img_id = img_id;
//     GUIBUTTON_SetBg(ctrl_id, &bg_info);  
//     
//     bg_info.bg_type = GUI_BG_IMG;
//     bg_info.img_id = focus_img_id;
//     GUIBUTTON_SetPressedBg(ctrl_id, &bg_info);  
// 
//     GUIBUTTON_SetRunSheen(ctrl_id, FALSE);
// }
// 
// /*****************************************************************************/
// //  Description : display arrow buttons
// //  Global resource dependence : 
// //  Author: robert.wang
// //  Note: 
// /*****************************************************************************/
// LOCAL void DisplayArrowBtns(BOOLEAN is_update_left,
//                             BOOLEAN is_update_right
//                             )
// {
//     MMI_CTRL_ID_T left_ctrl_id = MMIPIC_LEFT_ARROW_CTRL_ID;
//     MMI_CTRL_ID_T right_ctrl_id = MMIPIC_RIGHT_ARROW_CTRL_ID;
//     
//     if (is_update_left)
//     {
//         MMK_SendMsg(left_ctrl_id, MSG_CTL_PAINT, PNULL);
//     }
// 
//     if (is_update_right)
//     {
//         MMK_SendMsg(right_ctrl_id, MSG_CTL_PAINT, PNULL);
//     }
// }
// #endif

#ifdef PIC_EDIT_FRAME_SUPPORT
/*****************************************************************************/
//  Description : handle frame win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFrameWinMsg (
                                       MMI_WIN_ID_T     win_id,
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T   txt_id[MMIPIC_FRAME_NUM]= {TXT_PIC_FRAME_1, TXT_PIC_FRAME_2, TXT_PIC_FRAME_3};
    MMI_IMAGE_ID_T  img_id_v_array[MMIPIC_FRAME_NUM]=
    {
        IMAGE_PIC_FRAME1,
        IMAGE_PIC_FRAME2,
        IMAGE_PIC_FRAME3
    };
    MMI_IMAGE_ID_T  img_id_h_array[MMIPIC_FRAME_NUM]=
    {
        IMAGE_PIC_H_FRAME1,
        IMAGE_PIC_H_FRAME2,
        IMAGE_PIC_H_FRAME3
    };

    MMI_STRING_T        temp_str = {0};
    static int          s_index=0; //记录当前item项目
    GUI_POINT_T         point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T        error_alert_id = MMIPIC_ERROR_WIN_ID;
    GUI_POINT_T         dis_point = {0};

    BOOLEAN             is_landscape = MMITHEME_IsMainScreenLandscape();
    MMI_IMAGE_ID_T*     img_id_ptr = PNULL;
    MMI_CTRL_ID_T       iconctrl_id = MMIPIC_EDIT_FRAME_ICONLIST_CTRL_ID;
    uint16              pic_index = 0;//临时添加item时使用
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIICONLIST_DISP_T  icon_text = {0};
    MMI_IMAGE_ID_T      first_img = IMAGE_PS_PREVIEW_FRAME1;

    if (is_landscape)
    {
        img_id_ptr = img_id_h_array;
    }
    else
    {
        img_id_ptr = img_id_v_array;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_index=0;
#ifndef PIC_EDIT_U200_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, TXT_PIC_UNDO, FALSE);
        GUIICONLIST_SetDispCustomerSoftkey(iconctrl_id, TRUE);
#else
        MMI_GetLabelTextByLang(TXT_PIC_FRAME_TITLE, &temp_str);
        SetPicEditTitle(win_id, &temp_str, IMAG_PS_FRAME_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
#endif
        SetIconListParam(win_id, iconctrl_id, first_img, MMIPIC_FRAME_NUM, IMAG_PS_FRAME_BG);
        GUIICONLIST_SetStyle(iconctrl_id, GUIICONLIST_STYLE_ICON_UIDT); 
        GUIICONLIST_SetCurIconIndex(s_index, iconctrl_id); //设置当前icon的index值
        MMK_SetAtvCtrl(win_id, iconctrl_id);  //设置iconlist控件有效
        MMIPIC_CopyOrgImgDataToOutputImgData();

        if(!MMIPIC_GetFrameImage(img_id_ptr[s_index],win_id))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        GUIICONLIST_SetCurIconIndex((uint16)s_index, iconctrl_id);
        break;
    case MSG_FULL_PAINT:
        MMIPIC_DisplayOutputImage(win_id, msg_id);
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_FRAME_TITLE, &temp_str);
        SetPicEditTitle(win_id, &temp_str, IMAG_PS_FRAME_BG);
#endif
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        pic_index = *((uint16 *)param); //get pic index and item index
        MMI_GetLabelTextByLang(txt_id[pic_index], &icon_text.name_str);
        GUIICONLIST_AppendText(pic_index, iconctrl_id, &icon_text);//append icon
        break;
        
    case MSG_CTL_ICONLIST_APPEND_ICON:   
        pic_index = *((uint16 *)param); //get pic index and item index
        data_info.img_id = first_img + pic_index;
        if((1 == pic_index) &&(is_landscape))
        {
            data_info.img_id = IMAGE_PS_PREVIEW_FRAME2_H;
        }
        GUIICONLIST_AppendIcon(pic_index, iconctrl_id, &data_info, PNULL);//append icon
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMIPIC_SaveToUndoBuf();
        MMIPIC_CopyOutputRectToOrgRect();
        RecordChangedTimes();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        s_index = GUIICONLIST_GetCurIconIndex(iconctrl_id); //获得当前icon的index
        if(!MMIPIC_GetFrameImage(img_id_ptr[s_index],win_id))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
#ifdef PIC_EDIT_U200_SUPPORT
            case MMIPIC_OK_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
                
            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
#endif
            case MMIPIC_EDIT_FRAME_ICONLIST_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
                break;
                
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        
        break;

//     case MSG_APP_LEFT:
//     case MSG_CTL_TITLE_MOVE_PREV:    
// #ifndef PIC_EDIT_U200_SUPPORT
//         s_index--;
//         if(0==s_index+1)
//         {
//             s_index=MMIPIC_FRAME_NUM-1;
//         }
//         MMI_GetLabelTextByLang(txt_id[s_index], &temp_str); 
//         SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
//         if(!MMIPIC_GetFrameImage(img_id_ptr[s_index],win_id))
//         {
//             MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
//             MMIPIC_ExitFromPicEditor(); 
//             return (result);
//         }
//         MMIPIC_DisplayOutputImage(win_id, 0);
// #endif
//         break;
//         
//     case MSG_APP_RIGHT:
//     case MSG_CTL_TITLE_MOVE_NEXT:
// #ifndef PIC_EDIT_U200_SUPPORT
//         s_index++;
//         if(MMIPIC_FRAME_NUM==s_index)
//         {
//             s_index=0;
//         }		
//         MMI_GetLabelTextByLang(txt_id[s_index], &temp_str); 
//         SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
//         if(!MMIPIC_GetFrameImage(img_id_ptr[s_index],win_id))
//         {
//             MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
//             MMIPIC_ExitFromPicEditor(); 
//             return (result);
//         }
//         MMIPIC_DisplayOutputImage(win_id, 0);
// #endif
//         break;
        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_LONG:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

// #ifndef PIC_EDIT_U200_SUPPORT
// /*****************************************************************************/
// //  Description : handle RGB win msg
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleRGBWinMsg (
//                                      MMI_WIN_ID_T       win_id,
//                                      MMI_MESSAGE_ID_E   msg_id, 
//                                      DPARAM         param
//                                      )
// {
//     MMI_RESULT_E                 result = MMI_RESULT_TRUE;
//     PEPROCESS_COLOR_BALANCE_E    s_color = PEPROCESS_CYAN_TO_RED;
//     MMI_WIN_ID_T                 error_alert_id = MMIPIC_ERROR_WIN_ID;    
//     
//     uint16   focus_item = 0;
// 
//     
//     switch(msg_id)
//     {
//     case MSG_OPEN_WINDOW:
//         // 设置参数
//         GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
//         MMIPIC_ResetRGB();
// 
//         MMIPIC_CopyOrgImgDataToOutputImgData();
// 
//         SCI_MEMSET(&s_color_bar_rect, 0, sizeof(s_color_bar_rect));
//         Init_PROGRESSBAR_T(&s_color_bar);
//         s_color_bar.focus_item = 0;
// #ifdef    MMIPIC_SUPPORT_PNG
//         s_color_bar.img_bg_id = IMAGE_PIC_ADJUST_BG;
// #else
//         s_color_bar.img_bg_id = IMAGE_PIC_COLOR_BG;
// #endif
//         s_color_bar.total = 3;
//         s_color_bar.unit_width = MMIPIC_RGB_MOVING_VALUE;
//         s_color_bar.vertical_space = MMIPIC_PROGRESSBAR_SPACE;
//         s_color_bar.win_id = win_id;
//         s_color_bar.group[0].grade = 4;
//         s_color_bar.group[0].total_level = 8;
//         s_color_bar.group[0].item[0] = TXT_PIC_CYAN;
//         s_color_bar.group[0].item[1] = TXT_PIC_RED;
//         
//         s_color_bar.group[1].grade = 4;
//         s_color_bar.group[1].total_level = 8;
//         s_color_bar.group[1].item[0] = TXT_PIC_MAGENTA;
//         s_color_bar.group[1].item[1] = TXT_PIC_GREEN;
//         
//         s_color_bar.group[2].grade = 4;
//         s_color_bar.group[2].total_level = 8;
//         s_color_bar.group[2].item[0] = TXT_PIC_YELLOW;
//         s_color_bar.group[2].item[1] = TXT_PIC_BLUE;
//    
//         break;
//     case MSG_FULL_PAINT:
//         MMIPIC_DisplayOutputImage(win_id, msg_id);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
//         break;
//         
//     case MSG_GET_FOCUS:
//         
//         break;
//         
//     case MSG_LOSE_FOCUS:
//         
//         break;
//         
//     case MSG_TIMER:
//         
//         break;
//     case MSG_APP_OK:
//     case MSG_CTL_OK:
//     case MSG_APP_WEB:
//         MMIPIC_SaveToUndoBuf();
// 
//         if(!s_is_changed)
//         {
//             s_is_changed=TRUE;
//             s_is_saved = FALSE;
//         }       
//         s_operation_times ++;
//         MMK_CloseWin(win_id);
//         break;
// 
//     case MSG_CTL_PENOK:
//         break;
//         
//     case MSG_APP_CANCEL:
//     case MSG_CTL_CANCEL:
//         MMK_CloseWin(win_id);
//         
//         break;
//         
//     case MSG_APP_LEFT:
//         focus_item = s_color_bar.focus_item;
//         if (s_color_bar.group[focus_item].grade > MMIPIC_GRADE_MIN)
//         {
//             s_color_bar.group[focus_item].grade--;
// 
//             s_color = (PEPROCESS_COLOR_BALANCE_E)focus_item;
//             if(!MMIPIC_AdjustRGB(s_color,FALSE))
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                 MMIPIC_ExitFromPicEditor(); 
//                 return (result);
//             }        
// 
//         }    
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
//         break;
//         
//     case MSG_APP_RIGHT:
//         focus_item = s_color_bar.focus_item;
//         if (s_color_bar.group[focus_item].grade < MMIPIC_GRADE_MAX)
//         {
//             s_color_bar.group[focus_item].grade++;
// 
//             s_color = (PEPROCESS_COLOR_BALANCE_E)focus_item;
//             if(!MMIPIC_AdjustRGB(s_color,TRUE))
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                 MMIPIC_ExitFromPicEditor(); 
//                 return (result);
//             }        
//         }
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
//         break;
//     case MSG_APP_UP:
//         focus_item = s_color_bar.focus_item;
//         if(PEPROCESS_CYAN_TO_RED == focus_item)
//         {
//             focus_item = PEPROCESS_YELLOW_TO_BLUE;
//         }
//         else
//         {
//             focus_item--;
//         }
//         s_color_bar.focus_item = focus_item;
// 
//         MMIPIC_DisplayOutputImage(win_id, 0);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
//         break;
//     case MSG_APP_DOWN:
//         focus_item = s_color_bar.focus_item;
//         if(PEPROCESS_YELLOW_TO_BLUE == focus_item)
//         {
//             focus_item = PEPROCESS_CYAN_TO_RED;
//         }
//         else
//         {
//             focus_item++;
//         }
//         s_color_bar.focus_item = focus_item;
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
//         break;
//     case MSG_TP_PRESS_DOWN:
//         {
//             GUI_POINT_T  point = {0};
//             int16        new_adjust = 0;
//             int16        grade = 0;
//             RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
// 
// 
//             point.x = MMK_GET_TP_X(param);
//             point.y = MMK_GET_TP_Y(param);
// 
//             focus_item = s_color_bar.focus_item;
//             new_adjust = GetProgressBarStatusForTP(point, s_color_bar, s_color_bar_rect, &rect_index, &grade);
// 
//             if (focus_item != new_adjust)
//             {
//                 focus_item = new_adjust;
//                 s_color_bar.focus_item = focus_item;
//             }
// 
//             switch(rect_index)
//             {
//             case DOWN_RECT_INDEX:
//                 MMK_PostMsg(win_id,MSG_APP_LEFT,PNULL, 0);
//                 break;
//             case UP_RECT_INDEX:
//                 MMK_PostMsg(win_id,MSG_APP_RIGHT,PNULL, 0);
//                 break;
//             default:
//                 if ((GRADE_RECT_INDEX == rect_index)
//                     && (MMIPIC_GRADE_MIN <= grade)
//                     && (grade <= MMIPIC_GRADE_MAX)
//                     )
//                 {
//                     
//                     s_color_bar.group[focus_item].grade = grade;
//                     s_color = (PEPROCESS_COLOR_BALANCE_E)(focus_item);
//                     if (MMI_PIC_SUCCESS !=MMIPIC_SetRGBValue(grade, s_color))
//                     {
//                         MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                         MMIPIC_ExitFromPicEditor(); 
//                         return (result);
//                     }
//                 }
//                 MMIPIC_DisplayOutputImage(win_id, 0);            
//                 MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
//                 break;
//             }
//         }        
//         break;
//         
//     case MSG_CLOSE_WINDOW:
//         
//         break;
//         
//     default:
//         result = MMI_RESULT_FALSE;
//         break;
//     }
//     return (result);
// }
// 
// /*****************************************************************************/
// //  Description : handle adjust bright and constract window msg
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleAdjustBrightAndConstractWinMsg (
//                                                           MMI_WIN_ID_T      win_id,
//                                                           MMI_MESSAGE_ID_E  msg_id, 
//                                                           DPARAM            param
//                                                           )
// {
//     MMI_RESULT_E    result = MMI_RESULT_TRUE;
//     MMI_PIC_ADJUSTMENT_E  s_adjustment = MMI_PIC_ADJUSTMENT_BRIGHTNESS;
// 
//     MMI_STRING_T     temp_str = {0};
//     MMI_TEXT_ID_T    txt_id[2]= {TXT_COMMON_COMMON_BRIGHTNESS,TXT_COMM_CONTRAST};    
//     MMI_WIN_ID_T     error_alert_id = MMIPIC_ERROR_WIN_ID;
// 
//     uint16   focus_item = 0;
//     
//     switch(msg_id)
//     {
//     case MSG_OPEN_WINDOW:
//         s_adjustment = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
//         
//         SCI_TRACE_LOW("[MMIPIC] s_adjustment = %d",s_adjustment);
//         
//         if (s_adjustment >= 2)
//         {
//             focus_item = 0;
//         }
//         focus_item = s_adjustment;
//         MMI_GetLabelTextByLang(txt_id[focus_item], &temp_str); /*lint !e661*/
//         SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,FALSE);
// 		
//         // 设置参数
//         GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
//         MMIPIC_ResetAdjustment();
// 
//         MMIPIC_CopyOrgImgDataToOutputImgData();
// 
//         SCI_MEMSET(&s_bright_bar_rect, 0, sizeof(s_bright_bar_rect));
//         Init_PROGRESSBAR_T(&s_bright_bar);
//         
//         s_bright_bar.focus_item = focus_item;
//         s_bright_bar.img_bg_id = IMAGE_PIC_ADJUST_BG;
//         s_bright_bar.total = 2;
//         s_bright_bar.unit_width = MMIPIC_BRIGHT_MOVING_VALUE;
//         s_bright_bar.vertical_space = MMIPIC_PROGRESSBAR_SPACE;
//         s_bright_bar.win_id = win_id;
//         s_bright_bar.group[0].grade = 4;
//         s_bright_bar.group[0].total_level = 8;
//         s_bright_bar.group[0].item[0] = TXT_PIC_BRIGHT_DOWN_LIMIT;
//         s_bright_bar.group[0].item[1] = TXT_PIC_BRIGHT_UP_LIMIT;
//         s_bright_bar.group[1].grade = 4;
//         s_bright_bar.group[1].total_level = 8;
//         s_bright_bar.group[1].item[0] = TXT_PIC_CONSTRACT_DOWN_LIMIT;
//         s_bright_bar.group[1].item[1] = TXT_PIC_CONSTRACT_UP_LIMIT;
//         
//         break;
//     case MSG_FULL_PAINT:
//         MMIPIC_DisplayOutputImage(win_id, msg_id);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_bright_bar, &s_bright_bar_rect);
//         break;
//         
//     case MSG_GET_FOCUS:
//         
//         break;
//         
//     case MSG_LOSE_FOCUS:
//         
//         break;
//         
//     case MSG_TIMER:
//         
//         break;
//     case MSG_APP_OK:
//     case MSG_CTL_OK:
//     case MSG_APP_WEB:
//         MMIPIC_SaveToUndoBuf();
// 
//         if(!s_is_changed)
//         {
//             s_is_changed=TRUE;
//             s_is_saved = FALSE;
//         }
//         s_operation_times ++;
//         MMK_CloseWin(win_id);
//         break;
//         
//     case MSG_APP_CANCEL:
//     case MSG_CTL_CANCEL:
//         MMK_CloseWin(win_id);
//         
//         break;
// 
//     case MSG_CTL_PENOK:
//         break;
//         
//     case MSG_APP_LEFT:    
//         focus_item = s_bright_bar.focus_item;
//         if (s_bright_bar.group[focus_item].grade > MMIPIC_GRADE_MIN)
//         {
//             s_bright_bar.group[focus_item].grade--;
//             
//             s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;
//             if(!MMIPIC_Adjustment(s_adjustment,FALSE))
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                 MMIPIC_ExitFromPicEditor(); 
//                 return (result);
//             }        
//         }
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);           
//         MMIPIC_DrawProgressBarPanel(win_id, &s_bright_bar, &s_bright_bar_rect);
//         break;
//         
//     case MSG_APP_RIGHT:
//         focus_item = s_bright_bar.focus_item;
//         if (s_bright_bar.group[focus_item].grade < MMIPIC_GRADE_MAX)
//         {
//             s_bright_bar.group[focus_item].grade++;
// 
//             s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;
//             if(!MMIPIC_Adjustment(s_adjustment,TRUE))
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                 MMIPIC_ExitFromPicEditor(); 
//                 return (result);
//             }        
//         }
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);            
//         MMIPIC_DrawProgressBarPanel(win_id, &s_bright_bar, &s_bright_bar_rect);
//         break;
//         
//     case MSG_APP_UP:
//     case MSG_APP_DOWN:
//         s_bright_bar.focus_item = s_bright_bar.focus_item != 0 ? 0:1;
//         focus_item = s_bright_bar.focus_item;
// 
//         MMI_GetLabelTextByLang(txt_id[focus_item], &temp_str); 
//         SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);            
//         MMIPIC_DrawProgressBarPanel(win_id, &s_bright_bar, &s_bright_bar_rect);
//         break;
//     case MSG_TP_PRESS_DOWN:
//         {
//             GUI_POINT_T point = {0};
//             int16  new_adjust = 0;
//             int16 grade = 0;
//             RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
//     
// 
//             point.x = MMK_GET_TP_X(param);
//             point.y = MMK_GET_TP_Y(param);
// 
//             focus_item = s_bright_bar.focus_item;
//             new_adjust = GetProgressBarStatusForTP(point, s_bright_bar, s_bright_bar_rect, &rect_index, &grade);
// 
//             if (focus_item != new_adjust)
//             {
//                 focus_item = new_adjust;
//                 s_bright_bar.focus_item = focus_item;
//                 
//                 MMI_GetLabelTextByLang(txt_id[focus_item], &temp_str); 
//                 SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
//             }
// 
//             switch(rect_index)
//             {
//             case DOWN_RECT_INDEX:
//                 MMK_PostMsg(win_id,MSG_APP_LEFT,PNULL, 0);
//                 break;
//             case UP_RECT_INDEX:
//                 MMK_PostMsg(win_id,MSG_APP_RIGHT,PNULL, 0);
//                 break;
//             default:
//                 if ((GRADE_RECT_INDEX == rect_index)
//                     && (MMIPIC_GRADE_MIN <= grade)
//                     && (grade <= MMIPIC_GRADE_MAX)
//                     )
//                 {
//                     
//                     s_bright_bar.group[focus_item].grade = grade;
//                     s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;
//                     if (MMI_PIC_SUCCESS != MMIPIC_SetAdjustmentValue(grade, s_adjustment))
//                     {
//                         MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                         MMIPIC_ExitFromPicEditor(); 
//                         return (result);
//                     }
//                 }
//                 MMIPIC_DisplayOutputImage(win_id, 0);            
//                 MMIPIC_DrawProgressBarPanel(win_id, &s_bright_bar, &s_bright_bar_rect);
//                 break;
//             }
//         }
//         break;
//         
//     case MSG_CLOSE_WINDOW:
//         
//         break;
//         
//     default:
//         result = MMI_RESULT_FALSE;
//         break;
//     }
//     return (result);
// }
// 
// /*****************************************************************************/
// //  Description : handle adjust saturation and hue win msg
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleAdjustSaturationAndHueWinMsg (
//                                                         MMI_WIN_ID_T        win_id,
//                                                         MMI_MESSAGE_ID_E    msg_id, 
//                                                         DPARAM          param
//                                                         )
// {
//     MMI_RESULT_E    result = MMI_RESULT_TRUE;
//     MMI_PIC_ADJUSTMENT_E  s_adjust_type=MMI_PIC_ADJUSTMENT_SATURATION;
// 
//     MMI_STRING_T    temp_str = {0};
//     MMI_TEXT_ID_T   txt_id[2]= {TXT_PIC_SATURATION,TXT_PIC_HUE};
//     MMI_WIN_ID_T    error_alert_id = MMIPIC_ERROR_WIN_ID;
//     
//     uint16   focus_item = 0;
// 
//     
//     switch(msg_id)
//     {
//     case MSG_OPEN_WINDOW:
//         s_adjust_type = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
//         
//         SCI_TRACE_LOW("[MMIPIC] s_adjust_type = %d",s_adjust_type);
//         
//         if ((s_adjust_type - 2) >= 2)
//         {
//             focus_item = 0;
//         }
//         focus_item = s_adjust_type - 2;
//         
//         MMI_GetLabelTextByLang(txt_id[focus_item], &temp_str); /*lint !e661*/
//         SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,FALSE);
// 		
//         // 设置参数
//         GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, 0, STXT_RETURN, FALSE);
//         MMIPIC_ResetAdjustment();
// 
//         MMIPIC_CopyOrgImgDataToOutputImgData();
//         
//         SCI_MEMSET(&s_hue_bar_rect, 0, sizeof(s_hue_bar_rect));
//         Init_PROGRESSBAR_T(&s_hue_bar);
//         
//         s_hue_bar.focus_item = focus_item;
//         s_hue_bar.img_bg_id = IMAGE_PIC_ADJUST_BG;
//         s_hue_bar.total = 2;
//         s_hue_bar.unit_width = MMIPIC_BRIGHT_MOVING_VALUE;
//         s_hue_bar.vertical_space = MMIPIC_PROGRESSBAR_SPACE;
//         s_hue_bar.win_id = win_id;
//         s_hue_bar.group[0].grade = 4;
//         s_hue_bar.group[0].total_level = 8;
//         s_hue_bar.group[0].item[0] = TXT_PIC_CONSTRACT_DOWN_LIMIT;
//         s_hue_bar.group[0].item[1] = TXT_PIC_CONSTRACT_UP_LIMIT;
//         s_hue_bar.group[1].grade = 0;
//         s_hue_bar.group[1].total_level = 7;
//         s_hue_bar.group[1].item[0] = TXT_PIC_HUE_DOWN_LIMIT;
//         s_hue_bar.group[1].item[1] = TXT_PIC_HUE_UP_LIMIT;
// 
//         break;
//     case MSG_FULL_PAINT:
//         MMIPIC_DisplayOutputImage(win_id, msg_id);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_hue_bar, &s_hue_bar_rect);
//         break;
//         
//     case MSG_GET_FOCUS:
//         
//         break;
//         
//     case MSG_LOSE_FOCUS:
//         
//         break;
//         
//     case MSG_TIMER:
//         
//         break;
//     case MSG_APP_OK:
//     case MSG_CTL_OK:
//     case MSG_APP_WEB:
//         MMIPIC_SaveToUndoBuf();
// 
//         if(!s_is_changed)
//         {
//             s_is_changed=TRUE;
//             s_is_saved = FALSE;
//         }
//         s_operation_times ++;
//         MMK_CloseWin(win_id);
//         break;
//         
//     case MSG_APP_CANCEL:
//     case MSG_CTL_CANCEL:
//         MMK_CloseWin(win_id);
//         
//         break;
// 
//     case MSG_CTL_PENOK:
//         break;
//         
//     case MSG_APP_LEFT:
//         focus_item = s_hue_bar.focus_item;
//         if (s_hue_bar.group[focus_item].grade > MMIPIC_GRADE_MIN)
//         {
//             s_hue_bar.group[focus_item].grade--;
//             
//             s_adjust_type = (MMI_PIC_ADJUSTMENT_E)(focus_item + 2);
// 
//             if(!MMIPIC_Adjustment(s_adjust_type,FALSE))
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                 MMIPIC_ExitFromPicEditor(); 
//                 return (result);
//             }     
//         }
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_hue_bar, &s_hue_bar_rect);
//         break;
//         
//     case MSG_APP_RIGHT:
//         focus_item = s_hue_bar.focus_item;
//         s_adjust_type = (MMI_PIC_ADJUSTMENT_E)(focus_item + 2);
//         
//         //hue is 8 level values,include no effect
//         //and saturation is 9 level values,include no effect
//         if (((s_hue_bar.group[focus_item].grade < MMIPIC_GRADE_MAX) && (MMI_PIC_ADJUSTMENT_SATURATION == s_adjust_type))
//             || ((s_hue_bar.group[focus_item].grade < MMIPIC_GRADE_MAX - 1) && (MMI_PIC_ADJUSTMENT_HUE == s_adjust_type))
//             )
//         {
//             s_hue_bar.group[focus_item].grade++;
// 
// 
//             if(!MMIPIC_Adjustment(s_adjust_type,TRUE))
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                 MMIPIC_ExitFromPicEditor(); 
//                 return (result);
//             }   
//         }    
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);
//         MMIPIC_DrawProgressBarPanel(win_id, &s_hue_bar, &s_hue_bar_rect);
//         break;
//         
//     case MSG_APP_UP:
//     case MSG_APP_DOWN:
//         s_hue_bar.focus_item = s_hue_bar.focus_item != 0 ? 0:1;
//         focus_item = s_hue_bar.focus_item;
// 
//         MMI_GetLabelTextByLang(txt_id[focus_item], &temp_str); 
//         SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
//         
//         MMIPIC_DisplayOutputImage(win_id, 0);            
//         MMIPIC_DrawProgressBarPanel(win_id, &s_hue_bar, &s_hue_bar_rect);
//         break;
//     case MSG_TP_PRESS_DOWN:
//         {
//             GUI_POINT_T point = {0};
//             int16  new_adjust = 0;
//             int16 grade = 0;
//             RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
// 
// 
//             point.x = MMK_GET_TP_X(param);
//             point.y = MMK_GET_TP_Y(param);
// 
//             focus_item = s_hue_bar.focus_item;
//             new_adjust = GetProgressBarStatusForTP(point, s_hue_bar, s_hue_bar_rect, &rect_index, &grade);
// 
//             if (focus_item != new_adjust)
//             {
//                 focus_item = new_adjust;
//                 s_hue_bar.focus_item = focus_item;
//                 
//                 MMI_GetLabelTextByLang(txt_id[focus_item], &temp_str); 
//                 SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
//             }
// 
//             switch(rect_index)
//             {
//             case DOWN_RECT_INDEX:
//                 MMK_PostMsg(win_id,MSG_APP_LEFT,PNULL, 0);
//                 break;
//             case UP_RECT_INDEX:
//                 MMK_PostMsg(win_id,MSG_APP_RIGHT,PNULL, 0);
//                 break;
//             default:
//                 if ((GRADE_RECT_INDEX == rect_index)
//                     && (MMIPIC_GRADE_MIN <= grade)
//                     && (grade <= MMIPIC_GRADE_MAX)
//                     )
//                 {
//                     s_adjust_type = (MMI_PIC_ADJUSTMENT_E)(focus_item + 2);
// 
//                     //adjust hue
//                     if ((MMI_PIC_ADJUSTMENT_HUE == s_adjust_type)
//                         && (grade > MMIPIC_GRADE_MAX - 1)
//                         )
//                     {
//                         grade = MMIPIC_GRADE_MAX - 1;
//                     }
//                     
//                     s_hue_bar.group[focus_item].grade = grade;
// 
//                     if (MMI_PIC_SUCCESS != MMIPIC_SetAdjustmentValue(grade, s_adjust_type))
//                     {
//                         MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
//                         MMIPIC_ExitFromPicEditor(); 
//                         return (result);
//                     }
//                 }
//                 MMIPIC_DisplayOutputImage(win_id, 0);            
//                 MMIPIC_DrawProgressBarPanel(win_id, &s_hue_bar, &s_hue_bar_rect);
//                 break;
//             }
//         }
//         break;
//         
//     case MSG_CLOSE_WINDOW:
//         
//         break;
//         
//     default:
//         result = MMI_RESULT_FALSE;
//         break;
//     }
//     return (result);
// }

/********************************************************************/
//  Description : handle RGB win msg
//  Global resource dependence:
//  Author: yaye.jiang
//  Note:
/*******************************************************************/
LOCAL MMI_RESULT_E  HandleRGBWinMsg (
                                     MMI_WIN_ID_T       win_id,
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM         param
                                     )
{   
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    PEPROCESS_COLOR_BALANCE_E   s_color = PEPROCESS_CYAN_TO_RED;
    uint16                      focus_item = 0;
    GUI_BG_T                    bg_info = {0};
    GUI_RECT_T                  owndraw_ctrl_rect   = {0};
    MMI_STRING_T                title_txt = {0};
    MMI_WIN_ID_T                error_alert_id = MMIPIC_ERROR_WIN_ID ;   
    MMI_TEXT_ID_T               text_ptr[2] = {TXT_PIC_BRI_CON_HUE, TXT_COLOR};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:        
        GUIAPICTRL_SetRect(MMIPIC_ADJUST_COL_GROUP_ID, &owndraw_ctrl_rect);
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_ADJUST_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_ADJUST_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
#endif        
        SetFormParam(win_id, MMIPIC_ADJUST_COL_FORM_ID, MMIPIC_ADJUST_COL_BRI_BUTTON_ID, text_ptr, IMAG_PS_ADJUST_ATTRIBUTE, 2, IMAG_PS_ADJUST_BG);
        MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_COL_COL_BUTTON_ID);
        MMIPIC_ResetRGB();
        MMIPIC_CopyOrgImgDataToOutputImgData();
        SCI_MEMSET(&s_color_bar_rect, 0, sizeof(s_color_bar_rect));
        
        s_color_bar.focus_item = focus_item;
#ifdef    MMIPIC_SUPPORT_PNG
        s_color_bar.img_bg_id = IMAG_PS_ADJUST_PROCESS;
#else
        s_color_bar.img_bg_id = IMAGE_PIC_COLOR_BG;
#endif
        s_color_bar.total = 3;
        s_color_bar.unit_width = MMIPIC_RGB_MOVING_VALUE;
        s_color_bar.vertical_space = MMIPIC_PROGRESSBAR_SPACE;
        s_color_bar.win_id = win_id;
        s_color_bar.group[0].grade = 4;
        s_color_bar.group[0].total_level = 8;
        s_color_bar.group[0].item_type = COL_ITEM_TYPE;
        s_color_bar.group[0].item[0].col_num= ICON_COR_CYAN;
        s_color_bar.group[0].item[1].col_num= ICON_COR_RED;
        
        s_color_bar.group[1].grade = 4;
        s_color_bar.group[1].total_level = 8;
        s_color_bar.group[1].item_type = COL_ITEM_TYPE;
        s_color_bar.group[1].item[0].col_num= ICON_COR_MAGENTA;
        s_color_bar.group[1].item[1].col_num= ICON_COR_GREEN;
        
        s_color_bar.group[2].grade = 4;
        s_color_bar.group[2].total_level = 8;
        s_color_bar.group[2].item_type = COL_ITEM_TYPE;
        s_color_bar.group[2].item[0].col_num= ICON_COR_YELLOW;
        s_color_bar.group[2].item[1].col_num= ICON_COR_BLUE;
   
        break;
    case MSG_FULL_PAINT:
        MMIPIC_DisplayOutputImage(win_id, msg_id);
        MMIPIC_DrawProgressBarPanel(win_id, &s_color_bar, &s_color_bar_rect);
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_GROUP_ID))
        {
            focus_item = s_color_bar.focus_item;
            MMIPIC_DisplayCurSelectedFlag(s_color_bar_rect.group[focus_item].item[FULL_RECT_INDEX]);
        }
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_ADJUST_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_ADJUST_BG);
#endif
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        break;

    case MSG_APP_OK:        
    case MSG_CTL_OK:
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_SaveToUndoBuf();
        RecordChangedTimes();
        MMK_CloseWin(win_id);
        break;
#endif        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if(MMIPIC_ADJUST_COL_BRI_BUTTON_ID == MMK_GetActiveCtrlId(win_id))
        {
            MMK_CloseWin(win_id);
            MMK_CreateWin((uint32*)MMIPIC_ADJUS_BRI_CON_WIN_TAB, PNULL);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
#ifdef PIC_EDIT_U200_SUPPORT
            case MMIPIC_OK_CTRL_ID:
                MMIPIC_SaveToUndoBuf();
                RecordChangedTimes();
                MMK_CloseWin(win_id);
                break;

            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
#endif
            case MMIPIC_ADJUST_COL_BRI_BUTTON_ID:
                MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_APP_LEFT:
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_GROUP_ID))
        {
            focus_item = s_color_bar.focus_item;
            if (s_color_bar.group[focus_item].grade > MMIPIC_GRADE_MIN)
            {
                s_color_bar.group[focus_item].grade--;

                s_color = (PEPROCESS_COLOR_BALANCE_E)focus_item;
                if(!MMIPIC_AdjustRGB(s_color,FALSE))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                    MMIPIC_ExitFromPicEditor(); 
                    return (result);
                }        

            }    
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_APP_RIGHT:
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_GROUP_ID))
        {
            focus_item = s_color_bar.focus_item;
            if (s_color_bar.group[focus_item].grade < MMIPIC_GRADE_MAX)
            {
                s_color_bar.group[focus_item].grade++;

                s_color = (PEPROCESS_COLOR_BALANCE_E)focus_item;
                if(!MMIPIC_AdjustRGB(s_color,TRUE))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                    MMIPIC_ExitFromPicEditor(); 
                    return (result);
                }        
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
    case MSG_APP_UP:    
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_BRI_BUTTON_ID) || MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_COL_BUTTON_ID))
        {
            s_color_bar.focus_item = 2;
            MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_COL_GROUP_ID);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            if(0 == s_color_bar.focus_item)
            {
                MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_COL_FORM_ID);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else
            {
                s_color_bar.focus_item -= 1;
                focus_item = s_color_bar.focus_item;
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
    case MSG_APP_DOWN:
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_BRI_BUTTON_ID) || MMK_IsActiveCtrl(MMIPIC_ADJUST_COL_COL_BUTTON_ID))
        {
            s_color_bar.focus_item = 0;
            MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_COL_GROUP_ID);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            if(2 == s_color_bar.focus_item)
            {
                MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_COL_FORM_ID);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else
            {
                s_color_bar.focus_item += 1;
                focus_item = s_color_bar.focus_item;
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T  point_down = {0};
            int16        new_adjust = 0;
            int16        grade = 0;
            RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
            s_is_tp_progress_glide  = FALSE;
            s_pre_tp_msg            = MSG_TP_PRESS_DOWN;

            point_down.x = MMK_GET_TP_X(param);
            point_down.y = MMK_GET_TP_Y(param);

            focus_item = s_color_bar.focus_item;
            new_adjust = GetProgressBarStatusForTP(point_down, s_color_bar, s_color_bar_rect, &rect_index, &grade);

            if (focus_item != new_adjust)
            {
                focus_item = new_adjust;
                s_color_bar.focus_item = focus_item;
            }
            
            if(GUI_PointIsInRect(point_down, s_color_bar_rect.group[focus_item].item[SLIDE_RECT_INDEX]))
            {
                s_is_tp_progress_glide = TRUE;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            int16 cur_grade = 0;
            s_pre_tp_msg = MSG_TP_PRESS_MOVE;
            
            if(s_is_tp_progress_glide)
            {
                GUI_POINT_T point_move = {0};
                point_move.x = MMK_GET_TP_X(param);
                point_move.y = MMK_GET_TP_Y(param);
                focus_item = s_color_bar.focus_item;
                
                cur_grade = GetProgressGradeForPoint(point_move, focus_item);
                s_color_bar.group[focus_item].grade = cur_grade;
                
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point_up = {0};
            int16  new_adjust = 0;
            int16 grade = 0;
            RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
            
            point_up.x = MMK_GET_TP_X(param);
            point_up.y = MMK_GET_TP_Y(param);
            
            //move up
            if(s_is_tp_progress_glide && MSG_TP_PRESS_MOVE == s_pre_tp_msg)
            {
                focus_item = s_color_bar.focus_item;
                grade = GetProgressGradeForPoint(point_up, focus_item);
                s_color_bar.group[focus_item].grade = grade;
                s_color = (PEPROCESS_COLOR_BALANCE_E)focus_item;
                
                if (MMI_PIC_SUCCESS != MMIPIC_SetRGBValue(grade, s_color))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                    MMIPIC_ExitFromPicEditor(); 
                    return (result);
                }
            }
            //press up
            else
            {
                new_adjust = GetProgressBarStatusForTP(point_up, s_color_bar, s_color_bar_rect, &rect_index, &grade);
                
                if (focus_item != new_adjust)
                {
                    focus_item = new_adjust;
                    s_color_bar.focus_item = focus_item;                
                }
                
                if ((GRADE_RECT_INDEX == rect_index) && (MMIPIC_GRADE_MIN <= grade) && (grade <= MMIPIC_GRADE_MAX))
                {
                    s_color_bar.group[focus_item].grade = grade;
                    s_color = (PEPROCESS_COLOR_BALANCE_E)focus_item;
                    
                    if (MMI_PIC_SUCCESS != MMIPIC_SetRGBValue(grade, s_color))
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                        MMIPIC_ExitFromPicEditor(); 
                        return (result);
                    }
                }
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle adjust bright and constract window msg
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAdjustBrightAndConstractWinMsg (
                                                          MMI_WIN_ID_T      win_id,
                                                          MMI_MESSAGE_ID_E  msg_id, 
                                                          DPARAM            param
                                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_PIC_ADJUSTMENT_E  s_adjustment = MMI_PIC_ADJUSTMENT_BRIGHTNESS;

    MMI_STRING_T    temp_str = {0};
    MMI_IMAGE_ID_T  img_id[4]= {IMAG_PS_ADJUST_ICON_BRIGHTNESS, IMAG_PS_ADJUST_ICON_CONTRAST, IMAG_PS_ADJUST_ICON_SATURATION, IMAG_PS_ADJUST_ICON_HUE};    
    MMI_WIN_ID_T    error_alert_id = MMIPIC_ERROR_WIN_ID;
    GUI_BG_T        bg_info = {0};
    GUI_RECT_T      owndraw_ctrl_rect   = {0};

    uint16          focus_item = 0;
    MMI_STRING_T    title_txt  = {0};
    MMI_TEXT_ID_T   text_ptr[2] = {TXT_PIC_BRI_CON_HUE, TXT_COLOR};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_adjustment = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
        //SCI_TRACE_LOW:"[MMIPIC] s_adjustment = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_5190_112_2_18_2_46_5_75,(uint8*)"d",s_adjustment);
        GUIAPICTRL_SetRect(MMIPIC_ADJUST_BRI_GROUP_ID, &owndraw_ctrl_rect);

#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_ADJUST_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_ADJUST_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
#endif
        SetFormParam(win_id, MMIPIC_ADJUST_BRI_FORM_ID, MMIPIC_ADJUST_BRI_BRI_BUTTON_ID, text_ptr, IMAG_PS_ADJUST_ATTRIBUTE, 2, IMAG_PS_ADJUST_BG);
        MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_BRI_FORM_ID);

        if (s_adjustment >= MMI_PIC_ADJUSTMENT_MAX)
        {
            focus_item = 0;
        }
        focus_item = s_adjustment;
        
        MMIPIC_ResetAdjustment();

        MMIPIC_CopyOrgImgDataToOutputImgData();

        SCI_MEMSET(&s_bright_bar_rect, 0, sizeof(s_bright_bar_rect));
        
        s_bright_bar.focus_item = focus_item;
        s_bright_bar.img_bg_id = IMAG_PS_ADJUST_PROCESS;
        s_bright_bar.total = 4;
        s_bright_bar.unit_width = MMIPIC_BRIGHT_MOVING_VALUE;
        s_bright_bar.vertical_space = MMIPIC_PROGRESSBAR_SPACE;
        s_bright_bar.win_id = win_id;
        s_bright_bar.group[0].grade = 4;
        s_bright_bar.group[0].total_level = 8;
        s_bright_bar.group[0].item_type= IMG_ITEM_TYPE;
        s_bright_bar.group[0].item[0].img_id = img_id[0];
        s_bright_bar.group[0].item[1].img_id = NULL;
        s_bright_bar.group[1].grade = 4;
        s_bright_bar.group[1].total_level = 8;
        s_bright_bar.group[1].item_type= IMG_ITEM_TYPE;
        s_bright_bar.group[1].item[0].img_id = img_id[1];
        s_bright_bar.group[1].item[1].img_id = NULL;
        s_bright_bar.group[2].grade = 4;
        s_bright_bar.group[2].total_level = 8;
        s_bright_bar.group[2].item_type= IMG_ITEM_TYPE;
        s_bright_bar.group[2].item[0].img_id = img_id[2];
        s_bright_bar.group[2].item[1].img_id = NULL;
        s_bright_bar.group[3].grade = 0;
        s_bright_bar.group[3].total_level = 7;
        s_bright_bar.group[3].item_type= IMG_ITEM_TYPE;
        s_bright_bar.group[3].item[0].img_id = img_id[3];
        s_bright_bar.group[3].item[1].img_id = NULL;

        break;
    case MSG_FULL_PAINT:
        MMIPIC_DisplayOutputImage(win_id, msg_id);
        MMIPIC_DrawProgressBarPanel(win_id, &s_bright_bar, &s_bright_bar_rect);
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_GROUP_ID))
        {
            focus_item = s_bright_bar.focus_item;
            MMIPIC_DisplayCurSelectedFlag(s_bright_bar_rect.group[focus_item].item[FULL_RECT_INDEX]);
        }
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_ADJUST_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_ADJUST_BG);
#endif
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);       
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifndef PIC_EDIT_U200_SUPPORT
        MMIPIC_SaveToUndoBuf();
        RecordChangedTimes();
        MMK_CloseWin(win_id);
        break;
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if(MMIPIC_ADJUST_BRI_COL_BUTTON_ID == MMK_GetActiveCtrlId(win_id))
        {
            MMK_CloseWin(win_id);
            MMK_CreateWin((uint32*)MMIPIC_RGB_WIN_TAB, PNULL);
        }            
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
#ifdef PIC_EDIT_U200_SUPPORT
            case MMIPIC_OK_CTRL_ID:
                MMIPIC_SaveToUndoBuf();
                RecordChangedTimes();
                MMK_CloseWin(win_id);
                break;

            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
#endif
            case MMIPIC_ADJUST_BRI_COL_BUTTON_ID:                
                MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
                break;

            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_LEFT:
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_GROUP_ID))
        {
            focus_item = s_bright_bar.focus_item;
            if (s_bright_bar.group[focus_item].grade > MMIPIC_GRADE_MIN)
            {
                s_bright_bar.group[focus_item].grade--;
                
                s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;
                if(!MMIPIC_Adjustment(s_adjustment,FALSE))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                    MMIPIC_ExitFromPicEditor(); 
                    return (result);
                }        
            }
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_APP_RIGHT:    
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_GROUP_ID))
        {
            focus_item = s_bright_bar.focus_item;
            if (s_bright_bar.group[focus_item].grade < MMIPIC_GRADE_MAX)
            {
                s_bright_bar.group[focus_item].grade++;

                s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;
                if(!MMIPIC_Adjustment(s_adjustment,TRUE))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                    MMIPIC_ExitFromPicEditor(); 
                    return (result);
                }        
            }
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case MSG_APP_UP:
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_BRI_BUTTON_ID) || MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_COL_BUTTON_ID))
        {
            s_bright_bar.focus_item = 3;
            MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_BRI_GROUP_ID);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            if(0 == s_bright_bar.focus_item)
            {
                MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_BRI_FORM_ID);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else
            {
                s_bright_bar.focus_item -= 1;
                focus_item = s_bright_bar.focus_item;
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
        
    case MSG_APP_DOWN:    
        if(MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_BRI_BUTTON_ID) || MMK_IsActiveCtrl(MMIPIC_ADJUST_BRI_COL_BUTTON_ID))
        {
            s_bright_bar.focus_item = 0;
            MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_BRI_GROUP_ID);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            if(3 == s_bright_bar.focus_item)
            {
                MMK_SetAtvCtrl(win_id, MMIPIC_ADJUST_BRI_FORM_ID);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else
            {
                s_bright_bar.focus_item += 1;
                focus_item = s_bright_bar.focus_item;
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point_down = {0};
            int16  new_adjust = 0;
            int16 grade = 0;
            RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
            s_is_tp_progress_glide  = FALSE;
            s_pre_tp_msg            = MSG_TP_PRESS_DOWN;
        
            point_down.x = MMK_GET_TP_X(param);
            point_down.y = MMK_GET_TP_Y(param);
            focus_item = s_bright_bar.focus_item;
            new_adjust = GetProgressBarStatusForTP(point_down, s_bright_bar, s_bright_bar_rect, &rect_index, &grade);

            if (focus_item != new_adjust)
            {
                focus_item = new_adjust;
                s_bright_bar.focus_item = focus_item;                
            }

            if(GUI_PointIsInRect(point_down, s_bright_bar_rect.group[focus_item].item[SLIDE_RECT_INDEX]))
            {
                s_is_tp_progress_glide = TRUE;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            int16 cur_grade = 0;
            s_pre_tp_msg = MSG_TP_PRESS_MOVE;
        
            if(s_is_tp_progress_glide)
            {
                GUI_POINT_T point_move = {0};
                point_move.x = MMK_GET_TP_X(param);
                point_move.y = MMK_GET_TP_Y(param);
                focus_item = s_bright_bar.focus_item;
            
                cur_grade = GetProgressGradeForPoint(point_move, focus_item);
                s_bright_bar.group[focus_item].grade = cur_grade;
            
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point_up = {0};
            int16  new_adjust = 0;
            int16 grade = 0;
            RECT_INDEX_E rect_index = RECT_INDEX_MAX;     
    
            point_up.x = MMK_GET_TP_X(param);
            point_up.y = MMK_GET_TP_Y(param);
            
            //move up
            if(s_is_tp_progress_glide && MSG_TP_PRESS_MOVE == s_pre_tp_msg)
            {
                focus_item = s_bright_bar.focus_item;
                grade = GetProgressGradeForPoint(point_up, focus_item);
                s_bright_bar.group[focus_item].grade = grade;
                s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;//NEWMS00186092?
                if (MMI_PIC_SUCCESS != MMIPIC_SetAdjustmentValue(grade, s_adjustment))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                    MMIPIC_ExitFromPicEditor(); 
                    return (result);
                }
            }
            //press up
            else
            {
                new_adjust = GetProgressBarStatusForTP(point_up, s_bright_bar, s_bright_bar_rect, &rect_index, &grade);
                
                if (focus_item != new_adjust)
                {
                    focus_item = new_adjust;
                    s_bright_bar.focus_item = focus_item;                
                }
                
                if ((GRADE_RECT_INDEX == rect_index) && (MMIPIC_GRADE_MIN <= grade) && (grade <= MMIPIC_GRADE_MAX))
                {
                    s_bright_bar.group[focus_item].grade = grade;
                    s_adjustment = (MMI_PIC_ADJUSTMENT_E)focus_item;
                    
                    if (MMI_PIC_SUCCESS != MMIPIC_SetAdjustmentValue(grade, s_adjustment))
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
                        MMIPIC_ExitFromPicEditor(); 
                        return (result);
                    }
                }
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle effect win msg
//  Global resource dependence : 
//  Author: vincent
//  Note: yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEffectWinMsg (
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM          param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T       txt_id[MMI_PIC_EFFECT_MAX]= 
    {   TXT_PIC_GRAY,
        TXT_PIC_SEPIA,
        TXT_PIC_SHARPEN,
        TXT_PIC_SMOOTH,
        TXT_PIC_DIFFUSION,
        TXT_PIC_EMBOSS,
        TXT_PIC_EXPOSURE,
        TXT_PIC_NEGATIVE_RGB,
        TXT_PIC_DENOISE
    };
    
    MMI_WIN_ID_T error_alert_id = MMIPIC_ERROR_WIN_ID;

    MMI_STRING_T            temp_str = {0};
    GUI_POINT_T             point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_POINT_T         dis_point = {0};
    MMI_CTRL_ID_T       iconctrl_id = MMIPIC_EDIT_EFFECT_ICONLIST_CTRL_ID;
    uint16              pic_index = 0;    
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIICONLIST_DISP_T  icon_text = {0};
    MMI_STRING_T        title_txt = {0};
    MMI_IMAGE_ID_T      first_img = IMAGE_EFFECT_GRAY;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPIC_CopyOrgImgDataToOutputImgData();
#ifndef PIC_EDIT_U200_SUPPORT
        GUIICONLIST_SetDispCustomerSoftkey(iconctrl_id, TRUE);
#else
        MMI_GetLabelTextByLang(TXT_PIC_EFFECT_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_EFFECT_BG);
        SetCancelOkButtonParam(win_id, MMIPIC_OK_CTRL_ID, MMIPIC_CANCEL_CTRL_ID, FALSE);
#endif
        SetIconListParam(win_id, iconctrl_id, IMAGE_EFFECT_GRAY, MMI_PIC_EFFECT_MAX, IMAG_PS_EFFECT_BG);
        GUIICONLIST_SetStyle(iconctrl_id, GUIICONLIST_STYLE_ICON_UIDT); 

        if(!MMIPIC_GetEffectImage((MMI_PIC_EFFECT_E)pic_index))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        MMK_SetAtvCtrl(win_id, iconctrl_id);
        GUIICONLIST_SetCurIconIndex(pic_index, iconctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        MMIPIC_DisplayOutputImage(win_id, msg_id);
#ifdef PIC_EDIT_U200_SUPPORT
        MMI_GetLabelTextByLang(TXT_PIC_EFFECT_TITLE, &title_txt);
        SetPicEditTitle(win_id, &title_txt, IMAG_PS_EFFECT_BG);
#endif
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        pic_index = *((uint16 *)param); //get pic index and item index
        MMI_GetLabelTextByLang(txt_id[pic_index], &icon_text.name_str);
        GUIICONLIST_AppendText(pic_index, iconctrl_id, &icon_text);//append icon
        break;
        
    case MSG_CTL_ICONLIST_APPEND_ICON:   
        pic_index = *((uint16 *)param); //get pic index and item index
        data_info.img_id = first_img + pic_index;
        GUIICONLIST_AppendIcon(pic_index, iconctrl_id, &data_info, PNULL);//append icon
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;
        
    case MSG_TIMER:
        
        break;

    case MSG_APP_OK:  
    case MSG_CTL_OK:
        MMIPIC_SaveToUndoBuf();//保存undo buf
        RecordChangedTimes();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        pic_index = (GUIICONLIST_GetCurIconIndex(iconctrl_id)); //获得当前icon的index
        if(!MMIPIC_GetEffectImage((MMI_PIC_EFFECT_E)pic_index))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
#ifdef PIC_EDIT_U200_SUPPORT
            case MMIPIC_OK_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
                break;
                
            case MMIPIC_CANCEL_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
                break;
#endif
            case MMIPIC_EDIT_EFFECT_ICONLIST_CTRL_ID:
                MMK_PostMsg(win_id, MSG_APP_WEB, PNULL, 0);
                break;
                
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#if 0
    case MSG_APP_LEFT:
    case MSG_CTL_TITLE_MOVE_PREV:    
        s_effect--;
        if(MMI_PIC_EFFECT_GRAY==s_effect+1)
        {
            s_effect=MMI_PIC_EFFECT_DENOISE;
        }
        MMI_GetLabelTextByLang(txt_id[s_effect], &temp_str); 
        SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
        
        if(!MMIPIC_GetEffectImage(s_effect))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        MMIPIC_DisplayOutputImage(win_id, 0);
        break;        

    case MSG_APP_RIGHT: 
    case MSG_CTL_TITLE_MOVE_NEXT:    
        s_effect++;
        if(MMI_PIC_EFFECT_MAX==s_effect)
        {
            s_effect=MMI_PIC_EFFECT_GRAY;
        }
        MMI_GetLabelTextByLang(txt_id[s_effect], &temp_str); 
        SetFileNameWinTitle(win_id,temp_str.wstr_ptr,temp_str.wstr_len,TRUE);
        
        if(!MMIPIC_GetEffectImage(s_effect))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);                
            MMIPIC_ExitFromPicEditor(); 
            return (result);
        }
        MMIPIC_DisplayOutputImage(win_id, 0);      
        break;
#endif
//     case MSG_APP_UP:
//     case MSG_APP_DOWN:
//         MMIPIC_DisplayOutputImage(win_id, msg_id);
//         break;
// 
//     case MSG_TP_PRESS_MOVE:
//         {
//             point.x = MMK_GET_TP_X(param);
//             point.y = MMK_GET_TP_Y(param);
//             
//             MMIPIC_TouchPenMoveOutputImage(point, win_id, msg_id);
//         }
//         break;
//         
//     case MSG_TP_PRESS_DOWN:
//     case MSG_TP_PRESS_LONG:
//         {
//             point.x = MMK_GET_TP_X(param);
//             point.y = MMK_GET_TP_Y(param);
//             if(GUI_PointIsInRect(point, left_arrow_rect))
//             {
//                 dis_point.x = MMI_TITLE_LEFT_ARROW_LEFT;
//                 dis_point.y = MMI_TITLE_LEFT_ARROW_TOP;
//                 GUIRES_DisplayImg(&dis_point,
//                     PNULL,
//                     PNULL,
//                     win_id,
//                     IMAGE_LIST_TITLE_ARROW_LEFT_DOWN,
//                     &lcd_dev_info);           
//             }
//             else if(GUI_PointIsInRect(point, right_arrow_rect))
//             {
//                 dis_point.x = MMI_TITLE_RIGHT_ARROW_LEFT;
//                 dis_point.y = MMI_TITLE_RIGHT_ARROW_TOP;
//                 GUIRES_DisplayImg(&dis_point,
//                     PNULL,
//                     PNULL,
//                     win_id,
//                     IMAGE_LIST_TITLE_ARROW_RIGHT_DOWN,
//                     &lcd_dev_info);              
//             }
//             else
//             {
//                 MMIPIC_TouchPenMoveOutputImage(point,win_id, msg_id);
//             }
//         }
//         break;
//         
//     case MSG_TP_PRESS_UP://change arrow resource
//         point.x = MMK_GET_TP_X(param);
//         point.y = MMK_GET_TP_Y(param);
//         if (GUI_PointIsInRect(point, left_arrow_rect))
//         {
//             dis_point.x = MMI_TITLE_LEFT_ARROW_LEFT;
//             dis_point.y = MMI_TITLE_LEFT_ARROW_TOP;
//             GUIRES_DisplayImg(&dis_point,
//                 PNULL,
//                 PNULL,
//                 win_id,
//                 IMAGE_LIST_TITLE_ARROW_LEFT_UP,
//                 &lcd_dev_info);  
//             MMK_PostMsg(win_id,MSG_APP_LEFT,PNULL, 0);
//         }
//         else if (GUI_PointIsInRect(point, right_arrow_rect))
//         {
//             dis_point.x = MMI_TITLE_RIGHT_ARROW_LEFT;
//             dis_point.y = MMI_TITLE_RIGHT_ARROW_TOP;
//             GUIRES_DisplayImg(&dis_point,
//                 PNULL,
//                 PNULL,
//                 win_id,
//                 IMAGE_LIST_TITLE_ARROW_RIGHT_UP,
//                 &lcd_dev_info);
//             MMK_PostMsg(win_id,MSG_APP_RIGHT,PNULL, 0);
//         }
//         break;

    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : exit from pic editor
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIC_ExitFromPicEditor(void)
{
    if(MMK_IsOpenWin(MMIPIC_EDIT_MENU_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_MENU_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_SAVE_AS_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_SAVE_AS_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_ZOOM_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_ZOOM_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_ROTATE_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_ROTATE_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_PASTE_PREVIEW_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_PASTE_PREVIEW_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_PASTE_EDIT_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_PASTE_EDIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_ADD_CHARACTER_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_ADD_CHARACTER_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_CHAR_COLOR_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_CHAR_COLOR_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDITTEXT_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDITTEXT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_CHARACTER_PREVIEW_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_FRAME_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_FRAME_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_RESIZE_MENU_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_RESIZE_MENU_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_RESIZE_PREVIEW_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_RESIZE_PREVIEW_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_RGB_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_RGB_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_ADJUST_BRI_CON_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_ADJUST_BRI_CON_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_ADJUST_SAT_HUE_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_ADJUST_SAT_HUE_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPIC_EDIT_EFFECT_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_EDIT_EFFECT_WIN_ID);
    }    

    if(MMK_IsOpenWin(MMIPIC_NOSUPPORT_PUBWIN_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_NOSUPPORT_PUBWIN_WIN_ID);
    }   

    //added, @robert.wang, 09-8-20, cr147752
    if(MMK_IsOpenWin(MMIPIC_WAIT_PUBWIN_WIN_ID))
     {
            MMK_CloseWin(MMIPIC_WAIT_PUBWIN_WIN_ID);            
      }
    if(MMK_IsOpenWin(MMIPIC_COPY_FILE_WAIT_WIN_ID))
     {
            MMK_CloseWin(MMIPIC_COPY_FILE_WAIT_WIN_ID);            
      }
     if(MMK_IsOpenWin(MMIPIC_NOSAVE_ALERT_WIN_ID))
     {
            MMK_CloseWin(MMIPIC_NOSAVE_ALERT_WIN_ID);            
      }

    if(MMK_IsOpenWin(MMIPIC_EDIT_MAIN_WIN_ID))
    {
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(MMIPIC_EDIT_MAIN_WIN_ID);
    }
    
    MMIPUB_CloseWaitWin(MMIPIC_EDIT_MAIN_WIN_ID);
}

/*****************************************************************************/
//  Description : display success or fail  while  exiting pic editor
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
//       在退出提示时，无法处理2 个提示窗口
//      故添加此函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicExitSaveTipWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
   
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW :
            MMK_CloseWin(MMIPIC_EDIT_MAIN_WIN_ID);
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

LOCAL MMI_RESULT_E HandlePicEditorExitAlertWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_PIC_ERROR_TYPE_E pic_result = MMI_PIC_ERR_EXIT; //added, @robert.wang, 09-10-26, cr158188
   
    MMI_WIN_ID_T    wait_win_id = MMIPIC_WAIT_PUBWIN_WIN_ID; //added, @robert.wang, 09-11-2, cr147565


    switch (msg_id)
    {
    case MSG_APP_CANCEL:
        s_operation_times = 0;
        
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMIPIC_EDIT_MAIN_WIN_ID);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:  //end cr159609
    case MSG_APP_OK:
// #ifndef PIC_EDIT_U200_SUPPORT    
//         pic_result = MMIPIC_Save(TRUE);
//         MMK_CloseWin(win_id);
//         switch(pic_result)
//         {
//          case MMI_PIC_ERR_NO_SPACE:
//          case MMI_PIC_ERR_INVALID_NAME:
//          case MMI_PIC_ERR_LONG_FILENAME:
//             MMIPIC_ShowMsg(pic_result);
//             break;
//          default:
//             s_is_saved = TRUE;
//             s_operation_times = 0;
// 
//             //modified, @robert.wang, 09-11-2, cr147565
//             if(MMI_PIC_SUCCESS == pic_result)
//             {
//                 MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMI_SAVESUCC, TXT_NULL, IMAGE_PUBWIN_SUCCESS, &wait_win_id, PNULL, MMIPUB_SOFTKEY_ONE, HandlePicExitSaveTipWin);
//             }else{
//                 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_FAIL,&wait_win_id,PNULL,MMIPUB_SOFTKEY_ONE,HandlePicExitSaveTipWin);   
//             }
//             //end cr147565
//             break;
//         }
// #else
        SaveEditImg();
        MMK_CloseWin(win_id);
// #endif
        break;
		
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}

//[START]pic select folder
LOCAL uint32 MMIPIC_GetFilePathLen( uint16 * pWString )
{
	uint32 iLen = 0;

    if(PNULL == pWString)
    {
        //SCI_TRACE_LOW:"[MMIPIC]:GetFilePathLen pWString is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_5976_112_2_18_2_46_7_76,(uint8*)"");
        return 0;
    }

	while( *pWString )
	{
		iLen ++;
		pWString ++;
	}
	return iLen;
}
//[END]pic select folder


/*****************************************************************************/
// 	Description : open select patch callback
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void OpenSelectPathCallBack(BOOLEAN is_success, //in
                                        FILEARRAY file_array //in
                                        )
{
    uint32  file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    uint16 folder_len = 0;
    BOOLEAN ret = FALSE;
    
    //SCI_TRACE_LOW:"[MMIPIC]: OpenSelectPathCallBack is_success = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_6003_112_2_18_2_46_7_77,(uint8*)"d",is_success);
    if(is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        //SCI_TRACE_LOW:"[MMIPIC]: OpenSelectPathCallBack file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_6007_112_2_18_2_46_7_78,(uint8*)"d", file_num);
        if(file_num > 0)
        {        
            //read user selected dir  
            ret = MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data);
            folder_len = file_array_data.name_len;
            if( ret && (folder_len < MMIFILE_FULL_PATH_MAX_LEN - 5))
            {
                if (PNULL != s_selected_folder_ptr)
                {
                    SCI_MEMSET(s_selected_folder_ptr, 0, (sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1)));
                    MMI_WSTRNCPY(s_selected_folder_ptr,MMIFILE_FULL_PATH_MAX_LEN,
                        (file_array_data.filename),folder_len,folder_len);   
                    MMK_CreateWin((uint32 *)MMIPIC_SAVE_AS_WIN_TAB,PNULL);
                }
            }
            else
            {
                MMIPIC_ShowMsg(MMI_PIC_ERR_LONG_FILENAME);
            }
        }
    }
    
    //释放列表
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
// 	Description : open select folder win
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void MMIPic_OpenSelectFolderWin(void)
{
    FILEARRAY                   ret_array    = MMIAPIFILEARRAY_Create();
    MMIFMM_FILTER_T             filter       = {0};
    PIC_EDIT_FILE_INFO_T        file_info = {0};
    
    wchar root_path[3] = {0};
    uint16 dir_len = 0;


    MMIPIC_GetFileInfo(&file_info);
    dir_len = 2;
    MMI_WSTRNCPY(root_path, dir_len,\
       file_info.full_path_wstr_ptr, file_info.full_path_wstr_len,\
       dir_len);
       
    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(root_path,
                                            dir_len,
                                            &filter,
                                            FUNC_FIND_FOLDER,
                                            FALSE,
                                            PNULL,
                                            ret_array,
                                            OpenSelectPathCallBack,
                                            FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

/*****************************************************************************/
//  Description : get the current picture's  status  as changing
//  Global resource dependence : 
//  Author: Robert.wang
//  Date:  2009-8-20
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IsPictureChanged(void)
{
    BOOLEAN ret = FALSE;

    if (s_operation_times > 0)
    {
        ret = TRUE;
    }
    
    return  ret;
}

/*****************************************************************************/
// 	Description : handle waiting  win msg  for  saving  as while coping file
//	Global resource dependence : 
//  Author: robert.wang
//  Date: 09-8-20
//	Note: 
/*****************************************************************************/
PUBLIC  MMI_RESULT_E HandleCopyFileWaitWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       )
{

    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T error_alert_id = MMIPIC_ERROR_WIN_ID;
    BOOLEAN  copy_result=FALSE;
    
    switch( msg_id)
    {
        case MSG_PIC_COPY_FILE:
            copy_result=*(BOOLEAN *)param;

            //SCI_TRACE_LOW:"HandleCopyFileWaitWinMsg  copy_result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_6109_112_2_18_2_46_7_79,(uint8*)"d",copy_result);
            
            if(!copy_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR_OCCUR,TXT_NULL,IMAGE_PUBWIN_WARNING,&error_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
                MMIPIC_ExitFromPicEditor(); 
                return (result);
            }
            else
            {
                s_operation_times = 0;
                g_can_be_default_saved = TRUE;
                s_is_saved = TRUE;            
    					
                //add save successfully prompt	
                MMIPUB_OpenAlertSuccessWin( TXT_COMM_SAVE_SUCCESS);		
            }

            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
            break;
        default:
           result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);
}


/*****************************************************************************/
// 	Description : handle error  win msg  
//	Global resource dependence : 
//  Author: robert.wang
//  Date: 09-11-17
//	Note: 
//         close self window and  main window, exit pic editor
/*****************************************************************************/
PUBLIC  MMI_RESULT_E HandleErrorWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       )
{
	
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    
    switch( msg_id)
    {
	case MSG_CLOSE_WINDOW:
		MMIPIC_ExitFromPicEditor();
		result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
		break;
	default:
		result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
		break;
    }
	
    return (result);
}

/*****************************************************************************/
// 	Description : owner draw function for color and frame color window 
//	Global resource dependence : 
//  Author: robert.wang
//	Note: 
/*****************************************************************************/
LOCAL void AddCharColorDrawRect(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T       color_rect = {0};
    int32            rect_width = 0;   


    //draw font color rect
    color_rect = owner_draw_ptr->display_rect;
    
    rect_width = color_rect.right - color_rect.left + 1;
    
    color_rect.left += rect_width/4;
    color_rect.right -= rect_width/4;
    color_rect.top += 2;
    color_rect.bottom -= 2;

    if ((color_rect.left >= color_rect.right)
        || (color_rect.top >= color_rect.bottom)
       )
    {
        return;
    }

    GUI_FillRect(&lcd_dev_info, color_rect, s_char_color[s_char_info.color_index]);
}

/*****************************************************************************/
// 	Description : owner draw function for color and frame color window 
//	Global resource dependence : 
//  Author: robert.wang
//	Note: 
/*****************************************************************************/
LOCAL void AddCharFrameColorDrawRect(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T       color_rect = {0};
    int32            rect_width = 0;   

    
    //draw frame color rect
    color_rect = owner_draw_ptr->display_rect;

    rect_width = color_rect.right - color_rect.left + 1;
    
    color_rect.left += rect_width/4;
    color_rect.right -= rect_width/4; 
    color_rect.top += 2;
    color_rect.bottom -= 2;

    if ((color_rect.left >= color_rect.right)
        || (color_rect.top >= color_rect.bottom)
       )
    {
        return;
    }

    GUI_FillRect(&lcd_dev_info, color_rect, s_char_color[s_char_info.frame_color_index]);
}

/*****************************************************************************/
// 	Description : change state and  record changed times
//	Global resource dependence : 
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void RecordChangedTimes(void)
{
    if(!s_is_changed)
    {
        s_is_changed=TRUE;
        s_is_saved = FALSE;
    }
    s_operation_times ++;
}

/*****************************************************************************/
// 	Description : display char on pic
//	Global resource dependence : 
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void CalCharPosition(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T      actual_rect = {0};
    
    MMIPIC_CopyOrgImgDataToOutputImgData();
    actual_rect=MMIPIC_DrawText(&s_char_info.box,                           //the fixed display area
        s_char_info.content,                        //string to display
        s_char_info.length,                         //length of string to display
        s_char_info.space_two_chars,                //space between two chars.
        s_char_color[s_char_info.color_index],      //string color
        s_char_info.font,                           //font pointer  
        s_char_info.space_two_lines,                //space between 2 lines     
        s_char_color[s_char_info.frame_color_index],//font edge color/font frame color
        s_char_info.direction_type
        );      
    s_char_info.char_string_width=actual_rect.right-actual_rect.left+1;
    s_char_info.char_string_heigh=actual_rect.bottom-actual_rect.top+1;
}

/*****************************************************************************/
// 	Description :  set param of form for editor main menu win
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetMainWinFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T form_id, MMI_CTRL_ID_T first_child_id)
{
    MMI_TEXT_ID_T   txt_id[] = {TXT_PIC_ROTATE, TXT_PIC_ADJUST_ICON, TXT_EFFECT,
#ifdef PIC_EDIT_FRAME_SUPPORT
                                TXT_PIC_FRAME, 
#endif
                                TXT_PIC_CHARACTER, TXT_PIC_CLIP_ICON};
    GUI_RECT_T      form_rect   = MMITHEME_GetClientRectEx(win_id);
    uint16          form_height = MMIPICEDIT_FORM_HEIGHT;
    uint16          child_width = 0;
    GUI_BG_T        bg_set      = {0};
    GUI_BG_T        fg_set      = {0};
    GUI_FONT_ALL_T  txt_font    = {0};
    uint16          child_num   = PIC_EDIT_MAIN_MENU_NUM;
    uint16          form_hor_space  = 0;
    uint16          form_ver_space  = 0;
    uint16          i;
    
    GUIFORM_CHILD_WIDTH_T   child_width_ptr  = {0};
    GUIFORM_CHILD_HEIGHT_T  child_height_ptr = {0};
    
    GUIFORM_IsSlide(form_id, TRUE);
    
#ifndef PIC_EDIT_U200_SUPPORT
    form_rect.bottom    = form_rect.top + form_height - 1;
#else
    //form的rect设置
    form_rect.top   = form_rect.bottom - form_height + 1;
#endif
    GUIFORM_SetRect(form_id, &form_rect);

    GUIFORM_PermitChildBg(form_id,TRUE);
    GUIFORM_SetAlign(form_id, GUIFORM_CHILD_ALIGN_LEFT);
    GUIFORM_SetMargin(form_id, 0);
    GUIFORM_SetSpace(form_id, &form_hor_space, &form_ver_space);
    GUIFORM_PermitChildFont(form_id,FALSE);
    GUIFORM_PermitChildBorder(form_id, FALSE);
    {
        child_width = (form_rect.right - form_rect.left + 1) / child_num;
        child_width = MAX(child_width, form_height);
        child_width_ptr.type      = GUIFORM_CHILD_WIDTH_FIXED;
        child_width_ptr.add_data  = child_width;
        child_height_ptr.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        child_height_ptr.add_data = form_height;

        txt_font.color      = MMI_WHITE_COLOR;
        txt_font.font       = MMI_DEFAULT_SMALL_FONT;
        
        bg_set.bg_type      = GUI_BG_IMG;
        fg_set.bg_type      = GUI_BG_IMG;
        for(i = 0; i < child_num; i++)
        {
			if((child_num - 1) == i)
			{
	            child_width_ptr.add_data   = MAX(child_width, (form_rect.right - form_rect.left + 1 - i*(child_width)));
			}

            GUIFORM_SetChildWidth(form_id, first_child_id + i, &child_width_ptr);
            GUIFORM_SetChildHeight(form_id, first_child_id + i, &child_height_ptr);

            //设置button上图下文
            GUIBUTTON_SetStyle(first_child_id + i, GUIBUTTON_STYLE_UIDT);

            bg_set.img_id       = IMAG_PS_ROTATE_BG + i;
            GUIBUTTON_SetBg(first_child_id + i, &bg_set); //设置背景
            fg_set.img_id       = IMAG_PS_ICON_ROTATE + i;
            GUIBUTTON_SetFg(first_child_id + i, &fg_set); //设置前景
            
            GUIBUTTON_SetFont(first_child_id + i, &txt_font);
            GUIBUTTON_SetTextId(first_child_id + i, txt_id[i]);
            GUIBUTTON_SetRunSheen(first_child_id + i, TRUE);
            GUIBUTTON_SetOwnerDraw(first_child_id + i, DisplayFormBorder);
        }
    }

}

/*****************************************************************************/
// 	Description : open main menu icon tab ctrl
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T img_id, uint16 icon_num, MMI_IMAGE_ID_T bg_img)
{
    BOOLEAN     result      = TRUE;
    GUI_BG_T    bg_info     = {0};
    uint16      icon_width  = 0;
    uint16      icon_height  = 0;
    uint16      list_height  = 0;
    GUI_RECT_T  icon_rect   = MMITHEME_GetClientRectEx(win_id);
    
    GUI_BORDER_T    boder_type  = {0};
    GUIICONLIST_MARGINSPACE_INFO_T  icon_space = {0};
    
    list_height = MMIPICEDIT_ICONLIST_HEIGHT;
#ifndef PIC_EDIT_U200_SUPPORT
    icon_rect.bottom    = icon_rect.top + list_height - 1;
#else
    icon_rect.top = icon_rect.bottom - list_height + 1;
#endif

    //设置icon border
    boder_type.color    = MMITHEME_UNSELECT_BORDER_COLOR;
    boder_type.width    = 0;
    boder_type.type     = GUI_BORDER_SOLID;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &boder_type);    
    boder_type.width    = 2;
    boder_type.color    = 0xFEA3;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);

    //获取icon宽、高数据
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, img_id, win_id);
     
    {
        //设置横向滑动
        GUIICONLIST_SetLayoutStyle(ctrl_id, GUIICONLIST_LAYOUT_H);

        GUIICONLIST_SetLoadType(ctrl_id, GUIICONLIST_LOAD_ALL);
        //GUIICONLIST_SetCusSoftkey(ctrl_id, TRUE);//win的softkey显示

        //设置iconlist的范围和每个icon的宽高
        GUIICONLIST_SetIconWidthHeight(ctrl_id, icon_width, icon_height);
        GUIICONLIST_SetRect(ctrl_id, &icon_rect);

        //设置iconlist的间隔
        icon_space.iconlist_hmargin = MMIPICEDIT_ICON_MARGIN_H;
        icon_space.iconlist_vmargin = MMIPICEDIT_ICON_MARGIN_V;
        icon_space.icon_item_hspace = MMIPICEDIT_ICON_MARGIN_H;
        icon_space.icon_item_vspace = MMIPICEDIT_ICON_MARGIN_V;
        GUIICONLIST_SetIconItemSpace(ctrl_id, icon_space);
        
        //设置背景色
        bg_info.bg_type = GUI_BG_IMG;//GUI_BG_COLOR;//GUI_BG_NONE
        bg_info.img_id  = bg_img;
        GUIICONLIST_SetBg(ctrl_id, &bg_info);

        //设置icon数量
        GUIICONLIST_SetTotalIcon(ctrl_id, icon_num);
    }

    return result;
}

/*****************************************************************************/
// 	Description :  set param of form for adjust windows
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T child_ctrl_id, 
                               MMI_TEXT_ID_T* text_id, MMI_IMAGE_ID_T first_img_id,
                               uint16 child_ctrl_num, MMI_IMAGE_ID_T bg_img)
{
    GUI_RECT_T      form_rect   = MMITHEME_GetClientRectEx(win_id);
    uint16          form_height = MMIPICEDIT_FORM_HEIGHT;
    uint16          child_width = 0;
    GUI_BG_T        bg_set      = {0};
    GUI_FONT_ALL_T  txt_font    = {0};
    uint16          form_hor_space  = 0;
    uint16          form_ver_space  = 0;
    uint16          i;
    
    GUIFORM_CHILD_WIDTH_T   child_width_ptr  = {0};
    GUIFORM_CHILD_HEIGHT_T  child_height_ptr = {0};
    
    GUIFORM_IsSlide(ctrl_id, FALSE);
    //form的rect设置
#ifdef PIC_EDIT_U200_SUPPORT
    form_rect.top   = form_rect.bottom - form_height + 1;
#else
    form_rect.bottom= form_rect.top + form_height - 1;
#endif
    GUIFORM_SetRect(ctrl_id, &form_rect);

    //form的BG,
    bg_set.bg_type = GUI_BG_IMG;
    GUIFORM_PermitChildBg(ctrl_id,TRUE);
    //GUIFORM_SetBg(ctrl_id, &bg_set);    
    GUIFORM_SetAlign(ctrl_id, GUIFORM_CHILD_ALIGN_HMIDDLE);
    GUIFORM_SetMargin(ctrl_id, 0);
    GUIFORM_SetSpace(ctrl_id, &form_hor_space, &form_ver_space);
    GUIFORM_PermitChildFont(ctrl_id,FALSE);
    GUIFORM_PermitChildBorder(ctrl_id, FALSE);
    
    if(child_ctrl_num > 0)
    {
        child_width = (form_rect.right - form_rect.left + 1) / child_ctrl_num;
        child_width_ptr.type      = GUIFORM_CHILD_WIDTH_FIXED;
        child_width_ptr.add_data  = child_width;
        child_height_ptr.type     = GUIFORM_CHILD_HEIGHT_FIXED;
        child_height_ptr.add_data = form_height;

        txt_font.color    = MMI_WHITE_COLOR;
        if(PNULL != first_img_id)
        {
            txt_font.font     = MMI_DEFAULT_SMALL_FONT;
        }
        else
        {
            txt_font.font     = MMI_DEFAULT_NORMAL_FONT;
        }

        for(i = 0; i < child_ctrl_num; i++)
        {
			if((child_ctrl_num - 1) == i)
			{
	            child_width_ptr.add_data   = MAX(child_width, (form_rect.right - form_rect.left + 1 - i*(child_width)));
			}

            GUIFORM_SetChildWidth(ctrl_id, child_ctrl_id + i, &child_width_ptr);
            GUIFORM_SetChildHeight(ctrl_id, child_ctrl_id + i, &child_height_ptr);
            if((PNULL != first_img_id) && (PNULL != text_id))
            {
                //设置button上图下文
                GUIBUTTON_SetStyle(child_ctrl_id + i, GUIBUTTON_STYLE_UIDT);
            }
            
            bg_set.img_id  = bg_img;
            GUIBUTTON_SetBg(child_ctrl_id + i, &bg_set); //设置背景
            if(PNULL != first_img_id)
            {
                bg_set.img_id   = first_img_id + i;
                GUIBUTTON_SetFg(child_ctrl_id + i, &bg_set); //设置背景
            }
            if(PNULL != text_id)
            {                
                GUIBUTTON_SetTextId(child_ctrl_id + i, text_id[i]);
                GUIBUTTON_SetFont(child_ctrl_id + i, &txt_font);
            }
            GUIBUTTON_SetRunSheen(child_ctrl_id + i, TRUE);
            GUIBUTTON_SetOwnerDraw(child_ctrl_id + i, DisplayFormBorder);
        }
    }
}

#ifdef PIC_EDIT_U200_SUPPORT
/*****************************************************************************/
// 	Description : set title context && rect && bg_img
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetPicEditTitle(MMI_WIN_ID_T win_id, MMI_STRING_T *str_ptr, MMI_IMAGE_ID_T bg_img)
{
    BOOLEAN     result       = FALSE;
    GUI_RECT_T  title_rect   = MMITHEME_GetClientRectEx(win_id);
    uint16      title_height = MMIPICEDIT_TITLE_HEIGHT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      txt_style= {0};
    GUISTR_STATE_T      txt_state= GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD;

    //SCI_TRACE_LOW:"SetPicEditTitle: title_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_6509_112_2_18_2_46_8_80,(uint8*)"d",title_height);

    if(PNULL != str_ptr)
    {
        result  = TRUE;
        txt_style.font  = MMIPICEDIT_TITLE_FONT;
        txt_style.font_color = MMI_WHITE_COLOR;
    	txt_style.align = ALIGN_HVMIDDLE;//水平剧居中,垂直方向居中
        
        title_rect.bottom  = title_rect.top + title_height - 1;
        GUIRES_DisplayImg(PNULL,
                &title_rect,
                &title_rect,//&image_rect,
                win_id,
                bg_img,
                &lcd_dev_info );
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &title_rect, &title_rect, str_ptr, &txt_style, txt_state, GUISTR_TEXT_DIR_AUTO);
    }

    return result;
}

/*****************************************************************************/
// 	Description :  set the rect of buttons on the top of window
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetCancelOkButtonParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctr1_id, MMI_CTRL_ID_T ctr2_id, BOOLEAN is_main_win)
{
    GUI_BG_T    common_bg       = {0};
    uint16      button_height   = MMIPICEDIT_TITLE_HEIGHT;
    GUI_RECT_T  client_rect     = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T  button1_rect    = client_rect;
    GUI_RECT_T  button2_rect    = client_rect;
    uint16      img_width       = {0};
    uint16      img_height      = {0};
    GUI_FONT_ALL_T  txt_font    = {0};
    GUISTR_STYLE_T  txt_style   = {0};
    MMI_STRING_T    str_data    = {0};
    GUISTR_STATE_T  state       = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;

    if(is_main_win)
    { 
        uint16 txt_width1, txt_width2, txt_width3, max_width;
        txt_font.color  = MMI_WHITE_COLOR;
        txt_font.font   = MMIPICEDIT_TITLE_FONT;
        
        txt_style.font  = txt_font.font;
        txt_style.align = ALIGN_HMIDDLE;
        MMI_GetLabelTextByLang(STXT_SAVE, &str_data);
        txt_width1  = GUISTR_GetStringWidth(&txt_style, &str_data, state);
        MMI_GetLabelTextByLang(TXT_PIC_UNDO, &str_data);
        txt_width2  = GUISTR_GetStringWidth(&txt_style, &str_data, state);
        img_width   = MAX(txt_width1, txt_width2);
        MMI_GetLabelTextByLang(STXT_RETURN, &str_data);
        txt_width3  = GUISTR_GetStringWidth(&txt_style, &str_data, state);
        img_width   = MAX(img_width, txt_width3) + 10;
        max_width   = (client_rect.right - client_rect.left + 1) / 2;
        img_width   = MIN(img_width, max_width);

        GUIBUTTON_SetFont(ctr1_id, &txt_font);
        GUIBUTTON_SetFont(ctr2_id, &txt_font);
    }
    else
    {
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAG_PS_OK, win_id);
    }
    if(ctr1_id > 0)
    {
        //ctr1 rect设置
        button1_rect.bottom= button1_rect.top + button_height -1;
        button1_rect.left  = MMIPICEDIT_BUTTON_MARGIN_WIDTH;
        button1_rect.right = button1_rect.left + img_width - 1;
        
        GUIBUTTON_SetRect(ctr1_id, &button1_rect);
        if(is_main_win)
        {
            common_bg.bg_type = GUI_BG_NONE;
            GUIBUTTON_SetTextId(ctr1_id, STXT_SAVE);
        }
        else
        {
            common_bg.bg_type = GUI_BG_IMG;
            common_bg.img_id   = IMAG_PS_OK;
            GUIBUTTON_SetBg(ctr1_id, &common_bg); //设置背景
        }
        GUIBUTTON_SetRunSheen(ctr1_id, TRUE);
    }

    if(ctr2_id > 0)
    {
        //ctr2 rect设置
        button2_rect.bottom= button1_rect.bottom;
        button2_rect.right = client_rect.right - MMIPICEDIT_BUTTON_MARGIN_WIDTH;
        button2_rect.left  = button2_rect.right - img_width + 1;

        GUIBUTTON_SetRect(ctr2_id, &button2_rect);
        if(is_main_win)
        {
            common_bg.bg_type = GUI_BG_NONE;
            GUIBUTTON_SetTextId(ctr2_id, TXT_PIC_UNDO);
        }
        else
        {
            common_bg.bg_type = GUI_BG_IMG;
            common_bg.img_id   = IMAG_PS_CANCLE;
            GUIBUTTON_SetBg(ctr2_id, &common_bg); //设置背景
        }
        GUIBUTTON_SetRunSheen(ctr2_id, TRUE);
    }
}

/*****************************************************************************/
// 	Description :  set the param of buttons at bottom of character editor win
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetEditOptionButtonParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_TEXT_ID_T* text_id, uint16 ctrl_num, MMI_IMAGE_ID_T bg_img)
{
    BOOLEAN     result          = TRUE;
    GUI_BG_T    common_bg       = {0};
    uint16      button_height   = MMIPICEDIT_FORM_HEIGHT;
    uint16      button_width    = 0;
    GUI_RECT_T  client_rect     = MMITHEME_GetFullScreenRect();
    GUI_RECT_T  button_rect     = client_rect;
    GUI_FONT_ALL_T  txt_font    = {0};
    uint16      i;
    
    GUI_LCD_DEV_INFO   dev_info = {0};

    if(0 == ctrl_num || PNULL == ctrl_id || PNULL == win_id)
    {
        result = FALSE;
    }
    else
    {
        common_bg.bg_type = GUI_BG_IMG;
        common_bg.img_id  = bg_img;
        txt_font.color    = MMI_WHITE_COLOR;
        txt_font.font     = MMI_DEFAULT_NORMAL_FONT;
    
        button_width = (client_rect.right - client_rect.left + 1) / ctrl_num;
        button_rect.bottom = client_rect.bottom;
        button_rect.top    = button_rect.bottom - button_height + 1;
        for(i = 0; i < ctrl_num; i++)
        {
            button_rect.left   = i * button_width;
            button_rect.right  = button_rect.left + button_width - 1;
            GUIBUTTON_SetRect(ctrl_id[i], &button_rect);
            
            GUIBUTTON_SetBg(ctrl_id[i], &common_bg); //设置背景
            GUIBUTTON_SetRunSheen(ctrl_id[i], TRUE);
            GUIBUTTON_SetTextId(ctrl_id[i], text_id[i]);
            GUIBUTTON_SetFont(ctrl_id[i], &txt_font);
        }        
    }

    return result;
}

/*****************************************************************************/
// 	Description :  the handle of pubwin of charater size
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCharSizeWinMsg(MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param )
{
    MMI_RESULT_E            result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIPIC_CHAR_SIZE_LIST_CTRL_ID;
    MMI_WIN_ID_T            nosupport_win_id = MMIPIC_NOSUPPORT_PUBWIN_WIN_ID;
    
    MMI_PIC_CHAR_FONT_E     index = s_char_info.size_type;
    uint16                  cur_sel = 0;
    uint16                  font_size = MMI_DEFAULT_SMALL_FONT_SIZE;

    int16                  pic_height = 0;
    int16                  pic_width  = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
       GUILIST_SetMaxItem(ctrl_id, MMI_PIC_RESIZE_MAX, FALSE );
       //添加list选项
       MMIAPISET_AppendListItemByTextId(TXT_COMM_FONT_BIG, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
       MMIAPISET_AppendListItemByTextId(TXT_COMMON_MID, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
       MMIAPISET_AppendListItemByTextId(TXT_FONT_SMALL, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
       //设置当前选项
       GUILIST_SetSelectedItem(ctrl_id, index,TRUE);
       GUILIST_SetCurItemIndex(ctrl_id, index);   
       
       MMK_SetAtvCtrl(win_id, ctrl_id);
       break;
       
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_sel, MMI_PIC_RESIZE_MAX);

        switch(cur_sel)
        {
        case MMI_PIC_CHAR_FONT_LARGE:
            s_char_info.size_type = MMI_PIC_CHAR_FONT_LARGE;
            s_char_info.font=MMI_PIC_CHAR_LARGE_FONT;            
            font_size=MMI_PIC_CHAR_LARGE_FONT_SIZE;
            break;

        case MMI_PIC_CHAR_FONT_NORMAL:
            s_char_info.size_type = MMI_PIC_CHAR_FONT_NORMAL;
            s_char_info.font=MMI_PIC_CHAR_NORMAL_FONT;            
            font_size=MMI_PIC_CHAR_NORMAL_FONT_SIZE;
            break;

        default:
            s_char_info.size_type = MMI_PIC_CHAR_FONT_LITTLE;
            s_char_info.font=MMI_PIC_CHAR_SMALL_FONT;            
            font_size=MMI_PIC_CHAR_SMALL_FONT_SIZE; 
            break;
        }
                
        MMIPIC_GetPictureHeightAndWidth(&pic_height,&pic_width);
        if((font_size > pic_height)||(font_size > pic_width))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIC_TOO_SMALL,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {          
            MMK_CreateWin((uint32 *)MMIPIC_CHARACTER_PREVIEW_WIN_TAB, PNULL);
        }
        
        MMK_CloseWin(win_id);
        CalCharPosition(MMIPIC_CHARACTER_PREVIEW_WIN_ID);
       break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
       MMK_CloseWin(win_id);
       break;
       
    default :
       result = MMI_RESULT_FALSE;
       break;
    }    
    
    return result;
}
#endif //PIC_EDIT_U200_SUPPORT

/*****************************************************************************/
// 	Description :  save the edit of image
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SaveEditImg(void)
{
    MMK_CreatePubEditWin((uint32*)MMIPIC_SAVE_TAB, PNULL);
}

/*****************************************************************************/
// 	Description :  save the edit of image
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveMsg(MMI_WIN_ID_T win_id, 
                                 MMI_MESSAGE_ID_E msg_id, 
                                 DPARAM param )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIPIC_SAVE_EDIT_CTRL_ID;
    wchar                   forbid_char_value[]= {0x0d, 0x0a};
    MMI_PIC_ERROR_TYPE_E    pic_result = 0;
    
    static wchar            device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
    static uint16           device_name_len = 0;
    static wchar            dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
    static uint16           dir_name_len = 0;
    wchar                   no_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16                  no_suffix_name_len = 0;
	wchar                   file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16                  file_name_len = 0;
	static wchar            suffix_name[MMIFILE_SUFFIX_MAX_LEN + 1] = {0};
	static uint16           suffix_name_len = MMIFILE_SUFFIX_MAX_LEN;
    MMI_STRING_T            edit_string = {0};
    wchar                   wdot = MMIFILE_DOT;
    uint16                  max_len = 0;
        
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            MMIPIC_GetSaveLatterName();
            MMIAPIFMM_SplitFullPath(g_random_name_ptr, g_random_name_len,\
                                  device_name, &device_name_len,
                                  dir_name, &dir_name_len,
                                  file_name,&file_name_len );
			MMIAPIFMM_SplitFileName(file_name, file_name_len, no_suffix_name, &no_suffix_name_len, suffix_name, &suffix_name_len);
            
			max_len = MMIFILE_FILE_NAME_MAX_LEN - (g_random_name_len - no_suffix_name_len) - 1;
            GUIEDIT_SetMaxLen(ctrl_id, max_len);			
			GUIEDIT_SetString(ctrl_id, no_suffix_name, no_suffix_name_len);
            
            GUIEDIT_ConfigDisableChar(ctrl_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            MMK_SetAtvCtrl(win_id, ctrl_id);            
            //SCI_TRACE_LOW:"[mmipic_edit_wintab]HandleSaveMsg: case MSG_OPEN_WINDOW"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_EDIT_WINTAB_6797_112_2_18_2_46_8_81,(uint8*)"");
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        SCI_MEMSET(g_random_name_ptr, 0, g_random_name_len * sizeof(wchar));
        GUIEDIT_GetString(ctrl_id, &edit_string);
        MMIAPICOM_Wstrncpy(file_name, edit_string.wstr_ptr, edit_string.wstr_len);
		file_name_len  = edit_string.wstr_len;
        if (suffix_name_len > 0)
        {
            MMIAPICOM_Wstrncpy(file_name + file_name_len, &wdot, 1);
            file_name_len   += 1;
            MMIAPICOM_Wstrncpy(file_name + file_name_len, suffix_name, suffix_name_len);
		    file_name_len   += suffix_name_len ;
        }

        g_random_name_len   = MMIFILE_FULL_PATH_MAX_LEN;
        MMIAPIFMM_CombineFullPath(device_name, device_name_len,
                                dir_name, dir_name_len,
                                file_name, file_name_len,
                                g_random_name_ptr, &g_random_name_len);

        SaveImgFile(win_id, FALSE);
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        SaveImgFile(win_id, TRUE);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(win_id);
        MMIPUB_CloseQuerytWin(PNULL);
        break;
       
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description :  Save Img File
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SaveImgFile(MMI_WIN_ID_T win_id, BOOLEAN is_overwrite)
{
    MMI_PIC_ERROR_TYPE_E    pic_result = TRUE;
    
    pic_result = MMIPIC_Save(is_overwrite);
	
    SCI_TRACE_LOW("[mmipic_edit_wintab]SaveImgFile: pic_result = %d",(uint8*)pic_result);
    //modified, @robert.wang, 09-10-26, cr158188
    switch(pic_result)
    {
    case MMI_PIC_SUCCESS:
        s_is_changed = FALSE;
        s_operation_times = 0;
        s_is_saved = TRUE;
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMIPIC_EDIT_MAIN_WIN_ID);//保存就直接退出
        //MMIPIC_ShowMsg(pic_result);
        break;
        
    case MMI_PIC_ERR_NO_SPACE:
        MMK_CloseWin(win_id);
        MMIPIC_ShowMsg(pic_result);
        break;
        
    case MMI_PIC_ERR_LONG_FILENAME:
    case MMI_PIC_ERR_INVALID_NAME:
    case MMI_PIC_ERR_QUERY_OVERWRITE:
        MMIPIC_ShowMsg(pic_result);
        break;
    
    default:
        MMIPIC_ShowMsg(pic_result);
        MMIPIC_ExitFromPicEditor();
        break;
    }
    //end cr158200
}

/*****************************************************************************/
// 	Description : display the default img for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayBackgroudImg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          disp_rect   = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    dev_info    = {0};

    GUIRES_DisplayImg(PNULL, &disp_rect, &disp_rect, win_id, IMAG_PS_BG, &dev_info);
}

/*****************************************************************************/
// 	Description : display the focus border for form's child
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayFormBorder(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    GUI_LCD_DEV_INFO    dev_info    = {0};
    GUI_RECT_T          ctrl_rect   = {0};
    GUI_COLOR_T         disp_color  = 0xFEA3;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);

    if(MMK_IsActiveCtrl(owner_draw_ptr->ctrl_handle))
    {
        GUICTRL_GetRect(MMK_GetCtrlPtr(owner_draw_ptr->ctrl_handle),&ctrl_rect);
        LCD_DrawRect(&dev_info, ctrl_rect, disp_color);
    }
}

/*****************************************************************************/
// 	Description : set the main form buttons grayed
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetMainFormGrayed(BOOLEAN is_grayed)
{
	uint32 			i = 0;
	GUI_BG_T		button_bg	= {0};
	MMI_CTRL_ID_T 	first_ctl_id= MMIPIC_EDIT_MAINMENU_RAOTATE_ID;
	
    button_bg.bg_type      = GUI_BG_IMG;
    for(i = 0; i < PIC_EDIT_MAIN_MENU_NUM; i++)
    {
	    button_bg.img_id = IMAG_PS_ROTATE_BG + i;
    	GUIBUTTON_SetGrayed(first_ctl_id + i, is_grayed, &button_bg,TRUE);
	}
}

#endif //PIC_EDITOR_SUPPORT



/*Edit by script, ignore 22 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527


/*Edit by script, ignore 16 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
