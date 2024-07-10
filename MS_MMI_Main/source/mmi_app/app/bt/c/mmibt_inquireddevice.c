/****************************************************************************
** File Name:     mmibt_inquireddevice.c                                        *
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
#define _MMIBT_INQUIREDDEVICE_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
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
#include "mmibt_nv.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_inquireddevice.h"
#include "mmi_autotest.h"
#include "mmicc_export.h"
#include "guictrl_api.h"
#include "mmikl_export.h"
#include "mmibt_app.h"
#include "mmibt_headset.h"
#include "mmi_appmsg.h"
#include "watch_commonwin_export.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define  MMIBT_PIN_MIN_LENGTH               1                   //pair pin code min wstr_len
#define  MMIBT_PIN_MAX_LENGTH               16                  //pair pin code max wstr_len
#define  MMIBT_PAIR_TEXT_LEN                (51+BT_DEVICE_NAME_SIZE+7)//TXT+DEVICE_NAME+SSP_KEY
// 用于判断蓝牙密码输入的位数是否正确。 
#define PAIR_CODE_LEN_IS_IN_RANGE(_LEN)            \
    ((MMIBT_PIN_MIN_LENGTH <= (_LEN))    \
    && (MMIBT_PIN_MAX_LENGTH >= (_LEN))) \

#ifdef MMI_PDA_SUPPORT
typedef enum
{
    BT_OPTMENU_NODE_ROOT,           //root/*lint !e749*/
    BT_OPTMENU_NODE_AUTHORITY_RW,          
    BT_OPTMENU_NODE_AUTHORITY_READ                  
}MMIPB_OPTMENU_NODE_E;
#endif
#ifdef TOUCH_PANEL_SUPPORT
typedef enum
{
    BT_INQUIRY_OPTMENU_NODE_ROOT,           //root/*lint !e749*/
    MMIBT_INQUIRY_STOP_ID,
    MMIBT_INQUIRY_PAIRY_ID                
}MMIPB_INQUIRY_OPTMENU_NODE_E;
#endif

#define BT_HEADSET_OPENWAIT_TIMER    (2*1000)  //被动配对时等待主动连接的timer  
LOCAL uint8 s_headset_openwait_timer = 0;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static  BT_DEVICE_INFO   *s_inquired_device_ptr = PNULL;      //the info of inquired device
static  uint8            s_inquired_device_num = 0;           // the number of inquired device
static  BOOLEAN           s_bt_is_self_pair = FALSE;
static  BT_DEVICE_INFO      s_pin_request_info = {0};         //the address info of request input pin
static  BT_DEVICE_INFO_EXT  s_pin_request_info_ext = {0};     //the address info of ssp
static  BOOLEAN             s_is_pin_cancel = FALSE;          //TRUE: user refuse pair. FALSE: user accept pair request  
extern  MMIBT_OPERTATION_TYPE_E g_interrupt_opertor;
extern  BT_DEVICE_INFO      g_current_bt;
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsSelfPair(void);
/*****************************************************************************/
//  Discription:  check is searched device already be searched
//  Global resource dependence: None
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN IsSearchingDeviceExist(BT_ADDRESS *bt_addr_ptr);
/*****************************************************************************/
//  Discription: Handle blue tooth  pin input win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePinInputWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param 
                                          );
/*****************************************************************************/
//  Discription: Handle blue tooth  Inquired device list win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiredDevListWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param 
                                                 );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiryOptMenuWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param 
                                                 );

/*****************************************************************************/
//  Discription: Handle blue tooth  numeric passkey check  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleNumericPasskeyWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param 
                                          );

/*****************************************************************************/
//  Discription: Handle searching new device waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
#if 0
PUBLIC MMI_RESULT_E  HandleCanbeCancelWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    );
#endif
#ifdef MMI_PDA_SUPPORT
LOCAL void CreateSBandPopmenu(void);

/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id);
#endif
/*****************************************************************************/
//  Discription: Open the blue tooth Inquired device list win   
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL void  OpenInquiredDevListWin(void);

/*****************************************************************************/
//  Description : display numeric passkey information
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void Display_NumericPasskeyInfo(void);


/*****************************************************************************/
//  Discription: insert menu node bu text id 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                     MMI_CTRL_ID_T      ctrl_id,        //控件id
                     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                     uint16          	node_id,        //节点编号
                     uint16          	parent_node_id, //父节点编号
                     uint16          	index,          //位置
                     BOOLEAN            is_graged
                     );

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_OpenWaitTimerOut(void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandsetGatewayOpenIsPermit(BT_ADDRESS * bt_addr);
#endif

/*---------------------------------------------------------------------------*/
//the list of inquiried device win table
WINDOW_TABLE(MMIBT_INQUIRED_DEV_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleInquiredDevListWinMsg ),
    WIN_TITLE(TXT_NULL),            
    WIN_ID(MMIBT_INQUIRED_DEV_LIST_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIBT_INQUIRED_DEV_LIST_CTRL_ID),
    CREATE_ANIM_CTRL(MMIBT_ANIMATE_SEARCHING_CTRL_ID, MMIBT_INQUIRED_DEV_LIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
#else    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_STOP),
#endif
    END_WIN
};
// the window for input pin,startup
WINDOW_TABLE( MMIBT_PIN_INPUT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePinInputWinMsg ),    
    WIN_ID( MMIBT_PIN_INPUT_WIN_ID ),
    WIN_TITLE( TXT_BT_ENTER_PIN ),
	//WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_EDIT_PASSWORD_CTRL(MMIBT_PIN_MAX_LENGTH,MMIBT_PIN_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input pin,startup
WINDOW_TABLE( MMIBT_NUMERIC_PASSKEY_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleNumericPasskeyWinMsg ),    
    WIN_ID( MMIBT_NUMERIC_PASSKEY_WIN_ID ),
    WIN_TITLE( TXT_BT_PAIR_REQUEST ),
    CREATE_TEXT_CTRL(MMIBT_NUMERIC_PASSKEY_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIBT_INQUIRY_OPT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleInquiryOptMenuWinMsg),
    WIN_ID(MMIBT_INQUIRY_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    END_WIN
};
#endif

/*****************************************************************************/
//  Discription: Handle blue tooth  Inquired device list win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiredDevListWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param 
                                                 )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint16                  cur_select_item  = 0;
    uint16                  i = 0;
    uint16                  total_num = 0;
	uint16                  anim_width = 0;
	uint16                  anim_height = 0;
	uint16                  frame_num = 0;
    BT_STATUS               return_value = BT_PENDING;
    MMI_STRING_T            wait_text = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info =  {0};    
	GUI_BOTH_RECT_T         anim_both_rect = MMITHEME_GetWinTitleBothRect(win_id);
#ifdef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T         list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
    uint8                   addr_str[BT_ADDRESS_SIZE*4] = {0};
	uint16                  index = 0;
	uint32                  addr_str_len = 0; 
    MMI_STRING_T    query_text = {0};
	MMI_STRING_T    name_text = {0};
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
			GUIRES_GetAnimWidthHeight(&anim_width, &anim_height, &frame_num, AINM_BT_SEARCHING, win_id);

            if(MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
            {
    			anim_both_rect.h_rect.left = anim_both_rect.h_rect.right - anim_width;
    			anim_both_rect.v_rect.left = anim_both_rect.v_rect.right - anim_width;
#ifndef MMI_PDA_SUPPORT
    			anim_both_rect.h_rect.top = (anim_both_rect.h_rect.bottom - anim_height)/2;
    			anim_both_rect.v_rect.top = (anim_both_rect.v_rect.bottom - anim_height)/2;
#else
    			anim_both_rect.h_rect.top   += (anim_both_rect.h_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
    			anim_both_rect.v_rect.top   += (anim_both_rect.v_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
    			anim_both_rect.h_rect.bottom = anim_both_rect.h_rect.top + anim_height - 1;
    			anim_both_rect.v_rect.bottom = anim_both_rect.v_rect.top + anim_height - 1;
#endif
            }
            else
            {
                anim_both_rect.h_rect.left = 0;
    			anim_both_rect.v_rect.left = 0;
                anim_both_rect.h_rect.right = anim_width -1;
                anim_both_rect.v_rect.right = anim_width -1;
#ifndef MMI_PDA_SUPPORT
    			anim_both_rect.h_rect.top = (anim_both_rect.h_rect.bottom - anim_height)/2;
    			anim_both_rect.v_rect.top = (anim_both_rect.v_rect.bottom - anim_height)/2;
#else
    			anim_both_rect.h_rect.top   += (anim_both_rect.h_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
    			anim_both_rect.v_rect.top   += (anim_both_rect.v_rect.bottom - anim_both_rect.h_rect.top - anim_height)/2;
    			anim_both_rect.h_rect.bottom = anim_both_rect.h_rect.top + anim_height - 1;
    			anim_both_rect.v_rect.bottom = anim_both_rect.v_rect.top + anim_height - 1;
#endif
            }
            
			GUIAPICTRL_SetBothRect(MMIBT_ANIMATE_SEARCHING_CTRL_ID, &anim_both_rect);
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())
            {
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_STOP, FALSE);
                GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_LEFT, MSG_CTL_PENOK, MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID);
                MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, FALSE);
            }
            else
#endif				
            {
                MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, TRUE);
                MMIBT_CreateNewSoftKey(win_id, TXT_NULL, STXT_STOP, TXT_NULL);
            }

            list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
                GUIAPICTRL_SetBothRect(MMIBT_INQUIRED_DEV_LIST_CTRL_ID, &list_both_rect);
#endif     
            MMIDEFAULT_AllowOpenKeylocWin(FALSE);  
            lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
            lcd_dev_info.block_id = GUI_BLOCK_MAIN;
            //AnimatePlayCtrlByState(MMIBT_ANIM_INIT);
            GUIWIN_SetTitleTextId(win_id, TXT_COMMON_SEARCHING, FALSE);
            
            GUIWIN_SetTitleBackground(win_id, MMITHEME_GetWinTitleBar(), 0); 
    		GUIWIN_SetTitleFontColor(win_id, MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0));
            MMK_SetAtvCtrl( win_id, MMIBT_INQUIRED_DEV_LIST_CTRL_ID );
            if(s_inquired_device_ptr != PNULL)
            {
                sprintf((char*)addr_str, "%x", s_inquired_device_ptr[0].addr.addr[0]);
                for(i = 1; i < BT_ADDRESS_SIZE; i++)
                {
                    addr_str_len = strlen((char*)addr_str);
                    sprintf((char*)&addr_str[addr_str_len], " %x", s_inquired_device_ptr[0].addr.addr[i]);
                }
                MMI_Char_AutoTest_Trace((char*)MMI_BT_TEST_CHAR, strlen(MMI_BT_TEST_CHAR), (char*)addr_str, strlen((char*)addr_str));
            }            
        }
        break; 
    case MSG_GET_FOCUS:
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
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
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:                
                 MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                break;           
            default:
                if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
                {
                    break;
                }                
                if(GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID)>0)
                {
                    CreateSBandPopmenu();
                }
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_MENU:
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            break;
        }
        if(GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID)>0)
        {
            CreateSBandPopmenu();
        }
        break;
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            //only can pair when stop search
#ifdef BT_DIALER2_SUPPORT
            // Pair while searching: stop search
            return_value = BT_CancelSearch();
            if(BT_PENDING == return_value)
            {
                //canceling
                OpenCancelWaitingWin();
            }
            else
            {
                MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_CANCELED, PNULL);
            }
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
#else
            break;
#endif
        }        

        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_PAIR);
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID); 
        SCI_MEMSET(&s_pin_request_info, 0, sizeof(BT_DEVICE_INFO));        
        if(s_inquired_device_ptr != PNULL)
        {
            SCI_MEMCPY(&s_pin_request_info, &s_inquired_device_ptr[cur_select_item],sizeof(BT_DEVICE_INFO));
            MMIBT_SetCurBtInfo(&s_inquired_device_ptr[cur_select_item]);
        }
		BtPauseBGPlay(MMIBT_PAIR_SUSPEND);
        return_value = BT_PairDevice(&s_pin_request_info.addr);
        if (BT_PENDING != return_value)
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
			BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        }
        else
        {
            SetSelfPair(TRUE);
            SetIsPinCancel(FALSE);
            //pause by pair
            //need resume search suspend 
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PAIR_REQUEST_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);  
            MMIBT_StartOperationWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
         }
        break;
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_PREPAGE:
    case MSG_CTL_LIST_NXTPAGE:
		index = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
		if(index < MMIBT_MAX_DEV_ITEMS && s_inquired_device_ptr != PNULL)
		{			
			SCI_MEMSET(addr_str, 0x00, sizeof(addr_str));
			sprintf((char*)addr_str, "%x", s_inquired_device_ptr[index].addr.addr[0]);
			for(i = 1; i < BT_ADDRESS_SIZE; i++)
			{
				addr_str_len = strlen((char*)addr_str);
				sprintf((char*)&addr_str[addr_str_len], " %x", s_inquired_device_ptr[index].addr.addr[i]);
			}
			MMI_Char_AutoTest_Trace((char*)MMI_BT_TEST_CHAR, strlen(MMI_BT_TEST_CHAR), (char*)addr_str, strlen((char*)addr_str));		
		}
		result = MMI_RESULT_FALSE;
		break;
    case MSG_BT_SEARCH_FINISHED:
    case MSG_BT_SEARCH_CANCELED:
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
#ifdef MMI_PDA_SUPPORT
        if(MMK_IsOpenWin(MMIBT_INQUIRY_OPT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_INQUIRY_OPT_WIN_ID);
        }
#endif
        GUIWIN_SetTitleTextId(win_id, TXT_BT_INQUIRED_DEV, FALSE);
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
        }
        else
#endif			
        {
            MMIBT_CreateNewSoftKey(win_id, TXT_NULL, STXT_CANCEL, TXT_NULL);
        }
#endif
        GUIWIN_SetTitleBackground(win_id, MMITHEME_GetWinTitleBar(), 0);

#ifndef MMI_BLUETOOTH_MINI_SUPPORT
        AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
#else
        GUIANIM_SetVisible(MMIBT_ANIMATE_SEARCHING_CTRL_ID, FALSE, TRUE);
#endif
        total_num = GUILIST_GetTotalItemNum(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
        if(total_num > 0)
        {
            GUILIST_RemoveAllItems(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
            for(i = 0; s_inquired_device_ptr != PNULL && i < s_inquired_device_num; i++)
            {
                MMIBT_AppendDeviceListItem(
                    &s_inquired_device_ptr[i],
                    GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
                    STXT_PAIR,
                    TXT_NULL,
                    STXT_RETURN,
                    MMIBT_INQUIRED_DEV_LIST_CTRL_ID
                    );
            }
            if(cur_select_item < s_inquired_device_num)
            {
                GUILIST_SetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID, cur_select_item);
            }
        }
        
        if(MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion)
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
            //stop search
            return_value = BT_CancelSearch();
            if(BT_PENDING == return_value)
            {
                //canceling
                OpenCancelWaitingWin();
            }
            else
            {
                MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID , MSG_BT_SEARCH_CANCELED, PNULL);
            }
        }
        else
        {
            //exit
            MMK_CloseWin(win_id);
        }
        BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);   
        break;
        
    case MSG_CLOSE_WINDOW:
		BT_CancelSearch();
        BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
        if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
        {
            //run next step
            MMIBT_CreateUitestWin();
        }   
        if(s_inquired_device_ptr != PNULL)
        {
            SCI_FREE(s_inquired_device_ptr);
            s_inquired_device_ptr = PNULL;
        }
#if defined(BLUETOOTH_SUPPORT_SPRD_BT) && !defined(WIN32)
        //BT_FreeEirBuffer();
        //SCI_TRACE_LOW("BT_FreeEirBuffer called!");
#endif
        break;
    case MSG_KEYDOWN_RED:
       if(MMIBT_OPC_AUTO_TEST != operation_info.root_opertion)
       {
           result=MMI_RESULT_FALSE;
       }
	break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Discription: Handle Inquired device list option menu msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleInquiryOptMenuWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param 
                                                 )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint16              node_id       = 0;
    BT_STATUS           return_value = BT_PENDING;
    uint16              cur_select_item = 0;
    MMI_STRING_T            wait_text = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:

       node_id = GUIMENU_GetCurNodeId(MMIBT_LIST_INQUIRY_MENU_CTRL_ID);  
      if(node_id == MMIBT_INQUIRY_STOP_ID)
      {
            return_value = BT_CancelSearch();
            if(BT_PENDING == return_value)
            {
                //canceling
                OpenCancelWaitingWin();
            }
            else
            {
                MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID , MSG_BT_SEARCH_CANCELED, PNULL);
            }          
      }
      else if(node_id == MMIBT_INQUIRY_PAIRY_ID)
      {
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_PAIR);
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID); 
        SCI_MEMSET(&s_pin_request_info, 0, sizeof(BT_DEVICE_INFO));
        if(s_inquired_device_ptr != PNULL)
        {
            SCI_MEMCPY(&s_pin_request_info, &s_inquired_device_ptr[cur_select_item],sizeof(BT_DEVICE_INFO));
            MMIBT_SetCurBtInfo(&s_inquired_device_ptr[cur_select_item]);
        }
		BtPauseBGPlay(MMIBT_PAIR_SUSPEND);
        return_value = BT_PairDevice(&s_pin_request_info.addr);
        if (BT_PENDING != return_value)
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
			BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        }
        else
        {
            SetSelfPair(TRUE);
            SetIsPinCancel(FALSE);
            //pause by pair
            //need resume search suspend 
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PAIR_REQUEST_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
            MMIBT_StartOperationWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
         }          
      }
        
        MMK_CloseWin(win_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Discription: Handle blue tooth  pin input win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePinInputWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param 
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        pin_value= {0};   
    uint8               pin_str[MMIBT_PIN_MAX_LENGTH + 2 ] ={0};
    MMI_STRING_T        wait_text = {0};
    BT_STATUS           return_value = BT_PENDING;
    GUI_BOTH_RECT_T     edit_both_rect = {0};
    MMIBT_PIN_INPUT_PARAM_T *win_param_ptr = (MMIBT_PIN_INPUT_PARAM_T *)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
        GUIAPICTRL_SetBothRect(MMIBT_PIN_EDITBOX_CTRL_ID, &edit_both_rect);

		//set left softkey
		GUIEDIT_SetSoftkey(MMIBT_PIN_EDITBOX_CTRL_ID,0,MMIBT_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK,PNULL);

        MMK_SetAtvCtrl(win_id,MMIBT_PIN_EDITBOX_CTRL_ID);   
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id,IMAGE_NUMBERINPUT_PIN);
#endif
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
                GUIEDIT_GetString(MMIBT_PIN_EDITBOX_CTRL_ID,&pin_value);
        
                if (PAIR_CODE_LEN_IS_IN_RANGE(pin_value.wstr_len))  
                {           
                    win_param_ptr->callbackfunc(win_id,pin_value.wstr_ptr,pin_value.wstr_len);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_LENGTH_ERROR);
                    GUIEDIT_ClearAllStr(MMIBT_PIN_EDITBOX_CTRL_ID);
                }
                break;
            case  MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                SetIsPinCancel(TRUE);
                BT_CancelPair(&win_param_ptr->pin_request_device_info.addr);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:   
        // get the value about PIN
        GUIEDIT_GetString(MMIBT_PIN_EDITBOX_CTRL_ID,&pin_value);
        
        if (PAIR_CODE_LEN_IS_IN_RANGE(pin_value.wstr_len))  
        {           
            win_param_ptr->callbackfunc(win_id,pin_value.wstr_ptr,pin_value.wstr_len);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_LENGTH_ERROR);
            GUIEDIT_ClearAllStr(MMIBT_PIN_EDITBOX_CTRL_ID);
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        SetIsPinCancel(TRUE);
        BT_CancelPair(&win_param_ptr->pin_request_device_info.addr);
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREEIF(win_param_ptr);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Discription: Handle blue tooth  numeric passkey check  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleNumericPasskeyWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param 
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        wait_text = {0};
    BT_STATUS           return_value = BT_PENDING;
    GUI_BOTH_RECT_T     both_rect = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        both_rect = MMITHEME_GetFullScreenBothRect();
        MMK_SetBothRect(win_id, &both_rect);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        Display_NumericPasskeyInfo();        
        break;
        
    case MSG_FULL_PAINT:
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                s_is_pin_cancel = FALSE;
                return_value = BT_InputPairPin(&s_pin_request_info_ext.addr,(uint8*)s_pin_request_info_ext.ssp_numric_key,strlen(s_pin_request_info_ext.ssp_numric_key));
                if (BT_PENDING == return_value)
                {
                    MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
                    MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
                      
                }
                else
		        {
                     MMK_CloseWin(win_id);
		        }
                break;
            case  MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                s_is_pin_cancel = TRUE;
                BT_CancelPair(&s_pin_request_info_ext.addr);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                break;
            default:
                break;
            }
        }
        break;
#else
    case MSG_CTL_PENOK:
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:   
        s_is_pin_cancel = FALSE;
        return_value = BT_InputPairPin(&s_pin_request_info_ext.addr,(uint8*)s_pin_request_info_ext.ssp_numric_key,strlen(s_pin_request_info_ext.ssp_numric_key));
        if (BT_PENDING == return_value)
        {
            MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
              
        }
        else      
		{
             MMK_CloseWin(win_id);
		}
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        s_is_pin_cancel = TRUE;
        BT_CancelPair(&s_pin_request_info_ext.addr);
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        break;

    case MSG_APP_RED:
        s_is_pin_cancel = TRUE;
        BT_CancelPair(&s_pin_request_info_ext.addr);
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
        result = MMI_RESULT_FALSE;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Discription: when paired device successful should modify inquired device info
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void ModifyInquiredDeviceInfo(uint16  delete_device_index)
{
    int32       i = 0;
    
    for (i = delete_device_index; s_inquired_device_ptr != PNULL && i < (s_inquired_device_num - 1); i ++)
    {
        SCI_MEMSET(&s_inquired_device_ptr[i], 0 , sizeof(BT_DEVICE_INFO));
        
        //get device name info from message body        
        MMIAPICOM_Wstrncpy(s_inquired_device_ptr[i].name ,
            s_inquired_device_ptr[i + 1].name,
            MMIAPICOM_Wstrlen((wchar*)s_inquired_device_ptr[i + 1].name));
        
        //get device addr info from message body
        SCI_MEMCPY(s_inquired_device_ptr[i].addr.addr,
            s_inquired_device_ptr[i + 1].addr.addr,
            BT_ADDRESS_SIZE);
        
        //get new inquired device class info 
        s_inquired_device_ptr[i].dev_class = s_inquired_device_ptr[i + 1].dev_class;  
    }
}

PUBLIC BT_DEVICE_INFO*  MMIBT_GetInqDevInfo(void)
{
    return s_inquired_device_ptr;
}
PUBLIC uint8  MMIBT_GetInqDevNum(void)
{
    return s_inquired_device_num;
}
/*****************************************************************************/
//  Discription: get inquired device name info 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetInqDevName(
                                BT_MSG_T *msg_body_ptr
                                )
{
    BT_DEVICE_INFO      dev_info   = *((BT_DEVICE_INFO*)msg_body_ptr->body_ptr);    
    MMI_CTRL_ID_T       ctrl_id = 0;
    MMI_WIN_ID_T        win_id = 0;
    char temp[256] = {0};
    
    //turn on the back light
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInqDevName in calling or alarming !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5939_112_2_18_2_8_12_266,(uint8*)"");
		MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
        BT_CancelSearch();
//lzk mark
#ifdef BT_DIALER_SUPPORT		
        MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
#endif
        return;
    }
    if(BT_SUCCESS != msg_body_ptr->status)
    {
        //SCI_TRACE_LOW:"MMIBT_GetInqDevName status %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5947_112_2_18_2_8_12_267,(uint8*)"d", msg_body_ptr->status);
        return;
    }
	if(IsSearchingDeviceExist(&(dev_info.addr)) || (s_inquired_device_num >= MMIBT_MAX_DEV_ITEMS))
	{
		//SCI_TRACE_LOW:"MMIBT_GetInqDevName alerady exist s_inquired_device_num %d !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5952_112_2_18_2_8_12_268,(uint8*)"d", msg_body_ptr->status);
		return;
	}
    MMIDEFAULT_SetBackLight(TRUE);
    if(s_inquired_device_ptr == PNULL)
    {
         s_inquired_device_ptr = (BT_DEVICE_INFO *)SCI_ALLOCA(sizeof(BT_DEVICE_INFO)*MMIBT_MAX_DEV_ITEMS);
         if(s_inquired_device_ptr != PNULL)
         {
             SCI_MEMSET(s_inquired_device_ptr, 0x00, sizeof(BT_DEVICE_INFO)*MMIBT_MAX_DEV_ITEMS);
         }
    }
    if(0 == s_inquired_device_num)
    {
        //need open search list window
        if(s_inquired_device_ptr != PNULL)
        {
            SCI_MEMSET(s_inquired_device_ptr, 0x00, (MMIBT_MAX_DEV_ITEMS *sizeof(BT_DEVICE_INFO)));  
        }
        OpenInquiredDevListWin();
		MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
    }
    
    //add inquired device info to list
    if (MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID) || MMK_IsOpenWin(MMIBT_MAIN_WIN_ID)
            && (MMIBT_OPC_SEARCH == MMIBT_GetOpertor().current_opertion))
    {
        ctrl_id = MMIBT_INQUIRED_DEV_LIST_CTRL_ID;
        win_id = MMIBT_INQUIRED_DEV_LIST_WIN_ID;
        //get device name info from message body 
        SCI_TRACE_LOW("MMIBT_GetInqDevName dev_info.name wstr_len = %d",MMIAPICOM_Wstrlen((wchar*)dev_info.name));
        MMIAPICOM_WstrToStr(dev_info.name, temp);
        SCI_TRACE_LOW("MMIBT_GetInqDevName dev_info.name is %s",temp);
        if(MMIAPICOM_Wstrlen((wchar*)dev_info.name) > BT_DEVICE_NAME_SIZE
            ||0 == MMIAPICOM_Wstrlen((wchar*)dev_info.name))
        {
            return;
        }
        if(s_inquired_device_ptr != PNULL)
        {
            MMIAPICOM_Wstrncpy(s_inquired_device_ptr[s_inquired_device_num].name, dev_info.name, MMIAPICOM_Wstrlen( dev_info.name));
            //get device addr info from message body
            SCI_MEMCPY( 
                s_inquired_device_ptr[s_inquired_device_num].addr.addr,
                dev_info.addr.addr,
                BT_ADDRESS_SIZE
                );
            //get new inquired device class info 
            s_inquired_device_ptr[s_inquired_device_num].dev_class = dev_info.dev_class;
            s_inquired_device_num ++;
            
        }
        #if 1
        MMIBT_AppendDeviceListItem(
            &dev_info,
            GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
            TXT_NULL,
            TXT_NULL,
            STXT_STOP,
            ctrl_id
            );
        #endif
        //BTMainAppendInqDevListItem(&dev_info,ctrl_id);
        if (MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            //only search list in focus need paint
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            GUILIST_ValidCurItemPos(ctrl_id);
        }
    }
	else
	{
		BT_CancelSearch();
		BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
	}

}


/*****************************************************************************/
//  Discription:  check is searched device already be searched
//  Global resource dependence: None
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN IsSearchingDeviceExist(BT_ADDRESS *bt_addr_ptr)
{
	BOOLEAN is_exist = FALSE;
	uint32  i = 0;
	
    if(PNULL == bt_addr_ptr || (0 == s_inquired_device_num) || s_inquired_device_ptr == PNULL)
	{
		return FALSE;
	}

	for(i = 0; i < s_inquired_device_num; i++)
	{
		if(!memcmp(bt_addr_ptr, &(s_inquired_device_ptr[i].addr), sizeof(BT_ADDRESS)))
		{
            is_exist = TRUE;
			break;
		}
	}

	return is_exist;

}
/*****************************************************************************/
//  Discription: get device pair complete confirm (success or failure)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairComCnf(BT_MSG_T *msg_body_ptr)
{
    uint16                  cur_pos = 0;
    uint16                  device_index = 0;
    uint16                  dev_type = 0;
    MMI_STRING_T            name_text = {0};
    MMI_WIN_ID_T            alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    uint32                  current_service = 0;
    BT_DEVICE_INFO * pbtinfo = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
    BT_DEVICE_INFO         current_bt_device = MMIBT_GetCurBtInfo();
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    //when searching, then pair a device, the search will stop and return this msg
    BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);


    //SCI_TRACE_LOW:"MMIBT_GetPairComCnf msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6084_112_2_18_2_8_12_269,(uint8*)"d",msg_body_ptr->status);
    if (BT_SUCCESS == msg_body_ptr->status)
    {   
        //pair device successful
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
		BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        if(IsSelfPair())
        {
            if (MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
            {
                MMIDEFAULT_SetBackLight(TRUE);
				if(MMIBT_OPC_MAIN != operation_info.root_opertion && (!memcmp(&s_pin_request_info.addr, &current_bt_device.addr, sizeof(BT_ADDRESS))))
				{
					SCI_Sleep(MMI_3SECONDS);
					DoOperation(operation_info.root_opertion, FALSE);
				}
                cur_pos = GUILIST_GetCurItemIndex( MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
                if(!memcmp(s_pin_request_info.addr.addr, s_inquired_device_ptr[cur_pos].addr.addr, sizeof(s_pin_request_info.addr.addr)))
                {
                    //配对成功的设备在当前的搜索列表中
                    //如果不是设置窗口或搜索到的设备只有一项，则关闭搜索设备列表窗口
                    if (MMIBT_OPC_MAIN != operation_info.root_opertion || 1 == s_inquired_device_num)
                    {
                        MMK_CloseWin( MMIBT_INQUIRED_DEV_LIST_WIN_ID );
                    }
                    else
                    {
                        //将配对成功的设备从搜索到的设备列表中移除，并更新设备列表
                        GUILIST_RemoveItem( MMIBT_INQUIRED_DEV_LIST_CTRL_ID, cur_pos);
                        device_index = cur_pos;
                        ModifyInquiredDeviceInfo(device_index);
                        if(MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
                        {
                            MMK_PostMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID,MSG_FULL_PAINT,PNULL,PNULL);
                        }
                        s_inquired_device_num --;
                    }
                }
            }
        }
        if(MMIBT_OPC_MAIN == operation_info.root_opertion)
		{ 
            current_service = BT_GetPairedDeviceService(&pbtinfo->addr);
            if((BT_SERVICE_FTP|BT_SERVICE_OPP) & current_service)
            {
                MMI_STRING_T display_str = {0};
			
                MMI_GetLabelTextByLang(TXT_BT_AUTO_CONNECT_HINT, &display_str);
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT);
                OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_str, PNULL, MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT);  
            }
		}
        //if headset need open this headset 
        BT_GetDeviceType(&s_pin_request_info, &dev_type);
        if(dev_type & BT_DEVICE_CLASS_AUDIO)
        {
            //headset need open it directly
            if(HeadsetOpenIsPermit(&(s_pin_request_info.addr)))
            {
                 MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HEADSET);
                 OpenHeadset(&s_pin_request_info.addr);
            }
        }   
#ifdef BT_DIALER_SUPPORT
        else if(dev_type & BT_DEVICE_CLASS_PHONE)
        {
            //headset need open it directly
            //do not connect device when in send file process929676            
            if(HeadsetOpenIsPermit(&(s_pin_request_info.addr)) && (operation_info.root_opertion != MMIBT_OPC_OPP_SEND))
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HANDSET_GATEWAY);
                OpenHandsetGateway(&s_pin_request_info.addr);
                //DoOperation(MMIBT_OPC_OPEN_HEADSET, FALSE);
            }
        }
#endif
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    else
    {
        //close pair again query window
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        if (!IsPinCancel())
        {
            if (IsSelfPair())
            {
                MMI_STRING_T query_text = {0};

                MMIDEFAULT_SetBackLight(TRUE);
                MMI_GetLabelTextByLang(TXT_BT_PAIR_AGAIN, &query_text);
                //query try agian
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_PAIR);
                BtPauseBGPlay(MMIBT_PAIR_SUSPEND);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                OpenQueryWin(MMIBT_RETRY_WIN_ID, &query_text, PNULL, MMIBT_OPC_PAIR);
            }
            else
            {
                alert_id = MMI_BT_ALERT_WIN_ID;
                MMIDEFAULT_SetBackLight(TRUE);
                BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
                BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
				MMI_GetLabelTextByLang(TXT_BT_BOUNDING_FAILED, &name_text);
                MMIPUB_OpenAlertWinByTextPtr(PNULL, &name_text, PNULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }           
        }
        else
        {
            BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
            BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        }
        SetIsPinCancel(FALSE);
		if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
	MMK_CloseWin(MMIBT_PAIR_WAIT_WIN_ID);
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_CloseWin(MMIBT_NUMERIC_PASSKEY_WIN_ID);
}

/*****************************************************************************/
//  Discription: get earphone device pair complete confirm (success or failure)
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairComIND(BT_MSG_T *msg_body_ptr)
{
    MMI_STRING_T            name_text = {0};
    MMI_WIN_ID_T            alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
//lzk mark
#ifdef BT_DIALER_SUPPORT	
    BT_DEVICE_INFO*         current_bt_device = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
	uint16                  dev_type = 0;
#endif
    
    //when searching, then pair a device, the search will stop and return this msg
    BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
//lzk mark
#ifdef BT_DIALER_SUPPORT
    SCI_MEMSET(&g_current_bt, 0x00, sizeof(BT_DEVICE_INFO));	
	SCI_MEMCPY(&g_current_bt, current_bt_device, sizeof(BT_DEVICE_INFO));
#endif
    //SCI_TRACE_LOW:"MMIBT_GetPairComIND msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11013_112_2_18_2_8_22_353,(uint8*)"d",msg_body_ptr->status);
    if (BT_SUCCESS == msg_body_ptr->status)
    {   
        //pair device successful
        BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
		BtResumeBGPlay(MMIBT_PAIR_SUSPEND);

//lzk mark
#ifdef BT_DIALER_SUPPORT
        BT_GetDeviceType(current_bt_device, &dev_type);
        if(dev_type & BT_DEVICE_CLASS_AUDIO)
#endif
        {
            s_headset_openwait_timer = MMK_CreateTimerCallback(BT_HEADSET_OPENWAIT_TIMER, MMIBT_OpenWaitTimerOut, 0, FALSE);
            SCI_TRACE_LOW("[MMIBT] MMIBT_GetPairComIND, timer_id = %d", s_headset_openwait_timer);
        }
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }
        //close pair again query window
        if(!MMK_IsOpenWin( MMIBT_INCOMING_FILE_WIN_ID ))
        {
            //only when incoming win not opened,can prompt success
            //not local pair, need promt success
			MMI_GetLabelTextByLang(TXT_BT_FINISH_BOUNDING, &name_text);
			MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&name_text, PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);        
        }
#ifdef BT_DIALER_SUPPORT
        if(MMK_IsFocusWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            MMK_PostMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID,MSG_FULL_PAINT,PNULL,PNULL);
        }

        DoOperation(MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT, FALSE);
#endif
        //MMIPUB_CloseQuerytWin(PNULL); 
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    else
    {
        //close pair again query window
        MMK_CloseWin(MMI_BT_PAIR_QUERY_WIN_ID);
        if (!MMIBT_IsPinCancel())
        {
            alert_id = MMI_BT_ALERT_WIN_ID;
            MMIDEFAULT_SetBackLight(TRUE);
            BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
            BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
			MMI_GetLabelTextByLang(TXT_BT_BOUNDING_FAILED, &name_text);
            MMIPUB_OpenAlertWinByTextPtr(PNULL, &name_text, PNULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);      
        }
        else
        {
            BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
            BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
        }
        MMIBT_SetPinCancel(FALSE);
        //close input pin win
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
        //user do not refuse pair, reminder the result
    }
    MMIBT_StopOperationWin(MMIBT_OPC_PAIR); 
	MMK_CloseWin(MMIBT_PAIR_WAIT_WIN_ID);
	MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_CloseWin(MMIBT_NUMERIC_PASSKEY_WIN_ID);
}

/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void OpenSearchDeviceWaitWin(
                                   uint32 service_type
                                   )
{
    MMI_STRING_T            wait_text = {0};
    BT_STATUS               return_value = BT_SUCCESS;
    //uint8  paired_device_number = 0;
	//WATCH_SOFTKEY_TEXT_ID_T softkey_text = {0};

    return_value = BT_CancelSearch();
    MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING, &wait_text);
    s_inquired_device_num = 0;
    //paired_device_number =  BT_GetPairedDeviceCount(BT_SERVICE_ALL) ;
#if 1
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_SEARCH_DEVICE_PUBWIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);
#endif
    //WatchCOM_WaitingWin_1Line_Enter(MMIBT_SEARCH_DEVICE_PUBWIN_ID, wait_text, 0,softkey_text,PNULL);
    MMIBT_StartOperationWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID,MMIBT_OPC_SEARCH,CreatePubWinCallBack);
#if 0
    if( paired_device_number > 0)
    {
        OpenInquiredDevListWin();
        MMIBT_EnterDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
        MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
    }
#endif
   if(BT_PENDING != return_value)//already canceled, search directly
    {
        return_value = BT_SearchDevice(service_type);
        
        if(BT_PENDING != return_value)
        {
			MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
            MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
//lzk mark
#ifdef BT_DIALER_SUPPORT
            MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
#endif
        }
    }
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void SetSelfPair(BOOLEAN  is_self_pair)
{
    s_bt_is_self_pair = is_self_pair;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsSelfPair(void)
{
    return s_bt_is_self_pair ;
}
/*****************************************************************************/
//  Discription: get the input pin code request  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_GetInputPinReq(BT_MSG_T* msg_body_ptr)
{
    BT_DEVICE_INFO  device_info = {0};
    MMI_STRING_T    query_text = {0};
	MMI_STRING_T    name_text = {0};    
	MMI_WIN_ID_T    alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    
    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInputPinReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5454_112_2_18_2_8_11_258,(uint8*)"");
//lzk mark
#ifdef BT_DIALER_SUPPORT
        MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
#endif
        return ;
    }

	device_info = *(( BT_DEVICE_INFO *) msg_body_ptr->body_ptr);
#if 0
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        //calling, not deal with this request
		SetIsPinCancel(TRUE); 
		BT_CancelPair(&device_info.addr);
        return ;
    }
#endif
	if(MMK_IsOpenWin(MMI_BT_PAIR_QUERY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == MMIBT_GetInterruptOper()
	     ||MMIBT_OPC_PAIR == MMIBT_GetInterruptOper())
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in pairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5470_112_2_18_2_8_11_259,(uint8*)"");
		BT_CancelPair(&device_info.addr);
		return;
	}
	if(MMK_IsOpenWin(MMIBT_RETRY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == MMIBT_GetInterruptOper()
	     ||MMIBT_OPC_PAIR == MMIBT_GetInterruptOper())
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in repairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5479_112_2_18_2_8_11_260,(uint8*)"");
		BT_CancelPair(&device_info.addr);
		return;
	}
			
	if(MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID))
	{
		//cancelling, need not response this message
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in Cacelling !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5487_112_2_18_2_8_11_261,(uint8*)"");
		return;
	}

    MMIDEFAULT_SetBackLight(TRUE);
    //store the request pair device address
    SCI_MEMSET(&s_pin_request_info, 0, sizeof(BT_DEVICE_INFO));
    SCI_MEMCPY(&s_pin_request_info, &device_info,sizeof(BT_DEVICE_INFO));

    //有配对请求等待窗口说明是本机发起的配对
    if(MMK_IsOpenWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID))
    {
        MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INPUT_PIN);     
        BtPauseBGPlay(MMIBT_INPUTPIN_SUSPEND);
        BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
		if(BT_SUCCESS == msg_body_ptr->status)
		{
            MMIBT_CreatePinInputWin(&s_pin_request_info);
		}
		else
		{
			MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, PNULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);      	
		}            
    }
    else //对方请求配对
    {       
        MMIBT_SetSelfPair(FALSE);
        if(MMIBT_IsPhoneReady())
        {
			MMI_GetLabelTextByLang(TXT_ACCEPT_DEVICE_QUERY, &query_text);
			name_text.wstr_ptr = s_pin_request_info.name;
			name_text.wstr_len = MMIAPICOM_Wstrlen(s_pin_request_info.name);            
            OpenQueryWin(MMI_BT_PAIR_QUERY_WIN_ID, &name_text, &query_text, MMIBT_OPC_INPUT_PIN);
        }
        else
        {
            SetIsPinCancel(TRUE);
            BT_CancelPair(&s_pin_request_info.addr);
        }
    }    
}

/*****************************************************************************/
//  Discription: get the input pin code request  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_GetNumericPasskeyReq(BT_MSG_T* msg_body_ptr)
{
    BT_DEVICE_INFO_EXT device_info = {0};
    MMI_STRING_T    query_text = {0};
	MMI_STRING_T    name_text = {0};
    //wchar           text_info[MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH] = {0};
    MMI_WIN_ID_T    alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    uint8           bt_spp_numric_key[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_GetInputPinReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5454_112_2_18_2_8_11_258,(uint8*)"");
//lzk mark
#ifdef BT_DIALER_SUPPORT
        MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
#endif
        return ;
    }

	device_info = *(( BT_DEVICE_INFO_EXT *) msg_body_ptr->body_ptr);
#if 0
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        //calling, not deal with this request
		s_is_pin_cancel = TRUE; 
		BT_CancelPair(&device_info.addr);
        return ;
    }
#endif
	if(MMK_IsOpenWin(MMI_BT_PAIR_QUERY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == g_interrupt_opertor
	     ||MMIBT_OPC_PAIR == g_interrupt_opertor)
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in pairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5470_112_2_18_2_8_11_259,(uint8*)"");
		BT_CancelPair(&device_info.addr);
		return;
	}
	if(MMK_IsOpenWin(MMIBT_RETRY_WIN_ID) &&
		(MMIBT_OPC_INPUT_PIN == g_interrupt_opertor
	     ||MMIBT_OPC_PAIR == g_interrupt_opertor)
	 )
	{
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in repairing !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5479_112_2_18_2_8_11_260,(uint8*)"");
		BT_CancelPair(&device_info.addr);
		return;
	}
			
	if(MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID))
	{
		//cancelling, need not response this message
		//SCI_TRACE_LOW:"MMIBT_GetInputPinReq in Cacelling !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5487_112_2_18_2_8_11_261,(uint8*)"");
		return;
	}

    MMIDEFAULT_SetBackLight(TRUE);
    //store the request pair device address
    SCI_MEMSET(&s_pin_request_info_ext, 0, sizeof(BT_DEVICE_INFO_EXT));
    SCI_MEMCPY(&s_pin_request_info_ext, &device_info,sizeof(BT_DEVICE_INFO_EXT));
    
    //有配对请求等待窗口说明是本机发起的配对
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_NUMERIC_PASSKEY_CHECK);

    BtPauseBGPlay(MMIBT_INPUTPIN_SUSPEND);
    BtResumeBGPlay(MMIBT_PAIR_SUSPEND);

    //蓝牙耳机主动配对
    if(0 == memcmp(bt_spp_numric_key, s_pin_request_info_ext.ssp_numric_key, 7))
    {
        MMI_GetLabelTextByLang(TXT_ACCEPT_DEVICE_QUERY, &query_text);
		name_text.wstr_ptr = s_pin_request_info_ext.name;
		name_text.wstr_len = MMIAPICOM_Wstrlen(s_pin_request_info_ext.name);            
        OpenQueryWin(MMI_BT_PAIR_QUERY_WIN_ID, &name_text, &query_text, MMIBT_OPC_NUMERIC_PASSKEY_CHECK);

        return;
    }
    
	if(BT_SUCCESS == msg_body_ptr->status)
	{
		MMK_CreateWin((uint32 *)MMIBT_NUMERIC_PASSKEY_WIN_TAB, PNULL);        
	}
	else
	{
		MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, PNULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);      	
	}
    MMK_CloseWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID);
	
//lzk mark
#ifdef BT_DIALER_SUPPORT
    MMIBT_StopOperationWin(MMIBT_OPC_PAIR);
#endif
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC  void MMIBT_NumbericPasskeyCheck(void)
{
    MMI_STRING_T        wait_text = {0};
    BT_STATUS           return_value = BT_ERROR;
    
    return_value = BT_InputPairPin(&s_pin_request_info_ext.addr,(uint8*)s_pin_request_info_ext.ssp_numric_key,strlen(s_pin_request_info_ext.ssp_numric_key));
    if (BT_PENDING == return_value)
    {
       MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
       MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
           ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg);  
    }
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC  void MMIBT_CancelNumricPair(void)
{
    s_is_pin_cancel = TRUE;
    BT_CancelPair(&s_pin_request_info_ext.addr);
    BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC  BT_DEVICE_INFO GetPinRequestInfo(void)
{
    return  s_pin_request_info;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void SetIsPinCancel(BOOLEAN  is_pin_cancel)
{
    s_is_pin_cancel = is_pin_cancel;
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsPinCancel(void)
{
    return s_is_pin_cancel;
}
/*****************************************************************************/
//  Discription: Handle searching new device waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleCanbeCancelWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;   
    BT_STATUS       bt_status = BT_SUCCESS;
	static BOOLEAN  s_need_cancel = FALSE;
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		s_need_cancel = TRUE;
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
	case MSG_BT_AUTO_CLOSE_WIN:
        s_need_cancel = FALSE;
		break;
    case MSG_LOSE_FOCUS:
        if (MMIAPIKL_IsKLWinOnTop())
        {
            MMK_CloseWin( win_id );
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:     
    case MSG_APP_CANCEL:
		s_need_cancel = FALSE;
        //User cancel       
        bt_status = CancelOperation(operation_info.current_opertion);
        if((MSG_APP_RED == msg_id)&&!(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion))
        {
            //need back to idle
            recode = MMI_RESULT_FALSE;
        }
        else if(bt_status != BT_PENDING)
        {
            AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
            if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
            {
                //run next step
                MMIBT_CreateUitestWin();
            }
        }
        else
        {
            //BTMainCancelWaitingWin();
        }      
        MMK_CloseWin( win_id );     
        break;
    case MSG_CLOSE_WINDOW:
		if(s_need_cancel)
		{
			BT_CancelSearch();
		}
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : CreateSBandPopmenu
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void CreateSBandPopmenu(void)
{
    MMI_STRING_T      title_str = {0};
    uint16            cur_index     = 0;
    GUILIST_ITEM_T*   cur_list_ptr = {0};
    MMI_CTRL_ID_T      ctrl_id       = MMIBT_INQUIRED_DEV_LIST_CTRL_ID;

    MMK_CreateWin((uint32 *)MMIBT_INQUIRY_OPT_WIN_TAB,PNULL);
    GUIMENU_CreatDynamic(PNULL,MMIBT_INQUIRY_OPT_WIN_ID, MMIBT_LIST_INQUIRY_MENU_CTRL_ID,GUIMENU_STYLE_POPUP_AUTO);
    InsertNodeByLabel(
        MMIBT_LIST_INQUIRY_MENU_CTRL_ID,//ctrl_id
        STXT_PAIR,//text_ptr 
        MMIBT_INQUIRY_PAIRY_ID,//node_id
        0, //parent_node_id
        0,//index
        FALSE//is_grayed
        );

        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        cur_list_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);/*lint !e605*/
        if(PNULL != cur_list_ptr)
        {
            title_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_len;
            title_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_ptr;
            GUIMENU_SetMenuTitle(&title_str, MMIBT_LIST_INQUIRY_MENU_CTRL_ID);
         }
}

/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T   softkey_ctrl_id = 0;

    MMK_DestroyControl(MMK_GetCtrlHandleByWin(win_handle , MMITHEME_GetSoftkeyCtrlId()));

    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;
    

    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_CreateControl( &softkey_create );
    
    MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
    return TRUE;
}
#endif
/****************************************************************************/
//  Discription: Open the bluetooth Inquired device list win   
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL void  OpenInquiredDevListWin(void)
{
    MMK_CreateWin((uint32 *)MMIBT_INQUIRED_DEV_LIST_WIN_TAB, PNULL); 

    GUILIST_SetMaxItem(MMIBT_INQUIRED_DEV_LIST_CTRL_ID,MMIBT_MAX_DEV_ITEMS, FALSE);//max item 20

    GUILIST_SetListState(MMIBT_INQUIRED_DEV_LIST_CTRL_ID, GUILIST_STATE_NEED_TITLE, FALSE );
}
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetOpenIsPermit(BT_ADDRESS * bt_addr)
{
#if 0
    uint32      service_type = 0;
#endif
    BT_DEVICE_INFO dev_info = {0};

#if 0	
 	if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
 	{
 		//SCI_TRACE_LOW:"HeadsetOpenIsPermit in calling or alarming !"
 		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10854_112_2_18_2_8_22_347,(uint8*)"");
 		return FALSE;
 	}
#endif

    if(PNULL != bt_addr)
    {
        //first check already exist opened heaset
        if(MMIBT_OpenedHeadsetIsExist(&dev_info))
        {
           if(memcmp(&dev_info.addr, bt_addr, sizeof(BT_ADDRESS)))
           {
               //exist alrady opened headset
               //SCI_TRACE_LOW:"HeadsetOpenIsPermit exist alrady opened headset !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10865_112_2_18_2_8_22_348,(uint8*)"");
               return FALSE;
           }
        }
		#if 0
        service_type = BT_GetPairedDeviceService(bt_addr);
        if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            //Handfree, can not opened in call
            if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit in call !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10875_112_2_18_2_8_22_349,(uint8*)"");
                return FALSE;
            }
        }
        if((BT_SERVICE_A2DP | BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            //BT module does not need to judge audio condition
            /*
            //A2DP can not open, when music playing or paused
            if(MMIAPIMP3_IsPlayerActive())
            {
                //mp3 playing or paused 
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit mp3 pause or playing !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10887_112_2_18_2_8_22_350,(uint8*)"");
                return FALSE;
            }
            else if(MMIAPIKUR_IsPlayerActive())
            {
                //kuro playing or paused
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit kuro pause or playing  !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10893_112_2_18_2_8_22_351,(uint8*)"");
                return FALSE;
            }
            */
        }
		#endif

    }
    //SCI_TRACE_LOW:"HeadsetOpenIsPermit ok"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10900_112_2_18_2_8_22_352,(uint8*)"");
    return TRUE;
}
/*****************************************************************************/
//  Discription: insert menu node bu text id 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                     MMI_CTRL_ID_T      ctrl_id,        //控件id
                     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                     uint16          	node_id,        //节点编号
                     uint16          	parent_node_id, //父节点编号
                     uint16          	index,          //位置
                     BOOLEAN            is_graged
                     )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_graged;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
/*****************************************************************************/
//  Discription: open pin input win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E OpenPinInputWin(BT_DEVICE_INFO *param_ptr,BTFUNCWINCALLBACK call_back)
{
    MMIBT_PIN_INPUT_PARAM_T *win_param_ptr = PNULL;
    MMI_HANDLE_T                win_handle = 0;
    BOOLEAN                     res = MMI_RESULT_FALSE;  
    
    if(PNULL == param_ptr)
        return res;
    
    if(MMK_IsOpenWin(MMIBT_PIN_INPUT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }

    win_param_ptr = (MMIBT_PIN_INPUT_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_PIN_INPUT_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        SCI_MEMCPY(&win_param_ptr->pin_request_device_info, param_ptr, sizeof(BT_DEVICE_INFO));
        win_param_ptr->callbackfunc = call_back;
        win_handle = MMK_CreatePubEditWin((uint32 *)MMIBT_PIN_INPUT_WIN_TAB, PNULL);
        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_FREEIF(win_param_ptr);
        }
        else
        {
            MMK_SetWinUserData(MMIBT_PIN_INPUT_WIN_ID, (void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
        }        
    }
    else
    {
        SCI_TRACE_LOW("OpenPinInputWin alloc fail");
    }

    return res;
}
/*****************************************************************************/
//  Discription: open pin input win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void InputPairedPin(MMI_HANDLE_T win_handle,const wchar *str_ptr,int str_len)
{
    BT_STATUS        return_value = BT_PENDING;
    uint8            pin_str[MMIBT_PIN_MAX_LENGTH + 2 ] ={0};
    BT_DEVICE_INFO   pin_request_info = MMIBT_GetPinRequestInfo();
    MMI_STRING_T     wait_text = {0};
    
    MMI_WSTRNTOSTR(pin_str, MMIBT_PIN_MAX_LENGTH, str_ptr,str_len, str_len);
    pin_str[str_len] = 0;
    SetIsPinCancel(FALSE);
    return_value = BT_InputPairPin(&pin_request_info.addr,(uint8*)pin_str,str_len);
    if (BT_PENDING == return_value)
    {
        MMI_GetLabelTextByLang(TXT_BT_BOUNDING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text, PNULL ,PNULL,MMIBT_PAIR_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleCanbeCancelWaitingWinMsg); 
        MMIBT_StartOperationWin(MMIBT_PAIR_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
    }
    else
    {
         MMK_CloseWin(win_handle);
    }
}

/*****************************************************************************/
//  Description : display numeric passkey information
//  Global resource dependence : none
//  Author: 
//  Note:the 'MMIBT_PAIR_TEXT_LEN/2' in SCI_MEMCPY is to avoid array access
//       violation of content_str, it should be adjusted once the txt
//       TXT_BT_NUMERIC_PASSKEY couldn't display completely to a certain language
/*****************************************************************************/
LOCAL void Display_NumericPasskeyInfo(void)
{
    MMI_STRING_T        text_str                        = {0};//get and store TXT_BT_NUMERIC_PASSKEY
    MMI_CTRL_ID_T       ctrl_id                         = MMIBT_NUMERIC_PASSKEY_CTRL_ID;
    wchar               ssp_key[7]                      = {0};//get and store ssp pairing key
    wchar               content_str[MMIBT_PAIR_TEXT_LEN]= {0};//store the whole pairing info
    uint32              content_len                     = 0; 

    //get the content and length of TXT_BT_NUMERIC_PASSKEY, then copy the content to content_str
    MMI_GetLabelTextByLang(TXT_BT_NUMERIC_PASSKEY, &text_str);
	if(text_str.wstr_ptr != PNULL)
	{
        SCI_MEMCPY(content_str, text_str.wstr_ptr, sizeof(wchar)*MIN(text_str.wstr_len,MMIBT_PAIR_TEXT_LEN/2));
	}
	
    //get the device name from s_pin_request_info_ext and concat it to content_str, then contact a line break 
    MMIAPICOM_Wstrcat(content_str, L" ");
	MMIAPICOM_Wstrncat(content_str, s_pin_request_info_ext.name, MMIAPICOM_Wstrlen(s_pin_request_info_ext.name));
    MMIAPICOM_Wstrcat(content_str, L"?\n");

    //get ssp pairing key and concat to content_str
    MMIAPICOM_StrToWstr(s_pin_request_info_ext.ssp_numric_key, ssp_key);   
    MMIAPICOM_Wstrncat(content_str, ssp_key, MMIAPICOM_Wstrlen(ssp_key));

    //set the whole pairing info to the ctrl
    content_len = MMIAPICOM_Wstrlen(content_str);    
    GUITEXT_SetString(ctrl_id, content_str, content_len, FALSE);
    
    SCI_TRACE_LOW("Display_NumericPasskeyInfo info len %d",content_len);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_OpenWaitTimerOut(void)
{
    uint16    dev_type = 0;
    
    MMIBT_StopOpenWaitTimer();
//lzk mark
#ifdef BT_DIALER_SUPPORT
    BT_GetDeviceType(&g_current_bt, &dev_type);
    if(dev_type & BT_DEVICE_CLASS_AUDIO)
    {
        //headset need open it directly
        if(HeadsetOpenIsPermit(&(g_current_bt.addr)))
        {
             MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HEADSET);
             OpenHeadset(&g_current_bt.addr);
        }
    }  

    else if(dev_type & BT_DEVICE_CLASS_PHONE)
    {
        //headset need open it directly
        if(HandsetGatewayOpenIsPermit(&(s_pin_request_info.addr)))
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HANDSET_GATEWAY);
            OpenHandsetGateway(&s_pin_request_info.addr);
        }
    }
#else
    BT_GetDeviceType(&s_pin_request_info, &dev_type);
    if(dev_type & BT_DEVICE_CLASS_AUDIO)
    {
        //headset need open it directly
        if(HeadsetOpenIsPermit(&(s_pin_request_info.addr)))
        {
             MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HEADSET);
             OpenHeadset(&s_pin_request_info.addr);
        }
    }    
#endif
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_StopOpenWaitTimer(void)
{
    if(s_headset_openwait_timer != 0)    
    {   
        MMK_StopTimer(s_headset_openwait_timer);   
        s_headset_openwait_timer = 0;    
    } 
    SCI_TRACE_LOW("[MMIBT] MMIBT_CloseOpenWaitTimer, timer_id = %d", s_headset_openwait_timer);
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandsetGatewayOpenIsPermit(BT_ADDRESS * bt_addr)
{
    uint32      service_type = 0;
    BT_DEVICE_INFO dev_info = {0};
#ifdef BT_DIALER_SUPPORT
    BT_ADDRESS  dev_addr = {0};
#endif
    
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit in calling or alarming !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10854_112_2_18_2_8_22_347,(uint8*)"");
        return FALSE;
    }
    if(PNULL != bt_addr)
    {
        //first check already exist opened heaset
        if(MMIBT_OpenedHeadsetIsExist(&dev_info))
        {
            if(memcmp(&dev_info.addr, bt_addr, sizeof(BT_ADDRESS)))
            {
                //exist alrady opened headset
                //SCI_TRACE_LOW:"HeadsetOpenIsPermit exist alrady opened headset !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10865_112_2_18_2_8_22_348,(uint8*)"");
                return FALSE;
            }
        }
        //first check already exist opened heaset
        if(MMIBT_OpenedHandsetGatwayIsExist(&dev_info))
        {
           if(memcmp(&dev_info.addr, bt_addr, sizeof(BT_ADDRESS)))
           {
               //exist alrady opened headset
               //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit exist alrady opened headset !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10865_112_2_18_2_8_22_348,(uint8*)"");
               return FALSE;
           }
        }
        service_type = BT_GetPairedDeviceService(bt_addr);
        if ((BT_SERVICE_HANDFREE_GATEWAY |BT_SERVICE_HEADSET_GATEWAY) & service_type)
        {
            if(MMIBT_IsConnectDevice(&dev_addr))
            {
                SCI_TRACE_LOW("HeadsetOpenIsPermit if Hfu is not connected");
                return FALSE;
            }
        }
        if((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY | BT_SERVICE_MAP_SERVER |BT_SERVICE_PBAP_SERVER) & service_type)
        {
            //BT module does not need to judge audio condition
            /*
            //A2DP can not open, when music playing or paused
            if(MMIAPIMP3_IsPlayerActive())
            {
                //mp3 playing or paused 
                //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit mp3 pause or playing !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10887_112_2_18_2_8_22_350,(uint8*)"");
                return FALSE;
            }
            else if(MMIAPIKUR_IsPlayerActive())
            {
                //kuro playing or paused
                //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit kuro pause or playing  !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10893_112_2_18_2_8_22_351,(uint8*)"");
                return FALSE;
            }
            */
        }

    }
    //SCI_TRACE_LOW:"HandsetGatewayOpenIsPermit ok"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10900_112_2_18_2_8_22_352,(uint8*)"");
    return TRUE;
}
#endif
#endif





















