/*****************************************************************************
** File Name:      ctrlbaseflex_common.c                                     *
** Author:         hua.fang                                                  *
** Date:           09/14/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "mmi_gui_trc.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define GUIEDIT_DIGITAL_0   '0'
#define GUIEDIT_DIGITAL_9   '9'

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

/*****************************************************************************/
//  Description : is disable char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisableChar (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               char_value     //in
);

/*****************************************************************************/
//  Description : is add enable char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAddEnableChar (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar				char_value     //in
);

/*****************************************************************************/
//  Description : check string len
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckEditStringLen (//string is valid
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               *str_ptr,       //in:
    uint16              str_len,        //in:
    uint16              *actual_len_ptr //in/out:
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit reverse string
//  Global resource dependence :
//  Author: Jassmine
//  Note: 必须和FreeEditReverseString配对使用,负责内存泄露
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_GetReverseString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
    wchar               **str_pptr,         //in/out:
    uint16              *str_len_ptr        //in/out:
)
{
    BOOLEAN             result = FALSE;
    uint16              i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir) &&
        (0 < baseedit_ctrl_ptr->str_len))
    {
        //alloc string memory
        *str_pptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_len);

        for (i = 0; i < baseedit_ctrl_ptr->str_len; i++)
        {
            (*str_pptr) [i] = baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->str_len-1-i];
        }

        *str_len_ptr = baseedit_ctrl_ptr->str_len;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : free edit reverse string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_FreeReverseString (
    wchar     **str_pptr
)
{
    if ((PNULL != str_pptr) && (PNULL != *str_pptr))
    {
        SCI_FREE (*str_pptr);
    }
}

/*****************************************************************************/
//  Description : check edit string is valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CheckEditString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               *str_ptr,           //in:
    uint16              *str_len_ptr        //in/out:
)
{
    BOOLEAN     result = FALSE;

    //check string len
    if (CheckEditStringLen (baseflex_ctrl_ptr, str_ptr, *str_len_ptr, str_len_ptr))
    {
        result = VTLBASEFLEX_IsStringValid (baseflex_ctrl_ptr, str_ptr, *str_len_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : check text string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CheckTextStr (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
    wchar               *str_ptr,       //in:
    uint16              str_len         //in:
)
{
    BOOLEAN             result = TRUE;
    uint16              i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((0 != baseedit_ctrl_ptr->im_ctrl_handle) &&
        (GUIIM_IsOnlyInputAscii (baseedit_ctrl_ptr->im_ctrl_handle)))
    {
        if (0 < MMITHEME_CountUcs2Char (FALSE, str_ptr, str_len, GUIEDIT_SMS_ENCODE_AUTO))
        {
            for (i = 0; i < str_len; i++)
            {
                if ((MMITHEME_IsUcs2Char (FALSE, str_ptr[i], GUIEDIT_SMS_ENCODE_AUTO)) &&
                    (!IsAddEnableChar (baseflex_ctrl_ptr, str_ptr[i])))
                {
                    result = FALSE;
                    break;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is digital char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_IsDigitalChar (
    wchar   char_code
)
{
    BOOLEAN     result = FALSE;

    if ((GUIEDIT_DIGITAL_0 <= char_code) &&
        (GUIEDIT_DIGITAL_9 >= char_code))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : check string is digital string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CheckDigitalStr (
    wchar     *str_ptr,
    uint16    str_len
)
{
    BOOLEAN     result = TRUE;
    uint16      i = 0;

    for (i = 0; i < str_len; i++)
    {
        if (!BASEFLEX_IsDigitalChar (str_ptr[i]))
        {
            result = FALSE;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : 截掉了非首位的+号以及后面的字符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEFLEX_InterceptPhoneNum (//截取后的长度
    wchar    *str_ptr,
    uint16   str_len
)
{
    uint16      intercept_len = 0;
    uint16      i = 0;

    //截掉了非首位的+号以及后面的字符
    intercept_len = str_len;

    for (i = 1; i < str_len; i++)
    {
        if ('+' == str_ptr[i])
        {
            intercept_len = i;
            break;
        }
    }

    return (intercept_len);
}

/*****************************************************************************/
//  Description : is disable char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisableChar (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               char_value     //in
)
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;

    for (i = 0; i < baseflex_ctrl_ptr->disable_char_len; i++)
    {
        if (char_value == baseflex_ctrl_ptr->disable_char_ptr[i])
        {
            result = TRUE;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is add enable char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAddEnableChar (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar				char_value     //in
)
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;

    for (i = 0; i < baseflex_ctrl_ptr->add_enable_char_len; i++)
    {
        if (char_value == baseflex_ctrl_ptr->add_enable_char_ptr[i])
        {
            result = TRUE;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : check string len
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckEditStringLen (//string is valid
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               *str_ptr,       //in:
    uint16              str_len,        //in:
    uint16              *actual_len_ptr //in/out:
)
{
    BOOLEAN     result = TRUE;
    uint16      i = 0;

    if ((PNULL != str_ptr) && (PNULL != actual_len_ptr))
    {
        *actual_len_ptr = str_len;

        for (i = 0; i < str_len; i++)
        {
            if (0 == str_ptr[i])
            {
                *actual_len_ptr = i;
                break;
            }
            else if (IsDisableChar (baseflex_ctrl_ptr, str_ptr[i])) //is disable char
            {
                result = FALSE;
                break;
            }
        }
    }

    return (result);
}

