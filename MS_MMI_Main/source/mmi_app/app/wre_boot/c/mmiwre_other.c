#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT

#include "os_api.h"
#include "mmk_kbd.h"
#include "mmk_app.h"
#include "tasks_id.h"
#include "guilcd.h"
#include "guiblock.h"
#include "mmi_module.h"
#include "mmitheme_update.h"
#include "dal_time.h"
#include "watchdog.h"
#include "scm_api.h"
#include "mmiwre_other.h"
#include "cfl_dlmalloc.h"
/*added for demo 2010-11-25*/
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "app_tcp_if.h"
#include "sfs.h"
#include "sig_code.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmiWre_text.h"  
#include "mmiaudio_ctrl.h"
#include "mmk_tp.h"
#include "mmiwre.h"
#include "mmibt_export.h"
#include "mmk_msg.h"
#include "mmiwre_export.h"
#include "mmiwre_adaptor.h" // add by liang.chen, 2011-09-02
#include "mmiwre_anim.h"    //default wre app icon
#include "mmiwclk_export.h"
#include "mmiwre_nv.h"
#include "mmisms_text.h"
#include "mmisms_app.h"
#include "priority_app.h"
#include "mmidisplay_data.h"
#include "mmiwre_media.h"//added by leichi 20120417
#ifdef JAVA_SUPPORT
//#include "mmijava_wintab.h" //by huwei
#include "mmijava_export.h"
#endif
//#include "guistatusbar_data.h"
#include "mmiidle_statusbar.h"
#include "mmiidle_subwintab.h"
#include "mmi_id.h"

#ifdef ATEST_SUPPORT
#include "mmi_atc.h"
#endif //ATEST_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#include "mmisms_read.h"
#include "mmiwre_tapi.h"
#include "Mmiset_display.h"

#ifdef QBTHEME_SUPPORT
#include "Mmiqbtheme_export.h"
#endif

#ifdef SNS_PULLING_SUPPORT
#include "mmisns_srv.h"
#include "mmi_appmsg.h"
#endif //SNS_PULLING_SUPPORT

/*****************************************************************************/
//  Description : WRE_OtherTest
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

#ifdef _DEBUG
static void WRE_OtherTest()
{
    SCI_ASSERT(WRE_RESULT_FALSE == MMI_RESULT_FALSE);   /*assert verified*/
    SCI_ASSERT(WRE_RESULT_TRUE ==  MMI_RESULT_TRUE);    /*assert verified*/
}
#endif


#define WRE_APPREGTBL_NAME       L"\\appregtbl.dat"

#ifdef WIN32
extern void UniteAllLayers(void);
#endif
extern BOOLEAN WRE_InstalleApp(uint32 appid, uint16 *appname, uint *iconbuf, uint32  iconbuflen, uint16 *apppath);
extern BOOLEAN WRE_UninstallApp(uint32 appid, uint16 *appname);
//extern void MMIWRE_ResetRuntimePath(void);	// changed by yifei
extern void MMIWRE_WndDestroy(void);
extern void MMIWRE_InitDownLoad(BOOLEAN bFirstInit);	// changed by yifei
void WRE_InitDynamicMenu(uint32 firstid,uint32 lastid);
PUBLIC BOOLEAN WRE_HandleSMSMTInd(MmiSignalS** signal_ptr);

#ifdef SNS_PULLING_SUPPORT
#if defined(WRE_TWITTER_SUPPORT) && defined(WRE_FACEBOOK_SUPPORT)
#define NUMBER_OF_SNS_PULLING 2 //{WRE_FIX_ID_FACBK, WRE_FIX_ID_TWITR}
#endif //WRE_TWITTER_SUPPORT

LOCAL uint8 s_active_widget_tbl[MMI_SNS_MAX] = {0x01,0x02,0x04};
LOCAL uint8 s_active_widget_flag = 0;

LOCAL uint16 s_timer_preset_type[MMI_SNS_MAX] = {0};
LOCAL uint32 s_timer_time_tbl[MMI_SNS_TIMER_PRESET_MAX] = {0, 300000, 900000, 1800000, 3600000};

#ifdef NUMBER_OF_SNS_PULLING
LOCAL uint8 s_bg_fix_app_id = WRE_FIX_ID_TWITTER; //WRE_FIX_ID_FACBK
#else
LOCAL uint8 s_bg_fix_app_id = WRE_FIX_ID_FACEBOOK;
#endif //NUMBER_OF_SNS_PULLING

LOCAL uint8 s_bg_timer_id = 0;
LOCAL uint32 s_bg_restore_time = 900000; // 900 sec or 15min
LOCAL IMMISns *s_pMe = PNULL;

LOCAL void WRE_Wakeup_SNS_APPS(uint8 timer_id, uint32 param);
#endif //SNS_PULLING_SUPPORT

LOCAL  SWDG_OBJECT *s_watchdog_ptr = PNULL;
LOCAL  uint32      s_exitfromhomekey = WRE_NORMAL_EXIT_PATH;
MMI_APPLICATION_T   g_wre_app = {0};
LOCAL uint32 s_wre_vib_handle = 0;
BOOLEAN g_bDebugMode = FALSE;
BYTE   g_strDebugParam[WRE_DEBUG_TOTAL] = {0};
extern MMIWRE_RUNTIME_PATH_T g_wre_Runtime_Path;
/* asynchronous unpack file, by Tbill 20111110 */
#define UNPACK_READ_FILE_INFO_SIG   0x4168
#define UNPACK_READ_FILE_CONTENT_SIG   0x6168
#define UNPACK_WRITE_FILE_CONTENT_SIG   0x8168
#define UNPACK_FILE_FINISH_SIG   0x2168
#define UNPACK_STACK_SIZE					0x1000
#define UNPACK_QUEUE_NUM					0x0010
#define UNPACK_FILE_NAME_LEN    40
#define UNPACK_FILE_PATH_LEN    80

typedef struct asyn_unpack_info_struct_
{
    uint8 *databuf;
    uint16 srcfile[UNPACK_FILE_NAME_LEN];
    uint16 dstdir[UNPACK_FILE_PATH_LEN];
    uint32 offset;
    uint32 file_num;
    uint32 total_size;
    uint32 read_size;
    SFS_HANDLE file_handle;
    SFS_HANDLE hUnZipFile;
}asyn_unpack_info_struct;

typedef enum _unpack_file
{
    UNPACK_FILE_NOT_EXIST,
    UNPACK_FILE_EXIST,
    UNPACK_SOURCE_FILE_ERROR
}unpack_file_enum;

typedef enum _unpack_result
{
    UNPACK_FILE_FINISH,
    UNPACK_FILE_OPERATING_ERROR,
    UNPACK_PACKED_FILE_ERROR,
    UNPACK_FILE_PROCESSING
}unpack_result_enum;

BLOCK_ID   unpack_isr_id = SCI_INVALID_BLOCK_ID;
asyn_unpack_info_struct *gUnpackInfo = NULL;
/* asynchronous unpack file, by Tbill 20111110 */

/*****************************************************************************/
//  Description : WRE_Trace
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_BootTrace(const char *string)
{
#ifdef WIN32
    SCI_Trace("%s", string);
#else
    SCI_TRACE_LOW("%s", string);
#endif
}

/*****************************************************************************/
//  Description : WRE_Assert
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_Assert(void *exp, void *file, int  line)
{
    SCI_Assert(exp, file, line);/*assert verified*/
}

/*****************************************************************************/
//  Description : WRE_PAssert
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_PAssert(void *exp, void *file, uint32  line, char *assert_info_ptr)
{
    SCI_PAssert(exp, file, line, assert_info_ptr);/*assert verified*/
}
/*
uint16 *GetLcdLayer(void)
{
    uint16           *lcd_buf_ptr = NULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    lcd_buf_ptr = (uint16 *)GUIBLOCK_GetBuf(&lcd_dev_info);
    return lcd_buf_ptr;
}
*/


/*****************************************************************************/
//  Description : WRE_Free
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 WRE_Free(void *memory_ptr)
{
	SCI_FREE(memory_ptr);
    return 0;
}

/*****************************************************************************/
//  Description : WRE_Malloc
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void *WRE_Malloc(uint32 size, const char * file, uint32 line)
{
    return SCI_ALLOC_APP(size);
}

/*****************************************************************************/
//  Description : WRE_MEMCPY
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void *WRE_MEMCPY( void *dest, const void *src, uint32 count )
{
    void * p;
    p = memcpy(dest, src, count);
    return p;
}



/*****************************************************************************/
//  Description : GetMwinMocorWinHandle
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC WRE_HANDLE_T GetMwinMocorWinHandle(void)
{
    return MMIMWIN_WIN_ID;
}

/*****************************************************************************/
//  Description : WRE_IsFocusRunWin
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN WRE_IsFocusRunWin(void)
{
    return MMK_IsFocusWin(MMIMWIN_WIN_ID);
}


/*****************************************************************************/
//  Description : SetWatchDogPtr
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void SetWatchDogPtr(void *SwdgPtr)
{
    s_watchdog_ptr = (SWDG_OBJECT *)SwdgPtr;
}

/*****************************************************************************/
//  Description : WRETHEME_UpdateRect
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN WRETHEME_UpdateRect()
{
    //检验是否有层，并合并层;
    BOOLEAN     bRet;
    BOOLEAN     bLayerState;

#ifdef WIN32
        //UniteAllLayers();
#endif
    //bLayerState = UILAYER_GetDisplayRGB565TOARGB888State();
    //UILAYER_SetDisplayRGB565TOARGB888State(FALSE);
    bRet = MMITHEME_UpdateRect();
    //UILAYER_SetDisplayRGB565TOARGB888State(bLayerState);

    return bRet;
}

PUBLIC BOOLEAN WRE_HandleSMSMTInd(MmiSignalS** signal_ptr)
{    
    if ( PNULL == signal_ptr || PNULL == *signal_ptr )
    {
        return FALSE;
    }

    if ( (*signal_ptr)->SignalCode == APP_MN_SMS_IND )        
    {
		extern BOOLEAN  WRESMS_RecvSms_CallBack(MN_DUAL_SYS_E dual_sys,uint32 alphabet,MN_CALLED_NUMBER_T * origin_addr,APP_SMS_USER_DATA_T *sms_user_data_t);
        APP_SMS_USER_DATA_T sms_user_data_t = {0};
        APP_MN_SMS_IND_T *sig_ptr = signal_ptr;
        MN_SMS_USER_HEAD_T    user_head_ptr_t = {0};

        SCI_TRACE_LOW("[WRE]WRE_HandleSMSMTInd: APP_MN_SMS_IND!");
        
      /*  if (sig_ptr == NULL)
        {
            SCI_TRACE_LOW("[WRE]WRE_HandleSMSMTInd: sig_ptr is null!");
            return FALSE;
        }
		*/
        SCI_MEMSET( &sms_user_data_t, 0x00, sizeof( APP_SMS_USER_DATA_T ) );

        // need call MN API to decode user head and data.
        MMISMS_DecodeUserHeadAndContent(
            sig_ptr->dual_sys,
            sig_ptr->sms_t.user_head_is_exist,
            &(sig_ptr->sms_t.dcs),
            &(sig_ptr->sms_t.user_data_t),
            &sms_user_data_t
            );
        
        if(TRUE == WRESMS_RecvSms_CallBack(sig_ptr->dual_sys,sig_ptr->sms_t.dcs.alphabet_type,&sig_ptr->sms_t.origin_addr_t,&sms_user_data_t))
        {
            SCI_TRACE_LOW("[WRE]WRE_HandleSMSMTInd: WRESMS_RecvSms_CallBack return TRUE");
            
            MNSMS_UpdateSmsStatusEx(sig_ptr->dual_sys,sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE);
            return TRUE;
        }

    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : DispatchMocorSignal
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC void DispatchMocorSignal(void)
{
    MmiSignalS      *receiveSignal;
#ifndef WIN32
    SWDG_OBJECT     *watchdog_ptr = s_watchdog_ptr;  //监测mmi task出现死循环
#endif

#ifndef WIN32
    /* Set task status suspending*/
    SWDG_SetStatus(watchdog_ptr, SCI_FALSE);
#endif
    //Receive the system message(include the PS message and the REF message)
    MmiReceiveSignal(P_APP, &receiveSignal); 
#ifndef WIN32
    /* Fresh task tick */   
    SWDG_FreshTick(watchdog_ptr);
        /* Set task status running */
        SWDG_SetStatus(watchdog_ptr, SCI_TRUE);
#endif

#ifdef ATEST_SUPPORT		
			ATest_DispatchExtSig(&receiveSignal);
#endif //ATEST_SUPPORT

    if (WRE_HandleSMSMTInd(&receiveSignal))
    {
        SCI_TRACE_LOW("[WRE]Intercept SMS binding with WRE");
    }
    else
    {
        MMK_DispatchExtSig(&receiveSignal);
    }
        
    WRETHEME_UpdateRect();
    MMI_ChangeFreqByQuickKey();
    MmiDestroySignal(&receiveSignal);
}

/*****************************************************************************/
//  Description : WRE_GetMessageExx
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

long WRE_GetMessageExx(void *lpMsg, void *hwnd, uint32 wMsgFilterMin, uint32 wMsgFilterMax)
{
    static MMI_MESSAGE_PTR_T mmi_msg = PNULL;
    static int               needrefresh = 0;
    MmiSignalS               *receiveSignal;
#ifndef WIN32
    SWDG_OBJECT              *watchdog_ptr = s_watchdog_ptr;  //监测mmi task出现死循环
#endif

    if(needrefresh)
    {
        WRETHEME_UpdateRect();
        MMI_ChangeFreqByQuickKey();
        MMK_FreeMSG(mmi_msg);
        needrefresh = 0;
    }
label:
    if (MMK_GetMSGQueue(&mmi_msg))
    {
        if(mmi_msg->msg_type == MSG_TYPE_WINDOW)
        {
            MMK_DispatchWinMSG(mmi_msg);
        }
        else if(mmi_msg->msg_type == MSG_TYPE_BROADCAST)
        {
            MMK_DispatchBCMSG(mmi_msg);
        }
        else if(mmi_msg->msg_type == MSG_TYPE_WRE)
        {
        	extern uint32 fiberscheduler_GetMessage(void* p);
            WBOOT_SetMwinMsg(lpMsg);
                needrefresh = 1;

        return WRE_GetMessage(lpMsg);

        }

        WRETHEME_UpdateRect();
        MMI_ChangeFreqByQuickKey();
        MMK_FreeMSG(mmi_msg);
    }
    else
    {
#ifndef WIN32
        /* Set task status suspending*/
        SWDG_SetStatus(watchdog_ptr, SCI_FALSE);
#endif
        //Receive the system message(include the PS message and the REF message)
        MmiReceiveSignal(P_APP, &receiveSignal); 
#ifndef WIN32
        /* Fresh task tick */   
        SWDG_FreshTick(watchdog_ptr);
        /* Set task status running */
        SWDG_SetStatus(watchdog_ptr, SCI_TRUE);
#endif

#ifdef ATEST_SUPPORT		
			ATest_DispatchExtSig(&receiveSignal);
#endif //ATEST_SUPPORT

        //MMK_DispatchExtSig(&receiveSignal);
        if (WRE_HandleSMSMTInd(&receiveSignal))
        {
            SCI_TRACE_LOW("[WRE]Intercept SMS binding with WRE");
        }
        else
        {
            MMK_DispatchExtSig(&receiveSignal);
        }
        
        WRETHEME_UpdateRect();
        MMI_ChangeFreqByQuickKey();
        MmiDestroySignal(&receiveSignal);
        
    }// end of if
    goto label;    

/*    return TRUE;*/
}

/*****************************************************************************/
//  Description : WRE_ConvertMocorKeyToWreKey
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

LOCAL uint32 WRE_ConvertMocorKeyToWreKey( uint32 src_key )
{
    uint32  WRE_KeyCode = WRE_KEY_NONE;

    switch(src_key)
    {
        case KEY_0:
            WRE_KeyCode = WRE_KEY_0;
            break;
        case KEY_1:
            WRE_KeyCode = WRE_KEY_1;
            break;
        case KEY_2:
            WRE_KeyCode = WRE_KEY_2;
            break;
        case KEY_3:
            WRE_KeyCode = WRE_KEY_3;
            break;
        case KEY_4:
            WRE_KeyCode = WRE_KEY_4;
            break;
        case KEY_5:
            WRE_KeyCode = WRE_KEY_5;
            break;
        case KEY_6:
            WRE_KeyCode = WRE_KEY_6;
            break;
        case KEY_7:
            WRE_KeyCode = WRE_KEY_7;
            break;
        case KEY_8:
            WRE_KeyCode = WRE_KEY_8;
            break;
        case KEY_9:
            WRE_KeyCode = WRE_KEY_9;
            break;
        case KEY_A:
            WRE_KeyCode = WRE_KEY_A;
            break;
        case KEY_B:
            WRE_KeyCode = WRE_KEY_B;
            break;
        case KEY_C:
            WRE_KeyCode = WRE_KEY_C;
            break;
        case KEY_D:
            WRE_KeyCode = WRE_KEY_D;
            break;
        case KEY_E:
            WRE_KeyCode = WRE_KEY_E;
            break;
        case KEY_F:
            WRE_KeyCode = WRE_KEY_F;
            break;
        case KEY_G:
            WRE_KeyCode = WRE_KEY_G;
            break;
        case KEY_H:
            WRE_KeyCode = WRE_KEY_H;
            break;
        case KEY_I:
            WRE_KeyCode = WRE_KEY_I;
            break;
        case KEY_J:
            WRE_KeyCode = WRE_KEY_J;
            break;
        case KEY_K:
            WRE_KeyCode = WRE_KEY_K;
            break;
        case KEY_L:
            WRE_KeyCode = WRE_KEY_L;
            break;
        case KEY_M:
            WRE_KeyCode = WRE_KEY_M;
            break;
        case KEY_N:
            WRE_KeyCode = WRE_KEY_N;
            break;
        case KEY_O:
            WRE_KeyCode = WRE_KEY_O;
            break;
        case KEY_P:
            WRE_KeyCode = WRE_KEY_P;
            break;
        case KEY_Q:
            WRE_KeyCode = WRE_KEY_Q;
            break;
        case KEY_R:
            WRE_KeyCode = WRE_KEY_R;
            break;
        case KEY_S:
            WRE_KeyCode = WRE_KEY_S;
            break;
        case KEY_T:
            WRE_KeyCode = WRE_KEY_T;
            break;
        case KEY_U:
            WRE_KeyCode = WRE_KEY_U;
            break;
        case KEY_V:
            WRE_KeyCode = WRE_KEY_V;
            break;
        case KEY_W:
            WRE_KeyCode = WRE_KEY_W;
            break;
        case KEY_X:
            WRE_KeyCode = WRE_KEY_X;
            break;
        case KEY_Y:
            WRE_KeyCode = WRE_KEY_Y;
            break;
        case KEY_Z:
            WRE_KeyCode = WRE_KEY_Z;
            break;
        case KEY_STAR:
            WRE_KeyCode = WRE_KEY_STAR;
            break;
        case KEY_HASH:
            WRE_KeyCode = WRE_KEY_HASH;
            break;
        case KEY_LEFT:
            WRE_KeyCode = WRE_KEY_LEFT;
            break;
        case KEY_UP:
            WRE_KeyCode = WRE_KEY_UP;
            break;
        case KEY_RIGHT:
            WRE_KeyCode = WRE_KEY_RIGHT;
            break;
        case KEY_DOWN:
            WRE_KeyCode = WRE_KEY_DOWN;
            break;
        case KEY_OK:
            WRE_KeyCode = WRE_KEY_OK;
            break;
        case KEY_CANCEL:
            WRE_KeyCode = WRE_KEY_CANCEL;
            break;
        case KEY_WEB:
            WRE_KeyCode = WRE_KEY_WEB;
            break;
        case KEY_RED:
            WRE_KeyCode = WRE_KEY_RED;
            break;
        case KEY_GREEN:
            WRE_KeyCode = WRE_KEY_GREEN;
            break;
        case KEY_HEADSET_BUTTIN:
            WRE_KeyCode = WRE_KEY_HEADSET_BUTTIN;
            break;
        case KEY_HEADSET_DETECT:
            /* earphone insert or pull up */
            WRE_KeyCode = WRE_KEY_HEADSET_DETECT;
            break;
        case KEY_SDCARD_DETECT:
            WRE_KeyCode = WRE_KEY_SDCARD_DETECT;
            break;
        case KEY_DEL:
            WRE_KeyCode = WRE_KEY_DEL;
            break;
        case KEY_COMMA:
            WRE_KeyCode = WRE_KEY_COMMA;
            break;
        case KEY_PERIOD:
            WRE_KeyCode = WRE_KEY_PERIOD;
            break;
        case KEY_ENTER:
            WRE_KeyCode = WRE_KEY_ENTER;
            break;
        case KEY_FN:
            WRE_KeyCode = WRE_KEY_FN;
            break;
        case KEY_SHIFT:
            WRE_KeyCode = WRE_KEY_SHIFT;
            break;
        case KEY_SPACE:
            WRE_KeyCode = WRE_KEY_SPACE;
            break;
        case KEY_QUESTION:
            WRE_KeyCode = WRE_KEY_QUESTION;
            break;
        case KEY_CTRL:
            WRE_KeyCode = WRE_KEY_CTRL;
            break;
        case KEY_PLUS:
            WRE_KeyCode = WRE_KEY_PLUS;
            break;
        case KEY_AND:
            WRE_KeyCode = WRE_KEY_AND;
            break;
        case KEY_AT_QWERTY:
            WRE_KeyCode = WRE_KEY_AT_QWERTY;
            break;
        case KEY_LEFT_PARENTHESIS:
            WRE_KeyCode = WRE_KEY_LEFT_PARENTHESIS;
            break;
        case KEY_RIGHT_PARENTHESIS:
            WRE_KeyCode = WRE_KEY_RIGHT_PARENTHESIS;
            break;
        case KEY_MINUS:
            WRE_KeyCode = WRE_KEY_MINUS;
            break;
        case KEY_DOUBLE_QUOTES:
            WRE_KeyCode = WRE_KEY_DOUBLE_QUOTES;
            break;
        case KEY_SEMICOLON:
            WRE_KeyCode = WRE_KEY_SEMICOLON;
            break;
        case KEY_COLON:
            WRE_KeyCode = WRE_KEY_COLON;
            break;
        case KEY_SLASH:
            WRE_KeyCode = WRE_KEY_SLASH;
            break;
        case KEY_SHIFT_AND:
            WRE_KeyCode = WRE_KEY_SHIFT_AND;
            break;
        case KEY_EXCLAMATION:
            WRE_KeyCode = WRE_KEY_EXCLAMATION;
            break;
        case KEY_FLIP:
            WRE_KeyCode = WRE_KEY_FLIP;
            break;
        case KEY_SLIDE:
            WRE_KeyCode = WRE_KEY_SLIDE;
            break;
        case KEY_TV:
            WRE_KeyCode = WRE_KEY_TV;
            break;
        case KEY_CALL2:
            WRE_KeyCode = WRE_KEY_CALL2;
            break;
        case KEY_CALL3:
            WRE_KeyCode = WRE_KEY_CALL3;
            break;
        case KEY_MP3:
            WRE_KeyCode = WRE_KEY_MP3;
            break;
        case KEY_NOTES:
            WRE_KeyCode = WRE_KEY_NOTES;
            break;
        case KEY_SMS:
            WRE_KeyCode = WRE_KEY_SMS;
            break;
        case KEY_CALENDER:
            WRE_KeyCode = WRE_KEY_CALENDER;
            break;
        case KEY_IE:
            WRE_KeyCode = WRE_KEY_IE;
            break;
        case KEY_HANG:
            WRE_KeyCode = WRE_KEY_HANG;
            break;
        case KEY_VOL_UP:
            WRE_KeyCode = WRE_KEY_VOL_UP;
            break;
        case KEY_VOL_DOWN:
            WRE_KeyCode = WRE_KEY_VOL_DOWN;
            break;
        case KEY_MO:
            WRE_KeyCode = WRE_KEY_MO;
            break;
        case KEY_CAMREA:
            WRE_KeyCode = WRE_KEY_CAMREA;
            break;
        case KEY_SPUP:
            WRE_KeyCode = WRE_KEY_SPUP;
            break;
        case KEY_SPDW:
            WRE_KeyCode = WRE_KEY_SPDW;
            break;
        case KEY_UPSIDEKEY:
            WRE_KeyCode = WRE_KEY_UPSIDEKEY;
            break;
        case KEY_DOWNSIDEKEY:
            WRE_KeyCode = WRE_KEY_DOWNSIDEKEY;
            break;
        case KEY_GPIO_SIG1:
            WRE_KeyCode = WRE_KEY_GPIO_SIG1;
            break;
        case KEY_GPIO_SIG2:
            WRE_KeyCode = WRE_KEY_GPIO_SIG2;
            break;
        case KEY_VIDEO_TEL:
            WRE_KeyCode = WRE_KEY_VIDEO_TEL;
            break;
        case KEY_PLAYANDSTOP:
            WRE_KeyCode = WRE_KEY_PLAYANDSTOP;
            break;
        case KEY_FORWARD:
            WRE_KeyCode = WRE_KEY_FORWARD;
            break;
        case KEY_BACKWARD:
            WRE_KeyCode = WRE_KEY_BACKWARD;
            break;
        case KEY_MIDDLE:
            WRE_KeyCode = WRE_KEY_MIDDLE;
            break;
        case KEY_AT:
            WRE_KeyCode = WRE_KEY_AT;
            break;
        case KEY_HOOK:
            WRE_KeyCode = WRE_KEY_HOOK;
            break;
        case KEY_HANDFREE:
            WRE_KeyCode = WRE_KEY_HANDFREE;
            break;
        case KEY_CALL4:
            WRE_KeyCode = WRE_KEY_CALL4;
            break;
        case KEY_POWER:
            WRE_KeyCode = WRE_KEY_POWER;
            break;
        case KEY_MENU:
            WRE_KeyCode = WRE_KEY_MENU;
            break;
        case KEY_NONE:
            WRE_KeyCode = WRE_KEY_NONE;
            break;
        default:
            // SCI_PASSERT(0,("unknow key is: src_key = %lx", src_key));   /* assert to do*/
            break;
    }
    return WRE_KeyCode;
}

/*****************************************************************************/
//  Description : WreMwinWin_HandleMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
//static int wre_win_needpaint = FALSE;
extern BOOLEAN MMIWRE_IsWREAudioPlayerActive(void);//added by leichi 20120425   // changed by yifei
extern BOOLEAN MMIWRE_IsWREVideoPlayerActive(void);//added by leichi 20120425   // changed by yifei
extern LCD_ANGLE_E get_lcd_angle(void);
extern void set_lcd_angle(LCD_ANGLE_E angle);
#ifdef QBTHEME_SUPPORT
extern void MMIWRE_StartQbTheme(void);
#endif
MMI_RESULT_E WreMwinWin_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    uint32          mwin_msg_id  = 0;
    uint32          keys_status  = 0;
    uint32          key_code = 0;
    static BOOLEAN b_need_fill = FALSE;

    //SCI_TRACE_LOW:"WreMwinWin_HandleMsg: msg_id = 0x%08x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_679_112_2_18_3_10_50_121,(uint8*)"d",msg_id);
    if(msg_id == MSG_TP_PRESS_DOWN)
    {
        mwin_msg_id = WRE_MSG_TP_PRESS_DOWN;
    }
    else if(msg_id == MSG_TP_PRESS_UP)
    {
        mwin_msg_id = WRE_MSG_TP_PRESS_UP;
    }
    else if(msg_id == MSG_TP_PRESS_MOVE)
    {
        mwin_msg_id = WRE_MSG_TP_PRESS_MOVE;
    }
    else if(msg_id == MSG_TP_PRESS_LONG)
    {
        mwin_msg_id = WRE_MSG_TP_PRESS_LONG;
    }
    else if (msg_id == MSG_TP_GESTURE)
    {
        mwin_msg_id = WRE_MSG_TP_GESTURE;
    }
    else if (MSG_KEYUP_SDCARD_DETECT == msg_id) // add by liang.chen, 2011-09-27
    {
        mwin_msg_id = WRE_MSG_TP_SDPLUG_OUT;
    } // add end, 2011-09-27
    else
    {
        /* key process */
        keys_status = msg_id & 0xff00;
        if((keys_status >= KEY_PRESSED) && (keys_status <= KEY_REPEATED))
        {
            key_code = msg_id & 0xff;
            if((key_code == KEY_POWER) || (key_code == KEY_CAMREA))
            {
                if(msg_id == MSG_KEYUP_POWER)
                {
                    /*
                    key_code = KEY_RED;
                    MMK_CloseWin(MAIN_MAINMENU_WIN_ID);
                    */
                    /* press home key, close lcd. by wei.zhou */
                    MMIDEFAULT_CloseAllLight();
                    return MMI_RESULT_TRUE;
                }
                else
                {
                    return MMI_RESULT_FALSE;
                }
            }
            //heng.xiao modify for bug NEWMS00187723
        #if 0
            if(key_code == KEY_RED && MMK_IsOpenWin(MAIN_MAINMENU_WIN_ID))
            {
                s_exitfromhomekey = WRE_FROM_HOMEKEY_EXIT_PATH;
                MMK_CloseWin(MAIN_MAINMENU_WIN_ID);
            #ifdef QBTHEME_SUPPORT
                //MMIWRE_StartQbTheme();
            #endif
            }
        #endif
            /* remove the sweet menu fragment, no NEWMS00187723 cr. */
            if(key_code == KEY_RED)
            {
                #ifdef MENU_SWEETS_SUPPORT
                s_exitfromhomekey = WRE_FROM_HOMEKEY_EXIT_PATH;
                #endif
            }

            mwin_msg_id = WRE_ConvertMocorKeyToWreKey(key_code);

            switch(keys_status)
            {
                case KEY_PRESSED:
                    mwin_msg_id |= WRE_KEY_PRESSED;
                    break;
                case KEY_RELEASED:
                    mwin_msg_id |= WRE_KEY_RELEASED;
                    break;
                case KEY_LONG_PRESSED:
                    mwin_msg_id |= WRE_KEY_LONG_PRESSED;
                    break;
                case KEY_LONG_RELEASED:
                    mwin_msg_id |= WRE_KEY_LONG_RELEASED;
                    break;
                case KEY_REPEATED:
                    mwin_msg_id |= WRE_KEY_REPEATED;
                    break;
                default:
                    break;
            }
        }

        /* windows message process */
        switch(msg_id)
        {
			
            case MSG_OPEN_WINDOW:       
#if 1
#if 1                
                {
                    extern LCD_ANGLE_E get_lcd_angle(void); 
                    LCD_ANGLE_E tmp = get_lcd_angle();

					 if(MMK_GetScreenAngle() != LCD_ANGLE_0)
                    {			
                    SCI_TRACE_LOW("wrea:set b_need_fill");
                        b_need_fill = TRUE;
						
                    }
					MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
					MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
					
#if 0
                    if (tmp != LCD_ANGLE_0)
                    {//set to back                        
                        /*
                        MMK_SetScreenAngle(tmp);
                        */
                        /* by wei.zhou not must set to -1 */
                        if(tmp == -1)
                        {
                            MMK_SetScreenAngle(LCD_ANGLE_0);
                        }
                        else
                        {
                            MMK_SetScreenAngle(tmp);
                        }
                    }
                    b_need_fill = FALSE;
#endif

                    /* fragment garbage process, by wei.zhou*/
                    BLKMEM_GarbageClean();

                    /* enter wre, forbid the auto lock screen, by wei.zhou */
                    MMIDEFAULT_AllowOpenKeylocWin(FALSE);
                }
#else
				MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
                MMK_SetWinAngle(win_id, LCD_ANGLE_0, TRUE);
#endif      
#endif
                mwin_msg_id = WRE_MSG_OPEN_WINDOW;
                break;
            case MSG_FULL_PAINT:

#if 0
				if((int)get_lcd_angle() == -1)
                {//插usb后,返回,收不到MSG_GET_FOCUS,所以继续判断一下
                    set_lcd_angle(MMK_GetScreenAngle());
                    if(MMK_GetScreenAngle() != LCD_ANGLE_0)
                    {						
                        b_need_fill = TRUE;
                    }
                    else 
                    {
                        b_need_fill = FALSE;          		
                    }
                }
#endif
                if ( b_need_fill )
                {
					GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
					GUI_FillRect(&lcd_dev_info, MMITHEME_GetFullScreenRect(), MMI_BLACK_COLOR);                  
					b_need_fill = FALSE;  //reset
					                    SCI_TRACE_LOW("wrea:GUI_FillRect!2");
                }else{
									                    SCI_TRACE_LOW("wrea:GUI_FillRect no");
				}
                
                
                set_lcd_angle((LCD_ANGLE_E)-1);  //reset

                /* remove the call suspend, call lock screen, by wei.zhou */
                MMIDEFAULT_AllowOpenKeylocWin(FALSE);
                mwin_msg_id = WRE_MSG_FULL_PAINT;
                break;
            case MSG_USER_START:
                mwin_msg_id = WRE_MSG_USER_START;
                break;
            case MSG_GET_FOCUS:
                mwin_msg_id = WRE_MSG_GET_FOCUS;
			//	wre_win_needpaint = TRUE;
#if 1
				{
					LCD_ANGLE_E tmp = get_lcd_angle();
					LCD_ANGLE_E tmp2 = MMK_GetScreenAngle();
					if (tmp2 != LCD_ANGLE_0)
					{						
						b_need_fill = TRUE;
					}
                    else 
                    {
                        b_need_fill = FALSE;          		
					}	
                    
                    if((int)tmp != -1)  /*lint !e650*/
                    {//need set to back
                      //  MMK_SetScreenAngle(tmp);
                    }
                    else
                    {//save new lcd_angle
                        set_lcd_angle(tmp2);
                    }
				}
#endif
                break;
            case MSG_LOSE_FOCUS:
                mwin_msg_id = WRE_MSG_LOSE_FOCUS;
                break;
            case MSG_CLOSE_WINDOW:
                mwin_msg_id = WRE_MSG_CLOSE_WINDOW;
                break;
            case MSG_LCD_SWITCH:
                mwin_msg_id = WRE_MSG_LCD_SWITCH;
                break;
            case MSG_BACKLIGHT_TURN_ON:
                mwin_msg_id = WRE_MSG_BK_TURN_ON;
                break;
            case MSG_BACKLIGHT_TURN_OFF:
                mwin_msg_id = WRE_MSG_BK_TURN_OFF;
                break;
#if 0 //modified by leichi 20120416:so that wre application can receive the key_volume msg 
			case MSG_KEYDOWN_UPSIDE:
                return MMI_RESULT_FALSE;
            case MSG_KEYDOWN_DOWNSIDE:
                return MMI_RESULT_FALSE;
#else
			case MSG_KEYREPEAT_UPSIDE:
				if(MMIWRE_IsWREAudioPlayerActive()||MMIWRE_IsWREVideoPlayerActive())
				{
					MMIWRE_AdjustVolumeBySideKey_UP();
				}
				else
				{
					return MMI_RESULT_FALSE;
				}
				break;
            case MSG_KEYREPEAT_DOWNSIDE:
				if(MMIWRE_IsWREAudioPlayerActive()||MMIWRE_IsWREVideoPlayerActive())
				{
					MMIWRE_AdjustVolumeBySideKey_UP();
				}
				else
				{
					return MMI_RESULT_FALSE;
				}
				break;
			case MSG_KEYDOWN_UPSIDE:
		 		if(MMIWRE_IsWREAudioPlayerActive()||MMIWRE_IsWREVideoPlayerActive())
				{
					 MMIWRE_AdjustVolumeBySideKey_UP();
				}
				else
				{
					 return MMI_RESULT_FALSE;
				}
                break;
			case MSG_KEYDOWN_DOWNSIDE:
				if(MMIWRE_IsWREAudioPlayerActive()||MMIWRE_IsWREVideoPlayerActive())
		 		{
					  MMIWRE_AdjustVolumeBySideKey_DOWN();
				}
				else
				{
					 return MMI_RESULT_FALSE;
				}
                break;
#endif
			case MSG_KEYDOWN_CANCEL:
			//SCI_TRACE_LOW:"WRE  MSG_KEYDOWN_CANCEL 31"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_790_112_2_18_3_10_50_122,(uint8*)"");
			/*
			{
				extern uint32	g_download_window_id;
				if (g_download_window_id != 0)
				{
					MMK_PostMsg(g_download_window_id, MSG_APP_RED, PNULL,0);
				}
			}*/
				break;
            default:
                break;
        }
    }

    recode = WBOOT_MwinWin_HandleMsg(win_id, mwin_msg_id, param);
	 
	 if(msg_id == MSG_KEYDOWN_UPSIDE || msg_id == MSG_KEYDOWN_DOWNSIDE)//for side key wre
	 {
 		return MMI_RESULT_TRUE;
	 }
	 //added by leichi 20120329 begin
	 if(msg_id == MSG_KEYREPEAT_UPSIDE || msg_id == MSG_KEYREPEAT_DOWNSIDE)//for side keyrepeat wre
	 {
		 return MMI_RESULT_TRUE;
	 }//add by chenjianyun 20120725
	if((WRE_INIT_NO_ROOM == recode) &&(MSG_OPEN_WINDOW == msg_id) )  /*lint !e650*/
	{
		MMK_CloseWin(win_id);
	}
	//added by leichi 20120329 end
    if(recode == WRE_RESULT_TRUE)
    {
        recode = MMI_RESULT_TRUE;
    }
    else if(recode == WRE_RESULT_FALSE)
    {
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}

/*****************************************************************************/
//  Description : CreateMwinMangerWin
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

#if 0

WINDOW_TABLE( MMIWRE_BASE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ), 
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
		WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
		WIN_FUNC((uint32) WreMwinWin_HandleMsg),    
		WIN_ID(MMIMWIN_WIN_ID),
#ifdef MMI_PDA_SUPPORT
		WIN_HIDE_STATUS,
#endif
		END_WIN
};
#endif
void CreateMwinMangerWin(void)
{
#if 1
    MMI_WINDOW_CREATE_T win_create = {0};
    
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = MMIMWIN_WIN_ID;
    win_create.func = WreMwinWin_HandleMsg;
    win_create.win_priority = WIN_ONE_LEVEL;

    /* create the mocor windows */    
    MMK_CreateWinByApplet(SPRD_WRE_APPLET_ID,  (uint32 *)&win_create, PNULL);	// changed by yifei
#else
	 MMK_CreateWinByApplet(SPRD_WRE_APPLET_ID, (uint32*)MMIWRE_BASE_WIN_TAB, (ADD_DATA)0);
#endif
}

/*****************************************************************************/
//  Description : WRE_MMK_PostMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN WRE_MMK_PostMsg(WRE_HANDLE_T handle, uint32 msg_id, void  *param_ptr, uint32 size_of_param)
{
    MMI_MESSAGE_ID_E    MocorMsg_Id = MSG_USER_START;

    switch(msg_id)
    {
        case WRE_MSG_USER_START:
            MocorMsg_Id = MSG_USER_START;
            break;
        case WRE_MSG_TIMER:
            MocorMsg_Id = MSG_TIMER;
            break;
        case WRE_MSG_OPEN_WINDOW:
            MocorMsg_Id = MSG_OPEN_WINDOW;
            break;
        case WRE_MSG_CLOSE_WINDOW:
            MocorMsg_Id = MSG_CLOSE_WINDOW;
            break;
        case WRE_MSG_LOSE_FOCUS:
            MocorMsg_Id = MSG_LOSE_FOCUS;
            break;
        case WRE_MSG_GET_FOCUS:
            MocorMsg_Id = MSG_GET_FOCUS;
            break;
        case WRE_MSG_FULL_PAINT:
            MocorMsg_Id = MSG_FULL_PAINT;
            break;
        case WRE_MSG_LCD_SWITCH:
            MocorMsg_Id = MSG_LCD_SWITCH;
            break;
#if 0 /* SD plug out test */
        case WRE_MSG_TP_SDPLUG_OUT:
            MocorMsg_Id = MSG_KEYUP_SDCARD_DETECT;
            // break;

            #define     MSG_HANDLE_SD_ERR               0x903d
            return MMK_PostMsg(VIRTUAL_WIN_ID, MSG_HANDLE_SD_ERR, SCM_SLOT_0, sizeof(SCM_SLOT_NAME_E));
#endif /* SD plug out test */
        default:
            break;
    }

    return MMK_PostMsg(handle, MocorMsg_Id, param_ptr, size_of_param);
}

/*****************************************************************************/
//  Description : WRE_MMK_CloseWin
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN WRE_MMK_CloseWin(WRE_HANDLE_T win_id)
{
    return MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description : WRE_SetSysDate
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_SetSysDate(WRE_DATE_T  wre_sys_date)
{
    SCI_DATE_T  sys_date = {0};
    sys_date.mday = wre_sys_date.mday;
    sys_date.mon  = wre_sys_date.mon;
    sys_date.year = wre_sys_date.year;
    sys_date.wday = wre_sys_date.wday;

    TM_SetSysDate(sys_date);
}

/*****************************************************************************/
//  Description : WRE_GetSysDate
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_GetSysDate(WRE_DATE_T  *date_ptr)
{
    SCI_DATE_T  sys_date = {0};

    TM_GetSysDate(&sys_date);
    date_ptr->mday = sys_date.mday;
    date_ptr->mon  = sys_date.mon;
    date_ptr->year = sys_date.year;
    date_ptr->wday = sys_date.wday;
}

/*****************************************************************************/
//  Description : WRE_SetSysTime
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_SetSysTime(WRE_TIME_T  wre_sys_time)
{
    SCI_TIME_T  sys_time = {0};

    sys_time.hour = wre_sys_time.hour;
    sys_time.min  = wre_sys_time.min;
    sys_time.sec  = wre_sys_time.sec;

    TM_SetSysTime(sys_time);
}

/*****************************************************************************/
//  Description : WRE_GetSysTime
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_GetSysTime(WRE_TIME_T  *time_ptr)
{
    SCI_TIME_T  sys_time = {0};

    TM_GetSysTime(&sys_time);
    time_ptr->hour = sys_time.hour;
    time_ptr->min = sys_time.min;
    time_ptr->sec = sys_time.sec;
}

/*****************************************************************************/
//  Description : WRE_ConvertTpXcoord
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint16 WRE_ConvertTpXcoord(void *ptr)
{
    return MMK_GET_TP_X(ptr);
}

/*****************************************************************************/
//  Description : WRE_ConvertTpYcoord
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint16 WRE_ConvertTpYcoord(void *ptr)
{
    return MMK_GET_TP_Y(ptr);
}

/*****************************************************************************/
//  Description : GetMwinMsgAndMocorMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

static MMI_MESSAGE_PTR_T mmi_msg_ptr = NULL;

PUBLIC BOOLEAN GetMwinMsgAndMocorMsg(int *mmi_msg_type_ptr)
{
    BOOLEAN retcode;
    retcode = MMK_GetMSGQueue(&mmi_msg_ptr);
    if(retcode)
    {
        *mmi_msg_type_ptr = mmi_msg_ptr->msg_type;
    }
    return retcode;
}

/*****************************************************************************/
//  Description : DispatchMocorMSG
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC void DispatchMocorMSG(void)
{
    MMK_DispatchMSGQueue(mmi_msg_ptr);
    WRETHEME_UpdateRect();
    MMI_ChangeFreqByQuickKey();
    MMK_FreeMSG(mmi_msg_ptr);
}

/*****************************************************************************/
//  Description : WRE_IsMwinWin
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN WRE_IsMwinWin(int mmi_msg_type)
{
    if(mmi_msg_type == MSG_TYPE_WRE)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : WRE_APICOM_OtherTaskToMMI
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_APICOM_OtherTaskToMMI(WRE_HANDLE_T handle, uint32 msg_id, void *param_ptr, uint32 size_of_param)
{
    MMI_MESSAGE_ID_E    MocorMsg_Id = 0;

    /* windows message process */
    switch(msg_id)
    {
        case WRE_MSG_USER_START:
            MocorMsg_Id = MSG_USER_START;
            break;
        case WRE_MSG_TIMER:
            MocorMsg_Id = MSG_TIMER;
            break;
        case WRE_MSG_OPEN_WINDOW:
            MocorMsg_Id = MSG_OPEN_WINDOW;
            break;
        case WRE_MSG_CLOSE_WINDOW:
            MocorMsg_Id = MSG_CLOSE_WINDOW;
            break;
        case WRE_MSG_LOSE_FOCUS:
            MocorMsg_Id = MSG_LOSE_FOCUS;
            break;
        case WRE_MSG_GET_FOCUS:
            MocorMsg_Id = MSG_GET_FOCUS;
            break;
        case WRE_MSG_FULL_PAINT:
            MocorMsg_Id = MSG_FULL_PAINT;
            break;
        case WRE_MSG_LCD_SWITCH:
            MocorMsg_Id = MSG_LCD_SWITCH;
            break;
        default:
            break;
    }

    MMIAPICOM_OtherTaskToMMI(handle, MocorMsg_Id, param_ptr, size_of_param);
}


/*****************************************************************************/
//  Description : ADAPT_CFL_MspaceMalloc
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void* ADAPT_CFL_MspaceMalloc(void * msp_handle, int bytes)
{
    return CFL_MspaceMalloc((CFL_MSPACE_HANDLE)msp_handle,bytes);   
}

/*****************************************************************************/
//  Description : ADAPT_CFL_MspaceFree
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void ADAPT_CFL_MspaceFree(void* msp_handle, void* mem_ptr)
{
    CFL_MspaceFree((CFL_MSPACE_HANDLE)msp_handle,mem_ptr);
}

/*****************************************************************************/
//  Description : ADAPT_CFL_DestroyMspace
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int ADAPT_CFL_DestroyMspace(void *msp_handle)
{
    return CFL_DestroyMspace((CFL_MSPACE_HANDLE)msp_handle);
}

/*****************************************************************************/
//  Description : ADAPT_CFL_CreateMspace
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

 void * ADAPT_CFL_CreateMspace(void* base_ptr, int capacity)
 {
    void *p;
    SCI_TRACE_LOW("base_ptr=%x capacity=%d", (uint32)base_ptr, capacity);
    p = (void*)CFL_CreateMspace(base_ptr,capacity);
    SCI_TRACE_LOW("wre_mspace=%x", (uint32)p);
    return p;
 }

/*****************************************************************************/
//  Description : ADAPT_TurnOnBackLight
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void ADAPT_TurnOnBackLight(void)
{
    MMIDEFAULT_TurnOnBackLight();
}

/*****************************************************************************/
//  Description : ADAPT_AllowTurnOffBackLight
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
void ADAPT_AllowTurnOffBackLight(BOOLEAN allow_turn_off)
{
    MMIDEFAULT_AllowTurnOffBackLight(allow_turn_off);
    /* avoiding allow back light, open the keylock, by wei.zhou */
    if(allow_turn_off)
    {
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
    }
}

/*****************************************************************************/
//  Description : vibrate call back.
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void VibrateCB(void)
{
    s_wre_vib_handle = 0;
}

/*****************************************************************************/
//  Description : ADAPT_Vibrate
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//yj
BOOLEAN ADAPT_Vibrate(uint32 duaration){
    return s_wre_vib_handle = MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, duaration, 0, 1, VibrateCB);
}


/*****************************************************************************/
//  Description : ADAPT_StopVibrate
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN ADAPT_StopVibrate(void){
    if(s_wre_vib_handle != 0)
    {
        MMISRVAUD_StopVibrate(s_wre_vib_handle);
        s_wre_vib_handle = 0;
    }  
    return TRUE;
}

#define Z_PREFIX
#include "zlib.h"

#define     MAX_CHUNK_LEN              (128*1024)

#undef      MAX_PATH
#define     MAX_PATH                   260

#define     WRE_SZIP_IDENT_MAX_LEN     0x10
#define     WRE_UNPACK_BUFFER_SIZE     (256*1024)
typedef struct
{
    uint8   ident[WRE_SZIP_IDENT_MAX_LEN];
    uint32  file_num;
    uint32  total_size;
    uint32  crc_data;
    uint32  reserved;
}WRE_SZIP_HEAD_INFO;

typedef struct
{
    uint32  file_name_len;                //文件名长度
    uint32  file_size;                    //文件大小
    uint32  zip_size;                     //文件压缩后大小
    uint32  reserved;                     //保留
}WRE_SZIP_FILE_INFO;


/*****************************************************************************/
//  Description : wre_wcsstr
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

wchar *wre_wcsstr(const wchar *s,const wchar *b)
{
    wchar *x;
    wchar *y;
    wchar *c;

    if (s == NULL || b == NULL)
		return NULL;

    x=(wchar_t *)s;

    while (*x)
    {
        if (*x==*b)
        {
            y=x;
            c=(wchar_t *)b;
            while (*y && *c && *y==*c)
            {
                c++;
                y++;
            }
            if (!*c)
                return x;
        }
        x++;
    }
    return NULL;
}

/*****************************************************************************/
//  Description : CreateDirFile
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

SFS_HANDLE CreateDirFile(const uint16 *file_name, uint32 access_mode,uint32 share_mode,uint32 file_attri, uint16 *dstdir)
{
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint16      tempdir[WRE_MAX_PATH] = {0};
    uint16      outdir[WRE_MAX_PATH] = {0};
    uint16      *p, *q;

    hFile = SFS_CreateFile(file_name, access_mode, share_mode, file_attri);

    if(!hFile)
    {
        p = wre_wcsstr(file_name, L".\\");
        if(p)
        {
            p += 2;
            while(TRUE)/*lint !e716*/
            {
                q = wre_wcsstr(p, L"\\");
                if(!q)
                {
                    break;
                }
                memset(tempdir, 0, WRE_MAX_PATH * sizeof(uint16));
                MMIAPICOM_Wstrncpy(tempdir, p, (int)(q - p));

                MMIAPICOM_Wstrcat(outdir, L"\\");
                MMIAPICOM_Wstrcat(outdir, tempdir);
                ret = SFS_CreateDirectory(outdir);
                p = q + 1;
            }
        }
    }
    else
    {
        return hFile;
    }
    hFile = SFS_CreateFile(file_name, access_mode, share_mode, file_attri);

    return hFile;
}

/*****************************************************************************/
//  Description : unpack
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
BOOLEAN IsFileIdentify(SFS_HANDLE hSrcFile, uint32 filesize, uint32 offset, const uint16 *filename, uint8 *file_buf)
{
    SFS_HANDLE          hFile = PNULL;
    uint32              dstfilesize;
    uint8               *dst_fileubf = PNULL;
    SFS_ERROR_E         err = SFS_NO_ERROR;
    uint32              srcrdsize = 0;
    uint32              dstrdsize = 0;
    BOOLEAN             bRet = TRUE;

    hFile = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(!hFile)
    {
        return FALSE;
    }

    err = SFS_GetFileSize(hFile, &dstfilesize);
    if(dstfilesize != filesize)
    {
        SFS_CloseFile(hFile);
        return FALSE;
    }

    if(filesize == 0)
    {
        SFS_CloseFile(hFile);
        return TRUE;
    }

    dst_fileubf = (uint8*)SCI_ALLOC_APP( WRE_UNPACK_BUFFER_SIZE );
    err = SFS_SetFilePointer(hSrcFile, (__int64)offset, SFS_SEEK_BEGIN);

    while(TRUE) /*lint !e716*/
    {
        if(filesize > WRE_UNPACK_BUFFER_SIZE)
        {
            err = SFS_ReadFile(hSrcFile, (void *)file_buf, WRE_UNPACK_BUFFER_SIZE, &srcrdsize, PNULL);
            if(SFS_NO_ERROR == err)
            {
                err = SFS_ReadFile(hFile, (void *)dst_fileubf, WRE_UNPACK_BUFFER_SIZE, &dstrdsize, PNULL);
                //SCI_ASSERT(srcrdsize == dstrdsize);     /*assert verified*/
                SCI_PASSERT((srcrdsize == dstrdsize), ("file is file =%lx, src=%lx dst=%lx",filesize, srcrdsize, dstrdsize));/*assert verified*/	
                filesize = filesize - WRE_UNPACK_BUFFER_SIZE;
                offset += WRE_UNPACK_BUFFER_SIZE;
                if(memcmp(file_buf, dst_fileubf, srcrdsize) != 0)
                {
                    /* file not identify */
                    bRet = FALSE;
                    break;
                }
            }
        }
        else
        {
            err = SFS_ReadFile(hSrcFile, (void *)file_buf, filesize, &srcrdsize, PNULL);
            if(SFS_NO_ERROR == err)
            {
                err = SFS_ReadFile(hFile, (void *)dst_fileubf, filesize, &dstrdsize, PNULL);
                //SCI_ASSERT(srcrdsize == dstrdsize);     /*assert verified*/
                SCI_PASSERT((srcrdsize == dstrdsize), ("file is file =%lx, src=%lx dst=%lx",filesize, srcrdsize, dstrdsize));/*assert verified*/	
                offset += filesize;
                if(memcmp(file_buf, dst_fileubf, srcrdsize) != 0)
                {
                    /* file not identify */
                    bRet = FALSE;
                }
            }
            break;
        }
    }

    SCI_FREE(dst_fileubf);
    SFS_CloseFile(hFile);
    return bRet;
}

/* mocor版本的 szip接口 */
/* asynchronous unpack file begin, by Tbill 20111110 */
void AsyUpackSendSignal(unsigned short SignalCode)
{
   xSignalHeader signal;

   signal = SCI_ALLOC (sizeof (*signal));
   signal->SignalCode = SignalCode;
   signal->SignalSize = sizeof (*signal);
   signal->Sender = SCI_IdentifyThread();
   SCI_SendSignal (signal, unpack_isr_id); 
}


void UnpackReleaseMem(uint32 *result)
{
    if(gUnpackInfo->file_handle!= SFS_INVALID_HANDLE)
    {
       SFS_CloseFile(gUnpackInfo->file_handle);
       gUnpackInfo->file_handle = SFS_INVALID_HANDLE;
    }
   
    if(gUnpackInfo->hUnZipFile != SFS_INVALID_HANDLE)
    {
       SFS_CloseFile(gUnpackInfo->hUnZipFile);
       gUnpackInfo->hUnZipFile = SFS_INVALID_HANDLE;
    }

    if(*result == UNPACK_PACKED_FILE_ERROR)
    {
        SFS_DeleteFile(gUnpackInfo->srcfile, SCI_NULL);
    }
    if(gUnpackInfo != NULL)
    {
       if(gUnpackInfo->databuf != NULL)
       {
           SCI_FREE(gUnpackInfo->databuf);
           gUnpackInfo->databuf = NULL;
       }
       SCI_FREE(gUnpackInfo);
       gUnpackInfo = NULL;
    }

}



void WreReadFileProc(SFS_ERROR_E error, uint32 irp_param, uint32 param1, uint32 param2)
{
    uint32 *result = (uint32 *)irp_param;
    if(error == SFS_ERROR_NONE)
    {
       if(gUnpackInfo->read_size == param1)
       {
          AsyUpackSendSignal(UNPACK_WRITE_FILE_CONTENT_SIG);
          return;
       }
    }
    *result = UNPACK_FILE_OPERATING_ERROR;
    AsyUpackSendSignal(UNPACK_FILE_FINISH_SIG);
    return;
}


void WreWriteFileProc(SFS_ERROR_E error, uint32 irp_param, uint32 param1, uint32 param2)
{
    uint32 *result = (uint32 *)irp_param;

    if(error == SFS_ERROR_NONE)
    {
        if(gUnpackInfo->read_size == param1)
        {
           if(gUnpackInfo->read_size < gUnpackInfo->total_size)
           {
              gUnpackInfo->total_size -= gUnpackInfo->read_size;
              gUnpackInfo->offset += gUnpackInfo->read_size;
              gUnpackInfo->read_size = 0;
              AsyUpackSendSignal(UNPACK_READ_FILE_CONTENT_SIG);
           }
           else
           {
              if(gUnpackInfo->file_num > 1)
              {
                 gUnpackInfo->file_num--;
                 gUnpackInfo->offset += gUnpackInfo->read_size;
                 AsyUpackSendSignal(UNPACK_READ_FILE_INFO_SIG);
              }
              else if(gUnpackInfo->file_num == 1)
              {
                 gUnpackInfo->file_num--;
                 *result = UNPACK_FILE_FINISH;
                 AsyUpackSendSignal(UNPACK_FILE_FINISH_SIG);
              }
           }
           return;
        }
    }
    *result = UNPACK_FILE_OPERATING_ERROR;
    AsyUpackSendSignal(UNPACK_FILE_FINISH_SIG);

}

void UnpackNextFile(uint32 *result)
{
    if(gUnpackInfo->file_num > 1)
    {
        gUnpackInfo->file_num--;
        AsyUpackSendSignal(UNPACK_READ_FILE_INFO_SIG);
    }
    else if(gUnpackInfo->file_num == 1)
    {
        gUnpackInfo->file_num--;
        *result = UNPACK_FILE_FINISH;
        AsyUpackSendSignal(UNPACK_FILE_FINISH_SIG);
    }
}


unpack_file_enum CheckFileIdentify(SFS_HANDLE hSrcFile, uint32 filesize, uint32 offset, const uint16 *filename, uint8 *file_buf)
{
    SFS_HANDLE          hFile = PNULL;
    uint32              dstfilesize = 0;
    uint8               *dst_fileubf = PNULL;
    SFS_ERROR_E         err = SFS_NO_ERROR;
    uint32              srcrdsize = 0;
    uint32              dstrdsize = 0;
    unpack_file_enum     bRet = UNPACK_FILE_EXIST;


    hFile = SFS_CreateFile(filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(!hFile)
    {
        return UNPACK_FILE_NOT_EXIST;  /* the file don't exist */
    }

    err = SFS_GetFileSize(hFile, &dstfilesize);
    if(dstfilesize != filesize)
    {
        SFS_CloseFile(hFile);
        return UNPACK_FILE_NOT_EXIST;  /* file modify, need to create again*/
    }

    if(filesize == 0)
    {
        SFS_CloseFile(hFile);
        return UNPACK_FILE_EXIST;  /* file has no content, use the older*/
    }

    dst_fileubf = (uint8*)SCI_ALLOC_APP( WRE_UNPACK_BUFFER_SIZE );
    err = SFS_SetFilePointer(hSrcFile, (__int64)offset, SFS_SEEK_BEGIN);

    while(TRUE) //lint !e716
    {
        if(filesize > WRE_UNPACK_BUFFER_SIZE)
        {
            err = SFS_ReadFile(hSrcFile, (void *)file_buf, WRE_UNPACK_BUFFER_SIZE, &srcrdsize, PNULL);
            if(SFS_NO_ERROR == err)
            {
                err = SFS_ReadFile(hFile, (void *)dst_fileubf, WRE_UNPACK_BUFFER_SIZE, &dstrdsize, PNULL);
                if(srcrdsize != dstrdsize)
                {
                   bRet = UNPACK_SOURCE_FILE_ERROR;
                   break;
                }
                else
                {
                   filesize = filesize - WRE_UNPACK_BUFFER_SIZE;
                   offset += WRE_UNPACK_BUFFER_SIZE;
                   if(memcmp(file_buf, dst_fileubf, srcrdsize) != 0)
                   {
                       bRet = UNPACK_FILE_NOT_EXIST;  /* file content modified, need to create again*/
                       break;
                   }
                }
            }
            else
            {
                bRet = UNPACK_SOURCE_FILE_ERROR;
                break;
            }
        }
        else
        {
            err = SFS_ReadFile(hSrcFile, (void *)file_buf, filesize, &srcrdsize, PNULL);
            if(SFS_NO_ERROR == err)
            {
                err = SFS_ReadFile(hFile, (void *)dst_fileubf, filesize, &dstrdsize, PNULL);
                if(srcrdsize != dstrdsize)
                {
                   bRet = UNPACK_SOURCE_FILE_ERROR;
                }
                else
                {
                   offset += filesize;
                   if(memcmp(file_buf, dst_fileubf, srcrdsize) != 0)
                   {
                       bRet = UNPACK_FILE_NOT_EXIST; /* file content modified, need to create again*/
                   }
                }
            }
            else
            {
                bRet = UNPACK_SOURCE_FILE_ERROR;
                break;
            }
            break;
        }
    }

    SCI_FREE(dst_fileubf);
    SFS_CloseFile(hFile);
    if(bRet == UNPACK_SOURCE_FILE_ERROR)
    {
       SFS_DeleteFile(filename, SCI_NULL);
    }
    return bRet;
}

unpack_file_enum UnpackCreateFile(uint32 file_name_len, uint32 file_size)
{
    uint16 new_file_name[UNPACK_FILE_NAME_LEN];
    uint16 file_name_path[UNPACK_FILE_PATH_LEN];
    unpack_file_enum ret = UNPACK_FILE_NOT_EXIST;
    uint16 * pt;

    memset(new_file_name, 0, sizeof(new_file_name));
    memset(file_name_path, 0, sizeof(file_name_path));
    GUI_GBToWstr(new_file_name, gUnpackInfo->databuf, file_name_len);
    MMIAPICOM_Wstrcpy(file_name_path, gUnpackInfo->dstdir);
    MMIAPICOM_Wstrcat(file_name_path, L"\\");
    pt = (uint16 *)new_file_name;
    if(wre_wcsstr(pt, L".\\"))
    {
       pt += 2;
    }
    MMIAPICOM_Wstrcat(file_name_path, pt);

    memset(gUnpackInfo->databuf, 0, WRE_UNPACK_BUFFER_SIZE);
    ret = CheckFileIdentify(gUnpackInfo->file_handle, file_size, gUnpackInfo->offset, file_name_path, gUnpackInfo->databuf);
    if(ret == UNPACK_FILE_NOT_EXIST)
    {
       gUnpackInfo->hUnZipFile = CreateDirFile(file_name_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL, gUnpackInfo->dstdir);
    }
    return ret;
}


void UnpackWriteData(uint32 *result)
{
    SFS_ERROR_E err;
    SFS_OVERLAPPED_T    overLapped = {0};
    uint32 size;

    overLapped.complete_route = WreWriteFileProc;
    overLapped.param = (SFS_PARAM)result;
    err = SFS_WriteFile(gUnpackInfo->hUnZipFile, (void *)gUnpackInfo->databuf, gUnpackInfo->read_size, &size, &overLapped);
    if(SFS_ERROR_NONE != err && SFS_ERROR_IO_PENDING != err)
    {
        *result = UNPACK_FILE_OPERATING_ERROR;
        AsyUpackSendSignal(UNPACK_FILE_FINISH_SIG);
    }
}


void UnpackReadData(uint32 *result)
{
    SFS_ERROR_E err;
    SFS_OVERLAPPED_T    overLapped = {0};
    uint32 size;

    overLapped.complete_route = WreReadFileProc;
    overLapped.param = (SFS_PARAM)result;
    memset(gUnpackInfo->databuf, 0, WRE_UNPACK_BUFFER_SIZE);
    if(gUnpackInfo->total_size > WRE_UNPACK_BUFFER_SIZE)
    {
        gUnpackInfo->read_size = WRE_UNPACK_BUFFER_SIZE;
        err = SFS_ReadFile(gUnpackInfo->file_handle, (void *)gUnpackInfo->databuf, WRE_UNPACK_BUFFER_SIZE, &size, &overLapped);
    }
    else
    {
        gUnpackInfo->read_size = gUnpackInfo->total_size;
        err = SFS_ReadFile(gUnpackInfo->file_handle, (void *)gUnpackInfo->databuf, gUnpackInfo->total_size, &size, &overLapped);
    }
    if(SFS_ERROR_NONE != err && SFS_ERROR_IO_PENDING != err)
    {
        *result = UNPACK_FILE_OPERATING_ERROR;
        AsyUpackSendSignal(UNPACK_FILE_FINISH_SIG);
    }
}


void UnpackReadFileInfo(uint32 *result)
{
    SFS_OVERLAPPED_T    overLapped = {0};
    WRE_SZIP_FILE_INFO file_info = {0};
    unpack_file_enum ret = UNPACK_FILE_NOT_EXIST;
    SFS_ERROR_E err;
    uint32 size;

    //Read the file information
    if(gUnpackInfo->hUnZipFile != SFS_INVALID_HANDLE)
    {
       SFS_CloseFile(gUnpackInfo->hUnZipFile);
       gUnpackInfo->hUnZipFile = SFS_INVALID_HANDLE;
    }
    err = SFS_SetFilePointer(gUnpackInfo->file_handle, (__int64)gUnpackInfo->offset, SFS_SEEK_BEGIN);
    if(SFS_NO_ERROR != err)
    {
       *result = UNPACK_FILE_OPERATING_ERROR;
        return;
    }
    err = SFS_ReadFile(gUnpackInfo->file_handle, (void *)&file_info, sizeof(WRE_SZIP_FILE_INFO), &size, NULL);
    if(SFS_ERROR_NONE != err)
    {
       *result = UNPACK_FILE_OPERATING_ERROR;
        return;
    }
    file_info.file_name_len = htonl(file_info.file_name_len);
    file_info.file_size = htonl(file_info.file_size);
    file_info.zip_size  = htonl(file_info.zip_size);
    gUnpackInfo->offset += sizeof(WRE_SZIP_FILE_INFO);
                      
    err = SFS_SetFilePointer(gUnpackInfo->file_handle, (__int64)gUnpackInfo->offset, SFS_SEEK_BEGIN);
    if(SFS_NO_ERROR != err)
    {
       *result = UNPACK_FILE_OPERATING_ERROR;
       return;
    }
    if(gUnpackInfo->databuf == NULL)
    {
       gUnpackInfo->databuf = (uint8*)SCI_ALLOC_APP(WRE_UNPACK_BUFFER_SIZE);
    }
    memset(gUnpackInfo->databuf, 0, WRE_UNPACK_BUFFER_SIZE);
    err = SFS_ReadFile(gUnpackInfo->file_handle, (void *)gUnpackInfo->databuf, file_info.file_name_len * sizeof(uint8), &size, NULL);
    if(SFS_ERROR_NONE != err)
    {
       *result = UNPACK_FILE_OPERATING_ERROR;
        return;
    }  
    gUnpackInfo->offset += file_info.file_name_len;
    gUnpackInfo->total_size = file_info.file_size;
    ret = UnpackCreateFile(file_info.file_name_len, file_info.file_size);
    if(ret == UNPACK_FILE_EXIST)
    {
        gUnpackInfo->offset += file_info.file_size;
        UnpackNextFile(result);
        return;
    }
    else if(ret == UNPACK_SOURCE_FILE_ERROR)
    {
       *result = UNPACK_PACKED_FILE_ERROR;
       return;
    }

    if (gUnpackInfo->hUnZipFile == SCI_NULL)  /* hUnZipFile has been created in UnpackCreateFile */
    {
       *result = UNPACK_FILE_OPERATING_ERROR;
       return;
    }
    err = SFS_SetFilePointer(gUnpackInfo->file_handle, (__int64)gUnpackInfo->offset, SFS_SEEK_BEGIN);
    if(SFS_NO_ERROR != err)
    {
       *result = UNPACK_FILE_OPERATING_ERROR;
        return;
    }
    memset(gUnpackInfo->databuf, 0, WRE_UNPACK_BUFFER_SIZE);
    overLapped.complete_route = WreReadFileProc;
    overLapped.param = (SFS_PARAM)result;
    if(gUnpackInfo->total_size > WRE_UNPACK_BUFFER_SIZE)
    {
       gUnpackInfo->read_size = WRE_UNPACK_BUFFER_SIZE;
    }
    else
    {
       gUnpackInfo->read_size = gUnpackInfo->total_size;
    }
    err = SFS_ReadFile(gUnpackInfo->file_handle, (void *)gUnpackInfo->databuf, gUnpackInfo->read_size, &size, &overLapped);
    if(SFS_ERROR_NONE != err && SFS_ERROR_IO_PENDING != err)
    {
       SFS_CloseFile(gUnpackInfo->hUnZipFile);
       gUnpackInfo->hUnZipFile = SFS_INVALID_HANDLE;
       *result = UNPACK_FILE_OPERATING_ERROR;
       return;
    }
}


LOCAL void Asyn_UnpackThread(uint32 argc, void *argv)
{
   xSignalHeader signal = NULL;
   BLOCK_ID    tid = SCI_IdentifyThread();
   uint32 *result = (uint32 *)argv;
 
    while(1) //lint !e716
    {
       signal = (xSignalHeader)SCI_GetSignal(tid);
       if(signal->SignalCode == UNPACK_READ_FILE_INFO_SIG)
       {
          UnpackReadFileInfo(result);
       }
       else if(signal->SignalCode == UNPACK_READ_FILE_CONTENT_SIG)
       {
           UnpackReadData(result);
       }
       else if(signal->SignalCode == UNPACK_WRITE_FILE_CONTENT_SIG)
       {
           UnpackWriteData(result);
       }
       else if(signal->SignalCode == UNPACK_FILE_FINISH_SIG)
       {
           if(gUnpackInfo->hUnZipFile != SFS_INVALID_HANDLE)
           {
              SFS_CloseFile(gUnpackInfo->hUnZipFile);
              gUnpackInfo->hUnZipFile = SFS_INVALID_HANDLE;
           }
       }
       SCI_FREE(signal);
       signal = NULL;
       if(*result != UNPACK_FILE_PROCESSING)
       {
          UnpackReleaseMem(result);
          break;
       }
    }
    SCI_ThreadExit();
    unpack_isr_id = SCI_INVALID_BLOCK_ID;
}


/* asynchronous unpack file end, by Tbill 20111110 */

/*****************************************************************************/
//  Description : asyn unpack
//  Global resource dependence : 
//  Author:Tbill
//  Note:
/*****************************************************************************/
#if 1
int unpack(const uint16 *srcfile, uint16 *dstdir)
{
    WRE_SZIP_HEAD_INFO  info  = {0};
    uint32              size  = 0;
    SFS_HANDLE          hFile = PNULL;
    SFS_ERROR_E         err = SFS_NO_ERROR;
    unpack_result_enum  result = UNPACK_FILE_OPERATING_ERROR;
 
    hFile = SFS_CreateFile(srcfile, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(hFile == SFS_INVALID_HANDLE)
    {
        return result;
    }

    err = SFS_ReadFile(hFile, (void *)&info, sizeof(WRE_SZIP_HEAD_INFO), &size, PNULL);
    if((SFS_NO_ERROR != err) || (size != sizeof(WRE_SZIP_HEAD_INFO)))
    {
        err = SFS_CloseFile(hFile);
        err = SFS_DeleteFile(srcfile, SCI_NULL);
        return result;
    }

    if(gUnpackInfo == NULL)
    {
       gUnpackInfo = (asyn_unpack_info_struct *)SCI_ALLOC_APP(sizeof(asyn_unpack_info_struct));
    }
    if(gUnpackInfo == NULL)
    {
        SFS_CloseFile(hFile);
        return result;
    }
    memset(gUnpackInfo, 0, sizeof(asyn_unpack_info_struct));
    gUnpackInfo->databuf = (uint8*)SCI_ALLOC_APP(WRE_UNPACK_BUFFER_SIZE);
    if(PNULL == gUnpackInfo->databuf)
    {
        SCI_FREE(gUnpackInfo);
        SFS_CloseFile(hFile);
        return result;
    }

    result = UNPACK_FILE_PROCESSING;
    unpack_isr_id = SCI_CreateAppThread("T_ASYN_UNPACK_HANDLER",
                                                               "Q_ASYN_UNPACK",
									Asyn_UnpackThread,
									0,
									&result,
									UNPACK_STACK_SIZE,
									UNPACK_QUEUE_NUM,
									SCI_SYSTEM_PRI(28),
									SCI_PREEMPT,
									SCI_AUTO_START);  
   if(SCI_INVALID_BLOCK_ID == unpack_isr_id)
   {
       UnpackReleaseMem(&result);
       return result;
   }

    memset(gUnpackInfo->databuf, 0, WRE_UNPACK_BUFFER_SIZE);
    gUnpackInfo->file_num = htonl(info.file_num);
    gUnpackInfo->offset = sizeof(WRE_SZIP_HEAD_INFO);
    gUnpackInfo->file_handle = hFile;
    MMIAPICOM_Wstrcpy(gUnpackInfo->dstdir, dstdir);
    MMIAPICOM_Wstrcpy(gUnpackInfo->srcfile, srcfile);
    AsyUpackSendSignal(UNPACK_READ_FILE_INFO_SIG);
    while(1) //lint !e716
    {
       SCI_SLEEP(100);
       if(result != UNPACK_FILE_PROCESSING)
       {
          break;
       }
    }
    return result;
}

#else
int unpack(const uint16 *srcfile, uint16 *dstdir)
{
    uint16             *file_name_ptr = NULL;
    WRE_SZIP_HEAD_INFO  info  = {0};
    uint32              size  = 0;
    SFS_HANDLE          hFile = PNULL;
    uint32              offset = 0;
    uint32              i = 0;
    int                 ret = 0;
    SFS_ERROR_E         err = SFS_NO_ERROR;
    uint8*              file_buf = PNULL;

    hFile = SFS_CreateFile(srcfile, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(!hFile)
    {
        return 1;
    }

    err = SFS_ReadFile(hFile, (void *)&info, sizeof(WRE_SZIP_HEAD_INFO), &size, PNULL);
    if((SFS_NO_ERROR != err) || (size != sizeof(WRE_SZIP_HEAD_INFO)))
    {
        /* read file error */
        //SCI_TRACE_LOW:"read main file header hFile=%lx err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1876_112_2_18_3_10_53_123,(uint8*)"d", hFile);
        SFS_CloseFile(hFile);
        return 2;
    }

    file_buf = (uint8*)SCI_ALLOC_APP( WRE_UNPACK_BUFFER_SIZE );
    if(PNULL == file_buf)
    {
        //SCI_TRACE_LOW:"no enough memory room"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1884_112_2_18_3_10_53_124,(uint8*)"");
        SFS_CloseFile(hFile);
        return 3;
    }
    info.file_num = htonl(info.file_num);
    offset = sizeof(WRE_SZIP_HEAD_INFO);

    for (i = 0; i < info.file_num; i++)
    {
        SFS_HANDLE  hUnZipFile = NULL;
        uint8*      temp_name       = NULL;
        uint16*     new_file_name   = NULL;
        uint32      new_file_name_len = 0;
        WRE_SZIP_FILE_INFO file_info = {0};
        uint16*     p = NULL;

        //读文件信息
        err = SFS_SetFilePointer(hFile, (__int64)offset, SFS_SEEK_BEGIN);
        if(SFS_NO_ERROR != err)
        {
            /* set file pointer file error */
            //SCI_TRACE_LOW:"set file pointer error hFile=%lx err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1905_112_2_18_3_10_53_125,(uint8*)"d", hFile);
            ret = 5;
            break;
        }

        err = SFS_ReadFile(hFile, (void *)&file_info, sizeof(WRE_SZIP_FILE_INFO), &size, PNULL);
        if(SFS_NO_ERROR != err)
        {
            /* read file error */
            //SCI_TRACE_LOW:"read main file branch info hFile=%lx err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1914_112_2_18_3_10_53_126,(uint8*)"d", hFile);
            ret = 2;
            break;
        }

        /*change the sequence*/
        file_info.file_name_len = htonl(file_info.file_name_len);
        file_info.file_size = htonl(file_info.file_size);
        file_info.zip_size  = htonl(file_info.zip_size);
        offset += sizeof(WRE_SZIP_FILE_INFO);

        //SCI_TRACE_LOW:"[Szip]:file_name_len = %d, file_size = %d, zip_size = %d, reserved = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1926_112_2_18_3_10_53_127,(uint8*)"dddd",file_info.file_name_len, file_info.file_size, file_info.zip_size, file_info.reserved );
        
        temp_name = (uint8*)SCI_ALLOC_APP( file_info.file_name_len + 1 );
        memset( temp_name, 0, (file_info.file_name_len + 1) );
        new_file_name = (uint16*)SCI_ALLOC_APP( (file_info.file_name_len + 1) * sizeof(wchar_t) );
        memset( new_file_name, 0, ((file_info.file_name_len + 1) * sizeof(wchar_t)) );
        
        //读文件名
        err = SFS_SetFilePointer(hFile, (__int64)offset, SFS_SEEK_BEGIN);
        if(SFS_NO_ERROR != err)
        {
            //SCI_TRACE_LOW:"set file pointer error hFile=%lx err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1937_112_2_18_3_10_53_128,(uint8*)"d", hFile);
            ret = 5;
            goto label;
        }
        err = SFS_ReadFile(hFile, (void *)temp_name, file_info.file_name_len * sizeof(uint8), &size, PNULL);
        if(SFS_NO_ERROR != err)
        {
            //SCI_TRACE_LOW:"read main file branch info hFile=%lx err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1944_112_2_18_3_10_53_129,(uint8*)"d", hFile);
            ret = 2;
            goto label;
        }
        offset += file_info.file_name_len;

        new_file_name_len = GUI_GBToWstr( new_file_name, temp_name, (uint16)file_info.file_name_len );
        file_name_ptr = (uint16 *)SCI_ALLOC_APP( (MAX_PATH +1 ) * sizeof(wchar_t) );
        memset(file_name_ptr, 0, ((MAX_PATH +1 ) * sizeof(wchar_t)) );

        MMIAPICOM_Wstrcpy(file_name_ptr, dstdir);
        MMIAPICOM_Wstrcat(file_name_ptr, L"\\");

        p = (uint16 *)new_file_name;
        if(wre_wcsstr(p, L".\\"))
        {
            p += 2;
        }
        MMIAPICOM_Wstrcat(file_name_ptr, p);

        if(IsFileIdentify(hFile, file_info.file_size, offset, file_name_ptr, file_buf))
        {
            offset += file_info.file_size;
            SCI_FREE(file_name_ptr);
            SCI_FREE( temp_name );
            SCI_FREE( new_file_name );
            continue;
        }

        hUnZipFile = CreateDirFile(file_name_ptr, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL, dstdir);
        SCI_FREE(file_name_ptr);
        if(hUnZipFile )
        {
//            uint8* file_buf   = (uint8*)SCI_ALLOC_APP( WRE_UNPACK_BUFFER_SIZE );
            uint32 file_size  = file_info.file_size;
            
            //读文件内容并写入新文件 ???? effitioncy
            /*
            err = SFS_SetFilePointer(hFile, (__int64)offset, SFS_SEEK_BEGIN);
            */
            while ( 1 )/*lint !e716*/
            {
                err = SFS_SetFilePointer(hFile, (__int64)offset, SFS_SEEK_BEGIN);
                if(SFS_NO_ERROR != err)
                {
                    //SCI_TRACE_LOW:"set file pointer error hFile=%lx err"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_1989_112_2_18_3_10_53_130,(uint8*)"d", hFile);
                    ret = 5;
                    break;
                }

                if( file_size > WRE_UNPACK_BUFFER_SIZE )
                {
                    err = SFS_ReadFile(hFile, (void *)file_buf, WRE_UNPACK_BUFFER_SIZE, &size, PNULL);
                    if(SFS_NO_ERROR == err)
                    {
                        err = SFS_WriteFile(hUnZipFile, (void *)file_buf, WRE_UNPACK_BUFFER_SIZE, &size, PNULL);
                        file_size = file_size - WRE_UNPACK_BUFFER_SIZE;
                        offset += WRE_UNPACK_BUFFER_SIZE;
                    }
                }
                else
                {
                    err = SFS_ReadFile(hFile, (void *)file_buf, file_size, &size, PNULL);
                    if(SFS_NO_ERROR == err)
                    {
                        err = SFS_WriteFile(hUnZipFile, (void *)file_buf, file_size, &size, PNULL);
                        offset += file_size;
                    }
                    break;
                }
            }
//            SCI_FREE( file_buf );
            SFS_CloseFile(hUnZipFile);
        }
        else
        {
            /* create uncompress file failure */
            //SCI_TRACE_LOW:"create branch file hFile=%lx err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2021_112_2_18_3_10_53_131,(uint8*)"d", hFile);
            ret = 4;
        }
label:
        SCI_FREE( temp_name );
        SCI_FREE( new_file_name );
        if( ret )
        {
            break;
        }
    }

    SCI_FREE( file_buf );
    SFS_CloseFile(hFile);
    return ret;
}
#endif
/*****************************************************************************/
//  Description : UnZipcompressfile
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void *wre_Zcalloc(void  *opaque, unsigned  items, unsigned  size)
{
    void *p = PNULL;

    if (opaque) opaque = 0;
    p = SCI_ALLOC_APP(items * size);

    return p;
}

LOCAL void wre_Zcfree(void  *opaque, void  *ptr)
{
    if (opaque) opaque = 0;
    SCI_FREE(ptr);
}
/* wre版本的 zlib DLL(希望<30k)并提供如下接口 */
int UnZipcompressfile(SFS_HANDLE hSrcFile, SFS_HANDLE hDstFile)
{
    int ret;
    unsigned have;
    z_stream strm;
    SFS_ERROR_E err;
    uint32      dwRWNums;
    unsigned char *in;
    unsigned char *out;
    
    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    strm.zalloc  =  wre_Zcalloc;
    strm.zfree   =  wre_Zcfree;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    in  = SCI_ALLOC_APP(MAX_CHUNK_LEN);
    out = SCI_ALLOC_APP(MAX_CHUNK_LEN);
    /* decompress until deflate stream ends or end of file */
    do {
        err = SFS_ReadFile(hSrcFile, in, MAX_CHUNK_LEN, (uint32 *)&strm.avail_in, PNULL);
        if (err != SFS_NO_ERROR) {
            (void)inflateEnd(&strm);
            SCI_FREE(in);
            SCI_FREE(out);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
        
        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = MAX_CHUNK_LEN;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            /*
            // state not clobbered
            assert(ret != Z_STREAM_ERROR);  
            */
            switch (ret) {
            case Z_NEED_DICT:
                //ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                if(ret == Z_NEED_DICT)
                {
                    ret = Z_DATA_ERROR;
                }
                (void)inflateEnd(&strm);
                SCI_FREE(in);
                SCI_FREE(out);
                return ret;
            default:
                break;
            }
            have = MAX_CHUNK_LEN - strm.avail_out;

            err = SFS_WriteFile(hDstFile, (void *)out, have, &dwRWNums, PNULL);
            if((err != SFS_NO_ERROR)|| (dwRWNums != have)) {
                (void)inflateEnd(&strm);
                SCI_FREE(in);
                SCI_FREE(out);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        
        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
    
    /* clean up and return */
    (void)inflateEnd(&strm);

    SCI_FREE(in);
    SCI_FREE(out);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


/*****************************************************************************/
//  Description : unzip
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int unzip(const uint16 *srcfile, uint16 *dstfile)
{
    SFS_HANDLE  fpr;
    SFS_HANDLE  fpw;

    fpr = SFS_CreateFile(srcfile, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(PNULL == fpr)
    {
        //SCI_TRACE_LOW:"unzip CreateFile srcfile failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2155_112_2_18_3_10_53_132,(uint8*)"");
        return 1;
    }
    fpw = SFS_CreateFile(dstfile, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
    if(PNULL == fpw)
    {
        //SCI_TRACE_LOW:"unzip CreateFile dstfile failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2161_112_2_18_3_10_53_133,(uint8*)"");
        SFS_CloseFile(fpr);
        return 1;
    }

    if(UnZipcompressfile(fpr, fpw) != Z_OK)
    {
        /* unzip failure */
        SFS_CloseFile(fpr);
        SFS_CloseFile(fpw);
        //SCI_TRACE_LOW:"unzip UnZipcompressfile failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2171_112_2_18_3_10_53_134,(uint8*)"");
        return 2;
    }

    SFS_CloseFile(fpr);
    SFS_CloseFile(fpw);
    return 0;
}

/************ 动态安装菜单功能 *********/
#define MAX_DYNAMIC_NUM   100
#define MAX_WRE_APPNAME_LEN   32
typedef struct _tagDYNAMIC_APP_ITEM
{
    uint16   guid[50];
    uint16   filename[156];
	uint16   appname[MAX_WRE_APPNAME_LEN+1];
    uint16   iconpath[156];
}DYNAMIC_APP_ITEM,*PDYNAMIC_APP_ITEM;

typedef struct _tagDYNAMIC_TBL_HEADER
{
    uint32              valid_falg;
    uint32              max_app_num; //最大安装的应用个数
    //uint32              current_app_num;  //当前已经安装的最大应用个数
    uint32              dyn_id_lst[MAX_DYNAMIC_NUM];//当前应用id的使用表
    uint32              valid_id_first;//系统指定的有效ID起始值.
    uint32              valid_id_last;//系统指定的有效ID最后值.
}DYNAMIC_TBL_HEADER_,*PDYNAMIC_TBL_HEADER_;

typedef struct _tagDYNAMIC_TBL_HEADER_
{
    uint32              valid_falg;
    uint32              max_app_num; //最大安装的应用个数
    //uint32              current_app_num;  //当前已经安装的最大应用个数
    uint32              dyn_id_lst[MAX_DYNAMIC_NUM];//当前应用id的使用表
    //uint32              valid_id_first;//系统指定的有效ID起始值.
    //uint32              valid_id_last;//系统指定的有效ID最后值.
}DYNAMIC_TBL_HEADER,*PDYNAMIC_TBL_HEADER;

typedef struct _tagDYNAMIC_APP_TBL
{
    uint32              valid_falg;
    uint32              max_app_num; //最大安装的应用个数
    //uint32              cur_app_num;
    uint32              dyn_id_lst[MAX_DYNAMIC_NUM];
    DYNAMIC_APP_ITEM    dyan_app_info[MAX_DYNAMIC_NUM];
}DYNAMIC_APP_TBL,*PDYNAMIC_APP_TBL;

#define    INVALID_ID_FLAGS   0xffffffff
#define    VALID_ID_FLAGS     0x54345687


/*****************************************************************************/
//  Description : GetInstallingId
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PDYNAMIC_TBL_HEADER_   dynamic_tbl_header = NULL;
LOCAL  uint32 GetInstallingId()
{   
    uint32    idx;
    
    if(NULL == dynamic_tbl_header)
    {
        //SCI_TRACE_LOW:"Invalid install id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2235_112_2_18_3_10_53_135,(uint8*)"");
        return INVALID_ID_FLAGS;
    }

    for(idx = 0;idx < MAX_DYNAMIC_NUM;idx++)
    {
        if(dynamic_tbl_header->dyn_id_lst[idx] != VALID_ID_FLAGS)
        {
            return idx;
        }
    }

    //已经装满了
    return INVALID_ID_FLAGS;
}
#ifndef LOWORD
#define LOWORD(l)           ((WORD)(l))   
#endif
void Unicode2Asic(uint16 * strUnicode, uint8 *strAsic, uint32 iNum)
{
    while (iNum--)
    {
       *(strAsic++) = LOWORD(*(strUnicode++));
    }
}

/*****************************************************************************/
//  Description : Find_App_GUID
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
BOOLEAN Find_App_GUID(uint32 appid,  uint8 *guid, uint8 guidNum)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};

    memset(guid, 0, guidNum);
    
    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));

    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile == NULL)
    {
        return FALSE;
    }

    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;
            buf = SCI_ALLOC_APP(len+2);

            ret = SFS_ReadFile(hFile, (void *)buf, len, &size, PNULL);
            
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;
                uint8 iNum = 0;
                uint8 guidAsic[WRE_GUID_NUM] = {0};

                len = appid - dynamic_tbl_header->valid_id_first;

				//SCI_TRACE_LOW:"idx = %d,appid = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2312_112_2_18_3_10_53_136,(uint8*)"dd");

				if(len >= MAX_DYNAMIC_NUM)
				{
					SFS_CloseFile(hFile);
					if(buf)
						SCI_FREE(buf);

					return FALSE;
				}
                iNum = MMIAPICOM_Wstrlen(pDyAppTbl->dyan_app_info[len].guid);
                Unicode2Asic(pDyAppTbl->dyan_app_info[len].guid, guidAsic, iNum);
                iNum = iNum > guidNum?guidNum:iNum;
                strncpy((char *)guid,(const char *)guidAsic, iNum);
            }
        }
    }   
    
    SFS_CloseFile(hFile);
    if(buf)
        SCI_FREE(buf);

    return TRUE;
}

/*****************************************************************************/
//  Description : WriteGUIDToDeleteIni
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
BOOLEAN WriteGUIDToDeleteIni(uint8 *guid, uint8 guidNum)
{
    uint32 len = 0;
    SFS_HANDLE  hFile = PNULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    SFS_ERROR_E error_handle = 0;
    uint32 fileSize = 0;
    uint32 iByteSize = 0;
    uint32 iByteWrite = 0;
    uint32 iByteRead = 0;
    PWRE_UNINSTALLAPPINI  pWreAppIni = NULL;
	uint8 *pWreAppIniTmp = NULL;
	char chNum[10] = {0};

    if (guid == NULL || guidNum == 0)
    {
        return FALSE;
    }

    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));

    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_UNINSTALLAPP_INI_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    //SCI_TRACE_LOW:"DeleteApp: Open Ini File return = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2366_112_2_18_3_10_54_137,(uint8*)"d", hFile);

    if(hFile == NULL)
    {
        hFile = SFS_CreateFile(filepath, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
        //SCI_TRACE_LOW:"DeleteApp: Create Ini File return = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2371_112_2_18_3_10_54_138,(uint8*)"d", hFile);
        if (hFile == NULL)
        {
            return FALSE;
        }
    }
    
    error_handle = SFS_GetFileSize(hFile, &fileSize);
    //SCI_TRACE_LOW:"DeleteApp: GetFileSize handle= %x, fileSize = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2379_112_2_18_3_10_54_139,(uint8*)"dd", hFile, fileSize);
    if (error_handle == SFS_NO_ERROR)
    {
       if (fileSize == 0)
       {
            pWreAppIni = SCI_ALLOC_APP(sizeof(WRE_UNINSTALLAPPINI));
			if (pWreAppIni == NULL)
			{
				//SCI_TRACE_LOW:"DeleteApp: fileSize = 0, malloc WRE_UNINSTALLAPPINI Fail!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2387_112_2_18_3_10_54_140,(uint8*)"");
				SFS_CloseFile(hFile);
				return FALSE;
			}
			memset(pWreAppIni, 0, sizeof(WRE_UNINSTALLAPPINI));

			iByteSize = sizeof(WRE_APPGUID);
            pWreAppIniTmp = SCI_ALLOC_APP(iByteSize);
			if (pWreAppIniTmp == NULL)
			{
				//SCI_TRACE_LOW:"DeleteApp: fileSize = 0, malloc WRE_APPGUID Fail!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2397_112_2_18_3_10_54_141,(uint8*)"");
				SCI_FREE(pWreAppIni);
				SFS_CloseFile(hFile);
				return FALSE;
			}
            pWreAppIni->pWreAppGuid = (PWRE_APPGUID)pWreAppIniTmp;
			memset(pWreAppIni->pWreAppGuid, 0, iByteSize);

            pWreAppIni->iUnInstallAppNum = 1;
            memcpy(pWreAppIni->pWreAppGuid, guid, guidNum);
       }
       else
       {		    
            iByteSize = fileSize+sizeof(WRE_APPGUID);

			pWreAppIni = SCI_ALLOC_APP(sizeof(WRE_UNINSTALLAPPINI));
			if (pWreAppIni == NULL)
			{
				//SCI_TRACE_LOW:"DeleteApp: fileSize != 0, malloc WRE_UNINSTALLAPPINI Fail!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2415_112_2_18_3_10_54_142,(uint8*)"");
				SFS_CloseFile(hFile);
				return FALSE;
			}
			memset(pWreAppIni, 0, sizeof(WRE_UNINSTALLAPPINI));

            pWreAppIniTmp = SCI_ALLOC_APP(iByteSize);
			if (pWreAppIniTmp == NULL)
			{
				//SCI_TRACE_LOW:"DeleteApp: fileSize != 0, malloc WreAppTmp Fail!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2424_112_2_18_3_10_54_143,(uint8*)"");
				SFS_CloseFile(hFile);
				SCI_FREE(pWreAppIni);
				return FALSE;
			}
            memset(pWreAppIniTmp, 0, iByteSize);
            error_handle = SFS_ReadFile(hFile, pWreAppIniTmp, fileSize, &iByteRead, NULL);
            if (error_handle != SFS_NO_ERROR || iByteRead == 0)
            {
                //SCI_TRACE_LOW:"DeleteApp: ReadFile handle= %x, iByteRead = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2433_112_2_18_3_10_54_144,(uint8*)"dd", hFile, iByteRead);
				SFS_CloseFile(hFile);
				SCI_FREE(pWreAppIni);
				SCI_FREE(pWreAppIniTmp);
				return FALSE;
            }
			memcpy(chNum, pWreAppIniTmp, 4);
			pWreAppIni->iUnInstallAppNum = atoi(chNum)+1;
            //pWreAppIni->iUnInstallAppNum = ((PWRE_UNINSTALLAPPINI)pWreAppIniTmp)->iUnInstallAppNum+1;
			pWreAppIni->pWreAppGuid = (PWRE_APPGUID)(pWreAppIniTmp+sizeof(uint32));
            memcpy(&((pWreAppIni->pWreAppGuid)[pWreAppIni->iUnInstallAppNum-1]), guid, guidNum);
			iByteSize -= sizeof(uint32);
       }

	   sprintf(chNum, "%04d", pWreAppIni->iUnInstallAppNum);
	   SFS_SetFilePointer(hFile, (__int64)0, SFS_SEEK_BEGIN);
	   error_handle = SFS_WriteFile(hFile, chNum, sizeof(uint32), &iByteWrite, NULL);
	   if (error_handle != SFS_NO_ERROR)
	   {
		   //SCI_TRACE_LOW:"DeleteApp: WriteFile num return = %x, iByteSize = %d, iByteWrite = %d"
		   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2453_112_2_18_3_10_54_145,(uint8*)"ddd", error_handle, iByteSize, iByteWrite);
	   }
	   //SFS_SetFilePointer(hFile, 0, SFS_SEEK_END);
	   error_handle = SFS_WriteFile(hFile, (void *)(pWreAppIni->pWreAppGuid), iByteSize, &iByteWrite, NULL);
	   if (error_handle != SFS_NO_ERROR)
	   {
		   //SCI_TRACE_LOW:"DeleteApp: WriteFile data return = %x, iByteSize = %d, iByteWrite = %d"
		   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2460_112_2_18_3_10_54_146,(uint8*)"ddd", error_handle, iByteSize, iByteWrite);
	   }

        SCI_FREE(pWreAppIniTmp);
        pWreAppIniTmp = NULL;

        SCI_FREE(pWreAppIni);
        pWreAppIni = NULL;
    }
    
    SFS_CloseFile(hFile);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Find_App_Name
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN  Find_App_Name(uint16 *guid, uint16 *appname)
{
	uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};

    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));
	
    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);
	
    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
	
    if(hFile == NULL)
    {
		WRE_InitDynamicMenu(dynamic_tbl_header->valid_id_first,dynamic_tbl_header->valid_id_last);
		hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
		if(hFile == NULL)
			return FALSE;
    }
	
    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;
            buf = SCI_ALLOC_APP(len+2);
			
            ret = SFS_ReadFile(hFile, (void *)buf, len, &size, PNULL);
            
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;
                uint32   idx;
				
                for(idx = 0;idx < MAX_DYNAMIC_NUM;idx ++)
                {
                    if(pDyAppTbl->dyn_id_lst[idx] == VALID_ID_FLAGS
					  && 0 == MMIAPICOM_Wstrcmp(pDyAppTbl->dyan_app_info[idx].guid,guid))
                    {
						MMIAPICOM_Wstrcpy(appname, pDyAppTbl->dyan_app_info[idx].appname);

                        SFS_CloseFile(hFile);
                        SCI_FREE(buf);
                        return TRUE;
                    }
                }
            }
        }
    }   
    
    SFS_CloseFile(hFile);
    if(buf)
    {
        SCI_FREE(buf);
    }
	
    return FALSE;
}

/*****************************************************************************/
//  Description : Find_App_Id
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32  Find_App_Id(uint16 * guid)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    
    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));
	
    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile == NULL)
    {
		WRE_InitDynamicMenu(dynamic_tbl_header->valid_id_first,dynamic_tbl_header->valid_id_last);

        hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

		if(hFile == NULL)
			return INVALID_ID_FLAGS;
    }

    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;
            buf = SCI_ALLOC_APP(len+2);

            ret = SFS_ReadFile(hFile, (void *)buf, len, &size, PNULL);
            
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;
                uint32   idx;

                for(idx = 0;idx < MAX_DYNAMIC_NUM;idx ++)
                {
                    if(pDyAppTbl->dyn_id_lst[idx] == VALID_ID_FLAGS
                        && 0 == MMIAPICOM_Wstrcmp(pDyAppTbl->dyan_app_info[idx].guid,guid) )
//                      && 0 == MMIAPICOM_Wstrcmp(pDyAppTbl->dyan_app_info[idx].filename,appname))
                    {
                        SFS_CloseFile(hFile);
                        SCI_FREE(buf);
                        return idx;
                    }
                }
            }
        }
    }   
    
    SFS_CloseFile(hFile);
    if(buf)
    {
        SCI_FREE(buf);
    }

    return INVALID_ID_FLAGS;
}
//------------------------------------------------------------------------------------------------
//--- 函数名：FindAppGUIDByName
//--- 说明： 通过应用程序名获取应用程序的id 、GUID、路径、图标、带后缀名
//--- 参数：
//--- appname : 应用程序名字  (in)
//--- appid   : 应用程序 id   (out) 
//--- guid    : 应用程序 GUID (out) 
//--- path    : 应用程序路径  (out)
//--- iconpath: 应用程序图标路径  (out)
//--- apInname: 应用程序名字  (out)
//-------------------------------------------------------------------------------------------------
BOOLEAN  FindAppGUIDByName(uint16 * appname,uint32 * appid,uint16 * guid,uint16 * path,uint16 * iconpath,uint16 * apInname)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};

    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));

    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);
	
    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
	
    if(hFile == NULL)
    {
		WRE_InitDynamicMenu(dynamic_tbl_header->valid_id_first,dynamic_tbl_header->valid_id_last);
		
        hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
		
		if(hFile == NULL)
			return FALSE;
    }
    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;
            buf = SCI_ALLOC_APP(len+2);
			
            ret = SFS_ReadFile(hFile, (void *)buf, len, &size, PNULL);
 
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;
                uint32   idx;

                for(idx = 0;idx < MAX_DYNAMIC_NUM;idx ++)
                {
                    if((pDyAppTbl->dyn_id_lst[idx] == VALID_ID_FLAGS)
						&& (0 == MMIAPICOM_Wstrcmp(pDyAppTbl->dyan_app_info[idx].filename,appname)))
                    {
						*appid = dynamic_tbl_header->valid_id_first + idx;
						MMIAPICOM_Wstrcpy(path,pDyAppTbl->dyan_app_info[idx].filename);
						MMIAPICOM_Wstrcpy(guid,pDyAppTbl->dyan_app_info[idx].guid);
                        MMIAPICOM_Wstrcpy(apInname,pDyAppTbl->dyan_app_info[idx].appname);
                        MMIAPICOM_Wstrcpy(iconpath,pDyAppTbl->dyan_app_info[idx].iconpath);

                        SFS_CloseFile(hFile);
                        SCI_FREE(buf);
                        return TRUE;
                    }
                }
            }
        }
    }   
    
    SFS_CloseFile(hFile);
    if(buf)
    {
        SCI_FREE(buf);
    }
	
    return FALSE;
}

BOOLEAN  FindAppPathByGUID(uint16 * guid,uint16 * path,uint16 * appname,uint16 * iconpath)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    
    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));
	
    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile == NULL)
    {
		WRE_InitDynamicMenu(dynamic_tbl_header->valid_id_first,dynamic_tbl_header->valid_id_last);

        hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

		if(hFile == NULL)
			return FALSE;
    }

    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;
            buf = SCI_ALLOC_APP(len+2);

            ret = SFS_ReadFile(hFile, (void *)buf, len, &size, PNULL);
            
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;
                uint32   idx;

                for(idx = 0;idx < MAX_DYNAMIC_NUM;idx ++)
                {
                    if(pDyAppTbl->dyn_id_lst[idx] == VALID_ID_FLAGS
                        && 0 == MMIAPICOM_Wstrcmp(pDyAppTbl->dyan_app_info[idx].guid,guid) )
//                      && 0 == MMIAPICOM_Wstrcmp(pDyAppTbl->dyan_app_info[idx].filename,appname))
                    {
                    	MMIAPICOM_Wstrcpy(path,pDyAppTbl->dyan_app_info[idx].filename);
                        //MMIAPICOM_Wstrcat(path,L"=>>");
                        //MMIAPICOM_Wstrcat(path,pDyAppTbl->dyan_app_info[idx].appname);
                        MMIAPICOM_Wstrcpy(appname,pDyAppTbl->dyan_app_info[idx].appname);
                        MMIAPICOM_Wstrcpy(iconpath,pDyAppTbl->dyan_app_info[idx].iconpath);
                        
                        SFS_CloseFile(hFile);
                        SCI_FREE(buf);
                        return TRUE;
                    }
                }
            }
        }
    }   
    
    SFS_CloseFile(hFile);
    if(buf)
    {
        SCI_FREE(buf);
    }

    return FALSE;
}

BOOLEAN  FindAppPathByAppID(uint32 appid,uint16 * path,uint16 * appname,uint16 * iconpath)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    int     wre_boot_ret = WRE_INIT_OK;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};

    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));

    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile == NULL)
    {
        return FALSE;
    }

    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;
            buf = SCI_ALLOC_APP(len+2);

            ret = SFS_ReadFile(hFile, (void *)buf, len, &size, PNULL);
            
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;

                len = appid - dynamic_tbl_header->valid_id_first;

				//SCI_TRACE_LOW:"idx = %d,appid = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2749_112_2_18_3_10_54_147,(uint8*)"dd");

				if(len >= MAX_DYNAMIC_NUM)
				{
					SFS_CloseFile(hFile);
					if(buf)
						SCI_FREE(buf);

					return FALSE;
				}

                MMIAPICOM_Wstrcpy(path,pDyAppTbl->dyan_app_info[len].filename);
                MMIAPICOM_Wstrcpy(appname, pDyAppTbl->dyan_app_info[len].appname);
                MMIAPICOM_Wstrcpy(iconpath,pDyAppTbl->dyan_app_info[len].iconpath);
            }
        }
    }   
    
    SFS_CloseFile(hFile);
    if(buf)
        SCI_FREE(buf);

	{
		SFS_HANDLE  sfs_handle = SFS_INVALID_HANDLE;
		SFS_FIND_DATA_T find_data = {0};
#ifdef WIN32
		uint32 nLen= MMIAPICOM_Wstrlen(path);
		uint16 *pSurfix = path;
		uint16 *pDefaultSurfix = L".exe";
		uint32 nLenDefault = MMIAPICOM_Wstrlen(pDefaultSurfix);
		if (nLen > nLenDefault)
		{
		     pSurfix += nLen-nLenDefault+1;
		     *pSurfix = L'd';
		     *(pSurfix+1) = L'l';
		     *(pSurfix+2) = L'l';
#endif
			 sfs_handle = SFS_FindFirstFile((const uint16 *)path, &find_data);
			//SCI_TRACE_LOW:"MYW:filepath = %s, SFS_FindFirstFile return %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_2789_112_2_18_3_10_54_148,(uint8*)"sd", path, sfs_handle);
			
			if (SFS_INVALID_HANDLE == sfs_handle)
			{
				return FALSE;
			}
			else
			{
			   SFS_FindClose(sfs_handle);
			}
#ifdef WIN32
		}
		else
		{
			return FALSE;
		}
#endif		
	}

	return TRUE;
}


/*****************************************************************************/
//  Description : GetIconBuf
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//获得显示图标
LOCAL   uint8 * GetIconBuf(uint16 * filepath,uint32 * filelen)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * iconbuf = NULL;

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile == NULL)
        return iconbuf;

    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len > 15*1024)
        {
            len = 0;
        }
        else
        {
            uint32  size;
            iconbuf = SCI_ALLOC_APP(len+2);
	     if (iconbuf == NULL)
	     	{
	     	       SFS_CloseFile(hFile);
			return NULL;
	     	}
            ret = SFS_ReadFile(hFile, (void *)iconbuf, len, &size, PNULL);
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
                SCI_ASSERT(0);  /*assert verified*/
            }
        }
    }   

    *filelen = len;

    SFS_CloseFile(hFile);

    return iconbuf;
}


/*****************************************************************************/
//  Description : WRE_InitDynamicMenu
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//初始化动态菜单安装表
void WRE_InitDynamicMenu(uint32 firstid,uint32 lastid)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    //uint8 * buf;
    uint16 filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};

    if(dynamic_tbl_header)
    {
        SCI_FREE(dynamic_tbl_header);
        dynamic_tbl_header = NULL;
    }

    memset(filepath, 0, sizeof(filepath));
    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_WREAPP));
    SFS_CreateDirectory(filepath);

    memset(filepath, 0, sizeof(filepath));
    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_WREAPP_SD));
    SFS_CreateDirectory(filepath);

    memset(filepath, 0, sizeof(filepath));
    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));
    SFS_CreateDirectory(filepath);
    

    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile != NULL )
    {
        SFS_GetFileSize(hFile,&len);
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            //
        }
        else
        {
            uint32 size;
            len = sizeof(DYNAMIC_TBL_HEADER);
            dynamic_tbl_header = SCI_ALLOC_APP(sizeof(DYNAMIC_TBL_HEADER_));
            ret = SFS_ReadFile(hFile, (void *)(dynamic_tbl_header), len, &size, PNULL);
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
                SCI_ASSERT(0);  /*assert verified*/
            }
            goto OPER_END;
        }
    }

    //reset file
    if(hFile != NULL)
        SFS_CloseFile(hFile);


	if(dynamic_tbl_header)
		SCI_FREE(dynamic_tbl_header);
	
    hFile = SFS_CreateFile(filepath, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
    dynamic_tbl_header = SCI_ALLOC_APP(sizeof(DYNAMIC_APP_TBL));
    dynamic_tbl_header->max_app_num = 0;
    SCI_MEMSET(dynamic_tbl_header,0,sizeof(DYNAMIC_APP_TBL));
    
    dynamic_tbl_header->valid_falg = VALID_ID_FLAGS;
    
    SFS_WriteFile(hFile,dynamic_tbl_header,sizeof(DYNAMIC_APP_TBL),&len,PNULL);

	{
		uint8 * ptr = SCI_ALLOC_APP(sizeof(DYNAMIC_TBL_HEADER_));

		SCI_MEMCPY(ptr,dynamic_tbl_header,sizeof(DYNAMIC_TBL_HEADER_));

		SCI_FREE(dynamic_tbl_header);

		dynamic_tbl_header = (PDYNAMIC_TBL_HEADER_)ptr;
	}

	
OPER_END:
    dynamic_tbl_header->valid_id_first = firstid;
    dynamic_tbl_header->valid_id_last  = lastid;

    SFS_CloseFile(hFile);
    //SCI_FREE(buf);

    return;
}

/*****************************************************************************/
//  Description : RefreshInstallInfo
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//更新安装信息
BOOLEAN RefreshInstallInfo(uint16 * guid,uint16 * apppath, uint16 *appname, uint16 * iconpath,uint32 appid,BOOLEAN bAdd)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16 filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    if(appid>=MAX_DYNAMIC_NUM) return FALSE;
    dynamic_tbl_header->dyn_id_lst[appid] = VALID_ID_FLAGS;

    MMIAPICOM_Wstrcpy(filepath, WRE_Get_SYS_Dir(WRE_SYS_PATH_REGISTER));
    
    len = MMIAPICOM_Wstrlen(filepath);
    MMIAPICOM_Wstrcat(filepath,WRE_APPREGTBL_NAME);

    hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);

    if(hFile == NULL)
    {
        //SCI_ASSERT(0);
		WRE_InitDynamicMenu(dynamic_tbl_header->valid_id_first,dynamic_tbl_header->valid_id_last);
		hFile = SFS_CreateFile(filepath, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    }

    if(SFS_NO_ERROR == SFS_GetFileSize(hFile,&len))
    {
        if(len != sizeof(DYNAMIC_APP_TBL))
        {
            len = 0;
        }
        else
        {
            uint32  size;

            buf = SCI_ALLOC_APP(len+2);
            size = sizeof(DYNAMIC_TBL_HEADER);
            SFS_SetFilePointer(hFile, (__int64)size, SFS_SEEK_BEGIN);
            len -= size;


            ret = SFS_ReadFile(hFile, (void *)(buf+size), len, &size, PNULL);
            if(ret != SFS_NO_ERROR || len != size)
            {
                len = 0;
                SCI_ASSERT(0);  /*assert verified*/
            }
            else
            {
                PDYNAMIC_APP_TBL   pDyAppTbl = (PDYNAMIC_APP_TBL)buf;

                SCI_MEMCPY((void*)buf,(void*)dynamic_tbl_header,sizeof(DYNAMIC_TBL_HEADER));
                
                if(bAdd)
                {
                    pDyAppTbl->valid_falg = VALID_ID_FLAGS;
                    pDyAppTbl->dyn_id_lst[appid] = VALID_ID_FLAGS;
                    MMIAPICOM_Wstrcpy(pDyAppTbl->dyan_app_info[appid].filename,apppath);
                    MMIAPICOM_Wstrcpy(pDyAppTbl->dyan_app_info[appid].guid,guid);
					MMIAPICOM_Wstrcpy(pDyAppTbl->dyan_app_info[appid].appname,appname);
					MMIAPICOM_Wstrcpy(pDyAppTbl->dyan_app_info[appid].iconpath,iconpath);
                    
                    pDyAppTbl->max_app_num++;
                }
                else
                {
                    pDyAppTbl->valid_falg = VALID_ID_FLAGS;
                    pDyAppTbl->dyn_id_lst[appid] = INVALID_ID_FLAGS;
                    //MMIAPICOM_Wstrcpy(pDyAppTbl->dyan_app_info[appid].filename,apppath);
                    //MMIAPICOM_Wstrcpy(pDyAppTbl->dyan_app_info[appid].guid,guid);                 
                    pDyAppTbl->max_app_num--;
                }

                SFS_SetFilePointer(hFile, (__int64)0, SFS_SEEK_BEGIN); 
                ret = SFS_WriteFile(hFile, (void *)buf, sizeof(DYNAMIC_APP_TBL), &size, PNULL);

                if(ret == SFS_NO_ERROR)
                {
                    SCI_MEMCPY((void*)dynamic_tbl_header,(void*)buf,sizeof(DYNAMIC_TBL_HEADER));
                    SFS_CloseFile(hFile);
                    SCI_FREE(buf);
                    return TRUE;
                }
            }
        }
    }   
    if(hFile)
    {
        SFS_CloseFile(hFile);
    }
    if(buf)
    {
        SCI_FREE(buf);
    }
    return FALSE;
}


/*****************************************************************************/
//  Description : Adapt_InstallApp
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
//程序安装
BOOLEAN Adapt_InstallApp(uint16 * appname,
                      uint16 * guid,
                      uint16 * apppath,
                      void *   appparam,
                      uint16 * iconpath)
{
    uint32      app_id;
    uint8     * iconbuf;
    uint32      icon_buf_len;
    BOOLEAN     ret;


    app_id = Find_App_Id(guid);
    if(app_id != INVALID_ID_FLAGS)
    {
        Adapt_UninstallApp(guid,apppath);
    }

    //首先获得当前安装的WRE应用guid对于那个的应用ID,避免重复安装同一个ID
    app_id = GetInstallingId();

    if(app_id == INVALID_ID_FLAGS)
        return FALSE;

    //读取图标
    iconbuf = GetIconBuf(iconpath,&icon_buf_len);    
    if(NULL == iconbuf)
        return FALSE;

	ret = MMIWRE_IsFixApp(guid);

	if(FALSE == ret)//not fixed app
	{
		//所有安装的应用的ID都是机遇应用许可的ID范围
		ret = WRE_InstalleApp(app_id+dynamic_tbl_header->valid_id_first,
		                  appname,
		                  (uint *)iconbuf,
		                  icon_buf_len,
		                  apppath);
        //SCI_TRACE_LOW:"WRE_InstallApp: return %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3108_112_2_18_3_10_55_149,(uint8*)"d", ret);
        if (!ret)
        {
           	uint8 *defaultIconbuf = (uint8 *)MMI_GetLabelImage(IMAGE_WRE_INSTALL_ICON, GetMwinMocorWinHandle(), &icon_buf_len);
		    ret = WRE_InstalleApp(app_id+dynamic_tbl_header->valid_id_first,
		                  appname,
		                  (uint *)defaultIconbuf,
  		                  icon_buf_len,
  		                  apppath);
            //SCI_TRACE_LOW:"WRE_InstallApp: user ICON Default return %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3117_112_2_18_3_10_55_150,(uint8*)"d", ret);
        }
	}


    SCI_FREE(iconbuf);

	if(TRUE == ret) //将程序数据写入到系统文件中
		ret  = RefreshInstallInfo(guid,apppath,appname,iconpath,app_id,TRUE);

    return ret;
}


/*****************************************************************************/
//  Description : Adapt_UninstallApp
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//程序卸载
BOOLEAN  Adapt_UninstallApp(uint16 * guid,uint16 * apppath)
{
    uint32   app_id;
	uint16   appname[MAX_WRE_APPNAME_LEN+1] = {0};
    BOOLEAN  ret;

    //寻找对应的应用程序ID
    app_id = Find_App_Id(guid);
	if (!Find_App_Name(guid, appname))
	{
		//SCI_TRACE_LOW:"[WRE]: UninstallAPP--Find appName fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3147_112_2_18_3_10_55_151,(uint8*)"");
		return FALSE;
	}


	ret = MMIWRE_IsFixApp(guid);

	if(FALSE == ret)//not fixed app
	{
		//卸载应用程序
		ret = WRE_UninstallApp(app_id+dynamic_tbl_header->valid_id_first, appname);
		//SCI_TRACE_LOW:"WRE_UninstallApp %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3158_112_2_18_3_10_55_152,(uint8*)"d",ret);
	}
    ret = RefreshInstallInfo(guid,apppath,appname,NULL,app_id,FALSE);

    //SCI_TRACE_LOW:"RefreshInstallInfo %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3162_112_2_18_3_10_55_153,(uint8*)"d",ret);
    return ret; 
}


BOOLEAN Adapt_InstallJavaApp(uint16 * jarpathname,void *pCallbackFunc,void *pParam)
{
#if 0
//#ifdef JAVA_SUPPORT
    if(NULL == jarpathname||NULL == pCallbackFunc)
    {
        return FALSE;
    }
    g_pInStallJavaFunc = (INSTALLJAVACALLBACKFUNC *)pCallbackFunc;
    g_pSaveParam  = pParam;    
    MMIAPIJAVA_Install(jarpathname,MMIAPICOM_Wstrlen(jarpathname));
#endif
    return TRUE;
}

BOOLEAN Adapt_StartupJavaApp(int32 suiteId)
{
#if 0
//#ifdef JAVA_SUPPORT
    Jbed_nAms_intialize(FALSE);
    Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);
    MMIJAVA_suite_openOrLaunch(NULL, suiteId);
#endif
    return TRUE;
} 

BOOLEAN Adapt_UninstallJavaApp(int32 suiteId,BOOLEAN bOnlyDyDelete)
{
#if 0
//#ifdef JAVA_SUPPORT
    if(bOnlyDyDelete)
    {
        Jbed_nAms_intialize(FALSE);
        Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);
        MMIJAVA_DeleteSuiteFromMainmenu(suiteId);
    }
    else
    {
        Jbed_nAms_intialize(FALSE);
        Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);
        MMIJava_OptionMenu_Delete(suiteId);
    }
#endif
    return TRUE;
}


/*****************************************************************************/
//  Description : WRE_App_Startup_With_Data
//  Global resource dependence : 
//  Author:langbiao.tan
//  Note:
/*****************************************************************************/
BOOLEAN WRE_App_Startup_With_Data(uint32 appid, uint16 *guid, void *data)
{
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    uint16  appfilename[MAX_WRE_APPNAME_LEN+1] = {0};
    uint16  iconpath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    int     wre_boot_ret = WRE_INIT_OK;

    if(MMIWRE_GetFixAppPath(appid, filepath, appfilename, iconpath))
    {
       //启动应用程序
       MMIWRE_SetAppParam(filepath,appfilename,iconpath);
       wre_boot_ret = WREEntry(3, data);   /* 3 : TYPE_WRE_DATA_APP */
       if (wre_boot_ret != WRE_INIT_OK && wre_boot_ret != WRE_INIT_RUNNING)
       {
#ifdef WRE_VIRTUALDISK_SUPPORT	
		WRE_DestoryVirtualDisk();
#endif
       }	   
       if(wre_boot_ret == WRE_INIT_NO_ROOM)
       {
           MMIPUB_OpenAlertWinByTextIdEx(SPRD_WRE_APPLET_ID, PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
           return FALSE;
       }
       return TRUE;
    }

    WRE_ShowWarningBox();
    return FALSE;
}

/*****************************************************************************/
//  Description : WRE_DYN_Startup
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void WRE_DYN_Startup(uint32  appid, uint16 *appname)
{
    uint32  len = 0;
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;
    uint8 * buf = NULL;
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    uint16  appfilename[MAX_WRE_APPNAME_LEN+1] = {0};
    uint16  iconpath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    int     wre_boot_ret = WRE_INIT_OK;

	if(FALSE == WRE_CheckUDiskRunState())
		return;

    if (FindAppPathByAppID(appid, filepath, appfilename, iconpath))
    {
	    //启动应用程序
	    MMIWRE_SetAppParam(filepath,appfilename,iconpath);
	    wre_boot_ret = WREEntry(1,filepath);
	    //SCI_TRACE_LOW:"[WRE]   WREEntry return 0x%x"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3270_112_2_18_3_10_55_154,(uint8*)"d", ret);
           if (wre_boot_ret != WRE_INIT_OK && wre_boot_ret != WRE_INIT_RUNNING)
	    {
#ifdef WRE_VIRTUALDISK_SUPPORT	
		WRE_DestoryVirtualDisk();
#endif
	    }	   		
	    if(wre_boot_ret == WRE_INIT_NO_ROOM)
	    {
	        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
	    }
    }
    else
    {
        WRE_ShowWarningBox();
    }

    return;
}

/************ 动态安装菜单功能 *********/

/* register the RTC time message */
static MMI_RESULT_E WRE_HandleRegMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
#if 0
extern int WRE_RegEventNotify(uint16 msg_id);
#endif // delete by liang.chen, 2011-09-02


/*****************************************************************************/
//  Description : Wre_InitRtcEvent
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void Wre_InitRtcEvent(void)
{
    g_wre_app.ProcessMsg = WRE_HandleRegMsg;
}

/*****************************************************************************/
//  Description : WRE_HandleRegMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

static MMI_RESULT_E WRE_HandleRegMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E    recode  = MMI_RESULT_FALSE;
    WORD            wre_msg_id = 0;

    switch(msg_id)
    {
        case MISC_MSG_RTC_MIN:
            wre_msg_id = WRE_MSG_RTC_MIN;
            break;
        case MISC_MSG_RTC_HOUR:
            wre_msg_id = WRE_MSG_RTC_HOUR;
            break;
        case CHR_MSG_WARNING_IND:
        case CHR_MSG_SHUTDOWN_IND:
            wre_msg_id = WRE_MSG_CHR_WARNING;
            break;
        default:
            if((msg_id >= MMI_BT_ME_ON_CNF) && (msg_id <= MMI_BT_OPS_CONNECT_REQ))
            {
                //SCI_TRACE_LOW:"blue tooth event, event no %lx"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3332_112_2_18_3_10_56_155,(uint8*)"d", msg_id);
                wre_msg_id = msg_id - MMI_BT_ME_ON_CNF + WRE_BT_ME_ON_CNF;
            }
            break;
    }
    recode = WBOOT_WRE_RegEventNotify(wre_msg_id);
    return MMI_RESULT_FALSE;
}

/*****************************************************************************/
//  Description : WRE_GetTpStatus
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

WRE_TP_STATUS_E WRE_GetTpStatus(uint16 *px, uint16 *py)
{
    GUI_POINT_T     tppoint = {0, 0};
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    WRE_TP_STATUS_E wre_tp_status = WRE_TP_NONE;

#ifdef TOUCH_PANEL_SUPPORT
    MMK_GetMultiTPMsg(&tp_status, &tppoint);
#endif
    *px = tppoint.x;
    *py = tppoint.y;

    if(tp_status == MMI_TP_NONE)
    {
        wre_tp_status = WRE_TP_NONE;
    }
    else if(tp_status == MMI_TP_DOWN)
    {
        wre_tp_status = WRE_TP_DOWN;
    }
    else if(tp_status == MMI_TP_UP)
    {
        wre_tp_status = WRE_TP_UP;
    }
    else if(tp_status == MMI_TP_MOVE)
    {
        wre_tp_status = WRE_TP_MOVE;
    }
    else if(tp_status == MMI_TP_LONG)
    {
        wre_tp_status = WRE_TP_LONG;
    }
    else if(tp_status == MMI_TP_GESTURE)
    {
        wre_tp_status = WRE_TP_GESTURE;
    }

    return wre_tp_status;
}   

/*****************************************************************************/
//  Description : HideWreDirectroy
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void HideWreDirectroy(BOOLEAN bHide)
{
	int i = WRE_SYS_PATH_KERNEL;
	for (; i <= WRE_SYS_PATH_WREAPP_SD; i++)
	{
	    BOOL bFoldExist = FALSE;
		const uint16 *pSysPath = WRE_Get_SYS_Dir(i);
		if (pSysPath == NULL)
		{
			continue;
		}
        bFoldExist = MMIAPIFMM_IsFolderExist(pSysPath, MMIAPICOM_Wstrlen(pSysPath));
        if (!bFoldExist)
        {
            SFS_CreateDirectory(pSysPath);
        }
		{
            uint16 dwAttri = 0;
			if (SFS_NO_ERROR == SFS_GetAttr(pSysPath, &dwAttri))
			{
				DWORD dwRes = 0;
				dwAttri &= (~SFS_ATTR_DIR);
				if(bHide)
				{
					dwAttri |= SFS_ATTR_HIDDEN;
					dwRes = SFS_SetAttr(pSysPath,  dwAttri);
				}
				else
				{
					dwAttri &= (~SFS_ATTR_HIDDEN);  
					dwRes = SFS_SetAttr(pSysPath,  dwAttri);
				}
				
				//SFS_GetAttr(WreSysDir[i], &dwAttri);
				
				//SCI_TraceLow:"Set WreSysDir[%d] dir to Hide return %d,  dwAttri = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3424_112_2_18_3_10_56_156,(uint8*)"ddd", i, dwRes, dwAttri);
			}
      	}
	}
}

/*****************************************************************************/
//  Description : WRE_Init
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC void WRE_Init(void)
{
    WRE_InitDynamicMenu(1,MAX_DYNAMIC_NUM);
    Wre_InitRtcEvent();
    HideWreDirectroy(TRUE);
    //MMIWRE_DeleteInitFile(ONEKEY_DOWNLOAD_STEP_PART1);
    MMIWRE_ExitDebugMode();
    MMIWRE_InitRuntimePath();
#ifndef WIN32
	MMIWRE_InitDownLoad(TRUE);
#endif
 MMIWRE_InitSideKeyVolume();//for sidekey

#ifdef SNS_PULLING_SUPPORT
            // Read NV and set to s_time_cb when widget is opened
            MMISNS_LoadWidgetTimerSetting(MMI_SNS_FACEBOOK, &s_timer_preset_type[MMI_SNS_FACEBOOK]); 
            
            if (MMI_SNS_TIMER_PRESET_OFF != s_timer_preset_type[MMI_SNS_FACEBOOK])
            {
                uint16 timer_preset_type = s_timer_preset_type[MMI_SNS_FACEBOOK];
                MMIWRE_SetSnsPullingTimer(MMI_SNS_FACEBOOK, s_timer_time_tbl[timer_preset_type]);
                MMIWRE_StartSnsPullingTimer(MMI_SNS_FACEBOOK);
            }
#endif //SNS_PULLING_SUPPORT
}

PUBLIC void WRE_Reset()
{
    MMIWRE_ResetRuntimePath();
}

int WRE_Get_Gesture(void *ptr)
{
    MMI_GESTURE_TYPE_E gesture_type;
    int ret = 0;

    gesture_type = MMK_GET_TP_GESTURE(ptr);
    switch(gesture_type)
    {
    case MMI_GESTURE_ZOOM_IN:
        ret = WRE_GID_ZOOM_IN;
        break;
    case MMI_GESTURE_ZOOM_OUT:
        ret = WRE_GID_ZOOM_OUT;
        break;
    //other gesture not be supported at present
    default:
        ret = 0;
    }
    return ret;
}


PUBLIC void WRE_GetLogicTPMsg(WRE_TP_STATUS_E *tp_status_ptr, WRE_POINT_T *point_ptr)
{
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    GUI_POINT_T     tp_point  = {0};

#ifdef TOUCH_PANEL_SUPPORT
    MMK_GetLogicTPMsg(&tp_status, &tp_point);
#endif

    switch(tp_status)
    {
        case MMI_TP_NONE:
            *tp_status_ptr = WRE_TP_NONE;
            break;
        case MMI_TP_DOWN:
            *tp_status_ptr = WRE_TP_DOWN;
            break;
        case MMI_TP_UP:
            *tp_status_ptr = WRE_TP_UP;
            break;
        case MMI_TP_MOVE:
            *tp_status_ptr = WRE_TP_MOVE;
            break;
        case MMI_TP_LONG:
            *tp_status_ptr = WRE_TP_LONG;
            break;
        case MMI_TP_GESTURE:
            *tp_status_ptr = WRE_TP_GESTURE;
            break;
        default:
            *tp_status_ptr = WRE_TP_NONE;
            break;
    }
    point_ptr->x = tp_point.x;
    point_ptr->y = tp_point.y;
}

PUBLIC BOOLEAN WRE_GetTpPoints(WRE_TP_POINTS_T* points_ptr)
{
    uint16  i;
    BOOLEAN result = FALSE;
    MMI_TP_POINTS_T tp_points = {0};

#ifdef TOUCH_PANEL_SUPPORT 
    result = MMK_GetTpPoints(&tp_points);
#endif

    points_ptr->num = tp_points.num;
    for(i = 0; i < tp_points.num; i++)
    {
        points_ptr->points[i].x = tp_points.points[i].x;
        points_ptr->points[i].y = tp_points.points[i].y;
    }
    return result;
}

PUBLIC int WRE_GetMultiKeyMsg(WRE_KEYS_T *multi_key_msg_ptr)
{
    MMI_MESSAGE_ID_E    multi_key_msg[MMI_MAX_MULTI_KEYS]={0};
    int  i, numkeys = 0;
  
    if(multi_key_msg_ptr == NULL)return FALSE;

    numkeys = MMK_GetMultiKeyMsg(multi_key_msg, KEY_NONE);
    //SCI_TRACE_LOW:"WRE_GetMultiKeyMsg, numkeys %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3530_112_2_18_3_10_56_157,(uint8*)"d", numkeys);
    for(i = 0; i < numkeys; i++)
    {
        multi_key_msg_ptr[i].keycode   = (uint16)WRE_ConvertMocorKeyToWreKey((uint32)(multi_key_msg[i] & 0xff));
        //SCI_TRACE_LOW:"WRE_GetMultiKeyMsg, keycode %lx"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3534_112_2_18_3_10_56_158,(uint8*)"d", multi_key_msg[i]);
        if((multi_key_msg[i] & 0xff00) == MSG_KEYDOWN)
        {
            multi_key_msg_ptr[i].keystatus = WRE_KEY_PRESSED;
        }
        else
        {
            // only MSG_KEYLONG
            multi_key_msg_ptr[i].keystatus = WRE_KEY_LONG_PRESSED;
        }
    }

    return numkeys;
}

PUBLIC BOOLEAN Adapt_IsBacklightOn(void)
{
    return MMIDEFAULT_IsBacklightOn();
}

void MMIWRE_InitDebugMode(void)
{
    SFS_HANDLE  hFile = PNULL;
    SFS_ERROR_E ret   = SFS_NO_ERROR;

    memset(g_strDebugParam, 0, sizeof(g_strDebugParam));

    hFile = SFS_CreateFile(L"e:\\wre_debug_mode.dat", SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(!hFile)
    {
        MMIWRE_SetDebugMode(FALSE);
    }
    else
    {
        uint32 fileSize = 0;
        MMIWRE_SetDebugMode(TRUE);
        
        ret = SFS_GetFileSize(hFile, &fileSize);
        if (SFS_NO_ERROR == ret)
        {
            uint32 uToReadByte = fileSize>WRE_DEBUG_TOTAL?WRE_DEBUG_TOTAL:fileSize;
            uint32 uReadedByte = 0;
            ret = SFS_ReadFile(hFile, g_strDebugParam, uToReadByte, &uReadedByte, NULL);

            //SCI_TRACE_LOW:"MMIWRE_InitDebugMode: read debug file (uReadedByte = %d)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3578_112_2_18_3_10_56_159,(uint8*)"d", uReadedByte);
        }

        SFS_CloseFile(hFile);
    }

}

void MMIWRE_ExitDebugMode(void)
{
    SFS_DeleteFile(L"e:\\wre_debug_mode.dat", NULL);
}    

BOOLEAN MMIWRE_GetDebugMode(void)
{
    return g_bDebugMode;
}

void MMIWRE_SetDebugMode(BOOLEAN bDebugMode)
{
    g_bDebugMode = bDebugMode;
}

BOOLEAN Adapt_GetDebugInfo(void *debugParam)
{
    return g_bDebugMode;
}

void MMIWRE_InitRuntimePath(void)
{
    //SCI_TRACE_LOW:"[wre]:MMIWRE_InitRuntimePath---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3608_112_2_18_3_10_56_160,(uint8*)"");
    
    MMIWRE_ReadNV(MMIWRE_NV_RUNTIME_PATH);
}  

extern const uint16 *s_wre_sys_path[WRE_SYS_PATH_TOTAL];	// changed by yifei

PUBLIC void MMIWRE_PrintNVInfo()
{
    //SCI_TRACE_LOW:"[wre]:MMIWRE_PrintNVInfo---begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3617_112_2_18_3_10_56_161,(uint8*)"");

    //SCI_TRACE_LOW:"current Path"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3619_112_2_18_3_10_56_162,(uint8*)"");
    WRE_TraceWstr(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_CUR]);
    WRE_TraceWstr(g_wre_Runtime_Path.store_path[RUNTIMEPATH_CUR]);
    WRE_TraceWstr(g_wre_Runtime_Path.app_path[RUNTIMEPATH_CUR]);

    //SCI_TRACE_LOW:"previous Path"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3624_112_2_18_3_10_56_163,(uint8*)"");
    WRE_TraceWstr(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_PREV]);
    WRE_TraceWstr(g_wre_Runtime_Path.store_path[RUNTIMEPATH_PREV]);
    WRE_TraceWstr(g_wre_Runtime_Path.app_path[RUNTIMEPATH_PREV]);

    //SCI_TRACE_LOW:"preload Path"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3629_112_2_18_3_10_56_164,(uint8*)"");
#if defined(WRE_WREKERNEL_PRELOAD)||defined(WRE_WREKERNEL_NORFLASH)
    WRE_TraceWstr(WRE_WREKERNEL_PRELOAD_PATH);
    //SCI_TRACE_LOW:"type = %lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3632_112_2_18_3_10_56_165,(uint8*)"d", WRE_WREKERNEL_PRELOAD_TYPE);
#endif
#ifdef WRE_WRESTORE_PRELOAD
    WRE_TraceWstr(WRE_WRESTORE_PRELOAD_PATH);
    //SCI_TRACE_LOW:"type = %lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3636_112_2_18_3_10_56_166,(uint8*)"d", WRE_WRESTORE_PRELOAD_TYPE);
#endif
#ifdef WRE_WREAPP_PRELOAD
    WRE_TraceWstr(WRE_WREAPP_PRELOAD_PATH);
    //SCI_TRACE_LOW:"type = %lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3640_112_2_18_3_10_56_167,(uint8*)"d", WRE_WREAPP_PRELOAD_TYPE);
#endif

    //SCI_TRACE_LOW:"[wre]:MMIWRE_PrintNVInfo---end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3643_112_2_18_3_10_56_168,(uint8*)"");
}

void MMIWRE_ResetRuntimePath()
{
    //SCI_TRACE_LOW:"[wre]:MMIWRE_ResetRuntimePath---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3648_112_2_18_3_10_56_169,(uint8*)"");
    
    SCI_MEMSET(&g_wre_Runtime_Path, 0, sizeof(g_wre_Runtime_Path));
    MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_CUR], s_wre_sys_path[WRE_SYS_PATH_KERNEL]);
    MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.store_path[RUNTIMEPATH_CUR], s_wre_sys_path[WRE_SYS_PATH_STORE]);
    MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.app_path[RUNTIMEPATH_CUR], s_wre_sys_path[WRE_SYS_PATH_WREAPP]);

    MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_PREV], s_wre_sys_path[WRE_SYS_PATH_KERNEL]);
    MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.store_path[RUNTIMEPATH_PREV], s_wre_sys_path[WRE_SYS_PATH_STORE]);
    MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.app_path[RUNTIMEPATH_PREV], s_wre_sys_path[WRE_SYS_PATH_WREAPP]);

    g_wre_Runtime_Path.runtime_flag = 0;

    MMIWRE_WriteNV(MMIWRE_NV_RUNTIME_PATH);
}

PUBLIC void MMIWRE_ReadNV(MMIWRE_NV_ITEM_E nv_item)
{
    MN_RETURN_RESULT_E    ret_value = MN_RETURN_FAILURE;
    //SCI_TRACE_LOW:"[wre]:MMIWRE_ReadNV---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3667_112_2_18_3_10_56_170,(uint8*)"");
    
    switch(nv_item)		   	
    {
        case MMIWRE_NV_RUNTIME_PATH:
        {
#ifndef WRE_VIRTUALDISK_SUPPORT                        
            MMINV_READ(nv_item, &g_wre_Runtime_Path, ret_value);
#else
            ret_value = MN_RETURN_FAILURE;
#endif                
			//if (MN_RETURN_SUCCESS != ret_value)	// changed by yifei
            {
                SCI_MEMSET(&g_wre_Runtime_Path, 0, sizeof(g_wre_Runtime_Path));
                MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_CUR], s_wre_sys_path[WRE_SYS_PATH_KERNEL]);
                MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.store_path[RUNTIMEPATH_CUR], s_wre_sys_path[WRE_SYS_PATH_STORE]);
                MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.app_path[RUNTIMEPATH_CUR], s_wre_sys_path[WRE_SYS_PATH_WREAPP]);
                MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_PREV], s_wre_sys_path[WRE_SYS_PATH_KERNEL]);
                MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.store_path[RUNTIMEPATH_PREV], s_wre_sys_path[WRE_SYS_PATH_STORE]);
                MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.app_path[RUNTIMEPATH_PREV], s_wre_sys_path[WRE_SYS_PATH_WREAPP]);

                g_wre_Runtime_Path.runtime_flag = 0;
                
#ifndef WRE_VIRTUALDISK_SUPPORT                
                MMINV_WRITE(nv_item, &g_wre_Runtime_Path);
#endif
            }
            break;			            
        }
        default:
        {
            break;			            
        }
    }   

}

PUBLIC void MMIWRE_WriteNV(MMIWRE_NV_ITEM_E nv_item)
{
    MN_RETURN_RESULT_E    ret_value = MN_RETURN_FAILURE;
    //SCI_TRACE_LOW:"[wre]:MMIWRE_WriteNV---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3702_112_2_18_3_10_56_171,(uint8*)"");

    switch(nv_item)
    {
        case MMIWRE_NV_RUNTIME_PATH:
        {
#ifndef WRE_VIRTUALDISK_SUPPORT            
            MMINV_WRITE(nv_item, &g_wre_Runtime_Path);
#endif
            break;			            
        }
        default:
        {
            break;
        }
    }   

}

BOOLEAN RestoreWREEnv()
{
    SFS_HANDLE  fp = PNULL;
    SFS_ERROR_E fError = SFS_NO_ERROR;
    wchar       strBuff[32] = {0};
    wchar       strDest[64] = {0};
    wchar       strSour[64] = {0};
    PRELOAD_INFO preload_info = {0};
    wchar       *lpUpdateKernel = NULL;
    wchar       *lpUpdateStore = NULL;
    
    if (WRE_GetFeatureSupport(WRE_FEATURE_KERNEL_PRELOAD))
    {
        if (!RestoreFiles(WRE_SYS_PATH_KERNEL))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: kernel fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3734_112_2_18_3_10_56_172,(uint8*)"");
            return FALSE;
        }        
    }

    if (WRE_GetFeatureSupport(WRE_FEATURE_STORE_PRELOAD))
    {        
        if (!RestoreFiles(WRE_SYS_PATH_STORE))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: store fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3743_112_2_18_3_10_56_173,(uint8*)"");
            return FALSE;
        }        
    }

    return TRUE;
}

BOOLEAN RestoreFiles(WRE_SYS_PATH_TYPE sysPathType)
{
    SFS_HANDLE  fp = PNULL;
    SFS_ERROR_E fError = SFS_NO_ERROR;
    wchar       strBuff[32] = {0};
    wchar       strDest[64] = {0};
    wchar       strSour[64] = {0};
    PRELOAD_INFO preload_info = {0};
    wchar       *lpUpdatePath = NULL;
    wchar       *lpFileName = NULL;

    //SCI_TRACE_LOW:"[WRE]RestoreFiles: sysPathType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3762_112_2_18_3_10_56_174,(uint8*)"d", sysPathType);

    if (sysPathType == WRE_SYS_PATH_KERNEL)
    {
        lpUpdatePath = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);
        if (!WRE_Get_Preload_Dir(&preload_info, WRE_SYS_PATH_KERNEL))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreFiles: (kernel)preload path not found!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3769_112_2_18_3_10_56_175,(uint8*)"");
            
            return FALSE;
        }
        if (preload_info.PreloadType&WRE_PRELOAD_SZIP)
        {
            lpFileName = WRE_SYS_FILE_NAME_KERNELL_SZIP;
        }
        else if ((preload_info.PreloadType&WRE_PRELOAD_WPK))
        {
            lpFileName = WRE_SYS_FILE_NAME_KERNELL_WPK;
        }
        else
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: (kernel)PreloadType is WRE_PRELOAD_EXE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3783_112_2_18_3_10_56_176,(uint8*)"");
        }
    }
    else if (sysPathType == WRE_SYS_PATH_STORE)
    {
        lpUpdatePath = WRE_Get_SYS_Dir(WRE_SYS_PATH_STORE_UPDATE);
        if (!WRE_Get_Preload_Dir(&preload_info, WRE_SYS_PATH_STORE))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreFiles: (store)preload path not found!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3791_112_2_18_3_10_56_177,(uint8*)"");
            
            return FALSE;
        }

        if (preload_info.PreloadType&WRE_PRELOAD_SZIP)
        {
            lpFileName = WRE_SYS_FILE_NAME_STORE_SZIP;
        }
        else if ((preload_info.PreloadType&WRE_PRELOAD_WPK))
        {
            lpFileName = WRE_SYS_FILE_NAME_STORE_WPK;
        }
        else
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: (store)PreloadType is WRE_PRELOAD_EXE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3806_112_2_18_3_10_56_178,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[WRE]RestoreFiles: sysPathType not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3811_112_2_18_3_10_56_179,(uint8*)"");

        return FALSE;
    }
    
    if (lpUpdatePath == NULL)
    {
        //SCI_TRACE_LOW:"[WRE]RestoreFiles: path not found!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3818_112_2_18_3_10_57_180,(uint8*)"");

        return FALSE;
    }
    
    MMIAPICOM_Wstrcpy(strBuff, lpUpdatePath);
    if (!MMIAPIFMM_IsFolderExist(strBuff, MMIAPICOM_Wstrlen(strBuff)))
    {
        fError = SFS_CreateDirectory(strBuff);
        if (fError != SFS_NO_ERROR)
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: Create WRE dir Fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3829_112_2_18_3_10_57_181,(uint8*)"");
            return FALSE;
        }
    }

    memset(strDest, 0, sizeof(strDest));
    MMIAPICOM_Wstrcpy(strDest, lpUpdatePath);
    MMIAPICOM_Wstrcat(strDest, L"\\");

    memset(strSour, 0, sizeof(strSour));
    MMIAPICOM_Wstrcpy(strSour, preload_info.PreloadPath);
    MMIAPICOM_Wstrcat(strSour, L"\\");

    if (lpFileName != NULL)
    {
        MMIAPICOM_Wstrcat(strDest, lpFileName);
        MMIAPICOM_Wstrcat(strSour, lpFileName);
        
        if (!WRE_CopyFile(strDest, strSour))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: Copy Preload files Fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3849_112_2_18_3_10_57_182,(uint8*)"");
            return FALSE;
        }

        if (sysPathType == WRE_SYS_PATH_KERNEL)
        {
            memset(strDest, 0, sizeof(strDest));
            MMIAPICOM_Wstrcpy(strDest, lpUpdatePath);
            MMIAPICOM_Wstrcat(strDest, L"\\");
            MMIAPICOM_Wstrcat(strDest, WRE_SYS_FILE_NAME_KERNEL_DEVICEID);
            
            memset(strSour, 0, sizeof(strSour));
            MMIAPICOM_Wstrcpy(strSour, preload_info.PreloadPath);
            MMIAPICOM_Wstrcat(strSour, L"\\");
            MMIAPICOM_Wstrcat(strSour, WRE_SYS_FILE_NAME_KERNEL_DEVICEID);

            WRE_CopyFile(strDest, strSour);
        }
    }
    else
    {
        if (!WRE_CopyTree(strDest, strSour))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreWREEnv: Copy Preload Tree Fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3872_112_2_18_3_10_57_183,(uint8*)"");
            return FALSE;
        }
    }

    WRE_Set_SYS_Dir(sysPathType, lpUpdatePath);    

    return TRUE;
}

#define WRE_FILECOPY_BYTES_LOOP     (20*1024)

BOOLEAN WRE_CopyFile(uint16 *dst, uint16 *src)
{
    SFS_HANDLE  fpr = PNULL;
    SFS_HANDLE  fpw = PNULL;
    SFS_ERROR_E fError = SFS_NO_ERROR;
    uint32      fileSize = 0;
    DWORD       dwCopyNum = 0;
    uint8       *lpBuff = NULL;

    fpr = SFS_CreateFile(src, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if (fpr == NULL)
    {
        //SCI_TRACE_LOW:"[WRE] WRE_FileCopy Fail: src not found"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3896_112_2_18_3_10_57_184,(uint8*)"");

        return FALSE;
    }

    fpw = SFS_CreateFile(dst, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
    if (fpw == NULL)
    {
        //SCI_TRACE_LOW:"[WRE] WRE_FileCopy Fail: Create dst file fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3904_112_2_18_3_10_57_185,(uint8*)"");

        SFS_CloseFile(fpr);
        return FALSE;
    }

    fError = SFS_GetFileSize(fpr, &fileSize);
    if (fileSize == 0)
    {
        //SCI_TRACE_LOW:"[WRE] WRE_FileCopy Fail: file Size == 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3913_112_2_18_3_10_57_186,(uint8*)"");
        
        SFS_CloseFile(fpr);
        SFS_CloseFile(fpw);
        
        return FALSE;
    }

    lpBuff = SCI_ALLOC_APP(WRE_FILECOPY_BYTES_LOOP);
    if (lpBuff == NULL)
    {
        //SCI_TRACE_LOW:"[WRE] WRE_FileCopy Fail: SCI_ALLOC_APP fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3924_112_2_18_3_10_57_187,(uint8*)"");

        SFS_CloseFile(fpr);
        SFS_CloseFile(fpw);

        return FALSE;
    }

    memset(lpBuff, 0, WRE_FILECOPY_BYTES_LOOP);

    dwCopyNum = fileSize;
    while (dwCopyNum > 0)
    {
        DWORD dwToRW = (dwCopyNum > WRE_FILECOPY_BYTES_LOOP)? WRE_FILECOPY_BYTES_LOOP:dwCopyNum;
        DWORD dwRW = 0;
        memset(lpBuff, 0, WRE_FILECOPY_BYTES_LOOP);
        fError = SFS_ReadFile(fpr, lpBuff, dwToRW, &dwRW, NULL);
        if (dwToRW != dwRW)
        {
            //SCI_TRACE_LOW:"[WRE]WRE_FILECOPY Fail: ReadFile Error(%lx)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3943_112_2_18_3_10_57_188,(uint8*)"d", fError);
            break;
        }

        fError = SFS_WriteFile(fpw, lpBuff, dwToRW, &dwRW, NULL);
        if (dwToRW != dwRW)
        {
            //SCI_TRACE_LOW:"[WRE]WRE_FILECOPY Fail: WriteFile Error(%lx)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_3950_112_2_18_3_10_57_189,(uint8*)"d", fError);
            break;
        }

        dwCopyNum -= dwToRW;
    }

    SCI_FREE(lpBuff);
    lpBuff = NULL;

    SFS_FlushFile(fpw, NULL);

    SFS_CloseFile(fpr);
    SFS_CloseFile(fpw);

    if (dwCopyNum != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOLEAN WRE_CopyTree(uint16 *dstDir, uint16 *srcDir)
{
    wchar       strDest[128] = {0};
    wchar       strSour[128] = {0};
    SFS_HANDLE  fp = NULL;
    SFS_FIND_DATA_T findData = {0};
    SFS_ERROR_E fError = SFS_NO_ERROR;
    BOOL bRes = TRUE;

    if (dstDir == NULL || srcDir == NULL)
    {
        return FALSE;
    }

    fp = SFS_FindFirstFile(dstDir, &findData);
    if (fp == NULL)
    {
        return FALSE;
    }

    while (TRUE)   //lint !e716
    {        
        MMIAPICOM_Wstrcpy(strDest, dstDir);
        MMIAPICOM_Wstrcat(strDest, L"\\");
        MMIAPICOM_Wstrcat(strDest, findData.name);

        MMIAPICOM_Wstrcpy(strSour, srcDir);
        MMIAPICOM_Wstrcat(strSour, L"\\");
        MMIAPICOM_Wstrcat(strSour, findData.name);
        
        if (!WRE_CopyFile(strDest, strSour))
        {
            bRes = FALSE;
            break;
        }

        fError = SFS_FindNextFile(fp, &findData);
        if (fError != SFS_NO_ERROR)
        {
            bRes = FALSE;
            break;
        }
    }

    SFS_CloseFile(fp);
    fp = NULL;

    return bRes;
}

void WRE_TraceWstr(const uint16 *lpszString)
{
    char    pszBuf[128] = {0};

    if (lpszString == NULL)
    {
        return ;
    }

    MMIAPICOM_WstrToStr(lpszString, (uint8 *)pszBuf);
    SCI_TRACE_LOW(pszBuf);
}

BOOLEAN WRE_IsSystemPath(uint16 *lpDirPath)
{
    if (*lpDirPath == L'c' || *lpDirPath == L'C')
    {
        return TRUE;
    }

    return FALSE;
}

BOOLEAN WRE_IsDiskFreeSpaceEnough(WRE_SYS_PATH_TYPE sysPathType, uint16 *lpDirPath)
{
    SFS_ERROR_E error = 0;
    uint32 high_word = 0, low_word = 0;

    if (lpDirPath == NULL)
    {
        return FALSE;
    }

    error = SFS_GetDeviceFreeSpace(lpDirPath, &high_word, &low_word);
    if (error != SFS_NO_ERROR)
    {
        return FALSE;
    }

    if (high_word == 0 && low_word <= WRE_GetKernelMinSpace(sysPathType))
    {
        return FALSE;
    }

    return TRUE;
}

uint16 WRE_GetUpdateDevice(WRE_SYS_PATH_TYPE sysPathType)
{
    int i = WRE_DEVICE_UDISK;
    uint16 lpDevice[4] = L"d:";
    
    for (; i < WRE_DEVICE_MAX; i++)
    {
        if (WRE_IsDiskFreeSpaceEnough(sysPathType, lpDevice))
        {
            return *lpDevice ;
        }

        (*lpDevice)++;
    }

    return 0;
}

#define DMDO_0      0
#define DMDO_90     1
#define DMDO_180    2
#define DMDO_270    4
PUBLIC  BOOL  WRE_RotateLCD(int angle,int  * out ){
//SCI_TraceLow:"enter WRE_RotateLCD"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_OTHER_4093_112_2_18_3_10_57_190,(uint8*)"");
#if defined(MMI_PDA_SUPPORT)
    {
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
        GUI_RECT_T      tip_rect = {0};
        LCD_ANGLE_E     scrangle;
        if(angle == -1){
            *out=((int)MMK_GetScreenAngle()- (int)LCD_ANGLE_0);	// changed by yifei
            return TRUE;
        }
        switch(angle){
            case 0:
                scrangle = LCD_ANGLE_0;
                  break;
            case 1:
                scrangle = LCD_ANGLE_90;
                break;
            case 2:
                  scrangle = LCD_ANGLE_180;
                  break;
            case 3:
                scrangle = LCD_ANGLE_270;
                break;
            default:
                scrangle = LCD_ANGLE_0;

                
         }
        if(MMK_GetScreenAngle() != scrangle )
        {
            MMK_SetScreenAngle(scrangle );
            GUI_FillRect(&lcd_dev_info, MMITHEME_GetFullScreenRect(), MMI_WINDOW_BACKGROUND_COLOR);
}
    }
    return TRUE;
#else
    return FALSE;
#endif
    
}

void boot_Invliad_arm_cache(){
 #ifndef WIN32
        /*
        extern void MMU_InvalideDCACHE(void);
        extern void MMU_PHY_InvalideICACHEALL(void);
        */
        /* 使用新的cache函数处理 */
        extern void MMU_IcacheSyncAll (void);
        extern void MMU_DcacheSyncAll (void);
        /*
	    MMU_InvalideDCACHE();
	    MMU_PHY_InvalideICACHEALL();
        */
        /* 使用新的cache函数处理 */
        MMU_IcacheSyncAll();
        MMU_DcacheSyncAll();

     #endif
}


//add begin for wre_background by heng.xiao
/****************此处的定义须与gui.h保持一致*********************/
#define OBJ_BITMAP          7
#define OBJ_ICON			14
#define OBJ_PCX				15
#define OBJ_GIF				16
#define OBJ_JPG				17
#define OBJ_WBMP			18
#define OBJ_PNG				19

typedef struct _NOTIFYICONDATA {
    unsigned long cbSize;		//以字节计的结构大小
    int hWnd;			//接收Windows消息的窗口句柄。
    unsigned int uID;			
    unsigned int uFlags;
    unsigned int uCallbackMessage;		//回调消息ID，由用户自定义。
    int hIcon;			 
    unsigned short  szTip[64];		//图标文件路径
} WRE_NOTIFYICONDATA, *P_WRE_NOTIFYICONDATA;

#define NIM_ADD         0x00000000	//把小图标加到状态栏
#define NIM_MODIFY      0x00000001	//从状态栏中删除小图标
#define NIM_DELETE      0x00000002	//修改小图标
/*********************************************************/
#ifdef WRE_ICON_MAX
#undef WRE_ICON_MAX
#endif	// changed by yifei
#define WRE_ICON_MAX 6
typedef struct  
{
    void* pvoid;
    int len;
    //int type;
}ICON_DATA;

typedef struct  
{
    int idex;
    int hInst;
    ICON_DATA icon_data;
}WRE_ICON;

static WRE_ICON g_wre_icon[WRE_ICON_MAX];

extern void set_display_image_proc(void* proc);
//extern  void MAIN_UpdateIdleWREState(int index,BOOLEAN wre_state,BOOLEAN isupdate);   // changed by yifei
extern int wre_get_MMI_WIN_ICON_E_start_value(void);
extern  BOOLEAN MMIWRE_DisplayImage_Ex(const unsigned short* lpszData, unsigned long unDataLen,GUI_RECT_T* lpRC,const GUI_LCD_DEV_INFO *pdevInfo);

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar(int index,GUI_RECT_T* lpRC,const GUI_LCD_DEV_INFO *pdevInfo)
{
    ICON_DATA* p_icon_data;

    if (index < 0  || index >= WRE_ICON_MAX)
    {
        return FALSE;
    }
    if (g_wre_icon[index].hInst == 0)
    {
        return FALSE;
    }
    p_icon_data = (ICON_DATA*)(&g_wre_icon[index].icon_data);
    return MMIWRE_DisplayImage_Ex(p_icon_data->pvoid,p_icon_data->len,lpRC,pdevInfo);
}

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar0(GUISTBDATA_OWNER_DRAW_T *owner_draw_param)
{
    return MMIWRE_DisplayImageForStatusbar(0,&owner_draw_param->display_rect,&owner_draw_param->lcd_dev);
}

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar1(GUISTBDATA_OWNER_DRAW_T *owner_draw_param)
{
    return MMIWRE_DisplayImageForStatusbar(1,&owner_draw_param->display_rect,&owner_draw_param->lcd_dev);
}

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar2(GUISTBDATA_OWNER_DRAW_T *owner_draw_param)
{
    return MMIWRE_DisplayImageForStatusbar(2,&owner_draw_param->display_rect,&owner_draw_param->lcd_dev);
}

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar3(GUISTBDATA_OWNER_DRAW_T *owner_draw_param)
{
    return MMIWRE_DisplayImageForStatusbar(3,&owner_draw_param->display_rect,&owner_draw_param->lcd_dev);
}

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar4(GUISTBDATA_OWNER_DRAW_T *owner_draw_param)
{
    return MMIWRE_DisplayImageForStatusbar(5,&owner_draw_param->display_rect,&owner_draw_param->lcd_dev);
}

PUBLIC BOOLEAN  MMIWRE_DisplayImageForStatusbar5(GUISTBDATA_OWNER_DRAW_T *owner_draw_param)
{
    return MMIWRE_DisplayImageForStatusbar(5,&owner_draw_param->display_rect,&owner_draw_param->lcd_dev);
}

PUBLIC BOOLEAN MMIWRE_HAVE_BG(int index)
{    
    if (index < 0 || index >= WRE_ICON_MAX)
    {
        return FALSE;
    }
    else if (g_wre_icon[index].hInst != 0)
    {
        return TRUE;
    }
    return FALSE;
}

void MMIWRE_Shell_NotifyIcon(DWORD dwMessage, void* lpData){
    int i;
    ICON_DATA* p_icon_data;
    if (lpData == NULL)
    {
        return;
    }
	/* at present mask. */
#if 0
    set_display_image_proc((void*)MMIWRE_DisplayImageForStatusbar);
#endif
    if (dwMessage == NIM_ADD)
    {
        for (i=0;i<WRE_ICON_MAX;i++)
        {//first find
            if (g_wre_icon[i].hInst == (int)((P_WRE_NOTIFYICONDATA)lpData)->hWnd)
            {
                break;
            }
        }
        if (i >= WRE_ICON_MAX)
        {//add
            for (i=0;i<WRE_ICON_MAX;i++)
            {
                if (g_wre_icon[i].hInst == 0)
                {
                    g_wre_icon[i].idex = i;
                    //g_wre_icon[i].hInst = ((P_WRE_NOTIFYICONDATA)lpData)->uID;
                    g_wre_icon[i].hInst = ((P_WRE_NOTIFYICONDATA)lpData)->hWnd;
                    p_icon_data = ((ICON_DATA*)((P_WRE_NOTIFYICONDATA)lpData)->hIcon);
                    g_wre_icon[i].icon_data.pvoid = p_icon_data->pvoid;
                    g_wre_icon[i].icon_data.len = p_icon_data->len;
                    //g_wre_icon[i].icon_data.type = p_icon_data->type;
                    break;
                }
            }
            if (i < WRE_ICON_MAX)
            {
                MAIN_UpdateIdleWREState(i,TRUE,TRUE);
            }
        }                
    }
    else if (dwMessage == NIM_DELETE)
    {
        for (i=0;i<WRE_ICON_MAX;i++)
        {
            if (g_wre_icon[i].hInst == (int)((P_WRE_NOTIFYICONDATA)lpData)->hWnd)
            {
                memset(&g_wre_icon[i],0,sizeof(WRE_ICON));
                break;
            }
        }
        if (i < WRE_ICON_MAX)
        {
            MAIN_UpdateIdleWREState(i,FALSE,TRUE);
        }        
    }
    else if (dwMessage == NIM_MODIFY)
    {
        for (i=0;i<WRE_ICON_MAX;i++)
        {//first find
            if (g_wre_icon[i].hInst == (int)((P_WRE_NOTIFYICONDATA)lpData)->hWnd)
            {
                break;
            }
        }
        if (i < WRE_ICON_MAX)
        {
            //MAIN_UpdateIdleWREState(i,FALSE,TRUE);
            p_icon_data = ((ICON_DATA*)((P_WRE_NOTIFYICONDATA)lpData)->hIcon);
            g_wre_icon[i].icon_data.pvoid = p_icon_data->pvoid;
            g_wre_icon[i].icon_data.len = p_icon_data->len;
            MAIN_UpdateIdleWREState(i,TRUE,TRUE);
        }
    }
}


uint8 WRE_SetMainLCDContrast(uint8 lcd_contrast)
{
    uint8 curr_contrast = MMIAPISET_GetCurrentContrast();

    if(curr_contrast != lcd_contrast)
    {
        MMIAPISET_SetCurrentContrast(lcd_contrast);      
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, lcd_contrast);
        MMIAPISET_SetMainLCDContrast(); 
    }
    /* return the contrast before setting lcd contrast */
    return curr_contrast;
}

BOOLEAN MMIWRE_SetStatusBarValue(int index, void *item_data_ptr)
{
    GUIWIN_STBDATA_ITEM_DATA_T* p_item_data;
    if (item_data_ptr == NULL)
    {
        return FALSE;
    }
    p_item_data = (GUIWIN_STBDATA_ITEM_DATA_T*)item_data_ptr;
    switch(index)
    {
    case 0:
        p_item_data->owner_draw_func = (GUIWIN_STBDATA_OWNER_DRAW_FUNC)MMIWRE_DisplayImageForStatusbar0;
        break;
    case 1:
        p_item_data->owner_draw_func = (GUIWIN_STBDATA_OWNER_DRAW_FUNC)MMIWRE_DisplayImageForStatusbar1;
        break;
    case 2:
        p_item_data->owner_draw_func = (GUIWIN_STBDATA_OWNER_DRAW_FUNC)MMIWRE_DisplayImageForStatusbar2;
        break;
    case 3:
        p_item_data->owner_draw_func = (GUIWIN_STBDATA_OWNER_DRAW_FUNC)MMIWRE_DisplayImageForStatusbar3;
        break;
    case 4:
        p_item_data->owner_draw_func = (GUIWIN_STBDATA_OWNER_DRAW_FUNC)MMIWRE_DisplayImageForStatusbar4;
        break;
    case 5:
        p_item_data->owner_draw_func = (GUIWIN_STBDATA_OWNER_DRAW_FUNC)MMIWRE_DisplayImageForStatusbar5;
        break;
	default:
		break;	// changed by yifei
    }
    p_item_data->is_handle_callback = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MMIWRE_MaxmizeAPP;   /*lint !e64*/
    
    return TRUE;
}

PUBLIC void MAIN_UpdateIdleWREState(int index,BOOLEAN wre_state,BOOLEAN isupdate)
{
    MMI_WIN_ICON_E which;
    if (index < 0 || index >= WRE_ICON_MAX)
    {
        return;
    }
    which = MMI_WIN_WRE + index;
    if(wre_state)
    {
        GUIWIN_SetStbItemVisible(which, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(which, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT     
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(which, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(which, FALSE);
    }
    
    if(isupdate)
    {    
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

LOCAL BOOLEAN gWreUsingUdisk = FALSE;
PUBLIC void MMIWRE_SetUdiskFlag(BOOLEAN flag)
{
    SCI_TRACE_LOW("MMIWRE_SetUdiskFlag,  flag = %d", flag);
    gWreUsingUdisk = flag;
}

PUBLIC BOOLEAN MMIWRE_isUsingUdisk(void)
{
    return gWreUsingUdisk;
}

PUBLIC BOOLEAN MMIWRE_IsMocorAppBackRunning(void)
{
#ifdef JAVA_SUPPORT
    if (MMIAPIJAVA_IsJavaRuning())
    {
        return TRUE;
    }
#endif
#ifdef MCARE_V31_SUPPORT
    if (McareV31_IsMcareRunning())
    {
        return TRUE;
    }
#endif

    return FALSE;
}

/*****************************************************************************/
//  Description : judge the exit mode from home key or not.
//  Global resource dependence :
//  Author: wei.zhou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWRE_IsFromHomeKeyExit(void)
{
    return s_exitfromhomekey;
}

/*****************************************************************************/
//  Description : clear the home key flag.
//  Global resource dependence :
//  Author: wei.zhou
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_ClearHomeKeyFlag(void)
{
    s_exitfromhomekey = WRE_NORMAL_EXIT_PATH;
}

#ifdef QBTHEME_SUPPORT
LOCAL BOOLEAN gRestoreGbTheme =  FALSE;
/*****************************************************************************/
//  Description : check qbtheme
//  Global resource dependence :
//  Author: langbiao.tan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWRE_CheckQbTheme(void)
{
    BOOLEAN ret = FALSE;

    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        ret = TRUE;
    }
    SCI_TRACE_LOW("[WRE] MMIWRE_CheckQbTheme ret=%d", ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : close qbtheme
//  Global resource dependence :
//  Author: langbiao.tan
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_CloseQbTheme(void)
{
    MMISET_IDLE_STYLE_E idle_style = 0;

    if(MMIWRE_CheckQbTheme() == FALSE)
    {
        return;
    }
    //search idle style
    #if !defined(MMI_ANDROID_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
         idle_style = MMISET_IDLE_ISTYLE;
    #endif
    
    #if defined MMI_GRID_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_GRID;
    #endif
    
    #if defined MMIWIDGET_SUPPORT
        idle_style = MMISET_IDLE_STYLE_WIDGET;
    #endif
        
    #ifdef MMI_COMMON_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_COMMON;
    #endif
        
    #ifdef DPHONE_SUPPORT 
    idle_style = MMISET_IDLE_STYLE_COMMON;
    #endif

    SCI_TRACE_LOW("[WRE] MMIWRE_CloseQbTheme idle_style=%d", idle_style);
    
    //退出千变主题，需要释放内存
    MMIAPIQBTHEME_StyleStop();
    MMISET_SetIdleStyle(idle_style);
    gRestoreGbTheme = TRUE;
}

/*****************************************************************************/
//  Description : start qbtheme
//  Global resource dependence :
//  Author: langbiao.tan
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_StartQbTheme(void)
{
    MMISET_IDLE_STYLE_E idle_style = MMISET_IDLE_QBTHEME;


    SCI_TRACE_LOW("[WRE] MMIWRE_StartQbTheme, gRestoreGbTheme = %d", gRestoreGbTheme);
    if(gRestoreGbTheme)
    {
        MMISET_SetIdleStyle(idle_style);
        //千变主题需要做一些初始化
        MMIAPIQBTHEME_StyleStart();
        gRestoreGbTheme = FALSE;
    }
}
#endif

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************/
//  Description : start sns pulling timer
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
PUBLIC void WRE_StartSnsPullingTimer(int widget_id)
{
    if ((0 > widget_id) || (MMI_SNS_MAX <= widget_id))
    {
        return;
    }

    // Shared timer design, only one timer will be created, operation will be recorded in a flag
    if ((0 == s_active_widget_flag) && (0 == s_bg_timer_id))
    {
        s_bg_timer_id = MMK_CreateTimerCallback(s_bg_restore_time, WRE_Wakeup_SNS_APPS, PNULL, FALSE);
    }
 
    s_active_widget_flag |= s_active_widget_tbl[widget_id];
}

/*****************************************************************************/
//  Description : stop sns pulling timer
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
PUBLIC void WRE_StopSnsPullingTimer(int widget_id)
{
    if ((0 > widget_id) || (MMI_SNS_MAX <= widget_id))
    {
        return;
    }

    // Shared timer design, the only timer will stop if the flag is clear
    s_active_widget_flag &= (~s_active_widget_tbl[widget_id]);
   
    if ((0 != s_bg_timer_id) && (0 == s_active_widget_flag))
    {
        MMK_StopTimer(s_bg_timer_id);
        s_bg_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : release wakeup IMMISNS object
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
PUBLIC void WRE_Wakeup_IMMISNS_Release(void)
{
    if (PNULL != s_pMe)
    {
        IMMISNS_Release(s_pMe);
        s_pMe = PNULL;                    
    }    
}

/*****************************************************************************/
//  Description : set sns pulling timer
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
PUBLIC void WRE_SetSnsPullingTimer(int widget_id, uint32 time)
{
#ifdef NUMBER_OF_SNS_PULLING
    s_bg_restore_time = time / NUMBER_OF_SNS_PULLING;
#else
    s_bg_restore_time = time;
#endif //NUMBER_OF_SNS_PULLING
}

/*****************************************************************************/
//  Description : set sns pulling preset timer
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
PUBLIC void WRE_SetSnsPullingPresetTimer(int widget_id, uint16 preset_type)
{
    uint32 time = s_timer_time_tbl[preset_type];

    s_timer_preset_type[widget_id] = preset_type;
    
#ifdef NUMBER_OF_SNS_PULLING
    s_bg_restore_time = time / NUMBER_OF_SNS_PULLING;
#else
    s_bg_restore_time = time;
#endif //NUMBER_OF_SNS_PULLING    
}

/*****************************************************************************/
//  Description : get sns pulling preset timer setting
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 WRE_GetSnsPullingPresetType(int widget_id)
{
    return s_timer_preset_type[widget_id];
}

/*****************************************************************************/
//  Description : sns pulling timer callback (to launch WRE app)
//  Global resource dependence :
//  Author: david.chen
//  Note:
/*****************************************************************************/
LOCAL void WRE_Wakeup_SNS_APPS(
                   uint8 timer_id,
                   uint32 param
                   )
{
    uint8   temp[16] = {0};
    int result = WRE_INIT_APP_PARAERR;

    if (0 != s_bg_timer_id)
    {
        MMK_StopTimer(s_bg_timer_id);
        s_bg_timer_id = 0;
    }

    if (0 == s_bg_restore_time)
    {
        return;
    }

#ifdef NUMBER_OF_SNS_PULLING
    // Cycling method to determine which app to launch
    // Shared timer will also share time quota
    s_bg_fix_app_id %= NUMBER_OF_SNS_PULLING; // [0..NUMBER_OF_SNS_PULLING - 1] 
    s_bg_fix_app_id += 2;    // [2..NUMBER_OF_SNS_PULLING] Facebook (2) 
#endif //NUMBER_OF_SNS_PULLING

    if (PNULL == s_pMe)
    {
        IMMISNS_CreateObject(&s_pMe);
        MMISNS_SetWidgetCommandData(s_pMe, MMI_WIDGET_SNS_COMMOND_GET_DATA);
    }

    sprintf((char *)temp, "SNS%d", MMISNS_GetWidgetCommandData(s_pMe));

    result = MMIWRE_StartAppSilent(s_bg_fix_app_id, temp);

    if (0 >= result)
    {
        WRE_Wakeup_IMMISNS_Release();
    }

    s_bg_timer_id = MMK_CreateTimerCallback(s_bg_restore_time, WRE_Wakeup_SNS_APPS, PNULL, FALSE);   
}
#endif //SNS_PULLING_SUPPORT

#endif
