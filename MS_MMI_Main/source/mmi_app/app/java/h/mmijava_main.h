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
#include "caf.h"
#include "mmk_type.h"
#include "mmifmm_export.h"
#include "sig_code.h"
#include "guiim_base.h"
#include "guiedit.h"
#include "mmijava_config.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef struct
{
    SIGNAL_VARS
    uint16          javaSig;
}JAVASIG_T;

typedef struct
{
    GUI_RECT_T    *rect;
    wchar          *wstr_ptr;
    uint16         wstr_len;
    uint16         max_str_len;
    uint16         cusor_pos;
    GUIIM_TYPE_T          allow_inputmethod;
    GUIIM_TYPE_T         cur_inputmethod;
    GUIEDIT_TYPE_E	edit_type;	
} MMIJAVA_TEXTEDITBOX_INIT_DATA_T;

typedef enum
{
    PAUSE_WIN,
    RESUME_WIN
} MMIJAVA_VMEXIT_WIN_TYPE_T;
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
// 	Description : to open java main menu
//	Global resource dependence : none
//  Author: applezhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMainMenu(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_InitModule(void);

/*****************************************************************************/
//  Description : to init the java module
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Init(void);

/*****************************************************************************/
//  Description : get current java files storage device, sd or udisk.
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->udisk, 1->sd card
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIJAVA_GetCurrentStoreDevice(void);
/*****************************************************************************/
//  Description : up java thread priority to control other task
//  Global resource dependence : none
//  Author: 
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_UpThreadPriority(void);

/*****************************************************************************/
//  Description : up java thread priority to control other task
//  Global resource dependence : none
//  Author: 
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_DownThreadPriority(void);
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
PUBLIC BOOLEAN MMIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module);
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
PUBLIC BOOLEAN MMIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module);
PUBLIC BOOLEAN MMIJAVA_Shutdown(MMIJAVA_PAUSE_TYPE_E module);
PUBLIC void MMIJAVA_RunFromBackground(BOOLEAN background);
/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  liqing.peng
//  Param
//       void
//  Return:
//      void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_StopOperaSDPlug(void);
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
PUBLIC BOOLEAN MMIJAVA_IsJavaRuning( void );
/*****************************************************************************/
//  Description : IsFocusJAVARunWin
//  Global resource dependence : 
//  Author:aoke.hu
//  Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsFocusJAVARunWin(void);
/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_CreateJavaDirectory(MMIFILE_DEVICE_E cur_dev);
/*****************************************************************************/
//  Description : set status for having stopped by other
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsStoppedbyOther(BOOLEAN is_stopped);
PUBLIC void MMIJAVA_PauseAllMediaPlayer(void);
PUBLIC void MMIJAVA_ResumeAllMediaPlayer(void);
PUBLIC void MMIJAVA_CloseAllMediaPlayer(void);
PUBLIC void MMIJAVA_INSTALL_FROM_FILESYSTEM(wchar *pathname, int pathname_len, wchar *filename, int filename_len);
PUBLIC void MMIJAVA_INSTALL_FROM_FILESYSTEM_FOR_DM(wchar *pathname, int pathname_len, wchar *filename, int filename_len);
PUBLIC void MMIJAVA_INSTALL_FROM_BROWSER(wchar *local_pathnam, int local_pathname_len, unsigned char* url);
PUBLIC int32 MMIJAVA_enable_pclink(void);
PUBLIC void MMIJAVA_SendJVMMessage2MMI(APP_JAVA_SIGNALCODE_E signal, uint16 data);
PUBLIC void JAVAMMI_REMOVE_TMP_DOWNLOAD_FILE(void);
PUBLIC void MMIJAVA_DISPATCH_VM_MESSAGE(void* sig);
PUBLIC void Prepare_Launch_Midlet_Parameter(int suiteId, char *className);

/*****************************************************************************/
//  Description : install preload
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Preload_Install(void);

PUBLIC void MMIJAVA_update_preload_midlet(void);


/*****************************************************************************/
//  Description : when java is runing background, set to TRUE, else set to FALSE
//  Global resource dependence : none
//  Parameter: is_backgroup:TURE: java runing in background; FALSE:java runing in foreground;
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Set_PausedFlag(BOOLEAN is_background);

/*****************************************************************************/
//  Description : check java is running in backgroup or foregroup
//  Global resource dependence : none
//  Author: 
//  Note:
//  Return: TURE: java runing in backgroup; FALSE:java runing in foregroup;
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Is_Paused(void);

/*****************************************************************************/
//  Description : snapshot notify¥¶¿Ì
//  Global resource dependence :
//  Author: 
//  Note: restart camera
/*****************************************************************************/
PUBLIC void MMIJAVA_CallbackSnapshotEndNotifyProc(void);

/*****************************************************************************/
//  Discription: This function get net setting index
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC uint8 MMIJAVA_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : when java is paused, set to TRUE, else set to FALSE
//  Global resource dependence : none
//  Parameter: is_backgroup:TURE: java runing in background; FALSE:java runing in foreground;
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_Set_PausedFlag(BOOLEAN is_background);

/*****************************************************************************/
//  Description : check java is in paused state or not
//  Global resource dependence : none
//  Author: 
//  Note:
//  Return: TURE: java runing in backgroup; FALSE:java runing in foregroup;
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_Is_Paused(void);
/*****************************************************************************/
//  Description : check java is in shutdown (nor foreground or background)
//  Global resource dependence : none
//  Author: 
//  Note:
//  Return: TURE: java not runing in background or foreground,
//          FALSE: other
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsJavaShutdown(void);

/*****************************************************************************/
//  Description : java_enable_pclink
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/  
PUBLIC void MMIJAVA_ResumeBackgroundJVM( void );

/*****************************************************************************/
//  Description : java_enable_pclink
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/  
PUBLIC void MMIJAVA_PauseBackgroundJVM( void );

/*****************************************************************************/
//  Description : when java is runing background, set to TRUE, else set to FALSE
//  Global resource dependence : none
//  Parameter: is_backgroup:TURE: java runing in background; FALSE:java runing in foreground;
//  Author: 
//  Note:
/*****************************************************************************/
//PUBLIC void MMIJAVA_Set_BackgroundRunningFlag(BOOLEAN is_background);

/*****************************************************************************/
//  Description : check java is running in backgroup or foregroup
//  Global resource dependence : none
//  Author: 
//  Note:
//  Return: TURE: java runing in backgroup; FALSE:java runing in foregroup;
/*****************************************************************************/
//PUBLIC BOOLEAN MMIJAVA_Is_BackgroundRunning(void);

PUBLIC void JAVAMMI_javacall_lifecycle_set_suite_info(
                                                      int suiteId, 
                                                      uint16* version, 
                                                      int versionLength, 
                                                      uint16* name, 
                                                      int nameLength
                                                      );

PUBLIC long MMIJAVA_GetAppHeapMemSize( void );

PUBLIC void JAVAMMI_javacall_notify_suiteID(int suiteID);

/*****************************************************************************/
//  Description : pause current playing media when .
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseCurrentVideoMediaPlayer(void);

/*****************************************************************************/
//  Description : resume current playing media when JVM get focus.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeCurrentVideoMediaPlayer(void);

/*****************************************************************************/
//  Description : shutdown jvm synchronous 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_ShutdownSync(void);

/*****************************************************************************/
//  Discription: get rect for java txt editbox win  
//  Global resource dependence: None
//  Author: hui.zhao
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC GUI_RECT_T* MMIJAVA_GetInputMethodRect( void );

PUBLIC void mmijava_lock_device_multimedia(void);
PUBLIC void mmijava_unlock_device_multimedia(void);



/*****************************************************************************/
//  Discription: do something when a midlet start 
//  Global resource dependence: None
//  Author: hui.zhao
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIJAVA_javacall_lifecycle_midlet_started( void );
PUBLIC BOOLEAN mmijava_input_pointer_support(void);
PUBLIC BOOLEAN mmijava_input_motion_support(void);
PUBLIC int32 MMIJAVA_GetSimCount(void);
PUBLIC int32 MMIJAVA_IsSupportWIFI(void);
PUBLIC int32  MMIJAVA_GetWIFIStatus(void); //connected
PUBLIC uint32 MMIJAVA_GetWIFINetID(void);
PUBLIC MMI_TEXT_ID_T* MMIJAVA_I18nGetText(int32 *num);
PUBLIC void* MMIJAVA_PermissionGetText(int32 *num);
PUBLIC MMI_TEXT_ID_T* MMIJAVA_AudioGetMetadataText(int32 *num);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
