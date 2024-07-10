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
    XY_PLAYER_NONE=0,            //���������У���ʼ��״̬      
    XY_PLAYER_STOP,        		 //����ֹͣ
    XY_PLAYER_PLAY,              //����
    XY_PLAYER_PAUSE,          	 //��ͣ
    XY_PLAYER_WAITING,			 //���ݼ�����
    XY_PLAYER_SUSPEND,           //��ϵͳ��Ƶ��ϣ�����״̬
    XY_PLAYER_ERROR,             //���ų���
    XY_PLAYER_MAX, 
}xy_player_statue_e;


typedef enum 
{
    XY_APP_IDLE=0,            	//app��ʼ��״̬     
    XY_APP_RUNNING,        		//ǰ̨����
    XY_APP_RUN_BACK,            //��̨����
    XY_APP_EXIT,          	 	//�˳�
}xy_app_statue_e;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/* APP����ں���*/
PUBLIC void LIBXMLYAPI_CreateXysdkMainWin(void);

/* APP��ʼ��������ע��NV*/
PUBLIC void LIBXMLYAPI_InitModule(void);

/* APP�˳�������Ŀǰֻ��App�ڵ��ã���������µ����˵���̨����*/
PUBLIC void LIBXMLYAPI_AppExit(void);

/*��ȡ������״̬������xy_player_statue_e*/
PUBLIC xy_player_statue_e LIBXMLYAPI_GetAudioPlayerState(void);

/*
��ȡApp�Ƿ�����
����	 	xy_app_statue_e    
*/
PUBLIC xy_app_statue_e LIBXMLYAPI_AppRunningStatue(void);


/*
��̨���ŵ�ʱ����ͣ����
����	 	0:�ɹ�  
		-1:ʧ��    
*/
PUBLIC int LIBXMLYAPI_SetAudioPlayerPause(void);

/*
��̨������ͣ��ָ�����
����	 	0:�ɹ�  
		-1:ʧ��    
*/
PUBLIC int LIBXMLYAPI_SetAudioPlayerResume(void);

/*
resetʱ������浽���ص�����

����	 	  
*/
void LIBXMLYAPI_ClearCacheData(void);

// ˢ����ҳ
PUBLIC void LIBXMLYAPI_RefMainWin(void);
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/



#ifdef _cplusplus
	}
#endif

#endif//_LIBXMLY_API_H_