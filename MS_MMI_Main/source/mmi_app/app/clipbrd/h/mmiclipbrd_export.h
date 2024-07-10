/***************************************************************************
** File Name:      mmiclipbrd_export.h                                            *
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
#ifndef _MMI_CLIPBRD_EXPORT_H_
#define _MMI_CLIPBRD_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmitheme_clipbrd.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init clipboard module
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_InitModule(void);


/*****************************************************************************/
//  Description : set content into clipboard by specified format
//  Global resource dependence : 
//  Author: hua.fang
//  Note: now only support CLIPBRD_FORMAT_TEXT format
//        if format is CLIPBRD_FORMAT_TEXT, do not include '\0' at the end of text.
//        and size is byte. if text is wchar, data size = length of text * size of wchar.
/*****************************************************************************/
PUBLIC CLIPBRD_RESULT_E MMIAPICLIPBRD_SetData(
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
PUBLIC CLIPBRD_RESULT_E MMIAPICLIPBRD_GetData(
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
PUBLIC BOOLEAN MMIAPICLIPBRD_IsAvailable(
                                      CLIPBRD_FORMAT_E clipbrd_format  // [in] clipboard format
                                      );

/*****************************************************************************/
//  Description : clear all content in clipboard
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_Clear(
                                CLIPBRD_FORMAT_E clipbrd_format  // [in] clipboard format
                                );

/*****************************************************************************/
//  Description : show clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPICLIPBRD_ShowMenu(                                    // [ret] return clipboard window handle
                                        const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                        );

/*****************************************************************************/
//  Description : close clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_CloseMenu(                       // [ret]
                                    MMI_HANDLE_T handle    // [in] clipboard window handle
                                    );

/*****************************************************************************/
//  Description : show alert window by error code
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_ShowAlert(                                    // [ret] TRUE when succeeds
                                    CLIPBRD_RESULT_E error_code         // [in] data
                                    );

/*****************************************************************************/
//  Description : 创建滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICLIPBRD_CreateSlider(
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
PUBLIC void MMIAPICLIPBRD_DestroySlider(
                                     uint32    slider_handle
                                     );

/*****************************************************************************/
//  Description : 显示滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_ShowSlider(
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
PUBLIC void MMIAPICLIPBRD_HideSlider(
                                  uint32   slider_handle
                                  );

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMICLIPBRD_InitModule       MMIAPICLIPBRD_InitModule
#define MMICLIPBRD_SetData          MMIAPICLIPBRD_SetData
#define MMICLIPBRD_GetData          MMIAPICLIPBRD_GetData
#define MMICLIPBRD_IsAvailable      MMIAPICLIPBRD_IsAvailable
#define MMICLIPBRD_Clear            MMIAPICLIPBRD_Clear
#define MMICLIPBRD_ShowMenu         MMIAPICLIPBRD_ShowMenu
#define MMICLIPBRD_CloseMenu        MMIAPICLIPBRD_CloseMenu
#define MMICLIPBRD_ShowAlert        MMIAPICLIPBRD_ShowAlert
#define MMICLIPBRD_CreateSlider     MMIAPICLIPBRD_CreateSlider
#define MMICLIPBRD_DestroySlider    MMIAPICLIPBRD_DestroySlider
#define MMICLIPBRD_ShowSlider       MMIAPICLIPBRD_ShowSlider
#define MMICLIPBRD_HideSlider       MMIAPICLIPBRD_HideSlider

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMI_CLIPBRD_H_
