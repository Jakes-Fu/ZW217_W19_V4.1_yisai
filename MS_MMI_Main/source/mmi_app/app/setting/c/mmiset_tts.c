/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_TTS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmimms_export.h"
#include "mmiset_func.h"
#include "mmiset_display.h"


#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : to handle set TTS
//  Global resource dependence : 
//  Author:barry.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTTSWindow(
                                       MMI_WIN_ID_T    win_id, 
                                       MMI_MESSAGE_ID_E   msg_id, 
                                       DPARAM             param
                                       );

#endif

#ifdef TTS_SUPPORT
// the window for set tts //barry.peng
WINDOW_TABLE( MMISET_SET_TTS_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetTTSWindow ),    
        WIN_ID( MMISET_SET_TTS_WIN_ID ),
        WIN_TITLE( TXT_SET_TTS ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_TTS_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif


#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : to handle set TTS
//  Global resource dependence : 
//  Author:barry.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTTSWindow(
                                       MMI_WIN_ID_T    win_id, 
                                       MMI_MESSAGE_ID_E   msg_id, 
                                       DPARAM             param
                                       )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_TTS_CTRL_ID,MMISET_TTS_MAX_NUM, FALSE );
        MMISET_AppendTTSListItem(MMISET_SET_TTS_CTRL_ID);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_TTS_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (!GUILIST_IsCurrItemInvalid(MMISET_SET_TTS_CTRL_ID))
        {
            MMIAPISET_SetTTSInfo(MMISET_SET_TTS_CTRL_ID); 
            //success!
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
