/*****************************************************************************
** File Name:      mmijava_export.c                                           *
** Author:         apple.zhang                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       apple.zhang            Create                                *
******************************************************************************/

#include "mmi_app_java_trc.h"
#if defined(__cplusplus)
extern "C" {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "mmijava_config.h"
#include "sci_types.h"
#include "sig_code.h"

#ifdef JAVA_SUPPORT_SUN
#include "mmijava_main.h"
#include "mmijava_video.h"
#include "javacall_lifecycle.h"
#include "mmijava_preload.h"
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to open java main menu
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenMainMenu(void)
{   
    MMIJAVA_OpenMainMenu();
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InitModule(void)
{    
    MMIJAVA_InitModule();
}
/*****************************************************************************/
//  Description : to init the java module
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Init(void)
{
    MMIJAVA_Init();
}
/*****************************************************************************/
//  Description : get current java files storage device, sd or udisk.
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIJAVA_GetCurrentStoreDevice(void)
{
    return MMIJAVA_GetCurrentStoreDevice();
}
/*****************************************************************************/
//  Description:   to resume jbed installing processing when after mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to resume java
//  Return:
//       TRUE : resuming complete.
//       FALSE: don't need to resume or has been resumed.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module)
{
    return MMIJAVA_Resume(module);
}
/*****************************************************************************/
//  Description:   to pause jbed installing processing when mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to pause java
//  Return:
//       TRUE : paused normally.
//       FALSE: don't need pause or has been paused.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module)
{
    return MMIJAVA_Pause(module);
}

/*****************************************************************************/
//  Description : shutdown jvm
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Shutdown(MMIJAVA_PAUSE_TYPE_E module)
{
    return MMIJAVA_Shutdown(module);
}

/*****************************************************************************/
//  Description : shutdown jvm synchronous 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_ShutdownSync(void)
{
    return MMIJAVA_ShutdownSync();
}

PUBLIC void MMIAPIJAVA_RunFromBackground(BOOLEAN background)
{
    MMIJAVA_RunFromBackground(background);
}
/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  liqing.peng
//  Param
//       void
//  Return:
//      void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void)
{
    MMIJAVA_StopOperaSDPlug();
}
/*****************************************************************************/
//  Description:   whether java is running
//  Author     :   Arthur.Peng 2006/11
//  Param
//       void
//  Return:
//       TRUE : java is running.
//       FALSE: java is not running
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning( void )
{
    return MMIJAVA_IsJavaRuning();
}
/*****************************************************************************/
//  Description : IsFocusJAVARunWin
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void)
{
    return MMIJAVA_IsFocusJAVARunWin();
}

/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_CreateJavaDirectory(MMIFILE_DEVICE_E cur_dev)
{
    return MMIJAVA_CreateJavaDirectory(cur_dev);
}
/*****************************************************************************/
//  Description : set status for having stopped by other
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SetIsStoppedbyOther(BOOLEAN is_stopped)
{
    MMIJAVA_SetIsStoppedbyOther(is_stopped);
}

/*****************************************************************************/
//  Description : install preload
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PreloadInstall(void)
{
    MMIJAVA_Preload_Install();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_UpdatePreloadMidlet(void)
{
    MMIJAVA_update_preload_midlet();
}
/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystem(
                                             wchar *pathname, 
                                             int pathname_len, 
                                             wchar *filename, 
                                             int filename_len)
{
    MMIJAVA_INSTALL_FROM_FILESYSTEM(pathname,pathname_len,filename,filename_len);
}
/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(
                                             wchar *pathname, 
                                             int pathname_len, 
                                             wchar *filename, 
                                             int filename_len)
{
    MMIJAVA_INSTALL_FROM_FILESYSTEM_FOR_DM(pathname,pathname_len,filename,filename_len);
}
/*****************************************************************************/
//  Description : to install form browser
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromBrowser(
                                          wchar *local_pathnam, 
                                          int local_pathname_len, 
                                          unsigned char* url
                                          )
{
    MMIJAVA_INSTALL_FROM_BROWSER(local_pathnam,local_pathname_len,url);
}

/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SendJVMMessage2MMI(APP_JAVA_SIGNALCODE_E signal, uint16 data)
{
    MMIJAVA_SendJVMMessage2MMI(signal,data);
}

/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_DispatchVmMessage(void* sig)
{
    MMIJAVA_DISPATCH_VM_MESSAGE(sig);
}

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIAPIJAVA_CallbackFrameEndNotifyProc(void)
{
#ifndef WIN32
    MMIJAVA_CallbackFrameEndNotifyProc();
#endif
}
/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIAPIJAVA_CallbackEndNotifyProc(void)
{
#ifndef WIN32
    MMIJAVA_CallbackEndNotifyProc();
#endif
}

/*****************************************************************************/
//  Description : snapshot notify处理
//  Global resource dependence :
//  Author: 
//  Note: restart camera
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_CallbackSnapshotEndNotifyProc(void)
{
    MMIJAVA_CallbackSnapshotEndNotifyProc();
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIJAVA_javacall_lifecycle_is_ams_auto_start(void)
{
    return javacall_lifecycle_is_ams_auto_start();
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_javacall_lifecycle_set_ams_auto_start(int autoStart)
{
    javacall_lifecycle_set_ams_auto_start(autoStart);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_JavaCallClose(BOOLEAN is_need_send_message)
{
    if (MMIAPIJAVA_javacall_lifecycle_is_ams_auto_start() == 1) {
        MMIAPIJAVA_javacall_lifecycle_set_ams_auto_start(0);
        if (is_need_send_message)
        {
            MMIAPIJAVA_SendJVMMessage2MMI(APP_JAVA_START_AMS_IND, START_AMS);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC const  char* MMIAPIJAVA_GetVersion( void )
{
	return JAVA_GetVersion();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_GetSuiteInfo(int suiteId)
{
 	javanotify_get_suite_info(suiteId);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM( void )
{
	MMIJAVA_ResumeBackgroundJVM();
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PauseBackgroundJVM( void )
{
	MMIJAVA_PauseBackgroundJVM();
}

/*****************************************************************************/
//  Description : MMIAPIJAVA_StopAudio
//  Global resource dependence :stop java audio and also stop jvm
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void)
{
    MMIJAVA_PauseAllMediaPlayer();
    return MMIJAVA_Shutdown(JAVA_PAUSE_NONE);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_LaunchPreloadMidlet(char *midletName)
{
    MMIJAVA_LAUNCH_PRELOAD_MIDLET(midletName);
}
#else
/*****************************************************************************/
//  Description : to open java main menu
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenMainMenu(void)
{   

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InitModule(void)
{    
    //SCI_TRACE_LOW:"MMIAPIJAVA_Init for sun no support"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_EXPORT_366_112_2_18_2_30_32_0,(uint8*)"");
}
/*****************************************************************************/
//  Description : to init the java module
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Init(void)
{
    
}
/*****************************************************************************/
//  Description : get current java files storage device, sd or udisk.
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIJAVA_GetCurrentStoreDevice(void)
{
    return MMI_DEVICE_UDISK;
}
/*****************************************************************************/
//  Description:   to resume jbed installing processing when after mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to resume java
//  Return:
//       TRUE : resuming complete.
//       FALSE: don't need to resume or has been resumed.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module)
{
    return TRUE;

}
/*****************************************************************************/
//  Description:   to pause jbed installing processing when mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to pause java
//  Return:
//       TRUE : paused normally.
//       FALSE: don't need pause or has been paused.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module)
{
    return TRUE;
    
}

/*****************************************************************************/
//  Description : shutdown jvm
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Shutdown(MMIJAVA_PAUSE_TYPE_E module)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : shutdown jvm synchronous 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_ShutdownSync(void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  liqing.peng
//  Param
//       void
//  Return:
//      void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void)
{

}
/*****************************************************************************/
//  Description:   whether java is running
//  Author     :   Arthur.Peng 2006/11
//  Param
//       void
//  Return:
//       TRUE : java is running.
//       FALSE: java is not running
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning( void )
{
    return FALSE;
}
/*****************************************************************************/
//  Description : IsFocusJAVARunWin
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_CreateJavaDirectory(MMIFILE_DEVICE_E cur_dev)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : set status for having stopped by other
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SetIsStoppedbyOther(BOOLEAN is_stopped)
{
}

/*****************************************************************************/
//  Description : install preload
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PreloadInstall(void)
{
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_UpdatePreloadMidlet(void)
{
}

/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystem(wchar *pathname, int pathname_len, wchar *filename, int filename_len)
{
}

/*****************************************************************************/
//  Description : to install form file system for dm
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(
                                             wchar *pathname, 
                                             int pathname_len, 
                                             wchar *filename, 
                                             int filename_len)
{
}

/*****************************************************************************/
//  Description : to install form browser
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromBrowser(wchar *local_pathnam, int local_pathname_len, unsigned char* url)
{
}

/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SendJVMMessage2MMI(APP_JAVA_SIGNALCODE_E signal, uint16 data)
{
}

/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_DispatchVmMessage(void* sig)
{
}
/*****************************************************************************/
//  Description : preload install
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Preload_Install(void)
{
}
/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIAPIJAVA_CallbackFrameEndNotifyProc(void)
{

}

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIAPIJAVA_CallbackEndNotifyProc(void)
{

}

/*****************************************************************************/
//  Description : snapshot notify处理
//  Global resource dependence :
//  Author: 
//  Note: restart camera
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_CallbackSnapshotEndNotifyProc(void)
{
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIJAVA_javacall_lifecycle_is_ams_auto_start(void)
{
    return 0;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_javacall_lifecycle_set_ams_auto_start(int autoStart)
{
}

/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_CreateJavaDirectory(MMIFILE_DEVICE_E cur_dev)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_JavaCallClose(BOOLEAN is_need_send_message)
{

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM( void )
{

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PauseBackgroundJVM( void )
{

}

/*****************************************************************************/
//  Description : MMIAPIJAVA_StopAudio
//  Global resource dependence :stop java audio and also stop jvm
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void)
{
    return FALSE;
}
#endif
#if defined(__cplusplus)
}
#endif

