#ifndef _MMIIM_CSTAR_H_
#define _MMIIM_CSTAR_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_base.h"

#if defined(MMIIM_USE_KB_CSTAR)

#include "csk2api.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIM_CSTAR_KEY_TYPE_MASK (0xff00)
#define MMIIM_CSTAR_KEY_MASK (0x00ff)
#define MMIIM_CSTAR_VKEY_PRESSSED (0x00000000)
#define MMIIM_CSTAR_VKEY_RELEASED (0x00100000)


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUI_LCD_DEV_INFO *lcd_info_ptr;
    GUIIM_INIT_KERNEL_T  init_param;
    CS_HIMM im_handle;
    
    GUI_RECT_T rect;

    GUIIM_METHOD_T method;

    MMIIM_PRE_EDIT_WIN_T edit_win;
    MMIIM_CHOICE_WIN_T   choice_win;
    MMIIM_CAND_WIN_T cand_win;
    
    BOOLEAN is_timer_active;
}MMIIM_CSTAR_DATA_T;


/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

#ifdef __cplusplus 
} 
#endif 


#endif //MMIIM_USE_KB_CSTAR

#endif //_MMIIM_CSTAR_H_

