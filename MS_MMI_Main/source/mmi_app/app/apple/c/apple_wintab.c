
#ifdef SXH_APPLE_SUPPORT

#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guibutton.h"
//#include "guitextbox.h"    //liuhongmou
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
 
#include "tb_dal.h"
// #include "mmiacc_winmain.h"
// #include "mmialarm.h"
// #include "mmigame.h"
#include "mmi_textfun.h"
// #include "mmiphone.h"
#include "mmiphone_export.h"

#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipub.h"

#include "mmi_default.h"

#include "apple_id.h"

#include "mmi_appmsg.h"
#include "apple_smd_um.h"
#include "mmi_menutable.h"
#ifdef MMI_WIFI_SUPPORT  //IMPORTANT
#include "mmiwifi_export.h"
extern int APPLE_GetIsUseWifi();
#endif

#include "mmiset_export.h"
#include "mmipb_export.h"
#include "mmistk_export.h"

#include "OEMOS.h"

// extern 	BOOLEAN MMIAPIPB_IsPbReady(void); //RETURN: TRUE, 可以使用
// #define APPLE_LOG

#ifdef SXH_APPLE_YOUXIN	
// add jxh 2012-3-26
extern 	uint32 g_apple_ps_mode; 
extern 	void sxmvc_call_incoming_lsk(void);
extern  unsigned int sxmvc_sms_appid;
extern char sxmvc_sms_arg[100];
extern unsigned int sxmvc_sms_offline;
//unsigned int sxmvc_sms_offline=1;
#endif


extern  void MMIPUBDisplayWin(
								 MMIPUB_INFO_T	  *win_info_ptr
								 );

extern MMI_RESULT_E HandleAppleWinMsg(
									  MMI_WIN_ID_T win_id, 
									  MMI_MESSAGE_ID_E msg_id, 
									  DPARAM param
							   );

LOCAL MMI_RESULT_E HandleEditBoxWinMsg(
   MMI_WIN_ID_T        win_id, 
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   );

LOCAL MMI_RESULT_E HandleAppleMenuWinMsg(
   MMI_WIN_ID_T        win_id, 
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
	);

LOCAL MMI_RESULT_E HandleTESTWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									);
#ifdef __UM_DUAL_SIM__
LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(
                                         MMI_WIN_ID_T		win_id, 
                                         MMI_MESSAGE_ID_E	msg_id, 
                                         DPARAM				param
                                         );



typedef  void (*AppleStartCB) (void );
// extern int32 g_apple_active_sim;
AppleStartCB g_FunCB = NULL;

BOOLEAN g_apple_is_select_sim = FALSE; 
#endif
extern void APPLE_StartEx(unsigned int appid, char * arg);
WINDOW_TABLE( TEST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleTESTWinMsg ),    
    WIN_ID( APPLE_SMD_WIN_ID ),
    //WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN), //禁止3D旋转
    WIN_TITLE( TXT_NULL ),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
//    WIN_TITLE( TXT_ACC_CALCULATOR ),
//	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
}; 

WINDOW_TABLE( APPLE_MAIN_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAppleWinMsg ),    
    WIN_ID( APPLE_MAIN_WIN_ID ),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN), //禁止3D旋转
   WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
//         WIN_TITLE(TXT_NULL),
// 		WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#ifdef __UM_DUAL_SIM__
//显示使用SIM卡拨出的选项                                                
WINDOW_TABLE( APPLE_SELECT_SIM_WIN_TAB ) = 
{
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleSIMSelectWinMsg),    
        WIN_ID(APPLE_SELECT_SIM_WIN_ID),
       // WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
        WIN_MOVE_STYLE(MOVE_FORBIDDEN), //禁止3D旋转
        WIN_TITLE(TXT_SIM_SEL),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIAPPLE_SELECT_SIM_CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

static uint32 s_startAppId = 0 ;


uint32  Apple_GetCurStartAPPID()
{
	return s_startAppId;
}


SCI_TIMER_PTR g_apple_init_timer = NULL;
#define APPLE_INIT_TIMER  1000

#define APPLE_MAX_TIEM   30

int  g_apple_times = 0;

extern void APPLE_Init_SetTimer( uint32 nMSecs);

void APPLE_Init_TimerCB(uint32 param)
 {

 	g_apple_times ++;

	if(g_apple_times >= APPLE_MAX_TIEM)
		{
			MMK_PostMsg(APPLE_WAIT_WIN_ID,MSG_APPLE_INIT,PNULL,PNULL);
			return;
		}
	
	if(MMIAPIPB_IsPbReady())
	{
		MMK_PostMsg(APPLE_WAIT_WIN_ID,MSG_APPLE_INIT,PNULL,PNULL);
	}
	else
	{
		APPLE_Init_SetTimer(APPLE_INIT_TIMER);
	}
 }


void APPLE_Init_SetTimer( uint32 nMSecs)
{

//	if(g_aud_timer_state == 1)
//  		 return;
	g_apple_times = 0;
	SCI_ChangeTimer((SCI_TIMER_PTR)g_apple_init_timer, APPLE_Init_TimerCB, nMSecs);
	SCI_ActiveTimer((SCI_TIMER_PTR)g_apple_init_timer);
//	g_aud_timer_state = 1;

}

void APPLE_Init_StopTimer(void)
{

	if(NULL != g_apple_init_timer)
	{
		SCI_DeactiveTimer(g_apple_init_timer);
	}



}

void APPLE_Init_TimerUnInit(void)
{
	
	g_apple_times = 0;
	if(NULL!=g_apple_init_timer)
	{
		SCI_DeactiveTimer(g_apple_init_timer);
		SCI_DeleteTimer(g_apple_init_timer);
	}
	g_apple_init_timer = NULL;

}


void APPLE_Init_TimerInit(void)
{

	
	if(NULL != g_apple_init_timer)
		APPLE_Init_TimerUnInit();
	
	g_apple_init_timer = SCI_CreateTimer(NULL, APPLE_Init_TimerCB, NULL, APPLE_INIT_TIMER, FALSE);
	APPLE_Init_SetTimer(APPLE_INIT_TIMER);

	
	
}

//#define SXH_APPLE_GPRS
#ifdef  SXH_APPLE_GPRS
/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/

MMI_RESULT_E HandleAppleGPRSWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    )
{

   MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
        case MSG_TIMER:
        //                 recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
              break;
        case MSG_APP_WEB:
        case MSG_APP_OK:
        case MSG_CTL_OK:

          MMK_CreateWin((uint32 *)APPLE_MAIN_WIN_TAB,PNULL); 

	      MMK_CloseWin( win_id );	
            break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

  
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);//sxh az
        break;
    }
    
    return (recode);

}

void Apple_CreateMainWindow_GPRS()
{
	MMI_STRING_T		apple_str = {0}; 
	//MMI_WIN_ID_T win_id = MMIAPPLE_GPRS_WIN_ID;
	wchar TXT_LINK_INTERNET_QUERY[]={ 0x6B64, 0x5E94, 0x7528, 0x5C06, 0x4EA7, 0x751F, 0x0067, 0x0070, 0x0072, 0x0073, 0x6D41, 
	0x91CF, 0xFF0C, 0x8BF7, 0x786E, 0x8BA4, 0x662F, 0x5426, 0x4F7F, 0x7528, 0xFF1F, 0 }; 
	//此应用将产生gprs流量，请确认是否使用？
	wchar TXT_LINK_INTERNET_QUERY_ENG[]={ 0x0054, 0x0068, 0x0069, 0x0073, 0x0020, 0x0061, 0x0070, 0x0070, 0x006C, 0x0069, 
	0x0063, 0x0061, 0x0074, 0x0069, 0x006F, 0x006E, 0x0020, 0x0077, 0x0069, 0x006C, 0x006C, 0x0020, 0x0068, 0x0061, 0x0076, 0x0065, 
	0x0020, 0x0061, 0x0020, 0x0067, 0x0070, 0x0072, 0x0073, 0x0020, 0x0074, 0x0072, 0x0061, 0x0066, 0x0066, 0x0069, 0x0063, 0x002C, 
	0x0020, 0x006D, 0x0061, 0x006B, 0x0065, 0x0020, 0x0073, 0x0075, 0x0072, 0x0065, 0x0020, 0x0079, 0x006F, 0x0075, 0x0020, 0x0075, 
	0x0073, 0x0065, 0x003F, 0 }; //This application will have a gprs traffic, make sure you use?
	if(OEMOS_GetLangId()==86)
		{
			apple_str.wstr_ptr=TXT_LINK_INTERNET_QUERY;
			apple_str.wstr_len=MMIAPICOM_Wstrlen(TXT_LINK_INTERNET_QUERY);
		}
	else
		{
			apple_str.wstr_ptr=TXT_LINK_INTERNET_QUERY_ENG;
			apple_str.wstr_len=MMIAPICOM_Wstrlen(TXT_LINK_INTERNET_QUERY_ENG);
		}
	//MMI_GetLabelTextByLang(TXT_SXH_APPLE, &apple_str);//使用字串资源
	MMIPUB_OpenAlertWinByTextPtr(NULL,&apple_str,NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleAppleGPRSWin);   

                                                      
}

#endif
extern uint32 iAppId;
#ifdef APPLE_ROTATE_DYNAMIC 
extern int Apple_AppIsEnableRotate(int appid);
#endif

void Apple_CreateMainWindow()
{
	int support_angle = WIN_SUPPORT_ANGLE_0 ;

	int curAng = MMK_GetScreenAngle();


#ifdef MMI_WIFI_SUPPORT //IMPORTANT
#ifdef APPLE_LOG
    OEMOS_dbgprintf("Apple_CreateMainWindow APPLE_GetIsUseWifi():%d,MMIAPIWIFI_GetStatus():0x%x",APPLE_GetIsUseWifi,MMIAPIWIFI_GetStatus());
#endif
    if(APPLE_GetIsUseWifi()  &&  
        MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus()
        &&(Apple_GetCurStartAPPID() != 0x0300009c  && Apple_GetCurStartAPPID() !=  0x03000042) //解霸与演示厅不需要联网
        )
    {
        extern int32 OEMNET_WifiActive(void);
        OEMNET_WifiActive();
        return ;
    }
#endif
#ifdef  SXH_APPLE_GPRS//流量提示
	if(Apple_GetCurStartAPPID() != 0x0300009c  && Apple_GetCurStartAPPID() !=  0x03000042)
		{
			Apple_CreateMainWindow_GPRS();
		}
	else
		{
				MMK_CreateWin((uint32 *)APPLE_MAIN_WIN_TAB,PNULL); 
		}
#else
	MMK_CreateWin((uint32 *)APPLE_MAIN_WIN_TAB,PNULL); 
#endif
	
#ifdef APPLE_LOG
	OEMOS_dbgprintf("Apple_CreateMainWindow curAng:%d,iAppId:0x%x",curAng,iAppId);
#endif

	//init the apple lcd angle
    switch (curAng)
    {
    case LCD_ANGLE_0:
        support_angle = WIN_SUPPORT_ANGLE_0;
        break;
    case LCD_ANGLE_90:
        support_angle = WIN_SUPPORT_ANGLE_90;
        break;
    case LCD_ANGLE_180:
        support_angle = WIN_SUPPORT_ANGLE_180;
        break;
    case LCD_ANGLE_270:
        support_angle = WIN_SUPPORT_ANGLE_270;
        break;
    default:
        support_angle = WIN_SUPPORT_ANGLE_0;
        break;
    }



#if 0
#ifdef APPLE_ROTATE_DYNAMIC   //如果是VP 或VC就设置成那个不能转屏，如果都不支持动态转就都设置成只支持一个角度
	#if 1
		if(  (0x0300009a != iAppId ) && (0x03000041!=iAppId))
		{
			OEMOS_dbgprintf("Apple_CreateMainWindow   SET NO SUPORT ROTATE   support_angle:%d",support_angle);	
			MMK_SetWinSupportAngle(APPLE_MAIN_WIN_ID, support_angle);
			MMK_SetWinAngle(APPLE_MAIN_WIN_ID, curAng, FALSE);
		}

	#else
		if( !Apple_AppIsEnableRotate(iAppId))
		{
			OEMOS_dbgprintf(" SET NO SUPORT ROTATE ");
			
			MMK_SetWinAngle(APPLE_MAIN_WIN_ID, LCD_ANGLE_0, FALSE);
			MMK_SetWinSupportAngle(APPLE_MAIN_WIN_ID,WIN_SUPPORT_ANGLE_0);
			
		}
	#endif
#else 
		OEMOS_dbgprintf("Apple_CreateMainWindow   SET NO SUPORT ROTATE   support_angle:%d",support_angle);	
		MMK_SetWinSupportAngle(APPLE_MAIN_WIN_ID, support_angle);
		MMK_SetWinAngle(APPLE_MAIN_WIN_ID, curAng, FALSE);
	
#endif
#endif
}


LOCAL MMI_RESULT_E  APPLE_HandleNormalWaitingWin(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E				recode = MMI_RESULT_TRUE;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    case MSG_APP_RED:   // 不响应红键
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:// added by rui xu cr149256
           MMK_CloseWin(win_id);
         break;
#ifdef FLIP_PHONE_SUPPORT		 
    case MSG_APP_FLIP://翻盖不处理
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
   
    case MSG_FULL_PAINT:
        recode = MMIPUB_HandleWaitWinMsg( (MMI_WIN_ID_T)win_id, msg_id, param);
        break;
        
    case MSG_CLOSE_WINDOW:
	 APPLE_Init_TimerUnInit();
        recode = MMIPUB_HandleWaitWinMsg( (MMI_WIN_ID_T)win_id, msg_id, param);
        break;
    case MSG_APPLE_INIT:
        {           
		Apple_CreateMainWindow();
		   MMK_CloseWin(win_id);
        
                                       
        }            
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break; 
    }
    return(recode);
}


void  APPLE_CreateNormalWaitingWin(void)
{
    MMI_STRING_T initing_str = {0};
    
    SCI_MEMSET(&initing_str, 0, sizeof(initing_str));
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &initing_str);
    
    MMIPUB_OpenWaitWin(1,&initing_str,PNULL,PNULL,APPLE_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)APPLE_HandleNormalWaitingWin);
}


extern void AppleStartApp (unsigned int iAppId, char* pArgs);
extern MMI_RESULT_E OEMOS_HandleAppleWinMsg(
							   MMI_WIN_ID_T win_id, 
							   MMI_MESSAGE_ID_E msg_id, 
							   DPARAM param
							   );
#define APP_ARG_SIZE 100
uint32 iAppId = 0;
static char strAppArg[APP_ARG_SIZE] = {0};

MMI_RESULT_E HandleAppleWinMsg(
							   MMI_WIN_ID_T win_id, 
							   MMI_MESSAGE_ID_E msg_id, 
							   DPARAM param
							   )
{
	MMI_RESULT_E result = OEMOS_HandleAppleWinMsg(win_id, msg_id, param);
	if(!result)
		{
			return result;
		}

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		if(strlen(strAppArg) != 0)
			AppleStartApp(iAppId, strAppArg);
		else
			AppleStartApp(iAppId, NULL);
 
		break;
	default:
		break;
	}
	return result;
}

// AppleStartCB applestartcb = NULL;
#ifdef __UM_DUAL_SIM__
int APPLE_Select_Sim(AppleStartCB applestartcb )
{


#if 0
	BOOLEAN is_sim1_ok = FALSE;
   	BOOLEAN is_sim2_ok = FALSE;
#ifdef MULTI_SIM_SYS_TRI
         BOOLEAN is_sim3_ok = FALSE;
#endif
	
	
	g_FunCB = applestartcb;
#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_Select_Sim, set:%d, sim1:%d, sim2:%d", g_apple_active_sim, is_sim1_ok, is_sim2_ok);
#endif

//	if(g_apple_active_sim != -1)
//		return 0;
       if(g_apple_is_select_sim)
	   	return 0;
	
	MMIPHONE_GetDualSimStatus(&is_sim1_ok, &is_sim2_ok);
#ifdef MULTI_SIM_SYS_TRI
		is_sim3_ok = MMIPHONE_IsSimOk(MN_DUAL_SYS_3);
#endif

	if((is_sim1_ok && is_sim2_ok)
	#ifdef MULTI_SIM_SYS_TRI
	    ||(is_sim1_ok && is_sim3_ok)
	    || (is_sim2_ok && is_sim3_ok)
	    || (is_sim2_ok && is_sim3_ok&& is_sim1_ok)
	#endif
		)
	{

		MN_PHONE_PLMN_STATUS_E plmn_status1 = PLMN_NO_SERVICE;
		MN_PHONE_PLMN_STATUS_E plmn_status2 = PLMN_NO_SERVICE;

#ifdef MULTI_SIM_SYS_TRI
		MN_PHONE_PLMN_STATUS_E plmn_status3 = PLMN_NO_SERVICE;
#endif
    
    		plmn_status1 = MMIPHONE_GetPlmnStatus(MN_DUAL_SYS_1);
		plmn_status2 = MMIPHONE_GetPlmnStatus(MN_DUAL_SYS_2);

#ifdef MULTI_SIM_SYS_TRI
		 plmn_status3 = MMIPHONE_GetPlmnStatus(MN_DUAL_SYS_3);
#endif

#ifdef APPLE_LOG
		OEMOS_dbgprintf("APPLE_Select_Sim, plmn_status1:%d, plmn_status2:%d",  plmn_status1, plmn_status2);
#endif
		if((PLMN_NO_SERVICE != plmn_status1 && PLMN_NO_SERVICE !=  plmn_status2)
#ifdef MULTI_SIM_SYS_TRI
                   || (PLMN_NO_SERVICE != plmn_status1 && PLMN_NO_SERVICE !=  plmn_status3)
                   || (PLMN_NO_SERVICE != plmn_status2 && PLMN_NO_SERVICE !=  plmn_status3)
                   ||(PLMN_NO_SERVICE != plmn_status1 && PLMN_NO_SERVICE !=  plmn_status2 && PLMN_NO_SERVICE !=  plmn_status3)
#endif
		)
			{
				MMK_CreateWin(APPLE_SELECT_SIM_WIN_TAB, NULL);
				  return 1;
			}
	} 

//onlysim
	else
		{
		    MMI_STRING_T            no_sim_text = {0};
			wchar no_sim_text_buf[] = {0x53EA, 0x6709, 0x4E00, 0x5F20, 0x0053, 0x0069, 0x006D, 0x5361, 0};
			no_sim_text.wstr_ptr = no_sim_text_buf;
			no_sim_text.wstr_len = sizeof(no_sim_text_buf)/sizeof(wchar);
			MMIPUB_OpenAlertWinByTextPtr(PNULL,&no_sim_text,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
			
		}
//onlysim
#else

	uint16  sim_state_arr[MMI_DUAL_SYS_MAX] = {0};
	uint32  sim_num = 0;
	uint32  i = 0;

	g_FunCB = applestartcb;

#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_Select_Sim  g_apple_is_select_sim:%d",g_apple_is_select_sim);
#endif

	
	if(g_apple_is_select_sim)
	   	return 0;
	

	for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
	{
		sim_state_arr[i] = MN_DUAL_SYS_MAX; 
		if (MMIPHONE_IsSimOk(i)  && (PLMN_NO_SERVICE != MMIPHONE_GetPlmnStatus(i)))  
		{
			sim_state_arr[i] = i;
			sim_num++;
		}
	}		
#ifdef MMI_WIFI_SUPPORT
	//sim_num++;
#endif
	//sxh az add test
#ifndef MMI_WIFI_SUPPORT
if((Apple_GetCurStartAPPID()==0x03000000||OEMOS_GetLangId()!=86)&&(sim_num == 1))
{
	s_startAppId=0;
	return 0;
}
#endif
if(MMIAPISET_GetFlyMode()) // 飞行模式
	{
			    MMIAPISET_AlertFlyMode();
	}
else
	{
	if(sim_num>=1)
	//sxh az add test
/*

	if( ((sim_num == 1)&&( Apple_GetCurStartAPPID() != 0x03000000) )// sxh az 20110730  //:2   
	
        ||   (sim_num >= 2) 
#ifdef WIFI_SUPPORT
	||   (sim_num == 1) 
#endif
	)
*/
	{
#ifdef APPLE_LOG
		OEMOS_dbgprintf("APPLE_Select_Sim   CreateWin  APPLE_SELECT_SIM_WIN_TAB");
#endif
		MMK_CreateWin(APPLE_SELECT_SIM_WIN_TAB, NULL);
		return 1;
	}
	else
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SIM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	}
	}
	
#endif

     return 0;
}
#endif

 void APPLE_StartByMsg();
/*****************************************************************************/
//     Description : to handle the new message window
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
MMI_RESULT_E HandleAppleByMsgWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    )
{

// #ifdef SXH_APPLE_3GVEDIO_SUPPORT
   MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = (MMIPUB_INFO_T *)MMK_GetWinAddDataPtr(win_id);
	
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
          	// SetParentWinSoftkey(win_id);
            break;

        case MSG_FULL_PAINT:
		 //display pubwin
	       MMIPUBDisplayWin(win_info_ptr);
          
        //    recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;

        case MSG_TIMER:
        //                 recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
              break;

        case MSG_APP_WEB:
        case MSG_APP_OK:
        case MSG_CTL_OK:
#ifdef SXH_APPLE_YOUXIN	
 //           APPLE_StartByMsg();
 
 // add jxh 2012-3-26
			sxmvc_call_incoming_lsk();
#endif
	      MMK_CloseWin( win_id );	
            break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    

    case MSG_CLOSE_WINDOW:
       

 //       recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_LOSE_FOCUS:
       
        break;

    default:
    //    recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
// #else 
//    return MMI_RESULT_FALSE;
// #endif // SXH_APPLE_3GVEDIO_SUPPORT
}

// add jxh 2012-3-26
#ifdef SXH_APPLE_YOUXIN	
static char g_phonenumber[256] = {0};
static int g_phonenumber_length = 0;
MMI_STRING_T g_pname_t = {0};

uint32  sxmvc_call_timer_id = 0;

void  sxmvc_call_incoming_entry2();

void sxmvc_call_incoming_entry(char * arg, char *pn, MMI_STRING_T * pname_t)
{
	 int is_ansii = 1;

	OEMDisplay_BackLight(2);

 	if((strlen(pn)>=2) && (pn[0]==0))
	{
		is_ansii = 0;
	}

	if(is_ansii)
	{
		strcpy(g_phonenumber, pn);
	}
	else
	{
		MMIAPICOM_WstrToStr((wchar*)pn, (char*)g_phonenumber);
	}

	if(!pname_t && pname_t->wstr_len>0)
		{
			g_pname_t.wstr_ptr = OEMHEAP_GlobalMalloc(pname_t->wstr_len+1);
			MMIAPICOM_Wstrncpy(g_pname_t.wstr_ptr,pname_t->wstr_ptr,pname_t->wstr_len);
			g_pname_t.wstr_len = pname_t->wstr_len;
		}
	
	g_phonenumber_length = strlen(pn);

	apple_start_timer2(100,&sxmvc_call_timer_id,(void*)sxmvc_call_incoming_entry2);

	g_apple_ps_mode = 2;
}



void sxmvc_call_incoming_lsk(void)
{

	if(sxmvc_sms_appid==0x03000302)
	{
		if(AppleIsRun())
		{
			OEMOS_dbgprintf("APPLE IS RUNNING");
			AppleCloseWin();
		}
		OEMOS_dbgprintf("START YX");
		// AppleStartApp(0x03000302, NULL);
		iAppId = sxmvc_sms_appid;
		// strcpy(strAppArg, sxmvc_sms_arg);
		memset(strAppArg,0x0,APP_ARG_SIZE);
		APPLE_StartByMsg();
		return;
	}

	if((APPLE_GetCurrAppID() ==0x0300009a) || 
		(APPLE_GetCurrAppID() ==0x0300009E))
	{
		OEMOS_dbgprintf("VC IS RUNNING");
//		AppleStartApp2(sxmvc_sms_appid, sxmvc_sms_arg);  ask jungle
	}
	else
	{
		if(AppleIsRun())
		{
			OEMOS_dbgprintf("APPLE IS RUNNING");
			AppleCloseWin();
		}
		OEMOS_dbgprintf("START VC");
		iAppId = sxmvc_sms_appid;
		strcpy(strAppArg, sxmvc_sms_arg);
		APPLE_StartByMsg();
	}
}


void  sxmvc_call_incoming_entry2()
{
	uint16  scn_id;
	uint16 *ucs2_pn;
	char* pb_name = 0;
	char line_feed[] = {'\n', 0, 0, 0};

	 int is_ansii = 1;
	 uint16 temp[64] = {0};

	 char *phonenumber = g_phonenumber;

	  int phonenumber_length = g_phonenumber_length;

	//apple_stop_timer2(&sxmvc_call_timer_id);
	
	ucs2_pn  = OEMHEAP_GlobalMalloc(sizeof(uint16)*100);

	if(!ucs2_pn)
		{
		apple_stop_timer2(&sxmvc_call_timer_id);
		return;
		}

	memset(ucs2_pn, 0, sizeof(uint16)*100);

	if((phonenumber_length>=2) && (phonenumber[0]==0))
	{
		is_ansii = 0;
	}

	if(is_ansii)
	{
 		if(*phonenumber == '+')
 	  	{
      	 		phonenumber++;
    		}

    		if(phonenumber[0] == '8' && phonenumber[1] == '6')
    		{
			phonenumber += 2;
		}

		MMIAPICOM_StrToWstr((char*)phonenumber,(wchar *) ucs2_pn);
	}
	else
	{
    		if(*phonenumber == '+')
    		{
        		phonenumber+=2;
    		}

    		if(phonenumber[1] == '8' && phonenumber[3] == '6')
    		{
        		phonenumber += 4;
    		}

		MMIAPICOM_Wstrcpy((wchar*)ucs2_pn, (wchar*)phonenumber);
	}

#if defined(SXH_APPLE_3GVEDIO_SUPPORT)	
		if(sxmvc_sms_appid == 0x0300009E)
		{
			char *temp= OEMHEAP_GlobalMalloc(100);
			if(temp)
			{
				memset(temp,0,100);
				strcat(sxmvc_sms_arg,"|");
				OEM_WStrToUTF8(ucs2_pn, MMIAPICOM_Wstrlen((const wchar* )ucs2_pn),temp,100);
				strcat(sxmvc_sms_arg,temp);
				OEMHEAP_GlobalFree(temp);
			}
		}
#endif


	if(g_pname_t.wstr_ptr && g_pname_t.wstr_len)
		MMIAPICOM_Wstrcpy((wchar*)ucs2_pn, (wchar*)g_pname_t.wstr_ptr);
	
	MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)line_feed);

	if((sxmvc_sms_appid == 0x0300009a) || 
		(sxmvc_sms_appid == 0x0300009E) ||
		(sxmvc_sms_appid == 0x03000302 ))
	{
	
	if(AppleIsRun())
	{
		OEMDisplay_BackLight(2);
		apple_start_timer2(100,&sxmvc_call_timer_id, sxmvc_call_incoming_lsk);
	}
	else
	{	
		 MMI_STRING_T        prompt_str = {0};
		 MMI_WIN_ID_T        alert_win_id = APPLE_MSGPOP_WIN_ID;
		 MMI_WIN_PRIORITY_E  win_priority = WIN_TWO_LEVEL;

		if(sxmvc_sms_appid == 0x03000302 )
		{

#ifdef APPLE_LOG
			OEMOS_dbgprintf("sxmvc_call_incoming_entry2 sxmvc_sms_offline %d", sxmvc_sms_offline);
#endif			
			if(sxmvc_sms_offline==0)
				{
			const uint16 U_incomingvideocall[] =   { 0x60A8,  0x7684, 0x597D, 0x670B, 0x53CB,0};
			const uint16 U_incomingvideocall_2[] =   {0x9080, 0x8BF7, 0x60A8, 0x4F7F, 0x7528, 0x60A0, 0x4FE1, 0x514D, 0x8D39, 0x804A, 0x5929, 0xFF0C, 0x662F, 0x5426, 0x63A5, 0x53D7, 0x9080, 0x8BF7, 0xFF1F, 0 }; //您好朋友XXX邀请您使用悠信免费聊天，是否接受邀请？

			MMIAPICOM_Wstrcpy((wchar*)temp, (wchar*)ucs2_pn);
			MMIAPICOM_Wstrcpy((wchar*)ucs2_pn, (wchar*)U_incomingvideocall);
			MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)temp);
			MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)U_incomingvideocall_2);
				}
			else
				{
			const uint16 U_incomingvideocall[] =    { 0x60A8, 0x7684, 0x597D, 0x53CB,0}; 
			const uint16 U_incomingvideocall_2[] =   {0x901A, 0x8FC7, 0x60A0, 0x4FE1, 0x53D1, 0x6765, 0x4E00, 0x6BB5, 0x8BED, 0x97F3, 0x4FE1, 0x606F, 0x002F, 0x6587, 0x5B57, 0x4FE1, 0x606F, 0xFF0C, 0x5FEB, 0x53BB, 0x67E5, 0x770B, 0x5427, 0xFF01, 0 }; //您的好友XXX通过悠信发来一段语音信息/文字信息，快去查看吧！
			
			MMIAPICOM_Wstrcpy((wchar*)temp, (wchar*)ucs2_pn);
			MMIAPICOM_Wstrcpy((wchar*)ucs2_pn, (wchar*)U_incomingvideocall);
			MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)temp);
			MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)U_incomingvideocall_2);
				}
		}
		else
		{
			if(OEMOS_GetLangId()==86)
			{
				const uint16 U_incomingvideocall[] =   { 0x9080, 0x8BF7, 0x89C6, 0x9891, 0x804A, 0x5929, 0 }; //邀请视频聊天
				MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)U_incomingvideocall);
			}
			else
			{
				MMIAPICOM_Wstrcat((wchar*)ucs2_pn, (wchar*)L"Incoming video call ");
			}
		}

			prompt_str.wstr_len = MMIAPICOM_Wstrlen((wchar*)ucs2_pn) + 2;
			prompt_str.wstr_ptr = OEMHEAP_GlobalMalloc(prompt_str.wstr_len);

			if(prompt_str.wstr_ptr == NULL)
				{
				apple_stop_timer2(&sxmvc_call_timer_id);
				return;
				}
			MMIAPICOM_Wstrcpy(prompt_str.wstr_ptr,(wchar*)ucs2_pn);
			OEMHEAP_GlobalFree(ucs2_pn);
		
	MMIPUB_OpenAlertWinByTextPtr(PNULL,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,
		MMIPUB_SOFTKEY_OKCANCEL,HandleAppleByMsgWin); 

			if(NULL !=  prompt_str.wstr_ptr)
				OEMHEAP_GlobalFree(prompt_str.wstr_ptr);

	
	}

	}

	apple_stop_timer2(&sxmvc_call_timer_id);

}

#else
// #define APPLEDISPLAYMSG   "接受%s  视频聊天"
void sxmvc_call_incoming_entry(unsigned int appid, char * arg, char *pn, MMI_STRING_T * pname_t)
{

// #ifdef SXH_APPLE_3GVEDIO_SUPPORT
if(AppleIsRun())
{
	iAppId = appid;
	strcpy(strAppArg, arg);

#ifdef APPLE_LOG
	OEMOS_dbgprintf("sxmvc_call_incoming_entry:AppleIsRun \r\n");
#endif	
	if(iAppId == 0x0300009E)
	{
		int n = 0;
		strcat(strAppArg,"|");

		n = strlen(pn) - 11;
		strcat(strAppArg,pn+n);
#ifdef APPLE_LOG
		OEMOS_dbgprintf("sxmvc_call_incoming_entry:%d sxmvc_sms_arg:%s \r\n",iAppId,strAppArg);
#endif
	}

	 AppleStartApp(iAppId, strAppArg);
	// APPLE_StartByMsg();
}
else
{
   	 MMI_STRING_T        prompt_str = {0};
	 MMI_WIN_ID_T        alert_win_id = APPLE_MSGPOP_WIN_ID;
//	 char   displaymsg[64] = {0}; 
	MMI_WIN_PRIORITY_E  win_priority = WIN_TWO_LEVEL;
	 const uint16 U_text1[] =  { 0x63A5, 0x53D7, 0 }; //接受
 	 const uint16 U_text2[] = { 0x7684, 0x9080, 0x8BF7, 0x89C6, 0x9891, 0x901A, 0x8BDD, 0 }; //的邀请视频通话
	 uint16 phonenum[20] = {0};

	
	  prompt_str.wstr_len  = 0;
	  prompt_str.wstr_ptr = PNULL;

	  prompt_str.wstr_ptr = (wchar*)OEMHEAP_GlobalMalloc(64);
#ifdef APPLE_LOG 
	   OEMOS_dbgprintf("sxmvc_call_incoming_entry:prompt_str.wstr_ptr 0x%x \r\n",prompt_str.wstr_ptr );
#endif
	   
	  if(NULL == prompt_str.wstr_ptr)
	  	return;
	  
	  MMIAPICOM_Wstrcpy((wchar*)prompt_str.wstr_ptr,(uint16*)U_text1);
	  if(pname_t)
	 {
	 	if(pname_t->wstr_len != 0)
	 	{
	 		// MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr,(uint16*)(pname_t->wstr_ptr),pname_t->wstr_len);
	 		MMIAPICOM_Wstrncat((wchar*)prompt_str.wstr_ptr,(uint16*)(pname_t->wstr_ptr),pname_t->wstr_len);
	 	}
		else
		{
			MMIAPICOM_StrToWstr((const uint8*)(pn+3), (uint16*)phonenum);
			// MMIAPICOM_Wstrcpy(prompt_str.wstr_ptr,(uint16*)(phonenum));
			MMIAPICOM_Wstrcat(prompt_str.wstr_ptr,(uint16*)(phonenum));
		}
	 }
	 else
	 {
	 	MMIAPICOM_StrToWstr((const uint8*)(pn+3), (uint16*)phonenum);
	 	// MMIAPICOM_Wstrcpy(prompt_str.wstr_ptr,(uint16*)(phonenum));
	 	MMIAPICOM_Wstrcat(prompt_str.wstr_ptr,(uint16*)(phonenum));
	 }
	 
 	 MMIAPICOM_Wstrcat(prompt_str.wstr_ptr,(uint16*)U_text2);
	 
//	 sprintf(displaymsg,APPLEDISPLAYMSG,pn+3);
	  
//	  MMIAPICOM_StrToWstr(displaymsg, prompt_str.wstr_ptr);
	  
	  prompt_str.wstr_len = MMIAPICOM_Wstrlen(prompt_str.wstr_ptr);

	MMIPUB_OpenAlertWinByTextPtr(PNULL,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,
		MMIPUB_SOFTKEY_OKCANCEL,HandleAppleByMsgWin); 

	if(NULL !=  prompt_str.wstr_ptr)
		OEMHEAP_GlobalFree((void*)prompt_str.wstr_ptr);

	iAppId = appid;
	strcpy(strAppArg, arg);

	if(iAppId == 0x0300009E)
	{
		int n = 0;
		strcat(strAppArg,"|");

		n = strlen(pn) - 11;
		strcat(strAppArg,pn+n);
#ifdef APPLE_LOG
		OEMOS_dbgprintf("sxmvc_call_incoming_entry:%d sxmvc_sms_arg:%s \r\n",iAppId,strAppArg);
#endif

	}
}

// #endif // SXH_APPLE_3GVEDIO_SUPPORT
}

#endif
// end add jxh 2012-3-26
void APPLE_StartByMsg()
{

#ifdef __UM_DUAL_SIM__
	if(APPLE_Select_Sim((AppleStartCB) APPLE_StartByMsg ) != 0)
		return;
#endif	
	Apple_CreateMainWindow();
		//MMK_CreateWin((uint32 *)APPLE_MAIN_WIN_TAB,PNULL);
}

void APPLE_StartByPB(unsigned int appid, char * arg)
{
    APPLE_StartEx(appid, arg);
	
}


void APPLE_StartEx2()
{
	if(!MMIAPIPB_IsPbReady())
	{
	
#ifdef APPLE_LOG
		OEMOS_dbgprintf(" MMIAPIPB_IsPbReady");
#endif

      APPLE_CreateNormalWaitingWin();
		APPLE_Init_TimerInit();
	}
	else
	{
	
#ifdef APPLE_LOG
		OEMOS_dbgprintf(" APPLE_StartEx");
#endif
		//MMK_SetScreenAngle(0);
		//MMK_CreateWin((uint32 *)APPLE_MAIN_WIN_TAB,PNULL);
		Apple_CreateMainWindow();
	}
}
#ifdef JAVA_SUPPORT//sxh az 20110802
#include "mmijava_export.h"
#include "java_mmi_text.h"
#endif
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
void APPLE_StartEx(unsigned int appid, char * arg)
{

#ifdef __MMI_SKYQQ__//斯凯
	if (MMIMRAPP_CheckBackRunApp())
	{
		return;
	}
#endif
   #ifdef MCARE_MINI_SUPPORT//腾讯
               if(MCareV30_Exit_Confirm())
                   return ;
  #endif

  #ifdef QQ_SUPPORT//展讯
    if (QQ_IsRunning())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_EXIT_QQ, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return ;
    }
#endif

    //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d", MMIAPIJAVA_IsJavaRuning());
    if (MMIAPIJAVA_IsJavaRuning())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_EXIT_BACKGROUND_JAVA, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return ;
    }
#endif

#ifdef MCARE_V31_SUPPORT
    
            if(MCareV31_Exit_Confirm())
            {
                return;		
            }
    
#endif

#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_StartEx appid = 0x%x iAppId=0x%x\r\n", appid, iAppId);
#endif

	  //if(iAppId  == 0)
	   {
	       memset(strAppArg, 0, APP_ARG_SIZE);
		iAppId = appid;
		strcpy(strAppArg, arg);
	   }

	s_startAppId  = iAppId;
	
#ifdef __UM_DUAL_SIM__
	if(0x03000042 != iAppId  && 0x0300009c != iAppId)  //解霸与演示厅
	{

#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_StartEx APPLE_Select_Sim iAppId=0x%x  ret:%d\r\n", appid, iAppId,0x03000042!=iAppId);
#endif
		if(APPLE_Select_Sim((AppleStartCB) APPLE_StartEx2 ) != 0)
			return;
	}
#endif	

	if(!MMIAPIPB_IsPbReady())
	{
	
#ifdef APPLE_LOG
		OEMOS_dbgprintf(" MMIAPIPB_IsPbReady");
#endif
		APPLE_CreateNormalWaitingWin();
		APPLE_Init_TimerInit();
	}
	else
	{	

	
#ifdef APPLE_LOG
		OEMOS_dbgprintf(" APPLE_StartEx");
#endif
		//MMK_SetScreenAngle(0);
		//MMK_CreateWin((uint32 *)APPLE_MAIN_WIN_TAB,PNULL);
		Apple_CreateMainWindow();
		

	}
}

void APPLE_StartApple(void)
{

    APPLE_StartEx(0x03000000, "start");
	
}
void APPLE_StartSMDApp(uint32 appId)
{	
	
	extern uint32 start_appid;
	start_appid = appId;
    
    //s_startAppId  = appId;//sxh az
    
    #ifdef __MMI_SKYQQ__
        if (MMIMRAPP_CheckBackRunApp())
        {
            return;
        }
    #endif
    
     #ifdef MCARE_MINI_SUPPORT
                 if(MCareV30_Exit_Confirm())
                     return ;
    #endif
    #ifdef QQ_SUPPORT
    if (QQ_IsRunning())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_EXIT_QQ, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return ;
    }
#endif

    //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d", MMIAPIJAVA_IsJavaRuning());
    if (MMIAPIJAVA_IsJavaRuning())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_EXIT_BACKGROUND_JAVA, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return ;
    }
#endif
#ifdef MCARE_V31_SUPPORT
    
            if(MCareV31_Exit_Confirm())
            {
                return;		
            }
    
#endif

#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_StartSMDApp  appId:%d",appId);
#endif
	MMK_CreateWin((uint32 *)TEST_WIN_TAB,PNULL);
}
void APPLE_StartTv1(void)
{

  	APPLE_StartEx(0x03000041, "home:liv"); 
  
 	
}
void APPLE_StartBook(void)
{
  	APPLE_StartEx(0x030000ED, NULL);
}

void APPLE_StartTv2(void)
{


    
	APPLE_StartEx(0x03000041, "home:mov");
}
void APPLE_StartTv3(void)
{

    APPLE_StartEx(0x03000041, "home:vid");
	
}
void APPLE_StartUMvchat(void)
{
    APPLE_StartEx(0x0300009A, strAppArg);
    
}
void APPLE_StartRadio(void)
{
     APPLE_StartEx(0x03000093, strAppArg);
}


void APPLE_StartUMvchatDemo(void)
{

    APPLE_StartEx(0x0300009c, strAppArg);
}



void APPLE_StartUMPlayer(void)
{
#ifdef VIDEO_PLAYER_SUPPORT
if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_VIDEO_PLAYER))//sxh az add
    {
//        MMIAPISET_ValidatePrivacyPwd(VIRTUAL_WIN_ID, MMISET_PROTECT_SXH_VIDEO_PLAYER_TYPE );
        return;
    }
#endif
    APPLE_StartEx(0x03000042, strAppArg);
}
void APPLE_StartUMPlayer2(void)//sxh az add
{
    APPLE_StartEx(0x03000042, strAppArg);
}


void APPLE_StartOnlineGame(void)
{
	 APPLE_StartEx(0x030000B0, strAppArg);
}

void APPLE_StartMusic(void)
{
	 APPLE_StartEx(0x030000A8, strAppArg);
}

void  APPLE_StartVideoDL(void)
{
	 APPLE_StartEx(0x030000A9, strAppArg);
}

void  APPLE_StartWeather(void)
{
	 APPLE_StartEx(0x03000026, strAppArg);
}

#ifdef SXH_APPLE_3GVEDIO_SUPPORT
void APPLE_StartUMvchatthin(void)
{
   APPLE_StartEx(0x0300009E, strAppArg);
	
}
#endif // SXH_APPLE_3GVEDIO_SUPPORT

/*****************************************************************************/
//  Description : SIM 卡选择处理
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/

#ifdef __UM_DUAL_SIM__
PUBLIC void APPLE_AppendMultiSimNameExt(//选卡界面风格
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T left_softkey_id,
    MMI_TEXT_ID_T middle_softkey_id,
    MMI_TEXT_ID_T right_softkey_id,
    uint32 max_num,
    MMISET_APPEND_SIM_TYPE_E append_type
   )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint32 i = 0;
    MMISET_SIM_NAME_T   sim_str     = {0};
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    MMI_STRING_T network_str = {0};
    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
    MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        sim_img_id[i] = IMAGE_IDLE_PROMPT_SIM1 + (i - MN_DUAL_SYS_1);    //shoule IMAGE_IDLE_PROMPT_NO_SIM2 == IMAGE_IDLE_PROMPT_NO_SIM1+1.atc
        no_sim_img_id[i] = IMAGE_IDLE_PROMPT_NO_SIM1 + (i - MN_DUAL_SYS_1);
        gray_sim_img_id[i] = IMAGE_IDLE_PROMPT_GRAY_SIM1 + (i - MN_DUAL_SYS_1);
    }
    
    if (max_num < MMI_DUAL_SYS_MAX)
    {
        return;
    }
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        BOOLEAN is_append = FALSE;

        switch(append_type)
        {
        case MMISET_APPEND_SYS_OK:
            is_append = MMIPHONE_IsSimAvailable(i);
            break;

        case MMISET_APPEND_SIM_OK:
            is_append = MMIPHONE_IsSimCardOk(i);
            break;

        case MMISET_APPEND_PS_READY:
            is_append = MMIPHONE_GetIsPsReady(i);
            break;

        case MMISET_APPEND_PIN1_PERMIT:
            is_append = MMIPHONE_IsPermitSetPin1(i);
            break;

        case MMISET_APPEND_STK_CARD:
            is_append = MMIAPISTK_IsStkCard((MN_DUAL_SYS_E)i);
            break;

        case MMISET_APPEND_SIM_ALL:
            is_append = TRUE;
            break;

        default:
            is_append = FALSE;
            break;
        }

        if (is_append)
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            MMI_IMAGE_ID_T image_id = IMAGE_NULL;
            
            item_t.item_style    =GUIITEM_STYLE_TWO_LINE_RADIO;// GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;//界面风格
            item_t.item_data_ptr = &item_data;
            item_t.user_data = i;//indicate which sim is appended !!

            if (e_dualsys_setting & (0x01 << i))
            {
                if (MMIPHONE_GetSimExistedStatus(i))
                {
                    image_id = sim_img_id[i];
                }
                else
                {
                    image_id = no_sim_img_id[i];
                }
            }
            else
            {
                if (MMIPHONE_GetSimExistedStatus(i))
                {
                    image_id = gray_sim_img_id[i];
                }
                else
                {
                    image_id = no_sim_img_id[i];
                }
            }

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = image_id;

            sim_str = MMIAPISET_GetSimName(i);

            //display SIM Card Display string!!
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = sim_str.wstr_len;
            
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

            //get opn_spn information for display in sim selection page!!
            SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
            network_str.wstr_ptr = (wchar *)opn_spn_name;
            SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            
            if (MMISET_APPEND_STK_CARD == append_type)
            {
                BOOLEAN is_exist_stk_name = FALSE;

                is_exist_stk_name = MMIAPISTK_GetStkName((MN_DUAL_SYS_E)i, &network_str);

                if (!is_exist_stk_name)
                {
                    MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);
                }
            }
            else
            {
                MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);
            }
    
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_len = network_str.wstr_len;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;

            //set softkey
            item_data.softkey_id[0] = left_softkey_id;
            item_data.softkey_id[1] = middle_softkey_id;
            item_data.softkey_id[2] = right_softkey_id;
            
            GUILIST_AppendItem(ctrl_id, &item_t);      
        }
    }

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
#endif  //!defined(MMI_MULTI_SIM_SYS_SINGLE)
}
BOOLEAN AppleAppendSelectListItem(MMI_CTRL_ID_T ctrl_id)
{
    MMI_STRING_T sim_str = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

#ifdef MMI_WIFI_SUPPORT
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX + 1, FALSE );
#else
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE );
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    //MMIAPISET_AppendMultiSimName(ctrl_id, STXT_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
    APPLE_AppendMultiSimNameExt(ctrl_id, STXT_OK,TXT_NULL,STXT_RETURN, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MN_DUAL_SYS_1;

    MMI_GetLabelTextByLang(TXT_GPRS,&sim_str);
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = STXT_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

#ifdef MMI_WIFI_SUPPORT
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI

    MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = STXT_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

    return TRUE;
}
void Apple_SetIsUseWifi(int bUseWifi);
LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(
                                         MMI_WIN_ID_T	win_id, 
                                         MMI_MESSAGE_ID_E	msg_id, 
                                         DPARAM				param
                                         )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			list_index = 0;
	  uint32     item_data = 0;


#ifdef APPLE_LOG
	OEMOS_dbgprintf("OEMOS_HandleSIMSelectWinMsg     msg_id:%d",msg_id);
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //动态添加LIST ITEM
        //GUILIST_SetMaxItem(MMIAPPLE_SELECT_SIM_CTRL_ID, MN_DUAL_SYS_MAX, FALSE );//max item 2
        //MMIAPISET_AppendDualSimName(MMIAPPLE_SELECT_SIM_CTRL_ID, STXT_OK, MN_DUAL_SYS_MAX);
	AppleAppendSelectListItem(MMIAPPLE_SELECT_SIM_CTRL_ID);

#ifdef APPLE_LOG
	OEMOS_dbgprintf("  HandleSIMSelectWinMsg  ITEMNUM:%d  g_apple_active_sim:%d",GUILIST_GetTotalItemNum(MMIAPPLE_SELECT_SIM_CTRL_ID),g_apple_active_sim);
#endif

#ifdef MMI_WIFI_SUPPORT
		if(APPLE_GetIsUseWifi())
		{
			GUILIST_SetCurItemIndex(MMIAPPLE_SELECT_SIM_CTRL_ID,GUILIST_GetTotalItemNum(MMIAPPLE_SELECT_SIM_CTRL_ID)-1);
			GUILIST_SetSelectedItem(MMIAPPLE_SELECT_SIM_CTRL_ID,GUILIST_GetTotalItemNum(MMIAPPLE_SELECT_SIM_CTRL_ID)-1,TRUE);
		}
		else
		{
#endif	

			if(g_apple_active_sim < 0 ||  g_apple_active_sim >= MMI_DUAL_SYS_MAX)
				g_apple_active_sim  = OEMOS_GetActiveSim();

			for(list_index = 0 ; list_index <GUILIST_GetTotalItemNum(MMIAPPLE_SELECT_SIM_CTRL_ID);list_index++)
			{
				GUILIST_GetItemData(MMIAPPLE_SELECT_SIM_CTRL_ID, list_index, &item_data);

#ifdef APPLE_LOG
				OEMOS_dbgprintf("  HandleSIMSelectWinMsg list_index:%d  item_data:%d  g_apple_active_sim:%d",list_index,item_data,g_apple_active_sim);
#endif

				if(g_apple_active_sim == item_data  )
				{
					GUILIST_SetCurItemIndex(MMIAPPLE_SELECT_SIM_CTRL_ID,list_index);
					GUILIST_SetSelectedItem(MMIAPPLE_SELECT_SIM_CTRL_ID,list_index,TRUE);

					break;
				}
				
			}
#ifdef MMI_WIFI_SUPPORT
		}
#endif
	
		
        MMK_SetAtvCtrl(APPLE_SELECT_SIM_WIN_ID, MMIAPPLE_SELECT_SIM_CTRL_ID);

#ifdef APPLE_LOG
	OEMOS_dbgprintf("HandleSIMSelectWinMsg     MSG_OPEN_WINDOW");
#endif
        break;

     case MSG_CLOSE_WINDOW:
	g_apple_is_select_sim = FALSE;
	break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:

#ifdef APPLE_LOG
	OEMOS_dbgprintf("HandleSIMSelectWinMsg     OK ");
#endif

#if defined(MMI_MULTI_SIM_TRI)
        if (GUILIST_IsCurrItemInvalid(MMIAPPLE_SELECT_SIM_CTRL_ID))
        {
            break;
        }
  #endif
        list_index = GUILIST_GetCurItemIndex(MMIAPPLE_SELECT_SIM_CTRL_ID);
  	GUILIST_GetItemData(MMIAPPLE_SELECT_SIM_CTRL_ID, list_index, &item_data);

	GUILIST_SetSelectedItem(MMIAPPLE_SELECT_SIM_CTRL_ID,list_index,TRUE);
#ifdef APPLE_LOG  
	OEMOS_dbgprintf("HandleSIMSelectWinMsg     OK list_iindex:%d,item_data:%d",list_index,item_data);
#endif
	if(item_data < MMI_DUAL_SYS_MAX)
	{
#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleSIMSelectWinMsg  no  use wifi"); 
#endif
		g_apple_active_sim = item_data;
		Apple_SetIsUseWifi(0);
	}
#ifdef MMI_WIFI_SUPPORT
	else
	{
#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleSIMSelectWinMsg    use wifi");
#endif
		Apple_SetIsUseWifi(1);
		g_apple_active_sim = OEMOS_GetActiveSim();
	}
#endif


#if 0
        if (0 == list_index)
        {
            g_apple_active_sim = MN_DUAL_SYS_1;
        }
	
#if  defined(MULTI_SIM_SYS_TRI)|| defined(MULTI_SIM_SYS_QUAD)
        else if (2 == list_index)
        {
            g_apple_active_sim = MN_DUAL_SYS_3;

        }
#endif
#ifdef MULTI_SIM_SYS_QUAD
	else if(3 == list_index)
	{
		 g_apple_active_sim = MN_DUAL_SYS_4;
	}
#endif
	else  if(1 == list_index)
        {
            g_apple_active_sim = MN_DUAL_SYS_2;

        }
#endif
#ifdef APPLE_LOG
	OEMOS_dbgprintf("HandleSIMSelectWinMsg, FunCb 0x%x,g_apple_active_sim:%d,list_index:%d", g_FunCB,g_apple_active_sim,list_index);
#endif	
	g_apple_is_select_sim = TRUE;
	if(g_FunCB)
    	 	g_FunCB();
	
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
		//iAppId = 0;
	memset(strAppArg, 0, APP_ARG_SIZE);
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:		
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

//smd

//#include "umvchat.h"
// static const short U_Help[] = { 0x5E2E, 0x52A9, 0 }; //帮助
// static const short U_About[] = { 0x5173, 0x4E8E, 0 }; //关于




static const short U_Run[] = { 0x542F, 0x52A8, 0 }; //启动
static const short U_Setup[] = { 0x8BBE, 0x7F6E, 0 }; //设置
static const short U_TV1[] = {0x60A0, 0x7C73, 0x624B, 0x673A, 0x7535, 0x89C6, 0 };
static const short U_TV2[] = {0x60A0, 0x7C73, 0x5728, 0x7EBF, 0x7535, 0x5F71, 0};//悠米在线电影
static const short U_TV3[] = {0x60A0, 0x7C73, 0x5728, 0x7EBF, 0x89C6, 0x9891, 0 };//悠米在线视频
static const short U_Update[] = {0x66F4, 0x65B0, 0x7A0B, 0x5E8F, 0 }; //更新程序
static const short U_tlbb[] = {0x5929, 0x9F99, 0x516B, 0x90E8, 0 }; //天龙八部
static const short U_lwsg[] = {0x4E71, 0x821E, 0x4E09, 0x56FD, 0 }; //乱舞三国


static unsigned int SMD_highlight_item = 0;
static unsigned int SMD_menu_num = 0;
static SXM_Display_Menu display_menu;
// static int SMDSetup_curHightlightItem = 0;
extern SXM_LISTCTRL SMD_listctrl ; 
#define LIST_ID_DMS  10
uint32 start_appid;
uint32 save_startappid;
unsigned int SMDLoadSubMenu(char **list_of_items, int sub_menu_ffset);


/////////////////////////////////////////////////////////////////////////////////////
// List
////////////////////////////////////////////////////////////////////////////////////

typedef void  (*UMVCHAT_Listfun)(uint16 list_index);

typedef struct //_ListCtl_Imp
{
	MMI_CTRL_ID_T list_ID;
	MMI_HANDLE_T  parent_handle;
	void* pListPtr;//
	GUI_RECT_T      list_rect;
	UMVCHAT_Listfun	lis_fun;
	//uint16   maxitemnum ;
}ListCtl_Imp;

void OEM_update_screen()
{
	MMK_UpdateScreen();
}

 

LOCAL  IGUICTRL_T* Sxmobi_CreateList(MMI_CTRL_ID_T ctrl_id,MMI_HANDLE_T  parent_win_handle,
										 GUI_RECT_T* pRect,GUILIST_TYPE_E list_type,uint16  maxitemnum )
{
	MMI_CONTROL_CREATE_T create = {0};
	GUILIST_INIT_DATA_T init_data = {0};
	IGUICTRL_T * list_ptr  = NULL;
	init_data.both_rect = MMITHEME_GetWinClientBothRect(APPLE_SMD_WIN_ID);	
	init_data.type = list_type;		//这里是真的时候，appenditem 失败
	init_data.max_item_num = maxitemnum;
	
	create.ctrl_id = ctrl_id;
	create.guid    = SPRD_GUI_LIST_ID;
	create.parent_win_handle =parent_win_handle; 
	//create.parent_ctrl_handle = win_create.applet_handle;
	create.init_data_ptr = &init_data ;
	list_ptr = MMK_CreateControl( &create );
	
	MMK_SetAtvCtrl(APPLE_SMD_WIN_ID,LIST_ID_DMS);
	//GUILIST_SetBgColor(ctrl_id,MMI_BLUE_COLOR);
	
	
	return list_ptr;
}

// extern int OEMOS_BRW_ConnectUrl(char* url);
void SxmobiSMDOpenUrl(char* url)
{
    if(url && strlen(url) > 0)
    {
        OEMOS_BRW_ConnectUrl(url);
    }
}

// extern void* apple_preapp_get_entry(unsigned int app_id);
void SxmobiSMDStartApp(unsigned int entry, unsigned int appid, char* pArg)
{
	char args[128] ={0};
	if(NULL != pArg)
	sprintf(args,"dms:3_0x%08x_0x%08x|%s", entry, appid, pArg);
	else
	sprintf(args,"dms:3_0x%08x_0x%08x", entry, appid);
#ifdef APPLE_LOG
	OEMOS_dbgprintf("arg is %s" ,args);
#endif
/*
	if(apple_preapp_get_entry(0x030000C0) !=NULL)	// 有 amm
	 {
	 	iAppId= appid;
	 }
	 else
	 {
	 	iAppId = 0x03000098;
	 }
*/
	iAppId = 0x03000098;
	 strcpy(strAppArg, args);
	 
	 s_startAppId  = appid;
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("SxmobiSMDStartApp iAppId =0x%x, strAppArg is %s\r\n" ,iAppId, strAppArg);
#endif 


	Apple_CreateMainWindow();
}

int APPLE_AppDownLoadByAppman_SMD(int appid, char *args)
{
	char temp[100]={0};
#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_AppDownLoadByAppman_SMD %d \r\n", __LINE__);
#endif

	sprintf(temp, "dms1:3_0x%08x_0x%08x", appid, appid);
	
#ifdef APPLE_LOG
	OEMOS_dbgprintf("APPLE_AppDownLoadByAppman_SMD %d temp:%s \r\n", __LINE__, temp);
#endif

	iAppId = 0x03000098;
	strcpy(strAppArg, temp);


	Apple_CreateMainWindow();

	
	return 0;
}
static ListCtl_Imp g_ActiveListCtl = {0};

void OEM_ListCtl_Destroy();
void* OEM_ListCtl_Create(apple_rect listrect,uint16 maxitemnum,OEM_LIST_TYPE listType)
{
	GUI_RECT_T rc = {0};
	GUILIST_TYPE_E  listtype2 = 0;
	rc.left = listrect.x;
	rc.right = listrect.x+listrect.dx-1;
	rc.top = listrect.y;
	rc.bottom = listrect.y + listrect.dy-1;
	
	
	
	
	OEM_ListCtl_Destroy();
	
	
	
	switch (listType)
	{
	case OEM_LIST_RADIOLIST:/*!<  single selected list */
		{
			listtype2 = GUILIST_RADIOLIST_E;
		}
		break;
	case OEM_LIST_CHECKLIST: 							/*!<  multi selected list   */
		{
			listtype2 = GUILIST_CHECKLIST_E;
		}
		break;
	case OEM_LIST_TEXTLIST:							/*!<  text list */
		{
			listtype2 = GUILIST_TEXTLIST_E;
		}
		break;	
	default:
		{
			return NULL;
		}
//		break;
	}
	g_ActiveListCtl.list_ID = LIST_ID_DMS;
	g_ActiveListCtl.parent_handle = APPLE_SMD_WIN_ID;
	g_ActiveListCtl.pListPtr = Sxmobi_CreateList(LIST_ID_DMS,APPLE_SMD_WIN_ID,&rc,listtype2,maxitemnum);
	
	
	//出来LIST的时候，防止MSGBOX还在上面显示。
	
	
	return g_ActiveListCtl.pListPtr ;
}

int OEM_ListCtl_IsActive()
{
	return (NULL != g_ActiveListCtl.pListPtr)?1:0;
	
}


void OEM_ListCtl_Destroy()
{
	if(g_ActiveListCtl.pListPtr)  //
	{
		MMK_DestroyDynaCtrl(g_ActiveListCtl.list_ID);
		SCI_MEMSET(&g_ActiveListCtl, 0x0, sizeof(g_ActiveListCtl));

		//2010-10-14 by liuhongmou
		GUIWIN_SetTitleBackground(APPLE_SMD_WIN_ID,IMAGE_COMMON_TITLE_BAR,0);
	}	
}


static int GetResourceImageID(uint32 image_id)
{
	switch(image_id)
	{
	case ID_NUM1:
		return IMAGE_NUMBER_1;
	case ID_NUM2:
		return IMAGE_NUMBER_2;
	case ID_NUM3:
		return IMAGE_NUMBER_3;
	case ID_NUM4:
		return IMAGE_NUMBER_4;
	case ID_NUM5:
		return IMAGE_NUMBER_5;
	case ID_NUM6:
		return IMAGE_NUMBER_6;
	case ID_NUM7:
		return IMAGE_NUMBER_7;
	case ID_NUM8:
		return IMAGE_NUMBER_8;
	case ID_NUM9:
		return IMAGE_NUMBER_9;
	case ID_NUM10:
		return IMAGE_NUMBER_10;
	case ID_NUM11:
		return IMAGE_NUMBER_11;
	case ID_NUM12:
		return IMAGE_NUMBER_12;
	case ID_NUM13:
		return IMAGE_NUMBER_13;
	case ID_NUM14:
		return IMAGE_NUMBER_14;
	case ID_NUM15:
		return IMAGE_NUMBER_15;
	case ID_NUM16:
		return IMAGE_NUMBER_16;
	case ID_NUM17:
		return IMAGE_NUMBER_17;
	case ID_NUM18:
		return IMAGE_NUMBER_18;
	case ID_NUM19:
		return IMAGE_NUMBER_19;
	case ID_NUM20:
		return IMAGE_NUMBER_20;
		
	default:
		return 0;
	}
//	return 0;
} 
int OEM_GetTitleHeight()
{
	return MMI_TITLE_HEIGHT;
}

int OEM_GetStatusBarTop()
{
	return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
}

int  OEM_ListCtl_AppendListItem(OEM_LIST_ITEM_STYLE itemStyle, va_list arg_ptr)
{
	//va_list arg_ptr;  
	GUILIST_ITEM_DATA_T         item_data = {0};
	GUILIST_ITEM_T item = {0};
	int content_cnt = 0 ;
	int argval =0;
	GUIITEM_STYLE_E itemstyle2 = 0;
	wchar* pText =NULL;
	MMI_IMAGE_ID_T  image_id=IMAGE_NULL ;
	uint8 *pImgBuffer=NULL;
	uint32 buflen =0;
	int len=0;
	switch(itemStyle)
	{
	case OEM_LIST_ITEM_STYLE_ONE_LINE_TEXT:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_TEXT;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_ICON_TEXT:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_TWO_ICON_TEXT:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_NUMBER_TEXT:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_ICON_TWO_TEXT:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_CHECK:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_CHECK;
		break;
	case OEM_LIST_ITEM_STYLE_ONE_LINE_RADIO:
		itemstyle2 = GUIITEM_STYLE_ONE_LINE_RADIO;
		break;
	default:
		return 0;
	}	
	SCI_MEMSET(&item,0x0,sizeof(item));
	SCI_MEMSET(&item_data,0x0,sizeof(item_data));
	//va_start(arg_ptr, itemStyle);	
	do 
	{
		argval = (int)va_arg(arg_ptr,LISTCTL_DATA_TYPE);
		switch(argval)
		{
		case OEM_LISTCTL_DATA_TEXT_BUFFER:
			{
				
				 pText = va_arg(arg_ptr,wchar*);
				 len = MMIAPICOM_Wstrlen(pText);
				item_data.item_content[content_cnt].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
				item_data.item_content[content_cnt].item_data.text_buffer.wstr_len = len;
				//MMI_WSTRNCPY( item_data.item_content[content_cnt].item_data.text_buffer.wstr_ptr, GUILIST_STRING_MAX_NUM, pText, 255, len);
				item_data.item_content[content_cnt].item_data.text_buffer.wstr_ptr = pText;
				content_cnt++;
			}
			break;
		case OEM_LISTCTL_DATA_TEXT_IMAGEID:
			{
				image_id =  va_arg(arg_ptr,MMI_IMAGE_ID_T);
				item_data.item_content[content_cnt].item_data_type = GUIITEM_DATA_IMAGE_ID;
				item_data.item_content[content_cnt].item_data.image_id = GetResourceImageID(image_id);/*IMAGE_SECMENU_ICON_UMVCHAT*/;//GetResourceImageID(image_id);
				content_cnt ++;
			}
			break;
		case OEM_LISTCTL_DATA_TEXT_IMAGEBUFFER:
			{
				pImgBuffer = va_arg(arg_ptr,uint8*);
				 buflen = va_arg(arg_ptr,uint32);
				item_data.item_content[content_cnt].item_data_type = GUIITEM_DATA_ANIM_DATA;
				item_data.item_content[content_cnt].item_data.anim_data_ptr->data_size = buflen;//icon_id->content.wstr_len;
				item_data.item_content[content_cnt].item_data.anim_data_ptr->is_bitmap = FALSE;
				//item_data.item_content[content_cnt].item_data.anim_data_ptr->type = GUIANIM_TYPE_IMAGE_BMP;
				item_data.item_content[content_cnt].item_data.anim_data_ptr->data_ptr = pImgBuffer;//(uint8*)icon_id->content.wstr_ptr;
				content_cnt ++;
			}
//		default:
//			break;
		}
	} while( 0 != argval );
	// va_end(arg_ptr);  
	
	item.item_style = itemstyle2;
	item.item_data_ptr = &item_data;	
	item_data.softkey_id[0] = STXT_SELECT;
	item_data.softkey_id[2] = STXT_RETURN;
	
	GUILIST_AppendItem(LIST_ID_DMS, &item);
	
	//出来LIST的时候，防止MSGBOX还在上面显示。
	//MMK_UpdateScreen();
	
	//IGUICTRL_SetState(g_ActiveListCtl.pListPtr,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);
	
	return 0;
}

int  ListCtl_AppendListItem(OEM_LIST_ITEM_STYLE itemStyle,...)
{
	va_list args;
	int ret;
	va_start(args, itemStyle);
	ret = OEM_ListCtl_AppendListItem(itemStyle,args);
	va_end(args);
	return ret;
}
uint16 OEM_ListCtl_GetCuriItemIndex()
{
	//SCI_ASSERT(g_ActiveListCtl.pListPtr);
	if(!g_ActiveListCtl.pListPtr)
		return 0;
	return GUILIST_GetCurItemIndex(g_ActiveListCtl.list_ID);
}

unsigned char OEM_ListCtl_RemoveItem(int pos)
{
	if(!g_ActiveListCtl.pListPtr)
		return 0;
	return GUILIST_RemoveItem(g_ActiveListCtl.list_ID,pos);
}

unsigned char OEM_ListCtl_RemoveAllItems()
{
	if(!g_ActiveListCtl.pListPtr)
		return 0;
	return  GUILIST_RemoveAllItems(g_ActiveListCtl.list_ID);
}


unsigned char OEM_ListCtl_SetCurItemIndex(int pos)
{
	if(!g_ActiveListCtl.pListPtr)
		return 0;
	return GUILIST_SetCurItemIndex(g_ActiveListCtl.list_ID,pos);
}


/////////////////////////////////////////////////////////////////////////////////////
// List
////////////////////////////////////////////////////////////////////////////////////


void SMDMenuRun(void)
{
	SxmobiSMDStartApp(SMD_listctrl.entry_appid,SMD_listctrl.entry_appid, NULL);
}
void SMDMenuUpdate(void)
{
	AppleUpdateApp_SMD(SMD_listctrl.entry_appid);
}
#ifdef __UM_DUAL_SIM__
void EntrySMDSetup(void)
{
 	#ifdef __UM_DUAL_SIM__
	if(APPLE_Select_Sim((AppleStartCB) EntrySMDSetup ) != 0)
		return;
#endif 
}
#endif
static SXM_MenuItem VCMenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuRun},
	#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
static int SMDTransferHighlight2Index(unsigned int SMD_highlight_item2)
{
	int index,i;
	
	index = 0;
	for(i=0;i<SMD_listctrl.number_of_items;i++)
	{
		if(SMD_listctrl.node[i].parent_menu_index == SMD_listctrl.active_menu_index)
		{
			if(index==SMD_highlight_item2)
			{
				break;
			}
			index++;
		}
	}

	return i;
}


MMI_WIN_ID_T gSmdWinId = 0;

static char g_smd_url[512] = {0};
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
static void SMD_URL_ConnectCB(void* p)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
#endif
	if(0 != gSmdWinId)
		MMK_CloseWin(gSmdWinId);
#ifdef APPLE_LOG
        OEMOS_dbgprintf("SMD_URL_ConnectCB %s\r\n", g_smd_url);
#endif

#if defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G)||defined(PLATFORM_SC6530)//sxh az 20120705
	#ifdef BROWSER_SUPPORT
	    entry_param.type = MMIBROWSER_ACCESS_URL;
	    entry_param.dual_sys = OEMOS_GetActiveSim();
	    entry_param.url_ptr = g_smd_url;
	    entry_param.user_agent_ptr = PNULL;
	    MMIAPIBROWSER_Entry(&entry_param);
	#endif	
#else
#if defined(SXH_SPT_H7810) || defined(SXH_SPT_0839)
	MMICMSBRW_AccessUrl((uint8*)g_smd_url);
#else
	// MMICMSBRW_AccessUrl((uint8*)g_smd_url,MN_DUAL_SYS_MAX);
#endif
#endif

}

//#define APPLE_OS_EXTENSION_BEGIN 0x7fff
//#define APPLE_OEM_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 6000)
//#define APPLE_OS_SMSCENTER (APPLE_OEM_EX_BEGIN + 50)//返回短信中心号码
//#define APPLE_CURRENT_IMSI (APPLE_OEM_EX_BEGIN + 8) //取IMSI字符串
//#define APPLE_OS_API_MODEL (0x7fff+2)
//#define APPLE_OS_API_PLATFORM (0x7fff+1)
//#define APPLE_OS_RELEASE_BUILD_DATETIME (APPLE_OEM_EX_BEGIN + 14) // 编译时间
//#define APPLE_OS_API_LCDWIDTH (0x7fff+10)
//#define APPLE_OS_API_LCDHEIGHT (0x7fff+11)
//#define APPLE_CURRENT_OPERATOR (APPLE_OEM_EX_BEGIN + 9) //运营商判断:0 中国移动  1中国联通  2中国电信



// extern int OEM_GetDeviceInfoEx(int nItem, void *pBuff, int *pnSize);

static void SMD_GetSmsCenter(char *pBuf,int *pSize)
{
    char *startptr;
    int leftlen;
	memset(pBuf,0,(*pSize));
	OEM_GetDeviceInfoEx(APPLE_OS_SMSCENTER,(void*)pBuf,pSize );

	if(strlen(pBuf) > 0)
	{//检查非数字
		startptr = pBuf;
		while (strlen(startptr) > 0)
		{
			if( (*startptr) >= '0' && (*startptr) <= '9' )
			{
				startptr ++;
			}
			else
			{
				leftlen = strlen(pBuf) - (startptr - pBuf) - 1;
				if(leftlen > 0)
				{
					memcpy(startptr,startptr + 1,leftlen);
				}
				pBuf[strlen(pBuf) - 1] = 0;
			}
		}
	}
}
void SMD_ITOSTR(char *pBuf,int aBufLen,int aValue)
{
	char * p = pBuf;
	int i = 0;
	int bM;
	int tmp;
	int j = 0;
	if(aValue<0)
	{
		bM = 1;
		aValue = -aValue;
	}
	else
	{
		bM = 0;
	}

	do
	{
		*p = (aValue - (aValue/10) * 10 + '0');
		p ++;
		aValue = aValue / 10;
		i ++;
	}while(aValue != 0 && i < aBufLen - 1);
	if(bM && i < aBufLen - 1)
	{
		*p = '-';
		p ++;
		i ++;
	}
	*p = 0;

	for(j=0; j<i/2; j++)
	{
		tmp = pBuf[j];
		pBuf[j] = pBuf[i-j-1];
		pBuf[i-j-1] = tmp;
	}
}


static int SMD_MakeURLParam(char* url, int len)
{
    int leftlen = 0;
    int value = 0;
    char opbuf[8] = {0};

    if(!url || len <= 0)
    {
        return 0;
    }

    leftlen = len - strlen(url);
    if(leftlen < 33)
    {
        return 0;
    }
    strcat(url, "imsi=");
	leftlen = len - strlen(url) - 1;
	OEM_GetDeviceInfoEx(APPLE_CURRENT_IMSI, url+strlen(url), &leftlen);

    value = 0;
	leftlen = sizeof(value);
	OEM_GetDeviceInfoEx(APPLE_OEM_EX_BEGIN + 161, &value, &leftlen);
    sprintf(url+strlen(url),"&ps161=%d", value);

    value = 0;
	leftlen = sizeof(value);
	OEM_GetDeviceInfoEx(APPLE_OEM_EX_BEGIN + 148, &value, &leftlen);
    if(value)
    {
        strcat(url,"&ps148=1");
    }
    else
    {
        strcat(url,"&ps148=0");
    }

    leftlen = len - strlen(url);
    if(leftlen < 33)
    {
        return 0;
    }
    strcat(url, "&md=");
	leftlen = len - strlen(url) - 1;
	OEM_GetDeviceInfoEx(APPLE_OS_API_MODEL, url+strlen(url), &leftlen);

    strcat(url, "&pf=");
	leftlen = len - strlen(url) - 1;
	OEM_GetDeviceInfoEx(APPLE_OS_API_PLATFORM, url+strlen(url), &leftlen);

    leftlen = len - strlen(url);
    if(leftlen < 33)
    {
        return 0;
    }
    strcat(url,"&rbd=");
	leftlen = len-strlen(url);
    OEM_GetDeviceInfoEx(APPLE_OS_RELEASE_BUILD_DATETIME, url+strlen(url), &leftlen);

    leftlen = len - strlen(url);
    if(leftlen < 33)
    {
        return 0;
    }
    value = 0;
    leftlen = sizeof(value);
    OEM_GetDeviceInfoEx(APPLE_OS_API_LCDWIDTH, &value, &leftlen);
    sprintf(url+strlen(url),"&sw=%d", value);

    value = 0;
    leftlen = sizeof(value);
    OEM_GetDeviceInfoEx(APPLE_OS_API_LCDHEIGHT, &value, &leftlen);
    sprintf(url+strlen(url),"&sh=%d", value);

    value = -1;
	leftlen = sizeof(value);
	OEM_GetDeviceInfoEx(APPLE_CURRENT_OPERATOR, &value, &leftlen);
	if(-1 != value)
	{
		SMD_ITOSTR(opbuf,7,value);
		strcat(url,"&op=");
		strcat(url,opbuf);
	}
	else
	{
		strcat(url,"&op=0");
	}

    leftlen = len - strlen(url);
    if(leftlen < 33)
    {
        return 0;
    }
	strcat(url,"&sc=");
	leftlen = len-strlen(url) - 1;
	SMD_GetSmsCenter(url + strlen(url),&leftlen);
    return 1;

}

static int SMD_URL_ConnectUrl(char* url)
{
#if 0
	int msg = 0;
#ifdef APPLE_LOG
    OEMOS_dbgprintf("SMD_URL_ConnectUrl %s\r\n", url);
#endif
	strcpy(g_smd_url, url);
    MMK_PostMsg(gSmdWinId, SMD_BASE_MSG_ID, (DPARAM)SMD_URL_ConnectCB , 4);
#endif
    int msg = 0;
    if(!url || strlen(url) == 0)
    {
#ifdef APPLE_LOG
            OEMOS_dbgprintf("SMD_URL_ConnectUrl url null\r\n");
#endif
        return 0;
    }
#ifdef APPLE_LOG
    OEMOS_dbgprintf("SMD_URL_ConnectUrl %s\r\n", url);
#endif
    memset(g_smd_url, 0, sizeof(g_smd_url));
	strcpy(g_smd_url, url);

    SMD_MakeURLParam(g_smd_url, sizeof(g_smd_url));
    MMK_PostMsg(gSmdWinId, SMD_BASE_MSG_ID, (DPARAM)SMD_URL_ConnectCB , 4);
//	OEMOS_BRW_ConnectUrl(g_smd_url);
	return 0;
}

void EntrySxmobiSMDSubMenu(void) 
{ 
  
	signed long number_of_items;
	char *list_of_items[MAX_SUB_MENUS];
	char* guiBuffer; 
	int i;
	char  listitemtext[256] = {0};

	
	number_of_items = 0; 

	number_of_items += SMDLoadSubMenu(&list_of_items[0], number_of_items);
i = OEM_ListCtl_RemoveAllItems();
for(i=0;i<number_of_items;i++)
	{ 
	#ifndef __SXMOBI_NO_ICONIMAGE__
		 ListCtl_AppendListItem(OEM_LIST_ITEM_STYLE_ONE_LINE_ICON_TEXT,OEM_LISTCTL_DATA_TEXT_IMAGEID,i,
		OEM_LISTCTL_DATA_TEXT_BUFFER,list_of_items[i],0);
	#else
		
		memset(listitemtext,0x0,256);
	OEM_wsprintf(listitemtext,256,L"%d. %s",i+1,(wchar*)list_of_items[i]);
		 ListCtl_AppendListItem(OEM_LIST_ITEM_STYLE_ONE_LINE_TEXT,
		OEM_LISTCTL_DATA_TEXT_BUFFER,listitemtext,0);
	#endif
}
OEM_update_screen();
} 

static void EntrySxmobiSMD_LSK(void)
{
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("EntrySxmobiSMD_LSK SMD_highlight_item:%d display_menu.item_count %d", SMD_highlight_item,display_menu.item_count);
#endif		

	if (SMD_highlight_item<display_menu.item_count)
	{
		display_menu.pItems[SMD_highlight_item].pEntry();
	}
	else
	{
		int index = SMD_highlight_item -display_menu.item_count;
		unsigned int iappid;
        char*   url;
/*
		if(SMD_highlight_item==(SMD_menu_num-1))
		{
			if(SMD_listctrl.number_of_items)//help about
			{
				//SMDShowMessageHelp();
			}
			else
			{
				//SMDShowMessageAbout();
			}
		}
		else*/
		{
			index = SMDTransferHighlight2Index(index);
            if(SMD_listctrl.node[index].itemType == 1)
            {
                iappid = SMD_listctrl.node[index].appid;
#ifdef APPLE_LOG
	OEMOS_dbgprintf("EntrySxmobiSMD_LSK index:%d display_menu.item_count %d,iappid is %x", index,display_menu.item_count,iappid);
#endif
                if(iappid)
                {
                    SxmobiSMDStartApp(SMD_listctrl.entry_appid,iappid, NULL);
                }
                else
                {
                    SMD_listctrl.active_menu_index = SMD_listctrl.node[index].menu_index;
                    SMD_listctrl.active_item = index;
                    EntrySxmobiSMDSubMenu();
                }

            }
            else if(SMD_listctrl.node[index].itemType == 2)
            {
                url = (char*) SMD_listctrl.node[index].appid;
                if(url && strlen(url) > 0)
                {
#ifdef APPLE_LOG
                        OEMOS_dbgprintf("EntrySxmobiSMD_LSK url=%s", url);
#endif

                    SMD_URL_ConnectUrl(url);
                }
            }


		}
	}
}
static void EntrySxmobiSMD_RSK(void)
{
			int index;
			char* list_of_items[MAX_SUB_MENUS];
			S32 number_of_items;
			int i;
			SMD_listctrl.active_menu_index = 0;
				//SMD_listctrl.active_item = index;
				for(i=0;i< display_menu.item_count;i++)
	{
	#if 0
		if(display_menu.pItems[i].title_str_type == 0)
			list_of_items[i]=(U8*)GetString(display_menu.pItems[i].title);
		else 
	#endif
			list_of_items[i]=(char*)display_menu.pItems[i].title;
	}
	number_of_items = display_menu.item_count;
				EntrySxmobiSMDSubMenu();
			
		
		
}
extern  unsigned int SMDLoadDataFile(unsigned int menu_bind_appid);
void EntrySxmobiSMD(void) 
{ 
	
	S32 number_of_items;
	char* list_of_items[MAX_SUB_MENUS];
	char* guiBuffer; 
	int i;
	int ishelp = 0;
	char  listitemtext[256] = {0};

 

	for(i=0;i< display_menu.item_count;i++)
	{
	#if 0
		if(display_menu.pItems[i].title_str_type == 0)
			list_of_items[i]=(U8*)GetString(display_menu.pItems[i].title);
		else 
	#endif
			list_of_items[i]=(char*)display_menu.pItems[i].title;
	}
	number_of_items = display_menu.item_count;
 
	SMDLoadDataFile(display_menu.app_id);	
	number_of_items += SMDLoadSubMenu(&list_of_items[number_of_items], display_menu.item_count);
	/* 
	if(number_of_items < (MAX_SUB_MENUS-3))
	{
		if(SMD_listctrl.number_of_items)
		{
			list_of_items[number_of_items]=(char*)U_Help;
		}
		else
		{
			list_of_items[number_of_items]=(char*)U_About;
		}
		number_of_items++;
		
	}*/
	SMD_menu_num = number_of_items; 
	
	OEM_ListCtl_RemoveAllItems();
	for(i=0;i<SMD_menu_num;i++) 
	{ 
	#ifndef __SXMOBI_NO_ICONIMAGE__
		 ListCtl_AppendListItem(OEM_LIST_ITEM_STYLE_ONE_LINE_ICON_TEXT,OEM_LISTCTL_DATA_TEXT_IMAGEID,i,
		OEM_LISTCTL_DATA_TEXT_BUFFER,list_of_items[i],0);
	#else
		
		memset(listitemtext,0x0,256);
		OEM_wsprintf(listitemtext,256,L"%d. %s",i+1,(wchar*)list_of_items[i]);
		 ListCtl_AppendListItem(OEM_LIST_ITEM_STYLE_ONE_LINE_TEXT,
		OEM_LISTCTL_DATA_TEXT_BUFFER,listitemtext,0);
	#endif
	}
#ifdef APPLE_LOG    
		OEMOS_dbgprintf("SMD_menu_num is %d" ,SMD_menu_num);
#endif		

	OEM_ListCtl_SetCurItemIndex(SMD_highlight_item);
	OEM_update_screen();
} 

void APPLE_StartUMVchat_SMD(void)
{
	//display_menu.tilte_str = "HELLO";
	display_menu.pItems = &VCMenuItem[0];
	display_menu.item_count = sizeof(VCMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x0300009a;
	
	EntrySxmobiSMD();
}

void SMDMenuTv1(void)
{
	SxmobiSMDStartApp(0x03000041,0x03000041, "args:home:liv");
}
static SXM_MenuItem TV1MenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuTv1},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void APPLE_StartTV1_SMD(void)
{
	display_menu.pItems = &TV1MenuItem[0];
	display_menu.item_count = sizeof(TV1MenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x03000041;
	
	EntrySxmobiSMD();
}

void SMDMenuTv2(void)
{
	SxmobiSMDStartApp(0x03000041,0x03000041, "args:home:mov");
}
static SXM_MenuItem TV2MenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuTv2},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void APPLE_StartTV2_SMD(void)
{
	display_menu.pItems = &TV2MenuItem[0];
	display_menu.item_count = sizeof(TV2MenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x03000041;
	
	EntrySxmobiSMD();
}


void SMDMenuTv3(void)
{
	SxmobiSMDStartApp(0x03000041,0x03000041, "args:home:vid");
}
static SXM_MenuItem TV3MenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuTv3},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void APPLE_StartTV3_SMD(void)
{
	display_menu.pItems = &TV3MenuItem[0];
	display_menu.item_count = sizeof(TV3MenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x03000041;
	
	EntrySxmobiSMD();
}

static SXM_MenuItem RPMenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuRun},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
static SXM_MenuItem TVMenuItem[]=
{
	{0,1,(unsigned int)U_TV1,SMDMenuTv1},
	{0,1,(unsigned int)U_TV2,SMDMenuTv2},
	{0,1,(unsigned int)U_TV3,SMDMenuTv3},
	#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
};
void APPLE_StartTV_SMD(void)
{
	display_menu.pItems = &TVMenuItem[0];
	display_menu.item_count = sizeof(TVMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x03000041;
	
	EntrySxmobiSMD();
}
void EntrySxmobiSMD_RADIO(void)
{
	//display_menu.tilte_str = (U8*)GetString(STR_ID_SXMOBI_TV);
	display_menu.pItems = &RPMenuItem[0];
	display_menu.item_count = sizeof(RPMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x03000093;
	
	EntrySxmobiSMD();
}

void SMDMenuLWSG(void)
{
	SxmobiSMDStartApp(0x030000B0,0x030000B0,NULL);
}
//OnLineGame
static SXM_MenuItem ONLIEMenuItem[]=
{
	{0,1,(unsigned int)U_lwsg,SMDMenuLWSG},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void EntrySxmobiSMD_ONLINEGAME(void)
{
	display_menu.pItems = &ONLIEMenuItem[0];
	display_menu.item_count = sizeof(ONLIEMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x030000B0;
	
	EntrySxmobiSMD();
}
//OnLineGame


//Music
static SXM_MenuItem MusicMenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuRun},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void EntrySxmobiSMD_Music(void)
{
	//display_menu.tilte_str = (U8*)GetString(STR_ID_SXMOBI_TV);
	display_menu.pItems = &MusicMenuItem[0];
	display_menu.item_count = sizeof(MusicMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x030000A8;
	
	EntrySxmobiSMD();
}
//Music
//VIDELDL
static SXM_MenuItem VIDELDLMenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuRun},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void EntrySxmobiSMD_VIDELDL(void)
{
	//display_menu.tilte_str = (U8*)GetString(STR_ID_SXMOBI_TV);
	display_menu.pItems = &VIDELDLMenuItem[0];
	display_menu.item_count = sizeof(VIDELDLMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x030000A9;

	EntrySxmobiSMD();
}
//VIDELDL

static SXM_MenuItem BookMenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuRun},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void EntrySxmobiSMD_Book(void)
{
	display_menu.pItems = &BookMenuItem[0];
	display_menu.item_count = sizeof(BookMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x030000ED;

	EntrySxmobiSMD();
}
//Weather
static SXM_MenuItem WEATHERMenuItem[]=
{
	{0,1,(unsigned int)U_Run,SMDMenuRun},
		#ifdef __UM_DUAL_SIM__
	{0,1,(unsigned int)U_Setup,EntrySMDSetup},
	#endif
	{0,1,(unsigned int)U_Update,SMDMenuUpdate},
};
void EntrySxmobiSMD_Weather(void)
{
	//display_menu.tilte_str = (U8*)GetString(STR_ID_SXMOBI_TV);
	display_menu.pItems = &WEATHERMenuItem[0];
	display_menu.item_count = sizeof(WEATHERMenuItem)/sizeof(SXM_MenuItem);
	display_menu.app_id = 0x03000026;
	
	EntrySxmobiSMD();
}
//Weather
int OEM_WCSLEN(const uint16 *str)
{
	return MMIAPICOM_Wstrlen(str);
}

void OEM_SetTitleText(uint16* pTitle,boolean bNeedUpdate)
{
	GUIWIN_SetTitleText(APPLE_SMD_WIN_ID,pTitle,OEM_WCSLEN(pTitle),bNeedUpdate);
}
void OEM_SetTitleTextID(MMI_TEXT_ID_T Title,boolean bNeedUpdate)
{
	GUIWIN_SetTitleTextId(APPLE_SMD_WIN_ID,Title,bNeedUpdate);
}
void EntrySxmobiSMD_APP(void)
{
	switch(start_appid)
		{

			case SXH_UMVCHAT:
				OEM_SetTitleTextID(TXT_SXH_UMVCHAT,TRUE);
				APPLE_StartUMVchat_SMD();
				break;
			case SXH_TV:
				OEM_SetTitleTextID(TXT_SXH_TV,FALSE);
				APPLE_StartTV_SMD();
				break;
			case SXH_TV1:
				OEM_SetTitleTextID(TXT_SXH_TV1,FALSE);
				APPLE_StartTV1_SMD();
				break;
			case SXH_TV2:
				OEM_SetTitleTextID(TXT_SXH_TV2,TRUE);
				APPLE_StartTV2_SMD();
				break;
			case SXH_TV3:
				OEM_SetTitleTextID(TXT_SXH_TV3,TRUE);
				APPLE_StartTV3_SMD();
				break;
			case SXH_RADIO:
				OEM_SetTitleTextID(TXT_SXH_RADIO,TRUE);
				EntrySxmobiSMD_RADIO();
			break;
				case SXH_ONLINEGAME:
				OEM_SetTitleTextID(TXT_SXH_Games,TRUE);
				EntrySxmobiSMD_ONLINEGAME();
			break;
			case SXH_WEATHER:
				OEM_SetTitleTextID(TXT_SXH_WEATHER,TRUE);
				EntrySxmobiSMD_Weather();
			break;
			case SXH_MUSIC:
				OEM_SetTitleTextID(TXT_SXH_MUSIC,TRUE);
				EntrySxmobiSMD_Music();
			break;
			case SXH_VIDELDL:
				OEM_SetTitleTextID(TXT_SXH_VIDEODL,TRUE);
				EntrySxmobiSMD_VIDELDL();
			break;
      		case SXH_BOOK:
			OEM_SetTitleTextID(TXT_SXH_BOOK,TRUE);
			EntrySxmobiSMD_Book();
			break;
			default:
 
			break;
		}
}
#include "lcd_cfg.h"
extern void* OEM_ListCtl_Create(apple_rect listrect,uint16 maxitemnum,OEM_LIST_TYPE listType);


LOCAL MMI_RESULT_E HandleTESTWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
	uint32           button_num    = 0;
	GUI_RECT_T       rect          = {0};
	MMI_RESULT_E     recode        = MMI_RESULT_TRUE;
	//GUI_BUTTON_BG_T  button_bg     = {GUI_BUTTON_BG_NULL, 0, 0, 0, 0};
	apple_rect test;
	
#ifdef APPLE_LOG    
	//OEMOS_dbgprintf("HandleTESTWinMsg is msg_id %d,win_id %x,APPLE_SMD_WIN_IDis %d",msg_id,win_id,APPLE_SMD_WIN_ID);
#endif		

	switch(msg_id)
	{
		
	case MSG_OPEN_WINDOW:
		{
#ifdef APPLE_LOG    
			OEMOS_dbgprintf("HandleTESTWinMsg is ok" );
#endif		
			gSmdWinId = win_id;
			test.x=0;
			test.y=OEM_GetTitleHeight();
			test.dx =OEMOS_GetLCDWidth();
			test.dy=OEMOS_GetLCDHeight()-OEM_GetStatusBarTop()-OEM_GetTitleHeight();
			 OEM_ListCtl_Create(test,30,1);
			 MMK_SetAtvCtrl(APPLE_SMD_WIN_ID,LIST_ID_DMS);
			 EntrySxmobiSMD_APP();
			// EntrySxmobiSMD_UMVchat();
			 GUIWIN_SetSoftkeyTextId(win_id,  STXT_EQUAL, TXT_NULL,STXT_RETURN,FALSE);
		        GUIWIN_SetSoftkeyBtnState(win_id, 2, FALSE, TRUE);
		}
		break;
	case MSG_KEYDOWN_DOWN:
    	case MSG_CTL_LIST_MOVEBOTTOM:
    	case MSG_CTL_LIST_MOVETOP:
    	case MSG_CTL_LIST_MOVEDOWN:
    	case MSG_CTL_LIST_MOVEUP: 
    	case MSG_CTL_LIST_NXTPAGE:
    	case MSG_CTL_LIST_PREPAGE:
		{
			MMK_SetAtvCtrl(APPLE_SMD_WIN_ID,LIST_ID_DMS);
			SMD_highlight_item=	OEM_ListCtl_GetCuriItemIndex();
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
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("HandleTESTWinMsg msg_id:%d ", msg_id);
#endif		
			EntrySxmobiSMD_LSK();
    		}
		break;
	case MSG_FULL_PAINT:
	#ifdef APPLE_LOG	
		OEMOS_dbgprintf("HandleTESTWinMsg   MSG_FULL_PAINT %d",OEM_ListCtl_IsActive() );
	#endif
		break;
	case MSG_GET_FOCUS:
		{
#ifdef APPLE_LOG    
			OEMOS_dbgprintf("MSG_GET_FOCUS is ok list is active %d",OEM_ListCtl_IsActive() );
#endif		
			if(!OEM_ListCtl_IsActive())
			{
				test.x=0;
				test.y=OEM_GetTitleHeight();
				test.dx =OEMOS_GetLCDWidth();
				test.dy=OEMOS_GetLCDHeight()-OEM_GetStatusBarTop()-OEM_GetTitleHeight();
				start_appid =save_startappid ;
				OEM_ListCtl_Create(test,30,1);
				 EntrySxmobiSMD_APP();
				 GUIWIN_SetSoftkeyTextId(win_id,  STXT_EQUAL, TXT_NULL,STXT_RETURN,FALSE);
			        GUIWIN_SetSoftkeyBtnState(win_id, 2, FALSE, TRUE);
					GUIWIN_UpdateTitle(win_id);
			}
							start_appid =save_startappid ;
			//OEM_update_screen();
		}
		break;
		case MSG_LOSE_FOCUS:
		{ 
#ifdef APPLE_LOG    
			OEMOS_dbgprintf("MSG_LOSE_FOCUS is ok list is active %d",OEM_ListCtl_IsActive() );
#endif		
			save_startappid =start_appid ;
			//if(OEM_ListCtl_IsActive())
			//	OEM_ListCtl_Destroy();
		}
		break;
    	case MSG_CTL_CANCEL:
		
		{
			if(SMD_listctrl.active_menu_index==0)
			{
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("MSG_CTL_CANCEL is ok win_id %d",win_id);
#endif		
				MMK_CloseWin(win_id);
			}
			else
			{
			EntrySxmobiSMD();
			}
    		}
		break;
		case MSG_KEYUP_RED:
			{
				MMK_CloseWin(win_id);
			}
		break;
		case MSG_KEYLONG_POWER://sxh az 20120508
		{
			recode = MMI_RESULT_FALSE;
		}
		break;
	case MSG_KEYUP_POWER://sxh az 20120508
		{
			recode = MMI_RESULT_FALSE;
		}
		break;

       #if 0 
       case MSG_APP_HOME://sxh az 20110802
                {
                      recode =MMI_RESULT_FALSE;
                 }
       break;
	   #endif
        case SMD_BASE_MSG_ID:
            {
                SMD_URL_ConnectCB(NULL);
                #ifdef APPLE_LOG
                OEMOS_dbgprintf("SMD_BASE_MSG_ID \r\n");
                #endif
            }
        break;
		case MSG_CLOSE_WINDOW:
			SMD_highlight_item =0;
            gSmdWinId = 0;
			OEM_ListCtl_Destroy();
			DelSxmobiDMS();
#ifdef APPLE_LOG    
	OEMOS_dbgprintf("MSG_CLOSE_WINDOW is ok ");
#endif		

			break;
	default:
			break;

	
	}
	return recode;
}
//smd
#ifdef SXMOBI_UMMUSIC
void APPLE_StartAppEx(unsigned int appid,char* arg)
{
	iAppId = appid;
	if (arg)
	{
		memset(strAppArg,0,sizeof(strAppArg));
		strcpy(strAppArg,arg);
	}
	Apple_CreateMainWindow();
}
#endif

#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
