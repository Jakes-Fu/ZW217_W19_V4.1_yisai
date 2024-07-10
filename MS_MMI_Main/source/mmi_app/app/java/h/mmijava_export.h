/****************************************************************************
** File Name:      mmijava_export.h                                         *
** Author:         apple.zhang                                              *
** Date:           06/01/2009                                               *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the main application.      *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        apple.zhang        Create
****************************************************************************/
#ifndef  _MMIJAVA_EXPORT_H_    
#define  _MMIJAVA_EXPORT_H_   


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "sig_code.h"
#include "mmijava_config.h"
#include "mmifmm_export.h"
#include "java_mmi_text.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to open java main menu
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenMainMenu(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InitModule(void);
/*****************************************************************************/
//  Description : to init the java module
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Init(void);

/*****************************************************************************/
//  Description : get current java files storage device, sd or udisk.
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIJAVA_GetCurrentStoreDevice(void);
/*****************************************************************************/
//  Description : up java thread priority to control other task
//  Global resource dependence : none
//  Author: 
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_UpThreadPriority(void);

/*****************************************************************************/
//  Description : up java thread priority to control other task
//  Global resource dependence : none
//  Author: 
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_DownThreadPriority(void);
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
PUBLIC BOOLEAN MMIAPIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module);
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
PUBLIC BOOLEAN MMIAPIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module);

/*****************************************************************************/
//  Description : shutdown jvm
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Shutdown(MMIJAVA_PAUSE_TYPE_E module);

/*****************************************************************************/
//  Description : shutdown jvm synchronous 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_ShutdownSync(void);

PUBLIC void MMIAPIJAVA_RunFromBackground(BOOLEAN background);
/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  liqing.peng
//  Param
//       void
//  Return:
//      void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void);
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
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning( void );
/*****************************************************************************/
//  Description : IsFocusJAVARunWin
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void);
/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_CreateJavaDirectory(MMIFILE_DEVICE_E cur_dev);
/*****************************************************************************/
//  Description : set status for having stopped by other
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SetIsStoppedbyOther(BOOLEAN is_stopped);
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
                                             int filename_len
                                             );

/*****************************************************************************/
//  Description : to install form file system, for DM
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(
                                             wchar *pathname, 
                                             int pathname_len, 
                                             wchar *filename, 
                                             int filename_len
                                             );
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
                                          );
/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SendJVMMessage2MMI(APP_JAVA_SIGNALCODE_E signal, uint16 data);

/*****************************************************************************/
//  Description : to send jvm message to mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_DispatchVmMessage(void* sig);

/*****************************************************************************/
//  Description : preload install
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PreloadInstall(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_UpdatePreloadMidlet(void);

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIAPIJAVA_CallbackFrameEndNotifyProc(void);

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIAPIJAVA_CallbackEndNotifyProc(void);
/*****************************************************************************/
//  Description : snapshot notify处理
//  Global resource dependence :
//  Author: 
//  Note: restart camera
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_CallbackSnapshotEndNotifyProc(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIJAVA_javacall_lifecycle_is_ams_auto_start(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_javacall_lifecycle_set_ams_auto_start(int autoStart);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_JavaCallClose(BOOLEAN is_need_send_message);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM( void );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PauseBackgroundJVM( void );

/*****************************************************************************/
//  Description : MMIAPIJAVA_StopAudio
//  Global resource dependence :stop java audio and also stop jvm
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void);

#ifdef JAVA_SUPPORT_SUN
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC const  char* MMIAPIJAVA_GetVersion( void );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_GetSuiteInfo(int suiteId);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_LaunchPreloadMidlet(char *midletName);
#endif 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
