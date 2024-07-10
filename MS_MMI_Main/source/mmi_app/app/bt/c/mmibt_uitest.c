/****************************************************************************
** File Name:     mmibt_uitest.c                                     *
** Author:         yuantao.xu                                            *
** Date:           8/22/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth device paired*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/22/2012    yuantao.xu           Create                                  *
**                                                                         *
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#define _MMIBT_UITEST_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmibt_a2dp.h"
#include "mmibt_app.h"
#include "mmibt_nv.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_uitest.h"
#include "mmieng_text.h"
#include "mmieng_id.h"
#include "guictrl_api.h"
#include "mmieng_uitestwin.h"
#include "mmieng_export.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL   BOOLEAN     s_bt_eng_bt_status = FALSE;
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: Handle blue tooth ui test result win msg
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL   MMI_RESULT_E HandleBtUiResultWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            );
//the window for blue tooth UI test result
WINDOW_TABLE(MMIBT_UITEST_RESULT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBtUiResultWinMsg),    
    WIN_ID(MMIBT_UITEST_RESULT_WIN_ID),
    WIN_TITLE(TXT_BLUETOOTH),
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMIBT_UI_RESULT_LABEL_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif        
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN    
};

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth ui test result win msg
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL   MMI_RESULT_E HandleBtUiResultWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            )                                            
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    wait_text = {0};
	MMI_STRING_T    result_text = {0};
    GUI_BOTH_RECT_T both_rect = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text); 
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            both_rect = MMITHEME_GetWinClientBothRect(win_id);
            both_rect.h_rect.top = (both_rect.h_rect.bottom - GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT)) /2;
            both_rect.v_rect.top = (both_rect.v_rect.bottom - GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT))/2;
			both_rect.h_rect.bottom = both_rect.h_rect.top + GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT) +1;
			both_rect.v_rect.bottom = both_rect.v_rect.top + GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT) +1;
            GUIAPICTRL_SetBothRect(MMIBT_UI_RESULT_LABEL_CTRL_ID, &both_rect);
			result_text.wstr_ptr = L"Pass/Fail Key to Next";
			result_text.wstr_len = MMIAPICOM_Wstrlen(L"Pass/Fail Key to Next");
			GUILABEL_SetText(MMIBT_UI_RESULT_LABEL_CTRL_ID, &result_text, FALSE);
        }        
        break;
        
    case MSG_FULL_PAINT:
        MMIBT_ClrClientRect(win_id);
        MMIDEFAULT_SetBackLight(TRUE);
        MMIDEFAULT_SetLcdBackLight(TRUE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL, FALSE);
        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);

                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif  //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
//@CR NEWMS00166316 leon.wang 2012/3/6
#ifdef MMI_PDA_SUPPORT 
    case MSG_APP_MENU:
#endif
//@end
    case MSG_APP_OK:
#ifdef SCREENSAVER_SUPPORT
        MMK_CloseWin(MMIIDLE_SCREENSAVER_WIN_ID);
#endif
        MMK_CloseWin(MMIKL_CLOCK_DISP_WIN_ID);
        if (!s_bt_eng_bt_status && BT_GetState())
        {
            //only the bt status is OFF before UI Test, and bt current status is ON, need close BT
            MMIBT_OpenBluetooth(FALSE, TRUE);
        }
        else
        {
            MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);	                     
        }
        MMIAPIENG_SaveTestResult(MMIENG_UITESTBLUETOOTH_WIN_ID, 1) ;
        MMIAPIENG_RunUITestNextStep();      
        MMK_CloseWin(win_id);
        break;     
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        if (!s_bt_eng_bt_status && BT_GetState())
        {
            //only the bt status is OFF before UI Test, and bt current status is ON, need close BT
            MMIBT_OpenBluetooth(FALSE, TRUE);
        }
        else
        {
            MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);	   
        }
        
        MMIAPIENG_SaveTestResult(MMIENG_UITESTBLUETOOTH_WIN_ID, 0) ;
        MMIAPIENG_RunUITestNextStep();           
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:      // close flip
    case MSG_KEYDOWN_RED:
        break;  
#endif        
        
    case MSG_CTL_OK:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Discription: open blue tooth ui test window to test
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenUitestWin(BOOLEAN  bluetooth_on)
{
    s_bt_eng_bt_status = bluetooth_on;
    
    if (bluetooth_on)
    {
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
        MMIBT_OpenSearchDeviceWaitWin(BT_SERVICE_ALL);
    }
    else
    {
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
        MMIBT_OpenBluetooth(TRUE, TRUE);
    }
}
/*****************************************************************************/
//  Discription: open blue tooth ui test window to test
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CreateUitestWin(void)
{
   MMK_CreateWin((uint32 *)MMIBT_UITEST_RESULT_WIN_TAB, PNULL);
}
#endif