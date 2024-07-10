#ifndef _LIBXMLY_API_H_
#define _LIBXMLY_API_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    XY_PLAYER_NONE=0,            //播放器空闲，初始化状态      
    XY_PLAYER_STOP,        		 //播放停止
    XY_PLAYER_PLAY,              //播放
    XY_PLAYER_PAUSE,          	 //暂停
    XY_PLAYER_WAITING,			 //数据加载中
    XY_PLAYER_SUSPEND,           //被系统音频打断，挂起状态
    XY_PLAYER_ERROR,             //播放出错
    XY_PLAYER_MAX, 
}xy_player_statue_e;


typedef enum 
{
    XY_APP_IDLE=0,            	//app初始化状态     
    XY_APP_RUNNING,        		//前台运行
    XY_APP_RUN_BACK,            //后台运行
    XY_APP_EXIT,          	 	//退出
}xy_app_statue_e;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/* APP的入口函数*/
PUBLIC void LIBXMLYAPI_CreateXysdkMainWin(void);

/* APP初始化函数，注册NV*/
PUBLIC void LIBXMLYAPI_InitModule(void);

/* APP退出函数，目前只在App内调用，播放情况下调用退到后台播放*/
PUBLIC void LIBXMLYAPI_AppExit(void);

/*获取播放器状态，返回xy_player_statue_e*/
PUBLIC xy_player_statue_e LIBXMLYAPI_GetAudioPlayerState(void);

/*
获取App是否运行
返回	 	xy_app_statue_e    
*/
PUBLIC xy_app_statue_e LIBXMLYAPI_AppRunningStatue(void);


/*
后台播放的时候暂停播放
返回	 	0:成功  
		-1:失败    
*/
PUBLIC int LIBXMLYAPI_SetAudioPlayerPause(void);

/*
后台播放暂停后恢复播放
返回	 	0:成功  
		-1:失败    
*/
PUBLIC int LIBXMLYAPI_SetAudioPlayerResume(void);

/*
reset时清除缓存到本地的数据

返回	 	  
*/
void LIBXMLYAPI_ClearCacheData(void);

// 刷新首页
PUBLIC void LIBXMLYAPI_RefMainWin(void);
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/



#ifdef _cplusplus
	}
#endif

#endif//_LIBXMLY_API_H_