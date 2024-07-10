#define _MV_PLAYER_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mv_debug.h"
#include "sci_api.h"
#include "mmivp_export.h"
#include "mmivp_internal.h"
#include "mv_player_api.h"
#include "mv_net_api.h"
#include "mn_type.h"
#include "mmi_module.h"
#include "mv.h"
#include "mv_base_api.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"

/**-----

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MV_DEFAULT_VOL  3;
#define MV_NORMAL_HANDLE 1;
#define MV_PLAYER_ASYN_MODE
#ifdef VT_SUPPORT
#define MV_CANVT (1)	//用来标识终端本身是否支持视频通话[ 0:不支持 ;	// 1:支持]
#else
#define MV_CANVT (0)
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//播放器信息
typedef struct  
{
    VP_HANDLE           vp_handle;                              //状态机handle
    VP_STATE_E          state;                                  //播放器ui状态
    TMeidaPlayer_Program      play_type;                              //播放类型  
    uint32              total_time;                             //总时间
    uint32              process_time;                           //播放进度           
    uint8               url[VP_MAX_URL_LEN +1];                 //网络播放的url
    wchar               full_path_name[VP_MAX_NAME_LEN +1];     //本地播放时为全路径，网络播放时为片源名称
    uint16              full_path_len;                          //名称长度    
    MediaPlayerCallbackProc pMediaPlayerCallbackProc;         //当播放器事件发生的时候通过该回调通知应用
	void* pCallbackParam;                                       //回调函数的参数
    GUI_RECT_T          display_rect;                            //图象显示区域
    BOOLEAN             is_rotate;                               //是否旋转
    uint8               vol;                                     //音量
    BOOLEAN             is_replay;                               //是否继续重新播放
    uint8               timer_id;
}MV_PLAYING_INFO_T;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MV_PLAYING_INFO_T s_mv_control_info = {0};
LOCAL BOOLEAN           s_is_seeking = FALSE; //for seek when paused
LOCAL BOOLEAN           s_is_pausing = FALSE; //for living pause
LOCAL BOOLEAN			s_is_AllowFillRect = TRUE;	//是否允许刷区域
LOCAL _RECT				g_NormalRect = {0,0,0,0};	//正常刷屏区域
LOCAL BOOLEAN			g_isSuspend = FALSE;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 提示出错信息
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayErrorCode(VP_ERROR_E error_code);
/*****************************************************************************/
//  Description : HandleFsmMsg
//  Global resource dependence : 
//  Author: 
//  Note: 处理状态机给ui发送的消息
/*****************************************************************************/
LOCAL void HandleFsmMsg(VP_CALLBACK_T *msg_ptr);

/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(uint32 alpha);

/*****************************************************************************/
//  Description : 处理进度timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void MV_PROCESS_TIMER( uint8 timer_id, uint32 param );
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 提示出错信息
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayErrorCode(VP_ERROR_E error_code)
{
    TMeidaPlayerStatus  mv_error_type = PLAYER_CREATE;
    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] DisplayErrorCode error_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_104_112_2_18_2_39_17_139,(uint8*)"d", error_code);
#endif
    switch(error_code) 
    {      
    case VP_ERROR_NOT_SUPPORT:
        mv_error_type = PLAYER_NOT_SUPPORT;
        break;
    case VP_ERROR_INTERNAL_SERVER_ERROR:
        mv_error_type = REQUEST_SERV_ERR;
        break;
    case VP_ERROR_BAD_REQUEST:
        mv_error_type = REQUEST_SERV_ERR;
        break; 

    default:
        mv_error_type = SERV_RES_ERR;
        break;
    }            
     
    if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
    {    
        s_mv_control_info.pMediaPlayerCallbackProc(handle, 
            s_mv_control_info.pCallbackParam,
            mv_error_type, 
            PNULL);      
    }
}
/*****************************************************************************/
//  Description : HandleFsmMsg
//  Global resource dependence : 
//  Author: 
//  Note: 处理状态机给ui发送的消息
/*****************************************************************************/
LOCAL void HandleFsmMsg(VP_CALLBACK_T *msg_ptr)
{ 

    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;

    if (PNULL == msg_ptr || PNULL == msg_ptr->para)
    {
    #ifdef PLAYER_DEBUG
        //MV_TRACE_LOW:"[MV] HandleFsmMsg param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_143_112_2_18_2_39_17_140,(uint8*)"");
    #endif
        return;
    }
#ifdef PLAYER_DEBUG
    CM_WriteLogs(1, "[MV] HandleFsmMsg msg_ptr->msg_id = %x",msg_ptr->msg_id);
    //MV_TRACE_LOW:"[MV] HandleFsmMsg msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_149_112_2_18_2_39_17_141,(uint8*)"d",msg_ptr->msg_id);
#endif
    switch(msg_ptr->msg_id) 
    {
    case VP_PREPARE_IND:   //从媒体中获得显示信息
        {
            VP_PREPARE_IND_T prepare_info = *(VP_PREPARE_IND_T *)msg_ptr->para;                   

            if (prepare_info.vp_handler == s_mv_control_info.vp_handle)
            {                            
                s_mv_control_info.full_path_len = (uint16)MIN(prepare_info.file_name_len,VP_MAX_NAME_LEN);
                SCI_MEMSET(s_mv_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(s_mv_control_info.full_path_name,
                    prepare_info.file_name_ptr,
                    s_mv_control_info.full_path_len);               
                s_mv_control_info.total_time = prepare_info.total_time;  

			#if MV_CANVT
				//此处判断是否为直播节目,视屏通话的时候需要,
            	if(VP_MAX_INVALID_VALUE == s_mv_control_info.total_time)
				{
					s_mv_control_info.play_type = PROGRAM_LIVE;
				}
			#endif
			
			#ifdef PLAYER_DEBUG    
                CM_WriteLogs(1, "[MV] HandleFsmMsg play_type = %d, total_time = %d",prepare_info.play_type,s_mv_control_info.total_time);
				//MV_TRACE_LOW:"[MV] HandleFsmMsg play_type = %d, total_time = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_176_112_2_18_2_39_17_142,(uint8*)"dd",prepare_info.play_type,s_mv_control_info.total_time);
			#endif
            } 
        }
        break;   

    case VP_BUFFERING_IND: //开始缓冲
        {
            VP_BUFFERING_IND_T buffering_ind = *(VP_BUFFERING_IND_T *)msg_ptr->para;
            if (buffering_ind.vp_handler == s_mv_control_info.vp_handle)
            { 
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg buffering s_mv_control_info.state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_188_112_2_18_2_39_17_143,(uint8*)"d",s_mv_control_info.state);
#endif
                s_mv_control_info.state = VP_STATE_BUFFERING;
                if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
                {
                    s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                        s_mv_control_info.pCallbackParam,
                        DATA_BUFEER,
                        PNULL);    
                }
            }
        }       
        break;

    case VP_BUFFER_PROCESS_IND:
        {
            VP_BUFFERING_PROCESS_IND_T buffering_ind = *(VP_BUFFERING_PROCESS_IND_T *)msg_ptr->para;
            if (buffering_ind.vp_handler == s_mv_control_info.vp_handle)
            { 
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg buffering s_mv_control_info.state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_208_112_2_18_2_39_17_144,(uint8*)"d",s_mv_control_info.state);
#endif               
                if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
                {
                    s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                        s_mv_control_info.pCallbackParam, 
                        DATA_BUFEER, 
                        buffering_ind.buffer_percent);  
                }
            }
        }   
        break;

    case VP_BUFFER_END_IND: //缓冲完成
        {
            VP_BUFFER_END_IND_T buffer_end_ind = *(VP_BUFFER_END_IND_T *)msg_ptr->para;
            if (buffer_end_ind.vp_handler == s_mv_control_info.vp_handle)
            {           
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg buffer end s_mv_control_info.state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_227_112_2_18_2_39_17_145,(uint8*)"d",s_mv_control_info.state);
#endif
                s_mv_control_info.state = VP_STATE_PLAYING;
                if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
                {
                    s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                        s_mv_control_info.pCallbackParam, 
                        PLAY_VIDEO_OK, 
                        PNULL);       
                }
            }
        }        
        break;

         
    case VP_PLAY_CNF: //开始播放
        {
            VP_PLAY_CNF_T play_result = *(VP_PLAY_CNF_T *)msg_ptr->para;          
            if (play_result.vp_handler == s_mv_control_info.vp_handle)
            {
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg play_result.error_code = %d,  s_video_player_control_info.process_time = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_250_112_2_18_2_39_17_146,(uint8*)"dd",play_result.error_code,                 s_mv_control_info.process_time);
#endif
				CM_WriteLogs(128, "HandleFsmMsg()::case VP_PLAY_CNF::play_result.error_code == %d", play_result.error_code);
                if(VP_ERROR_NONE == play_result.error_code) //开始播放成功
                {  
#ifdef PLAYER_DEBUG
                    //MV_TRACE_LOW:"[MV] HandleFsmMsg play cnf s_mv_control_info.state = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_256_112_2_18_2_39_17_147,(uint8*)"d",s_mv_control_info.state);
#endif                    
                    s_mv_control_info.state = VP_STATE_PLAYING;
                    if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
                    {
                        s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                            s_mv_control_info.pCallbackParam, 
                            PLAY_VIDEO_OK, 
                            PNULL);
                    }
                    s_mv_control_info.timer_id =  MMK_CreateTimerCallback(
                                     300,
                                     MV_PROCESS_TIMER,
                                     PNULL,
                                     FALSE);
                    MMK_StartTimerCallback(s_mv_control_info.timer_id,300,MV_PROCESS_TIMER,PNULL,FALSE);
#ifdef PLAYER_DEBUG
                    //MV_TRACE_LOW:"[MV] HandleFsmMsg s_mv_control_info.timer_id = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_273_112_2_18_2_39_17_148,(uint8*)"d",s_mv_control_info.timer_id);
#endif  
                }
                else
                {   
#ifdef PLAYER_DEBUG
                    //MV_TRACE_LOW:"[MV] HandleFsmMsg play cnf error s_mv_control_info.state = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_279_112_2_18_2_39_17_149,(uint8*)"d",s_mv_control_info.state);
#endif
                    s_mv_control_info.state = VP_STATE_READY; //播放失败，进入ready状态   
                    DisplayErrorCode(play_result.error_code);
                }                         
            }            
            
        }        
        break;

    case VP_PAUSE_CNF://暂停
        {
            VP_PAUSE_CNF_T pause_result = *(VP_PAUSE_CNF_T *)msg_ptr->para;
            if (pause_result.vp_handler == s_mv_control_info.vp_handle)
            {
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg pause_result.error_code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_295_112_2_18_2_39_17_150,(uint8*)"d",pause_result.error_code);
#endif
                if (VP_ERROR_NONE == pause_result.error_code)  //暂停成功
                {    
#ifdef PLAYER_DEBUG
                    //MV_TRACE_LOW:"[MV] HandleFsmMsg pause cnf s_mv_control_info.state = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_300_112_2_18_2_39_17_151,(uint8*)"d",s_mv_control_info.state);
#endif
                    if (VP_STATE_PAUSED == s_mv_control_info.state
                        || VP_STATE_PAUSING == s_mv_control_info.state)
                    {
                        s_mv_control_info.state = VP_STATE_PAUSED;  //进入暂停状态，显示播放图标，显示暂停成功提示
//                        if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
//                        {
//                            s_mv_control_info.pMediaPlayerCallbackProc(handle, 
//                                s_mv_control_info.pCallbackParam, 
//                                PAUSE_VIDEO_OK, 
//                                PNULL);  
//                        }
                    }                                 
                    
                }
            }
            
        }
        break;    

    case VP_SEEK_CNF: //seek
        {
            VP_SEEK_CNF_T seek_result = *(VP_SEEK_CNF_T *)msg_ptr->para;
            if (seek_result.vp_handler == s_mv_control_info.vp_handle) 
            {
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg seek_result.error_code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_327_112_2_18_2_39_18_152,(uint8*)"d",seek_result.error_code);
#endif
                s_mv_control_info.state = VP_STATE_PLAYING; //seek成功，进入播放状态   
                if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
                {
                    s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                        s_mv_control_info.pCallbackParam, 
                        PLAY_VIDEO_OK, 
                        PNULL);
                }
                
            }            
        }
        break;

    case VP_FATAL_ERROR_IND:
        {
            VP_FATAL_ERROR_IND_T fatal_error = *(VP_FATAL_ERROR_IND_T *)msg_ptr->para;           

            if (fatal_error.vp_handler == s_mv_control_info.vp_handle)
            {
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg fatal_error.error_code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_349_112_2_18_2_39_18_153,(uint8*)"d",fatal_error.error_code);
#endif
                s_mv_control_info.state = VP_STATE_READY;  //发生异常错误，提示出错类型，禁止弹出授权窗口               
                DisplayErrorCode(fatal_error.error_code);  
                
            }            
        }
        break;  
    
    case VP_PLAY_END_IND://播放结束，循环播放等，每首播放结束都可以收到该消息
        {
            VP_PLAY_END_IND_T play_end_ind = *(VP_PLAY_END_IND_T *)msg_ptr->para;

            if (play_end_ind.vp_handler == s_mv_control_info.vp_handle)
            {     
                s_mv_control_info.process_time = s_mv_control_info.total_time;
                GUI_FillRect(MMITHEME_GetDefaultLcdDev(),s_mv_control_info.display_rect,MMIAPIVP_GetTransparenceColor());
                GUIBLOCK_ResetMainType();
                if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
                {
                    s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                        s_mv_control_info.pCallbackParam, 
                        PLAYER_ENDOFCLIP, 
                        PNULL);  
                }
                
            }
        }
        break;

    case VP_STOP_IND: //全部停止播放
        {
            VP_STOP_IND_T stop_ind = *(VP_STOP_IND_T *)msg_ptr->para;     
            
            if (stop_ind.vp_handler == s_mv_control_info.vp_handle)
            {   
#ifdef PLAYER_DEBUG
                //MV_TRACE_LOW:"[MV] HandleFsmMsg stop cnf s_mv_control_info.state = %d, s_mv_control_info.is_replay = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_386_112_2_18_2_39_18_154,(uint8*)"dd",s_mv_control_info.state,s_mv_control_info.is_replay);
#endif
                if (s_is_pausing)  //living paused
                {
                    s_mv_control_info.state = VP_STATE_PAUSED;
                    s_is_pausing = FALSE;
                    break;                    
                }
                
                if (VP_STATE_READY != s_mv_control_info.state)  //如果是正常结束，则提示停止，否则显示停止出错提示信息
                {
                    s_mv_control_info.state = VP_STATE_READY;                   
                }
               
                s_mv_control_info.process_time = 0;       //当前播放时间归零         
                if (0 != s_mv_control_info.timer_id)
                {
                    MMK_StopTimer(s_mv_control_info.timer_id);
                    s_mv_control_info.timer_id = 0;
                }

                
                
                if (s_mv_control_info.is_replay)
                {
                    s_mv_control_info.is_replay = FALSE;
                    CM_MediaPlayer_Play(handle);
                }   
                else
                {
                    //GUI_FillRect(MMITHEME_GetDefaultLcdDev(),s_mv_control_info.display_rect,MMIAPIVP_GetTransparenceColor());
                }
            }
        }
        break;

    default:
        break;
    }
    
}
/*****************************************************************************/
//  Description : MV_MediaPlayer_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Create(void)
{

    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;

    if (PNULL != s_mv_control_info.vp_handle)
    {
        return;
    }
    s_mv_control_info.vp_handle = MMIAPIVP_FSMInit(HandleFsmMsg); 

#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_Create after:MMIAPIVP_FSMInit s_mv_control_info.vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_443_112_2_18_2_39_18_155,(uint8*)"d",s_mv_control_info.vp_handle);
    CM_WriteLogs(1, "[MV] MV_MediaPlayer_Create after:MMIAPIVP_FSMInit s_mv_control_info.vp_handle = %x",s_mv_control_info.vp_handle);
#endif

    if (PNULL != s_mv_control_info.vp_handle)
    {
        s_mv_control_info.state = VP_STATE_READY;
        if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
        {     
            s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                s_mv_control_info.pCallbackParam, 
                PLAYER_CREATE_OK, 
                PNULL); 
        }
    }
    else if(PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
    {
         s_mv_control_info.pMediaPlayerCallbackProc(handle, 
             s_mv_control_info.pCallbackParam, 
             PLAYER_CREATE_ERR, 
             PNULL);  
    }
}
/*****************************************************************************/
//  Description : CM_MediaPlayer_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_HANDLE CM_MediaPlayer_Create(
									_INT left,
									_INT top,
									_INT width,
									_INT height,
									MediaPlayerCallbackProc pMediaPlayerCallbackProc,
									_VOID* pCallbackParam,
									_INT nProgram   //0 local, 1 点播, 2直播 (rtsp)
)
{
    _HANDLE handle = NULL;
    
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Create left = %d, top = %d, width = %d, height = %d, pMediaPlayerCallbackProc = %x, nProgram = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_484_112_2_18_2_39_18_156,(uint8*)"dddddd",left,top,width,height,pMediaPlayerCallbackProc, nProgram);
#endif
    if (PNULL == pMediaPlayerCallbackProc) //参数合法性判断 
    {
        return NULL;
    }

    if (nProgram >= PROGRAMCOUNT)
    {
        return NULL;
    }

    if (0 != s_mv_control_info.vp_handle)
    {
        return NULL;
    }

    
    s_mv_control_info.display_rect.left = left;
    s_mv_control_info.display_rect.top = top;
    s_mv_control_info.display_rect.right = left + width - 1;
    s_mv_control_info.display_rect.bottom = top + height - 1;
    s_mv_control_info.pCallbackParam = pCallbackParam;
    s_mv_control_info.pMediaPlayerCallbackProc = pMediaPlayerCallbackProc;
    s_mv_control_info.play_type = nProgram;       
    s_mv_control_info.vp_handle = PNULL; 
    s_mv_control_info.is_rotate = FALSE;
    s_mv_control_info.process_time = 0;
    s_mv_control_info.total_time = 0;
    if(MV_IsSilentMode())
    {
        s_mv_control_info.vol = 0;
    }
    else
    {
        s_mv_control_info.vol = VP_DEFAULT_VOLUME;
    }
    s_mv_control_info.timer_id = 0;
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Create vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_523_112_2_18_2_39_18_157,(uint8*)"d", s_mv_control_info.vp_handle);
#endif
#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_CREATE_REQ,0);
#else
    MV_MediaPlayer_Create();
#endif
    handle = (_HANDLE)MV_NORMAL_HANDLE;
    return handle;
   
}
/*****************************************************************************/
//  Description : MV_MediaPlayer_Exit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Exit(void)
{
    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
#ifdef PLAYER_DEBUG    
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_Exit s_mv_control_info.vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_542_112_2_18_2_39_18_158,(uint8*)"d",s_mv_control_info.vp_handle);
#endif    
    s_is_pausing = FALSE;
    MMIAPIVP_FSMExit(s_mv_control_info.vp_handle);
    GUIBLOCK_ResetMainType();
    s_mv_control_info.vp_handle = PNULL;
    if (0 != s_mv_control_info.timer_id)
    {
       MMK_StopTimer(s_mv_control_info.timer_id);
       s_mv_control_info.timer_id = 0;
    }
    if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
    {
        s_mv_control_info.pMediaPlayerCallbackProc(handle, 
            s_mv_control_info.pCallbackParam, 
            PLAYER_RELEASE_OK, 
            PNULL);   
    }
    
    
}

/*****************************************************************************/
//  Description : MV_MediaPlayer_Release
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Release(void)
{
    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
#ifdef PLAYER_DEBUG    
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_Release s_mv_control_info.vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_572_112_2_18_2_39_18_159,(uint8*)"d",s_mv_control_info.vp_handle);
#endif
    
    if (PNULL != s_mv_control_info.vp_handle)
    {
        MMIAPIVP_FSMRelease(s_mv_control_info.vp_handle);   
    }   
    GUIBLOCK_ResetMainType();
    s_mv_control_info.vp_handle = PNULL;
    if (0 != s_mv_control_info.timer_id)
    {
       MMK_StopTimer(s_mv_control_info.timer_id);
       s_mv_control_info.timer_id = 0;
    }
    if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
    {
        s_mv_control_info.pMediaPlayerCallbackProc(handle, 
            s_mv_control_info.pCallbackParam, 
            PLAYER_RELEASE_OK, 
            PNULL);   
    }   
    SCI_MEMSET(&s_mv_control_info,0,sizeof( MV_PLAYING_INFO_T));
}
/*****************************************************************************/
//  Description : CM_MediaPlayer_Release
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_Release(_HANDLE hPlayer)
{
#ifdef PLAYER_DEBUG    
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Release hPlayer= %x, state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_602_112_2_18_2_39_18_160,(uint8*)"dd",hPlayer,s_mv_control_info.state);
#endif
    if (NULL == hPlayer)
    {
        return _FALSE;
    }
    if (PNULL == s_mv_control_info.vp_handle)
    {
        s_mv_control_info.pMediaPlayerCallbackProc(hPlayer, 
            s_mv_control_info.pCallbackParam, 
            PLAYER_RELEASE_OK, 
            PNULL);   
        return _TRUE;
    }
    if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
    {
        s_mv_control_info.pMediaPlayerCallbackProc(hPlayer, 
            s_mv_control_info.pCallbackParam, 
            PLAYER_RELEASE_OK, 
            PNULL);   
    }
    
    if (MV_IsPlaying())
    {
    #ifdef MV_PLAYER_ASYN_MODE
        CM_SendSignalToMMI(MVIDEO_EXIT_REQ,0);
    #else
        MV_MediaPlayer_Exit();
    #endif
    }
    else
    {
    #ifdef MV_PLAYER_ASYN_MODE
        CM_SendSignalToMMI(MVIDEO_RELEASE_REQ,0);
    #else
        MV_MediaPlayer_Release();
    #endif
    }
    return _TRUE;   
}

/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(uint32 alpha)
{    
    GUI_COLOR_T             color_key       =    MMIAPIVP_GetTransparenceColor();
    GUIBLOCK_SetType(alpha,color_key,GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev());    
}


/*****************************************************************************/
//  Description : CM_MediaPlayer_Open
//  Global resource dependence : 
//  Author:
//  Note: *nUrlSize: the char number of Url address
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_Open(_HANDLE hPlayer,_CONST _WCHAR* pUrl,_INT nUrlSize,_INT nProgram)
{
    DPLAYER_MEDIA_INFO_T media_info = {0};
    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Open hPlayer = %x, pUrl= %x, nUrlSize = %d, play_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_663_112_2_18_2_39_18_161,(uint8*)"dddd", hPlayer, pUrl, nUrlSize,s_mv_control_info.play_type);
#endif
    if (NULL == hPlayer)	return _FALSE;

    if (PNULL != pUrl)
    {
        s_mv_control_info.play_type = nProgram;       
        if (PROGRAM_LOCAL != s_mv_control_info.play_type)
        {
            SCI_MEMSET(s_mv_control_info.url,0,(VP_MAX_URL_LEN +1));
            MMIAPICOM_WstrToStr(pUrl, (uint8 *)(s_mv_control_info.url));
            s_mv_control_info.full_path_len = 0;
            SCI_MEMSET(s_mv_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        }
        else
        {
            s_mv_control_info.full_path_len = 0;
            SCI_MEMSET(s_mv_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
            MV_path_transfer((uint16*)pUrl, s_mv_control_info.full_path_name);           
            s_mv_control_info.full_path_len  = MMIAPICOM_Wstrlen((wchar *)s_mv_control_info.full_path_name);
            MMIAPIVP_GetInfoFromFullPathName(s_mv_control_info.full_path_name,s_mv_control_info.full_path_len,&media_info);
            s_mv_control_info.total_time = media_info.media_length;              
            SCI_MEMSET(s_mv_control_info.url,0,(VP_MAX_URL_LEN +1));
        #ifdef PLAYER_DEBUG
            //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Open full_path_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_687_112_2_18_2_39_18_162,(uint8*)"d", s_mv_control_info.full_path_len);
            CM_WriteLogs(1, "[MV] CM_MediaPlayer_Open full_path_len = %d", s_mv_control_info.full_path_len);
        #endif
        }
        
        #ifdef PLAYER_DEBUG
            //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Open() s_mv_control_info.total_time=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_693_112_2_18_2_39_18_163,(uint8*)"d", s_mv_control_info.total_time);
            CM_WriteLogs(1, "[MV] CM_MediaPlayer_Open() s_mv_control_info.total_time=%d", s_mv_control_info.total_time);
        #endif
        if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
        {
            s_mv_control_info.pMediaPlayerCallbackProc(handle,  
                s_mv_control_info.pCallbackParam, 
                REQUEST_SERV_OK, 
                PNULL); 
        }
        return _TRUE;
    }
	
    return _FALSE;
}


/*****************************************************************************/
//  Description : MV_MediaPlayer_Play
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Play(void)
{
    VP_PLAY_PARAM_T    *play_param_ptr = PNULL;   
    VP_DISPLAY_PARAM_T 	display_param = {0};
  // begin
#if MV_CANVT
	if(TRUE == g_isSuspend && (PNULL != s_mv_control_info.vp_handle))
	{
		SetBlockType(255);
       	GUI_FillRect(MMITHEME_GetDefaultLcdDev(),s_mv_control_info.display_rect,MMIAPIVP_GetTransparenceColor());
		g_isSuspend = FALSE;
		MMIAPIVP_FSMCompulsiveResume(s_mv_control_info.vp_handle);

		return;
	}
#endif
  	//end
#ifdef PLAYER_DEBUG
	//MV_TRACE_LOW:"[MV] MV_MediaPlayer_Play()    s_mv_control_info.(state:%d, play_type:%d, vp_handle:%d)"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_732_112_2_18_2_39_18_164,(uint8*)"ddd",s_mv_control_info.state,s_mv_control_info.play_type,s_mv_control_info.vp_handle);
	CM_WriteLogs(1, "[MV] MV_MediaPlayer_Play()    s_mv_control_info.(state:%d, play_type:%d, vp_handle:%d)",s_mv_control_info.state,s_mv_control_info.play_type,s_mv_control_info.vp_handle);
#endif

    if (VP_STATE_PAUSED == s_mv_control_info.state || VP_STATE_PAUSING == s_mv_control_info.state)  //恢复播放
    {
        s_mv_control_info.state = VP_STATE_PLAYING;     

        if (PROGRAM_LOCAL != s_mv_control_info.play_type)
        {    
            s_mv_control_info.state = VP_STATE_RESUMING;
        } 
        if (PROGRAM_DEMAND == s_mv_control_info.play_type || PROGRAM_LOCAL == s_mv_control_info.play_type)
        {
            SetBlockType(255);
		
			MV_ReSetDisplayParam(s_mv_control_info.is_rotate, (_VOID*)&display_param);
			
	        MMIAPIVP_FillRectWithTransparenceColor(&(display_param.disp_rect.dx),
	            &(display_param.disp_rect.dy),
	            &(display_param.disp_rect.x),
	            &(display_param.disp_rect.y),
	            display_param.RotateAngle,
	            TRUE);

	        MMIAPIVP_FillRectWithTransparenceColor(&(display_param.target_rect.dx),
	            &(display_param.target_rect.dy),
	            &(display_param.target_rect.x),
	            &(display_param.target_rect.y),
	            display_param.RotateAngle,
	            TRUE);
	        GUI_FillRect(MMITHEME_GetDefaultLcdDev(), s_mv_control_info.display_rect, MMIAPIVP_GetTransparenceColor());

            MMIAPIVP_FSMResume(s_mv_control_info.vp_handle);
            return;
        }
    }
    else
    {
        if (PROGRAM_LOCAL != s_mv_control_info.play_type)
	    {
	        s_mv_control_info.state = VP_STATE_PLAY_PENDING;
	    }
	    else
	    {
        	s_mv_control_info.state = VP_STATE_PLAYING;
    	}
		//判断是否需要旋转，避免本地播放或网络播放失败后重播刷屏问题
		if((GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) == LCD_ANGLE_90) || (GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) == LCD_ANGLE_270))
		{
			display_param.RotateAngle = TRUE;	// 0：0°； 1：90°； 2：180°； 3：270°；
		}
    }    

    play_param_ptr = (VP_PLAY_PARAM_T*)SCI_ALLOCA(sizeof(VP_PLAY_PARAM_T));
    
    if (PNULL == play_param_ptr)	return;

    SCI_MEMSET(play_param_ptr,0,sizeof(VP_PLAY_PARAM_T));
    
    SetBlockType(255);
    play_param_ptr->dual_sys = MMIAPISET_GetActiveSim();
    play_param_ptr->display_param.audioSupport = 1;
    play_param_ptr->display_param.videoSupport = 1;
    play_param_ptr->display_param.b_support_seek = 1;
	play_param_ptr->display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
    play_param_ptr->display_param.RotateAngle = display_param.RotateAngle;	//GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);      //获取默认屏幕旋转角度（仅针对宽屏手机）
    play_param_ptr->is_scal_up = TRUE;
    play_param_ptr->is_random_play = FALSE;
    play_param_ptr->is_save_history = FALSE;
    play_param_ptr->is_share_play = TRUE;
    play_param_ptr->mode = VP_REPEAT_PLAY_MODE_CLOSE;
    play_param_ptr->vol = MV_DEFAULT_VOL;   
    
    play_param_ptr->display_param.disp_rect.dy = s_mv_control_info.display_rect.bottom - s_mv_control_info.display_rect.top + 1;
    play_param_ptr->display_param.disp_rect.dx = s_mv_control_info.display_rect.right - s_mv_control_info.display_rect.left + 1;       
    play_param_ptr->display_param.disp_rect.x = s_mv_control_info.display_rect.left;
    play_param_ptr->display_param.disp_rect.y = s_mv_control_info.display_rect.top;   

    play_param_ptr->display_param.target_rect.x = play_param_ptr->display_param.disp_rect.x;
    play_param_ptr->display_param.target_rect.y = play_param_ptr->display_param.disp_rect.y;
    play_param_ptr->display_param.target_rect.dx = play_param_ptr->display_param.disp_rect.dx;
    play_param_ptr->display_param.target_rect.dy = play_param_ptr->display_param.disp_rect.dy;    

    play_param_ptr->video_buf_info.type = 0;
    play_param_ptr->video_buf_info.video_buf_len = 0;
    play_param_ptr->video_buf_info.video_buf_ptr = PNULL;

    if (PROGRAM_LOCAL == s_mv_control_info.play_type)
    {
        play_param_ptr->play_type = VP_PLAY_FROM_FILE;
    }
    else
    {
        play_param_ptr->play_type = VP_PLAY_FROM_NET;
    }    
    
    play_param_ptr->is_save_history = FALSE; 
    play_param_ptr->is_random_play = FALSE;
    play_param_ptr->mode = VP_REPEAT_PLAY_MODE_CLOSE;
     
    if (PROGRAM_LOCAL == s_mv_control_info.play_type)
    {
        play_param_ptr->full_path_len = s_mv_control_info.full_path_len;
        play_param_ptr->full_path_ptr = s_mv_control_info.full_path_name;           
        play_param_ptr->url_ptr = PNULL;            
    }
    else
    {
        play_param_ptr->full_path_len = 0;
        play_param_ptr->full_path_ptr = PNULL;     
       
        play_param_ptr->url_ptr = (char *)(s_mv_control_info.url);            
    }
    
    play_param_ptr->vol = s_mv_control_info.vol;    

    MMIAPIVP_FSMPlay(s_mv_control_info.vp_handle,play_param_ptr);      
   
    SCI_FREE(play_param_ptr);
    play_param_ptr = PNULL;
}
/*****************************************************************************/
//  Description : this function is called by UI, when player is pause ,it is used to replay media file. when player is stop,it is used to  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_Play(_HANDLE hPlayer)
{
#ifdef PLAYER_DEBUG
	//MV_TRACE_LOW:"[CM] CM_MediaPlayer_Play()   s_mv_control_info.state = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_861_112_2_18_2_39_19_165,(uint8*)"d",s_mv_control_info.state);
	CM_WriteLogs(1, "[CM] CM_MediaPlayer_Play()   s_mv_control_info.state = %d",s_mv_control_info.state);
#endif

    if (NULL == hPlayer)
    {
        return _FALSE;
    }   
	s_is_AllowFillRect = TRUE;
#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_PLAY_REQ,0);
#else
    MV_MediaPlayer_Play();
#endif
    return _TRUE;    
}

/*****************************************************************************/
//  Description : this function is used to pause the player
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Pause(void)
{
    BOOLEAN result = TRUE;
    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
#ifdef PLAYER_DEBUG
	//MV_TRACE_LOW:"[MV] MV_MediaPlayer_Pause()  s_mv_control_info.state = %d,type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_887_112_2_18_2_39_19_166,(uint8*)"dd",s_mv_control_info.state,s_mv_control_info.play_type);
	CM_WriteLogs(1, "[MV] MV_MediaPlayer_Pause()  s_mv_control_info.state = %d,type = %d",s_mv_control_info.state,s_mv_control_info.play_type);
#endif
	if((VP_STATE_PAUSED == s_mv_control_info.state) || (VP_STATE_PAUSING == s_mv_control_info.state))
		return;
    s_mv_control_info.state = VP_STATE_PAUSED;     


    if(PROGRAM_LOCAL != s_mv_control_info.play_type)
    {
        s_mv_control_info.state = VP_STATE_PAUSING;
    } 

    if (PROGRAM_LIVE == s_mv_control_info.play_type)
    {
#ifdef PLAYER_DEBUG
	//MV_TRACE_LOW:"[MV] MV_MediaPlayer_Pause()  vdeotype = PROGRAM_LIVE will MMIAPIVP_FSMStop()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_903_112_2_18_2_39_19_167,(uint8*)"");
    CM_WriteLogs(1, "[MV] MV_MediaPlayer_Pause()  vdeotype = PROGRAM_LIVE will MMIAPIVP_FSMStop()");
#endif
        s_is_pausing = TRUE;
        result = MMIAPIVP_FSMStop(s_mv_control_info.vp_handle);
    }
    else
    {
#if MV_CANVT
		if(MMIAPIVT_IsVtCalling())
		{
		CM_WriteLogs(1, "[MV] MV_MediaPlayer_Pause()  vdeotype != PROGRAM_LIVE will MMIAPIVP_FSMCompulsivePause()");
	        MMIAPIVP_FSMCompulsivePause(s_mv_control_info.vp_handle);
			result = TRUE;
			g_isSuspend = TRUE;
		}
		else
#endif
		{
			result = MMIAPIVP_FSMPause(s_mv_control_info.vp_handle);
			g_isSuspend = FALSE;
		}
    }

    GUIBLOCK_ResetMainType();
	
	CM_WriteLogs(1, "[MV] MV_MediaPlayer_Pause PAUSE ret is %d",result);

    //GUIBLOCK_ClearByColor(MMIAPIVP_GetTransparenceColor(), (GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev());

    if(!result) //暂停失败，恢复状态
    {
CM_WriteLogs(1, "[MV] MV_MediaPlayer_Pause PAUSE FAIL s_mv_control_info.state = %d",s_mv_control_info.state);

        s_mv_control_info.state = VP_STATE_PLAYING;        
    }
    else
    {
        if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
        {
CM_WriteLogs(1, "[MV] MV_MediaPlayer_Pause PAUSE OK will call CallBackProc");
            s_mv_control_info.pMediaPlayerCallbackProc(handle, 
                s_mv_control_info.pCallbackParam, 
                PAUSE_VIDEO_OK, 
                PNULL);
        }
    }
}
/*****************************************************************************/
//  Description : this function is used to pause the player
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_Pause(_HANDLE hPlayer)
{
#ifdef PLAYER_DEBUG
     //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Pause "
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_958_112_2_18_2_39_19_168,(uint8*)"");
#endif

    if (NULL == hPlayer)	return _FALSE;

	s_is_AllowFillRect = FALSE;

#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_PAUSE_REQ,0);
#else
    MV_MediaPlayer_Pause();
#endif
	return _TRUE; 
}

/*****************************************************************************/
//  Description : this function is used to seek in media file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Seek(void)
{
    VP_SEEK_PARAM_T seek_param = {0};
   
    seek_param.seek_frame = VP_MAX_INVALID_VALUE;
    seek_param.seek_time = s_mv_control_info.process_time;  

#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_Seek s_mv_control_info.process_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_985_112_2_18_2_39_19_169,(uint8*)"d",s_mv_control_info.process_time);
#endif
    

    if (PROGRAM_LOCAL != s_mv_control_info.play_type)
    {         
        s_mv_control_info.state = VP_STATE_SEEKING;
    }         
    MMIAPIVP_FSMSeek(s_mv_control_info.vp_handle,&seek_param);    
   
}
/*****************************************************************************/
//  Description : this function is used to seek in media file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_Seek(_HANDLE hPlayer,_INT nTimeStamp)//单位s
{


    if (NULL == hPlayer)
    {
        return _FALSE;
    }

    if (PROGRAM_LIVE == s_mv_control_info.play_type)
    {
        return _TRUE;
    }
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_Seek nTimeStamp = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1014_112_2_18_2_39_19_170,(uint8*)"d",nTimeStamp);
#endif   
    s_mv_control_info.process_time = nTimeStamp * 1000;   
#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_SEEK_REQ,0);
#else
    MV_MediaPlayer_Seek();
#endif
	return _TRUE;
}

/*****************************************************************************/
//  Description : MV_MediaPlayer_Stop
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Stop(void)
{

    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;

    CM_WriteLogs(1, "[MV] CM_MediaPlayer_Seek s_mv_control_info.play_type = %d",s_mv_control_info.play_type);
    
    if (PROGRAM_LOCAL != s_mv_control_info.play_type)
    { 
        s_mv_control_info.state = VP_STATE_STOPING;   
    }
	
    s_is_pausing = FALSE;   
    MMIAPIVP_FSMStop(s_mv_control_info.vp_handle);  
	
    if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
    {    
        s_mv_control_info.pMediaPlayerCallbackProc(handle, s_mv_control_info.pCallbackParam, STOP_VIDEO_OK, PNULL); 
    }

}
/*****************************************************************************/
//  Description : CM_MediaPlayer_Stop
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_Stop(_HANDLE hPlayer)
{  
#ifdef PLAYER_DEBUG    
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_Release hPlayer= %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1057_112_2_18_2_39_19_171,(uint8*)"d",hPlayer);
#endif   
    if (NULL == hPlayer)
    {
        return _FALSE;
    } 

	s_is_AllowFillRect = FALSE;
	g_isSuspend = FALSE;

#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_STOP_REQ,0);
#else
    MV_MediaPlayer_Stop();
#endif
	return _TRUE;
}

/*****************************************************************************/
//  Description : MV_MediaPlayer_SetPlayWnd
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_SetPlayWnd(void)
{
    VP_DISPLAY_PARAM_T display_param = {0};
	
	MV_ReSetDisplayParam(s_mv_control_info.is_rotate, (_VOID*)&display_param);
	if (s_mv_control_info.is_rotate)
    {
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect(), MMIAPIVP_GetTransparenceColor());
    }
    MMIAPIVP_FSMSwitchDisplayMode(s_mv_control_info.vp_handle,&display_param,TRUE);
}
/*****************************************************************************/
//  Description : CM_MediaPlayer_SetPlayWnd
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_SetPlayWnd(_HANDLE handle,
										_BOOLEAN bRotate,  /*TURE ---全屏  FLSE ---半屏*/
										_BOOLEAN bZoom,
										_INT x,
										_INT y,
										_INT w,
										_INT h
)
{
    //VP_DISPLAY_PARAM_T display_param = {0};      
    
    if (NULL == handle)	return _FALSE;
	
CM_WriteLogs(1, "[MV] CM_MediaPlayer_SetPlayWnd bRotate = %d, bZoom = %d, x= %d, y = %d, w= %d, h = %d", bRotate,bZoom,x,y,w,h);   
    
    s_mv_control_info.is_rotate = bRotate;
    s_mv_control_info.display_rect.left = x;
    s_mv_control_info.display_rect.top = y;
	if(w < 1) w = 1;
	if(h < 1) h = 1;
    s_mv_control_info.display_rect.right = x + w - 1;
    s_mv_control_info.display_rect.bottom = y + h - 1;
    
#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_SET_PARAM_REQ,0);
#else
    MV_MediaPlayer_SetPlayWnd();
#endif
	// 2011-06-14	des:用来存储非全屏时的播放区域坐标，当全屏播放时处理并发事件并退出全屏
	if(_FALSE == bRotate)
	{
		g_NormalRect.nLeft = x;
		g_NormalRect.nTop = y;
		g_NormalRect.nWidth = w;
		g_NormalRect.nHeight = h;
	}
	return _TRUE;      
    
}

/*****************************************************************************/
// 	Description : to fill the special rect use the color key
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC _VOID CM_MediaPlayer_FillSpecRect(_INT nX, _INT nY, _INT nWidth, _INT nHeight)
{
    GUI_RECT_T spec_lcd_rect ={0};
//CM_WriteLogs(1, "[MV] [CM_MediaPlayer_FillSpecRect]: nX:%d, nY:%d, nWidth:%d, nHeight:%d", nX, nY, nWidth, nHeight); 

	if(s_is_AllowFillRect)
	{
		if(nWidth > 0 && nHeight > 0)
		{
		    spec_lcd_rect.left = nX;
			spec_lcd_rect.top = nY;
			spec_lcd_rect.right = nX+nWidth + 1;
			spec_lcd_rect.bottom = nY+nHeight + 1;
		} 
		GUI_FillRect(MMITHEME_GetDefaultLcdDev(),spec_lcd_rect,MMIAPIVP_GetTransparenceColor());
	}
//CM_WriteLogs(1, "[MV] [CM_MediaPlayer_FillSpecRect]	change: nX:%d, nY:%d, nWidth:%d, nHeight:%d", nX, nY, nWidth, nHeight); 
}

/*****************************************************************************/
//  Description : CM_MediaPlayer_GetMaxVolume
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_MediaPlayer_GetMaxVolume(_HANDLE handle)
{
	
	return MMISET_VOL_MAX; // 跟handle有关系吗?
}

/*****************************************************************************/
//  Description : CM_MediaPlayer_GetCurVolume
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT  CM_MediaPlayer_GetCurVolume(_HANDLE handle)
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_GetCurVolume s_mv_control_info.vol = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1174_112_2_18_2_39_19_172,(uint8*)"d",s_mv_control_info.vol);
#endif
    if (NULL != handle)
	{
        return s_mv_control_info.vol;
	}	
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : MV_MediaPlayer_UpdateVolume
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_UpdateVolume(void)
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_UpdateVolume nVolume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1193_112_2_18_2_39_19_173,(uint8*)"d", s_mv_control_info.vol);
#endif
	if(PNULL == s_mv_control_info.vp_handle)
		return;
    if(MV_IsSilentMode() && 0 != s_mv_control_info.vol)
    {
        s_mv_control_info.vol = 0;
    }
    MMIAPIVP_FSMUpdateCurrentVol(s_mv_control_info.vp_handle, s_mv_control_info.vol);
}
/*****************************************************************************/
//  Description : CM_MediaPlayer_SetVolume
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_SetVolume(_HANDLE handle,_INT nVolume)
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_SetVolume nVolume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1210_112_2_18_2_39_19_174,(uint8*)"d", nVolume);
#endif
    if (NULL != handle)
    {
        if(MV_IsSilentMode() && 0 != nVolume)
        {
            s_mv_control_info.vol = 0;
        }
        else
        {
            s_mv_control_info.vol = nVolume;
        }
        MMIAPISET_InitAudioDevice(s_mv_control_info.vol);

    #ifdef MV_PLAYER_ASYN_MODE
        CM_SendSignalToMMI(MVIDEO_UPDATE_VOL_REQ,0);
    #else
        MV_MediaPlayer_UpdateVolume();
    #endif
		return _TRUE;
    }
    else
    {
        return _FALSE;
    }
    

}

/*****************************************************************************/
//  Description : this function is used to get current time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_GetCurTime(void)
{
    s_mv_control_info.process_time = MMIAPIVP_FSMGetTimeOffset(s_mv_control_info.vp_handle); 
#ifdef PLAYER_DEBUG
   //MV_TRACE_LOW:"[MV] MV_MediaPlayer_GetCurTime process_time = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1247_112_2_18_2_39_19_175,(uint8*)"d",s_mv_control_info.process_time);
#endif
   if (0 != s_mv_control_info.timer_id)
   {
       MMK_StopTimer(s_mv_control_info.timer_id);
       s_mv_control_info.timer_id = 0;
   }
   s_mv_control_info.timer_id =  MMK_CreateTimerCallback(
                                     300,
                                     MV_PROCESS_TIMER,
                                     PNULL,
                                     FALSE);
    MMK_StartTimerCallback(s_mv_control_info.timer_id,300,MV_PROCESS_TIMER,PNULL,FALSE);
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_MediaPlayer_GetCurTime s_mv_control_info.timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1261_112_2_18_2_39_20_176,(uint8*)"d",s_mv_control_info.timer_id);
#endif  
}

/*****************************************************************************/
//  Description : 处理进度timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void MV_PROCESS_TIMER( uint8 timer_id, uint32 param )
{  
#ifdef MV_PLAYER_ASYN_MODE
    CM_SendSignalToMMI(MVIDEO_GET_CUR_TIME_REQ,0);
#else
    MV_MediaPlayer_GetCurTime();
#endif
}
/*****************************************************************************/
//  Description : this function is used to get current time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_MediaPlayer_GetCurTime(_HANDLE handle)
{
    int32  process_time = 0;

    if (NULL != handle)
    {
        process_time = s_mv_control_info.process_time;        
    }
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_GetCurTime process_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1290_112_2_18_2_39_20_177,(uint8*)"d",process_time);
#endif
    return process_time/1000;   
}

/*****************************************************************************/
//  Description : this function is used to get total time of media file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_MediaPlayer_GetTotalTime(_HANDLE handle)
{
    _INT  total_time = 0;
    if (NULL != handle)
    {
        
        total_time = s_mv_control_info.total_time / 1000;
    }    
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW("[MV] CM_MediaPlayer_GetTotalTime handle = %x, vp_handle = %x, total_time = %d",handle,s_mv_control_info.vp_handle,total_time);
#endif   
    return total_time;
	
}

/*****************************************************************************/
//  Description : CM_MediaPlayer_GetPercent
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_MediaPlayer_GetPercent()
{
    _INT  percent = 0;
    
	percent = MMIAPIVP_FSMGetCurBufPercent(s_mv_control_info.vp_handle);
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_GetPercent percent = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1324_112_2_18_2_39_20_178,(uint8*)"d",percent);
#endif
    
    return percent;
}

/*****************************************************************************/
//  Description : CM_MediaPlayer_GetPercent
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_MediaPlayer_CanBeCreate()
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_CanBeCreate s_mv_control_info.vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1337_112_2_18_2_39_20_179,(uint8*)"d",s_mv_control_info.vp_handle);
#endif

	if (PNULL != s_mv_control_info.vp_handle)
	{
        return FALSE;
	}
	else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description :this function is used to replay a media file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_HANDLE CM_MediaPlayer_RePlay(_HANDLE hPlayer)
{

#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] CM_MediaPlayer_RePlay hPlayer = %x, state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1358_112_2_18_2_39_20_180,(uint8*)"dd",hPlayer, s_mv_control_info.state );
#endif
    if (NULL == hPlayer)
    {
        return PNULL;
    }

    if (VP_STATE_READY != s_mv_control_info.state)
    {
        s_mv_control_info.is_replay = TRUE;         
        CM_MediaPlayer_Stop(hPlayer);
    }
    else
    {
        CM_MediaPlayer_Play(hPlayer);
    }
    return hPlayer;
	
}

/*****************************************************************************/
//  Description :MV_MediaPlayer_LoseFocus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_LoseFocus(void)
{
	#if 1
	CM_WriteLogs(1, "[MV]::MV_MediaPlayer_LoseFocus() isLiving=%d, vedeio_type=%d, s_mv_control_info.vp_handle=%d", MV_IsLiving(), s_mv_control_info.play_type, s_mv_control_info.vp_handle);
    if(PNULL != s_mv_control_info.vp_handle)
    {
		MV_MediaPlayer_Pause();
	    
		s_is_AllowFillRect = FALSE;

		if(_TRUE == s_mv_control_info.is_rotate)
		{
			CM_MediaPlayer_SetPlayWnd((_HANDLE)s_mv_control_info.vp_handle, _FALSE, _FALSE, g_NormalRect.nLeft, g_NormalRect.nTop, g_NormalRect.nWidth, g_NormalRect.nHeight);
		}
		CM_WriteLogs(1, "[MV]::CM_MediaPlayer_Suspend() g_NormalRect=[%d, %d, %d, %d]", g_NormalRect.nLeft, g_NormalRect.nTop, g_NormalRect.nWidth, g_NormalRect.nHeight);
	    #ifndef WIN32
		CM_MediaPlayer_Suspend();
	    #endif
    }
	#else
    MMIAPIVP_FSMCompulsivePause(s_mv_control_info.vp_handle);
    if(PNULL != s_mv_control_info.vp_handle)
    {
        GUIBLOCK_ResetMainType();
        //GUI_FillRect(MMITHEME_GetDefaultLcdDev(),s_mv_control_info.display_rect,MMIAPIVP_GetTransparenceColor());
    }
	#endif
	CM_WriteLogs(1, "[MV]::CM_MediaPlayer_Suspend() finish");
}

/*****************************************************************************/
//  Description :MV_MediaPlayer_GetFocus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_GetFocus(void)
{
	CM_WriteLogs(1, "[MV]::MV_MediaPlayer_GetFocus()");
#if 1
	//此处必须添加判断，否则多次并发容易死机
	if(PNULL != s_mv_control_info.vp_handle)
    {
    #ifndef WIN32
		CM_MediaPlayer_Reactive();
    #endif
    }
#else
    if(PNULL != s_mv_control_info.vp_handle)
    {
        SetBlockType(255);
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(),s_mv_control_info.display_rect,MMIAPIVP_GetTransparenceColor());
    	MMIAPIVP_FSMCompulsiveResume(s_mv_control_info.vp_handle); 
    }
#endif
	CM_WriteLogs(1, "[MV]::CM_MediaPlayer_Reactive() finish");
}

/*****************************************************************************/
//  Description :MV_MediaPlayer_IsFullScreen
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MV_MediaPlayer_IsFullScreen(void)
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW("MV] MV_MediaPlayer_IsFullScreen s_mv_control_info.is_rotate = %d",s_mv_control_info.is_rotate);
#endif
    return s_mv_control_info.is_rotate;
}

/*****************************************************************************/
//  Description :MV_IsPlaying
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MV_IsPlaying(void)
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_IsPlaying s_mv_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1457_112_2_18_2_39_20_181,(uint8*)"d",s_mv_control_info.state);
#endif

    if (VP_STATE_PLAYING == s_mv_control_info.state)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :MV_IsLiving
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MV_IsLiving(void)
{
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_IsLiving state = %d, play_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1477_112_2_18_2_39_20_182,(uint8*)"dd",s_mv_control_info.state,s_mv_control_info.play_type);
#endif
    if(VP_STATE_PLAYING == s_mv_control_info.state
        && PROGRAM_LIVE == s_mv_control_info.play_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :MV_Exit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_Exit(void)
{
    if (PNULL != s_mv_control_info.vp_handle)
    {
        MMIAPIVP_FSMExit(s_mv_control_info.vp_handle);
        s_mv_control_info.vp_handle = PNULL;
    }
}

/*****************************************************************************/
//  Description :MV_Stop
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_Stop(void)
{
	_HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
	//仅当为视频通话的时候采取处理
    if (/*MMIAPIVT_IsVtCalling() && */(PNULL != s_mv_control_info.vp_handle))
    {
    CM_WriteLogs(1, "[MV]	MV_Exit()	videoType = %d", s_mv_control_info.play_type);
        MMIAPIVP_FSMStop(s_mv_control_info.vp_handle);

        //新增处理直播来电或视频通话时的逻辑处理
        //s_mv_control_info.vp_handle = PNULL;	// 此处无需将播放器句柄值NULL
        GUIBLOCK_ResetMainType();
		s_mv_control_info.state = VP_STATE_PAUSING;
		if (PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
        {
            s_mv_control_info.pMediaPlayerCallbackProc(handle, s_mv_control_info.pCallbackParam, PAUSE_VIDEO_OK, PNULL);
        }
    }
}

/*****************************************************************************/
//  Description For other app use, when lose focuse
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_Pause(void)
{
    _HANDLE handle = (_HANDLE)MV_NORMAL_HANDLE;
 
    if(s_mv_control_info.state != VP_STATE_PAUSED)
    {
        s_mv_control_info.state = VP_STATE_PAUSED;
        MMIAPIVP_FSMCompulsivePause(s_mv_control_info.vp_handle);
        g_isSuspend = TRUE;
        if(PNULL != s_mv_control_info.pMediaPlayerCallbackProc)
        {
            s_mv_control_info.pMediaPlayerCallbackProc(handle, s_mv_control_info.pCallbackParam, PAUSE_VIDEO_OK, PNULL);
        }
    }
    GUIBLOCK_ResetMainType();
}

/*****************************************************************************/
// 	Description : environment is or not silent mode
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MV_IsSilentMode(void)
{
    uint8	 envset_mode = 0;
    BOOLEAN  is_silent = FALSE;
    
    //envset(ring,vibration,silence)
    envset_mode = MMIAPIENVSET_GetActiveModeId();
#ifdef PLAYER_DEBUG
    //MV_TRACE_LOW:"[MV] MV_IsSilentMode mode=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PLAYER_API_1560_112_2_18_2_39_20_183,(uint8*)"d", envset_mode);
#endif
    switch(envset_mode)
    {
    case MMIENVSET_SILENT_MODE:     // 安静环境（静音）
    //case MMIENVSET_MEETING_MODE:    // 会议环境（振动）
        is_silent = TRUE;
        break;
    default:
        break;
    }

    return is_silent;
}

/* function:ReSetDisPlayRect()
 * DES:根据屏幕初始值是否选中重新设置播放区域及全屏播放旋转处理
 * AUTHOR:丶Jone
 * DATE:2011.06.23
*/
_VOID MV_ReSetDisplayParam(BOOLEAN is_ratate, _VOID* dis_param)
{	
	VP_DISPLAY_PARAM_T* display_param = (VP_DISPLAY_PARAM_T*)dis_param;
	//初始值以默认的旋转角度
	display_param->RotateAngle = GUILCD_GetInitAngle(GUI_MAIN_LCD_ID);
	
	if(_TRUE == is_ratate)
	{
		//如果初始Angle为0°或180°
		if((GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) == LCD_ANGLE_0) || (GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) == LCD_ANGLE_180))
		{
			display_param->RotateAngle = is_ratate;	// 0: 0°;	 1: 90°;	 2: 180°;  	3: 270°;
			display_param->disp_rect.dy = s_mv_control_info.display_rect.right - s_mv_control_info.display_rect.left + 1;
            display_param->disp_rect.dx = s_mv_control_info.display_rect.bottom - s_mv_control_info.display_rect.top + 1;
		}else
		{
//			s_mv_control_info.display_rect = MMITHEME_GetFullScreenRect();	 //此处传进来的值已经是全屏坐标无需重新获取
			display_param->disp_rect.dy = s_mv_control_info.display_rect.bottom - s_mv_control_info.display_rect.top + 1;
			display_param->disp_rect.dx = s_mv_control_info.display_rect.right - s_mv_control_info.display_rect.left + 1;
		}
	}
	else
	{
		// 设置显示区域坐标
		display_param->disp_rect.dy = s_mv_control_info.display_rect.bottom - s_mv_control_info.display_rect.top + 1;
		display_param->disp_rect.dx = s_mv_control_info.display_rect.right - s_mv_control_info.display_rect.left + 1;
		
		
		if((GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) == LCD_ANGLE_0) || (GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) == LCD_ANGLE_180))
		{
			display_param->RotateAngle = FALSE;	// 0：0°； 1：90°； 2：180°； 3：270°；
		}
	}
	display_param->disp_rect.x = s_mv_control_info.display_rect.left;
	display_param->disp_rect.y = s_mv_control_info.display_rect.top;
	//设置显示位置坐标
	display_param->target_rect = display_param->disp_rect;
}


