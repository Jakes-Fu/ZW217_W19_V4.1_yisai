/***************************************************************************
** File Name:      mmitheme_clipbrd.h                                      *
** Author:         hua.fang                                                *
** Date:           06/05/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_CLIPBRD_H_
#define _MMITHEME_CLIPBRD_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "os_api.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define SHOWCLIPBRD_DRAGGING            // show clipboard when dragging
#define SHOWCLIPBRD_LONGPRESS           // show clipboard when long press

#define CLIPBRD_SLIDER_DIS_PERIOD       5000    // 3000ms

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// clipboard format
typedef enum
{
    CLIPBRD_FORMAT_TEXT = 0,

    // add other clipboard format

    CLIPBRD_FORMAT_MAX
} CLIPBRD_FORMAT_E;

// clipboard error code
typedef enum
{
    CLIPBRD_RESULT_NOERROR,
    CLIPBRD_RESULT_OUT_OF_MEMORY,
    CLIPBRD_RESULT_OUT_OF_RANGE,
    CLIPBRD_RESULT_FORMAT_NOT_SUPPORT,
    CLIPBRD_RESULT_INVALID_ARGUMENT,
} CLIPBRD_RESULT_E;

/*! @enum MMITHEME_DRAGMODE_E
@brief 拖动方式
*/
typedef enum
{
    MMITHEME_DRAGMODE_DEFAULT,       /*!<默认方式*/
    MMITHEME_DRAGMODE_SLIDER,        /*!<滑块方式*/
    MMITHEME_DRAGMODE_MAGNIFIER,     /*!<放大镜方式*/
    MMITHEME_DRAGMODE_MAX            /*!<保留位*/
} MMITHEME_DRAGMODE_E;

typedef enum
{
    MMITHEME_SLIDER_TYPE_BLOCK = 0,
    MMITHEME_SLIDER_TYPE_LEFT_SELECTOR,
    MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR,
    MMITHEME_SLIDER_TYPE_MAX
} MMITHEME_SLIDER_TYPE_E;

typedef struct
{
    MMITHEME_SLIDER_TYPE_E  slider_type;
    GUI_POINT_T             tp_point;
} MMITHEME_SLIDER_MSG_PARAM_T;

// clipboard menu data
typedef struct clipbrdmenu_data_tag
{
    MMI_HANDLE_T    dst_ctrl_handle;        // dst object handle
    GUI_POINT_T     start_point;            // menu display point
    BOOLEAN         is_cut_disabled;        // whether cut item is disabled
    BOOLEAN         is_copy_disabled;       // whether copy item is disabled
    BOOLEAN         is_paste_disabled;      // whether paste item is disabled
    BOOLEAN         is_select_all_disabled; // whether select all item is disabled
    BOOLEAN         is_select_disabled;     // whether select item is disabled
} CLIPBRDMENU_DATA_T;

#ifdef GUIF_CLIPBOARD
/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set content into clipboard by specified format
//  Global resource dependence : 
//  Author: hua.fang
//  Note: now only support CLIPBRD_FORMAT_TEXT format
//        if format is CLIPBRD_FORMAT_TEXT, do not include '\0' at the end of text.
//        and size is byte. if text is wchar, data size = length of text * size of wchar.
/*****************************************************************************/
PUBLIC CLIPBRD_RESULT_E MMITHEME_SetClipbrdData(
                                                CLIPBRD_FORMAT_E clipbrd_format, // [in] clipboard format
                                                void             *data_ptr,      // [in] data
                                                uint32           data_size       // [in] data size, unit is byte
                                                );

/*****************************************************************************/
//  Description : get content by specified format
//  Global resource dependence : 
//  Author: hua.fang
//  Note: now only support CLIPBRD_FORMAT_TEXT format
/*****************************************************************************/
PUBLIC CLIPBRD_RESULT_E MMITHEME_GetClipbrdData(
                                                CLIPBRD_FORMAT_E clipbrd_format, // [in] clipboard format
                                                void             **data_pptr,    // [out] data
                                                uint32           *data_size_ptr  // [out] data size
                                                );

/*****************************************************************************/
//  Description : check whether clipboard has content by specified format
//  Global resource dependence : 
//  Author: hua.fang
//  Note: now only support CLIPBRD_FORMAT_TEXT format
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsClipbrdAvailable(
                                           CLIPBRD_FORMAT_E clipbrd_format  // [in] clipboard format
                                           );

/*****************************************************************************/
//  Description : show clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMITHEME_ShowClipbrdMenu(                                    // [ret] return clipboard window handle
                                             const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                             );

/*****************************************************************************/
//  Description : close clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CloseClipbrdMenu(                       // [ret]
                                         MMI_HANDLE_T handle    // [in] clipboard window handle
                                         );

/*****************************************************************************/
//  Description : get max size
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetClipbrdMaxSize(void);

/*****************************************************************************/
//  Description : 创建滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_CreateSlider(
                               MMITHEME_SLIDER_TYPE_E   slider_type,
                               MMI_WIN_ID_T             win_id,
                               MMI_HANDLE_T             ctrl_handle
                               );

/*****************************************************************************/
//  Description : 销毁滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroySlider(
                                   uint32    slider_handle
                                   );

/*****************************************************************************/
//  Description : 显示滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_ShowSlider(
                                uint32   slider_handle,
                                int16    x,
                                int16    y
                                );

/*****************************************************************************/
//  Description : 隐藏滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_HideSlider(
                                uint32   slider_handle
                                );

/*****************************************************************************/
//  Description : get clipbrd height
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClipbrdHeight(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
	}
#endif
#endif
#endif  // _MMITHEME_CLIPBRD_H_
