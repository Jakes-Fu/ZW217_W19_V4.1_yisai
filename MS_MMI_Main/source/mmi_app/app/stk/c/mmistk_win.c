/*****************************************************************************
** File Name:      mmistk_win.c                                                  *
** Author:                                                                   *
** Date:           08/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2004         Allen

******************************************************************************/

#define _MMISTK_WIN_C_

/**--------------------------------------------------------------------------*
		                        Include Files      
 **--------------------------------------------------------------------------*/
#include "mmi_app_stk_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_msg.h"
#include "mmk_timer.h"
#include "mn_type.h"
#include "simat_data_object.h"
#include "simat_proactive.h"
#include "simat_api.h"
#include "sci_codec.h"
#include "mn_api.h"
#include "mmistk_export.h"
#include "mmistk_app.h"
#include "mmistk_win.h"
#include "mmk_timer.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmistk_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiidle_export.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guimenu.h"
#include "mmicc_export.h"
#include "tb_dal.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "os_api.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmi_textfun.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#include "mmipub.h"
#include "guilistbox.h"
#include "guitips.h"
#include "gpio_prod_api.h"
#include "mmimms_export.h"
#include "mmipb_export.h"
#include "mmisms_export.h"
#include "mmiussd_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_app.h"
#include "mmi_default.h"
#include "mmialarm_export.h"
#include "mmiwclk_export.h"
#include "sci_codec.h"
#include "gui_ucs2b_converter.h"
#include "guiedit.h"
#include "socket_api.h"
#include "mmiconnection_export.h"
#include "guictrl_api.h"
#include "mmisrvaud_api.h"
#include "guirichtext.h"
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#include "guilabel.h"
#endif
#include "mmicc_id.h"
#include "guires.h"

#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#include "mmiphone_export.h"
#include "watch_commonwin_export.h"
#include "mmicc_internal.h"

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMI_SHOW_TITLE_LEN							8
#define MMI_DCS_DEFAULT_ALPHABET			0x00
#define MMI_DCS_8_BIT_ALPHABET				0x04
#define MMI_DCS_UCS2						0x08

#define MMISTK_SELECTITEM_DURATION					63000
#define MMISTK_DISPLAYTEXT_USER_CLEAR_DURATION		60000
#define MMISTK_DISPLAYTEXT_DELAY_DURATION			5000
#define MMISTK_GETINPUT_DURATION					120000
#define MMISTK_GETINKEY_DURATION					63000

#define MMISTK_SOCKET_CONNECT_DURATION			800
#define MMISTK_SOCKET_RECEIVE_DURATION                  80
#define MMISTK_SOCKET_GET_DATALEN_DURATION        1000

#define MMISTK_FIXED_CALL_RING_NUM					MMISET_RING_USER_MAX_NUM
#define MMISTK_TONE_ID_SPAN							15
#define MMISTK_PLAY_TONE_NUM						18

#define MMISTK_TELE_NUM_MAX_LEN                     80

#define MMISTK_DTMF_DURATION                     3000
#define MMISTK_DTMF_FIRST_DURATION        4000
#define MMISTK_CHANNEL_MAX  (8)
#define MMISTK_RECONNECT_SOCKET_TIMES_MAX  (40)
#define MMISTK_CNF_RECEIVEDATA_MAX_LEN       237//200
#define MMISTK_GET_DATA_TIMES_MAX                60
#define MMISTK_CNF_SENDDATA_MAX_LEN       200
#define MMISTK_OPENCHANNEL_BUF_SIZE_MAX    1400

#define MMISTK_ICON_DISPLAY_HEIGHT 16

#define MMISTK_DISPLAYTEXT_IR_TIMES    2
//#define MMISTK_LANG_NOTI_LEN_MAX    2
#define MMISTK_LANG_COUNTRY_CODE_MAX    8

#define GCF_TEST 0  
// Need to set the GCF_TEST to 1 when run GCF/PTCRB test, corresponding bugzilla ID 503572   

typedef enum
{
    MMISTK_SETUP_CALL_NO_OPER,
    MMISTK_SETUP_CALL_CALL_OPER,
    MMISTK_SETUP_CALL_HOLD_OPER,
    MMISTK_SETUP_CALL_DISCON_OPER,
    MMISTK_SETUP_CALL_OPER_MAX
}MMISTK_SETUP_CALL_OPER_E;

#ifdef WIN32
MN_SIM_IMG_T s_sim_img_basic = {8, 8, MN_SIM_IMG_CODE_TYPE_BASIC, 8, 0, 0, 0, \
{
0xFF, 0xC3, 0xA5, 0x99, 0x99, 0xA5, 0xC3, 0xFF
}
};

MN_SIM_IMG_T s_sim_img_color = {8, 8, MN_SIM_IMG_CODE_TYPE_COLOR, 25, 2, 3, 16, \
{
//0xFF, 0xC3, 0xA5, 0x99, 0x99, 0xA5, 0xC3, 0xFF
0xAA, 0xAA, 0x80, 0x02, 0x85, 0x42, 0x81, 0x42,
0x81, 0x42, 0x81, 0x52, 0x80, 0x02, 0xAA, 0xAA,
0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF
}
};
#endif

#define RGB565(r, g, b) 		((uint16)(((uint16)(r & 0xF8) << 8) + ((uint16)(g & 0xFC) << 3) + ((uint16)(b & 0xF8) >> 3)))

/**--------------------------------------------------------------------------*
			                         GLOBLE DEFINITION
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
			                         STATIC DEFINITION
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_stk_refresh = FALSE;
LOCAL SIMAT_INPUT_TYPE_E s_mmistk_input_type = SIMAT_INPUT_DIGIT_ONLY;
LOCAL BOOLEAN s_mmistk_is_input_help_info = FALSE;
LOCAL BOOLEAN s_mmistk_input_is_echo = TRUE;
LOCAL BOOLEAN s_mmistk_input_is_icon_id = FALSE;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_input_icon_id = {0};
#endif
LOCAL SIMAT_DO_RESPONSE_LEN_T s_mmistk_input_length = {0, 0};
LOCAL BOOLEAN s_mmistk_input_is_pack_req = FALSE;
LOCAL BOOLEAN s_mmistk_displaytext_is_ir = FALSE;
LOCAL BOOLEAN s_mmistk_displaytext_is_icon_exist = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_displaytext_icon_id = {0};
LOCAL SIMAT_INKEY_TYPE_E s_mmistk_inkey_type = SIMAT_DIGIT_ONLY;
LOCAL BOOLEAN s_mmistk_is_inkey_help_info = FALSE;
LOCAL BOOLEAN s_mmistk_inkey_is_icon_id = FALSE;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
LOCAL BOOLEAN s_mmistk_idle_text_is_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_idle_text_icon_id = {0};
#endif
LOCAL BOOLEAN s_mmistk_ss_is_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_ss_icon_id = {0};
LOCAL BOOLEAN s_mmistk_sms_is_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_sms_icon_id = {0};
LOCAL BOOLEAN s_mmistk_ussd_is_icon_id = FALSE;
LOCAL BOOLEAN s_mmistk_dtmf_is_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_dtmf_icon_id = {0};
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
LOCAL GUIIMG_BITMAP_T s_mmistk_dtmf_icon_bmp = {0};
#endif
LOCAL BOOLEAN s_mmistk_setup_call_is_uc_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_setup_call_uc_icon_id = {0};
LOCAL BOOLEAN s_mmistk_setup_call_is_cs_icon_id = FALSE;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_setup_call_cs_icon_id = {0};
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_inkey_icon_id = {0};
#endif
LOCAL BOOLEAN s_mmistk_launch_browser_is_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_launch_browser_icon_id = {0};
LOCAL BOOLEAN s_mmistk_open_channel_is_icon_id = FALSE;
LOCAL SIMAT_DO_ICON_ID_T s_mmistk_open_channel_icon_id = {0};
LOCAL BOOLEAN s_mmistk_icon_display_result = FALSE;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
LOCAL GUIIMG_BITMAP_T s_icon_bitmap = {0};
LOCAL GUIIMG_BITMAP_T s_idle_icon_bmp = {0};
#endif

// Timer ID
LOCAL uint8 s_mmistk_displaytext_timer_id = 0;
LOCAL uint16 s_mmistk_displaytext_timer_duration = 0;
LOCAL uint8 s_mmistk_getinput_timer_id = 0;
LOCAL uint8 s_mmistk_getinkey_timer_id = 0;
LOCAL uint32 s_mmistk_getinkey_duration = 0;//bug593012
LOCAL BOOLEAN s_mmistk_getinkey_duration_exist = FALSE;//bug593012
LOCAL uint8 s_mmistk_playtone_timer_id = 0;
LOCAL uint8 s_mmistk_selectitem_timer_id = 0;
LOCAL uint8 s_mmistk_setupcall_duration_timer_id = 0;
LOCAL uint8 s_mmistk_connect_socket_timer_id = 0;
LOCAL uint8 s_mmistk_socket_get_data_timer_id = 0;

// EVENT LIST amount
LOCAL uint8 s_mmistk_eventlist_amount = 0;
// EVENT LIST
LOCAL BOOLEAN s_mmistk_eventlist[SIMAT_EVENT_REVERSED] = {FALSE};

LOCAL STK_MENU_ITEM_T *s_title_from_selected_item_context;

LOCAL SIMAT_SETUP_CALL_TYPE_E s_mmistk_setup_call_type = SIMAT_STC_FREE;
LOCAL BOOLEAN s_is_mmistk_setup_call = FALSE;
LOCAL BOOLEAN s_is_setup_call_duration = FALSE;
LOCAL BOOLEAN s_is_setup_call_cps =FALSE;
LOCAL uint32 s_mmistk_setup_call_duration = 0;
LOCAL MMISTK_SETUP_CALL_OPER_E s_mmistk_call_oper_type = MMISTK_SETUP_CALL_NO_OPER;
LOCAL SIMAT_DO_CCP_T s_mmistk_setup_call_cpp[MMI_DUAL_SYS_MAX] = {0};

LOCAL uint8 *s_mmistk_setup_call_str = PNULL;
LOCAL MN_SUBADDR_T  s_mmistk_sub_addr = {0};/*lint !e64*/
LOCAL uint16    s_mmistk_setup_call_str_len = 0;
LOCAL wchar *s_mmistk_displaybuf2 = PNULL;
LOCAL MMI_STRING_T	s_mmistk_displaydata2 = {0};
LOCAL MN_DUAL_SYS_E    s_stk_dual_sys = MMI_DUAL_SYS_MAX;

LOCAL STK_MENU_INFO_T *s_mmistk_toplevel_menu_info[MMI_DUAL_SYS_MAX] = {PNULL};

LOCAL STK_MENU_INFO_T *s_mmistk_cur_menu_info_ptr[MMI_DUAL_SYS_MAX] = {PNULL};

LOCAL BOOLEAN s_simtk_is_detected[MMI_DUAL_SYS_MAX] = {FALSE};							// SIMTK card valid flag
LOCAL wchar *s_mmistk_displaybuf = PNULL;	//The buffer for display buffer
LOCAL MMI_STRING_T s_mmistk_displaydata = {PNULL, 0};				//display date struct
LOCAL wchar *s_mmistk_inputbuf = PNULL;		//the buffer for input data
LOCAL MMI_STRING_T  s_mmistk_inputdata = {PNULL, 0};
LOCAL BOOLEAN s_mmistk_send_sms = FALSE;								// whether send SMS by STK
LOCAL BOOLEAN s_mmistk_indmenusel_cnf = FALSE;						// send menusel Cnf flag
LOCAL BOOLEAN s_mmistk_sendsms_cnf[MMI_DUAL_SYS_MAX] = {0};							// send SMS Cnf flag

// send Cnf flag
LOCAL BOOLEAN s_mmistk_selectitem_cnf = FALSE;
LOCAL BOOLEAN s_mmistk_displaytext_cnf = FALSE;
LOCAL BOOLEAN s_mmistk_getinkey_cnf = FALSE;
LOCAL BOOLEAN s_mmistk_getinput_cnf = FALSE;
LOCAL BOOLEAN s_mmistk_playtone_cnf = FALSE;

// STK second menu title flag
LOCAL BOOLEAN s_mmistk_menutitle_need_refresh = FALSE;

// visual process status
LOCAL BOOLEAN s_mmistk_sms_is_sending[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_mmistk_item_is_selecting[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_mmistk_text_is_displaying[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_mmistk_key_is_getting[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_mmistk_input_is_getting[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_mmistk_tone_is_playing[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_mmistk_sendss_waiting_respond = FALSE;
LOCAL BOOLEAN s_mmistk_sendussd_waiting_respond = FALSE;
// close window in the future
LOCAL MMISTK_CLOSE_WIN_E s_mmistk_close_window = MMISTK_CLOSE_NONE;
LOCAL uint8 *s_mmistk_dtmf_string = PNULL;
LOCAL uint16 s_mmistk_dtmf_len = 0;
LOCAL uint16 s_mmistk_dtmf_index = 0;
LOCAL uint8 s_mmistk_dtmf_timer_id = 0;
LOCAL BOOLEAN s_is_mmi_dtmf_pause = FALSE;
LOCAL MMISTK_PDP_STATUS_E s_mmistk_pdp_state = MMISTK_PDP_STATUS_NONE;
LOCAL SIMAT_TRANS_LEVEL_T s_mmistk_trans_info = {0};/*lint !e64*/
LOCAL SIMAT_OTHER_ADDRESS_T s_mmistk_dest_addr = {0};/*lint !e64*/
LOCAL BOOLEAN s_mmistk_is_bear_desc_qos = FALSE;
LOCAL BOOLEAN s_mmistk_is_buf_size = FALSE;
LOCAL BOOLEAN s_mmistk_is_other_address= FALSE;
LOCAL SIMAT_BEAR_DESC_T s_mmistk_bear_desc ={0};
LOCAL SIMAT_OTHER_ADDRESS_T s_mmistk_other_address = {0};
LOCAL uint16 s_mmistk_buf_size = 0;
LOCAL long s_mmistk_sock_id = -1;
LOCAL MMISTK_CHANNELINFO_T s_mmistk_channel_info[MMISTK_CHANNEL_MAX] = {0};
LOCAL MMISTK_PDPINFO_T* s_mmistk_pdp_info = PNULL;
LOCAL uint8 s_mmistk_reconnect_socket_times = 0;
LOCAL uint8 s_mmistk_socket_receive_times = 0;
LOCAL uint8 s_mmistk_get_data_times = 0;
LOCAL uint8 *s_mmistk_data_receive_ptr = PNULL;
LOCAL uint8 *s_mmistk_data_receive_ptr_copy = PNULL;
LOCAL uint32 s_mmistk_data_receive_len = 0;
LOCAL uint32 s_mmistk_data_send_len = 0;
//bug1299237
LOCAL uint8 *s_mmistk_data_send_ptr = PNULL;
LOCAL uint8 *s_mmistk_data_send_copy_ptr = PNULL;
/*for bug 1300675*/
LOCAL MMI_STRING_T   s_mmistk_refresh_dis_str={0};
LOCAL BOOLEAN        s_mmistk_refresh_is_distext=FALSE;

LOCAL SIMAT_REFRESH_TYPE_E  s_mmistk_refresh_type[MMI_DUAL_SYS_MAX] = {0};

#ifdef WIN32
LOCAL uint8 data_receive[1000] =
{
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
      0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
      0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
      0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
      0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
      0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
      0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
      0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
      0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
      0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
      0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
      0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
      0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
      0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
      0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
      0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
      0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
      0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
      0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
      0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
      0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
      0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
      0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
      0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
      0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
      0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
      0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
      0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7
      };
#endif

/**--------------------------------------------------------------------------*
			                         TYPE AND CONSTANT
 **--------------------------------------------------------------------------*/
const MMI_STK_DISPLAY_T mmi_stk_display = 
{
    MMI_DEFAULT_TEXT_FONT,                          // text_font
    MMI_DEFAULT_TEXT_COLOR,                         // text_color
    MMI_WINDOW_BACKGROUND_COLOR,        // bkgd_color
    0,	                                                            // char_space
    0,                                                          // line_space
    SCROLL_BAR_WIDTH                            // scrollbar_width
};

typedef struct sci_sockaddr SockAddr;

typedef struct _MMISTK_LANG_NOTI_T_
{
    uint8 lang_type;
    char lang_noti[MN_SIMAT_LANGE_LEN + 1];
}MMISTK_LANG_NOTI_T;

LOCAL const MMISTK_LANG_NOTI_T s_lang_noti[] = {
    {MMISET_LANGUAGE_ENGLISH, "en"},
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_SIMP_CHINESE, "zh"},
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_TRAD_CHINESE, "zh"},
#endif
#ifdef IM_ARABIC_SUPPORT
    {MMISET_LANGUAGE_ARABIC, "ar"},
#endif
#ifdef IM_FRENCH_SUPPORT
    {MMISET_LANGUAGE_FRENCH, "fr"},
#endif
#ifdef IM_HINDI_SUPPORT
    {MMISET_LANGUAGE_HINDI, "hi"},
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {MMISET_LANGUAGE_HUNGARIAN, "hu"},
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {MMISET_LANGUAGE_INDONESIAN, "id"},
#endif
#ifdef IM_MALAY_SUPPORT
    {MMISET_LANGUAGE_MALAY, "ms"},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {MMISET_LANGUAGE_PORTUGUESE, "pt"},
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {MMISET_LANGUAGE_RUSSIAN, "ru"},
#endif
#ifdef IM_SPANISH_SUPPORT
    {MMISET_LANGUAGE_SPANISH, "es"},
#endif
#ifdef IM_TAGALOG_SUPPORT
    {MMISET_LANGUAGE_TAGALOG, "tl"},
#endif
#ifdef IM_THAI_SUPPORT
    {MMISET_LANGUAGE_THAI, "th"},
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {MMISET_LANGUAGE_VIETNAMESE, "vi"},
#endif
#ifdef IM_URDU_SUPPORT
    {MMISET_LANGUAGE_URDU, "ur"},
#endif
#ifdef IM_ITALIAN_SUPPORT
    {MMISET_LANGUAGE_ITALIAN, "it"},
#endif
#ifdef IM_PERSIAN_SUPPORT
    {MMISET_LANGUAGE_PERSIAN, "fa"},//"ps-IR"
#endif
#ifdef IM_TURKISH_SUPPORT
    {MMISET_LANGUAGE_TURKISH, "tr"},
#endif
#ifdef IM_GERMAN_SUPPORT
    {MMISET_LANGUAGE_GERMAN, "de"},
#endif
#ifdef IM_GREEK_SUPPORT
    {MMISET_LANGUAGE_GREEK, "el"},
#endif
#ifdef IM_HEBREW_SUPPORT
    {MMISET_LANGUAGE_HEBREW, "he"},
#endif
#ifdef IM_BENGALI_SUPPORT
    {MMISET_LANGUAGE_BENGALI, "be"},
#endif
#ifdef IM_CZECH_SUPPORT
    {MMISET_LANGUAGE_CZECH, "cs"},
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    {MMISET_LANGUAGE_SLOVENIAN, "sl"},
#endif
#ifdef IM_ROMANIAN_SUPPORT
    {MMISET_LANGUAGE_ROMANIAN, "ro"},
#endif
#ifdef IM_TELUGU_SUPPORT
    {MMISET_LANGUAGE_TELUGU, "te"},
#endif
#ifdef IM_MARATHI_SUPPORT
    {MMISET_LANGUAGE_MARATHI, "mr"},
#endif
#ifdef IM_TAMIL_SUPPORT
    {MMISET_LANGUAGE_TAMIL, "ta"},
#endif
#ifdef IM_GUJARATI_SUPPORT
    {MMISET_LANGUAGE_GUJARATI, "gu"},
#endif
#ifdef IM_KANNADA_SUPPORT
    {MMISET_LANGUAGE_KANNADA, "kn"},
#endif
#ifdef IM_MALAYALAM_SUPPORT
    {MMISET_LANGUAGE_MALAYALAM, "ml"},
#endif
#ifdef IM_ORIYA_SUPPORT
    {MMISET_LANGUAGE_ORIYA, "or"},
#endif
#ifdef IM_PUNJABI_SUPPORT
    {MMISET_LANGUAGE_PUNJABI, "pa"},
#endif
#ifdef IM_AFRIKAANS_SUPPORT
    {MMISET_LANGUAGE_AFRIKAANS, "af"},
#endif
#ifdef IM_ALBANIAN_SUPPORT
    {MMISET_LANGUAGE_ALBANIAN, "sq"},
#endif
#ifdef IM_ARMENIAN_SUPPORT
    {MMISET_LANGUAGE_ARMENIAN, "hy"},
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
    {MMISET_LANGUAGE_AZERBAIJANI, "az"},
#endif
#ifdef IM_BASQUE_SUPPORT
    {MMISET_LANGUAGE_BASQUE, "eu"},
#endif
#ifdef IM_BULGARIAN_SUPPORT
    {MMISET_LANGUAGE_BULGARIAN, "bg"},
#endif
#ifdef IM_CATALAN_SUPPORT
    {MMISET_LANGUAGE_CATALAN, "ca"},
#endif
#ifdef IM_CROATIAN_SUPPORT
    {MMISET_LANGUAGE_CROATIAN, "hr"},
#endif
#ifdef IM_DANISH_SUPPORT
    {MMISET_LANGUAGE_DANISH, "da"},
#endif
#ifdef IM_DUTCH_SUPPORT
    {MMISET_LANGUAGE_DUTCH, "nl"},
#endif
#ifdef IM_ESTONIAN_SUPPORT
    {MMISET_LANGUAGE_ESTONIAN, "et"},
#endif
#ifdef IM_FILIPINO_SUPPORT
    {MMISET_LANGUAGE_FILIPINO, "fp"},//"fil-PH"
#endif
#ifdef IM_FINNISH_SUPPORT
    {MMISET_LANGUAGE_FINNISH, "fi"},
#endif
#ifdef IM_GALICIAN_SUPPORT
    {MMISET_LANGUAGE_GALICIAN, "gl"},
#endif
#ifdef IM_GEORGIAN_SUPPORT
    {MMISET_LANGUAGE_GEORGIAN, "ka"},
#endif
#ifdef IM_HAUSA_SUPPORT
    {MMISET_LANGUAGE_HAUSA, "ha"},
#endif
#ifdef IM_ICELANDIC_SUPPORT
    {MMISET_LANGUAGE_ICELANDIC, "is"},
#endif
#ifdef IM_IGBO_SUPPORT
    {MMISET_LANGUAGE_IGBO, "ig"},
#endif
#ifdef IM_IRISH_SUPPORT
    {MMISET_LANGUAGE_IRISH, "ga"},
#endif
#ifdef IM_KAZAKH_SUPPORT
    {MMISET_LANGUAGE_KAZAKH, "kk"},
#endif
#ifdef IM_LATVIAN_SUPPORT
    {MMISET_LANGUAGE_LATVIAN, "lv"},
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    {MMISET_LANGUAGE_LITHUANIAN, "lt"},
#endif
#ifdef IM_MACEDONIAN_SUPPORT
    {MMISET_LANGUAGE_MACEDONIAN, "mk"},
#endif
#ifdef IM_MOLDOVAN_SUPPORT
    {MMISET_LANGUAGE_MOLDOVAN, "mo"},
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    {MMISET_LANGUAGE_NORWEGIAN, "nb"},//"no"
#endif
#ifdef IM_POLISH_SUPPORT
    {MMISET_LANGUAGE_POLISH, "pl"},
#endif
#ifdef IM_SERBIAN_SUPPORT
    {MMISET_LANGUAGE_SERBIAN, "sr"},
#endif
#ifdef IM_SESOTHO_SUPPORT
    {MMISET_LANGUAGE_SESOTHO, "st"},
#endif
#ifdef IM_SLOVAK_SUPPORT
    {MMISET_LANGUAGE_SLOVAK, "sk"},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {MMISET_LANGUAGE_SWEDISH, "sv"},
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    {MMISET_LANGUAGE_UKRAINIAN, "uk"},
#endif
#ifdef IM_YORUBA_SUPPORT
    {MMISET_LANGUAGE_YORUBA, "yo"},
#endif
#ifdef IM_XHOSA_SUPPORT
    {MMISET_LANGUAGE_XHOSA, "xh"},
#endif
#ifdef IM_ZULU_SUPPORT
    {MMISET_LANGUAGE_ZULU, "zu"},
#endif
#ifdef IM_ASSAMESE_SUPPORT
    {MMISET_LANGUAGE_ASSAMESE, "as"},
#endif
#ifdef IM_SWAHILI_SUPPORT
    {MMISET_LANGUAGE_SWAHILI, "sw"}
#endif
//    {MMISET_LANGUAGE_HINGLISH, "en-IN"},
//    {MMISET_LANGUAGE_KHMER, "km-IN"},
//    {MMISET_LANGUAGE_MYANMAR, "my-IN"},
//    {MMISET_LANGUAGE_LAO, "lo-IN"},
//    {MMISET_LANGUAGE_AMHARIC, "am-ET"},
//    {MMISET_LANGUAGE_UYGHUR, "ug-ZH"},
};

typedef struct _MMISTK_ACTION_NAME_T_
{
    uint8 pc_tag;
    const char * menu_action_name_ptr;
}MMISTK_ACTION_NAME_T;

//Note:ref to GSM 11.14 section 13.4
LOCAL const MMISTK_ACTION_NAME_T s_menu_action_name[] = {
    {0x01, "REFRESH"},
    {0x02, "MORE TIME"},
    {0x03, "POLL INTERVAL"},
    {0x04, "POLLING OFF"},
    {0x05, "SET UP EVENT LIST"},
    {0x10, "SET UP CALL"},
    {0x11, "SEND SS"},
    {0x12, "SEND USSD"},
    {0x13, "SEND SHORT MESSAGE"},
    {0x14, "SEND DTMF"},
    {0x15, "LAUNCH BROWSER"},
    {0x20, "PLAY TONE"},
    {0x21, "DISPLAY TEXT"},
    {0x22, "GET INKEY"},
    {0x23, "GET INPUT"},
    {0x24, "SELECT ITEM"},
    {0x25, "SET UP MENU"},
    {0x26, "PROVIDE LOCAL INFORMATION"},
    {0x27, "TIMER MANAGEMENT"},
    {0x28, "SET UP IDLE MODEL TEXT"},
    {0x30, "PERFORM CARD APDU"},
    {0x31, "POWER ON CARD"},
    {0x32, "POWER OFF CARD"},
    {0x33, "GET READER STATUS"},
    {0x34, "RUN AT COMMAND"},
    {0x35, "LANG NOTIFICATION"},
    {0x40, "OPEN CHANNEL"},
    {0x41, "CLOSE CHANNEL"},
    {0x42, "RECEIVE DATA"},
    {0x43, "SEND DATA"},
    {0x44, "GET CHANNEL STATUS"},
    {0x60, "RETRIEVE MULTIMEDIA MESSAGE"},
    {0x61, "SUBMIT MULTIMEDIA MESSAGE"},
    {0x62, "DISPLAY MULTIMEDIA MESSAGE"}
};

typedef struct
{
    uint16 num_index;  //number index
    MMI_MESSAGE_ID_E key_msg;    //number key message
} MMISTK_NUMKEY_MAP_T;

LOCAL MMISTK_NUMKEY_MAP_T s_mmistk_numkey_map[] = {
                                                      0, MSG_APP_1,
                                                      1, MSG_APP_2,
                                                      2, MSG_APP_3,
                                                      3, MSG_APP_4,
                                                      4, MSG_APP_5,
                                                      5, MSG_APP_6,
                                                      6, MSG_APP_7,
                                                      7, MSG_APP_8,
                                                      8, MSG_APP_9,
                                                      9, MSG_APP_STAR,
                                                      10,MSG_APP_0,
                                                      11,MSG_APP_HASH
                                                      };

/**--------------------------------------------------------------------------*
			                         GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T   g_mmistk_app;										// STK application

/**--------------------------------------------------------------------------*
			                         EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/
extern  int strnicmp (const char *a, const char *b, size_t c);

/**--------------------------------------------------------------------------*
			                         LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN CanDisplayStk(MN_DUAL_SYS_E dual_sys);

LOCAL BOOLEAN ExistActiveOrSetupCall(void);

LOCAL void SIMATStr2MMIStr(SIMAT_DO_TEXT_STRING_T* simat_str, MMI_STRING_T* mmi_str, uint32 mmi_buf_len);

LOCAL void MMIStr2SIMATStr(SIMAT_DO_TEXT_STRING_T* simat_str, uint16 simat_buf_len, MMI_STRING_T* mmi_str, BOOLEAN is_pack);

LOCAL void AlphaId2MMIStr(SIMAT_DO_ALPHA_ID_T* alpha_id_str, MMI_STRING_T* mmi_str, uint32 mmi_buf_len);

LOCAL void SimItem2MMIStr(SIMAT_ITEMTEMPLATE_T* item_str, MMI_STRING_T* mmi_str, uint32 mmi_buf_len);

LOCAL void CreateDisplayTextVision(MN_DUAL_SYS_E dual_sys);

LOCAL void CreateGetInkeyVision(MN_DUAL_SYS_E dual_sys);

LOCAL void CreateGetInputVision(MN_DUAL_SYS_E dual_sys);

LOCAL void CreateSendSmsVision(MN_DUAL_SYS_E dual_sys);

LOCAL AUD_GENERIC_TONE_ID_E GetPlayToneId(SIMAT_TONE_ID_E tone);

LOCAL void OpenSelectItemOptionWin(MN_DUAL_SYS_E dual_sys);

LOCAL MMI_RESULT_E SelectItemOptionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void CreatePlayToneVision(MN_DUAL_SYS_E dual_sys);

LOCAL void CreateSetupCallVision(MN_DUAL_SYS_E dual_sys);

LOCAL void CheckUnCloseWin(uint32 curr_win);

LOCAL void CheckCloseDisplayTextIRWin(void);

LOCAL MMI_RESULT_E MenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E DisplayTextWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E GetInkeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E GetInputWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E SendSmsWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E PlayToneWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL ERR_MNSS_CODE_E MMISTK_SendReqToSs(MN_DUAL_SYS_E dual_sys, MN_SS_OPERATION_CODE_E ss_oper, MN_SS_CODE_E ss_code, MN_SS_BASIC_SERVICE_T ss_basic_service, MN_SS_REGISTER_PARAM_U ss_register_param,SIMAT_DO_SS_STRING_T ss_string);

LOCAL MMI_RESULT_E SendSSWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void CreateSendSSVision(MN_DUAL_SYS_E dual_sys);

LOCAL MMI_RESULT_E SetupCallWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL BOOLEAN	IsInProactiveStkSession(void);

LOCAL MMI_RESULT_E TopMenuOptionWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void OpenTopMenuOptionWin(MN_DUAL_SYS_E dual_sys);

LOCAL void HandlePlaytoneTimer(void);

LOCAL void HandleSetupCallDurationTimer(void);

LOCAL MMI_RESULT_E HandleSendDtmfWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL void HandleSTKSendDtmfStr(MN_DUAL_SYS_E dual_sys);

#if 0
// no use
LOCAL BOOLEAN Is_FocusSTKWin(void);
#endif

/*****************************************************************************/
//     Description : start setup call Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void StartSetupCallOperation(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:			OnSetupMenuCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		bown.zhang
 DATE:			2007.07.11
********************************************************************************/
LOCAL void OnSetupMenuInit(MN_DUAL_SYS_E dual_sys, SIMAT_SETUP_MENU_T* param_in, STK_MENU_INFO_T* toplevel_menu_info);

/*****************************************************************************/
//  Description : Refresh wating win
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRefreshWaitingWindow (
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM 		        param
                                           );

/********************************************************************************
 NAME:			OnRefreshCnfSimFileChange
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2008.04.24
********************************************************************************/
LOCAL ERR_MNDATAMAG_CODE_E OnRefreshCnfSimFileChange(MN_DUAL_SYS_E dual_sys,SIMAT_REFRESH_RESULT_T *refresh_result);

/********************************************************************************
 NAME:                      CreateToplevelMenuVision
 DESCRIPTION:           
 PARAM IN:		
 PARAM OUT:             None
 AUTHOR:                allen
 DATE:                  2004.10.06
********************************************************************************/
LOCAL void CreateToplevelMenuVision(MN_DUAL_SYS_E dual_sys, STK_MENU_INFO_T* toplevel_menu_info);

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatSoftkeyCtrl(
                            MMI_HANDLE_T    win_handle,     //win id
                            MMI_TEXT_ID_T   leftsoft_id,
                            MMI_TEXT_ID_T   midsoft_id,
                            MMI_TEXT_ID_T   rightsoft_id
                            );

#if 0
// no use
/********************************************************************************
 NAME:			MMISTK_LaunchBrowserAlertWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_LaunchBrowserAlertWin(MMI_STRING_T *text_ptr, MMI_IMAGE_ID_T image_id);
#endif

#ifdef BROWSER_SUPPORT
/********************************************************************************
 NAME:			MMISTK_LaunchBrowserWithURL
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_LaunchBrowserWithURL(MN_DUAL_SYS_E dual_sys, uint8 *url_ptr);

/********************************************************************************
 NAME:			MMISTK_LaunchBrowserByDefault
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_LaunchBrowserByDefault(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
  NAME:          CreateLaunchBrowserVision
  DESCRIPTION:   
  PARAM IN:      
  PARAM OUT:     None
  AUTHOR:        dave.ruan
  DATE:
 ********************************************************************************/
 LOCAL void CreateLaunchBrowserVision( APP_MN_SIMAT_LAUNCH_BROWSER_IND_T* launch_browser_ind);

/*****************************************************************************/
//     Description : start setup call Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void StartLaunchBrowserOperation(MN_DUAL_SYS_E dual_sys,SIMAT_LAUNCH_BROWSER_T* launch_para_in);

/********************************************************************************
 NAME: LaunchBrowserWinHandleMsg
 DESCRIPTION:Launch Browser Win Handle Msg
 PARAM IN:
 PARAM OUT:		
 AUTHOR:dave.ruan
 DATE:
********************************************************************************/
LOCAL MMI_RESULT_E LaunchBrowserWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

/********************************************************************************
 NAME:			CloseAllStkWin
 DESCRIPTION: close all stk win id, if another sim come STK message
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		dave.ruan
 DATE:			2011.11.28
 ********************************************************************************/
LOCAL void CloseAllStkWin(void);

/********************************************************************************
 NAME:			MMISTK_SetPdpActiveStatus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_E state);

/********************************************************************************
 NAME:			MMISTK_GetPdpActiveStatus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL MMISTK_PDP_STATUS_E MMISTK_GetPdpActiveStatus(void);

#ifdef MMI_OPEN_CHANNEL_SUPPORT
/********************************************************************************
 NAME:			MMISTK_CreateSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL long MMISTK_CreateSocket(uint8 net_id);
#endif

/********************************************************************************
 NAME:			MMISTK_CloseSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_CloseSocket(long sock_id);

/********************************************************************************
 NAME:			MMISTK_ConnectSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_ConnectSocket(long sock_id);

/********************************************************************************
 NAME:			MMISTK_SocketSendData
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL int MMISTK_SocketSendData(long sock_id, char *in_buf, int in_len);

#ifdef MMI_OPEN_CHANNEL_SUPPORT
/********************************************************************************
 NAME:			MMISTK_HandlePdpActiveCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_HandlePdpActiveCnf(MMIPDP_CNF_INFO_T * msg_ptr);

/********************************************************************************
 NAME:			MMISTK_ActivePDPcontext
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void HandleSTKPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);
#endif

/********************************************************************************
 NAME:			MMISTK_ActivePDPcontext
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_ActivePDPContext(MN_DUAL_SYS_E dual_sys, uint8 *apn_ptr,  uint8 *login_ptr, uint8 *psw_ptr);

/********************************************************************************
 NAME:			MMISTK_OnCloseChannelCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_DeactivePDPContext(void);

/********************************************************************************
 NAME:			MMISTK_CnfOpenChannel
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_CnfOpenChannel(BOOLEAN is_success, SIMAT_GENERAL_PC_RESULT_E result);

/********************************************************************************
 NAME:			MMISTK_AssignChannelID
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL uint8 MMISTK_AssignChannelID(void);

/********************************************************************************
 NAME:			MMISTK_IsAllChannelIDReleased
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_IsAllChannelIDReleased(void);

/********************************************************************************
 NAME:			MMISTK_GetSocketData
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_GetSocketData(void);

/********************************************************************************
 NAME:			HandleSTKSocketGetDataTimer
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void HandleSTKSocketGetDataTimer(void);

/********************************************************************************
 NAME: OpenChannelWinHandleMsg
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:		
 AUTHOR:
 DATE:
********************************************************************************/
LOCAL MMI_RESULT_E OpenChannelWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/********************************************************************************
  NAME:          CreateOpenChannelVision
  DESCRIPTION:   
  PARAM IN:      
  PARAM OUT:     None
  AUTHOR:        
  DATE:
 ********************************************************************************/
 LOCAL void CreateOpenChannelVision(APP_MN_SIMAT_OPEN_CHANNEL_IND_T* open_channel_ind);

/*****************************************************************************/
//  Description : Open Channel waitting win
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenChannelWaitingWin(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM           param
                                           );

#if 0
// no use
/*****************************************************************************/
// 	Description : Create Open Channel Waiting Window
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void CreateOpenChannelWaitingWin(MMI_STRING_T *open_channel_txt_ptr);
#endif

/*****************************************************************************/
//  Description : Get Menu Action Id Data
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL uint16 GetMenuActionIdName(uint8 action_id, wchar *name_ptr, uint16 max_name_len);

/*****************************************************************************/
//  Description : Set Menu Action Id Data
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL void SetMenuActionIdData(
                                                               STK_MENU_INFO_T *menu_info_ptr,
                                                               void *tips_data_ptr
                                                               );

/*****************************************************************************/
//  Description : CreateDynaMenuItem
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL void CreateDynaMenuItem(MMI_CTRL_ID_T ctrl_id, STK_MENU_INFO_T *menu_info_ptr);

/*****************************************************************************/
//  Description : CreateDynaMenuItemData
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL void CreateDynaMenuItemData(MMI_CTRL_ID_T ctrl_id, uint16 index, STK_MENU_INFO_T *menu_info_ptr,MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : CreateDynaMenuItemContent
//  Global resource dependence : none
//  Author: andrew.ma
//  Note: 
//        
/*****************************************************************************/
LOCAL void CreateDynaMenuItemContent(MMI_CTRL_ID_T ctrl_id, uint16 index, uint16 cont_index, STK_MENU_INFO_T *menu_info_ptr, MN_DUAL_SYS_E dual_sys);

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify STK
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifySTKFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif

/*****************************************************************************/
//     Description : Create STK Buffer Memory
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateSTKBufferMemory(void** stk_buffer_ptr, uint16 buffer_size);

/*****************************************************************************/
//     Description : Destroy STK Str Memory
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroySTKStrMemory(void** stk_buffer_ptr);

/*****************************************************************************/
//     Description : Create icon display anim control
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateIconAnimCtrl(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUIIMG_BITMAP_T* icon_bitmap, GUI_BOTH_RECT_T *both_rect_ptr);

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
/*****************************************************************************/
//     Description : decode sim image (L4) to bitmap data(for anim control)
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeSimImgToBmp(MN_SIM_IMG_T* sim_img, GUIIMG_BITMAP_T *icon_bitmap);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeBasicImg(MN_SIM_IMG_T* sim_img, GUIIMG_BITMAP_T *icon_bitmap);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeColorImg(MN_SIM_IMG_T* sim_img, GUIIMG_BITMAP_T *icon_bitmap);
#endif

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetRichtextItemInfo(MN_DUAL_SYS_E dual_sys,
                                                                BOOLEAN is_icon_id,
                                                                SIMAT_DO_ICON_ID_T icon_id,
                                                                MMI_STRING_T display_data_string,
                                                                MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void SetStkWinTitle(MN_DUAL_SYS_E dual_sys, MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*
			                         GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/

/*===============================================================================
								IMPLEMENTATION
===============================================================================*/

/********************************************************************************
 NAME:			CanDisplayStk
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL BOOLEAN CanDisplayStk(MN_DUAL_SYS_E dual_sys)
{
    if (MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID)) 
    {
        if (s_mmistk_item_is_selecting[dual_sys] || 
            s_mmistk_text_is_displaying[dual_sys] || 
            s_mmistk_key_is_getting[dual_sys] || 
            s_mmistk_input_is_getting[dual_sys] || 
            s_mmistk_sms_is_sending[dual_sys] || 
            s_mmistk_tone_is_playing[dual_sys]
            )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/********************************************************************************
 NAME:			ExistActiveOrSetupCall
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL BOOLEAN ExistActiveOrSetupCall(void)
{
    return (MMIAPICC_IsInState(CC_IN_CALL_STATE));
}

/********************************************************************************
 NAME:			SIMATStr2Uint8Str
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL void SIMATStr2Uint8Str(SIMAT_DO_TEXT_STRING_T* in_simat_str, uint8 *out_str_ptr, uint32 in_buf_len)
{
    uint16 str_len = 0;
    uint8 * temp_buf = PNULL;

    
    if(PNULL == in_simat_str ||PNULL == out_str_ptr||0 == in_buf_len)
    {
        //SCI_TRACE_LOW:"[MMI_STK] enter SIMATStr2Uint8Str:PNULL == in_simat_str ||PNULL == out_str_ptr||0 == in_buf_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1034_112_2_18_2_55_54_4,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter SIMATStr2Uint8Str, simat_str->dcs = 0x%02x, len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1038_112_2_18_2_55_54_5,(uint8*)"dd", in_simat_str->dcs, in_simat_str->len);

    switch (in_simat_str->dcs & 0x0C) 
    {
    case MMI_DCS_DEFAULT_ALPHABET:	// compressed 7 bit code
        SCI_TRACE_BUF("mmistk_win.c: SIMATStr2Uint8Str 7 bits string",in_simat_str->text_ptr,sizeof(in_simat_str->text_ptr));

        str_len = in_buf_len;

        temp_buf = SCI_ALLOC_APP(in_buf_len);
        SCI_MEMSET(temp_buf, 0x00, in_buf_len);

        SCI_Unpack7bitsTo8bits(in_simat_str->text_ptr, // in: the input data buffer
                               in_simat_str->len,	    // the length of the data input
                               0,
                               temp_buf,   // out: the output buf
                               (int16*)&str_len);

        str_len= MMIAPICOM_Default2Ascii(
                                            (uint8 *)temp_buf,
                                            out_str_ptr,
                                            str_len
                                            );

        SCI_FREE(temp_buf);
        temp_buf = PNULL;
        break;

   case MMI_DCS_8_BIT_ALPHABET:	// uncompressed 8 bit code//03.38 default
        str_len = (uint16)MIN(in_simat_str->len, in_buf_len);

        str_len = MMIAPICOM_Default2Ascii(
                                            (uint8 *)in_simat_str->text_ptr,
                                            out_str_ptr,
                                            str_len
                                            );
        break;

    case MMI_DCS_UCS2:	 // UNICODE
        MMI_WSTRNTOSTR(out_str_ptr,
                                    in_buf_len,
                                    (wchar *)in_simat_str->text_ptr,
                                    in_buf_len,
                                    in_buf_len
                                    );
        break;

    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave SIMATStr2Uint8Str"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1089_112_2_18_2_55_54_6,(uint8*)"");
}

/********************************************************************************
 NAME:			SIMATStr2MMIStr
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL void SIMATStr2MMIStr(SIMAT_DO_TEXT_STRING_T* simat_str, MMI_STRING_T* mmi_str, uint32 mmi_buf_len)
{
    uint8 * temp_buf = PNULL;
    uint8* temp_str  = PNULL;
    
    if(PNULL == simat_str ||PNULL == mmi_str||0 == mmi_buf_len)
    {
        //SCI_TRACE_LOW:"SIMATStr2MMIStr:PNULL == simat_str ||PNULL == mmi_str||0 == mmi_buf_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1106_112_2_18_2_55_54_7,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter SIMATStr2MMIStr, simat_str->dcs = 0x%02x, len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1110_112_2_18_2_55_54_8,(uint8*)"dd", simat_str->dcs, simat_str->len);
    SCI_MEMSET(mmi_str->wstr_ptr, 0, (mmi_buf_len * sizeof(wchar)));

    switch (simat_str->dcs & 0x0C) 
    {
        case MMI_DCS_DEFAULT_ALPHABET:	// compressed 7 bit code
            SCI_TRACE_BUF("mmistk_win.c: SIMATStr2MMIStr 7 bits string",simat_str->text_ptr,sizeof(simat_str->text_ptr));
            mmi_str->wstr_len = mmi_buf_len;
            temp_buf = SCI_ALLOC_APP(mmi_str->wstr_len);
            SCI_MEMSET(temp_buf, 0x00, mmi_str->wstr_len);
            
            SCI_Unpack7bitsTo8bits(simat_str->text_ptr, // in: the input data buffer
                                   simat_str->len,	    // the length of the data input
                                   0,
                                   temp_buf,   // out: the output buf
                                   (int16*)&mmi_str->wstr_len);

            mmi_str->wstr_len = MIN(mmi_str->wstr_len, mmi_buf_len);

            mmi_str->wstr_len = MMIAPICOM_Default2Wchar(
                                                        (uint8 *)temp_buf,
                                                        mmi_str->wstr_ptr,
                                                        mmi_str->wstr_len
                                                        );

            SCI_FREE(temp_buf);
            temp_buf = PNULL;

            //SCI_TRACE_LOW:"[MMI_STK] enter SIMATStr2MMIStr, 8 bit string length is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1138_112_2_18_2_55_54_9,(uint8*)"d", mmi_str->wstr_len);
            break;

        case MMI_DCS_8_BIT_ALPHABET:	// uncompressed 8 bit code//03.38 default
            mmi_str->wstr_len = (uint16)MIN(simat_str->len, mmi_buf_len);

            mmi_str->wstr_len = MMIAPICOM_Default2Wchar(
                                                        (uint8 *)simat_str->text_ptr,
                                                        mmi_str->wstr_ptr,
                                                        mmi_str->wstr_len
                                                        );
            break;

        case MMI_DCS_UCS2:	 // UNICODE
            mmi_str->wstr_len = MIN(simat_str->len/sizeof(wchar), mmi_buf_len);

            temp_str = (uint8*)mmi_str->wstr_ptr;

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            MMI_MEMCPY(temp_str,
                                mmi_buf_len*sizeof(wchar),
                                simat_str->text_ptr,
                                mmi_str->wstr_len*sizeof(wchar),
                                mmi_str->wstr_len*sizeof(wchar));
#else
            GUI_UCS2L2UCS2B(temp_str,
                                        mmi_buf_len*sizeof(wchar),
                                        simat_str->text_ptr,
                                        mmi_str->wstr_len*sizeof(wchar));
#endif
            break;

        default:
            break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave SIMATStr2MMIStr"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1174_112_2_18_2_55_54_10,(uint8*)"");
}

/********************************************************************************
 NAME:			MMIStr2SIMATStr
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL void MMIStr2SIMATStr(SIMAT_DO_TEXT_STRING_T* simat_str, uint16 simat_buf_len, MMI_STRING_T* mmi_str, BOOLEAN is_pack)
{
    if(PNULL == simat_str||PNULL == mmi_str)
    {
        //SCI_TRACE_LOW:"MMIStr2SIMATStr:PNULL == simat_str||PNULL == mmi_str "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1188_112_2_18_2_55_54_11,(uint8*)"");
        return;
    }
	
    switch (s_mmistk_input_type) 
    {
    case SIMAT_INPUT_DIGIT_ONLY:  // the input is be digit(0-9,*,#)
        if (is_pack) 
        {
            uint8* temp_buf = PNULL;
            int16 pack_len = MN_MAX_SIMAT_DATA_OBJECT_LEN;
            uint8 offset = 0;

            simat_str->dcs = MMI_DCS_DEFAULT_ALPHABET;

            temp_buf = SCI_ALLOC_APP(mmi_str->wstr_len);
            SCI_MEMSET(temp_buf, 0x00, mmi_str->wstr_len);

            MMI_WSTRNTOSTR(temp_buf,
                                        mmi_str->wstr_len,
                                        mmi_str->wstr_ptr,
                                        mmi_str->wstr_len,
                                        mmi_str->wstr_len);

            mmi_str->wstr_len = MIN(mmi_str->wstr_len, simat_buf_len);

            SCI_Pack8bitsTo7bits((uint8*)temp_buf,
                                            (int16)mmi_str->wstr_len,
                                            simat_str->text_ptr,
                                            &pack_len,
                                            &offset
                                            );

            simat_str->len = pack_len;

            SCI_FREE(temp_buf);
            temp_buf = PNULL;
        }
        else 
        {
            simat_str->dcs = MMI_DCS_8_BIT_ALPHABET;
            simat_str->len = (uint8)MIN(mmi_str->wstr_len, simat_buf_len);

            MMI_WSTRNTOSTR((uint8 *)simat_str->text_ptr,
                                        simat_buf_len,
                                        mmi_str->wstr_ptr,
                                        mmi_str->wstr_len,
                                        simat_str->len);
        }
        break;

    case SIMAT_INPUT_DUAL_DIGIT:
        simat_str->dcs = MMI_DCS_UCS2;
        simat_str->len = (uint8)(MIN(mmi_str->wstr_len*sizeof(wchar), simat_buf_len));

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        MMI_WSTRNCPY((wchar *)simat_str->text_ptr,
                MN_MAX_SIMAT_DATA_OBJECT_LEN/sizeof(wchar),
                mmi_str->wstr_ptr,
                mmi_str->wstr_len,
                simat_str->len/sizeof(wchar));
#else
        GUI_UCS2B2UCS2L((uint8 *)simat_str->text_ptr,
                                        MN_MAX_SIMAT_DATA_OBJECT_LEN,
                                        (uint8 *)mmi_str->wstr_ptr,
                                        simat_str->len);
#endif
        break;

    case SIMAT_INPUT_SMS_ALPHABET:	// the input is unpacked sms alphabet
        // whether need do this?
        //  pmi_unicode_to_alphabet(src_string, *src_string);
        if (is_pack) 
        {
            uint8* temp_buf = PNULL;
            int16 pack_len = MN_MAX_SIMAT_DATA_OBJECT_LEN;
            uint8 offset = 0;

            temp_buf = SCI_ALLOC_APP(simat_buf_len);
            SCI_MEMSET(temp_buf, 0x00, simat_buf_len);

            simat_str->dcs = MMI_DCS_DEFAULT_ALPHABET;

            simat_str->len = (uint8)MIN(mmi_str->wstr_len, simat_buf_len);
            simat_str->len = MMIAPICOM_Wchar2default(mmi_str->wstr_ptr,
                                                                                temp_buf,
                                                                                simat_str->len
                                                                            );

            SCI_Pack8bitsTo7bits((uint8*)temp_buf,
                                            (int16)simat_str->len,
                                            simat_str->text_ptr,
                                            &pack_len,
                                            &offset
                                            );

            simat_str->len = pack_len;

            SCI_FREE(temp_buf);
            temp_buf = PNULL;
        }
        else 
        {
            simat_str->dcs = MMI_DCS_8_BIT_ALPHABET;

            simat_str->len = (uint8)MIN(mmi_str->wstr_len, simat_buf_len);
            simat_str->len = MMIAPICOM_Wchar2default(mmi_str->wstr_ptr,
                                                                                simat_str->text_ptr,
                                                                                simat_str->len
                                                                            );
        }
        break;

    case SIMAT_INPUT_UCS2_ALPHABET: // the input is ucs2 alphabet
        simat_str->dcs = MMI_DCS_UCS2;
        simat_str->len = (uint8)(MIN(mmi_str->wstr_len*sizeof(wchar), simat_buf_len));

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        MMI_WSTRNCPY((wchar *)simat_str->text_ptr,
                MN_MAX_SIMAT_DATA_OBJECT_LEN/sizeof(wchar),
                mmi_str->wstr_ptr,
                mmi_str->wstr_len,
                simat_str->len/sizeof(wchar));
#else
        GUI_UCS2B2UCS2L((uint8 *)simat_str->text_ptr,
                                        MN_MAX_SIMAT_DATA_OBJECT_LEN,
                                        (uint8 *)mmi_str->wstr_ptr,
                                        simat_str->len);
#endif
        break;

    default:
        break;
    }
}

/********************************************************************************
 NAME:			AlphaId2MMIStr
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.27
********************************************************************************/
LOCAL void AlphaId2MMIStr(SIMAT_DO_ALPHA_ID_T* alpha_id_str, MMI_STRING_T* mmi_str, uint32 mmi_buf_len)
{
    if(PNULL == alpha_id_str||PNULL == mmi_str)
    {
        //SCI_TRACE_LOW:"AlphaId2MMIStr:PNULL == alpha_id_str||PNULL == mmi_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1335_112_2_18_2_55_54_12,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter AlphaId2MMIStr, alpha_id_str->id_ptr[0] = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1339_112_2_18_2_55_54_13,(uint8*)"d", alpha_id_str->id_ptr[0]);
    MMIAPICOM_ParseSimStr(alpha_id_str->id_ptr, alpha_id_str->len, mmi_str, mmi_buf_len);
}

/********************************************************************************
 NAME:			SimItem2MMIStr
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.11.11
********************************************************************************/
LOCAL void SimItem2MMIStr(SIMAT_ITEMTEMPLATE_T* item_str, MMI_STRING_T* mmi_str, uint32 mmi_buf_len)
{
    uint32 i = 0;
    uint32 j = 0;
    wchar filter_char_value[]= {0x0d, 0x0a};
    uint16 filter_char_len = 0;
    wchar string_data[MMISTK_MENU_ITEMSTR_LEN + 1] = {0};
    uint16 string_data_len = 0;

    if(PNULL == item_str||PNULL == mmi_str)
    {
        //SCI_TRACE_LOW:"SimItem2MMIStr:PNULL == item_str||PNULL == mmi_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1361_112_2_18_2_55_54_14,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter SimItem2MMIStr, item_str->item_content_entry_buf[0] = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1365_112_2_18_2_55_54_15,(uint8*)"d", item_str->item_content_entry_buf[0]);
    MMIAPICOM_ParseSimStr(item_str->item_content_entry_buf, item_str->len, mmi_str, mmi_buf_len);

    filter_char_len = sizeof(filter_char_value)/sizeof(filter_char_value[0]);

    for (i = 0; i < mmi_str->wstr_len; i++)
    {
        BOOLEAN is_filter = FALSE;

        for (j = 0; j < filter_char_len; j++)
        {
            if (filter_char_value[j] == mmi_str->wstr_ptr[i])
            {
                is_filter = TRUE;
                break;
            }
        }

        if (!is_filter)
        {
            string_data[string_data_len++] = mmi_str->wstr_ptr[i];
        }

        if (string_data_len >= MMISTK_MENU_ITEMSTR_LEN)
        {
            break;
        }
    }

    SCI_MEMSET(mmi_str->wstr_ptr, 0x00, mmi_str->wstr_len * sizeof(wchar));
    MMI_WSTRNCPY(mmi_str->wstr_ptr, mmi_str->wstr_len, string_data, MMISTK_MENU_ITEMSTR_LEN, string_data_len);
    mmi_str->wstr_len = string_data_len;
}

/********************************************************************************
 NAME:			MMISTK_OnSetupMenuCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnSetupMenuCmd(void* arg)
{
    SIMAT_SETUP_MENU_T* param_in = PNULL;
    APP_MN_SIMAT_SETUP_MENU_IND_T* arg_in_ptr = PNULL;
    uint16 item_index = 0;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSetupMenuCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1414_112_2_18_2_55_54_16,(uint8*)"");
        return;
    }

    arg_in_ptr = (APP_MN_SIMAT_SETUP_MENU_IND_T *)arg;
    param_in = (SIMAT_SETUP_MENU_T*)(&arg_in_ptr->com);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSetupMenuCmd, item_data_object_num is %d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1421_112_2_18_2_55_54_17,(uint8*)"dd",param_in->item.item_data_object_num,arg_in_ptr->dual_sys);

#ifdef WIN32
	arg_in_ptr->dual_sys = 0;
#endif
    if(arg_in_ptr->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSetupMenuCmd:arg_in_ptr->dual_sys >= MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1425_112_2_18_2_55_54_18,(uint8*)"");
        return;
    }

    if (PNULL != s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys])
    {
        if (s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->is_icon_id)
        {
            DestroySTKStrMemory(&(s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->menu_title_icon_bmp.bit_ptr));
        }

        if (s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->is_icon_list)
        {
            while (item_index < s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->menu_item_amount)
            {
                DestroySTKStrMemory(&(s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->menu_item[item_index].icon_bmp.bit_ptr));
                item_index++;
            }
        }

        DestroySTKStrMemory(&s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->menu_item);
    }
    CreateSTKBufferMemory(&s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys], sizeof(STK_MENU_INFO_T));

//####ÒòÎªarg_in_ptr->dual_sys´«¹ýÀ´µÄÖµÊÇ-1»áµ¼ÖÂassert£¬ÁÙÊ±µÄ½â¾ö·½°¸ÊÇ½«¸Ã²ÎÊýÇ¿ÐÐÉèÖÃÎª0£¬ºóÐø»áÑ°Çó½â¾ö·½°¸
#ifndef WIN32
    OnSetupMenuInit(arg_in_ptr->dual_sys, param_in, s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]);
#else
    OnSetupMenuInit(0, param_in, s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]);
#endif
    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSetupMenuCmd,dual_sys=%d,menu_item_amount = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1450_112_2_18_2_55_54_19,(uint8*)"dd", arg_in_ptr->dual_sys, s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]->menu_item_amount);

    if (arg_in_ptr->dual_sys == MMISTK_GetSTKSim())
    {
        if((!s_simtk_is_detected[arg_in_ptr->dual_sys]) && (MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID)))
        {
            MMK_CloseWin(MMISTK_MAINMENU_WIN_ID);
        }
        //if Open STK mainmenu update menu
        else if (MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID))
        {
            CreateToplevelMenuVision(arg_in_ptr->dual_sys, s_mmistk_toplevel_menu_info[arg_in_ptr->dual_sys]);    
        }
    }
}

/********************************************************************************
 NAME:			MMISTK_OnSelectItemCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnSelectItemCmd(void* arg)
{
    uint8 m = 0, menuitem_offset = 0;
    SIMAT_SELECT_ITEM_T* param_in = PNULL;
    SIMAT_ITEMTEMPLATE_T menuitem = {0};
    SIMAT_SELECT_ITEM_CNF_T param_out = {0, SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};
    MMI_STRING_T menu_name = {PNULL, 0};
    //GUI_RECT_T menu_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T both_rect = {0};
    BOOLEAN result = FALSE;
    BOOLEAN menuwin_is_open = FALSE;
    int32 i = 0;
    APP_MN_SIMAT_SELECT_ITEM_IND_T *arg_in_ptr = PNULL;
    uint16 index = 0;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_can_display = FALSE;//(is_icon_id ||self_explan) && decode OK
#endif

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSelectItemCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1490_112_2_18_2_55_55_20,(uint8*)"");
        return;
    }

    arg_in_ptr = (APP_MN_SIMAT_SELECT_ITEM_IND_T *)arg;
    param_in = (SIMAT_SELECT_ITEM_T*)(&arg_in_ptr->com);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSelectItemCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1497_112_2_18_2_55_55_21,(uint8*)"");

    if (PNULL != s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys])
    {
        if (s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_id)
        {
            DestroySTKStrMemory(&(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_icon_bmp.bit_ptr));
        }
        if (s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_list)
        {
            while (index < s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount)
            {
                DestroySTKStrMemory(&(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[index].icon_bmp.bit_ptr));
                index++;
            }
        }
    }

    CheckCloseDisplayTextIRWin();

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSelItemEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        param_out.mp_info = SIMAT_MP_BUSY_ON_CALL;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSelItemEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        MMK_CloseWin(MMISTK_MENU_WIN_ID);
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    CheckCloseDisplayTextIRWin();

    // this means can call SIMAT_CnfSelItem() function
    s_mmistk_selectitem_cnf = TRUE;

    if (PNULL != s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys])
    {
        DestroySTKStrMemory(&s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item);
    }

    if (!CreateSTKBufferMemory(&s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys], sizeof(STK_MENU_INFO_T)))//申请不到内存
    {
        if (s_mmistk_selectitem_cnf)
        {
            param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            param_out.result = SIMAT_TR_ME_UNABLE;
            s_mmistk_selectitem_cnf = FALSE;
            s_mmistk_item_is_selecting[arg_in_ptr->dual_sys] = FALSE;
            SIMAT_CnfSelItemEx((uint8)arg_in_ptr->dual_sys, &param_out);
            //SCI_TRACE_LOW:"[MMI_STK] Leave MMISTK_OnSelectItemCmd No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1571_112_2_18_2_55_55_23,(uint8*)"");
        }

        return;
    }

    if (!CanDisplayStk(arg_in_ptr->dual_sys)) 
    {
        if (s_mmistk_selectitem_cnf)
        {
            param_out.result = SIMAT_TR_TERMINATED_BY_USER;
            param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_selectitem_cnf = FALSE;
            s_mmistk_item_is_selecting[arg_in_ptr->dual_sys] = FALSE;
            SIMAT_CnfSelItemEx((uint8)arg_in_ptr->dual_sys, &param_out);
        }
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSelectItemCmd because CanDisplayStk() return FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1587_112_2_18_2_55_55_24,(uint8*)"");
        return;
    }
    s_mmistk_item_is_selecting[arg_in_ptr->dual_sys] = TRUE;
    /////////////////////////////
    // Create Menu Window & Menu
    /////////////////////////////

    /*if (s_mmistk_menutitle_need_refresh) */
    {
        SCI_MEMSET(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title, 0, sizeof(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title));
        s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len = 0;
    }

    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_alpha_id = %d, s_mmistk_menutitle_need_refresh = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1601_112_2_18_2_55_55_25,(uint8*)"dd", param_in->is_alpha_id, s_mmistk_menutitle_need_refresh);
    // result means can get title string
    result = TRUE;

    if (!param_in->is_icon_id || !param_in->icon_id.is_self_explanatory)
    {
        if (param_in->is_alpha_id /*&& s_mmistk_menutitle_need_refresh*/) 
        {
            //SCI_TRACE_LOW:"[MMI_STK] param_in->alpha_id.id_ptr = 0x%02x, param_in->alpha_id.len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1609_112_2_18_2_55_55_26,(uint8*)"dd", *(param_in->alpha_id.id_ptr), param_in->alpha_id.len);
            if ((uint8)0x80 == *(param_in->alpha_id.id_ptr) || 
                (uint8)0x81 == *(param_in->alpha_id.id_ptr) || 
                (uint8)0x82 == *(param_in->alpha_id.id_ptr)) 
            {
                if (param_in->alpha_id.len >= 2)		// the first byte is flag, and the 2nd & 3th is UNICODE
                {
                    menu_name.wstr_ptr = (wchar*)s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title;

                    AlphaId2MMIStr(&param_in->alpha_id, &menu_name, MMISTK_MENU_TITLE_LEN);

                    //SCI_TRACE_LOW:"[MMI_STK] menu_name.wstr_len=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1620_112_2_18_2_55_55_27,(uint8*)"d", menu_name.wstr_len);

                    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len = menu_name.wstr_len;

                    s_mmistk_menutitle_need_refresh = FALSE;
                }
                else
                {
                    MMI_STRING_T opt_title_str = {0};		
                    MMI_GetLabelTextByLang(STXT_OPTION, &opt_title_str);
                    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len = opt_title_str.wstr_len;
                    SCI_MEMCPY(&(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title[0]),
                                        opt_title_str.wstr_ptr,
                                        opt_title_str.wstr_len * sizeof(wchar)
                                        );

                    result = FALSE;
                }
            }
            else 
            {
                if (param_in->alpha_id.len >= 1)
                {
                    menu_name.wstr_ptr = (wchar*)s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title;
                    AlphaId2MMIStr(&param_in->alpha_id, &menu_name, MMISTK_MENU_TITLE_LEN);

                    //SCI_TRACE_LOW:"[MMI_STK] menu_name.wstr_len=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1646_112_2_18_2_55_55_28,(uint8*)"d", menu_name.wstr_len);

                    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len = menu_name.wstr_len;

                    s_mmistk_menutitle_need_refresh = FALSE;
                }
                else
                {
                    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len = 0;
                    result = FALSE;
                }
            }
        }
        else /*if (s_mmistk_menutitle_need_refresh)*/
        {
            MMI_STRING_T opt_title_str = {0};
            MMI_GetLabelTextByLang(STXT_OPTION, &opt_title_str);
            s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len = opt_title_str.wstr_len * sizeof(wchar);
            SCI_MEMCPY(&(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title[0]),
                                opt_title_str.wstr_ptr, 
                                opt_title_str.wstr_len * sizeof(wchar)
                                );

            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    // if s_mmistk_menutitle_need_refresh is FALSE, it means the cur_menu title 
    // already have been initialized, and it should not be re-initialized.

    if (!result) 
    {
        s_mmistk_menutitle_need_refresh = FALSE;
    }
    if (MMK_IsOpenWin(MMISTK_MENU_WIN_ID))
    {
        menuwin_is_open = TRUE;
    }

    if (!menuwin_is_open) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_MENU_WIN_ID;
        win_create.func = MenuWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)arg_in_ptr->dual_sys;
        
        MMK_CreateWindow( &win_create );

        GUIWIN_CreateTitleDyna(MMISTK_MENU_WIN_ID, TXT_NULL);

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_MENU_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#endif

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_MENU_WIN_ID);
#endif

        result = TRUE;
    }

    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->help_info = param_in->help_info;

    if (menuwin_is_open) 
    {
        result = MMK_DestroyDynaCtrl(MMISTK_MENU_CTRL_ID);
    }
    
    both_rect = MMITHEME_GetWinClientBothRect(MMISTK_MENU_WIN_ID);

    menu_name.wstr_len = s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_len;
    menu_name.wstr_ptr = s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title;

    //set title
    GUIWIN_SetTitleText(MMISTK_MENU_WIN_ID, menu_name.wstr_ptr, menu_name.wstr_len, FALSE);

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_NONE);

    /////////////////////////////
    // 		Set Menu Data
    /////////////////////////////
    i = 0;

    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_action_id = FALSE;//cr195474  param_in->is_action_id;

    if (menuwin_is_open)
    {
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(MMISTK_MENU_WIN_ID, MMITHEME_GetTipsCtrlId()));
    }

    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_id = param_in->is_icon_id;
    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_list = param_in->is_icon_list;

    if (param_in->is_icon_id)
    {
        s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_icon_id = param_in->icon_id.icon_id;
        s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_menu_title_icon_self_explan = param_in->icon_id.is_self_explanatory;
    }

    if (param_in->is_icon_list)
    {
        s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_list_self_explan = param_in->icon_list.is_self_explanatory;
    }

    s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount = MIN(param_in->item.item_data_object_num, MMISTK_SELECTITEM_MAX_ITEM);
    //SCI_TRACE_LOW:"[MMI_STK] load %d current level menu items,object_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1759_112_2_18_2_55_55_29,(uint8*)"dd", s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount, param_in->item.item_data_object_num);

    if (!CreateSTKBufferMemory(&s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item, sizeof(STK_MENU_ITEM_T) * s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount))
    {
        if (s_mmistk_selectitem_cnf)
        {
            param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            param_out.result = SIMAT_TR_ME_UNABLE;
            s_mmistk_selectitem_cnf = FALSE;
            s_mmistk_item_is_selecting[arg_in_ptr->dual_sys] = FALSE;
            SIMAT_CnfSelItemEx((uint8)arg_in_ptr->dual_sys, &param_out);
            //SCI_TRACE_LOW:"[MMI_STK] Leave MMISTK_OnSelectItemCmd No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1770_112_2_18_2_55_55_30,(uint8*)"");
        }

        return;
    }

    for (m = 0; m < s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount; m++)
    {
        menuitem.id = param_in->item.item_data_object_info[m][0];
        menuitem.len = param_in->item.item_data_object_info[m][1];
        menuitem_offset = param_in->item.item_data_object_info[m][2];
        SCI_MEMCPY(menuitem.item_content_entry_buf,param_in->item.item_data_object_buf+menuitem_offset,
                            menuitem.len);

        // menu ID
        s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[i].itemID = menuitem.id;
        // menu name 
        // = ;
        menu_name.wstr_ptr = s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[i].stringData;
        
        SimItem2MMIStr(&menuitem, &menu_name,
            MMISTK_MENU_ITEMSTR_LEN /sizeof(wchar));

        s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[i].length = menu_name.wstr_len;

        if (param_in->is_action_id)
        {
            s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[i].nextActionIndicator = param_in->next_action.next_action_list[i];
        }
        if (param_in->is_icon_list)
        {
            s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[i].iconID = param_in->icon_list.icon_ids[i];
        }

        //////////////////////
        // 	   process next
        //////////////////////
        i++;
    }

    //create dynamic list
    if (s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount > 0)
    {
        GUILIST_INIT_DATA_T init_data = {0};

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
        if(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_id || s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_menu_title_icon_self_explan)
        {
            MN_SIM_IMG_T sim_img = {0};
            
            MNSIM_GetImgEx(arg_in_ptr->dual_sys, s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_icon_id, &sim_img);
            
            //decode OK
            if(DecodeSimImgToBmp(&sim_img, &(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_icon_bmp)))
            {
                is_icon_can_display = TRUE;
            }
            else
            {
                is_icon_can_display = FALSE;
            }
        }

        if (s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_id && is_icon_can_display)
        {
            both_rect.v_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
            both_rect.h_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
        }
#endif
#endif

        init_data.type = GUILIST_TEXTLIST_E;
        init_data.both_rect = both_rect;
        init_data.is_dynamic_item_data = TRUE;
        init_data.max_item_num = s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount;
        
        //dynamic create
        GUILIST_CreateListBox(MMISTK_MENU_WIN_ID, 0, MMISTK_MENU_CTRL_ID, &init_data);
    }

    /////////////////////////////
    // 		Set Menu Data
    /////////////////////////////
    CreateDynaMenuItem(MMISTK_MENU_CTRL_ID, s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]);

    if (s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_action_id)
    {
        MMI_CONTROL_CREATE_T create = {0};
        GUITIPS_INIT_DATA_T init_data = {0};

        init_data.both_rect = both_rect;

        create.ctrl_id = MMITHEME_GetTipsCtrlId();
        create.guid = SPRD_GUI_TIPS_ID;
        create.parent_win_handle = MMISTK_MENU_WIN_ID;
        create.init_data_ptr = &init_data;

        MMK_CreateControl(&create);
    }

    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_item_id = %d, param_in->item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1852_112_2_18_2_55_55_31,(uint8*)"dd", param_in->is_item_id, param_in->item_id);
    if(param_in->is_item_id)
    {
        uint16 select_index = 0;

        for (i = 0; i < s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item_amount; i++)
        {
            if (param_in->item_id == s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_item[i].itemID)
            {
                select_index = i;
                break;
            }
        }

        //SCI_TRACE_LOW:"[MMI_STK] GUILIST_SetTopItemIndex"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1866_112_2_18_2_55_55_32,(uint8*)"");
        GUILIST_SetCurItemIndex(MMISTK_MENU_CTRL_ID, select_index);
    }

    if (menuwin_is_open) 
    {
        if (!MMK_IsFocusWin(MMISTK_MENU_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_MENU_WIN_ID);
            MMK_SetAtvCtrl(MMISTK_MENU_WIN_ID, MMISTK_MENU_CTRL_ID);
        }
        else
        {
            MMK_SendMsg(MMISTK_MENU_WIN_ID, MSG_STK_SET_MENU_ATV, PNULL);
            MMK_PostMsg(MMISTK_MENU_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    if (s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if(is_icon_can_display)
        {
            GUI_BOTH_RECT_T anim_disp_rect = MMITHEME_GetWinClientBothRect(MMISTK_MENU_WIN_ID)/*MMITHEME_GetWinTitleBothRect(MMISTK_MENU_WIN_ID)*/;

            anim_disp_rect.v_rect.bottom = anim_disp_rect.v_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;
            anim_disp_rect.h_rect.bottom = anim_disp_rect.h_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;

            s_mmistk_icon_display_result = CreateIconAnimCtrl(MMISTK_MENU_WIN_ID, MMISTK_SELECT_ITEM_ANIM_CTRL_ID, &(s_mmistk_cur_menu_info_ptr[arg_in_ptr->dual_sys]->menu_title_icon_bmp), &anim_disp_rect);
        }
        else
#endif
        {
            s_mmistk_icon_display_result = FALSE;
        }
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSelectItemCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1905_112_2_18_2_55_55_33,(uint8*)"");
}

/********************************************************************************
 NAME:			SelectItemOptionWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		kelly.li
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E SelectItemOptionWinHandleMsg(MMI_WIN_ID_T win_id, 
												MMI_MESSAGE_ID_E msg_id, 
												DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    SIMAT_SELECT_ITEM_CNF_T menu_select = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    uint16  select_item_id = 0;
    uint16  menuitem_id = 1;
    uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID, &both_rect);

        MMK_SetAtvCtrl(MMISTK_SELECT_ITEM_OPTION_WIN_ID, MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID);
        if (0 != s_mmistk_selectitem_timer_id )
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;

    case MSG_FULL_PAINT:
        if (0 == s_mmistk_selectitem_timer_id && s_mmistk_selectitem_cnf)
        {
            s_mmistk_selectitem_timer_id = MMK_CreateWinTimer(	
                                                                                                MMISTK_SELECT_ITEM_OPTION_WIN_ID, 
                                                                                                MMISTK_SELECTITEM_DURATION,
                                                                                                FALSE
                                                                                                );
        }
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmistk_selectitem_timer_id) 
        {
            //SCI_TRACE_LOW:"[MMI_STK] after timer duration %d, Timer(%d) arrived"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1952_112_2_18_2_55_55_34,(uint8*)"dd", MMISTK_SELECTITEM_DURATION, s_mmistk_selectitem_timer_id);
            if (s_mmistk_selectitem_cnf) 
            {
                menu_select.result = SIMAT_TR_NO_RESPONSE;
                menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                s_mmistk_selectitem_cnf = FALSE;
                s_mmistk_item_is_selecting[dual_sys] = FALSE;
                SIMAT_CnfSelItemEx(dual_sys, &menu_select);
            }

            MMISTK_OpenAlertWin(TXT_STK_OVERTIME_RETURN, IMAGE_PUBWIN_WARNING, PNULL);
            s_mmistk_selectitem_timer_id = 0;
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        select_item_id = GUIMENU_GetCurNodeId(MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID);

        if(PNULL == s_mmistk_cur_menu_info_ptr[dual_sys])
        {
            //SCI_TRACE_LOW:"SelectItemOptionWinHandleMsg:PNULL == s_mmistk_cur_menu_info_ptr[dual_sys]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_1981_112_2_18_2_55_55_35,(uint8*)"");
            break;
        }

        if (1 == select_item_id)
        {
            //help
            menu_select.result = SIMAT_TR_HELP_INFO;
            menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        }
        else
        {
            //confirm
            if (s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_id || s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_list)
            {
                menu_select.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                menu_select.result = SIMAT_TR_SUCCESS;
            }

            menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        }

        if (s_mmistk_selectitem_cnf) 
        {
            if (s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount > 0)
            {
                menuitem_id = GUILIST_GetCurItemIndex(MMISTK_MENU_CTRL_ID);
            }

            if (menuitem_id < s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount)
            {
                menu_select.item_id = s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[menuitem_id].itemID;
                s_mmistk_selectitem_cnf = FALSE;
                s_mmistk_item_is_selecting[dual_sys] = FALSE;
                if (CreateSTKBufferMemory(&s_title_from_selected_item_context, sizeof(STK_MENU_ITEM_T)))
                {
                    SCI_MEMCPY(s_title_from_selected_item_context, &s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[menuitem_id], sizeof(STK_MENU_ITEM_T));
                }
                SIMAT_CnfSelItemEx(dual_sys, &menu_select);
            }
        }

        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        MMK_CloseWin(win_id);	
        break;

    case MSG_CLOSE_WINDOW:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;

    case MSG_LOSE_FOCUS:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;

    case MSG_GET_FOCUS:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;

    case MSG_APP_RED:
    case MSG_APP_FLIP:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        menu_select.result = SIMAT_TR_TERMINATED_BY_USER;
        menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        s_mmistk_selectitem_cnf = FALSE;
        s_mmistk_item_is_selecting[dual_sys] = FALSE;
        SIMAT_CnfSelItemEx(dual_sys, &menu_select);
        result = MMI_RESULT_FALSE;		// need additional process
       break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/********************************************************************************
 NAME:			open select item option window
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		kelly.li
 DATE:			
********************************************************************************/
LOCAL void OpenSelectItemOptionWin(MN_DUAL_SYS_E dual_sys)
{
#if 0
// no use
    BOOLEAN result = FALSE;
#endif
    MMI_STRING_T    opt_title_str = {0};
    MMI_STRING_T    opt_ok_str = {0};
    //GUI_RECT_T menu_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_WINDOW_CREATE_T win_create = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = MMISTK_SELECT_ITEM_OPTION_WIN_ID;
    win_create.func = SelectItemOptionWinHandleMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ptr = (ADD_DATA)dual_sys;

    MMK_CreateWindow( &win_create );
#ifndef MMI_PDA_SUPPORT
    CreatSoftkeyCtrl(MMISTK_SELECT_ITEM_OPTION_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#endif

#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(MMISTK_SELECT_ITEM_OPTION_WIN_ID);
#endif

#if 0
// no use
    result = TRUE;
#endif

    both_rect = MMITHEME_GetWinClientBothRect(MMISTK_SELECT_ITEM_OPTION_WIN_ID);
    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,MMISTK_SELECT_ITEM_OPTION_WIN_ID,MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID,GUIMENU_STYLE_THIRD);

    MMI_GetLabelTextByLang(TXT_HELP, &opt_title_str);
    node_item.item_text_ptr = &opt_title_str;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(0,1,0,&node_item,MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID);

    MMI_GetLabelTextByLang(TXT_COMMON_OK, &opt_ok_str);    
    node_item.item_text_ptr = &opt_ok_str;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(1,2,0,&node_item,MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID);

    //set title
    GUIWIN_CreateTitleDyna(MMISTK_SELECT_ITEM_OPTION_WIN_ID, TXT_HELP);

    GUIMENU_SetMenuTitle(&opt_title_str,MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID);
    GUIMENU_SetDynamicMenuSoftkey(MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID,TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
}

/********************************************************************************
 NAME:			TopMenuOptionWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		kelly.li
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E TopMenuOptionWinHandleMsg(MMI_WIN_ID_T win_id, 
                                                                                                MMI_MESSAGE_ID_E msg_id, 
                                                                                                DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    SIMAT_MENU_SELECTION_IND_T menu_select = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    uint16  select_item_id = 0;
    uint16  menuitem_id = 1;
    uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID, &both_rect);

        MMK_SetAtvCtrl(MMISTK_TOP_MENU_OPTION_WIN_ID, MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        select_item_id = GUIMENU_GetCurNodeId(MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID);
        if (1 == select_item_id)
        {
            //help
            menu_select.is_help_IND = TRUE;
        }
        else
        {
            //confirm
            menu_select.is_help_IND = FALSE;
        }

        if (s_mmistk_indmenusel_cnf)
        {
            if (s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount > 0)
            {
                menuitem_id = GUILIST_GetCurItemIndex(MMISTK_MAINMENU_CTRL_ID);
            }
            menu_select.menu_id = s_mmistk_toplevel_menu_info[dual_sys]->menu_item[menuitem_id].itemID;
            s_mmistk_indmenusel_cnf = FALSE;
            if(menuitem_id < s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount)
            {
                if (CreateSTKBufferMemory(&s_title_from_selected_item_context, sizeof(STK_MENU_ITEM_T)))
                {
                    SCI_MEMCPY(s_title_from_selected_item_context, &s_mmistk_toplevel_menu_info[dual_sys]->menu_item[menuitem_id], sizeof(STK_MENU_ITEM_T));				
                }
            }
            SIMAT_IndMenuSelEx(dual_sys, &menu_select);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);	
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/********************************************************************************
 NAME:			open Top Menu option window
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		kelly.li
 DATE:			
********************************************************************************/
LOCAL void OpenTopMenuOptionWin(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T    opt_title_str = {0};
    MMI_STRING_T    opt_ok_str = {0};
    //GUI_RECT_T menu_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_WINDOW_CREATE_T win_create = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = MMISTK_TOP_MENU_OPTION_WIN_ID;
    win_create.func = TopMenuOptionWinHandleMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ptr = (ADD_DATA)dual_sys;

    MMK_CreateWindow( &win_create );	

#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(MMISTK_TOP_MENU_OPTION_WIN_ID);
#endif

#ifndef MMI_PDA_SUPPORT
    GUIWIN_SetSoftkeyTextId(MMISTK_TOP_MENU_OPTION_WIN_ID,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#endif

    both_rect = MMITHEME_GetWinClientBothRect(MMISTK_TOP_MENU_OPTION_WIN_ID);
    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,MMISTK_TOP_MENU_OPTION_WIN_ID,MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID,GUIMENU_STYLE_THIRD);

    GUIWIN_CreateTitleDyna(MMISTK_TOP_MENU_OPTION_WIN_ID, TXT_NULL);

    MMI_GetLabelTextByLang(TXT_HELP, &opt_title_str);
    node_item.item_text_ptr = &opt_title_str;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(0,1,0,&node_item,MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID);

    MMI_GetLabelTextByLang(TXT_COMMON_OK, &opt_ok_str);
    node_item.item_text_ptr = &opt_ok_str;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(1,2,0,&node_item,MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID);

    GUIMENU_SetMenuTitle(&opt_title_str,MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID);
    GUIMENU_SetDynamicMenuSoftkey(MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID,TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
}

/********************************************************************************
 NAME:			CreateDisplayTextVision
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.07
********************************************************************************/
LOCAL void CreateDisplayTextVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_open = FALSE;

    if (!MMK_IsOpenWin(MMISTK_DISPLAYTEXT_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_DISPLAYTEXT_WIN_ID;
        win_create.func = DisplayTextWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;
        
        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_DISPLAYTEXT_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_DISPLAYTEXT_WIN_ID, TXT_NULL);

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_DISPLAYTEXT_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#else
        CreatSoftkeyCtrl(MMISTK_DISPLAYTEXT_WIN_ID, TXT_NULL, TXT_COMMON_OK, TXT_NULL);
#endif
        
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_DISPLAYTEXT_WIN_ID);
        
        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_DISPLAYTEXT_WIN_ID;
        create.init_data_ptr = &init_data;
        
        MMK_CreateControl( &create );

        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    SetStkWinTitle(dual_sys, MMISTK_DISPLAYTEXT_WIN_ID);

    SetRichtextItemInfo(dual_sys,
                                        s_mmistk_displaytext_is_icon_exist,
                                        s_mmistk_displaytext_icon_id,
                                        s_mmistk_displaydata,
                                        MMISTK_TEXBOX_CTRL_ID);

    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_DISPLAYTEXT_WIN_ID);

        if (!MMK_IsFocusWin(MMISTK_DISPLAYTEXT_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_DISPLAYTEXT_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_DISPLAYTEXT_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_DISPLAYTEXT_WIN);
}

/********************************************************************************
 NAME:			MMISTK_OnDisplayTextCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnDisplayTextCmd(void* arg)
{
    SIMAT_DISPLAY_TEXT_T* param_in = PNULL;
    SIMAT_DISPLAY_TEXT_CNF_T param_out = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};
    APP_MN_SIMAT_DISPLAY_TEXT_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnDisplayTextCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2401_112_2_18_2_55_56_36,(uint8*)"");
        return;
    }

    arg_in_ptr = (APP_MN_SIMAT_DISPLAY_TEXT_IND_T *)arg;
    param_in = (SIMAT_DISPLAY_TEXT_T*)(&arg_in_ptr->com);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnDisplayTextCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2408_112_2_18_2_55_56_37,(uint8*)"");

    MMK_CloseWin(MMISTK_MORE_TIME_WIN_ID);

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfDisplayTextEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        param_out.mp_info = SIMAT_MP_BUSY_ON_CALL;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfDisplayTextEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    if (0 != s_mmistk_displaytext_timer_id)
    {
        MMK_StopTimer(s_mmistk_displaytext_timer_id);
        s_mmistk_displaytext_timer_id = 0;
    }

    s_mmistk_displaytext_is_ir = FALSE;

    // this means can call SIMAT_CnfDisplayText() function
    s_mmistk_displaytext_cnf = TRUE;

    //@songbin.zeng cr58792
    //SCI_TRACE_LOW:"[MMI_STK] get if the text is high priority %d,if is display text cnf = %d, if is diaplaying text %d, is_ir= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2442_112_2_18_2_55_56_38,(uint8*)"dddd",param_in->high_priority,s_mmistk_displaytext_cnf,s_mmistk_text_is_displaying[arg_in_ptr->dual_sys],param_in->is_ir);

    //when the screen is busy,and is in proactive SIM session should display text no matter the priority
    if (!MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
#ifdef DYNAMIC_WATCHFACE_SUPPORT
        if(!MMK_IsFocusWin(WATCHFACE_WIN_ID))//2092392   watch idle win is watchface win
#endif
        {
            //is not in SIM session,and the priority is low,need not to display text
            if (!IsInProactiveStkSession()
                &&!MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
                &&!param_in->high_priority)
            {			
                if (s_mmistk_displaytext_cnf)
                {
                    param_out.result = SIMAT_TR_ME_UNABLE;
                    param_out.mp_info = SIMAT_MP_SCREEN_BUSY;
                    s_mmistk_displaytext_cnf = FALSE;
                    s_mmistk_text_is_displaying[arg_in_ptr->dual_sys] = FALSE;
                    SIMAT_CnfDisplayTextEx((uint8)arg_in_ptr->dual_sys, &param_out);
                }      
                //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnDisplayTextCmd because low priority"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2460_112_2_18_2_55_56_39,(uint8*)"");
                return;
            }
        }
    }

    s_mmistk_displaytext_is_icon_exist = param_in->is_icon_exist;

    if (s_mmistk_displaytext_is_icon_exist)
    {
        s_mmistk_displaytext_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_displaytext_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnDisplayTextCmd, if is icon exist = %d, the icon id is = %d, the text length is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2473_112_2_18_2_55_56_40,(uint8*)"ddd",param_in->is_icon_exist,param_in->icon_id.icon_id,param_in->text_str.len);

    //the icon identifier with bit 1 set to 0,and the text is null,should return
    if (!param_in->is_icon_exist  /*andrew modify, if icon exist, show icon*/
//        && !(param_in->icon_id.icon_id & 1)
        &&(0 == param_in->text_str.len))
    {
        param_out.result = SIMAT_TR_DATA_NOT_UNDERSTOOD;
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        s_mmistk_displaytext_cnf = FALSE;
        s_mmistk_text_is_displaying[arg_in_ptr->dual_sys] = FALSE;  
        SIMAT_CnfDisplayTextEx((uint8)arg_in_ptr->dual_sys, &param_out);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnDisplayTextCmd because the data not understood by ME"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2485_112_2_18_2_55_56_41,(uint8*)"");
        return;
    }

    if (param_in->user_clear) 
    {
        s_mmistk_displaytext_timer_duration = MMISTK_DISPLAYTEXT_USER_CLEAR_DURATION;
    }
    else
    {
        s_mmistk_displaytext_timer_duration = MMISTK_DISPLAYTEXT_DELAY_DURATION;
    }

    //SCI_TRACE_LOW:"[MMI_STK] DisplayText  timer_duration = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2498_112_2_18_2_55_57_42,(uint8*)"d", s_mmistk_displaytext_timer_duration);

    s_mmistk_text_is_displaying[arg_in_ptr->dual_sys] = TRUE;
    /////////////////////////////
    // 		Set Display Data
    /////////////////////////////
    CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));

    s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
    
    //SCI_TRACE_LOW:"[MMI_STK] param_in->text_str.len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2508_112_2_18_2_55_57_43,(uint8*)"d", param_in->text_str.len);
    SIMATStr2MMIStr(&(param_in->text_str), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_displaydata.length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2510_112_2_18_2_55_57_44,(uint8*)"d", s_mmistk_displaydata.wstr_len);
    /////////////////////////////
    // Create Window & TextBox
    /////////////////////////////
    MMIDEFAULT_TurnOnBackLight();

    CreateDisplayTextVision(arg_in_ptr->dual_sys);

    s_mmistk_displaytext_is_ir = param_in->is_ir;

    if (param_in->is_ir) 
    {
        if (s_mmistk_displaytext_cnf)
        {
            if (s_mmistk_displaytext_is_icon_exist)
            {
//                param_out.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                param_out.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                param_out.result = SIMAT_TR_SUCCESS;
            }
            param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

            s_mmistk_displaytext_cnf = FALSE;
            s_mmistk_text_is_displaying[arg_in_ptr->dual_sys] = FALSE;
            SIMAT_CnfDisplayTextEx((uint8)arg_in_ptr->dual_sys, &param_out);
        }
//      MMK_CloseWin(MMISTK_DISPLAYTEXT_WIN_ID);
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnDisplayTextCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2548_112_2_18_2_55_57_45,(uint8*)"");
}

/********************************************************************************
 NAME:			CreateGetInkeyVision
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.07
********************************************************************************/
LOCAL void CreateGetInkeyVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_edit_text = FALSE;
    void* edit_box_data = PNULL;
    BOOLEAN win_exist = FALSE;
    GUIEDIT_INIT_DATA_T init_data = {0};
    GUIIM_TYPE_T allow_im = 0;
    GUIIM_TYPE_T init_im = 0;
    MMI_TEXT_ID_T help_text_id = TXT_NULL;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_can_display = FALSE;//(is_icon_id ||self_explan) && decode OK
#endif

    win_exist = MMK_IsOpenWin(MMISTK_GETINKEY_WIN_ID);
    if (!win_exist)
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_GETINKEY_WIN_ID;
        win_create.func =   GetInkeyWinHandleMsg;                            
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;

        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_GETINKEY_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_GETINKEY_WIN_ID, TXT_NULL);
    }
    else
    {
        MMK_DestroyDynaCtrl(MMISTK_EDITBOX_CTRL_ID);
    }

    if (!win_exist)
    {
        if (s_mmistk_inkey_type != SIMAT_YES_NO)
        {
#ifndef MMI_PDA_SUPPORT
            CreatSoftkeyCtrl(MMISTK_GETINKEY_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#else
            if (s_mmistk_is_inkey_help_info)
            {
                CreatSoftkeyCtrl(MMISTK_GETINKEY_WIN_ID, STXT_OK, TXT_NULL, TXT_HELP);
            }
            else
            {
                CreatSoftkeyCtrl(MMISTK_GETINKEY_WIN_ID, TXT_COMMON_OK, TXT_NULL, TXT_NULL);
            }
#endif
        }
        else
        {
            CreatSoftkeyCtrl(MMISTK_GETINKEY_WIN_ID, TXT_YES, TXT_NULL, TXT_COMM_NO);
        }
    }
    else
    {
        if (s_mmistk_inkey_type != SIMAT_YES_NO)
        {
#ifndef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(MMISTK_GETINKEY_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN,FALSE);
#else
            if (s_mmistk_is_inkey_help_info)
            {
                GUIWIN_SetSoftkeyTextId(MMISTK_GETINKEY_WIN_ID, STXT_OK, TXT_NULL, TXT_HELP, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(MMISTK_GETINKEY_WIN_ID, TXT_COMMON_OK, TXT_NULL, TXT_NULL, FALSE);
            }
#endif
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(MMISTK_GETINKEY_WIN_ID, TXT_YES, TXT_NULL, TXT_COMM_NO,FALSE);
        }
    }

    switch (s_mmistk_inkey_type) 
    {
    case SIMAT_DIGIT_ONLY:
        init_data.type = GUIEDIT_TYPE_PHONENUM;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINKEY_WIN_ID);
        init_data.str_max_len = 1;
        break;

    case SIMAT_YES_NO:
        break;

    case SIMAT_SMS_ALPHABET:
        init_data.type = GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINKEY_WIN_ID);
        init_data.str_max_len = 1;
        is_edit_text = TRUE;

        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        break;

    case SIMAT_UCS2_ALPHABET:
        init_data.type = GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINKEY_WIN_ID);
        init_data.str_max_len = 1;
        is_edit_text = TRUE;

        allow_im = GUIIM_TYPE_SET_ALL;
        init_im = GUIIM_TYPE_SMART;
        break;

    default:
        break;
    }
    //bug2107932 watch theme no softkey ,stk win need softkey
    init_data.both_rect.v_rect.bottom = init_data.both_rect.v_rect.bottom - 80/*softkey height*/;

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
    if(s_mmistk_inkey_is_icon_id)
    {
        MN_SIM_IMG_T sim_img = {0};
        
        MNSIM_GetImgEx(dual_sys, s_mmistk_inkey_icon_id.icon_id, &sim_img);
        
        //decode OK
        if(DecodeSimImgToBmp(&sim_img, &(s_icon_bitmap)))
        {
            is_icon_can_display = TRUE;
        }
        else
        {
            is_icon_can_display = FALSE;
        }
    }

    if (is_icon_can_display)
    {
        init_data.both_rect.v_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
        init_data.both_rect.h_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
    }
#endif
#endif
    
    GUIWIN_SetTitleText(MMISTK_GETINKEY_WIN_ID, 
                                s_mmistk_displaydata.wstr_ptr,
                                s_mmistk_displaydata.wstr_len,
                                FALSE);

    if (s_mmistk_is_inkey_help_info)
    {
        help_text_id = TXT_HELP;
    }
    else
    {
        help_text_id = TXT_NULL;
    }

    if (s_mmistk_inkey_type != SIMAT_YES_NO)
    {        
        MMI_CONTROL_CREATE_T create = {0};
        
        create.ctrl_id = MMISTK_EDITBOX_CTRL_ID;
        create.guid    = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = MMISTK_GETINKEY_WIN_ID; //use win handle here
        create.init_data_ptr = &init_data;
        edit_box_data = MMK_CreateControl( &create );

        GUIEDIT_SetHandleRedKey(FALSE, create.ctrl_id);

#ifndef MMI_PDA_SUPPORT
        GUIEDIT_SetSoftkey(MMISTK_EDITBOX_CTRL_ID, 1, 0, help_text_id, TXT_NULL,PNULL);
#endif

        if (is_edit_text)
        {
            //set im
            //GUIEDIT_SetIm(create.ctrl_id,allow_im,init_im);//2103075
            GUIEDIT_SetIm(create.ctrl_id,GUIIM_TYPE_SYS,GUIIM_TYPE_SYS);
        }

        //set string
        if (0 != s_mmistk_inputdata.wstr_len)
        {
            GUIEDIT_SetString(create.ctrl_id, s_mmistk_inputdata.wstr_ptr, s_mmistk_inputdata.wstr_len);
        }
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(MMISTK_GETINKEY_WIN_ID, TXT_YES, help_text_id, TXT_COMM_NO, FALSE);
    }

    if (s_mmistk_inkey_is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if (is_icon_can_display)
        {
            GUI_BOTH_RECT_T anim_disp_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINKEY_WIN_ID)/*MMITHEME_GetWinTitleBothRect(MMISTK_GETINKEY_WIN_ID)*/;

            anim_disp_rect.v_rect.bottom = anim_disp_rect.v_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;
            anim_disp_rect.h_rect.bottom = anim_disp_rect.h_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;

            s_mmistk_icon_display_result = CreateIconAnimCtrl(MMISTK_GETINKEY_WIN_ID, MMISTK_GET_INKEY_ANIM_CTRL_ID, &s_icon_bitmap, &anim_disp_rect);
        }
        else
#endif
        {
            s_mmistk_icon_display_result = FALSE;
        }
    }

    if (win_exist) 
    {
        // because this window maybe same as the last window, in this case the Edit Ctrl must be reactive
        if (s_mmistk_inkey_type != SIMAT_YES_NO)
        {
            MMK_SetAtvCtrl(MMISTK_GETINKEY_WIN_ID, MMISTK_EDITBOX_CTRL_ID);
        }

        if (!MMK_IsFocusWin(MMISTK_GETINKEY_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_GETINKEY_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_GETINKEY_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_GETINKEY_WIN);
}
/********************************************************************************
 NAME:			MMISTK_OnGetInkeyCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnGetInkeyCmd(void* arg)
{
    SIMAT_GET_INKEY_T* param_in = PNULL;
    SIMAT_GET_INKEY_CNF_T param_out = {0};
    APP_MN_SIMAT_GET_INKEY_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnGetInkeyCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2786_112_2_18_2_55_57_46,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_GET_INKEY_IND_T *)arg;
    param_in = (SIMAT_GET_INKEY_T*)(&arg_in_ptr->com);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnGetInkeyCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2792_112_2_18_2_55_57_47,(uint8*)"");

    CheckCloseDisplayTextIRWin();

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfGetInkeyEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        param_out.mp_info = SIMAT_MP_BUSY_ON_CALL;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfGetInkeyEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    // this means can call SIMAT_CnfGetInkey() function
    s_mmistk_getinkey_cnf = TRUE;	

    if (!CanDisplayStk(arg_in_ptr->dual_sys)) 
    {
        if (s_mmistk_getinkey_cnf)
        {
            //SIMAT_CloseShareBuf(param_in->buf_id);
            param_out.result = SIMAT_TR_ME_UNABLE;
            param_out.mp_info = SIMAT_MP_SCREEN_BUSY;
            s_mmistk_getinkey_cnf = FALSE;
            s_mmistk_key_is_getting[arg_in_ptr->dual_sys] = FALSE;
            SIMAT_CnfGetInkeyEx((uint8)arg_in_ptr->dual_sys, &param_out);
        }
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnGetInkeyCmd because CanDisplayStk() return FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2827_112_2_18_2_55_57_48,(uint8*)"");
        return;
    }
    s_mmistk_key_is_getting[arg_in_ptr->dual_sys] = TRUE;

    s_mmistk_inkey_is_icon_id = param_in->is_icon_id;
    if (s_mmistk_inkey_is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        s_mmistk_inkey_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_inkey_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
#endif
    }
    s_mmistk_inkey_type = param_in->inkey_type;
    s_mmistk_is_inkey_help_info = param_in->help_info;

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_inkey_type = %d, help_info=%d, is_icon_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2842_112_2_18_2_55_57_49,(uint8*)"ddd", s_mmistk_inkey_type, s_mmistk_is_inkey_help_info, s_mmistk_inkey_is_icon_id);
    // inkey tip
    CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
    s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
    SIMATStr2MMIStr(&(param_in->text_str), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
    //SIMAT_CloseShareBuf(param_in->buf_id);

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_displaydata.wstr_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2849_112_2_18_2_55_57_50,(uint8*)"d", s_mmistk_displaydata.wstr_len);

    CreateSTKBufferMemory(&s_mmistk_inputbuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
    s_mmistk_inputdata.wstr_len = 0;

    s_mmistk_inputdata.wstr_ptr = (wchar *)s_mmistk_inputbuf;
#if defined (MODEM_SUPPORT_SPRD)
    s_mmistk_getinkey_duration_exist = param_in->is_duration_exist;
    s_mmistk_getinkey_duration = param_in->duration;
#endif

    /////////////////////////////
    // Create Window & EditBox
    /////////////////////////////
    MMIDEFAULT_TurnOnBackLight();

    CreateGetInkeyVision(arg_in_ptr->dual_sys);

    if (s_mmistk_getinkey_timer_id != 0)
    {
        MMK_StopTimer(s_mmistk_getinkey_timer_id);
        s_mmistk_getinkey_timer_id = 0;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnGetInkeyCmd, start timer(%d), timer duration = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_2872_112_2_18_2_55_57_51,(uint8*)"dd", s_mmistk_getinkey_timer_id, MMISTK_GETINKEY_DURATION);
}

/********************************************************************************
 NAME:			CreateGetInputVision
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.07
********************************************************************************/
LOCAL void CreateGetInputVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_edit_text = FALSE;
    void* edit_box_data = PNULL;
    BOOLEAN win_exist = FALSE;
    MMI_CONTROL_CREATE_T create = {0};
    GUIEDIT_INIT_DATA_T init_data = {0};
    GUIIM_TYPE_T allow_im = 0;
    GUIIM_TYPE_T init_im = 0;
    MMI_TEXT_ID_T help_text_id = TXT_NULL;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_can_display = FALSE;//(is_icon_id ||self_explan) && decode OK
#endif
    
    win_exist = MMK_IsOpenWin(MMISTK_GETINPUT_WIN_ID);
    if (!win_exist) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_GETINPUT_WIN_ID;
        win_create.func = GetInputWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;
        
        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_GETINPUT_WIN_ID);
#endif

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_GETINPUT_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#else
        if (s_mmistk_is_input_help_info)
        {
            CreatSoftkeyCtrl(MMISTK_GETINPUT_WIN_ID, STXT_OK, TXT_NULL, TXT_HELP);
        }
        else
        {
            CreatSoftkeyCtrl(MMISTK_GETINPUT_WIN_ID, TXT_COMMON_OK, TXT_NULL, TXT_NULL);
        }
#endif
        //set title
        GUIWIN_CreateTitleDyna(MMISTK_GETINPUT_WIN_ID, TXT_NULL);
    }
    else
    {
#ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(MMISTK_GETINPUT_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN,FALSE);
#else
        if (s_mmistk_is_input_help_info)
        {
            GUIWIN_SetSoftkeyTextId(MMISTK_GETINPUT_WIN_ID, STXT_OK, TXT_NULL, TXT_HELP, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(MMISTK_GETINPUT_WIN_ID, TXT_COMMON_OK, TXT_NULL, TXT_NULL, FALSE);
        }
#endif
        MMK_DestroyDynaCtrl(MMISTK_EDITBOX_CTRL_ID);
    }

    if (!s_mmistk_input_is_echo) 
    {
        init_data.type = GUIEDIT_TYPE_PASSWORD;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINPUT_WIN_ID);
        init_data.str_max_len = s_mmistk_input_length.max_len;
    }
    else if (s_mmistk_input_type == SIMAT_INPUT_DIGIT_ONLY) 
    {
        init_data.type = GUIEDIT_TYPE_PHONENUM;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINPUT_WIN_ID);
        init_data.str_max_len = s_mmistk_input_length.max_len;
    }
    else if(s_mmistk_input_type == SIMAT_INPUT_DUAL_DIGIT)//@songbin.zeng cr49315
    {
        init_data.type = GUIEDIT_TYPE_PHONENUM;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINPUT_WIN_ID);
        init_data.str_max_len = s_mmistk_input_length.max_len;
    }
    else if (s_mmistk_input_type == SIMAT_INPUT_SMS_ALPHABET) 
    {
        init_data.type = GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINPUT_WIN_ID);
        init_data.str_max_len = 0;
        is_edit_text = TRUE;

        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
    }
    else 
    {
        init_data.type = GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINPUT_WIN_ID);
        init_data.str_max_len = 0;
        is_edit_text = TRUE;

        allow_im = GUIIM_TYPE_SET_ALL;
        init_im = GUIIM_TYPE_SMART;
    }
    
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
    if(s_mmistk_input_is_icon_id)
    {
        MN_SIM_IMG_T sim_img = {0};
        
        MNSIM_GetImgEx(dual_sys, s_mmistk_input_icon_id.icon_id, &sim_img);
        
        //decode OK
        if(DecodeSimImgToBmp(&sim_img, &(s_icon_bitmap)))
        {
            is_icon_can_display = TRUE;
        }
        else
        {
            is_icon_can_display = FALSE;
        }
    }

    if (is_icon_can_display)
    {
        init_data.both_rect.v_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
        init_data.both_rect.h_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
    }
#endif
#endif
    //bug2107932 watch theme no softkey ,stk win need softkey
    init_data.both_rect.v_rect.bottom = init_data.both_rect.v_rect.bottom - 80/*softkey height*/;

    create.ctrl_id = MMISTK_EDITBOX_CTRL_ID;
    create.guid    = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = MMISTK_GETINPUT_WIN_ID;
    create.init_data_ptr = &init_data;

    edit_box_data = MMK_CreateControl( &create );

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    if (!s_mmistk_input_is_icon_id || !s_mmistk_input_icon_id.is_self_explanatory)
#endif
    {
        GUIWIN_SetTitleText(MMISTK_GETINPUT_WIN_ID, 
                                    s_mmistk_displaydata.wstr_ptr,
                                    s_mmistk_displaydata.wstr_len,
                                    FALSE);
    }

    GUIEDIT_SetHandleRedKey(FALSE, create.ctrl_id);

    if (s_mmistk_is_input_help_info)
    {
        help_text_id = TXT_HELP;
    }
    else
    {
        help_text_id = TXT_NULL;
    }

#ifndef MMI_PDA_SUPPORT
    GUIEDIT_SetSoftkey(MMISTK_EDITBOX_CTRL_ID, 1, 0, help_text_id, TXT_NULL, PNULL);
#endif

    if (!s_mmistk_input_is_echo) 
    {
        GUIEDIT_SetStyle(create.ctrl_id, GUIEDIT_STYLE_MULTI);

        if (SIMAT_INPUT_DIGIT_ONLY == s_mmistk_input_type || SIMAT_INPUT_DUAL_DIGIT == s_mmistk_input_type)
        {
            GUIEDIT_SetPasswordStyle(MMISTK_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_EX);
        }
        else
        {
            allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
            init_im = GUIIM_TYPE_ABC;

            GUIEDIT_SetPasswordStyle(MMISTK_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_ALL);

            //GUIEDIT_SetIm(create.ctrl_id, allow_im, init_im);
            GUIEDIT_SetIm(create.ctrl_id,GUIIM_TYPE_SYS,GUIIM_TYPE_SYS);
        }
    }

    if (is_edit_text)
    {
#if 0//bug1325493
         if(SIMAT_INPUT_DUAL_DIGIT == s_mmistk_input_type || SIMAT_INPUT_UCS2_ALPHABET == s_mmistk_input_type)
         {
             GUIEDIT_SetTextMaxLen(create.ctrl_id, s_mmistk_input_length.max_len/2, s_mmistk_input_length.max_len/2);
	     }
	     else
	     {
	      	 GUIEDIT_SetTextMaxLen(create.ctrl_id, s_mmistk_input_length.max_len, s_mmistk_input_length.max_len);
	     }
#endif
        GUIEDIT_SetTextMaxLen(create.ctrl_id, s_mmistk_input_length.max_len, s_mmistk_input_length.max_len);
        //GUIEDIT_SetIm(create.ctrl_id, allow_im, init_im);
        GUIEDIT_SetIm(create.ctrl_id,GUIIM_TYPE_SYS,GUIIM_TYPE_SYS);
    }

    if (0 != s_mmistk_inputdata.wstr_len)
    {
        GUIEDIT_SetString(create.ctrl_id, s_mmistk_inputdata.wstr_ptr, s_mmistk_inputdata.wstr_len);
    }

    //@gang.tong 05-12-23	Since a design bug that cannot get the parent window's pointer from its children control's pointer in GUI edit_box process,
    //			so, when the children control want to create a timer, it has to get the current process pointer instead of its parent window's pointer.
    //			In this case, the current process pointer is not always the same with the its parent window' pointer, therefore, the MSG_TIMER cant be 
    //			send to its parent msg hand process. One of the the bug's performances is that the cursor in edit box dosent glitter.
    //{
    // because this window maybe same as the last window, in this case the Edit Ctrl must be reactive
    if (win_exist)
    {
        //MMK_WinGrabFocus(MMISTK_GETINPUT_WIN_ID);
        //MMK_SendMsg(MMISTK_GETINPUT_WIN_ID, MSG_GET_FOCUS, PNULL);
        MMK_SetAtvCtrl(MMISTK_GETINPUT_WIN_ID, MMISTK_EDITBOX_CTRL_ID);

        if (!MMK_IsFocusWin(MMISTK_GETINPUT_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_GETINPUT_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_GETINPUT_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    if (s_mmistk_input_is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if (is_icon_can_display)
        {
            GUI_BOTH_RECT_T anim_disp_rect = MMITHEME_GetWinClientBothRect(MMISTK_GETINPUT_WIN_ID)/*MMITHEME_GetWinTitleBothRect(MMISTK_GETINPUT_WIN_ID)*/;

            anim_disp_rect.v_rect.bottom = anim_disp_rect.v_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;
            anim_disp_rect.h_rect.bottom = anim_disp_rect.h_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;

            s_mmistk_icon_display_result = CreateIconAnimCtrl(MMISTK_GETINPUT_WIN_ID, MMISTK_GET_INPUT_ANIM_CTRL_ID, &s_icon_bitmap, &anim_disp_rect);
        }
        else
#endif
        {
            s_mmistk_icon_display_result = FALSE;
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_GETINPUT_WIN);
}
/********************************************************************************
 NAME:			MMISTK_OnGetInputCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnGetInputCmd(void* arg)
{
    SIMAT_GET_INPUT_T* param_in = PNULL;
    SIMAT_GET_INPUT_CNF_T param_out = {0};
    APP_MN_SIMAT_GET_INPUT_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnGetInputCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3111_112_2_18_2_55_58_52,(uint8*)"");
        return;
    }

    arg_in_ptr = (APP_MN_SIMAT_GET_INPUT_IND_T *)arg;
    param_in = (SIMAT_GET_INPUT_T*)(&arg_in_ptr->com);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnGetInputCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3118_112_2_18_2_55_58_53,(uint8*)"");

    CheckCloseDisplayTextIRWin();

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfGetInputEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        param_out.mp_info = SIMAT_MP_BUSY_ON_CALL;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfGetInputEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    // this means can call SIMAT_CnfGetInput() function
    s_mmistk_getinput_cnf = TRUE;	

    if (!CanDisplayStk(arg_in_ptr->dual_sys)) 
    {
        if (s_mmistk_getinput_cnf)
        {
            //SIMAT_CloseShareBuf(param_in->buf_id);
            param_out.result = SIMAT_TR_ME_UNABLE;
            param_out.mp_info = SIMAT_MP_SCREEN_BUSY;
            s_mmistk_getinput_cnf = FALSE;
            s_mmistk_input_is_getting[arg_in_ptr->dual_sys] = FALSE;
            SIMAT_CnfGetInputEx((uint8)arg_in_ptr->dual_sys, &param_out);
        }
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnGetInputCmd because CanDisplayStk() return FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3153_112_2_18_2_55_58_54,(uint8*)"");
        return;
    }
    s_mmistk_input_is_getting[arg_in_ptr->dual_sys] = TRUE;

    s_mmistk_input_is_icon_id = param_in->is_icon_id;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    if (s_mmistk_input_is_icon_id)
    {
        s_mmistk_input_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_input_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }
#endif
    s_mmistk_is_input_help_info = param_in->help_info;
    s_mmistk_input_type = param_in->input_type;
    s_mmistk_input_is_echo = param_in->input_echo;
    s_mmistk_input_length = param_in->response_len;
    s_mmistk_input_is_pack_req = param_in->is_packed;

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_input_type = %d, s_mmistk_input_is_echo = %d, s_mmistk_input_is_pack_req = %d, s_mmistk_is_input_help_info=%d,s_mmistk_input_is_icon_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3170_112_2_18_2_55_58_55,(uint8*)"ddddd",s_mmistk_input_type, s_mmistk_input_is_echo, s_mmistk_input_is_pack_req, s_mmistk_is_input_help_info, s_mmistk_input_is_icon_id);
    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_input_length.min_len = %d, s_mmistk_input_length.max_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3171_112_2_18_2_55_58_56,(uint8*)"dd", s_mmistk_input_length.min_len, s_mmistk_input_length.max_len);
  
    if (s_mmistk_input_length.min_len > s_mmistk_input_length.max_len)
    {
        s_mmistk_input_length.min_len = 0;
    }

    // input tip
    CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));

    s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
    
    //SCI_TRACE_LOW:"[MMI_STK] param_in->text_str.len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3177_112_2_18_2_55_58_57,(uint8*)"d", param_in->text_str.len);
    SIMATStr2MMIStr(&(param_in->text_str), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);

    // default input
    CreateSTKBufferMemory(&s_mmistk_inputbuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
    s_mmistk_inputdata.wstr_len = 0;
    s_mmistk_inputdata.wstr_ptr = (wchar *)s_mmistk_inputbuf;

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_displaydata.length = %d, is_default_text = %d,default_text.len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3186_112_2_18_2_55_58_58,(uint8*)"ddd",s_mmistk_displaydata.wstr_len, param_in->is_default_text, param_in->default_text.len);
    if (param_in->is_default_text) 
    {
        SIMATStr2MMIStr((SIMAT_DO_TEXT_STRING_T*)&(param_in->default_text), &s_mmistk_inputdata, MMISTK_STRING_MAX_LEN);
    }
    //SIMAT_CloseShareBuf(param_in->buf_id);
    /////////////////////////////
    // Create Window & EditBox
    /////////////////////////////
    MMIDEFAULT_TurnOnBackLight();

    CreateGetInputVision(arg_in_ptr->dual_sys);

    if (s_mmistk_getinput_timer_id != 0)
    {
        MMK_StopTimer(s_mmistk_getinput_timer_id);
        s_mmistk_getinput_timer_id = 0;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnGetInputCmd, start timer(%d), timer duration = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3206_112_2_18_2_55_58_59,(uint8*)"dd",s_mmistk_getinput_timer_id, MMISTK_GETINPUT_DURATION);
}

/********************************************************************************
 NAME:			MMISTK_OnMoreTimeCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		guisen
 DATE:			2005.08.31
********************************************************************************/
void MMISTK_OnMoreTimeCmd(void* arg)
{
#if 0
    MMI_STRING_T wait_text = {0};
    
    //SCI_TRACE_LOW:"MMISTK: MMISTK_OnMoreTimeCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3221_112_2_18_2_55_58_60,(uint8*)"");

    //enter waiting win
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);

    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMISTK_MORE_TIME_WIN_ID,IMAGE_NULL,
                                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
#else
    //SCI_TRACE_LOW:"MMISTK: MMISTK_OnMoreTimeCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3229_112_2_18_2_55_58_61,(uint8*)"");
#endif
}

/********************************************************************************
 NAME:			CreateSendSmsVision
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.07
********************************************************************************/
LOCAL void CreateSendSmsVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_open = FALSE;

    if (!MMK_IsOpenWin(MMISTK_SENDSM_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_SENDSM_WIN_ID;
        win_create.func = SendSmsWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;
        
        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_SENDSM_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_SENDSM_WIN_ID, TXT_NULL);

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_SENDSM_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#endif

        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_SENDSM_WIN_ID);
        
        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_SENDSM_WIN_ID;
        create.init_data_ptr = &init_data;
        
        MMK_CreateControl( &create );

        MMK_SetWinDisplayStyleState(MMISTK_SENDSM_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);

        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    SetStkWinTitle(dual_sys, MMISTK_SENDSM_WIN_ID);

#ifndef MMI_PDA_SUPPORT
    GUIWIN_SetSoftkeyTextId(MMISTK_SENDSM_WIN_ID, TXT_NULL, TXT_NULL, TXT_NULL, FALSE);
#endif

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_displaydata.length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3317_112_2_18_2_55_58_62,(uint8*)"d", s_mmistk_displaydata.wstr_len);

    SetRichtextItemInfo(dual_sys,
                                        s_mmistk_sms_is_icon_id,
                                        s_mmistk_sms_icon_id,
                                        s_mmistk_displaydata,
                                        MMISTK_TEXBOX_CTRL_ID);

    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_SENDSM_WIN_ID);

        if (!MMK_IsFocusWin(MMISTK_SENDSM_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_SENDSM_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_SENDSM_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_SENDSMS_WIN);
}

/********************************************************************************
 NAME:			MMISTK_OnSendSmsCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.27
********************************************************************************/
void MMISTK_OnSendSmsCmd(void* arg)
{
    SIMAT_SEND_SM_T* param_in = PNULL;
    ERR_MNSMS_CODE_E mn_result = ERR_MNSMS_NONE;
    SIMAT_SEND_SMS_CNF_T param_out = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE,
                                                                    SIMAT_NP_NO_SPECIFIC_CAUSE, SIMAT_SMSP_NO_SPECIFIC_CAUSE, SIMAT_CS_CONTROL_NO_SPECIFIC_CAUSE};
    APP_MN_SIMAT_SEND_SMS_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSendSmsCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3360_112_2_18_2_55_58_63,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_SEND_SMS_IND_T *)arg;
    param_in = (SIMAT_SEND_SM_T*)(&arg_in_ptr->com);

    CheckCloseDisplayTextIRWin();

    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_alpha_id = %d, param_in->alpha_id.len = %d, param_in->is_packet_IND = %d, param_in->is_address = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3373_112_2_18_2_55_58_64,(uint8*)"dddd", param_in->is_alpha_id,param_in->alpha_id.len,param_in->is_packet_IND,param_in->is_address);
    //SCI_TRACE_LOW:"[MMI_STK] param_in->address.number_type = 0x%02x, param_in->address.number_plan = 0x%02x, param_in->address.number_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3378_112_2_18_2_55_58_65,(uint8*)"ddd", param_in->address.number_type, param_in->address.number_plan, param_in->address.num_len);

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSendSmsEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        param_out.mp_info = SIMAT_MP_BUSY_ON_CALL;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSendSmsEx((uint8)arg_in_ptr->dual_sys, &param_out);

        return;
    }

    if (s_mmistk_sms_is_sending[arg_in_ptr->dual_sys]
        || MMIAPISMS_IsSendingSMS())
    {
        param_out.result = SIMAT_TR_ME_UNABLE;
        param_out.mp_info = SIMAT_MP_SCREEN_BUSY;

        SIMAT_CnfSendSmsEx((uint8)arg_in_ptr->dual_sys, &param_out);

        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendSmsCmd s_mmistk_sendsms_cnf=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3398_112_2_18_2_55_58_66,(uint8*)"d", s_mmistk_sms_is_sending[arg_in_ptr->dual_sys], s_mmistk_sendsms_cnf[arg_in_ptr->dual_sys]);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    if (!MMIAPIPHONE_IsCSAvail(arg_in_ptr->dual_sys))
    {
        param_out.result = SIMAT_TR_ME_UNABLE;
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

        SIMAT_CnfSendSmsEx((uint8)arg_in_ptr->dual_sys, &param_out);
        return;
    }

    // this means can call SIMAT_CnfSendSms() function
    s_mmistk_sendsms_cnf[arg_in_ptr->dual_sys] = TRUE;
    s_mmistk_sms_is_sending[arg_in_ptr->dual_sys] = TRUE;
    /////////////////////////////
    // 		Set Display Data
    /////////////////////////////
    s_mmistk_sms_is_icon_id = param_in->is_icon_id;

    if (s_mmistk_sms_is_icon_id)
    {
        s_mmistk_sms_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_sms_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }

    SIMAT_SetOperSourceEx(arg_in_ptr->dual_sys, SIMAT_OPER_SEND_SMS);

    MMIDEFAULT_TurnOnBackLight();

    //Send SMS Data
    mn_result = MNSMS_SendSmsPDUEx(
                                                            arg_in_ptr->dual_sys, 
                                                            param_in->is_address, 
                                                            &(param_in->address), 
                                                            &(param_in->tpdu), 
                                                            param_in->is_packet_IND, 
                                                            FALSE
                                                            );

    s_mmistk_send_sms = TRUE;

    if (ERR_MNSMS_NONE == mn_result)
    {
        if ((param_in->is_alpha_id && param_in->alpha_id.len > 0) || !param_in->is_alpha_id)
        {
            if (param_in->is_alpha_id) 
            {
                CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
                s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;

                AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
            }
            else 
            {
                MMI_GetLabelTextByLang(TXT_SENDING, &s_mmistk_displaydata);
            }

            // Create Window & TextBox
            if (param_in->is_alpha_id && param_in->alpha_id.len > 0)
            {
                CreateSendSmsVision(arg_in_ptr->dual_sys);
            }
            else
            {
                SCI_TraceLow("MMISTK_OnSendSmsCmd  not display send screen");
            }
        }

        //SCI_TRACE_LOW:"[MMI_STK] MNSMS_SendSTKSmsPDU return MN_RETURN_SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3468_112_2_18_2_55_58_67,(uint8*)"");
    }
    else 
    {
        if (ERR_MNSMS_OUT_OF_RANGE == mn_result)
        {
            param_out.result = SIMAT_TR_BEYOND_ME_CAP;
        }
        else
        {
            param_out.result = SIMAT_TR_ME_UNABLE;
        }
        param_out.mp_info = (SIMAT_ME_PROBLEM_E)mn_result;
        s_mmistk_sendsms_cnf[arg_in_ptr->dual_sys] = FALSE;
        s_mmistk_sms_is_sending[arg_in_ptr->dual_sys] = FALSE;
        SIMAT_CnfSendSmsEx((uint8)arg_in_ptr->dual_sys, &param_out);

        //SCI_TRACE_LOW:"[MMI_STK] MNSMS_SendSTKSmsPDU return MN_RETURN_FAILURE, the code is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3478_112_2_18_2_55_58_68,(uint8*)"d",mn_result);
    }
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSendSmsCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3480_112_2_18_2_55_58_69,(uint8*)"");
}

/********************************************************************************
  NAME:          CreateSetupCallVision
  DESCRIPTION:   
  PARAM IN:      
  PARAM OUT:     None
  AUTHOR:        songbin.zeng
  DATE:          2006.09.27
 ********************************************************************************/
LOCAL void CreateSetupCallVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_open = FALSE;

    if (!MMK_IsOpenWin(MMISTK_SETUP_CALL_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;

        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_SETUP_CALL_WIN_ID;
        win_create.func = SetupCallWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;

        MMK_CreateWindow( &win_create );	

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_SETUP_CALL_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_SETUP_CALL_WIN_ID, TXT_NULL);

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_SETUP_CALL_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#else
        CreatSoftkeyCtrl(MMISTK_SETUP_CALL_WIN_ID, TXT_NULL, TXT_COMMON_OK, TXT_NULL);
#endif
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_SETUP_CALL_WIN_ID);

        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_SETUP_CALL_WIN_ID;
        create.init_data_ptr = &init_data;

        MMK_CreateControl( &create );

        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    SetStkWinTitle(dual_sys, MMISTK_SETUP_CALL_WIN_ID);

#ifndef MMI_PDA_SUPPORT
    GUIWIN_SetSoftkeyTextId(MMISTK_SETUP_CALL_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, FALSE);
#else
    GUIWIN_SetSoftkeyTextId(MMISTK_SETUP_CALL_WIN_ID, TXT_NULL, TXT_COMMON_OK, TXT_NULL, FALSE);
#endif
    
    SetRichtextItemInfo(dual_sys,
                                        s_mmistk_setup_call_is_uc_icon_id,
                                        s_mmistk_setup_call_uc_icon_id,
                                        s_mmistk_displaydata2,
                                        MMISTK_TEXBOX_CTRL_ID);

    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_SETUP_CALL_WIN_ID);

        if (!MMK_IsFocusWin(MMISTK_SETUP_CALL_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_SETUP_CALL_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_SETUP_CALL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_SETUP_CALL_WIN);
}
 
 /**********************************************************************
 NAME:			MMISTK_OnSetupCallCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.28
********************************************************************************/
void MMISTK_OnSetupCallCmd(void* arg)
{
    SIMAT_SETUP_CALL_T* param_in = PNULL;
    SIMAT_SETUP_CALL_CNF_T call_cnf = {0};
    uint8 telnumber_str[MMISTK_TELE_NUM_MAX_LEN + 1] = {0};
    uint8 tel_len = 0;
    APP_MN_SIMAT_SETUP_CALL_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSetupCallCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3610_112_2_18_2_55_59_70,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_SETUP_CALL_IND_T *)arg;
    param_in = (SIMAT_SETUP_CALL_T*)(&arg_in_ptr->com);

    CheckCloseDisplayTextIRWin();

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        call_cnf.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSetupCallEx((uint8)arg_in_ptr->dual_sys, &call_cnf);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSetupCallCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3635_112_2_18_2_55_59_71,(uint8*)"");
    if (0 == param_in->call_parameter.address.num_len) 
    {
        //SIMAT_CloseShareBuf(param_in->buf_id);
        //SIMAT_OpenShareBuf(&call_cnf.buf_id);
        call_cnf.is_command_changed = FALSE;
        call_cnf.result = SIMAT_TR_BEYOND_ME_CAP;
        call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        SIMAT_CnfSetupCallEx((uint8)arg_in_ptr->dual_sys, &call_cnf);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSetupCallCmd because no call address"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3644_112_2_18_2_55_59_72,(uint8*)"");
        return;
    }

    //if there is active call, and call type is SIMAT_STC_FREE,will not execute the command
    //SCI_TRACE_LOW:"[MMI_STK] get make the call type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3649_112_2_18_2_55_59_73,(uint8*)"d",param_in->call_parameter.call_type);

    s_mmistk_setup_call_type = param_in->call_parameter.call_type;
    s_mmistk_call_oper_type = MMISTK_SETUP_CALL_CALL_OPER;

    switch(s_mmistk_setup_call_type)
    {
    case SIMAT_STC_FREE:
    case SIMAT_STC_FREE_REDIAL:
        if (ExistActiveOrSetupCall())
        {
            call_cnf.is_command_changed = FALSE;
            call_cnf.result = SIMAT_TR_ME_UNABLE;
            call_cnf.mp_info = SIMAT_MP_BUSY_ON_CALL;
            SIMAT_CnfSetupCallEx((uint8)arg_in_ptr->dual_sys, &call_cnf);
            //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSetupCallCmd because there is in call already"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3664_112_2_18_2_55_59_74,(uint8*)"");
            return;
        }
        break;

    default:
        break;
    }

    /////////////////////////////
    // 		Set Display Data
    /////////////////////////////
    s_mmistk_setup_call_is_uc_icon_id = param_in->is_uc_icon_id;
    if (s_mmistk_setup_call_is_uc_icon_id)
    {
        s_mmistk_setup_call_uc_icon_id.icon_id = param_in->uc_icon_id.icon_id;
        s_mmistk_setup_call_uc_icon_id.is_self_explanatory = param_in->uc_icon_id.is_self_explanatory;
    }
    s_mmistk_setup_call_is_cs_icon_id = param_in->is_cs_icon_id;
    if (s_mmistk_setup_call_is_cs_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        s_mmistk_setup_call_cs_icon_id.icon_id = param_in->cs_icon_id.icon_id;
        s_mmistk_setup_call_cs_icon_id.is_self_explanatory = param_in->cs_icon_id.is_self_explanatory;
#endif
    }

    CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
    s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
    //s_mmistk_displaydata.is_ucs2 = FALSE;
    s_mmistk_displaydata.wstr_len = 0;
    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_cs_alpha_id = %d, param_in->cs_alpha_id.len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3695_112_2_18_2_55_59_75,(uint8*)"dd", param_in->is_cs_alpha_id, param_in->cs_alpha_id.len);
    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_uc_alpha_id = %d, param_in->uc_alpha_id.len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3696_112_2_18_2_55_59_76,(uint8*)"dd", param_in->is_uc_alpha_id, param_in->uc_alpha_id.len);

    CreateSTKBufferMemory(&s_mmistk_displaybuf2, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));

    //get user confirm alert text
    s_mmistk_displaydata2.wstr_ptr = (wchar*)s_mmistk_displaybuf2;
    s_mmistk_displaydata2.wstr_len = 0;
    if (param_in->is_uc_alpha_id && (param_in->uc_alpha_id.len > 0)) 
    {
        AlphaId2MMIStr(&(param_in->uc_alpha_id), &s_mmistk_displaydata2, MMISTK_STRING_MAX_LEN);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_STK_SETUP_CALL, &s_mmistk_displaydata2);
    }

    s_is_setup_call_duration = param_in->call_parameter.is_duration;

    if(s_is_setup_call_duration)
    {
        s_mmistk_setup_call_duration = param_in->call_parameter.duration;
    }
    else
    {
        s_mmistk_setup_call_duration = 0;
    }

    //SCI_TRACE_LOW:"[MMI_STK] s_is_setup_call_duration=%d,duration=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3723_112_2_18_2_55_59_77,(uint8*)"dd", s_is_setup_call_duration, s_mmistk_setup_call_duration);

    /////////////////////////////
    //       Make a MO Call
    /////////////////////////////
   tel_len = MMIAPICOM_GenDispNumber(param_in->call_parameter.address.number_type,
                                                param_in->call_parameter.address.num_len,
                                                &(param_in->call_parameter.address.party_num[0]),
                                                (uint8*)telnumber_str,
                                                MMISTK_TELE_NUM_MAX_LEN);

    CreateSTKBufferMemory(&s_mmistk_setup_call_str, MMISTK_STRING_MAX_LEN + 1);

    SCI_MEMCPY(s_mmistk_setup_call_str, telnumber_str, tel_len);
    s_mmistk_setup_call_str_len  = tel_len;
    
    //SCI_TRACE_LOW:"[MMI_STK] MMIAPICOM_BcdToStr tel_ptr=%s, tel_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3739_112_2_18_2_55_59_78,(uint8*)"sd", s_mmistk_setup_call_str, s_mmistk_setup_call_str_len);

    if (!MMIAPIPHONE_IsCSAvail(arg_in_ptr->dual_sys) && !MMIAPICC_IsEmergencyNum(s_mmistk_setup_call_str, s_mmistk_setup_call_str_len, arg_in_ptr->dual_sys, FALSE))
    {
        call_cnf.is_command_changed = FALSE;
        call_cnf.result = SIMAT_TR_ME_UNABLE;
        call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

        SIMAT_CnfSetupCallEx((uint8)arg_in_ptr->dual_sys, &call_cnf);
        return;
    }

    //get the call parameter of called parameter sub-address
    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_cps = %d,param_in->cps.len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3752_112_2_18_2_55_59_79,(uint8*)"dd", param_in->call_parameter.is_cps,param_in->call_parameter.cps.addr_len);

    s_is_setup_call_cps = FALSE;
    SCI_MEMSET(&s_mmistk_sub_addr, 0,  sizeof(s_mmistk_sub_addr));

    if (param_in->call_parameter.is_cps) 
    {
        s_is_setup_call_cps = TRUE;

        s_mmistk_sub_addr.addr_len = param_in->call_parameter.cps.addr_len;
        s_mmistk_sub_addr.odd_even_type = param_in->call_parameter.cps.odd_even_type;
        s_mmistk_sub_addr.sub_addr_type = param_in->call_parameter.cps.sub_addr_type;        
        SCI_MEMCPY(s_mmistk_sub_addr.sub_num, param_in->call_parameter.cps.sub_num, param_in->call_parameter.cps.addr_len);
//        call_cnf.result = SIMAT_TR_BEYOND_ME_CAP;
//        call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
//        SIMAT_CnfSetupCallEx((uint8)arg_in_ptr->dual_sys, &call_cnf);
//        return;
    }
    else
    {
        s_is_setup_call_cps = FALSE;
    }

    if (param_in->call_parameter.is_ccp)
    {
        uint8 i=0;
        SCI_TRACE_LOW("[MMI_STK] MMISTK_OnSetupCallCmdcall_parameter.is_ccp =TRUE");
        SCI_MEMSET(&s_mmistk_setup_call_cpp[arg_in_ptr->dual_sys], 0, sizeof(SIMAT_DO_CCP_T));
        s_mmistk_setup_call_cpp[arg_in_ptr->dual_sys].len = param_in->call_parameter.ccp.ccp_ptr[0];
        for(i=1; i<param_in->call_parameter.ccp.len; i++)//[0] is ccp content len; [1~len] is ccp content
        {
            s_mmistk_setup_call_cpp[arg_in_ptr->dual_sys].ccp_ptr[i-1] = param_in->call_parameter.ccp.ccp_ptr[i];
        }
    }
    s_is_mmistk_setup_call = TRUE;

    if ((param_in->is_cs_alpha_id) && (param_in->cs_alpha_id.len > 0) || !param_in->is_cs_alpha_id)
    {
        if (param_in->is_cs_alpha_id)
        {
            AlphaId2MMIStr(&(param_in->cs_alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_CALLING, &s_mmistk_displaydata);
        }
    }

    if ((param_in->is_uc_alpha_id) && (param_in->uc_alpha_id.len > 0) || !param_in->is_uc_alpha_id)
    {
        CreateSetupCallVision(arg_in_ptr->dual_sys);
    }
    else
    {
        StartSetupCallOperation(arg_in_ptr->dual_sys);
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSetupCallCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3799_112_2_18_2_55_59_80,(uint8*)"");
}
/*****************************************************************************/
//  Description : Get Call Param For Set Up Cal
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:s_mmistk_setup_call_cpp
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/

PUBLIC void MMISTK_GetCallCppParamForSetUpCall(MN_DUAL_SYS_E dual_sys, SIMAT_DO_CCP_T *ccp_ptr)
{
    if(MMI_DUAL_SYS_MAX <= dual_sys)//cid611363
    {
        return;
    }
    SCI_MEMCPY(ccp_ptr, &s_mmistk_setup_call_cpp[dual_sys], sizeof(SIMAT_DO_CCP_T));
}
/********************************************************************************
 NAME:			CreatePlayToneVision
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.10.13
********************************************************************************/
LOCAL void CreatePlayToneVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_open = FALSE;
    SIMAT_DO_ICON_ID_T play_tone_icon_id = {0};

    if (!MMK_IsOpenWin(MMISTK_PLAYTONE_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_PLAYTONE_WIN_ID;
        win_create.func = PlayToneWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;

        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_PLAYTONE_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_PLAYTONE_WIN_ID, TXT_NULL);

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_PLAYTONE_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#else
        CreatSoftkeyCtrl(MMISTK_PLAYTONE_WIN_ID, TXT_NULL, TXT_COMMON_OK, TXT_NULL);
#endif
        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_PLAYTONE_WIN_ID);

        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_PLAYTONE_WIN_ID;
        create.init_data_ptr = &init_data;
        
        MMK_CreateControl( &create );

        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    if (PNULL != s_mmistk_toplevel_menu_info[dual_sys]
        && 0 != s_mmistk_toplevel_menu_info[dual_sys]->menu_title_len)
    {
        GUIWIN_SetTitleText(MMISTK_PLAYTONE_WIN_ID, 
                        s_mmistk_toplevel_menu_info[dual_sys]->menu_title, 
                        s_mmistk_toplevel_menu_info[dual_sys]->menu_title_len,
                        FALSE);
    }
    else if (PNULL != s_mmistk_cur_menu_info_ptr[dual_sys])
    {
        GUIWIN_SetTitleText(MMISTK_PLAYTONE_WIN_ID, 
                        s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title, 
                        s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_len,
                        FALSE);
    }

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_displaydata.wstr_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3873_112_2_18_2_55_59_81,(uint8*)"d", s_mmistk_displaydata.wstr_len);

    SetRichtextItemInfo(dual_sys,
                                        FALSE,
                                        play_tone_icon_id,
                                        s_mmistk_displaydata,
                                        MMISTK_TEXBOX_CTRL_ID);

    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_PLAYTONE_WIN_ID);
        if (!MMK_IsFocusWin(MMISTK_PLAYTONE_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_PLAYTONE_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_PLAYTONE_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_PLAYTONE_WIN);
}

/********************************************************************************
 NAME:			GetPlayToneId
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kelly.li
 DATE:			
********************************************************************************/
LOCAL AUD_GENERIC_TONE_ID_E GetPlayToneId(SIMAT_TONE_ID_E tone)
{
    AUD_GENERIC_TONE_ID_E tone_id = AUD_GENERIC_PRESS_KEY_TONE;

    //SCI_TRACE_LOW:"GetPlayToneId: the audio tone is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3909_112_2_18_2_55_59_82,(uint8*)"d",tone);
    switch (tone)
    {
    case SIMAT_DIAL_TONE:
        tone_id = AUD_GENERIC_DIAL_TONE;
        break;

    case SIMAT_CALLED_SUBSCRIBER_BUSY_TONE:
        tone_id = AUD_GENERIC_SUBSCRIBER_BUSY_TONE;
        break;

    case SIMAT_CONGESTION_TONE:
        tone_id = AUD_GENERIC_CONGESTION_TONE;
        break;

    case SIMAT_RADIO_PATH_ACK_TONE:
        tone_id = AUD_GENERIC_SV_RADIO_PATH_ACK_TONE;
        break;

    case SIMAT_RADIO_PATH_NOT_AVAIL_TONE:
        tone_id = AUD_GENERIC_SV_RADIO_PATH_NOT_AVAIL_TONE;
        break;

    case SIMAT_ERROR_SPECIAL_INFO_TONE:
        tone_id = AUD_GENERIC_INFO_FREE_TONE;
        break;

    case SIMAT_CALL_WAITING_TONE:
        tone_id = AUD_GENERIC_SV_CALL_WAITING_TONE;
        break;

    case SIMAT_RINGING_TONE:
        tone_id = AUD_GENERIC_SPECIAL_DIAL_TONE;
        break;

    case SIMAT_GENERAL_BEEP_TONE:
        tone_id = AUD_GENERIC_PRESS_KEY_TONE;
        break;

    case SIMAT_POSITIVE_ACK_TONE:
        tone_id = AUD_GENERIC_SV_ERROR_INFO_TONE;
        break;

    case SIMAT_NEGATIVE_ACK_ERROR_TONE:
        tone_id = AUD_GENERIC_BATT_WARNING_TONE;
        break;

    default:
        tone_id = AUD_GENERIC_WARNING_TONE;
        break;
    }

    return tone_id;
}

/********************************************************************************
 NAME:			MMISTK_OnPlayToneCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnPlayToneCmd(void* arg)
{
    SIMAT_PLAY_TONE_T* param_in = PNULL;
    uint32 duration = 2000;
    uint8 mmi_tone_id = 0;
    APP_MN_SIMAT_PLAY_TONE_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnPlayToneCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_3980_112_2_18_2_55_59_83,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_PLAY_TONE_IND_T *)arg;
    param_in = (SIMAT_PLAY_TONE_T*)(&arg_in_ptr->com);

    CheckCloseDisplayTextIRWin();

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        SIMAT_PLAY_TONE_CNF_T playtone_cnf = {0};

        playtone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        playtone_cnf.result = SIMAT_TR_ME_UNABLE; 
        SIMAT_CnfPlayToneEx(arg_in_ptr->dual_sys, &playtone_cnf);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    // this means can call SIMAT_CnfPlayTone() function
    s_mmistk_playtone_cnf = TRUE;    

    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_alpha_id = %d, param_in->alpha_id.len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4009_112_2_18_2_56_0_84,(uint8*)"dd", param_in->is_alpha_id, param_in->alpha_id.len);
    if ((param_in->is_alpha_id && param_in->alpha_id.len > 0) || !param_in->is_alpha_id)
    {
        if (param_in->is_alpha_id)
        {
            CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
            s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
            //SCI_TRACE_LOW:"[MMI_STK] param_in->alpha_id.len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4016_112_2_18_2_56_0_85,(uint8*)"d", param_in->alpha_id.len);
            AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
        }
        else 
        {
            MMI_GetLabelTextByLang(TXT_VP_PLAYING, &s_mmistk_displaydata);
        }

        /////////////////////////////
        // Create Window & TextBox
        /////////////////////////////
        CreatePlayToneVision(arg_in_ptr->dual_sys);
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnPlayToneCmd, param_in->is_tone_exist = %d, the tone type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4030_112_2_18_2_56_0_86,(uint8*)"dd", param_in->is_tone_exist,param_in->tone);
    //SCI_TRACE_LOW:"[MMI_STK] param_in->is_duration_exist = %d, param_in->duration.duration = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4031_112_2_18_2_56_0_87,(uint8*)"dd", param_in->is_duration_exist, param_in->duration);

    if (param_in->is_duration_exist
        && 0 != param_in->duration)
    {
        duration = param_in->duration;
    }	

    MMIDEFAULT_TurnOnBackLight();

    mmi_tone_id = GetPlayToneId(param_in->tone);
    MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
    MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_GENERIC, mmi_tone_id, duration,MMISRVAUD_TONE_FREQ_DEFAULT);

    if (s_mmistk_playtone_timer_id != 0)
    {
        MMK_StopTimer(s_mmistk_playtone_timer_id);
        s_mmistk_playtone_timer_id = 0;
    }
    s_mmistk_playtone_timer_id = MMK_CreateTimerCallback(duration, MMISTK_HandleSTKTimer, NULL , FALSE);

    //SIMAT_CloseShareBuf(param_in->buf_id);	
    s_mmistk_tone_is_playing[arg_in_ptr->dual_sys] = TRUE;

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnPlayToneCmd, start timer(%d), timer duration = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4053_112_2_18_2_56_0_88,(uint8*)"dd", s_mmistk_playtone_timer_id, duration);
}

/********************************************************************************
 NAME:			MMISTK_OnProvideLICmd
 DESCRIPTION:	when user use the LI(Location Information) function, SIMTK will send
 				this command to MMI, MMI should collect some infomation like:
 				MCC(mobile country code), MNC(mobile network code), LAC(local area code)
 				and IMEI..., etc, then invoke relevant Cnf function to send them to SIMTK,
 				SIMTK will support the LI to user base on these infor.
 				this function don't need user input any data to get LI usually.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnProvideLICmd(void* arg)
{
    SIMAT_LOCAL_INFO_TYPE_E info_type = SIMAT_LI_LOCATION_INFO;
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info = {PLMN_NO_SERVICE};
    SIMAT_LAI_CNF_T lai = {0};
    SIMAT_GET_IMEI_CNF_T imei = {PNULL};
    SIMAT_NMR_BA_CNF_T nmr = {PNULL};
    SIMAT_GET_DT_TZ_CNF_T gdttz = {PNULL};
    SIMAT_GET_LANG_CNF_T glang = {PNULL};
    SIMAT_TIMING_AD_CNF_T ta = {0};
    APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T *arg_in_ptr = PNULL;
#ifdef MMI_DUALMODE_ENABLE
    #ifndef _WIN32
    MN_GMMREG_RAT_E     gmmreg_rat = MN_GMMREG_RAT_GPRS;
    #endif
    SIMAT_ACCESS_CNF_T  access_cnf = {0};/*lint !e64*/
#endif

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnProvideLICmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4088_112_2_18_2_56_0_89,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T *)arg;
    info_type = *((SIMAT_LOCAL_INFO_TYPE_E*)(&arg_in_ptr->com));

    CheckCloseDisplayTextIRWin();

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnProvideLICmd, info_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4096_112_2_18_2_56_0_90,(uint8*)"d", info_type);
    switch (info_type) 
    {
    case SIMAT_LI_LOCATION_INFO: 
        //the location information(MCC, MNC, LACm and Cell identity)
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(arg_in_ptr->dual_sys);
        switch (plmn_info.plmn_status) 
        {
        case PLMN_NO_SERVICE:
            lai.result = SIMAT_TR_ME_UNABLE;
            lai.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;

        case PLMN_EMERGENCY_ONLY:
            lai.result = SIMAT_TR_LIMITED_SERVICE;
            lai.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;

        case PLMN_NORMAL_GSM_ONLY:
        case PLMN_NORMAL_GPRS_ONLY:
        case PLMN_NORMAL_GSM_GPRS_BOTH:
            lai.result = SIMAT_TR_SUCCESS;
            lai.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            lai.lai.lac = plmn_info.lac;
            lai.lai.cell_id = plmn_info.cell_id;
            MMI_MEMCPY(&(lai.lai.mcc_mnc_arr[0]), SIMAT_MCC_MNC_BCD_LEN, &(plmn_info.mcc), 2, 2);
            lai.lai.mcc_mnc_arr[2] = (uint8)plmn_info.mnc;
            break;

        default:
            lai.result = SIMAT_TR_SUCCESS;
            lai.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
        }
        SIMAT_CnfLAIEx((uint8)arg_in_ptr->dual_sys, &lai);
        break;

    case SIMAT_LI_IMEI:  //IMEI of the ME
        if (MNNV_GetIMEIEx(arg_in_ptr->dual_sys, &(imei.imei.imei_arr[0]))) 
        {
            imei.result = SIMAT_TR_SUCCESS;
            imei.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        }
        else 
        {
            imei.result = SIMAT_TR_ME_UNABLE;
            imei.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        }
        SIMAT_CnfGetImeiEx((uint8)arg_in_ptr->dual_sys, &imei);
        break;

    case SIMAT_LI_NMR:   //network measurement results
        nmr.result = SIMAT_TR_BEYOND_ME_CAP;
        nmr.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        SIMAT_CnfNMRBAEx((uint8)arg_in_ptr->dual_sys, &nmr);
        break;

    case SIMAT_LI_DT_TZ: //date,time and time zone
        {
            SCI_TIME_T  tmp_time = {0};
            SCI_DATE_T  tmp_date = {0};

            TM_GetSysDate(&tmp_date);
            TM_GetSysTime(&tmp_time);   

            gdttz.result = SIMAT_TR_SUCCESS;
            gdttz.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            gdttz.dt_tz.dt_tz_arr[0] = (uint8)(tmp_date.year - 2000);
            gdttz.dt_tz.dt_tz_arr[1] = (uint8)tmp_date.mon;
            gdttz.dt_tz.dt_tz_arr[2] = (uint8)tmp_date.mday;
            gdttz.dt_tz.dt_tz_arr[3] = (uint8)tmp_time.hour;
            gdttz.dt_tz.dt_tz_arr[4] = (uint8)tmp_time.min;     /*lint !e123*/
            gdttz.dt_tz.dt_tz_arr[5] = (uint8)tmp_time.sec;
#ifdef WORLD_CLOCK_SUPPORT            
            gdttz.dt_tz.dt_tz_arr[6] = (int8)(MMIAPIACC_GetLocalTimeZone()*4);
#else
            gdttz.dt_tz.dt_tz_arr[6] = 0xFF;
#endif
            SCI_TRACE_LOW("MMISTK_OnProvideLICmd dt_tz_arr[6] =%2x, MMIAPIACC_GetLocalTimeZone() = %.4f", gdttz.dt_tz.dt_tz_arr[6], MMIAPIACC_GetLocalTimeZone());
            
            SIMAT_CnfGetDttzEx((uint8)arg_in_ptr->dual_sys, &gdttz);
        }
        break;

    case SIMAT_LI_LANG:  //language setting
        glang.result = SIMAT_TR_SUCCESS;
        glang.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

        MMISTK_GetStkLang(&glang.lang);

        SIMAT_CnfGetLangEx((uint8)arg_in_ptr->dual_sys, &glang);
        break;

    case SIMAT_LI_TIMING_AD: //timing advance
        {
#if defined (MODEM_SUPPORT_SPRD)
            uint8 cnt=0;
#endif
            ta.result = SIMAT_TR_BEYOND_ME_CAP;
            ta.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
#if defined (MODEM_SUPPORT_SPRD)
         /* fix compile error ,modify by zhigang.peng , 2019-07-15 */
            for(;cnt<MN_SIMAT_MAX_TIMING_ADVANCE_LEN;cnt++)
                ta.timing_ad.timing_ad[cnt] = 0;
#elif defined (MODEM_SUPPORT_RDA)
             ta.timing_ad.timing_ad = 0;
#else
        #error
#endif
            SIMAT_CnfTimeAdvanceEx(arg_in_ptr->dual_sys, &ta);

        }
        break;

    case SIMAT_LI_ACCESS_TECH:
#ifdef MMI_DUALMODE_ENABLE
    #ifndef _WIN32
        gmmreg_rat = MNPHONE_GetCurrentRAT();
//- '00' = GSM;  MN_GMMREG_RAT_GPRS
//- '01' = TIA/EIA-553;
//- '02' = TIA/EIA-136 [25];
//- '03' = UTRAN;  MN_GMMREG_RAT_3G  MN_GMMREG_RAT_TD
//- '04' = TETRA;
//- '05' = TIA/EIA-95;
//- '06' = TIA/EIA/IS-2000;
            if (MN_GMMREG_RAT_GPRS == gmmreg_rat)
            {
                access_cnf.result = SIMAT_TR_SUCCESS;
                access_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
		   /* fix compile error ,modify by zhigang.peng , 2019-07-15 */		
                access_cnf.access_type = MN_ACCESS_RAT_GSM;  // SIMAT_ACCESS_TECHNOLOGY_TYPE_GSM;
            }
            else if (MN_GMMREG_RAT_3G == gmmreg_rat || MN_GMMREG_RAT_TD == gmmreg_rat)
            {
                access_cnf.result = SIMAT_TR_SUCCESS;
                access_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
		   /* fix compile error ,modify by zhigang.peng , 2019-07-15 */
                access_cnf.access_type = MN_ACCESS_RAT_UTRAN;	//SIMAT_ACCESS_TECHNOLOGY_TYPE_UTRAN;
            }
            else
    #endif
            {
                access_cnf.result = SIMAT_TR_BEYOND_ME_CAP;
                access_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }
            //access_cnf
            //SIMAT_CnfAccess(&access_cnf);
#endif
            break;

    case SIMAT_LI_RESERVED:   //reserved
        break;

    default:
        break;
    }
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnProvideLICmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4234_112_2_18_2_56_0_91,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnSendSSCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnSendSSCmd(void* arg)
{
    uint8   bcd_str[MN_MAX_SIMAT_DATA_OBJECT_LEN]    = {0};
    SIMAT_SEND_SS_T *param_in = PNULL;
    SIMAT_SEND_SS_CNF_T ss_cnf = {0};
    ERR_MNSS_CODE_E result = ERR_MNSS_NO_ERR;
    MN_OPERATION_E  phone_oper = MN_SS_OPERATION;
    MN_PARSE_INFO_U ss_parse_info = {0};/*lint !e64*/
    APP_MN_SIMAT_SEND_SS_IND_T *arg_in_ptr = PNULL;

    SCI_MEMSET(&ss_parse_info, 0, sizeof(MN_PARSE_INFO_U));
    
    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSendSSCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4258_112_2_18_2_56_0_92,(uint8*)"");
        return;
    }

    CheckCloseDisplayTextIRWin();
    
    arg_in_ptr = (APP_MN_SIMAT_SEND_SS_IND_T *)arg;
    param_in = (SIMAT_SEND_SS_T*)(&arg_in_ptr->com);
    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSendSSCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4266_112_2_18_2_56_0_93,(uint8*)"");

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        ss_cnf.result = SIMAT_TR_ME_UNABLE; 
        SIMAT_CnfSendSsEx(arg_in_ptr->dual_sys, &ss_cnf);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        ss_cnf.mp_info = SIMAT_MP_BUSY_ON_CALL;
        ss_cnf.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSendSsEx((uint8)arg_in_ptr->dual_sys, &ss_cnf);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    CheckCloseDisplayTextIRWin();

    if (s_mmistk_sendss_waiting_respond)
    {
        //currently busy on SS transaction, return
        ss_cnf.result = SIMAT_TR_ME_UNABLE;
        ss_cnf.mp_info = SIMAT_MP_BUSY_ON_SS;
        SIMAT_CnfSendSsEx(arg_in_ptr->dual_sys, &ss_cnf);
        //SCI_TRACE_LOW:"MMISTK_OnSendSSCmd: currently busy on SS transaction, return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4292_112_2_18_2_56_0_94,(uint8*)"");
        return;
    }

    if (s_mmistk_sendussd_waiting_respond)
    {
        //currently busy on USSD transaction, return
        ss_cnf.result = SIMAT_TR_ME_UNABLE;
        ss_cnf.mp_info = SIMAT_MP_BUSY_ON_USSD;
        SIMAT_CnfSendSsEx(arg_in_ptr->dual_sys, &ss_cnf);
        //SCI_TRACE_LOW:"MMISTK_OnSendSSCmd: currently busy on USSD transaction, return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4302_112_2_18_2_56_0_95,(uint8*)"");
        return;
    }

    if (!MMIAPIPHONE_IsCSAvail(arg_in_ptr->dual_sys))
    {
        ss_cnf.result = SIMAT_TR_ME_UNABLE;
        ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

        SIMAT_CnfSendSsEx((uint8)arg_in_ptr->dual_sys, &ss_cnf);
        return;
    }

    s_mmistk_ss_is_icon_id = param_in->is_icon_id;
    if (s_mmistk_ss_is_icon_id)
    {
        s_mmistk_ss_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_ss_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }

    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, 
                                        (uint8 *)param_in->ss.ss_ptr, 
                                        (param_in->ss.len << 1),    //party_len
                                        (char*)bcd_str);

    //SCI_TRACE_LOW:"[MMI_STK] bcd_str length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4346_112_2_18_2_56_0_96,(uint8*)"d", SCI_STRLEN((char*)bcd_str));

    MMIDEFAULT_TurnOnBackLight();

    phone_oper = MNPHONE_ParseBStringEx(arg_in_ptr->dual_sys, bcd_str, SCI_STRLEN((char*)bcd_str), &ss_parse_info);

    ss_parse_info.ss_parse_info.ss_info_u.ss_basic_info.register_param.forward_num.number_plan = param_in->ss.npi;
    ss_parse_info.ss_parse_info.ss_info_u.ss_basic_info.register_param.forward_num.number_type = param_in->ss.ton;

    if (MN_SS_OPERATION == phone_oper)
    {     
        result = MMISTK_SendReqToSs(
                                                        arg_in_ptr->dual_sys,
                                                        ss_parse_info.ss_parse_info.ss_oper,
                                                        ss_parse_info.ss_parse_info.ss_code,
                                                        ss_parse_info.ss_parse_info.ss_info_u.ss_basic_info.basic_service,
                                                        ss_parse_info.ss_parse_info.ss_info_u.ss_basic_info.register_param,
                                                        param_in->ss
                                                        );
    }
    else if (MN_USSD_OPERATION == phone_oper)
    {
        SIMAT_SetOperSourceEx(arg_in_ptr->dual_sys, SIMAT_OPER_SEND_USSD);

        result = MNSS_UserOriginateUSSDEx(
                                                                    arg_in_ptr->dual_sys,
                                                                    FALSE,//need pack
                                                                    ss_parse_info.ussd_parse_info.data_code_scheme,
                                                                    ss_parse_info.ussd_parse_info.str_len,
                                                                    ss_parse_info.ussd_parse_info.ussd_str
                                                                );
    }
    else
    {
        result = ERR_MNSS_INVALID_SS_CODE;
    }

    //SCI_TRACE_LOW:"[MMI_STK] phone_oper = %d, send ss result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4383_112_2_18_2_56_0_97,(uint8*)"dd", phone_oper,result);

    if(ERR_MNSS_NO_ERR == result
        || MNSS_INFORM_STK == (MNSS_SENDSS_RESULT_E)result)
    {		
        if ((param_in->is_alpha_id && param_in->alpha_id.len > 0) || !param_in->is_alpha_id)
        {
            if (param_in->is_alpha_id)
            {
                CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
                s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
                AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
            }
            else 
            {
                MMI_GetLabelTextByLang(TXT_SENDING, &s_mmistk_displaydata);
            }

            /////////////////////////////
            // Create Window & TextBox
            /////////////////////////////
            CreateSendSSVision(arg_in_ptr->dual_sys);
        }

        s_mmistk_sendss_waiting_respond = TRUE;

        if (MN_USSD_OPERATION == phone_oper)
        {
            s_mmistk_sendussd_waiting_respond = TRUE;
        }
    }
    else
    {		
        ss_cnf.result = SIMAT_TR_SS_ERROR;
        ss_cnf.ss_info = (SIMAT_SS_PROBLEM_E)result;
        SIMAT_CnfSendSsEx(arg_in_ptr->dual_sys, &ss_cnf);

        //SCI_TRACE_LOW:"[MMI_STK] send ss failure, the result is= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4400_112_2_18_2_56_0_98,(uint8*)"d",ss_cnf.ss_info);
    }	

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSendSSCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4403_112_2_18_2_56_0_99,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnSendUSSDCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnSendUSSDCmd(void* arg)
{
    SIMAT_SEND_USSD_T* param_in = PNULL;
    SIMAT_SEND_USSD_CNF_T ussd_cnf = {0};
    ERR_MNSS_CODE_E result = ERR_MNSS_NO_ERR;
    APP_MN_SIMAT_SEND_USSD_IND_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSendUSSDCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4422_112_2_18_2_56_0_100,(uint8*)"");
        return;
    }

    CheckCloseDisplayTextIRWin();

    arg_in_ptr = (APP_MN_SIMAT_SEND_USSD_IND_T *)arg;
    param_in = (SIMAT_SEND_USSD_T*)(&arg_in_ptr->com);
    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSendUSSDCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4430_112_2_18_2_56_0_101,(uint8*)"");

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        ussd_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        ussd_cnf.result = SIMAT_TR_ME_UNABLE; 
        SIMAT_CnfSendUssdEx(arg_in_ptr->dual_sys, &ussd_cnf);

        return;
    }

    if ((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        ussd_cnf.mp_info = SIMAT_MP_BUSY_ON_CALL;
        ussd_cnf.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSendUssdEx((uint8)arg_in_ptr->dual_sys, &ussd_cnf);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    if (s_mmistk_sendss_waiting_respond)
    {
        //currently busy on SS transaction, return
        ussd_cnf.result = SIMAT_TR_ME_UNABLE;
        ussd_cnf.mp_info = SIMAT_MP_BUSY_ON_SS;
        SIMAT_CnfSendUssdEx(arg_in_ptr->dual_sys, &ussd_cnf);
        //SCI_TRACE_LOW:"MMISTK_OnSendUSSDCmd: currently busy on SS transaction, return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4454_112_2_18_2_56_0_102,(uint8*)"");
        return;
    }

    if (s_mmistk_sendussd_waiting_respond)
    {
        //currently busy on USSD transaction, return
        ussd_cnf.result = SIMAT_TR_ME_UNABLE;
        ussd_cnf.mp_info = SIMAT_MP_BUSY_ON_USSD;
        SIMAT_CnfSendUssdEx(arg_in_ptr->dual_sys, &ussd_cnf);
        //SCI_TRACE_LOW:"MMISTK_OnSendUSSDCmd: currently busy on USSD transaction, return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4464_112_2_18_2_56_0_103,(uint8*)"");
        return;
    }

    if (!MMIAPIPHONE_IsCSAvail(arg_in_ptr->dual_sys))
    {
        ussd_cnf.result = SIMAT_TR_ME_UNABLE;
        ussd_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

        SIMAT_CnfSendUssdEx((uint8)arg_in_ptr->dual_sys, &ussd_cnf);
        return;
    }

    s_mmistk_ussd_is_icon_id = param_in->is_icon_id;
    s_mmistk_ss_is_icon_id = s_mmistk_ussd_is_icon_id;
    if (s_mmistk_ss_is_icon_id)
    {
        s_mmistk_ss_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_ss_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }

    //SCI_TRACE_LOW:"[MMI_STK] param_in->ussd.data_code_scheme %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4504_112_2_18_2_56_1_104,(uint8*)"d", param_in->ussd.data_code_scheme);
    //SCI_TRACE_LOW:"[MMI_STK] param_in->ussd.str_len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4505_112_2_18_2_56_1_105,(uint8*)"d", param_in->ussd.str_len);

    SIMAT_SetOperSourceEx(arg_in_ptr->dual_sys, SIMAT_OPER_SEND_USSD);

    //initiate a ussd service
    result = MNSS_UserOriginateUSSDEx(
                                                            arg_in_ptr->dual_sys,
                                                            TRUE,
                                                            param_in->ussd.data_code_scheme,
                                                            param_in->ussd.str_len,
                                                            param_in->ussd.ussd_str
                                                            );	

    if(ERR_MNSS_NO_ERR == result)
    {
        if ((param_in->is_alpha_id && param_in->alpha_id.len > 0)||!param_in->is_alpha_id)
        {
            if (param_in->is_alpha_id)
            {
                CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
                s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
                AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SENDING, &s_mmistk_displaydata);
            }

            /////////////////////////////
            // Create Window & TextBox
            /////////////////////////////
            CreateSendSSVision(arg_in_ptr->dual_sys);
        }

        //HandlePsMsg() will recive ps msg
        s_mmistk_sendussd_waiting_respond = TRUE;
        //SCI_TRACE_LOW:"[MMI_STK] send originate ussd success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4524_112_2_18_2_56_1_106,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"[MMI_STK] send ussd failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4528_112_2_18_2_56_1_107,(uint8*)"");
        ussd_cnf.result = SIMAT_TR_BEYOND_ME_CAP;
        ussd_cnf.mp_info = (SIMAT_ME_PROBLEM_E)result;
        SIMAT_CnfSendUssdEx(arg_in_ptr->dual_sys, &ussd_cnf);
    }	
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSendUSSDCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4533_112_2_18_2_56_1_108,(uint8*)"");
}

/*****************************************************************************/
//  Description : Refresh waiting win
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRefreshWaitingWindow (
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E     msg_id,
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        

    default:
        //result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/********************************************************************************
 NAME:			OnRefreshCnfSimFileChange
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2008.04.24
********************************************************************************/
LOCAL ERR_MNDATAMAG_CODE_E OnRefreshCnfSimFileChange(MN_DUAL_SYS_E dual_sys,SIMAT_REFRESH_RESULT_T *refresh_result)
{
    MN_INI_SIMFILE_E sim_file = MN_SIMFILE_START;
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;

    //SCI_TRACE_LOW:"OnRefreshCnfSimFileChange: dual_sys=%d,is_file_change=%d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4579_112_2_18_2_56_1_109,(uint8*)"dd", dual_sys, refresh_result->is_file_changed);

    if (SIMAT_RF_RESET == s_mmistk_refresh_type[dual_sys])
    {
        MMIAPIPB_ReinitPB(dual_sys);//Bug 1320105

        MMIAPISMS_RefreshSMS(dual_sys);
    }
    else if (refresh_result->is_file_changed)
    {
        for (sim_file = (MN_INI_SIMFILE_E)(MN_SIMFILE_START + 1); sim_file < MN_SIMFILE_END; sim_file++)
        {
            if (refresh_result->flag_arr[sim_file])
            {
                switch (sim_file)
                {
                    case MN_SIMFILE_MSISDN://local number
                        //do nothing
                        break;

                    case MN_SIMFILE_ADN://phone number
                        MMIAPIPB_ResortPB();
                        break;

                    case MN_SIMFILE_FDN://fixed dial number
                        //do nothing
                        break;

                    case MN_SIMFILE_SMSP://short message parameters
                        //do nothing
                        break;

                    case MN_SIMFILE_SMSS://short message state
                        //do nothing
                        break;

                    case MN_SIMFILE_SMS://short message
                        if (!MMIAPISMS_RefreshSMS(dual_sys))
                        {
                            err_code = ERR_MNDATAMAG_RETURN_ERROR;
                        }
                        break;
                    case MN_SIMFILE_SPN:
                        MMIAPIPHONE_SyncSimSPN(dual_sys);
                        break;
                    default:
                        //SCI_TRACE_MID:"OnRefreshCnfSimFileChange:sim_file = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4617_112_2_18_2_56_1_110,(uint8*)"d", sim_file);
                        break;
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OnRefreshCnfSimFileChange: No file change"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4625_112_2_18_2_56_1_111,(uint8*)"");
    }

    return err_code;
}

/********************************************************************************
 NAME:			MMISTK_OnRefreshCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2008.04.24
********************************************************************************/
void MMISTK_OnRefreshCnf(void* arg)
{
    APP_MN_SIMAT_REFRESH_CNF_T * arg_in_ptr = PNULL;
    SIMAT_REFRESH_RESULT_T * refresh_result = PNULL;
    SIMAT_REFRESH_CNF_T prefresh = {0};/*lint !e64*/
#if 0
// no use
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT 
    MMI_STRING_T mmistk_idle_mode_mmistr = {0};
#endif
    arg_in_ptr = (APP_MN_SIMAT_REFRESH_CNF_T *)arg;
    refresh_result = (SIMAT_REFRESH_RESULT_T*)(&arg_in_ptr->result);

    //SCI_TRACE_LOW:"MMISTK_OnRefreshCnf: dual_sys=%d, is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4649_112_2_18_2_56_1_112,(uint8*)"dd", arg_in_ptr->dual_sys, refresh_result->is_ok);

    s_is_stk_refresh = FALSE;

    MMIAPISMS_SetIsSMSRefresh(arg_in_ptr->dual_sys, FALSE);

    if (refresh_result->is_ok)
    {
#if 0
// no use
        err_code = OnRefreshCnfSimFileChange(arg_in_ptr->dual_sys, refresh_result);
#else
        OnRefreshCnfSimFileChange(arg_in_ptr->dual_sys, refresh_result);
#endif

        //alert success
        MMIPUB_CloseAlertWin();
        if(s_mmistk_refresh_is_distext)
        {
            s_mmistk_refresh_is_distext=FALSE;
            if(s_mmistk_refresh_dis_str.wstr_len == 0)
            {
                MMISTK_OpenAlertWin(TXT_SIM_UPDATE_COMPLETE,IMAGE_PUBWIN_SUCCESS,PNULL);
            }
            else
            {
                MMISTK_OpenAlertWinByTextPtr(&s_mmistk_refresh_dis_str);
            }
        }
    }
    else
    {
#if 0
// no use
        err_code = ERR_MNDATAMAG_RETURN_ERROR;
#endif
    
        //alert error
        MMISTK_OpenAlertWin(TXT_ERROR, IMAGE_PUBWIN_WARNING, PNULL);
    }

    //respond refresh cnf
    if (refresh_result->is_ok)
    {
        prefresh.result = SIMAT_TR_SUCCESS;
        prefresh.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    }
    else
    {
        prefresh.result = SIMAT_TR_ME_UNABLE;
        prefresh.mp_info = SIMAT_MP_SCREEN_BUSY;
    }

#if 0 // for bug 1289952 already power on by modem
    if (SIMAT_RF_RESET == s_mmistk_refresh_type[arg_in_ptr->dual_sys]
        && !MMIAPISET_GetFlyMode()
        && (e_dualsys_setting & (0x01 << arg_in_ptr->dual_sys)))
    {
        MMIAPIPHONE_PowerOnPs(arg_in_ptr->dual_sys);
    }
    else
#endif
    {
        SIMAT_CnfRefreshEx(arg_in_ptr->dual_sys, &prefresh);
    }

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    if (s_idle_icon_bmp.bit_ptr != NULL)
    {
        SCI_FREE(s_idle_icon_bmp.bit_ptr);
        SCI_MEMSET(&s_idle_icon_bmp, 0x00, sizeof(s_idle_icon_bmp));
    }
#endif

#ifdef MMI_IDLE_STKTEXT_SUPPORT 
    MMIAPIIDLE_MAIN_SetIdleModeText(arg_in_ptr->dual_sys, &mmistk_idle_mode_mmistr);
#endif
    //close wating win
    MMK_CloseWin(MMISTK_REFRESH_WAITING_WIN_ID);
}
/*****************************************************************************/
//  Description : parse raw data ,get string to display
//  Paramter:
//          [In]:raw_data,from modem
//          [Out]:None
//          [Return]:TRUE OR FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OnRefreshCmdIsExitDisplayText(SIMAT_RAW_APDU_T* raw_data)
{
   uint8 command_len=0;
   uint8 i=0;
   uint8 display_text_len=0;
   BOOLEAN result=FALSE;
   SIMAT_DO_TEXT_STRING_T simat_str={0};
   if(raw_data==PNULL)
   {
       SCI_TRACE_LOW("OnRefreshCmdIsExitDisplayText:raw_data is null");
       return FALSE;
   }
   command_len = raw_data->data_len;
   for(i = 2; i < command_len; i++)//tag len xx xx ... ts 10223V140000 6.6.13
   {
        if(raw_data->data_arr[i] == 0x85)//texttag + strlen + str(such as "85 07 53 65 6E 64 20 53 4D ",Send SM)
        {
           if((i+1) >= command_len)
           {
                SCI_TRACE_LOW("OnRefreshCmdIsExitDisplayText:raw_data is invalid");
                return FALSE;
           }
           if(raw_data->data_arr[i+1] == 0x00)//1326494
           {
                SCI_TRACE_LOW("OnRefreshCmdIsExitDisplayText:no tip");
                return FALSE;/*if the alpha identifier is provided by the UICC and is a null data object (i.e. length = '00' and no value part),
                               this is an indication that the terminal should not give any information to the user on the fact that the terminal is
                               performing the refresh command;*/
           }
           if(raw_data->data_arr[i+1] > 0x80)//1326463
           {
                i=i+1;//if raw len > 0x80 ,len need 2 byte expression(0x81+actual len),   actual len is raw_data->data_arr[i+2]
                if((i+1) >= command_len)
                {
                    SCI_TRACE_LOW("OnRefreshCmdIsExitDisplayText:raw_data is invalid");
                    return FALSE;
                }
           }
           display_text_len = raw_data->data_arr[i+1];
           if((display_text_len + i + 1) > command_len)
           {
                SCI_TRACE_LOW("OnRefreshCmdIsExitDisplayText:raw_data is invalid");
                return FALSE;
           }
           SCI_MEMCPY(&simat_str.text_ptr, &raw_data->data_arr[i+2], display_text_len);
           simat_str.len = display_text_len;
           simat_str.dcs = MMI_DCS_8_BIT_ALPHABET;
           CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
           s_mmistk_refresh_dis_str.wstr_ptr = (wchar*)s_mmistk_displaybuf;

           SIMATStr2MMIStr(&simat_str, &s_mmistk_refresh_dis_str, MMISTK_STRING_MAX_LEN);

           result=TRUE;

        }
   }
   if(i>=command_len)
   {
       result=TRUE;/*if the alpha identifier is not provided by the UICC, the terminal may give information to the user concerning
                     what is happening.*/
   }
   SCI_TRACE_LOW("OnRefreshCmdIsExitDisplayText result is %d",result);
   return result;
}
/********************************************************************************
 NAME:			MMISTK_OnRefreshCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnRefreshCmd(void* arg)
{
    SIMAT_REFRESH_T* param_in = PNULL;
    APP_MN_SIMAT_REFRESH_IND_T *arg_in_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    SIMAT_REFRESH_CNF_T prefresh = {0};/*lint !e64*/
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;
    SIMAT_RAW_APDU_T*  raw_data=PNULL;
    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnRefreshCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4715_112_2_18_2_56_1_113,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_REFRESH_IND_T *)arg;
    dual_sys = arg_in_ptr->dual_sys;
    param_in = (SIMAT_REFRESH_T*)(&arg_in_ptr->com);
    s_mmistk_refresh_type[dual_sys] = param_in->refresh_type;
    /*for bug 1300675*/
    raw_data =(SIMAT_RAW_APDU_T*)(&arg_in_ptr->raw_apdu);
    if(TRUE==OnRefreshCmdIsExitDisplayText(raw_data))
    {
        s_mmistk_refresh_is_distext=TRUE;
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnRefreshCmd, refresh_type = %d,dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4722_112_2_18_2_56_1_114,(uint8*)"dd", param_in->refresh_type, dual_sys);

    CheckCloseDisplayTextIRWin();

    switch(param_in->refresh_type)
    {
        case SIMAT_RF_FILE_CHANGE:
        case SIMAT_RF_INITIAL:
        case SIMAT_RF_INITIAL_FULL_FILE_CHANGE:
        case SIMAT_RF_INITIAL_FILE_CHANGE:
        case SIMAT_RF_3G_APPLICATIN_RESET:
        case SIMAT_RF_3G_SESSION_RESET:
        //case SIMAT_RF_STEER_OF_ROAMING: //for bug 1291065
            if (MMIAPIPB_IsPbReady()
                && MMIAPISMS_IsOrderOk())
            {
                err_code = MNSIM_RefreshEx(dual_sys, *param_in);

                if (ERR_MNDATAMAG_NO_ERR == err_code)
                {
                    MMI_STRING_T wait_text = {0};
                    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
                    s_is_stk_refresh = TRUE;
                    MMIAPISMS_SetIsSMSRefresh(dual_sys, TRUE);
                    
                    //enter waiting win
                    MMI_GetLabelTextByLang(TXT_SIM_UPDATING, &wait_text);

                    WatchCOM_WaitingWin_1Line_Enter(MMISTK_REFRESH_WAITING_WIN_ID, &wait_text, 0, softKey, HandleRefreshWaitingWindow);

                    MMK_SetWinDisplayStyleState(MMISTK_REFRESH_WAITING_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
                    // if (SIMAT_RF_RESET == param_in->refresh_type)//for bug 1289952 already power off by modem
                }
                else
                {
                    prefresh.result = SIMAT_TR_ME_UNABLE;
                    prefresh.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                    SIMAT_CnfRefreshEx(dual_sys, &prefresh);

                    //alert fail
                    MMISTK_OpenAlertWin(TXT_ERROR, IMAGE_PUBWIN_WARNING, PNULL);
               }
            }
            else
            {
                //Not allow do STK Refresh
                prefresh.result = SIMAT_TR_ME_UNABLE;
                prefresh.mp_info = SIMAT_MP_SCREEN_BUSY;
                SIMAT_CnfRefreshEx(dual_sys, &prefresh);

                //alert wating
                MMISTK_OpenAlertWin(TXT_COMMON_WAITING, IMAGE_PUBWIN_WAIT, PNULL);
                break;
            }
            break;

        case SIMAT_RF_RESET://1343185,1334476   ts102 223 6.4.7
             {
                err_code = MNSIM_RefreshEx(dual_sys, *param_in);

                if (ERR_MNDATAMAG_NO_ERR == err_code)
                {
                    MMI_STRING_T wait_text = {0};
                    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
                    s_is_stk_refresh = TRUE;
                    MMIAPISMS_SetIsSMSRefresh(dual_sys, TRUE);

                    //enter waiting win 
                    MMI_GetLabelTextByLang(TXT_SIM_UPDATING, &wait_text);

                    WatchCOM_WaitingWin_1Line_Enter(MMISTK_REFRESH_WAITING_WIN_ID, &wait_text, 0, softKey, HandleRefreshWaitingWindow);
                    MMK_SetWinDisplayStyleState(MMISTK_REFRESH_WAITING_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
                }
            }
            break;

        case SIMAT_RF_RESERVED:
            break;

        default:
            //SCI_TRACE_LOW:"refresh type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4791_112_2_18_2_56_1_115,(uint8*)"d", param_in->refresh_type);
            break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnRefreshCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4795_112_2_18_2_56_1_116,(uint8*)"");
}


/********************************************************************************
 NAME:			MMISTK_OnSetupEventlistCmd
 DESCRIPTION:	some SIMTK will be care of some things of MMI layer, for example: call, SMS .. etc.
 				SIMTK can inform MMI that which he will care by send this cmd. after received this cmd,
 				MMI can copy them in local, and when some case(SIMTK care) occur in MMI layer,
 				MMI can invoke the Ind function like SIMAT_IndMTCall(), SIMAT_IndMOCC(),.. etc to
 				inform SIMTK layer, SIMTK will do some things base on this action, ep: saved the last call number.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSetupEventlistCmd(void* arg)
{
    SIMAT_SETUP_EVENT_LIST_T* param_in = PNULL;
    SIMAT_SETUP_EVENT_LIST_CNF_T event_cnf = {SIMAT_TR_SUCCESS};
    int32 i = 0;
    SIMAT_SIG_SETUP_EVENT_LIST_REQ_T *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSetupEventlistCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4819_112_2_18_2_56_1_117,(uint8*)"");
        return;
    }
    arg_in_ptr = (SIMAT_SIG_SETUP_EVENT_LIST_REQ_T *)arg;
    param_in = (SIMAT_SETUP_EVENT_LIST_T*)(&arg_in_ptr->com);
    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSetupEventlistCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4824_112_2_18_2_56_1_118,(uint8*)"");
    SCI_MEMSET(s_mmistk_eventlist, 0, (sizeof(BOOLEAN) * SIMAT_EVENT_REVERSED));
    s_mmistk_eventlist_amount = param_in->event_list.num;
    for (i = 0; i < s_mmistk_eventlist_amount; i++) 
    {
        s_mmistk_eventlist[param_in->event_list.events_arr[i]] = TRUE;
    }
    event_cnf.result = SIMAT_TR_SUCCESS;
    event_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    SIMAT_CnfSetEventListEx(arg_in_ptr->dual_sys, &event_cnf);
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSetupEventlistCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4834_112_2_18_2_56_1_119,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnSetupIdletextCmd
 DESCRIPTION:	SIMTK can support a text to MMIi layer,
 				MMI can use it to show in idle screen if need.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSetupIdletextCmd(void* arg)
{
    SIMAT_SETUP_IDLE_TEXT_T* param_in = PNULL;
    SIMAT_SETUP_IDLE_TEXT_CNF_T idletext_cnf = {SIMAT_TR_SUCCESS};
    APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T *arg_in_ptr = PNULL;
    wchar idle_mode_str[MMISTK_STRING_MAX_LEN + 1] = {0};
    MMI_STRING_T mmistk_idle_mode_mmistr = {0};

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSetupIdletextCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4855_112_2_18_2_56_1_120,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T *)arg;
    param_in = (SIMAT_SETUP_IDLE_TEXT_T*)(&arg_in_ptr->com);
    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSetupIdletextCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4860_112_2_18_2_56_1_121,(uint8*)"");
    //SIMAT_CloseShareBuf(param_in->buf_id);
    SCI_MEMSET(&mmistk_idle_mode_mmistr, 0, sizeof(mmistk_idle_mode_mmistr));
    SCI_MEMSET(idle_mode_str, 0 , sizeof(idle_mode_str));
    mmistk_idle_mode_mmistr.wstr_ptr = (wchar*)idle_mode_str;

    CheckCloseDisplayTextIRWin();

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    if (s_idle_icon_bmp.bit_ptr != NULL)
    {
        SCI_FREE(s_idle_icon_bmp.bit_ptr);
        SCI_MEMSET(&s_idle_icon_bmp, 0x00, sizeof(s_idle_icon_bmp));
    }
#endif

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    s_mmistk_idle_text_is_icon_id = param_in->is_icon_id;
#endif

    if (param_in->is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        s_mmistk_idle_text_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_idle_text_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
#endif
    }

    //If the "Text string" is a null data object (i.e. length = '00' and no value part),
    //the ME shall remove the existing idle mode text in the ME
    if (0 != param_in->text_string.len || (0 == param_in->text_string.len &&!param_in->is_icon_id))
    {
        //if (!param_in->is_icon_id || !param_in->icon_id.is_self_explanatory)
        {
            SIMATStr2MMIStr(&(param_in->text_string), &mmistk_idle_mode_mmistr, MMISTK_STRING_MAX_LEN);
#ifdef MMI_IDLE_STKTEXT_SUPPORT 
            MMIAPIIDLE_MAIN_SetIdleModeText(arg_in_ptr->dual_sys, &mmistk_idle_mode_mmistr);
#endif
        }

        if (param_in->is_icon_id)
        {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
            MN_SIM_IMG_T sim_img = {0};
            MNSIM_GetImgEx(arg_in_ptr->dual_sys, param_in->icon_id.icon_id, &sim_img);

            if (DecodeSimImgToBmp(&sim_img, &s_idle_icon_bmp))
            {
                s_mmistk_icon_display_result = TRUE;
            }
            else
#endif
#endif
            {
                s_mmistk_icon_display_result = FALSE;
            }
            
            idletext_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
        }
        else
        {
            idletext_cnf.result = SIMAT_TR_SUCCESS;
        }
        idletext_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    }
    else
    {
        //ME receives an icon and either an empty, or no, alpha identifier/text string is given by the SIM
        //shall reject the command with general result "Command data not understood by ME"
        idletext_cnf.result = SIMAT_TR_DATA_NOT_UNDERSTOOD;
        idletext_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    }

    SIMAT_CnfSetIdleTextEx(arg_in_ptr->dual_sys, &idletext_cnf);
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSetupIdletextCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4929_112_2_18_2_56_1_122,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnSendDtmfCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSendDtmfCmd(void* arg)
{
    SIMAT_SEND_DTMF_T* param_in = PNULL;
    APP_MN_SIMAT_SEND_DTMF_IND_T *arg_in_ptr = PNULL;
    MMI_WIN_ID_T send_dtmf_win = MMISTK_SEND_DTMF_WIN_ID;
    uint32 send_dtmf_win_time = 0;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSendDtmfCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4948_112_2_18_2_56_1_123,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_SEND_DTMF_IND_T*)arg;
    param_in = (SIMAT_SEND_DTMF_T*)(&arg_in_ptr->com);

    CheckCloseDisplayTextIRWin();

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        SIMAT_SEND_DTMF_CNF_T dtmf_cnf = {0};

        dtmf_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        dtmf_cnf.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfSendDtmfEx(arg_in_ptr->dual_sys, &dtmf_cnf);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(arg_in_ptr->dual_sys);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSendDtmfCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4975_112_2_18_2_56_1_124,(uint8*)"");

    //SCI_TRACE_LOW:"[MMI_STK]OnSendDtmfCmd: param_in->is_alpha_id = %d,param_in->alpha_id.len = %d,is_icon_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4977_112_2_18_2_56_1_125,(uint8*)"ddd",param_in->is_alpha_id,param_in->alpha_id.len,param_in->is_icon_id);

    s_mmistk_dtmf_is_icon_id = param_in->is_icon_id;
    if (s_mmistk_dtmf_is_icon_id)
    {
        s_mmistk_dtmf_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_dtmf_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }

    SCI_MEMSET(&s_mmistk_displaydata, 0x00, sizeof(s_mmistk_displaydata));
    if ((param_in->is_alpha_id && (param_in->alpha_id.len != 0)) || !param_in->is_alpha_id) //fix bug678211
    {
        if(param_in->is_alpha_id)
        {
	        CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
	        s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
	        AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_STK_SENDING_DTMF, &s_mmistk_displaydata);
        }
    }

    CreateSTKBufferMemory(&s_mmistk_dtmf_string, (MN_MAX_SIMAT_DATA_OBJECT_LEN + 1)*2);
    s_mmistk_dtmf_len = 0;
    s_mmistk_dtmf_index = 0;
    s_is_mmi_dtmf_pause = FALSE;

    //SCI_TRACE_LOW:"[MMI_STK]OnSendDtmfCmd: param_in->dtmf.len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_4999_112_2_18_2_56_2_126,(uint8*)"d",param_in->dtmf.len);
    SCI_TRACE_BUF("[MMI_STK]OnSendDtmfCmd: The dtmf is",param_in->dtmf.dtmf,sizeof(param_in->dtmf.dtmf));

    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, 
                                        (uint8 *)param_in->dtmf.dtmf, 
                                        param_in->dtmf.len<<1,  //party_len
                                        (char*)s_mmistk_dtmf_string);

    s_mmistk_dtmf_len = SCI_STRLEN((char*)s_mmistk_dtmf_string);
    //SCI_TRACE_LOW:"[MMI_STK]OnSendDtmfCmd: s_mmistk_dtmf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5008_112_2_18_2_56_2_127,(uint8*)"d",s_mmistk_dtmf_len);
    
    if (0 != s_mmistk_displaydata.wstr_len)
    {
        uint32 i = 0;
        MMI_IMAGE_ID_T img_id = s_mmistk_dtmf_is_icon_id ? IMAGE_NULL : IMAGE_PUBWIN_WAIT;

        for (i = 0; i < s_mmistk_dtmf_len; i++)
        {
            if ('p' == s_mmistk_dtmf_string[i] || 'P' == s_mmistk_dtmf_string[i])
            {
                s_is_mmi_dtmf_pause = TRUE;
                break;
            }
        }

        //SCI_TRACE_LOW:"[MMI_STK]OnSendDtmfCmd: s_is_mmi_dtmf_pause=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5024_112_2_18_2_56_2_128,(uint8*)"d", s_is_mmi_dtmf_pause);

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if (s_mmistk_dtmf_is_icon_id && s_mmistk_dtmf_icon_id.is_self_explanatory)
        {
            if (s_is_mmi_dtmf_pause)
            {
                MMIPUB_OpenAlertWinByTextPtr(&send_dtmf_win_time, PNULL, PNULL, img_id, &send_dtmf_win, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSendDtmfWin);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextPtr(PNULL, PNULL, PNULL, img_id, &send_dtmf_win, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSendDtmfWin);

                MMIPUB_SetWinTitleTextId(send_dtmf_win, TXT_COMMON_PROMPT, FALSE);
            }
        }
        else
#endif
        {
            if (s_is_mmi_dtmf_pause)
            {
                MMIPUB_OpenAlertWinByTextPtr(&send_dtmf_win_time, &s_mmistk_displaydata, PNULL, img_id, &send_dtmf_win, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSendDtmfWin);
            }
            else
            {

                MMIPUB_OpenAlertWinByTextPtr(PNULL, &s_mmistk_displaydata, PNULL, img_id, &send_dtmf_win, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSendDtmfWin);

                MMIPUB_SetWinTitleTextId(send_dtmf_win, TXT_COMMON_PROMPT, FALSE);
            }
        }

        if (s_mmistk_dtmf_is_icon_id)
        {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
            MN_SIM_IMG_T sim_img = {0};

            MNSIM_GetImgEx(arg_in_ptr->dual_sys, s_mmistk_dtmf_icon_id.icon_id, &sim_img);
            s_mmistk_icon_display_result = DecodeSimImgToBmp(&sim_img, &s_mmistk_dtmf_icon_bmp);
#endif
#else
            s_mmistk_icon_display_result = FALSE;
#endif
        }
    }

    HandleSTKSendDtmfStr(arg_in_ptr->dual_sys);

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSendDtmfCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5064_112_2_18_2_56_2_129,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnLangNotifyCmd
 DESCRIPTION:	some SIMTK will send this cmd to notify MMI that 
				the following text from SIMTK will be in which language.
 				!!!!! note:  this cmd need not MMI send Cnf to SIMTK,
 				!!!!! the Cnf was done by SIMTK layer self.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnLangNotifyCmd(void* arg)
{
    SIMAT_LANG_NOTIFICATION_T* param_in= PNULL;
    APP_MN_SIMAT_LANG_NOTIFY_IND_T *arg_in_ptr = PNULL;
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
    uint32 i = 0;
    uint16 lang_noti_num = sizeof(s_lang_noti)/sizeof(s_lang_noti[0]);

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnLangNotifyCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5087_112_2_18_2_56_2_130,(uint8*)"");
        return;
    }

    arg_in_ptr = (APP_MN_SIMAT_LANG_NOTIFY_IND_T *)arg;
    param_in = (SIMAT_LANG_NOTIFICATION_T *)(&arg_in_ptr->com);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnLangNotifyCmd specific_lang_noti=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5094_112_2_18_2_56_2_131,(uint8*)"d", param_in->specific_lang_noti);

    CheckCloseDisplayTextIRWin();

    MMIAPISET_GetLanguageType(&language_type);

    if (param_in->specific_lang_noti)
    {
        for (i = 0; i < lang_noti_num; i++)
        {
            if (0 == strnicmp((char *)param_in->lang.lang, (char *)s_lang_noti[i].lang_noti, MN_SIMAT_LANGE_LEN))
            {
                if (((MMISET_LANGUAGE_TYPE_E)s_lang_noti[i].lang_type != language_type)
                    && (MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)s_lang_noti[i].lang_type)))
                {
                    MMIAPISET_SetLanguageType((MMISET_LANGUAGE_TYPE_E)s_lang_noti[i].lang_type);

                    MMISTK_OpenAlertWin(TXT_STK_LANG_NOTI, IMAGE_PUBWIN_WARNING, PNULL);
                }
                break;
            }
        }

        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnLangNotifyCmd lang=%s, language_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5117_112_2_18_2_56_2_132,(uint8*)"sd", (char *)param_in->lang.lang, language_type);
    }
    
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnLangNotifyCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5120_112_2_18_2_56_2_133,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnCcToMmiCmd
 DESCRIPTION:	.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2010.10.05
********************************************************************************/
void MMISTK_OnCcToMmiCmd(void* arg)
{
    SIMAT_CC_RSP_T* param_in = PNULL;
    APP_MN_SIMAT_CC_TO_MMI_IND_T *arg_in_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    BOOLEAN is_cc_clt_success = FALSE;
    MMI_STRING_T alert_string = {0};
    MMI_STRING_T display_string = {0};
    wchar displaybuf[MMISTK_STRING_MAX_LEN + 1] = {0};
    SIMAT_SIM_CONTROL_RESULT_E result = SIMAT_SIMC_ALLOWED;
    MMI_TEXT_ID_T cc_text_id = TXT_NULL;
    BOOLEAN is_need_modified = TRUE;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnCcToMmiCmd arg is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5145_112_2_18_2_56_2_134,(uint8*)"");
        return;
    }
    
    arg_in_ptr = (APP_MN_SIMAT_CC_TO_MMI_IND_T *)arg;
    param_in = (SIMAT_CC_RSP_T *)(&arg_in_ptr->com);
    dual_sys = arg_in_ptr->dual_sys;
    result = param_in->result;

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnCcToMmiCmd dual_sys=%d, is_simc_data=%d, is_ccp1=%d, is_ccp2=%d, is_cps=%d, is_alpha_id=%d, len=%d, result=%d, data_type=%d, num_len=%d, data_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5156_112_2_18_2_56_2_135,(uint8*)"ddddddddddd",dual_sys, param_in->is_simc_data, param_in->is_ccp1, param_in->is_ccp2, param_in->is_cps, param_in->is_alpha_id, param_in->alpha_id.len, param_in->result,  param_in->data_type, param_in->simc_data.add.num_len, param_in->data_type);

    switch (result)
    {
    case SIMAT_SIMC_ALLOWED:
        //cc_text_id = TXT_STK_CALL_ALLOWED; 
        /*do not prompt*/
        break;

    case SIMAT_SIMC_NOT_ALLOWED:
        cc_text_id = TXT_COMM_NOT_ALLOWED; 
        break;
        

    case SIMAT_SIMC_ALLOWED_AND_MODIFIED:
        cc_text_id = TXT_STK_CALL_MODIFIED;
        break;

    default:
        cc_text_id = TXT_NULL;
        break;
    }

    if (!param_in->is_alpha_id)//alpha no present
    {
        MMI_GetLabelTextByLang(cc_text_id, &alert_string);
    }
    
    else
    {
        if (0 != param_in->alpha_id.len)//alpha id present
        {
            SCI_MEMSET(displaybuf, 0, sizeof(displaybuf));

            display_string.wstr_ptr = (wchar*)displaybuf;

            AlphaId2MMIStr(&(param_in->alpha_id), &display_string, MMISTK_STRING_MAX_LEN);

            if (SIMAT_SIMC_ALLOWED_AND_MODIFIED == result)//call modified,alpha id exit and can't fix number
            {
                is_need_modified = FALSE;
            }
        }
        else
        {
            is_need_modified = FALSE;
        }
    }

    //if (param_in->is_simc_data && param_in->simc_data.add.num_len > 0)
    {
        is_cc_clt_success = MMIAPICC_ResponseCallCtl(dual_sys, &alert_string, &display_string, &param_in->simc_data.add, result, param_in->data_type, is_need_modified);
    }

    //SCI_TRACE_LOW:"[MMI_STK] Leave MMISTK_OnCcToMmiCmd is_cc_clt_success=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5210_112_2_18_2_56_2_136,(uint8*)"d", is_cc_clt_success);
}

/********************************************************************************
 NAME:			MMISTK_OnSSControlRspCmd
 DESCRIPTION:	if you want SIMTK to affect the supply service by SIMTK, you can 
 				invoke Ind function to notify SIMTK layer before a SS code be
 				sent, then SIMTK will send this cmd to response MMI that 
 				the SS what should to do.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSSControlRspCmd(void* arg)
{
    SIMAT_SS_CONTROL_RSP_T  *param_in = PNULL;
    SIMAT_SIG_MO_SS_CONTROL_RSP_T    *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSSControlRspCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5230_112_2_18_2_56_2_137,(uint8*)"");
        return;
    }
    arg_in_ptr = (SIMAT_SIG_MO_SS_CONTROL_RSP_T *)arg;
    param_in = (SIMAT_SS_CONTROL_RSP_T*)(&arg_in_ptr->ss_rsp);
    //SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnSSControlRspCmd");
    SIMAT_CloseShareBufEx(arg_in_ptr->dual_sys, param_in->buf_id);
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSSControlRspCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5237_112_2_18_2_56_2_138,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnUSSDControlRspCmd
 DESCRIPTION:	if you want SIMTK to affect the unstructed supply service data by SIMTK,
 				you can invoke Ind function to notify SIMTK layer before a USSD be
 				sent, then SIMTK will send this cmd to response MMI that 
 				the USSD what should to do.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnUSSDControlRspCmd(void* arg)
{
    SIMAT_USSD_CONTROL_RSP_T*param_in = PNULL;
    SIMAT_SIG_MO_USSD_CONTROL_RSP_T  *arg_in_ptr = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnUSSDControlRspCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5257_112_2_18_2_56_2_139,(uint8*)"");
        return;
    }
    arg_in_ptr = (SIMAT_SIG_MO_USSD_CONTROL_RSP_T *)arg;
    param_in = (SIMAT_USSD_CONTROL_RSP_T*)(&arg_in_ptr->ussd_rsp);
    //SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnUSSDControlRspCmd");
    SIMAT_CloseShareBufEx(arg_in_ptr->dual_sys, param_in->buf_id);
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnUSSDControlRspCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5264_112_2_18_2_56_2_140,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnSmsCtlToMmiCmd
 DESCRIPTION:	if you want SIMTK to affect the SMS by SIMTK,
 				you can invoke Ind function to notify SIMTK layer before a SMS be
 				sent, then SIMTK will send this cmd to response MMI that 
 				the SMS what should to do.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2010.10.05
********************************************************************************/
void MMISTK_OnSmsCtlToMmiCmd(void* arg)
{
    SIMAT_SMS_CTL_TO_MMI_IND_T* param_in = PNULL;
    APP_MN_SIMAT_SMS_CTL_TO_MMI_IND_T *arg_in_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_STRING_T alert_string = {0};
    MMI_STRING_T display_string = {0};
    wchar displaybuf[MMISTK_STRING_MAX_LEN + 1] = {0};
    BOOLEAN is_sms_clt_success = FALSE;
    SIMAT_SIM_CONTROL_RESULT_E result = SIMAT_SIMC_ALLOWED;
    MMI_TEXT_ID_T sms_text_id = TXT_NULL;
    BOOLEAN is_need_modified = TRUE;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSmsCtlToMmiCmd arg is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5292_112_2_18_2_56_2_141,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_SMS_CTL_TO_MMI_IND_T *)arg;
    param_in = (SIMAT_SMS_CTL_TO_MMI_IND_T *)(&arg_in_ptr->com);
    dual_sys = arg_in_ptr->dual_sys;
    result = param_in->cntrl_rsp.result;

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSmsCtlToMmiCmd dual_sys=%d, is_alpha_id=%d, len=%d, is_dest_me=%d, is_dest_sc=%d, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5301_112_2_18_2_56_2_142,(uint8*)"dddddd",dual_sys, param_in->is_alpha_id, param_in->alpha_id.len, param_in->cntrl_rsp.is_dest_me, param_in->cntrl_rsp.is_dest_sc, result);

    switch (result)
    {
    case SIMAT_SIMC_ALLOWED:
        //sms_text_id = TXT_STK_SMS_ALLOWED;
        /*do not prompt*/
        break;

    case SIMAT_SIMC_NOT_ALLOWED:
        sms_text_id = TXT_STK_SMS_NOT_ALLOWED;
        break;

    case SIMAT_SIMC_ALLOWED_AND_MODIFIED:
        sms_text_id = TXT_STK_SMS_MODIFIED;
        break;

    default:
        sms_text_id = TXT_NULL;
        break;
    }

    if (!param_in->is_alpha_id)//alpha no present
    {
        MMI_GetLabelTextByLang(sms_text_id, &alert_string);
    }
    else
    {
        if (0 != param_in->alpha_id.len)//alpha id present
        {
            SCI_MEMSET(displaybuf, 0, sizeof(displaybuf));

            display_string.wstr_ptr = (wchar*)displaybuf;

            AlphaId2MMIStr(&(param_in->alpha_id), &display_string, MMISTK_STRING_MAX_LEN);

            if (SIMAT_SIMC_ALLOWED_AND_MODIFIED == result)//call modified,alpha id exit and can't fix number
            {
                is_need_modified = FALSE;
            }
        }
        else
        {
            is_need_modified = FALSE;
        }
    }

    if (MMK_IsOpenWin(MMISTK_SENDSM_WIN_ID))
    {
        if (0 != display_string.wstr_len)
        {
            GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
            uint16 index = 0;

            if (s_mmistk_sms_is_icon_id && s_mmistk_icon_display_result)
            {
                index = 2;
            }
            else
            {
                index = 1;
            }

            item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            item_data.text_data.buf.str_ptr = display_string.wstr_ptr;
            item_data.text_data.buf.len = display_string.wstr_len;

            GUIRICHTEXT_AddItem(
                MMISTK_TEXBOX_CTRL_ID,
                &item_data,
                &index
                );

            MMK_SendMsg(MMISTK_SENDSM_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }

    is_sms_clt_success = MMIAPISMS_ResponseSmsCtl(dual_sys, &alert_string, &display_string, &param_in->cntrl_rsp.dest_me, &param_in->cntrl_rsp.dest_sc, is_need_modified);

    //SCI_TRACE_LOW:"[MMI_STK] Leave MMISTK_OnSmsCtlToMmiCmd is_sms_clt_success=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5381_112_2_18_2_56_2_143,(uint8*)"d", is_sms_clt_success);
}

/********************************************************************************
 NAME:			MMISTK_OnSMSP2PRspCmd
 DESCRIPTION:	SIMTK can support dynamic menu managerment. SIMTK server can send
 				the dynamic menu information in a SMS to ME, the MN layer should parse the 
 				special SMS to SIMTK bypass the user. this means PP(point to point).
 				after SIMTK received the parsed result, SIMTK will response this signal.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSMSP2PRspCmd(void* arg)
{
    //SIMAT_SMS_PP_RSP_T* param_in = PNULL; 

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSMSP2PRspCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5400_112_2_18_2_56_2_144,(uint8*)"");
        return;
    }
    //param_in = (SIMAT_SMS_PP_RSP_T*)arg;
    //SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnSMSP2PRspCmd");
    // this need not the close share buffer
    //SIMAT_IndSMSPP()
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSMSP2PRspCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5407_112_2_18_2_56_2_145,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_OnSMSCBRspCmd
 DESCRIPTION:	SIMTK can support dynamic menu managerment. SIMTK server can send
 				the dynamic menu information in a CB to ME, the MN layer should parse the 
 				special SMS to SIMTK bypass the user. 
 				after SIMTK received the parsed result, SIMTK will response this signal.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSMSCBRspCmd(void* arg)
{
    //SIMAT_SMS_CB_RES* param_in = PNULL;

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSMSCBRspCmd:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5426_112_2_18_2_56_2_146,(uint8*)"");
        return;
    }
    //param_in = (SIMAT_SMS_CB_RES*)arg;
    //SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnSMSCBRspCmd");
    // this need not the close share buffer
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSMSCBRspCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5432_112_2_18_2_56_2_147,(uint8*)"");
}

/********************************************************************************
 NAME:			CheckCloseDisplayTextIRWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.10.07
 ********************************************************************************/
LOCAL void CheckCloseDisplayTextIRWin(void)
{
    if (s_mmistk_displaytext_is_ir)
    {
        s_mmistk_displaytext_is_ir = FALSE;
        MMK_CloseWin(MMISTK_DISPLAYTEXT_WIN_ID);
    }
    else
    {
        if (MMK_IsOpenWin(MMISTK_DISPLAYTEXT_WIN_ID)
            && (MMISTK_DISPLAYTEXT_DELAY_DURATION == s_mmistk_displaytext_timer_duration))
        {
            MMK_SendMsg(MMISTK_DISPLAYTEXT_WIN_ID, MSG_APP_OK, PNULL);
        }
    }

    MMK_CloseWin(MMISTK_MORE_TIME_WIN_ID);
}

/********************************************************************************
 NAME:			CheckUnCloseWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.10.07
 ********************************************************************************/
LOCAL void CheckUnCloseWin(uint32 curr_win)
{
    //SCI_TRACE_LOW:"[MMI_STK] enter CheckUnCloseWin, s_mmistk_close_window = %d, curr_win = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5470_112_2_18_2_56_2_148,(uint8*)"dd", s_mmistk_close_window, curr_win);
    if (MMISTK_CLOSE_DISPLAYTEXT_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_DISPLAYTEXT_WIN)
        {
            MMK_CloseWin(MMISTK_DISPLAYTEXT_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_GETINPUT_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_GETINPUT_WIN)
        {
            MMIPUB_CloseAlertWin();
            MMK_CloseWin(MMISTK_ALERT_WIN_ID);
            MMK_CloseWin(MMISTK_GETINPUT_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_GETINKEY_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_GETINKEY_WIN)
        {
            MMIPUB_CloseAlertWin();
            MMK_CloseWin(MMISTK_ALERT_WIN_ID);
            MMK_CloseWin(MMISTK_GETINKEY_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_SENDSMS_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_SENDSMS_WIN)
        {
            MMK_CloseWin(MMISTK_SENDSM_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_PLAYTONE_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_PLAYTONE_WIN)
        {
            MMK_CloseWin(MMISTK_PLAYTONE_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_SENDSS_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_SENDSS_WIN)
        {
            MMK_CloseWin(MMISTK_SENDSS_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_SETUP_CALL_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_SETUP_CALL_WIN)
        {
            MMK_CloseWin(MMISTK_SETUP_CALL_WIN_ID);
        }
    }
    else if (MMISTK_CLOSE_LAUNCH_BROWSER_WIN == s_mmistk_close_window)
    {
        if (curr_win != MMISTK_CLOSE_LAUNCH_BROWSER_WIN)
        {
            MMK_CloseWin(MMISTK_LAUNCH_BROWSER_WIN_ID);
        }
    }

    s_mmistk_close_window = MMISTK_CLOSE_NONE;
    //SCI_TRACE_LOW:"[MMI_STK] leave CheckUnCloseWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5531_112_2_18_2_56_3_149,(uint8*)"");
}

/********************************************************************************
 NAME:			CloseAllStkWin
 DESCRIPTION: close all stk win id, if another sim come STK message
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		dave.ruan
 DATE:			2011.11.28
 ********************************************************************************/
LOCAL void CloseAllStkWin(void)
{
    //SCI_TRACE_LOW:"[MMI_STK] enter CloseAllStkWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5543_112_2_18_2_56_3_150,(uint8*)"");
    MMK_CloseWin(MMISTK_MORE_TIME_WIN_ID);
    MMK_CloseWin(MMISTK_DISPLAYTEXT_WIN_ID);
    MMK_CloseWin(MMISTK_GETINPUT_WIN_ID);
    MMK_CloseWin(MMISTK_GETINKEY_WIN_ID);
    MMK_CloseWin(MMISTK_SENDSM_WIN_ID);
    MMK_CloseWin(MMISTK_PLAYTONE_WIN_ID);
    MMK_CloseWin(MMISTK_SEND_DTMF_WIN_ID);
    MMK_CloseWin(MMISTK_SENDSS_WIN_ID);
    MMK_CloseWin(MMISTK_SETUP_CALL_WIN_ID);
    MMK_CloseWin(MMISTK_LAUNCH_BROWSER_WIN_ID);
    MMK_CloseWin(MMISTK_MENU_WIN_ID);
    s_mmistk_close_window = MMISTK_CLOSE_NONE;
    //SCI_TRACE_LOW:"[MMI_STK] leave CloseAllStkWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5555_112_2_18_2_56_3_151,(uint8*)"");
}

/********************************************************************************
 NAME:			IsInProactiveStkSession
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kelly.li
 DATE:			
 ********************************************************************************/
LOCAL BOOLEAN IsInProactiveStkSession(void)
{
    if (MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID)
        || MMK_IsOpenWin(MMISTK_MENU_WIN_ID)
        || MMK_IsOpenWin(MMISTK_GETINPUT_WIN_ID)
        || MMK_IsOpenWin(MMISTK_GETINKEY_WIN_ID)
        || MMK_IsOpenWin(MMISTK_DISPLAYTEXT_WIN_ID)
        || MMK_IsOpenWin(MMISTK_SENDSM_WIN_ID)
        || MMK_IsOpenWin(MMISTK_PLAYTONE_WIN_ID)
        || MMK_IsOpenWin(MMISTK_SETUP_CALL_WIN_ID)
        || MMK_IsOpenWin(MMISTK_SENDSS_WIN_ID)
        || MMK_IsOpenWin(MMISTK_SETUP_CALL_WIN_ID)
        || MMK_IsOpenWin(MMISTK_SELECT_ITEM_OPTION_WIN_ID)
        || MMK_IsOpenWin(MMISTK_TOP_MENU_OPTION_WIN_ID)
        || MMK_IsOpenWin(MMISTK_OPEN_CHANNEL_WAITING_WIN_ID)
        || MMK_IsOpenWin(MMISTK_OPEN_CHANNEL_WIN_ID)
        )
    {
        //SCI_TRACE_LOW:"mmistk_win.c: is in proactive stk session"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5583_112_2_18_2_56_3_152,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"mmistk_win.c: is not in proactive stk session"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5588_112_2_18_2_56_3_153,(uint8*)"");
        return FALSE;
    }
}

/********************************************************************************
 NAME:			MMISTK_OnSessionTerminateCmd
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.19
********************************************************************************/
void MMISTK_OnSessionTerminateCmd(void* arg)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSessionTerminateCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5604_112_2_18_2_56_3_154,(uint8*)"");

    dual_sys = ((APP_MN_SIMAT_SESSION_TERMINATED_IND_T *)arg)->dual_sys;

    if (dual_sys != MMISTK_GetSTKSim())
    {
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSessionTerminateCmd dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5610_112_2_18_2_56_3_155,(uint8*)"d", ((APP_MN_SIMAT_SESSION_TERMINATED_IND_T *)arg)->dual_sys);

        return;
    }

    //CheckUnCloseWin(MMISTK_CLOSE_NONE);
    if (!s_mmistk_displaytext_is_ir)
    {
        MMK_CloseWin(MMISTK_DISPLAYTEXT_WIN_ID);
    }
    MMK_CloseWin(MMISTK_MORE_TIME_WIN_ID);
    MMK_CloseWin(MMISTK_GETINPUT_WIN_ID);
    MMK_CloseWin(MMISTK_GETINKEY_WIN_ID);
    MMK_CloseWin(MMISTK_SENDSM_WIN_ID);
    MMK_CloseWin(MMISTK_PLAYTONE_WIN_ID);
    MMK_CloseWin(MMISTK_SENDSS_WIN_ID);
    MMK_CloseWin(MMISTK_SETUP_CALL_WIN_ID);
    MMK_CloseWin(MMISTK_LAUNCH_BROWSER_WIN_ID);
    MMK_CloseWin(MMISTK_OPEN_CHANNEL_WAITING_WIN_ID);
    MMK_CloseWin(MMISTK_OPEN_CHANNEL_WIN_ID);
    s_mmistk_close_window = MMISTK_CLOSE_NONE;

    MMK_CloseWin(MMISTK_MENU_WIN_ID);

    DestroySTKStrMemory(&s_mmistk_setup_call_str);
    DestroySTKStrMemory(&s_mmistk_displaybuf2);
    if (PNULL != s_mmistk_cur_menu_info_ptr[dual_sys])
    {
        DestroySTKStrMemory(&s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item);
        DestroySTKStrMemory(&s_mmistk_cur_menu_info_ptr[dual_sys]);
    }
    DestroySTKStrMemory(&s_mmistk_displaybuf);
    DestroySTKStrMemory(&s_mmistk_inputbuf);
    DestroySTKStrMemory(&s_mmistk_dtmf_string);
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    DestroySTKStrMemory(&(s_icon_bitmap.bit_ptr));
#endif
    DestroySTKStrMemory(&s_title_from_selected_item_context);

    //@gang.tong 
    //Sometimes, ME will receive a session termination msg after selecting a item in the Main menu from SIM.
    //The reason for getting the termination msg is the service indicated by selected item is invalid. so SIM send the msg to ME.
    //In this case, we should set s_mmistk_indmenusel_cnf to TRUE, for another item selection in Main menu.
    s_mmistk_indmenusel_cnf = TRUE;	// this means can call SIMAT_IndMenuSel() function
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSessionTerminateCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5651_112_2_18_2_56_3_156,(uint8*)"");
}

/*****************************************************************************/
//  Description : Get Menu Action Id Data
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL uint16 GetMenuActionIdName(uint8 action_id, wchar *name_ptr, uint16 max_name_len)
{
    uint16 name_len = 0;
    // const char * action_name_ptr = PNULL;
    uint32 i = 0;
    uint16 menu_action_name_num = sizeof(s_menu_action_name)/sizeof(s_menu_action_name[0]);

    if(PNULL == name_ptr)
    {
        //SCI_TRACE_LOW:"GetMenuActionIdName:PNULL == name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5668_112_2_18_2_56_3_157,(uint8*)"");
        return 0;
    }

    for (i = 0; i < menu_action_name_num; i++)
    {
        if (action_id == s_menu_action_name[i].pc_tag)
        {
            name_len = SCI_STRLEN((char *)s_menu_action_name[i].menu_action_name_ptr);
            name_len = MIN(max_name_len, name_len);

            MMI_STRNTOWSTR(name_ptr,
                                            max_name_len,
                                            (uint8 *)s_menu_action_name[i].menu_action_name_ptr,
                                            name_len,
                                            name_len);

            break;
        }
    }

    return name_len;
}

/*****************************************************************************/
//  Description : Set Menu Action Id Data
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL void SetMenuActionIdData(
                                                               STK_MENU_INFO_T *menu_info_ptr,
                                                               void *tips_data_ptr
                                                               )
{
    GUITIPS_DATA_T *need_tips_data_ptr = (GUITIPS_DATA_T*)tips_data_ptr;
    uint16 index = 0;
    wchar menu_action_name[MMITIPS_TEXT_MAX_LEN + 1] = {0};

    if (PNULL == menu_info_ptr || PNULL == need_tips_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMI_STK] SetMenuActionIdData Param Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5708_112_2_18_2_56_3_158,(uint8*)"");
        return;
    }
	
    index = need_tips_data_ptr->index;

    if (menu_info_ptr->is_action_id && index < menu_info_ptr->menu_item_amount)
    {
        need_tips_data_ptr->wstr_len = GetMenuActionIdName(menu_info_ptr->menu_item[index].nextActionIndicator, (wchar *)menu_action_name, MMITIPS_TEXT_MAX_LEN);

        if (need_tips_data_ptr->wstr_len > 0)
        {
            if(need_tips_data_ptr->wstr_len > 0)
            {
                MMI_WSTRNCPY(need_tips_data_ptr->wstr_ptr,
                                            MMITIPS_TEXT_MAX_LEN,
                                            menu_action_name,
                                            MMITIPS_TEXT_MAX_LEN,
                                            need_tips_data_ptr->wstr_len);

                need_tips_data_ptr->is_need_tips = TRUE;
            }
            else
            {
                need_tips_data_ptr->is_need_tips = FALSE;
            }
        }
    }
    else
    {
        need_tips_data_ptr->is_need_tips = FALSE;
    }
}

/********************************************************************************
 NAME:			MMISTK_MainMenuWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
MMI_RESULT_E MMISTK_MainMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    SIMAT_MENU_SELECTION_IND_T menu_select = {FALSE};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16 menuitem_id = 1;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    uint16 index = 0;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_can_display = FALSE;//(is_icon_id ||self_explan) && decode OK
    static BOOLEAN s_first_paint = FALSE;
#endif

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_MainMenuWinHandleMsg, msg_id = 0x%04X, s_mmistk_indmenusel_cnf = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5763_112_2_18_2_56_3_159,(uint8*)"dd", msg_id, s_mmistk_indmenusel_cnf);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
        s_first_paint = TRUE;

        if(s_mmistk_toplevel_menu_info[dual_sys]->is_icon_id || s_mmistk_toplevel_menu_info[dual_sys]->is_menu_title_icon_self_explan)
        {
            MN_SIM_IMG_T sim_img = {0};
            
            MNSIM_GetImgEx(dual_sys, s_mmistk_toplevel_menu_info[dual_sys]->menu_title_icon_id, &sim_img);
            
            //decode OK
            if(DecodeSimImgToBmp(&sim_img, &(s_mmistk_toplevel_menu_info[dual_sys]->menu_title_icon_bmp)))
            {
                is_icon_can_display = TRUE;
            }
            else
            {
                is_icon_can_display = FALSE;
            }
        }

        if (s_mmistk_toplevel_menu_info[dual_sys]->is_icon_id  && is_icon_can_display)
        {
            both_rect.v_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
            both_rect.h_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
        }
#endif
#endif
        GUIAPICTRL_SetBothRect(MMISTK_MAINMENU_CTRL_ID, &both_rect);

#ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#endif
        MMK_SetAtvCtrl(MMISTK_MAINMENU_WIN_ID, MMISTK_MAINMENU_CTRL_ID);
        break;

    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
//    case MSG_APP_0:
//    case MSG_APP_STAR:
//    case MSG_APP_HASH:
        {
            uint16 i = 0;
            uint16 key_max_num = 0;
            uint16 total_num = 0;

            key_max_num = sizeof(s_mmistk_numkey_map)/sizeof(MMISTK_NUMKEY_MAP_T);
            total_num = GUILIST_GetTotalItemNum(MMISTK_MAINMENU_CTRL_ID);
            for (i=0; i<key_max_num; i++)
            {
                if (msg_id == s_mmistk_numkey_map[i].key_msg)
                {
                    if (i < total_num)
                    {
                        GUILIST_SetCurItemIndex(MMISTK_MAINMENU_CTRL_ID, s_mmistk_numkey_map[i].num_index);

                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                        MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                    }
                    break;
                }
            }
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if( PNULL == need_item_data_ptr )
            {
                break;
            }

            index = need_item_data_ptr->item_index;
            CreateDynaMenuItemData(MMISTK_MAINMENU_CTRL_ID, index, s_mmistk_toplevel_menu_info[dual_sys],dual_sys);        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            uint16 cont_index = 0;
            GUILIST_NEED_ITEM_CONTENT_T *need_item_cont_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            
            if(PNULL == need_item_cont_ptr)
            {
                //SCI_TRACE_LOW:"MMISTK_MainMenuWinHandleMsg need_item_cont_ptr is NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5841_112_2_18_2_56_3_160,(uint8*)"");
                break;
            }

            index = need_item_cont_ptr->item_index;
            cont_index = need_item_cont_ptr->item_content_index;
            CreateDynaMenuItemContent(MMISTK_MAINMENU_CTRL_ID, index, cont_index, s_mmistk_toplevel_menu_info[dual_sys], dual_sys);
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        result = MMI_RESULT_FALSE;		// need additional process
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMISTK_MAINMENU_WIN_ID);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if (s_mmistk_toplevel_menu_info[dual_sys]->help_info)
        {
            OpenTopMenuOptionWin(dual_sys);
        }
        else
        {
            if (s_mmistk_indmenusel_cnf) 
            {
                menu_select.is_help_IND = FALSE;
                if (s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount > 0)
                {
                    menuitem_id = GUILIST_GetCurItemIndex(MMISTK_MAINMENU_CTRL_ID);
                }

                if (menuitem_id < s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount)
                {
                    menu_select.menu_id = s_mmistk_toplevel_menu_info[dual_sys]->menu_item[menuitem_id].itemID;
                    s_mmistk_indmenusel_cnf = FALSE;
                    //@gang.tong
                    if (CreateSTKBufferMemory(&s_title_from_selected_item_context, sizeof(STK_MENU_ITEM_T)))
                    {
                        SCI_MEMCPY(s_title_from_selected_item_context, &s_mmistk_toplevel_menu_info[dual_sys]->menu_item[menuitem_id], sizeof(STK_MENU_ITEM_T));
                    }
                    SIMAT_IndMenuSelEx(dual_sys, &menu_select);
                }
            }
        }
        break;
#if 0 //no response
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (s_mmistk_indmenusel_cnf) 
        {
            menu_select.is_help_IND = FALSE;
            if (s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount > 0)
            {
                menuitem_id = GUILIST_GetCurItemIndex(MMISTK_MAINMENU_CTRL_ID);
            }

            if (menuitem_id < s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount)
            {
                menu_select.menu_id = s_mmistk_toplevel_menu_info[dual_sys]->menu_item[menuitem_id].itemID;
                s_mmistk_indmenusel_cnf = FALSE;
                //@gang.tong
                if (CreateSTKBufferMemory(&s_title_from_selected_item_context, sizeof(STK_MENU_ITEM_T)))
                {
                    SCI_MEMCPY(s_title_from_selected_item_context, &s_mmistk_toplevel_menu_info[dual_sys]->menu_item[menuitem_id], sizeof(STK_MENU_ITEM_T));
                }
                SIMAT_IndMenuSelEx(dual_sys, &menu_select);
            }
        }
        break;
#endif
    case MSG_LOSE_FOCUS:
        break;

    case MSG_GET_FOCUS:
        MMK_CloseWin(MMISTK_MENU_WIN_ID);
        // this means can call SIMAT_IndMenuSel() function again
        s_mmistk_indmenusel_cnf = TRUE;	
        s_mmistk_menutitle_need_refresh = TRUE;
        break;

    case MSG_CTL_TIPS_NEED_DATA:
        SetMenuActionIdData(s_mmistk_toplevel_menu_info[dual_sys], param);
        break;

    case MSG_CLOSE_WINDOW:
        if (s_mmistk_indmenusel_cnf)
        {
            s_mmistk_indmenusel_cnf = FALSE;
        }
        MMIAPIALM_CheckAndStartDelayedAlarm();
        break;

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    case MSG_END_FULL_PAINT:
        if (s_first_paint)
        {
            s_first_paint = FALSE;
            MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
        }
        break;
#endif

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_MainMenuWinHandleMsg(0x%02X), s_mmistk_indmenusel_cnf = %d, result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5954_112_2_18_2_56_3_161,(uint8*)"ddd", msg_id, s_mmistk_indmenusel_cnf, result);
    return (result);
}

/********************************************************************************
 NAME:			MenuWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL MMI_RESULT_E MenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    SIMAT_SELECT_ITEM_CNF_T menu_select = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16 menuitem_id = 1;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_can_display = FALSE;//(is_icon_id ||self_explan) && decode OK
    static BOOLEAN s_first_paint = FALSE;
#endif

    //SCI_TRACE_LOW:"[MMI_STK] enter MenuWinHandleMsg, msg_id = 0x%04X, s_mmistk_selectitem_cnf = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_5978_112_2_18_2_56_3_162,(uint8*)"dd", msg_id, s_mmistk_selectitem_cnf);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
        s_first_paint = TRUE;

        if(s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_id || s_mmistk_cur_menu_info_ptr[dual_sys]->is_menu_title_icon_self_explan)
        {
            MN_SIM_IMG_T sim_img = {0};
            
            MNSIM_GetImgEx(dual_sys, s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_icon_id, &sim_img);
            
            //decode OK
            if(DecodeSimImgToBmp(&sim_img, &(s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_icon_bmp)))
            {
                is_icon_can_display = TRUE;
            }
            else
            {
                is_icon_can_display = FALSE;
            }
        }

        if (s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_id  && is_icon_can_display)
        {
            both_rect.v_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
            both_rect.h_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
        }
#endif
#endif

        GUIAPICTRL_SetBothRect(MMISTK_MENU_CTRL_ID, &both_rect);

        MMK_SetAtvCtrl(MMISTK_MENU_WIN_ID, MMISTK_MENU_CTRL_ID);
        break;

    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
//    case MSG_APP_0:
//    case MSG_APP_STAR:
//    case MSG_APP_HASH:
        {
            uint16 i = 0;
            uint16 key_max_num = 0;
            uint16 total_num = 0;

            key_max_num = sizeof(s_mmistk_numkey_map)/sizeof(MMISTK_NUMKEY_MAP_T);
            total_num = GUILIST_GetTotalItemNum(MMISTK_MENU_CTRL_ID);

            for (i=0; i<key_max_num; i++)
            {
                if (msg_id == s_mmistk_numkey_map[i].key_msg)
                {
                    if (i < total_num)
                    {
                        GUILIST_SetCurItemIndex(MMISTK_MENU_CTRL_ID, s_mmistk_numkey_map[i].num_index);

                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                        MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                    }
                    break;
                }
            }
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if( PNULL == need_item_data_ptr )
            {
                break;
            }

            index = need_item_data_ptr->item_index;
            CreateDynaMenuItemData(MMISTK_MENU_CTRL_ID, index, s_mmistk_cur_menu_info_ptr[dual_sys],dual_sys);        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            uint16 index = 0;
            uint16 cont_index = 0;
            GUILIST_NEED_ITEM_CONTENT_T *need_item_cont_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            
            if(PNULL == need_item_cont_ptr)
            {
                //SCI_TRACE_LOW:"MenuWinHandleMsg need_item_cont_ptr is NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6057_112_2_18_2_56_4_163,(uint8*)"");
                break;
            }

            index = need_item_cont_ptr->item_index;
            cont_index = need_item_cont_ptr->item_content_index;
            CreateDynaMenuItemContent(MMISTK_MENU_CTRL_ID, index, cont_index, s_mmistk_cur_menu_info_ptr[dual_sys], dual_sys);
        }
        break;

    case MSG_FULL_PAINT:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        s_mmistk_selectitem_timer_id = MMK_CreateWinTimer(	
                                                                                            MMISTK_MENU_WIN_ID, 
                                                                                            MMISTK_SELECTITEM_DURATION,
                                                                                            FALSE
                                                                                            );
        break;

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    case MSG_END_FULL_PAINT:
        if (s_first_paint)
        {
            s_first_paint = FALSE;
            MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
        }
        break;
#endif

    case MSG_STK_SET_MENU_ATV:
        MMK_SetAtvCtrl(MMISTK_MENU_WIN_ID, MMISTK_MENU_CTRL_ID);
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmistk_selectitem_timer_id) 
        {
            //SCI_TRACE_LOW:"[MMI_STK] after timer duration %d, Timer(%d) arrived"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6097_112_2_18_2_56_4_164,(uint8*)"dd", MMISTK_SELECTITEM_DURATION, s_mmistk_selectitem_timer_id);
            if (s_mmistk_selectitem_cnf) 
            {
                menu_select.result = SIMAT_TR_NO_RESPONSE;
                menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                s_mmistk_selectitem_cnf = FALSE;
                s_mmistk_item_is_selecting[dual_sys] = FALSE;
                SIMAT_CnfSelItemEx(dual_sys, &menu_select);
            }

            MMISTK_OpenAlertWin(TXT_STK_OVERTIME_RETURN, IMAGE_PUBWIN_WARNING, PNULL);
            s_mmistk_selectitem_timer_id = 0;
        }
        else
        {
            result = MMI_RESULT_FALSE;		// need additional process
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }

        if (s_mmistk_selectitem_cnf) 
        {
            menu_select.result = SIMAT_TR_TERMINATED_BY_USER;
            menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_selectitem_cnf = FALSE;
            s_mmistk_item_is_selecting[dual_sys] = FALSE;
            SIMAT_CnfSelItemEx(dual_sys, &menu_select);
        }
        result = MMI_RESULT_FALSE;		// need additional process
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (s_mmistk_selectitem_cnf) 
        {
            menu_select.result = SIMAT_TR_BACK_MOVE;
            menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_selectitem_cnf = FALSE;
            s_mmistk_item_is_selecting[dual_sys] = FALSE;
            SIMAT_CnfSelItemEx(dual_sys, &menu_select);
        }
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(PNULL == s_mmistk_cur_menu_info_ptr[dual_sys])
        {
            //SCI_TRACE_LOW:"MenuWinHandleMsg ::MSG_CTL_OK s_mmistk_cur_menu_info_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6161_112_2_18_2_56_4_165,(uint8*)"");
            break;
        }
        
        if (s_mmistk_cur_menu_info_ptr[dual_sys]->help_info)
        {
            OpenSelectItemOptionWin(dual_sys);
        }
        else
        {
            if (s_mmistk_selectitem_cnf) 
            {
                if (s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_id || s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_list)
                {
//                    menu_select.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                    menu_select.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
                }
                else
                {
                    menu_select.result = SIMAT_TR_SUCCESS;
                }
                menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                if (s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount > 0)
                {
                    menuitem_id = GUILIST_GetCurItemIndex(MMISTK_MENU_CTRL_ID);
                }

                if (menuitem_id < s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount)
                {
                    menu_select.item_id = s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[menuitem_id].itemID;
                    s_mmistk_selectitem_cnf = FALSE;
                    s_mmistk_item_is_selecting[dual_sys] = FALSE;
                    if (CreateSTKBufferMemory(&s_title_from_selected_item_context, sizeof(STK_MENU_ITEM_T)))
                    {
                        SCI_MEMCPY(s_title_from_selected_item_context, &s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[menuitem_id], sizeof(STK_MENU_ITEM_T));
                    }
                    SIMAT_CnfSelItemEx(dual_sys, &menu_select);
                }
            }
            if (0 != s_mmistk_selectitem_timer_id)
            {
                MMK_StopTimer(s_mmistk_selectitem_timer_id);
                s_mmistk_selectitem_timer_id = 0;
            }
        }
        break;
#if 0
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        if(PNULL == s_mmistk_cur_menu_info_ptr[dual_sys])
        {
            break;
        }
        

        if (s_mmistk_selectitem_cnf) 
        {
            if (s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_id || s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_list)
            {
//                menu_select.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                menu_select.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                menu_select.result = SIMAT_TR_SUCCESS;
            }
            menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            if (s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount > 0)
            {
                menuitem_id = GUILIST_GetCurItemIndex(MMISTK_MENU_CTRL_ID);
            }

            if (menuitem_id < s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount)
            {
                menu_select.item_id = s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[menuitem_id].itemID;
                s_mmistk_selectitem_cnf = FALSE;
                s_mmistk_item_is_selecting[dual_sys] = FALSE;
                if (CreateSTKBufferMemory(&s_title_from_selected_item_context, sizeof(STK_MENU_ITEM_T)))
                {
                    SCI_MEMCPY(s_title_from_selected_item_context, &s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[menuitem_id], sizeof(STK_MENU_ITEM_T));
                }
                SIMAT_CnfSelItemEx(dual_sys, &menu_select);
            }
        }
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;
#endif
    case MSG_LOSE_FOCUS:
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }
        break;

    case MSG_CTL_TIPS_NEED_DATA:
        SetMenuActionIdData(s_mmistk_cur_menu_info_ptr[dual_sys], param);
        break;

    case MSG_CLOSE_WINDOW:
        if (s_mmistk_selectitem_cnf) 
        {
            menu_select.result = SIMAT_TR_TERMINATED_BY_USER;
            menu_select.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_selectitem_cnf = FALSE;
            s_mmistk_item_is_selecting[dual_sys] = FALSE;
            SIMAT_CnfSelItemEx(dual_sys, &menu_select);
        }
        if (0 != s_mmistk_selectitem_timer_id)
        {
            MMK_StopTimer(s_mmistk_selectitem_timer_id);
            s_mmistk_selectitem_timer_id = 0;
        }

        {
            uint16 index = 0;
            if (s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_id)
            {
                DestroySTKStrMemory(&(s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_icon_bmp.bit_ptr));
            }
            if (s_mmistk_cur_menu_info_ptr[dual_sys]->is_icon_list)
            {
                while (index < s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item_amount)
                {
                    DestroySTKStrMemory(&(s_mmistk_cur_menu_info_ptr[dual_sys]->menu_item[index].icon_bmp.bit_ptr));
                    index++;
                }
            }
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MenuWinHandleMsg(0x%02X), s_mmistk_selectitem_cnf = %d, result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6298_112_2_18_2_56_4_166,(uint8*)"ddd", msg_id, s_mmistk_selectitem_cnf, result);
    return (result);
}

/********************************************************************************
 NAME:			DisplayTextWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
LOCAL MMI_RESULT_E DisplayTextWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_DISPLAY_TEXT_CNF_T disp_cnf = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"[MMI_STK] enter DisplayTextWinHandleMsg, msg_id = 0x%04X, s_mmistk_displaytext_cnf = %d, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6319_112_2_18_2_56_4_167,(uint8*)"ddd", msg_id, s_mmistk_displaytext_cnf, s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifdef  ADULT_WATCH_SUPPORT
        GUIRICHTEXT_SetBgColor(MMISTK_TEXBOX_CTRL_ID,MMI_WHITE_COLOR);//fix bug1608095
#endif
        MMK_SetAtvCtrl(MMISTK_DISPLAYTEXT_WIN_ID, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        if (0 != s_mmistk_displaytext_timer_duration)
        {
            if (0 == s_mmistk_displaytext_timer_id && s_mmistk_displaytext_cnf)
            {
                s_mmistk_displaytext_timer_id = MMK_CreateWinTimer(MMISTK_DISPLAYTEXT_WIN_ID,
                                                                   s_mmistk_displaytext_timer_duration,
                                                                   FALSE);
            }
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        if (0 != s_mmistk_displaytext_timer_id)
        {
            MMK_StopTimer(s_mmistk_displaytext_timer_id);
            s_mmistk_displaytext_timer_id = 0;
        }
        if (s_mmistk_displaytext_cnf) 
        {
            disp_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            disp_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_displaytext_cnf = FALSE;
            s_mmistk_text_is_displaying[dual_sys] = FALSE;

            if (!s_mmistk_displaytext_is_ir)
            {
                SIMAT_CnfDisplayTextEx(dual_sys, &disp_cnf);
            }

            s_mmistk_displaytext_is_ir = FALSE;
        }
        result = MMI_RESULT_FALSE;		// need additional process
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (s_mmistk_displaytext_cnf) 
        {
            if (0 != s_mmistk_displaytext_timer_id)
            {
                MMK_StopTimer(s_mmistk_displaytext_timer_id);
                s_mmistk_displaytext_timer_id = 0;
            }
            disp_cnf.result = SIMAT_TR_BACK_MOVE;
            disp_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_displaytext_cnf = FALSE;
            s_mmistk_text_is_displaying[dual_sys] = FALSE;

            if (!s_mmistk_displaytext_is_ir)
            {
                SIMAT_CnfDisplayTextEx(dual_sys, &disp_cnf);
                s_mmistk_close_window = MMISTK_CLOSE_DISPLAYTEXT_WIN;
            }
        }
        if (s_mmistk_displaytext_is_ir)
        {
            MMK_CloseWin(win_id);

            s_mmistk_displaytext_is_ir = FALSE;
        }
        break;

    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //IGNORE9527 
#endif
        if (s_mmistk_displaytext_cnf) 
        {
            if (0 != s_mmistk_displaytext_timer_id)
            {
                MMK_StopTimer(s_mmistk_displaytext_timer_id);
                s_mmistk_displaytext_timer_id = 0;
            }
            if (s_mmistk_displaytext_is_icon_exist)
            {
//                disp_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                disp_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                disp_cnf.result = SIMAT_TR_SUCCESS;
            }
            disp_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_displaytext_cnf = FALSE;
            s_mmistk_text_is_displaying[dual_sys] = FALSE;

            if (!s_mmistk_displaytext_is_ir)
            {
                SIMAT_CnfDisplayTextEx(dual_sys, &disp_cnf);
                s_mmistk_close_window = MMISTK_CLOSE_DISPLAYTEXT_WIN;
            }
        }
        if (s_mmistk_displaytext_is_ir)
        {
            MMK_CloseWin(win_id);

            s_mmistk_displaytext_is_ir = FALSE;
        }
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmistk_displaytext_timer_id) 
        {
            //SCI_TRACE_LOW:"[MMI_STK] after timer duration %d, Timer(%d) arrived"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6435_112_2_18_2_56_4_168,(uint8*)"dd", s_mmistk_displaytext_timer_duration, s_mmistk_displaytext_timer_id);

            s_mmistk_displaytext_timer_id = 0;

            if (s_mmistk_displaytext_cnf) 
            {
                if (s_mmistk_displaytext_timer_duration == MMISTK_DISPLAYTEXT_USER_CLEAR_DURATION)
                {
                    disp_cnf.result = SIMAT_TR_NO_RESPONSE;
                    disp_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                }
                else
                {
                    if (s_mmistk_displaytext_is_icon_exist)
                    {
//                        disp_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                        disp_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
                    }
                    else
                    {
                        disp_cnf.result = SIMAT_TR_SUCCESS;
                    }
                    disp_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                }
                s_mmistk_displaytext_cnf = FALSE;
                s_mmistk_text_is_displaying[dual_sys] = FALSE;

                if (!s_mmistk_displaytext_is_ir)
                {
                    SIMAT_CnfDisplayTextEx(dual_sys, &disp_cnf);
                }
            }
            if(s_mmistk_displaytext_is_ir)
            {
                MMK_CloseWin(win_id);
            }
            s_mmistk_close_window = MMISTK_CLOSE_DISPLAYTEXT_WIN;
        }
        else
        {
            result = MMI_RESULT_FALSE;		// need additional process
        }
        break;

    case MSG_LOSE_FOCUS:
        if (0 != s_mmistk_displaytext_timer_id)
        {
            if (s_mmistk_displaytext_cnf) 
            {
                MMK_PauseTimer(s_mmistk_displaytext_timer_id);
            }
            else
            {
                MMK_StopTimer(s_mmistk_displaytext_timer_id);
                s_mmistk_displaytext_timer_id = 0;
            }
        }
        break;

    case MSG_GET_FOCUS:
        if (0 != s_mmistk_displaytext_timer_id)
        {
            if (s_mmistk_displaytext_cnf) 
            {
                MMK_ResumeTimer(s_mmistk_displaytext_timer_id);
            }
            else
            {
                MMK_StopTimer(s_mmistk_displaytext_timer_id);
                s_mmistk_displaytext_timer_id = 0;
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (0 != s_mmistk_displaytext_timer_id)
        {
            MMK_StopTimer(s_mmistk_displaytext_timer_id);
            s_mmistk_displaytext_timer_id = 0;
        }
        if (s_mmistk_displaytext_cnf) 
        {
            disp_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            disp_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_displaytext_cnf = FALSE;
            s_mmistk_text_is_displaying[dual_sys] = FALSE;

            if (!s_mmistk_displaytext_is_ir)
            {
                SIMAT_CnfDisplayTextEx(dual_sys, &disp_cnf);
            }

            s_mmistk_displaytext_is_ir = FALSE;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave DisplayTextWinHandleMsg(0x%02X), s_mmistk_displaytext_cnf = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6536_112_2_18_2_56_5_169,(uint8*)"ddd", msg_id, s_mmistk_displaytext_cnf, result);
    return (result);
}

/********************************************************************************
 NAME:			GetInkeyWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
LOCAL MMI_RESULT_E GetInkeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_GET_INKEY_CNF_T inkey_cnf = {0};
    MMI_STRING_T txt_info = {PNULL};
    MMI_STRING_T str_info = {PNULL};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN next_act = FALSE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"[MMI_STK] enter GetInkeyWinHandleMsg, msg_id = 0x%04X, s_mmistk_getinkey_cnf = %d, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6560_112_2_18_2_56_5_170,(uint8*)"ddd", msg_id, s_mmistk_getinkey_cnf, s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        if (s_mmistk_inkey_type != SIMAT_YES_NO)
        {
            MMK_SetAtvCtrl(MMISTK_GETINKEY_WIN_ID, MMISTK_EDITBOX_CTRL_ID);
        }
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        if (0 == s_mmistk_getinkey_timer_id && s_mmistk_getinkey_cnf)
        {
            if(s_mmistk_getinkey_duration_exist && (s_mmistk_getinkey_duration > 0))
            {
                s_mmistk_getinkey_timer_id = MMK_CreateWinTimer(
                                                                                        MMISTK_GETINKEY_WIN_ID, 
                                                                                        s_mmistk_getinkey_duration,
                                                                                        FALSE
                                                                                        );
            }
            else
            {
                s_mmistk_getinkey_timer_id = MMK_CreateWinTimer(
                                                                                        MMISTK_GETINKEY_WIN_ID, 
                                                                                        MMISTK_GETINKEY_DURATION,
                                                                                        FALSE
                                                                                        );
            }
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        MMK_StopTimer(s_mmistk_getinkey_timer_id);
        s_mmistk_getinkey_timer_id = 0;
        if (s_mmistk_getinkey_cnf) 
        {
            //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
            inkey_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_getinkey_cnf = FALSE;
            s_mmistk_key_is_getting[dual_sys] = FALSE;
            SIMAT_CnfGetInkeyEx(dual_sys, &inkey_cnf);
        }
        result = MMI_RESULT_FALSE;		// need additional process
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (s_mmistk_getinkey_cnf) 
        {
            MMK_StopTimer(s_mmistk_getinkey_timer_id);
            s_mmistk_getinkey_timer_id = 0;
            //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
            if (s_mmistk_inkey_type == SIMAT_YES_NO) 
            {
                inkey_cnf.text_str.len = 1;
                inkey_cnf.text_str.dcs = MMI_DCS_8_BIT_ALPHABET;
                //inkey_cnf.text_str.text_ptr = SIMAT_AllocFromShareBuf(inkey_cnf.buf_id, 2);
                inkey_cnf.text_str.text_ptr[0] = 0;
                inkey_cnf.text_str.text_ptr[1] = 0;
                if (s_mmistk_inkey_is_icon_id)
                {
//                    inkey_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                    inkey_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
                }
                else
                {
                    inkey_cnf.result = SIMAT_TR_SUCCESS;
                }
                inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }
            else 
            {
                inkey_cnf.result = SIMAT_TR_BACK_MOVE;
                inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }
            s_mmistk_getinkey_cnf = FALSE;
            s_mmistk_key_is_getting[dual_sys] = FALSE;
            SIMAT_CnfGetInkeyEx(dual_sys, &inkey_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_GETINKEY_WIN;
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_SK_SKB_ICON_PRESSED:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:   
        if (MSG_CTL_PENOK == msg_id)
        {
            if (SIMAT_YES_NO != s_mmistk_inkey_type)
            {
                if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                {
                    MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);

                    break;
                }
                else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                {
                    MMK_SendMsg(win_id, MSG_CTL_MIDSK, PNULL);

                    break;
                }
            }
            else
            {
                if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                {
                    MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);

                    break;
                }
                else if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                {
                    MMK_SendMsg(win_id, MSG_CTL_MIDSK, PNULL);

                    break;
                }
                else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                {
                    MMK_SendMsg(win_id, MSG_CTL_CANCEL, PNULL);

                    break;
                }
            }
        }
#endif //IGNORE9527         
#endif 
        if (s_mmistk_getinkey_cnf)
        {
            if (s_mmistk_is_inkey_help_info
                && (MSG_CTL_MIDSK == msg_id || MSG_APP_WEB == msg_id || MSG_SK_SKB_ICON_PRESSED == msg_id))
            {
                MMK_StopTimer(s_mmistk_getinkey_timer_id);
                s_mmistk_getinkey_timer_id = 0;
                inkey_cnf.result = SIMAT_TR_HELP_INFO;
                inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                s_mmistk_getinkey_cnf = FALSE;
                s_mmistk_key_is_getting[dual_sys] = FALSE;
                SIMAT_CnfGetInkeyEx(dual_sys, &inkey_cnf);
            }
            else
            {
                switch (s_mmistk_inkey_type) 
                {
                case SIMAT_DIGIT_ONLY:
                    GUIEDIT_GetPhoneNumString(MMISTK_EDITBOX_CTRL_ID, &str_info);
                    if (str_info.wstr_len > 0) 
                    {
                        //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
                        inkey_cnf.text_str.len = 1;
                        inkey_cnf.text_str.dcs = MMI_DCS_8_BIT_ALPHABET;
                        //inkey_cnf.text_str.text_ptr = SIMAT_AllocFromShareBuf(inkey_cnf.buf_id, 2);
                        inkey_cnf.text_str.text_ptr[0] = str_info.wstr_ptr[0] & 0xff;
                        inkey_cnf.text_str.text_ptr[1] = 0;
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    break;

                case SIMAT_YES_NO:
                    //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
                    inkey_cnf.text_str.len = 1;
                    inkey_cnf.text_str.dcs = MMI_DCS_8_BIT_ALPHABET;
                    //inkey_cnf.text_str.text_ptr = SIMAT_AllocFromShareBuf(inkey_cnf.buf_id, 2);
                    inkey_cnf.text_str.text_ptr[0] = 1;
                    inkey_cnf.text_str.text_ptr[1] = 0;
                    next_act = TRUE;
                    break;

                case SIMAT_SMS_ALPHABET:
                    GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &txt_info);
                    if (txt_info.wstr_len > 0) 
                    {
                        //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
                        //inkey_cnf.text_str.text_ptr = SIMAT_AllocFromShareBuf(inkey_cnf.buf_id, 4);
                        inkey_cnf.text_str.len = 1;
                        inkey_cnf.text_str.dcs = MMI_DCS_8_BIT_ALPHABET;
                        inkey_cnf.text_str.text_ptr[0] = txt_info.wstr_ptr[0] & 0xff;
                        inkey_cnf.text_str.text_ptr[1] = 0;
                        inkey_cnf.text_str.text_ptr[2] = 0;
                        inkey_cnf.text_str.text_ptr[3] = 0;
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    break;

                case SIMAT_UCS2_ALPHABET:
                    GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &txt_info);
                    if (txt_info.wstr_len > 0) 
                    {
                        //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
                        //inkey_cnf.text_str.text_ptr = SIMAT_AllocFromShareBuf(inkey_cnf.buf_id, 4);
                        inkey_cnf.text_str.len = 2;
                        inkey_cnf.text_str.dcs = MMI_DCS_UCS2;
                        inkey_cnf.text_str.text_ptr[0] = (txt_info.wstr_ptr[0] & 0xff00)>>8;
                        inkey_cnf.text_str.text_ptr[1] = txt_info.wstr_ptr[0] & 0xff;
                        inkey_cnf.text_str.text_ptr[2] = 0x00;
                        inkey_cnf.text_str.text_ptr[3] = 0x00;
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    break;

                default:
                    break;
                }

                if (next_act) 
                {
                    MMK_StopTimer(s_mmistk_getinkey_timer_id);
                    s_mmistk_getinkey_timer_id = 0;
                    if (s_mmistk_inkey_is_icon_id)
                    {
//                    inkey_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                        inkey_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
                    }
                    else
                    {
                        inkey_cnf.result = SIMAT_TR_SUCCESS;
                    }
                    inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                    s_mmistk_getinkey_cnf = FALSE;
                    s_mmistk_key_is_getting[dual_sys] = FALSE;
                    SIMAT_CnfGetInkeyEx(dual_sys, &inkey_cnf);
                    s_mmistk_close_window = MMISTK_CLOSE_GETINKEY_WIN;
                }
                else
                {
                    MMI_STRING_T mmistk_msgbox_string = {0};
                    wchar alert_string[MMISTK_STRING_MAX_LEN + 1] = {0};
                    
                    mmistk_msgbox_string.wstr_ptr = alert_string;
                    MMI_GetLabelTextByLang(TXT_STK_INPUT_1, &mmistk_msgbox_string);
                    //MMK_CreateWin((uint32*)MMISTK_MSGBOX_WIN_TAB, PNULL);
                    MMISTK_OpenAlertWinByTextPtr(&mmistk_msgbox_string);
                }
            }
        }
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmistk_getinkey_timer_id) 
        {
            //SCI_TRACE_LOW:"[MMI_STK] after timer duration %d, Timer(%d) arrived"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6809_112_2_18_2_56_5_171,(uint8*)"dd", MMISTK_GETINKEY_DURATION, s_mmistk_getinkey_timer_id);
            if (s_mmistk_getinkey_cnf) 
            {
                //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
                inkey_cnf.result = SIMAT_TR_NO_RESPONSE;
                inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                if((s_mmistk_getinkey_duration > 0) && s_mmistk_getinkey_duration_exist)
                {
                    inkey_cnf.duration = s_mmistk_getinkey_duration;
                }
                s_mmistk_getinkey_cnf = FALSE;
                s_mmistk_key_is_getting[dual_sys] = FALSE;
                SIMAT_CnfGetInkeyEx(dual_sys, &inkey_cnf);
            }
            s_mmistk_close_window = MMISTK_CLOSE_GETINKEY_WIN;

            MMISTK_OpenAlertWin(TXT_STK_OVERTIME_RETURN, IMAGE_PUBWIN_WARNING, PNULL);
        }
        else
        {
            result = MMI_RESULT_FALSE;		// need additional process
        }
        break;

    case MSG_LOSE_FOCUS:
        if (s_mmistk_getinkey_cnf) 
        {
            MMK_PauseTimer(s_mmistk_getinkey_timer_id);
        }
        else
        {
            MMK_StopTimer(s_mmistk_getinkey_timer_id);
            s_mmistk_getinkey_timer_id = 0;
        }
        break;

    case MSG_GET_FOCUS:
        if (s_mmistk_getinkey_cnf) 
        {
            MMK_ResumeTimer(s_mmistk_getinkey_timer_id);
        }
        else
        {
            MMK_StopTimer(s_mmistk_getinkey_timer_id);
            s_mmistk_getinkey_timer_id = 0;
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmistk_getinkey_timer_id);
        s_mmistk_getinkey_timer_id = 0;
        if (s_mmistk_getinkey_cnf) 
        {
            //SIMAT_OpenShareBuf(&inkey_cnf.buf_id);
            inkey_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_getinkey_cnf = FALSE;
            s_mmistk_key_is_getting[dual_sys] = FALSE;
            SIMAT_CnfGetInkeyEx(dual_sys, &inkey_cnf);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave GetInkeyWinHandleMsg(0x%02X), s_mmistk_getinkey_cnf = %d, result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6872_112_2_18_2_56_5_172,(uint8*)"ddd", msg_id, s_mmistk_getinkey_cnf, result);
    return (result);
}

/********************************************************************************
 NAME:			GetInputWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.18
********************************************************************************/
LOCAL MMI_RESULT_E GetInputWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_GET_INPUT_CNF_T input_cnf = {0};
    MMI_STRING_T txt_info = {PNULL};
    MMI_STRING_T str_info = {PNULL};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN next_act = FALSE;
    uint8 min_len[10] = {0};
    uint8 i_len = 0;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    //SCI_TRACE_LOW:"[MMI_STK] enter GetInputWinHandleMsg, msg_id = 0x%04X, s_mmistk_getinput_cnf = %d, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6898_112_2_18_2_56_5_173,(uint8*)"ddd", msg_id, s_mmistk_getinput_cnf, s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //+CR226157
        #ifdef MAINLCD_SIZE_128X64
        {
            GUI_RECT_T edit_rect = {0};
            
            edit_rect = MMITHEME_GetClientRect();
            edit_rect.top += MMI_STATUSBAR_HEIGHT;
            GUIEDIT_SetRect(MMISTK_EDITBOX_CTRL_ID, &edit_rect);
        }
        #endif
        //-CR226157
        
        MMK_SetAtvCtrl(MMISTK_GETINPUT_WIN_ID, MMISTK_EDITBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        if (0 == s_mmistk_getinput_timer_id && s_mmistk_getinput_cnf)
        {
            s_mmistk_getinput_timer_id = MMK_CreateWinTimer(MMISTK_GETINPUT_WIN_ID, 
                                                                                                        MMISTK_GETINPUT_DURATION,
                                                                                                        FALSE);
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        if (0 != s_mmistk_getinput_timer_id)
        {
            MMK_StopTimer(s_mmistk_getinput_timer_id);
            s_mmistk_getinput_timer_id = 0;
        }

        if (s_mmistk_getinput_cnf) 
        {
            //SIMAT_OpenShareBuf(&input_cnf.buf_id);
            input_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_getinput_cnf = FALSE;
            s_mmistk_input_is_getting[dual_sys] = FALSE;
            SIMAT_CnfGetInputEx(dual_sys, &input_cnf);
            //SCI_TRACE_LOW:"[MMI_STK] GetInputWinHandleMsg, SIMAT_TR_TERMINATED_BY_USER, the result is = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6946_112_2_18_2_56_5_174,(uint8*)"d",input_cnf.result);
        }
        result = MMI_RESULT_FALSE;		// need additional process
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (s_mmistk_getinput_cnf) 
        {
            //SIMAT_OpenShareBuf(&input_cnf.buf_id);
            input_cnf.result = SIMAT_TR_BACK_MOVE;
            input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_getinput_cnf = FALSE;
            s_mmistk_input_is_getting[dual_sys] = FALSE;
            SIMAT_CnfGetInputEx(dual_sys, &input_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_GETINPUT_WIN;
            //SCI_TRACE_LOW:"[MMI_STK] GetInputWinHandleMsg, SIMAT_TR_BACK_MOVE, the result is = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_6961_112_2_18_2_56_5_175,(uint8*)"d",input_cnf.result);
        }
        if (0 != s_mmistk_getinput_timer_id)
        {
            MMK_StopTimer(s_mmistk_getinput_timer_id);
            s_mmistk_getinput_timer_id = 0;
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_SK_SKB_ICON_PRESSED:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);

				break;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                MMK_SendMsg(win_id, MSG_CTL_MIDSK, PNULL);

				break;
            }
        }
#endif //IGNORE9527
#endif

        if (s_mmistk_getinput_cnf)
        {
#ifndef GCF_TEST 
            GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &str_info);
            if (0 == str_info.wstr_len)
            {
                MMIPUB_OpenAlertFailWin(TXT_INPUT_ERROR);
                break;
            }
#endif             
            if (s_mmistk_is_input_help_info
                && (MSG_CTL_MIDSK == msg_id || MSG_APP_WEB == msg_id || MSG_SK_SKB_ICON_PRESSED == msg_id))
            {
                if (0 != s_mmistk_getinput_timer_id)
                {
                    MMK_StopTimer(s_mmistk_getinput_timer_id);
                    s_mmistk_getinput_timer_id = 0;
                }
                s_mmistk_getinput_cnf = FALSE;
                s_mmistk_input_is_getting[dual_sys] = FALSE;
                input_cnf.result = SIMAT_TR_HELP_INFO;
                input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

                SIMAT_CnfGetInputEx(dual_sys, &input_cnf);
            }
            else
            {
                if (!s_mmistk_input_is_echo) 
                {
                    GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &str_info);
                    if (str_info.wstr_len >= s_mmistk_input_length.min_len && 
                        str_info.wstr_len <= s_mmistk_input_length.max_len) 
                    {
                        s_mmistk_inputdata = str_info;
                        //s_mmistk_inputdata.is_ucs2 = FALSE;
                        //MMI_MEMCPY(s_mmistk_inputdata.str_ptr, MMISTK_STRING_MAX_LEN, str_info.str_ptr, str_info.str_len, str_info.str_len);
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    //SCI_TRACE_LOW:"[MMI_STK] next_act = %d, str_info.wstr_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7029_112_2_18_2_56_6_176,(uint8*)"dd",next_act, str_info.wstr_len);
                }
                else if (s_mmistk_input_type == SIMAT_INPUT_DIGIT_ONLY) //@songbin.zeng cr49315
                {
                    GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &str_info);
                    if (str_info.wstr_len >= s_mmistk_input_length.min_len && 
                        str_info.wstr_len <= s_mmistk_input_length.max_len) 
                    {
                        s_mmistk_inputdata = str_info;
                        //s_mmistk_inputdata.is_ucs2 = FALSE;
                        //MMI_MEMCPY(s_mmistk_inputdata.str_ptr, MMISTK_STRING_MAX_LEN, str_info.str_ptr, str_info.str_len, str_info.str_len);
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    //SCI_TRACE_LOW:"[MMI_STK] next_act = %d, str_info.wstr_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7047_112_2_18_2_56_6_177,(uint8*)"dd",next_act, str_info.wstr_len);
                }
                else if (s_mmistk_input_type == SIMAT_INPUT_DUAL_DIGIT) 
                {
                    GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &str_info);
                    if (str_info.wstr_len >= s_mmistk_input_length.min_len 
                        && str_info.wstr_len <= s_mmistk_input_length.max_len) 
                    {
                        s_mmistk_inputdata = str_info;
                        //s_mmistk_inputdata.is_ucs2 = FALSE;
                        
                        //MMI_MEMCPY(s_mmistk_inputdata.str_ptr, MMISTK_STRING_MAX_LEN, str_info.str_ptr, str_info.str_len, str_info.str_len);
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    //SCI_TRACE_LOW:"[MMI_STK] next_act = %d, str_info.wstr_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7066_112_2_18_2_56_6_178,(uint8*)"dd",next_act, str_info.wstr_len);
                }
                else 
                {
                    GUIEDIT_GetString(MMISTK_EDITBOX_CTRL_ID, &txt_info);
                    // note: because STK layer look the max_len as word number, 
                    // but  MMI 's edit component look as byte number, 
                    // so need to look the max_len as (max_len * 2).
                    // when user change input method to ABC or English, the input length is 
                    // limited by component EditBox as (max_len).
                    if (txt_info.wstr_len >= s_mmistk_input_length.min_len && 
                        txt_info.wstr_len <= s_mmistk_input_length.max_len) 
                    {
                        s_mmistk_inputdata.wstr_len = txt_info.wstr_len;

                        MMI_WSTRNCPY(s_mmistk_inputdata.wstr_ptr,
                            MMISTK_STRING_MAX_LEN,
                            txt_info.wstr_ptr,
                            txt_info.wstr_len,
                            txt_info.wstr_len);
                        next_act = TRUE;
                    }
                    else
                    {
                        next_act = FALSE;
                    }
                    //SCI_TRACE_LOW:"[MMI_STK] next_act = %d, txt_info.str_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7093_112_2_18_2_56_6_179,(uint8*)"dd",next_act, txt_info.wstr_len);
                }
                if (next_act) 
                {
                    if (0 != s_mmistk_getinput_timer_id)
                    {
                        MMK_StopTimer(s_mmistk_getinput_timer_id);
                        s_mmistk_getinput_timer_id = 0;
                    }
                    //SIMAT_OpenShareBuf(&input_cnf.buf_id);
                    //input_cnf.text_str.text_ptr = SIMAT_AllocFromShareBuf(input_cnf.buf_id, 256);
                    MMIStr2SIMATStr(
                                                &(input_cnf.text_str), 
                                                MN_MAX_SIMAT_DATA_OBJECT_LEN,
                                                &s_mmistk_inputdata, 
                                                s_mmistk_input_is_pack_req
                                                );
                    s_mmistk_getinput_cnf = FALSE;
                    s_mmistk_input_is_getting[dual_sys] = FALSE;
                    if (s_mmistk_input_is_icon_id)
                    {
//                        input_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                        input_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
                    }
                    else
                    {
                        input_cnf.result = SIMAT_TR_SUCCESS;
                    }
                    input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

                    SIMAT_CnfGetInputEx(dual_sys, &input_cnf);
                    //SCI_TRACE_LOW:"[MMI_STK] after MMIStr2SIMATStr, s_mmistk_input_is_pack_req = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7124_112_2_18_2_56_6_180,(uint8*)"d", s_mmistk_input_is_pack_req);
                    s_mmistk_close_window = MMISTK_CLOSE_GETINPUT_WIN;
                }
                else
                {
                    MMI_STRING_T mmistk_msgbox_string = {0};
                    wchar alert_string[MMISTK_STRING_MAX_LEN + 1] = {0};
                    wchar min_len_wchar[10] = {0};

                    MMI_GetLabelTextByLang(TXT_STK_INPUT_MIN, &mmistk_msgbox_string);

                    MMIAPICOM_Wstrncpy(alert_string, mmistk_msgbox_string.wstr_ptr,
                        mmistk_msgbox_string.wstr_len);

                    /* ???? ���ﲻ������ڴ�й©��???? */
                    mmistk_msgbox_string.wstr_ptr = alert_string;
                    // get the min length of input string
                    SCI_MEMSET(min_len, 0, 10);
                    sprintf((char*)min_len, "%d", s_mmistk_input_length.min_len);
                    i_len = SCI_STRLEN((char*)min_len);

                    MMI_STRNTOWSTR(min_len_wchar,
                                                10,
                                                (uint8 *)min_len,
                                                10,
                                                i_len
                                                );

                    MMIAPICOM_Wstrcat(alert_string, min_len_wchar);

                    mmistk_msgbox_string.wstr_len += i_len;
                    MMISTK_OpenAlertWinByTextPtr(&mmistk_msgbox_string);

                    //MMIPUB_OpenAlertWinByTextPtr(PNULL,&mmistk_msgbox_string,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
        }        
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmistk_getinput_timer_id) 
        {
            s_mmistk_getinput_timer_id = 0;
            //SCI_TRACE_LOW:"[MMI_STK] after timer duration %d, Timer(%d) arrived"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7167_112_2_18_2_56_6_181,(uint8*)"dd", MMISTK_GETINPUT_DURATION, s_mmistk_getinput_timer_id);
            if (s_mmistk_getinput_cnf) 
            {
                //SIMAT_OpenShareBuf(&(input_cnf.buf_id));
                input_cnf.result = SIMAT_TR_NO_RESPONSE;
                input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                s_mmistk_getinput_cnf = FALSE;
                s_mmistk_input_is_getting[dual_sys] = FALSE;
                SIMAT_CnfGetInputEx(dual_sys, &input_cnf);
            }
            s_mmistk_close_window = MMISTK_CLOSE_GETINPUT_WIN;

            MMISTK_OpenAlertWin(TXT_STK_OVERTIME_RETURN, IMAGE_PUBWIN_WARNING, PNULL);
      }
        else
        {
            result = MMI_RESULT_FALSE;		// need additional process
        }
        break;

    case MSG_LOSE_FOCUS:
        if (0 != s_mmistk_getinput_timer_id)
        {
            MMK_StopTimer(s_mmistk_getinput_timer_id);
            s_mmistk_getinput_timer_id = 0;
        }
        break;

    case MSG_GET_FOCUS:
        if (0 != s_mmistk_getinput_timer_id)
        {
            MMK_StartTimer(
                                        s_mmistk_getinput_timer_id, 
                                        MMISTK_GETINPUT_DURATION, 
                                        FALSE
                                        );
        }
        else
        {
            s_mmistk_getinput_timer_id = MMK_CreateTimer(MMISTK_GETINPUT_DURATION, FALSE);
        }                   
        break;

    case MSG_CLOSE_WINDOW:
        if (0 != s_mmistk_getinput_timer_id)
        {
            MMK_StopTimer(s_mmistk_getinput_timer_id);
            s_mmistk_getinput_timer_id = 0;
        }
        if (s_mmistk_getinput_cnf) 
        {
            //SIMAT_OpenShareBuf(&(input_cnf.buf_id));
            input_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_getinput_cnf = FALSE;
            s_mmistk_input_is_getting[dual_sys] = FALSE;
            SIMAT_CnfGetInputEx(dual_sys, &input_cnf);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave GetInputWinHandleMsg(0x%02X), s_mmistk_getinput_cnf = %d, result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7232_112_2_18_2_56_6_182,(uint8*)"ddd", msg_id, s_mmistk_getinput_cnf, result);
    return (result);
}

/********************************************************************************
 NAME:			SendSmsWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.27
********************************************************************************/
LOCAL MMI_RESULT_E SendSmsWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_SEND_SMS_CNF_T sendsms_cnf = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE, 
                                                                        SIMAT_NP_NO_SPECIFIC_CAUSE, SIMAT_SMSP_NO_SPECIFIC_CAUSE, SIMAT_CS_CONTROL_NO_SPECIFIC_CAUSE};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"[MMI_STK] enter SendSmsWinHandleMsg, msg_id = 0x%04X, s_mmistk_sendsms_cnf = %d, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7254_112_2_18_2_56_6_183,(uint8*)"ddd", msg_id, s_mmistk_sendsms_cnf[dual_sys], s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);//not return
#endif
        MMK_SetAtvCtrl(MMISTK_SENDSM_WIN_ID, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        MMK_CloseWin(MMISTK_SENDSM_WIN_ID); //cr190965
        result = MMI_RESULT_TRUE;
        break;

    case MSG_CLOSE_WINDOW:
        if (s_mmistk_sendsms_cnf[dual_sys]) 
        {
#ifndef MMI_DUALMODE_ENABLE
            MNSMS_CancelMoSmsEx(dual_sys);
#endif

            sendsms_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            sendsms_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_sendsms_cnf[dual_sys] = FALSE;
            s_mmistk_sms_is_sending[dual_sys] = FALSE;
            SIMAT_CnfSendSmsEx(dual_sys, &sendsms_cnf);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave SendSmsWinHandleMsg(0x%02X), s_mmistk_sendsms_cnf = %d, result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7302_112_2_18_2_56_6_184,(uint8*)"ddd", msg_id, s_mmistk_sendsms_cnf[dual_sys], result);
    return (result);
}

/********************************************************************************
 NAME:			PlayToneWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.10.13
********************************************************************************/
LOCAL MMI_RESULT_E PlayToneWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_PLAY_TONE_CNF_T playtone_cnf = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"[MMI_STK] enter PlayToneWinHandleMsg, msg_id = 0x%04X, s_mmistk_playtone_cnf = %d, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7323_112_2_18_2_56_6_185,(uint8*)"ddd", msg_id, s_mmistk_playtone_cnf, s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(MMISTK_PLAYTONE_WIN_ID, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        break;

    case MSG_APP_FLIP:	
    case MSG_APP_RED:
        MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
        MMK_StopTimer(s_mmistk_playtone_timer_id);
        s_mmistk_playtone_timer_id = 0;
        if (s_mmistk_playtone_cnf) 
        {
            playtone_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            playtone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_playtone_cnf = FALSE;
            s_mmistk_tone_is_playing[dual_sys] = FALSE;
            SIMAT_CnfPlayToneEx(dual_sys, &playtone_cnf);
        }
        s_mmistk_close_window = MMISTK_CLOSE_PLAYTONE_WIN;
        result = MMI_RESULT_FALSE;		// need additional process
        break;

    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
        MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
        MMK_StopTimer(s_mmistk_playtone_timer_id);
        s_mmistk_playtone_timer_id = 0;
        if (s_mmistk_playtone_cnf) 
        {
            playtone_cnf.result = SIMAT_TR_TERMINATED_BY_USER; 
            playtone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_playtone_cnf = FALSE;
            s_mmistk_tone_is_playing[dual_sys] = FALSE;
            SIMAT_CnfPlayToneEx(dual_sys, &playtone_cnf);
        }
        s_mmistk_close_window = MMISTK_CLOSE_PLAYTONE_WIN;
        break;

    case MSG_LOSE_FOCUS:
        if (s_mmistk_playtone_cnf) 
        {
            MMK_PauseTimer(s_mmistk_playtone_timer_id);
        }
        else
        {
            MMK_StopTimer(s_mmistk_playtone_timer_id);
            s_mmistk_playtone_timer_id = 0;
        }
        break;

    case MSG_GET_FOCUS:
        if (s_mmistk_playtone_cnf) 
        {
            MMK_ResumeTimer(s_mmistk_playtone_timer_id);
        }
        else
        {
            MMK_StopTimer(s_mmistk_playtone_timer_id);
            s_mmistk_playtone_timer_id = 0;
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
        MMK_StopTimer(s_mmistk_playtone_timer_id);
        s_mmistk_playtone_timer_id = 0;
        if (s_mmistk_playtone_cnf) 
        {
            playtone_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            playtone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            s_mmistk_playtone_cnf = FALSE;
            s_mmistk_tone_is_playing[dual_sys] = FALSE;
            SIMAT_CnfPlayToneEx(dual_sys, &playtone_cnf);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave PlayToneWinHandleMsg(0x%02X), s_mmistk_playtone_cnf = %d, result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7417_112_2_18_2_56_6_186,(uint8*)"ddd", msg_id, s_mmistk_playtone_cnf, result);
    return (result);
}

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author: gang.tong
/*****************************************************************************/
LOCAL ERR_MNSS_CODE_E MMISTK_SendReqToSs(
                                         MN_DUAL_SYS_E dual_sys,
                                         MN_SS_OPERATION_CODE_E ss_oper,
                                         MN_SS_CODE_E			ss_code,          
                                         MN_SS_BASIC_SERVICE_T	ss_basic_service,
                                         MN_SS_REGISTER_PARAM_U	ss_register_param,
                                         SIMAT_DO_SS_STRING_T      ss_string
                                         )
{
    ERR_MNSS_CODE_E ss_result = ERR_MNSS_NO_ERR;

    switch (ss_oper)
    {
    case MN_SS_REGISTERSS:
    case MN_SS_ERASESS:
    case MN_SS_ACTIVATESS:
    case MN_SS_DEACTIVATESS:
    case MN_SS_REGISTERPASSWORD:
        break;

    case MN_SS_INTERROGATESS:
        //!!!look up service, should all service, don't about service code!!!
        ss_basic_service.service_type = MN_SS_ALL_SERVICE;
        break;        

    default:
        //SCI_TRACE_LOW:"MMISTK_SendReqToSs:PNULL == ss_oper"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7450_112_2_18_2_56_6_187,(uint8*)"");
        break;
    }

    MMIAPISET_SetSSStatus(dual_sys, ss_code, ss_basic_service);

    SIMAT_SetOperSourceEx(dual_sys, SIMAT_OPER_SEND_SS);

    ss_result = (ERR_MNSS_CODE_E)MNSS_SendSSEx(dual_sys, ss_oper, ss_code, ss_basic_service, ss_register_param, ss_string);

    return (ss_result);
}

/********************************************************************************
 NAME:			CreateSendSSVision
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.21
********************************************************************************/
LOCAL void CreateSendSSVision(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_open = FALSE;

    if (!MMK_IsOpenWin(MMISTK_SENDSS_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_SENDSS_WIN_ID;
        win_create.func = SendSSWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        
        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_SENDSS_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_SENDSS_WIN_ID, TXT_NULL);

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_SENDSS_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#endif

        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_SENDSS_WIN_ID);

        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_SENDSS_WIN_ID;
        create.init_data_ptr = &init_data;
        
        MMK_CreateControl( &create );

        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    SetStkWinTitle(dual_sys, MMISTK_SENDSS_WIN_ID);

#ifndef MMI_PDA_SUPPORT
    GUIWIN_SetSoftkeyTextId(MMISTK_SENDSS_WIN_ID,  TXT_NULL, TXT_NULL, TXT_NULL, FALSE);
#endif

    //SCI_TRACE_LOW:"[MMI_STK] s_mmistk_displaydata.length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7547_112_2_18_2_56_7_188,(uint8*)"d", s_mmistk_displaydata.wstr_len);

    SetRichtextItemInfo(dual_sys,
                                        s_mmistk_ss_is_icon_id,
                                        s_mmistk_ss_icon_id,
                                        s_mmistk_displaydata,
                                        MMISTK_TEXBOX_CTRL_ID);
    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_SENDSS_WIN_ID);

        if (!MMK_IsFocusWin(MMISTK_SENDSS_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_SENDSS_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_SENDSS_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_SENDSS_WIN);
}

/*****************************************************************************/
//     Description : start setup call Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void StartSetupCallOperation(MN_DUAL_SYS_E dual_sys)
{
    SIMAT_SETUP_CALL_CNF_T setup_call_cnf = {0};
    CC_RESULT_E cc_result = CC_RESULT_ERROR;

    //SCI_TRACE_LOW:"[MMI_STK] StartSetupCallOperation dual_sys=%d, s_mmistk_setup_call_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7581_112_2_18_2_56_7_189,(uint8*)"dd", dual_sys, s_mmistk_setup_call_type);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    switch(s_mmistk_setup_call_type)
    {
    case SIMAT_STC_FREE:
    case SIMAT_STC_FREE_REDIAL:
        if (ExistActiveOrSetupCall() && s_is_mmistk_setup_call)
        {
            s_is_mmistk_setup_call = FALSE;
            setup_call_cnf.is_command_changed = FALSE;
            setup_call_cnf.result = SIMAT_TR_ME_UNABLE;
            setup_call_cnf.mp_info = SIMAT_MP_BUSY_ON_CALL;
            SIMAT_CnfSetupCallEx((uint8)dual_sys, &setup_call_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
            //SCI_TRACE_LOW:"[MMI_STK] SetupCallWinHandleMsg because there is in call already"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7600_112_2_18_2_56_7_190,(uint8*)"");
            return;
        }

        MMISTK_SetupCall(dual_sys, TRUE, FALSE, 0);
        break;
        
    case SIMAT_STC_HOLD:
    case SIMAT_STC_HOLD_REDIAL:
        if (ExistActiveOrSetupCall() && s_is_mmistk_setup_call)
        {
            // TODO: new cc api
            cc_result = MMICC_HoldCall();//MMIAPICC_HoldAndAnswerCW();//Hold call

            if (CC_RESULT_SUCCESS != cc_result)
            {
                s_is_mmistk_setup_call = FALSE;
                setup_call_cnf.is_command_changed = FALSE;
                setup_call_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetupCallEx((uint8)dual_sys, &setup_call_cnf);
                s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
            }
            else
            {
                s_mmistk_call_oper_type = MMISTK_SETUP_CALL_HOLD_OPER;
            }
        }
        else
        {
            MMISTK_SetupCall(dual_sys, TRUE, FALSE, 0);
        }
        break;
        
    case SIMAT_STC_DISCON:
    case SIMAT_STC_DISCON_REDIAL:
        if (ExistActiveOrSetupCall() && s_is_mmistk_setup_call)
        {
            cc_result = MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NOPROMPT);//release all call

            if (CC_RESULT_SUCCESS != cc_result)
            {
                s_is_mmistk_setup_call = FALSE;
                setup_call_cnf.is_command_changed = FALSE;
                setup_call_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetupCallEx((uint8)dual_sys, &setup_call_cnf);
                s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
            }
            else
            {
                s_mmistk_call_oper_type = MMISTK_SETUP_CALL_DISCON_OPER;
            }
        }
        else
        {
            MMISTK_SetupCall(dual_sys, TRUE, FALSE, 0);
        }
        break;

    default:
        break;
    }
}

/********************************************************************************
 NAME:			SetupCallWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.09.27
********************************************************************************/
LOCAL MMI_RESULT_E SetupCallWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_SETUP_CALL_CNF_T setup_call_cnf = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/

    //SCI_TRACE_LOW:"[MMI_STK] enter SetupCallWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7681_112_2_18_2_56_7_191,(uint8*)"dd", msg_id, s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        break;

    case MSG_APP_FLIP:
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
        if (s_is_mmistk_setup_call)
        {
            s_is_mmistk_setup_call = FALSE;
            setup_call_cnf.result = SIMAT_TR_USER_DENY_CALL;                   
            setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            SIMAT_CnfSetupCallEx(dual_sys, (SIMAT_SETUP_CALL_CNF_T*)&setup_call_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //IGNORE9527
#endif
        StartSetupCallOperation(dual_sys);
        break;

    case MSG_LOSE_FOCUS:
        if(MMICC_IsExistIncommingCall())
        {
            SCI_TRACE_LOW("[MMI_STK] SetupCallWinHandleMsg :exist incomming call");
            MMK_CloseWin(win_id);//1343801
        }
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_CLOSE_WINDOW:
        if (s_is_mmistk_setup_call)
        {
            s_is_mmistk_setup_call = FALSE;
            setup_call_cnf.result = SIMAT_TR_USER_DENY_CALL;                   
            setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            SIMAT_CnfSetupCallEx(dual_sys, (SIMAT_SETUP_CALL_CNF_T*)&setup_call_cnf);
        }
        MMIAPICC_CloseDisconnectWin();
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    //SCI_TRACE_LOW:"[MMI_STK] leave SetupCallWinHandleMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7742_112_2_18_2_56_7_192,(uint8*)"");
    return (result);
}

/*****************************************************************************/
//  Description : Is Setup Call Duration
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_GetSetupCallDuration(void)
{
    if (0 != s_mmistk_setupcall_duration_timer_id
         && (SIMAT_STC_FREE_REDIAL == s_mmistk_setup_call_type
                || SIMAT_STC_HOLD_REDIAL == s_mmistk_setup_call_type
                || SIMAT_STC_DISCON_REDIAL == s_mmistk_setup_call_type))
    {
        return TRUE;
    }

    return FALSE;
}

/********************************************************************************
 NAME:      MMISTK_SetupCall
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMISTK_SetupCall(MN_DUAL_SYS_E dual_sys, 
                                BOOLEAN is_oper_cnf,
                                BOOLEAN is_call_active,
                                MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
    SIMAT_SETUP_CALL_CNF_T setup_call_cnf = {0};
    CC_RESULT_E cc_result = CC_RESULT_ERROR;
    MN_SUBADDR_T* psub_addr = PNULL;

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_SetupCall, s_is_mmistk_setup_call=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7777_112_2_18_2_56_7_193,(uint8*)"d", s_is_mmistk_setup_call);
    if(!s_is_mmistk_setup_call)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_SetupCall, is_oper_cnf=%d,is_call_active=%d,s_mmistk_call_oper_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7783_112_2_18_2_56_7_194,(uint8*)"ddd", is_oper_cnf, is_call_active, s_mmistk_call_oper_type);

    s_is_mmistk_setup_call = FALSE;

    if (is_oper_cnf)
    {
        if (!is_call_active && (MMISTK_SETUP_CALL_NO_OPER != s_mmistk_call_oper_type))
        {
            if( s_is_setup_call_cps && ( 0 != s_mmistk_sub_addr.addr_len) )
            {
                psub_addr = &s_mmistk_sub_addr;
            }

            s_mmistk_call_oper_type = MMISTK_SETUP_CALL_NO_OPER;

            SIMAT_SetOperSourceEx(dual_sys, SIMAT_OPER_SETUP_CALL);

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
            if (s_mmistk_setup_call_is_cs_icon_id && s_mmistk_setup_call_cs_icon_id.is_self_explanatory)
            {
                cc_result = MMIAPICC_MakeCall(
                                                dual_sys,
                                                s_mmistk_setup_call_str,
                                                s_mmistk_setup_call_str_len,
                                                PNULL,
                                                PNULL,
                                                MMIAPICC_GetCallSimType(dual_sys),
                                                CC_CALL_STK_CALL,
                                                psub_addr
                                                );
            }
            else
#endif
            {
                cc_result = MMIAPICC_MakeCall(
                                                dual_sys,
                                                s_mmistk_setup_call_str,
                                                s_mmistk_setup_call_str_len,
                                                &s_mmistk_displaydata,
                                                PNULL,
                                                MMIAPICC_GetCallSimType(dual_sys),
                                                CC_CALL_STK_CALL,
                                                psub_addr
                                                );
            }

            if (CC_RESULT_SUCCESS == cc_result)
            {
                s_is_mmistk_setup_call = TRUE;

                if (0 != s_mmistk_setupcall_duration_timer_id)
                {
                    MMK_StopTimer(s_mmistk_setupcall_duration_timer_id);
                }

                if (s_is_setup_call_duration && (0 != s_mmistk_setup_call_duration)
                    && (SIMAT_STC_FREE_REDIAL == s_mmistk_setup_call_type
                        || SIMAT_STC_FREE_REDIAL == s_mmistk_setup_call_type
                        || SIMAT_STC_DISCON_REDIAL == s_mmistk_setup_call_type))
                {
                    s_mmistk_setupcall_duration_timer_id = MMK_CreateTimerCallback(s_mmistk_setup_call_duration, MMISTK_HandleSTKTimer, NULL, FALSE);
                }

                if (s_mmistk_setup_call_is_cs_icon_id)
                {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
                    MN_SIM_IMG_T sim_img = {0};
                    GUIIMG_BITMAP_T icon_bitmap = {0};

                    MNSIM_GetImgEx(dual_sys, s_mmistk_setup_call_cs_icon_id.icon_id, &sim_img);

                    if (DecodeSimImgToBmp(&sim_img, &icon_bitmap))
                    {
                        GUILABEL_DATA_IMAGE_U img_data = {0};

                        s_mmistk_icon_display_result = TRUE;

                        img_data.buf.buf_ptr = icon_bitmap.bit_ptr;
                        img_data.buf.size = icon_bitmap.img_width * icon_bitmap.img_height * 2;
                        img_data.buf.width = icon_bitmap.img_width;
                        img_data.buf.height = icon_bitmap.img_height;

#ifdef MMI_PDA_SUPPORT
                        GUILABEL_SetIconEx(MMICC_CONNECTED_STATE_LABEL_CTRL_ID,
                                                        TRUE, GUILABEL_IMAGE_BUF, img_data);
#else
                        GUILABEL_SetIconEx(MMICC_CALLING_STATE_LABEL_CTRL_ID,
                                                        TRUE, GUILABEL_IMAGE_BUF, img_data);
#endif
                    }
                    else
                    {
                        s_mmistk_icon_display_result = FALSE;
                    }

                    if (PNULL != icon_bitmap.bit_ptr)
                    {
                        SCI_FREE(icon_bitmap.bit_ptr);
                    }
#endif
#else
                    s_mmistk_icon_display_result = FALSE;
#endif
                }
            }
            else
            {
                SIMAT_SetOperSourceEx(dual_sys, SIMAT_OPER_NULL);

                setup_call_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

                SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
            }
        }
        else
        {
            s_is_mmistk_setup_call = FALSE;


            if (0 != s_mmistk_setupcall_duration_timer_id)
            {
                MMK_StopTimer(s_mmistk_setupcall_duration_timer_id);
            }
            
            if( s_is_setup_call_cps && ( 0 != s_mmistk_sub_addr.addr_len ) )
            {
                s_is_setup_call_cps = FALSE;
                setup_call_cnf.cc_result.cps.cps_ptr = s_mmistk_sub_addr.sub_num;

#ifndef PRODUCT_DM 
                setup_call_cnf.cc_result.cps.len = s_mmistk_sub_addr.addr_len;
#endif
            }

            if (is_call_active)
            {
                if (s_mmistk_setup_call_is_uc_icon_id || s_mmistk_setup_call_is_cs_icon_id)
                {
//                    setup_call_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                    setup_call_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
                }
                else
                {
                    setup_call_cnf.result = SIMAT_TR_SUCCESS;
                }

                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }
            else
            {
                setup_call_cnf.result = SIMAT_TR_USER_DENY_CALL;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }

            SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
        }
    }
    else
    {
        if (!MMIAPISTK_GetSetupCallDuration())
        {
            if (MN_CAUSE_SIM_STK_NOT_ALLOWED == disc_cause)
            {
                setup_call_cnf.result = SIMAT_TR_PERM_PROBLEM_INN_CC;
            }
            else
            {
                setup_call_cnf.result = SIMAT_TR_NE_UNABLE;
            }
            setup_call_cnf.ne_info = (SIMAT_NE_PROBLEM_E)(disc_cause | 0x80);//Facility Rejected
            setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

            SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
            s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;
        }
        else
        {
            s_is_mmistk_setup_call = TRUE;
        }
    }

    return TRUE;
}

/********************************************************************************
 NAME:			SendSSWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		gang.tong
 DATE:			2005.11.21
********************************************************************************/
LOCAL MMI_RESULT_E SendSSWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    //SCI_TRACE_LOW:"[MMI_STK] enter SendSSWinHandleMsg, msg_id = 0x%04X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_7988_112_2_18_2_56_7_195,(uint8*)"d", msg_id);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(MMISTK_SENDSS_WIN_ID, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_APP_FLIP:		// close flip
#endif    
    case MSG_APP_RED:
        result = MMI_RESULT_FALSE;		// need additional process
        // when SMS was sent to MN layer, this window must be closed by MN layer 's feedback signal
        s_mmistk_close_window = MMISTK_CLOSE_SENDSS_WIN;
        break;

    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        // do nothing
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    //SCI_TRACE_LOW:"[MMI_STK] leave SendSSWinHandleMsg(0x%02X), result(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8026_112_2_18_2_56_8_196,(uint8*)"dd", msg_id, result);
    return (result);
}

/********************************************************************************
 NAME:			MMISTK_OnSendSSCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMISTK_OnSendSSCnf(void *arg)
{
    SIMAT_SEND_SS_CNF_T ss_cnf = {0};

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendSSCnf, s_mmistk_sendss_waiting_respond= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8041_112_2_18_2_56_8_197,(uint8*)"d", s_mmistk_sendss_waiting_respond);
    if(PNULL ==arg)
    {
        //SCI_TRACE_LOW:"MMISTK_OnSendSSCnf:PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8044_112_2_18_2_56_8_198,(uint8*)"");
        return FALSE;
    }
    if(!s_mmistk_sendss_waiting_respond)
    {
        return FALSE;
    }
    s_mmistk_sendss_waiting_respond = FALSE;

    switch( ((APP_MN_REGISTER_SS_CNF_T*)(arg))->result_type )
    {
    case MN_SS_OPERATION_SUCC:		
        //SCI_TRACE_LOW:"send ss success: MN_SS_OPERATION_SUCC result_octs_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8056_112_2_18_2_56_8_199,(uint8*)"d", ((APP_MN_REGISTER_SS_CNF_T*)(arg))->result_octs_len);
#ifdef MMI_DUALMODE_ENABLE
        ss_cnf.no_add_data = ((APP_MN_REGISTER_SS_CNF_T*)(arg))->result_octs_len;	
        ss_cnf.add_data = (uint8 *)(((APP_MN_REGISTER_SS_CNF_T*)(arg))->result_octs);
#endif

        if (s_mmistk_ss_is_icon_id)
        {
//            ss_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
            ss_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
        }
        else
        {
            ss_cnf.result = SIMAT_TR_SUCCESS;
        }

        ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;	
        break;

    case MN_SS_OPERATION_REJECT:
        //SCI_TRACE_LOW:"send ss failure: MN_SS_OPERATION_REJECT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8076_112_2_18_2_56_8_200,(uint8*)"");
        ss_cnf.result = SIMAT_TR_SS_ERROR;		
        break;

    case MN_SS_OPERATION_ERROR:
        //SCI_TRACE_LOW:"send ss failure: MN_SS_OPERATION_ERROR,The error code is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8081_112_2_18_2_56_8_201,(uint8*)"d",((APP_MN_REGISTER_SS_CNF_T*)(arg))->err_code);
        ss_cnf.result = SIMAT_TR_SS_ERROR;
        ss_cnf.ss_info = (SIMAT_SS_PROBLEM_E)((APP_MN_REGISTER_SS_CNF_T*)(arg))->err_code;
        break;

    default:
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8087_112_2_18_2_56_8_202,(uint8*)"");
        break;
    }

    SIMAT_CnfSendSsEx(((APP_MN_REGISTER_SS_CNF_T*)(arg))->dual_sys, &ss_cnf);
    return TRUE;
}

/********************************************************************************
 NAME:			MMISTK_OnSendUSSCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMISTK_OnSendUSSDCnf(void *arg)
{
    APP_MN_USSD_SERVICE_CNF_T ussd_service_cnf = *((APP_MN_USSD_SERVICE_CNF_T*)arg);
    SIMAT_SEND_USSD_CNF_T ussd_cnf = {0};
    BOOLEAN result = TRUE;

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf, s_mmistk_sendussd_waiting_respond= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8108_112_2_18_2_56_8_203,(uint8*)"d", s_mmistk_sendussd_waiting_respond);

    if(!s_mmistk_sendussd_waiting_respond)
    {
        return FALSE;
    }
    
    s_mmistk_sendussd_waiting_respond = FALSE;

    if (s_mmistk_sendss_waiting_respond)
    {
        SIMAT_SEND_SS_CNF_T ss_cnf = {0};

        s_mmistk_sendss_waiting_respond = FALSE;

        switch (ussd_service_cnf.result_type)
        {
        case MN_SS_OPERATION_SUCC:
            if (s_mmistk_ss_is_icon_id)
            {
//                ss_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                ss_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                ss_cnf.result = SIMAT_TR_SUCCESS;
            }
            ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_SUCC"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8136_112_2_18_2_56_8_204,(uint8*)"");
            break;

        case MN_SS_OPERATION_ERROR:
            ss_cnf.result = SIMAT_TR_SS_ERROR;
            ss_cnf.ss_info = (SIMAT_SS_PROBLEM_E)ussd_service_cnf.err_code;
            ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;	
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_ERROR, ussd info is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8143_112_2_18_2_56_8_205,(uint8*)"d",ussd_service_cnf.err_code);
            break;

        case MN_SS_OPERATION_REJECT:
            ss_cnf.result = SIMAT_TR_SS_ERROR;
            ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;	
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_REJECT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8149_112_2_18_2_56_8_206,(uint8*)"");
            break;

        default:
            //SCI_TRACE_LOW:"result type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8153_112_2_18_2_56_8_207,(uint8*)"d", ussd_service_cnf.result_type);
            break;
        }

        SIMAT_CnfSendSsEx((uint8)ussd_service_cnf.dual_sys, &ss_cnf);
    }
    else
    {
        switch (ussd_service_cnf.result_type)
        {
        case MN_SS_OPERATION_SUCC:
            if (s_mmistk_ussd_is_icon_id)
            {
//                ussd_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                ussd_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                ussd_cnf.result = SIMAT_TR_SUCCESS;
            }
            ussd_cnf.ussd_return = ussd_service_cnf.ussd_data;
            result = TRUE;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_SUCC"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8175_112_2_18_2_56_8_208,(uint8*)"");
            break;

        case MN_SS_OPERATION_ERROR:
            ussd_cnf.result = SIMAT_TR_USSD_ERROR;
            ussd_cnf.ussd_info = (SIMAT_USSD_PROBLEM_E)ussd_service_cnf.err_code;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_ERROR, ussd info is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8181_112_2_18_2_56_8_209,(uint8*)"d",ussd_service_cnf.err_code);
            break;

        case MN_SS_OPERATION_REJECT:
            ussd_cnf.result = SIMAT_TR_USSD_ERROR;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendUSSDCnf: MN_SS_OPERATION_REJECT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8186_112_2_18_2_56_8_210,(uint8*)"");
            break;

        default:
            //SCI_TRACE_LOW:"result type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8190_112_2_18_2_56_8_211,(uint8*)"d", ussd_service_cnf.result_type);
            break;
        }
    }

    SIMAT_CnfSendUssdEx((uint8)ussd_service_cnf.dual_sys, &ussd_cnf);

    MMIAPIUSSD_CloseSendWin();

    return result;
}

/********************************************************************************
 NAME:			MMISTK_OnSendRegisterPaswdCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMISTK_OnSendRegisterPaswdCnf(void *arg)
{
    SIMAT_SEND_SS_CNF_T ss_cnf = {0};
    APP_MN_REGISTER_PASSWORD_CNF_T* reg_pd_cnf_ptr = (APP_MN_REGISTER_PASSWORD_CNF_T*)(arg);

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"result MMISTK_OnSendRegisterPaswdCnf ::PNULL == arg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8216_112_2_18_2_56_8_212,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendRegisterPaswdCnf, s_mmistk_sendss_waiting_respond= %d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8220_112_2_18_2_56_8_213,(uint8*)"dd", s_mmistk_sendss_waiting_respond, reg_pd_cnf_ptr->dual_sys);
    if(!s_mmistk_sendss_waiting_respond)
    {
        return FALSE;
    }

    s_mmistk_sendss_waiting_respond = FALSE;
    switch( ((APP_MN_REGISTER_PASSWORD_CNF_T*)(arg))->result_type )
    {
        case MN_SS_OPERATION_SUCC:
            //SCI_TRACE_LOW:"[MMI_STK] send ss success"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8230_112_2_18_2_56_8_214,(uint8*)"");
            ss_cnf.result = SIMAT_TR_SUCCESS;
            ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;	
            break;

        default:
            //SCI_TRACE_LOW:"[MMI_STK] send ss failure"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8236_112_2_18_2_56_8_215,(uint8*)"");
            ss_cnf.result = SIMAT_TR_BEYOND_ME_CAP;
            ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
    }

    SIMAT_CnfSendSsEx((uint8)reg_pd_cnf_ptr->dual_sys, &ss_cnf);
    
    return TRUE;
}

/********************************************************************************
 NAME:			OnSetupMenuCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		bown.zhang
 DATE:			2007.07.11
********************************************************************************/
LOCAL void OnSetupMenuInit(MN_DUAL_SYS_E dual_sys, SIMAT_SETUP_MENU_T* param_in, STK_MENU_INFO_T* toplevel_menu_info)
{
    uint8 menuitem_offset;
    SIMAT_ITEMTEMPLATE_T menuitem = {0};
    MMI_STRING_T menu_title = {PNULL, 0};
    SIMAT_SETUP_MENU_CNF_T param_out = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};
    uint16  i = 0;

    toplevel_menu_info->is_action_id = FALSE;//cr195474 param_in->is_action_id;
    toplevel_menu_info->is_icon_id = param_in->is_icon_id;
    toplevel_menu_info->is_icon_list = param_in->is_icon_list;
    toplevel_menu_info->help_info = param_in->help_info;

    if (param_in->is_icon_id)		// STK menu group's icon id
    {
        toplevel_menu_info->menu_title_icon_id = param_in->icon_id.icon_id;
        toplevel_menu_info->is_menu_title_icon_self_explan = param_in->icon_id.is_self_explanatory;
    }	
    if (param_in->is_icon_list)
    {
        toplevel_menu_info->is_icon_list_self_explan = param_in->icon_list.is_self_explanatory;
    }

    // total menu item count
    toplevel_menu_info->menu_item_amount = MIN(param_in->item.item_data_object_num, MMISTK_MENU_MAX_ITEM);
    //SCI_TRACE_LOW:"MMISTK_OnSetupMenuCmd: toplevel_menu_info.menu_item_amount %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8279_112_2_18_2_56_8_216,(uint8*)"d", toplevel_menu_info->menu_item_amount);

    CreateSTKBufferMemory(&toplevel_menu_info->menu_item, sizeof(STK_MENU_ITEM_T) * toplevel_menu_info->menu_item_amount);

    for (i = 0; i < toplevel_menu_info->menu_item_amount; i++)
    {
        menuitem.id = param_in->item.item_data_object_info[i][0];
        menuitem.len = param_in->item.item_data_object_info[i][1];
        menuitem_offset = param_in->item.item_data_object_info[i][2];
        SCI_MEMCPY(menuitem.item_content_entry_buf, param_in->item.item_data_object_buf+menuitem_offset,
                            menuitem.len);

        // menu ID
        toplevel_menu_info->menu_item[i].itemID = menuitem.id;
        // menu name 
        SCI_MEMSET(toplevel_menu_info->menu_item[i].stringData, 0, sizeof(toplevel_menu_info->menu_item[i].stringData));
        menu_title.wstr_ptr = toplevel_menu_info->menu_item[i].stringData;

        SimItem2MMIStr(&menuitem, &menu_title,
            MMISTK_MENU_ITEMSTR_LEN + 1);
        toplevel_menu_info->menu_item[i].length = menu_title.wstr_len;
        
        if (param_in->is_action_id)
        {
            toplevel_menu_info->menu_item[i].nextActionIndicator = param_in->next_action.next_action_list[i];
        }
        if (param_in->is_icon_list)	
        {
            toplevel_menu_info->menu_item[i].iconID = param_in->icon_list.icon_ids[i];
        }
    }

    // if STK menu is not empty, put STK flag for main menu
    if (toplevel_menu_info->menu_item_amount > 0 )
    {
        if ((toplevel_menu_info->menu_item_amount == 1) && (toplevel_menu_info->menu_item[0].length == 0))
        {
            s_simtk_is_detected[dual_sys] = FALSE;
        } 
        else
        {
            s_simtk_is_detected[dual_sys] = TRUE;
        }
        
    }
    else
    {
        s_simtk_is_detected[dual_sys] = FALSE;
    }

    // menu title name
    //SCI_TRACE_LOW:"MMISTK_OnSetupMenuCmd: the alpha_id.len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8322_112_2_18_2_56_8_217,(uint8*)"d", param_in->alpha_id.len);
    if (param_in->alpha_id.len > 0)
    {
        SCI_MEMSET(toplevel_menu_info->menu_title, 0, sizeof(toplevel_menu_info->menu_title));
        menu_title.wstr_ptr = (wchar *)toplevel_menu_info->menu_title;

        AlphaId2MMIStr(&param_in->alpha_id, &menu_title, MMISTK_MENU_TITLE_LEN);

        //SCI_TRACE_LOW:"[MMI_STK] menu_title.wstr_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8330_112_2_18_2_56_8_218,(uint8*)"d", menu_title.wstr_len);

        toplevel_menu_info->menu_title_len = menu_title.wstr_len;
        
        //////////////////////////////////////////
        // pre-evaluate toplevel menu title to menu title
        //////////////////////////////////////////
        //s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_len = toplevel_menu_info->menu_title_len;
        //MMI_WSTRNCPY(&(s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title[0]), 
        //                        MMISTK_MENU_TITLE_LEN,
        //                        &toplevel_menu_info->menu_title[0], 
        //                        MMISTK_MENU_TITLE_LEN,
        //                        s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_len
        //                        );
    }

    if (param_in->is_icon_id || param_in->is_icon_list)
    {
//        param_out.result = SIMAT_TR_ICON_FAIL_DISPLAY;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
    {
        MN_SIM_IMG_T sim_img = {0};
        
        MNSIM_GetImgEx(dual_sys, toplevel_menu_info->menu_title_icon_id, &sim_img);
        
        //decode OK
        if(DecodeSimImgToBmp(&sim_img, &(toplevel_menu_info->menu_title_icon_bmp)))
        {
            param_out.result = SIMAT_TR_SUCCESS;
        }
        else
        {
            param_out.result = SIMAT_TR_ICON_FAIL_DISPLAY;
        }
    }
#endif
        //param_out.result = SIMAT_TR_SUCCESS;
#else
        param_out.result = SIMAT_TR_ICON_FAIL_DISPLAY;
#endif
    }
    else
    {
        param_out.result = SIMAT_TR_SUCCESS;
    }
    param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    SIMAT_CnfSetMenuEx(dual_sys, &param_out);
    //SCI_TRACE_LOW:"[MMI_STK] leave OnSetupMenuCmd, load %d top level menus"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8376_112_2_18_2_56_8_219,(uint8*)"d", toplevel_menu_info->menu_item_amount);
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E STKSimSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        MMIAPISTK_EnterToplevelMenu((MN_DUAL_SYS_E)dual_sys);
    }

    return (recode);
}
#endif

/*****************************************************************************/
//  Description : set current used sim for stk
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
void MMISTK_SetSTKSim(MN_DUAL_SYS_E dual_sys)
{
    s_stk_dual_sys = dual_sys;
}

/*****************************************************************************/
//  Description : get current used sim for stk
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
MN_DUAL_SYS_E MMISTK_GetSTKSim(void)
{
    return s_stk_dual_sys;
}

/********************************************************************************
 NAME:			MMISTK_IsStkCard
 DESCRIPTION:	whether STK card
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.18
********************************************************************************/
PUBLIC BOOLEAN MMISTK_IsStkCard(MN_DUAL_SYS_E dual_sys)
{
    return (s_simtk_is_detected[dual_sys]
                && MMIAPIPHONE_IsSimAvailable(dual_sys)
                && SIM_STATUS_PIN_BLOCKED != MMIAPIPHONE_GetSimStatus(dual_sys)
                && SIM_STATUS_PUK_BLOCKED != MMIAPIPHONE_GetSimStatus(dual_sys));
}

/********************************************************************************
 NAME:          MMISTK_IsStkCardEx
 DESCRIPTION:   whether STK card--Bug 1330168
 PARAM IN:      None
 PARAM OUT:	
 AUTHOR:        longwei.qiao
 DATE:          2020.06.16
********************************************************************************/
PUBLIC BOOLEAN MMISTK_IsStkCardEx(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMISTK_IsStkCardEx SimStatus is %d", MMIAPIPHONE_GetSimStatus(dual_sys));
    return (s_simtk_is_detected[dual_sys]
                && MMIAPIPHONE_IsSimCardOk(dual_sys)
                && SIM_STATUS_PIN_BLOCKED != MMIAPIPHONE_GetSimStatus(dual_sys)
                && SIM_STATUS_PUK_BLOCKED != MMIAPIPHONE_GetSimStatus(dual_sys));
}

/********************************************************************************
 NAME:			MMISTK_IsSendSms
 DESCRIPTION:	whether send SMS by STK
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.11.18
********************************************************************************/
PUBLIC BOOLEAN MMISTK_IsSendSms(void)
{
    return s_mmistk_send_sms;
}

/********************************************************************************
 NAME:			MMISTK_SendSMSPDUCnf
 DESCRIPTION:	send SMS Cnf to STK layer
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.01
********************************************************************************/
PUBLIC void MMISTK_SendSMSPDUCnf(MN_DUAL_SYS_E dual_sys, MN_SMS_CAUSE_E send_result)
{
    SIMAT_SEND_SMS_CNF_T sms_cnf = {SIMAT_TR_SUCCESS,
                                                                SIMAT_MP_NO_SPECIFIC_CAUSE, 
                                                                SIMAT_NP_NO_SPECIFIC_CAUSE, 
                                                                SIMAT_SMSP_NO_SPECIFIC_CAUSE,
                                                                SIMAT_CS_CONTROL_NO_SPECIFIC_CAUSE};
#ifndef MMI_DUALMODE_ENABLE
    MN_SEND_SMS_SMS_CAUSE_T last_fail_cause = {0};
#endif

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_SendSMSPDUCnf, s_mmistk_sendsms_cnf = %d, send_result = %d, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8457_112_2_18_2_56_8_220,(uint8*)"ddd", s_mmistk_sendsms_cnf[dual_sys], send_result, s_mmistk_close_window);
    if (s_mmistk_sendsms_cnf[dual_sys])
    {
        if (send_result == MN_SMS_OPERATE_SUCCESS)
        {
            if (s_mmistk_sms_is_icon_id)
            {
//                sms_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                sms_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                sms_cnf.result =  SIMAT_TR_SUCCESS;
            }
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_SendSMSPDUCnf, send success"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8471_112_2_18_2_56_8_221,(uint8*)"");
        }
        else if(send_result == MN_SMS_SIM_STK_NOT_ALLOWED)
        {
            sms_cnf.result =  SIMAT_TR_PERM_PROBLEM_INN_CC;
            sms_cnf.cc_info = SIMAT_CS_CONTROL_ACTION_NOT_ALLOWED_CAUSE;
        }
        else
        {
#ifndef PRODUCT_DM
            last_fail_cause = MNSMS_GetLastMoFailCauseEx(dual_sys);

            sms_cnf.result =  SIMAT_TR_SMS_ERROR;
            sms_cnf.sms_info = (SIMAT_SMS_PROBLEM_E)last_fail_cause.cause_value;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_SendSMSPDUCnf, send fail, fail code is %d, last_fail_cause=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8485_112_2_18_2_56_8_222,(uint8*)"dd",send_result, last_fail_cause.cause_value);
#else
            sms_cnf.result =  SIMAT_TR_SMS_ERROR;
            sms_cnf.sms_info = (SIMAT_SMS_PROBLEM_E)send_result;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_SendSMSPDUCnf, send fail, fail code is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8489_112_2_18_2_56_8_223,(uint8*)"d",send_result);
#endif
        }
        s_mmistk_sendsms_cnf[dual_sys] = FALSE;
        s_mmistk_sms_is_sending[dual_sys] = FALSE;
        SIMAT_CnfSendSmsEx(dual_sys, &sms_cnf);
    }

    s_mmistk_close_window = MMISTK_CLOSE_SENDSMS_WIN;
    s_mmistk_send_sms = FALSE;
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_SendSMSPDUCnf, s_mmistk_sendsms_cnf = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8499_112_2_18_2_56_9_224,(uint8*)"d", s_mmistk_sendsms_cnf[dual_sys]);
}

/********************************************************************************
 NAME:			MMISTK_EnterToplevelMenu
 DESCRIPTION:	enter STK top level menu from main menu
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMISTK_EnterToplevelMenu(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == s_mmistk_toplevel_menu_info[dual_sys])
    {
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_EnterToplevelMenu Ptr NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8513_112_2_18_2_56_9_225,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_EnterToplevelMenu dual_sys=%d menu_item_amount = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8517_112_2_18_2_56_9_226,(uint8*)"dd", dual_sys, s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount);

    if (0 == s_mmistk_toplevel_menu_info[dual_sys]->menu_item_amount || MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID)) 
    {
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_EnterToplevelMenu because top level menu items <=0 or STk Main Menu Window already open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8521_112_2_18_2_56_9_227,(uint8*)"");
        return;
    }

    //set stk sim
    MMISTK_SetSTKSim(dual_sys);

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_EnterToplevelMenu"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8528_112_2_18_2_56_9_228,(uint8*)"");

    //////////////////////////////////////
    // Create STK MainMenu Window & MainMenu
    //////////////////////////////////////
    CreateToplevelMenuVision(dual_sys, s_mmistk_toplevel_menu_info[dual_sys]);

    s_mmistk_indmenusel_cnf = TRUE;	// this means can call SIMAT_IndMenuSel() function
    s_mmistk_menutitle_need_refresh = TRUE;
    // close window in the fulture
    s_mmistk_close_window = MMISTK_CLOSE_NONE;
    //SCI_TRACE_LOW:"[MMI_STK] leave MMIAPISTK_EnterToplevelMenu"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8539_112_2_18_2_56_9_229,(uint8*)"");
}

/*****************************************************************************/
//  Description : CreateDynaMenuItem
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL void CreateDynaMenuItem(MMI_CTRL_ID_T ctrl_id, STK_MENU_INFO_T *menu_info_ptr)
{
    GUILIST_ITEM_T item_t = {0};
    uint32 i = 0;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_display = FALSE;
#endif

    if (PNULL == menu_info_ptr)
    {
        return;
    }
    
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
    if(menu_info_ptr->is_icon_list)
    {
        MN_SIM_IMG_T sim_img = {0};
        MNSIM_GetImgEx(MMISTK_GetSTKSim(), menu_info_ptr->menu_item[0].iconID, &sim_img);

        if (DecodeSimImgToBmp(&sim_img, &(menu_info_ptr->menu_item[0].icon_bmp)))
        {
            is_icon_display = TRUE;
        }
    }

    if (is_icon_display)
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    }
    else
#endif
#endif
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    }

    for (i = 0; i < menu_info_ptr->menu_item_amount; i++)
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
}

/*****************************************************************************/
//  Description : CreateDynaMenuItemContent
//  Global resource dependence : none
//  Author: andrew.ma
//  Note: 
//        
/*****************************************************************************/
LOCAL void CreateDynaMenuItemContent(MMI_CTRL_ID_T ctrl_id, uint16 index, uint16 cont_index, STK_MENU_INFO_T *menu_info_ptr, MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    GUIANIM_DATA_INFO_T anim_data = {0}; 
#endif

    if (PNULL == menu_info_ptr)
    {
        return;
    }

    if (index >= menu_info_ptr->menu_item_amount)
    {
        return;
    }

    if (menu_info_ptr->is_icon_list)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
        MN_SIM_IMG_T sim_img = {0};
        MNSIM_GetImgEx(dual_sys, menu_info_ptr->menu_item[index].iconID, &sim_img);

        if (DecodeSimImgToBmp(&sim_img, &(menu_info_ptr->menu_item[index].icon_bmp)))
        {
            item_data.item_content[cont_index].item_data_type = GUIITEM_DATA_ANIM_DATA;
            item_data.item_content[cont_index].item_data.anim_data_ptr = &anim_data;
            item_data.item_content[cont_index].item_data.anim_data_ptr->is_bitmap = TRUE;
            item_data.item_content[cont_index].item_data.anim_data_ptr->bitmap_data.bit_ptr    = menu_info_ptr->menu_item[index].icon_bmp.bit_ptr;
            item_data.item_content[cont_index].item_data.anim_data_ptr->bitmap_data.img_height = menu_info_ptr->menu_item[index].icon_bmp.img_height;
            item_data.item_content[cont_index].item_data.anim_data_ptr->bitmap_data.img_width  = menu_info_ptr->menu_item[index].icon_bmp.img_width;

            s_mmistk_icon_display_result = GUILIST_SetItemContent(ctrl_id, &(item_data.item_content[cont_index]), index, cont_index);
        }
        else
#endif
#endif
        {
            s_mmistk_icon_display_result = FALSE;
        }
    }
}

/*****************************************************************************/
//  Description : CreateDynaMenuItemData
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//        
/*****************************************************************************/
LOCAL void CreateDynaMenuItemData(MMI_CTRL_ID_T ctrl_id, uint16 index, STK_MENU_INFO_T *menu_info_ptr,MN_DUAL_SYS_E dual_sys)
{
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    GUIANIM_DATA_INFO_T anim_data = {0}; 
    BOOLEAN result = FALSE;
    uint8 icon_id = 0;
    MN_SIM_IMG_T sim_img = {0};
#endif

    if (PNULL == menu_info_ptr)
    {
        return;
    }

    if (index >= menu_info_ptr->menu_item_amount)
    {
        return;
    }

    if (menu_info_ptr->help_info)
    {
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
    }
    else
    {
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
    }

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    icon_id = menu_info_ptr->menu_item[index].iconID;
    result = MNSIM_GetImgEx(dual_sys, menu_info_ptr->menu_item[index].iconID, &sim_img);

    if ((menu_info_ptr->is_icon_list)&&(icon_id != 0xFF)&&(result == TRUE))//bug901922
    {
//        if (DecodeSimImgToBmp(&sim_img, &(menu_info_ptr->menu_item[index].icon_bmp)))
        if (!menu_info_ptr->is_icon_list_self_explan)
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
            item_data.item_content[0].item_data.anim_data_ptr = &anim_data;
            item_data.item_content[0].item_data.anim_data_ptr->is_bitmap = TRUE;
            
            item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)menu_info_ptr->menu_item[index].stringData;
            item_data.item_content[1].item_data.text_buffer.wstr_len = menu_info_ptr->menu_item[index].length;
        }
        else
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
            item_data.item_content[0].item_data.anim_data_ptr = &anim_data;
            item_data.item_content[0].item_data.anim_data_ptr->is_bitmap = TRUE;

        }
        s_mmistk_icon_display_result = TRUE;
    }
    else
#endif
    {
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = (wchar *)menu_info_ptr->menu_item[index].stringData;
    
        item_data.item_content[0].item_data.text_buffer.wstr_len = menu_info_ptr->menu_item[index].length;
    }
    
    GUILIST_SetItemData(ctrl_id, &item_data, index);
}

/********************************************************************************
 NAME:                      CreateToplevelMenuVision
 DESCRIPTION:           
 PARAM IN:		
 PARAM OUT:             None
 AUTHOR:                allen
 DATE:                  2004.10.06
********************************************************************************/
LOCAL void CreateToplevelMenuVision(MN_DUAL_SYS_E dual_sys, STK_MENU_INFO_T* toplevel_menu_info)
{
    MMI_STRING_T menu_name = {PNULL, 0};
    //GUI_RECT_T menu_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T both_rect = {0};
    // uint32  i = 0;
    MMI_WINDOW_CREATE_T win_create = {0};
    // GUIMENU_DYNA_ITEM_T node_item = {0};
    BOOLEAN win_exist = FALSE;
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    BOOLEAN is_icon_can_display = FALSE;//(is_icon_id ||self_explan) && decode OK
#endif

    win_exist = MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID);

    if (!win_exist)
    {
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_MAINMENU_WIN_ID;
        win_create.func = MMISTK_MainMenuWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)dual_sys;
        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_MAINMENU_WIN_ID);
#endif

#ifndef MMI_PDA_SUPPORT
        CreatSoftkeyCtrl(MMISTK_MAINMENU_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#endif

        GUIWIN_CreateTitleDyna(MMISTK_MAINMENU_WIN_ID, TXT_NULL);
    }
    else
    {
        MMK_DestroyDynaCtrl(MMISTK_MAINMENU_CTRL_ID);
    }
    
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM
    if(toplevel_menu_info->is_icon_id || toplevel_menu_info->is_menu_title_icon_self_explan)
    {
        MN_SIM_IMG_T sim_img = {0};
        
        MNSIM_GetImgEx(dual_sys, toplevel_menu_info->menu_title_icon_id, &sim_img);
        
        //decode OK
        if(DecodeSimImgToBmp(&sim_img, &(toplevel_menu_info->menu_title_icon_bmp)))
        {
            is_icon_can_display = TRUE;
        }
        else
        {
            is_icon_can_display = FALSE;
        }
    }
#endif
#endif    

    both_rect = MMITHEME_GetWinClientBothRect(MMISTK_MAINMENU_WIN_ID);

    //create dynamic list
    if (toplevel_menu_info->menu_item_amount > 0)
    {
        GUILIST_INIT_DATA_T init_data = {0};

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if (toplevel_menu_info->is_icon_id && is_icon_can_display)
        {
            both_rect.v_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
            both_rect.h_rect.top += MMISTK_ICON_DISPLAY_HEIGHT;
        }
#endif

        init_data.type = GUILIST_TEXTLIST_E;
        init_data.both_rect = both_rect;
        init_data.is_dynamic_item_data = TRUE;
        init_data.max_item_num = toplevel_menu_info->menu_item_amount;
        
        // dynamic create
        GUILIST_CreateListBox(MMISTK_MAINMENU_WIN_ID, 0, MMISTK_MAINMENU_CTRL_ID, &init_data);
    }

    menu_name.wstr_len = toplevel_menu_info->menu_title_len;

    menu_name.wstr_ptr = toplevel_menu_info->menu_title;
    
    //set title
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    if (!is_icon_can_display)
#endif
    {
        GUIWIN_SetTitleText(MMISTK_MAINMENU_WIN_ID, menu_name.wstr_ptr, menu_name.wstr_len, FALSE);
    }

    /////////////////////////////
    // 		Set Menu Data
    /////////////////////////////
    CreateDynaMenuItem(MMISTK_MAINMENU_CTRL_ID, toplevel_menu_info);

    if (win_exist)
    {
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(MMISTK_MAINMENU_WIN_ID, MMITHEME_GetTipsCtrlId()));
    }

    if (toplevel_menu_info->is_action_id)
    {
        MMI_CONTROL_CREATE_T create = {0};
        GUITIPS_INIT_DATA_T init_data = {0};

        init_data.both_rect = both_rect;

        create.ctrl_id = MMITHEME_GetTipsCtrlId();
        create.guid = SPRD_GUI_TIPS_ID;
        create.parent_win_handle = MMISTK_MAINMENU_WIN_ID;
        create.init_data_ptr = &init_data;

        MMK_CreateControl(&create);
    }

    if (win_exist)
    {
        MMK_SetAtvCtrl(MMISTK_MAINMENU_WIN_ID, MMISTK_MAINMENU_CTRL_ID);

        if (!MMK_IsFocusWin(MMISTK_MAINMENU_WIN_ID))
        {
            if (!MMK_IsOpenWin(MMISTK_REFRESH_WAITING_WIN_ID) 
        #if 0 //!defined(MMI_CALL_MINIMIZE_SUPPORT)
            && !ExistActiveOrSetupCall() 
        #endif
            )
            {
                MMK_WinGrabFocus(MMISTK_MAINMENU_WIN_ID);
            }
        }
        else
        {
            MMK_PostMsg(MMISTK_MAINMENU_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    if (toplevel_menu_info->is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if (is_icon_can_display)
        {
            GUI_BOTH_RECT_T anim_disp_rect = MMITHEME_GetWinClientBothRect(MMISTK_MAINMENU_WIN_ID)/*MMITHEME_GetWinTitleBothRect(MMISTK_MAINMENU_WIN_ID)*/;

            anim_disp_rect.v_rect.bottom = anim_disp_rect.v_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;
            anim_disp_rect.h_rect.bottom = anim_disp_rect.h_rect.top + MMISTK_ICON_DISPLAY_HEIGHT - 1;

            s_mmistk_icon_display_result = CreateIconAnimCtrl(MMISTK_MAINMENU_WIN_ID, MMISTK_TOP_MENU_ANIM_CTRL_ID, &(toplevel_menu_info->menu_title_icon_bmp), &anim_disp_rect);
        }
        else
#endif
        {
            s_mmistk_icon_display_result = FALSE;
        }
    }
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify STK
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifySTKFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMISTK]: HandleSIMPlugNotifySTKFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_8872_112_2_18_2_56_10_230,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        //not care
        break;

    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        if (IsInProactiveStkSession() && (dual_sys == MMISTK_GetSTKSim()))
        {
            MMK_CloseMiddleWin(MMISTK_WIN_ID_START, MMISTK_MAX_WIN_ID);

            s_mmistk_item_is_selecting[dual_sys] = FALSE;
            s_mmistk_text_is_displaying[dual_sys] = FALSE;
            s_mmistk_key_is_getting[dual_sys] = FALSE;
            s_mmistk_input_is_getting[dual_sys] = FALSE;
            s_mmistk_sms_is_sending[dual_sys] = FALSE;
            s_mmistk_tone_is_playing[dual_sys] = FALSE;
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMK_CloseWin(MMISTK_COM_SELECT_SIM_WIN_ID);
#endif
        break;

    default:
        break;
    }
}
#endif

/********************************************************************************
 NAME:			MMISTK_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMISTK_AppInit(void)
{
    uint32 i = 0;
#ifdef SIM_PLUG_IN_SUPPORT   
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif    
    g_mmistk_app.ProcessMsg = MMISTK_AppHandlePsMsg;
    g_mmistk_app.component_type = CT_APPLICATION;

    s_mmistk_send_sms = FALSE;
    // initialize the Cnf flag
    s_mmistk_indmenusel_cnf = FALSE;
    //s_mmistk_sendsms_cnf = FALSE;
    s_mmistk_selectitem_cnf = FALSE;
    s_mmistk_displaytext_cnf = FALSE;
    s_mmistk_getinkey_cnf = FALSE;
    s_mmistk_getinput_cnf = FALSE;
    s_mmistk_playtone_cnf = FALSE;

    // visual process status
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        s_mmistk_sendsms_cnf[i] = FALSE;
        s_mmistk_sms_is_sending[i] = FALSE;
        s_mmistk_item_is_selecting[i] = FALSE;
        s_mmistk_text_is_displaying[i] = FALSE;
        s_mmistk_key_is_getting[i] = FALSE;
        s_mmistk_input_is_getting[i] = FALSE;
        s_mmistk_tone_is_playing[i] = FALSE;
    }
    s_mmistk_sendss_waiting_respond = FALSE;
    s_mmistk_sendussd_waiting_respond = FALSE;

#ifdef SIM_PLUG_IN_SUPPORT    
    sim_plug_notify_info.module_id = MMI_MODULE_STK;
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifySTKFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif    
}

#if 0
// no use
/********************************************************************************
 NAME:			MMISTK_LaunchBrowserAlertWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_LaunchBrowserAlertWin(MMI_STRING_T *text_ptr, MMI_IMAGE_ID_T image_id)
{
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

    MMIPUB_OpenAlertWinByTextPtr(PNULL, text_ptr, PNULL, image_id,
                                     PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
}
#endif

#ifdef BROWSER_SUPPORT
/********************************************************************************
 NAME:			MMISTK_LaunchBrowserWithURL
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_LaunchBrowserWithURL(MN_DUAL_SYS_E dual_sys, uint8 *url_ptr)
{
    BOOLEAN ret = FALSE;
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    MMIBROWSER_ERROR_E result_code = MMIBROWSER_SUCC;

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = dual_sys;
    entry_param.url_ptr = (char *)url_ptr;
    entry_param.user_agent_ptr = PNULL;
    
    result_code = MMIAPIBROWSER_Entry(&entry_param);

    if (MMIBROWSER_SUCC == result_code)
    {
        ret = TRUE;
    }
#endif

    return ret;
}

/********************************************************************************
 NAME:			MMISTK_LaunchBrowserByDefault
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_LaunchBrowserByDefault(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN ret = FALSE;
#ifdef BROWSER_SUPPORT
    uint8 index = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    MMIBROWSER_ERROR_E result_code = MMIBROWSER_SUCC;

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = dual_sys;
    entry_param.url_ptr = (char *)link_setting_ptr->homepage;
    entry_param.user_agent_ptr = PNULL;
    
    result_code = MMIAPIBROWSER_Entry(&entry_param);

    if (MMIBROWSER_SUCC == result_code)
    {
        ret = TRUE;
    }
#endif

    return ret;
}

/********************************************************************************
  NAME:          CreateLaunchBrowserVision
  DESCRIPTION:   
  PARAM IN:      
  PARAM OUT:     None
  AUTHOR:        dave.ruan
  DATE:
 ********************************************************************************/
 LOCAL void CreateLaunchBrowserVision( APP_MN_SIMAT_LAUNCH_BROWSER_IND_T* launch_browser_ind)
{
#if 0
// no use
    BOOLEAN result = FALSE;
#endif
    BOOLEAN is_open = FALSE;

    if(PNULL == launch_browser_ind)
    {
        return;
    }
    if (!MMK_IsOpenWin(MMISTK_LAUNCH_BROWSER_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;

        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_LAUNCH_BROWSER_WIN_ID;
        win_create.func = LaunchBrowserWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)launch_browser_ind;

        MMK_CreateWindow( &win_create );	
  
#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_LAUNCH_BROWSER_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_LAUNCH_BROWSER_WIN_ID, TXT_NULL);

        CreatSoftkeyCtrl(MMISTK_LAUNCH_BROWSER_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_LAUNCH_BROWSER_WIN_ID);

        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_LAUNCH_BROWSER_WIN_ID;
        create.init_data_ptr = &init_data;

        MMK_CreateControl(&create);

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_LAUNCH_BROWSER_WIN_ID, TXT_NULL);

#if 0
// no use
        result = TRUE;
#endif

        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    SetStkWinTitle(launch_browser_ind->dual_sys, MMISTK_LAUNCH_BROWSER_WIN_ID);
    
    GUIWIN_SetSoftkeyTextId(MMISTK_LAUNCH_BROWSER_WIN_ID,  TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, FALSE);

    SetRichtextItemInfo(launch_browser_ind->dual_sys,
                                        s_mmistk_launch_browser_is_icon_id,
                                        s_mmistk_launch_browser_icon_id,
                                        s_mmistk_displaydata,
                                        MMISTK_TEXBOX_CTRL_ID);

    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_LAUNCH_BROWSER_WIN_ID);

        if (!MMK_IsFocusWin(MMISTK_LAUNCH_BROWSER_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_LAUNCH_BROWSER_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_LAUNCH_BROWSER_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }

    // open the new window, then close the old window
    CheckUnCloseWin(MMISTK_CLOSE_LAUNCH_BROWSER_WIN);
    // first close the menu window to avoid the window glitter
    //MMK_CloseWin(MMISTK_MENU_WIN_ID);
}

/*****************************************************************************/
//     Description : start setup call Operation
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void StartLaunchBrowserOperation(MN_DUAL_SYS_E dual_sys,SIMAT_LAUNCH_BROWSER_T* launch_para_in)
{
    SIMAT_LAUNCH_BROWSER_CNF_T param_out = {0};/*lint !e64*/
    BOOLEAN launch_ret = FALSE;

    //SCI_TRACE_LOW:"[MMI_STK] StartLaunchBrowserOperation dual_sys=%d, url_len=%d,launch_browser_type"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9146_112_2_18_2_56_10_231,(uint8*)"dd", dual_sys, launch_para_in->url.url_len,launch_para_in->launch_browser_type);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }
    
    if(0 < launch_para_in->url.url_len)
    {
        switch(launch_para_in->launch_browser_type)
        {
        case SIMAT_LAUNCH_BROWSER_USE_EXIST:
        case SIMAT_LAUNCH_BROWSER_CLOSE_OLD_NEW_BROWSER:
        case SIMAT_LAUNCH_BROWSER_NOT_LAUCHED:
#ifdef BROWSER_SUPPORT
            if(MMIAPIBROWSER_IsRunning())
            {
                param_out.result = SIMAT_TR_LAUNCH_BROWSER_ERR;
                param_out.mp_info = SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE;
                //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED with url browser is running!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9163_112_2_18_2_56_10_232,(uint8*)"");
            }
            else
#endif
            {
                launch_ret = MMISTK_LaunchBrowserWithURL(dual_sys, launch_para_in->url.url);
                if(launch_ret)
                {
                    param_out.result = SIMAT_TR_SUCCESS;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url success!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9173_112_2_18_2_56_11_233,(uint8*)"");
                }
                else
                {
                    param_out.result = SIMAT_TR_ME_UNABLE;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9179_112_2_18_2_56_11_234,(uint8*)"");
                }
            }
            break;
#if 0            
        case SIMAT_LAUNCH_BROWSER_USE_EXIST:
        case SIMAT_LAUNCH_BROWSER_CLOSE_OLD_NEW_BROWSER:
#ifdef BROWSER_SUPPORT
            if(!MMIAPIBROWSER_IsRunning())
            {
                param_out.result = SIMAT_TR_ME_UNABLE;
                param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST with url browser is not running!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9191_112_2_18_2_56_11_235,(uint8*)"");
            }
            else
#endif
            {
                launch_ret = MMISTK_LaunchBrowserWithURL(dual_sys, launch_para_in->url.url);
                if(launch_ret)
                {
                    param_out.result = SIMAT_TR_SUCCESS;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser with url success!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9201_112_2_18_2_56_11_236,(uint8*)"");
                }
                else
                {
                    param_out.result = SIMAT_TR_ME_UNABLE;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser with url error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9207_112_2_18_2_56_11_237,(uint8*)"");
                }
            }
            break;
#endif

        default:
            param_out.result = SIMAT_TR_ME_UNABLE;
            param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
            //SCI_TRACE_LOW:"[MMI_STK] launch_browser_type is default with url!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9215_112_2_18_2_56_11_238,(uint8*)"");
            break;
        }
    }
    else
    {
        switch(launch_para_in->launch_browser_type)
        {
        case SIMAT_LAUNCH_BROWSER_USE_EXIST:
        case SIMAT_LAUNCH_BROWSER_CLOSE_OLD_NEW_BROWSER:
        case SIMAT_LAUNCH_BROWSER_NOT_LAUCHED:
#ifdef BROWSER_SUPPORT
            if(MMIAPIBROWSER_IsRunning())
            {
                param_out.result = SIMAT_TR_LAUNCH_BROWSER_ERR;
                param_out.mp_info = SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE;
                //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED default browser is running!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9229_112_2_18_2_56_11_239,(uint8*)"");
            }
            else
#endif
            {
                launch_ret = MMISTK_LaunchBrowserByDefault(dual_sys);
                if(launch_ret)
                {
                    param_out.result = SIMAT_TR_SUCCESS;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser default success!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9239_112_2_18_2_56_11_240,(uint8*)"");
                }
                else
                {
                    param_out.result = SIMAT_TR_ME_UNABLE;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser default error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9245_112_2_18_2_56_11_241,(uint8*)"");
                }
            }
            break;
#if 0            
        case SIMAT_LAUNCH_BROWSER_USE_EXIST:
        case SIMAT_LAUNCH_BROWSER_CLOSE_OLD_NEW_BROWSER:
#ifdef BROWSER_SUPPORT
            if(!MMIAPIBROWSER_IsRunning())
            {
                param_out.result = SIMAT_TR_ME_UNABLE;
                param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST default browser is not running!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9257_112_2_18_2_56_11_242,(uint8*)"");
            }
            else
#endif
            {
                launch_ret = MMISTK_LaunchBrowserByDefault(dual_sys);
                if(launch_ret)
                {
                    param_out.result = SIMAT_TR_SUCCESS;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser default success!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9267_112_2_18_2_56_11_243,(uint8*)"");
                }
                else
                {
                    param_out.result = SIMAT_TR_ME_UNABLE;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_USE_EXIST browser default error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9273_112_2_18_2_56_11_244,(uint8*)"");
                }
            }
            break;
#endif

        default:
            param_out.result = SIMAT_TR_ME_UNABLE;
            param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
            //SCI_TRACE_LOW:"[MMI_STK] launch_browser_type is default with default url!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9281_112_2_18_2_56_11_245,(uint8*)"");
            break;
        }
    }

    if (s_mmistk_launch_browser_is_icon_id && (SIMAT_TR_SUCCESS == param_out.result))
    {
//        param_out.result = SIMAT_TR_ICON_FAIL_DISPLAY;
        param_out.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
    }

    SIMAT_CnfLaunchBrowserEx(dual_sys, &param_out);
}

/********************************************************************************
 NAME: LaunchBrowserWinHandleMsg
 DESCRIPTION:Launch Browser Win Handle Msg
 PARAM IN:
 PARAM OUT:		
 AUTHOR:dave.ruan
 DATE:
********************************************************************************/
LOCAL MMI_RESULT_E LaunchBrowserWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    SIMAT_LAUNCH_BROWSER_CNF_T param_out = {0};/*lint !e64*/
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    APP_MN_SIMAT_LAUNCH_BROWSER_IND_T* launch_browser_ind = (APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)MMK_GetWinAddDataPtr(win_id);
    LOCAL BOOLEAN is_stk_browser_win_closing = FALSE;

    //SCI_TRACE_LOW:"[MMI_STK] enter LaunchBrowserWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9311_112_2_18_2_56_11_246,(uint8*)"dd", msg_id, s_mmistk_close_window);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        is_stk_browser_win_closing = FALSE;
        MMK_SetAtvCtrl(win_id, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        break;

    case MSG_APP_FLIP:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(!is_stk_browser_win_closing)
        {
            param_out.result = SIMAT_TR_ME_UNABLE;
            param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
            //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser user deny"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9337_112_2_18_2_56_11_247,(uint8*)"");
            SIMAT_CnfLaunchBrowserEx(launch_browser_ind->dual_sys, &param_out);
            s_mmistk_close_window = MMISTK_CLOSE_LAUNCH_BROWSER_WIN;
            is_stk_browser_win_closing = TRUE;
        }
        //MMK_CloseWin(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT  //support PDA  for STK
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(msg_id ==MSG_CTL_PENOK)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;               
            switch(src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                StartLaunchBrowserOperation(launch_browser_ind->dual_sys,&launch_browser_ind->com);
                //MMK_CloseWin(win_id);
                break;
              
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                {
                    param_out.result = SIMAT_TR_ME_UNABLE;
                    param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
                    //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser user deny"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9368_112_2_18_2_56_11_248,(uint8*)"");
                    SIMAT_CnfLaunchBrowserEx(launch_browser_ind->dual_sys, &param_out);
                    s_mmistk_close_window = MMISTK_CLOSE_LAUNCH_BROWSER_WIN;
                    is_stk_browser_win_closing = TRUE;                   
                }
                //MMK_CloseWin(win_id);
                break;

            default:
                break;
            }
            break;
        }
#endif //IGNORE9527
#endif
        StartLaunchBrowserOperation(launch_browser_ind->dual_sys,&launch_browser_ind->com);
        //MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(!is_stk_browser_win_closing)
        {
            param_out.result = SIMAT_TR_ME_UNABLE;
            param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;
            SIMAT_CnfLaunchBrowserEx(launch_browser_ind->dual_sys, &param_out);
            s_mmistk_close_window = MMISTK_CLOSE_LAUNCH_BROWSER_WIN;
            is_stk_browser_win_closing = TRUE;
            //SCI_TRACE_LOW:"[MMI_STK] SIMAT_LAUNCH_BROWSER_NOT_LAUCHED browser with url error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9394_112_2_18_2_56_11_249,(uint8*)"");
        }
        MMK_FreeWinAddData(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    //SCI_TRACE_LOW:"[MMI_STK] leave LaunchBrowserWinHandleMsg result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9403_112_2_18_2_56_11_250,(uint8*)"d",result);
    return (result);
}
#endif

/********************************************************************************
 NAME:			MMISTK_OnLaunchBrowserCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnLaunchBrowserCmd(void* arg)
{
#if 0//remove APP_MN_SIMAT_LAUNCH_BROWSER_IND process,only reply not support TR,bug1397785
#ifdef BROWSER_SUPPORT
    APP_MN_SIMAT_LAUNCH_BROWSER_IND_T* arg_in_ptr = PNULL;
    SIMAT_LAUNCH_BROWSER_T* param_in = PNULL;
    SIMAT_LAUNCH_BROWSER_CNF_T param_out = {0};/*lint !e64*/
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    if(arg == PNULL)
    {
        //SCI_TRACE_LOW:"MMISTK_OnLaunchBrowserCmd arg is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9423_112_2_18_2_56_11_251,(uint8*)"");
        return;
    }
    
    if(MMK_IsOpenWin(MMISTK_LAUNCH_BROWSER_WIN_ID)) 
    {
        //to free memory for present window
        MMK_CloseWin(MMISTK_LAUNCH_BROWSER_WIN_ID);
    }
    
    //alloc mem for browser       
    arg_in_ptr = (APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)SCI_ALLOCAZ(sizeof(APP_MN_SIMAT_LAUNCH_BROWSER_IND_T));
    if(PNULL == arg_in_ptr)
    {
        param_out.result = SIMAT_TR_ME_UNABLE;
        param_out.mp_info = SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE;
        SIMAT_CnfLaunchBrowserEx(dual_sys, &param_out);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because alloc APP_MN_SIMAT_LAUNCH_BROWSER_IND_T fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9440_112_2_18_2_56_11_252,(uint8*)"");
        return;
    }
     
    //arg_in_ptr = (APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)arg;
    //SCI_MEMSET(arg_in_ptr, 0x0,sizeof(APP_MN_SIMAT_LAUNCH_BROWSER_IND_T));
    SCI_MEMCPY(arg_in_ptr, arg, sizeof(APP_MN_SIMAT_LAUNCH_BROWSER_IND_T));
    
    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    CheckCloseDisplayTextIRWin();

    param_in = (SIMAT_LAUNCH_BROWSER_T *)(&arg_in_ptr->com);
    dual_sys = arg_in_ptr->dual_sys;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnLaunchBrowserCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9458_112_2_18_2_56_11_253,(uint8*)"");
    //SCI_TRACE_LOW:"[MMI_STK] dual_sys:%d, launch_browser_type:%d, is_browser_id:%d, is_icon_id:%d, is_alpha_id:%d, is_gate_way:%d, is_bear:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9467_112_2_18_2_56_12_254,(uint8*)"ddddddd", dual_sys,param_in->launch_browser_type,param_in->is_browser_id,param_in->is_icon_id,param_in->is_alpha_id,param_in->is_gate_way,param_in->is_bear);

    //active call, return ME unable
    if (ExistActiveOrSetupCall()) 
    {
        //SIMAT_CloseShareBuf(param_in->buf_id);
        param_out.result = SIMAT_TR_ME_UNABLE;
        param_out.mp_info = SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE;
        SIMAT_CnfLaunchBrowserEx(dual_sys, &param_out);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because ExistActiveOrSetupCall"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9476_112_2_18_2_56_12_255,(uint8*)"");
        SCI_FREE(arg_in_ptr);
        return;
    }

#ifdef MMS_SUPPORT
    if (MMIAPIMMS_IsSendingOrRecving())
    {
        param_out.result = SIMAT_TR_ME_UNABLE;
        param_out.mp_info = SIMAT_LB_NO_SPECIFIC_CAUSE;

        SIMAT_CnfLaunchBrowserEx(dual_sys, &param_out);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because MMS sending"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9488_112_2_18_2_56_12_256,(uint8*)"");
        SCI_FREE(arg_in_ptr);
        return;
    }
#endif

    MMISTK_SetSTKSim(dual_sys);

    s_mmistk_launch_browser_is_icon_id = param_in->is_icon_id;
    if (s_mmistk_launch_browser_is_icon_id)
    {
        s_mmistk_launch_browser_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_launch_browser_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
    }

    SCI_MEMSET(&s_mmistk_displaydata, 0x00, sizeof(s_mmistk_displaydata));
    CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));

    s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;

    if (param_in->is_alpha_id && param_in->alpha_id.len >0) 
    {
        AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
    }

    //fix gateway info
    if(param_in->is_gate_way)
    {
        
    }

    //bearing information
    if (param_in->is_bear)
    {

    }
    
    if(0 < s_mmistk_displaydata.wstr_len)
    {
        //query if launch browser
        CreateLaunchBrowserVision(arg_in_ptr);
    }
    else
    {
        //no diplay data will launch browser directly
        StartLaunchBrowserOperation(dual_sys,param_in);
        SCI_FREE(arg_in_ptr);
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnLaunchBrowserCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9537_112_2_18_2_56_12_257,(uint8*)"");
//#else
#endif
#endif
    APP_MN_SIMAT_LAUNCH_BROWSER_IND_T* arg_in_ptr = PNULL;
    SIMAT_LAUNCH_BROWSER_CNF_T param_out = {0};/*lint !e64*/
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    if(arg == PNULL)
    {
        //SCI_TRACE_LOW:"MMISTK_OnLaunchBrowserCmd arg is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9423_112_2_18_2_56_11_251,(uint8*)"");
        return;
    }
    arg_in_ptr = (APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)arg;
    dual_sys = arg_in_ptr->dual_sys;
    param_out.result = SIMAT_TR_BEYOND_ME_CAP;
    param_out.mp_info = SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE;
    SIMAT_CnfLaunchBrowserEx(dual_sys, &param_out);
    SCI_TRACE_LOW("[MMI_STK] leave MMISTK_OnLaunchBrowserCmd because BROWSER_SUPPORT = FALSE");

//#endif    
}


/********************************************************************************
 NAME:			MMISTK_SetPdpActiveStatus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_E state)
{
    s_mmistk_pdp_state = state;
}

/********************************************************************************
 NAME:			MMISTK_SetPdpActiveStatus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL MMISTK_PDP_STATUS_E MMISTK_GetPdpActiveStatus(void)
{
    return s_mmistk_pdp_state;
}

#ifdef MMI_OPEN_CHANNEL_SUPPORT
/********************************************************************************
 NAME:			MMISTK_CreateSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL long MMISTK_CreateSocket(uint8 net_id)
{
    long sock_id = -1;
    int32 e = 0;
    int result = 0;
    int type = 0;

    //SCI_TRACE_LOW:"[MMI_STK]MMISTK_CreateSocket enter netid = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9581_112_2_18_2_56_12_258,(uint8*)"d", net_id);

    if(SIMAT_TRANS_LEVEL_TYPE_UDP_CLIENT == s_mmistk_trans_info.trans_level_type)
    {
        type = SOCK_DGRAM;
    }
    else if(SIMAT_TRANS_LEVEL_TYPE_TCP_CLIENT == s_mmistk_trans_info.trans_level_type)
    {
        type = SOCK_STREAM;
    }
    
    sock_id = sci_sock_socket(AF_INET, type, 0, (uint32)net_id);

    if (sock_id == -1)
    {
        e = sci_sock_errno((long)sock_id);
        //SCI_TRACE_LOW:"[MMI_STK]MMISTK_CreateSocket, e = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9597_112_2_18_2_56_12_259,(uint8*)"d", e);

        return -1;
    }
    else
    {
        result = sci_sock_setsockopt(sock_id, SO_NBIO, PNULL);

        if (result < 0)
        {
            sci_sock_socketclose(sock_id);
            //SCI_TRACE_LOW:"[MMI_STK]sci_sock_setsockopt, result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9608_112_2_18_2_56_12_260,(uint8*)"d", result);

            return -1;
        }
    }

    //SCI_TRACE_LOW:"[MMI_STK]MMISTK_CreateSocket, return %ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9614_112_2_18_2_56_12_261,(uint8*)"d", sock_id);

    s_mmistk_sock_id = sock_id;

    return sock_id;
}
#endif

/********************************************************************************
 NAME:			MMISTK_CloseSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_CloseSocket(long sock_id)
{
    //SCI_TRACE_LOW:"[MMI_STK]MMISTK_CloseSocket: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9630_112_2_18_2_56_12_262,(uint8*)"d", sock_id);

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    if(-1 != sock_id)
    {
        sci_sock_socketclose((long)sock_id);
    }
#endif
}

/********************************************************************************
 NAME:			MMISTK_ConnectSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_ConnectSocket(long sock_id)
{
    BOOLEAN ret = FALSE;

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    int err = -1;
    SockAddr addr = {0};

    addr.family = AF_INET;
    addr.port = htons((unsigned short)s_mmistk_trans_info.port);
    SCI_MEMCPY(&(addr.ip_addr), s_mmistk_dest_addr.other_addr, sizeof(addr.ip_addr));

    err = sci_sock_connect((long)sock_id, &addr, sizeof(SockAddr));
    
    if (err >= 0)
    {
        ret = TRUE;
        s_mmistk_reconnect_socket_times = 0;
        //SCI_TRACE_LOW:"[MMI_STK]connection is established, sock=%ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9665_112_2_18_2_56_12_263,(uint8*)"d", sock_id);
    }
    else
    {
        err = sci_sock_errno((long)sock_id);
        //SCI_TRACE_LOW:"[MMI_STK]MMISTK_ConnectSocket, err = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9670_112_2_18_2_56_12_264,(uint8*)"d", err);
        
        if (0 != s_mmistk_connect_socket_timer_id)
        {
            MMK_StopTimer(s_mmistk_connect_socket_timer_id);
            s_mmistk_connect_socket_timer_id = 0;
        }
        s_mmistk_connect_socket_timer_id = MMK_CreateTimerCallback(MMISTK_SOCKET_CONNECT_DURATION, MMISTK_HandleSTKTimer, NULL, FALSE);

        ret = FALSE;
    }
#endif

    //SCI_TRACE_LOW:"[MMI_STK]SocketConnect, sock=%d, return=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9683_112_2_18_2_56_12_265,(uint8*)"dd", sock_id, ret);

    return ret;
}

/********************************************************************************
 NAME:			MMISTK_SocketSendData
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL int MMISTK_SocketSendData(long sock_id, char *in_buf, int in_len)
{
    int ret = -1;
#ifdef MMI_OPEN_CHANNEL_SUPPORT
    int err = 0;

    ret = sci_sock_send((long)sock_id, (char *)in_buf, in_len, 0);

    if (ret < 0)
    {
        err = sci_sock_errno((long)sock_id);

        //SCI_TRACE_LOW:"[MMI_STK]MMISTK_SocketSendData error= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9707_112_2_18_2_56_12_266,(uint8*)"d", err);
    }
#endif

    //SCI_TRACE_LOW:"[MMI_STK]MMISTK_SocketSendData, s=%d, return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9711_112_2_18_2_56_12_267,(uint8*)"dd", sock_id, ret);

    return ret;
}

/********************************************************************************
 NAME:			MMISTK_AssignChannelID
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL uint8 MMISTK_AssignChannelID(void)
{
    uint8 index = 1;
    
    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_AssignChannelID enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9727_112_2_18_2_56_12_268,(uint8*)"");

    while (index < MMISTK_CHANNEL_MAX)
    {
        if( !s_mmistk_channel_info[index].is_used )
        {
            s_mmistk_channel_info[index].channel_id = index;
            s_mmistk_channel_info[index].is_used = TRUE;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_AssignChannelID channel_id = %d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9735_112_2_18_2_56_12_269,(uint8*)"d", index);
            return (index);
        }
        index++;
    }

    return 0;
}

/********************************************************************************
 NAME:			MMISTK_IsChannelIDAvailable
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL uint8 MMISTK_GetAvailableChannelID(void)
{
  uint8 index = 0;

  for(index = 1; index < MMISTK_CHANNEL_MAX; index++)
  {
    if(0 != s_mmistk_channel_info[index].channel_id)
    {
      //SCI_TRACE_LOW:"[MMI_STK] MMISTK_GetAvailableChannelID = %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9759_112_2_18_2_56_12_270,(uint8*)"d", index);
      return index;
    }
  }

  //SCI_TRACE_LOW:"[MMI_STK] MMISTK_GetAvailableChannelID = 0"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9764_112_2_18_2_56_12_271,(uint8*)"");
  return 0;
}

/********************************************************************************
 NAME:			MMISTK_ReleaseChannelID
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_ReleaseChannelID(uint8 channel_id)
{
    uint8 index = 1;
    
    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_ReleaseChannelID enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9779_112_2_18_2_56_13_272,(uint8*)"");

    for(index = 1; index < MMISTK_CHANNEL_MAX; index++)
    {
        if(index == channel_id)
        {
            s_mmistk_channel_info[index].channel_id = 0;
            s_mmistk_channel_info[index].is_used = FALSE;
            s_mmistk_channel_info[index].is_enable = FALSE;
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_ReleaseChannelID channel_id = %d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9788_112_2_18_2_56_13_273,(uint8*)"d", index);
            break;
        }
    }
}

/********************************************************************************
 NAME:			MMISTK_ReleaseAllChannelID
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_ReleaseAllChannelID(void)
{
    uint8 index = 1;
    
    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_ReleaseAllChannelID enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9805_112_2_18_2_56_13_274,(uint8*)"");

    for(index = 1; index < MMISTK_CHANNEL_MAX; index++)
    {
        s_mmistk_channel_info[index].channel_id = 0;
        s_mmistk_channel_info[index].is_used = FALSE;
        s_mmistk_channel_info[index].is_enable = FALSE;
    }
}

/********************************************************************************
 NAME:			MMISTK_IsAllChannelIDReleased
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_IsAllChannelIDReleased(void)
{
    uint8 index = 1;
    BOOLEAN ret = TRUE;

    for(index = 1; index < MMISTK_CHANNEL_MAX; index++)
    {
        if(s_mmistk_channel_info[index].is_used)
        {
            ret = FALSE;
            break;
        }
    }

    return ret;
}

/********************************************************************************
 NAME:			HandleSTKReConnectSocket
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void HandleSTKReConnectSocketTimer(void)
{
    // uint8 channel_id = 0;
    // SIMAT_OPEN_CHANNEL_CNF_T param_out = {0};
    
    if(s_mmistk_reconnect_socket_times < MMISTK_RECONNECT_SOCKET_TIMES_MAX)
    {
        s_mmistk_reconnect_socket_times++;
        if(MMISTK_ConnectSocket(s_mmistk_sock_id))
        {
            s_mmistk_reconnect_socket_times = 0;
            if (0 != s_mmistk_connect_socket_timer_id)
            {
                MMK_StopTimer(s_mmistk_connect_socket_timer_id);
                s_mmistk_connect_socket_timer_id = 0;
            }

            MMISTK_CnfOpenChannel(TRUE, SIMAT_TR_SUCCESS);
            //SCI_TRACE_LOW:"[MMI_STK] SIMAT_CnfOpenChannel success!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9864_112_2_18_2_56_13_275,(uint8*)"");
        }
    }
    else if(s_mmistk_reconnect_socket_times == MMISTK_RECONNECT_SOCKET_TIMES_MAX)
    {
        s_mmistk_reconnect_socket_times = 0;
            
        MMISTK_CloseSocket(s_mmistk_sock_id);
        MMISTK_DeactivePDPContext();
            
        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_NE_UNABLE);
        
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_ConnectSocket error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9876_112_2_18_2_56_13_276,(uint8*)"");
    }
}

#ifdef MMI_OPEN_CHANNEL_SUPPORT
/********************************************************************************
 NAME:			MMISTK_HandlePdpActiveCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_HandlePdpActiveCnf(MMIPDP_CNF_INFO_T * msg_ptr)
{
#ifndef WIN32    
    long sock_id = -1;
#endif
    // uint8 channel_id = 0;
    // SIMAT_OPEN_CHANNEL_CNF_T param_out = {0};

    //SCI_TRACE_LOW:"[MMI_STK]MMISTK_HandlePdpActiveCnf result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9895_112_2_18_2_56_13_277,(uint8*)"d",msg_ptr->result);

    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        //pdp active success
        MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_ACTIVE_OK);

#ifndef WIN32
        sock_id = MMISTK_CreateSocket(msg_ptr->nsapi);

        if(sock_id == -1)
        {            
            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_NE_UNABLE);
            MMISTK_DeactivePDPContext();
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_CreateSocket error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9909_112_2_18_2_56_13_278,(uint8*)"");
        }
        else
        {
            if(MMISTK_ConnectSocket(sock_id))
            {
                MMISTK_CnfOpenChannel(TRUE, SIMAT_TR_SUCCESS);
            }
        }
#else
	MMISTK_CnfOpenChannel(TRUE, SIMAT_TR_SUCCESS);
#endif
    }
    else
    {
        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_NE_UNABLE);
        MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_DEACTIVED);
        
#ifdef FDN_SUPPORT
        if(MMIPDP_RESULT_FDN_NOT_PERMIT == msg_ptr->result)
        {
            MMISTK_OpenAlertWin(TXT_CC_FDN_ONLY, IMAGE_PUBWIN_WARNING, PNULL);
        }
        else
#endif        
        {
            MMISTK_OpenAlertWin(TXT_DIAL_FAILED, IMAGE_PUBWIN_WARNING, PNULL);
        }
    }
}

/********************************************************************************
 NAME:			MMISTK_ActivePDPcontext
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void HandleSTKPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        //SCI_TRACE_LOW:" MMISTK_OnLaunchBrowserCmd ::PNULL == msg_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9951_112_2_18_2_56_13_279,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"[MMI_STK]:HandleSTKPDPMsg enter msg_id = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_9954_112_2_18_2_56_13_280,(uint8*)"d", msg_ptr->msg_id);

    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        MMISTK_HandlePdpActiveCnf(msg_ptr);
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_DEACTIVED);
        break;
        
    case MMIPDP_DEACTIVE_IND:
        {
            uint8 channel_id = 0;
            SIMAT_CHANNEL_STATUS_IND_T channel_status = {0};

            MMISTK_CloseSocket(s_mmistk_sock_id);
            MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_DEACTIVED);
            channel_id = MMISTK_GetAvailableChannelID();
            channel_status.channel_id = channel_id;
            if(0 != channel_id)
            {
                channel_status.channel_status.channel_id = channel_id;
                channel_status.channel_status.mode_info = SIMAT_CHANNEL_MODE_LINK_DROPPED;
                channel_status.channel_status.is_link_establish = FALSE;
            }
            else
            {
                channel_status.channel_status.channel_id = channel_id;
                channel_status.channel_status.mode_info = SIMAT_CHANNEL_MODE_NO_FURTHER_INFO;
                channel_status.channel_status.is_link_establish = FALSE;
            }
#ifdef MMI_DUALMODE_ENABLE
#ifndef WIN32
            SIMAT_IndChannelStatusEvent(&channel_status);
#endif
#else
            SIMAT_IndChannelStatusEx((uint8)(MMISTK_GetSTKSim()), &channel_status);
#endif

            MMISTK_ReleaseAllChannelID();
        }
        break;
        
    default:
        break;
    }
    
    //SCI_TRACE_LOW:"[MMI_STK]:HandleSTKPDPMsg exit!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10003_112_2_18_2_56_13_281,(uint8*)"");
}
#endif

/********************************************************************************
 NAME:			MMISTK_ActivePDPcontext
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL BOOLEAN MMISTK_ActivePDPContext(MN_DUAL_SYS_E dual_sys, uint8 *apn_ptr,  uint8 *login_ptr, uint8 *psw_ptr)
{
    BOOLEAN ret = FALSE;

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    
    //SCI_TRACE_LOW:"[MMI_STK]:MMISTK_ActivatePDPcontext enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10020_112_2_18_2_56_13_282,(uint8*)"");
    
    app_info.app_handler = MMI_MODULE_STK;
    app_info.auth_type = 2;
    app_info.dual_sys = dual_sys;
    app_info.apn_ptr = (char*)apn_ptr;
    app_info.user_name_ptr = (char*)login_ptr;
    app_info.psw_ptr = (char*)psw_ptr;
    app_info.priority = 3; 
    app_info.ps_service_rat = MN_UNSPECIFIED;
    app_info.ps_service_type = BROWSER_E;
    app_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
    app_info.ip_type = MMICONNECTION_IP_V4;//only support ipv4
#endif    

    app_info.handle_msg_callback = HandleSTKPDPMsg;

    MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_ACTIVING);
        
    ret = MMIAPIPDP_Active(&app_info);
    //SCI_TRACE_LOW:"[MMI_STK]:MMIAPIPDP_Active = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10036_112_2_18_2_56_13_283,(uint8*)"d", ret);
#endif    

    return ret;    
}

/********************************************************************************
 NAME:			MMISTK_OnCloseChannelCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_DeactivePDPContext(void)
{
#ifdef MMI_OPEN_CHANNEL_SUPPORT
    MMISTK_PDP_STATUS_E bak_state = MMISTK_GetPdpActiveStatus();
    SCI_TRACE_LOW("[MMI_STK] MMISTK_DeactivePDPContext bak_state = %d",bak_state);
    MMISTK_SetPdpActiveStatus(MMISTK_PDP_STATUS_DEACTIVING);
    if (!MMIAPIPDP_Deactive(MMI_MODULE_STK))
    {
       MMISTK_SetPdpActiveStatus(bak_state);
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_DeactivePDPContext fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10055_112_2_18_2_56_13_284,(uint8*)"");
    }
#endif
}

/********************************************************************************
 NAME:			MMISTK_CnfOpenChannelFail
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_CnfOpenChannel(BOOLEAN is_success, SIMAT_GENERAL_PC_RESULT_E result)
{
    uint8 channel_id = 0;
    SIMAT_OPEN_CHANNEL_CNF_T param_out = {0};/*lint !e64*/

    //SCI_TRACE_LOW:"[MMISTK]MMISTK_CnfOpenChannel is_success=%d, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10072_112_2_18_2_56_13_285,(uint8*)"dd", is_success, result);

    param_out.is_bear_desc = TRUE;
    param_out.bear_desc.bear_type = s_mmistk_bear_desc.bear_type;

    if(s_mmistk_is_bear_desc_qos)//the bear_desc_qos exist for bug 1292230
    {
        //for bug 1281784
        if(s_mmistk_bear_desc.bear_type == 0x0B)//If bear type is 0x0B and Non-GBR,the bear_desc_qos contains only need QCI and PDNtype
        {
            if( (s_mmistk_bear_desc.bear_desc_qos[0] >= 5 && s_mmistk_bear_desc.bear_desc_qos[0] <= 9)
                ||(69 == s_mmistk_bear_desc.bear_desc_qos[0]) || (70 == s_mmistk_bear_desc.bear_desc_qos[0])
                ||(79 == s_mmistk_bear_desc.bear_desc_qos[0]) || (80 == s_mmistk_bear_desc.bear_desc_qos[0]))//Non-GBR refer to TS 23.203/6.1.7.2
            {
                s_mmistk_bear_desc.bear_desc_qos[1] = s_mmistk_bear_desc.bear_desc_qos[s_mmistk_bear_desc.bear_len-1];//refer to TS 31.111/8.52.5
                s_mmistk_bear_desc.bear_len = 2;
            }
        }
        SCI_MEMCPY(&param_out.bear_desc, &s_mmistk_bear_desc, sizeof(SIMAT_BEAR_DESC_T));
    }

    param_out.is_buf_size = s_mmistk_is_buf_size;

    if(is_success)
    {
        channel_id = MMISTK_AssignChannelID();
        if(0 == channel_id)
        {
            if (s_mmistk_is_buf_size)
            {
                param_out.buf_size = s_mmistk_buf_size;
            }

            param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            param_out.result = SIMAT_TR_BEYOND_ME_CAP;

            SIMAT_CnfOpenChannelEx(MMISTK_GetSTKSim(), &param_out);

            if(MMISTK_IsAllChannelIDReleased())
            {
                MMISTK_CloseSocket(s_mmistk_sock_id);
                MMISTK_DeactivePDPContext();
            }
            //SCI_TRACE_LOW:"[MMI_STK] MMISTK_AssignChannelID error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10103_112_2_18_2_56_13_286,(uint8*)"");
            return;
        }

        param_out.is_buf_size= s_mmistk_is_buf_size;

        if (s_mmistk_is_buf_size)
        {
            if (s_mmistk_buf_size > MMISTK_OPENCHANNEL_BUF_SIZE_MAX)
            {
                param_out.result = SIMAT_TR_WITH_MODIFICATION;
                param_out.buf_size = MMISTK_OPENCHANNEL_BUF_SIZE_MAX;
            }
            else
            {
                param_out.result = SIMAT_TR_SUCCESS;
                param_out.buf_size = s_mmistk_buf_size;
            }   
        }
        else
        {
            param_out.result = SIMAT_TR_SUCCESS;
        }

        param_out.is_other_address = s_mmistk_is_other_address;
        if(s_mmistk_is_other_address && (0 != s_mmistk_other_address.addr_len))
        {
            SCI_MEMCPY(&param_out.other_address, &s_mmistk_other_address, sizeof(SIMAT_OTHER_ADDRESS_T));
        }

        param_out.is_channel_status = TRUE;
        param_out.channel_status.is_link_establish =TRUE;
        param_out.channel_status.mode_info = SIMAT_CHANNEL_MODE_NO_FURTHER_INFO; //TODO
        param_out.channel_status.channel_id = channel_id; 

        param_out.channel_id = channel_id;

        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        //SCI_TRACE_LOW:"[MMI_STK] SIMAT_CnfOpenChannel success!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10141_112_2_18_2_56_13_287,(uint8*)"");
    }
    else
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = result;
    }

    SIMAT_CnfOpenChannelEx(MMISTK_GetSTKSim(), &param_out);
}

/********************************************************************************
 NAME: OpenChannelWinHandleMsg
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:		
 AUTHOR:
 DATE:
********************************************************************************/
LOCAL MMI_RESULT_E OpenChannelWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    LOCAL BOOLEAN s_is_open_channel_rsp = FALSE;

    //SCI_TRACE_LOW:"[MMI_STK] enter OpenChannelWinHandleMsg, msg_id = 0x%04X, s_mmistk_close_window = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10166_112_2_18_2_56_14_288,(uint8*)"dd", msg_id, s_mmistk_close_window);

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_is_open_channel_rsp = FALSE;
        MMK_SetAtvCtrl(win_id, MMISTK_TEXBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        if(MMITHEME_IsMainScreenLandscape())
        {
            LCD_FillRect(&lcd_dev_info, both_rect.h_rect, mmi_stk_display.bkgd_color);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, both_rect.v_rect, mmi_stk_display.bkgd_color);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if (!s_is_open_channel_rsp)
        {
            s_is_open_channel_rsp = TRUE;

            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_USER_DENY_CALL);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT  //support PDA  for STK
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (msg_id == MSG_CTL_PENOK)
        {
            if (!s_is_open_channel_rsp)
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

                s_is_open_channel_rsp = TRUE;

                switch (src_id)
                {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    if(PNULL != s_mmistk_pdp_info)
                    {
                        if(!MMISTK_ActivePDPContext(MMISTK_GetSTKSim(), s_mmistk_pdp_info->apn_text, s_mmistk_pdp_info->login_text, s_mmistk_pdp_info->psw_text))
                        {
                            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
                        }
                    } 
                    else
                    {
                        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
                    }
                    break;
                  
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    break;

                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_USER_DENY_CALL);
                    break;

                default:
                    break;
                }
                break;
            }
        }
        else
#endif //IGNORE9527
#endif
        if (!s_is_open_channel_rsp)
        {
            s_is_open_channel_rsp = TRUE;

            if(PNULL != s_mmistk_pdp_info)
            {
                if (!MMISTK_ActivePDPContext(MMISTK_GetSTKSim(), s_mmistk_pdp_info->apn_text, s_mmistk_pdp_info->login_text, s_mmistk_pdp_info->psw_text))
                {
                    MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
                }
            }
            else
            {
                MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (!s_is_open_channel_rsp)
        {
            s_is_open_channel_rsp = TRUE;
            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_USER_DENY_CALL);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMI_STK] leave OpenChannelWinHandleMsg result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10258_112_2_18_2_56_14_289,(uint8*)"d",result);

    return (result);
}

/********************************************************************************
  NAME:          CreateOpenChannelVision
  DESCRIPTION:   
  PARAM IN:      
  PARAM OUT:     None
  AUTHOR:        
  DATE:
 ********************************************************************************/
 LOCAL void CreateOpenChannelVision(APP_MN_SIMAT_OPEN_CHANNEL_IND_T* open_channel_ind)
{
#if 0
// no use
    BOOLEAN result = FALSE;
#endif
    BOOLEAN is_open = FALSE;

    if (!MMK_IsOpenWin(MMISTK_OPEN_CHANNEL_WIN_ID)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        //void* ctrl_ptr = PNULL;

        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MMISTK_OPEN_CHANNEL_WIN_ID;
        win_create.func = OpenChannelWinHandleMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ptr = (ADD_DATA)open_channel_ind;

        MMK_CreateWindow( &win_create );	
  
#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(MMISTK_OPEN_CHANNEL_WIN_ID);
#endif

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_OPEN_CHANNEL_WIN_ID, TXT_NULL);

        CreatSoftkeyCtrl(MMISTK_OPEN_CHANNEL_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

        init_data.both_rect = MMITHEME_GetWinClientBothRect(MMISTK_OPEN_CHANNEL_WIN_ID);

        create.ctrl_id = MMISTK_TEXBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = MMISTK_OPEN_CHANNEL_WIN_ID;
        create.init_data_ptr = &init_data;

        MMK_CreateControl(&create);

        //set title
        GUIWIN_CreateTitleDyna(MMISTK_OPEN_CHANNEL_WIN_ID, TXT_NULL);
#if 0
// no use
        result = TRUE;
#endif
        is_open = FALSE;
    }
    else
    {
        is_open = TRUE;
    }

    SetStkWinTitle(open_channel_ind->dual_sys, MMISTK_OPEN_CHANNEL_WIN_ID);

    GUIWIN_SetSoftkeyTextId(MMISTK_OPEN_CHANNEL_WIN_ID, TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, FALSE);

    SetRichtextItemInfo(open_channel_ind->dual_sys,
                                        s_mmistk_open_channel_is_icon_id,
                                        s_mmistk_open_channel_icon_id,
                                        s_mmistk_displaydata,
                                        MMISTK_TEXBOX_CTRL_ID);

    if (is_open)
    {
        GUIWIN_UpdateTitle(MMISTK_OPEN_CHANNEL_WIN_ID);

        if (!MMK_IsFocusWin(MMISTK_OPEN_CHANNEL_WIN_ID))
        {
            MMK_WinGrabFocus(MMISTK_OPEN_CHANNEL_WIN_ID);
        }
        else
        {
            MMK_PostMsg(MMISTK_OPEN_CHANNEL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}

/*****************************************************************************/
// 	Description : Open Channel ????
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenChannelWaitingWin(
                                           MMI_WIN_ID_T      win_id, 	// ???ID
                                           MMI_MESSAGE_ID_E     msg_id, 	// ???????ID
                                           DPARAM 		        param		// ???????
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

#if 0
// no use
/*****************************************************************************/
// 	Description : Create Open Channel Waiting Window
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void CreateOpenChannelWaitingWin(MMI_STRING_T *open_channel_txt_ptr)
{
    if (!MMK_IsOpenWin(MMISTK_OPEN_CHANNEL_WAITING_WIN_ID))
    {
        MMIPUB_OpenWaitWin(1, open_channel_txt_ptr, PNULL, PNULL, MMISTK_OPEN_CHANNEL_WAITING_WIN_ID, IMAGE_NULL,
                                        ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleOpenChannelWaitingWin);

        MMK_SetWinDisplayStyleState(MMISTK_OPEN_CHANNEL_WAITING_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
    }
    else
    {
        MMIPUB_SetWaitWinText(TRUE, 1, open_channel_txt_ptr, PNULL, PNULL, MMISTK_OPEN_CHANNEL_WAITING_WIN_ID);
    }
}
#endif

/********************************************************************************
 NAME:			MMISTK_OnOpenChannelCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnOpenChannelCmd(void* arg)
{
    APP_MN_SIMAT_OPEN_CHANNEL_IND_T *arg_in_ptr = PNULL;
    SIMAT_OPEN_CHANNEL_RELATED_GPRS_T *param_in = PNULL;
    SIMAT_OPEN_CHANNEL_CNF_T param_out = {0};/*lint !e64*/
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#ifdef MMI_OPEN_CHANNEL_SUPPORT
    uint8 channel_id = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
#endif
    uint16  i=0;
    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnOpenChannelCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10438_112_2_18_2_56_14_290,(uint8*)"");

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnOpenChannelCmd arg == PNULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10442_112_2_18_2_56_14_291,(uint8*)"");

        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_MISS_INFO);
        return;
    }

    CheckCloseDisplayTextIRWin();

    arg_in_ptr = (APP_MN_SIMAT_OPEN_CHANNEL_IND_T*)arg;
    param_in = (SIMAT_OPEN_CHANNEL_RELATED_GPRS_T *)(&arg_in_ptr->open_channel);

    dual_sys = arg_in_ptr->dual_sys;

    if (!MMIAPIPHONE_IsSimAvailable(arg_in_ptr->dual_sys))
    {
        param_out.bear_desc.bear_type = param_in->bear_desc.bear_type;
        param_out.is_bear_desc = TRUE;
        
        if (param_in->bear_desc.bear_len > 0)//the length of bear_desc_qos, not contain the bear_type for bug 1292230
        {
            //for bug 1281784
            if(param_in->bear_desc.bear_type == 0x0B)//If bear type is 0x0B and Non-GBR,the bear_desc_qos contains only need QCI and PDNtype
            {
                if( (param_in->bear_desc.bear_desc_qos[0] >= 5 && param_in->bear_desc.bear_desc_qos[0] <= 9)
                    ||(69 == param_in->bear_desc.bear_desc_qos[0]) || (70 == param_in->bear_desc.bear_desc_qos[0])
                    ||(79 == param_in->bear_desc.bear_desc_qos[0]) || (80 == param_in->bear_desc.bear_desc_qos[0]))//Non-GBR refer to TS 23.203/6.1.7.2
                {
                    param_in->bear_desc.bear_desc_qos[1] = param_in->bear_desc.bear_desc_qos[param_in->bear_desc.bear_len-1];//refer to TS 31.111/8.52.5
                    param_in->bear_desc.bear_len = 2;
                }
            }
            SCI_MEMCPY(&param_out.bear_desc, &param_in->bear_desc, sizeof(SIMAT_BEAR_DESC_T));
        }

        if (param_in->buf_size > 0)
        {
            param_out.is_buf_size = TRUE;
            param_out.buf_size = param_in->buf_size;
        }

        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
#ifdef MMI_OPEN_CHANNEL_SUPPORT
        param_out.result = SIMAT_TR_ME_UNABLE;
#else
        param_out.result = SIMAT_TR_BEYOND_ME_CAP;
#endif

        SIMAT_CnfOpenChannelEx(dual_sys, &param_out);

        return;
    }

    if (arg_in_ptr->dual_sys != MMISTK_GetSTKSim())
    {
        CloseAllStkWin();
    }

    MMISTK_SetSTKSim(dual_sys);

    //SCI_TRACE_LOW:"[MMI_STK] is_alpha_id:%d, len:%d is_other_address:%d, is_text_str_login:%d, is_text_str_pwd:%d, is_trans_level:%d, is_data_dest_addr:%d, is_net_access_name:%d,is_icon_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10519_112_2_18_2_56_14_293,(uint8*)"ddddddddd",param_in->is_alpha_id,param_in->alpha_id.len,param_in->is_other_address,param_in->is_text_str_login,param_in->is_text_str_pwd,param_in->is_trans_level,param_in->is_data_dest_addr,param_in->is_net_access_name,param_in->is_icon_id);

 //add trace for network access name
     SCI_TRACE_LOW("[MMI_STK] net_access_name:%s,net_access_name_len:%d",(char*)param_in->net_access_name.net_access_name, (char*)param_in->net_access_name.name_len);
     for(i=0;i<param_in->net_access_name.name_len;i++)//for debug
     	{
     	      SCI_TRACE_LOW("net_access_name[%d]:%x\n",i,param_in->net_access_name.net_access_name[i]);
     	}

    s_mmistk_bear_desc.bear_type = param_in->bear_desc.bear_type;
    if(param_in->bear_desc.bear_len > 0)//the length of bear_desc_qos, not contain the bear_type
    {
        s_mmistk_is_bear_desc_qos = TRUE;
        //SCI_TRACE_LOW:"[MMI_STK] bear_desc.bear_type = %d, len = %d, bear_desc_qos = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10525_112_2_18_2_56_14_294,(uint8*)"dds", param_in->bear_desc.bear_type, param_in->bear_desc.bear_len, param_in->bear_desc.bear_desc_qos);
        SCI_MEMCPY(&s_mmistk_bear_desc, &param_in->bear_desc, sizeof(SIMAT_BEAR_DESC_T));
    }
    else
    {
        s_mmistk_is_bear_desc_qos = FALSE;
    }

    if(param_in->buf_size > 0)
    {
        s_mmistk_is_buf_size = TRUE;
        //SCI_TRACE_LOW:"[MMI_STK] param_in->buf_size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10536_112_2_18_2_56_14_295,(uint8*)"d", param_in->buf_size);
        s_mmistk_buf_size = param_in->buf_size;
    }
    else
    {
        s_mmistk_is_buf_size = FALSE;
    }

    //SCI_TRACE_LOW:"[MMI_STK] param_in->bear_desc.bear_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10544_112_2_18_2_56_14_296,(uint8*)"d", param_in->bear_desc.bear_type);
    if((SIMAT_BEAR_TYPE_GPRS != param_in->bear_desc.bear_type) && 
        (SIMAT_BEAR_TYPE_GPRS_3G != param_in->bear_desc.bear_type)&&
        (SIMAT_BEAR_TYPE_DEFALT_BEAR != param_in->bear_desc.bear_type)
#if defined (PLATFORM_ANTISW3)
        &&(SIMAT_BEAR_TYPE_MAP_GPRS_3G != param_in->bear_desc.bear_type)//for bug1282039 and bug1281784
#endif
        )
    {
        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_BEYOND_ME_CAP);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnOpenChannelCmd because beyond bear type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10549_112_2_18_2_56_14_297,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"[MMI_STK] param_in->data_dest_address.addr_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10553_112_2_18_2_56_14_298,(uint8*)"d", param_in->data_dest_address.addr_type);
    if(SIMAT_ADDRESS_TYPE_IPV4 != param_in->data_dest_address.addr_type)
    {
        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_BEYOND_ME_CAP);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnOpenChannelCmd because beyond dest addr type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10557_112_2_18_2_56_14_299,(uint8*)"");
        return;
    }

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    if (ExistActiveOrSetupCall())
    {
        param_out.bear_desc.bear_type = param_in->bear_desc.bear_type;
        param_out.is_bear_desc = TRUE;
        if (param_in->bear_desc.bear_len > 0)//the length of bear_desc_qos, not contain the bear_type
        {

            //for bug 1281784
            if(param_in->bear_desc.bear_type == 0x0B)//If bear type is 0x0B and Non-GBR,the bear_desc_qos contains only need QCI and PDNtype
            {
                if( (param_in->bear_desc.bear_desc_qos[0] >= 5 && param_in->bear_desc.bear_desc_qos[0] <= 9)
                    ||(69 == param_in->bear_desc.bear_desc_qos[0]) || (70 == param_in->bear_desc.bear_desc_qos[0])
                    ||(79 == param_in->bear_desc.bear_desc_qos[0]) || (80 == param_in->bear_desc.bear_desc_qos[0]) )//Non-GBR refer to TS 23.203/6.1.7.2
                {
                    param_in->bear_desc.bear_desc_qos[1] = param_in->bear_desc.bear_desc_qos[param_in->bear_desc.bear_len-1];//refer to TS 31.111/8.52.5
                    param_in->bear_desc.bear_len = 2;
                }
            }

            SCI_MEMCPY(&param_out.bear_desc, &param_in->bear_desc, sizeof(SIMAT_BEAR_DESC_T));
        }

        if (param_in->buf_size > 0)
        {
            param_out.is_buf_size = TRUE;
            param_out.buf_size = param_in->buf_size;
        }
        param_out.mp_info = SIMAT_MP_BUSY_ON_CALL;
        param_out.result = SIMAT_TR_ME_UNABLE;

        SIMAT_CnfOpenChannelEx(dual_sys, &param_out);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnSetupCallCmd because there is in call already"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10504_112_2_18_2_56_14_292,(uint8*)"");
        return;
    }

    s_mmistk_open_channel_is_icon_id = param_in->is_icon_id;
    if (s_mmistk_open_channel_is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        s_mmistk_open_channel_icon_id.icon_id = param_in->icon_id.icon_id;
        s_mmistk_open_channel_icon_id.is_self_explanatory = param_in->icon_id.is_self_explanatory;
#endif
    }
    
    if(MMISTK_PDP_STATUS_NONE == MMISTK_GetPdpActiveStatus() || MMISTK_PDP_STATUS_DEACTIVED == MMISTK_GetPdpActiveStatus())
    {    
        if(param_in->is_data_dest_addr)
        {
            //SCI_TRACE_LOW:"[MMI_STK] ata_dest_address.addr_len = %d, type = %d, dest_address = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10576_112_2_18_2_56_14_300,(uint8*)"dds", param_in->data_dest_address.addr_len, param_in->data_dest_address.addr_type, param_in->data_dest_address.other_addr);
            SCI_MEMCPY(&s_mmistk_dest_addr, &param_in->data_dest_address, sizeof(SIMAT_OTHER_ADDRESS_T));
        }

        if(param_in->is_trans_level)
        {
            //SCI_TRACE_LOW:"[MMI_STK] port = %d, trans_level_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10582_112_2_18_2_56_14_301,(uint8*)"dd", param_in->trans_level.port, param_in->trans_level.trans_level_type);
            SCI_MEMCPY(&s_mmistk_trans_info, &param_in->trans_level, sizeof(SIMAT_TRANS_LEVEL_T));
        }

        if(param_in->is_other_address && (param_in->other_address.addr_len != 0))
        {
            s_mmistk_is_other_address = TRUE;
            SCI_MEMCPY(&s_mmistk_other_address, &param_in->other_address, sizeof(SIMAT_OTHER_ADDRESS_T));
        }
        else
        {
            s_mmistk_is_other_address = FALSE;
        }

        s_mmistk_pdp_info = (MMISTK_PDPINFO_T*)SCI_ALLOCAZ(sizeof(MMISTK_PDPINFO_T));
        if(PNULL == s_mmistk_pdp_info)
        {
            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
            return;
        }
            
        SCI_MEMSET(s_mmistk_pdp_info->apn_text, 0x00, (sizeof(s_mmistk_pdp_info->apn_text)));
        if(param_in->is_net_access_name && (param_in->net_access_name.name_len > 0))
        {
//            strncpy((char *)s_mmistk_pdp_info->apn_text, (char *)param_in->net_access_name.net_access_name, param_in->net_access_name.name_len);
              //add for bip:for example,if we receive 6TestGp2rs from ps, we must change it to TestGp.rs, and send it to ps
               #if 0 //
               uint16 k=param_in->net_access_name.net_access_name[0];//the max is 40,
               SCI_TRACE_LOW("[MMI_STK] MMISTK_OnOpenChannelCmd net_access_name.name_len=%d,k=%d",param_in->net_access_name.name_len,k);
               k=MIN(k,MN_SIMAT_NETWORK_ACCESS_NAME_LEN);
               strncpy((char *)s_mmistk_pdp_info->apn_text, (char *)(param_in->net_access_name.net_access_name+1),k);
	           if((k+1)<param_in->net_access_name.name_len)//the situation of  6TestGp. is not existed,so,if 6+1<param_in->net_access_name.name_len,the apn must be the "6TestGp2rs"
	           {
	             uint16 m=param_in->net_access_name.net_access_name[k+1];
			     SCI_TRACE_LOW("[MMI_STK] MMISTK_OnOpenChannelCmd m=%d",m);
	             strcat((char *)s_mmistk_pdp_info->apn_text,L".");//use "." to connect the two parts
			     strncat((char *)s_mmistk_pdp_info->apn_text, (char *)(param_in->net_access_name.net_access_name+k+2),m);
	           }
               #endif
               //internet.comcel.com.co  // 8internet6comcel3com2co
               {
                    uint16 current=0;
                    uint16 k=0;
                    uint16 access_name_length = MIN(MN_SIMAT_NETWORK_ACCESS_NAME_LEN,param_in->net_access_name.name_len);

                    k = param_in->net_access_name.net_access_name[current];
                    SCI_TRACE_LOW("[MMI_STK] MMISTK_OnOpenChannelCmd net_access_name.name_len=%d,k=%d",param_in->net_access_name.name_len,k);
                    strncpy((char *)s_mmistk_pdp_info->apn_text, (char *)(param_in->net_access_name.net_access_name+current+1),k);
                    current+=k+1;

                    SCI_TRACE_LOW("[MMI_STK] MMISTK_OnOpenChannelCmd current=%d",current);
                    while (current < access_name_length)
                    {
                        k = param_in->net_access_name.net_access_name[current];
                        SCI_TRACE_LOW("[MMI_STK] MMISTK_OnOpenChannelCmd k=%d",k);
                        if (current+1+k > access_name_length || 0==k )
                        {
                            SCI_TRACE_LOW("[MMI_STK] MMISTK_OnOpenChannelCmd invalid part!break;");
                            break;
                        }
                  
                        strcat((char *)s_mmistk_pdp_info->apn_text,L".");//use "." to connect the two parts
                        strncat((char *)s_mmistk_pdp_info->apn_text, (char *)(param_in->net_access_name.net_access_name+current+1),k);
                        current+=k+1;
                    }
               }
        }
        else
        {
#ifdef BROWSER_SUPPORT
            int16 apn_len = 0;
            uint8 index = 0;
            index = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
            apn_len = SCI_STRLEN((char *)link_setting_ptr->apn);
            strncpy((char *)s_mmistk_pdp_info->apn_text, (char *)link_setting_ptr->apn, MIN(apn_len, MMICONNECTION_MAX_APN_LEN));
#else
            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_BEYOND_ME_CAP);
            return;
#endif
        }

        SCI_MEMSET(s_mmistk_pdp_info->login_text, 0x00, (sizeof(s_mmistk_pdp_info->login_text)));
        if(param_in->is_text_str_login && (param_in->text_str_login.len > 0))
        {
            SIMATStr2Uint8Str(&param_in->text_str_login, s_mmistk_pdp_info->login_text, MN_MAX_SIMAT_DATA_OBJECT_LEN);
        }

        SCI_MEMSET(s_mmistk_pdp_info->psw_text, 0x00, (sizeof(s_mmistk_pdp_info->psw_text)));
        if(param_in->is_text_str_pwd && (param_in->text_str_pwd.len > 0))
        {
            SIMATStr2Uint8Str(&param_in->text_str_pwd, s_mmistk_pdp_info->psw_text, MN_MAX_SIMAT_DATA_OBJECT_LEN);
        }

        //SCI_TRACE_LOW:"[MMI_STK] dual_sys:%d, apn_ptr:%s, login_ptr:%s, psw_ptr:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10630_112_2_18_2_56_15_302,(uint8*)"dsss",dual_sys,(char *)s_mmistk_pdp_info->apn_text,(char *)s_mmistk_pdp_info->login_text,(char *)s_mmistk_pdp_info->psw_text                         );

        if (!MMIAPIPHONE_IsPSAvail(dual_sys))
        {
        
            //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnOpenChannelCmd because sim not ready!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10635_112_2_18_2_56_15_303,(uint8*)"");

            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
            return;
        }
        else
        {
            CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));

            s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;

            if (param_in->is_alpha_id && param_in->alpha_id.len > 0)//((param_in->is_alpha_id && param_in->alpha_id.len >0) || !param_in->is_alpha_id)
            {
                //if (param_in->is_alpha_id)
                {
                    AlphaId2MMIStr(&(param_in->alpha_id), &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
                }
                //else
                //{
                //    MMI_GetLabelTextByLang(TXT_STK_OPEN_CHANNEL_QUERY, &s_mmistk_displaydata);
                //}

                CreateOpenChannelVision(arg_in_ptr);
                return;
            }
            else
            {
                if(!MMISTK_ActivePDPContext(dual_sys, s_mmistk_pdp_info->apn_text, s_mmistk_pdp_info->login_text, s_mmistk_pdp_info->psw_text))
                {
                    MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_ME_UNABLE);
                    //SCI_TRACE_LOW:"[MMI_STK] leave MMITSK_OnOpenChannelCmd because pdp active fail!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10665_112_2_18_2_56_15_304,(uint8*)"");
                    return;
                }
            }
        }
    }
    else if(MMISTK_PDP_STATUS_ACTIVE_OK == MMISTK_GetPdpActiveStatus())
    {
        if((0 == strcmp((char *)s_mmistk_pdp_info->apn_text, (char *)param_in->net_access_name.net_access_name)) &&
                (0 == strcmp((char *)s_mmistk_pdp_info->login_text, (char *)param_in->text_str_login.text_ptr)) &&
                (0 == strcmp((char *)s_mmistk_pdp_info->psw_text, (char *)param_in->text_str_pwd.text_ptr)))
        {
            channel_id = MMISTK_AssignChannelID();
            if(0 == channel_id)
            {
                MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_BEYOND_ME_CAP);
                //SCI_TRACE_LOW:"[MMI_STK] MMISTK_AssignChannelID MMISTK_PDP_STATUS_ACTIVE_OK error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10681_112_2_18_2_56_15_305,(uint8*)"");
                return;
            }

            param_out.bear_desc.bear_type = param_in->bear_desc.bear_type;
            param_out.is_bear_desc = TRUE;

            if(param_in->bear_desc.bear_len > 0)//the length of bear_desc_qos, not contain the bear_type
            {
                //SCI_TRACE_LOW:"[MMI_STK] MMISTK_PDP_STATUS_ACTIVE_OK bear_desc.bear_type = %d, len = %d, bear_desc_qos = %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10689_112_2_18_2_56_15_306,(uint8*)"dds", param_in->bear_desc.bear_type, param_in->bear_desc.bear_len, param_in->bear_desc.bear_desc_qos);

                //for bug 1281784
                if(param_in->bear_desc.bear_type == 0x0B)//If bear type is 0x0B and Non-GBR,the bear_desc_qos contains only need QCI and PDNtype
                {
                    if( (param_in->bear_desc.bear_desc_qos[0] >= 5 && param_in->bear_desc.bear_desc_qos[0] <= 9)
                        ||(69 == param_in->bear_desc.bear_desc_qos[0]) || (70 == param_in->bear_desc.bear_desc_qos[0])
                        ||(79 == param_in->bear_desc.bear_desc_qos[0]) || (80 == param_in->bear_desc.bear_desc_qos[0]) )//Non-GBR refer to TS 23.203/6.1.7.2
                    {
                        param_in->bear_desc.bear_desc_qos[1] = param_in->bear_desc.bear_desc_qos[param_in->bear_desc.bear_len-1];//refer to TS 31.111/8.52.5
                        param_in->bear_desc.bear_len = 2;
                    }
                }

                SCI_MEMCPY(&param_out.bear_desc, &param_in->bear_desc, sizeof(SIMAT_BEAR_DESC_T));
            }

            if(param_in->buf_size > 0)
            {
                param_out.is_buf_size = TRUE;
                //SCI_TRACE_LOW:"[MMI_STK] MMISTK_PDP_STATUS_ACTIVE_OK param_in->buf_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10700_112_2_18_2_56_15_307,(uint8*)"d", param_in->buf_size);
                param_out.buf_size = param_in->buf_size;
            }
            else
            {
                param_out.is_buf_size = FALSE;
            }

            if(param_in->is_other_address)
            {
                param_out.is_other_address = TRUE;
                SCI_MEMCPY(&param_out.other_address, &param_in->other_address, sizeof(SIMAT_OTHER_ADDRESS_T));
            }
            else
            {
                param_out.is_other_address = FALSE;
            }

            param_out.is_channel_status = TRUE;
            param_out.channel_status.is_link_establish =TRUE;
            param_out.channel_status.mode_info = SIMAT_CHANNEL_MODE_NO_FURTHER_INFO; //TODO
            param_out.channel_status.channel_id = channel_id; 

            param_out.channel_id = channel_id;

            param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

            if (s_mmistk_open_channel_is_icon_id)
            {
//                param_out.result = SIMAT_TR_ICON_FAIL_DISPLAY;
                param_out.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
            }
            else
            {
                param_out.result = SIMAT_TR_SUCCESS;
            }

            SIMAT_CnfOpenChannelEx(dual_sys, &param_out);
            //SCI_TRACE_LOW:"[MMI_STK] SIMAT_CnfOpenChannel success!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10738_112_2_18_2_56_15_308,(uint8*)"");
        }
        else
        {
            MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_BEYOND_ME_CAP);
            //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnOpenChannelCmd because MMISTK_PDP_STATUS_ACTIVE_OK apn,psw,login not match!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10743_112_2_18_2_56_15_309,(uint8*)"");
        }
    }
    else
    {
        MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_NE_UNABLE);
        //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnOpenChannelCmd because MMISTK_GetPdpActiveStatus activing!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10749_112_2_18_2_56_15_310,(uint8*)"");
    }
#else
    MMISTK_CnfOpenChannel(FALSE, SIMAT_TR_BEYOND_ME_CAP);
#endif
}

/********************************************************************************
 NAME:			MMISTK_OnCloseChannelCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnCloseChannelCmd(void* arg)
{
    APP_MN_SIMAT_CLOSE_CHANNEL_IND_T *arg_in_ptr = PNULL;
    SIMAT_CLOSE_CHANNEL_T *param_in = PNULL;
    SIMAT_CLOSE_CHANNEL_CNF_T param_out = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnCloseChannelCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10770_112_2_18_2_56_15_311,(uint8*)"");

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnCloseChannelCmd arg == PNULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10774_112_2_18_2_56_15_312,(uint8*)"");
        return;
    }
    //bug1359976
    SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnCloseChannelCmd timer_id is %d",s_mmistk_socket_get_data_timer_id);
    if (0 != s_mmistk_socket_get_data_timer_id)
    {
        MMK_StopTimer(s_mmistk_socket_get_data_timer_id);
        s_mmistk_socket_get_data_timer_id = 0;
    }
    CheckCloseDisplayTextIRWin();
    
    arg_in_ptr = (APP_MN_SIMAT_CLOSE_CHANNEL_IND_T*)arg;
    param_in = (SIMAT_CLOSE_CHANNEL_T *)(&arg_in_ptr->close_channel);

    dual_sys = arg_in_ptr->dual_sys;

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    param_out.channel_id = param_in->channel_id;
    if(param_in->channel_id != MMISTK_GetAvailableChannelID())
    {
        param_out.mp_info = SIMAT_BI_CHANNEL_ID_INVALID_CAUSE;
        param_out.result = SIMAT_TR_BEAR_INDEPEND_ERROR;
    }
    else
    {
        param_out.mp_info = SIMAT_BI_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_SUCCESS;
    }

    SIMAT_CnfCloseChannelEx(dual_sys, &param_out);

    MMISTK_ReleaseChannelID(param_in->channel_id);

    if(MMISTK_IsAllChannelIDReleased())
    {
        MMISTK_CloseSocket(s_mmistk_sock_id);
        MMISTK_DeactivePDPContext();
    }
#else
    param_out.mp_info = SIMAT_BI_CHANNEL_ID_INVALID_CAUSE;
    param_out.result = SIMAT_TR_BEYOND_ME_CAP;

    SIMAT_CnfCloseChannelEx(dual_sys, &param_out);
#endif
}

/********************************************************************************
 NAME:			MMISTK_OnGetChannelStatusCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnGetChannelStatusCmd(void* arg)
{
    APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T *arg_in_ptr = PNULL;
    SIMAT_GET_CHANNEL_STATUS_CNF_T param_out = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 channel_id = 0;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnGetChannelStatusCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10829_112_2_18_2_56_15_313,(uint8*)"");

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnGetChannelStatusCmd arg == PNULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10833_112_2_18_2_56_15_314,(uint8*)"");
        return;
    }

    CheckCloseDisplayTextIRWin();

    arg_in_ptr = (APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T*)arg;

    dual_sys = arg_in_ptr->dual_sys;

    channel_id = MMISTK_GetAvailableChannelID();

 #ifdef MMI_OPEN_CHANNEL_SUPPORT
   if(0 != channel_id)
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_SUCCESS;
        param_out.channel_id = channel_id;
        param_out.channel_status.channel_id = channel_id;
        param_out.channel_status.is_link_establish = TRUE;
        param_out.channel_status.mode_info = SIMAT_CHANNEL_MODE_NO_FURTHER_INFO;
        //SCI_TRACE_LOW:"[MMI_STK] GetChannelStatus success ChannelID = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10854_112_2_18_2_56_15_315,(uint8*)"d", channel_id);
    }
    else
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_SUCCESS;//SIMAT_TR_ME_UNABLE;
        //SCI_TRACE_LOW:"[MMI_STK] GetChannelStatus failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10860_112_2_18_2_56_15_316,(uint8*)"");
    }

    SIMAT_CnfGetChannelStatusEx(dual_sys, &param_out);
#else
    param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    param_out.result = SIMAT_TR_BEYOND_ME_CAP;

    SIMAT_CnfGetChannelStatusEx(dual_sys, &param_out);
#endif

    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_OnGetChannelStatusCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10871_112_2_18_2_56_15_317,(uint8*)"");
}

/********************************************************************************
 NAME:			MMISTK_GetSocketData
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void MMISTK_GetSocketData(void)
{
#ifdef MMI_OPEN_CHANNEL_SUPPORT
#if 0
// no use
   int ret = -1;
#endif
    int status = 0;
    uint32 to_recv_len = 0;
    SIMAT_DATA_AVAILABLE_IND_T  data_info = {0};
    
    status = sci_sock_getsockopt(s_mmistk_sock_id,SO_RXDATA,&to_recv_len);

    if (0 != s_mmistk_socket_get_data_timer_id)
    {
        MMK_StopTimer(s_mmistk_socket_get_data_timer_id);
        s_mmistk_socket_get_data_timer_id = 0;
    }

    if((0 == status) && (0 != to_recv_len))
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_GetSocketDataLen = %ld, channel id= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10899_112_2_18_2_56_15_318,(uint8*)"dd", to_recv_len, MMISTK_GetAvailableChannelID());
        s_mmistk_data_receive_len = to_recv_len;
        s_mmistk_get_data_times = 0;
        if(to_recv_len > MMISTK_CNF_RECEIVEDATA_MAX_LEN)
        {
            data_info.data_len = 0xFF;
        }
        else
        {
            data_info.data_len = to_recv_len;
        }
        data_info.channel_status.channel_id = MMISTK_GetAvailableChannelID();
        data_info.channel_status.is_link_establish = TRUE;
#ifndef WIN32
        SIMAT_IndDataAvailableEx(MMISTK_GetSTKSim(), &data_info);
#endif
        
        s_mmistk_data_receive_ptr = SCI_ALLOCA((to_recv_len + 1) * sizeof(uint8));
        SCI_MEMSET(s_mmistk_data_receive_ptr, 0x00, ((to_recv_len + 1) * sizeof(uint8)));  
        s_mmistk_data_receive_ptr_copy = s_mmistk_data_receive_ptr;
        
#if 0
// no use        
        ret = sci_sock_recv((long)s_mmistk_sock_id, (char *)s_mmistk_data_receive_ptr, (int)to_recv_len, 0);
#else
        sci_sock_recv((long)s_mmistk_sock_id, (char *)s_mmistk_data_receive_ptr, (int)to_recv_len, 0);
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_GetSocketData timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10924_112_2_18_2_56_15_319,(uint8*)"");
        s_mmistk_socket_get_data_timer_id = MMK_CreateTimerCallback(MMISTK_SOCKET_GET_DATALEN_DURATION, MMISTK_HandleSTKTimer, NULL, FALSE);
    }
#endif
}

/********************************************************************************
 NAME:			HandleSTKSocketGetDataTimer
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
LOCAL void HandleSTKSocketGetDataTimer(void)
{    
    //SCI_TRACE_LOW:"[MMI_STK] HandleSTKSocketGetDataTimer times = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10939_112_2_18_2_56_15_320,(uint8*)"d", s_mmistk_get_data_times);
    if(s_mmistk_get_data_times < MMISTK_GET_DATA_TIMES_MAX)
    {
        s_mmistk_get_data_times++;
        MMISTK_GetSocketData();
    }
    else
    {
        //time out prompt
        MMISTK_CloseSocket(s_mmistk_sock_id);
        MMISTK_DeactivePDPContext();
        s_mmistk_get_data_times = 0;

        MMISTK_OpenAlertWin(TXT_STK_OVERTIME_RETURN, IMAGE_PUBWIN_WARNING, PNULL);
    }
}

/********************************************************************************
 NAME:			MMISTK_OnSendDataCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnSendDataCmd(void* arg)
{
    APP_MN_SIMAT_SEND_DATA_IND_T *arg_in_ptr = PNULL;
    SIMAT_SEND_DATA_T *param_in = PNULL;
    SIMAT_SEND_DATA_CNF_T param_out = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnSendDataCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10969_112_2_18_2_56_16_321,(uint8*)"");

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnSendDataCmd arg == PNULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_10973_112_2_18_2_56_16_322,(uint8*)"");
        return;
    }

    CheckCloseDisplayTextIRWin();

    arg_in_ptr = (APP_MN_SIMAT_SEND_DATA_IND_T*)arg;
    param_in = (SIMAT_SEND_DATA_T *)(&arg_in_ptr->send_data);

    dual_sys = arg_in_ptr->dual_sys;

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    if(param_in->channel_id != MMISTK_GetAvailableChannelID())
    {
        param_out.mp_info = (SIMAT_ME_PROBLEM_E)SIMAT_BI_CHANNEL_ID_INVALID_CAUSE;
        param_out.result = SIMAT_TR_BEAR_INDEPEND_ERROR;

        SIMAT_CnfSendDataEx(dual_sys, &param_out);
        return;
    }

    s_mmistk_data_send_copy_ptr=s_mmistk_data_send_ptr;

    s_mmistk_data_send_ptr = SCI_ALLOCA((s_mmistk_data_send_len + param_in->channel_data.data_len+ 1) * sizeof(uint8));
    if(s_mmistk_data_send_ptr==PNULL)
    {
        SCI_TRACE_LOW("[MMI_STK] MMISTK_OnSendDataCmd SCI_ALLOCA failed");
        if(s_mmistk_data_send_copy_ptr!=PNULL)
        {
            SCI_FREE(s_mmistk_data_send_copy_ptr);
            s_mmistk_data_send_copy_ptr = PNULL;
        }
        return;
    }
    SCI_MEMSET(s_mmistk_data_send_ptr, 0x00, ((s_mmistk_data_send_len + param_in->channel_data.data_len + 1) * sizeof(uint8)));
    if(s_mmistk_data_send_copy_ptr!=PNULL)
    {
        //1.save pre data , 2.release
        SCI_MEMCPY(s_mmistk_data_send_ptr, s_mmistk_data_send_copy_ptr,s_mmistk_data_send_len);
        SCI_FREE(s_mmistk_data_send_copy_ptr);
        s_mmistk_data_send_copy_ptr = PNULL;
    }
    //save cur data
    SCI_MEMCPY(s_mmistk_data_send_ptr + s_mmistk_data_send_len, param_in->channel_data.data, param_in->channel_data.data_len);

    s_mmistk_data_send_len +=param_in->channel_data.data_len;

    if(param_in->is_send_immediate)
    {
        MMISTK_SocketSendData(s_mmistk_sock_id, (char *)s_mmistk_data_send_ptr, s_mmistk_data_send_len);

        if(PNULL != s_mmistk_data_send_ptr)
        {
            SCI_FREE(s_mmistk_data_send_ptr);
            s_mmistk_data_send_ptr = PNULL;
        }
        s_mmistk_data_send_len = 0;
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_SUCCESS;

        if(s_mmistk_buf_size - s_mmistk_data_send_len > MMISTK_CNF_SENDDATA_MAX_LEN)
        {
            param_out.channel_data_len = 0xFF; //socket buffer - param_in->channel_data.data_len > 200   0xFF
        }
        else
        {
            param_out.channel_data_len = s_mmistk_buf_size - s_mmistk_data_send_len;
        }

        SIMAT_CnfSendDataEx(dual_sys, &param_out);
        MMISTK_GetSocketData();
    }
    else
    {
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.result = SIMAT_TR_SUCCESS;

        if(s_mmistk_buf_size - s_mmistk_data_send_len > MMISTK_CNF_SENDDATA_MAX_LEN)
        {
            param_out.channel_data_len = 0xFF; //socket buffer - param_in->channel_data.data_len > 200   0xFF
        }
        else
        {
            param_out.channel_data_len = s_mmistk_buf_size - s_mmistk_data_send_len;
        }

        SIMAT_CnfSendDataEx(dual_sys, &param_out);
    }
#ifdef WIN32
    {
        char stk_case_str[10] = {0};
        uint8 case_step = 0;
        SIMAT_DATA_AVAILABLE_IND_T data_info = {0};
        
        if(SIMATDUM_IsInTestCase(dual_sys, &stk_case_str, &case_step))
        {
            if(strcmp(stk_case_str,"248") == 0)
            {
                s_mmistk_data_receive_len = 1000;
                s_mmistk_data_receive_ptr = SCI_ALLOCA((s_mmistk_data_receive_len + 1) * sizeof(uint8));
                SCI_MEMSET(s_mmistk_data_receive_ptr, 0x00, ((s_mmistk_data_receive_len + 1) * sizeof(uint8)));  
                s_mmistk_data_receive_ptr_copy = s_mmistk_data_receive_ptr;
                SCI_MEMCPY(s_mmistk_data_receive_ptr, data_receive, s_mmistk_data_receive_len);
                data_info.data_len = 0xFF;
                data_info.channel_status.channel_id = MMISTK_GetAvailableChannelID();
                data_info.channel_status.is_link_establish = TRUE;
                SIMAT_IndDataAvailableEx(dual_sys, &data_info);
            }
        }
    }
#endif
#else
    param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    param_out.result = SIMAT_TR_BEYOND_ME_CAP;

    SIMAT_CnfSendDataEx(dual_sys, &param_out);
#endif
}

/********************************************************************************
 NAME:			MMISTK_OnReceiveDataCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnReceiveDataCmd(void* arg)
{
    APP_MN_SIMAT_RECEIVE_DATA_IND_T *arg_in_ptr = PNULL;
    SIMAT_RECEIVE_DATA_T *param_in = PNULL;
    SIMAT_RECEIVE_DATA_CNF_T param_out = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 data_len = 0;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_OnReceiveDataCmd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11061_112_2_18_2_56_16_323,(uint8*)"");
    SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnReceiveDataCmd s_mmistk_data_receive_len:%d ",s_mmistk_data_receive_len);

    if(PNULL == arg)
    {
        //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnReceiveDataCmd arg == PNULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11065_112_2_18_2_56_16_324,(uint8*)"");
        return;
    }

    CheckCloseDisplayTextIRWin();

    arg_in_ptr = (APP_MN_SIMAT_RECEIVE_DATA_IND_T*)arg;
    param_in = (SIMAT_RECEIVE_DATA_T *)(&arg_in_ptr->receive_data);

    SCI_TRACE_LOW("[MMI_STK] enter MMISTK_OnReceiveDataCmd channel_data_len:%d s_mmistk_data_receive_len:%d ",param_in->channel_data_len,s_mmistk_data_receive_len);
    
    dual_sys = arg_in_ptr->dual_sys;

#ifdef MMI_OPEN_CHANNEL_SUPPORT
    if(PNULL != s_mmistk_data_receive_ptr)
    {
        if(param_in->channel_data_len >= MMISTK_CNF_RECEIVEDATA_MAX_LEN)
        {
            if(s_mmistk_data_receive_len - MMISTK_CNF_RECEIVEDATA_MAX_LEN > MMISTK_CNF_RECEIVEDATA_MAX_LEN)
            {
                data_len = 0xFF;
            }
            else
            {
                data_len = s_mmistk_data_receive_len - MMISTK_CNF_RECEIVEDATA_MAX_LEN;
            }
            SCI_MEMCPY((char *)param_out.channel_data.data, (char *)s_mmistk_data_receive_ptr, MMISTK_CNF_RECEIVEDATA_MAX_LEN);
            s_mmistk_data_receive_len = s_mmistk_data_receive_len - MMISTK_CNF_RECEIVEDATA_MAX_LEN;
            s_mmistk_data_receive_ptr = s_mmistk_data_receive_ptr + MMISTK_CNF_RECEIVEDATA_MAX_LEN;
            param_out.channel_data.data_len = MMISTK_CNF_RECEIVEDATA_MAX_LEN;
            if(0 == data_len)
            {
                if(PNULL != s_mmistk_data_receive_ptr_copy)
                {
                    SCI_FREE(s_mmistk_data_receive_ptr_copy);
                    s_mmistk_data_receive_ptr_copy = PNULL;
                }
            }
        }
        else if (s_mmistk_data_receive_len > param_in->channel_data_len)
        {
            if(s_mmistk_data_receive_len - param_in->channel_data_len > MMISTK_CNF_RECEIVEDATA_MAX_LEN)
            {
                data_len = 0xFF;
            }
            else
            {
                data_len = s_mmistk_data_receive_len - param_in->channel_data_len;
            }
            SCI_MEMCPY((char *)param_out.channel_data.data, (char *)s_mmistk_data_receive_ptr, param_in->channel_data_len);
            s_mmistk_data_receive_len = s_mmistk_data_receive_len - param_in->channel_data_len;
            s_mmistk_data_receive_ptr = s_mmistk_data_receive_ptr + param_in->channel_data_len;
            param_out.channel_data.data_len = param_in->channel_data_len;
            if(0 == data_len)
            {
                if(PNULL != s_mmistk_data_receive_ptr_copy)
                {
                    SCI_FREE(s_mmistk_data_receive_ptr_copy);
                    s_mmistk_data_receive_ptr_copy = PNULL;
                }
            }           
        }		
        else
        {
            data_len = 0;
            SCI_MEMCPY((char *)param_out.channel_data.data, (char *)s_mmistk_data_receive_ptr, s_mmistk_data_receive_len);
            param_out.channel_data.data_len = s_mmistk_data_receive_len;
            s_mmistk_data_receive_len = 0;
            if(PNULL != s_mmistk_data_receive_ptr_copy)
            {
                SCI_FREE(s_mmistk_data_receive_ptr_copy);
                s_mmistk_data_receive_ptr_copy = PNULL;
            }
        }

        param_out.result = SIMAT_TR_SUCCESS;
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.channel_data_len = data_len;
    }
    else
    {
        param_out.result = SIMAT_TR_BEYOND_ME_CAP;
        param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        param_out.channel_data_len = data_len;
    }
    
    SIMAT_CnfReceiveDataEx(dual_sys, &param_out);
#else
    param_out.result = SIMAT_TR_BEYOND_ME_CAP;
    param_out.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    param_out.channel_data_len = data_len;
    
    SIMAT_CnfReceiveDataEx(dual_sys, &param_out);
#endif
}

/*****************************************************************************/
// 	Description : MMISTK_IsStkRefresh
//	Global resource dependence : 
//   Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISTK_IsStkRefresh(void)
{
    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_IsStkRefresh s_is_stk_refresh=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11142_112_2_18_2_56_16_325,(uint8*)"d", s_is_stk_refresh);

    return s_is_stk_refresh;
}

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatSoftkeyCtrl(
                            MMI_HANDLE_T    win_handle,     //win id
                            MMI_TEXT_ID_T   leftsoft_id,
                            MMI_TEXT_ID_T   midsoft_id,
                            MMI_TEXT_ID_T   rightsoft_id
                            )
{
    MMI_CTRL_ID_T	        softkey_ctrl_id = 0;
    MMI_CONTROL_CREATE_T    softkey_create = {0};
    GUISOFTKEY_INIT_DATA_T	softkey_data = {0};

    softkey_data.leftsoft_id  = leftsoft_id;
    softkey_data.midsoft_id	  = midsoft_id;
    softkey_data.rightsoft_id = rightsoft_id;

    softkey_ctrl_id                  = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id           = softkey_ctrl_id;
    softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr     = &softkey_data;

#ifdef MMI_PDA_SUPPORT
    MMK_SetWinDisplayStyleState( win_handle, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );
#endif

    MMK_CreateControl(&softkey_create);
    MMK_SetWinSoftkeyCtrlId(win_handle,softkey_ctrl_id);
}

/*****************************************************************************/
// 	Description : MMISTK_GetStkLang
//	Global resource dependence : 
//   Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMISTK_GetStkLang(SIMAT_DO_LANG_T *lang_ptr)
{
    uint32 i = 0;
    uint16 lang_noti_num = sizeof(s_lang_noti)/sizeof(s_lang_noti[0]);
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;

    if(PNULL == lang_ptr)
    {
        //SCI_TRACE_LOW:" MMISTK_GetStkLang ::PNULL == lang_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11192_112_2_18_2_56_16_326,(uint8*)"");
        return;
    }

    MMIAPISET_GetLanguageType(&language_type);

    for (i = 0; i < lang_noti_num; i++)
    {
        if (language_type == (MMISET_LANGUAGE_TYPE_E)s_lang_noti[i].lang_type)
        {
            break;
        }
    }

    if (i < lang_noti_num)
    {
        lang_ptr->lang[0] = s_lang_noti[i].lang_noti[0];
        lang_ptr->lang[1] = s_lang_noti[i].lang_noti[1];
    }
    else
    {
        lang_ptr->lang[0] = 'e';
        lang_ptr->lang[1] = 'n';
    }
}

/********************************************************************************
 NAME:      MMISTK_OnRelUSSD
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMISTK_OnRelUSSD(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"[MMI_STK] MMISTK_OnRelUSSD, s_mmistk_sendussd_waiting_respond=%d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11225_112_2_18_2_56_16_327,(uint8*)"dd", s_mmistk_sendussd_waiting_respond, dual_sys);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    if(!s_mmistk_sendussd_waiting_respond)
    {
    	return FALSE;
    }
    s_mmistk_sendussd_waiting_respond = FALSE;

    if (s_mmistk_sendss_waiting_respond)
    {
        SIMAT_SEND_SS_CNF_T ss_cnf = {0};

        s_mmistk_sendss_waiting_respond = FALSE;

        ss_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
        ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        SIMAT_CnfSendSsEx((uint8)dual_sys, &ss_cnf);
    }
    else
    {
        SIMAT_SEND_USSD_CNF_T ussd_cnf = {0};

        ussd_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
        ussd_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

        SIMAT_CnfSendUssdEx((uint8)dual_sys, &ussd_cnf);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle stk timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMISTK_HandleSTKTimer(uint8 timer_id, uint32 param )
{

    //SCI_TRACE_LOW:"MMISTK_HandleSTKTimer timer_id=%d,playtone_timer_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11268_112_2_18_2_56_16_328,(uint8*)"dd", timer_id, s_mmistk_playtone_timer_id);

    if(0 == timer_id)
    {
        //SCI_TRACE_LOW:" MMISTK_GetStkLang ::0 == timer_id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11272_112_2_18_2_56_16_329,(uint8*)"");
        return;
    }

    if (timer_id == s_mmistk_playtone_timer_id)
    {
        HandlePlaytoneTimer();
    }
    else if (timer_id == s_mmistk_setupcall_duration_timer_id)
    {
        HandleSetupCallDurationTimer();
    }
    else if (timer_id == s_mmistk_dtmf_timer_id)
    {
        HandleSTKSendDtmfStr(MMISTK_GetSTKSim());
    }
    else if(timer_id == s_mmistk_connect_socket_timer_id)
    {
        HandleSTKReConnectSocketTimer();
    }
    else if(timer_id == s_mmistk_socket_get_data_timer_id)
    {
        HandleSTKSocketGetDataTimer();
    }
}

/*****************************************************************************/
//  Description : handle play tone timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void HandlePlaytoneTimer(void)
{
    SIMAT_PLAY_TONE_CNF_T playtone_cnf = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};

    //SCI_TRACE_LOW:"[MMI_STK] after play duration, Timer(%d) arrived"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11306_112_2_18_2_56_16_330,(uint8*)"d", s_mmistk_playtone_timer_id);

    MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);

    if (s_mmistk_playtone_cnf) 
    {
        playtone_cnf.result = SIMAT_TR_SUCCESS;
        playtone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        s_mmistk_playtone_cnf = FALSE;
        s_mmistk_tone_is_playing[MMISTK_GetSTKSim()] = FALSE;
        SIMAT_CnfPlayToneEx(MMISTK_GetSTKSim(), &playtone_cnf);
    }
    s_mmistk_playtone_timer_id = 0;
    s_mmistk_close_window = MMISTK_CLOSE_PLAYTONE_WIN;
}

/*****************************************************************************/
//  Description : handle setup call duratin timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void HandleSetupCallDurationTimer(void)
{
    SIMAT_SETUP_CALL_CNF_T setup_call_cnf = {0};

    s_is_mmistk_setup_call = FALSE;

    MMIAPICC_ReleaseCallByRedkey();

    setup_call_cnf.result = SIMAT_TR_NE_UNABLE;
    setup_call_cnf.ne_info = (SIMAT_NE_PROBLEM_E)(MN_CAUSE_USER_BUSY | 0x80);//user busy
    setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

    SIMAT_CnfSetupCallEx(MMISTK_GetSTKSim(), &setup_call_cnf);

    s_mmistk_close_window = MMISTK_CLOSE_SETUP_CALL_WIN;

    s_mmistk_setupcall_duration_timer_id = 0;
}

/*****************************************************************************/
//     Description : to handle the send dtmf window
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendDtmfWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    SIMAT_SEND_DTMF_CNF_T dtmf_cnf = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        if (0 != s_mmistk_dtmf_timer_id)
        {
            MMK_StopTimer(s_mmistk_dtmf_timer_id);
            s_mmistk_dtmf_timer_id = 0;

            dtmf_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            dtmf_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;

            SIMAT_CnfSendDtmfEx(MMISTK_GetSTKSim(), &dtmf_cnf);
        }
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        DestroySTKStrMemory(&(s_mmistk_dtmf_icon_bmp.bit_ptr));
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    
    case MSG_FULL_PAINT:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
        if (s_mmistk_dtmf_is_icon_id && s_mmistk_icon_display_result)
        {
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            GUI_RECT_T pubwin_rect = MMITHEME_GetMsgBoxRect();
            GUI_BOTH_RECT_T anim_disp_rect = {0};
            GUI_POINT_T dis_point = {0};

            anim_disp_rect.h_rect = pubwin_rect;
            anim_disp_rect.v_rect = pubwin_rect;

#ifdef MAINLCD_SIZE_320X480
            anim_disp_rect.v_rect.top += 60;
            anim_disp_rect.v_rect.bottom -= 60;
            anim_disp_rect.v_rect.left += 60;
            anim_disp_rect.v_rect.right -= 60; 
#else
            anim_disp_rect.v_rect.top += 40;
            anim_disp_rect.v_rect.bottom -= 40;
            anim_disp_rect.v_rect.left += 40;
            anim_disp_rect.v_rect.right -= 40;
#endif
            dis_point.x = anim_disp_rect.v_rect.left;
            dis_point.y = anim_disp_rect.v_rect.top;

            MMIPUB_GetPubWinLayer(win_id, &lcd_dev_info);

            s_mmistk_icon_display_result = GUIIMG_DisplayBmp(FALSE,
                                                            &(anim_disp_rect.v_rect),
                                                            &dis_point,
                                                            &s_mmistk_dtmf_icon_bmp,
                                                            &lcd_dev_info);
        }
#endif
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//     Description : send dtmf
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void HandleSTKSendDtmfStr(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_send_success = TRUE;
    SIMAT_SEND_DTMF_CNF_T dtmf_cnf = {SIMAT_TR_SUCCESS, SIMAT_MP_NO_SPECIFIC_CAUSE};

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:" HandleSTKSendDtmfStr ::dual_sys >= MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11426_112_2_18_2_56_16_331,(uint8*)"");
        return;
    }

    if (0 != s_mmistk_dtmf_timer_id)
    {
        MMK_StopTimer(s_mmistk_dtmf_timer_id);
        s_mmistk_dtmf_timer_id = 0;
    }

    if (PNULL != s_mmistk_dtmf_string)
    {
        for (; s_mmistk_dtmf_index < s_mmistk_dtmf_len; s_mmistk_dtmf_index++) 
        {
            //SCI_TRACE_LOW:"[MMI_STK]HandleSTKSendDtmfStr, the dtmf length is %d, the dtmf value is 0x%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11440_112_2_18_2_56_16_332,(uint8*)"dd",s_mmistk_dtmf_len,s_mmistk_dtmf_string[s_mmistk_dtmf_index]);

            if ('p' != s_mmistk_dtmf_string[s_mmistk_dtmf_index] && 'P' != s_mmistk_dtmf_string[s_mmistk_dtmf_index])
            {
                if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
                {
                    if (CC_RESULT_SUCCESS != MMIAPICC_SendDtmfByChar(dual_sys, s_mmistk_dtmf_string[s_mmistk_dtmf_index]))
                    {
                        is_send_success = FALSE;
                        dtmf_cnf.result = SIMAT_TR_ME_UNABLE;
                        dtmf_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                        break;
                    }
                }
                else
                {
                    is_send_success = FALSE;
                    dtmf_cnf.result = SIMAT_TR_ME_UNABLE;
                    dtmf_cnf.mp_info = SIMAT_MP_NOT_IN_SPEECH_CALL;
                    //SCI_TRACE_LOW:"[MMI_STK]HandleSTKSendDtmfStr, is not in speech now, send dtmf fail"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11459_112_2_18_2_56_16_333,(uint8*)"");
                    break;
                }
            }
            else
            {
                if((s_mmistk_dtmf_index >= 1) && (('p' == s_mmistk_dtmf_string[s_mmistk_dtmf_index - 1] ) || ( 'P' == s_mmistk_dtmf_string[s_mmistk_dtmf_index - 1])))
                {
                    s_mmistk_dtmf_timer_id = MMK_CreateTimerCallback(MMISTK_DTMF_DURATION,  MMISTK_HandleSTKTimer, NULL, FALSE);
                }
                else
                {
                    s_mmistk_dtmf_timer_id = MMK_CreateTimerCallback(MMISTK_DTMF_FIRST_DURATION,  MMISTK_HandleSTKTimer, NULL, FALSE);
                }
                s_mmistk_dtmf_index++;
                break;
            }
        }
    }
    else
    {
        is_send_success = FALSE;
    }

    if (!is_send_success)//send fail
    {
        SIMAT_CnfSendDtmfEx(dual_sys, &dtmf_cnf);

        if (s_is_mmi_dtmf_pause)
        {
            MMK_CloseWin(MMISTK_SEND_DTMF_WIN_ID);
        }
    }
    else if (s_mmistk_dtmf_index >= s_mmistk_dtmf_len)//send complete
    {
        if (s_mmistk_dtmf_is_icon_id)
        {
//            dtmf_cnf.result = SIMAT_TR_ICON_FAIL_DISPLAY;
            dtmf_cnf.result = s_mmistk_icon_display_result ? SIMAT_TR_SUCCESS : SIMAT_TR_ICON_FAIL_DISPLAY;
        }
        else
        {
            dtmf_cnf.result = SIMAT_TR_SUCCESS;
        }
    
        SIMAT_CnfSendDtmfEx(dual_sys, &dtmf_cnf);

        if (s_is_mmi_dtmf_pause)
        {
            MMK_CloseWin(MMISTK_SEND_DTMF_WIN_ID);
        }
    }
}

#if 0
// no use
/*****************************************************************************/
//  Description : Is Focus STK Win
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Is_FocusSTKWin(void)
{
    uint32 i = 0;

    for (i = MMISTK_WIN_ID_START; i < MMISTK_MAX_WIN_ID; i++)
    {
        if (MMK_IsFocusWin(i))
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : Is Focus STK Win
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISTK_CloseDisplayTextWin(void)
{
    MMK_CloseWin(MMISTK_DISPLAYTEXT_WIN_ID);
}

/********************************************************************************
 NAME:      MMISTK_BrowserTermination
 DESCRIPTION:
 AUTHOR: kun.yu
 DATE:		
********************************************************************************/
PUBLIC void MMISTK_BrowserTermination(MN_DUAL_SYS_E dual_sys)
{
#ifdef BROWSER_SUPPORT
    SIMAT_BROWSER_TERMINATION_IND_T brw_terminal = {0};
    
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMISTK_BrowserTermination dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11544_112_2_18_2_56_17_334,(uint8*)"d", dual_sys);
        return;
    }

    brw_terminal.termination_cause = SIMAT_BROWSER_TERMINATION_BY_TERMINAL;
    SIMAT_IndBrowserTerminationEx(dual_sys, &brw_terminal);
#endif    
}

/*****************************************************************************/
//  Description : MMISTK_OpenAlertWin
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMISTK_OpenAlertWin(MMI_TEXT_ID_T text_id, MMI_IMAGE_ID_T image_id, MMIPUB_HANDLE_FUNC handle_func)
{
    MMI_WIN_ID_T alert_win_id = MMISTK_ALERT_WIN_ID;
    MMI_STRING_T              tip_content  = {0};
    WATCH_SOFTKEY_TEXT_ID_T   softket = {TXT_NULL,TXT_NULL,TXT_NULL};

    MMK_CloseWin(alert_win_id);

    MMI_GetLabelTextByLang(text_id, &tip_content);
    WatchCOM_NoteWin_1Line_Enter(alert_win_id,&tip_content,0,softket,PNULL);

    //MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
}
/*****************************************************************************/
//  Description : MMISTK_OpenAlertWinEx
//  Global resource dependence :
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void MMISTK_OpenAlertWinByTextPtr(MMI_STRING_T   *text1_ptr)
{
    MMI_WIN_ID_T alert_win_id = MMISTK_ALERT_WIN_ID;
    WATCH_SOFTKEY_TEXT_ID_T   softket = {TXT_NULL,TXT_NULL,TXT_NULL};
    MMK_CloseWin(alert_win_id);
    WatchCOM_NoteWin_1Line_Enter(alert_win_id,text1_ptr,0,softket,PNULL);
}

/*****************************************************************************/
// 	Description : MMIAPISTK_CheckCloseDisplayTextIRWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISTK_CheckCloseDisplayTextIRWin(void)
{
    CheckCloseDisplayTextIRWin();
}

/*****************************************************************************/
//     Description : Get Stk Name
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISTK_GetStkName(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_name_ptr)
{
    BOOLEAN is_exist_stk_name = FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX
        || PNULL == stk_name_ptr)
    {
        return is_exist_stk_name;
    }

    if (MMISTK_IsStkCard(dual_sys)
        &&PNULL != s_mmistk_toplevel_menu_info[dual_sys]
        && 0 != s_mmistk_toplevel_menu_info[dual_sys]->menu_title_len)
    {
        is_exist_stk_name = TRUE;
        stk_name_ptr->wstr_ptr = s_mmistk_toplevel_menu_info[dual_sys]->menu_title;
        stk_name_ptr->wstr_len = s_mmistk_toplevel_menu_info[dual_sys]->menu_title_len;
    }
    else
    {
        is_exist_stk_name = FALSE;
        stk_name_ptr->wstr_len = 0;
    }

    return is_exist_stk_name;
}

/*****************************************************************************/
//     Description : Create STK Buffer Memory
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateSTKBufferMemory(void** stk_buffer_ptr, uint16 buffer_size)
{
    DestroySTKStrMemory(stk_buffer_ptr);

    *stk_buffer_ptr = (void *)SCI_ALLOCAZ(buffer_size);

    if (PNULL != *stk_buffer_ptr)
    {
        //SCI_MEMSET(*stk_buffer_ptr, 0x00, buffer_size);

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//     Description : Destroy STK Str Memory
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroySTKStrMemory(void** stk_buffer_ptr)
{
    if (PNULL != *stk_buffer_ptr)
    {
        SCI_FREE(*stk_buffer_ptr);
        *stk_buffer_ptr = PNULL;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Select SIM API of STK app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISTK_SelectSIMFunc(void)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T	selectSimWinID = MMISTK_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)MMISET_APPEND_STK_CARD;
    caller_data.callback_func = STKSimSelectCallback ;
    caller_data.select_type = MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMISTK_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = PNULL;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//     Description : Create icon display anim control
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateIconAnimCtrl(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUIIMG_BITMAP_T* icon_bitmap, GUI_BOTH_RECT_T *both_rect_ptr)
{
    MMI_CONTROL_CREATE_T    create = {0};
    GUIANIM_INIT_DATA_T     anim_init_data = {0};
    void          *anim_ctrl_ptr = PNULL;
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    
    if (icon_bitmap == NULL || icon_bitmap->bit_ptr == NULL)
    {
        //SCI_TRACE_LOW:"CreateIconAnimCtrl fail! icon is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11681_112_2_18_2_56_17_335,(uint8*)"");
        return FALSE;
    }

    MMK_DestroyControl(ctrl_id);

    anim_init_data.is_form_bg = FALSE;
    anim_init_data.both_rect = *both_rect_ptr;
//    anim_init_data.both_rect.v_rect.left = both_rect_ptr->v_rect.left;
//    anim_init_data.both_rect.v_rect.right = anim_init_data.both_rect.v_rect.left + icon_bitmap->img_width;
//    anim_init_data.both_rect.v_rect.top = both_rect_ptr->v_rect.top;
//    anim_init_data.both_rect.v_rect.bottom = anim_init_data.both_rect.v_rect.top + icon_bitmap->img_height;
//    anim_init_data.both_rect.h_rect.left = both_rect_ptr->h_rect.left;
//    anim_init_data.both_rect.h_rect.right = anim_init_data.both_rect.h_rect.left + icon_bitmap->img_width;
//    anim_init_data.both_rect.h_rect.top = both_rect_ptr->h_rect.top;
//    anim_init_data.both_rect.h_rect.bottom = anim_init_data.both_rect.h_rect.top + icon_bitmap->img_height;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_id; //use win handle here
    create.init_data_ptr = &anim_init_data;
    anim_ctrl_ptr = (void*)MMK_CreateControl( &create );

    //set anim param
    ctrl_info.is_ctrl_id = FALSE;
    ctrl_info.ctrl_ptr   = anim_ctrl_ptr;

    //set display info
    display_info.bg.bg_type        = GUI_BG_COLOR;
    display_info.bg.color          = mmi_stk_display.bkgd_color;
    display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_disp_one_frame = TRUE;

    data_info.is_bitmap = TRUE;
    data_info.bitmap_data.bit_ptr    = icon_bitmap->bit_ptr;
    data_info.bitmap_data.img_height = icon_bitmap->img_height;
    data_info.bitmap_data.img_width  = icon_bitmap->img_width;

    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);

    return TRUE;
}

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
/*****************************************************************************/
//     Description : decode sim image (L4) to bitmap data(for anim control)
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeSimImgToBmp(MN_SIM_IMG_T* sim_img, GUIIMG_BITMAP_T *icon_bitmap)
{
    BOOLEAN result = FALSE;

    //SCI_TRACE_LOW:"DecodeSimImgToBmp img_width =%d, img_height = %d, cs = %d, len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11733_112_2_18_2_56_17_336,(uint8*)"dddd", sim_img->img_width,sim_img->img_height,sim_img->img_cs, sim_img->img_len);

    switch(sim_img->img_cs)
    {
    case MN_SIM_IMG_CODE_TYPE_BASIC:
#ifdef WIN32
        result = DecodeBasicImg(&s_sim_img_basic, icon_bitmap);
#else
        result = DecodeBasicImg(sim_img, icon_bitmap);
#endif
    	break;
    case MN_SIM_IMG_CODE_TYPE_COLOR:
#ifdef WIN32
        result = DecodeColorImg(&s_sim_img_color, icon_bitmap);
#else
        result = DecodeColorImg(sim_img, icon_bitmap);
#endif
        break;
    default:
#ifdef WIN32
        result = DecodeColorImg((MN_SIM_IMG_T *)&s_sim_img_color, icon_bitmap);
#else
        //SCI_TRACE_LOW:"DecodeSimImgToBmp MN_SIM_IMG_CODE_TYPE_E is ERROR!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11755_112_2_18_2_56_17_337,(uint8*)"");
#endif
        break;
    }
    return result;
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeBasicImg(MN_SIM_IMG_T* sim_img, GUIIMG_BITMAP_T *icon_bitmap)
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 *bmp_buf = NULL;
    uint16 sim_img_data_size = sim_img->img_len;
    uint16 real_size = sim_img->img_width * sim_img->img_height;
    
    if (sim_img == NULL || icon_bitmap == NULL)
    {
        //SCI_TRACE_LOW:"DecodeBasicImg fail! src data = NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11776_112_2_18_2_56_17_338,(uint8*)"");
        return FALSE;
    }

    if ((((uint32)real_size+7) >> 3) > sim_img_data_size)
    {
        //SCI_TRACE_LOW:"DecodeBasicImg fail! icon data error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11782_112_2_18_2_56_17_339,(uint8*)"");
        return FALSE;
    }

    bmp_buf = (uint16*)SCI_ALLOCAZ(real_size * 2);
    if (bmp_buf == NULL)
    {
        //SCI_TRACE_LOW:"DecodeBasicImg alloc fail,no mem!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11789_112_2_18_2_56_17_340,(uint8*)"");
        return FALSE;
    }
    for (j=0; j<sim_img_data_size; j++)
    {
        uint16 offset = (j<<3);
        if (offset >= real_size)
        {
            break;
        }
        for (i=0; i<8; i++)
        {
            uint16 mask = (0x01<<(7-i));
            if (offset+i >= real_size)
            {
                break;
            }
            if ((sim_img->img_body[j] & mask) == 0)
            {
                bmp_buf[offset + i] = 0xFFFF;
            }
            else
            {
                bmp_buf[offset + i] = 0x0000;
            }
        }
    }
    icon_bitmap->img_height = sim_img->img_height;
    icon_bitmap->img_width  = sim_img->img_width;
    
    if (icon_bitmap->bit_ptr != NULL)
    {
        SCI_FREE(icon_bitmap->bit_ptr);
        icon_bitmap->bit_ptr = NULL;
    }
    icon_bitmap->bit_ptr    = bmp_buf;

    return TRUE;
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:andrew.ma
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeColorImg(MN_SIM_IMG_T* sim_img, GUIIMG_BITMAP_T *icon_bitmap)
{
    uint16 j = 0;
    uint16 *bmp_buf   = NULL;
    uint16 img_width  = (uint16)(sim_img->img_width);
    uint16 img_height = (uint16)(sim_img->img_height);
    uint16 sim_img_data_size = img_width * img_height * sim_img->bits_per_img_point;
    uint16 bmp_buf_size = img_width * img_height;
    uint8 *clut = (sim_img->img_body + sim_img->location_clut);
    uint16 clut_len = sim_img->img_len - (sim_img->location_clut);

    if (sim_img == NULL || icon_bitmap == NULL)
    {
        //SCI_TRACE_LOW:"DecodeColorImg fail! src data = NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11846_112_2_18_2_56_17_341,(uint8*)"");
        return FALSE;
    }

    if (((((uint32)(sim_img_data_size)+7) >> 3) > ((uint32)(sim_img->location_clut))) 
        || ((((uint16)sim_img->num_of_clut_entry) * 3) > clut_len))
    {
        //SCI_TRACE_LOW:"DecodeColorImg fail! icon data error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11853_112_2_18_2_56_17_342,(uint8*)"");
        return FALSE;
    }

    bmp_buf = (uint16*)SCI_ALLOCAZ(bmp_buf_size * 2);
    if (bmp_buf == PNULL)
    {
        //SCI_TRACE_LOW:"DecodeColorImg alloc fail,no mem!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11860_112_2_18_2_56_17_343,(uint8*)"");
        return FALSE;
    }
    for (j=0; j< sim_img_data_size; j+=sim_img->bits_per_img_point)
    {
        uint16 m = j >> 3;
        uint16 m1 = (j+sim_img->bits_per_img_point) >> 3;
        uint16 n = j - (m << 3);
        uint16 n1 = 0;
        uint8 data = sim_img->img_body[m];
        uint8 data1 = 0;
        uint8 r, g, b; 
        uint16 offset = j/sim_img->bits_per_img_point;
        if (offset >= bmp_buf_size)
        {
            break;
        }
        if (m1 > m)
        {
            data1 = sim_img->img_body[m1];
            n1 = n + sim_img->bits_per_img_point - 8;
            data1 = data1 >> (8-n1);
        }
        data = data << n;
        data = data >> (8-sim_img->bits_per_img_point);
        data += data1;
        data = data * 3;
        if(data+3 > clut_len)
        {
            //SCI_TRACE_LOW:"DecodeColorImg data error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_WIN_11889_112_2_18_2_56_18_344,(uint8*)"");
            SCI_FREE(bmp_buf);
            return FALSE;
        }
        r = clut[data];
        g = clut[data+1];
        b = clut[data+2];
        bmp_buf[offset] = RGB565(r, g, b);
    }
    icon_bitmap->img_height = sim_img->img_height;
    icon_bitmap->img_width  = sim_img->img_width;
    
    if (icon_bitmap->bit_ptr != NULL)
    {
        SCI_FREE(icon_bitmap->bit_ptr);
        icon_bitmap->bit_ptr = NULL;
    }
    icon_bitmap->bit_ptr    = bmp_buf;

    return TRUE;
}
#endif

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetRichtextItemInfo(MN_DUAL_SYS_E dual_sys,
                                                                BOOLEAN is_icon_id,
                                                                SIMAT_DO_ICON_ID_T icon_id,
                                                                MMI_STRING_T display_data_string,
                                                                MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;

    GUIRICHTEXT_DeleteAllItems(ctrl_id);

    if (is_icon_id)
    {
#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
#ifndef PRODUCT_DM        
		//get icon data
        MN_SIM_IMG_T sim_img = {0};
        MNSIM_GetImgEx(dual_sys, icon_id.icon_id, &sim_img);

        if (DecodeSimImgToBmp(&sim_img, &s_icon_bitmap))
        {
            s_mmistk_icon_display_result = TRUE;

            item_data.text_type = GUIRICHTEXT_TEXT_NONE;
            item_data.img_type = GUIRICHTEXT_IMAGE_BUF;
            item_data.is_bit_map = TRUE;
            item_data.img_data.buf.buf_ptr = (uint8 *)s_icon_bitmap.bit_ptr;
            item_data.img_data.buf.size = s_icon_bitmap.img_width * s_icon_bitmap.img_height * 2;
            item_data.img_data.buf.width = s_icon_bitmap.img_width;
            item_data.img_data.buf.height = s_icon_bitmap.img_height;
            item_data.img_data.buf.height = s_icon_bitmap.img_height;
            item_data.img_set_size = TRUE;
            item_data.img_setting_width = s_icon_bitmap.img_width;
            item_data.img_setting_height = s_icon_bitmap.img_height;
            item_data.img_set_align = TRUE;
            item_data.img_setting_align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;

            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                &index
                );
        }
        else
#endif
#endif
        {
            s_mmistk_icon_display_result = FALSE;
        }

    }

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    if (!is_icon_id || !icon_id.is_self_explanatory)
#endif
    {
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_data.buf.str_ptr = display_data_string.wstr_ptr;
        item_data.text_data.buf.len = display_data_string.wstr_len;

        GUIRICHTEXT_AddItem(
            ctrl_id,
            &item_data,
            &index
            );
    }

    return TRUE;
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC GUIIMG_BITMAP_T *MMIAPISTK_GetIdleTextIcon(BOOLEAN *is_icon_exist, BOOLEAN *is_self_explanatory)
{
    if (PNULL == is_icon_exist || PNULL == is_self_explanatory)
    {
        return PNULL;
    }

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    *is_icon_exist = s_mmistk_idle_text_is_icon_id;
    *is_self_explanatory = s_mmistk_idle_text_icon_id.is_self_explanatory;
#else
    *is_icon_exist = FALSE;
    *is_self_explanatory = FALSE;
#endif

#ifdef MMI_STK_ICON_DISPLAY_SUPPORT
    return &s_idle_icon_bmp;
#else
    return PNULL;
#endif
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
LOCAL void SetStkWinTitle(MN_DUAL_SYS_E dual_sys, MMI_WIN_ID_T win_id)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (PNULL != s_mmistk_toplevel_menu_info[dual_sys]
        && 0 != s_mmistk_toplevel_menu_info[dual_sys]->menu_title_len)
    {
        GUIWIN_SetTitleText(win_id, 
                        s_mmistk_toplevel_menu_info[dual_sys]->menu_title, 
                        s_mmistk_toplevel_menu_info[dual_sys]->menu_title_len,
                        FALSE);
    }
    else
    {
        if (PNULL != s_title_from_selected_item_context
            && 0 != s_title_from_selected_item_context->length)
        {
            GUIWIN_SetTitleText(win_id, 
                            s_title_from_selected_item_context->stringData,
                            s_title_from_selected_item_context->length,
                            FALSE);
        }
        else if (PNULL != s_mmistk_cur_menu_info_ptr[dual_sys])
        {
            GUIWIN_SetTitleText(win_id, 
                            s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title, 
                            s_mmistk_cur_menu_info_ptr[dual_sys]->menu_title_len,
                            FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : get type of STK refresh
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC SIMAT_REFRESH_TYPE_E MMISTK_GetStkRefreshType(MN_DUAL_SYS_E dual_sys)
{
    return s_mmistk_refresh_type[dual_sys];
}

/*****************************************************************************/
//  Description : set type of STK refresh is 0
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMISTK_SetStkRefreshType(MN_DUAL_SYS_E dual_sys, int val_type)
{
    s_mmistk_refresh_type[dual_sys] = val_type;
}

/*****************************************************************************/
//  Description : close refresh waiting win
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMISTK_CloseRefreshWaitingWin(void)
{
    MMK_CloseWin(MMISTK_REFRESH_WAITING_WIN_ID);
}
// TODO: need  add ss feature   
PUBLIC void MMIAPISTK_DisplayResultByAlphaID(MN_DUAL_SYS_E dual_sys, BOOLEAN is_alpha_id, SIMAT_DO_ALPHA_ID_T *alpha_id)
{
    if ((is_alpha_id && alpha_id->len > 0) || !is_alpha_id)
    {
        if (is_alpha_id)
        {
            CreateSTKBufferMemory(&s_mmistk_displaybuf, (MMISTK_STRING_MAX_LEN + 1)*sizeof(wchar));
            s_mmistk_displaydata.wstr_ptr = (wchar*)s_mmistk_displaybuf;
            AlphaId2MMIStr(alpha_id, &s_mmistk_displaydata, MMISTK_STRING_MAX_LEN);
        }
        else 
        {
            MMI_GetLabelTextByLang(TXT_SENDING, &s_mmistk_displaydata);
        }

        /////////////////////////////
        // Create Window & TextBox
        /////////////////////////////
        CreateSendSSVision(dual_sys);
    }
    
    s_mmistk_sendss_waiting_respond = TRUE;

    return;
}
/*Edit by script, ignore 12 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
