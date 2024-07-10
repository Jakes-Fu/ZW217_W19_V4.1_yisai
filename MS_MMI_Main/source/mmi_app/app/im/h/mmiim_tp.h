#ifndef _MMIIM_TP_H_
#define _MMIIM_TP_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_tp_base.h"

#if defined(MMIIM_USE_TP_KB)

#include "spim_ch_py26k.h"
#if defined(MMIIM_USE_TP_CSTAR)
#include "csk2api.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUIIM_INIT_KERNEL_T init_param;
    GUI_LCD_DEV_INFO tips_lcd_info;

    BOOLEAN is_py_initialized;
    SPIM_HANDLE_T py_core;

    BOOLEAN is_py_inputting;
    MMIIM_PRE_EDIT_WIN_T edit_win;
    MMIIM_CAND_WIN_T cand_win;
    
    GUIIM_METHOD_T method;

    MMIIM_TP_VKB_T vkb;
}MMIIM_TP_DATA_T;



/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

#ifdef __cplusplus 
}
#endif

#endif //MMIIM_USE_TP_KB

#endif //_MMIIM_TP_H_


