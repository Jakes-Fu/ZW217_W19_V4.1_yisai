/*****************************************************************************
** File Name:      mmiclipbrd.c                                              *
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
#include "window_parse.h"
#include "mmiclipbrd.h"
#include "mmiclipbrd_id.h"
#include "mmiclipbrd_menutable.h"
#include "mmiclipbrd_text.h"
#include "mmiclipbrd_internal.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmitheme_clipbrd.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// clipboard data
typedef struct clipbrd_data_tag
{
    void    *data_ptr;      // pointer to data
    uint32  data_size;      // size of data
} CLIPBRD_DATA_T;

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
// store clipboard data
LOCAL CLIPBRD_DATA_T s_clipbrd_data[CLIPBRD_FORMAT_MAX];

/*****************************************************************************/
//  Description : alloc memory
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocFormatMem(
                             CLIPBRD_FORMAT_E   clipbrd_format,     // [in]
                             uint32             data_size           // [in]
                             );

/*****************************************************************************/
//  Description : free memory
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeFormatMem(
                            CLIPBRD_FORMAT_E    clipbrd_format       // [in]
                            );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init clipboard module
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_InitModule(void)
{
    MMICLIPBRD_Init();
    MMICLIPBRD_RegMenuGroup();
    MMICLIPBRD_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : show clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPICLIPBRD_ShowMenu(                                    // [ret]  return clipboard window handle
                                        const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                        )
{
    if (MMITHEME_DRAGMODE_DEFAULT == MMITHEME_GetDragMode())
    {
        return _MMICLIPBRD_ShowMenu(data_ptr);
    }
    else
    {
        return _MMICLIPBRD_ShowToolbar(data_ptr);
    }
}

/*****************************************************************************/
//  Description : close clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_CloseMenu(                       // [ret]
                                    MMI_HANDLE_T handle    // [in] clipboard window handle
                                    )
{
    if (MMITHEME_DRAGMODE_DEFAULT == MMITHEME_GetDragMode())
    {
        return _MMICLIPBRD_CloseMenu(handle);
    }
    else
    {
        return _MMICLIPBRD_CloseToolbar(handle);
    }
}

/*****************************************************************************/
//  Description : show alert window by error code
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_ShowAlert(                                    // [ret] TRUE when succeeds
                                    CLIPBRD_RESULT_E error_code         // [in] data
                                    )
{
    BOOLEAN result = FALSE;

    if (CLIPBRD_RESULT_OUT_OF_RANGE == error_code)
    {
        MMIPUB_OpenAlertWarningWin( TXT_CLIPBRD_CONTENT_TOO_LARGE);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : initialize the clipboard
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICLIPBRD_Init(void)
{
    SCI_MEMSET(s_clipbrd_data, 0, sizeof(s_clipbrd_data));

    return TRUE;
}

/*****************************************************************************/
//  Description : close the clipboard
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICLIPBRD_Close(void)
{
    uint32 i = 0;

    for (; i < CLIPBRD_FORMAT_MAX; i++)
    {
        FreeFormatMem(i);
    }

    return TRUE;
}

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
                                           )
{
    // to assure param is correct
    if (PNULL == data_ptr || 0 == data_size)                    // must have data
    {
        return CLIPBRD_RESULT_INVALID_ARGUMENT;
    }

#ifdef GUIF_CLIPBOARD
    if (MMITHEME_GetClipbrdMaxSize() < data_size)/*lint !e574*//*lint !e737*/
    {
        return CLIPBRD_RESULT_OUT_OF_RANGE;
    }
#endif

    if (CLIPBRD_FORMAT_MAX <= clipbrd_format)
    {
        return CLIPBRD_RESULT_FORMAT_NOT_SUPPORT;
    }

    // clear content first
    FreeFormatMem(clipbrd_format);

    if (!AllocFormatMem(clipbrd_format, data_size))
    {
        return CLIPBRD_RESULT_OUT_OF_MEMORY;
    }

    SCI_MEMCPY(s_clipbrd_data[clipbrd_format].data_ptr, data_ptr, data_size);
    s_clipbrd_data[clipbrd_format].data_size = data_size;

    return CLIPBRD_RESULT_NOERROR;
}

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
                                           )
{
    if (PNULL == data_pptr && PNULL == data_size_ptr)
    {
        return CLIPBRD_RESULT_INVALID_ARGUMENT;
    }

    if (CLIPBRD_FORMAT_MAX <= clipbrd_format)
    {
        return CLIPBRD_RESULT_FORMAT_NOT_SUPPORT;
    }

    if (PNULL != data_size_ptr)
    {
        *data_size_ptr = s_clipbrd_data[clipbrd_format].data_size;
    }

    if (PNULL != data_pptr)
    {
        *data_pptr = s_clipbrd_data[clipbrd_format].data_ptr;
    }

    return CLIPBRD_RESULT_NOERROR;
}

/*****************************************************************************/
//  Description : check whether clipboard has content by specified format
//  Global resource dependence : 
//  Author: hua.fang
//  Note: now only support CLIPBRD_FORMAT_TEXT format
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_IsAvailable(
                                      CLIPBRD_FORMAT_E clipbrd_format  // [in] clipboard format
                                      )
{
    BOOLEAN is_available = FALSE;

    if (CLIPBRD_FORMAT_MAX <= clipbrd_format)
    {
        return is_available;
    }

    is_available = (0 != s_clipbrd_data[clipbrd_format].data_size);

    return is_available;
}

/*****************************************************************************/
//  Description : clear all content in clipboard
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICLIPBRD_Clear(
                                CLIPBRD_FORMAT_E clipbrd_format  // [in] clipboard format
                                )
{
    if (CLIPBRD_FORMAT_MAX <= clipbrd_format)
    {
        return FALSE;
    }

    return FreeFormatMem(clipbrd_format);
}

/*****************************************************************************/
//  Description : alloc memory
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocFormatMem(
                             CLIPBRD_FORMAT_E   clipbrd_format,     // [in]
                             uint32             data_size           // [in]
                             )
{
    // alloc another sizeof(wchar) memory to store '\0', user can set the data
    // which is not ended by '\0'.
    s_clipbrd_data[clipbrd_format].data_ptr = SCI_ALLOCA(data_size + sizeof(wchar));
    if (PNULL == s_clipbrd_data[clipbrd_format].data_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(s_clipbrd_data[clipbrd_format].data_ptr, 0, data_size);

    return TRUE;
}

/*****************************************************************************/
//  Description : free memory
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeFormatMem(
                            CLIPBRD_FORMAT_E    clipbrd_format       // [in]
                            )
{
    if (PNULL != s_clipbrd_data[clipbrd_format].data_ptr)
    {
        SCI_FREE(s_clipbrd_data[clipbrd_format].data_ptr);

        s_clipbrd_data[clipbrd_format].data_ptr = PNULL;
        s_clipbrd_data[clipbrd_format].data_size = 0;
    }

    return TRUE;
}
