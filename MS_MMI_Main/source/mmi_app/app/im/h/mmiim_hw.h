#ifndef _MMIIM_HW_H_
#define _MMIIM_HW_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_tp_base.h"

#if defined(MMIIM_USE_HANDWRITING)

#if defined(MMIIM_USE_HANDWRITING_HANWANG)
#include "hwapi.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
enum
{
    MMIIM_HW_TRACE_LENGTH_MAX = 512,
    MMIIM_HW_DEFAULT_TRACE_COLOR = 0x001f,
    MMIIM_HW_DEFAULT_TRACE_WIDTH = 3,
    MMIIM_HW_DEFAULT_LABLE_WIDTH = 3,
};


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUIIM_INIT_KERNEL_T init_param;
    GUI_LCD_DEV_INFO tips_lcd_info;

    BOOLEAN is_timer_active;
    BOOLEAN is_char_underline;

    GUI_RECT_T rect_write;
    GUI_RECT_T rect_draw;

    MMIIM_CAND_WIN_T cand_win;
    
    MMIIM_TP_VKB_T vkb;

#if defined(MMIIM_USE_HANDWRITING_HANWANG)
    THWPoint trace[MMIIM_HW_TRACE_LENGTH_MAX];
    uint32 trace_len;
    THWAttribute attr;
#endif
#if defined(MMIIM_USE_HANDWRITING_XUNFEI)
	Thw_HWPoint trace[MMIIM_HW_TRACE_LENGTH_MAX];
	uint32 trace_len;
	THWAttribute attr;
#endif
    wchar result[MAXCANDNUM];
    int32 result_len;

    BOOLEAN is_writing;
    BOOLEAN has_recognized;

    uint32 recognize_tick;
    GUI_COLOR_T trace_color;
    uint16 trace_width;
}MMIIM_HW_DATA_T;



/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

#ifdef __cplusplus 
}
#endif

#endif //MMIIM_USE_HANDWRITING

#endif //_MMIIM_HW_H_



