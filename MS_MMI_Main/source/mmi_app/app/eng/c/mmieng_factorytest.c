/*****************************************************************************
** File Name:   mmieng_factorytest.c                                         *
** Author:      chunjuan.liang                                               *
** Date:        2020.09                                                      *
** Copyright:                                                                *
** Description: 工厂测试                                                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE              NAME                    DESCRIPTION                   *
**   2020.09           chunjuan.liang          Create                        *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#ifndef WIN32
#ifdef FT_WIFI_SUPPORT_STANDARD
#include "mmieng_export.h"
#include "mmieng_text.h"
#include "mmieng_id.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmidisplay_color.h"

#include "mmi_appmsg.h"
#include "mmieng_text.h"
#include "mmieng_image.h"
#include "mmieng_id.h"
#include "mmieng_menutable.h"

#include "mmieng_nv.h"
#include "mmicom_trace.h"
#include "ctrlform_export.h"
#include "ctrllabel_export.h"
#include "guidropdownlist.h"
#include "guiform.h"
#include "guilabel.h"
#include "mmicom_trace.h"
#include "caliparam.h"
#include "chnl_wifi.h"
#include "chnl_sock.h"//Bug 1635670
#include "mmieng_win.h"


/**--------------------------------------------------------------------------*
**                macro declaration                                          *
**-------------------------------------------------------------------------- */

#define CONNECT_FIXAP_BASE      (0)
#define CONNECT_FIXAP_SIZE      (sizeof(char))
#define SSID_KEY_BASE           (CONNECT_FIXAP_BASE+CONNECT_FIXAP_SIZE)
#define SSID_KEY_SIZE           (128)

#define TESTMODE_BASE           (0)

#define SSID_MAX_LEN            (32)
#define PSK_MAX_LEN             (64)
#define IP_ADDR_MAX_LEN         (64)
#define SN_MAX_LEN              (SP09_MAX_SN_LEN)
#define QR_CODE_MAX_LEN         (100)

#define MODE_NUM_MAX            (20)
#define WIFI_STATUS_CONNECTED   (2)

#define RESULT_SUCCESS          (0)
#define RESULT_FAIL             (-1)
#define RESULT_READ_FAIL        (-3)
#define RESULT_PARAM_ERROR      (-2)
#define FIXAP_WIFI              (1)
#define FIXAP_USB               (0)




typedef struct
{
    uint32  cmd;              // 1: save ssid into DUT
    uint32 ssid_len;         // ssid length, max. characters is 32
    char ssid[SSID_MAX_LEN]; // ssid name
    uint32 psk_len;         // ssid password length
    char psk[PSK_MAX_LEN]; // password
}MMIENG_SSID_INFO_T;

typedef struct
{
    uint32  cmd;    // 1: save ssid into DUT   2: setup connect mode
    uint32  mode;   // 0: USB,  1: fix AP by WIFI
}MMIENG_CONN_MODE_T;

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

LOCAL uint8 s_connet_timer_id = 0;


/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

LOCAL MMI_RESULT_E ENGFactorySetModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGFactorySetWIFIParamWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGFactorySocketConnectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL BOOLEAN ENGFactory_GetSN(char *sn,uint32 lenth);

LOCAL int32 ENGFactory_GetConnFixApFlag(void);


WINDOW_TABLE(MMIENG_MMI_SETBOOTMODE_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGFactorySetModeWinHandleMsg),
    WIN_ID(MMIENG_MMI_SETBOOTMODE_WIN_ID),
    WIN_TITLE(TXT_ENG_SETMODE_TEST),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID),
    //set mode
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIENG_FACTORY_FORM1_CTRL_ID,MMIENG_FORM_CTRL_ID),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL1_ID, MMIENG_FACTORY_FORM1_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIENG_DROPDOWN_SET_MODE_ID, MMIENG_FACTORY_FORM1_CTRL_ID),
    //button
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIENG_FACTORY_FORM2_CTRL_ID,MMIENG_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_NULL,MMIENG_FACTORY_BUTTON_SET_ID,MMIENG_FACTORY_FORM2_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_NULL,MMIENG_FACTORY_BUTTON_CLEAR_ID,MMIENG_FACTORY_FORM2_CTRL_ID),
    WIN_SOFTKEY(TXT_ENG_COMMON_SET,TXT_NULL,TXT_ENG_CLEAR),
    END_WIN
};

WINDOW_TABLE(MMIENG_MMI_SETWIFIPARAM_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGFactorySetWIFIParamWinHandleMsg),
    WIN_ID(MMIENG_MMI_SETWIFIPARAM_WIN_ID),
    WIN_TITLE(TXT_ENG_SETWIFIPARAM_TEST),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID),
      //SSID
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIENG_FACTORY_FORM1_CTRL_ID,MMIENG_FORM_CTRL_ID),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL1_ID, MMIENG_FACTORY_FORM1_CTRL_ID ),
        CHILD_EDIT_TEXT_CTRL(TRUE, 20,MMIENG_COMMON_EDIT_CTRL1_ID, MMIENG_FACTORY_FORM1_CTRL_ID ),
      //PWD
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIENG_FACTORY_FORM2_CTRL_ID,MMIENG_FORM_CTRL_ID),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL2_ID, MMIENG_FACTORY_FORM2_CTRL_ID ),
        CHILD_EDIT_TEXT_CTRL(TRUE, 20,MMIENG_COMMON_EDIT_CTRL2_ID, MMIENG_FACTORY_FORM2_CTRL_ID ),
      //Fix ip
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIENG_FACTORY_FORM3_CTRL_ID,MMIENG_FORM_CTRL_ID),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL3_ID, MMIENG_FACTORY_FORM3_CTRL_ID ),
        CHILD_EDIT_DIGITAL_CTRL(TRUE, 20,MMIENG_COMMON_EDIT_CTRL3_ID, MMIENG_FACTORY_FORM3_CTRL_ID ),
      //Button
      CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIENG_FACTORY_FORM4_CTRL_ID,MMIENG_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_NULL,MMIENG_FACTORY_BUTTON_SET_ID,MMIENG_FACTORY_FORM4_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_NULL,MMIENG_FACTORY_BUTTON_CLEAR_ID,MMIENG_FACTORY_FORM4_CTRL_ID),
    WIN_SOFTKEY(TXT_ENG_COMMON_SET, TXT_NULL, TXT_ENG_CLEAR),
    END_WIN
};

WINDOW_TABLE(MMIENG_SOCKETCONNET_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGFactorySocketConnectWinHandleMsg),
    WIN_ID(MMIENG_MMI_SOCKETCONNET_WIN_ID),
    WIN_TITLE(TXT_ENG_SOCKETCONN_TEST),
    CREATE_BUTTON_CTRL(IMAGE_NULL,MMIENG_FACTORY_BUTTON_CLEAR_ID),
    END_WIN
};

LOCAL int32 ENGFactory_SetBootMode(uint16 idx)
{
    int32 ret = 0;
    uint32 val = 0;
    val = (uint32)((uint32)idx + 0x53464d00);
    if (CaliParam_Open()){
        if (!CaliParam_WriteOffset(PARAM_POWERON_MODE, TESTMODE_BASE, sizeof(val), (const uint8*)(&val)))
        {
            ret = -3;
            TRACE_APP_ENGINEERMODE("write offset fail");
        }else
        {
            ret = 0;
            TRACE_APP_ENGINEERMODE("write offset ok.");
        }
        CaliParam_Close();
    }
    return ret;
}

LOCAL int32 ENGFactory_GetBootMode(uint16* idx)
{
    int32 ret = RESULT_SUCCESS;
    uint32 val = 0;
    //val = (uint32)idx + 0x53464d00;
    if (CaliParam_Open()){
        if (!CaliParam_ReadOffset(PARAM_POWERON_MODE, TESTMODE_BASE, sizeof(val), (uint8*)(&val)))
        {
            ret = RESULT_READ_FAIL;
            TRACE_APP_ENGINEERMODE("read offset fail");
        }else{
            TRACE_APP_ENGINEERMODE("read offset ok,val = %d",val);
            ret = RESULT_SUCCESS;
            *idx = (uint16)(val - 0x53464d00);
            TRACE_APP_ENGINEERMODE("read offset ok.*idx = %d",*idx);
        }
        CaliParam_Close();
    }
    return ret;
}

LOCAL int32 ENGFactory_SetConnFixApFlag(MMIENG_CONN_MODE_T *fix_ap_flag)
{
    int32 ret = RESULT_SUCCESS;
    char  ap_val = FALSE;
    if(PNULL == fix_ap_flag)
    {
        TRACE_APP_ENGINEERMODE("fix_ap_flag is null");
        return RESULT_PARAM_ERROR;
    }
    TRACE_APP_ENGINEERMODE("fix_ap_flag->mode=%d",fix_ap_flag->mode);
    ap_val = (char)fix_ap_flag->mode;
    if (CaliParam_Open())
    {
        if (CaliParam_WriteOffset(PARAM_WIFI_CONNECT, CONNECT_FIXAP_BASE, sizeof(ap_val), &ap_val))
        {
            ret = RESULT_SUCCESS;
        }else
        {
            TRACE_APP_ENGINEERMODE("write caliparam fail");
            ret = RESULT_FAIL;
        }

           CaliParam_Close();
   }else
   {
        TRACE_APP_ENGINEERMODE("open caliparam fail");
        ret = RESULT_FAIL;
   }
    return ret;
}

LOCAL int32 ENGFactory_GetConnFixApFlag(void)
{
    int32 ret = FIXAP_USB;
    char mode = 0;

    if (CaliParam_Open())
    {
        if (CaliParam_ReadOffset(PARAM_WIFI_CONNECT, CONNECT_FIXAP_BASE, sizeof(char), &mode))
        {
            TRACE_APP_ENGINEERMODE("mode = %d",mode);
            if(1 == mode )
                ret = FIXAP_WIFI;

        }else
        {
            TRACE_APP_ENGINEERMODE("read fail");
        }
       CaliParam_Close();
   }else
   {
        TRACE_APP_ENGINEERMODE("open caliparam fail");
   }

    return ret;
}

LOCAL MMI_RESULT_E ENGFactorySetModeBTNCB(void)
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    uint16 index = 0;
    int32  set_result = 0;

    index = GUIDROPDOWNLIST_GetCurItemIndex(MMIENG_DROPDOWN_SET_MODE_ID);
    TRACE_APP_ENGINEERMODE("index = %d",index);
    if((9 == index) || (10 == index))
    {
        //do nothing
        TRACE_APP_ENGINEERMODE("index = %d,do nothing",index);
    }else
    {
        set_result = ENGFactory_SetBootMode(index);
    }

    TRACE_APP_ENGINEERMODE("set_result = %d",set_result);

    if(0 == set_result)
    {
        ret = MMI_RESULT_TRUE;
    }
    return ret;
}

LOCAL MMI_RESULT_E ENGFactorySetModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret_value      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id       = MMIENG_DROPDOWN_SET_MODE_ID;

    GUI_BG_T         bg_btn	 = {0};
    GUI_FONT_ALL_T font_info = {0};

    bg_btn.bg_type  = GUI_BG_COLOR;
    bg_btn.color    = MMI_GRAY_WHITE_COLOR;
    font_info.font  = WATCH_DEFAULT_NORMAL_FONT;
    font_info.color = MMI_BLACK_COLOR;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_TEXT_ID_T MODE_TEXT_INFO[MODE_NUM_MAX] = {TXT_ENG_NORMAL_TEST,TXT_ENG_GSMCALI_TEST,TXT_ENG_GSMFT_TEST,TXT_ENG_WCDMACAL_TEST,////4
                                               TXT_ENG_WCDMAFT_TEST,TXT_ENG_TDSCAL_TEST,TXT_ENG_TDSFT_TEST,TXT_ENG_LTECAL_TEST,TXT_ENG_LTEFT_TEST,////9
                                               TXT_ENG_NRCAL_TEST,TXT_ENG_NRFT_TEST,TXT_ENG_NRMMWCAL_TEST,TXT_ENG_NRMMWFT_TEST,TXT_ENG_C2KCAL_TEST,////14
                                               TXT_ENG_C2KFT_TEST,TXT_ENG_BBAT_TEST,TXT_ENG_NATIVEMMI_TEST,TXT_ENG_APKMMI_TEST,TXT_ENG_NBIOTCAL_TEST,TXT_ENG_UPT_TEST////20
                                               };
            char        *str  = PNULL;
            wchar wstr[20]    = {0};
            MMI_STRING_T text = {0};
            uint8        i    = 0;
            uint16        idx = 0;

            MMI_HANDLE_T form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIENG_FORM_CTRL_ID);
            MMI_HANDLE_T dropdown_handle = MMK_GetCtrlHandleByWin(win_id, MMIENG_DROPDOWN_SET_MODE_ID);
			GUIFORM_CHILD_HEIGHT_T height_info = {0};
            GUIFORM_CHILD_HEIGHT_T form_height = {0};
            GUIFORM_CHILD_WIDTH_T  form_width  = {0};

            GUIFORM_SetStyle(MMIENG_FACTORY_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);

            

            height_info.type     = GUIFORM_CHILD_HEIGHT_FIXED;
            height_info.add_data = MMIENG_MENU_ITEM_HEIGHT;

            form_height.type     = GUIFORM_CHILD_HEIGHT_PERCENT;
            form_height.add_data = 100;

            form_width.type      = GUIFORM_CHILD_WIDTH_PERCENT;
            form_width.add_data  = 50;


            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FACTORY_FORM1_CTRL_ID,&height_info);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM1_CTRL_ID,MMIENG_COMMON_LABEL_CTRL1_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM1_CTRL_ID,MMIENG_DROPDOWN_SET_MODE_ID,&form_height);

            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FACTORY_FORM2_CTRL_ID,&height_info);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM2_CTRL_ID,MMIENG_FACTORY_BUTTON_SET_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM2_CTRL_ID,MMIENG_FACTORY_BUTTON_CLEAR_ID,&form_height);

            GUIFORM_SetChildWidth(form_handle,MMK_GetCtrlHandleByWin(win_id,MMIENG_DROPDOWN_SET_MODE_ID),&form_width);
            GUIFORM_SetChildWidth(form_handle,MMK_GetCtrlHandleByWin(win_id,MMIENG_FACTORY_BUTTON_SET_ID),&form_width);

            str = "Boot Mode";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 14, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL1_ID, &text, TRUE);

            GUIFORM_SetChildHeight(form_handle, dropdown_handle, &height_info);
            GUIDROPDOWNLIST_SetVisible(dropdown_handle,TRUE,TRUE);
            GUIDROPDOWNLIST_SetMaxItem(dropdown_handle,MODE_NUM_MAX);
            for(i=0;i<ARR_SIZE(MODE_TEXT_INFO);i++)
            {
                GUIDROPDOWNLIST_AppendItemById(MMIENG_DROPDOWN_SET_MODE_ID,MODE_TEXT_INFO[i]);
            }
            ENGFactory_GetBootMode(&idx);
            TRACE_APP_ENGINEERMODE("idx = %d",idx);
            GUIDROPDOWNLIST_SetCurItemIndex(MMIENG_DROPDOWN_SET_MODE_ID,idx);

            GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg_btn);
            GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);
            GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_SET_ID,&bg_btn);
            GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_SET_ID,&font_info);

            GUIBUTTON_SetTextId(MMIENG_FACTORY_BUTTON_SET_ID,TXT_ENG_COMMON_SET);
            GUIBUTTON_SetTextId(MMIENG_FACTORY_BUTTON_CLEAR_ID,TXT_ENG_CLEAR);

            GUIBUTTON_SetCallBackFunc(MMIENG_FACTORY_BUTTON_SET_ID,ENGFactorySetModeBTNCB);

            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
            break;
        case MSG_FULL_PAINT:
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_APP_OK:
        case MSG_CTL_OK:
            ENGFactorySetModeBTNCB();//bug1533267
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_MENU:
        case MSG_APP_WEB:
            MMK_CloseWin(win_id);
            break;
        case MSG_NOTIFY_GET_ACTIVE:
        {

            if(MMIENG_FACTORY_BUTTON_SET_ID == MMK_GetActiveCtrlId(win_id))
            {
                GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);

                bg_btn.color    = MMI_HIGH_LIGHT_COLOR;
                font_info.color = MMI_DARK_BLUE_COLOR;

                GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_SET_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_SET_ID,&font_info);
    
            }else if(MMIENG_FACTORY_BUTTON_CLEAR_ID == MMK_GetActiveCtrlId(win_id))
            {
                CTRLBUTTON_SetBg(MMIENG_FACTORY_BUTTON_SET_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_SET_ID,&font_info);

                bg_btn.color    = MMI_HIGH_LIGHT_COLOR;
                font_info.color = MMI_DARK_BLUE_COLOR;

                CTRLBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);
            }

            MMK_PostMsg(MMIENG_FACTORY_BUTTON_SET_ID, MSG_CTL_PAINT, NULL, NULL);
            MMK_PostMsg(MMIENG_FACTORY_BUTTON_CLEAR_ID, MSG_CTL_PAINT, NULL, NULL);
        }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            ret_value = MMI_RESULT_FALSE;
            break;
    }
    return ret_value;
}
LOCAL int ENGFactory_GetAPParam(MMIENG_SSID_INFO_T *ap_param)
{
    uint32      len = sizeof(MMIENG_SSID_INFO_T)-sizeof(ap_param->cmd);
    int         ret = RESULT_SUCCESS;

    if(PNULL == ap_param)
    {
        TRACE_APP_ENGINEERMODE("ap_param is null");
        return RESULT_PARAM_ERROR;
    }

    if (CaliParam_Open())
    {
        if (CaliParam_ReadOffset(PARAM_WIFI_CONNECT, SSID_KEY_BASE, len, (char*)(&(ap_param->ssid_len))))
        {
            TRACE_APP_ENGINEERMODE("read success");
            ret = RESULT_SUCCESS;
        }else
        {
            TRACE_APP_ENGINEERMODE("read fail");
            ret = RESULT_FAIL;
        }

        CaliParam_Close();
    }else
    {
        TRACE_APP_ENGINEERMODE("open fail");
    }
    return ret;
}

LOCAL int ENGFactory_SetAPParam(MMIENG_SSID_INFO_T *ap_param)
{
    uint32      len = sizeof(MMIENG_SSID_INFO_T)-sizeof(ap_param->cmd);
    int         ret = RESULT_SUCCESS;

    if(PNULL == ap_param)
    {
        TRACE_APP_ENGINEERMODE("ap_param is null");
        return RESULT_PARAM_ERROR;
    }

    if (CaliParam_Open())
    {
        if (CaliParam_WriteOffset(PARAM_WIFI_CONNECT, SSID_KEY_BASE, len, (char*)(&(ap_param->ssid_len))))
        {
            TRACE_APP_ENGINEERMODE("set success");
            ret = RESULT_SUCCESS;
        }else
        {
            TRACE_APP_ENGINEERMODE("set fail");
            ret = RESULT_FAIL;
        }

        CaliParam_Close();
    }else
    {
        TRACE_APP_ENGINEERMODE("open fail");
    }

    return ret;

}

LOCAL MMI_RESULT_E FactorySetWifiParamBTNCB(void)
{
    MMI_STRING_T ssid    = {0};
    MMI_STRING_T psk     = {0};
    MMI_STRING_T fixap   = {0};
    MMIENG_SSID_INFO_T ap_param = {0};

    uint8       str_ssid[SSID_MAX_LEN] = {0};
    uint8       str_psk[PSK_MAX_LEN]  = {0};
    uint8       str_fixap[2] = {0};
    uint32      ssid_len = 0;
    uint32      psk_len  = 0;
    uint32      fixap_val = 0;
    MMIENG_CONN_MODE_T   fixap_param = {0};


    TRACE_APP_ENGINEERMODE("MMIENG_FACTORY_BUTTON_SET_ID");

    GUIEDIT_GetString(MMIENG_COMMON_EDIT_CTRL1_ID,&ssid);
    if ((PNULL != ssid.wstr_ptr) && (0 != ssid.wstr_len))
    {
        MMI_WSTRNTOSTR(str_ssid,ssid.wstr_len,ssid.wstr_ptr,ssid.wstr_len,ssid.wstr_len);
        ssid_len = strlen(str_ssid);
        TRACE_APP_ENGINEERMODE("str_ssid=%s,ssid_len=%d",str_ssid,ssid_len);
        ap_param.ssid_len = MIN(ssid_len,SSID_MAX_LEN);
        SCI_MEMCPY(ap_param.ssid,str_ssid,ap_param.ssid_len);
    }

    GUIEDIT_GetString(MMIENG_COMMON_EDIT_CTRL2_ID,&psk);
    if((PNULL != psk.wstr_ptr) && (0 != psk.wstr_len))
    {
        MMI_WSTRNTOSTR(str_psk,psk.wstr_len,psk.wstr_ptr,psk.wstr_len,psk.wstr_len);
        psk_len = strlen(str_psk);
        TRACE_APP_ENGINEERMODE("str_psk=%s,psk_len=%d",str_psk,psk_len);
        ap_param.psk_len = MIN(psk_len,PSK_MAX_LEN);
        SCI_MEMCPY(ap_param.psk,str_psk,ap_param.psk_len);
    }
    TRACE_APP_ENGINEERMODE("ssid_len=%d,psk_len=%d",ssid_len,psk_len);
    ENGFactory_SetAPParam(&ap_param);

    GUIEDIT_GetString(MMIENG_COMMON_EDIT_CTRL3_ID,&fixap);
    TRACE_APP_ENGINEERMODE("fixap.wstr_len=%d",fixap.wstr_len);
    if((PNULL != fixap.wstr_ptr) && (0 != fixap.wstr_len))
    {
        MMI_WSTRNTOSTR(str_fixap,fixap.wstr_len,fixap.wstr_ptr,fixap.wstr_len,fixap.wstr_len);
        TRACE_APP_ENGINEERMODE("str_fixap=%c",str_fixap);
        MMIAPICON_ConvertStringtoNumber(str_fixap,strlen(str_fixap),&fixap_val);
        fixap_param.mode = fixap_val;
        TRACE_APP_ENGINEERMODE("fixap_val=%d",fixap_val);
    }
    ENGFactory_SetConnFixApFlag(&fixap_param);
}

LOCAL MMI_RESULT_E FactoryClearWifiParamBTNCB(void)
{
    TRACE_APP_ENGINEERMODE("MMIENG_FACTORY_BUTTON_CLEAR_ID");
    GUIEDIT_ClearAllStrEx(MMIENG_COMMON_EDIT_CTRL1_ID,TRUE);
    GUIEDIT_ClearAllStrEx(MMIENG_COMMON_EDIT_CTRL2_ID,TRUE);
    GUIEDIT_ClearAllStrEx(MMIENG_COMMON_EDIT_CTRL3_ID,TRUE);

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E ENGFactorySetWIFIParamWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret_value      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id       = MMIENG_COMMON_EDIT_CTRL1_ID;
    GUI_FONT_ALL_T   font_info = {0};
    GUI_BG_T            bg_btn = {0};

    bg_btn.bg_type  = GUI_BG_COLOR;
    bg_btn.color    = MMI_GRAY_WHITE_COLOR;
    font_info.font  = WATCH_DEFAULT_NORMAL_FONT;
    font_info.color = MMI_BLACK_COLOR;

    TRACE_APP_ENGINEERMODE("enter");

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            char        *str  = PNULL;
            char         str_ap[2]={0};
            wchar wstr[64]    = {0};
            MMI_STRING_T text = {0};
            uint8        i    = 0;

            MMIENG_SSID_INFO_T    ap_param = {0};
            int32         conn_flag = 0;
			GUIFORM_CHILD_HEIGHT_T height_info = {0};
            GUIFORM_CHILD_HEIGHT_T form_height = {0};
            GUIFORM_CHILD_WIDTH_T  form_width  = {0};

            MMI_HANDLE_T form1_handle    = MMK_GetCtrlHandleByWin(win_id, MMIENG_FACTORY_FORM1_CTRL_ID);
            MMI_HANDLE_T form2_handle    = MMK_GetCtrlHandleByWin(win_id, MMIENG_FACTORY_FORM2_CTRL_ID);
            MMI_HANDLE_T form3_handle    = MMK_GetCtrlHandleByWin(win_id, MMIENG_FACTORY_FORM3_CTRL_ID);
            MMI_HANDLE_T form4_handle    = MMK_GetCtrlHandleByWin(win_id, MMIENG_FACTORY_FORM4_CTRL_ID);

            //form set style
            GUIFORM_SetStyle(MMIENG_FACTORY_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_FACTORY_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_FACTORY_FORM3_CTRL_ID,GUIFORM_STYLE_UNIT);

            

            height_info.type     = GUIFORM_CHILD_HEIGHT_FIXED;
            height_info.add_data = MMIENG_MENU_ITEM_HEIGHT;

            form_height.type     = GUIFORM_CHILD_HEIGHT_PERCENT;
            form_height.add_data = 100;

            form_width.type      = GUIFORM_CHILD_WIDTH_PERCENT;
            form_width.add_data  = 50;

            //form set child height
            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FACTORY_FORM1_CTRL_ID,&height_info);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM1_CTRL_ID,MMIENG_COMMON_LABEL_CTRL1_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM1_CTRL_ID,MMIENG_COMMON_EDIT_CTRL1_ID,&form_height);

            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FACTORY_FORM2_CTRL_ID,&height_info);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM2_CTRL_ID,MMIENG_COMMON_LABEL_CTRL2_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM2_CTRL_ID,MMIENG_COMMON_EDIT_CTRL2_ID,&form_height);

            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FACTORY_FORM3_CTRL_ID,&height_info);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM3_CTRL_ID,MMIENG_COMMON_LABEL_CTRL3_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM3_CTRL_ID,MMIENG_COMMON_EDIT_CTRL3_ID,&form_height);

            GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_FACTORY_FORM4_CTRL_ID,&height_info);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM4_CTRL_ID,MMIENG_FACTORY_BUTTON_CLEAR_ID,&form_height);
            GUIFORM_SetChildHeight(MMIENG_FACTORY_FORM4_CTRL_ID,MMIENG_FACTORY_BUTTON_SET_ID,&form_height);

            //form set width
            GUIFORM_SetChildWidth(form1_handle,MMK_GetCtrlHandleByWin(win_id, MMIENG_COMMON_LABEL_CTRL1_ID),&form_width);
            GUIFORM_SetChildWidth(form2_handle,MMK_GetCtrlHandleByWin(win_id, MMIENG_COMMON_LABEL_CTRL2_ID),&form_width);
            GUIFORM_SetChildWidth(form3_handle,MMK_GetCtrlHandleByWin(win_id, MMIENG_COMMON_LABEL_CTRL3_ID),&form_width);
            GUIFORM_SetChildWidth(form1_handle,MMK_GetCtrlHandleByWin(win_id, MMIENG_COMMON_LABEL_CTRL4_ID),&form_width);

            ENGFactory_GetAPParam(&ap_param);
            conn_flag = ENGFactory_GetConnFixApFlag();


            str = "AP SSID:";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 64, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL1_ID, &text, TRUE);
            GUIEDIT_SetStyle(MMIENG_COMMON_EDIT_CTRL1_ID, GUIEDIT_STYLE_SINGLE);

            SCI_MEMSET(wstr,0,64*sizeof(wchar));
            TRACE_APP_ENGINEERMODE("ssid=%s,ap_param.ssid_len = %d",ap_param.ssid,ap_param.ssid_len);
            MMI_STRNTOWSTR( wstr, 64, ap_param.ssid, 32, ap_param.ssid_len);
            GUIEDIT_SetString(MMIENG_COMMON_EDIT_CTRL1_ID,wstr,MMIAPICOM_Wstrlen(wstr));

            str = "Password:";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 64, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL2_ID, &text, TRUE);
            GUIEDIT_SetStyle(MMIENG_COMMON_EDIT_CTRL2_ID, GUIEDIT_STYLE_SINGLE);
            TRACE_APP_ENGINEERMODE("psk = %s,ap_param.psk_len = %d",ap_param.psk,ap_param.psk_len);
            SCI_MEMSET(wstr,0,64*sizeof(wchar));
            MMI_STRNTOWSTR( wstr, 64, ap_param.psk, 64, ap_param.psk_len);
            GUIEDIT_SetString(MMIENG_COMMON_EDIT_CTRL2_ID,wstr,MMIAPICOM_Wstrlen(wstr));

            str = "Conn. fix ip:";
            text.wstr_len = strlen(str);
            text.wstr_ptr = wstr;
            MMI_STRNTOWSTR( wstr, 15, (uint8*)str, text.wstr_len, text.wstr_len );
            GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL3_ID, &text, TRUE);
            GUIEDIT_SetStyle(MMIENG_COMMON_EDIT_CTRL3_ID, GUIEDIT_STYLE_SINGLE);
#ifdef ADULT_WATCH_SUPPORT
            GUIEDIT_SetIm(MMIENG_COMMON_EDIT_CTRL3_ID,GUIIM_TYPE_SYS,GUIIM_TYPE_SYS);
#endif
            SCI_MEMSET(wstr,0,64*sizeof(wchar));
            TRACE_APP_ENGINEERMODE("conn_flag=%d",conn_flag);
            MMIAPICOM_Int2Str((uint32)conn_flag,str_ap,2);
            MMI_STRNTOWSTR( wstr, 64, str_ap, 2, 2);
            GUIEDIT_SetString(MMIENG_COMMON_EDIT_CTRL3_ID,wstr,MMIAPICOM_Wstrlen(wstr));

            bg_btn.bg_type = GUI_BG_COLOR;
            bg_btn.color   = MMI_GRAY_WHITE_COLOR;
            font_info.font  = WATCH_DEFAULT_NORMAL_FONT;
            font_info.color = MMI_BLACK_COLOR;
            GUIBUTTON_SetTextId(MMIENG_FACTORY_BUTTON_SET_ID,TXT_ENG_COMMON_SET);
            GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_SET_ID,&bg_btn);
            GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_SET_ID,&font_info);

            GUIBUTTON_SetTextId(MMIENG_FACTORY_BUTTON_CLEAR_ID,TXT_ENG_CLEAR);
            GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg_btn);
            GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);

            GUIBUTTON_SetCallBackFunc(MMIENG_FACTORY_BUTTON_SET_ID,FactorySetWifiParamBTNCB);
            GUIBUTTON_SetCallBackFunc(MMIENG_FACTORY_BUTTON_CLEAR_ID,FactoryClearWifiParamBTNCB);

            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
            break;
        case MSG_FULL_PAINT:
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_MENU:
        case MSG_APP_WEB:
            MMK_CloseWin(win_id);
            break;
        case MSG_NOTIFY_GET_ACTIVE:
        {
            if(MMIENG_FACTORY_BUTTON_SET_ID == MMK_GetActiveCtrlId(win_id))
            {
                CTRLBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);

                bg_btn.color    = MMI_HIGH_LIGHT_COLOR;
                font_info.color = MMI_DARK_BLUE_COLOR;

                CTRLBUTTON_SetBg(MMIENG_FACTORY_BUTTON_SET_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_SET_ID,&font_info);

            }else if(MMIENG_FACTORY_BUTTON_CLEAR_ID == MMK_GetActiveCtrlId(win_id))
            {
                CTRLBUTTON_SetBg(MMIENG_FACTORY_BUTTON_SET_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_SET_ID,&font_info);

                bg_btn.color    = MMI_HIGH_LIGHT_COLOR;
                font_info.color = MMI_DARK_BLUE_COLOR;

                CTRLBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);
            }

            MMK_PostMsg(MMIENG_FACTORY_BUTTON_SET_ID, MSG_CTL_PAINT, NULL, NULL);
            MMK_PostMsg(MMIENG_FACTORY_BUTTON_CLEAR_ID, MSG_CTL_PAINT, NULL, NULL);
        }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            ret_value = MMI_RESULT_FALSE;
            break;
    }
    return ret_value;
}
LOCAL BOOLEAN ENGFactory_GetIPAddr(char *ip_ptr,uint32 buf_len)
{
    unsigned long ip_u32 = 0;

    ip_u32 = (unsigned long)sci_gethostaddress();
    {
        typedef union
        {
            uint32 whole;
            struct
            {
                uint32 byte0 : 8;
                uint32 byte1 : 8;
                uint32 byte2 : 8;
                uint32 byte3 : 8;
            }all_part;
        }DEPART_T;

        DEPART_T depart = {0};
        depart.whole = ip_u32;

        _snprintf(
            ip_ptr,
            buf_len,
            "%d.%d.%d.%d",
            depart.all_part.byte0,
            depart.all_part.byte1,
            depart.all_part.byte2,
            depart.all_part.byte3
            );
    }
    return TRUE;

}

LOCAL MMI_RESULT_E FactoryConnWifiBTNCB(void)
{
    char ipAddr[IP_ADDR_MAX_LEN] = {0};
    char info[QR_CODE_MAX_LEN]   = {0};
    char sn[SN_MAX_LEN]          = {0};
    uint32  ip_len               = sizeof(ipAddr);

    if(WIFI_STATUS_CONNECTED == chnl_wifi_isConnect())
    {
        TRACE_APP_ENGINEERMODE("CONNECTED");

        ENGFactory_GetIPAddr(ipAddr,ip_len);
        ENGFactory_GetSN(sn,SN_MAX_LEN);
        sprintf(info,"%s-%s-10000",sn,ipAddr);
        TRACE_APP_ENGINEERMODE("info len=%d",strlen(info));
        WatchQRCode_MainWin_Enter(info,strlen(info));
    }else
    {
        TRACE_APP_ENGINEERMODE("Disconnect");
        s_connet_timer_id = MMK_CreateTimer(1000,TRUE);
    }

}
LOCAL BOOLEAN ENGFactory_GetSN(char *sn,uint32 lenth)
{
    SP09_PHASE_CHECK_T  eng_phrase = {0};
    uint8 sn_len = 0;
    if(PNULL == sn)
    {
        TRACE_APP_ENGINEERMODE("sn is null");
        return FALSE;
    }
    if(0 == EFS_ReadPhaseCheck(&eng_phrase,SP09_MAX_PHASE_BUFF_SIZE))
    {
        TRACE_APP_ENGINEERMODE("read phrase check is OK");
    }
    sn_len = SCI_STRLEN(eng_phrase.SN1);
    TRACE_APP_ENGINEERMODE("sn_len=%d",sn_len);
    sn_len = MIN(sn_len, lenth);
    SCI_MEMCPY(sn,eng_phrase.SN1,sn_len);

    return TRUE;
}
LOCAL MMI_RESULT_E ENGFactorySocketConnectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E ret_value      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id       = MMIENG_FACTORY_BUTTON_CLEAR_ID;

    char ipAddr[IP_ADDR_MAX_LEN] = {0};
    char info[QR_CODE_MAX_LEN]   = {0};
    char sn[SN_MAX_LEN]          = {0};
    uint32  ip_len               = sizeof(ipAddr);

    GUI_BG_T        bg          = {0};
    GUI_FONT_ALL_T  font_info   = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            uint32 i = 0;

            GUIBUTTON_SetCallBackFunc(MMIENG_FACTORY_BUTTON_CLEAR_ID,FactoryConnWifiBTNCB);

            MMIDEFAULT_AllowTurnOffBackLight(FALSE);

            MMK_SetAtvCtrl(win_id, ctrl_id);

            TRACE_APP_ENGINEERMODE("chnl_wifi_isConnect() = %d",chnl_wifi_isConnect());
            //if(0 == chnl_wifi_isConnect())
            //建立socket
            {
                chnl_wifi_init(1);
                chnl_sock_init(1);// Bug 1628941,1628929
            }

            if(WIFI_STATUS_CONNECTED == chnl_wifi_isConnect()) //bug1649185
            {
                TRACE_APP_ENGINEERMODE("CONNECTED");

                ENGFactory_GetIPAddr(ipAddr,ip_len);
                ENGFactory_GetSN(sn,SN_MAX_LEN);
                sprintf(info,"%s-%s-10000",sn,ipAddr);
                TRACE_APP_ENGINEERMODE("info len=%d",strlen(info));
                WatchQRCode_MainWin_Enter(info,strlen(info));
            }else
            {
                TRACE_APP_ENGINEERMODE("Disconnect");
                s_connet_timer_id = MMK_CreateTimer(1000,TRUE);
            }
        }
            break;
        case MSG_FULL_PAINT:
#if 0
        {
			char        *str  = PNULL;
            wchar wstr[20]    = {0};
            GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
           
            GUI_RECT_T      button_rect = client_rect;
            button_rect.top = MMI_STATUSBAR_HEIGHT + 10;
            button_rect.left = 10;
            button_rect.right = client_rect.right - 10;
            button_rect.bottom = (client_rect.top + client_rect.bottom)*2/3;

            

            bg.bg_type = GUI_BG_COLOR;
            bg.color   = MMI_IS_SELECTED_COLOR;
            font_info.font  = WATCH_DEFAULT_BIG_FONT;
            font_info.color = MMI_BLACK_COLOR;

            str = "CONNECT";
            MMI_STRNTOWSTR(wstr,7,str,7,7);

            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                              IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());

            GUIBUTTON_SetRect(MMIENG_FACTORY_BUTTON_CLEAR_ID,&button_rect);

            GUIBUTTON_SetBg(MMIENG_FACTORY_BUTTON_CLEAR_ID,&bg);
            GUIBUTTON_SetFont(MMIENG_FACTORY_BUTTON_CLEAR_ID,&font_info);
            GUIBUTTON_SetText(MMIENG_FACTORY_BUTTON_CLEAR_ID,wstr,7);
        }  
#endif
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_MENU:
        case MSG_APP_WEB:
#if 0
        {
            uint32 i = 0;

            if(WIFI_STATUS_CONNECTED == chnl_wifi_isConnect())
            {
                TRACE_APP_ENGINEERMODE("CONNECTED");

                ENGFactory_GetIPAddr(ipAddr,ip_len);
                ENGFactory_GetSN(sn,SN_MAX_LEN);
                sprintf(info,"%s-%s-10000",sn,ipAddr);
                TRACE_APP_ENGINEERMODE("info len=%d",strlen(info));
                
                WatchQRCode_MainWin_Enter(info,strlen(info));
            }else
            {
                TRACE_APP_ENGINEERMODE("Disconnect");
                s_connet_timer_id = MMK_CreateTimer(1000,TRUE);
            }
        }
#endif
            break;
        case MSG_TIMER:
            if(s_connet_timer_id ==  *( GUI_TIMER_ID_T*)param)
            {
                TRACE_APP_ENGINEERMODE("TIMER,CONN_STATUS=%d",chnl_wifi_isConnect());
                if(WIFI_STATUS_CONNECTED == chnl_wifi_isConnect())
                {
                    TRACE_APP_ENGINEERMODE("CONNECTED");
                    if(0 < s_connet_timer_id)
                    {
                        MMK_StopTimer(s_connet_timer_id);
                        s_connet_timer_id = 0;
                    }
                    ENGFactory_GetIPAddr(ipAddr,ip_len);
                    ENGFactory_GetSN(sn,SN_MAX_LEN);
                    TRACE_APP_ENGINEERMODE("sn len=%d",strlen(sn));
                    sprintf(info,"%s-%s-10000",sn,ipAddr);
                    TRACE_APP_ENGINEERMODE("info len=%d",strlen(info));
                    WatchQRCode_MainWin_Enter(info,strlen(info));
                }else
                {
                    TRACE_APP_ENGINEERMODE("disconnect");
                }
            }
            break;
        case MSG_NOTIFY_GET_ACTIVE:
        {
            if(MMIENG_FACTORY_BUTTON_CLEAR_ID == MMK_GetActiveCtrlId(win_id))
            {
                bg.bg_type      = GUI_BG_COLOR;
                bg.color        = MMI_HIGH_LIGHT_COLOR;
                font_info.font  = WATCH_DEFAULT_BIG_FONT;
                font_info.color = MMI_BLACK_COLOR;
            }
            MMK_PostMsg(MMIENG_FACTORY_BUTTON_CLEAR_ID, MSG_CTL_PAINT, NULL, NULL);
        }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            if(0 != s_connet_timer_id)
            {
                MMK_StopTimer(s_connet_timer_id);
                s_connet_timer_id = 0;
            }

            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            break;
        default:
            ret_value = MMI_RESULT_FALSE;
            break;
    }
    return ret_value;
}

PUBLIC void MMIENGFactorySetMode_Enter(void)
{
    MMK_CreateWin((uint32 *)MMIENG_MMI_SETBOOTMODE_WIN_TAB,PNULL);
}

PUBLIC void MMIENGFactorySetParam_Enter(void)
{
    MMK_CreateWin((uint32 *)MMIENG_MMI_SETWIFIPARAM_WIN_TAB,PNULL);
}

PUBLIC void MMIENGFactorySocketConn_Enter(void)
{
    MMK_CreateWin((uint32 *)MMIENG_SOCKETCONNET_WIN_TAB,PNULL);
}

PUBLIC void MMIENG_QrCode_Enter(void)
{
    char ipAddr[IP_ADDR_MAX_LEN]     = {0};
    char info[QR_CODE_MAX_LEN]       = {0};
    char sn[SN_MAX_LEN]              = {0};
    uint32 ip_len                    = IP_ADDR_MAX_LEN;
    GUI_RECT_T          bgRect       = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };

    TRACE_APP_ENGINEERMODE("chnl_wifi_isConnect()=%d",chnl_wifi_isConnect());
    for(;;)
    {
        if(WIFI_STATUS_CONNECTED == chnl_wifi_isConnect())
        {
            break;
        }else
        {
            TRACE_APP_ENGINEERMODE("chnl_wifi_isConnect()=%d",chnl_wifi_isConnect());
            SCI_Sleep(1000);
        }
    }

    TRACE_APP_ENGINEERMODE("CONNECTED");
    ENGFactory_GetIPAddr(ipAddr,ip_len);
    ENGFactory_GetSN(sn,SN_MAX_LEN);
    sprintf(info,"%s-%s-10000",sn,ipAddr);
    TRACE_APP_ENGINEERMODE("info len=%d",strlen(info));
    GUI_FillRect( &lcd_dev_info, bgRect, MMI_WHITE_COLOR );
    WatchQRCode_Draw(info,strlen(info));
}

PUBLIC BOOLEAN MMIENGFacotry_IsTestMode(void )
{
    uint16 mode = 0;
    //ENGFactory_GetBootMode(&mode);
    if((1 == ENGFactory_GetConnFixApFlag()) /*&& (TRUE == POWER_IsCalibrationResetMode())*/)
    {
        TRACE_APP_ENGINEERMODE("IsTestMode is true");
        return TRUE;
    }else
    {
        TRACE_APP_ENGINEERMODE("IsTestMode is false");
        return FALSE;
    }
}
#endif

#endif
