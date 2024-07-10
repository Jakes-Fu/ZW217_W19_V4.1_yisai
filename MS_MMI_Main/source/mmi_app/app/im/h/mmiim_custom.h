#ifndef _MMIIM_CUSTOM_H_
#define _MMIIM_CUSTOM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_base.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIM_CUSTOM_KEY_PRESSED (0x0100)
#define MMIIM_CUSTOM_KEY_RELEASED (0x0200)
#define MMIIM_CUSTOM_KEY_LONG_PRESSED (0x0300)
#define MMIIM_CUSTOM_KEY_LONG_RELEASED (0x0400)
#define MMIIM_CUSTOM_KEY_REPEATED (0x0500)
#define MMIIM_CUSTOM_KEY_TYPE_MASK (0xff00)
#define MMIIM_CUSTOM_KEY_MASK (0x00ff)
#define MMIIM_CUSTOM_KEY_INVALID (0xffff)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef GUIIM_CUSTOM_KEY_TABLE_T const cGUIIM_CUSTOM_KEY_TABLE_T;

//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUIIM_INIT_KERNEL_T  init_param;

    GUI_RECT_T rect;

    BOOLEAN is_custom_kb_abc;
    GUIIM_CUSTOM_KEY_TABLE_T *custom_kb_abc_ptr;
    GUIIM_CUSTOM_KEY_TABLE_T *custom_kb_abc_upper_ptr;
    BOOLEAN is_custom_kb_123;
    GUIIM_CUSTOM_KEY_TABLE_T *custom_kb_123_ptr;

    cGUIIM_CUSTOM_KEY_TABLE_T *key_table_ptr;

    GUIIM_METHOD_T method;

    GUIIM_CUSTOM_KEY_T last_key;
    GUIIM_CUSTOM_KEY_T last_key_index;
    BOOLEAN is_timer_active;
    BOOLEAN is_lead_upper;
    GUIIM_METHOD_T lower_method;

    wchar last_char;
}MMIIM_CUSTOM_DATA_T;

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif

#endif //_MMIIM_CUSTOM_H_


