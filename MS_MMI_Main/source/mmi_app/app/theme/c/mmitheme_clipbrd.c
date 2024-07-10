/*****************************************************************************
** File Name:      mmitheme_clipbrd.c                                        *
** Author:         hua.fang                                                  *
** Date:           06/05/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#ifdef GUIF_CLIPBOARD
#include "mmitheme_clipbrd.h"
#include "mmiclipbrd_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
// data size must smaller than this value
#define CLIPBRD_MAX_SIZE        (1024 * 5)

#define MMITHEME_CLIPBRD_HEIGHT     44      // 此值为IMAGE_CLIPBRD_BG图片的高度

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
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
                                                )
{
    CLIPBRD_RESULT_E result = CLIPBRD_RESULT_NOERROR;

    result = MMIAPICLIPBRD_SetData(clipbrd_format, data_ptr, data_size);

    if (CLIPBRD_RESULT_NOERROR != result)
    {
        MMIAPICLIPBRD_ShowAlert(result);
    }

    return result;
}

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
                                                )
{
    return MMIAPICLIPBRD_GetData(clipbrd_format, data_pptr, data_size_ptr);
}

/*****************************************************************************/
//  Description : check whether clipboard has content by specified format
//  Global resource dependence : 
//  Author: hua.fang
//  Note: now only support CLIPBRD_FORMAT_TEXT format
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsClipbrdAvailable(
                                           CLIPBRD_FORMAT_E clipbrd_format  // [in] clipboard format
                                           )
{
    return MMIAPICLIPBRD_IsAvailable(clipbrd_format);
}

/*****************************************************************************/
//  Description : show clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMITHEME_ShowClipbrdMenu(                                    // [ret] return clipboard window handle
                                             const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                             )
{
    return MMIAPICLIPBRD_ShowMenu(data_ptr);
}

/*****************************************************************************/
//  Description : close clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CloseClipbrdMenu(                       // [ret]
                                         MMI_HANDLE_T handle    // [in] clipboard window handle
                                         )
{
    return MMIAPICLIPBRD_CloseMenu(handle);
}

/*****************************************************************************/
//  Description : get max size
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetClipbrdMaxSize(void)
{
    return CLIPBRD_MAX_SIZE;
}

PUBLIC uint32 MMITHEME_CreateSlider(
                               MMITHEME_SLIDER_TYPE_E   slider_type,
                               MMI_WIN_ID_T             win_id,
                               MMI_HANDLE_T             ctrl_handle
                               )
{
    return MMIAPICLIPBRD_CreateSlider(slider_type, win_id, ctrl_handle);
}

PUBLIC void MMITHEME_DestroySlider(
                              uint32    slider_handle
                              )
{
    MMIAPICLIPBRD_DestroySlider(slider_handle);
}

PUBLIC void MMITHEME_ShowSlider(
                           uint32   slider_handle,
                           int16    x,
                           int16    y
                           )
{
    MMIAPICLIPBRD_ShowSlider(slider_handle, x, y);
}

PUBLIC void MMITHEME_HideSlider(
                           uint32   slider_handle
                           )
{
    MMIAPICLIPBRD_HideSlider(slider_handle);
}

/*****************************************************************************/
//  Description : get clipbrd height
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClipbrdHeight(void)
{
    return MMITHEME_CLIPBRD_HEIGHT;
}
#endif
