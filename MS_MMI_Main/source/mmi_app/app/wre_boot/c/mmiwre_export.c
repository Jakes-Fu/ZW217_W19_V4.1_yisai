#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT
/****************************************************************************
** File Name:      mmiwre_export.c
** Author:                                                                  
** Date:            2011/08/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define game module outside API .
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 08/01/2011       qingjun.yu          Create
** 
****************************************************************************/
#include "mmk_msg.h"
#include "watchdog.h"
#include "mmi_module.h"
#include "mmiwre_other.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_export.h"
#include "mmiwre.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmiwre_id.h"
#include "mmi_appmsg.h"
#include "mmisms_text.h"
#include "mmicc_export.h"
#include "mmi_applet_conflict.h"
#include "mmi_mainmenu_synchronize.h"
#include "mmi_applet_table.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_export.h"
#endif
#include "mmi_mainmenu_export.h"
#include "mmicom_panel.h"

#ifdef SNS_PULLING_SUPPORT
#include "mmisns_srv.h"
#include "mmiudisk_export.h"
#include "mmiidle_statusbar.h"
#include "mmidc_export.h"
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMISET_IDLE_STYLE_E s_cur_style = MMISET_IDLE_STYLE_GRID;
LOCAL MMISET_WALLPAPER_SET_T s_wallpaper_info = {0};
LOCAL uint32 s_entrance = WRE_ENTRANCE_FROM_MENU;
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
INSTALLJAVACALLBACKFUNC * g_pInStallJavaFunc = NULL;
void * g_pSaveParam =NULL;
BOOL OneKeyDownloading = FALSE;

/*****************************************************************************/
//  Description : forbid the sweet menu before enter wre.
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_ForbibSweetMenu(void);

/*****************************************************************************/
//  Description : revival the sweet menu after exit wre.
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_RevivalSweetMenu(void);

/*****************************************************************************/
//  Description : mmiwre qbtheme
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_CloseQbTheme(void);

/*****************************************************************************/
//  Description : mmwre start qbtheme
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_StartQbTheme(void);

extern void wre_init_global(void);
PUBLIC int MMIWRE_StartNormal(uint8 appType, uint32 appId, uint16 *appName, void *data);
PUBLIC int MMIWRE_StartNormalEx(uint8 appType, uint32 appId, uint16 *appName, void *data);
//PUBLIC int MMIWRE_StartConflictApplet(uint8 appType, uint32 appId, uint16 *appName, void *data);
MMI_RESULT_E MMIWRE_StartApplet(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : dispatch the WRE message 
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_DispatchWreMSG(void)
{
    WBOOT_DispatchMwinMSG();
}

/*****************************************************************************/
// 	Description : get WRE message
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GetWreMessageForMocor(void)
{
    return WBOOT_GetMwinMessageForMocor();
}

/*****************************************************************************/
// 	Description : NetworkIndHandle
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_NetworkIndHandle(DPARAM param)
{
     WRE_NETWORK_IND_Handle(param);
}

/*****************************************************************************/
// 	Description : SetDeepSleepStatus
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_SetDeepSleepStatus(BOOLEAN is_deep_sleep)
{
     WBOOT_WRE_SetDeepSleepStatus(is_deep_sleep);
}

/*****************************************************************************/
// 	Description : set watch dog pointer
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_SetWatchDogPtr(void *SwdgPtr)
{
    SetWatchDogPtr((SWDG_OBJECT *)SwdgPtr);
}


/*****************************************************************************/
//  Description :  Handle open waiting win
//  Global resource dependence : 
//  Author:wei.zhou
//  Note: 
/*****************************************************************************/
#if 0
LOCAL MMI_RESULT_E HandleWREOpenWaitingWin(
                                           MMI_WIN_ID_T       win_id,     
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    int             ret = WRE_INIT_OK;
    
    //SCI_TRACE_LOW:"[WRE] HandleWREOpenWaitingWin: msg = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_EXPORT_90_112_2_18_3_10_37_51,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_WRE_ENTRY_INIT:
        {            
            ret = WREEntry(0,NULL);
            //SCI_TRACE_LOW:"[WRE]   WREEntry return 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_EXPORT_97_112_2_18_3_10_37_52,(uint8*)"d", ret);
            if(ret == WRE_INIT_NO_ROOM)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
            }
            else if (ret == WRE_INIT_RUNNING)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_BUSY);
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_WRE_STARTAPP_INIT:
        {
            uint32 fix_app_id = *((uint32 *)param);
            ret = WREEntry(TYPE_WREFIX_APP,(void *)fix_app_id);
            //SCI_TRACE_LOW:"[WRE]   WREEntry return 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_EXPORT_115_112_2_18_3_10_37_53,(uint8*)"d", ret);
            if(ret == WRE_INIT_NO_ROOM)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
            }
            else if (ret == WRE_INIT_RUNNING)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_BUSY);
            }            

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_WRE_SYNSTARTUP_INIT:
        {
            PDYNAMIC_STARTUP_PARAM  pDynStartupParam = (PDYNAMIC_STARTUP_PARAM)param;
            WRE_DYN_Startup(pDynStartupParam->appid, pDynStartupParam->appname);
            SCI_FREE(pDynStartupParam->appname);

            MMK_CloseWin(win_id);
        }
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}
#endif
/*****************************************************************************/
//  Description : wreentry
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
PUBLIC int MMIWRE_Entry(void)
{
    return MMIWRE_StartConflictApplet(TYPE_WREAPP_STORE, 0, NULL, NULL);
}

/*****************************************************************************/
//  Description : MMIWRE_StartApp
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC int MMIWRE_StartApp(uint32 fix_app_id)
{
    return MMIWRE_StartConflictApplet(TYPE_WREFIX_APP, fix_app_id, NULL, NULL);
}

/*****************************************************************************/
//  Description : MMIWRE_DynStartup
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC int MMIWRE_DynStartup(uint32  appid, uint16 *appname)
{
    return MMIWRE_StartConflictApplet(TYPE_WREAPP_APP, appid, appname, NULL);
}


CONST SNS_APP_INFO_T  sns_app_list[MAX_FIXED_APP_NUM] =
{
    {WRE_FIX_ID_DIGGOLD,  L"D3D34282-469F-4e53-9896-527A50685C11", L"20400080000001", 0, 0, 0}, //
 #if !defined(WRE_WEIBO_SUPPORT)||defined(WIN32)
     {WRE_FIX_ID_SINA,     L"F05C8ECD-704E-4b98-A59D-EDCEF5EFF22B", L"20400080000001", L"e:\\wreapp\\sinaweibo\\sinaweibo.exe",  L"sinaweibo.exe", L"e:\\wreapp\\sinaweibo\\sinaweibo.png"}, //SINA
 #else
     {WRE_FIX_ID_SINA,     L"F05C8ECD-704E-4b98-A59D-EDCEF5EFF22B", L"20400080000001", L"c:\\wreapp\\sinaweibo\\sinaweibo.exe",  L"sinaweibo.exe", L"c:\\wreapp\\sinaweibo\\sinaweibo.png"}, //SINA
 #endif
 #if !defined(WRE_FACEBOOK_SUPPORT)||defined(WIN32)
     {WRE_FIX_ID_FACEBOOK, L"B442BAD1-1176-4522-920B-5221C93D84AF", L"10200010000002", L"e:\\wreapp\\facebook\\facebook.exe",    L"facebook.exe",  L"e:\\wreapp\\facebook\\facebook.png"}, //facebook
 #else
     {WRE_FIX_ID_FACEBOOK, L"B442BAD1-1176-4522-920B-5221C93D84AF", L"10200010000002", L"c:\\wreapp\\facebook\\facebook.exe",    L"facebook.exe",  L"c:\\wreapp\\facebook\\facebook.png"}, //facebook
 #endif
 #if !defined(WRE_TWITTER_SUPPORT)||defined(WIN32)
      {WRE_FIX_ID_TWITTER,  L"0B7C5DBA-F104-4ad4-A3A0-83574CF73E17", L"10100010000001", L"e:\\wreapp\\WRETwit\\WRETwit.exe",      L"WRETwit.exe",   L"e:\\wreapp\\WRETwit\\WRETwit.png"},//twitter
 #else
      {WRE_FIX_ID_TWITTER,  L"0B7C5DBA-F104-4ad4-A3A0-83574CF73E17", L"10100010000001", L"c:\\wreapp\\WRETwit\\WRETwit.exe",      L"WRETwit.exe",   L"c:\\wreapp\\WRETwit\\WRETwit.png"},//twitter
 #endif
#ifdef NES_SUPPORT
#ifdef WRE_CONTRA_SUPPORT
  #ifdef WIN32
    {WRE_FIX_ID_CONTRA,     L"04141EAB-1514-42af-BD0A-8A95E54EF03B", L"10100010001111", L"e:\\wreapp\\contra\\contra.exe",  L"contra.exe", L"e:\\wreapp\\contra\\contra.png"}, //contra
  #else
     {WRE_FIX_ID_CONTRA,     L"04141EAB-1514-42af-BD0A-8A95E54EF03B", L"10100010001111", L"C:\\wreapp\\contra\\contra.exe",  L"contra.exe", L"C:\\wreapp\\contra\\contra.png"}, //contra
  #endif
#endif
#ifdef WRE_SUPERMARIO_SUPPORT
  #ifdef WIN32
     {WRE_FIX_ID_SUPERMARIO,     L"A08541D7-4A93-4f09-9D97-1EE2656E726B", L"10100010001111", L"e:\\wreapp\\supermario\\supermario.exe",  L"supermario.exe", L"e:\\wreapp\\supermario\\supermario.png"}, //supermario
  #else
     {WRE_FIX_ID_SUPERMARIO,     L"A08541D7-4A93-4f09-9D97-1EE2656E726B", L"10100010001111", L"C:\\wreapp\\supermario\\supermario.exe",  L"supermario.exe", L"C:\\wreapp\\supermario\\supermario.png"}, //supermario
  #endif
#endif
#ifdef WRE_TSLAND_SUPPORT
  #ifdef WIN32
     {WRE_FIX_ID_TSLAND,     L"F27DAF24-BCE0-4015-B0D7-2CDEC15EF50F", L"10100010001111", L"e:\\wreapp\\tsland\\tsland.exe",  L"tsland.exe", L"e:\\wreapp\\tsland\\tsland.png"} //tsland
  #else
     {WRE_FIX_ID_TSLAND,     L"F27DAF24-BCE0-4015-B0D7-2CDEC15EF50F", L"10100010001111", L"C:\\wreapp\\tsland\\tsland.exe",  L"tsland.exe", L"C:\\wreapp\\tsland\\tsland.png"} //tsland
  #endif
#endif
#endif
};

/*****************************************************************************/
//  Description : MMIWRE_StartAppEx
//  Global resource dependence : 
//  Author: Langbiao.tan
//  Note: start up a app with data
/*****************************************************************************/
PUBLIC uint16* MMIWRE_GetAppGuid(uint32 appid)
{
    uint16 *guid = NULL;

    //SCI_TRACE_LOW:"[WRE]:MMIWRE_GetAppGuid appid = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_EXPORT_336_112_2_18_3_10_38_56,(uint8*)"d", appid);
    if(appid < MAX_FIXED_APP_NUM)
    {
       guid = sns_app_list[appid].guid;
    }

    return guid;
}

 PUBLIC BOOLEAN MMIWRE_GetFixAppPath(uint32 appid, uint16 * path,uint16 * appname,uint16 * iconpath)
 {
    if(appid < MAX_FIXED_APP_NUM)
    {
       //SCI_TRACE_LOW:"[WRE]MMIWRE_GetFixAppPath"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_EXPORT_349_112_2_18_3_10_38_57,(uint8*)"");
#ifdef WRE_VIRTUALDISK_SUPPORT	   
       MMIAPICOM_Wstrcpy(path, sns_app_list[appid].filepath);
       MMIAPICOM_Wstrcpy(appname, sns_app_list[appid].appname);
       MMIAPICOM_Wstrcpy(iconpath, sns_app_list[appid].iconpath);
#else
       return FindAppPathByAppID(appid, path, appname, iconpath);
#endif

  	    return TRUE;
    } 	

	return FALSE;
 }
/*****************************************************************************/
//  Description : MMIWRE_StartAppEx
//  Global resource dependence : 
//  Author: Langbiao.tan
//  Note: start up a app with data
/*****************************************************************************/
PUBLIC int MMIWRE_StartAppEx(uint32 appid, void *data)
{
    return MMIWRE_StartConflictApplet(TYPE_WRE_DATA_APP, appid, NULL, data);
}

PUBLIC int MMIWRE_StartAppMinimize(uint32 appid, void *data)
{
    char *Str1 = (char *)data;
    char Str2[64] = {0};

    SCI_TRACE_LOW("[WRE] MMIWRE_StartAppMinimize");
    SCI_STRCPY(Str2, "Mini");
    if(Str1)
    {
        strcat(Str2, Str1);
    }
    return MMIWRE_StartConflictApplet(TYPE_WRE_DATA_APP, appid, NULL, (void *)Str2);
}

#define MAX_WRE_APPNAME_LEN   32

PUBLIC int MMIWRE_StartNormalEx(uint8 appType, uint32 appId, uint16 *appName, void *data)
{
    uint16  filepath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    uint16  appfilename[MAX_WRE_APPNAME_LEN+1] = {0};
    uint16  iconpath[MAX_WRE_FILE_PATH_LEN+1] = {0};
    int ret = WRE_INIT_OK;
    void *param = NULL;
	{
		 extern int g_wre_kernel_norflash_startup;
		VERSION_T ver={0}; 
		WRE_GetBOOTVersion(&ver);
			SCI_TRACE_LOW("[WRE]BOOT VERSION:[%s][%d]",ver.version,g_wre_kernel_norflash_startup);

	}

    SCI_TRACE_LOW("[WRE]MMIWRE_StartNormalEx: appType = %d, appid = %d", \
        appType, appId);
    if (appName)
    {
        WRE_TraceWstr(appName);
    }
    if (data)
    {
        SCI_TRACE_LOW("%s", data);
    }

    if (!WRE_CheckUDiskRunState())
    {
        return WRE_INIT_UDISK_NORUNNING;
    }

  	if( !WRE_SDCardInsert() && appType == TYPE_WREAPP_STORE )
	{		
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_INSERT_MEMORYCARD);
        return WRE_INIT_NOSDCARD;
	}

    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        return WRE_INIT_INCALLING;
    }    

    if(MMIWRE_IsMocorAppBackRunning()) // Mocor App is running in background
    {
        MMIPUB_OpenAlertWarningWin( TXT_SET_MANUAL_UPDATE_TIME_PROMPT);
        return WRE_INIT_MOCORAPP_BACKRUNNING;
    }
    
    //MMIPUB_OpenTempWaitWin();   //heng.xiao del 20120608 --- do not need
/*
    if (WRE_MEMORY_MIN > SCI_GetSystemSpaceMaxAvalidSpace())
    {
        return WRE_INIT_NO_ROOM;
    }
*/
    if (appType == TYPE_WREAPP_APP)
    {	 
        if (!FindAppPathByAppID(appId, filepath, appfilename, iconpath)
            || MMIAPICOM_Wstrcmp(appfilename, appName))
        {
    	    WRE_ShowWarningBox();

            return WRE_INIT_APP_PARAERR;
        }

        MMIWRE_SetAppParam(filepath,appfilename,iconpath);
        param = filepath;
    }
    else if (appType == TYPE_WRE_DATA_APP)
    {
        uint16 *guid = MMIWRE_GetAppGuid(appId);
        BOOLEAN flag = FALSE;
        if(guid == NULL || !MMIWRE_GetFixAppPath(appId, filepath, appfilename, iconpath))
        {
    	    WRE_ShowWarningBox();

            return WRE_INIT_APP_PARAERR;
        }

        MMIWRE_SetAppParam(filepath,appfilename,iconpath);
        param = data;
            
    }
    else if (appType == TYPE_WREFIX_APP)
    {
        param = (uint32 *)appId;	// changed by yifei
    }
    else if (appType == TYPE_WREAPP_STORE)
    {
        param = NULL;
    }
    else
    {
        return WRE_INIT_APP_PARAERR;
    }

#ifdef WRE_VIRTUALDISK_SUPPORT		
    if (!WRE_CreateVirtualROMDisk())
    {
	  return WRE_INIT_VIRTUALDISK_FAIL;
    }
#endif

    ret = WREEntry(appType,(void *)param);
    SCI_TRACE_LOW("[WRE]   WREEntry return 0x%x", ret);
    
    if(ret == WRE_INIT_NO_ROOM)
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
    }

    if (ret != WRE_INIT_OK && ret != WRE_INIT_RUNNING && ret != WRE_INIT_QUERY_UPDATE)
    {
#ifdef WRE_VIRTUALDISK_SUPPORT	
		WRE_DestoryVirtualDisk();
#endif
    }
    
    return ret;
}


typedef struct _tagWRE_START_PARAM
{
    uint8 appType;
    uint32 appId;
    uint16 *appName;
    void *data;
}WRE_START_PARAM, *LPWRE_START_PARAM;

LOCAL MMI_RESULT_E HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E eResult = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int iRet = WRE_INIT_OK;
            LPWRE_START_PARAM lpStartParam = (LPWRE_START_PARAM)MMK_GetWinAddDataPtr(win_id);
            if (lpStartParam)
            {
                     /* fragment garbage process, by wei.zhou*/
                #if 0
                    BLKMEM_GarbageClean();
                    UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
                    #ifdef DYNAMIC_MAINMENU_SUPPORT
                    MMIMENU_DyDestroy();
                    #endif
                #endif
                    /* fragment centralize processing */
                    MMIWRE_GarbageClean();
                    iRet = MMIWRE_StartNormalEx(lpStartParam->appType, lpStartParam->appId, lpStartParam->appName, lpStartParam->data);
                    if (lpStartParam->appName)
                    {
                        SCI_FREE(lpStartParam->appName);
                    }
                    if (lpStartParam->data)
                    {
                        SCI_FREE(lpStartParam->data);
                    }
                    SCI_FREE(lpStartParam);
            }

            if (iRet == WRE_INIT_OK || iRet == WRE_INIT_RUNNING || iRet == WRE_INIT_QUERY_UPDATE)
            {                    
            }
            else
            {
                MMK_CloseApplet(SPRD_WRE_APPLET_ID);
            }

            MMK_CloseWin(win_id);
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        eResult = MMI_RESULT_FALSE;
        break;
     }

    return eResult;
}

//20120607 add --创建异步临时窗口，以让其他模块或应用有机会释放内存
PUBLIC int MMIWRE_StartNormal(uint8 appType, uint32 appId, uint16 *appName, void *data)
{
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T        win_handle = 0;
    LPWRE_START_PARAM lpStartParam = NULL;
    uint32 nLen= 0;

    lpStartParam = SCI_ALLOC(sizeof(WRE_START_PARAM));
    if (lpStartParam == NULL)
    {
        SCI_TRACE_LOW("[WRE]MMIWRE_StartNormal: malloc param fail!");
        return 0;
    }
    memset(lpStartParam, 0, sizeof(WRE_START_PARAM));
    lpStartParam->appId = appId;
    lpStartParam->appType = appType;
    if (appName)
    {
        nLen = MMIAPICOM_Wstrlen(appName);
        lpStartParam->appName = SCI_ALLOC((nLen+1)*sizeof(uint16));
        if (lpStartParam->appName == NULL)
        {
            SCI_TRACE_LOW("[WRE]MMIWRE_StartNormal: malloc param fail!");
            SCI_FREE(lpStartParam);
            return 0;
        }
        memset(lpStartParam->appName, 0, (nLen+1)*sizeof(uint16));
        MMIAPICOM_Wstrcpy(lpStartParam->appName, appName);
    }
    if (data)
    {
        nLen = strlen(data);
        lpStartParam->data = SCI_ALLOC(nLen+1);
        if (lpStartParam->data == NULL)
        {
            SCI_TRACE_LOW("[WRE]MMIWRE_StartNormal: malloc param fail!");
            if (lpStartParam->appName)
            {
                SCI_FREE(lpStartParam->appName);
            }
            SCI_FREE(lpStartParam);
            
            return 0;
        }

        memset(lpStartParam->data, 0, nLen+1);
        strcpy(lpStartParam->data, data);
    }

    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = MMIMWIN_WRE_TMP_ID;
    win_create.func = HandleMsg;
    win_create.win_priority = WIN_ONE_LEVEL;//WIN_LOWEST_LEVEL;
    win_create.window_style = WS_DISABLE_RETURN_WIN|WS_NO_DEFAULT_STYLE; //heng.xiao modify for statusbar bug,hide statusbar
    win_create.add_data_ptr = lpStartParam;

    win_handle = MMK_CreateWindow( &win_create );
    if(PNULL==win_handle)
    {
        return 0;
    }

    return 1;
}



int MMIWRE_StartConflictApplet(uint8 appType, uint32 appId, uint16 *appName, void *data)
{
    MMI_APPLET_CONFLICT_T conflict = {0};
    MMIWRE_INSTANCE_T Instance = {0};

    SCI_TRACE_LOW("[WRE]MMIWRE_StartConflictApplet: begin");
    
#ifdef SNS_PULLING_SUPPORT
    MAIN_UpdateIdleSnsState(FALSE, MMI_SNS_FACEBOOK);
    MAIN_UpdateIdleSnsState(FALSE, MMI_SNS_TIWTTER);
#endif //SNS_PULLING_SUPPORT    
    
    Instance.appId = appId;
    Instance.appType = appType;
	if (appName)
	{
        uint32 nLen = MMIAPICOM_Wstrlen(appName);        
        Instance.appName = (uint16 *)SCI_ALLOCA((nLen+1)*sizeof(uint16));
        if (Instance.appName == NULL)
        {
            SCI_TRACE_LOW("[WRE]appName alloc fail!");
            
            return 0;
        }
        
        memset(Instance.appName, 0, (nLen+1)*sizeof(uint16));
        MMIAPICOM_Wstrcpy(Instance.appName, appName);
        Instance.appNameLen = nLen;
	}
    if (data)
	{
        uint32 nLen = strlen(data);
        Instance.data = (uint8 *)SCI_ALLOC(nLen+1);
        if (Instance.data == NULL )
        {
            SCI_TRACE_LOW("[WRE]data alloc fail!");
            if (Instance.appName != NULL)
            {
                SCI_FREE(Instance.appName);
                
                Instance.appName = NULL;
                Instance.appNameLen = 0;
            }
                        
            return 0;
        }
        
        memset(Instance.data, 0, nLen+1);
        strcpy(Instance.data, data);
        Instance.dataLen = nLen;
	}

    conflict.func = MMIWRE_StartApplet;
    conflict.param = &Instance;
    conflict.size_of_param = sizeof(MMIWRE_INSTANCE_T);

    //if (!OneKeyDownloading)
    //MMI_CheckAppletConflict(&conflict);  
    //else
    //MMIWRE_StartApplet(0,0,&Instance);
    //SCI_TRACE_LOW("[WRE]MMIWRE_StartConflictApplet: end");
    //return 0;	// changed by yifei

    if (!OneKeyDownloading)
    {
        return (int)MMI_CheckAppletConflict(&conflict);  
    }
    else
    {
        return MMIWRE_StartApplet(0,0,&Instance);
    }

}

MMI_RESULT_E MMIWRE_StartApplet(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_APPLET_START_T start = {0};
    MMIWRE_INSTANCE_T *lpInstance = (MMIWRE_INSTANCE_T *)param;

    SCI_TRACE_LOW("[WRE]MMIWRE_StartApplet:");

    start.guid = SPRD_WRE_APPLET_ID;     
    start.state = /*MMI_APPLET_STATE_BG_HIDE_WIN|*/MMI_APPLET_STATE_MEM_CONFLICT|MMI_APPLET_STATE_TASK_MANAGER|MMI_APPLET_STATE_DISABLE_RETURN_IDLE;    
    start.param_ptr = lpInstance;
    start.size_of_param = sizeof(MMIWRE_INSTANCE_T);
    
    return MMK_StartApplet(&start);
}

/*****************************************************************************/
//  Description : MMIWRE_Actived
//  Global resource dependence : 
//  Author: Wei.zhou
//  Note: judge the WRE running or not
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_Actived(void)
{
    return WBOOT_IsWRERunning();
}

/*****************************************************************************/
//  Description : forbid the sweet menu before enter wre.
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_ForbibSweetMenu(void)
{
#ifdef MENU_SWEETS_SUPPORT
    if(MMIMAINMENU_SWEETS_E == MMIAPISET_GetMainmenuStyle())
    {
        if(MMK_IsOpenWin(MAIN_MAINMENU_WIN_ID))
        {
            s_entrance = WRE_ENTRANCE_FROM_MENU;
            MMK_CloseWin(MAIN_MAINMENU_WIN_ID);
        }
        else
        {
            s_entrance = WRE_ENTRANCE_FROM_IDLE;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : revival the sweet menu after exit wre.
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_RevivalSweetMenu(void)
{
#ifdef MENU_SWEETS_SUPPORT
    if(MMIMAINMENU_SWEETS_E == MMIAPISET_GetMainmenuStyle())
    {
        if(WRE_NORMAL_EXIT_PATH == MMIWRE_IsFromHomeKeyExit())
        {
            if(WRE_ENTRANCE_FROM_MENU == s_entrance)
            {
                MMIAPIMENU_CreatMainMenu();
            }
        }
        else
        {
            MMIWRE_ClearHomeKeyFlag();
        }
    }
#endif
}

/*****************************************************************************/
//  Description : forbid the livewallpaper before enter wre.
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_ForbibLiveWallPaper(void)
{
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

    /* reserved the pre setting */
    MMIAPISET_GetWallpaperSetInfo(&s_wallpaper_info);
    /* set the fix wall paper */
    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
    wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
}

/*****************************************************************************/
//  Description : revival the livewallpaper after exit wre.
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_RevivalLiveWallPaper(void)
{
    MMIAPISET_SetWallpaperSetInfo(s_wallpaper_info);
}

/*****************************************************************************/
//  Description : close qbtheme before enter wre
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_CloseQbTheme(void)
{
#ifdef QBTHEME_SUPPORT
    MMISET_IDLE_STYLE_E idle_style = 0;

    #if !defined(MMI_ANDROID_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
         idle_style = MMISET_IDLE_ISTYLE;
    #endif
    
    #if defined MMI_GRID_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_GRID;
    #endif
    
    #if defined MMIWIDGET_SUPPORT
        idle_style = MMISET_IDLE_STYLE_WIDGET;
    #endif
        
    #ifdef MMI_COMMON_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_COMMON;
    #endif
        
    #ifdef DPHONE_SUPPORT 
        idle_style = MMISET_IDLE_STYLE_COMMON;
    #endif

    s_cur_style = MMIAPISET_GetIdleStyle();

    SCI_TRACE_LOW("[MMIWRE] MMIWRE_CloseQbTheme");
   /* exit the qbthime, need free memory */
    /* if(s_cur_style == MMISET_IDLE_QBTHEME) */
    if(MMIAPIQBTHEME_IsSupport(s_cur_style))
    {
        MMIAPIQBTHEME_StyleStop();
    }

    /* NEWMS00232001, by wei.zhou
    MMISET_SetIdleStyle(idle_style);
    */
#endif
}

/*****************************************************************************/
//  Description : mmwre start qbtheme
//  Global resource dependence :
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void MMIWRE_StartQbTheme(void)
{
#ifdef QBTHEME_SUPPORT
    SCI_TRACE_LOW("[MMIWRE] MMIWRE_StartQbTheme");
    
    /* NEWMS00232001, by wei.zhou
    MMISET_SetIdleStyle(pre_style);
    */

    /* qbtheme initial startup is or not */
    if(MMIAPIQBTHEME_IsSupport(s_cur_style))
    {
        MMIAPIQBTHEME_StyleStart();
    }
#endif
}

/*****************************************************************************/
//  Description : memory fragment clean centralize processing
//  Global resource dependence : 
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GarbageClean(void)
{
    BLKMEM_GarbageClean();
    UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMENU_DyDestroy();
#endif
    MMIWRE_CloseQbTheme();
    /* bugfree11691, games desktop exit wre interface errors. by wei.zhou */
    /*
    MMIWRE_ForbibLiveWallPaper();
    */
    MMIWRE_ForbibSweetMenu();
    /* bugzilla5349 */
    if(MMIAPICOM_IsPanelOpened())
    {
        MMIAPICOM_ClosePanelChildWin();
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : memory fragment revival centralize processing
//  Global resource dependence : 
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GarbageRevial(void)
{
    UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
    MMIWRE_StartQbTheme();
    /* bugfree11691, games desktop exit wre interface errors. by wei.zhou */
    /*
    MMIWRE_RevivalLiveWallPaper();
    */
    MMIWRE_RevivalSweetMenu();
    return TRUE;
}

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************/
//  Description : Start WRE app without any notification
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC int MMIWRE_StartAppSilent(uint32 fix_app_id, void *data)
{
    // (WRE_INIT_OK != WRE_IsFixAppInstalled(fix_app_id)) 
    if (MMIAPIUDISK_UdiskIsRun())
    {
        return WRE_INIT_RUNNING;
    }
    else if (!WRE_SDCardInsert())
    {
        return WRE_INIT_RUNNING;
    }
    else if (MMIAPIDC_IsOpened())
    {
        return WRE_INIT_RUNNING;
    }
    else if (WBOOT_IsWRERunning())
    {
        return WRE_INIT_RUNNING;
    }
    else if (MMIWRE_IsMocorAppBackRunning())
    {
        return WRE_INIT_RUNNING;
    }
    else if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        return WRE_INIT_RUNNING;
    }
    
    return MMIWRE_StartAppMinimize(fix_app_id, data);
}

/*****************************************************************************/
//  Description : Release wakeup IMMISNS object
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWRE_Wakeup_IMMISNS_Release(void)
{
    WRE_Wakeup_IMMISNS_Release();
}

/*****************************************************************************/
//  Description : Start SNS pulling timer
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWRE_StartSnsPullingTimer(int widget_id)
{
    WRE_StartSnsPullingTimer(widget_id);
}

/*****************************************************************************/
//  Description : Stop SNS pulling timer
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWRE_StopSnsPullingTimer(int widget_id)
{
    WRE_StopSnsPullingTimer(widget_id);
}

/*****************************************************************************/
//  Description : Set SNS pulling timer
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWRE_SetSnsPullingTimer(int widget_id, uint32 time)
{
    WRE_SetSnsPullingTimer(widget_id, time);
}

/*****************************************************************************/
//  Description : Set SNS pulling preset timer
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWRE_SetSnsPullingPresetTimer(int widget_id, uint16 preset_type)
{
    WRE_SetSnsPullingPresetTimer(widget_id, preset_type);
}

/*****************************************************************************/
//  Description : Get SNS pulling preset timer
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWRE_GetSnsPullingPresetType(int widget_id)
{
    return WRE_GetSnsPullingPresetType(widget_id);
}
#endif //SNS_PULLING_SUPPORT

#endif //WRE_SUPPORT
