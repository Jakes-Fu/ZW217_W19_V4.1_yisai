#include "std_header.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#ifndef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_dummy.h"
/****************************************************************************/
//Description : init the streaming controller
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
STRM_HANDLE STRM_Init (
						STRM_INIT_PARAM_T	*init_param_ptr //in: init parameter
							)
{
    return PNULL;
}

/****************************************************************************/
//Description : streaming PLAY request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Play (
						STRM_HANDLE     strm_handler,   //in: streaming handler
						DPLAYER_HANDLE	player_handler  //in: player handler
							)
{
    return FALSE;
}

/****************************************************************************/
//Description : streaming PAUSE request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Pause (
						STRM_HANDLE strm_handler    //in: streaming handler
							)
{
    return FALSE;
}

/****************************************************************************/
//Description : streaming SEEK request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Seek (
						STRM_HANDLE         strm_handler,       //in: streaming handler
						STRM_SEEK_PARAM_T  *seek_param_ptr      //in: seek parameters
							)
{
    return FALSE;
}

/****************************************************************************/
//Description : streaming RESUME request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Resume(
						STRM_HANDLE 	    strm_handler,    //in: streaming handler
						STRM_RESUME_PARAM_T	*param_ptr      //in: resume parameters
							)
{
    return FALSE;
}

/****************************************************************************/
//Description : streaming STOP request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Stop (
						STRM_HANDLE strm_handler    //in: streaming handler
							)
{
    return FALSE;
}

/****************************************************************************/
//Description : streaming CLOSE request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Close (
						STRM_HANDLE strm_handler    //in: streaming handler
							)
{
    return FALSE;
}
/****************************************************************************/
//Description : streaming describe auth rsp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN STRM_DescribeAuthRsp(
						STRM_HANDLE 			strm_handler,
						const STRM_AUTH_INFO_T	*auth_info_ptr
							)
{
    return FALSE;
}
/****************************************************************************/
//Description : streaming setup auth rsp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN STRM_SetupAuthRsp(
						STRM_HANDLE 			strm_handler,
						const STRM_AUTH_INFO_T	*auth_info_ptr
							)
{
    return FALSE;
}
#endif
#else 
#include "mmk_app.h"

/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Play (
                VP_PLAY_PARAM_T  *play_param_ptr,
                VP_CALLBACK        call_back_func,   
                 uint32                    start_time
                )
{
    
    return FALSE;
}

/*****************************************************************************/
//  Description : 退出播放视频文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitPlay(void)
{
    
}

/*****************************************************************************/
//  Description : init vp module
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_InitModule(void)
{
}

/*****************************************************************************/
//  Description : init vp
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_Init(void)
{
}
#endif


