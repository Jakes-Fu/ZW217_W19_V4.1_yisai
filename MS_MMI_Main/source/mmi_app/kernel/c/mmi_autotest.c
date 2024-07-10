#define MMI_AUTOTEST_C

#include "mmi_app_kernel_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "os_api.h"
#include "mmk_msg.h"
#include "mmi_autotest.h"
#include "dal_keypad.h"
#include "mmi_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_modu_main.h"

#include "mmieng_nv.h"  //, @CR250530 renwei add
#include "mmi_menutable.h"

#ifdef WRE_SUPPORT
#include "mmiwre_adaptor.h"
#endif
#include "mmi_mainmenu_export.h"
#if defined MMI_AUTOTEST_SUPPORT && defined ENG_SUPPORT

#define ALLOC_AND_MEMSET(buf_ptr, len)      { (buf_ptr) = (uint8*)SCI_ALLOC_APP((len));    SCI_MEMSET((buf_ptr), 0x00, (len));  }
#define ALLOC_AND_MEMSET_16(buf_ptr, len)   {   (buf_ptr) = (wchar*)SCI_ALLOC_APP((len));    SCI_MEMSET((buf_ptr), 0x00, (len)); }
#define FREE_AND_CLEAR(buf_ptr)             {  SCI_FREE(buf_ptr);    (buf_ptr) = PNULL;   }

//win name最大长度限制
#define MAX_WIN_NAME_LEN            50

//softkey
#define MAX_LSOFT_CONTEXT_LEN       50
#define MAX_MSOFT_CONTEXT_LEN       50
#define MAX_RSOFT_CONTEXT_LEN       50

//text
#define MAX_TEXT_CONTEXT_LEN        100

//message
#define MAX_MESSAGE_CONTEXT_LEN     100

//browser title
#define MAX_BWORSER_TITLE_LEN       100

//打印%ld, %d, %x需要的最大长度
#define MAX_PRINT_LD_LEN            20  

//www cursor   
#define MMIWWW_CURSOR_LEN           (60 + 1)

//LOCAL BOOLEAN s_auto_test_set[MMI_AUTO_TEST_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

LOCAL BOOLEAN  s_is_first_get_autotest = TRUE;
LOCAL BOOLEAN  s_nv_autotest[MMI_AUTO_TEST_MAX] = {0};

extern PUBLIC int SCI_Trace_Autotest(const uint8 *src_ptr, uint16 src_len);

#undef MENU_ID_DEF

#define MENU_ID_DEF(menu_id)    menu_id

LOCAL const MMI_MENU_ID_E s_menu_id_arr[] =
{
    #include "mmi_menu_id.def"
};

#undef MENU_ID_DEF

#define MENU_ID_DEF(menu_id)    #menu_id

LOCAL const char* const s_menu_id_name_arr[] =
{
    #include "mmi_menu_id.def" 
};

#undef MENU_ID_DEF

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL void TraceAutoTestNotifyString(char *tag, MMI_STRING_T *in_string);

/*****************************************************************************/
//  Description:    handle autotest module
//	Global resource dependence:
//  Author:         James.Zhang
//	Note:
/*****************************************************************************/   
PUBLIC BOOLEAN MMI_HandleAutoTestModule(
                                        const char* module_str
                                        )
{
    BOOLEAN result = FALSE;

    if ( PNULL != module_str )
    {
        uint32 module_num = ARR_SIZE(s_menu_id_arr);
        uint32 i = 0;
        
        for ( ; i < module_num; i++ )
        {
            if ( 0 == strcmp( s_menu_id_name_arr[i], module_str ) )
            {
                MMIAPIMENU_HandleIdleMenuItem( 0, s_menu_id_arr[i] );
                MMI_TraceAutoTest( (const uint8*)"TEST MODULE:enter success", strlen("TEST MODULE:enter success") );
                result = TRUE;
                break;
            }
        }

    #ifdef WRE_SUPPORT
        {                    
            if ( !result && WREMonkeyTest_EXE(module_str) == 0) //WRE_INIT_OK
            {
               MMI_TraceAutoTest( (const uint8*)"TEST WRE MODULE:enter app success", strlen("TEST WRE MODULE:enter app success") );
               result = TRUE;
            }
        }
    #endif        
        
    }

    return result;
}

/*, @CR250530 renwei modify begin*/ 
/*****************************************************************************/
//  Description:    get auto test setting items value.
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMI_GetAtSetValue(uint8 at_type)
{
    MN_RETURN_RESULT_E	nv_result   = MN_RETURN_FAILURE;
    
    if (at_type < MMI_AUTO_TEST_MAX)
    {
        if (s_is_first_get_autotest)
        {
            s_is_first_get_autotest = FALSE;
            
            MMINV_READ(MMIENG_NV_AUTOTEST, s_nv_autotest, nv_result);

            if (MN_RETURN_SUCCESS != nv_result)
            {
                MMINV_WRITE(MMIENG_NV_AUTOTEST, s_nv_autotest);
            }
        }
        return s_nv_autotest[at_type];
    }
	
    return FALSE;

}

/*****************************************************************************/
//  Description:    set auto test setting items value.
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_SetAtValue(uint8 at_type, BOOLEAN is_on)
{
    MN_RETURN_RESULT_E	nv_result                      = MN_RETURN_FAILURE;
    int32               i                              = MMI_AUTO_TEST_IMAGE;
    int32               num                            = 0;
    
    if (at_type < MMI_AUTO_TEST_MAX)
    {
        MMINV_READ(MMIENG_NV_AUTOTEST, s_nv_autotest, nv_result);

        if(MN_RETURN_SUCCESS != nv_result)
        {
            s_nv_autotest[at_type] = is_on;
            MMINV_WRITE(MMIENG_NV_AUTOTEST, s_nv_autotest);
            return;
        }
        else
        {
            s_nv_autotest[at_type] = is_on;

            for(; i < MMI_AUTO_TEST_MAX; i++)
            {
            	if(s_nv_autotest[i])
            	{
            		num++;
            	}
            }

            if((MMI_AUTO_TEST_MAX-1) == num)
            {
            	s_nv_autotest[MMI_AUTO_TEST_ALL_SET] = TRUE;
            }
            else
            {
            	s_nv_autotest[MMI_AUTO_TEST_ALL_SET] = FALSE;
            }

            MMINV_WRITE(MMIENG_NV_AUTOTEST, s_nv_autotest);
        }   
    }
}

/*, @CR247145 renwei add begin*/
/*****************************************************************************/
//  Description:    set all auto test items value.
//	Global resource dependence:
//  Author:     renwei     
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_SetAllAtValue(BOOLEAN is_on)
{

    int32    i                              = MMI_AUTO_TEST_ALL_SET;

    for(; i < MMI_AUTO_TEST_MAX; i++)
    {
    	s_nv_autotest[i] = is_on;
    }        
	
    MMINV_WRITE(MMIENG_NV_AUTOTEST, s_nv_autotest);

}

/*, @CR247145 renwei add end*/
/*, @CR250530 renwei modify end*/ 
/*****************************************************************************/
//  Description:    get string by text_id
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
LOCAL BOOLEAN GetEnglishStringByTextID(
                                    uint8 *autotest_str,   //out
                                    uint16 autotest_len,
                                    MMI_TEXT_ID_T text_id
                                    );

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC int MMI_TraceAutoTest( // None
                             const uint8 *src_ptr,   // Pointer of the input message. 
                             uint16 src_len          // Size of the source buffer in uint8.
                             )
{
//    SCI_TRACE_LOW("---autotest---%s----%d----", src_ptr, src_len);
    if ((PNULL != src_ptr) && (src_len != 0))
    {
#ifndef WIN32
        SCI_Trace_Autotest(src_ptr, src_len);

        /*+CR171053*//*+CR179060*/
        #ifdef APP_PRODUCT_DM
        MNDUMMY_SendRspInfoToAT(src_ptr,src_len);
        #endif
        /*-CR171053*//*-CR179060*/
#endif
    }
    return 0;
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfImage(
                                    MMI_IMAGE_ID_T         img_id             //in:image id
                                    )
{
    const uint8 head_str[] = "IMAGEID: ";
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_IMAGE))
    {
        return;
    }

    autotest_len = strlen((char*)head_str)+MAX_PRINT_LD_LEN;
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));
    sprintf((char*)autotest_str, "%s0x%x", head_str, img_id);
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));    
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfText(
                                    wchar *text_ptr,
                                    uint16 len
                                    )
{
    const uint8 head_str[] = "WINSTRING: ";
    // uint32 head_len = strlen((char*)head_str);
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;
    uint8 *text_temp_ptr = PNULL;
    wchar *text_wchar_temp_ptr = PNULL;
    uint16 cut_len = 0;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_TEXT))
    {
        return;
    }

    if ((PNULL == text_ptr) || (0 == len))
    {
        return;
    }
    //len > MAX_TEXT_CONTEXT_LEN截取
    cut_len = MIN(len, MAX_TEXT_CONTEXT_LEN);
    ALLOC_AND_MEMSET(text_temp_ptr, (cut_len+1)*sizeof(wchar));
    if (MMIAPICOM_IsASCIIString(text_ptr, cut_len))
    {
        ALLOC_AND_MEMSET_16(text_wchar_temp_ptr, (cut_len+1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy(text_wchar_temp_ptr, text_ptr, cut_len);
        MMIAPICOM_WstrToStr(text_wchar_temp_ptr, text_temp_ptr);
        FREE_AND_CLEAR(text_wchar_temp_ptr);
    }
    else
    {
        GUI_WstrToGB(text_temp_ptr, text_ptr, cut_len);
    }		
    autotest_len = strlen((char*)head_str)+strlen((char*)text_temp_ptr);
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));
    sprintf((char*)autotest_str, "%s%s", head_str, text_temp_ptr);
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));
    FREE_AND_CLEAR(text_temp_ptr);
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
// 	Description : trace auto test open win
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMI_TraceAutoTestOfWinMessage(
                                MMI_WIN_ID_T win_id,
                                MMI_MESSAGE_ID_E msg_id
                                )
{    
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;
    uint8 *win_id_name = PNULL;
    const uint8 openwin_str[]="OPENWIN: ";
    const uint8 closewin_str[]="CLOSEWIN: ";
    const uint8 getfocuswin_str[]="FOCUSWIN: ";
    uint8 const *head_str_ptr = PNULL;
    uint16 cut_len = 0;
    char time_str[50] = {0};
    uint16 time_len = 0;
    uint32 ticks = 0;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_WIN_MSG))
    {
        return;
    }
#ifdef MMI_WINIDNAME_SUPPORT
    MMI_GetWinIdNameArr(win_id, &win_id_name);
#endif
    if (PNULL == win_id_name)
    {
        return;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        head_str_ptr = openwin_str;
        break;

    case MSG_CLOSE_WINDOW:
        head_str_ptr = closewin_str;
        break;

    case MSG_GET_FOCUS:
        head_str_ptr = getfocuswin_str;
        break;
        
    default:
        return;
    }

    ticks = SCI_GetTickCount();
    time_len = sprintf(time_str, " TICKS: 0x%x", ticks);

    cut_len = strlen((char*)win_id_name);
    cut_len = MIN(cut_len, MAX_WIN_NAME_LEN);
    autotest_len = strlen((char*)head_str_ptr)+cut_len+time_len;
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));
    strcpy((char*)autotest_str, (char*)head_str_ptr);
    strncat((char*)autotest_str, (char*)win_id_name, cut_len);
    strncat((char*)autotest_str, time_str, time_len);
    //SCI_TRACE_LOW:"---autotest---%s----%d----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_306_112_2_18_1_59_53_125,(uint8*)"sd", autotest_str, strlen((char*)autotest_str));
    MMI_TraceAutoTest((const uint8*)autotest_str, (uint16)strlen((char*)autotest_str));        
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfTpPress(
                                       uint32 x,
                                       uint32 y,
                                       uint16 gesture_type,
                                       uint16 signalCode
                                       )
{
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;
    const char *title = PNULL;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_TP))
    {
        return;
    }
    switch (signalCode)
    {
    case MMI_TP_DOWN:
        title = "MMI_TP_DOWN";
        break;
    case MMI_TP_UP:
        title = "MMI_TP_UP";
        break;
    case MMI_TP_MOVE:
        title = "MMI_TP_MOVE";
        break;
#ifdef TOUCH_GESTURE_ENABLE
    case MMI_TP_GESTURE:
        title = "MMI_TP_GESTURE";
        break;
#endif
    default:
        return;
    }
    autotest_len = strlen(title) + MAX_PRINT_LD_LEN*3;
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1)); 
    
    sprintf((char*)autotest_str, "%s: x=%d, y=%d, gesture_type=%d", title, x, y, gesture_type);
    //SCI_TRACE_LOW:"---autotest---%s----%d----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_353_112_2_18_1_59_54_126,(uint8*)"sd", autotest_str, strlen((char*)autotest_str));
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));    
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfKeyPress(
                                       uint16 key_type,
                                       uint32 key_code
                                       )
{
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;
    int32 kd_len = strlen("KPD_DOWN: ");
    int32 ku_len = strlen("KPD_UP: ");

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_KEY))
    {
        return;
    }

    autotest_len = MAX(kd_len, ku_len) + MAX_PRINT_LD_LEN;
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));    
    if (KPD_DOWN == key_type)
    {
        sprintf((char*)autotest_str, "KPD_DOWN: %d", key_code);
    }
    else
    {
        sprintf((char*)autotest_str, "KPD_UP: %d", key_code);
    }
    //SCI_TRACE_LOW:"---autotest---%s----%d----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_387_112_2_18_1_59_54_127,(uint8*)"sd", autotest_str, strlen((char*)autotest_str));
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));    
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfMessage(
                                       MMI_TEXT_ID_T text_id
                                       )
{
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;
    const uint8 head_str[]="MESSAGE: ";
    // uint16 head_len = strlen((char*)head_str);
    uint8 *message_context = PNULL;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_MSG))
    {
        return;
    }

    ALLOC_AND_MEMSET(message_context, (MAX_MESSAGE_CONTEXT_LEN+1));
    GetEnglishStringByTextID(message_context, MAX_MESSAGE_CONTEXT_LEN, text_id);
    autotest_len = strlen((char*)head_str)+strlen((char*)message_context);
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));
    sprintf((char*)autotest_str, "%s%s", head_str, message_context);
    //SCI_TRACE_LOW:"---autotest---%s----%d----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_416_112_2_18_1_59_54_128,(uint8*)"sd", autotest_str, strlen((char*)autotest_str));
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));

    FREE_AND_CLEAR(autotest_str);
    FREE_AND_CLEAR(message_context);
}


/*****************************************************************************/
//  Description:    get string by text_id
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
LOCAL BOOLEAN GetEnglishStringByTextID(
                                    uint8 *autotest_str,   //out
                                    uint16 autotest_len,
                                    MMI_TEXT_ID_T text_id
                                    )                                       
{   
#ifdef NANDBOOT_SUPPORT
    // uint32          str_len = 0;
    MMISET_LANGUAGE_TYPE_E  cur_hw_lan = MMISET_LANGUAGE_ENGLISH;
#endif
    MMI_STRING_T    str = {0};
    wchar           *text_ptr = PNULL;
    uint16          cut_len = 0;
    BOOLEAN         result = FALSE;

    if ((PNULL == autotest_str) || (0 == autotest_len))
    {
        return result;
    }
    if (MMI_IsTextNull(text_id))
    {
        return result;
    }
#ifdef NANDBOOT_SUPPORT
    MMIAPISET_GetLanguageType(&cur_hw_lan);
    if (MMISET_LANGUAGE_ENGLISH == cur_hw_lan)
    {            
        MMI_GetLabelTextEnglish(text_id, &str);
        if (PNULL != str.wstr_ptr)
        {
            cut_len = MIN(str.wstr_len, autotest_len);
            ALLOC_AND_MEMSET_16(text_ptr, (cut_len+1)*sizeof(wchar));
            MMIAPICOM_Wstrncpy(text_ptr, str.wstr_ptr, cut_len);
            MMIAPICOM_WstrToStr(text_ptr, autotest_str);
            FREE_AND_CLEAR(text_ptr);
            result = TRUE;
        }
        else
        {
            MMI_ASSERT_WARNING("GetEnglishStringByTextID PNULL == str.wstr_ptr");
            return result;
        }
    }
    else
    {
        uint16  module_id = MMICOM_GET_WORD_HB(text_id);
        uint16  res_offset = MMICOM_GET_WORD_LB(text_id);
        uint8   head_str1[]="text id is ";

        if (autotest_len > (strlen((char*)head_str1)+MAX_PRINT_LD_LEN*2))
        {
            sprintf((char*)autotest_str, "%s%ld %ld", head_str1,module_id,res_offset);
            result = TRUE;
        }
    }
#else
    MMI_GetLabelTextEnglish(text_id, &str);
    if (PNULL != str.wstr_ptr)
    {
        cut_len = MIN(str.wstr_len, autotest_len);
        ALLOC_AND_MEMSET_16(text_ptr, (cut_len+1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy(text_ptr, str.wstr_ptr, cut_len);
        MMIAPICOM_WstrToStr(text_ptr, autotest_str);
        FREE_AND_CLEAR(text_ptr);
        result = TRUE;
    }
    else
    {
        MMI_ASSERT_WARNING("GetEnglishStringByTextID PNULL == str.wstr_ptr");
    }
#endif
    
    return result;
}

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_TraceAutoTestNotifySoftkey(MMI_TEXT_ID_T left_text_id, MMI_TEXT_ID_T middle_text_id, MMI_TEXT_ID_T right_text_id)
{
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;
    uint8 *lsoft_context = PNULL;
    uint8 *msoft_context = PNULL;
    uint8 *rsoft_context = PNULL;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_SOFTKEY))
    {
        return;
    }

    ALLOC_AND_MEMSET(lsoft_context, (MAX_LSOFT_CONTEXT_LEN+1));
    ALLOC_AND_MEMSET(msoft_context, (MAX_MSOFT_CONTEXT_LEN+1));
    ALLOC_AND_MEMSET(rsoft_context, (MAX_RSOFT_CONTEXT_LEN+1));

    GetEnglishStringByTextID(lsoft_context, MAX_LSOFT_CONTEXT_LEN, left_text_id);
    GetEnglishStringByTextID(msoft_context, MAX_MSOFT_CONTEXT_LEN, middle_text_id);
    GetEnglishStringByTextID(rsoft_context, MAX_RSOFT_CONTEXT_LEN, right_text_id);
    autotest_len = strlen("SOFTKEY: LEFT:") + strlen((char*)lsoft_context) + strlen(", MIDDLE:") + strlen((char*)msoft_context) 
        + strlen(", RIGHT:") + strlen((char*)rsoft_context);
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));
    sprintf((char*)autotest_str, "SOFTKEY: LEFT:%s, MIDDLE:%s, RIGHT:%s", lsoft_context, msoft_context, rsoft_context);
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));

    FREE_AND_CLEAR(autotest_str);
    FREE_AND_CLEAR(lsoft_context);
    FREE_AND_CLEAR(msoft_context);
    FREE_AND_CLEAR(rsoft_context);
}

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_TraceAutoTestNotifyTiTle(MMI_STRING_T *in_title)
{
    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_TITLE))
    {
        return;
    }
    TraceAutoTestNotifyString("BROWSERTITLE:", in_title);
}

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_TraceAutoTestNotifyInfo(MMI_STRING_T *in_string)
{
    TraceAutoTestNotifyString("MESSAGE:", in_string);
}

/*****************************************************************************/
//  Description : notify pointer shape callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyPointerStatesProc(int in_x,int in_y,int in_shape,int in_show, int in_nf)
{
    uint8 cursor[MMIWWW_CURSOR_LEN] = {0};
    uint16 len = 0;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_POINTER_SHAPE))
    {
        return;
    }

    sprintf((char*)cursor, "%s", "BROWSERCURSOR:");
    len += strlen("BROWSERCURSOR:");
    sprintf((char*)(cursor + len), "X=%03d,Y=%03d;", in_x, in_y);
    len += 12;
    sprintf((char*)(cursor + len), "%s", "Shap:");
    len += strlen("Shap:");
    if (2 == in_shape)
    {
        sprintf((char*)(cursor + len), "%s", "Hand");
    }
    else if (0 == in_shape)
    {
        sprintf((char*)(cursor + len), "%s", "Arrow");
    }
    else if (13 == in_shape)
    {
        sprintf((char*)(cursor + len), "%s", "Clock");
    }
    else
    {
        sprintf((char*)(cursor + len), "%s", "Unknown");
    }

    MMI_TraceAutoTest((unsigned char*)cursor, (uint16)(strlen((char*)cursor)));
}

/*****************************************************************************/
//  Description:    Handle autotest string
//	Global resource dependence:
//  Author:        
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_Wchar_AutoTest_Trace(
                                    wchar *trace_head_ptr,
                                    uint16 trace_head_len,							        
                                    wchar *trace_body_ptr,
                                    uint16 trace_body_len
                                    )
{
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;

    if ((PNULL == trace_body_ptr) || (0 == trace_body_len))
    {
		//SCI_TRACE_LOW:"[AUTOTEST] MMI_AutoTest_Trace invalid parameter !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_642_112_2_18_1_59_54_129,(uint8*)"");
        return;
    }

    ALLOC_AND_MEMSET(autotest_str, (MIN((trace_head_len + trace_body_len + 1), MAX_TEXT_CONTEXT_LEN)+1)*sizeof(wchar));
	if(PNULL == autotest_str)   /*lint !e774*/
	{
		//SCI_TRACE_LOW:"[AUTOTEST] MMI_AutoTest_Trace malloc fail parameter !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_649_112_2_18_1_59_54_130,(uint8*)"");
		return;
	}
	if(PNULL != trace_head_ptr && (trace_head_len > 0))
	{
		//head string
		if (MMIAPICOM_IsASCIIString(trace_head_ptr, trace_head_len))
		{
			MMI_WSTRNTOSTR(&autotest_str[autotest_len], 
				(MAX_TEXT_CONTEXT_LEN - autotest_len),
				trace_head_ptr,
				MIN(trace_head_len, (MAX_TEXT_CONTEXT_LEN - autotest_len)),
				MIN(trace_head_len, (MAX_TEXT_CONTEXT_LEN - autotest_len)));
		}
		else
		{
			GUI_WstrToGB(&autotest_str[autotest_len], trace_head_ptr, MIN(trace_head_len, (MAX_TEXT_CONTEXT_LEN - autotest_len)));
		}
		autotest_len = strlen((char*)autotest_str);
	}
	if(autotest_len < MAX_TEXT_CONTEXT_LEN)
	{
		//body string
		if (MMIAPICOM_IsASCIIString(trace_body_ptr, trace_body_len))
		{
			MMI_WSTRNTOSTR(&autotest_str[autotest_len], 
				(MAX_TEXT_CONTEXT_LEN - autotest_len),
				trace_body_ptr,
				MIN(trace_body_len, (MAX_TEXT_CONTEXT_LEN - autotest_len)),
				MIN(trace_body_len, (MAX_TEXT_CONTEXT_LEN - autotest_len)));
		}
		else
		{
			GUI_WstrToGB(&autotest_str[autotest_len], trace_body_ptr, MIN(trace_body_len, (MAX_TEXT_CONTEXT_LEN - autotest_len)));
		}
		autotest_len = strlen((char*)autotest_str);
	}
	//SCI_TRACE_LOW((char*)autotest_str);
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
//  Description:    Handle autotest string
//	Global resource dependence:
//  Author:        
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_Char_AutoTest_Trace(
                                    char *trace_head_ptr,
                                    uint16 trace_head_len,							        
                                    char *trace_body_ptr,
                                    uint16 trace_body_len
                                    )
{
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;

    if ((PNULL == trace_body_ptr) || (0 == trace_body_len))
    {
		//SCI_TRACE_LOW:"[AUTOTEST] MMI_AutoTest_Trace invalid parameter !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_707_112_2_18_1_59_54_131,(uint8*)"");
        return;
    }

    ALLOC_AND_MEMSET(autotest_str, (MIN((trace_head_len + trace_body_len + 1), MAX_TEXT_CONTEXT_LEN)+1));
	if (PNULL == autotest_str)  /*lint !e774*/
	{
		//SCI_TRACE_LOW:"[AUTOTEST] MMI_AutoTest_Trace malloc fail parameter !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_AUTOTEST_714_112_2_18_1_59_54_132,(uint8*)"");
		return;
	}
	if(PNULL != trace_head_ptr && (trace_head_len > 0))
	{
		strncpy((char*)autotest_str, trace_head_ptr, MIN(trace_head_len, MAX_TEXT_CONTEXT_LEN));
		//add space at head 
		autotest_len = strlen((char*)autotest_str);
	}
	if(autotest_len + 1 <= MAX_TEXT_CONTEXT_LEN)
	{
		strncpy((char*)&autotest_str[autotest_len], trace_body_ptr, MIN(trace_body_len, MAX_TEXT_CONTEXT_LEN - autotest_len));
	}

	//SCI_TRACE_LOW((char*)autotest_str);
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));
    FREE_AND_CLEAR(autotest_str);
}

/*****************************************************************************/
//  Description:    Handle autotest Backlight Status
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoBackLightStatus(MMI_BACKLIGHT_STATUS_E backlight_status)
{

    const uint8 head_str[] = "BACKLIGHTS: ";
    uint8 *autotest_str = PNULL;
    uint32 autotest_len = 0;

    if (!MMI_GetAtSetValue(MMI_AUTO_TEST_BACKLIGHT))
    {
        return;
    }

    autotest_len = strlen((char*)head_str)+MAX_PRINT_LD_LEN;
    ALLOC_AND_MEMSET(autotest_str, (autotest_len+1));
    sprintf((char*)autotest_str, "%s0x%x", head_str,(uint32) backlight_status);
    MMI_TraceAutoTest((const uint8*)autotest_str, strlen((char*)autotest_str));    
    FREE_AND_CLEAR(autotest_str);

}

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL void TraceAutoTestNotifyString(char * tag, MMI_STRING_T *in_string)
{
    uint8 *title = PNULL;
    uint32 title_len = 0;
    uint8 *title_temp = PNULL;
    wchar *text_wchar_temp = PNULL;
    uint16 cut_len = 0;

    if ((PNULL == in_string) || (PNULL == in_string->wstr_ptr) || (0 == in_string->wstr_len))
    {
        return;
    }

    SCI_TRACE_LOW("TraceAutoTestNotifyString len = %d", in_string->wstr_len);

    cut_len = MIN(in_string->wstr_len, MAX_TEXT_CONTEXT_LEN);
    ALLOC_AND_MEMSET_16(text_wchar_temp, (cut_len+1)*sizeof(wchar));
    ALLOC_AND_MEMSET(title_temp, (cut_len+1)*sizeof(uint16));  
    if (MMIAPICOM_IsASCIIString(in_string->wstr_ptr, cut_len))
    {
        MMIAPICOM_Wstrncpy(text_wchar_temp, in_string->wstr_ptr, cut_len);
        MMIAPICOM_WstrToStr(text_wchar_temp, title_temp);
    }
    else
    {
        GUI_WstrToGB(title_temp, in_string->wstr_ptr, cut_len);
    }    
    title_len = strlen((char*)tag)+strlen((char*)title_temp);
    ALLOC_AND_MEMSET(title, (title_len+1));    
    sprintf((char*)title, "%s%s", tag, title_temp);
    MMI_TraceAutoTest((unsigned char *)title, (uint16)(strlen((char *)title)));
    FREE_AND_CLEAR(text_wchar_temp);
    FREE_AND_CLEAR(title_temp);
    FREE_AND_CLEAR(title);
}


#else   //MMI_AUTOTEST_SUPPORT

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfImage(
                                    MMI_IMAGE_ID_T         img_id             //in:image id
                                    )
{
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfText(
                                    wchar *text_ptr,
                                    uint16 len
                                    )
{
}

/*****************************************************************************/
// 	Description : trace auto test open win
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMI_TraceAutoTestOfWinMessage(
                                MMI_WIN_ID_T win_id,
                                MMI_MESSAGE_ID_E msg_id
                                )
{
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfTpPress(
                                       uint32 x,
                                       uint32 y,
                                       uint16 gesture_type,
                                       uint16 signalCode
                                       )
{
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfKeyPress(
                                       uint16 key_type,
                                       uint32 key_code
                                       )
{
}

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfMessage(
                                       MMI_TEXT_ID_T text_id
                                       )
{
}

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_TraceAutoTestNotifySoftkey(MMI_TEXT_ID_T left_text_id, MMI_TEXT_ID_T middle_text_id, MMI_TEXT_ID_T right_text_id)
{   
}


/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_TraceAutoTestNotifyTiTle(MMI_STRING_T *in_title)
{ 
}

/*****************************************************************************/
//  Description : notify pointer shape callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyPointerStatesProc(int in_x,int in_y,int in_shape,int in_show, int in_nf)
{
}


/*****************************************************************************/
//  Description:    Handle autotest string
//	Global resource dependence:
//  Author:        
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_Wchar_AutoTest_Trace(
                                    wchar *trace_head_ptr,
                                    uint16 trace_head_len,							        
                                    wchar *trace_body_ptr,
                                    uint16 trace_body_len
                                    )
{

}

/*****************************************************************************/
//  Description:    Handle autotest string
//	Global resource dependence:
//  Author:        
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_Char_AutoTest_Trace(
                                    char *trace_head_ptr,
                                    uint16 trace_head_len,							        
                                    char *trace_body_ptr,
                                    uint16 trace_body_len
                                    )
{
}

/*****************************************************************************/
//  Description:    Handle autotest Backlight Status
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoBackLightStatus(MMI_BACKLIGHT_STATUS_E backlight_status)
{


}

#endif      //MMI_AUTOTEST_SUPPORT
