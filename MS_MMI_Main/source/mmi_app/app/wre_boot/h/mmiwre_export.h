#ifdef WRE_SUPPORT
/****************************************************************************
** File Name:      mmiwre_export.h
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
#ifndef  _MMIWRE_EXPORT_H_
#define  _MMIWRE_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
extern "C"
{
#endif

#define TYPE_WREAPP_STORE 0     //启动应用商店
#define TYPE_WREAPP_APP   1     //启动动态菜单应用
#define TYPE_WREFIX_APP   2     //启动内置应用
#define TYPE_WRE_DATA_APP   3  //传输数据到应用

#define WRE_ENTRANCE_FROM_IDLE  0
#define WRE_ENTRANCE_FROM_MENU  1

#ifdef SNS_PULLING_SUPPORT
#define SNS_COMMAND_RES_NEWPOST 2  // For sns notification use
#define SNS_COMMAND_RES_BG_RUN  3  // For sns pulling use
#endif //SNS_PULLING_SUPPORT

typedef struct _tagDYNAMIC_STARTUP_PARAM
{
    uint32  appid;
    uint16  *appname;
}DYNAMIC_STARTUP_PARAM, *PDYNAMIC_STARTUP_PARAM;

typedef void INSTALLJAVACALLBACKFUNC(int,void *);
extern INSTALLJAVACALLBACKFUNC * g_pInStallJavaFunc;
extern void *g_pSaveParam; 
/*****************************************************************************/
// 	Description : dispatch the WRE message 
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_DispatchWreMSG(void);

/*****************************************************************************/
// 	Description : get WRE message
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GetWreMessageForMocor(void);

/*****************************************************************************/
// 	Description : set watch dog pointer
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_SetWatchDogPtr(void *SwdgPtr);

/*****************************************************************************/
// 	Description : NetworkIndHandle
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_NetworkIndHandle(DPARAM param);

/*****************************************************************************/
// 	Description : SetDeepSleepStatus
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_SetDeepSleepStatus(BOOLEAN is_deep_sleep);

/*****************************************************************************/
// 	Description : wreentry
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC int MMIWRE_Entry(void);


/*****************************************************************************/
// 	Description : MMIWRE_GetFixAppGuid
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GetFixAppGuid(uint16 * guid,
									   uint16 * store_app_id,
									   uint32  fixed_app_id);

/*****************************************************************************/
// 	Description : MMIWRE_IsFixApp
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_IsFixApp(uint16 * guid);

/*****************************************************************************/
// 	Description : MMIWRE_DeleteInitFile
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
PUBLIC void MMIWRE_DeleteInitFile(uint32 step);

/*****************************************************************************/
//  Description : MMIWRE_DynStartup
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
PUBLIC int MMIWRE_DynStartup(uint32  appid, uint16 *appname);
/*****************************************************************************/
//  Description : MMIWRE_PauseMM
//  Global resource dependence : 
//  Author:Riancy.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_PauseMM(void);
/*****************************************************************************/
//  Description : MMIWRE_ResumeMM
//  Global resource dependence : 
//  Author:Riancy.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_ResumeMM(void);
/*****************************************************************************/
//  Description : MMIWRE_StopMM
//  Global resource dependence : 
//  Author:Riancy.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIWRE_StopMM(void);

/*****************************************************************************/
//  Description : MMIWRE_IsMMPlaying
//  Global resource dependence : 
//  Author:Riancy.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_IsMMPlaying(uint32 nType);

/*****************************************************************************/
// 	Description : get the WRE version
//	Global resource dependence : 
//  Author:jie.yang
//	Note:
/*****************************************************************************/
PUBLIC char*  MMIWRE_GetVersionInfo(void);

/*****************************************************************************/
//  Description : MMIWRE_StartAppEx
//  Global resource dependence : 
//  Author:Langbiao.tan
//  Note:
/*****************************************************************************/
PUBLIC int MMIWRE_StartAppEx(uint32 appid, void *data);
PUBLIC int MMIWRE_StartAppMinimize(uint32 appid, void *data);

/*****************************************************************************/
//  Description : MMIWRE_GetAppGuid
//  Global resource dependence : 
//  Author:Langbiao.tan
//  Note:
/*****************************************************************************/
#ifdef SNS_SUPPORT
PUBLIC uint16* MMIWRE_GetAppGuid(uint32 appid);

 PUBLIC BOOLEAN MMIWRE_GetFixAppPath(uint32 appid, uint16 * path,uint16 * appname,uint16 * iconpath);
#endif

PUBLIC void MMIWRE_HandleCallBackMsg(
                    void *msg_ptr
                    );

/*****************************************************************************/
//  Description : MMIWRE_Actived
//  Global resource dependence : 
//  Author: Wei.zhou
//  Note: judge the WRE running or not
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_Actived(void);
PUBLIC int MMIWRE_StartConflictApplet(uint8 appType, uint32 appId, uint16 *appName, void *data);
PUBLIC MMI_RESULT_E MMIWRE_PowerOff(int flag);
/*****************************************************************************/
//  Description : MMIWRE_StartApp
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
PUBLIC int MMIWRE_StartApp(uint32 fix_app_id);

/*****************************************************************************/
//  Description : memory fragment clean centralize processing
//  Global resource dependence : 
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GarbageClean(void);

/*****************************************************************************/
//  Description : memory fragment revival centralize processing
//  Global resource dependence : 
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_GarbageRevial(void);

#ifdef SNS_PULLING_SUPPORT
PUBLIC int MMIWRE_StartAppSilent(uint32 fix_app_id, void *data);
PUBLIC void MMIWRE_Wakeup_IMMISNS_Release(void);
PUBLIC void MMIWRE_StartSnsPullingTimer(int widget_id);
PUBLIC void MMIWRE_StopSnsPullingTimer(int widget_id);
PUBLIC void MMIWRE_SetSnsPullingTimer(int widget_id, uint32 time);
PUBLIC void MMIWRE_SetSnsPullingPresetTimer(int widget_id, uint16 preset_type);
PUBLIC uint16 MMIWRE_GetSnsPullingPresetType(int widget_id);
#endif //SNS_PULLING_SUPPORT

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMIWRE_EXPORT_H_  */
#endif
