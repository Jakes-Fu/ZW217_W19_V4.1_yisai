/****************************************************************************
** File Name:      mmistk_app.h							                    *
** Author:          allen								                    *
** Date:             2004.08.17						        				*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    									               		    *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 08/2004      allen               Create
** 
****************************************************************************/
#ifndef  _MMISTK_APP_H_    
#define  _MMISTK_APP_H_   

#include "mmk_app.h"
#include "mmidisplay_data.h"
#include "guimenu.h"
#include "guiimg.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMISTK_MENU_ITEMSTR_LEN 			(255*8/7)
#define MMISTK_MENU_MAX_ITEM				45
#define MMISTK_MENU_TITLE_LEN				(255*8/7)
#define MMISTK_STRING_MAX_LEN				(255*8/7)
#define MMISTK_SELECTITEM_MAX_ITEM		45

typedef struct nextitem
{
    uint8 nextActionIndicator;
    uint8 iconID;							// no use now
    uint8 itemID;
    uint8 length;
    wchar stringData[MMISTK_MENU_ITEMSTR_LEN + 1];
    GUIIMG_BITMAP_T     icon_bmp;              
    struct nextitem* 	next;							// no use now
} STK_MENU_ITEM_T;

typedef struct
{
    STK_MENU_ITEM_T *menu_item;	// STK menu items
    uint8 menu_item_amount;								// STK menu item amount
    wchar menu_title[MMISTK_MENU_TITLE_LEN + 1];			// STK menu title
    uint8 menu_title_len;									// the length of STK menu title
    BOOLEAN is_action_id;
    BOOLEAN is_icon_id;								// STK menu group's icon id
    BOOLEAN is_icon_list;								// STK menu group's icon id
    BOOLEAN is_icon_list_self_explan;                   
    uint8 menu_title_icon_id;								// STK menu group's icon id
    BOOLEAN is_menu_title_icon_self_explan;             
    GUIIMG_BITMAP_T menu_title_icon_bmp;              
    BOOLEAN help_info;                                                              // SYK menu help_info
} STK_MENU_INFO_T;

typedef enum
{
    MMISTK_CLOSE_NONE,
    MMISTK_CLOSE_DISPLAYTEXT_WIN,
    MMISTK_CLOSE_GETINPUT_WIN,
    MMISTK_CLOSE_GETINKEY_WIN,
    MMISTK_CLOSE_SENDSMS_WIN,
    MMISTK_CLOSE_PLAYTONE_WIN,
    MMISTK_CLOSE_SENDSS_WIN,
    MMISTK_CLOSE_SETUP_CALL_WIN,
    MMISTK_CLOSE_LAUNCH_BROWSER_WIN,
    MMISTK_CLOSE_WIN_MAX
} MMISTK_CLOSE_WIN_E;

/********************************************************************************
 NAME:			MMISTK_IsStkCard
 DESCRIPTION:	whether STK card
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.18
********************************************************************************/
PUBLIC BOOLEAN MMISTK_IsStkCard(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:			MMISTK_IsSendSms
 DESCRIPTION:	whether send SMS by STK
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.11.18
********************************************************************************/
PUBLIC BOOLEAN MMISTK_IsSendSms(void);

/*****************************************************************************/
//  Description : set current used sim for stk
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
void MMISTK_SetSTKSim(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:			MMISTK_SendSMSPDUCnf
 DESCRIPTION:	send SMS Cnf to STK layer
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.01
********************************************************************************/
PUBLIC void MMISTK_SendSMSPDUCnf(MN_DUAL_SYS_E dual_sys, MN_SMS_CAUSE_E send_result);

/********************************************************************************
 NAME:			MMISTK_EnterToplevelMenu
 DESCRIPTION:	enter STK top level menu from main menu
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMISTK_EnterToplevelMenu(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:                  MMISTK_AppHandlePsMsg
 DESCRIPTION:	
 PARAM IN:
 PARAM OUT:
 AUTHOR:                allen
 DATE:                  2004.08.17
********************************************************************************/
PUBLIC MMI_RESULT_E MMISTK_AppHandlePsMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/********************************************************************************
 NAME:			MMISTK_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMISTK_AppInit(void);

/********************************************************************************
 NAME:			MMISTK_OnSendRegisterPaswdCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMISTK_OnSendRegisterPaswdCnf(void *arg);

/********************************************************************************
 NAME:			MMISTK_OnSendSSCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMISTK_OnSendSSCnf(void *arg);

/********************************************************************************
 NAME:			MMISTK_OnSendUSSCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMISTK_OnSendUSSDCnf(void *arg);

/********************************************************************************
 NAME:      MMISTK_OnRelUSSD
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMISTK_OnRelUSSD(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : handle stk timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMISTK_HandleSTKTimer(uint8 timer_id, uint32 param);

/********************************************************************************
 NAME:      MMISTK_BrowserTermination
 DESCRIPTION:
 AUTHOR: kun.yu
 DATE:		
********************************************************************************/
PUBLIC void MMISTK_BrowserTermination(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Select SIM API of STK app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISTK_SelectSIMFunc(void);

/*****************************************************************************/
//  Description : get type of STK refresh
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC SIMAT_REFRESH_TYPE_E MMISTK_GetStkRefreshType(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set type of STK refresh is 0
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMISTK_SetStkRefreshType(MN_DUAL_SYS_E dual_sys, int val_type);

/*****************************************************************************/
//  Description : close refresh waiting win
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMISTK_CloseRefreshWaitingWin(void);

#ifdef __cplusplus
    }
#endif

#endif

