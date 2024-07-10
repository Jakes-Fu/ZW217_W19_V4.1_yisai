/*****************************************************************************
** File Name:      mmijava_main.c                                            *
** Author:         apple.zhang                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       apple.zhang            Create                                *
******************************************************************************/

#include "mmi_app_java_trc.h"
#if defined(__cplusplus)
extern "C" {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include <stdio.h>
#include <ctype.h>
#include "mmijava_main.h"
#ifdef JAVA_SUPPORT_SUN
#include "sci_types.h"
#ifndef WIN32
#include "tx_api_arm.h"
#endif
#include "os_api.h"
#include "mmifmm_export.h"
#include "javacall_lifecycle.h"
#include "mmijava_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiaudio_ctrl.h"
#include "mmi_default.h"
#include "chng_freq.h"
#include "freq_cfg.h"
#include "mmifilearray_export.h"
#include "mmijava_id.h"
#include "mmk_msg.h"
#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "guiedit.h"
#include "mmifmm_id.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "guires.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#include "mmiudisk_export.h"
#include "mmimtv_export.h"
#include "mmiatv_export.h"
#include "mmidc_export.h"

#include "guistatusbar.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmitheme_statusbar.h"
#include "caf.h"

#include "javacall_keypress.h"
#include "javacall_logging.h"
#include "javacall_lifecycle.h"
#include "javacall_time.h"
#include "javacall_input.h"
#include "javacall_penevent.h"
#include "javacall_file.h"
#include "javacall_dir.h"
#include "javacall_lcd.h"
#include "javacall_multimedia.h"
#include "javacall_security.h"
#include "mmijava_multimedia.h"
#include "java_sigdispatchtask.h"
#include "java_mmi_image.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmifmm_text.h"
#include "mmiacc_text.h"
#include "mmijava_preload.h"
#include "mmiacc_text.h"
#include "gen_preload_file.h"

#include "mmi_appmsg.h"
#ifdef MMI_DUALMODE_ENABLE
#include "mmidm_export.h"
#endif

#include "mmiwclk_export.h"
#include "mmiacc_id.h"
#include "java_mmi_text.h"
#include "javacall_network.h"
#include "mmiidle_export.h"
#include "mmivt_export.h"
#include "gpio_prod_api.h"
#include "in_message.h"
#include "socket_types.h" 
#include "Java_socket.h"
#include "mmijava_video.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
 
#define DYNAMIC_CREATE_STATUSBAR

#define IMAGE_JAVA_SPLASH_WIDTH     123
#define IMAGE_JAVA_SPLASH_HEIGHT    169

#define MMIJAVA_INSTALL_FILE_SUFFIX          "*.jar *.jad"
#define KEY_TYPE_NUM 5

#define MAX_FILENAME_LEN (MMIFMM_FULL_FILENAME_LEN + 8 + 1) //"file:///" + '/0'
#define MEDIA_PAUSE_RESUME_FROM_JAVACALL

uint32 keyTypeMapTable[KEY_TYPE_NUM][2] = {
    {JAVACALL_KEYPRESSED, KEY_PRESSED},
    {JAVACALL_KEYRELEASED, KEY_RELEASED},
    {JAVACALL_KEYREPEATED, KEY_REPEATED},
    {JAVACALL_KEYRELEASED, KEY_LONG_RELEASED},
    {JAVACALL_KEYPRESSED, KEY_LONG_PRESSED},
};

#define KEY_CODE_NUM 20
int32 keyCodeMapTable[KEY_CODE_NUM][2] = {
    {JAVACALL_KEY_BACKSPACE, KEY_BACKWARD},
    {JAVACALL_KEY_POUND, KEY_HASH},
    {JAVACALL_KEY_ASTERISK, KEY_STAR},
    {JAVACALL_KEY_0, KEY_0},
    {JAVACALL_KEY_1, KEY_1},
    {JAVACALL_KEY_2, KEY_2},
    {JAVACALL_KEY_3, KEY_3},
    {JAVACALL_KEY_4, KEY_4},
    {JAVACALL_KEY_5, KEY_5},
    {JAVACALL_KEY_6, KEY_6},
    {JAVACALL_KEY_7, KEY_7}, 
    {JAVACALL_KEY_8, KEY_8},
    {JAVACALL_KEY_9, KEY_9},
    {JAVACALL_KEY_UP, KEY_UP},
    {JAVACALL_KEY_DOWN, KEY_DOWN},
    {JAVACALL_KEY_LEFT, KEY_LEFT},
    {JAVACALL_KEY_RIGHT, KEY_RIGHT},
    {JAVACALL_KEY_SELECT, KEY_WEB},
    {JAVACALL_KEY_SOFT1, KEY_OK},
    {JAVACALL_KEY_SOFT2, KEY_CANCEL},
};
typedef enum
{
    MMIJAVA_VM_PAUSE_RESUME, 
    MMIJAVA_VM_SHUTDOWN,
    MMIJAVA_VM_BACKGROUND_FOREGROUND_RUNNING,
    MMIJAVA_MAX_VM_OPERATION
}MMIJAVA_VM_OPERATION_E;
LOCAL BOOLEAN s_is_need_tree_manage = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E HandleJavaWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param);
/*****************************************************************************/
//  Description : RemoveTmpDownloadFileWhenInit
//  Global resource dependence : none
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void RemoveTmpDownloadFileWhenInit(void);

LOCAL BOOLEAN MMIJAVA_CheckWhetherAllowedToRun(void);
/*****************************************************************************/
//  Description : check if there is enough dev space for java
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDevEnoughSpaceForJava(void);

LOCAL void shutdown_jvm(void);
LOCAL int32 LaunchMidlet(void);
LOCAL MMI_RESULT_E HandleSelectBeforeEnterJAVAQueryWinMsg(                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            );

WINDOW_TABLE( MMI_JAVA_WIN_TAB ) =
{
    WIN_ID(MMI_JAVA_WIN_ID),
#ifndef DYNAMIC_CREATE_STATUSBAR
     WIN_STATUSBAR,
#endif
		WIN_STYLE(WS_NEED_DBLCLK),
    WIN_FUNC((uint32)HandleJavaWinMsg),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};

LOCAL MMI_RESULT_E HandleJavaTextEditWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
WINDOW_TABLE(MMIJAVA_TEXT_EDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandleJavaTextEditWinMsg),    
    WIN_ID(MMIJAVA_TEXT_EDIT_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

LOCAL MMI_RESULT_E  HandleVMExitSelectWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
//the window for usb operation select window
WINDOW_TABLE(MMIJAVA_EXIT_SELECT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleVMExitSelectWindow),    
    WIN_ID(MMIJAVA_EXIT_SELECT_WIN_ID),
    WIN_TITLE(TXT_JAVA_RESUME_WIN_TITLE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIJAVA_CTRL_ID_EXIT_VM),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

LOCAL BOOLEAN pause_win_launched = FALSE;
LOCAL BOOLEAN input_method_launched = FALSE;
LOCAL BOOLEAN file_browser_launched = FALSE;
LOCAL volatile BOOLEAN s_java_paused = FALSE;           //internal pause flag
LOCAL volatile BOOLEAN s_java_paused_external = FALSE;   //external pause flag
LOCAL MMIJAVA_VMEXIT_WIN_TYPE_T exit_win_type;
//LOCAL volatile BOOLEAN s_java_backgroundrunning = FALSE;

extern void javacall_prepare_inputmethod_data(MMIJAVA_TEXTEDITBOX_INIT_DATA_T *storeData);
extern void javacall_get_inputmethod_data(MMI_STRING_T *inputData,uint16 pos);

LOCAL javacall_keypress_type KeyTypeConvert(MMI_MESSAGE_ID_E messageId) {
    uint32 keyType = 0;
    int i = 0;

    keyType = messageId & 0xff00;
    for (i = 0; i < KEY_TYPE_NUM; i++) {
        if (keyType == keyTypeMapTable[i][1]) {
            return (javacall_keypress_type)keyTypeMapTable[i][0];
        }
    }

    javacall_printf("[KeyTypeConvert] >> Can not find the match key type. %d\n", keyType);
    return JAVACALL_KEYTYPED;
}

LOCAL BOOLEAN KeyCodeConvert(MMI_MESSAGE_ID_E messageId, javacall_key* java_key_code) {
    uint16 keyCode = 0;
    int i = 0;

    keyCode = (uint16)(messageId & 0x00ff);
    for (i = 0; i < KEY_CODE_NUM; i++) {
        if (keyCode == keyCodeMapTable[i][1]) {
            *java_key_code = (javacall_key)keyCodeMapTable[i][0];
            return TRUE;
        }
    }

    // fix me one day, using backspace to stand for unknown code currently.
    javacall_printf("[KeyCodeConvert] >> Can not find the match key code.\n");
    return FALSE;
}

LOCAL javacall_penevent_type PenEventConvert(MMI_MESSAGE_ID_E platformPenEvent) {
    javacall_penevent_type ret = JAVACALL_PENPRESSED;

    switch (platformPenEvent) {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
            ret = JAVACALL_PENPRESSED;
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            ret = JAVACALL_PENRELEASED;
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_LONG:
        case MSG_TP_PRESS_DOUBLE:
            ret = JAVACALL_PENHOLD;
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DRAG:
        case MSG_TP_PRESS_MOVE:
            ret = JAVACALL_PENDRAGGED;
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        default:
            //SCI_TRACE_LOW:"mmijava_main.c PenEventConvert platformPenEvent=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_284_112_2_18_2_30_33_1,(uint8*)"d",platformPenEvent);
            break;
    }

    return ret;
}

/*****************************************************************************/
//  Description : create java module. 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_EnterMain(void)
{
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_JAVA);
#endif 
   if( MMK_IsOpenWin(MMI_JAVA_WIN_ID))
        return ;
    MMK_CreateWin((uint32*)MMI_JAVA_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_InitModule(void)
{
//#ifndef _MSC_VER
    MMIJAVA_RegJavaNv();
//#endif
}
/*****************************************************************************/
//  Description : to init the java module
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
//extern void JavaTask(void);

BLOCK_ID g_java_task_id;
uint32 g_java_old_priority;

//lint -esym(552, startTimeInMillisecond) 
#ifdef _MSC_VER
__int64 startTimeInMillisecond;
const __int64 deltaTime = 315532800;//946684800;
#else
long long startTimeInMillisecond;   /*lint !e552 */
//seconds from 1970 to 2000
const long long deltaTime = 315532800ll;//946684800ll;
#endif

uint32 deltaTick = 0;

static void setStarttime() {
    uint32 startTime, startTick;

#ifdef WIN32
__int64 currTimeInMillisecond;
#else
long long currTimeInMillisecond;
#endif

    startTime = TM_GetTotalSeconds();
    startTick = SCI_GetTickCount();
    currTimeInMillisecond = (startTime + deltaTime)  * 1000 + (startTick - deltaTick) % 1000;
    startTimeInMillisecond = currTimeInMillisecond - startTick - (uint32)(MMIAPIACC_GetLocalTimeZone() * 3600 * 1000);
}
static void jvmTask(uint32 argc, void *argv) {
    uint32 time, tick, startTime, startTick;

    time = TM_GetTotalSeconds();
    tick = SCI_GetTickCount();
    do {
        startTime = TM_GetTotalSeconds();
        startTick = SCI_GetTickCount();
    } while (time == startTime);

    //startTimeInMillisecond = (startTime + deltaTime) * 1000 - startTick - MMIAPIACC_GetLocalTimeZone() * 3600 * 1000;
    deltaTick = (startTick - tick) % 1000;

    SCI_SLEEP(5000);
#ifndef SC6600L_ARM7
    // ARM7 device is lack of memory to support the preload midlet
    javacall_preloader_check_n_install();
    javacall_printf("javacall_preloader_check_n_install, ok");
#endif
    javanotify_check_suites_integrity();
   javacall_printf("javanotify_check_suites_integrity,ok");
   java_CreateSigDispatchTask();
    JavaTask();
   javacall_printf("jvmTask ,leave ok");
}

#ifdef BROWSER_SUPPORT
LOCAL void JavaJadFile_otacalback(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    wchar fileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0,};
    int fileNameLen = 0;
    int i;

//    unsigned short tmpFileName[MMIFILE_FULL_PATH_MAX_LEN] = {0,};
//    int tmpFileNameLen = MMIFILE_FULL_PATH_MAX_LEN;
//    unsigned short filesep[2];
//    javacall_handle fileHandle;
//
//    javacall_dir_get_root_path(tmpFileName, &tmpFileNameLen);
//    tmpFileName[tmpFileNameLen] = 0;
//
//    filesep[0] = javacall_get_file_separator();
//    filesep[1] = 0;
//    
//    MMIAPICOM_Wstrcat(tmpFileName, filesep);
//    MMIAPICOM_Wstrcat(tmpFileName, L"wapDownloadedTmp.jad");
//    tmpFileNameLen = MMIAPICOM_Wstrlen(tmpFileName);
//
//    // Create temp file to store the jad information
//    javacall_file_open(tmpFileName, tmpFileNameLen, JAVACALL_FILE_O_CREAT | JAVACALL_FILE_O_RDWR, &fileHandle);
//    javacall_file_write(fileHandle, buf, len);
//    javacall_file_close(fileHandle);

    //SCI_TRACE_LOW:"mmijava_export.c JavaJadFile_otacalback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_403_112_2_18_2_30_33_2,(uint8*)"");

    if(!param_ptr->download_result)
    {
        return ;
    }   
	
	#ifdef VT_SUPPORT	
    if (MMIAPIVT_IsVtCalling())
    {
        //SCI_TRACE_LOW:"mmijava_export.c JavaJadFile_otacalback MMIAPIVT_IsVtCalling"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_413_112_2_18_2_30_33_3,(uint8*)"");
        //删除拔电板后留下来的临时文件
        //RemoveTmpDownloadFileWhenInit();
        return;
    }
	#endif
	
	MMIJAVA_WriteOtaCallbackFile(param_ptr);
	
    MMIAPICOM_Wstrcpy(fileName, param_ptr->file_name);
    fileNameLen = MMIAPICOM_Wstrlen(fileName);
    for (i=0; i<fileNameLen; i++) {
        if (fileName[i] == '/') {
            fileName[i] = '\\';
        }
    }
    MMIJAVA_INSTALL_FROM_BROWSER(fileName,fileNameLen, (unsigned char*)param_ptr->url_ptr);
    
    return ;
}

LOCAL void JavaJarFile_otacallback(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    wchar fileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0,};
    int fileNameLen = 0;
    int i;
//    unsigned short tmpFileName[MMIFILE_FULL_PATH_MAX_LEN] = {0,};
//    int tmpFileNameLen = MMIFILE_FULL_PATH_MAX_LEN;
//    unsigned short filesep[2];
//    javacall_handle fileHandle;
//
//    javacall_dir_get_root_path(tmpFileName, &tmpFileNameLen);
//    tmpFileName[tmpFileNameLen] = 0;
//
//    filesep[0] = javacall_get_file_separator();
//    filesep[1] = 0;
//    
//    MMIAPICOM_Wstrcat(tmpFileName, filesep);
//    MMIAPICOM_Wstrcat(tmpFileName, L"wapDownloadedTmp.jar");
//    tmpFileNameLen = MMIAPICOM_Wstrlen(tmpFileName);
//
//    // Create temp file to store the jad information
//    javacall_file_open(tmpFileName, tmpFileNameLen, JAVACALL_FILE_O_CREAT | JAVACALL_FILE_O_RDWR, &fileHandle);
//    javacall_file_write(fileHandle, jardata, jarlen);
//    javacall_file_close(fileHandle);

    //SCI_TRACE_LOW:"mmijava_export.c JavaJarFile_otacallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_459_112_2_18_2_30_34_4,(uint8*)"");

    if(!param_ptr->download_result)
    {
        return ;
    }
    
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        //删除拔电板后留下来的临时文件
        //RemoveTmpDownloadFileWhenInit();
        //SCI_TRACE_LOW:"mmijava_export.c JavaJarFile_otacallback MMIAPIVT_IsVtCalling"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_471_112_2_18_2_30_34_5,(uint8*)"");
        return;
    }
#endif

	MMIJAVA_WriteOtaCallbackFile(param_ptr);
	
    MMIAPICOM_Wstrcpy(fileName, param_ptr->file_name);
    fileNameLen = MMIAPICOM_Wstrlen(fileName);
    //MMIJAVA_INSTALL_FROM_BROWSER(param_ptr->file_name,MMIAPICOM_Wstrlen(param_ptr->file_name), param_ptr->url_ptr);
    for (i=0; i<fileNameLen; i++) {
        if (fileName[i] == '/') {
            fileName[i] = '\\';
        }
    }
    MMIJAVA_INSTALL_FROM_BROWSER(fileName,fileNameLen, (unsigned char*)param_ptr->url_ptr);

    return;
}
#endif

/*****************************************************************************/
//  Description : to init the java module
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Init(void)
{
	MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
#ifdef BROWSER_SUPPORT
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T jar_otacallback = {0,PNULL};
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T jad_otacallback = {0,PNULL};
#endif
    s_java_paused= FALSE;    
    s_java_paused_external = FALSE;
    MMIJAVA_RegWinIdNameArr();
    
    //删除拔电板后留下来的临时文件
    RemoveTmpDownloadFileWhenInit();
   
    g_java_task_id = SCI_CreateThread( 
            "JVM task", 
            "JVM task queue", 
            jvmTask, 
            0, 
            0, 
            32768, 
            64,
#ifdef SC6600L_ARM7
            PRI_JVM_TASK,
#else
            PRI_JVM_TASK, //same to MMI task, so camera can work
#endif
            SCI_PREEMPT, 
            SCI_AUTO_START);
    javacall_time_initialize();
    javacall_media_initialize();
#ifdef BROWSER_SUPPORT    
	jad_otacallback.mime_type = MMIBROWSER_MIME_TYPE_JAD;
	jad_otacallback.call_back = JavaJadFile_otacalback;
	jar_otacallback.mime_type = MMIBROWSER_MIME_TYPE_JAR;
	jar_otacallback.call_back = JavaJarFile_otacallback;
#ifndef CMCC_UI_STYLE //for cr NEWMS00160637
    MMIAPIBROWSER_RegMimeFunc(&jar_otacallback);
#endif

	MMIAPIBROWSER_RegMimeFunc(&jad_otacallback);
#endif	

	//create JAVA dir for storaging jar,jad
	for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
	{
		MMIJAVA_CreateJavaDirectory(device);
	}
}

/*****************************************************************************/
//  Description : RemoveTmpDownloadFileWhenInit
//  Global resource dependence : none
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void RemoveTmpDownloadFileWhenInit(void)
{
    MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr = PNULL;    
    param_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T));    
    SCI_MEMSET(param_ptr, 0x00, sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T));

    if(0 != MMIJAVA_ReadOtaCallbackFile(param_ptr)) //read nv failed ,return directly 
    {
        return ; 
    }
    //SCI_TRACE_LOW:"mmijava_main.c RemoveTmpDownloadFileWhenInit url = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_557_112_2_18_2_30_34_6,(uint8*)"s", param_ptr->url_ptr);
    if(PNULL != param_ptr->url_ptr)
    {
        MMIAPIFMM_DeleteFile(param_ptr->file_name, PNULL);
    }
#ifdef BROWSER_SUPPORT
    if(PNULL != param_ptr->url_ptr)
    {
        //SCI_TRACE_LOW:"mmijava_main.c RemoveTmpDownloadFileWhenInit user_data = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_565_112_2_18_2_30_34_7,(uint8*)"s", (uint32)(param_ptr->user_data));
        MMIAPIBROWSER_DeleteCompletedDownloadTask((uint32)(param_ptr->user_data));//删除下载列表
    }
#endif    

    SCI_MEMSET(param_ptr, 0x00, sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T));    
	MMIJAVA_WriteOtaCallbackFile(param_ptr);    //CLEAR nv
    SCI_FREE(param_ptr);
}

static char *config_fgets(char *line, int length, javacall_handle file_handle) {
    long read_length = 0;
    long actual_read = 1;

    while ((read_length < length-1) && (actual_read>0)) {
        actual_read = javacall_file_read(file_handle, (unsigned char *)&line[read_length], 1);
        if (0 < actual_read) {
            if (read_length==0 && ((line[0]=='\n') || (line[0]=='\r'))) {
                continue;
            }

            if (line[read_length] == '\n' || line[read_length] == '\r') {
                break;
            }

            read_length += actual_read;
        }
    }

    line[read_length] = 0;

    if (read_length > 0) {
        //javacall_printbinary(line, read_length+1);
        return line;
    }

    return NULL;
}

struct tck_config_t
{
    int initialized;
    int tck_mode;
    int pclink_mode;
    int domain;
    char url[100];
};

//lint -esym(552, java_install_mode)  
int java_install_mode = 1;  /*lint !e552 */
static struct tck_config_t g_tck_config={0, 0, 0,
    JAVACALL_LIFECYCLE_TCK_DOMAIN_MANUFACTURER,
    "http://129.158.145.188:1080/test/getNextApp.jad"};

/* file E:\javahome\tck.cfg contains configuration for tck.
 * it should looks like
 *
 * tck_mode=1
 * pclink_mode=1
 * domain=6
 * url=http://129.158.145.111:1080/test/getNextApp.jad
 */
 
/*lint --e{48} */ //isalnum pclint
static void read_tck_config()
{
#ifndef WIN32
    /*char fileName[]={0,'E',0,':',0,'\\',0,'j',0,'a',0,'v',0,'a',
        0,'h',0,'o',0,'m',0,'e',0,'\\',0,'t',0,'c',0,'k',0,'.',
        0,'c',0,'f',0,'g'};*/
    javacall_utf16 fileName[128] = {0,};
    int fileNameLen = 127;
    char line[100];
    char* tck_mode="tck_mode=";
    char* pclink_mode="pclink_mode=";
    char* install_mode="install_mode=";
    char* url="url=";
    char* domain="domain=";
    javacall_handle file_handle;
    javacall_result res;

    if (g_tck_config.initialized)
        return;

    javacall_dir_get_root_path(fileName, &fileNameLen);
    fileName[fileNameLen] = 0;
    MMIAPICOM_Wstrcat(fileName, L"\\tck.cfg");
    fileNameLen = MMIAPICOM_Wstrlen(fileName);

    res = javacall_file_open(fileName, fileNameLen, JAVACALL_FILE_O_RDONLY, &file_handle);
    if (res != JAVACALL_OK) {
        javacall_printf(LOG_STR("Error: Unable to open tck.cfg"));
        return;
    }
    
    //lint -esym(628, __rt_ctype_table*)     //pclint of isdigit
    //lint -esym(718, __rt_ctype_table*) 
    //lint -esym(746, __rt_ctype_table*)  
    //lint -esym(48, __rt_ctype_table*)  

    if (config_fgets(line, 100, file_handle) != NULL) {
        if (memcmp(line, tck_mode, SCI_STRLEN(tck_mode))==0)  
            if (isdigit(line[SCI_STRLEN(tck_mode)]))    /* lint !e48 */
                g_tck_config.tck_mode=line[SCI_STRLEN(tck_mode)]-'0';    }
    if (config_fgets(line, 100, file_handle) != NULL) {
        if (memcmp(line, pclink_mode, SCI_STRLEN(pclink_mode))==0) {
            javacall_printf(LOG_STR("pclink match"));            
            if (isdigit(line[SCI_STRLEN(pclink_mode)]) != 0) {
                //javacall_printf(LOG_STR("isdigit"));
                g_tck_config.pclink_mode=line[SCI_STRLEN(pclink_mode)]-'0';
            }
        }
    }
    if (config_fgets(line, 100, file_handle) != NULL) {
      if (memcmp(line, install_mode, SCI_STRLEN(install_mode))==0)
          if (isdigit(line[SCI_STRLEN(install_mode)]))
            java_install_mode=line[SCI_STRLEN(install_mode)]-'0';
    }
    if (config_fgets(line, 100, file_handle) != NULL) {
        if (memcmp(line, domain, SCI_STRLEN(domain))==0)
            if (isdigit(line[SCI_STRLEN(domain)]))
                g_tck_config.domain=line[SCI_STRLEN(domain)]-'0';
    }
    if (config_fgets(line, 100, file_handle) != NULL) {
        int32 length = SCI_STRLEN(url);
        if (memcmp(line, url, length)==0) {        
            SCI_MEMCPY(g_tck_config.url, line + length, 100 - length);
            g_tck_config.url[99]=0;
        }
    }
    javacall_printf(LOG_STR("tck=%d, pclink=%d, domain=%d, url=%s"), g_tck_config.tck_mode, g_tck_config.pclink_mode, g_tck_config.domain, g_tck_config.url);
    javacall_file_close(file_handle);
    g_tck_config.initialized = 1;
    return ;
#endif
}


LOCAL void JVM_MMI_DRAW_SPLASH()
{
     // Draw splash
    const GUI_LCD_DEV_INFO *lcd_dev_info =  MMITHEME_GetDefaultLcdDev();
    GUI_POINT_T         dis_point = {0};
    GUI_RECT_T       dist_rect  =  MMITHEME_GetFullScreenRect();
    MMI_IMAGE_ID_T       image_id =  IMAGE_JAVA_SPLASH ;    

    int16       image_widht = 0 ;
    int16       image_height = 0;

   GUIRES_GetImgWidthHeight((uint16*)&image_widht, (uint16*)&image_height, image_id , MMI_JAVA_WIN_ID);
      
   dis_point.x = (dist_rect.right + 1 - image_widht )/2;
   dis_point.y = (dist_rect.bottom + 1 - image_height )/2;

	LCD_FillRect(lcd_dev_info, dist_rect, MMI_WHITE_COLOR);

    GUIRES_DisplayImg(&dis_point,
        &dist_rect,
        &dist_rect,
        MMI_JAVA_WIN_ID,
        image_id,
        lcd_dev_info);
}

LOCAL wchar full_filename[MAX_FILENAME_LEN] = {0,};
LOCAL int full_filename_len = 0;

LOCAL wchar s_jadurl[1024] = {0,};
LOCAL int s_jadurl_len = 0;

LOCAL char suite_id[256] = {0,};
LOCAL int suite_id_len = 0;
LOCAL uint32 s_java_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;

//LOCAL BOOLEAN g_java_ring_settings_saved=FALSE;
//LOCAL BOOLEAN g_java_tp_ring_settings=FALSE;
//LOCAL BOOLEAN g_java_key_ring_settings=FALSE;
LOCAL char s_suiteId[10] =  {0};
LOCAL char *s_className = PNULL;

PUBLIC void java_save_clear_tp_key_ring(void)
{
    /*
    if (g_java_ring_settings_saved)
        return;

    g_java_ring_settings_saved=TRUE;
    g_java_key_ring_settings=MMIAudio_isEnableKeyRing();
    g_java_tp_ring_settings=MMIDEFAULT_GetEnableTpRing();

    if (g_java_key_ring_settings)
    {            
        MMIDEFAULT_EnableKeyRing(FALSE);
        MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);
        
    }
    if (g_java_tp_ring_settings)
    {            
        MMIDEFAULT_EnableTpRing(FALSE);
        MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);       
        
    }
    */
    MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_JAVA, FALSE);
}

//lint -esym(552, g_vmHoldLcd) 
PUBLIC BOOLEAN g_vmHoldLcd; /*lint !e552 */
LOCAL BOOLEAN s_generatePreinstalledMidlet = FALSE;
/*
 * After lose/get focus, the key event maybe lost for ever,
 * so we record the last key to produce the lose key release event.
 */
LOCAL BOOLEAN g_is_last_key_unreleased = FALSE;
LOCAL javacall_key g_last_pressed_key_code;

LOCAL int32 LaunchMidlet(void)
{

        if (! MMIJAVA_CheckWhetherAllowedToRun()) 
        {
            return 4;
        }
        
        if (!IsDevEnoughSpaceForJava())//没有存储空间
        {
            javacall_printf("MMIAPIJAVA_OpenMainMenu:has no dev space");
            return 5;
        }

        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN) 
           {
            MMIJAVA_EnterMain();
            return 1;
        } 
        else 
        {           
            if (javacall_lifecycle_get_running_suiteId() == atoi(s_suiteId)     /*lint !e746*/
                && javacall_lifecycle_get_running_suiteId() != 1)       /*lint !e746*/ //not ams midlet
            {
                s_className = PNULL;
                exit_win_type = RESUME_WIN;
                MMK_CreateWin((uint32*)MMIJAVA_EXIT_SELECT_WIN_TAB, &exit_win_type);
                return 2;
            } 
            else
            {       
            //two option continue with  last midlet or present midlet   
            //ADDING A FUNCTION HANDLEMENT
            MMIPUB_OpenQueryWinByTextId(TXT_JAVA_LAUNCHMIDLET_OPTION, IMAGE_PUBWIN_QUERY, PNULL, HandleSelectBeforeEnterJAVAQueryWinMsg);
            return 3;
            //shutdown_jvm();
            //javacall_lifecycle_set_ams_auto_start(1);
            }
        }
}

LOCAL MMI_RESULT_E HandleSelectBeforeEnterJAVAQueryWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            shutdown_jvm();
           javacall_lifecycle_set_ams_auto_start(1);
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);    
}

LOCAL void pause_jvm(BOOLEAN jvm_on_top)
{   
    javacall_printf(LOG_STR("jvm state = %d"),javacall_lifecycle_get_vm_state());
        if (JAVACALL_LIFECYCLE_MIDLET_PAUSED== javacall_lifecycle_get_vm_state() 
            ||  JAVACALL_LIFECYCLE_MIDLET_PAUSING == javacall_lifecycle_get_vm_state())
        {
            return ;
        }
        MMIJAVA_Set_PausedFlag(TRUE);
        javacall_lifecycle_state_changed(JAVACALL_LIFECYCLE_MIDLET_PAUSING, JAVACALL_OK);
        javacall_media_stop_tone(0);    //FOR dealing with audio quickly
        MMIJAVA_PauseAllMediaPlayer();
        mmijava_unlock_device_multimedia();
        MAIN_UpdateIdleJavaState(TRUE);        
        javanotify_pause();         //must after mmi task operation, or audio_pause will be reenterd
}

LOCAL void resume_jvm(BOOLEAN jvm_on_top)
{
    javacall_printf(LOG_STR_ENTER("JVM ON TOP = %d"), jvm_on_top);
    javacall_printf(LOG_STR("jvm state = %d"),javacall_lifecycle_get_vm_state());
  
    if (JAVACALL_LIFECYCLE_MIDLET_PAUSED == javacall_lifecycle_get_vm_state() 
        || JAVACALL_LIFECYCLE_MIDLET_PAUSING == javacall_lifecycle_get_vm_state())
    {
        
        MMIJAVA_Set_PausedFlag(FALSE);    //clear inter/external pause flag
        s_java_paused_external = FALSE;
        //java_save_clear_tp_key_ring();
        MMIJAVA_ResumeAllMediaPlayer();   //do not delete the line
        //mmijava_lock_device_multimedia();        
        javanotify_resume();       
        if (g_is_last_key_unreleased) {
            g_is_last_key_unreleased=FALSE;
            javacall_printf(LOG_STR("regenerate lost key release, code=%d"), g_last_pressed_key_code);
            javanotify_key_event(g_last_pressed_key_code, JAVACALL_KEYRELEASED);
        }        
    }     
    
    if (jvm_on_top) {
		g_vmHoldLcd = TRUE;
    // Reflush the screen when previous pause does not make effect
    
        if ((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_STARTED)
                || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_RESUMED)
                || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSED)
                || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSING))
        {    
            javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_FOREGROUND);
            javacall_lcd_flush(-1);  //should not call java function in app task
            //status bar
            MAIN_UpdateIdleJavaState(FALSE);
        }
    }
}
LOCAL void shutdown_jvm(void)
{
    //MMIJAVA_Set_BackgroundRunningFlag(FALSE);
    javacall_printf(LOG_STR_ENTER("jvm state = %d"),javacall_lifecycle_get_vm_state() );
    MMIJAVA_Set_PausedFlag(FALSE);
    g_vmHoldLcd = FALSE;
    g_is_last_key_unreleased = FALSE;
    javacall_media_stop_tone(0);       //FOR dealing with audio quickly
    MMIJAVA_CloseAllMediaPlayer();    
    MMIAPISET_SetVibrator(0,MMISET_RING_TYPE_OTHER);
    mmijava_unlock_device_multimedia();
    g_tck_config.initialized = FALSE;
    g_tck_config.tck_mode = FALSE;
    g_tck_config.pclink_mode = FALSE;
    if (javacall_lifecycle_get_vm_state() != JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN) {
        javanotify_shutdown();
    }
    
#ifndef WIN32
    // Restore CPU frequency
    if (s_java_chng_freq_req_handler != CHNG_FREQ_REQ_HANDLER_NULL) {
        CHNG_FREQ_RestoreARMClk(s_java_chng_freq_req_handler);
        CHNG_FREQ_DeleteReqHandler(s_java_chng_freq_req_handler);
        s_java_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
    }
#endif
    MAIN_UpdateIdleJavaState(FALSE);
}

#ifdef USE_PCSL_MEM_FOR_LCD
#define fast_change_screen_buffer_pointer(p)
#else
extern void fast_change_screen_buffer_pointer(javacall_bool store);
#endif
LOCAL void background_jvm(void) {
    javacall_printf(LOG_STR_ENTER(""));
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        return;
    fast_change_screen_buffer_pointer(JAVACALL_TRUE);
    g_vmHoldLcd = FALSE;
    javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_BACKGROUND);
    MAIN_UpdateIdleJavaState(TRUE);    
    javanotify_background();
}

LOCAL void foreground_jvm(void) {    
    javacall_printf(LOG_STR_ENTER(""));
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        return;
    fast_change_screen_buffer_pointer(JAVACALL_FALSE);
    g_vmHoldLcd = TRUE;
    javacall_lifecycle_set_vm_state(JAVACALL_LIFECYCLE_MIDLET_STARTED);
    javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_FOREGROUND);
    MAIN_UpdateIdleJavaState(FALSE);    
	javacall_lcd_flush(-1);  //should not call java function in app task
    javanotify_foreground();
}

LOCAL javacall_bool dmSilentInstall = JAVACALL_FALSE;
LOCAL MMI_RESULT_E HandleJavaWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param) {
    MMI_STRING_T wait_text = {0};
    javacall_keypress_type javaKeyType;
    javacall_key javaKeyCode;

    //SCI_TRACE_LOW("HandleJavaWinMsg msg_id = %x",msg_id);    
    javacall_printf(LOG_STR("with win id %x, msg id %x"), win_id, msg_id);
	switch(msg_id) {
	case MSG_OPEN_WINDOW:
        //would send status bar message when resume / foreground
        if ((javacall_lifecycle_get_vm_state() != JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
            &&(!javacall_lcd_is_full_screen_mode(0))) {   
            GUIWIN_CreateStbDyn(win_id);
            GUIWIN_ReDrawStbCtrl(win_id,FALSE);
        }
#if 0   //not found any usement now     
        if ((javacall_lifecycle_get_vm_state() != JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN) &&
            (javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND)){
            foreground_jvm();
        }
#endif        
        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_CREATING) {
            JVM_MMI_DRAW_SPLASH();
            g_vmHoldLcd = TRUE;
        } else if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN) {
            MMIJAVA_Set_PausedFlag(FALSE);
            setStarttime();
            pause_win_launched = FALSE;
            input_method_launched = FALSE;
            file_browser_launched = FALSE;
		g_vmHoldLcd = TRUE;

		// Set the high frequency when enter Java
#ifndef _MSC_VER
		if (CHNG_FREQ_REQ_HANDLER_NULL == s_java_chng_freq_req_handler)
		{
			s_java_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("java");
		}
		CHNG_FREQ_SetArmClk(s_java_chng_freq_req_handler,FREQ_INDEX_JAVA_HIGH);
#endif
            //java_save_clear_tp_key_ring();
		    g_is_last_key_unreleased = FALSE;
		read_tck_config();
		if (g_tck_config.tck_mode) {
			javacall_printf("HandleJavaWinMsg: javanotify_start_tck()");
			javanotify_start_tck(g_tck_config.url, g_tck_config.domain);
		} else if ((full_filename_len > 0) && (s_jadurl_len == 0)){
			javacall_printf("HandleJavaWinMsg: javanotify_install_midlet_from_filesystem()");
            javanotify_install_midlet_from_filesystem((javacall_utf16 *)full_filename,
                full_filename_len, dmSilentInstall, dmSilentInstall);
            dmSilentInstall = JAVACALL_FALSE;
			full_filename_len = 0;
		} else if ((full_filename_len > 0) && (s_jadurl_len > 0)){
			javacall_printf("HandleJavaWinMsg: javanotify_install_midlet_from_browser()");
            javanotify_install_midlet_from_browser(s_jadurl, s_jadurl_len, full_filename,
                full_filename_len, dmSilentInstall, dmSilentInstall);
            dmSilentInstall = JAVACALL_FALSE;
			// Does not reset the len vars here, wait until delete the temp files in JAVAMMI_REMOVE_TMP_DOWNLOAD_FILE
			//full_filename_len = 0;
			//s_jadurl_len = 0;
		} else if (suite_id_len > 0) {
			javacall_printf("HandleJavaWinMsg: javanotify_remove_suite()");
			javanotify_remove_suite(suite_id);  /*lint !e718 !e746 !e628*/  //not prototyped explictyly by sun
			suite_id_len = 0;
		} else if (s_className != PNULL) {
			javacall_printf("HandleJavaWinMsg: javanotify_start_midlet()");
			JVM_MMI_DRAW_SPLASH();
			javanotify_start_suite(s_suiteId);
			s_className = PNULL;
		} else {
			javacall_printf("HandleJavaWinMsg: javanotify_start()");
			JVM_MMI_DRAW_SPLASH();
			javanotify_start();
		}
        }
        //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
         MMIDEFAULT_AllowOpenKeylocWin(FALSE);
		break;
		
	case MSG_CLOSE_WINDOW:
        if (javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_SWITCHING_TO_BACKGROUND
                || javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND) 
                {               
            fast_change_screen_buffer_pointer(JAVACALL_TRUE);
            g_vmHoldLcd = FALSE;
        } 
        else if(javacall_lifecycle_get_ground_state() != JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        {
            shutdown_jvm();
            //in case somehow close all window directly, not call shutdown_jvm
        } 
       //MMIDEFAULT_TurnOnBackLight();
       MMIDEFAULT_AllowOpenKeylocWin(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_JAVA);
#endif          
		break;

	case MSG_FULL_PAINT:    
		break;
		
	case MSG_BACKLIGHT_TURN_ON:
		fast_change_screen_buffer_pointer(JAVACALL_FALSE);
		g_vmHoldLcd = TRUE;
		javacall_lcd_flush(-1); //should not call java function in app task
		break;
		
	case MSG_BACKLIGHT_TURN_OFF:
		fast_change_screen_buffer_pointer(JAVACALL_TRUE);
		g_vmHoldLcd = FALSE;
		break;  
		
	case MSG_GET_FOCUS:
        javacall_printf(LOG_STR("MSG_GET_FOCUS"));
        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        {
            javacall_printf(LOG_STR("jvm is already shutdown"));
            break;
        }     
        if (javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND &&
//            javacall_lcd_get_background_allowed() && javacall_lcd_get_cmcc_keep_background()) {
        javacall_lcd_get_background_allowed() ) {       /*lint !e718 !e746*/
            foreground_jvm();
        } else {
            resume_jvm(TRUE);
        }
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
		break;
		
	case MSG_LOSE_FOCUS:
		javacall_printf(LOG_STR("MSG_LOSE_FOCUS"));
        if (javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND)
        {
            javacall_printf(LOG_STR("jvm is already background"));
            break;
        }
        if ((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSING)
            || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSED)) {
            break;
        }
        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        {
            javacall_printf(LOG_STR("jvm is already shutdown"));
            break;
        }		
 //       if (javacall_lcd_get_background_allowed() && javacall_lcd_get_cmcc_keep_background()) {
        if (javacall_lcd_get_background_allowed() ) {
            background_jvm();
        } else 
        {
            g_vmHoldLcd = FALSE;    // release lcd control first
            if(input_method_launched == FALSE && file_browser_launched == FALSE && pause_win_launched == FALSE) 
            { 
                pause_jvm(TRUE);
            }  
        }
       MMIDEFAULT_AllowOpenKeylocWin(TRUE);
		break;  
		
	case MSG_APP_RED: //red dwon
        //if jvm is creating, do nothing
        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_CREATING)
            break;
        if (javacall_lifecycle_get_running_suiteId() == -1) {
            javacall_printf(LOG_STR("shutdown internal midlet"));
            shutdown_jvm();
            MMK_CloseWin(MMI_JAVA_WIN_ID);
            MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &wait_text );
            MMIPUB_OpenWaitWin(1,
            &wait_text,
            PNULL,
            PNULL,
            MMIJAVA_WAIT_WIN_ID,
            IMAGE_NULL,
            ANIM_PUBWIN_WAIT,
            WIN_ONE_LEVEL,
            MMIPUB_SOFTKEY_NONE,
            PNULL);
            break;
        }
		g_vmHoldLcd = FALSE;
        //java_restore_tp_key_ring();
        //sometimes midlet is not allowed to run in background
        if (javacall_lifecycle_get_vm_state() != JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        {
            javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_SWITCHING_TO_BACKGROUND);
        }
        //do not change the sequence of closing and opening window
        pause_win_launched = TRUE ;
        exit_win_type = PAUSE_WIN;
        MMK_CreateWin((uint32*)MMIJAVA_EXIT_SELECT_WIN_TAB, &exit_win_type); //直接创建，防止消息方式的画面滞后
        //MMIJAVA_SendJVMMessage2MMI(APP_JAVA_LAUNCH_VM_EXIT_WIN_IND, LAUNCH_VM_EXIT_WIN);
      
		break;

#ifdef FLIP_PHONE_SUPPORT		
	case MSG_KEYDOWN_FLIP: 
		g_vmHoldLcd = FALSE;
		//javanotify_shutdown();
        shutdown_jvm();
		break;
#endif
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
	case MSG_TP_PRESS_UP:
	case MSG_TP_PRESS_MOVE:
	case MSG_TP_PRESS_LONG:
	case MSG_TP_PRESS_DOUBLE:
	case MSG_TP_PRESS_DRAG:
		javanotify_pen_event((int)MMK_GET_TP_X(param), 
                (MMK_GET_TP_Y(param) - javacall_lcd_get_flushStartLine() > 0?(int)(MMK_GET_TP_Y(param) - javacall_lcd_get_flushStartLine()):0), 
			PenEventConvert(msg_id));
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	case MSG_APP_UPSIDE	:
	    javacall_media_increase_volume();
        break;
				
	case MSG_APP_DOWNSIDE:
        javacall_media_decrease_volume();
        break;
				
	default:
        if (FALSE == KeyCodeConvert(msg_id, &javaKeyCode))
            break;
		javaKeyType = KeyTypeConvert(msg_id);
		if ((s_generatePreinstalledMidlet == TRUE) && (javaKeyCode == JAVACALL_KEY_POUND) 
			&& (javaKeyType == JAVACALL_KEYPRESSED)) {
			// Generate the preinstalled midlet required files
			preload_init();
			preload_gen_file_one2n();
			preload_gen_file_zero();
		}
			if (g_is_last_key_unreleased == FALSE) {
				if ((javaKeyType == JAVACALL_KEYREPEATED) || (javaKeyType == JAVACALL_KEYRELEASED)) {
                    //javacall_printf(LOG_STR("regenerate lost key pressed for code=%d"), javaKeyCode);
					/* Because if user release key a little slowly, we will get a key repeat event,
					   and the key pressed event will confused the app. */
					//javanotify_key_event(javaKeyCode, JAVACALL_KEYPRESSED);
					g_is_last_key_unreleased = TRUE;
					g_last_pressed_key_code = javaKeyCode;
				}
			}
		javanotify_key_event(javaKeyCode, javaKeyType);
			if (javaKeyType == JAVACALL_KEYPRESSED) {
				g_is_last_key_unreleased = TRUE;
				g_last_pressed_key_code = javaKeyCode;
			}
			if (javaKeyType == JAVACALL_KEYRELEASED) {
			    g_is_last_key_unreleased = FALSE;
			}
		break;
    }
	
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : java resume bg jvm
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/  
PUBLIC void MMIJAVA_ResumeBackgroundJVM( void )
{
    javacall_printf(LOG_STR_ENTER("JVM state = %d, ground = %d"), 
        javacall_lifecycle_get_vm_state(), javacall_lifecycle_get_ground_state());    
      
    //check whether paused external rather not internal
    if(TRUE == s_java_paused_external && FALSE ==  MMIJAVA_Is_Paused())
    {   
        if (JAVACALL_LIFECYCLE_MIDLET_PAUSED == javacall_lifecycle_get_vm_state()
            || JAVACALL_LIFECYCLE_MIDLET_PAUSING == javacall_lifecycle_get_ground_state())
        {  
            MMIJAVA_ResumeAllMediaPlayer();   //do not delete the line
            javanotify_resume();       
        } 
    }
    s_java_paused_external = FALSE;
}

/*****************************************************************************/
//  Description : java pause bg jvm
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/  
PUBLIC void MMIJAVA_PauseBackgroundJVM( void )
{
   //at present, shoule pause jvm running, no matter foreground or background
   //called by audio pausebgplay interface
    javacall_printf(LOG_STR_ENTER("jvm state = %d, ground = %d"),
        javacall_lifecycle_get_vm_state(),javacall_lifecycle_get_vm_state()); 
    
    if (JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN == javacall_lifecycle_get_vm_state())
    {
        return ;    
    }
    s_java_paused_external = TRUE;
    
    //do not allow pause jvm many times
    if ((JAVACALL_LIFECYCLE_MIDLET_PAUSED == javacall_lifecycle_get_vm_state())
        || (JAVACALL_LIFECYCLE_MIDLET_PAUSING == javacall_lifecycle_get_vm_state()))
    {
        return;
    }
    
    javacall_media_stop_tone(0);    //FOR dealing with audio quickly
    MMIJAVA_PauseAllMediaPlayer();
    javacall_lifecycle_state_changed(JAVACALL_LIFECYCLE_MIDLET_PAUSING, JAVACALL_OK);
    javanotify_pause();

    //mmijava_unlock_device_multimedia(); //not need this line when paused by others      
    
}

int java_enable_pclink()
{
    return g_tck_config.pclink_mode;
}

/*****************************************************************************/
//  Description : java_enable_pclink
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/  
PUBLIC int32 MMIJAVA_enable_pclink(void)
{
    return g_tck_config.pclink_mode;
}



/*****************************************************************************/
//  Description : to open java main menu
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMainMenu(void)
{
    javacall_printf(("MMIAPIJAVA_OpenMainMenu:enter, vm stat=%d, ground=%d"), javacall_lifecycle_get_vm_state(), javacall_lifecycle_get_ground_state());

    if (! MMIJAVA_CheckWhetherAllowedToRun()) 
    {
        return ;
    }

    if (!IsDevEnoughSpaceForJava())//没有存储空间
    {
        javacall_printf("MMIAPIJAVA_OpenMainMenu:has no dev space");
        return;
    }
	
    //MMK_CloseWin(MMI_JAVA_WIN_ID);
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN) {
        //MMK_CreateWin((uint32*)MMI_JAVA_WIN_TAB, PNULL);
        MMIJAVA_EnterMain();
    } else {
        exit_win_type = RESUME_WIN;
        MMK_CreateWin((uint32*)MMIJAVA_EXIT_SELECT_WIN_TAB, &exit_win_type);
    }
    //A demo of launching preloaded midlet
//    MMIJAVA_LAUNCH_PRELOAD_MIDLET("Fetion");//主菜单中直接打开应用接口

    /*read_tck_config();
    if (g_tck_config.tck_mode) {
        javacall_printf("MMIAPIJAVA_OpenMainMenu: javanotify_start_tck()");
        javanotify_start_tck(g_tck_config.url, g_tck_config.domain);
    } else {
        javacall_printf("MMIAPIJAVA_OpenMainMenu: javanotify_start()");
        javanotify_start();
    }*/
}

/*****************************************************************************/
//  Description : check if there is enough dev space for java
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDevEnoughSpaceForJava(void)
{
    wchar * device_ptr = PNULL;
    uint16 device_len = 0;
    uint32  free_high = 0;
    uint32 free_low = 0;
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_SYSTEM;
    BOOLEAN          dev_ok  = FALSE;
    DEVICE_STATUS_E  dev_ret = DEVICE_STATUS_OK;
    uint32           txt_id  = 0;
    

    do
    {
        if(!MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            continue;
        }

        device_ptr = MMIAPIFMM_GetDevicePath(dev);	/*lint !e605 */	
        device_len = MMIAPIFMM_GetDevicePathLen(dev); 
        dev_ret    = MMIAPIUDISK_GetDeviceStatus(device_ptr, device_len);
        if(DEVICE_STATUS_FORMAT == dev_ret)
        {
            txt_id = TXT_SD_NOT_SUPPORT;
        }
        else if(DEVICE_STATUS_ERROR == dev_ret)
        {
#ifdef NANDBOOT_SUPPORT
            txt_id = TXT_UDISK_SD_NOT_EXIST; //nand提示u盘和sd卡都不存在
#else
            txt_id = TXT_SD_NO_EXIST;//nor只提示sd卡不存在
#endif
        }
        else
        {
            dev_ok     = TRUE;
        }
        
        break;
    }while(++dev < MMI_DEVICE_NUM);

    //没有有效的DEV
    if(!dev_ok)
    {
        MMIPUB_OpenAlertWarningWin(txt_id);
        return FALSE;
    }
   
    MMIAPIFMM_GetDeviceFreeSpace(device_ptr, device_len, & free_high, & free_low);
    
    if((0 == free_high)&&(0 == free_low))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        return FALSE;
        
    }    

    return TRUE;
}
extern void javacall_get_file_browser_data(FILEARRAY file_array) ;     /*lint -e{765}*/
PUBLIC void JavaOpenFileCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    javacall_printf("OpenSavePathCallBack is_success = %d!",is_success);
    if (is_success) {
        javacall_get_file_browser_data(file_array); /*lint !e718 !e746 !e628*/
    }
    //Free array
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
    file_browser_launched = FALSE;
    javanotify_file_browser_complete(JAVACALL_OK);
}

LOCAL void Mmijava_Open_FileBrowser()
{
    MMIFMM_FILTER_T filter = {0};
    FILEARRAY ret_array = MMIAPIFILEARRAY_Create(); 
    SCI_MEMCPY(filter.filter_str, MMIJAVA_INSTALL_FILE_SUFFIX, sizeof(MMIJAVA_INSTALL_FILE_SUFFIX));
    
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(NULL, 0, 
        &filter, FUNC_FIND_FILE_AND_ALLFOLER, FALSE, PNULL, ret_array, JavaOpenFileCallBack,FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

PUBLIC void Prepare_Launch_Midlet_Parameter(int suiteId, char *className)
{
    sprintf(s_suiteId, "%d", suiteId);
    s_className = className;
}

/*****************************************************************************/
// 	Description : Running from platform task, handle the message sent from VM 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMIJAVA_TEXTEDITBOX_INIT_DATA_T init_data;
PUBLIC void MMIJAVA_DISPATCH_VM_MESSAGE(void* signal)
{
    char *wapUrl = PNULL;
    MMI_HANDLE_T ctrl_handle = PNULL;
    JAVASIG_T  *sig = (JAVASIG_T*)signal;
    extern javacall_timer_callback_para call_para;
#ifdef BROWSER_SUPPORT    
    MMIBROWSER_ENTRY_PARAM brw_parm = {0};
#endif	

    javacall_printf("MMIJAVA_DISPATCH_VM_MESSAGE sig->javaSig = %d!",sig->javaSig);
    switch (sig->javaSig)
    {
    case SHUTDOWN_JAVA:
        shutdown_jvm();
        MMK_CloseWin(MMI_JAVA_WIN_ID);
        MMK_CloseWin(MMIJAVA_EXIT_SELECT_WIN_ID);       
        MMIPUB_CloseWaitWin(MMIJAVA_WAIT_WIN_ID);
        break;
    case LAUNCH_INPUT_METHOD:
        javacall_prepare_inputmethod_data(&init_data);
        javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_SWITCHING_TO_BACKGROUND);
        input_method_launched = TRUE;
        MMK_CreateWin((uint32*)MMIJAVA_TEXT_EDIT_WIN_TAB, &init_data);
        break;
    case LAUNCH_FILE_BROWSER:
        javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_SWITCHING_TO_BACKGROUND);
        file_browser_launched = TRUE;
        Mmijava_Open_FileBrowser();
        break;
#ifdef BROWSER_SUPPORT        
    case LAUNCH_WAP_BROWSER:
        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_STARTED)
        {
            if (javacall_lcd_get_cmcc_keep_background() == JAVACALL_TRUE) {
                background_jvm();
                MMK_CloseWin(MMI_JAVA_WIN_ID);
            } else {
                pause_jvm(TRUE);
            }
        }
        if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        {
            shutdown_jvm();
            MMK_CloseWin(MMI_JAVA_WIN_ID);
        }       
        wapUrl = javacall_lifecycle_get_wapUrlPointer();
//        javacall_lifecycle_set_ams_auto_start(0);
        brw_parm.type = MMIBROWSER_ACCESS_URL;
	 brw_parm.url_ptr = wapUrl;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	 brw_parm.dual_sys = MN_DUAL_SYS_MAX;     
#else
        brw_parm.dual_sys = MN_DUAL_SYS_1;     
#endif

        MMIAPIBROWSER_Entry(&brw_parm);
        //should not call funciton called by java task in app task
        javacall_lifecycle_release_url_memory();
        break;
#endif	        
    case START_AMS:
        shutdown_jvm();
        MMK_CloseWin(MMI_JAVA_WIN_ID);
        MMIJAVA_EnterMain();
        break;
    case CREATE_STATUS_BAR:
#ifdef DYNAMIC_CREATE_STATUSBAR
        if (MMK_IsOpenWin(MMI_JAVA_WIN_ID))
        {        
            //SCI_TRACE_LOW:"java status bar create enter"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_1504_112_2_18_2_30_36_8,(uint8*)"");
            GUIWIN_CreateStbDyn(MMI_JAVA_WIN_ID);
            ctrl_handle = NULL;
            ctrl_handle = MMK_GetCtrlHandleByWin(MMI_JAVA_WIN_ID,  MMITHEME_GetStatusBarCtrlId());
            MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, 0);
           //GUIWIN_ReDrawStbCtrl(MMI_JAVA_WIN_ID,FALSE);  
        }
#endif         
        break;
    case DESTROY_STATUS_BAR:
#ifdef DYNAMIC_CREATE_STATUSBAR
        if (MMK_IsOpenWin(MMI_JAVA_WIN_ID))
        {     
            //SCI_TRACE_LOW:"java status bar destroy enter"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_1517_112_2_18_2_30_36_9,(uint8*)"");
            ctrl_handle = MMK_GetCtrlHandleByWin(MMI_JAVA_WIN_ID,  MMITHEME_GetStatusBarCtrlId());
            MMK_DestroyControl(ctrl_handle);
        }
#endif
        break;
    case REFLUSH_JAVA_WIN:
        MMK_PostMsg(MMI_JAVA_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        break;
    case LAUNCH_MIDLET:
        LaunchMidlet();
        break;
    case LAUNCH_VM_EXIT_WIN:
        exit_win_type = PAUSE_WIN;
        MMK_CreateWin((uint32*)MMIJAVA_EXIT_SELECT_WIN_TAB, &exit_win_type);
        break;
    case BACKGROUND_TIMER_EVENT:
    //SCI_TRACE_LOW:"java BACKGROUND_TIMER_EVENT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_1534_112_2_18_2_30_36_10,(uint8*)"");
        if (call_para.handler) {
            call_para.handler(call_para.para);
        }
        break;
    case PDP_ACTIVE:
		java_Do_NetConnect();
	 	break;
    case PDP_DEACTIVE:
		java_Do_NetDisconnect();
	 	break;  	
    case JAVA_VIDEO_START :    
    case JAVA_VIDEO_CLOSE :     
    case JAVA_VIDEO_ACQUIRE_DEVICE :    
    case JAVA_VIDEO_STOP :     
    case JAVA_VIDEO_PAUSE :     
    case JAVA_VIDEO_RESUME :   
    case JAVA_VIDEO_SET_LOCATION :     
    case JAVA_VIDEO_SET_VISIBLE :    
    case JAVA_VIDEO_SET_FULLSCREEN :    
    case JAVA_VIDEO_RELEASE_DEVICE :   
    case JAVA_VIDEO_SEEK_FRAME :
    case JAVA_VIDEO_SKIP_FRAME :   
    case JAVA_VIDEO_GET_TIME:   
    case JAVA_VIDEO_GET_DURATION:
    case JAVA_VIDEO_GET_SIZE:  
     	java_HandleVideoMessage(sig->javaSig);      /*lint -e718 -e746*/
        break;
    default:
		break;        
    }
}

LOCAL BOOLEAN Prepare_Fileinstaller_Parameter(wchar *pathname, int pathname_len, wchar *filename, int filename_len) {
    wchar * device_ptr = PNULL;
    uint16 device_len = 0;
    uint32  free_high = 0;
    uint32 free_low = 0;	
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    
    dev = MMIAPIFMM_GetFirstValidDevice();
    if(dev >= MMI_DEVICE_NUM)
    {
        return FALSE;
    }
    else
    {
            device_ptr = MMIAPIFMM_GetDevicePath(dev);
            device_len = MMIAPIFMM_GetDevicePathLen(dev);         
    }
    
     MMIAPIFMM_GetDeviceFreeSpace(device_ptr, device_len, & free_high, & free_low);

     if((0 == free_high)&&(0 == free_low))
     {
         MMIPUB_OpenAlertWarningWin(TXT_FMM_SPACE_IS_NOT_ENOUGH);
            return FALSE;
            
     }
	 
     if (pathname_len + filename_len + 1 > MMIFMM_FULL_FILENAME_LEN) {
         //从文件管理器中获得的文件全路径长度不可能大于255
         javacall_printf(LOG_STR("file name length > 255"));
        return FALSE;
    }

    SCI_MEMSET(full_filename,0,sizeof(full_filename));
    full_filename_len = 0;
    SCI_MEMCPY(full_filename,L"file:///",8*sizeof(wchar));  /*lint !e737 */
    full_filename_len += 8;
    SCI_MEMCPY(full_filename+full_filename_len,pathname,(uint32)pathname_len*sizeof(wchar));
    full_filename_len += pathname_len;
    SCI_MEMCPY(full_filename+full_filename_len, L"\\", sizeof(wchar));  /*lint !e737 */
    full_filename_len ++;
    SCI_MEMCPY(full_filename+full_filename_len, filename, (uint32)filename_len*sizeof(wchar));
    full_filename_len += filename_len;
    return TRUE;
}
PUBLIC void MMIJAVA_INSTALL_FROM_FILESYSTEM(wchar *pathname, int pathname_len, wchar *filename, int filename_len)
{
    if (FALSE == Prepare_Fileinstaller_Parameter(pathname, pathname_len, filename, filename_len)) {
        javacall_printf(LOG_STR_LEAVE("prepare file parameter failed"));
        return;
    }
#ifdef SC6600L_ARM7
    MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
#endif
    LaunchMidlet();
    dmSilentInstall = JAVACALL_FALSE;
    s_is_need_tree_manage = FALSE;
}
PUBLIC void MMIJAVA_INSTALL_FROM_FILESYSTEM_FOR_DM(wchar *pathname, int pathname_len, wchar *filename, int filename_len)
{
    if (FALSE == Prepare_Fileinstaller_Parameter(pathname, pathname_len, filename, filename_len)) {
        javacall_printf(LOG_STR_LEAVE("prepare file parameter failed"));
        return;
    }
#ifdef SC6600L_ARM7
    MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
#endif
    LaunchMidlet();
    dmSilentInstall = JAVACALL_TRUE;//TRUE:不干预方式安装,FALSE:干预方式安装
    s_is_need_tree_manage = TRUE;
}

PUBLIC void MMIJAVA_INSTALL_FROM_BROWSER(wchar *local_pathnam, int local_pathname_len, unsigned char* url)
{
    int i = 0;

    if (local_pathname_len >= MMIFMM_FULL_FILENAME_LEN) {
        return;
    }

    full_filename_len = 0;
    SCI_MEMCPY(full_filename,L"file:///",8*sizeof(wchar));  /*lint !e737 */
    full_filename_len += 8;
    SCI_MEMCPY(full_filename+full_filename_len,local_pathnam, (uint32)local_pathname_len*sizeof(wchar));
    full_filename_len += local_pathname_len;

    s_jadurl_len = strlen((char*)url);
    if (s_jadurl_len > 1024) {
        return;
    }
    for (i = 0; i < s_jadurl_len; i++) {
        s_jadurl[i] = (wchar)url[i];
    }
    s_jadurl[i] = 0;

    //MMK_ReturnIdleWin();
    //MMK_CloseWin(MMICMSBRW_WEB_WIN_ID);
    //MMK_CloseWin(MMI_JAVA_WIN_ID);
    LaunchMidlet();
    dmSilentInstall = JAVACALL_FALSE;
    s_is_need_tree_manage = TRUE;
}

PUBLIC void JAVAMMI_REMOVE_TMP_DOWNLOAD_FILE()
{
    if ((s_jadurl_len > 0) && (full_filename_len > 8)) {
        // Exclude the "file:///" header
        
        //删除拔电板后留下来的临时文件
        RemoveTmpDownloadFileWhenInit();
        s_jadurl_len = 0;
        full_filename_len = 0;
    }
}

PUBLIC void JAVAMMI_javacall_notify_suiteID(int suiteID)
{
    
    if (0 < suiteID) //install
    {
        javanotify_get_suite_info(suiteID);
    }
    else if(0 > suiteID) //remove
    {
        MMIAPIDM_HandleScomoCallback(suiteID,PNULL,0,PNULL,0);
    }
}

PUBLIC void JAVAMMI_javacall_lifecycle_set_suite_info(
                                                      int suiteId, 
                                                      uint16* version, 
                                                      int versionLength, 
                                                      uint16* name, 
                                                      int nameLength
                                                      )
{
    if (s_is_need_tree_manage)
    {
        MMIAPIDM_HandleScomoCallback(suiteId,version,versionLength,name,nameLength);
    }
}
PUBLIC void MMIJAVA_REMOVE_MIDLET(wchar *suiteid, int suiteid_len)
{
    int32 i = 0;
    //SCI_TRACE_LOW:"MMIJAVA_REMOVE_MIDLET"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_1716_112_2_18_2_30_36_11,(uint8*)"");
    for(i=0; i<suiteid_len; i++)
        suite_id[i] = suiteid[i];

    suite_id[i] = 0;

    suite_id_len = suiteid_len;
    shutdown_jvm();
    MMK_CloseWin(MMI_JAVA_WIN_ID);
    MMIJAVA_EnterMain();
}
LOCAL void IMCallbackProcess(MMI_STRING_T * editbox_str_ptr,uint16 pos)
{
    javacall_get_inputmethod_data(editbox_str_ptr, pos);
    javanotify_textfield_complete(JAVACALL_TEXTFIELD_COMMIT);
}

LOCAL MMI_RESULT_E HandleJavaTextEditWinMsg(
											MMI_WIN_ID_T win_id, 
											MMI_MESSAGE_ID_E msg_id, 
											DPARAM param
											)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T editbox_str_info = {0};
    MMI_CONTROL_CREATE_T create = {0};
    MMIJAVA_TEXTEDITBOX_INIT_DATA_T *init_data_ptr = (MMIJAVA_TEXTEDITBOX_INIT_DATA_T*)MMK_GetWinAddDataPtr(win_id);
    GUIEDIT_INIT_DATA_T		edit_init_data = {0};
    uint16 pos = 0;
	
	//SCI_TRACE_LOW:"HandleJavaTextEditWinMsg msg_id = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_1746_112_2_18_2_30_36_12,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:        
        if (init_data_ptr == PNULL) {
            return MMI_RESULT_FALSE;
        }
		
        input_method_launched = TRUE;
        edit_init_data.type = init_data_ptr->edit_type;
      	edit_init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
     
      
        edit_init_data.str_max_len = init_data_ptr->max_str_len;		
        
        create.ctrl_id = MMIJAVA_CTRL_ID_IM; //Control id
        create.guid = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &edit_init_data; 
        
        MMK_CreateControl(&create); //Create Control dynamicly        
        
		if (GUIEDIT_TYPE_PASSWORD == edit_init_data.type)//密码输入界面增加输入法切换显示
		{
			BOOLEAN is_disp_im_icon = TRUE;
			BOOLEAN is_disp_num_info = TRUE;
			GUIEDIT_SetStyle(MMIJAVA_CTRL_ID_IM, GUIEDIT_STYLE_MULTI);//多行
                   GUIEDIT_SetAlign(MMIJAVA_CTRL_ID_IM, ALIGN_LEFT);//靠左上起			
			GUIEDIT_SetDispImIcon(MMIJAVA_CTRL_ID_IM,&is_disp_im_icon,&is_disp_num_info);
		}
		
        GUIEDIT_SetIm(MMIJAVA_CTRL_ID_IM,init_data_ptr->allow_inputmethod,init_data_ptr->cur_inputmethod);
        GUIEDIT_SetString(MMIJAVA_CTRL_ID_IM,init_data_ptr->wstr_ptr,(uint16)init_data_ptr->wstr_len);
        GUIEDIT_SetCursorCurPos(MMIJAVA_CTRL_ID_IM,init_data_ptr->cusor_pos );
        MMK_SetAtvCtrl(win_id, MMIJAVA_CTRL_ID_IM);
        //GUIWIN_SetSoftkeyTextId(win_id, STXT_OK, COMMON_TXT_NULL, STXT_RETURN, FALSE); //Softbutton
        break;
		
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:	
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(MMIJAVA_CTRL_ID_IM, &editbox_str_info);
        pos = GUIEDIT_GetCursorCurPos(MMIJAVA_CTRL_ID_IM);
        javacall_get_inputmethod_data(&editbox_str_info, pos);
        MMK_CloseWin(win_id);
        break;
		
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;
		
    case MSG_CLOSE_WINDOW:
        input_method_launched = FALSE;
        javanotify_textfield_complete(JAVACALL_TEXTFIELD_COMMIT);       
        break;
        
	case MSG_GET_FOCUS:
		break;
		
	case MSG_LOSE_FOCUS: 
		break;  
#if 0
	case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
#endif
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

LOCAL uint8 AppendVMOperListItem(
                                           MMI_CTRL_ID_T        ctrl_id,
                                           MMIJAVA_VMEXIT_WIN_TYPE_T    win_type
                                           )
{
    uint16  cur_selection   =   0;
    uint8 item_num = 0;

    //SCI_TRACE_LOW("mmijava_main.c AppendVMOperListItem javacall_lifecycle_get_vm_state is %d,win_type is %d",javacall_lifecycle_get_vm_state(),win_type);

    if (win_type == PAUSE_WIN) {
        if ((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_STARTED)
            || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_RESUMED)
            || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSED)
            || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSING))
        {
            MMIAPISET_AppendListItemByTextId(TXT_JAVA_VM_PAUSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            item_num ++;
        }

        MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_END, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        item_num ++;

        if (javacall_lcd_get_background_allowed() == JAVACALL_TRUE) {
            MMIAPISET_AppendListItemByTextId(TXT_JAVA_VM_BACKGROUND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            item_num ++;
        }
    } else if (win_type == RESUME_WIN) {
        if ((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSED)
            || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSING)) {
            MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_CONTINUE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            item_num ++;
        }
        else if ((javacall_lcd_get_background_allowed() == JAVACALL_TRUE) && 
            (javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND)) {
            MMIAPISET_AppendListItemByTextId(TXT_JAVA_VM_FOREGROUND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            item_num ++;
        } else if ((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_CREATING)
            && (javacall_lifecycle_get_ground_state() == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND)) {
            MMIAPISET_AppendListItemByTextId(TXT_JAVA_VM_FOREGROUND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            item_num ++;
        }  else if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_STARTED        
             || javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_RESUMED) {        
            MMIAPISET_AppendListItemByTextId(TXT_JAVA_VM_FOREGROUND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            item_num ++;
        }   //in case java win lose focus ,and red key was pressed, then jvm is running ,win was closed

        MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_END, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        item_num ++;
    }

    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    return item_num;
}

LOCAL MMI_RESULT_E HandleVMExitSelectWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param) {
    MMI_RESULT_E                result              =   MMI_RESULT_TRUE;
    uint16                      cur_selection       =   0;
    MMI_CTRL_ID_T               ctrl_id             =   MMIJAVA_CTRL_ID_EXIT_VM;
    uint8                       item_num            =   0;
    GUILIST_ITEM_T *            list_item_ptr      = PNULL;
    MMIJAVA_VMEXIT_WIN_TYPE_T*  init_data_ptr = (MMIJAVA_VMEXIT_WIN_TYPE_T*)MMK_GetWinAddDataPtr(win_id);
    MMI_STRING_T                wait_text = {0};
    MMI_TEXT_ID_T               select_text_id = 0;
    
    javacall_printf(LOG_STR_ENTER("msg id=%x, resume win=%d, vm state=%d, ground=%d\n"), msg_id, *init_data_ptr, javacall_lifecycle_get_vm_state(), javacall_lifecycle_get_ground_state());
    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        if (*init_data_ptr == PAUSE_WIN)
        {
            //MMIJAVA_PauseCurrentVideoMediaPlayer();
            pause_win_launched = TRUE ;
        }
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0) 
        {
            GUILIST_RemoveAllItems(ctrl_id);
        }
        GUILIST_SetMaxItem(ctrl_id, MMIJAVA_MAX_VM_OPERATION, FALSE ); 
        item_num = AppendVMOperListItem(ctrl_id, *init_data_ptr);
        //SCI_TRACE_LOW:"HandleVMExitSelectWindow item_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_1907_112_2_18_2_30_37_13,(uint8*)"d",item_num);
        if (0 == item_num) {
            MMK_CloseWin(win_id);
            break;
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CLOSE_WINDOW:
         if (*init_data_ptr == PAUSE_WIN)
        {    
             pause_win_launched = FALSE ;
        }
        break;
        
    case MSG_FULL_PAINT:            
        break;
        
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_selection);  /*lint !e605*/
        select_text_id = list_item_ptr->item_data_ptr->item_content[0].item_data.text_id;
                     
        switch(select_text_id)//使用字串来做判断，不容易乱掉
        {
            case TXT_PAUSE:
                MMK_CloseWin(MMI_JAVA_WIN_ID);
                pause_jvm(FALSE);
                javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_FOREGROUND);
                break;
            case TXT_COMMON_COMMON_END:
                shutdown_jvm();
                MMK_CloseWin(MMI_JAVA_WIN_ID);
                MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &wait_text );

                MMIPUB_OpenWaitWin(1,
                &wait_text,
                PNULL,
                PNULL,
                MMIJAVA_WAIT_WIN_ID,
                IMAGE_NULL,
                ANIM_PUBWIN_WAIT,
                WIN_ONE_LEVEL,
                MMIPUB_SOFTKEY_NONE,
                PNULL);
                break;
            case TXT_COMMON_COMMON_CONTINUE:
                if ((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSED)
                    || (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_PAUSING))
                {               
                    //can not change the sequence,
                    //because of rusume_jvm will refresh the screen
                    MMIJAVA_EnterMain();    
                    resume_jvm(TRUE);  
                }                                                      
                //MMK_CreateWin((uint32*)MMI_JAVA_WIN_TAB, PNULL); 
                break;
             case TXT_JAVA_VM_BACKGROUND:
                 // MMIJAVA_ResumeCurrentVideoMediaPlayer();
                 MMK_CloseWin(MMI_JAVA_WIN_ID);
                 background_jvm();    
                 break;
             case TXT_JAVA_VM_FOREGROUND:
                 //	MIJAVA_ResumeCurrentVideoMediaPlayer();
                 MMIJAVA_EnterMain();
                 foreground_jvm();
                 break;
                
             default:
                 break;
                 
        }
        MMK_CloseWin(win_id);   
        break;
        
            case MSG_CTL_CANCEL:
            case MSG_KEYDOWN_CANCEL:
                if (*init_data_ptr == PAUSE_WIN) {
 //               MMIJAVA_ResumeCurrentVideoMediaPlayer();
            }
                  MMK_CloseWin(win_id);
                break;
                
            case MSG_APP_RED:
                if (*init_data_ptr == PAUSE_WIN) {
                MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &wait_text );
                MMIPUB_OpenWaitWin(1,
                &wait_text,
                PNULL,
                PNULL,
                MMIJAVA_WAIT_WIN_ID,
                IMAGE_NULL,
                ANIM_PUBWIN_WAIT,
                WIN_ONE_LEVEL,
                MMIPUB_SOFTKEY_NONE,
                PNULL);
                shutdown_jvm();
                MMK_CloseWin(MMI_JAVA_WIN_ID);                
                MMK_CloseWin(win_id);
                break;
                }
                result = MMI_RESULT_FALSE;
                break;
                
            default:
                result = MMI_RESULT_FALSE;
                break;
    }  
    return result;
}

/*****************************************************************************/
//  Description : get current java files storage device, sd or udisk.
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIJAVA_GetCurrentStoreDevice(void)
{ 
    return MMIAPIFMM_GetFirstValidDevice();
}
/*****************************************************************************/
//  Description : up java thread priority
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_UpThreadPriority(void)
{
#ifndef WIN32
    if (TX_SUCCESS == SCI_ChangeThreadPriority(g_java_task_id, SCI_PRIORITY_LOWEST - 2 , &g_java_old_priority)) {
        //javacall_printf(LOG_STR_LEAVE("ok"));
        return TRUE;
    }
    javacall_printf(LOG_STR_LEAVE("fail"));
#endif
    return FALSE;
}
/*****************************************************************************/
//  Description : down java thread priority
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_DownThreadPriority(void)
{
#ifndef WIN32
    if (TX_SUCCESS == SCI_ChangeThreadPriority(g_java_task_id, 31, &g_java_old_priority)) {
        //javacall_printf(LOG_STR_LEAVE("ok"));
        return TRUE;
    }
    javacall_printf(LOG_STR_LEAVE("fail"));
#endif
    return FALSE;
}

/*****************************************************************************/
//  Description : whether java win is open
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsOpenWin(void)
{
    javacall_printf(LOG_STR_ENTER(""));
    return MMK_IsOpenWin(MMI_JAVA_WIN_ID);
}

/*****************************************************************************/
//  Description:   to resume jbed installing processing when after mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to resume java
//  Return:
//       TRUE : resuming complete.
//       FALSE: don't need to resume or has been resumed.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module)
{
    if(MMK_IsOpenWin(MMI_JAVA_WIN_ID))
    {
        resume_jvm(TRUE);
    }
    return TRUE;

}
/*****************************************************************************/
//  Description:   to pause jbed installing processing when mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to pause java
//  Return:
//       TRUE : paused normally.
//       FALSE: don't need pause or has been paused.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module)
{
    pause_jvm(FALSE);	
    return TRUE;    
}

/*****************************************************************************/
//  Description : shutdown jvm
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Shutdown(MMIJAVA_PAUSE_TYPE_E module)
{
    shutdown_jvm();
    MMK_CloseWin(MMI_JAVA_WIN_ID);
    return TRUE;
}

/*****************************************************************************/
//  Description : shutdown jvm synchronous 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_ShutdownSync(void)
{
    //SCI_TRACE_LOW:"MMIJAVA_Shutdown enter vm_state is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_2123_112_2_18_2_30_38_14,(uint8*)"d",javacall_lifecycle_get_vm_state());
    
    while((javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_CREATING))//需要等待java初始话化完成,变成JAVACALL_LIFECYCLE_MIDLET_STARTED之后再shutdown
    {
        SCI_Sleep(200);
    }    

    //SCI_TRACE_LOW:"MMIJAVA_Shutdown start shutdown_jvm"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_2130_112_2_18_2_30_38_15,(uint8*)"");
    shutdown_jvm();

    while(MMIJAVA_IsJavaRuning())//需要等待java shutdown完成,jvm中会有保护,不会出现死循环的情况
    {
        SCI_Sleep(200);
    }
    MMK_CloseWin(MMI_JAVA_WIN_ID);
    //SCI_TRACE_LOW:"MMIJAVA_Shutdown leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_MAIN_2138_112_2_18_2_30_38_16,(uint8*)"");
    return TRUE;
}
/*****************************************************************************/
//  Description : check java is in shutdown (nor foreground or background)
//  Global resource dependence : none
//  Author: 
//  Note:
//  Return: TURE: java not runing in background or foreground,
//          FALSE: other
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsJavaShutdown(void)
{
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        return TRUE;
    else
        return FALSE;
}

PUBLIC void MMIJAVA_RunFromBackground(BOOLEAN background)
{
    if ((background) && (MMK_IsOpenWin(MMI_JAVA_WIN_ID)))
    {
        if (javacall_lifecycle_get_vm_state() != JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
           javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_BACKGROUND);
        MMK_CloseWin(MMI_JAVA_WIN_ID);  //can't call mmi fucntion in java task
    }
    else if ((!background) && (!(MMK_IsOpenWin(MMI_JAVA_WIN_ID))))
    {
        MMIJAVA_EnterMain();
        //MMK_CreateWin((uint32*)MMI_JAVA_WIN_TAB, PNULL);
    }
}
/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  liqing.peng
//  Param
//       void
//  Return:
//      void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_StopOperaSDPlug(void)
{
     javanotify_shutdown();
}
/*****************************************************************************/
//  Description:   whether java is running
//  Author     :   Arthur.Peng 2006/11
//  Param
//       void
//  Return:
//       TRUE : java is running.
//       FALSE: java is not running
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsJavaRuning( void )
{
    BOOLEAN result = FALSE;
    
    if (JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN != javacall_lifecycle_get_vm_state())
    {
        result = TRUE;
    }

    javacall_printf(LOG_STR_LEAVE("MMIJAVA_IsJavaRuning result is %d"),result);
    return result;
}
/*****************************************************************************/
//  Description : IsFocusJAVARunWin
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsFocusJAVARunWin(void)
{
    return MMK_IsFocusWin(MMI_JAVA_WIN_ID);
}

//extern BOOLEAN MMIAPIFMM_GetDeviceTypeStatus(MMIFILE_DEVICE_E memory_type);
/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_CreateJavaDirectory(MMIFILE_DEVICE_E cur_dev)
{
    BOOLEAN         ret                                         = TRUE; 
    wchar           full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          full_path_len                               = MMIFILE_FULL_PATH_MAX_LEN+2;
    const wchar     *  dir                                      = MMIJAVA_JAVA_MAIN_DIR_UCS2;
    uint16          dir_len                                     = MMIJAVA_JAVA_MAIN_DIR_BYTE_LEN;
    const wchar     *cur_dev_ptr                                = PNULL;
    
    if(!MMIAPIFMM_GetDeviceTypeStatus(cur_dev))
    {
        return FALSE;
    }

    cur_dev_ptr = MMIAPIFMM_GetDevicePath(cur_dev);
    
    SCI_MEMSET(full_path_name,0,((MMIFILE_FULL_PATH_MAX_LEN+2)*sizeof(wchar)));
    full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    if (MMIAPIFMM_CombineFullPath(cur_dev_ptr, MMIFILE_DEVICE_LEN,
                                dir, dir_len, 
                                NULL, 0, 
                                full_path_name, &full_path_len))
    {
        if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            if(!MMIAPIFMM_CreateDir(full_path_name, full_path_len))
            {  
                ret = FALSE;                
            }
        }
    }    
    
    return ret;

}

/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SendJVMMessage2MMI(APP_JAVA_SIGNALCODE_E signal, uint16 data)
{
    MmiAppCopyMsgS *sendSignal = PNULL;

    MmiCreateSignal(signal, sizeof(MmiAppCopyMsgS), (MmiSignalS**)&sendSignal);
    sendSignal->Sender = P_APP;
    sendSignal->data = data;
    MmiSendSignal(TASK_FL_ID, (MmiSignalS*)sendSignal);
}

/*****************************************************************************/
//  Description : install preload
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Preload_Install(void)
{
    javacall_preloader_check_n_install();
}

/*****************************************************************************/
//  Description : when java is runing backgroup, set to TRUE, else set to FALSE
//  Global resource dependence : none
//  Parameter: is_backgroup:TURE: java runing in backgroup; FALSE:java runing in foregroup;
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Set_PausedFlag(BOOLEAN is_paused)
{
    s_java_paused = is_paused;
}

/*****************************************************************************/
//  Description : check java is running in background or foreground
//  Global resource dependence : none
//  Author: 
//  Note:
//  Return: TURE: java runing in background; FALSE:java runing in foreground;
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Is_Paused(void)
{
    return s_java_paused;
}

/*****************************************************************************/
//  Discription: This function get net setting index
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC uint8 MMIJAVA_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8 local_java_network_index = 0;
#ifdef BROWSER_SUPPORT
	local_java_network_index = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);				
#endif
    return local_java_network_index;
}

/*****************************************************************************/
//  Discription: get app heap size for java 
//  Global resource dependence: None
//  Author: hui.zhao
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC long MMIJAVA_GetAppHeapMemSize( void )
{
	return MMIJAVA_MAX_SIZE;
}

/*****************************************************************************/
//  Discription: get rect for java txt editbox win  
//  Global resource dependence: None
//  Author: hui.zhao
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC GUI_RECT_T* MMIJAVA_GetInputMethodRect( void )
{
    static GUI_RECT_T inputMethodRect = {0};
    inputMethodRect =  MMITHEME_GetClientRect();
	return &inputMethodRect;
}

/*****************************************************************************/
//  Discription: do something when a midlet start 
//  Global resource dependence: None
//  Author: hui.zhao
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIJAVA_javacall_lifecycle_midlet_started( void )
{
    int suiteId = 0;//对于预制的应用suiteId对应preload_rt_static中的midSid
    
    if (suiteId == javacall_lifecycle_get_running_suiteId())
    {//在启动某个应用时,需要做一些特殊的处理,如停止背景播放等

    }
}
/**
 * Whether the platform supports pointer event.
 *
 * @retval <tt>JAVACALL_TRUE</tt> if a platform supports pointer event
 * @retval <tt>JAVACALL_FALSE</tt> if a platform does not support pointer event
 */
PUBLIC BOOLEAN mmijava_input_pointer_support(void) {
#ifdef TOUCH_PANEL_SUPPORT
    return TRUE;
#else
    return FALSE;
#endif
}

/**
 * Whether the platform supports motion event (Move the pen upon the device screen).
 *
 * @retval <tt>JAVACALL_TRUE</tt> if a platform supports motion event
 * @retval <tt>JAVACALL_FALSE</tt> if a platform does not support motion event
 */
PUBLIC BOOLEAN mmijava_input_motion_support(void) {
#ifdef TOUCH_PANEL_SUPPORT
    return TRUE;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description :MMIJAVA_CheckWhetherAllowedToRun
//  Global resource dependence : none
//  Author: ying.xu
//  Note: check if permit auto run java midlet
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_CheckWhetherAllowedToRun(void)
{     
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }   
    
    return TRUE;
}

PUBLIC void  mmijava_lock_device_multimedia(void)
{
    javacall_printf(LOG_STR(""));
    MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_JAVA, FALSE);
    MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_JAVA_BG);    //caution ,may call jvm_pause looply
    return ;
}

PUBLIC void mmijava_unlock_device_multimedia(void)
{
    javacall_printf(LOG_STR(""));
    MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_JAVA, TRUE);
    MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_JAVA_BG);    //caution ,may call jvm_resume  looply
    return ;    
}

PUBLIC int32 MMIJAVA_GetSimCount(void)
{
   return MMI_DUAL_SYS_MAX;
}

PUBLIC int32 MMIJAVA_IsSupportWIFI(void)
{
#ifdef MMI_WIFI_SUPPORT    
   return 1;
#else
   return 0;
#endif
}

PUBLIC int32  MMIJAVA_GetWIFIStatus(void)//connected
{
#ifdef MMI_WIFI_SUPPORT    
    if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())//connected
    {
        return 0;
    }   
    else
    {
        return -1;
    }
#else
    return -1;
#endif
}

PUBLIC uint32 MMIJAVA_GetWIFINetID(void)
{
#ifdef MMI_WIFI_SUPPORT    
    return MMIAPIWIFI_GetNetId();
#else
    return 0;
#endif
}


PUBLIC MMI_TEXT_ID_T* MMIJAVA_I18nGetText(int32 *num)
{
    static MMI_TEXT_ID_T indexMap[] = {
#include "javacall_i18n_StringResource_inc.h"
    };
    *num = sizeof(indexMap) / sizeof(indexMap[0]);
    return indexMap;
}

PUBLIC void* MMIJAVA_PermissionGetText(int32 *num)
{
    static struct _permission_string 
    {
        MMI_TEXT_ID_T list[DEF_NUM_OF_LINES];
    } permission_string[] = 
    {
        {{TXT_COMMON_NET,
            TXT_JAVA_AMS_MGR_NET_SETTINGS_QUE,
            TXT_COMM_SETTING_QUE_DONT,
            TXT_JAVA_PERMISSION_NET_ACCESS_DIALOG_TITLE,
            TXT_JAVA_PERMISSION_NET_ACCESS_QUE, 0
        }},
        /*{{TXT_JAVA_AMS_MGR_LOW_LEVEL_NET_SETTINGS,
        TXT_JAVA_AMS_MGR_LOW_LEVEL_NET_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_LOW_LEVEL_NET_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_LOW_LEVEL_NET_ACCESS_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_LOW_LEVEL_NET_ACCESS_QUE, 0
    }},*/
        {{TXT_JAVA_AMS_MGR_CALL_CONTROL_SETTINGS,
        TXT_JAVA_AMS_MGR_CALL_CONTROL_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_CALL_CONTROL_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_CALL_CONTROL_TITLE,
        TXT_JAVA_PERMISSION_CALL_CONTROL_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_AUTO_START_SETTINGS,
        TXT_JAVA_AMS_MGR_AUTO_START_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_AUTO_START_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_AUTO_START_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_AUTO_START_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_LOCAL_CONN_SETTINGS,
        TXT_JAVA_AMS_MGR_LOCAL_CONN_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_LOCAL_CONN_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_LOCAL_CONN_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_LOCAL_CONN_QUE, 0
        }},
        {{TXT_MSG_SETTINGS,
        TXT_JAVA_AMS_MGR_MSG_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_MSG_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_SEND_MESSAGE_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_SEND_MESSAGE_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_SECURE_MSG_SETTINGS,
        TXT_JAVA_AMS_MGR_SECURE_MSG_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_SECURE_MSG_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_SEND_SECURE_MESSAGE_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_SEND_SECURE_MESSAGE_QUE, 0
        }},
        {{TXT_COMMON_SETTINGS_ENTRIES,
        TXT_JAVA_AMS_MGR_REC_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_REC_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_MULTIMEDIA_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_MULTIMEDIA_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_READ_USER_DATA_SETTINGS,
        TXT_JAVA_AMS_MGR_READ_USER_DATA_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_READ_USER_DATA_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_READ_USER_DATA_TITLE,
        TXT_JAVA_PERMISSION_READ_USER_DATA_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_WRITE_USER_DATA_SETTINGS,
        TXT_JAVA_AMS_MGR_WRITE_USER_DATA_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_WRITE_USER_DATA_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_WRITE_USER_DATA_TITLE,
        TXT_JAVA_PERMISSION_WRITE_USER_DATA_QUE,
        TXT_JAVA_PERMISSION_WRITE_USER_DATA_ONESHOT_QUE
        }},
        {{TXT_JAVA_AMS_MGR_LOC_SETTINGS,
        TXT_JAVA_AMS_MGR_LOC_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_LOC_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_LOCATION_TITLE,
        TXT_JAVA_PERMISSION_LOCATION_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_LANDMARK_SETTINGS,
        TXT_JAVA_AMS_MGR_LANDMARK_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_LANDMARK_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_LANDMARK_TITLE,
        TXT_JAVA_PERMISSION_LANDMARK_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_PAYMENT_SETTINGS, 0, 0, 0, 0, 0}}, 
        {{TXT_JAVA_AMS_MGR_AUTHENTICATION_SETTINGS,
        TXT_JAVA_AMS_MGR_AUTHENTICATION_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_AUTHENTICATION_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_SIGNATURE_DIALOG_TITLE,
        TXT_JAVA_PERMISSION_SIGNATURE_QUE, 0
        }},
        {{TXT_JAVA_AMS_MGR_SMART_CARD_SETTINGS,
        TXT_JAVA_AMS_MGR_SMART_CARD_SETTINGS_QUE,
        TXT_JAVA_AMS_MGR_SMART_CARD_SETTINGS_QUE_DONT,
        TXT_JAVA_PERMISSION_SMART_CARD_TITLE,
        TXT_JAVA_PERMISSION_SMART_CARD_QUE, 0
        }},
        {{TXT_COMM_SECURITY, 0, 0, 0, 0, 0}} 
    };
    *num = sizeof(permission_string) / sizeof(permission_string[0]);
    return permission_string;
}

PUBLIC MMI_TEXT_ID_T* MMIJAVA_AudioGetMetadataText(int32 *num)
{
    static MMI_TEXT_ID_T array[] = 
    {
        TXT_JAVA_SONG_ALBUM,
        TXT_JAVA_SONG_ARTIST,
        TXT_JAVA_SONG_TITLE,
        TXT_JAVA_SONG_YEAR
    };
    *num = sizeof(array) / sizeof(array[0]);
    return array;
}

#endif
#if defined(__cplusplus)
}
#endif



/*Edit by script, ignore 12 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
