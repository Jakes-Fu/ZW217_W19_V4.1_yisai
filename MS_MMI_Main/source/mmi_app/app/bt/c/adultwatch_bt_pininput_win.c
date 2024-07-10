/*****************************************************************************
** File Name:      adultwatch_bt_pininput.c                                  *
** Author:         chunjuan.liang                                            *
** Date:           06/07/2021                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2021      chunjuan.liang            Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "window_parse.h"
#include "mmk_type.h"

#include "ctrlim_base.h"
#include "guiedit.h"
#include "guibutton.h"
#include "mmibt_id.h"
#include "mmibt_text.h"

#include "mmi_common.h"
#include "watch_common.h"
#include "watch_common_btn.h"
#include "mmidisplay_data.h"
#include "mmicom_trace.h"

#include "ual_bt.h"
#include "watch_commonwin_export.h"

#define PIN_EDIT_RECT_LEFT DP2PX_VALUE(0)
#define PIN_EDIT_RECT_RIGHT DP2PX_VALUE(240)
#define PIN_EDIT_RECT_TOP DP2PX_VALUE(10)
#define PIN_EDIT_RECT_BOTTOM DP2PX_VALUE(160)
#ifdef ADULT_WATCH_SUPPORT
#define PIN_EDIT_MARGIN_LEFTRIGHT DP2PX_VALUE(37)
#define PIN_EDIT_MARGIN_TOPBOTTOM DP2PX_VALUE(5)
#endif


LOCAL ual_bt_dev_info_t   g_inpupin_dev_info = {0};//pin code


LOCAL uint8   g_pin_code[UAL_BT_DEV_SSP_NUM_MAX_LEN] = {0};

LOCAL void BT_Pininput_APP_OK( MMI_WIN_ID_T win_id )
{
    MMI_STRING_T wait_text = {0};
    MMI_CTRL_ID_T   editCtrlId =  MMIBT_ADULTWACHT_PIN_EDIT_ID;
    ual_bt_status_e return_value = UAL_BT_STATUS_SUCCESS;
    MMI_STRING_T    txtPin = {0};

    GUIEDIT_GetString( editCtrlId, &txtPin );
    SCI_MEMSET(g_pin_code,0,UAL_BT_DEV_SSP_NUM_MAX_LEN);
    TRACE_APP_BT(" txtPin.wstr_len = %d",txtPin.wstr_len);
    if(0 < txtPin.wstr_len)
    {
        MMI_WSTRNTOSTR( g_pin_code, UAL_BT_DEV_SSP_NUM_MAX_LEN, txtPin.wstr_ptr, txtPin.wstr_len, txtPin.wstr_len );
    }
    else
    {
        TRACE_APP_BT(" txtPin.wstr_len = 0");
    }

    TRACE_APP_BT("g_inpupin_dev_info.addr=%d,%d,%d,%d,%d,%d,g_pin_code=%s ",g_inpupin_dev_info.addr[0],g_inpupin_dev_info.addr[1],g_inpupin_dev_info.addr[2],
                 g_inpupin_dev_info.addr[3],g_inpupin_dev_info.addr[4],g_inpupin_dev_info.addr[5],g_pin_code);
    TRACE_APP_BT("g_pin_code len = %d",strlen(g_pin_code));
    return_value = ual_bt_dev_input_pinpair(g_inpupin_dev_info.addr,(uint8*)g_pin_code,strlen(g_pin_code));

    if (UAL_BT_STATUS_PENDING == return_value)
    {
        MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
        //close pin edit win
        if(MMK_IsOpenWin(ADULTWATCH_BT_PAIR_PININPUT_WIN_ID))
        {
            MMK_CloseWin(ADULTWATCH_BT_PAIR_PININPUT_WIN_ID);
        }
        AdultWatchCOM_WaitingWin_1Icon_1Str_Enter(ADULTWATCH_BT_PAIR_WAITING_WIN_ID,&wait_text,PNULL,PNULL);
    }
    else
    {
        TRACE_APP_BT("return_value = %d",return_value);

    }


    MMK_CloseWin( win_id );
}

LOCAL void BT_Pininput_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E BT_PininputBtnCancelCb( void )
{
    ual_bt_dev_cancel_pair(g_inpupin_dev_info.addr);
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E BT_PininputBtnOKCb( void )
{
    BT_Pininput_APP_OK(ADULTWATCH_BT_PAIR_PININPUT_WIN_ID);
    return MMI_RESULT_TRUE;
}

LOCAL void BT_Pininput_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T      editRect = { PIN_EDIT_RECT_LEFT,
                                 PIN_EDIT_RECT_TOP,
                                 PIN_EDIT_RECT_RIGHT,
                                 PIN_EDIT_RECT_BOTTOM};
    MMI_CTRL_ID_T   editCtrlId =  MMIBT_ADULTWACHT_PIN_EDIT_ID;

    BOOLEAN         reCode = FALSE;
    MMI_CTRL_ID_T   cancelBtn = MMIBT_ADULTWACHT_PIN_CANCEL_BTN_ID;
    MMI_CTRL_ID_T   okBtn = MMIBT_ADULTWACHT_PIN_CNF_BTN_ID;
    GUI_FONT_ALL_T  tempFont = {WATCH_COMMONWIN_NORMAL_FONT,WATCH_COMMONWIN_FONT_COLOR};
    wchar           default_pin_code[] = {'1','2','3','4', 0};

    TRACE_APP_BT(" Enter");

    GUIEDIT_SetRect( editCtrlId, &editRect );
    GUIEDIT_SetIm( editCtrlId, GUIIM_TYPE_SYS, GUIIM_TYPE_SYS );
    GUIEDIT_SetStyle(editCtrlId, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetString(editCtrlId,default_pin_code,MMIAPICOM_Wstrlen(default_pin_code));

    WatchCOM_Btn_SetPos_2Btn( okBtn, cancelBtn );
    GUIBUTTON_SetTextId( okBtn, TXT_COMMON_OK );
    GUIBUTTON_SetTextAlign( okBtn, ALIGN_HVMIDDLE );
    GUIBUTTON_SetFont( okBtn, &tempFont );
    GUIBUTTON_SetFontColor( okBtn, WATCH_COMMONWIN_FONT_COLOR );
    reCode = GUIBUTTON_SetCallBackFunc( okBtn, BT_PininputBtnOKCb );
    if( reCode != TRUE )
    {
        TRACE_APP_BT("GUIBUTTON_SetCallBackFunc okBtn set error");
    }
    GUIBUTTON_SetTextId( cancelBtn, STXT_RETURN );
    GUIBUTTON_SetTextAlign( cancelBtn, ALIGN_HVMIDDLE );
    GUIBUTTON_SetFont( cancelBtn, &tempFont );
    GUIBUTTON_SetFontColor( cancelBtn, WATCH_COMMONWIN_FONT_COLOR );
    reCode = GUIBUTTON_SetCallBackFunc( cancelBtn, BT_PininputBtnCancelCb );
    if( reCode != TRUE )
    {
        TRACE_APP_BT("GUIBUTTON_SetCallBackFunc cancelBtn set error");
    }
#ifdef ADULT_WATCH_SUPPORT
    GUIEDIT_SetStyle( editCtrlId, GUIEDIT_STYLE_SINGLE );
    CTRLBASEEDIT_SetAlign( editCtrlId,ALIGN_HVMIDDLE );
    CTRLBASEEDIT_SetMargin( editCtrlId,PIN_EDIT_MARGIN_LEFTRIGHT,PIN_EDIT_MARGIN_TOPBOTTOM );
#endif
    MMK_SetAtvCtrl( win_id, editCtrlId );
}

LOCAL void BT_Pininput_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    //bg color
    WATCHCOM_DisplayBackground( win_id );
}

LOCAL MMI_RESULT_E  HandleBTPininputWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    TRACE_APP_BT("enter msg id = %x",msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            BT_Pininput_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            BT_Pininput_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            BT_Pininput_APP_OK( win_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            BT_Pininput_APP_CANCEL( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

WINDOW_TABLE( ADULTWATCH_BT_PININPUT_WIN_TAB ) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleBTPininputWindow ),
    WIN_ID( ADULTWATCH_BT_PAIR_PININPUT_WIN_ID ),
    CREATE_EDIT_DIGITAL_CTRL(UAL_BT_DEV_SSP_NUM_MAX_LEN , MMIBT_ADULTWACHT_PIN_EDIT_ID ),
    CREATE_BUTTON_CTRL( IMAGE_NULL, MMIBT_ADULTWACHT_PIN_CNF_BTN_ID ),
    CREATE_BUTTON_CTRL( IMAGE_NULL, MMIBT_ADULTWACHT_PIN_CANCEL_BTN_ID ),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY( TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

/****************************************************************************/
//  Description : enter BT pininput window
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:
/****************************************************************************/
PUBLIC void AdultWatchBT_Open_Pininput_Win( ual_bt_dev_info_t dev_info)
{
    SCI_MEMSET(&g_inpupin_dev_info, 0, sizeof(ual_bt_dev_info_t));
    SCI_MEMCPY(&g_inpupin_dev_info, &dev_info,sizeof(ual_bt_dev_info_t));

    MMK_CreateWin( (uint32*)ADULTWATCH_BT_PININPUT_WIN_TAB, PNULL );

}


