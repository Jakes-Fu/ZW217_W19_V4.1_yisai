/****************************************************************************
** File Name:      mmiWre_app.c                            
** Author:                                                                  
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define game module outside API .
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/   
#ifdef WRE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmiwre.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmiwre_id.h"
#include "tb_dal.h"
#include "mmiwre.h"
#include "mmi_textfun.h"
#include "mmiwre_text.h"
#include "mmiwre_menutable.h"
#include "http_api.h"
#include "in_message.h"
#include "mmi_signal_ext.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipdp_export.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmiwre_other.h"
   
#include "mmiwre_adaptor.h"
#include "mmi_applet_table.h"
#include "mmiwre_export.h"	// changed by yifei

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                   FUNCTION DEFINITION                                                                               */
/*---------------------------------------------------------------------------*/

extern int MMIWRE_StartNormal(uint8 appType, uint32 appId, uint16 *appName, void *data);	// changed by yifei
PUBLIC void MMIWRE_RegAppletInfo(void);

/*****************************************************************************/
// 	Description : MMIAPIWRE_InitModule
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWRE_InitModule(void)
{
	MMIWRE_RegWreNv();
    MMIWRE_RegAppletInfo();
}

/*****************************************************************************/
// 	Description : WRE_CheckUDiskRunState
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN WRE_CheckUDiskRunState(void)
{
	if(MMIAPIUDISK_UdiskIsRun())  
	{
		MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
		return FALSE;
	}

	return TRUE;

}

/*****************************************************************************/
// 	Description : WRE_ShowWarningBox
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void WRE_ShowWarningBox(void)
{
    MMIPUB_OpenAlertWinByTextIdEx(SPRD_WRE_APPLET_ID, PNULL, 
		TXT_WRE_EXE_NOEXISTED,
		TXT_NULL,
		IMAGE_PUBWIN_WARNING,
		PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
}

/*****************************************************************************/
// 	Description : MMIAPIWRE_Init
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWRE_Init(void)
{
    WRE_Init();
}

extern void WRE_Reset(void);
/*****************************************************************************/
// 	Description : MMIAPIWRE_Init
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWRE_Reset(void)
{
    WRE_Reset();
}
#ifdef QBTHEME_SUPPORT
extern void MMIWRE_CloseQbTheme(void);
extern void MMIWRE_StartQbTheme(void);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  WREAppletHandleEvent(    
                                       IAPPLET_T*          iapplet_ptr,
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_HANDLE_T     applet_handle = PNULL;
    MMIWRE_APPLET_T* applet_ptr = (MMIWRE_APPLET_T*)iapplet_ptr;

    SCI_TRACE_LOW("[WRE]WREAppletHandleEvent: msg_id = 0x%X", msg_id);

    switch (msg_id)
    {
    case MSG_APPLET_START:
        {
            if (applet_ptr != NULL && param != NULL)
            {
                int iRet = 0;
                MMIWRE_INSTANCE_T *lpInstance = (MMIWRE_INSTANCE_T *)param;
                memcpy(&(applet_ptr->instance), lpInstance, sizeof(MMIWRE_INSTANCE_T));

            #ifdef QBTHEME_SUPPORT
                //MMIWRE_CloseQbTheme();
            #endif
                iRet = MMIWRE_StartNormal(applet_ptr->instance.appType,
                    applet_ptr->instance.appId,
                    applet_ptr->instance.appName,
                    applet_ptr->instance.data);

                SCI_TRACE_LOW("[WRE]MMIWRE_StartNormal return %d", iRet);
                
                if (iRet == 0)
                {
                    MMK_CloseApplet(SPRD_WRE_APPLET_ID);
                    
                    return MMI_RESULT_FALSE;
                }
            }            
        }
        break;
        
    case MSG_APPLET_STOP: 
        {
            if (applet_ptr != NULL)
            {
                if (applet_ptr->instance.appName)
                {
                    SCI_FREE(applet_ptr->instance.appName);
                    applet_ptr->instance.appName = NULL;
                    applet_ptr->instance.appNameLen = 0;
                }
                
                if (applet_ptr->instance.data)
                {
                    SCI_FREE(applet_ptr->instance.data);
                    applet_ptr->instance.data = NULL;
                    applet_ptr->instance.dataLen = 0;
                }

                if (WBOOT_IsWRERunning())
                {
                    MMIWRE_CloseWin();
                }                
            }
        }
        break;
        
    case MSG_APPLET_SUSPEND:
    #ifdef QBTHEME_SUPPORT
        //MMIWRE_StartQbTheme();
    #endif
        break;
    case MSG_APPLET_RESUME: 
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiwre_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_WRE_APPLET_ID), sizeof(MMIWRE_APPLET_T), WREAppletHandleEvent, 
        IMAGE_MAINMENU_ICON_STORE, TXT_WRE_ENTERTAINMENT}, 
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmiwre_applet_info = 
{
    s_mmiwre_applet_array,
        ARR_SIZE(s_mmiwre_applet_array)
};

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_WRE, &g_mmiwre_applet_info);
}



#endif
