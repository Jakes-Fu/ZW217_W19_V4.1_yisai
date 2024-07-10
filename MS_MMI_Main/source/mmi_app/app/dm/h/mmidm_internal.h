/****************************************************************************
** File Name:      mmidm_internal.h						                   *
** Author:                                                                 *
** Date:           07/21/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the SMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 7/2009        jian.ma            Create
** 
****************************************************************************/
#ifndef _MMIDM_INTERNAL_H_
#define _MMIDM_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidm_export.h"
#include "sci_types.h"
#include "vdm_pl_types.h"
#include "vdm_mmi_pl.h"
#include "vdm_error.h"
#include "vdm_types.h"
#include "vdm_api.h"
#include "vdm_fumo_types.h"
#include "vdm_fumo.h"
#include "vdm_scomo_types.h"
#include "vdm_scomo_dp_types.h"
#include "vdm_scomo_dc_types.h"
#include "vdm_scomo.h"
#include "vdm_scomo_dp.h"
#include "vdm_tree.h"
#include "vdm_scomo_pl_inventory.h"
#include "vdm_scomo_dc.h"
#include "vdm_config.h"
#include "vdm_utl_logger.h"
#include "vdm_pl_resume.h"
#include "vdm_pl_alloc.h"
#include "vdm_lawmo_types.h"
#include "scomo_dp_file.h"
#include "scomo_plugin.h"
#include "dp_extractor.h"
#include "vdm_scomo_dp_instance.h"
#include "vdm_scomo_dp_sessiontypes.h"
#include "mmidm_app.h"

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
 
#ifdef __cplusplus
extern "C"
{
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
typedef	enum {
	DM_FOTA_NONE = 0,//没有升级
	DM_FOTA_SUCCESS,   // 升级成功
	DM_FOTA_UNCOMPATIBLE // 版本不兼容导致升级失败
}DM_FOTA_RESULT_E;

typedef	enum{
	DM_FOTA_OP_SET_UPDATE_FLAG,
	DM_FOTA_OP_CLEAR_FLAG
}DM_FOTA_FLAG_OPERATION_E;

typedef enum {
    DM_FOTA_ERR_NONE  = 0,   			/* Success */    
    DM_FOTA_ERR_SYSTEM			/* System error, e.g. hardware failure */
}DM_FOTA_ERROR_E;

typedef	enum {
    DM_START_NONE,
    DM_START_SUCC,
    DM_START_FAIL,
    DM_START_DONE,
}DM_START_RESULT_E;
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : start dm 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC DM_START_RESULT_E MMIDM_StartVDM(DM_SESSION_TYPE type,char* msg_body, uint32 msg_size);

/*****************************************************************************/
//  Description : get DP hender length
//  Global resource dependence : 
//  Author:jianhui.luo
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDPHeaderLength(void);

/*****************************************************************************/
//	Description : delete all resume files
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_DeleteAllResumeFile(void);

/*****************************************************************************/
//  Description : free all mo instance
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_FreeAllMOInstance(void);


/*****************************************************************************/
//  Description : get Observer data ptr
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC VDM_MMI_Observer_t* MMIDM_GetObserverData(void);

/*****************************************************************************/
//	Description : whether resume file is exist or not
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsResumeFileExist(MMIDM_RESUME_TYPE type);

/*****************************************************************************/
//	Description : remove resume file
//	Global resource dependence : 
//	Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC int MMIDM_RemoveResumeFile(MMIDM_RESUME_TYPE type);


/*****************************************************************************/
//	Description : vdm run
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Run(void);

/*****************************************************************************/
//	Description : dm destroy
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Destroy(void);

/*****************************************************************************/
// 	Description : dm SessionCancel
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SessionCancel(void);

/*****************************************************************************/
// 	Description : dm  NotifyNIASessionProceed
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_NotifyNIASessionProceed(void);

/*****************************************************************************/
// 	Description : dm  MMIDM_registerSessionStateObserver
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_RegisterSessionStateObserver(VDM_SessionStateNotifyCB inHandlerFunc);
/*****************************************************************************/
// 	Description : dm  MMIDM_RegisterResume
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_RegisterResume(VDM_DL_resumeGetFunc getFunc, VDM_DL_resumeSetFunc setFunc);
/*****************************************************************************/
// 	Description : dm  MMIDM_registerSessionStateObserver
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_ConfigSetNotificationVerificationMode(E_VDM_NotificationVerificationMode_t inMode);
/*****************************************************************************/
// 	Description : dm  MMIDM_ConfigSetSessionIDAsDec
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_ConfigSetSessionIDAsDec(IBOOL inSessionIDAsDec);
/*****************************************************************************/
// 	Description : dm  MMIDM_UTL_Logger_setDefaultLevel
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_UTL_Logger_setDefaultLevel( E_VDM_LOGLEVEL_TYPE inLevel);
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxObjSize
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxObjSize(IU32 inMaxObjSize);
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxMsgSize
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxMsgSize(IU32 inMaxMsgSize);
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setEncodeWBXMLMsg
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setEncodeWBXMLMsg(IBOOL inIsEncode);
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setCheckDDVersion
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setCheckDDVersion(IBOOL inDDVersionCheck);

/*****************************************************************************/
// 	Description : dm  MMIDM_Tree_registerExternalStorage
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Tree_registerExternalStorage( 
                                                    UTF8CStr inPath, 
                                                    VDM_Tree_ReadExternalCB inReadFunc, 
                                                    VDM_Tree_WriteExternalCB inWriteFunc, 
                                                    void* inContext
                                                    );
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_createInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Handle_t MMIDM_FUMO_createInstance(
                                            UTF8CStr inFumoRootURI, 
                                            VDM_FUMO_Callbacks_t* inCallbacks, 
                                            void* inContext
                                            );
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_FUMO_destroyInstance(VDM_Handle_t* ioFumoInstance);
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DC_createInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Handle_t MMIDM_SCOMO_DC_createInstance( 
                                                UTF8CStr inScomoDCName, 
                                                VDM_SCOMO_DC_Callbacks_t* inCallbacks, 
                                                void* inContext
                                                );
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_createInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Handle_t MMIDM_SCOMO_DP_createInstance( 
                                                UTF8CStr inScomoDPName, 
                                                VDM_SCOMO_DP_Callbacks_t* inCallbacks, 
                                                void* inContext
                                                );
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_create
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_create(UTF8CStr inScomoRootURI);
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_SessionController_triggerDL
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_SessionController_triggerDL(
	                                        VDM_Handle_t 	inInstance, 
	                                        E_VDM_FUMO_Session_t	inSessionType
                                            );
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_triggerSession
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_triggerSession( 
                                         VDM_Handle_t inFumoInstance, 
                                         IU8* inMessage, 
                                         IU32 inMessageLen, 
                                         E_VDM_FUMO_Client_t inClientType
                                         );
/*****************************************************************************/
// 	Description : dm  MMIDM_TriggerNIADMSession
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_TriggerNIADMSession( 
                                         IU8* inMessage, 
                                         IU32 inMessageLen, 
                                         VDM_NIAHandlerCB inHandlerFunc, 
                                         VDM_SessionContext_t* inSessionContext
                                         );
/*****************************************************************************/
// 	Description : dm  MMIDM_TriggerDMSession
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_TriggerDMSession( 
                                      UTF8CStr inPath, 
                                      UTF8CStr inGenericAlertType, 
                                      IU8* inMessage, 
                                      IU32 inMessageLen, 
                                      VDM_SessionContext_t* inSessionContext
                                      );
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_PL_INV_getComponentId
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_PL_INV_getComponentId(
                                                 UTF8CStr inRoot, 
                                                 UTF8CStr inCompId, 
                                                 IU8* inBuffer, 
                                                 IU32* ioLength
                                                 );
/*****************************************************************************/
// 	Description : dm  MMIDM_UTL_utf8dup
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC UTF8Str MMIDM_UTL_utf8dup(UTF8CStr src);
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_PL_INV_getAllComponents
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_PL_INV_getAllComponents( 
                                                    UTF8CStr inRoot, 
                                                    IU8* inBuffer, 
                                                    IU32* ioLength
                                                    );
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_DP_destroyInstance(VDM_Handle_t* inScomoDPInstance);
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_PL_INV_addComponent
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_PL_INV_addComponent( 
                                               UTF8CStr inRoot, 
                                               UTF8CStr inCompId, 
                                               UTF8CStr inCompName, 
                                               UTF8CStr InPkgId, 
                                               UTF8CStr inCompVersion, 
                                               UTF8CStr inCompDesc, 
                                               UTF8CStr inCompEnv, 
                                               UTF8CStr inParentOperationPath
                                               );
/*****************************************************************************/
// 	Description : dm  VDM_SCOMO_PL_INV_deleteComponent
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_PL_INV_deleteComponent( 
                                                  UTF8CStr inRoot, 
                                                  UTF8CStr inCompId
                                                  );

/*****************************************************************************/
// 	Description : dm  VDM_SCOMO_DC_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_DC_destroyInstance(VDM_Handle_t* ioScomoDCInstance);

/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_setUpdateResult
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_setUpdateResult(
                                            VDM_Handle_t inFumoInstance, 
                                            E_VDM_FUMO_RESULT_t inResultCode
                                            );


/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_createInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Handle_t MMIDM_LAWMO_createInstance(
                                               UTF8CStr inLAWMORootURI,
                                               VDM_LAWMO_Callbacks_t *inCallbacks,
                                               void *inContext
                                               );
/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_setLockState
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_LAWMO_setLockState(
                                          VDM_Handle_t inLAWMOInstance, 
                                          E_VDM_LAWMO_STATE_t inState
                                          );

/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_LAWMO_destroyInstance(VDM_Handle_t* ioLAWMOInstance);

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_destroy
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_destroy(void);

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_setInstallationResult
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DP_setInstallationResult( 
                                                    VDM_Handle_t inScomoDPInstance, 
                                                    E_VDM_SCOMO_RESULT_t inResult
                                                    );

/*****************************************************************************/
// 	Description : dm  MMIDM_IU32toa
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_IU32toa(IU32 value, char *string, IU8 radix);

/*****************************************************************************/
// 	Description : dm  MMIDM_atoIU32
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/

PUBLIC IU32 MMIDM_atoIU32(const char *string, IU8 radix, IBOOL *success);

/*****************************************************************************/
//  Description : set dp header
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetDPHeader(void *header,BOOLEAN is_old_version);

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DC_deleteFromInventory
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DC_deleteFromInventory(VDM_Handle_t inScomoDCInstance);

/*****************************************************************************/
// 	Description : dm  MMIDM_Tree_writeToPersistentStorage
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Tree_writeToPersistentStorage(void);

/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxNetRetries
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxNetRetries(uint32 inMaxNetRetries);


/*****************************************************************************/
// 	Description : get fota update result
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC DM_FOTA_RESULT_E MMIDM_GetFotaUpdateResult(void);

/*****************************************************************************/
// 	Description : set fota update flag
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetFotaUpdateFlag(DM_FOTA_FLAG_OPERATION_E op);

/*****************************************************************************/
// Description :    Initialize fota  device,must first call before 
//					any other fota operation
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC DM_FOTA_ERROR_E MMIDM_FOTA_ImageArea_Open(void);

/*****************************************************************************/
// Description :    Used to write data to fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32 MMIDM_FOTA_ImageArea_Write(uint8 *buffer,uint32 size);


/*****************************************************************************/
// Description :    Used to read data from fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_ImageArea_read(uint8 *buffer,uint32 size);

/*****************************************************************************/
// Description :    Used to erase fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_ImageArea_Erase(void);

/*****************************************************************************/
// Description :    Used to close fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC void  MMIDM_FOTA_ImageArea_Close(void);
/*****************************************************************************/
// Description :    Used to modify flag field of fota device.
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_SetUpdateFlag(DM_FOTA_FLAG_OPERATION_E op);

/*****************************************************************************/
// Description :    Used to query update result 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC DM_FOTA_RESULT_E  MMIDM_FOTA_GetUpdateResult(void);

/*****************************************************************************/
// Description :    Used to query fota device space size 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_GetSpaceSize(void);

/*****************************************************************************/
// Description :    Used to Set user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_SetUserFlag(uint32 flag);

/*****************************************************************************/
// Description :   Used to get user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_GetUserFlag(uint32 *flag);

/*****************************************************************************/
//	Description : is need reset
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedReset(void);
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_SessionHandler_DL_trigger
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DP_SessionHandler_DL_trigger(
	VDM_Handle_t	inInstance, 
	E_VDM_SCOMO_DP_Session_t	inSessionType);

/*****************************************************************************/
//  Description : check is runing in backgroup
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIDM_IsBackgroupRun(void);

/*****************************************************************************/
//  Description : check is need display progress win
//  Global resource dependence : 
//  Author: jian.ma
//  Note: alert 1101 need display progress win
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedDisplayProgresswin(void);

/*****************************************************************************/
//  Description : Function used to extract app file 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ExtractAPPFile(MMIDM_EXT_FILE_INFO_T *file_info);

/*****************************************************************************/
// 	Description : to get choice list info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_MMI_ListContext_t* MMIDM_GetChoiceListInfo(void);

/*****************************************************************************/
// 	Description : to clean choice list info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CleanChoiceListInfo(void);

/*****************************************************************************/
//	Description : The function is NIA_Plugin_handleAlert
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC VDM_Error NIA_Plugin_handleAlert(
	E_VDM_NIA_UIMode_t		inUIMode,
	IU16					inDMVersion,
	UTF8Str 				inVendorSpecificData,
	IU16					inVendorSpecificDataLength,
	VDM_SessionContext_t*	inSessionContext);
	
      	
	
#ifdef DM_SUPPORT_DFRESH   
/********************************************************************************
* Send the data for the next msg to the specified address. This function is
* not expected to return until all the data has been sent.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \param   inConnId    Reference to the connection, as returned by 
*                      VDM_Comm_PL_HTTP_open().
*
* \param   inMsg       Pointer to the data to be sent.
*
* \param   inMsgLen    The length of the data to send.
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error)
********************************************************************************/
PUBLIC VDM_Error VDM_Comm_PL_HTTP_send(
    void*   inContext, 
    IS32    inConnId, 
    IU8 *   inMsg, 
    IS32    inMsgLength);
  
/********************************************************************************
* End the request/response exchange for the current message.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \param   inConnId    Reference to the connection, as returned by 
*                      VDM_Comm_PL_HTTP_open().
*******************************************************************************/
PUBLIC void VDM_Comm_PL_HTTP_close(
    void*   inContext, 
    IS32    inConnId);
    
/********************************************************************************
* Terminate the connection.
*
* Terminate the communications support code and perform any cleanup required.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error)
********************************************************************************/
PUBLIC VDM_Error VDM_Comm_PL_HTTP_term(void* inContext);

                         
PUBLIC VDM_Error VDM_Comm_PL_HTTP_init_dfresh(void);

/*****************************************************************************/
//	Description : LAWMO confirm factoryreset
//	Global resource dependence : none
//	Author:mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC IBOOL DM_ConfirmFactoryReset(void);
/*****************************************************************************/
//	Description : LAWMO confirm lock
//	Global resource dependence : none
//	Author:mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC IBOOL DM_ConfirmPartiallyLock(void);
/*****************************************************************************/
//	Description : LAWMO confirm unlock
//	Global resource dependence : none
//	Author:mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC IBOOL DM_ConfirmUnLock(void);
#endif	

#ifdef __cplusplus
}
#endif

#endif





