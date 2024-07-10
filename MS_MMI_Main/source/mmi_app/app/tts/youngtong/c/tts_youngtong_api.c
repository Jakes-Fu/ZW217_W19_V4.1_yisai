/*****************************************************************************
** File Name:      tts_youngtong_api.c                                       *
** Author:                                                                   *
** Date:           2007.5.8                                                  *
** Copyright:      All Rights Reserved.                                      *
** Description:    Youngtong tts                                             *
*****************************************************************************/

#include "mmi_app_tts_trc.h"
#ifdef TTS_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "threadx_os.h"
#include "os_apiext.h"
#include "block_mem.h"
#include "mmidc_export.h"
//#include  "mmiaudio_ctrl.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "ivTTS.h"
#include "mmiidle_export.h"
#include "freq_cfg.h"
#include "os_api.h"
#include "mmicc_export.h"
#include "mmk_timer.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmisd_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmisms_id.h"
#include "mmiudisk_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifm_export.h"
#include "mmiset_nv.h"
#include "mmi_nv.h"
#include "tts_youngtong_api.h"
#include "tts_youngtong_audio.h"
#include "socket_types.h"
#include "tcpip_api.h"
#include "mmisms_export.h"
#include "mmisms_app.h"

#ifndef YT_TTS_TWO_RES_FILE
#include "youngtone_tts_interface.h"
#else
#include "youngtone_tts_interface_6800.h"
#endif
#ifndef TTS_FILE_RES
#include "tts_data.h"
#endif
//added by tonny 2011-05-31
//#include "mmiset.h"
#include "mmiset_export.h"
//end added 
#include "mmitts_export.h"
#include "priority_app.h"
#include "setjmp.h" 
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#ifndef WIN32		
extern void yte_tts_init(void);
#endif
/**--------------------------------------------------------------------------*
                                     MACRO DEFINITION
 **--------------------------------------------------------------------------*/
/* constant for TTS heap size */
#define ivTTS_HEAP_SIZE     KiTTS_HEAP_BUFFER_SIZE //128000

#define PCM_STREAM_PLAY_MAX_LEN     (15*1024)

#define MMITTS_TASK_STACK_SIZE                    (12*1024) //任务栈大小

#define MMITTS_TASK_QUEUE_NUM                     8   //任务消息队列大小
// 把TTS的优先级提高一级是为了避免java 与TTS 同级而造成的tts 无法运行
// 而把MMI挂起，被JAVA阻塞的情况， 具体见cr184217
#define MMITTS_TASK_PRIORITY                    PRI_MMITTS_TASK //  29//(SCI_PRIORITY_IDLE - 1)    提高优先级不低于图片解码

//added by tonny 2011-05-31
#define TTS_YT_RES_FILE_NAME_LEN       (60)
//end added 

/* constant for cache allocation */
#ifdef TTS_FILE_RES

#define TTS_CALLBACK_CHECKPOINT		24


// jun.hu 把tts 资源文件放到卡文件下
LOCAL const wchar s_mmitts_reserved_dir[]  = { 't', 't', 's' ,0 };

#ifndef YT_TTS_TWO_RES_FILE
LOCAL const wchar s_mmitts_res_file_name[] = {'t', 't', 's', 'h', 'i', 'n', 'd', 'i', '.', 'd','a','t', 0};
LOCAL const wchar s_mmitts_default_file_path[] = L"D:\\tts\\ttshindi.dat";
#else
LOCAL uint16 s_mmitts_language_id = 0;

LOCAL  wchar s_mmitts_res_file_name_1[TTS_YT_RES_FILE_NAME_LEN] = {0};
LOCAL  wchar s_mmitts_default_file_path_1[TTS_YT_RES_FILE_NAME_LEN] = L"";
LOCAL  char s_char_mmitts_default_file_path_1[TTS_YT_RES_FILE_NAME_LEN] = "";

LOCAL  wchar s_mmitts_res_file_name_2[TTS_YT_RES_FILE_NAME_LEN] = {0};
LOCAL  wchar s_mmitts_default_file_path_2[TTS_YT_RES_FILE_NAME_LEN] = L"";
LOCAL  char  s_char_mmitts_default_file_path_2[TTS_YT_RES_FILE_NAME_LEN] = "";

LOCAL  wchar s_mmitts_full_file_path_1[TTS_YT_RES_FILE_NAME_LEN] = L"";
LOCAL  char s_char_mmitts_full_file_path_1[TTS_YT_RES_FILE_NAME_LEN] = "";

LOCAL  wchar s_mmitts_full_file_path_2[TTS_YT_RES_FILE_NAME_LEN] = L"";
LOCAL  char  s_char_mmitts_full_file_path_2[TTS_YT_RES_FILE_NAME_LEN] = "";

//udisk system root
LOCAL const wchar s_mmitts_dev_udisk[] = L"D:\\system";
LOCAL const char s_char_mmitts_dev_udisk[] = "D:\\system";
//sdcard system root
LOCAL const wchar s_mmitts_dev_sdcard[] = L"E:\\system";
LOCAL const char s_char_mmitts_dev_sdcard[] = "E:\\system";



#if defined(YT_TTS_VER_S2)

	// 上海优同科技 中文版本
	LOCAL const wchar s_mmitts_res_file_name_man_1[] = {'y', 't', '_', 't', 't', 's', '_', 'm','a', 'n', '_','d','a','t','a','_', 'o','n', 'e', '_', 's', '2', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_man_1[] = L"\\tts\\yt_tts_man_data_one_s2.dat";
	LOCAL const char s_char_mmitts_default_file_path_man_1[] = "\\tts\\yt_tts_man_data_one_s2.dat";

	LOCAL const wchar s_mmitts_res_file_name_man_2[] = {'y', 't', '_', 't', 't', 's', '_', 'm','a', 'n', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','_', 's', '2', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_man_2[] = L"\\tts\\yt_tts_man_data_two_01_s2.dat";
	LOCAL const char  s_char_mmitts_default_file_path_man_2[] = "\\tts\\yt_tts_man_data_two_01_s2.dat";


	// 上海优同科技 英语版本
	LOCAL const wchar s_mmitts_res_file_name_eng_1[] = {'y', 't', '_', 't', 't', 's', '_', 'e','n', 'g', '_','d','a','t','a','_', 'o','n', 'e', '_', 's', '2', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_eng_1[] = L"\\tts\\yt_tts_eng_data_one_s2.dat";
	LOCAL const char s_char_mmitts_default_file_path_eng_1[] = "\\tts\\yt_tts_eng_data_one_s2.dat";

	LOCAL const wchar s_mmitts_res_file_name_eng_2[] = {'y', 't', '_', 't', 't', 's', '_', 'e','n', 'g', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','_', 's', '2', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_eng_2[] = L"\\tts\\yt_tts_eng_data_two_01_s2.dat";
	LOCAL const char  s_char_mmitts_default_file_path_eng_2[] = "\\tts\\yt_tts_eng_data_two_01_s2.dat";

	/*TTS YOUNGTONG support Hindi*/
	// 上海优同科技 HINDI语 印地语
	LOCAL const wchar s_mmitts_res_file_name_hin_1[] = {'y', 't', '_', 't', 't', 's', '_', 'h','i', 'n', '_','d','a','t','a','_', 'o','n', 'e', '_', 's', '2','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_hin_1[] = L"\\tts\\yt_tts_hin_data_one_s2.dat";
	LOCAL const char s_char_mmitts_default_file_path_hin_1[] = "\\tts\\yt_tts_hin_data_one_s2.dat";

	LOCAL const wchar s_mmitts_res_file_name_hin_2[] = {'y', 't', '_', 't', 't', 's', '_', 'h','i', 'n', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','_', 's', '2','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_hin_2[] = L"\\tts\\yt_tts_hin_data_two_01_s2.dat";
	LOCAL const char  s_char_mmitts_default_file_path_hin_2[] = "\\tts\\yt_tts_hin_data_two_01_s2.dat";

	/*TTS YOUNGTONG support Bahasa*/
	// 上海优同科技 bahasa语，印度尼西亚语
	LOCAL const wchar s_mmitts_res_file_name_bahasa_1[] = {'y', 't', '_', 't', 't', 's', '_', 'b','a', 'h','a','s','a', '_','d','a','t','a','_', 'o','n', 'e', '_', 's', '2','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_bahasa_1[] = L"\\tts\\yt_tts_bahasa_data_one_s2.dat";
	LOCAL const char s_char_mmitts_default_file_path_bahasa_1[] = "\\tts\\yt_tts_bahasa_data_one_s2.dat";

	LOCAL const wchar s_mmitts_res_file_name_bahasa_2[] = {'y', 't', '_', 't', 't', 's', '_', 'b','a', 'h','a','s','a', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','_', 's', '2','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_bahasa_2[] = L"\\tts\\yt_tts_bahasa_data_two_01_s2.dat";
	LOCAL const char  s_char_mmitts_default_file_path_bahasa_2[] = "\\tts\\yt_tts_bahasa_data_two_01_s2.dat";

#else

	/*TTS YOUNGTONG support english*/
	LOCAL const wchar s_mmitts_res_file_name_eng_1[] = {'y', 't', '_', 't', 't', 's', '_', 'e','n', 'g', '_','d','a','t','a','_', 'o','n', 'e', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_eng_1[] = L"\\tts\\yt_tts_eng_data_one.dat";
	LOCAL const char s_char_mmitts_default_file_path_eng_1[] = "\\tts\\yt_tts_eng_data_one.dat";

	LOCAL const wchar s_mmitts_res_file_name_eng_2[] = {'y', 't', '_', 't', 't', 's', '_', 'e','n', 'g', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_eng_2[] = L"\\tts\\yt_tts_eng_data_two_01.dat";
	LOCAL const char  s_char_mmitts_default_file_path_eng_2[] = "\\tts\\yt_tts_eng_data_two_01.dat";

	/*TTS YOUNGTONG support Hindi*/
	LOCAL const wchar s_mmitts_res_file_name_hin_1[] = {'y', 't', '_', 't', 't', 's', '_', 'h','i', 'n', '_','d','a','t','a','_', 'o','n', 'e', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_hin_1[] = L"\\tts\\yt_tts_hin_data_one.dat";
	LOCAL const char s_char_mmitts_default_file_path_hin_1[] = "\\tts\\yt_tts_hin_data_one.dat";

	LOCAL const wchar s_mmitts_res_file_name_hin_2[] = {'y', 't', '_', 't', 't', 's', '_', 'h','i', 'n', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_hin_2[] = L"\\tts\\yt_tts_hin_data_two_01.dat";
	LOCAL const char  s_char_mmitts_default_file_path_hin_2[] = "\\tts\\yt_tts_hin_data_two_01.dat";

	/*TTS YOUNGTONG support Bahasa*/
	LOCAL const wchar s_mmitts_res_file_name_bahasa_1[] = {'y', 't', '_', 't', 't', 's', '_', 'b','a', 'h','a','s','a', '_','d','a','t','a','_', 'o','n', 'e', '.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_bahasa_1[] = L"\\tts\\yt_tts_bahasa_data_one.dat";
	LOCAL const char s_char_mmitts_default_file_path_bahasa_1[] = "\\tts\\yt_tts_bahasa_data_one.dat";

	LOCAL const wchar s_mmitts_res_file_name_bahasa_2[] = {'y', 't', '_', 't', 't', 's', '_', 'b','a', 'h','a','s','a', '_', 'd','a','t','a','_','t','w', 'o', '_', '0','1','.', 'd','a','t', 0};
	LOCAL const wchar s_mmitts_default_file_path_bahasa_2[] = L"\\tts\\yt_tts_bahasa_data_two_01.dat";
	LOCAL const char  s_char_mmitts_default_file_path_bahasa_2[] = "\\tts\\yt_tts_bahasa_data_two_01.dat";
	//end added 
#endif
#endif //YT_TTS_TWO_RES_FILE

LOCAL BOOLEAN s_is_prompt_when_err = TRUE;

#define TTS_HIDEN_DIR_LEN             3

#ifndef YT_TTS_TWO_RES_FILE
#define TTS_RES_FILE_NAME_LEN         12
#endif

#define TTS_RES_FILE_CHECK_BYTE_NUM   7   //IsResV4

#else //TTS_FILE_RES
#define     TTS_CALLBACK_CHECKPOINT     32
#endif //TTS_FILE_RES

typedef struct tagTCBParam
{
    ivPointer   pParam1st;
    ivPointer   pParam2nd;
    ivPointer   pParam3rd;
}TCBParam, *PCBParam;

typedef struct TxtParam_
{
    uint8* str;
    uint32 len;
    BOOLEAN is_unicode;
    uint32 position;
}TxtParam;

typedef struct
{
    SIGNAL_VARS
    int32 data;
} MMITTS_SIG_T;
typedef enum
{
    MMITTS_MSG_MIN = 0x304,
    MMITTS_MSG_START,
    MMITTS_MSG_STOP,
    MMITTS_MSG_OVER,
    MMITTS_MSG_EXIT,
    MMITTS_MSG_MAX
} MMITTS_MSG_TYPE_E;

typedef enum
{
    MMITTS_PLAYING,
    MMITTS_STOPING,
    MMITTS_EXITING,
    MMITTS_OVERING,
    MMITTS_IDLE
}MMITTS_STATUS_TYPE_E;


/**---------------------------------------------------------------------------*
 **                         LOCAL Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL BLOCK_ID                             s_tts_task_id = 0;    
#ifndef WIN32
LOCAL __align(4) MMITTS_STRING_T                  s_tts_text={0};
#else
LOCAL MMITTS_STRING_T                             s_tts_text={0};    
#endif
LOCAL BOOLEAN                                     s_tts_is_running           = FALSE;      //tts运行状态标志

LOCAL uint8*                                      s_tts_stream_buffer = PNULL;
//LOCAL uint8                                       g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] = {0};
LOCAL char*                                       s_tts_voice_data           = PNULL;
LOCAL BOOLEAN                                     MMITTS_user_stop           = FALSE;
LOCAL uint32                                      s_kitts_callback_count     = 0;          //kitts回调次数
LOCAL BOOLEAN                                     s_tts_SynthText_is_running = FALSE;      //tts运行状态标志
LOCAL uint8                                       s_tts_playover_timer_id    = 0;  

LOCAL MMITTS_STATUS_TYPE_E                 MMITTS_Task_status         = MMITTS_IDLE;

LOCAL char*                                       KiTTS_HeapBuffer           = PNULL;
LOCAL char*                                CycleBuffer                = NULL;
LOCAL uint32                               CycleBufferLen             = 0;
LOCAL uint32                               putPosition                = 0;
LOCAL uint32                               getPosition                = 0;
LOCAL BOOLEAN                              TTS_ClkIsChange            = FALSE;

// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT，  故请谨慎把此变量置为TRUE ...(jun.hu)....
LOCAL BOOLEAN                               s_asynch_stop             = FALSE; //
LOCAL uint32                                s_tts_pcm_stream_count       = 0;
jmp_buf                                     s_jumpbuf_tts;

#ifndef TTS_FILE_RES
#ifndef WIN32
LOCAL const unsigned char TTS_ResData[]=
{
#ifdef TTS_DATA_RES_LIMIT
    #include "tts_res_array_600k.dat"
#else
    #include "tts_res_array.dat"
#endif  
};
#endif
#else
char * tts_resDataCathe;
#endif

/**--------------------------------------------------------------------------*
                                     FUNCTION DECLARE
 **--------------------------------------------------------------------------*/ 
#ifndef YT_TTS_TWO_RES_FILE
LOCAL void TTS_SynthText(TxtParam* txtPar,char*HeapBuffer,uint32 HEAP_BUFFER_SIZE,void *ResData);
#else
LOCAL void TTS_SynthText(TxtParam* txtPar,char*HeapBuffer,uint32 HEAP_BUFFER_SIZE);
#endif
LOCAL void TTS_SendSignaltoTask(
                             MMITTS_MSG_TYPE_E type, 
                             BLOCK_ID task_id,
                             int32 data
                             );
LOCAL uint32 cycleBuffeGetUsedNum(void );
LOCAL BOOLEAN cycleBufferPut(char* Buffer,uint32 BufferLen);
LOCAL BOOLEAN cycleBufferGet(char* Buffer,uint32 *BufferLen_p);
LOCAL uint32 cycleBuffeGetEmptyNum(void );
LOCAL void cycleBufferInit( char* Buffer,uint32 BufferLen);
LOCAL void TTS_RestoreClk(void);
LOCAL void TTS_SetDevUdiskFileFullPath(void);
LOCAL void TTS_SetDevSDCardFileFullPath(void);

/*****************************************************************************/
//  Description : 申请的内存
//  Global resource dependence : none
//  Author: jun hu 2010/11/4 把它改成了动态分配的方式....
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TTS_MemoryAlloc(void);
/*****************************************************************************/
//  Description : 当需要异步停止TTS时.....
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void StopTTSByASynch(void);

#ifdef TTS_FILE_RES
/*****************************************************************************/
//  Description : create tts dir 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: for 把tts的文件夹放到系统文件夹隐藏起来....
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSHidenDir(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
#ifndef YT_TTS_TWO_RES_FILE

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSResouceFileHandle(
                                         MMIFILE_HANDLE* file_handle_ptr // [out]
                                         );

LOCAL BOOLEAN GetTTSResouceFileFullName(
                                        MMIFILE_DEVICE_E    dev_type,      //[IN]
                                        wchar*  full_path_name_ptr,    //[OUT]
                                        uint16* full_path_len_ptr      //[OUT])
                                        );
#else
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                        MMIFILE_DEVICE_E    dev_type,      //[IN]
                                        uint16 file_index,             //[IN:1,2]
                                        wchar*  full_path_name_ptr,    //[OUT]
                                        uint16* full_path_len_ptr      //[OUT])
                                        );

#endif //YT_TTS_TWO_RES_FILE

#endif //TTS_FILE_RES
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AUDIO_PcmStreamPlayCallback(//HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset);
                                                          HAUDIO hAudio, 
                                                          AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
                                                          void *pvOtherInfo
                                                          );
/*****************************************************************************/
//  Description : AllocTTS_HeapBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_HeapBuffer(void);
/*****************************************************************************/
//  Description : AllocTTS_VoiceData
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_VoiceData(void);
/*****************************************************************************/
//  Description : free tts HeapBuffer
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeKiTTS_HeapBuffer(void);
/*****************************************************************************/
//  Description : free tts VoiceData
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_VoiceData(void);
/*****************************************************************************/
//  Description : AllocTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_StreamBuffer(void);
/*****************************************************************************/
//  Description : FreeTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL void FreeTTS_StreamBuffer(void);
/*****************************************************************************/
//  Description : AllocTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_ResDataCathe(void);
/*****************************************************************************/
//  Description : FreeTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_ResDataCathe(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void TTS_HandleTTSPlayOverTimerMsg(
                                          uint8 timer_id,
                                          uint32   param
                                          );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TTSPlayPreCheck(TTS_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description : GetDitigMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetDitigMode(MMITTS_DIGIT_MODE_TYPE_E digit_mode);

/*****************************************************************************/
//  Description : GetPuncMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetPuncMode(MMITTS_PUNC_MODE_TYPE_E punc_mode);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AUDIO_PcmStreamPlayCallback(//HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset)
                                                          HAUDIO hAudio, 
                                                          AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
                                                          void *pvOtherInfo
                                                          )
{
    uint32 usedDataLen;   

    //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  enter1 time:%u used =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_390_112_2_18_3_3_49_234,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
    if(PNULL == s_tts_stream_buffer)
    {
        //SCI_TRACE_LOW:"[MMITTS]: AUDIO_PcmStreamPlayCallback s_tts_stream_buffer ERROR!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_393_112_2_18_3_3_49_235,(uint8*)"");
        if(!AllocTTS_StreamBuffer())
        {
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
    }
    ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =PCM_STREAM_PLAY_MAX_LEN;
    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr =s_tts_stream_buffer;

    while (cycleBuffeGetUsedNum()<=PCM_STREAM_PLAY_MAX_LEN) 
    {
        if (s_tts_is_running==FALSE||s_tts_SynthText_is_running==FALSE ||(MMITTS_user_stop==TRUE))
        {
            /** puiDataLength=0;*/
            break;
        }
        SCI_Sleep(50);
        //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  waiting time:%u used =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_410_112_2_18_3_3_49_236,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
    }

    if (!s_tts_is_running || MMITTS_user_stop)
    {
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength=0;
    }
        
    if(0 < ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength)
    {
        usedDataLen=cycleBuffeGetUsedNum();
        usedDataLen-=usedDataLen%2;
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = MIN(PCM_STREAM_PLAY_MAX_LEN,usedDataLen);
        
        cycleBufferGet((char *)(ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr), (uint32 *)&(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength));
        
        //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  enter2 time:%u used =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_426_112_2_18_3_3_49_237,(uint8*)"ud",SCI_GetTickCount (),cycleBuffeGetUsedNum());
        if(0 < ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength)
        {
            return AUDIO_STREAM_GET_SRC_SUCCESS;
        }
        else
        {
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"AUDIO_PcmStreamPlayCallback()  g_tts_pcm_stream_is_over SET TRUE!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_438_112_2_18_3_3_49_238,(uint8*)"");
        return AUDIO_STREAM_GET_SRC_ERROR;
    }

}

#ifndef WIN32

/* iTTS系统有PCM语音数据输出的时候会调用该回调函数输出生成的PCM数据 */ 
#ifdef SAVE_PCM_STREAM
MMIFILE_HANDLE g_file_handle = SFS_INVALID_HANDLE;
#endif

LOCAL int callback_function_for_yt_tts(int nFlag, char *pData, int nLen, int *pCommand)
{
    char* tempbufPtr = PNULL;
    uint32 nSampleNumber = 0;
	 
#ifdef SAVE_PCM_STREAM
	uint32 write_size = 0;
	MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
	SFS_ERROR_E ret = SFS_NO_ERROR;
	const wchar PCM_DATA_filename[] = { 'E', ':',MMIFILE_SLASH,'p','c','m','s','t','r','e','a','m','.','d','a','t',0 };
	//SCI_TRACE_LOW:"call back enter save file"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_460_112_2_18_3_3_49_239,(uint8*)"");
	if(SFS_INVALID_HANDLE==g_file_handle)
	{
	    file_handle = MMIAPIFMM_CreateFile(PCM_DATA_filename
		                                 ,SFS_MODE_APPEND|SFS_MODE_WRITE
		                                 ,NULL
		                                 ,NULL);

        if(SFS_INVALID_HANDLE!=file_handle)
        {
            g_file_handle = file_handle;
			//SCI_TRACE_LOW:"create file"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_471_112_2_18_3_3_49_240,(uint8*)"");
        }
        else
        {
            g_file_handle = SFS_INVALID_HANDLE;
            return ivTTS_ERR_FAILED;	   
        }
    }
#endif

    //SCI_TRACE_LOW:"callback_function_for_yt_tts() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_481_112_2_18_3_3_49_241,(uint8*)"");
    *pCommand = 1;

    if(nLen>0) 
    {
		//here we can play the speech data in pData,its size is nLen in byte.
        nSampleNumber = nLen;
        tempbufPtr = pData;

#ifdef SAVE_PCM_STREAM
        ret = MMIAPIFMM_WriteFile(
                                g_file_handle, 
                                tempbufPtr, 
                                nSampleNumber, 
                                &write_size, 
                                NULL
                                );
        //SCI_TRACE_LOW:"nSampleNumber:%d,write_size:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_498_112_2_18_3_3_49_242,(uint8*)"dd",nSampleNumber,write_size);
        
        if(0 != ret)
        {
            return ivTTS_ERR_FAILED;
        }
#endif

        if(nSampleNumber > 0)
        {
            //SCI_TRACE_LOW:"callback_function_for_yt_tts()  enter time:%u counter =%d dataLen= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_511_112_2_18_3_3_49_243,(uint8*)"udd",SCI_GetTickCount(), s_kitts_callback_count, nSampleNumber);

            while (cycleBuffeGetEmptyNum() <= nSampleNumber+PCM_STREAM_PLAY_MAX_LEN)
            {
                if (s_tts_is_running == FALSE //|| !TTS_IsCreatePcmStreamHandle()
                    || MMITTS_user_stop == TRUE || !TTSAPI_GetAudioIsOpen())
                {
                    //SCI_TRACE_LOW:"callback_function_for_yt_tts()  cycle error exit time:%u counter =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_520_112_2_18_3_3_49_244,(uint8*)"ud",SCI_GetTickCount (),s_kitts_callback_count);

			        return ivTTS_ERR_FAILED;
                }

                SCI_Sleep(50);

                //SCI_TRACE_LOW:"callback_function_for_yt_tts()  waiting time:%u empty =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_529_112_2_18_3_3_50_245,(uint8*)"ud",SCI_GetTickCount(), cycleBuffeGetEmptyNum());
            }

            if (s_tts_is_running == FALSE //|| !TTS_IsCreatePcmStreamHandle()
                || MMITTS_user_stop == TRUE || !TTSAPI_GetAudioIsOpen())
            {
                //SCI_TRACE_LOW:"callback_function_for_yt_tts()  error exit time:%u counter =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_537_112_2_18_3_3_50_246,(uint8*)"ud",SCI_GetTickCount (),s_kitts_callback_count);
                return ivTTS_ERR_FAILED;
            }

            s_kitts_callback_count++;
            if (s_kitts_callback_count == TTS_CALLBACK_CHECKPOINT)
            {
                //合成ing播放前的初始化
                //SCI_TRACE_LOW:"callback_function_for_yt_tts()  s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_545_112_2_18_3_3_50_247,(uint8*)"");
                s_tts_pcm_stream_count = TTS_GetPcmStreamId();
                MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_AUDIO_PLAY, &s_tts_pcm_stream_count, sizeof (s_tts_pcm_stream_count));
                MMI_TriggerMMITask();   //加快反应 
            }

            //SCI_TRACE_LOW:"callback_function_for_yt_tts()  enter time:%u used =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_553_112_2_18_3_3_50_248,(uint8*)"ud",SCI_GetTickCount(), cycleBuffeGetUsedNum());

            cycleBufferPut((char *) tempbufPtr, nSampleNumber);

            //SCI_TRACE_LOW:"callback_function_for_yt_tts()  enter time:%u used =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_559_112_2_18_3_3_50_249,(uint8*)"ud",SCI_GetTickCount(), cycleBuffeGetUsedNum());

            return ivTTS_ERR_OK;
        }
    }
	return ivTTS_ERR_OK;
}

LOCAL void UILogPrintDebug(char *p_text, unsigned int *p_timeSnapshot)
{
    //SCI_TRACE_LOW:"YT LOG: %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_569_112_2_18_3_3_50_250,(uint8*)"s",p_text);
}

LOCAL long yt_file_wfopen(const unsigned short *file_name_utf16)//open file for reading data only...
{	
    if(PNULL != file_name_utf16)
    {
	    MMIFILE_HANDLE hHandle = MMIAPIFMM_CreateFile(file_name_utf16, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
		if(SFS_INVALID_HANDLE == hHandle)
			return -1;
		else
			return (long)hHandle;	
    }
    else
    {
        return -1;
    }
}

LOCAL unsigned int yt_file_wfseek(void *file_handle, unsigned int offset, unsigned int whence)//whence:VEN_FILE_SEEK_SET 
{
	MMIFILE_HANDLE hHandle = (long)file_handle;
		
	if(SFS_INVALID_HANDLE != hHandle)
	{
		return (MMIAPIFMM_SetFilePointer(hHandle, offset, SFS_SEEK_BEGIN));		
	}
	else
	{
	    return 0;
	}
}

LOCAL unsigned int yt_file_wfread(void * pBuffer, unsigned int size, void* file_handle)
{
	uint32 bytes_read, bytes_to_read;
	MMIFILE_HANDLE hHandle = SFS_INVALID_HANDLE;

	hHandle = (long)file_handle;
	bytes_to_read = (uint32)size;

	if (SFS_INVALID_HANDLE!= hHandle)
	{
		if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile((MMIFILE_HANDLE)file_handle, pBuffer, bytes_to_read, &bytes_read, NULL))
		{
		    if (bytes_to_read != bytes_read)
		    {
                return 0;
		    }
		    else
		    {
		        return bytes_read;
		    }
		}
		else
		{
		    return 0;
		}
	}
	else
	{
		return 0;
	}
}

LOCAL void yt_file_wfclose(void *file_handle)
{
		MMIFILE_HANDLE hHandle = (long)file_handle;
		if(SFS_INVALID_HANDLE != hHandle)
		{
		    MMIAPIFMM_CloseFile(hHandle);
		}
}

#else //WIN32


int callback_function_for_yt_tts(int nFlag, char *pData, int nLen, int *pCommand)
{	
	return ivTTS_ERR_OK;
}

void UILogPrintDebug(char *p_text, unsigned int *p_timeSnapshot)
{
    ;
}

long yt_file_wfopen(const unsigned short *file_name_utf16)
{
    return SFS_INVALID_HANDLE;
}

unsigned int yt_file_wfseek(void *file_handle, unsigned int offset, unsigned int whence)
{
    return 0;
}

unsigned int yt_file_wfread(void * pBuffer, unsigned int size, void* file_handle)
{
    return 0;	
}

void yt_file_wfclose(void *file_handle)
{
    ;
}

#endif  //WIN32

/********************************Cycle buffer*********************************/


/*api*/
LOCAL uint32 cycleBuffeGetEmptyNum(void )
{
    if (putPosition>=getPosition)
        return CycleBufferLen-(putPosition-getPosition)-1;
    else
        return getPosition-putPosition-1;
        
}

LOCAL uint32 cycleBuffeGetUsedNum(void )
{
    uint32 len = 0;
    if (putPosition > getPosition)
    {
        len = putPosition - getPosition;
    }
    else if(getPosition > putPosition)
    {
        len = CycleBufferLen - (getPosition - putPosition);
    }
    return len;
}

LOCAL void cycleBufferInit( char* Buffer,uint32 BufferLen)
{
    CycleBuffer=Buffer;
    CycleBufferLen=BufferLen;
    putPosition=0;
    getPosition=0;
}

LOCAL BOOLEAN cycleBufferPut(char* Buffer,uint32 BufferLen)
{
    BOOLEAN ret = FALSE;
    if (cycleBuffeGetEmptyNum() > BufferLen)
    {
        uint32 i = CycleBufferLen - putPosition;
        if (BufferLen+putPosition > CycleBufferLen) 
        {
            SCI_MEMCPY(CycleBuffer+putPosition, Buffer, i);
            SCI_MEMCPY(CycleBuffer, Buffer+i, BufferLen-i);
        }
        else
        {
            SCI_MEMCPY(CycleBuffer+putPosition, Buffer, BufferLen);
        }
        
        putPosition = (BufferLen+putPosition) % CycleBufferLen;
        ret = TRUE;
    }   
    return ret;
}

LOCAL BOOLEAN cycleBufferGet(char* Buffer,uint32 *BufferLen_p)
{
    BOOLEAN ret      = FALSE;
    uint32 activeNum = cycleBuffeGetUsedNum();

    if (0 < activeNum)
    {
        uint32 i = CycleBufferLen - getPosition;
        uint32 outLen = MIN(*BufferLen_p, activeNum);
        if (outLen+getPosition > CycleBufferLen) 
        {
            SCI_MEMCPY(Buffer, CycleBuffer+getPosition, i);
            SCI_MEMCPY(Buffer+i, CycleBuffer, outLen-i);
        }
        else
        {
            SCI_MEMCPY(Buffer, CycleBuffer+getPosition, outLen);
        }
        
        *BufferLen_p = outLen;
        getPosition = (getPosition+outLen) % CycleBufferLen;
        ret = TRUE;
    }
    
    return ret;
    
}


LOCAL uint32 TTS_GetQueueAvilable(BLOCK_ID id)
{
    uint32          status              = 0;
    uint32          count               = 0;
    uint32          enqueued            = 0;
    uint32          store               = 0;
    char            *queue_name_ptr     = SCI_NULL;
    TX_THREAD       * suspended_thread  = SCI_NULL; 
    TX_QUEUE        * next_queue        = SCI_NULL;
    SCI_THREAD_T    * thread_block  = SCI_NULL;
       
    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(id);
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
                               &queue_name_ptr,
                               (uint32 *)&enqueued,
                               (uint32 *)&store,
                               (TX_THREAD **)&suspended_thread,
                               (uint32 *)&count,
                               (TX_QUEUE **)&next_queue);
    
    //SCI_TRACE_LOW:"TTS_GetQueueAvilable: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_782_112_2_18_3_3_50_251,(uint8*)"d",store);
    
    return store;
}

LOCAL void TTS_SendSignaltoTask(
                             MMITTS_MSG_TYPE_E type, 
                             BLOCK_ID task_id,
                             int32 data
                             )
{
    MMITTS_SIG_T *sig_ptr = PNULL;
    
    //保证消息能够到达
    if (task_id == PNULL)
    {
        return;
    }
    sig_ptr = (MMITTS_SIG_T *)SCI_ALLOC(sizeof(MMITTS_SIG_T));
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SendSignaltoTask sig_ptr error!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_803_112_2_18_3_3_50_252,(uint8*)"");
        return;
    }
    
    sig_ptr->SignalCode = (uint16)type;
    sig_ptr->SignalSize = sizeof(xSignalHeaderRec);
    sig_ptr->Sender = PNULL;
    sig_ptr->data = data;
    
    if(SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, task_id))
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SendSignaltoTask SCI_SendSignal error!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_814_112_2_18_3_3_50_253,(uint8*)"");
    }
}

PUBLIC void TTS_Task(
                       uint32 argc, 
                       void * argv
                       )
{
    MMITTS_SIG_T  *   sig_ptr = PNULL;
    BOOLEAN             is_task_exit = FALSE;
#ifdef TTS_FILE_RES
#ifndef YT_TTS_TWO_RES_FILE
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
#endif
#endif
    //SCI_TRACE_LOW:"TTS_Task start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_830_112_2_18_3_3_50_254,(uint8*)"");
    
    for(;;)
    {
        sig_ptr = (MMITTS_SIG_T *)SCI_GetSignal(SCI_IdentifyThread());
        
        if(PNULL != sig_ptr)
        {
            switch (sig_ptr->SignalCode)
            {
            case MMITTS_MSG_EXIT:
                MMITTS_Task_status=MMITTS_EXITING;
                //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_EXIT"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_842_112_2_18_3_3_50_255,(uint8*)"");
                is_task_exit=TRUE;
                break;
                
            case MMITTS_MSG_START:
                {
                    TxtParam txtPar;
                    //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_START"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_849_112_2_18_3_3_50_256,(uint8*)"");
                    MMITTS_Task_status = MMITTS_PLAYING;
                    
                    if (NULL==s_tts_voice_data ||NULL==KiTTS_HeapBuffer)
                    {
                        break;
                    }
                    
                    if (!s_tts_is_running )
                    {
                        break;
                    }
                    
                    s_kitts_callback_count  = 0;    
                    cycleBufferInit(s_tts_voice_data, KiTTS_DATA_BUFFER_SIZE);  
                    s_tts_SynthText_is_running=TRUE;
                    
                    txtPar.is_unicode=s_tts_text.is_ucs2;
                    txtPar.len=s_tts_text.tts_str_len;
                    txtPar.str=s_tts_text.str_arr;
                    txtPar.position=0;
#ifdef TTS_FILE_RES
#ifndef YT_TTS_TWO_RES_FILE
                    CreateTTSResouceFileHandle(&file_handle);
                    //SCI_TRACE_LOW:"TTS_Task :MMIAPIFMM_CreateFile: handle=%u"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_873_112_2_18_3_3_50_257,(uint8*)"u",file_handle);
                    
                    if (SFS_INVALID_HANDLE != file_handle)
                    {
                        
                        TTS_SynthText(&txtPar, KiTTS_HeapBuffer, ivTTS_HEAP_SIZE, (void *) tts_resDataCathe);
                        
                    }
                    else
                    {
                        MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL,0 );
                        MMI_TriggerMMITask();//加快反应
                    }
                    
                    if (SFS_INVALID_HANDLE != file_handle)
                    {
                        MMIAPIFMM_CloseFile(file_handle);
                    }
#else //YT_TTS_TWO_RES_FILE
                    TTS_SynthText(&txtPar, KiTTS_HeapBuffer, ivTTS_HEAP_SIZE);
#endif //YT_TTS_TWO_RES_FILE
                    
#else  //TTS_FILE_RES
                    TTS_SynthText(   &txtPar, KiTTS_HeapBuffer, ivTTS_HEAP_SIZE,( void *)TTS_ResData);
                    StopTTSByASynch();
#endif //TTS_FILE_RES
                    s_tts_SynthText_is_running=FALSE;
                }
                break;
                
            case MMITTS_MSG_STOP:
                //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_STOP"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_904_112_2_18_3_3_50_258,(uint8*)"");
                MMITTS_Task_status=MMITTS_STOPING;
                TTS_MemoryRelease();
                break;
                
            case MMITTS_MSG_OVER:
                MMITTS_Task_status=MMITTS_OVERING;
                //SCI_TRACE_LOW:"TTS_Task :recieve MMITTS_MSG_OVER"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_911_112_2_18_3_3_50_259,(uint8*)"");
                {
                    //启动定时器来关闭资源
                    if( s_tts_playover_timer_id != 0 )
                    {
                        MMK_StopTimer(s_tts_playover_timer_id);
                        s_tts_playover_timer_id = 0;
                    }
                    if (s_tts_is_running &&(MMITTS_user_stop==FALSE))
                    {
                        s_tts_playover_timer_id = MMK_CreateTimerCallback(  2000, TTS_HandleTTSPlayOverTimerMsg, NULL, FALSE);
                    }
                }
                break;
                
            default:
                break;
            }
            
            SCI_FREE(sig_ptr);
        }
        
        if (is_task_exit)
        {
            break;
        }
        
        MMITTS_Task_status = MMITTS_IDLE;
    }
    
    s_tts_task_id = 0;
    
    //SCI_TRACE_LOW:"TTS_Task exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_943_112_2_18_3_3_50_260,(uint8*)"");
    MMITTS_Task_status=MMITTS_IDLE;
    SCI_ThreadExit(); 
}



LOCAL void TTS_ChangeClk(void)
{
    if (!TTS_ClkIsChange)
    {
        TTS_ClkIsChange=TRUE;
        MMI_SetFreq(FREQ_INDEX_APP_HIGH);
    }
}

LOCAL void TTS_RestoreClk(void)
{
    if (TTS_ClkIsChange)
    {
        TTS_ClkIsChange=FALSE;
        MMI_RestoreFreq(); 
    }
}

PUBLIC BOOLEAN  TTSAPI_SynthTextByTask(void )
{    

    //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_971_112_2_18_3_3_50_261,(uint8*)"");
        
#ifdef TTS_FILE_RES
    if (!TTSAPI_checkResFile())
    {
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask no res file exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_976_112_2_18_3_3_50_262,(uint8*)"");
        return FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask res file find"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_981_112_2_18_3_3_50_263,(uint8*)"");
    }
#endif

    if (s_tts_is_running)
    {
        return FALSE;
    }

    if (!TTS_MemoryAlloc())
    {
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask no memory exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_992_112_2_18_3_3_50_264,(uint8*)"");
        if (s_is_prompt_when_err)
        {
        MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
        }
        return FALSE;
    }
#ifndef YT_TTS_TWO_RES_FILE
{
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    CreateTTSResouceFileHandle(&file_handle);
    //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask :MMIAPIFMM_CreateFile: handle=%u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1003_112_2_18_3_3_50_265,(uint8*)"u",file_handle);

    if (SFS_INVALID_HANDLE!=file_handle)
    {
        uint32 bytes_read, bytes_to_read;

        if(NULL == tts_resDataCathe)
        {
            if (s_is_prompt_when_err)
            {
                MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
            }
            MMIAPIFMM_CloseFile(file_handle); 
            TTS_MemoryRelease();
            return FALSE; 
        }

        bytes_to_read = MMIAPIFMM_GetFileSize(file_handle);

        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask :MMIAPIFMM_GetFileSize=%u"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1022_112_2_18_3_3_51_266,(uint8*)"u", bytes_to_read);

        if (bytes_to_read >= KiTTS_RESCATHEBUFFER_SIZE)
        {
            //MMIAPIFMM_CloseFile(s_file_handle);
            TTS_MemoryRelease();
//            TTS_RestoreClk();
            return FALSE;
        }
        MMIAPIFMM_ReadFile(file_handle, tts_resDataCathe, bytes_to_read, &bytes_read, NULL);

        MMIAPIFMM_CloseFile(file_handle);

        if (bytes_to_read != bytes_read)
        {
            TTS_MemoryRelease();
//            TTS_RestoreClk();
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}
#endif
         
    s_tts_is_running = TRUE; 

    if (TTSAPI_GetAudioIsOpen())
    {
        TTS_StopAndClosePcmStreamHandle();
    }
         
    if (0 == s_tts_task_id)
    {
        s_tts_task_id = SCI_CreateThread( 
                                        "T_TTS", 
                                        "Q_TTS", 
                                        TTS_Task, 
                                        0, 
                                        0, 
                                        MMITTS_TASK_STACK_SIZE, 
                                        MMITTS_TASK_QUEUE_NUM, 
                                        PRI_MMITTS_TASK, 
                                        SCI_PREEMPT, 
                                        SCI_AUTO_START
                                        );   
    }
    else
    {
         /*等待TTS TASK 处理结束*/
        int i=4;
        //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task created"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1075_112_2_18_3_3_51_267,(uint8*)"");
        while(i)
        {
            i--;
            if (MMITTS_TASK_QUEUE_NUM==TTS_GetQueueAvilable(s_tts_task_id))
                break;
            SCI_Sleep(100);
            //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task while"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1082_112_2_18_3_3_51_268,(uint8*)"");
        }
        /*等待一段时间还不能结束，就退出*/
        if (0==i)
        {
            //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task quue exit"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1087_112_2_18_3_3_51_269,(uint8*)"");
            TTS_MemoryRelease();
            return FALSE;
        }
                    
    }
    if (TTS_CreatePcmStreamHandle(s_tts_text.ring_type, AUDIO_PcmStreamPlayCallback,s_tts_text.tts_route))/*lint !e64*/
    {
        TTS_SendSignaltoTask(MMITTS_MSG_START,s_tts_task_id,0);
        TTS_ChangeClk();
        SCI_Sleep(50);  /*run TTS task*/
    }
    //SCI_TRACE_LOW:"TTSAPI_SynthTextByTask task exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1099_112_2_18_3_3_51_270,(uint8*)"");
    return TRUE;
}
/*****************************************************************************/
//  Description : AllocTTS_HeapBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_HeapBuffer(void)
{
    FreeKiTTS_HeapBuffer();
    // jun hu 2010/11/4 开始采用动态分配的方式.....
    /*KiTTS_HeapBuffer = (char*)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSHEAPBUFFER); */
    KiTTS_HeapBuffer = SCI_ALLOCA(KiTTS_HEAP_BUFFER_SIZE);
    return (PNULL != KiTTS_HeapBuffer);
    
}
/*****************************************************************************/
//  Description : AllocTTS_VoiceData
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_VoiceData(void)
{
    FreeTTS_VoiceData();
    // jun hu 2010/11/4 开始采用动态分配的方式.....
    /*s_tts_voice_data = (char*)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSVOICEDATA);*/
    s_tts_voice_data = SCI_ALLOCA(KiTTS_DATA_BUFFER_SIZE);
    return (PNULL != s_tts_voice_data);
    
}
/*****************************************************************************/
//  Description : AllocTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_ResDataCathe(void)
{
#ifdef TTS_FILE_RES
    FreeTTS_ResDataCathe();
    tts_resDataCathe = SCI_ALLOCA(KiTTS_RESCATHEBUFFER_SIZE);
    return (PNULL != tts_resDataCathe);
#else
    return TRUE;
#endif
}
/*****************************************************************************/
//  Description : FreeTTS_ResDataCathe
//  Global resource dependence : none
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_ResDataCathe(void)
{
#ifdef TTS_FILE_RES
    if(PNULL != tts_resDataCathe)
    {
        //SCI_TRACE_LOW:" tts tts_resDataCathe  is not null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1151_112_2_18_3_3_51_271,(uint8*)"");
        SCI_FREE(tts_resDataCathe);
        tts_resDataCathe = PNULL;
    }
#endif
}
/*****************************************************************************/
//  Description : free tts HeapBuffer
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeKiTTS_HeapBuffer(void)
{
    if(PNULL != KiTTS_HeapBuffer)
    {
        //SCI_TRACE_LOW:" tts KiTTS_HeapBuffer   is not null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1165_112_2_18_3_3_51_272,(uint8*)"");
        SCI_FREE(KiTTS_HeapBuffer);
        KiTTS_HeapBuffer = PNULL;
    }
}
/*****************************************************************************/
//  Description : free tts VoiceData
//  Global resource dependence : none
//  Author: jun hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeTTS_VoiceData(void)
{
    if(PNULL != s_tts_voice_data)
    {
        //SCI_TRACE_LOW:"tts s_tts_voice_data   is not null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1178_112_2_18_3_3_51_273,(uint8*)"");
        SCI_FREE(s_tts_voice_data);
        s_tts_voice_data = PNULL;
    }
}

/*****************************************************************************/
//  Description : AllocTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL BOOLEAN AllocTTS_StreamBuffer(void)
{
    if(PNULL != s_tts_stream_buffer)
    {
        //SCI_TRACE_LOW:"[MMITTS]:AllocTTS_StreamBuffer PNULL != s_tts_stream_buffer ERROR!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1192_112_2_18_3_3_51_274,(uint8*)"");
        SCI_FREE(s_tts_stream_buffer);
        s_tts_stream_buffer = PNULL;
    }
    s_tts_stream_buffer = SCI_ALLOCA(PCM_STREAM_PLAY_MAX_LEN);
    return (PNULL != s_tts_stream_buffer);
}
/*****************************************************************************/
//  Description : FreeTTS_StreamBuffer
//  Global resource dependence : none
//  Author: 
//  Note: 将g_tts_stream_buffer[PCM_STREAM_PLAY_MAX_LEN] 15k空间动态分配释放
/*****************************************************************************/
LOCAL void FreeTTS_StreamBuffer(void)
{
    if(PNULL != s_tts_stream_buffer)
    {
        //SCI_TRACE_LOW:"[MMITTS]:FreeTTS_StreamBuffer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1207_112_2_18_3_3_51_275,(uint8*)"");
        SCI_FREE(s_tts_stream_buffer);
        s_tts_stream_buffer = PNULL;
    }
}

/*****************************************************************************/
//  Description : 申请内存资源
//  Global resource dependence : none
//  Author: jun hu 2010/11/4 把它改成了动态分配的方式....
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TTS_MemoryAlloc(void)
{
    BOOLEAN result = TRUE;
    
    //SCI_TRACE_LOW:" [MMITTS]: TTS_MemoryAlloc entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1221_112_2_18_3_3_51_276,(uint8*)"");
    if(!AllocTTS_HeapBuffer())
    {
        //SCI_TRACE_LOW:" [MMITTS]: MemoryAlloc HeapBuffer failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1224_112_2_18_3_3_51_277,(uint8*)"");
        result  = FALSE;
    }

    if(result && !AllocTTS_VoiceData())
    {
        //SCI_TRACE_LOW:" [MMITTS]:  AllocTTS_VoiceData failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1230_112_2_18_3_3_51_278,(uint8*)"");
        result  = FALSE;
    }

    if(result && !AllocTTS_StreamBuffer())
    {
        //SCI_TRACE_LOW:" [MMITTS]:  AllocTTS_StreamBuffer failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1236_112_2_18_3_3_51_279,(uint8*)"");
        result  = FALSE;
    }
    
#ifdef TTS_FILE_RES
#ifndef YT_TTS_TWO_RES_FILE
    if(result && !AllocTTS_ResDataCathe())
    {
        //SCI_TRACE_LOW:"[MMITTS]:  AllocTTS_ResDataCathe failed!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1244_112_2_18_3_3_51_280,(uint8*)"");
        result  = FALSE;
    }
#endif
#endif
    if(result)
    {
        //SCI_TRACE_LOW:"[MMITTS]:KiTTS_HeapBuffer address = %x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1251_112_2_18_3_3_51_281,(uint8*)"d", (uint32)KiTTS_HeapBuffer );
        //SCI_TRACE_LOW:"[MMITTS]:KiTTS_voice data address = %x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1252_112_2_18_3_3_51_282,(uint8*)"d", (uint32)s_tts_voice_data );
    }
    else
    {
        FreeKiTTS_HeapBuffer();
        FreeTTS_VoiceData();
        FreeTTS_StreamBuffer();
#ifdef TTS_FILE_RES
#ifndef YT_TTS_TWO_RES_FILE
        FreeTTS_ResDataCathe();
#endif
#endif
    }
    return result;
}

/*****************************************************************************/
//  Description : 释放内存资源
//  Global resource dependence : none
//  Author: jun hu 2010/11/4 修改了此函数....
//  Note: 
/*****************************************************************************/
PUBLIC void TTS_MemoryRelease(void)
{
    //SCI_TRACE_LOW:" tts TTS_MemoryRelease entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1274_112_2_18_3_3_51_283,(uint8*)"");
    FreeKiTTS_HeapBuffer();
    FreeTTS_VoiceData();
    FreeTTS_StreamBuffer();
#ifdef TTS_FILE_RES
#ifndef YT_TTS_TWO_RES_FILE
    FreeTTS_ResDataCathe();
#endif
#endif
}
/*****************************************************************************/
//  Description :  同步停tts
//  Global resource dependence : none
//  Author: jun hu 2010/10 修改了此函数....
//  Note: 同步停tts,绝大数情况下都应调用此函数....
/*****************************************************************************/
PUBLIC void TTSAPI_StopSynthTextByTask(void)
{
    // int i=30;
    
    /*NOTE: !!下面这个is_call_ring不能要，否则会造成多个assert问题, jun.hu*/
    //BOOLEAN is_call_ring = TTSAPI_IsPlayRing(MMISET_RING_TYPE_CALL);
    
    //SCI_TRACE_LOW:"MMITTS TTSAPI_StopSynthTextByTask entry time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1295_112_2_18_3_3_51_284,(uint8*)"u",SCI_GetTickCount ());
    // 增加对MMITTS_user_stop的判断是支持对同步和异步停TTS的两种方式的支持，以使两种互斥进行.....
    if(!MMITTS_user_stop)
    {
        MMITTS_user_stop = TRUE;
        
        if (TTSAPI_GetAudioIsOpen())
        {
            TTS_StopAndClosePcmStreamHandle();
        }
        
        //启动定时器来关闭资源
        if( s_tts_playover_timer_id != 0 )
        {
            MMK_StopTimer(s_tts_playover_timer_id);
            s_tts_playover_timer_id = 0;
        }
        
        
        /*等待TTS TASK 结束*/
        if (s_tts_task_id!=0   /*&&   //Code from ZhenhaoHuang's advice....
            !is_call_ring*/)/*这里不能要is_call_ring这个条件,否则会造成一些assert问题，jun.hu*/
        {
            while (MMITTS_Task_status!=MMITTS_IDLE 
                || (MMITTS_TASK_QUEUE_NUM!=TTS_GetQueueAvilable(s_tts_task_id)))  
            {
                SCI_Sleep(100);
#if 0
                /*防止死循环*/
                if (0==i--)
                    break;
#endif
            }
        }
        
        TTS_RestoreClk();
        TTS_MemoryRelease();
        s_tts_is_running = FALSE;
        MMITTS_user_stop = FALSE;
    }

    //SCI_TRACE_LOW:"TTSAPI_StopSynthTextByTask exit time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1336_112_2_18_3_3_51_285,(uint8*)"u",SCI_GetTickCount ());
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: jun.hu
//  Note:// for异步停止TTS，该情况仅在来电接听时调用, 否则会造成没有释放TTS所占用的内存
// 而发生一些ASSERT， 故请谨慎调用此函数 ...(jun.hu)....
/*****************************************************************************/
PUBLIC void TTSAPI_StopASynchTextByTask(void)
{
    //SCI_TRACE_LOW:"MMITTS TTSAPI_StopASynchTextByTask entry time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1345_112_2_18_3_3_51_286,(uint8*)"u",SCI_GetTickCount ());
    
    if(!MMITTS_user_stop)
    {
        MMITTS_user_stop = TRUE;
        if (TTSAPI_GetAudioIsOpen())
        {
            TTS_StopAndClosePcmStreamHandle();
        }
        //启动定时器来关闭资源
        if( s_tts_playover_timer_id != 0 )
        {
            MMK_StopTimer(s_tts_playover_timer_id);
            s_tts_playover_timer_id = 0;
        }
        
        s_asynch_stop = TRUE;
    }
    
}
/*****************************************************************************/
//  Description : 当需要异步停止TTS时.....
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void StopTTSByASynch(void)
{
    if(s_asynch_stop)
    {
        TTS_RestoreClk();
        TTS_MemoryRelease();
        s_tts_is_running = FALSE;
        MMITTS_user_stop=FALSE;
        s_asynch_stop = FALSE;
    }
}
/*************************************************************************************/
                            /*TTS kernel*/
/*************************************************************************************/
#ifdef TTS_FILE_RES
/* read resource callback */
LOCAL void ivCall ReadResCB(
                      ivPointer     pParameter,     /* [in] user callback parameter */
                      ivPointer     pBuffer,        /* [out] read resource buffer */
                      ivResAddress  iPos,           /* [in] read start position */
                      ivResSize     nSize )         /* [in] read size */
{
    MMIFILE_HANDLE pFile = (MMIFILE_HANDLE)pParameter;
    uint32 bytes_read;
    
    MMIAPIFMM_SetFilePointer(pFile, iPos, MMIFILE_SEEK_BEGIN);

    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(
                                pFile,
                                pBuffer,
                                nSize,
                                &bytes_read,
                                NULL
                                ))
    {
        //SCI_TRACE_LOW:"[MMITTS]:ReadResCB FAIL!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1401_112_2_18_3_3_51_287,(uint8*)"");
        longjmp(s_jumpbuf_tts, 1);
    }
}
#else
/* read resource callback */
LOCAL void ivCall ReadResCB(
                      ivPointer     pParameter,     /* [in] user callback parameter */
                      ivPointer     pBuffer,        /* [out] read resource buffer */
                      ivResAddress  iPos,           /* [in] read start position */
                      ivResSize     nSize )         /* [in] read size */
{
    uint8 * pBuf = pParameter;
    memcpy(pBuffer,  pBuf+iPos, nSize);
}
#endif

/* input callback */
LOCAL  ivTTSErrID ivCall InputCB(
                          ivPointer     pParameter,     /* [in] user callback parameter */
                          ivPointer     pText,          /* [out] input text buffer */
                          ivSize ivPtr  pnSize )        /* [out] input text size */
{
    PCBParam    pParam;
    TxtParam* txtPar;
    uint32 copyLen;

    if (MMITTS_user_stop==TRUE)
        return ivTTS_ERR_END_OF_INPUT;
    
    pParam = (PCBParam)pParameter;
    txtPar=(TxtParam*)pParam->pParam1st;
    copyLen=MIN(*pnSize,txtPar->len-txtPar->position);

    //SCI_TRACE_LOW:"MMITTS: InputCB pParameter= %u, txtPar=%d ,copyLen=%d,pParam=%u,txtPar->position=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1433_112_2_18_3_3_51_288,(uint8*)"uddud",pParameter,txtPar,copyLen,pParam,txtPar->position);
    
    if (copyLen!=0)
        memcpy(pText,txtPar->str+txtPar->position,copyLen);
    
    txtPar->position+=copyLen;
        
    *pnSize = copyLen;
        
    return (0 == *pnSize)?ivTTS_ERR_END_OF_INPUT:ivTTS_ERR_OK;
}

/*****************************************************************************/
//  Description : GetPuncMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetPuncMode(MMITTS_PUNC_MODE_TYPE_E punc_mode)
{
	uint16 tts_punc_mode = 0;	

	return tts_punc_mode;
}

/*****************************************************************************/
//  Description : GetDitigMode
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetDitigMode(MMITTS_DIGIT_MODE_TYPE_E digit_mode)
{
	uint16 tts_digit_mode = 0;
	

	return tts_digit_mode;
}

#ifndef YT_TTS_TWO_RES_FILE
LOCAL void TTS_SynthText(TxtParam * txtPar, char *HeapBuffer,
                      uint32 HEAP_BUFFER_SIZE, void *ResData)

{
    int32 init_ret = 0;
    int32 nResult = 0;
    LOCAL uint32 times= 0;

    //SCI_TRACE_LOW:"TTS_SynthText enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1455_112_2_18_3_3_51_289,(uint8*)"");

    yt_tts_set_memory_buffer(KiTTS_HeapBuffer,KiTTS_HEAP_BUFFER_SIZE);

    yt_set_callback_func_for_debug((long)UILogPrintDebug);

    //SCI_TRACE_LOW:"TTS_SynthText set mem"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1461_112_2_18_3_3_51_290,(uint8*)"");

    init_ret = yt_tts_initialize(YT_LANG_ID_HINDI,(char*)ResData, 1, (char*)((char*)ResData+KiTTS_HINDI_FRONT_DATA_SIZE));

    //SCI_TRACE_LOW:"TTS_SynthText yt init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1465_112_2_18_3_3_51_291,(uint8*)"");

    if(init_ret != 0)
    {
		MMK_PostMsg(VIRTUAL_WIN_ID, MSG_TTS_ERROR_RESOURCE, PNULL ,0);
//        if(s_is_prompt_when_err)
//        {
//            MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
//        }
        MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL,0 );
        return;
    }
    yt_tts_input_text_utf16((uint16*)txtPar->str, 
                                      txtPar->len/2,
                                         0, 
                                         0);
    //SCI_TRACE_LOW:"hidi length %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1480_112_2_18_3_3_51_292,(uint8*)"d", txtPar->len/2);

    times = 0;
    while(1)
    {
        times++;
        //SCI_TRACE_LOW:"hidi times %d,time:%u"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1486_112_2_18_3_3_51_293,(uint8*)"du", times,SCI_GetTickCount());
        nResult = yt_tts_play_text((long)callback_function_for_yt_tts,'N');
        if((s_tts_is_running == FALSE 
             //|| !TTS_IsCreatePcmStreamHandle()
             || MMITTS_user_stop == TRUE 
             || !TTSAPI_GetAudioIsOpen()) 
             || (0 == nResult))
        {
            //SCI_TRACE_LOW:"hidi stop after time:%u"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1494_112_2_18_3_3_51_294,(uint8*)"u", SCI_GetTickCount());
            //SCI_TRACE_LOW:"1:%d,2:%d,3:%d,4:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1500_112_2_18_3_3_51_295,(uint8*)"dddd", s_tts_is_running,MMITTS_user_stop,TTSAPI_GetAudioIsOpen(),nResult );
            break;
        }
    }

    if (s_kitts_callback_count < TTS_CALLBACK_CHECKPOINT
        && MMITTS_user_stop == FALSE && s_tts_is_running
        && TTSAPI_GetAudioIsOpen())
    {
		
		MMK_PostMsg(VIRTUAL_WIN_ID, MSG_TTS_AUDIO_PLAY, PNULL, 0);
		MMI_TriggerMMITask();   //加快反应
		
    }
    yt_tts_free_resource();
}
#else //YT_TTS_TWO_RES_FILE
LOCAL void TTS_SynthText(TxtParam * txtPar, char *HeapBuffer, uint32 HEAP_BUFFER_SIZE)

{
    int32 init_ret = 0;
    int32 nResult = 0;
    LOCAL uint32 times= 0;
    int jump_value = 0;
     
    //SCI_TRACE_LOW:"TTS_SynthText enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1523_112_2_18_3_3_52_296,(uint8*)"");

    yt_tts_set_memory_buffer(KiTTS_HeapBuffer,KiTTS_HEAP_BUFFER_SIZE);
    //SCI_TRACE_LOW:"TTS_SynthText set mem"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1526_112_2_18_3_3_52_297,(uint8*)"");

    yt_set_callback_func_for_debug((long)UILogPrintDebug);
    yt_set_callback_func_for_fopen((long)yt_file_wfopen);
    yt_set_callback_func_for_fread((long)yt_file_wfread);
    yt_set_callback_func_for_fseek((long)yt_file_wfseek);
    yt_set_callback_func_for_fclose((long)yt_file_wfclose);

    //SCI_TRACE_LOW:"string ptr before ini:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1534_112_2_18_3_3_52_298,(uint8*)"d",txtPar->str);

	yt_tts_free_resource();
    init_ret = yt_tts_initialize("reserved",s_mmitts_language_id,s_char_mmitts_full_file_path_1, 1, s_char_mmitts_full_file_path_2);
    
    //SCI_TRACE_LOW:"string ptr after ini:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1538_112_2_18_3_3_52_299,(uint8*)"d",txtPar->str);

    //SCI_TRACE_LOW:"TTS_SynthText yt init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1540_112_2_18_3_3_52_300,(uint8*)"");

	//init_ret = 0 yt_tts_initialize ok
    if(init_ret != 0)
    {
		MMK_PostMsg(VIRTUAL_WIN_ID, MSG_TTS_ERROR_RESOURCE, PNULL ,0);
//        if(s_is_prompt_when_err)
//        {
//            MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
//        }
        MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL,0 );
        //MMI_TriggerMMITask();//加快反应
        return;
    }
   
    //SCI_TRACE_LOW:"hidi length %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1554_112_2_18_3_3_52_301,(uint8*)"d", txtPar->len/2);

    //SCI_TRACE_LOW:"before yt_tts_input_text_utf16"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1556_112_2_18_3_3_52_302,(uint8*)"");

    yt_tts_input_text_utf16((uint16*)txtPar->str, 
                            txtPar->len/2,
                            0, 
                            0);

    //SCI_TRACE_LOW:"after yt_tts_input_text_utf16"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1563_112_2_18_3_3_52_303,(uint8*)"");

    times = 0;

    for(; ;)
    {
        times++;
        //SCI_TRACE_LOW:"hidi times %d,time:%u"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1570_112_2_18_3_3_52_304,(uint8*)"du", times,SCI_GetTickCount());
        nResult = yt_tts_play_text((long)callback_function_for_yt_tts,'N');
        if((s_tts_is_running == FALSE 
            //|| !TTS_IsCreatePcmStreamHandle()
            || MMITTS_user_stop == TRUE 
            || !TTSAPI_GetAudioIsOpen()) 
            || (0 == nResult))
        {
            //SCI_TRACE_LOW:"hidi stop after time:%u"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1578_112_2_18_3_3_52_305,(uint8*)"u", SCI_GetTickCount());
            //SCI_TRACE_LOW:"1:%d,2:%d,3:%d,4:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1584_112_2_18_3_3_52_306,(uint8*)"dddd", s_tts_is_running,MMITTS_user_stop,TTSAPI_GetAudioIsOpen(),nResult );
            break;
        }
    }

#ifdef SAVE_PCM_STREAM
	//SCI_TRACE_LOW:"close file handle:%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1590_112_2_18_3_3_52_307,(uint8*)"d",g_file_handle);

    if(g_file_handle != SFS_INVALID_HANDLE)
    {
        MMIAPIFMM_CloseFile(g_file_handle);
		//SCI_TRACE_LOW:"SAVE_PCM_STREAM close file"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1595_112_2_18_3_3_52_308,(uint8*)"");
        g_file_handle = SFS_INVALID_HANDLE;
    }
#endif

    jump_value = setjmp(s_jumpbuf_tts);
    if(jump_value == 0)
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SynthText ivTTS_SynthStart"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1603_112_2_18_3_3_52_309,(uint8*)"");
        if (s_kitts_callback_count < TTS_CALLBACK_CHECKPOINT
        && MMITTS_user_stop == FALSE && s_tts_is_running
        && TTSAPI_GetAudioIsOpen())
        {		
			s_tts_pcm_stream_count = TTS_GetPcmStreamId();
			MMK_PostMsg( VIRTUAL_WIN_ID, MSG_TTS_AUDIO_PLAY, &s_tts_pcm_stream_count, sizeof (s_tts_pcm_stream_count));
			MMI_TriggerMMITask();   //加快反应
			
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMITTS_SynthText jump_value=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1615_112_2_18_3_3_52_310,(uint8*)"d",jump_value);
    }
    
    yt_tts_free_resource();
}
#endif

/*****************************************************************************/
//  Description : handle the timer msg 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TTS_HandleTTSPlayOverTimerMsg(
                                          uint8 timer_id,
                                          uint32   param
                                          )
{    
    //SCI_TRACE_LOW:"MMITTS, HandleTTSPlayOverTimerMsg entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1631_112_2_18_3_3_52_311,(uint8*)"");
    if (s_tts_playover_timer_id == *(uint8*)param)
    {
        //SCI_TRACE_LOW:"TTS_HandleTTSPlayOverTimerMsg:handle s_tts_playover_timer_id!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1634_112_2_18_3_3_52_312,(uint8*)"");
        
        s_tts_playover_timer_id = 0;
        
        if (s_tts_is_running)
        {
            s_tts_is_running = FALSE;
            TTSAPI_NotifyTTSOver();
            if (TTSAPI_GetAudioIsOpen())
            {
                TTS_StopAndClosePcmStreamHandle();
            }
        }
        
        TTS_MemoryRelease();
        TTS_RestoreClk();
    }
	else
	{
#ifndef WIN32
		yte_tts_timer_handle(param);
#endif
	}
    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_NotifyTTSOver(void)
{
    if(0 != s_tts_text.win_handle)
    {
        MMK_PostMsg(s_tts_text.win_handle, MSG_TTS_OVER, PNULL,0 );
    }
}


PUBLIC BOOLEAN TTSAPI_PlayText(MMITTS_PLAY_PARAM_T * play_param_ptr)
{
    MMITTS_RESULT_E tts_result = MMITTS_PLAY_SUCCESS;
    
    if (PNULL == play_param_ptr)
    {
		return FALSE;
    }
    //SCI_TRACE_LOW:"TTSAPI_PlayText() txt_len_in_byte= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1671_112_2_18_3_3_52_313,(uint8*)"d", play_param_ptr->txt_len_in_byte);
    //SCI_TRACE_LOW:"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1672_112_2_18_3_3_52_314,(uint8*)"dd",s_tts_text.tts_start_pos,s_tts_text.tts_str_len);
    //启动定时器来关闭资源
    if( s_tts_playover_timer_id != 0 )
    {
        MMK_StopTimer(s_tts_playover_timer_id);
        s_tts_playover_timer_id = 0;
    }
    
    if(!TTSPlayPreCheck(play_param_ptr->ring_type))
    {
        return FALSE;
    }
    if(play_param_ptr->txt_addr==PNULL || play_param_ptr->txt_len_in_byte==0)   //如果数据无效,直接返回
    {
        tts_result = MMITTS_INVALID_PARAM;
        return FALSE;
    }
    else if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        tts_result = MMITTS_NO_MEMORY;
        if (s_is_prompt_when_err)
        {
        MMIPUB_OpenAlertWarningWin( TXT_CALL_USING);
        }
        return FALSE;
            
    }
   // 这一段去掉......
    /*else if (
#ifdef TTS_FILE_RES
        NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSRESCATHEBUFFER) || //block memory被占用
#endif
        NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSHEAPBUFFER) ||    
        NOT_USE != BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_TTSVOICEDATA) )
    {
        tts_result = MMITTS_NO_MEMORY;
        MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_MEMORY);
        return FALSE;
    }*/
#ifdef TTS_FILE_RES
    else if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        if (s_is_prompt_when_err)    
        {
            tts_result = MMITTS_NO_FILE_RES;
            MMIPUB_OpenAlertWarningWin(TXT_TTS_UDISK_USING);
        }
        return FALSE;
    }
    else if(!TTSAPI_checkResFile())
    {
        tts_result = MMITTS_NO_FILE_RES;
        if (s_is_prompt_when_err)
        {
        MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);
        }
        return FALSE;
    }
#endif
    /*if use 600 res file, only chinese can read*/
#ifdef TTS_DATA_RES_LIMIT
    {
        int i=0;
        if (play_param_ptr->is_ucs2)
        {
            for (i=0;i<play_param_ptr->txt_len_in_byte;i+=2)
            {
                if (play_param_ptr->txt_addr[i]!=0 ||(play_param_ptr->txt_addr[i+1]<=0x39 && play_param_ptr->txt_addr[i+1]>=0x30 ))
                    break;
            }
            if (i>=play_param_ptr->txt_len_in_byte)
                return FALSE;
        }
        else
        {
            for (i=0;i<play_param_ptr->txt_len_in_byte;i++)
            {
                if (play_param_ptr->txt_addr[i]<=0x39 && play_param_ptr->txt_addr[i]>=0x30)
                    break;
            }
            if (i>=play_param_ptr->txt_len_in_byte)
                return FALSE;
        }
    }
#endif
    
    play_param_ptr->txt_len_in_byte = MIN( play_param_ptr->txt_len_in_byte,MMITTS_STRING_SIZE );
    SCI_MEMSET(&s_tts_text, 0, sizeof(MMITTS_STRING_T));

    s_tts_text.str_len = play_param_ptr->txt_len_in_byte;
    s_tts_text.is_ucs2 = play_param_ptr->is_ucs2;    
    MMI_MEMCPY( s_tts_text.str_arr, MMITTS_STRING_SIZE, 
                play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, 
                play_param_ptr->txt_len_in_byte);
	//SCI_TRACE_LOW:"s_tts_text.str_arr:%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1766_112_2_18_3_3_52_315,(uint8*)"d",s_tts_text.str_arr);

	#if 0
    if (0==play_param_ptr->play_times)
    {
        while (MMITTS_STRING_SIZE-s_tts_text.str_len>play_param_ptr->txt_len_in_byte)
            {
                MMI_MEMCPY( s_tts_text.str_arr+s_tts_text.str_len, MMITTS_STRING_SIZE, 
                            play_param_ptr->txt_addr, play_param_ptr->txt_len_in_byte, 
                            play_param_ptr->txt_len_in_byte);
                s_tts_text.str_len += play_param_ptr->txt_len_in_byte;
                s_tts_text.is_ucs2 = play_param_ptr->is_ucs2;    
            }
    }
	#endif

    play_param_ptr->play_times = play_param_ptr->play_times>20 ? 20:play_param_ptr->play_times;
    
    s_tts_text.play_times       = play_param_ptr->play_times;//是否反复播放
    s_tts_text.ring_type        = play_param_ptr->ring_type;
    s_tts_text.tts_vol          = play_param_ptr->tts_vol;

    s_tts_text.tts_start_pos    = 0;  /*no use*/
    s_tts_text.tts_str_len      =s_tts_text.str_len;  //txt_len_in_byte;
    s_tts_text.win_handle = play_param_ptr->win_handle;

	s_tts_text.tts_route = play_param_ptr->tts_route;
	s_tts_text.digit_mode = play_param_ptr->digit_mode;
	s_tts_text.punc_mode = play_param_ptr->punc_mode;
	s_tts_text.volume_type = play_param_ptr->volume_type;
	

    //SCI_TRACE_LOW:"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1790_112_2_18_3_3_52_316,(uint8*)"dd",s_tts_text.tts_start_pos,s_tts_text.tts_str_len);
#ifdef TTS_FILE_RES
    {
//      char tempStr[MAX_SAVE_TCARD_FILE_LEN/2]={0};
//       sprintf (tempStr,"%s%s",SaveTcardDirStr,TTS_RES_FILE_NAME);    
//      memset(tts_res_file_name,0,sizeof(tts_res_file_name));
//      MMIAPICOM_Char2wchar((wchar*)tts_res_file_name,(char const *)tempStr);
    }
#endif
    return TTSAPI_SynthTextByTask();
}

PUBLIC BOOLEAN TTS_IsRepeatePlayText(void)
{
    if (s_tts_text.play_times==0)
        return TRUE;
    else
        return FALSE;
}

/*****************************************************************************/
//  Description : stop tts by type
//  Global resource dependence :
//  Author: aoke.hu
//  Note: 返回值 true:tts未播放或调用了停止播放;  false:tts仍旧在播放
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_StopPlayTextByType(TTS_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN result = TRUE;
    
    //SCI_TRACE_LOW:"TTSAPI_StopPlayTextByType ring_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1818_112_2_18_3_3_52_317,(uint8*)"d",ring_type);
    if(s_tts_is_running         
        ||TTSAPI_GetAudioIsOpen())
    {
        if(s_tts_text.ring_type == ring_type)
        {
            TTSAPI_StopPlayText();
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    
    return result;
}

PUBLIC void TTSAPI_StopPlayText()
{
    //SCI_TRACE_LOW:"TTSAPI_StopPlayText entry time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1838_112_2_18_3_3_52_318,(uint8*)"u",SCI_GetTickCount ());

    MMK_SendMsg( VIRTUAL_WIN_ID, MSG_TTS_STOP, PNULL );

    s_tts_text.str_len = 0;
    s_tts_text.tts_str_len = 0;
    
    //SCI_TRACE_LOW:"TTSAPI_StopPlayText exit time: %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1845_112_2_18_3_3_52_319,(uint8*)"u",SCI_GetTickCount ());
}

PUBLIC BOOLEAN TTSAPI_GetPlayStatus(void)
{
    return s_tts_is_running||TTSAPI_GetAudioIsOpen();
}

/*****************************************************************************/
//  Description : 判断是否在播放
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_IsPlayRing(TTS_ALL_RING_TYPE_E ring_type)
{
    if(s_tts_is_running         &&
       TTSAPI_GetAudioIsOpen()  &&
       s_tts_text.ring_type == ring_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TTSPlayPreCheck(TTS_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN result = TRUE;

    //SCI_TRACE_LOW:"TTSAPI_StopPlayTextByType ring_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1879_112_2_18_3_3_52_320,(uint8*)"d",ring_type);
    if(s_tts_is_running         
        ||TTSAPI_GetAudioIsOpen())
    {
        result = FALSE;
        switch(ring_type) 
        {
        case TTS_RING_TYPE_CALL:
            TTSAPI_StopPlayText();
            result = TRUE;
        	break;
        
        case TTS_RING_TYPE_MENU:
            if(s_tts_text.ring_type == ring_type)
            {
                TTSAPI_StopPlayText();
                result = TRUE;
            }
        	break;
        default:
            if(TTS_RING_TYPE_CALL < s_tts_text.ring_type)
            {
                TTSAPI_StopPlayText();
                result = TRUE;
            }
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置TTS有关窗口的softekey
//  Global resource dependence :
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void TTSAPI_SetSoftkeyTextId( 
                                    MMI_WIN_ID_T    win_id,        // 窗口id
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update // whether update)
                                    )
{
     // 如果TTS正在播放....
    if(TTSAPI_GetPlayStatus())
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, STXT_STOP, rightsoft_id, is_need_update);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, TXT_SET_TTS_PLAY, rightsoft_id, is_need_update);
    }
}

#ifdef TTS_FILE_RES
/*****************************************************************************/
//  Description : create tts dir 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: for 把tts的文件夹放到系统文件夹隐藏起来....
/*****************************************************************************/
LOCAL BOOLEAN CreateTTSHidenDir(void)
{
    BOOLEAN return_val = FALSE;
    //  创建目录...
    if(MMIAPIFMM_CreateSysFileDir(MMIAPIFMM_GetDefaultDisk(), s_mmitts_reserved_dir, TTS_HIDEN_DIR_LEN, PNULL))
    {
        return_val = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD create dcd main dir fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1948_112_2_18_3_3_52_321,(uint8*)"");
    }
    
    return return_val;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
#ifndef YT_TTS_TWO_RES_FILE
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                        MMIFILE_DEVICE_E    dev_type,      //[IN]
                                        wchar*  full_path_name_ptr,    //[OUT]
                                        uint16* full_path_len_ptr      //[OUT])
                                        )
{
    BOOLEAN  return_val    = FALSE;

    if((PNULL != full_path_name_ptr) && (PNULL != full_path_len_ptr))
    {
        return_val = MMIAPIFMM_CombineSysPath(
                                            dev_type,
                                            s_mmitts_reserved_dir,
                                            TTS_HIDEN_DIR_LEN,
                                            s_mmitts_res_file_name,
                                            TTS_RES_FILE_NAME_LEN,
                                            full_path_name_ptr,
                                            full_path_len_ptr
                                            );

    }
    else
    {
        //SCI_TRACE_LOW:"[MMITTS]:GetTTSResouceFileFullName error!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1982_112_2_18_3_3_52_322,(uint8*)"");
    }

    return return_val;
}
#else
LOCAL BOOLEAN GetTTSResouceFileFullName(
                                        MMIFILE_DEVICE_E    dev_type,      //[IN]
                                        uint16 file_index,             //[IN:1,2]
                                        wchar*  full_path_name_ptr,    //[OUT]
                                        uint16* full_path_len_ptr      //[OUT])
                                        )
{
    BOOLEAN  return_val    = FALSE;
    uint16 file_name_len_1 = (uint16)MMIAPICOM_Wstrlen((const wchar*)s_mmitts_res_file_name_1);
    uint16 file_name_len_2 = (uint16)MMIAPICOM_Wstrlen((const wchar*)s_mmitts_res_file_name_2);
   
    //SCI_TRACE_LOW:"TTS GetTTSResouceFileFullName entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_1999_112_2_18_3_3_52_323,(uint8*)"");

    if((PNULL != full_path_name_ptr) 
		&& (PNULL != full_path_len_ptr) 
		&&((1 == file_index)||(2 == file_index)))
    {
        if(1 == file_index)
        {
            return_val = MMIAPIFMM_CombineSysPath(
                                            dev_type,
                                            s_mmitts_reserved_dir,
                                            TTS_HIDEN_DIR_LEN,
                                            s_mmitts_res_file_name_1,
                                            file_name_len_1,
                                            full_path_name_ptr,
                                            full_path_len_ptr
                                            );
        }
        if(2 == file_index)
        {
            return_val = MMIAPIFMM_CombineSysPath(
                                            dev_type,
                                            s_mmitts_reserved_dir,
                                            TTS_HIDEN_DIR_LEN,
                                            s_mmitts_res_file_name_2,
                                            file_name_len_2,
                                            full_path_name_ptr,
                                            full_path_len_ptr
                                            );            
        }
    }
    else
    {
        //SCI_TRACE_LOW:"TTS GetTTSResouceFileFullName error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2032_112_2_18_3_3_53_324,(uint8*)"");
    }
    
    //SCI_TRACE_LOW:"TTS GetTTSResouceFileFullName + full_path_name_ptr:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2035_112_2_18_3_3_53_325,(uint8*)"s",full_path_name_ptr);
    
    return return_val;
}
#endif //YT_TTS_TWO_RES_FILE
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
#ifndef YT_TTS_TWO_RES_FILE
LOCAL BOOLEAN CreateTTSResouceFileHandle(
                                         MMIFILE_HANDLE* file_handle_ptr // [out]
                                        )
{
    wchar    res_file_full_path_name[MMIFILE_FILE_NAME_MAX_LEN + 1]  = {0};
    MMIFILE_HANDLE file_handle  = SFS_INVALID_HANDLE;
    uint16   path_len      = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN  return_val    = FALSE;
    BOOLEAN  find_resut    = FALSE;
    //MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E   i = MMI_DEVICE_UDISK;
    wchar*   dev_path_ptr = PNULL;
    uint16   dev_len = 0;
    
    if(PNULL == file_handle_ptr)
    {
        return FALSE;
    }
    
    // 先在C: 盘下面找...
    file_handle = MMIAPIFMM_CreateFile(s_mmitts_default_file_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
    if(SFS_INVALID_HANDLE != file_handle)
    {
        find_resut = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"TTS c: disk not exist resouce file"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2072_112_2_18_3_3_53_326,(uint8*)"");
    }

    if(!find_resut)
    {
        for (i = MMI_DEVICE_NUM -1 ;i >= MMI_DEVICE_UDISK;i--)
        {
            dev_len = MMIAPIFMM_GetDevicePathLen(i);
            dev_path_ptr =  MMIAPIFMM_GetDevicePath(i);

            if(!MMIAPIFMM_GetDeviceStatus(dev_path_ptr, dev_len))
            {
                continue;
            }
            else
            {
                return_val = GetTTSResouceFileFullName(i, res_file_full_path_name, &path_len); 
                if(return_val)
                {
                    file_handle = MMIAPIFMM_CreateFile(res_file_full_path_name, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
                    if(SFS_INVALID_HANDLE != file_handle)
                    {
                        find_resut = TRUE;
                        break;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"TTS %d disk not exist resouce file"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2099_112_2_18_3_3_53_327,(uint8*)"d",i);
                    }
                }
            }
        }
    }

    *file_handle_ptr = file_handle;

    return find_resut;
}
#endif
/*****************************************************************************/
//  Description : check  tts file 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
#ifndef YT_TTS_TWO_RES_FILE
PUBLIC BOOLEAN TTSAPI_checkResFile(void)
{
    uint8    header_info[TTS_RES_FILE_CHECK_BYTE_NUM + 1]   =  {0};
    MMIFILE_HANDLE file_handle  = SFS_INVALID_HANDLE;
    uint32   readed_size   = 0;
    
    BOOLEAN  return_val    = FALSE;
    
    if(CreateTTSResouceFileHandle(&file_handle))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return_val = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"TTS TTSAPI_checkResFile not find the resouce file  "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2131_112_2_18_3_3_53_328,(uint8*)"");
    }
    
    return return_val;
}

#else //YT_TTS_TWO_RES_FILE

//added by tonny 2011-05-31
LOCAL void TTS_copyFileInfo(void)
{
    MMISET_LANGUAGE_TYPE_E  tts_language_type = 0;
    
    memset(s_mmitts_res_file_name_1, 0, TTS_YT_RES_FILE_NAME_LEN);
    memset(s_mmitts_default_file_path_1, 0, TTS_YT_RES_FILE_NAME_LEN);  
    memset(s_char_mmitts_default_file_path_1, 0, TTS_YT_RES_FILE_NAME_LEN); 

    memset(s_mmitts_res_file_name_2, 0, TTS_YT_RES_FILE_NAME_LEN);
    memset(s_mmitts_default_file_path_2, 0, TTS_YT_RES_FILE_NAME_LEN);  
    memset(s_char_mmitts_default_file_path_2, 0, TTS_YT_RES_FILE_NAME_LEN); 
  
    MMIAPISET_GetLanguageType(&tts_language_type);
    //SCI_TRACE_LOW:"TTS_copyFileInfo tts_language_type:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2153_112_2_18_3_3_53_329,(uint8*)"d",tts_language_type);

    switch(tts_language_type)
    {
		//中文简体 
		case MMISET_LANGUAGE_SIMP_CHINESE:
    	case MMISET_LANGUAGE_TRAD_CHINESE://中文繁体
		{
				s_mmitts_language_id = YT_LANG_ID_MANDARIN;
            SCI_MEMCPY(&s_mmitts_res_file_name_1, (const void *)&s_mmitts_res_file_name_man_1, 
                       sizeof(s_mmitts_res_file_name_man_1));

            SCI_MEMCPY(&s_mmitts_default_file_path_1, (const void *)&s_mmitts_default_file_path_man_1, 
                       sizeof(s_mmitts_default_file_path_man_1));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_1, (const void *)&s_char_mmitts_default_file_path_man_1, 
                       sizeof(s_char_mmitts_default_file_path_man_1));

            SCI_MEMCPY(&s_mmitts_res_file_name_2, (const void *)&s_mmitts_res_file_name_man_2, 
                       sizeof(s_mmitts_res_file_name_man_2));

            SCI_MEMCPY(&s_mmitts_default_file_path_2, (const void *)&s_mmitts_default_file_path_man_2, 
                       sizeof(s_mmitts_default_file_path_man_2));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_2, (const void *)&s_char_mmitts_default_file_path_man_2, 
                       sizeof(s_char_mmitts_default_file_path_man_2));  
         }
		 break;
        case MMISET_LANGUAGE_ENGLISH:
        {
        #ifdef IM_ENGINE
            s_mmitts_language_id = YT_LANG_ID_US_ENGLISH;
            SCI_MEMCPY(&s_mmitts_res_file_name_1, (const void *)&s_mmitts_res_file_name_eng_1, 
                       sizeof(s_mmitts_res_file_name_eng_1));

            SCI_MEMCPY(&s_mmitts_default_file_path_1, (const void *)&s_mmitts_default_file_path_eng_1, 
                       sizeof(s_mmitts_default_file_path_eng_1));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_1, (const void *)&s_char_mmitts_default_file_path_eng_1, 
                       sizeof(s_char_mmitts_default_file_path_eng_1));

            SCI_MEMCPY(&s_mmitts_res_file_name_2, (const void *)&s_mmitts_res_file_name_eng_2, 
                       sizeof(s_mmitts_res_file_name_eng_2));

            SCI_MEMCPY(&s_mmitts_default_file_path_2, (const void *)&s_mmitts_default_file_path_eng_2, 
                       sizeof(s_mmitts_default_file_path_eng_2));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_2, (const void *)&s_char_mmitts_default_file_path_eng_2, 
                       sizeof(s_char_mmitts_default_file_path_eng_2));  
        #endif
        }
            break;

        //印尼语    
        case MMISET_LANGUAGE_INDONESIAN:
        {
        #ifdef IM_INDONESIAN_SUPPORT
            s_mmitts_language_id = YT_LANG_ID_INDONESIAN;
            
           SCI_MEMCPY(&s_mmitts_res_file_name_1, (const void *)&s_mmitts_res_file_name_bahasa_1, 
                       sizeof(s_mmitts_res_file_name_bahasa_1));

            SCI_MEMCPY(&s_mmitts_default_file_path_1, (const void *)&s_mmitts_default_file_path_bahasa_1, 
                       sizeof(s_mmitts_default_file_path_bahasa_1));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_1, (const void *)&s_char_mmitts_default_file_path_bahasa_1, 
                       sizeof(s_char_mmitts_default_file_path_bahasa_1));

            SCI_MEMCPY(&s_mmitts_res_file_name_2, (const void *)&s_mmitts_res_file_name_bahasa_2, 
                       sizeof(s_mmitts_res_file_name_bahasa_2));

            SCI_MEMCPY(&s_mmitts_default_file_path_2, (const void *)&s_mmitts_default_file_path_bahasa_2, 
                       sizeof(s_mmitts_default_file_path_bahasa_2));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_2, (const void *)&s_char_mmitts_default_file_path_bahasa_2, 
                       sizeof(s_char_mmitts_default_file_path_bahasa_2)); 
        #endif    
        }
            break;
        //印地语    
        case MMISET_LANGUAGE_HINDI:
        {
        #ifdef IM_HINDI_SUPPORT
            s_mmitts_language_id = YT_LANG_ID_HINDI;
            SCI_MEMCPY(&s_mmitts_res_file_name_1, (const void *)&s_mmitts_res_file_name_hin_1, 
                       sizeof(s_mmitts_res_file_name_hin_1));

            SCI_MEMCPY(&s_mmitts_default_file_path_1, (const void *)&s_mmitts_default_file_path_hin_1, 
                       sizeof(s_mmitts_default_file_path_hin_1));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_1, (const void *)&s_char_mmitts_default_file_path_hin_1, 
                       sizeof(s_char_mmitts_default_file_path_hin_1));

            SCI_MEMCPY(&s_mmitts_res_file_name_2, (const void *)&s_mmitts_res_file_name_hin_2, 
                       sizeof(s_mmitts_res_file_name_hin_2));

            SCI_MEMCPY(&s_mmitts_default_file_path_2, (const void *)&s_mmitts_default_file_path_hin_2, 
                       sizeof(s_mmitts_default_file_path_hin_2));
 
            SCI_MEMCPY(&s_char_mmitts_default_file_path_2, (const void *)&s_char_mmitts_default_file_path_hin_2, 
                       sizeof(s_char_mmitts_default_file_path_hin_2));  
        #endif      
        }
            break;
        default:
            break;   
            
    }

    //SCI_TRACE_LOW:"TTS_copyFileInfo s_mmitts_res_file_name_1:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2239_112_2_18_3_3_53_330,(uint8*)"s",s_mmitts_res_file_name_1);
    //SCI_TRACE_LOW:"TTS_copyFileInfo s_mmitts_default_file_path_1:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2240_112_2_18_3_3_53_331,(uint8*)"s",s_mmitts_default_file_path_1);
    //SCI_TRACE_LOW:"TTS_copyFileInfo s_char_mmitts_default_file_path_1:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2241_112_2_18_3_3_53_332,(uint8*)"s",s_char_mmitts_default_file_path_1);

    //SCI_TRACE_LOW:"TTS_copyFileInfo s_mmitts_res_file_name_2:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2243_112_2_18_3_3_53_333,(uint8*)"s",s_mmitts_res_file_name_2);
    //SCI_TRACE_LOW:"TTS_copyFileInfo s_mmitts_default_file_path_2:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2244_112_2_18_3_3_53_334,(uint8*)"s",s_mmitts_default_file_path_2);
    //SCI_TRACE_LOW:"TTS_copyFileInfo s_char_mmitts_default_file_path_2:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2245_112_2_18_3_3_53_335,(uint8*)"s",s_char_mmitts_default_file_path_2);
    
}
//end added 

LOCAL void TTS_SetDevUdiskFileFullPath(void)
{
    uint16   dev_udisk_len      = 0;
    
    memset(s_mmitts_full_file_path_1, 0, TTS_YT_RES_FILE_NAME_LEN);  
    memset(s_char_mmitts_full_file_path_1, 0, TTS_YT_RES_FILE_NAME_LEN); 
    memset(s_mmitts_full_file_path_2, 0, TTS_YT_RES_FILE_NAME_LEN);  
    memset(s_char_mmitts_full_file_path_2, 0, TTS_YT_RES_FILE_NAME_LEN); 
    
    dev_udisk_len = (uint16)MMIAPICOM_Wstrlen((const wchar*)s_mmitts_dev_udisk);

    //copy root path
    MMI_WSTRNCPY(s_mmitts_full_file_path_1, TTS_YT_RES_FILE_NAME_LEN, s_mmitts_dev_udisk,
                 TTS_YT_RES_FILE_NAME_LEN, dev_udisk_len);
    MMI_WSTRNCPY(s_mmitts_full_file_path_2, TTS_YT_RES_FILE_NAME_LEN, s_mmitts_dev_udisk,
                 TTS_YT_RES_FILE_NAME_LEN, dev_udisk_len);
    //strcat file name
    MMIAPICOM_Wstrcat(s_mmitts_full_file_path_1, (const wchar*)s_mmitts_default_file_path_1);
    MMIAPICOM_Wstrcat(s_mmitts_full_file_path_2, (const wchar*)s_mmitts_default_file_path_2);

    //copy root path
    SCI_MEMCPY(&s_char_mmitts_full_file_path_1, (const void *)&s_char_mmitts_dev_udisk, 
                       sizeof(s_char_mmitts_dev_udisk));
    SCI_MEMCPY(&s_char_mmitts_full_file_path_2, (const void *)&s_char_mmitts_dev_udisk, 
                       sizeof(s_char_mmitts_dev_udisk));

    //strcat file name
    strcat(s_char_mmitts_full_file_path_1, s_char_mmitts_default_file_path_1);
    strcat(s_char_mmitts_full_file_path_2, s_char_mmitts_default_file_path_2);
    
}

LOCAL void TTS_SetDevSDCardFileFullPath(void)
{
    uint16   dev_sdcard_len      = 0;
    
    memset(s_mmitts_full_file_path_1, 0, TTS_YT_RES_FILE_NAME_LEN);  
    memset(s_char_mmitts_full_file_path_1, 0, TTS_YT_RES_FILE_NAME_LEN); 
    memset(s_mmitts_full_file_path_2, 0, TTS_YT_RES_FILE_NAME_LEN);  
    memset(s_char_mmitts_full_file_path_2, 0, TTS_YT_RES_FILE_NAME_LEN); 
    
    dev_sdcard_len = (uint16)MMIAPICOM_Wstrlen((const wchar*)s_mmitts_dev_sdcard);

    //copy root path
    MMI_WSTRNCPY(s_mmitts_full_file_path_1, TTS_YT_RES_FILE_NAME_LEN, s_mmitts_dev_sdcard,
                 TTS_YT_RES_FILE_NAME_LEN, dev_sdcard_len);
    MMI_WSTRNCPY(s_mmitts_full_file_path_2, TTS_YT_RES_FILE_NAME_LEN, s_mmitts_dev_sdcard,
                 TTS_YT_RES_FILE_NAME_LEN, dev_sdcard_len);
    //strcat file name
    MMIAPICOM_Wstrcat(s_mmitts_full_file_path_1, (const wchar*)s_mmitts_default_file_path_1);
    MMIAPICOM_Wstrcat(s_mmitts_full_file_path_2, (const wchar*)s_mmitts_default_file_path_2);

    //copy root path
    SCI_MEMCPY(&s_char_mmitts_full_file_path_1, (const void *)&s_char_mmitts_dev_sdcard, 
                       sizeof(s_char_mmitts_dev_sdcard));
    SCI_MEMCPY(&s_char_mmitts_full_file_path_2, (const void *)&s_char_mmitts_dev_sdcard, 
                       sizeof(s_char_mmitts_dev_sdcard));

    //strcat file name
    strcat(s_char_mmitts_full_file_path_1, s_char_mmitts_default_file_path_1);
    strcat(s_char_mmitts_full_file_path_2, s_char_mmitts_default_file_path_2);
    
}

PUBLIC BOOLEAN TTSAPI_checkResFile(void)
{
    //wchar    res_file_full_path_name_1[MMIFILE_FILE_NAME_MAX_LEN + 1]  = {0};
    //wchar    res_file_full_path_name_2[MMIFILE_FILE_NAME_MAX_LEN + 1]  = {0};

    MMIFILE_HANDLE file_handle_1  = SFS_INVALID_HANDLE;
    MMIFILE_HANDLE file_handle_2  = SFS_INVALID_HANDLE;

    //uint16   path_len_1      = MMIFILE_FULL_PATH_MAX_LEN;
    //uint16   path_len_2      = MMIFILE_FULL_PATH_MAX_LEN;

    BOOLEAN  sd_disk_is_ok = FALSE;
	
    //BOOLEAN  return_val_1    = FALSE;
    //BOOLEAN  return_val_2    = FALSE;

    BOOLEAN  find_resut    = FALSE;
    
    sd_disk_is_ok = MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD);

    //added by tonny 2011-05-31
    TTS_copyFileInfo();
    //end added 

    //find the two resource file in Udisk
    TTS_SetDevUdiskFileFullPath();
    
    file_handle_1 = MMIAPIFMM_CreateFile(s_mmitts_full_file_path_1, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
    file_handle_2 = MMIAPIFMM_CreateFile(s_mmitts_full_file_path_2, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);

    if((SFS_INVALID_HANDLE != file_handle_1)&&(SFS_INVALID_HANDLE != file_handle_2))
    {
        find_resut = TRUE;
		MMIAPIFMM_CloseFile(file_handle_1);
		MMIAPIFMM_CloseFile(file_handle_2);

        //SCI_TRACE_LOW:"TTSAPI_checkResFile: TTS Udisk exist resouce file"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2350_112_2_18_3_3_53_336,(uint8*)"");
    }
    else
    {
        if(SFS_INVALID_HANDLE != file_handle_1)
        {
            MMIAPIFMM_CloseFile(file_handle_1);
        }
        if(SFS_INVALID_HANDLE != file_handle_2)
        {
            MMIAPIFMM_CloseFile(file_handle_2);
        }
	    
        //SCI_TRACE_LOW:"TTSAPI_checkResFile: TTS Udisk not exist resouce file"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2363_112_2_18_3_3_53_337,(uint8*)"");
    }

    if(!find_resut && sd_disk_is_ok)
    {
        //return_val_1 = GetTTSResouceFileFullName(MMI_DEVICE_SDCARD/*FS_MMC*/, 1, res_file_full_path_name_1, &path_len_1); 
        //return_val_2 = GetTTSResouceFileFullName(MMI_DEVICE_SDCARD/*FS_MMC*/, 2, res_file_full_path_name_2, &path_len_2); 
		
        //if(return_val_1 && return_val_2)//if(return_val_1 && return_val_1)
        {
            //find the two resource file in sddisk
            TTS_SetDevSDCardFileFullPath();
             
            file_handle_1 = MMIAPIFMM_CreateFile(s_mmitts_full_file_path_1, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
            file_handle_2 = MMIAPIFMM_CreateFile(s_mmitts_full_file_path_2, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);

            if((SFS_INVALID_HANDLE != file_handle_1)&&(SFS_INVALID_HANDLE != file_handle_2))
            {
                find_resut = TRUE;
                MMIAPIFMM_CloseFile(file_handle_1);
		        MMIAPIFMM_CloseFile(file_handle_2);

                //SCI_TRACE_LOW:"TTSAPI_checkResFile: TTS SDCard exist resouce file"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2385_112_2_18_3_3_53_338,(uint8*)"");
            }
            else
            {
				if(SFS_INVALID_HANDLE != file_handle_1)
				{
					MMIAPIFMM_CloseFile(file_handle_1);
				}
				if(SFS_INVALID_HANDLE != file_handle_2)
				{
					MMIAPIFMM_CloseFile(file_handle_2);
				}
				
				//SCI_TRACE_LOW:"TTS SDCard not exist resouce file"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2398_112_2_18_3_3_53_339,(uint8*)"");
            }            
        }
    }

    return find_resut;
}

#endif //YT_TTS_TWO_RES_FILE

/*****************************************************************************/
//  Description : gui 调用此接口朗读菜单 
//  Global resource dependence : none
//  Author: violent.wei
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_PlayTextForControl(
                                      MMI_STRING_T* str_ptr, 
                                      uint32 num,
									  MMISRVAUD_ROUTE_T tts_route
                                      )
{
#ifdef MMI_READMENU_ENABLE
    if(PNULL == str_ptr || 0 == num)
    {
        //SCI_TRACE_LOW:"[MMITTS]:MMIAPITTS_PlayTextForControl ERROR!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_YOUNGTONG_API_2420_112_2_18_3_3_53_340,(uint8*)"");
        return;
    }
    if(!TTSPlayPreCheck(TTS_RING_TYPE_MENU))
    {
        return ;
    }
    
    // 0 为开关打开状态
    if (0 == MMIAPIENVSET_GetActiveModeOptValue(0, ENVSET_MENU_READ_SWITCH) &&
        !MMIAPIENG_GetIQModeStatus())
    {
        uint32 i = 0, len = 0;
        wchar* total_str_ptr = NULL;
        uint8 vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_READMENU);
		MMITTS_PLAY_PARAM_T  play_param = {0};
        
        // 根据需求，开关此功能
#if 1
        // 考虑到隐私问题，不自动读出收件箱，发件箱，草稿箱，已发信箱在list中部分内容
        MMI_WIN_ID_T focus_win_id = MMK_GetFocusWinId();
        if (MMISMS_MSGBOX_MAIN_WIN_ID == focus_win_id)
        {
            if (3 == num) num = 2;
        }
#endif
        
        // 不知道所有字符串的总长，需要累加得出
        for (; i < num; i ++)
        {
            len += str_ptr[i].wstr_len + 1;
        }
        total_str_ptr = (wchar*)SCI_ALLOC((len + 1) * 2);
        SCI_MEMSET(total_str_ptr, 0, (len + 1) * 2);
        
        // 将所有字符串复制到一个buffer中，便于朗读
        for (i = 0, len = 0; i < num; i ++)
        {
            if (str_ptr[i].wstr_len > 0)
            {
                // 复制
                SCI_MEMCPY(total_str_ptr + len, str_ptr[i].wstr_ptr, str_ptr[i].wstr_len * 2);
                len += str_ptr[i].wstr_len;
                // 在字符串与字符串之间加空格符，这样可以产生停顿效果
                *(total_str_ptr + len) = 0x20;
                len ++;
            }
        }
        
        // 此时不应该弹出出错提示（比如缺少TTS资源文件等），因为菜单朗读太频繁，提示会给用户带来困扰
        s_is_prompt_when_err = FALSE;
		play_param.txt_addr = (uint8*)total_str_ptr;
		play_param.txt_len_in_byte = len * 2;
		play_param.is_ucs2 = TRUE;
		play_param.play_times = 1;
		play_param.tts_vol = vol;
		play_param.ring_type = TTS_RING_TYPE_MENU;
		play_param.win_handle = 0;
		play_param.digit_mode = TTS_DIGIT_NUMBER;
		play_param.punc_mode = TTS_PUNC_OFF;
		play_param.tts_route = tts_route;
		play_param.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;
        TTSAPI_PlayText(&play_param);
        // 恢复弹出出错提示标志量
        s_is_prompt_when_err = TRUE;
        
        SCI_FREE(total_str_ptr);
    }

#endif
}

#endif //TTS_FILE_RES
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void TTSAPI_Init(void)
{

#ifdef TTS_FILE_RES
    CreateTTSHidenDir();
#endif
#ifndef WIN32
	yte_tts_init();
#endif
}

/*****************************************************************************/
//  Description : 设置TTS运行状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSRunStatus(BOOLEAN tts_is_run)
{
    s_tts_is_running = tts_is_run;
}

/*****************************************************************************/
//  Description :  获取TTS运行状态
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_GetTTSRunStatus(void)
{
    return  s_tts_is_running;   
}

/*****************************************************************************/
//  Description :  获取TTS ring type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC TTS_ALL_RING_TYPE_E TTS_GetTTSRingType(void)
{
    return  s_tts_text.ring_type;   
}

/*****************************************************************************/
//  Description :  获取TTS volume type
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_VOLUME_TYPE_E TTS_GetTTSVolumeType(void)
{
	return s_tts_text.volume_type;
}
/*****************************************************************************/
//  Description :  获取TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 TTS_GetTTSVolume(void)
{
    return s_tts_text.tts_vol;   
}

/*****************************************************************************/
//  Description :  设置TTS volume
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void TTS_SetTTSVolume(uint16 set_vol)
{
    MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
    uint8           cur_vol      = TTS_GetTTSVolume();
    
    if((audio_handle > 0) && (cur_vol != set_vol))
    {
        MMISRVAUD_SetVolume(audio_handle, set_vol);
        s_tts_text.tts_vol = set_vol;
    }
}

/*****************************************************************************/
//  Description :  write nv
/*****************************************************************************/
PUBLIC VOID TTS_WRITE_NV_INFO(SET_NV_ITEM_E item, void *addr)
{
	MMINV_WRITE(item, addr);
}
/*****************************************************************************/
//  Description :  read nv
/*****************************************************************************/
PUBLIC VOID TTS_READ_NV_INFO(SET_NV_ITEM_E item, void *addr, MN_RETURN_RESULT_E *return_value)
{
	MMINV_READ(item, addr, *return_value);
}

/*****************************************************************************/
// 	Description : 获得当前主sim卡的id 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E yte_get_active_sim(void)
{
	return MMIAPISET_GetActiveSim();
}

/*****************************************************************************/
//  Description : convert ascii string to unicode string(UCS2)
/****************************************************************************/
PUBLIC void yte_convert_alphabet_to_unicode(unsigned char *in, unsigned char *out, signed short out_buf_size)
{
	MMIAPICOM_ConvertAlphabetToUnicode(in, out, out_buf_size);
}

/*****************************************************************************/
//  Description : create win timer
/****************************************************************************/
PUBLIC unsigned char yte_create_win_timer(MMI_HANDLE_T owner_handle, unsigned int time_out, BOOLEAN is_period)
{
	return MMK_CreateTimerCallback(  time_out, TTS_HandleTTSPlayOverTimerMsg, NULL, is_period);//MMK_CreateWinTimer(owner_handle, time_out, is_period);
}

/*****************************************************************************/
//  Description : start win timer
/****************************************************************************/
PUBLIC BOOLEAN yte_start_win_timer(MMI_HANDLE_T owner_handle, unsigned char timer_id, unsigned int time_out, BOOLEAN is_period)
{
	return MMK_StartTimerCallback(timer_id, time_out,TTS_HandleTTSPlayOverTimerMsg, NULL, is_period);
}

/*****************************************************************************/
//  Description : stop timer
/****************************************************************************/
PUBLIC BOOLEAN yte_stop_timer(unsigned char timer_id)
{
	return MMK_StopTimer(timer_id);
}

/*****************************************************************************/
//  Description : str lower
/****************************************************************************/
PUBLIC char* yte_strlwr(char *string)
{
	return MMIAPICOM_Strlower(string);
}

/*****************************************************************************/
//  Description : judge if the character is chinese or not
/****************************************************************************/
PUBLIC int yte_is_chinese_character(unsigned short p_wchar)
{
	if (p_wchar >= 0x4e00 && p_wchar <= 0x9fa5)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************
 * @desc
 *****************************************************************************/
int yte_str_ucs2_cmp(const unsigned short *str1, const unsigned short *str2)
{
    return MMIAPICOM_Wstrcmp(str1,str2);
}

/*****************************************************************************
 * @desc
 *****************************************************************************/
signed char * yte_str_ucs2_cpy(unsigned short *strDestination, const unsigned short *strSource)
{
    return MMIAPICOM_Wstrcpy(strDestination,strSource);
}

/*****************************************************************************
 * @desc
 *****************************************************************************/
signed char * yte_str_ucs2_cat(unsigned short *strDestination, const unsigned short *strSource)
{
    return MMIAPICOM_Wstrcat(strDestination,strSource);
}

/*****************************************************************************
 * @desc
 *****************************************************************************/
void yte_str_utf8_to_ucs2( unsigned short* wstr_ptr, unsigned short wstr_len, const char* utf8_ptr, unsigned short utf8_len )
{
	GUI_UTF8ToWstr(wstr_ptr,wstr_len,utf8_ptr,utf8_len);
}

#ifdef WIN32
/*****************************************************************************
 * @desc
 *****************************************************************************/
int yte_str_ucslen(const char *str)
{
    int count = 0;
    int length = 0;

    if (str)
    {
        while (str[count] != 0 || str[count + 1] != 0)
        {
            ++length;
            count += 2;
        }
    }
    return length; 
}
#endif

/*****************************************************************************
 * @desc ascii 2 default
 *****************************************************************************/
uint16 yte_ascii_2_default(uint8 *ascii_ptr, uint8 *default_ptr, uint16 str_len)
{
     return MMIAPICOM_Ascii2default(ascii_ptr, default_ptr, str_len);
}

/*****************************************************************************
 * @desc wstrn to str
 *****************************************************************************/
void *yte_wstrn_to_str(uint8* dst, size_t dst_max_count, const wchar* src, size_t src_max_count, size_t count)
{
   return MMI_WSTRNTOSTR(dst, dst_max_count, src, src_max_count, count);
}

/*****************************************************************************
 * @desc memset
 *****************************************************************************/
void yte_memset(void* _DEST_PTR, uint32 _VALUE, uint32 _SIZE)
{
    SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE);
}

/*****************************************************************************
 * @desc memcpy
 *****************************************************************************/
void yte_memcpy(void* _DEST_PTR, const  void* _SRC_PTR, uint32 _SIZE)
{
    SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE);
}

/*****************************************************************************
 * @desc split long msg
 *****************************************************************************/
uint8 yte_sms_split_long_msg(MMISMS_CONTENT_T *sms_content_ptr, BOOLEAN is_head_has_port)
{
    return MMISMS_SplitLongMsg(sms_content_ptr, is_head_has_port);
}

/*****************************************************************************
 * @desc get sms user data
 *****************************************************************************/
void yte_sms_get_user_data(
                                uint8                    sms_index,        //IN: the index of SMS (1 ~ MMISMS_SPLIT_MAX_NUM) 
                                MMISMS_CONTENT_T            *sms_content_ptr, //IN: the content of need send
                                MN_SMS_USER_DATA_HEAD_T  *tpdu_head_ptr,   //IN/OUT: the head of user data 
                                BOOLEAN                  is_need_convert,
                                APP_SMS_USER_DATA_T      *user_data_ptr,    //OUT: the user data
                                MMISMS_SEND_T *send_info_ptr
                                )
{
    MMISMS_GetUserData(sms_index, sms_content_ptr, tpdu_head_ptr, is_need_convert, user_data_ptr, send_info_ptr);
}

/*****************************************************************************
 * @desc get party number
 *****************************************************************************/
BOOLEAN yte_sms_get_party_number(
                                        uint8               *tele_num_ptr,
                                        int16               tele_len,
                                        MMI_PARTY_NUMBER_T *party_num
                                        )
{
    return MMIAPICOM_GenPartyNumber(tele_num_ptr, tele_len, party_num);
}

/*****************************************************************************
 * @desc set oper status
 *****************************************************************************/
BOOLEAN yte_sms_set_oper_status(
                                    MMISMS_OPER_STATUS_E    oper_status
                                    )
{
	return MMISMS_SetOperStatus(oper_status);
}

/*****************************************************************************
 * @desc decode user head and  content
 *****************************************************************************/
void yte_sms_decode_user_head_and_content(
                                     MN_DUAL_SYS_E           dual_sys,           //IN:
                                     BOOLEAN                 user_head_is_exist, //IN:
                                     MN_SMS_DCS_T            *dcs_ptr_t,         //IN:
                                     MN_SMS_USER_DATA_T      *user_data_ptr_t,     //IN:
                                     APP_SMS_USER_DATA_T     *decode_data_ptr     //OUT:
                                     )
{
     MMISMS_DecodeUserHeadAndContent(dual_sys, user_head_is_exist, dcs_ptr_t, user_data_ptr_t, decode_data_ptr);
}

/*****************************************************************************
 * @desc get net display number
 *****************************************************************************/
uint8 yte_sms_get_net_display_number(
                                     MN_NUMBER_TYPE_E  number_type,
                                     uint8             party_len,
                                     uint8             *party_num,
                                     uint8              *tele_num,
                                     uint8              max_tele_len
                                     )
{
    return MMIAPICOM_GenNetDispNumber(number_type, party_len, party_num, tele_num, max_tele_len);
}

/*****************************************************************************
 * @desc sms update status extension
 *****************************************************************************/
ERR_MNSMS_CODE_E yte_sms_update_status_ex(                                  //RETURN:
                                         MN_DUAL_SYS_E          dual_sys,   //IN:
                                         MN_SMS_STORAGE_E       storage,    //IN: the destination storage, ME or SIM
                                         MN_SMS_RECORD_ID_T     record_id,  //IN:
                                         MN_SMS_STATUS_E        status      //IN: the SMS status
                                         )
{
    return MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, status);
}

/******************************************************************************
	@desc: 获取时间
*******************************************************************************/
char yte_sys_get_time(char  *min, char* sec)
{
	SCI_TIME_T currTime;

	TM_GetSysTime(&currTime);
	if(min != NULL)
	{
		*min = currTime.min;
	}
	if(sec != NULL)
	{
		*sec = currTime.sec;
	}
	return currTime.hour;
}

/******************************************************************************
	@desc: 获取短信中心号
*******************************************************************************/
void yte_sys_get_sc_num ( int sim , char * buf)
{
	uint8 sc_num_len = 0;
	
	if ( buf == NULL )
		return;
	
	MMISMS_AppGetSCAddr ( sim, buf, &sc_num_len, MMISMS_STRNUM_MAX_LEN + 2);	
}

/******************************************************************************
	@desc: 获取当前时间
*******************************************************************************/
void yte_sys_get_current_time ( int *hour, int* min, int* sec )
{
	SCI_TIME_T curr_time;

	TM_GetSysTime( &curr_time );

	if ( hour != NULL )
		*hour = curr_time.hour;

	if ( min != NULL )
		*min=curr_time.min;
	
	if ( sec != NULL )
		*sec=curr_time.sec;
}

/******************************************************************************
	@desc: 获取imsi号
*******************************************************************************/
void yte_sys_get_imsi ( int sim, char * imsi_buf )
{
	MN_IMSI_T imsi = {0}; 	

	if ( imsi_buf == NULL )
		return;
	
	imsi = MNSIM_GetImsiEx(sim);

	if ( imsi.imsi_val == NULL )
		return;
	
	MMIAPICOM_BcdToStr ( PACKED_LSB_FIRST, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN << 1, imsi_buf );
}

/******************************************************************************
	@desc: malloc
*******************************************************************************/
void* yte_malloc(size_t size)
{
    return SCI_ALLOC_APP(size);
}

/******************************************************************************
	@desc: free
*******************************************************************************/
void yte_free(void *ptr)
{
    SCI_Free(ptr);
}

/******************************************************************************
	@desc: create socket
*******************************************************************************/
TCPIP_SOCKET_T yte_sock_socket(
    int family, int type, int proto, TCPIP_NETID_T netid)
{
	return sci_sock_socket(family, type, proto, netid);
}

/******************************************************************************
	@desc: create socket
*******************************************************************************/
int yte_sock_setsockopt(TCPIP_SOCKET_T so, int name, void* arg_ptr)
{
	return sci_sock_setsockopt(so, name, arg_ptr);
}

/******************************************************************************
	@desc: close socket
*******************************************************************************/
int yte_sock_socketclose(TCPIP_SOCKET_T so)
{
	return sci_sock_socketclose(so);
}

/******************************************************************************
	@desc:socket asyn select
*******************************************************************************/
int yte_sock_asyncselect(TCPIP_SOCKET_T so, BLOCK_ID task_id, uint32 event)
{
	return sci_sock_asyncselect(so, task_id, event);
}

/******************************************************************************
	@desc:socket connect
*******************************************************************************/
int yte_sock_connect(TCPIP_SOCKET_T so, struct sci_sockaddr* addr_ptr, int addr_len)
{
	return sci_sock_connect(so, addr_ptr, addr_len);
}

/******************************************************************************
	@desc:get sock error
*******************************************************************************/
int yte_sock_errno(TCPIP_SOCKET_T so)
{
	return sci_sock_errno(so);
}

/******************************************************************************
	@desc:get host by name
*******************************************************************************/
TCPIP_HOST_HANDLE yte_async_gethostbyname(
    char*           name_ptr,
    BLOCK_ID        task_id,
    uint32          time_out,
    TCPIP_NETID_T   netid)
{
	return sci_async_gethostbyname(name_ptr, task_id, time_out, netid);
}

/******************************************************************************
	@desc:get signal of task
*******************************************************************************/
void* yte_get_signal(uint32 task_id)
{
	return SCI_GetSignal(task_id);
}

/******************************************************************************
	@desc: identify current thread
*******************************************************************************/
uint32 yte_identify_thread(void)
{
	return SCI_IdentifyThread();
}

/******************************************************************************
	@desc: identify current thread
*******************************************************************************/
void yte_thread_exit(void)
{
	SCI_ThreadExit();
}

/******************************************************************************
	@desc: create thread
*******************************************************************************/
BLOCK_ID yte_create_thread(       // If successful, returns the thread ID
                                        // of the new created thread, 
                                        // else returns SCI_INVALID_BLOCK_ID
    const char  *thread_name,           // Name string of the thread
    const char  *queue_name,            // Queue name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    uint32  stack_size,                 // Size of the thread stack in bytes
    uint32  queue_num,          // Number of messages which can be enqueued
    uint32  priority,           // Prority of the thread.
    uint32  preempt,            // Indicates if the thread is preemptable.
    uint32  auto_start          // Specifies whether the thread starts 
                                // immediately or stays in a pure suspended
                                // state. Legal options are SCI_AUTO_START
                                // and SCI_DONT_START.
    )
{
	return SCI_CreateThread(thread_name, queue_name, entry, argc, argv, stack_size, queue_num, priority, preempt, auto_start);
}


/******************************************************************************
	@desc: socket send
*******************************************************************************/
int yte_sock_send(TCPIP_SOCKET_T so, char* buf, int len, int flags)
{
	return sci_sock_send(so, buf, len, flags);
}

/******************************************************************************
	@desc: socket asyn receive
*******************************************************************************/
int yte_sock_asyncrecv(
    TCPIP_SOCKET_T  so,
    char*           buf,
    int             len, 
    int             flag, 
    int*            left_len_ptr)
{
	return sci_sock_asyncrecv(so, buf, len, flag, left_len_ptr);
}

/******************************************************************************
	@desc: pdp active
*******************************************************************************/
BOOLEAN yte_pdp_active(MMIPDP_ACTIVE_INFO_T *app_info_ptr)
{
	return MMIAPIPDP_Active(app_info_ptr);
}

/******************************************************************************
	@desc: pdp deactive
*******************************************************************************/
BOOLEAN yte_pdp_deactive(uint32 app_handler)
{
	return MMIAPIPDP_Deactive(app_handler);
}

/******************************************************************************
	@desc: handle sms ind
*******************************************************************************/
void yte_sms_handle_msg_id(DPARAM param)
{
	MMISMS_HandleSMSMTInd(param);
}
#ifdef WIN32
/* get SDK version */
ivTTSErrID ivCall ivTTS_GetVersion(
    ivPUInt8        piMajor,            /* [out] major version number */
    ivPUInt8        piMinor,            /* [out] minor version number */
    ivPUInt16       piRevision)         /* [out] revision number */
{
    return 0;
}

/* create an instance */
ivTTSErrID ivCall ivTTS_Create(
    ivHTTS ivPtr    phTTS,              /* [out] handle to an instance */
    ivPointer       pHeap,              /* [in] heap for instance */
    ivSize          nHeapSize,          /* [in] size of the heap */
    ivPointer       pCBParam,           /* [in] user callback parameter */
    ivPResPackDesc  pResPackDesc,       /* [in] resource pack description array */
    ivSize          nResPackCount)      /* [in] resource pack count */
{
    return 0;
}

/* get a parameter associated with an instance */
ivTTSErrID ivCall ivTTS_GetParam(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivUInt32        nParamID,           /* [in] parameter ID */
    ivPUInt32       pnParamValue )      /* [out] buffer to receive the parameter value */
{
    return 0;
}

/* set a parameter associated with an instance */
ivTTSErrID ivCall ivTTS_SetParam(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivUInt32        nParamID,           /* [in] parameter ID */
    ivUInt32        nParamValue )       /* [in] parameter value */
{
    return 0;
}

/* get pcm data */
ivTTSErrID ivCall ivTTS_GetData(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivPointer       pData,              /* [in] pointer of pcm data buffer */
    ivSize ivPtr    pSize )             /* [in/out] data size */
{
    return 0;
}

/* exit running of an instance and leave current thread's control */
ivTTSErrID ivCall ivTTS_Exit(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

/* synthesize a buffer of text on an instance */
ivTTSErrID ivCall ivTTS_SynthText(
    ivHTTS          hTTS,               /* [in] handle to an instance */
    ivCPointer      pcData,             /* [in] pointer of text buffer data to be synthesized */
    ivSize          nSize )             /* [in] size of text buffer data to be synthesized */
{
    return 0;
}

/* destroy an instance */
ivTTSErrID ivCall ivTTS_Destroy(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

/* run an instance and hold current thread's control */
ivTTSErrID ivCall ivTTS_Run(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

/* begin to synthesize from callback on an instance */
ivTTSErrID ivCall ivTTS_SynthStart(
    ivHTTS          hTTS )              /* [in] handle to an instance */
{
    return 0;
}

void yt_tts_free_resource(void)
{

}

int yt_tts_play_text(long yt_callback_function_for_tts, char char_reserved)
{
    return 0;
}

int  yt_tts_input_text_utf16(unsigned short *strText_U16,//[in]
												  unsigned int nTextLen_U16,//[in]
												  unsigned int DATE_FORMAT,
												  unsigned int nTextType)
{
    return 0;
}

void yt_set_callback_func_for_debug(long pCallBack)
{

}

int yt_tts_set_memory_buffer(char *pBufferForTTSEngine,unsigned int nMemSize)
{
    return 0;
}

#ifndef YT_TTS_TWO_RES_FILE
int  yt_tts_initialize( int nLangID,char *strArgOne, int nVoiceID, char *strArgTwo)
{
    return 0;
}
#else
int  yt_tts_initialize( char *strReserved,
						int nLangID,
						char *strArgOne, 
						int nVoiceID, 
						char *strArgTwo)
{
    return 0;
}

void yt_set_callback_func_for_fclose(long pCallBack)
{

}

void yt_set_callback_func_for_fseek(long pCallBack)
{

}

void yt_set_callback_func_for_fread(long pCallBack)
{

}

void yt_set_callback_func_for_fopen(long pCallBack)
{

}
#endif //YT_TTS_TWO_RES_FILE

int yte_tts_handle_sms_ind ( void * param )
{
	return 0;
}
#endif //WIN32

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
