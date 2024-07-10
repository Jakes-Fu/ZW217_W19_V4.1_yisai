
#ifndef _MMI_SZTE_MENU_H_
#define _MMI_ZTE_MENU_H_ 

#include "sci_types.h"
#include "mmk_type.h"

PUBLIC void zte_menu_semicircle_close_win( void );

PUBLIC void zte_menu_semicircle_open_win(void);

PUBLIC MMI_RESULT_E HandleMsgMenuSemicircleWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );

#endif
