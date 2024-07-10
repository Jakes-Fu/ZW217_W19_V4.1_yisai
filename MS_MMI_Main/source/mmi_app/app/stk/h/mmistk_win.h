/****************************************************************************
** File Name:      mmistk_win.h							                                                 *
** Author:          allen								                                                 *
** Date:             2004.08.17														*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      				*
** Description:    									               					*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 08/2004      allen               Create
** 
****************************************************************************/
#ifndef  _MMISTK_WIN_H_
#define  _MMISTK_WIN_H_   

#include "mmk_app.h"
#include "guicommon.h"
#include "guitext.h"
#include "mmistk_id.h"
#include "mn_type.h"
#include "simat_data_object.h"
#include "mmipub.h"
#ifdef __cplusplus
    extern "C"
    {
#endif

typedef struct
{
	GUI_FONT_T 					text_font;      
	GUI_COLOR_T 				text_color;    
	GUI_COLOR_T 				bkgd_color;    
	uint8						char_space;
	uint8						line_space;
	uint16						scrollbar_width;
} MMI_STK_DISPLAY_T;	

typedef struct _MMISTK_CHANNELINFO_T_
{
    uint8 channel_id;
    BOOLEAN is_used;
    BOOLEAN is_enable;
}MMISTK_CHANNELINFO_T;

typedef struct _MMISTK_PDPINFO_T_
{
    uint8   apn_text[MN_SIMAT_NETWORK_ACCESS_NAME_LEN];
    uint8   login_text[MN_MAX_SIMAT_DATA_OBJECT_LEN]; 
    uint8   psw_text[MN_MAX_SIMAT_DATA_OBJECT_LEN]; 
}MMISTK_PDPINFO_T;

typedef enum 
{
     MMISTK_PDP_STATUS_NONE,  //未连接     
     MMISTK_PDP_STATUS_ACTIVING, //开始连接 
     MMISTK_PDP_STATUS_ACTIVE_OK,//已连接
     MMISTK_PDP_STATUS_DEACTIVING,//正在断开连接
     MMISTK_PDP_STATUS_DEACTIVED //断开连接
}MMISTK_PDP_STATUS_E;

/********************************************************************************
 NAME:			MMISTK_OnSetupMenuCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnSetupMenuCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSelectItemCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnSelectItemCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnDisplayTextCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnDisplayTextCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnGetInkeyCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnGetInkeyCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnGetInputCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
void MMISTK_OnGetInputCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnMoreTimeCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		guisen
 DATE:			2005.08.31
********************************************************************************/
void MMISTK_OnMoreTimeCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnSendSmsCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.27
********************************************************************************/
void MMISTK_OnSendSmsCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSetupCallCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.28
********************************************************************************/
void MMISTK_OnSetupCallCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnPlayToneCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnPlayToneCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnProvideLICmd
 DESCRIPTION:	when user use the LI(Location Information) function, SIMTK will send
 				this command to MMI, MMI should collect some infomation like:
 				MCC(mobile country code), MNC(mobile network code), LAC(local area code)
 				and IMEI..., etc, then invoke relevant Cnf function to send them to SIMTK,
 				SIMTK will support the LI to user base on these infor.
 				this function don't need user input any data to get LI usually.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnProvideLICmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSendSSCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnSendSSCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSendUSSDCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.30
********************************************************************************/
void MMISTK_OnSendUSSDCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnRefreshCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2008.04.24
********************************************************************************/
void MMISTK_OnRefreshCnf(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnRefreshCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnRefreshCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSetupEventlistCmd
 DESCRIPTION:	some SIMTK will be care of some things of MMI layer, for example: call, SMS .. etc.
 				SIMTK can inform MMI that which he will care by send this cmd. after received this cmd,
 				MMI can copy them in local, and when some case(SIMTK care) occur in MMI layer,
 				MMI can invoke the Ind function like SIMAT_IndMTCall(), SIMAT_IndMOCC(),.. etc to
 				inform SIMTK layer, SIMTK will do some things base on this action, ep: saved the last call number.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSetupEventlistCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSetupIdletextCmd
 DESCRIPTION:	SIMTK can support a text to MMIi layer,
 				MMI can use it to show in idle screen if need.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSetupIdletextCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSendDtmfCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSendDtmfCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnLangNotifyCmd
 DESCRIPTION:	some SIMTK will send this cmd to notify MMI that 
				the following text from SIMTK will be in which language.
 				!!!!! note:  this cmd need not MMI send Cnf to SIMTK,
 				!!!!! the Cnf was done by SIMTK layer self.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnLangNotifyCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnCcToMmiCmd
 DESCRIPTION:	.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2010.10.05
********************************************************************************/
void MMISTK_OnCcToMmiCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnSSControlRspCmd
 DESCRIPTION:	if you want SIMTK to affect the supply service by SIMTK, you can 
 				invoke Ind function to notify SIMTK layer before a SS code be
 				sent, then SIMTK will send this cmd to response MMI that 
 				the SS what should to do.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSSControlRspCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnUSSDControlRspCmd
 DESCRIPTION:	if you want SIMTK to affect the unstructed supply service data by SIMTK,
 				you can invoke Ind function to notify SIMTK layer before a USSD be
 				sent, then SIMTK will send this cmd to response MMI that 
 				the USSD what should to do.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnUSSDControlRspCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnSmsCtlToMmiCmd
 DESCRIPTION:	.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			2010.10.05
********************************************************************************/
void MMISTK_OnSmsCtlToMmiCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnSMSP2PRspCmd
 DESCRIPTION:	SIMTK can support dynamic menu managerment. SIMTK server can send
 				the dynamic menu information in a SMS to ME, the MN layer should parse the 
 				special SMS to SIMTK bypass the user. this means PP(point to point).
 				after SIMTK received the parsed result, SIMTK will response this signal.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSMSP2PRspCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSMSCBRspCmd
 DESCRIPTION:	SIMTK can support dynamic menu managerment. SIMTK server can send
 				the dynamic menu information in a CB to ME, the MN layer should parse the 
 				special SMS to SIMTK bypass the user. 
 				after SIMTK received the parsed result, SIMTK will response this signal.
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.31
********************************************************************************/
void MMISTK_OnSMSCBRspCmd(void* arg);


/********************************************************************************
 NAME:			MMISTK_OnSessionTerminateCmd
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.19
********************************************************************************/
void MMISTK_OnSessionTerminateCmd(void* arg);

/*****************************************************************************/
//  Description : get current used sim for stk
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
MN_DUAL_SYS_E MMISTK_GetSTKSim(void);

/********************************************************************************
 NAME:			MMISTK_MainMenuWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
MMI_RESULT_E MMISTK_MainMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/********************************************************************************
 NAME:			MMISTK_OnLaunchBrowserCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		wancan.you
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnLaunchBrowserCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnOpenChannelCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnOpenChannelCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnCloseChannelCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnCloseChannelCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnGetChannelStatusCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnGetChannelStatusCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnSendDataCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnSendDataCmd(void* arg);

/********************************************************************************
 NAME:			MMISTK_OnReceiveDataCmd
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		kun.yu
 DATE:			
********************************************************************************/
PUBLIC void MMISTK_OnReceiveDataCmd(void* arg);

/********************************************************************************
 NAME:      MMISTK_SetupCall
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMISTK_SetupCall(MN_DUAL_SYS_E dual_sys, 
                                BOOLEAN is_oper_cnf,
                                BOOLEAN is_call_active,
                                MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

/*****************************************************************************/
// 	Description : MMISTK_GetStkLang
//	Global resource dependence : 
//   Author: wancna.you
//	Note:
/*****************************************************************************/
PUBLIC void MMISTK_GetStkLang(SIMAT_DO_LANG_T *lang_ptr);

/*****************************************************************************/
// 	Description : MMISTK_IsStkRefresh
//	Global resource dependence : 
//   Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISTK_IsStkRefresh(void);

/*****************************************************************************/
// 	Description : MMISTK_OpenAlertWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMISTK_OpenAlertWin(MMI_TEXT_ID_T text_id, MMI_IMAGE_ID_T image_id, MMIPUB_HANDLE_FUNC handle_func);
/*****************************************************************************/
//  Description : MMISTK_OpenAlertWinEx
//  Global resource dependence :
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void MMISTK_OpenAlertWinByTextPtr(MMI_STRING_T   *text1_ptr);

/*****************************************************************************/
//     Description : Get Stk Name
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISTK_GetStkName(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_name_ptr);

#ifdef __cplusplus
    }
#endif

#endif

