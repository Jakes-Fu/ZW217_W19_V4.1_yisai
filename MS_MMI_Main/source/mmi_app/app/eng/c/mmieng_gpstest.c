#include <math.h>
#include "mmk_app.h"
#include "mmi_menutable.h"
#include "mmieng_menutable.h"
#include "mmieng_text.h"
#include "window_parse.h"
#include "mmieng_id.h"
#include "mmi_signal_ext.h"
#include "mmk_timer.h"
#include "mmieng_gpstest.h"
#ifdef GPS_SUPPORT
//#include "mmigps_api.h"
#endif
#include "guilistbox.h"
#include "guiform.h"
#include "guilabel.h"
#include "guiedit.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#include "mmieng_gpstest_nmea_log.h"
#include "mmidisplay_data.h"
#include "sig_code.h"
#include "mmicom_trace.h"
#include "mmieng_export.h"
#include "mmi_position.h"
#ifdef UAL_GNSS_SUPPORT
#include "ual_gnss.h"
#endif
/************************   Common  start**************************************/
#define LATLONG_LEN_MAX 15
#ifdef TOUCH_PANEL_SUPPORT
#define TP_BUTTON_WIDTH  70
#define TP_BUTTON_HEIGHT 40
#else
#define TP_BUTTON_WIDTH  0
#define TP_BUTTON_HEIGHT 0
#endif

LOCAL double   g_latitude_angle        = 0;//default value
LOCAL double   g_longtitude_angle      = 0;//default value
typedef enum
{
    GNSS_TEST_START_MODE_HOT = 0,
    GNSS_TEST_START_MODE_COLD,
    GNSS_TEST_START_MODE_WARM,
    GNSS_TEST_START_MODE_FACTORY,
    GNSS_TEST_START_MODE_MAX
}GNSS_TEST_START_MODE_TYPE_E;
typedef enum
{
    ENTER_START_MODE_FROM_TRACKING_TEST,
    ENTER_START_MODE_FROM_CIRCLE_TEST,
    ENTER_START_MODE_TYPE_MAX
}ENTER_START_MODE_TYPE;
LOCAL GNSS_TEST_START_MODE_TYPE_E g_start_mode_for_tracking_test = GNSS_TEST_START_MODE_COLD;

//LOCAL BOOLEAN _EngHandleGpsMessageCallback(APP_MN_GPS_SIGNAL_E gps_sig_id,void* gps_info);
LOCAL void EngAppendStartModeOneLineItemContent(MMI_CTRL_ID_T current_ctrl_id,ENTER_START_MODE_TYPE start_mode_from);
LOCAL void OpenCircleTestLogFile(GNSS_TEST_START_MODE_TYPE_E start_mode);
/************************   Common  end************************************** /



/****************************************************************************/
/*****************************GPS NMEA Log win start***************************** /
/****************************************************************************/
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MAX_NMEA_LOG_BUFFER          512
#define MMIENG_TEST_GPS_NMEA_LOG_MENU_INDEX_0             0
#define MMIENG_TEST_GPS_NMEA_LOG_MENU_INDEX_1             1
//#define MMIENG_GPS_NMEA_LOG_MENU_INDEX_2               2 

//GPS NMEA log save path
typedef enum
{
    MMIENG_TEST_GPS_NMEA_LOG_SAVE_T_CARD,
    MMIENG_TEST_GPS_NMEA_LOG_OUTPUT_DEFAULT,
    //MMIENG_GPS_NMEA_LOG_SAVE_PHONE,
    MMIENG_TEST_GPS_NMEA_LOG_SAVE_MODE_MAX,
} MMIENG_TEST_GPS_NMEA_LOG_SAVE_MODE_E;

 typedef enum
{
    NMEA_NONE = 0, 
    NMEA_GGA   = 1, 
    NMEA_GLL    = 2,
    NMEA_GSA   = 4,
    NMEA_GSV   = 8,
    NMEA_RMC   = 16,  
    NMEA_VTG   = 32,
    NMEA_ALL   = 255
} GPS_NMEA_TYPE_E;
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                  */
/*---------------------------------------------------------------------------*/
LOCAL MMI_WIN_ID_T g_gpsNmeaLogWnd = 0;
LOCAL MMIENG_TEST_GPS_NMEA_LOG_SAVE_MODE_E  s_nmea_log_save_mode = MMIENG_TEST_GPS_NMEA_LOG_OUTPUT_DEFAULT;//MMIENG_TEST_GPS_NMEA_LOG_SAVE_T_CARD;
LOCAL GPS_NMEA_TYPE_E     cmd_nmea_type;
LOCAL BOOLEAN g_nmeaLog_isIdle = TRUE;
LOCAL BOOLEAN g_nmeaLog_started = FALSE;
LOCAL uint8 reveive_log_buf[MAX_NMEA_LOG_BUFFER+1] = {0};
LOCAL MN_DUAL_SYS_E g_sys_sim_index = MN_DUAL_SYS_1;
LOCAL uint8 g_output_req_timer_id = 0;
LOCAL MMI_STRING_T g_log_path = {0};
BOOLEAN g_is_circle_test = FALSE;
LOCAL uint32 g_gnss_handle = 0;

#define GNSS_TEST_GPS_SATELLITE_PRN_MIN   1
#define GNSS_TEST_GPS_SATELLITE_PRN_MAX   32
#define GNSS_TEST_BEIDOU_SATELLITE_PRN_MIN   201
#define GNSS_TEST_BEIDOU_SATELLITE_PRN_MAX   264
#define GNSS_TEST_GALIEO_SATELLITE_PRN_MIN   95
#define GNSS_TEST_GALIEO_SATELLITE_PRN_MAX   130
#define GNSS_TEST_GLONASS_SATELLITE_PRN_MIN   65
#define GNSS_TEST_GLONASS_SATELLITE_PRN_MAX   92

typedef enum
{
    GNSS_SATELLITE_TYPE_GPS,
    GNSS_SATELLITE_TYPE_BEIDOU,
    GNSS_SATELLITE_TYPE_GALIEO,
    GNSS_SATELLITE_TYPE_GLONASS,
    GNSS_SATELLITE_TYPE_MAX
}ENG_GNSS_SATELLITE_TYPE_E;
LOCAL void CloseCircleTestLogFile(void);

LOCAL void StartNmeaLogReq(void);
LOCAL void Gps_StartRequestSatInfoTimer(MN_DUAL_SYS_E sys_sim_index);
/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void Gps_HandleOutputReqTimer(uint8 timer_id, uint32 param)
{
    if (g_output_req_timer_id == timer_id)
    {
        MMK_StopTimer(g_output_req_timer_id);
        g_output_req_timer_id = 0;
    }
    StartNmeaLogReq();
}
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void Gps_StartOutputReqTimer(void)
{
    if (NULL != g_output_req_timer_id)
    {
        MMK_StopTimer(g_output_req_timer_id);
        g_output_req_timer_id = 0;
    }
    g_output_req_timer_id = MMK_CreateTimerCallback(10000, Gps_HandleOutputReqTimer, NULL, FALSE);
}


/*****************************************************************************/
//  Description : set the mode of saving log
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetSaveMode(
               MMIENG_TEST_GPS_NMEA_LOG_SAVE_MODE_E  save_mode//in
               )
{
    if(MMIENG_TEST_GPS_NMEA_LOG_SAVE_MODE_MAX <= save_mode)
    {
        //TRACE_APP_ENGINEERMODE:"[MMIMP3]:MMIMP3_SetRepeatMode, param error!"
        TRACE_APP_ENGINEERMODE("MMIENG_GPS_NMEA_LOG_SetSaveMode, param_error!");
        return;
    }

    s_nmea_log_save_mode = save_mode;

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void AppendSavePathListItem(
                    MMI_CTRL_ID_T       ctrl_id
                    )
{
    uint16  cur_selection = 0;

    GUILIST_SetMaxItem(ctrl_id,MMIENG_TEST_GPS_NMEA_LOG_SAVE_MODE_MAX, FALSE );

    //MMIAPISET_AppendListItemByTextId(TXT_ENG_GPS_NMEA_LOG_SAVE_T_CARD,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    //MMIAPISET_AppendListItemByTextId(TXT_ENG_GPS_NMEA_LOG_OUTPUT_DEFAULT,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    //MMIAPISET_AppendListItemByTextId(TXT_ENG_GPS_NMEA_LOG_SAVE,TXT_NULL,ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT);
   // GUILIST_SetItemInvalid(ctrl_id, MMIENG_GPS_NMEA_LOG_SAVE_PHONE,TRUE);

    //get current lyric set
    switch(s_nmea_log_save_mode)
    {
    case     MMIENG_TEST_GPS_NMEA_LOG_SAVE_T_CARD:
        cur_selection = MMIENG_TEST_GPS_NMEA_LOG_MENU_INDEX_0;
        break;
        
  /*  case MMIENG_GPS_NMEA_LOG_SAVE_PHONE:
        cur_selection = MMIENG_GPS_NMEA_LOG_MENU_INDEX_1;
        break;*/

    case MMIENG_TEST_GPS_NMEA_LOG_OUTPUT_DEFAULT:
        cur_selection = MMIENG_TEST_GPS_NMEA_LOG_MENU_INDEX_1;
        break;

    default:
        break;
    }
    //set select item
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Discription: draw nmea log win
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMainWindow(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 timer = 5;
    MMI_STRING_T        title_text            = {0};
    GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T lab_rect = client_rect ;
    GUI_RECT_T text_rect = client_rect;
    GUI_FONT_T font = SONG_FONT_8;
    GUI_COLOR_T font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_FONT);
    uint32        softkey_height = 0;

    MMI_HANDLE_T    win_handle=MMK_ConvertIdToHandle(win_id);
    MMI_CTRL_ID_T   ctrl_id = ID_ENG_GPS_TEST_NMEA_LOG_LABEL_CTRL_ID;
    GUI_BG_T label_bg = {0};
    label_bg.bg_type = GUI_BG_IMG;
    label_bg.img_id = IMAGE_COMMON_BG;

    /*label*/
    ctrl_id = ID_ENG_GPS_TEST_NMEA_LOG_LABEL_CTRL_ID;
    lab_rect.bottom =  (client_rect.bottom + client_rect.top)*3/10;
    GUILABEL_SetRect(ctrl_id, &lab_rect, FALSE);       
    GUILABEL_SetBg(ctrl_id,&label_bg);
    MMI_GetLabelTextByLang(TXT_ENG_GPS_NMEA_LOG_OUTPUT_DEFAULT, &title_text);

    if (TRUE == g_nmeaLog_isIdle)
    {
        if(NULL != g_log_path.wstr_ptr)
        {
            SCI_FREE(g_log_path.wstr_ptr);
            g_log_path.wstr_ptr = NULL;
            g_log_path.wstr_len = 0;
        }
        g_log_path.wstr_ptr = SCI_ALLOC_APPZ(GUILIST_STRING_MAX_NUM * sizeof(wchar));
        if(NULL != g_log_path.wstr_ptr)
        {
            SCI_MEMSET(g_log_path.wstr_ptr, 0, GUILIST_STRING_MAX_NUM * sizeof(wchar));
        }
        else
        {
            TRACE_APP_ENGINEERMODE("[GPS]: g_log_path.wstr_ptr malloc fail!");
        }
        g_log_path.wstr_len = title_text.wstr_len;
        MMIAPICOM_Wstrncpy(g_log_path.wstr_ptr, title_text.wstr_ptr,MIN(g_log_path.wstr_len, GUILIST_STRING_MAX_NUM));
    }
    GUILABEL_SetText(ctrl_id, &g_log_path, TRUE);
    GUILABEL_SetFont(ctrl_id,font,MMI_BLACK_COLOR);

    /*log text*/
    ctrl_id = ID_ENG_GPS_TEST_NMEA_LOG_TEXTBOX_CTRL_ID;
    text_rect.top = lab_rect.bottom ;
    win_handle = MMK_ConvertIdToHandle(win_id);
    softkey_height = MMITHEME_GetWinSoftkeyHeight(win_handle);
    text_rect.bottom = client_rect.bottom-softkey_height;
    GUITEXT_SetRect(MMK_GetCtrlHandleByWin(win_handle, ctrl_id), &text_rect);
    LCD_FillRect(&lcd_dev_info, text_rect, MMI_WHITE_COLOR); //mmi_eng_display.bkgd_color  
    GUITEXT_SetFont(ctrl_id, &font,&font_color);
    GUITEXT_SetAutoScroll(TRUE,&timer,ctrl_id);
    GUITEXT_SetCircularHandle(FALSE,ctrl_id);

   // GUIWIN_SetSoftkeyTextId(win_id, TXT_NMEA_LOG_START, TXT_NULL, TXT_NMEA_LOG_STOP, TRUE);
}
/*****************************************************************************/
//  Description : update new log data to display
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RefreshNmeaLogData(uint8 *logData)
{
    const int32 buffer_size = MAX_NMEA_LOG_BUFFER ;
    wchar wstr[MAX_NMEA_LOG_BUFFER +1] = {0};     
    int32 str_length = MAX_NMEA_LOG_BUFFER ;
    MMI_CTRL_ID_T ctrl_id = ID_ENG_GPS_TEST_NMEA_LOG_TEXTBOX_CTRL_ID;

    /*log text*/
    MMI_STRNTOWSTR(wstr, buffer_size, logData, str_length,SCI_STRLEN(logData));  
    TRACE_APP_ENGINEERMODE("[GPS]: RefreshNmeaLogData, refresh LEN, wchar:%ls", wstr);
    GUITEXT_SetString(ctrl_id , wstr , (uint16)str_length, TRUE);  
    MMK_SendMsg(MMIENG_TEST_GPS_NMEA_LOG_WIN_ID, MSG_FULL_PAINT, PNULL);
}
/*****************************************************************************/
//  Description : handle NMEA log data
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
#ifdef UAL_GNSS_SUPPORT
PUBLIC BOOLEAN MMIENG_HandleNmeaLogData(ual_gnss_output_nmea_ind_t*NmeaLogData)
{
    BOOLEAN res = FALSE;
    uint32 buf_len = 0;
    uint32 newData_len = 0;
    uint8 text_buf[MAX_NMEA_LOG_BUFFER +1] = {0};
    uint8 newData_buf[MAX_NMEA_LOG_BUFFER +1] = {0};
    ual_gnss_nmea_output_param_t p_output_param = {0};
    TRACE_APP_ENGINEERMODE("[GPS]: MMIENG_HandleNmeaLogData , nmea type: 0x%x, length: %d", NmeaLogData->type, NmeaLogData->length);
    if(NMEA_NONE == cmd_nmea_type)
    {
        //MNGPS_OutputReq(g_sys_sim_index, 0, 1);
        p_output_param.nmea_type = UAL_GNSS_NMEA_NONE_TYPE;
        p_output_param.time_interval = 1;
        ual_gnss_output_nmea(&p_output_param);
    }
    else if(NMEA_ALL == cmd_nmea_type)
    {
        if(NMEA_NONE == NmeaLogData->type)
        {
          return res;
        }
        else
        {
            SCI_MEMCPY(newData_buf , NmeaLogData->nmea_data, MIN(NmeaLogData->length, MAX_NMEA_LOG_BUFFER));
            newData_len = SCI_STRLEN(newData_buf);

            /*if new receive buffer is being full, delete the original buffer content*/
            buf_len = SCI_STRLEN(reveive_log_buf);
            if(MAX_NMEA_LOG_BUFFER <= (buf_len + newData_len)){
                TRACE_APP_ENGINEERMODE("[GPS]: MMIENG_HandleNmeaLogData, receive buffer full");
                SCI_MEMCPY(text_buf, reveive_log_buf + newData_len, SCI_STRLEN(reveive_log_buf + newData_len));
                SCI_MEMSET(reveive_log_buf, 0, MAX_NMEA_LOG_BUFFER + 1);
                SCI_MEMCPY(reveive_log_buf, text_buf, SCI_STRLEN(text_buf));
            }
            buf_len = SCI_STRLEN(reveive_log_buf);
            SCI_MEMCPY(reveive_log_buf +buf_len, newData_buf, newData_len);
            TRACE_APP_ENGINEERMODE("[GPS]: MMIENG_HandleNmeaLogData, refresh LEN, reveive_log_buf:%s", reveive_log_buf);

            switch(s_nmea_log_save_mode)
            {
                case MMIENG_TEST_GPS_NMEA_LOG_SAVE_T_CARD:
                    TRACE_APP_ENGINEERMODE("[GPS]: MMIENG_HandleNmeaLogData,write new data to file, newData_buf:%s", newData_buf);
                    res = MMIENG_WriteLOGFile(newData_buf,newData_len,LOG_SAVE_NMEALOG);
                break;
                //case MMIENG_GPS_NMEA_LOG_SAVE_PHONE:
               // break;
                case MMIENG_TEST_GPS_NMEA_LOG_OUTPUT_DEFAULT:
                    TRACE_APP_ENGINEERMODE("[GPS]: MMIENG_HandleNmeaLogData, nmea data: %s", reveive_log_buf);
                break;
                default:

                break;

            }
            RefreshNmeaLogData(reveive_log_buf);
        }
    }
    return res;
}
#endif
/*****************************************************************************/
//  Description : start request output NMEA log
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartNmeaLogReq(void)
{
    uint32 time_interval = 1;
    //GPS_NMEA_TYPE_E type = NMEA_ALL;
    uint8 fileName[GUILIST_STRING_MAX_NUM] = "NMEALog";
    uint8 fulPathFileName[GUILIST_STRING_MAX_NUM] = {0};
    wchar fulPathFileName_wchar[GUILIST_STRING_MAX_NUM]={0};
    wchar labelDisplay_wchar[GUILIST_STRING_MAX_NUM]={0};
    MMI_STRING_T temp_str = {0};
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_nmea_output_param_t  p_param = {0};
    ual_gnss_nmea_type_e          type    = UAL_GNSS_NMEA_ALL_TYPE;
#endif
    sprintf(fulPathFileName,"E:\\%s.log",fileName);   //fix Bug 1322183

    MMI_STRNTOWSTR(fulPathFileName_wchar,strlen((char*)fulPathFileName),fulPathFileName,strlen((char*)fulPathFileName),strlen((char*)fulPathFileName));

    TRACE_APP_ENGINEERMODE("[GPS]: enter StartNmeaLogReq , fileName:%s", fulPathFileName);
    g_nmeaLog_started= TRUE;

    /*init g_log_path before set label string*/
    if(NULL != g_log_path.wstr_ptr)
    {
        SCI_FREE(g_log_path.wstr_ptr);
        g_log_path.wstr_ptr = NULL;
        g_log_path.wstr_len = 0;
    }
    g_log_path.wstr_ptr = SCI_ALLOC_APPZ(GUILIST_STRING_MAX_NUM * sizeof(wchar));
    if(NULL != g_log_path.wstr_ptr)
    {
        SCI_MEMSET(g_log_path.wstr_ptr, 0, GUILIST_STRING_MAX_NUM * sizeof(wchar));
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS]: g_log_path.wstr_ptr malloc fail!");
    }
    g_log_path.wstr_len = 0;

    if (MN_DUAL_SYS_MAX == g_sys_sim_index)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: enter StartNmeaLogReq , g_sys_sim_index is invalid, GPS start not suc!");
        Gps_StartOutputReqTimer();
        return;
    }
    TRACE_APP_ENGINEERMODE("[GPS]: enter StartNmeaLogReq ,s_nmea_log_save_mode:%d", s_nmea_log_save_mode);
    SCI_MEMSET(reveive_log_buf, 0, MAX_NMEA_LOG_BUFFER+1);
    switch(s_nmea_log_save_mode)
    {
        case MMIENG_TEST_GPS_NMEA_LOG_SAVE_T_CARD:
            MMI_GetLabelTextByLang(TXT_ENG_GPS_NMEA_LOG_SAVE, &temp_str);
            MMIAPICOM_Wstrncpy(labelDisplay_wchar, temp_str.wstr_ptr, temp_str.wstr_len);
            MMIAPICOM_Wstrncat(labelDisplay_wchar, fulPathFileName_wchar, MMIAPICOM_Wstrlen(fulPathFileName_wchar));
            TRACE_APP_ENGINEERMODE("[GPS]: enter StartNmeaLogReq ,fullFileName_wchar:%ls", labelDisplay_wchar);
            SCI_MEMSET(g_log_path.wstr_ptr, 0, sizeof(wchar)*GUILIST_STRING_MAX_NUM);
            g_log_path.wstr_len = MMIAPICOM_Wstrlen(labelDisplay_wchar);
            MMIAPICOM_Wstrncpy(g_log_path.wstr_ptr, labelDisplay_wchar,g_log_path.wstr_len);
            GUILABEL_SetText(ID_ENG_GPS_TEST_NMEA_LOG_LABEL_CTRL_ID, &g_log_path, TRUE);

            MMIENG_LogOpen(fileName,LOG_SAVE_NMEALOG);
        break;
       // case MMIENG_GPS_NMEA_LOG_SAVE_PHONE:
       // break;
        case MMIENG_TEST_GPS_NMEA_LOG_OUTPUT_DEFAULT:
        default:
        break;
    }
   // MNGPS_OutputReq(g_sys_sim_index, (uint16)type, time_interval);
#ifdef UAL_GNSS_SUPPORT
    p_param.nmea_type = type;
    p_param.time_interval = time_interval;
    ual_gnss_output_nmea(&p_param);
#endif
}
/*****************************************************************************/
//  Description : stop request output NMEA log
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopNmeaLogReq(void)
{
    uint32 time_interval = 1;
    //GPS_NMEA_TYPE_E type = NMEA_NONE;
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_nmea_output_param_t  p_param = {0};
    ual_gnss_nmea_type_e          type    = UAL_GNSS_NMEA_NONE_TYPE;
#endif
    /*free receive buffer
    if (NULL != reveive_log_buf){
        SCI_FREE(reveive_log_buf);
        reveive_log_buf = NULL;
   }*/
   SCI_MEMSET(reveive_log_buf, 0, MAX_NMEA_LOG_BUFFER +1 );
   MMIENG_LogClose(LOG_SAVE_NMEALOG);
   //MNGPS_OutputReq(g_sys_sim_index, (uint16)type, time_interval);
#ifdef UAL_GNSS_SUPPORT
   p_param.nmea_type = type;
   p_param.time_interval = time_interval;
   ual_gnss_output_nmea(&p_param);
#endif
   g_nmeaLog_started = FALSE;

}
/*****************************************************************************/
//  Discription:   handle gps win message
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGpsNmeaLogWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

	TRACE_APP_ENGINEERMODE("[GPS]: enter HandleGpsNmeaLogWinMsg, msg_id:%d,  task", msg_id);
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW: 
        TRACE_APP_ENGINEERMODE("[GPS]: enter HandleGpsNmeaLogWinMsg, MSG_OPEN_WINDOW, g_nmeaLog_isIdle:%d,",g_nmeaLog_isIdle);
        DisplayMainWindow(win_id);
		break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    	TRACE_APP_ENGINEERMODE("enter HandleGpsNmeaLogWinMsg, wanna start save log");
        if (TRUE == g_nmeaLog_isIdle)
        {
            //if(GPS_STARTED_STATE == MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
            if(UAL_GNSS_STATE_WORK == ual_gnss_get_cur_state())
            {
                //start save NMEA log
                cmd_nmea_type = NMEA_ALL;
                StartNmeaLogReq();
            }
#endif
            g_nmeaLog_isIdle = FALSE;
        }
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
    break;

    case MSG_APP_WEB:
        TRACE_APP_ENGINEERMODE("[GPS]: enter HandleGpsNmeaLogWinMsg, wanna exit save log ");
        if (TRUE == g_nmeaLog_started){
            //stop save NMEA log
            cmd_nmea_type = NMEA_NONE;
            StopNmeaLogReq();
        }
        g_nmeaLog_isIdle = TRUE;
        break;

    case MSG_CLOSE_WINDOW:
        if (TRUE == g_nmeaLog_isIdle)
        {
            if(NULL != g_log_path.wstr_ptr)
            {
                SCI_FREE(g_log_path.wstr_ptr);
                g_log_path.wstr_ptr = NULL;
            }
            g_log_path.wstr_len = 0;
        }
        TRACE_APP_ENGINEERMODE("[GPS]: enter HandleGpsNmeaLogWinMsg, close NMEA log window ");
        break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}
/************************ window definition *****************************************/
WINDOW_TABLE( MMIENG_TEST_GPS_NMEA_LOG_WIN_TAB ) = 
{
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32) HandleGpsNmeaLogWinMsg ),    
    WIN_ID( MMIENG_TEST_GPS_NMEA_LOG_WIN_ID),
    WIN_TITLE(TXT_ENG_GPS_NMEA_LOG),
    WIN_SOFTKEY(TXT_NMEA_LOG_START, TXT_NMEA_LOG_STOP, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,ID_ENG_GPS_TEST_NMEA_LOG_LABEL_CTRL_ID),
    CREATE_TEXT_CTRL(ID_ENG_GPS_TEST_NMEA_LOG_TEXTBOX_CTRL_ID),

    END_WIN
}; 
/****************************************************************************/
/*****************************GPS NMEA Log win end***************************** /
/****************************************************************************/

/****************************************************************************/
/************************   Satellite Info start  ************************************** /
/****************************************************************************/

/************************ function definition *****************************************/
#define GPS_SATELLITE_NUM                                       32                             //卫星最大数目
#ifdef UAL_GNSS_SUPPORT
LOCAL  BOOLEAN  MMIENG_GPSSatCN_HandleSatIndMsg(ual_gnss_satellite_info_list_t * pSatInfo );
#endif
/************************ function realization *****************************************/
LOCAL MMI_RESULT_E EngGpsTrackingStartModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID);
            GUILIST_SetMaxItem( ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID, GNSS_TEST_START_MODE_MAX, FALSE );
            EngAppendStartModeOneLineItemContent(ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID,ENTER_START_MODE_FROM_TRACKING_TEST);
        break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                uint16 cur_index = 0;
                cur_index = GUILIST_GetCurItemIndex(ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID);
                g_start_mode_for_tracking_test = cur_index;
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                MMK_CloseWin(win_id);
            }
        break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;
        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

WINDOW_TABLE(MMIENG_GPS_TRACKINGTEST_STARTMODE_WIN_TAB) =
{
    WIN_HIDE_STATUS,

    WIN_FUNC((uint32)EngGpsTrackingStartModeWinHandleMsg),
    WIN_ID(MMIENG_GPS_TRACKINGTEST_STARTMODE_WIN_ID),
    WIN_TITLE(TXT_START_MODE_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : fill one line content to listbox 
//  param:IN ctrl_id
//  Author: 
//  RETRUN: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIENG_GPSSatCN_AppendONELINEItemContent(MMI_CTRL_ID_T current_ctrl_id)
{
    GUILIST_ITEM_T              item_t    = {0}; 
    GUILIST_ITEM_DATA_T     item_data = {0}; 
    GUI_RECT_T  list_rect = {0};
    wchar temp[20]  = {'S', 'a', 't','e','l','l','i','t','e',' ','S','e','a','r','c','h','i','n','g',0};
    uint16 lcdWidth = 0;
    uint16 lcdHeight = 0;
    GUILCD_GetLogicWidthHeight(0, &lcdWidth, &lcdHeight);

    //设置LISTBOX控件
    list_rect.left = 0;
    list_rect.top = MMITHEME_GetStatusBarHeight();
    list_rect.right = lcdWidth-1;
    list_rect.bottom = lcdHeight-1 - TP_BUTTON_HEIGHT- MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    TRACE_APP_ENGINEERMODE("[GPS]: enter");


    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp);   //satellite zhi
    TRACE_APP_ENGINEERMODE("[GPS]: %d\n",MMIAPICOM_Wstrlen(temp));
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp;

    GUILIST_SetRect(current_ctrl_id, &list_rect);
    GUILIST_AppendItem(current_ctrl_id,&item_t);

    //if(GPS_STARTED_STATE == MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
    if(UAL_GNSS_STATE_WORK == ual_gnss_get_cur_state())
    {
        TRACE_APP_ENGINEERMODE("[GPS]: gps started state\n");
        GUIWIN_SetSoftkeyTextId(MMIENG_TEST_GPS_SATELLITECN_WIN_ID,STXT_OPTION,TXT_STOP_GPS_SHORT, TXT_ENG_RETURN,  TRUE);
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS]: gps not started state\n");
        GUIWIN_SetSoftkeyTextId(MMIENG_TEST_GPS_SATELLITECN_WIN_ID,STXT_OPTION, TXT_START_GPS_SHORT, TXT_ENG_RETURN, TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description : fill content to listbox 
//  param:IN ctrl_id,satellite_info
//  Author: 
//  RETRUN: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIENG_GPSSatCN_AppendTWOLINEItemContent(MMI_CTRL_ID_T current_ctrl_id,uint16 sat_ID,uint16 sat_cn)
{
    GUILIST_ITEM_T              item_t    = {0}; 
    GUILIST_ITEM_DATA_T     item_data = {0}; 
   
    char sate_cn_temp_str[GUILIST_STRING_MAX_NUM ] = {0};
    char sate_num_temp_str[GUILIST_STRING_MAX_NUM ] = {0};
    wchar sate_cn_temp_wchar[GUILIST_STRING_MAX_NUM ] = {0};
    wchar sate_num_temp_wchar[GUILIST_STRING_MAX_NUM] = {0};
    uint16  sate_num_len      = 0;
    uint16  sate_cn_len         = 0 ;

    wchar temp_satnum[20]  = {'S', 'a', 't','e', 'l','l','i','t','e',' ','I','D',':',0};
    wchar temp_satcn0[20]  = {'S', 'a', 't','e', 'l','l','i','t','e',' ','C','/','N','0',':',0};
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    TRACE_APP_ENGINEERMODE("[GPS]");

     //获取卫星编号和它的信噪比值
     MMIAPICOM_Int2Str((uint32)sat_cn,sate_cn_temp_str,GUILIST_STRING_MAX_NUM);
     sate_cn_len = strlen(sate_cn_temp_str);
     MMI_STRNTOWSTR(sate_cn_temp_wchar,GUILIST_STRING_MAX_NUM,sate_cn_temp_str,GUILIST_STRING_MAX_NUM,sate_cn_len);/*lint !e64*/

     MMIAPICOM_Int2Str((uint32)sat_ID,sate_num_temp_str,GUILIST_STRING_MAX_NUM);
     sate_num_len = strlen(sate_num_temp_str);
     MMI_STRNTOWSTR(sate_num_temp_wchar,GUILIST_STRING_MAX_NUM,sate_num_temp_str,GUILIST_STRING_MAX_NUM,sate_num_len);/*lint !e64*/

     //第一行卫星编号
     TRACE_APP_ENGINEERMODE("[GPS]: enter GUIITEM_DATA_TEXT_BUFFER satellite ID\n");
     item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
     item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_satnum);   //satellite zifuchuan
     item_data.item_content[0].item_data.text_buffer.wstr_ptr =temp_satnum;
        
     item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
     item_data.item_content[1].item_data.text_buffer.wstr_len = sate_num_len;   //satellite zhi
     item_data.item_content[1].item_data.text_buffer.wstr_ptr = sate_num_temp_wchar;
     
     //第二行 信噪比
     TRACE_APP_ENGINEERMODE("[GPS]: enter GUIITEM_DATA_TEXT_BUFFER xinzaobi\n");
     item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
     item_data.item_content[2].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_satcn0);
     item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_satcn0;

     item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
     item_data.item_content[3].item_data.text_buffer.wstr_len = sate_cn_len;   //xinzaobi zhi
     item_data.item_content[3].item_data.text_buffer.wstr_ptr = sate_cn_temp_wchar; 

    GUILIST_AppendItem(current_ctrl_id,&item_t);
}

/*****************************************************************************/
//  Description : MMIENG_GPSSatCN_HandleSatIndMsg
//  param:handle pAPP_MN_GPS_SATELLITE_INFO_IND message
//  Author: 
//  RETRUN: 
//  Note: 
/*****************************************************************************/
#ifdef UAL_GNSS_SUPPORT
LOCAL  BOOLEAN  MMIENG_GPSSatCN_HandleSatIndMsg(ual_gnss_satellite_info_list_t* pSatInfo )
{
    MMI_CTRL_ID_T               current_ctrl_id = ID_ENG_GPS_TEST_SATELLITECN_LIST_CTRL_ID;
    int i                       =0;
    uint8 sat_total_num         = 0;
    if(PNULL == pSatInfo)
    {
         TRACE_APP_ENGINEERMODE("[GPS]: enter MMIENG_GPSSatCN_HandleSatIndMsg pAPP_MN_GPS_SATELLITE_INFO_IND is null\n");
         return FALSE;
    }
      
    GUILIST_RemoveAllItems(current_ctrl_id);
    sat_total_num = pSatInfo->satellite_num;
    TRACE_APP_ENGINEERMODE("[GPS]: enter MMIENG_GPSSatCN_HandleSatIndMsg sat_total_num = %d\n",sat_total_num);
    if(0 == sat_total_num)
    {
        MMIENG_GPSSatCN_AppendONELINEItemContent(current_ctrl_id);
    }
    else
    {
        for (i = 0; i< sat_total_num; i++)
        {
        //填充satellite number and satellite c/n0
        TRACE_APP_ENGINEERMODE("[GPS]: enter MMIENG_GPSSatCN_HandleSatIndMsg sat_ID = %d,sat_cno= %d\n",pSatInfo->satellite_info[i].satellite_identifier,pSatInfo->satellite_info[i].cn0);
        MMIENG_GPSSatCN_AppendTWOLINEItemContent(current_ctrl_id,pSatInfo->satellite_info[i].satellite_identifier,pSatInfo->satellite_info[i].cn0);
        }
    }

    MMK_SendMsg(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, MSG_FULL_PAINT, PNULL);
    return TRUE;
}
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : Init Weekly Button
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
#define SATE_CN_BUTTON_OK    {8,180,70,240}
#define SATE_CN_BUTTON_START {85,180,155,240}
#define SATE_CN_BUTTON_STOP  {163,180,233,240}
LOCAL MMI_RESULT_E ButtonOkHandler(void)
{
    TRACE_APP_ENGINEERMODE("[GPS]: ButtonOkHandler enter\n");

    MMK_SendMsg((uint32 *)MMIENG_TEST_GPS_SATELLITECN_WIN_ID , MSG_CTL_OK,PNULL);
    return MMI_RESULT_TRUE;
}
LOCAL MMI_RESULT_E ButtonStartHandler(void)
{
    TRACE_APP_ENGINEERMODE("[GPS]: ButtonStartHandler enter\n");
    MMK_SendMsg((uint32 *)MMIENG_TEST_GPS_SATELLITECN_WIN_ID ,MSG_CTL_MIDSK, PNULL);
    return MMI_RESULT_TRUE;
}
LOCAL MMI_RESULT_E ButtonStopHandler(void)
{
    TRACE_APP_ENGINEERMODE("[GPS]: ButtonStopHandler enter\n");

    MMK_SendMsg((uint32 *)MMIENG_TEST_GPS_SATELLITECN_WIN_ID ,MSG_CTL_CANCEL, PNULL);
    return MMI_RESULT_TRUE;
}
LOCAL void InitSatelliteCNButtonCtrl(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T rect = {0};
    GUI_BG_T   bg_btn = {0};
    GUI_FONT_ALL_T  button_font_all = {0};//button的字体与颜色
    GUI_RECT_T button_ok = SATE_CN_BUTTON_OK;
    GUI_RECT_T button_start = SATE_CN_BUTTON_START;
    GUI_RECT_T button_stop = SATE_CN_BUTTON_STOP;
    MMK_GetWinRect(win_id, &rect);
    TRACE_APP_ENGINEERMODE("[GPS]: InitSatelliteCNButtonCtrl\n");

    //set win background color to black first
    WATCHCOM_DisplayBackground(win_id);

    //button OK
    button_ok.left = rect.left; //set button posision
    button_ok.right = rect.left+TP_BUTTON_WIDTH-1;
    button_ok.bottom = rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    button_ok.top = button_ok.bottom - TP_BUTTON_HEIGHT+1;

    //button start
    button_start.left = rect.left+TP_BUTTON_WIDTH +(rect.right-TP_BUTTON_WIDTH*3)/2-1;  // center
    button_start.right = button_start.left + TP_BUTTON_WIDTH-1; // center
    button_start.bottom = rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    button_start.top = button_start.bottom - TP_BUTTON_HEIGHT+1;

    //button stop
    button_stop.left = rect.right-TP_BUTTON_WIDTH-1;
    button_stop.right = rect.right;
    button_stop.bottom = rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    button_stop.top = button_stop.bottom -TP_BUTTON_HEIGHT+1;

    button_font_all.font  = SONG_FONT_16;
    button_font_all.color =  MMI_BLACK_COLOR;
    bg_btn.bg_type = GUI_BG_COLOR;
    bg_btn.color = MMI_DARK_WHITE_COLOR;

    GUIBUTTON_SetRunSheen( MMIENG_MEM_BUTTON_REPORT_ID, TRUE);//btn_get
    GUIBUTTON_SetPressedBg(MMIENG_MEM_BUTTON_REPORT_ID, &bg_btn);
    GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_REPORT_ID, &button_font_all);
    GUIBUTTON_SetTextId(MMIENG_MEM_BUTTON_REPORT_ID, STXT_OPTION);
    GUIBUTTON_SetRect(MMIENG_MEM_BUTTON_REPORT_ID, &button_ok);
    GUIBUTTON_SetBg(MMIENG_MEM_BUTTON_REPORT_ID, &bg_btn);
    GUIBUTTON_SetCallBackFunc(MMIENG_MEM_BUTTON_REPORT_ID, ButtonOkHandler);//the button get param of TP

    GUIBUTTON_SetRunSheen( MMIENG_MEM_BUTTON_START_ID, TRUE);//btn_set
    GUIBUTTON_SetPressedBg(MMIENG_MEM_BUTTON_START_ID, &bg_btn);
    GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_START_ID, &button_font_all);
    GUIBUTTON_SetTextId(MMIENG_MEM_BUTTON_START_ID, TXT_START_GPS_SHORT);
    GUIBUTTON_SetRect(MMIENG_MEM_BUTTON_START_ID, &button_start);
    GUIBUTTON_SetBg(MMIENG_MEM_BUTTON_START_ID, &bg_btn);
    GUIBUTTON_SetCallBackFunc(MMIENG_MEM_BUTTON_START_ID, ButtonStartHandler);//the button set param of TP

    GUIBUTTON_SetRunSheen( MMIENG_MEM_BUTTON_STOP_ID, TRUE);//btn_test
    GUIBUTTON_SetPressedBg(MMIENG_MEM_BUTTON_STOP_ID, &bg_btn);
    GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_STOP_ID, &button_font_all);
    GUIBUTTON_SetTextId(MMIENG_MEM_BUTTON_STOP_ID, TXT_ENG_RETURN);
    GUIBUTTON_SetRect(MMIENG_MEM_BUTTON_STOP_ID, &button_stop);
    GUIBUTTON_SetBg(MMIENG_MEM_BUTTON_STOP_ID, &bg_btn);
    GUIBUTTON_SetCallBackFunc(MMIENG_MEM_BUTTON_STOP_ID, ButtonStopHandler);//the button draw TP

}
#endif

LOCAL void UpdateSateCNButton(void)
{
    WATCHCOM_DisplayBackground(MMIENG_TEST_GPS_SATELLITECN_WIN_ID);

    //if(GPS_STARTED_STATE == MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
    if(UAL_GNSS_STATE_WORK == ual_gnss_get_cur_state())
    {
        TRACE_APP_ENGINEERMODE("[GPS]: MSG_FULL_PAINT gps started state\n");
        GUIWIN_SetSoftkeyTextId(MMIENG_TEST_GPS_SATELLITECN_WIN_ID,TXT_NULL,TXT_STOP_GPS_SHORT, TXT_ENG_RETURN,TRUE);
        GUIWIN_SetSoftkeyBtnGray(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, 1, TRUE);
#ifdef TOUCH_PANEL_SUPPORT
        CTRLBUTTON_SetTextId(MMIENG_MEM_BUTTON_START_ID,TXT_STOP_GPS_SHORT);
        CTRLBUTTON_SetVisible(MMIENG_MEM_BUTTON_REPORT_ID,FALSE,TRUE);
        CTRLBUTTON_Update(MMIENG_MEM_BUTTON_START_ID);
#endif
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS]: MSG_FULL_PAINT gps not started state\n");
        GUIWIN_SetSoftkeyTextId(MMIENG_TEST_GPS_SATELLITECN_WIN_ID,STXT_OPTION, TXT_START_GPS_SHORT, TXT_ENG_RETURN, TRUE);
        GUIWIN_SetSoftkeyBtnGray(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, 1, FALSE);
#ifdef TOUCH_PANEL_SUPPORT
        CTRLBUTTON_SetTextId(MMIENG_MEM_BUTTON_START_ID,TXT_START_GPS_SHORT);
        CTRLBUTTON_SetVisible(MMIENG_MEM_BUTTON_REPORT_ID,TRUE,TRUE);
        CTRLBUTTON_Update(MMIENG_MEM_BUTTON_START_ID);
#endif
    }
#endif
}
LOCAL MMI_RESULT_E HandleTrackingTestOptWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T          ctrl_id  = MMIENG_COMMON_CTRL1_ID;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_ENGINEERMODE("[GPS]: Open window:%d\n",win_id);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        {
            switch (GUIMENU_GetCurNodeId(ctrl_id))
            {
                case ID_ENG_TRACKING_TEST_OPT_START_MODE_SET:
                    {
                        // set start mode
                        TRACE_APP_ENGINEERMODE("[GPS]: Open window TRACKING_TEST_OPT_START_MODE_SET\n");
                        MMK_CreateWin((uint32 *)MMIENG_GPS_TRACKINGTEST_STARTMODE_WIN_TAB, PNULL);
                        MMK_CloseWin(win_id);
                    }
                    break;

                default:
                    TRACE_APP_ENGINEERMODE("[GPS]: invalid item \n");
                    break;
            }
            break;
        }
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }
        default:
        {
            recode = MMI_RESULT_FALSE;
            break;
        }
    }
    return recode;
}


WINDOW_TABLE( MMIENG_GPS_TRACKING_TEST_OPT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleTrackingTestOptWindow),
    WIN_ID( MMIENG_GPS_TRACKING_TEST_OPT_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN

};

/*****************************************************************************/
//  Description : open Tracking Test option menu window
//  Global resource dependence : none
//  Author: Xiuyun.Wang
//  Note: 
/*****************************************************************************/
LOCAL void  OpenTrackingTestOptMenuWin(void)
{
    MMI_CTRL_ID_T    ctrl_id = MMIENG_COMMON_CTRL1_ID;
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    if (MMK_IsOpenWin(MMIENG_GPS_TRACKING_TEST_OPT_WIN_ID))
    {
        MMK_CloseWin(MMIENG_GPS_TRACKING_TEST_OPT_WIN_ID);
    }
    MMK_CreateWin((uint32 *)MMIENG_GPS_TRACKING_TEST_OPT_WIN_TAB, NULL);

    //creat dynamic menu
    GUIMENU_CreatDynamic(PNULL, MMIENG_GPS_TRACKING_TEST_OPT_WIN_ID, ctrl_id, GUIMENU_STYLE_POPUP);

    //set start mode
    MMI_GetLabelTextByLang(TXT_START_MODE_SET, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(0,ID_ENG_TRACKING_TEST_OPT_START_MODE_SET,NULL,&node_item,ctrl_id);

}

/*****************************************************************************/
//  Description : window handle message
//  Global resource dependence :
//  Author: 
//  RETRUN: 
//  Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E MMIENG_GPSSatCN_WinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T        current_ctrl_id = ID_ENG_GPS_TEST_SATELLITECN_LIST_CTRL_ID;
    //MMIGPS_START_GPS_PARAM_T start_param = {0};
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_start_param_t  gnss_start_param = {0};
#endif
    TRACE_APP_ENGINEERMODE("[GPS]: Enter\n");
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_ENGINEERMODE("[GPS]: Open window:%d\n",win_id);
            GUILIST_SetMaxItem( current_ctrl_id, GPS_SATELLITE_NUM, FALSE);
            MMIENG_GPSSatCN_AppendONELINEItemContent(current_ctrl_id);
#ifdef TOUCH_PANEL_SUPPORT
            InitSatelliteCNButtonCtrl(win_id);
#else
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);
#endif
        }
            break;
        case MSG_FULL_PAINT:
        {
            UpdateSateCNButton();
            break;
        }
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#if 0
            TRACE_APP_ENGINEERMODE("[GPS]: MSG_CTL_MIDSK MMIGPS_CurrentStatus_Get():%d\n",MMIGPS_CurrentStatus_Get());
            if(GPS_STARTED_STATE != MMIGPS_CurrentStatus_Get())
            {
                TRACE_APP_ENGINEERMODE("[GPS]: MSG_CTL_MIDSK gps not started state\n");
                start_param.request_type = MMIGPS_REQUEST_TYPE_ENG;
                start_param.start_type = MMIGPS_START_TYPE_NONE;
                start_param.start_mode = g_start_mode_for_tracking_test;
                MMIGPS_Start(&start_param,_EngHandleGpsMessageCallback);
                Gps_StartRequestSatInfoTimer(g_sys_sim_index);
            }
            else
            {
                TRACE_APP_ENGINEERMODE("[GPS]:MSG_CTL_MIDSK  gps started state\n");
                MMIGPS_Stop();
                //close circle test file
                if(TRUE == g_is_circle_test)
                {
                    g_is_circle_test = FALSE;
                    CloseCircleTestLogFile();
                }
            }
#endif
#ifdef UAL_GNSS_SUPPORT
            if(UAL_GNSS_STATE_WORK != ual_gnss_get_cur_state())
            {
                TRACE_APP_ENGINEERMODE("[GPS]: MSG_CTL_MIDSK gps not work state\n");
                g_is_circle_test = FALSE;
                TRACE_APP_ENGINEERMODE("[GPS]: set circle test false\n");
                gnss_start_param.start_mode = g_start_mode_for_tracking_test;
                ual_gnss_start(&gnss_start_param);
                //Gps_StartRequestSatInfoTimer(g_sys_sim_index);
            }
            else
            {
                TRACE_APP_ENGINEERMODE("[GPS]:MSG_CTL_MIDSK  gps work state\n");
                ual_gnss_stop();
                //close circle test file
                if(TRUE == g_is_circle_test)
                {
                    g_is_circle_test = FALSE;
                    CloseCircleTestLogFile();
                }
            }
#endif
            break;
        case MSG_APP_MENU:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        {
            //if(GPS_STARTED_STATE != MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
            if(UAL_GNSS_STATE_WORK != ual_gnss_get_cur_state())
            {
                OpenTrackingTestOptMenuWin();
            }
#endif
        }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
            break;
        case MSG_KEYUP_UP:
        case MSG_KEYDOWN_UP:
        case MSG_KEYLONG_UP:
        {
            uint16 top_item = GUILIST_GetTopItemIndex(current_ctrl_id);
            GUILIST_SetTopItemIndex(current_ctrl_id, (top_item - 1));
            MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL);
        }
            break;
        case MSG_KEYUP_DOWN:
        case MSG_KEYDOWN_DOWN:
        case MSG_KEYLONG_DOWN:
        {
            uint16 top_item = GUILIST_GetTopItemIndex(current_ctrl_id);
            GUILIST_SetTopItemIndex(MMIENG_LISTBOX_CTRL_ID, (top_item + 1));
            MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL);
        }
            break;
        default:
            return MMI_RESULT_FALSE;
        break;
    }
    return MMI_RESULT_TRUE;
}


/************************ window definition *****************************************/
WINDOW_TABLE(MMIENG_TEST_GPS_SATELLITECN_TAB) =
{  
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32)MMIENG_GPSSatCN_WinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_SATELLITECN_WIN_ID),
    WIN_TITLE(TXT_SATELLITES),
    WIN_SOFTKEY(STXT_OPTION, TXT_START_GPS_SHORT, TXT_ENG_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,ID_ENG_GPS_TEST_SATELLITECN_LIST_CTRL_ID),
#ifdef TOUCH_PANEL_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_GRAYED_BUTTON,MMIENG_MEM_BUTTON_REPORT_ID),
    CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_GRAYED_BUTTON,MMIENG_MEM_BUTTON_START_ID),
    CREATE_BUTTON_CTRL(IMAGE_PDASOFTKEY_GRAYED_BUTTON,MMIENG_MEM_BUTTON_STOP_ID),
#endif
    END_WIN
};
/****************************************************************************/
/*****************************Satellite Info part end***************************** /
/****************************************************************************/

/************************************************************************/
/**********************GPS&Satellite information test start********************/
/************************************************************************/
#ifdef UAL_GNSS_SUPPORT
LOCAL void AppendSatelliteListItem(MMI_CTRL_ID_T  ctrl_id, ual_gnss_read_info_cnf_t *gpsSatelliteInfo, ual_gnss_fix_ind_t* p_fix_ind);
#endif
LOCAL uint8  g_request_sat_info_timer_id   = 0;
LOCAL void Gps_HandleRequestSatInfoTimer(uint8 timer_id,uint32 param)
{
    /*if (g_request_sat_info_timer_id == timer_id)
    {
        MMK_StopTimer(g_request_sat_info_timer_id);
        g_request_sat_info_timer_id = 0;
    }*/
    //MNGPS_ReadInfoReq(g_sys_sim_index);
#ifdef UAL_GNSS_SUPPORT
        ual_gnss_read_info();
#endif
    TRACE_APP_ENGINEERMODE("[GPS]:readInfoReq in timer\n");
}

LOCAL void Gps_StartRequestSatInfoTimer(MN_DUAL_SYS_E sys_sim_index)
{
    if (NULL != g_request_sat_info_timer_id)
    {
        MMK_StopTimer(g_request_sat_info_timer_id);
        g_request_sat_info_timer_id = 0;
    }
    TRACE_APP_ENGINEERMODE("[GPS]:Gps_StartRequestSatInfoTimer sys_sim_index:%d\n",sys_sim_index);
    g_request_sat_info_timer_id = MMK_CreateTimerCallback(1000, Gps_HandleRequestSatInfoTimer, sys_sim_index, TRUE);
}

#ifdef UAL_GNSS_SUPPORT
LOCAL void AppendSatelliteListItem(MMI_CTRL_ID_T  ctrl_id, ual_gnss_read_info_cnf_t*gpsSatelliteInfo, ual_gnss_fix_ind_t* p_fix_ind)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T     item_data= {0};

    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    uint8 i = 0;
    uint8 GPS_Num = 0;
    uint8 BD_Num = 0;
    uint8 GALIEO_Num = 0;
    uint8 GLONASS_Num = 0;
    wchar temp_gps[4] = {'G','P','S',0};
    wchar temp_bd[7] = {'B','e','i','d','o','u',0};
    wchar temp_galieo[7] = {'G','a','l','i','e','o',0};
    wchar temp_glonass[8] = {'G','l','o','n','a','s','s',0};
    ENG_GNSS_SATELLITE_TYPE_E  satellite_type = GNSS_SATELLITE_TYPE_MAX;
    wchar wstr_gps_num[50] = {0};
    wchar wstr_bd_num[50] = {0};
    wchar wstr_galieo_num[50] = {0};
    wchar wstr_glonass_num[50] = {0};
    MMI_STRING_T sat_num_str = {0};
    MMI_STRING_T position_ok = {0};
    MMI_STRING_T position_fail = {0};
    wchar wstr_position_result[50] = {0};

    if(PNULL == gpsSatelliteInfo)
    {
        SCI_TRACE_LOW("[ENG] [GPS]: AppendSatelliteListItem, gpsSatelliteInfo is null!!!\n");
        return;
    }
    SCI_TRACE_LOW("[ENG] [GPS]: AppendSatelliteListItem\n");
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetListState(MMIENG_LISTBOX_CTRL_ID,GUILIST_STATE_FOCUS,FALSE);

    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_info.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    //未定位/已经定位
    if(PNULL == p_fix_ind)
    {
        if(0 == gpsSatelliteInfo->gps_info.ttff_time)
        {
            SCI_TRACE_LOW("[ENG][GPS]no position");
            MMI_GetLabelTextByLang(TXT_ENG_NO_POSITION,&position_fail);
            sprintf(tmp_item, "  %s:%d", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
            MMI_STRNTOWSTR(tmp_wstr_1,
                strlen((char*)tmp_item),
                tmp_item,
                strlen((char*)tmp_item),
                strlen((char*)tmp_item));
            MMIAPICOM_Wstrncat(wstr_position_result,position_fail.wstr_ptr,position_fail.wstr_len);
        }
        else
        {
            SCI_TRACE_LOW("[ENG][GPS]have positioned");
            MMI_GetLabelTextByLang(TXT_ENG_HAVE_POSITIONED,&position_ok);
            sprintf(tmp_item, "  %s:%d", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
            MMI_STRNTOWSTR(tmp_wstr_1,
                strlen((char*)tmp_item),
                tmp_item,
                strlen((char*)tmp_item),
                strlen((char*)tmp_item));
            MMIAPICOM_Wstrncat(wstr_position_result,position_ok.wstr_ptr,position_ok.wstr_len);
        }
        MMIAPICOM_Wstrncat(wstr_position_result,tmp_wstr_1,strlen(tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_position_result);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_position_result;
        GUILIST_AppendItem(ctrl_id,&item_info);

        //纬度 经度
        SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
        SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
        sprintf(tmp_item, "%s:%f  %s:%f", "N", gpsSatelliteInfo->gps_info.latitude,"E", gpsSatelliteInfo->gps_info.longitude);
        MMI_STRNTOWSTR( tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
        GUILIST_AppendItem(ctrl_id,&item_info);
    }
    else
    {
        //已经定位
        SCI_TRACE_LOW("[ENG][GPS]have positioned");
        MMI_GetLabelTextByLang(TXT_ENG_HAVE_POSITIONED,&position_ok);
        sprintf(tmp_item, "  %s:%d", "TTFF", p_fix_ind->ttff_time);
        MMI_STRNTOWSTR(tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        MMIAPICOM_Wstrncat(wstr_position_result,position_ok.wstr_ptr,position_ok.wstr_len);
        MMIAPICOM_Wstrncat(wstr_position_result,tmp_wstr_1,strlen(tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_position_result);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_position_result;
        GUILIST_AppendItem(ctrl_id,&item_info);

        //纬度 经度
        SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
        SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
        sprintf(tmp_item, "%s:%f  %s:%f", "N", p_fix_ind->latitude,"E", p_fix_ind->longitude);
        MMI_STRNTOWSTR( tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
        GUILIST_AppendItem(ctrl_id,&item_info);
    }

    //append satellite info
    for(i=0;i<gpsSatelliteInfo->satellite_list_info.satellite_num;i++)
    {
        //GPS星座
        if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GPS_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GPS_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GPS;
            GPS_Num += 1;
        }
        //北斗星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_BEIDOU_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_BEIDOU_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_BEIDOU;
            BD_Num += 1;
        }
        //Galieo星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GALIEO_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GALIEO_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GALIEO;
            GALIEO_Num += 1;
        }
        //GLONASS星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GLONASS_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GLONASS_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GLONASS;
            GLONASS_Num += 1;
        }
        // Satellite Identifier  and CN0
        SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
        SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
        switch(satellite_type)
        {
            case GNSS_SATELLITE_TYPE_GPS:
                sprintf(tmp_item, "%s [%d]:%d", "GPS", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_BEIDOU:
                sprintf(tmp_item, "%s [%d]:%d", "Beidou", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_GALIEO:
                sprintf(tmp_item, "%s [%d]:%d", "Galieo", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_GLONASS:
                sprintf(tmp_item, "%s [%d]:%d", "Glonass", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            default:
                SCI_TRACE_LOW("[ENG] [GPS]:other satellite type");
                break;
        }
        MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
        GUILIST_InsertItem(ctrl_id, &item_info,i+6);
        SCI_TRACE_LOW("[ENG] [GPS]: AppendSatelliteListItem %s\n",tmp_item);
    }

    //GPS卫星数量
    SCI_TRACE_LOW("[ENG] [GPS]: GPS satellite num:%d\n",GPS_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_gps_num,temp_gps,MMIAPICOM_Wstrlen(temp_gps));
    MMI_GetLabelTextByLang(TXT_ENG_SATELLITE_NUM,&sat_num_str);
    MMIAPICOM_Wstrncat(wstr_gps_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GPS_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_gps_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_gps_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_gps_num;
    GUILIST_InsertItem(ctrl_id, &item_info,2);

    //北斗卫星数量
    SCI_TRACE_LOW("[ENG] [GPS]: Beidou satellite num:%d\n",BD_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_bd_num,temp_bd,MMIAPICOM_Wstrlen(temp_bd));
    MMIAPICOM_Wstrncat(wstr_bd_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", BD_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_bd_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_bd_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_bd_num;
    GUILIST_InsertItem(ctrl_id, &item_info,3);

    //Galieo卫星数量
    SCI_TRACE_LOW("[ENG] [GPS]: Galieo satellite num:%d\n",GALIEO_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_galieo_num,temp_galieo,MMIAPICOM_Wstrlen(temp_galieo));
    MMIAPICOM_Wstrncat(wstr_galieo_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GALIEO_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_galieo_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_galieo_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_galieo_num;
    GUILIST_InsertItem(ctrl_id, &item_info,4);

    //GLONASS卫星数量
    SCI_TRACE_LOW("[ENG] [GPS]: Glonass satellite num:%d\n",GLONASS_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_glonass_num,temp_glonass,MMIAPICOM_Wstrlen(temp_glonass));
    MMIAPICOM_Wstrncat(wstr_glonass_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GLONASS_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_glonass_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_glonass_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_glonass_num;
    GUILIST_InsertItem(ctrl_id, &item_info,5);

    MMK_PostMsg(MMIENG_TEST_GPS_SATELLITEINFO_WIN_ID,MSG_FULL_PAINT,PNULL,0);
}
#endif
LOCAL void InitSatelliteList(MMI_CTRL_ID_T  ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T item_data= {0};
    wchar tmp_wstr_1[] = {'w', 'a', 'i', 't', 'i', 'n', 'g', 0};

    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;     
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_wstr_1);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;

    item_info.item_data_ptr = &item_data;
    GUILIST_AppendItem(ctrl_id, &item_info);    
}

LOCAL MMI_RESULT_E EngGpstTestSatelliteInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;  

    TRACE_APP_ENGINEERMODE("[GPS]: MMI_MESSAGE_ID_E:%d\n",msg_id);
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_ENGINEERMODE("open window satellite_status_flag is true, start MNGPS_ReadInfoReq");
            GUILIST_SetMaxItem(MMIENG_LISTBOX_CTRL_ID, 140, FALSE );
            GUILIST_SetNeedHiLightBar(MMIENG_LISTBOX_CTRL_ID,FALSE);
            MMK_SetAtvCtrl(win_id,MMIENG_LISTBOX_CTRL_ID);
            InitSatelliteList(MMIENG_LISTBOX_CTRL_ID);
            //MNGPS_ReadInfoReq(0);
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_read_info();
#endif
             //the first request has no response so start timer to avoid null display 
            //Gps_StartRequestSatInfoTimer(0);
        }
        break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(MMIENG_TEST_GPS_SATELLITEINFO_WIN_ID);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        {
            TRACE_APP_ENGINEERMODE(" refresh satellite information, start MNGPS_ReadInfoReq");
            //MNGPS_ReadInfoReq(0);
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_read_info();
#endif
        }
        break;   
        case MSG_KEYUP_UP:
        case MSG_KEYDOWN_UP:
        case MSG_KEYLONG_UP:
        {
            uint16 top_item = GUILIST_GetTopItemIndex(MMIENG_LISTBOX_CTRL_ID);
            GUILIST_SetTopItemIndex(MMIENG_LISTBOX_CTRL_ID, (top_item - 1));
            MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL);
        }
            break;
        case MSG_KEYUP_DOWN:
        case MSG_KEYDOWN_DOWN:
        case MSG_KEYLONG_DOWN:
        {
            uint16 top_item = GUILIST_GetTopItemIndex(MMIENG_LISTBOX_CTRL_ID);
            GUILIST_SetTopItemIndex(MMIENG_LISTBOX_CTRL_ID, (top_item + 1));
            MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL);
        }
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

WINDOW_TABLE(MMIENG_TEST_GPS_SATELLITEINFO_TAB) = 
{
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32)EngGpstTestSatelliteInfoWinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_SATELLITEINFO_WIN_ID),
    WIN_TITLE(TXT_ENG_SATALITE_INFO),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_LISTBOX_CTRL_ID),
    //WIN_SOFTKEY(TXT_GPS_REFRESH, TXT_NULL, STXT_RETURN),
    END_WIN
};
/****************************************************************************/
/*****************************GPS&Satellite information test end***************** /
/****************************************************************************/

/************************************************************************/
/**********************GPS CIRCLE TEST START*****************************/
/************************************************************************/
#define EDIT_MAX_LEN (3)
#define EDIT_PARAM_SET_MAX_NUM (3*2)
#define PI 3.1415926
#define RADIUS_EARTH 6371004
typedef enum
{
    CIRCLE_TEST_PARAM_SET = 0,
    CIRCLE_TEST_START_MODE,
    CIRCLE_TEST_START_GPS,
    CIRCLE_TEST_STOP_GPS,
    CIRCLE_TEST_SHOW_DETAIL,
    CIRCLE_TEST_ITEM_MAX
} GPS_CIRCLE_TEST_INDEX_E;

typedef struct _GPS_CIRCLE_TEST_PARAM_SET_
{
  uint16 circle_times;//every mode test times
  uint16 test_interval;//between obtain fix info to next start
  //uint16 transfer_interval;//time of save log if abtain the fix info
  uint16 ttff_timeout;//the max time of abtain fix info   
}GPS_CIRCLE_TEST_PARAM_SET_ST;

typedef struct _GPS_CIRCLE_TEST_START_MODE_PARAM_ST_
{
  BOOLEAN is_select;
  uint16 current_times;
  uint32 last_ttff_time;
  uint32 total_ttff_time;
  uint32 ttff_timeout_times;
}GPS_CIRCLE_TEST_START_MODE_PARAM_ST;

typedef enum 
{
    APP_GPS_GNSS_MODE_GPS_GLONASS,      //GPS+Glonass
    APP_GPS_GNSS_MODE_GPS_BDS,      //GPS+BeiDou
    APP_GPS_GNSS_MODE_GPS,    //GPS only
    APP_GPS_GNSS_MODE_BDS,  //BeiDou Only
    APP_GPS_GNSS_MODE_GLONASS,   //Glonass Only
    APP_GPS_GNSS_MODE_GPS_BDS_GALILEO,  //GPS+BeiDou+Galileo
    APP_GPS_GNSS_MODE_GPS_B1C_GLONASS_GALILEO,  //GPS+BeiDou+Galileo+Glonass
    APP_GPS_GNSS_MODE_MAX
}APP_GPS_GNSS_MODE_ENUM;

GPS_CIRCLE_TEST_PARAM_SET_ST g_param_set_data = {1,20,50};
GPS_CIRCLE_TEST_START_MODE_PARAM_ST g_start_gps_data[4] = {{FALSE,0,0,0,0},{TRUE,0,0,0,0},{FALSE,0,0,0,0},{FALSE,0,0,0,0}};
BOOLEAN g_gnss_mode_data[APP_GPS_GNSS_MODE_MAX]={1,0,0,0,0,0};

//BOOLEAN g_is_gps_start = FALSE;// 用来决定start 和stop 安扭是否置灰，同时决定参数设置窗口是否能进入
uint8 g_cur_start_mode = 0xff;//用来决定当前循环测试到哪一个mode
BOOLEAN g_have_gps_stop_cnf = FALSE;//用来决定time out之后，再上来fix ind消息是否要接收。
BOOLEAN g_is_saved_data_currentCircleTime = FALSE;
BOOLEAN g_isCircleTest_finish = FALSE;
LOCAL uint8  g_ttff_timeout_time_id   = 0;//ttff timeout detect timer
LOCAL uint8  g_next_start_gps_timeout_time_id   = 0;//param interval determin this timer
#ifdef UAL_GNSS_SUPPORT
LOCAL ual_gnss_read_info_cnf_t g_read_info_t = {0};
LOCAL BOOLEAN g_is_registered = FALSE;
#endif
LOCAL BOOLEAN EngGps_CircleTest_JudgeIfNeedStartGps(void);
LOCAL BOOLEAN EngGps_WriteCircleTestResultTitle(void);
LOCAL BOOLEAN EngGps_WriteCircleTestResultNA(void);

LOCAL void EngAppendGpsGnssMode(MMI_CTRL_ID_T current_ctrl_id,BOOLEAN is_update)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar tmp_GpsGlo_wstr[8] = {'G', 'P', 'S', '+','G','L','O',0};
    wchar tmp_GpsBds_wstr[8] = {'G', 'P', 'S', '+','B','D','S',0};
    wchar tmp_Gps_wstr[9] = {'G', 'P', 'S',' ','O','N','L','Y',0};
    wchar tmp_Bds_wstr[9] = {'B','D','S',' ','O','N','L','Y',0};
    wchar tmp_Glo_wstr[9] = {'G', 'L', 'O',' ','O','N','L','Y',0};
    wchar tmp_GpsBdsGal_wstr[12] = {'G','P','S','+','B','D','S','+','G','A','L',0};
    wchar tmp_GpsBdsGalGlon_wstr[16] = {'G','P','S','+','B','1','C','+','G','L','O','+','G','A','L',0};
    item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
    item_t.item_data_ptr = &item_data;
    //GPS+Glonass
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_GpsGlo_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_GpsGlo_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 0,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_GLONASS]);

    //GPS_BDS
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_GpsBds_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_GpsBds_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 1,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_BDS]);

    //GPS
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_Gps_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_Gps_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 2,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS]);

    //BDS
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_Bds_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_Bds_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 3,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_BDS]);

    //Glonass
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_Glo_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_Glo_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 4,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_GLONASS]);

    //GPS+BDS+Galileo
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_GpsBdsGal_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_GpsBdsGal_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 5,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_BDS_GALILEO]);

    //GPS+B1C+Glonass+Galileo
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_GpsBdsGalGlon_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_GpsBdsGalGlon_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, 6,GUIITEM_STATE_SELECTED, g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_B1C_GLONASS_GALILEO]);
}

LOCAL void EngAppendCircleTestOneLineItemContent(MMI_CTRL_ID_T current_ctrl_id)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    //MMIGPS_STATUS_EUM gps_state = MMIGPS_CurrentStatus_Get();
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_state_e    gps_state = ual_gnss_get_cur_state();
#endif
    item_t.item_style    = GUIITEM_STYLE_1STR_ENG;
    item_t.item_data_ptr = &item_data;
    //Param set
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_CIRCLE_PARAM_SET;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    //Start mode
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_START_MODE_SET;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    //Start gps
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_START_GPS;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    //Stop gps
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_STOP_GPS;
    GUILIST_AppendItem(current_ctrl_id, &item_t);

    //if(GPS_STARTED_STATE == gps_state)
#ifdef UAL_GNSS_SUPPORT
    TRACE_APP_ENGINEERMODE("[GPS] current gps state:%d", gps_state);
    if(UAL_GNSS_STATE_WORK == gps_state)
    {
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_STOP_GPS, FALSE);
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_START_GPS, TRUE);
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_PARAM_SET, TRUE);
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_START_MODE, TRUE);

    }
    //else if((GPS_IDLE_STATE == gps_state) || (GPS_STOPED_STATE == gps_state)||
    //(GPS_INIT_READY_STATE == gps_state)|| (GPS_DOWNLOAD_STATE == gps_state))
    else if(UAL_GNSS_STATE_IDLE == gps_state || UAL_GNSS_STATE_READY == gps_state)
    {
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_START_GPS, FALSE);
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_PARAM_SET, FALSE);
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_START_MODE, FALSE);
        GUILIST_SetItemGray(current_ctrl_id, CIRCLE_TEST_STOP_GPS, TRUE);
    }
#endif
}

LOCAL void EngAppendStartModeOneLineItemContent(MMI_CTRL_ID_T current_ctrl_id,ENTER_START_MODE_TYPE start_mode_from)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar tmp_warm_wstr[] = {'W', 'a', 'r', 'm',0};
    wchar tmp_cold_wstr[] = {'C', 'o', 'l', 'd',0};
    wchar tmp_hot_wstr[] = {'H', 'o', 't',0};
    wchar tmp_factory_wstr[] = {'F', 'a', 'c', 't','o','r','y',0};
    uint8 i = 0;
    BOOLEAN start_mode[GNSS_TEST_START_MODE_MAX]={TRUE, FALSE, FALSE, FALSE,};

    if(ENTER_START_MODE_FROM_TRACKING_TEST == start_mode_from)
    {
        item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
        for( i = 0; i< GNSS_TEST_START_MODE_MAX; i++)
        {
            if(g_start_mode_for_tracking_test == i)
            {
                start_mode[i] = TRUE;
            }
            else
            {
                start_mode[i] = FALSE;
            }
        }
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_1CHECK_ENG;
        for( i = 0; i< GNSS_TEST_START_MODE_MAX; i++)
        {
            if(TRUE == g_start_gps_data[i].is_select)
            {
                start_mode[i] = TRUE;
            }
            else
            {
                start_mode[i] = FALSE;
            }
        }
    }
    item_t.item_data_ptr = &item_data;

    //hot
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_hot_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_hot_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, GNSS_TEST_START_MODE_HOT,GUIITEM_STATE_SELECTED, start_mode[GNSS_TEST_START_MODE_HOT]);

    //cold
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_cold_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_cold_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, GNSS_TEST_START_MODE_COLD,GUIITEM_STATE_SELECTED, start_mode[GNSS_TEST_START_MODE_COLD]);

    //warm
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_warm_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_warm_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, GNSS_TEST_START_MODE_WARM,GUIITEM_STATE_SELECTED, start_mode[GNSS_TEST_START_MODE_WARM]);

    //factory
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(tmp_factory_wstr);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_factory_wstr;
    GUILIST_AppendItem(current_ctrl_id, &item_t);
    CTRLLIST_SetItemStateById(current_ctrl_id, GNSS_TEST_START_MODE_FACTORY,GUIITEM_STATE_SELECTED, start_mode[GNSS_TEST_START_MODE_FACTORY]);
}

LOCAL void _EngGpsHandleStartNextStartGpsTimeout(uint8 timer_id,uint32 param)
{
    uint32  i=0;
    uint8 cur_start_mode = (uint8)param;
    //MMIGPS_START_GPS_PARAM_T start_param = {0}; 
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_start_param_t  gnss_start_param = {0};
#endif
    if (g_next_start_gps_timeout_time_id == timer_id)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:\n");    
        MMK_StopTimer(g_next_start_gps_timeout_time_id);
        g_next_start_gps_timeout_time_id = 0;
        g_have_gps_stop_cnf = FALSE;
        //g_is_gps_start = TRUE;
        //MMIGPS_Start(&start_param,_EngHandleGpsMessageCallback);
#ifdef UAL_GNSS_SUPPORT
        gnss_start_param.start_mode = cur_start_mode;
        ual_gnss_start(&gnss_start_param);
#endif
    }
}

LOCAL void EngGpsStartNextStartGpsTimeOutTimer(uint8 cur_start_mode,uint32 timeout)
{
    if (NULL != g_next_start_gps_timeout_time_id)
    {
        MMK_StopTimer(g_next_start_gps_timeout_time_id);
        g_next_start_gps_timeout_time_id = 0;
    }
    TRACE_APP_ENGINEERMODE("[GPS]:timeout:%d \n",timeout);
    g_next_start_gps_timeout_time_id = MMK_CreateTimerCallback((timeout*1000), _EngGpsHandleStartNextStartGpsTimeout, cur_start_mode, FALSE);
}


LOCAL void _EngGpsHandleTtffTimeout(uint8 timer_id,uint32 param)
{
    uint32  i=0;
    
    if (g_ttff_timeout_time_id == timer_id)
    {
        MMK_StopTimer(g_ttff_timeout_time_id);
        g_ttff_timeout_time_id = 0;
        //timer out 次数+1
        g_start_gps_data[g_cur_start_mode].ttff_timeout_times = g_start_gps_data[g_cur_start_mode].ttff_timeout_times +1;
        TRACE_APP_ENGINEERMODE("[GPS]: g_cur_start_mode:%d,ttff current_times:%d,ttff_timeout_times:%d TIME OUT!\n",g_cur_start_mode,g_start_gps_data[g_cur_start_mode].current_times,g_start_gps_data[g_cur_start_mode].ttff_timeout_times);

        //save result to auto test file
        EngGps_WriteCircleTestResultNA();

        //judge if need to start gps
        if(FALSE == EngGps_CircleTest_JudgeIfNeedStartGps())
        {
            TRACE_APP_ENGINEERMODE("[GPS]:no need to start next\n");
            //stop GPS刷新界面
            //MMIGPS_Stop();
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_stop();
#endif
            g_isCircleTest_finish = TRUE;
        }
        else
        {
            TRACE_APP_ENGINEERMODE("[GPS]:need to start next,stop this first\n");
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_stop();
#endif
            //EngGpsStartNextStartGpsTimeOutTimer(g_cur_start_mode,g_param_set_data.test_interval);
        }
    }
}

LOCAL void EngGpsStartTtffTimeOutTimer(uint32 timeout)
{
    if (NULL != g_ttff_timeout_time_id)
    {
        MMK_StopTimer(g_ttff_timeout_time_id);
        g_ttff_timeout_time_id = 0;
    }
    TRACE_APP_ENGINEERMODE("[GPS]:timeout:%d \n",timeout);    
    g_ttff_timeout_time_id = MMK_CreateTimerCallback((timeout*1000), _EngGpsHandleTtffTimeout, 0, FALSE);
}

LOCAL BOOLEAN EngGps_CircleTest_JudgeIfNeedStartGps(void)
{
    uint32 i=0;
    BOOLEAN  next_start_mode= FALSE;
    TRACE_APP_ENGINEERMODE("[GPS]: g_cur_start_mode:%d,current_times:%d,circle_times:%d\n",g_cur_start_mode,g_start_gps_data[g_cur_start_mode].current_times,g_param_set_data.circle_times);
    if(g_start_gps_data[g_cur_start_mode].current_times < g_param_set_data.circle_times)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: g_cur_start_mode:%d,current_times:%d,circle_times:%d,current times need continue start!\n",g_cur_start_mode,g_start_gps_data[g_cur_start_mode].current_times,g_param_set_data.circle_times);
        //当前mode需要继续start,将次数加1
        g_start_gps_data[g_cur_start_mode].current_times ++;
        return TRUE;
        //从上一次的stop 到开启下一次的sart 需要延时一段时间
       // EngGpsStartNextStartGpsTimeOutTimer(g_cur_start_mode,g_param_set_data.test_interval);
    }
    else//需要检测是否还有其他的start mode
    {
        //close last startmode log first
        CloseCircleTestLogFile();

        g_cur_start_mode = g_cur_start_mode + 1;
        if(GNSS_TEST_START_MODE_FACTORY < g_cur_start_mode)
        {
            TRACE_APP_ENGINEERMODE("[GPS]: User Select Start Mode Have Test Done!\n");
            return FALSE;
        }else
        {
            //寻找下一个需要start gps的start mode
            for(i = g_cur_start_mode; i<=GNSS_TEST_START_MODE_FACTORY;i++)
            {
                if(TRUE == g_start_gps_data[i].is_select)
                {
                    g_cur_start_mode = i;
                    next_start_mode = TRUE;
                    TRACE_APP_ENGINEERMODE("[GPS]: cur_start_mode change,g_cur_start_mode:%d\n",g_cur_start_mode);
                    break;
                }
            }
            if(FALSE == next_start_mode)
            {
                
                TRACE_APP_ENGINEERMODE("[GPS]: User Select Start Mode Have Test Done!\n");
                return FALSE;
            }
            else
            {
                OpenCircleTestLogFile((GNSS_TEST_START_MODE_TYPE_E)g_cur_start_mode);
                //write circle test file title first
                EngGps_WriteCircleTestResultTitle();

                //开始启动新的start mode
                g_start_gps_data[g_cur_start_mode].current_times = 1;
                //从上一次的stop 到开启下一次的sart 需要延时一段时间
                return TRUE;
                //EngGpsStartNextStartGpsTimeOutTimer(g_cur_start_mode,g_param_set_data.test_interval);
            }
        }
    }
    return TRUE;
}

LOCAL void EngGps_CircleTest_HandleStartCnf(void)
{

    if(MMK_IsOpenWin(MMIENG_TEST_GPS_CRICLETEST_WIN_ID))
    {
        //刷新界面
        MMK_PostMsg(MMIENG_TEST_GPS_CRICLETEST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
    }

   EngGpsStartTtffTimeOutTimer(g_param_set_data.ttff_timeout);

   g_isCircleTest_finish = FALSE;
}
LOCAL void EngGps_CircleTest_HandleStopCnf(void)
{
    TRACE_APP_ENGINEERMODE("Enter\n");
    g_have_gps_stop_cnf = TRUE;
    if (TRUE ==  g_isCircleTest_finish)
    {
        TRACE_APP_ENGINEERMODE("circle test finished\n");
        g_is_circle_test = FALSE;
        CloseCircleTestLogFile();
    }
    else
    {
        TRACE_APP_ENGINEERMODE("circle test not finish,start next\n");
        EngGpsStartNextStartGpsTimeOutTimer(g_cur_start_mode,g_param_set_data.test_interval);
    }
    if(MMK_IsOpenWin(MMIENG_TEST_GPS_CRICLETEST_WIN_ID))
    {
        //刷新界面
        MMK_PostMsg(MMIENG_TEST_GPS_CRICLETEST_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
    }

}

LOCAL  BOOLEAN  EngGps_CircleTest_HandleFixIndMsg(uint32 ttff_time)
{
    if(NULL != g_ttff_timeout_time_id)
    {
        //ttff 没有time out，GPS 成功定位
        MMK_StopTimer(g_ttff_timeout_time_id);
        g_ttff_timeout_time_id = 0;
        //保存ttff time
        g_start_gps_data[g_cur_start_mode].total_ttff_time = g_start_gps_data[g_cur_start_mode].total_ttff_time + ttff_time;
        //保存last ttff time
        g_start_gps_data[g_cur_start_mode].last_ttff_time = ttff_time;
        if(FALSE == EngGps_CircleTest_JudgeIfNeedStartGps())
        {
            TRACE_APP_ENGINEERMODE("[GPS]: position success,no need to start next");
            //stop Gps 刷新界面
            //EngGpsJudgeForStopGps();
            //MMIGPS_Stop();
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_stop();
#endif
            g_isCircleTest_finish = TRUE;
            //g_is_gps_start = FALSE;
        }else
        {
            TRACE_APP_ENGINEERMODE("[GPS]: position success,need to start next,stop this first");
            //EngGpsStartNextStartGpsTimeOutTimer(g_cur_start_mode,g_param_set_data.test_interval);
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_stop();
#endif
        }
    }
}
/*****************************************************************************/
//  Description : start record autotest log
//  Global resource dependence : none
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL void OpenCircleTestLogFile(GNSS_TEST_START_MODE_TYPE_E start_mode)
{
    uint8 fileName[GUILIST_STRING_MAX_NUM] = "GPSCircleTest";
    TRACE_APP_ENGINEERMODE("[GPS]: enter OpenCircleTestLogFile , start_mode:%d", start_mode);
    switch(start_mode)
    {
        case GNSS_TEST_START_MODE_HOT:
            strcat(fileName, "-Hot");
            break;
        case GNSS_TEST_START_MODE_WARM:
            strcat(fileName, "-Warm");
        break;
        case GNSS_TEST_START_MODE_COLD:
            strcat(fileName, "-Cold");
            break;
        case GNSS_TEST_START_MODE_FACTORY:
            strcat(fileName, "-Fac");
            break;
        default:
            break;
    }

    TRACE_APP_ENGINEERMODE("[GPS]: enter OpenCircleTestLogFile , fileName:%s", fileName);

    MMIENG_LogOpen(fileName,LOG_SAVE_CIRCLETEST);

}
/*****************************************************************************/
//  Description : stop request output NMEA log
//  Global resource dependence : none
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL void CloseCircleTestLogFile(void)
{
    TRACE_APP_ENGINEERMODE("[GPS]: enter CloseCircleTestLogFile");
    MMIENG_LogClose(LOG_SAVE_CIRCLETEST);
}
// change longitude and latitude to angle first
LOCAL double EngGps_ChangeLongLatToAngle(float LongLatData)
{
    double angle = 0;
    angle = PI * LongLatData/180;
    TRACE_APP_ENGINEERMODE("[GPS]angle:%f", angle);

    return angle;
}

LOCAL double EngGps_ChangePositionToDistance(float lati_test,float longi_test,float lati_ref, float longi_ref)
{
    double lati_test_angle = 0;
    double longi_test_angle = 0;
    double lati_ref_angle = 0;
    double longi_ref_angle = 0;
    double include_angle = 0;
    double distance_btp = 0;

    //change all latitude and longitude to angle first
    lati_test_angle = EngGps_ChangeLongLatToAngle(lati_test);
    longi_test_angle = EngGps_ChangeLongLatToAngle(longi_test);
    lati_ref_angle = EngGps_ChangeLongLatToAngle(lati_ref);
    longi_ref_angle = EngGps_ChangeLongLatToAngle(longi_ref);

    //change angle to raian
    include_angle = acos(cos(lati_test_angle)*cos(lati_ref_angle)*
                    cos(longi_ref_angle - longi_test_angle)+
                    sin(lati_test_angle)* sin(lati_ref_angle));
    distance_btp = include_angle * RADIUS_EARTH;
    TRACE_APP_ENGINEERMODE("[GPS]distance_btp:%lf", distance_btp);

    return distance_btp;
}
#ifdef UAL_GNSS_SUPPORT
LOCAL BOOLEAN EngGps_handleCircleTestData(ual_gnss_fix_ind_t *gps_info)
{
    BOOLEAN res = FALSE;
    double distance = 0;
    uint32 newData_len = 0;
    uint8 text_buf[MAX_NMEA_LOG_BUFFER *8 +1] = {0};
    uint8 startmode_buf[MAX_NMEA_LOG_BUFFER +1] = {0};

    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_handleCircleTestData , ttff: %d,longitude:%f, latitude:%f", gps_info->ttff_time,gps_info->longitude, gps_info->latitude);
    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_handleCircleTestData , setted standard longitude:%f, latitude:%f",g_longtitude_angle, g_latitude_angle);

    //calc distance first
    distance = EngGps_ChangePositionToDistance(gps_info->latitude,gps_info->longitude,(float)g_latitude_angle, (float)g_longtitude_angle);

    SCI_MEMSET(startmode_buf, 0x00,MAX_NMEA_LOG_BUFFER +1);
    switch(g_cur_start_mode)
    {
        case GNSS_TEST_START_MODE_WARM:
            sprintf(startmode_buf, "%s","warm");
            break;
        case GNSS_TEST_START_MODE_COLD:
            sprintf(startmode_buf, "%s","cold");
            break;
        case GNSS_TEST_START_MODE_HOT:
            sprintf(startmode_buf, "%s","hot");
            break;
        case GNSS_TEST_START_MODE_FACTORY:
            sprintf(startmode_buf, "%s","factory");
            break;
        default:
            break;
    }

    SCI_MEMSET(text_buf, 0x00,MAX_NMEA_LOG_BUFFER *8 +1);
    // current times     current start mode    ttff    distance
    sprintf(text_buf, "%d    %s    %d    %lf\r\n",g_start_gps_data[g_cur_start_mode].current_times, startmode_buf, gps_info->ttff_time, distance);
    newData_len = SCI_STRLEN(text_buf);

    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_handleCircleTestData,write new data to file, text_buf:%s", text_buf);
    res = MMIENG_WriteLOGFile(text_buf,newData_len,LOG_SAVE_CIRCLETEST);
    return res;
}
#endif
LOCAL BOOLEAN EngGps_WriteCircleTestResultNA(void)
{
    BOOLEAN res = FALSE;
    uint32 text_len = 0;
    uint8 text_buf[MAX_NMEA_LOG_BUFFER*6 +1] = {0};
    uint8 startmode_buf[MAX_NMEA_LOG_BUFFER +1] = {0};

    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_WriteCircleTestResultNA ");
    SCI_MEMSET(startmode_buf, 0x00,MAX_NMEA_LOG_BUFFER +1);
    switch(g_cur_start_mode)
    {
        case GNSS_TEST_START_MODE_WARM:
            sprintf(startmode_buf, "%s","warm");
            break;
        case GNSS_TEST_START_MODE_COLD:
            sprintf(startmode_buf, "%s","cold");
            break;
        case GNSS_TEST_START_MODE_HOT:
            sprintf(startmode_buf, "%s","hot");
            break;
        case GNSS_TEST_START_MODE_FACTORY:
            sprintf(startmode_buf, "%s","factory");
            break;
        default:
            break;
    }

    SCI_MEMSET(text_buf, 0x00,MAX_NMEA_LOG_BUFFER*6 +1);
    // current times     current start mode    ttff    distance
    sprintf(text_buf, "%d    %s    N/A \r\n",g_start_gps_data[g_cur_start_mode].current_times, startmode_buf);
    text_len = SCI_STRLEN(text_buf);

    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_WriteCircleTestResultNA,text:%s", text_buf);
    res = MMIENG_WriteLOGFile(text_buf,text_len,LOG_SAVE_CIRCLETEST);
    return res;
}

LOCAL BOOLEAN EngGps_WriteCircleTestResultTitle(void)
{
    BOOLEAN res = FALSE;
    uint32 text_len = 0;
    uint8 text_buf[MAX_NMEA_LOG_BUFFER*6 +1] = {0};

    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_WriteCircleTestResultTitle ");

    SCI_MEMSET(text_buf, 0x00,MAX_NMEA_LOG_BUFFER *6 +1);
    // current times     current start mode    ttff    distance
    sprintf(text_buf, "%s    %s    %s    %s\r\n","current_time", "startmode", "ttff_time", "distance");
    text_len = SCI_STRLEN(text_buf);

    TRACE_APP_ENGINEERMODE("[GPS]: EngGps_WriteCircleTestResultTitle,file title:%s", text_buf);
    res = MMIENG_WriteLOGFile(text_buf,text_len,LOG_SAVE_CIRCLETEST);
    return res;
}
LOCAL void HandleCircleTestStartGps(void)
{
    uint32                  i                = 0;
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_start_param_t  gnss_start_param = {0};
#endif
    //判断哪一个作为startmode
    for(i=0;i<GNSS_TEST_START_MODE_MAX;i++)
    {
        if(TRUE == g_start_gps_data[i].is_select)
        {
            g_cur_start_mode = i;
            TRACE_APP_ENGINEERMODE("[GPS]: First Start Mode:%d", g_cur_start_mode);
            break;
        }
    }
    if(g_cur_start_mode < GNSS_TEST_START_MODE_HOT || g_cur_start_mode >= GNSS_TEST_START_MODE_MAX)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: user not set start mode,default cold start!");
        g_cur_start_mode = GNSS_TEST_START_MODE_COLD;
    }
    //将是否启动置为true
    //g_is_gps_start = TRUE;
    // 清空每个mode 的timeout的次数，当前的次数，total ttff time，
    for(i=0; i< GNSS_TEST_START_MODE_MAX; i++)
    {
        g_start_gps_data[i].current_times = 0;
        g_start_gps_data[i].total_ttff_time = 0;
        g_start_gps_data[i].ttff_timeout_times = 0;
    }
    //当前为第一次启动
    g_start_gps_data[g_cur_start_mode].current_times = 1;
    g_have_gps_stop_cnf = FALSE;
#ifdef UAL_GNSS_SUPPORT
    gnss_start_param.start_mode = g_cur_start_mode;
    ual_gnss_start(&gnss_start_param);
#endif
    //set the flag
    g_is_circle_test = TRUE;
    OpenCircleTestLogFile((GNSS_TEST_START_MODE_TYPE_E)g_cur_start_mode);
    //write circle test file title first
    EngGps_WriteCircleTestResultTitle();
}

LOCAL void HandleCircleTestStopGps(void)
{
     //MMIGPS_Stop();
     ual_gnss_stop();
    //停止timer
     if (NULL != g_ttff_timeout_time_id)
     {
         MMK_StopTimer(g_ttff_timeout_time_id);
         g_ttff_timeout_time_id = 0;
     }
     if (NULL != g_next_start_gps_timeout_time_id)
     {
         MMK_StopTimer(g_next_start_gps_timeout_time_id);
         g_next_start_gps_timeout_time_id = 0;
     }
     //finish circle test
     if(TRUE == g_is_circle_test)
     {
         g_isCircleTest_finish = TRUE;   //bug2096415
     }
}

LOCAL MMI_RESULT_E EngGpsCircleTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID);
            GUILIST_SetMaxItem( ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID, 5, FALSE );
            EngAppendCircleTestOneLineItemContent(ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID);
            //judge set start or stop diable
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                uint16 cur_index=0;
                cur_index = GUILIST_GetCurItemIndex(ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID);
                TRACE_APP_ENGINEERMODE("[GPS]: cur_index:%d\n",cur_index);
                switch(cur_index)
                {
                    case CIRCLE_TEST_PARAM_SET:
                        //when start gps ,the window can not access
                        //if(GPS_STARTED_STATE != MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
                        if(UAL_GNSS_STATE_WORK != ual_gnss_get_cur_state())
                        {
                            MMK_CreateWin((uint32 *)MMIENG_TEST_GPS_CIRCLETEST_PARAMSET_TAB, PNULL);
                        }
#endif
                    break;
                    case CIRCLE_TEST_START_MODE:
                        // when start gps, the window can not access
                        //if(GPS_STARTED_STATE != MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
                        if(UAL_GNSS_STATE_WORK != ual_gnss_get_cur_state())
                        {
                            MMK_CreateWin((uint32 *)MMIENG_TEST_GPS_CIRCLETEST_STARTMODE_TAB, PNULL);
                        }
#endif
                    break;
                    case CIRCLE_TEST_START_GPS:
                            //if(GPS_STARTED_STATE != MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
                        if(UAL_GNSS_STATE_WORK != ual_gnss_get_cur_state())
                        {
                            HandleCircleTestStartGps();   //bug2067923
                        }
                        else
                        {
                            TRACE_APP_ENGINEERMODE("[GPS]: gps is work, should Not start again!\n");
                        }
#endif
                    break;
                    case CIRCLE_TEST_STOP_GPS:
                    {
                        //if(GPS_STARTED_STATE == MMIGPS_CurrentStatus_Get())
#ifdef UAL_GNSS_SUPPORT
                        if(UAL_GNSS_STATE_WORK == ual_gnss_get_cur_state())
                        {
                            HandleCircleTestStopGps();   //bug2067923
                        }
                        else
                        {
                            TRACE_APP_ENGINEERMODE("[GPS]: gps is stoped, should Not stop again!\n");
                        }
#endif
                    }
                    break;
                    case CIRCLE_TEST_SHOW_DETAIL:
                    break;
                    default:
                        result = MMI_RESULT_FALSE;
                    break;
                }

            }
            break;
        case MSG_FULL_PAINT:
        {
            
            GUILIST_RemoveAllItems(ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID);
            EngAppendCircleTestOneLineItemContent(ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID);
          
        }
        break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;  
}

LOCAL MMI_RESULT_E EngGpsCircleParamSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    TRACE_APP_ENGINEERMODE("[GPS]Enter,msg_id:%d",msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    label_temp_str = {0};
            MMI_STRING_T    edit_temp_str = {0};
            wchar tmp_wstr_1[EDIT_PARAM_SET_MAX_NUM + 1] = {0};
            uint8 tmp_string[EDIT_PARAM_SET_MAX_NUM + 1] = {0};

            wchar time_str[] = {'T', 'i', 'm', 'e','s','(' ,'1', '-', '9','9','9',')',0};
            wchar interval_str[] = {'I', 'n', 't', 'e','r','v','a','l', '(','1', '-', '9','9','9',')','(','s',')',0};
            //wchar transfer_interval_str[] = {'T','r','a','n','s','f','e','r',' ','I', 'n', 't', 'e','r','v','a','l','(','1', '-', '9','9','9',')',0};
            wchar ttff_timeout_str[] = {'T','t','f','f',' ','T','i','m','e', 'o', 'u', 't', '(','1', '-', '9','9','9',')','(','s',')',0};
            MMK_SetAtvCtrl(win_id,ID_ENG_GPS_TEST_CIRCLETEST_TIME_EDIT_CTRL_ID);

            TRACE_APP_ENGINEERMODE("[GPS]open win");

            label_temp_str.wstr_ptr = time_str;
            label_temp_str.wstr_len = MMIAPICOM_Wstrlen(time_str);
            GUILABEL_SetText(ID_ENG_GPS_TEST_CIRCLETEST_TIME_LABEL_CTRL_ID, &label_temp_str, FALSE);

            SCI_MEMSET(&label_temp_str,0x0,sizeof(MMI_STRING_T));
            label_temp_str.wstr_ptr = interval_str;
            label_temp_str.wstr_len = MMIAPICOM_Wstrlen(interval_str);
            GUILABEL_SetText(ID_ENG_GPS_TEST_CIRCLETEST_INTERVAL_LABEL_CTRL_ID, &label_temp_str, FALSE);

            /*SCI_MEMSET(&label_temp_str,0x0,sizeof(MMI_STRING_T));
            label_temp_str.wstr_ptr = transfer_interval_str;
            label_temp_str.wstr_len = MMIAPICOM_Wstrlen(transfer_interval_str);
            GUILABEL_SetText(ID_ENG_GPS_TEST_CIRCLETEST_TRANSFERINTERVAL_LABEL_CTRL_ID, &label_temp_str, FALSE);*/

            SCI_MEMSET(&label_temp_str,0x0,sizeof(MMI_STRING_T));
            label_temp_str.wstr_ptr = ttff_timeout_str;
            label_temp_str.wstr_len = MMIAPICOM_Wstrlen(ttff_timeout_str);
            GUILABEL_SetText(ID_ENG_GPS_TEST_CIRCLETEST_TTFFTIMEOUT_LABEL_CTRL_ID, &label_temp_str, FALSE);
            // below abtain edit content
            //times
            sprintf((char *)tmp_string, "%d",g_param_set_data.circle_times);
            MMIAPICOM_StrToWstr(tmp_string, tmp_wstr_1);
            GUIEDIT_SetString(ID_ENG_GPS_TEST_CIRCLETEST_TIME_EDIT_CTRL_ID,tmp_wstr_1,MMIAPICOM_Wstrlen(tmp_wstr_1));
            //interval
            SCI_MEMSET(tmp_string, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(char));
            SCI_MEMSET(tmp_wstr_1, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(wchar));
            sprintf((char *)tmp_string, "%d",g_param_set_data.test_interval);
            MMIAPICOM_StrToWstr(tmp_string, tmp_wstr_1);
            GUIEDIT_SetString(ID_ENG_GPS_TEST_CIRCLETEST_INTERVAL_EDIT_CTRL_ID,tmp_wstr_1,MMIAPICOM_Wstrlen(tmp_wstr_1));
            //transfer interval
            /*SCI_MEMSET(tmp_string, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(char));
            SCI_MEMSET(tmp_wstr_1, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(wchar));
            sprintf((char *)tmp_string, "%d",g_param_set_data.transfer_interval);
            MMIAPICOM_StrToWstr(tmp_string, tmp_wstr_1);
            GUIEDIT_SetString(ID_ENG_GPS_TEST_CIRCLETEST_TRANSFERINTERVAL_EDIT_CTRL_ID,tmp_wstr_1,MMIAPICOM_Wstrlen(tmp_wstr_1));*/
            //ttff timeout
            SCI_MEMSET(tmp_string, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(char));
            SCI_MEMSET(tmp_wstr_1, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(wchar));
            sprintf((char *)tmp_string, "%d",g_param_set_data.ttff_timeout);
            MMIAPICOM_StrToWstr(tmp_string, tmp_wstr_1);
            GUIEDIT_SetString(ID_ENG_GPS_TEST_CIRCLETEST_TTFFTIMEOUT_EDIT_CTRL_ID,tmp_wstr_1,MMIAPICOM_Wstrlen(tmp_wstr_1));

        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                MMI_STRING_T times_edit_str = {0};
                MMI_STRING_T interval_edit_str = {0};
                //MMI_STRING_T transfer_interval_edit_str = {0};
                MMI_STRING_T ttff_timeout_edit_str = {0};
                char tmp_string_value[EDIT_PARAM_SET_MAX_NUM + 1] = {0};

                if(TRUE == g_is_circle_test && FALSE == g_isCircleTest_finish)
                {
                    TRACE_APP_ENGINEERMODE("[GPS]circle test not finish");
                    MMIPUB_OpenAlertWarningWin(TXT_CIRCLE_TEST_NOT_FINISH);
                    break;
                }

                //abtain string from every edit ctrl
                GUIEDIT_GetString(ID_ENG_GPS_TEST_CIRCLETEST_TIME_EDIT_CTRL_ID, &times_edit_str);
                GUIEDIT_GetString(ID_ENG_GPS_TEST_CIRCLETEST_INTERVAL_EDIT_CTRL_ID, &interval_edit_str);
                //GUIEDIT_GetString(ID_ENG_GPS_TEST_CIRCLETEST_TRANSFERINTERVAL_EDIT_CTRL_ID, &transfer_interval_edit_str);
                GUIEDIT_GetString(ID_ENG_GPS_TEST_CIRCLETEST_TTFFTIMEOUT_EDIT_CTRL_ID, &ttff_timeout_edit_str);
                //judge if have null string
                if((0 == times_edit_str.wstr_len)||(0 == interval_edit_str.wstr_len)||(0 == ttff_timeout_edit_str.wstr_len))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PARAM_SET_IS_ERROR);
                }else
                {
                    MMI_WSTRNTOSTR(tmp_string_value, (EDIT_PARAM_SET_MAX_NUM + 1), times_edit_str.wstr_ptr, EDIT_PARAM_SET_MAX_NUM, times_edit_str.wstr_len);
                    g_param_set_data.circle_times = (uint16)atoi(tmp_string_value);  

                    SCI_MEMSET(tmp_string_value, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(char));
                    MMI_WSTRNTOSTR(tmp_string_value, (EDIT_PARAM_SET_MAX_NUM + 1), interval_edit_str.wstr_ptr, EDIT_PARAM_SET_MAX_NUM, interval_edit_str.wstr_len);
                    g_param_set_data.test_interval =(uint16)atoi(tmp_string_value);  
                    
                    /*SCI_MEMSET(tmp_string_value, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(char));
                    MMI_WSTRNTOSTR(tmp_string_value, (EDIT_PARAM_SET_MAX_NUM + 1), transfer_interval_edit_str.wstr_ptr, EDIT_PARAM_SET_MAX_NUM, transfer_interval_edit_str.wstr_len);
                    g_param_set_data.transfer_interval = (uint16)atoi(tmp_string_value);*/

                    SCI_MEMSET(tmp_string_value, 0x00, (EDIT_PARAM_SET_MAX_NUM +1)*sizeof(char));
                    MMI_WSTRNTOSTR(tmp_string_value, (EDIT_PARAM_SET_MAX_NUM + 1), ttff_timeout_edit_str.wstr_ptr, EDIT_PARAM_SET_MAX_NUM, ttff_timeout_edit_str.wstr_len);
                    g_param_set_data.ttff_timeout = (uint16)atoi(tmp_string_value);

                    TRACE_APP_ENGINEERMODE("[GPS]circle_times:%d,test_interval:%d,ttff_time:%d",
                        g_param_set_data.circle_times,g_param_set_data.test_interval,g_param_set_data.ttff_timeout);
                    
                    if((0 ==  g_param_set_data.circle_times)||(0 == g_param_set_data.test_interval)||(0 == g_param_set_data.ttff_timeout))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_PARAM_SET_IS_ERROR);
                    }else
                    {
                        MMK_CloseWin(win_id);
                    }
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;  
}

LOCAL MMI_RESULT_E EngGpsCircleStartModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID);
            GUILIST_SetMaxItem( ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID, 4, FALSE );
            EngAppendStartModeOneLineItemContent(ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID,ENTER_START_MODE_FROM_CIRCLE_TEST);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                uint16 cur_index = 0;
                uint8  i = 0;
                if(TRUE == g_is_circle_test && FALSE == g_isCircleTest_finish)
                {
                    TRACE_APP_ENGINEERMODE("[GPS]circle test not finish");
                    MMIPUB_OpenAlertWarningWin(TXT_CIRCLE_TEST_NOT_FINISH);
                    break;
                }
                cur_index = GUILIST_GetCurItemIndex(ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID);
                if(TRUE == g_start_gps_data[cur_index].is_select)
                {
                    g_start_gps_data[cur_index].is_select = FALSE;
                }else
                {
                    g_start_gps_data[cur_index].is_select = TRUE;
                }
                if( GUILIST_GetItemState( GUILIST_GetItemPtrByIndex( ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID, cur_index ), GUIITEM_STATE_SELECTED ) )
                { //selected
                    CTRLLIST_SetItemStateById( ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID, cur_index, GUIITEM_STATE_SELECTED, FALSE );
                }
                else
                {
                    CTRLLIST_SetItemStateById( ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID, cur_index, GUIITEM_STATE_SELECTED, TRUE );
                }
                for(i=0;i<GNSS_TEST_START_MODE_MAX;i++)
                {
                    TRACE_APP_ENGINEERMODE("[GPS]cur_index:%d,is_select:%d",i,g_start_gps_data[i].is_select);
                }
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            break;
        case MSG_FULL_PAINT:
            //EngAppendCircleStartModeOneLineItemContent(ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID,TRUE);
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

LOCAL GnssModeIndex_Get(void)
{
#ifdef WIN32
    int gnss_mode = 0;
#else
#ifdef UAL_GNSS_SUPPORT
    int gnss_mode = ual_gnss_get_cp_mode();
#else
    int gnss_mode = 0;
#endif
#endif /* WIN32 */    
    int i =0;
    TRACE_APP_ENGINEERMODE("[GPS]GnssModeIndex_Get, gnss_mode:%d",gnss_mode);
    //clear up gnss mode index selected first
    for(i=APP_GPS_GNSS_MODE_GPS_GLONASS;i<APP_GPS_GNSS_MODE_MAX; i++)
    {
        g_gnss_mode_data[i] = FALSE;
    }

    switch(gnss_mode)
    {
#if 0
        case MNGPS_START_GPS_GLONASS:
           g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_GLONASS] = TRUE;
           break;
        case MNGPS_START_GPS_BDS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_BDS]= TRUE;
            break;
        case MNGPS_START_GPS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS]= TRUE;
            break;
        case MNGPS_START_BDS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_BDS]= TRUE;
            break;
        default:
           g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_GLONASS] = TRUE;
            break;
#endif
#ifdef UAL_GNSS_SUPPORT
        case UAL_GNSS_MODE_GPS_GLONASS:
           g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_GLONASS] = TRUE;
           break;
        case UAL_GNSS_MODE_GPS_BDS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_BDS]= TRUE;
            break;
        case UAL_GNSS_MODE_GPS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS]= TRUE;
            break;
        case UAL_GNSS_MODE_BDS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_BDS]= TRUE;
            break;
        case UAL_GNSS_MODE_GLONASS:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GLONASS] = TRUE;
            break;
        case UAL_GNSS_MODE_GPS_BDS_GALILEO:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_BDS_GALILEO] = TRUE;
            break;
        case UAL_GNSS_MODE_GPS_B1C_GLONASS_GALILEO:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_B1C_GLONASS_GALILEO] = TRUE;
            break;
#endif
        default:
            g_gnss_mode_data[APP_GPS_GNSS_MODE_GPS_GLONASS] = TRUE;
            break;
    }

}
LOCAL MMI_RESULT_E EngGpsGnssModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //MN_GPS_GNSS_MODE_E gnss_mode =0;
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_mode_e gnss_mode = 0;
#endif
    uint8 i=0;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_ENGINEERMODE("[GPS]MSG_OPEN_WINDOW");
            GnssModeIndex_Get();
            MMK_SetAtvCtrl(win_id,ID_ENG_GPS_TEST_GNSSMODE_LIST_BOX_CTRL_ID);
            GUILIST_SetMaxItem( ID_ENG_GPS_TEST_GNSSMODE_LIST_BOX_CTRL_ID, APP_GPS_GNSS_MODE_MAX, FALSE );
            CTRLLIST_SetTextFont(ID_ENG_GPS_TEST_GNSSMODE_LIST_BOX_CTRL_ID,SONG_FONT_16,MMI_WHITE_COLOR);
            EngAppendGpsGnssMode(ID_ENG_GPS_TEST_GNSSMODE_LIST_BOX_CTRL_ID,FALSE);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            uint16 cur_index = 0;
            cur_index = GUILIST_GetCurItemIndex(ID_ENG_GPS_TEST_GNSSMODE_LIST_BOX_CTRL_ID);
            //set other option to be false
            for(i=APP_GPS_GNSS_MODE_GPS_GLONASS;i<APP_GPS_GNSS_MODE_MAX; i++)
            {
                g_gnss_mode_data[i] = FALSE;
            }
            g_gnss_mode_data[cur_index] = TRUE;
            TRACE_APP_ENGINEERMODE("[GPS]select list index:%d",cur_index); 
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            TRACE_APP_ENGINEERMODE("[GPS]press ok to open popwindow!!!");
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_AFTER_GNSS_SET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            TRACE_APP_ENGINEERMODE("[GPS]press ok to open popwindow!!!");
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_AFTER_GNSS_SET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
        }
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
        case MSG_PROMPTWIN_OK:
            TRACE_APP_ENGINEERMODE("[GPS] MSG_PROMPTWIN_OK"); 
            for(i=APP_GPS_GNSS_MODE_GPS_GLONASS;i<APP_GPS_GNSS_MODE_MAX; i++)
            {
                if(TRUE == g_gnss_mode_data[i])
                {
                    switch(i)
                    {
#if 0
                        case APP_GPS_GNSS_MODE_GPS_GLONASS:
                           gnss_mode =  MNGPS_START_GPS_GLONASS;
                           break;
                        case APP_GPS_GNSS_MODE_GPS_BDS:
                            gnss_mode = MNGPS_START_GPS_BDS;
                            break;
                        case APP_GPS_GNSS_MODE_GPS:
                            gnss_mode = MNGPS_START_GPS;
                            break;
                        case APP_GPS_GNSS_MODE_BDS:
                            gnss_mode = MNGPS_START_BDS;
                            break;
#endif
#ifdef UAL_GNSS_SUPPORT
                        case APP_GPS_GNSS_MODE_GPS_GLONASS:
                            gnss_mode = UAL_GNSS_MODE_GPS_GLONASS;
                            break;
                        case APP_GPS_GNSS_MODE_GPS_BDS:
                            gnss_mode = UAL_GNSS_MODE_GPS_BDS;
                            break;
                        case APP_GPS_GNSS_MODE_GPS:
                            gnss_mode = UAL_GNSS_MODE_GPS;
                            break;
                        case APP_GPS_GNSS_MODE_BDS:
                            gnss_mode = UAL_GNSS_MODE_BDS;
                            break;
                        case APP_GPS_GNSS_MODE_GLONASS:
                            gnss_mode = UAL_GNSS_MODE_GLONASS;
                            break;
                        case APP_GPS_GNSS_MODE_GPS_BDS_GALILEO:
                            gnss_mode = UAL_GNSS_MODE_GPS_BDS_GALILEO;
                            break;
                        case APP_GPS_GNSS_MODE_GPS_B1C_GLONASS_GALILEO:
                            gnss_mode = UAL_GNSS_MODE_GPS_B1C_GLONASS_GALILEO;
                            break;
#endif
                        default:
                            break;
                    }
                    //MMIGPS_GnssMode_Set(gnss_mode);
#ifdef UAL_GNSS_SUPPORT
                    TRACE_APP_ENGINEERMODE("[GPS] gnss_mode is:%d",gnss_mode); 
                    ual_gnss_set_gnss_mode(gnss_mode);
#endif
                    break;
                }
            }
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(PNULL);
            break;
        case MSG_PROMPTWIN_CANCEL:
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(PNULL);
            break;
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;  
}
WINDOW_TABLE(MMIENG_TEST_GPS_GNSSMODE_TAB) = 
{
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32)EngGpsGnssModeWinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_GNSSMODE_WIN_ID),
    WIN_TITLE(TXT_GNSS_MODE_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, ID_ENG_GPS_TEST_GNSSMODE_LIST_BOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_TEST_GPS_CIRCLETEST_PARAMSET_TAB) = 
{
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32)EngGpsCircleParamSetWinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_CRICLETEST_PARAMSET_WIN_ID),
    WIN_TITLE(TXT_CIRCLE_PARAM_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, ID_ENG_GPS_TEST_CIRCLETEST_TIME_LABEL_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),
    CHILD_EDIT_DIGITAL_CTRL(TRUE,EDIT_MAX_LEN, ID_ENG_GPS_TEST_CIRCLETEST_TIME_EDIT_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, ID_ENG_GPS_TEST_CIRCLETEST_INTERVAL_LABEL_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),
    CHILD_EDIT_DIGITAL_CTRL(TRUE,EDIT_MAX_LEN, ID_ENG_GPS_TEST_CIRCLETEST_INTERVAL_EDIT_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),

    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, ID_ENG_GPS_TEST_CIRCLETEST_TRANSFERINTERVAL_LABEL_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),
    //CHILD_EDIT_DIGITAL_CTRL(TRUE,EDIT_MAX_LEN, ID_ENG_GPS_TEST_CIRCLETEST_TRANSFERINTERVAL_EDIT_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, ID_ENG_GPS_TEST_CIRCLETEST_TTFFTIMEOUT_LABEL_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),
    CHILD_EDIT_DIGITAL_CTRL(TRUE,EDIT_MAX_LEN, ID_ENG_GPS_TEST_CIRCLETEST_TTFFTIMEOUT_EDIT_CTRL_ID, ID_ENG_GPS_TEST_CIRCLETEST_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_TEST_GPS_CIRCLETEST_STARTMODE_TAB) = 
{
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32)EngGpsCircleStartModeWinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_CRICLETEST_STARTMODE_WIN_ID),
    WIN_TITLE(TXT_START_MODE_SET),
    //WIN_SOFTKEY(TXT_COMMON_OK, NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, ID_ENG_GPS_TEST_STARTMODE_LIST_BOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_TEST_GPS_CIRCLETEST_TAB) =
{
	WIN_HIDE_STATUS,

    WIN_FUNC((uint32)EngGpsCircleTestWinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_CRICLETEST_WIN_ID),
    WIN_TITLE(TXT_GPS_CIRCLE_TEST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, ID_ENG_GPS_TEST_CIRCLETEST_LIST_BOX_ID),
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_ENG_RETURN),
    END_WIN
};
/************************************************************************/
/**********************GPS CIRCLE TEST START*****************************/
/************************************************************************/


/****************************************************************************/
/************************   GPS LATLONGSET WIN START  ************************/
/****************************************************************************/
LOCAL double ConvertStringToDouble(const MMI_STRING_T *str_ptr)
{
    int32  ret =0;
    uint32 num_len = 0;
    uint8  num_buf[LATLONG_LEN_MAX + 1] = {0};
    double double_data = 0;

    if(str_ptr != PNULL)
    {
        if(str_ptr->wstr_ptr != PNULL)
        {
            num_len = MIN(LATLONG_LEN_MAX, str_ptr->wstr_len);/*lint !e64*/
            MMI_WSTRNTOSTR((char *)num_buf, LATLONG_LEN_MAX, str_ptr->wstr_ptr, LATLONG_LEN_MAX, num_len);/*lint !e64*/
            TRACE_APP_ENGINEERMODE("[GPS]: angle char:%s\n",num_buf);
            sscanf(num_buf, "%lf", &double_data);
        }
    }
    return double_data;
}

LOCAL void SetLatitudeParam(void)
{
    MMI_STRING_T  latitude_angle = {0};
    wchar  wangle_str[LATLONG_LEN_MAX + 1] = {0};
    uint8   angle_str[LATLONG_LEN_MAX + 1] = {0};
    int32   angle_str_len = 0;
    MMI_STRING_T  latitude_text = {0};
    MMI_STRING_T longtitude_text= {0};
    wchar  negative_str[] = {'.'};//fix Bug 1322128

    //latitude form info
    GUIFORM_SetStyle(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_FORM_CTRL_ID,GUIFORM_STYLE_NORMAL);
    MMI_GetLabelTextByLang(TXT_LATITUDE_OPT, &latitude_text);
    GUILABEL_SetText(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_LABEL_CTRL_ID, &latitude_text, FALSE);

    GUIEDIT_SetStyle(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetIm(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
    GUIEDIT_ConfigImCustomKey(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, negative_str, 1);
    GUIEDIT_SetImTag(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID, GUIIM_TAG_CLASSIC_NUM);

    sprintf(angle_str, "%lf",g_latitude_angle);
    angle_str_len = strlen((char*)angle_str);
    angle_str_len = MIN(LATLONG_LEN_MAX, angle_str_len);
    MMI_STRNTOWSTR(wangle_str, LATLONG_LEN_MAX, angle_str, LATLONG_LEN_MAX, angle_str_len);
    GUIEDIT_SetString(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID, wangle_str, MMIAPICOM_Wstrlen(wangle_str));
}

LOCAL void SetLongtitudeParam(void)
{
    MMI_STRING_T  longtitude_angle = {0};
    wchar  wangle_str[LATLONG_LEN_MAX + 1] = {0};
    uint8   angle_str[LATLONG_LEN_MAX + 1] = {0};
    int32   angle_str_len = 0;
    MMI_STRING_T  latitude_text = {0};
    MMI_STRING_T longtitude_text= {0};
    wchar  negative_str[] = {'.'};//fix Bug 1322128

    //longtitude form info
    GUIFORM_SetStyle(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_FORM_CTRL_ID,GUIFORM_STYLE_NORMAL);
    MMI_GetLabelTextByLang(TXT_LONGTITUDE_OPT, &longtitude_text);
    GUILABEL_SetText(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_LABEL_CTRL_ID, &longtitude_text, FALSE);

    GUIEDIT_SetStyle(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetIm(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
    GUIEDIT_ConfigImCustomKey(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, negative_str, 1);
    GUIEDIT_SetImTag(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID, GUIIM_TAG_CLASSIC_NUM);

    sprintf(angle_str, "%lf",g_longtitude_angle);
    angle_str_len = strlen((char*)angle_str);
    angle_str_len = MIN(LATLONG_LEN_MAX, angle_str_len);
    MMI_STRNTOWSTR(wangle_str, LATLONG_LEN_MAX, angle_str, LATLONG_LEN_MAX, angle_str_len);

    GUIEDIT_SetString(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID, wangle_str, MMIAPICOM_Wstrlen(wangle_str));
}

LOCAL BOOLEAN  SaveLatitudeParam(void)
{
    MMI_STRING_T  latitude_str        = {0};

    GUIEDIT_GetString(ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID, &latitude_str);
    TRACE_APP_ENGINEERMODE("[GPS]: latitude angle wchar:%ls\n",latitude_str.wstr_ptr);
    g_latitude_angle = ConvertStringToDouble(&latitude_str);
    TRACE_APP_ENGINEERMODE("[GPS]: latitude angle float:%lf\n",g_latitude_angle);
    if (g_latitude_angle >360 || g_latitude_angle < -360 || latitude_str.wstr_len == 0)
    {
        return FALSE;
    }
    return TRUE ;
}

LOCAL BOOLEAN  SaveLongtitudeParam(void)
{
    MMI_STRING_T  longtitude_str        = {0};

    GUIEDIT_GetString(ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID, &longtitude_str);
    TRACE_APP_ENGINEERMODE("[GPS]: longtitude angle wchar:%ls\n",longtitude_str.wstr_ptr);
    g_longtitude_angle = ConvertStringToDouble(&longtitude_str);
    TRACE_APP_ENGINEERMODE("[GPS]: longtitude angle float:%lf\n",g_longtitude_angle);
    if (g_longtitude_angle >360 || g_longtitude_angle < -360|| longtitude_str.wstr_len == 0)
    {
        return FALSE;
    }
    return TRUE ;
}

LOCAL MMI_RESULT_E  HandleLatLongSetWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E    recode =   MMI_RESULT_TRUE;
    MMI_RESULT_E    recode1 =   MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIFORM_SetCircularHandleUpDown(ID_ENG_GPS_TEST_LATLONGSET_FORM_CTRL_ID,TRUE);
            MMK_SetAtvCtrl(win_id, ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID);

            SetLatitudeParam();
            SetLongtitudeParam();
            break;
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            recode = SaveLatitudeParam();
            recode1 = SaveLongtitudeParam();
            TRACE_APP_ENGINEERMODE("[GPS]: recode = %d, recode1 = %d  \n",recode,recode1);
            if(recode && recode1) // save success
            {
                MMK_CloseWin( win_id );
            }
            else // save fail
            {
                MMIPUB_OpenAlertWarningWin(TXT_GPS_ANGLE_ILLEGAL);
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

WINDOW_TABLE( MMIENG_TEST_GPS_LATLONGSET_TAB ) =
{
	WIN_HIDE_STATUS,

    WIN_FUNC( (uint32)HandleLatLongSetWinMsg ),
    WIN_ID( MMIENG_TEST_GPS_LATLONGSET_WIN_ID ),
    WIN_TITLE(TXT_GPSTEST_PARAM_SETTING),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,ID_ENG_GPS_TEST_LATLONGSET_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_FORM_CTRL_ID,ID_ENG_GPS_TEST_LATLONGSET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_LABEL_CTRL_ID,ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,LATLONG_LEN_MAX,ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_EDITBOX_CTRL_ID,ID_ENG_GPS_TEST_LATLONGSET_LATITUDE_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_FORM_CTRL_ID,ID_ENG_GPS_TEST_LATLONGSET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_LABEL_CTRL_ID,ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,LATLONG_LEN_MAX,ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_EDITBOX_CTRL_ID,ID_ENG_GPS_TEST_LATLONGSET_LONGTITUDE_FORM_CTRL_ID),
    END_WIN
};

/****************************************************************************/
/************************   GPS LATLONGSET WIN END  **************************/
/****************************************************************************/


/************************************************************************/
/**********************GPS TEST COMMON START*****************************/
/************************************************************************/
#if 0
LOCAL BOOLEAN _EngHandleGpsMessageCallback(APP_MN_GPS_SIGNAL_E gps_sig_id,void* gps_info)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    static APP_MN_GPS_READ_INFO_CNF_T report_gpsinfo = {0};//get information from report
    APP_MN_GPS_READ_INFO_CNF_T read_gpsinfo = {0};    
    int i =0;
    char test_str[256] = {0};
    TRACE_APP_ENGINEERMODE("[GPS]: gps_sig_id:%d\n",gps_sig_id);
    switch ( gps_sig_id )
    {

        case APP_MN_GPS_INIT_CNF:
        {
            APP_MN_GPS_INIT_CNF_T *pInitcnf = (APP_MN_GPS_INIT_CNF_T *)gps_info;
            if(PNULL == pInitcnf)
            {
                TRACE_APP_GPS("[GPS]:APP_MN_GPS_INIT_CNF param is null\n");
                break;
            }
            if(TRUE == pInitcnf->is_ok)
            {
                TRACE_APP_ENGINEERMODE("[GPS OTA]:gps init succ");
            }
            break;
        }
        case APP_MN_GPS_START_CNF:
        {
            //CALL report req
            APP_MN_GPS_START_CNF_T *pStartInfo = (APP_MN_GPS_START_CNF_T *)gps_info;
            if(PNULL == pStartInfo)
            {
                TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_START_CNF  pStartInfo is null\n");
                return MMI_RESULT_TRUE;
            }
            if(TRUE == pStartInfo->is_ok)
            {
                MNGPS_ReadInfoReq(0);   //request gps and location info
                if(pStartInfo->dual_sys < MN_SYS_SIM_MAX)
                {
                    g_sys_sim_index = pStartInfo->dual_sys;
                }
                else
                {
                    g_sys_sim_index = MN_SYS_SIM_1;
                }

                // for circle test
                if(TRUE == g_is_circle_test)
                {
                    EngGps_CircleTest_HandleStartCnf();

                    g_is_saved_data_currentCircleTime = FALSE;

                }

                //for nmealog
                if(FALSE == g_nmeaLog_isIdle)
                {
                    cmd_nmea_type = NMEA_ALL;
                    StartNmeaLogReq();
                }

                TRACE_APP_ENGINEERMODE("[GPS]:APP_MN_GPS_START_CNF is OK\n");
                if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
                {
                    MMK_SendMsg(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, MSG_FULL_PAINT, PNULL);
                }

            }
            else
            {
                TRACE_APP_ENGINEERMODE("[GPS]:APP_MN_GPS_START_CNF is NOT OK\n");
                MMIPUB_OpenAlertFailWin(TXT_START_GPS_FAIL);
            }
            break;
        }
        case APP_MN_GPS_RESET_CNF:
        {
            break;
        }
        case APP_MN_GPS_READ_STATUS_CNF:
        {
            break;
        }
        case APP_MN_GPS_READ_INFO_CNF:
        {
            APP_MN_GPS_READ_INFO_CNF_T *param_ptr = (APP_MN_GPS_READ_INFO_CNF_T *)gps_info;
            TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_READ_INFO_CNF\n");
            if(PNULL == param_ptr)
            {
                TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_READ_INFO_CNF  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            read_gpsinfo.gps_info.longitude = param_ptr->gps_info.longitude;
            read_gpsinfo.gps_info.latitude  = param_ptr->gps_info.latitude;     
            read_gpsinfo.gps_info.altitude = param_ptr->gps_info.altitude;            
            read_gpsinfo.gps_info.ttff_time = param_ptr->gps_info.ttff_time;
            read_gpsinfo.gps_info.satellite_num = param_ptr->gps_info.satellite_num;
            read_gpsinfo.gps_info.m_nHorizontalAccuracy = param_ptr->gps_info.m_nHorizontalAccuracy;
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.longitude = %f ",read_gpsinfo.gps_info.longitude);
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.latitude = %f ",read_gpsinfo.gps_info.latitude);
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.altitude = %f ",read_gpsinfo.gps_info.altitude);
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.ttff_time = %f ",read_gpsinfo.gps_info.ttff_time);
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.satellite_num = %d ",read_gpsinfo.gps_info.satellite_num);
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.m_nHorizontalAccuracy = %f",read_gpsinfo.gps_info.m_nHorizontalAccuracy);
            read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
            SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info, 
                param_ptr->satellite_list_info.satellite_info, 
                sizeof(param_ptr->satellite_list_info.satellite_info));
            TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
            for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                sprintf(test_str, "%s: APP_MN_GPS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
                read_gpsinfo.satellite_list_info.satellite_info[i].m_nSatelliteIdentifier,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_cn0,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_nElevation,  param_ptr->satellite_list_info.satellite_info[i].m_nAzimuth,
                read_gpsinfo.satellite_list_info.satellite_info[i].m_IsUsed);
                TRACE_APP_ENGINEERMODE("[GPS]:%s",test_str);
            }

            if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITEINFO_WIN_ID))
            {
                if(read_gpsinfo.satellite_list_info.satellite_num >0)
                {
                    AppendSatelliteListItem(MMIENG_LISTBOX_CTRL_ID, &read_gpsinfo);
                }
               else
               {
                   SCI_MEMCPY(&read_gpsinfo, &report_gpsinfo, sizeof(report_gpsinfo));
                    AppendSatelliteListItem(MMIENG_LISTBOX_CTRL_ID, &read_gpsinfo);
                    Gps_StartRequestSatInfoTimer(param_ptr->dual_sys);
                }
            }
            if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
            {
                MMIENG_GPSSatCN_HandleSatIndMsg(&(read_gpsinfo.satellite_list_info));
                Gps_StartRequestSatInfoTimer(param_ptr->dual_sys);
            }
             break;
        }
        case APP_MN_GPS_OUTPUT_CNF:
        {
            break;
        }
        case APP_MN_GPS_OUTPUT_INFO_IND:
        {
             APP_MN_GPS_OUTPUT_INFO_IND_T *param_ptr = (APP_MN_GPS_OUTPUT_INFO_IND_T *)gps_info;
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND\n");
             /*TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND sys_sim_index:%d -------\n", param_ptr->sys_sim_index);
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND type: 0x%x  -------\n", param_ptr->type);
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND length:%d  -------\n", param_ptr->length);
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND data:%d -------\n", param_ptr->nmea_data);*/
             MMIENG_HandleNmeaLogData(param_ptr);

            break;
        }
        case APP_MN_GPS_ERR_IND:
        {
            break;
        }
         case APP_MN_GPS_REPORT_CNF:
        {
            APP_MN_GPS_REPORT_CNF_T* pReportCnf= (APP_MN_GPS_REPORT_CNF_T *)gps_info;
            TRACE_APP_ENGINEERMODE("[GPS]: enter APP_MN_GPS_REPORT_CNF\n");
            TRACE_APP_ENGINEERMODE("[GPS]: enter APP_MN_GPS_REPORT_CNF %d\n",pReportCnf->is_ok);
            if(FALSE == pReportCnf->is_ok)
            {
                MNGPS_ReportInfoReq(pReportCnf->dual_sys,TRUE,TRUE);
            }
            break;
        }
        case APP_MN_GPS_SATELLITE_INFO_IND:
        {
            APP_MN_GPS_SATELLITE_INFO_IND_T * pSateInfo = (APP_MN_GPS_SATELLITE_INFO_IND_T *) gps_info;
            TRACE_APP_ENGINEERMODE("[GPS]: enter APP_MN_GPS_SATELLITE_INFO_IND\n");
            //refresh listbox
            if(PNULL == pSateInfo)
            {
                TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_SATELLITE_INFO_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            if(0 == pSateInfo->satellite_list_info.satellite_num)
            {
                TRACE_APP_ENGINEERMODE("[GPS]: enter APP_MN_GPS_SATELLITE_INFO_IND\n");
            }else if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
            {
                MMIENG_GPSSatCN_HandleSatIndMsg(pSateInfo);
            }
            
            //save satellite information to report_gpsinfo
            report_gpsinfo.satellite_list_info.satellite_num = pSateInfo->satellite_list_info.satellite_num;
            SCI_MEMCPY(report_gpsinfo.satellite_list_info.satellite_info, 
                pSateInfo->satellite_list_info.satellite_info, 
                sizeof(pSateInfo->satellite_list_info.satellite_info));
            for(i=0; i<report_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                sprintf(test_str, "%s: satellite info %d,%d,%d,%d,%d", "report_gpsinfo",
                report_gpsinfo.satellite_list_info.satellite_info[i].m_nSatelliteIdentifier,
                report_gpsinfo.satellite_list_info.satellite_info[i].m_cn0,
                report_gpsinfo.satellite_list_info.satellite_info[i].m_nElevation,  report_gpsinfo.satellite_list_info.satellite_info[i].m_nAzimuth,
                report_gpsinfo.satellite_list_info.satellite_info[i].m_IsUsed);
                TRACE_APP_ENGINEERMODE("zyh:%s",test_str); 
            }
            TRACE_APP_ENGINEERMODE("[GPS]:APP_MN_GPS_SATELLITE_INFO_IND, copyed satellite num=%d\n",  report_gpsinfo.satellite_list_info.satellite_num);
            break;
        }
        
        case APP_MN_GPS_LOCATION_INFO_IND:
        {
            APP_MN_GPS_LOC_INFO_IND_T * pSateInfo = (APP_MN_GPS_LOC_INFO_IND_T *) gps_info;
            TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_LOCATION_INFO_IND\n");
            if(PNULL == pSateInfo)
            {
            
                TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_LOCATION_INFO_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            
            TRACE_APP_ENGINEERMODE("[GPS]:APP_MN_GPS_LOCATION_INFO_IND, latitude=%f\n", pSateInfo->gps_info.latitude);
            //save gps information to g_gpsinfo
            report_gpsinfo.gps_info.altitude = pSateInfo->gps_info.altitude;
            report_gpsinfo.gps_info.latitude= pSateInfo->gps_info.latitude;
            report_gpsinfo.gps_info.longitude= pSateInfo->gps_info.longitude;
            report_gpsinfo.gps_info.satellite_num= pSateInfo->gps_info.satellite_num;
            report_gpsinfo.gps_info.ttff_time= pSateInfo->gps_info.ttff_time;
            //SCI_MEMCPY(&(report_gpsinfo.gps_info), &(pSateInfo->gps_info), sizeof(pSateInfo->gps_info));
            TRACE_APP_ENGINEERMODE("[GPS]:APP_MN_GPS_LOCATION_INFO_IND, altitude =%f, latitude=%f, longitude=%f, satellite_num=%d, ttff_time = %f",
                report_gpsinfo.gps_info.altitude,
                report_gpsinfo.gps_info.latitude,
                report_gpsinfo.gps_info.longitude,
                report_gpsinfo.gps_info.satellite_num,
                report_gpsinfo.gps_info.ttff_time);
         }
      break;
        case APP_MN_GPS_FIX_IND:
        {
            APP_MN_GPS_FIX_IND_T * pFixInfo = (APP_MN_GPS_FIX_IND_T *)gps_info;
            if(PNULL == pFixInfo)
            {
                TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_FIX_IND  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_FIX_IND  gps_info,ttff:%d, longi:%f, lati:%f\n", pFixInfo->ttff_time, pFixInfo->longitude, pFixInfo->latitude);
           // if(TRUE == g_is_circle_test)//表示当前是由circle test 窗体发起的test
           //save circle test result to autotest file
           if(TRUE == g_is_circle_test)
           {
               if((pFixInfo->ttff_time >0)&&(FALSE == g_is_saved_data_currentCircleTime))
               {
                   EngGps_handleCircleTestData(pFixInfo);
                   EngGps_CircleTest_HandleFixIndMsg(pFixInfo->ttff_time);
                   g_is_saved_data_currentCircleTime = TRUE;
               }
           }
            break;
        }
        case APP_MN_GPS_STOP_CNF:
        {
            APP_MN_GPS_STOP_CNF_T *pStopcnf = (APP_MN_GPS_STOP_CNF_T *)gps_info;
            if(PNULL == pStopcnf)
            {
                TRACE_APP_ENGINEERMODE("[GPS]: APP_MN_GPS_STOP_CNF  gps_info is null\n");
                return MMI_RESULT_FALSE;
            }
            TRACE_APP_ENGINEERMODE("[GPS]:APP_MN_GPS_STOP_CNF is OK\n");
            if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
            {
                MMK_SendMsg(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            TRACE_APP_ENGINEERMODE("[GPS]:<%d>,<%d>APP_MN_GPS_STOP_CNF is OK\n", pStopcnf->dual_sys, pStopcnf->is_ok);
            if (FALSE == g_nmeaLog_isIdle)
            {
                //stop save NMEA log
                StopNmeaLogReq();
            }
            EngGps_CircleTest_HandleStopCnf();
            break;
        }

        default:
        {
            result = MMI_RESULT_FALSE;
            TRACE_APP_ENGINEERMODE("[GPS]: Unknow msg id!\n");
            break;
        }
    }
    return result;
}
#endif
/************************************************************************/
/**********************GPS TEST COMMON END*******************************/
/************************************************************************/

/************************************************************************/
/**********************GPS TEST MAIN WIN START***************************/
/************************************************************************/
#ifdef UAL_GNSS_SUPPORT
LOCAL BOOLEAN HandleGnssStartCnf(ual_cms_msg_t param)
{
    ual_gnss_start_cnf_t*  pStartInfo = (ual_gnss_start_cnf_t*)(param.p_body);
    BOOLEAN                ret        = TRUE;
    if(PNULL == pStartInfo)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: MSG_UAL_GNSS_START_CNF  pStartInfo is null!\n");
        return FALSE;
    }
    if(TRUE == pStartInfo->is_ok)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: start success\n");
        ual_gnss_read_info();
        Gps_StartRequestSatInfoTimer(0);
        g_sys_sim_index = 0;
        // for circle test
        if(TRUE == g_is_circle_test)
        {
            TRACE_APP_ENGINEERMODE("[GPS]:circle test\n");
            EngGps_CircleTest_HandleStartCnf();
            g_is_saved_data_currentCircleTime = FALSE;
        }
        //for nmealog
        if(FALSE == g_nmeaLog_isIdle)
        {
            cmd_nmea_type = NMEA_ALL;
            StartNmeaLogReq();
        }
        if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
        {
            MMK_SendMsg(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
    else
    {
        TRACE_APP_ENGINEERMODE("[GPS]:start fail!\n");
        MMIPUB_OpenAlertFailWin(TXT_START_GPS_FAIL);
    }
    return ret;
}

LOCAL BOOLEAN HandleGnssReadInfoCnf(ual_cms_msg_t param)
{
    ual_gnss_read_info_cnf_t*  param_ptr    = (ual_gnss_read_info_cnf_t*)(param.p_body);
    ual_gnss_read_info_cnf_t   read_gpsinfo = {0};
    int                        i            = 0;
    char                       test_str[256]= {0};
    BOOLEAN                    ret          = TRUE;
    if(PNULL == param_ptr)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: MSG_UAL_GNSS_READ_INFO_CNF  gps_info is null\n");
        return FALSE;
    }
    read_gpsinfo.gps_info.longitude = param_ptr->gps_info.longitude;
    read_gpsinfo.gps_info.latitude  = param_ptr->gps_info.latitude;
    read_gpsinfo.gps_info.altitude = param_ptr->gps_info.altitude;
    read_gpsinfo.gps_info.ttff_time = param_ptr->gps_info.ttff_time;
    read_gpsinfo.gps_info.satellite_num = param_ptr->gps_info.satellite_num;
    read_gpsinfo.gps_info.HorizontalAccuracy = param_ptr->gps_info.HorizontalAccuracy;
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.longitude = %f ",read_gpsinfo.gps_info.longitude);
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.latitude = %f ",read_gpsinfo.gps_info.latitude);
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.altitude = %f ",read_gpsinfo.gps_info.altitude);
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.ttff_time = %d ",read_gpsinfo.gps_info.ttff_time);
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.satellite_num = %d ",read_gpsinfo.gps_info.satellite_num);
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.gps_ReadInfo.gps_info.HorizontalAccuracy = %f",read_gpsinfo.gps_info.HorizontalAccuracy);
    read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
    SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info,
        param_ptr->satellite_list_info.satellite_info,
        sizeof(param_ptr->satellite_list_info.satellite_info));
    //g_read_info_t赋值是为了收到fix_ind后，刷新location information界面
    g_read_info_t.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;;
    SCI_MEMSET(g_read_info_t.satellite_list_info.satellite_info,0,sizeof(param_ptr->satellite_list_info.satellite_info));
    SCI_MEMCPY(g_read_info_t.satellite_list_info.satellite_info,
        param_ptr->satellite_list_info.satellite_info,
        sizeof(param_ptr->satellite_list_info.satellite_info));
    TRACE_APP_ENGINEERMODE("[GPS]read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
    for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
    {
        sprintf(test_str, "%s: MSG_UAL_GNSS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
        read_gpsinfo.satellite_list_info.satellite_info[i].satellite_identifier,
        read_gpsinfo.satellite_list_info.satellite_info[i].cn0,
        read_gpsinfo.satellite_list_info.satellite_info[i].elevation,  param_ptr->satellite_list_info.satellite_info[i].azimuth,
        read_gpsinfo.satellite_list_info.satellite_info[i].is_used);
        TRACE_APP_ENGINEERMODE("[GPS]:%s",test_str);
    }

    if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITEINFO_WIN_ID))
    {
        AppendSatelliteListItem(MMIENG_LISTBOX_CTRL_ID, &read_gpsinfo, PNULL);
    }
    if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
    {
        MMIENG_GPSSatCN_HandleSatIndMsg(&(read_gpsinfo.satellite_list_info));
        //Gps_StartRequestSatInfoTimer(0);
    }
    return ret;
}

LOCAL BOOLEAN HandleGnssFixInd(ual_cms_msg_t param)
{
     ual_gnss_fix_ind_t*  pFixInfo = (ual_gnss_fix_ind_t*)(param.p_body);
     BOOLEAN              ret      = TRUE;
     if(PNULL == pFixInfo)
     {
         TRACE_APP_ENGINEERMODE("[GPS]: MSG_UAL_GNSS_FIX_IND  pFixInfo is null\n");
         return FALSE;
     }
     TRACE_APP_ENGINEERMODE("[GPS]: MSG_UAL_GNSS_FIX_IND ttff:%d, longi:%f, lati:%f\n", pFixInfo->ttff_time, pFixInfo->longitude, pFixInfo->latitude);

    //刷新location information界面
    if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITEINFO_WIN_ID))
    {
        AppendSatelliteListItem(MMIENG_LISTBOX_CTRL_ID, &g_read_info_t, pFixInfo);
    }
    // if(TRUE == g_is_circle_test)//表示当前是由circle test 窗体发起的test
    //save circle test result to autotest file
    if(TRUE == g_is_circle_test)
    {
        if((pFixInfo->ttff_time >0)&&(FALSE == g_is_saved_data_currentCircleTime))
        {
            EngGps_handleCircleTestData(pFixInfo);
            EngGps_CircleTest_HandleFixIndMsg(pFixInfo->ttff_time);
            g_is_saved_data_currentCircleTime = TRUE;
        }
    }
    return ret;
}

LOCAL BOOLEAN HandleGnssStopCnf(ual_cms_msg_t param)
{
    ual_gnss_stop_cnf_t* pStopcnf = (ual_gnss_stop_cnf_t*)(param.p_body);
    BOOLEAN              ret      = TRUE;
    if(PNULL == pStopcnf)
    {
        TRACE_APP_ENGINEERMODE("[GPS]: MSG_UAL_GNSS_STOP_CNF  gps_info is null\n");
        return FALSE;
    }
    if (0 != g_request_sat_info_timer_id)
    {
        MMK_StopTimer(g_request_sat_info_timer_id);
        g_request_sat_info_timer_id = 0;
    }
    if(MMK_IsOpenWin(MMIENG_TEST_GPS_SATELLITECN_WIN_ID))
    {
        MMK_SendMsg(MMIENG_TEST_GPS_SATELLITECN_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    TRACE_APP_ENGINEERMODE("[GPS]:stop result:%d\n", pStopcnf->is_ok);
    if (FALSE == g_nmeaLog_isIdle)
    {
        //stop save NMEA log
        StopNmeaLogReq();
    }
    if(TRUE == g_is_circle_test)
    {
        TRACE_APP_ENGINEERMODE("[GPS]:circle test\n");
        EngGps_CircleTest_HandleStopCnf();
    }
    return ret;
}

LOCAL BOOLEAN gnss_callback(ual_cms_msg_t param)
{
    BOOLEAN  ret    = TRUE;
    uint32   msg_id = 0;
    TRACE_APP_GPS("[ENG GPS]:Enter\n");
    msg_id = param.msg_id;
    TRACE_APP_GPS("[ENG GPS]:msg_id:%d\n",msg_id);
    switch(msg_id)
    {
        case MSG_UAL_GNSS_START_CNF:
        {
            TRACE_APP_ENGINEERMODE("[GPS]:MSG_UAL_GNSS_START_CNF\n");
            ret = HandleGnssStartCnf(param);   //bug2067923
            break;
        }
        case MSG_UAL_GNSS_READ_INFO_CNF:
        {
            TRACE_APP_ENGINEERMODE("[GPS]:MSG_UAL_GNSS_READ_INFO_CNF\n");
            ret = HandleGnssReadInfoCnf(param);   //bug2067923
            break;
        }
        case MSG_UAL_GNSS_OUTPUT_NMEA_IND:
        {
             ual_gnss_output_nmea_ind_t* param_ptr = (ual_gnss_output_nmea_ind_t*)(param.p_body);
             TRACE_APP_ENGINEERMODE("[GPS]:gnss_callback MSG_UAL_GNSS_OUTPUT_INFO_IND\n");
             /*TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND sys_sim_index:%d -------\n", param_ptr->sys_sim_index);
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND type: 0x%x  -------\n", param_ptr->type);
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND length:%d  -------\n", param_ptr->length);
             TRACE_APP_ENGINEERMODE("[GPS]:_EngHandleGpsMessageCallback APP_MN_GPS_OUTPUT_INFO_IND data:%d -------\n", param_ptr->nmea_data);*/
             MMIENG_HandleNmeaLogData(param_ptr);
             break;
        }
        case MSG_UAL_GNSS_FIX_IND:
        {
            TRACE_APP_ENGINEERMODE("[GPS]:MSG_UAL_GNSS_FIX_IND\n");
            ret = HandleGnssFixInd(param);   //bug2067923
            break;
        }
        case MSG_UAL_GNSS_STOP_CNF:
        {
            TRACE_APP_ENGINEERMODE("[GPS]:MSG_UAL_GNSS_STOP_CNF\n");
            ret = HandleGnssStopCnf(param);   //bug2067923
            break;
        }
        default:
            ret = FALSE;
            break;
    }
    TRACE_APP_GPS("[ENG GPS]:Exit,ret:%d\n",ret);
    return ret;
}
#endif
/*刚进入的测试GPS的窗口*/
LOCAL MMI_RESULT_E EngGpsTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    TRACE_APP_GPS("[ENG GPS]:EngGpsTestWinHandleMsg, msg_id:%0x\n",msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                TRACE_APP_GPS("[ENG GPS]:open MMIENG_OpenGPSTestWin, MSG_OPEN_WINDOW\n");
                CTRLMENU_SetSecMenuFontSize(ID_ENG_GPS_TEST_MENU_ID,MMIENG_MENU_ITEM_FONT,MMIENG_MENU_ITEM_FONT);
                CTRLMENU_SetSecMenuItemHeight(ID_ENG_GPS_TEST_MENU_ID,MMIENG_MENU_ITEM_HEIGHT,MMIENG_MENU_ITEM_HEIGHT);
                CTRLMENU_SetFontColor(ID_ENG_GPS_TEST_MENU_ID,MMI_WHITE_COLOR);
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);

                MMK_SetAtvCtrl(win_id,ID_ENG_GPS_TEST_MENU_ID);

                //init gps first
                //MMIGPS_Open();
#ifdef UAL_GNSS_SUPPORT
                if(FALSE == g_is_registered)   //bug2081879
                {
                    TRACE_APP_GPS("[ENG GPS]:have not registered,first register\n");
                    ual_gnss_register(gnss_callback,&g_gnss_handle);
                    g_is_registered = TRUE;
                }
                else
                {
                    TRACE_APP_GPS("[ENG GPS]:have registered,don't need register again!\n");
                }
#endif
            }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                uint32 group_id = 0;
                uint32 menu_id = 0;
                TRACE_APP_GPS("[GPS]:open MMIENG_OpenGPSTestWin, MSG_CTL_MIDSK\n");
                GUIMENU_GetId(ID_ENG_GPS_TEST_MENU_ID,&group_id,&menu_id);
                switch (menu_id)
                {
                    case ID_ENG_GPS_TEST_GNSSMODE:
                        MMK_CreateWin((uint32 *)MMIENG_TEST_GPS_GNSSMODE_TAB, PNULL);
                    break;
                    case ID_ENG_GPS_TEST_SATELLITECN:
                        MMK_CreateWin((uint32*)MMIENG_TEST_GPS_SATELLITECN_TAB,  (ADD_DATA)FALSE);
                    break;
                    case ID_ENG_GPS_TEST_GPSINFO:
                        MMK_CreateWin((uint32*)MMIENG_TEST_GPS_SATELLITEINFO_TAB,  PNULL);
                    break;
                    case ID_ENG_GPS_TEST_GPSNMEALOG:
                        MMK_CreateWin((uint32 *)(MMIENG_TEST_GPS_NMEA_LOG_WIN_TAB), PNULL);		
                    break;
                    case ID_ENG_GPS_TEST_CIRCLETEST:
                        MMK_CreateWin((uint32*)MMIENG_TEST_GPS_CIRCLETEST_TAB,  PNULL);
                    break;
                    case ID_ENG_GPS_TEST_LATLONGSET:
                        MMK_CreateWin((uint32*)MMIENG_TEST_GPS_LATLONGSET_TAB,  PNULL);
                    break;
                    default:
                        break;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            //MMIGPS_Close();
#ifdef UAL_GNSS_SUPPORT
            //ual_gnss_poweroff();   //bug2081879
            //ual_gnss_unregister(g_gnss_handle);
#endif
        break;
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;  
}

WINDOW_TABLE(MMIENG_TEST_GPS_TAB) = 
{
	WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngGpsTestWinHandleMsg),
    WIN_ID(MMIENG_TEST_GPS_WIN_ID),
    WIN_TITLE(TXT_ENG_GPS_TEST_MODULE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_GPS_INFO, ID_ENG_GPS_TEST_MENU_ID),
    END_WIN
};

PUBLIC int32 MMIENG_OpenGPSTestWin(void)
{
    MMK_CreateWin((uint32 * )MMIENG_TEST_GPS_TAB, PNULL);
}
/************************************************************************/
/**********************GPS TEST MAIN WIN END*****************************/
/************************************************************************/

