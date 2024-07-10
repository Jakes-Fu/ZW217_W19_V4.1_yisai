/*************************************************************************
 ** File Name:      mmidm_pl_mmi.c                                       *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/03                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/03     jian.ma          Create.                              *
*************************************************************************/
#define _MMIDM_PL_MMI_C_
//#include "std_header.h"

#include "mmi_app_dm_trc.h"
#ifdef DM_SUPPORT
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmidm_internal.h"
#ifdef DM_SUPPORT_REDBENCH
#include "vdm_utl_utf8.h"
#include "fota.h"
#include "vdm_scomo_dp_instance.h"
#include "vdm_fumo_instance.h"
#endif
#include "os_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define DM_VDM_WIN32        
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern void VDM_PL_IU32toa (IU32 value, char *string, IU8 radix);
#ifdef DM_SUPPORT_REDBENCH
extern VDM_Error VDM_FUMO_SessionController_triggerDL(
	VDM_FUMO_Instance_t*	inInstance, 
	E_VDM_FUMO_Session_t	inSessionType);
extern VDM_Handle_t VDM_LAWMO_createInstance(
                                               UTF8CStr inLAWMORootURI,
                                               VDM_LAWMO_Callbacks_t *inCallbacks,
                                               void *inContext);
extern VDM_Error VDM_LAWMO_setLockState(VDM_Handle_t inLAWMOInstance, E_VDM_LAWMO_STATE_t inState);
extern void VDM_LAWMO_destroyInstance(VDM_Handle_t* ioLAWMOInstance);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//	Description : vdm run
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Run(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  MMIDM_Run "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_63_112_2_18_2_13_54_177,(uint8*)"");
    VDM_run();
}

/*****************************************************************************/
//	Description : dm destroy
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Destroy(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  MMIDM_Destroy "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_73_112_2_18_2_13_54_178,(uint8*)"");
    VDM_destroy();
}

/*****************************************************************************/
// 	Description : dm SessionCancel
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SessionCancel(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  MMIDM_SessionCancel "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_83_112_2_18_2_13_55_179,(uint8*)"");
    VDM_cancelSession();
}

/*****************************************************************************/
// 	Description : dm  NotifyNIASessionProceed
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_NotifyNIASessionProceed(void)
{
    return VDM_notifyNIASessionProceed();
}

/*****************************************************************************/
// 	Description : dm  MMIDM_registerSessionStateObserver
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_RegisterSessionStateObserver(VDM_SessionStateNotifyCB inHandlerFunc)
{
    return  VDM_registerSessionStateObserver (inHandlerFunc);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_RegisterResume
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_RegisterResume(VDM_DL_resumeGetFunc getFunc, VDM_DL_resumeSetFunc setFunc)
{
    VDM_DL_registerResume(getFunc,setFunc);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_registerSessionStateObserver
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_ConfigSetNotificationVerificationMode(E_VDM_NotificationVerificationMode_t inMode)
{
    return VDM_Config_setNotificationVerificationMode(inMode);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_ConfigSetSessionIDAsDec
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_ConfigSetSessionIDAsDec(IBOOL inSessionIDAsDec)
{
    return VDM_Config_setSessionIDAsDec(inSessionIDAsDec);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_UTL_Logger_setDefaultLevel
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_UTL_Logger_setDefaultLevel( E_VDM_LOGLEVEL_TYPE inLevel)
{
    return VDM_UTL_Logger_setDefaultLevel(inLevel);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxObjSize
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxObjSize(IU32 inMaxObjSize)
{
    return VDM_Config_setMaxObjSize(inMaxObjSize);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxMsgSize
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxMsgSize(IU32 inMaxMsgSize)
{
    return VDM_Config_setMaxMsgSize(inMaxMsgSize);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setEncodeWBXMLMsg
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setEncodeWBXMLMsg(IBOOL inIsEncode)
{
    return VDM_Config_setEncodeWBXMLMsg(inIsEncode);
}

/*****************************************************************************/
// 	Description : dm  MMIDM_Config_GetEncodeWBXMLMsg
//	Global resource dependence : 
//  Author:taiping.lai
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_Config_GetEncodeWBXMLMsg(void)
{
    BOOLEAN is_wbxml = VDM_Config_getEncodeWBXMLMsg();
    
    SCI_TRACE_LOW("MMIDM_Config_GetEncodeWBXMLMsg is_wbxml = %d",is_wbxml);
    return is_wbxml;
}

/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setCheckDDVersion
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setCheckDDVersion(IBOOL inDDVersionCheck)
{
   return VDM_Config_setCheckDDVersion(inDDVersionCheck);
}

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
                                                    )
{
    return VDM_Tree_registerExternalStorage(inPath,inReadFunc,inWriteFunc,inContext);
}
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
                                            )
{
    return VDM_FUMO_createInstance(inFumoRootURI,inCallbacks,inContext);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_FUMO_destroyInstance(VDM_Handle_t* ioFumoInstance)
{
    VDM_FUMO_destroyInstance(ioFumoInstance);
}
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
                                                )
{
    return VDM_SCOMO_DC_createInstance(inScomoDCName,inCallbacks,inContext);
}
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
                                                )
{
    return VDM_SCOMO_DP_createInstance(inScomoDPName,inCallbacks,inContext);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_create
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_create(UTF8CStr inScomoRootURI)
{
    return VDM_SCOMO_create(inScomoRootURI);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_SessionController_triggerDL
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_SessionController_triggerDL(
	                                        VDM_Handle_t 	inInstance, 
	                                        E_VDM_FUMO_Session_t	inSessionType)
{
    VDM_FUMO_Instance_t * fumo_instance = (VDM_FUMO_Instance_t *)inInstance;
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_FUMO_SessionController_triggerDL pRootURI=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_243_112_2_18_2_13_55_180,(uint8*)"s",fumo_instance->rootURI);
    return VDM_FUMO_SessionController_triggerDL(fumo_instance,inSessionType);
}

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_SessionHandler_DL_trigger
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DP_SessionHandler_DL_trigger(
	VDM_Handle_t	inInstance, 
	E_VDM_SCOMO_DP_Session_t	inSessionType)
{
    VDM_SCOMO_DP_Instance_t *scomo_dp_instance = (VDM_SCOMO_DP_Instance_t *)inInstance;
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SCOMO_DP_SessionHandler_DL_trigger pRootURI %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_256_112_2_18_2_13_55_181,(uint8*)"s",scomo_dp_instance->pRootURI);
    return VDM_SCOMO_DP_SessionHandler_DL_trigger(scomo_dp_instance,inSessionType);
}


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
                                         )
{
    return VDM_FUMO_triggerSession(inFumoInstance,inMessage,inMessageLen,inClientType);
}
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
                                         )
{
    return VDM_triggerNIADMSession(inMessage,inMessageLen,inHandlerFunc,inSessionContext);
}
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
                                      )
{
    return VDM_triggerDMSession(inPath,inGenericAlertType,inMessage,inMessageLen,inSessionContext);
}
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
                                                 )
{
    return VDM_SCOMO_PL_INV_getComponentId(inRoot,inCompId,inBuffer,ioLength);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_UTL_utf8dup
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC UTF8Str MMIDM_UTL_utf8dup(UTF8CStr src)
{
    return VDM_UTL_utf8dup(src);
}
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
                                                    )
{
    return VDM_SCOMO_PL_INV_getAllComponents(inRoot,inBuffer,ioLength);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_DP_destroyInstance(VDM_Handle_t* inScomoDPInstance)
{
    VDM_SCOMO_DP_destroyInstance (inScomoDPInstance);
}
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
                                               )
{
    return VDM_SCOMO_PL_INV_addComponent(inRoot,inCompId,inCompName,InPkgId,inCompVersion,inCompDesc,inCompEnv,inParentOperationPath);
}
/*****************************************************************************/
// 	Description : dm  VDM_SCOMO_PL_INV_deleteComponent
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_PL_INV_deleteComponent( 
                                                  UTF8CStr inRoot, 
                                                  UTF8CStr inCompId
                                                  )
{
    return VDM_SCOMO_PL_INV_deleteComponent(inRoot,inCompId);
}

/*****************************************************************************/
// 	Description : dm  VDM_SCOMO_DC_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_DC_destroyInstance(VDM_Handle_t* ioScomoDCInstance)
{
    VDM_SCOMO_DC_destroyInstance(ioScomoDCInstance);
}

/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_setUpdateResult
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_setUpdateResult(VDM_Handle_t inFumoInstance, E_VDM_FUMO_RESULT_t inResultCode)
{
    return VDM_FUMO_setUpdateResult(inFumoInstance,inResultCode);
}

/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_createInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Handle_t MMIDM_LAWMO_createInstance(
                                               UTF8CStr inLAWMORootURI,
                                               VDM_LAWMO_Callbacks_t *inCallbacks,
                                               void *inContext)
{
    return VDM_LAWMO_createInstance(inLAWMORootURI,inCallbacks,inContext);
}
/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_setLockState
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_LAWMO_setLockState(VDM_Handle_t inLAWMOInstance, E_VDM_LAWMO_STATE_t inState)
{
    return VDM_LAWMO_setLockState(inLAWMOInstance,inState);
}

/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_LAWMO_destroyInstance(VDM_Handle_t* ioLAWMOInstance)
{
    VDM_LAWMO_destroyInstance(ioLAWMOInstance);
}

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_destroy
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_destroy(void)
{
    VDM_SCOMO_destroy();
}


/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_setInstallationResult
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DP_setInstallationResult( 
                                                    VDM_Handle_t inScomoDPInstance, 
                                                    E_VDM_SCOMO_RESULT_t inResult
                                                    )
{
    return VDM_SCOMO_DP_setInstallationResult(inScomoDPInstance,inResult);
}

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DC_deleteFromInventory
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DC_deleteFromInventory(VDM_Handle_t inScomoDCInstance)
{
    return VDM_SCOMO_DC_deleteFromInventory(inScomoDCInstance);
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Tree_writeToPersistentStorage
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Tree_writeToPersistentStorage(void)
{
    return VDM_Tree_writeToPersistentStorage();
}

/*****************************************************************************/
// 	Description : dm  MMIDM_IU32toa
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_IU32toa(IU32 value, char *string, IU8 radix)
{
    VDM_PL_IU32toa(value,string,radix);
}

/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxNetRetries
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxNetRetries(uint32 inMaxNetRetries)
{
    return VDM_Config_setMaxNetRetries(inMaxNetRetries);
}

/*****************************************************************************/
// 	Description : get fota update result
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC DM_FOTA_RESULT_E MMIDM_GetFotaUpdateResult(void)
{
    DM_FOTA_RESULT_E  result = DM_FOTA_NONE;
#ifndef WIN32
    FOTA_ERROR_E  fota_error = FOTA_ERR_NONE;

    fota_error = FOTA_ImageArea_Open();

    if(FOTA_ERR_NONE == fota_error)
    {
        result = (DM_FOTA_RESULT_E)FOTA_GetUpdateResult();
    }
    FOTA_ImageArea_Close();
#endif
    return result;
}
/*****************************************************************************/
// 	Description : set fota update flag
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetFotaUpdateFlag(DM_FOTA_FLAG_OPERATION_E op)
{
    BOOLEAN       result = FALSE;
#ifndef WIN32
    FOTA_ERROR_E  fota_error = FOTA_ERR_NONE;
    fota_error = FOTA_ImageArea_Open();
    if(FOTA_ERR_NONE == fota_error)
    {
        result= FOTA_SetUpdateFlag((FOTA_FLAG_OPERATION_E)op);
    }
    FOTA_ImageArea_Close();
#endif
    return result;
}

/*****************************************************************************/
// Description :    Initialize fota  device,must first call before 
//					any other fota operation
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC DM_FOTA_ERROR_E MMIDM_FOTA_ImageArea_Open(void)
{
#ifndef WIN32
    return (DM_FOTA_ERROR_E)FOTA_ImageArea_Open();
#else
        return DM_FOTA_ERR_NONE;
#endif
}

/*****************************************************************************/
// Description :    Used to write data to fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32 MMIDM_FOTA_ImageArea_Write(uint8 *buffer,uint32 size)
{
#ifndef WIN32
    return FOTA_ImageArea_Write(buffer,size);
#else
    return size;
#endif
}


/*****************************************************************************/
// Description :    Used to read data from fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_ImageArea_read(uint8 *buffer,uint32 size)
{
#ifndef WIN32
    return FOTA_ImageArea_read(buffer,size);
#else
    return size;
#endif
}

/*****************************************************************************/
// Description :    Used to erase fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_ImageArea_Erase(void)
{
#ifndef WIN32
    return FOTA_ImageArea_Erase();
#else
    return 0;
#endif
}

/*****************************************************************************/
// Description :    Used to close fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC void  MMIDM_FOTA_ImageArea_Close(void)
{
#ifndef WIN32
    FOTA_ImageArea_Close();
#endif
}
/*****************************************************************************/
// Description :    Used to modify flag field of fota device.
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_SetUpdateFlag(DM_FOTA_FLAG_OPERATION_E op)
{
#ifndef WIN32
    return FOTA_SetUpdateFlag((FOTA_FLAG_OPERATION_E)op);
#else
    return TRUE;
#endif
}


/*****************************************************************************/
// Description :    Used to query update result 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC DM_FOTA_RESULT_E  MMIDM_FOTA_GetUpdateResult(void)
{
#ifndef WIN32
    return (DM_FOTA_RESULT_E)FOTA_GetUpdateResult();
#else
    return DM_FOTA_NONE;
#endif
}

/*****************************************************************************/
// Description :    Used to query fota device space size 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_GetSpaceSize(void)
{
#ifndef WIN32
    return FOTA_GetSpaceSize();
#else
    return 0;
#endif
}
/*****************************************************************************/
// Description :    Used to Set user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_SetUserFlag(uint32 flag)
{
    BOOLEAN       result = FALSE;
#ifndef WIN32
    result = FOTA_SetUserFlag(flag);
#endif
    return result;
}

/*****************************************************************************/
// Description :   Used to get user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_GetUserFlag(uint32 *flag)
{
    BOOLEAN       result = FALSE;
#ifndef WIN32
    result = FOTA_GetUserFlag(flag);
#endif
    return result;
}
#else
/*****************************************************************************/
//	Description : vdm run
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Run(void)
{
}

/*****************************************************************************/
//	Description : dm destroy
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_Destroy(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  MMIDM_Destroy "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_657_112_2_18_2_13_56_182,(uint8*)"");

}

/*****************************************************************************/
// 	Description : dm SessionCancel
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SessionCancel(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  MMIDM_SessionCancel "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_INTERNAL_667_112_2_18_2_13_56_183,(uint8*)"");
 
}

/*****************************************************************************/
// 	Description : dm  NotifyNIASessionProceed
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_NotifyNIASessionProceed(void)
{
    return VDM_ERR_OK;
}

/*****************************************************************************/
// 	Description : dm  MMIDM_registerSessionStateObserver
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_RegisterSessionStateObserver(VDM_SessionStateNotifyCB inHandlerFunc)
{
    return  VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_RegisterResume
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_RegisterResume(VDM_DL_resumeGetFunc getFunc, VDM_DL_resumeSetFunc setFunc)
{
    
}
/*****************************************************************************/
// 	Description : dm  MMIDM_registerSessionStateObserver
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_ConfigSetNotificationVerificationMode(E_VDM_NotificationVerificationMode_t inMode)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_ConfigSetSessionIDAsDec
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_ConfigSetSessionIDAsDec(IBOOL inSessionIDAsDec)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_UTL_Logger_setDefaultLevel
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_UTL_Logger_setDefaultLevel( E_VDM_LOGLEVEL_TYPE inLevel)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxObjSize
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxObjSize(IU32 inMaxObjSize)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxMsgSize
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxMsgSize(IU32 inMaxMsgSize)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setEncodeWBXMLMsg
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setEncodeWBXMLMsg(IBOOL inIsEncode)
{
    return VDM_ERR_OK;
}

/*****************************************************************************/
// 	Description : dm  MMIDM_Config_GetEncodeWBXMLMsg
//	Global resource dependence : 
//  Author:taiping.lai
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_Config_GetEncodeWBXMLMsg(void)
{
    return FALSE;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setCheckDDVersion
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setCheckDDVersion(IBOOL inDDVersionCheck)
{
   return VDM_ERR_OK;
}

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
                                                    )
{
    return VDM_ERR_OK;
}
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
                                            )
{
    return PNULL;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_FUMO_destroyInstance(VDM_Handle_t* ioFumoInstance)
{

}
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
                                                )
{
    return PNULL;
}
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
                                                )
{
    return PNULL;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_create
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_create(UTF8CStr inScomoRootURI)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_SessionController_triggerDL
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_SessionController_triggerDL(
	                                        VDM_Handle_t	inInstance, 
	                                        E_VDM_FUMO_Session_t	inSessionType)
{
    return VDM_ERR_OK;
}
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
                                         )
{
    return VDM_ERR_OK;
}
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
                                         )
{
    return VDM_ERR_OK;
}
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
                                      )
{
    return VDM_ERR_OK;
}
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
                                                 )
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_UTL_utf8dup
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC UTF8Str MMIDM_UTL_utf8dup(UTF8CStr src)
{
    return 0;
}
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
                                                    )
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_DP_destroyInstance(VDM_Handle_t* inScomoDPInstance)
{
    
}
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
                                               )
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  VDM_SCOMO_PL_INV_deleteComponent
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_PL_INV_deleteComponent( 
                                                  UTF8CStr inRoot, 
                                                  UTF8CStr inCompId
                                                  )
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  VDM_SCOMO_DC_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_DC_destroyInstance(VDM_Handle_t* ioScomoDCInstance)
{

}

/*****************************************************************************/
// 	Description : dm  MMIDM_FUMO_setUpdateResult
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_FUMO_setUpdateResult(VDM_Handle_t inFumoInstance, E_VDM_FUMO_RESULT_t inResultCode)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_createInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Handle_t MMIDM_LAWMO_createInstance(UTF8CStr inLAWMORootURI,VDM_LAWMO_Callbacks_t *inCallbacks,void *inContext)
{
    return PNULL;
}
/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_setLockState
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_LAWMO_setLockState(VDM_Handle_t inLAWMOInstance, E_VDM_LAWMO_STATE_t inState)
{
    return VDM_ERR_OK;
}

/*****************************************************************************/
// 	Description : dm  VDM_LAWMO_destroyInstance
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_LAWMO_destroyInstance(VDM_Handle_t* ioLAWMOInstance)
{
    
}

/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_destroy
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SCOMO_destroy(void)
{

}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_setInstallationResult
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DP_setInstallationResult( 
                                                    VDM_Handle_t inScomoDPInstance, 
                                                    E_VDM_SCOMO_RESULT_t inResult
                                                    )
{
    return VDM_ERR_OK;
}

/*****************************************************************************/
// 	Description : dm  MMIDM_IU32toa
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_IU32toa(IU32 value, char *string, IU8 radix)
{
    VDM_PL_IU32toa(value,string,radix);
}

/*****************************************************************************/
// 	Description : get fota update result
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC DM_FOTA_RESULT_E MMIDM_GetFotaUpdateResult(void)
{
    return DM_FOTA_NONE;
}
/*****************************************************************************/
// 	Description : set fota update flag
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetFotaUpdateFlag(DM_FOTA_FLAG_OPERATION_E op)
{
    return FALSE;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DC_deleteFromInventory
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DC_deleteFromInventory(VDM_Handle_t inScomoDCInstance)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Tree_writeToPersistentStorage
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Tree_writeToPersistentStorage(void)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_Config_setMaxNetRetries
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_Config_setMaxNetRetries(uint32 inMaxNetRetries)
{
    return VDM_ERR_OK;
}

/*****************************************************************************/
// Description :    Initialize fota  device,must first call before 
//					any other fota operation
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC DM_FOTA_ERROR_E MMIDM_FOTA_ImageArea_Open(void)
{
    return DM_FOTA_ERR_NONE;
}

/*****************************************************************************/
// Description :    Used to write data to fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32 MMIDM_FOTA_ImageArea_Write(uint8 *buffer,uint32 size)
{
    return size;
}


/*****************************************************************************/
// Description :    Used to read data from fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_ImageArea_read(uint8 *buffer,uint32 size)
{
    return size;
}

/*****************************************************************************/
// Description :    Used to erase fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_ImageArea_Erase(void)
{
    return 0;
}

/*****************************************************************************/
// Description :    Used to close fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC void  MMIDM_FOTA_ImageArea_Close(void)
{

}
/*****************************************************************************/
// Description :    Used to modify flag field of fota device.
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_SetUpdateFlag(DM_FOTA_FLAG_OPERATION_E op)
{
    return TRUE;
}


/*****************************************************************************/
// Description :    Used to query update result 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC DM_FOTA_RESULT_E  MMIDM_FOTA_GetUpdateResult(void)
{
    return DM_FOTA_NONE;
}

/*****************************************************************************/
// Description :    Used to query fota device space size 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC uint32  MMIDM_FOTA_GetSpaceSize(void)
{
    return 0;
}
/*****************************************************************************/
// 	Description : dm  MMIDM_SCOMO_DP_SessionHandler_DL_trigger
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_Error MMIDM_SCOMO_DP_SessionHandler_DL_trigger(
	VDM_Handle_t	inInstance, 
	E_VDM_SCOMO_DP_Session_t	inSessionType)
{
    return VDM_ERR_OK;
}
/*****************************************************************************/
// Description :    Used to Set user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_SetUserFlag(uint32 flag)
{
    return FALSE;
}

/*****************************************************************************/
// Description :   Used to get user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_FOTA_GetUserFlag(uint32 *flag)
{
    return FALSE;
}

extern IU32 VDM_PL_atoIU32(const char *string, IU8 radix, IBOOL *success);
/*****************************************************************************/
// 	Description : dm  MMIDM_atoIU32
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/

PUBLIC IU32 MMIDM_atoIU32(const char *string, IU8 radix, IBOOL *success)
{
    return VDM_PL_atoIU32(string, radix, success);
}
#endif
#endif