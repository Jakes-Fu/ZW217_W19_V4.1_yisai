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
#include "mmi_nv.h"
#ifdef DM_SUPPORT
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmidm_app.h"
#include "mmi_id.h"
#include "mmidm_text.h"
#include "mmi_appmsg.h"
#include "mmidm_internal.h"
#include "mmidm_config.h"
#include "mmipub.h"
#include "mmidm_nv.h"
//#include "vdm_fumo_instance.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

#include "gui_ucs2b_converter.h" //@CR121389
#ifdef PIM_SUPPORT
#include "mmipim.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define VDM_ETAG_LEN                    256
#ifndef BUFFER_SIZE
#define BUFFER_SIZE                     4096
#endif
#define MAX_ID_LEN                      128
#define MAX_COMPARE_PATH_LEN           128
#define DM_SCOMO_INSTALL_TREE_PATH      "DownloadInstall"
#define DM_ALERT_CHOICE_MAX_NUM         10
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint8                       inPath[70];
    VDM_Tree_ReadExternalCB     inReadFunc;
    VDM_Tree_WriteExternalCB    inWriteFunc;
    void*                       inContext;
} DM_TREE_ITEM_T;

typedef struct DM_RESUME_TAG
{ 
    uint32 offset;
    char eTag[VDM_ETAG_LEN+1];
}DM_RESUME_DATA_STRUCT;

typedef struct _SCOMO_DC_t
{
    VDM_Handle_t handle;
    UTF8Str id;
    struct _SCOMO_DC_t *next;
} SCOMO_DC_t;

typedef struct
{
    VDM_Handle_t scomoDP;
    VDM_Handle_t scomoDC;
    uint8 compParentOperationPath[MAX_COMPARE_PATH_LEN];//decide which component in DC process
    uint32 dcCount;
    SCOMO_DC_t *dcList;
} SCOMO_Plugin_t;

typedef struct  
{
    uint8   name[(DM_STRING_LENGTH+1)<<1];
    uint8   version[(DM_STRING_LENGTH+1)<<1];
 }MMIDM_SCOMO_APP_T;
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

const char *s_resume_file = DM_CLIENT_PL_RESUME_FUMO_FILE;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern IS32 VDM_PL_strcmp(const char *string1, const char *string2);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL VDM_MMI_Observer_t s_observer;
LOCAL VDM_Handle_t  s_fumo_instance = PNULL;
LOCAL DM_RESUME_DATA_STRUCT  s_vdm_resume = {0};
LOCAL VDM_Handle_t s_lawmo = NULL;
LOCAL MMIDM_SCOMO_APP_T s_dp_header = {0};
LOCAL SCOMO_Plugin_t* s_scomoInstance = NULL;
LOCAL DM_SESSION_TYPE  s_task_session_type = DM_SESSION_NONE;
LOCAL BOOLEAN s_is_need_reset = FALSE;
//LOCAL E_VDM_NIA_UIMode_t s_UIMode = E_VDM_NIA_UIMode_NotSpecified;
//LOCAL MMIDM_PL_ALERT_TYPE s_pl_alert_type = DM_PL_ALERT_1101;//maryxiao cr234319
LOCAL VDM_MMI_ListContext_t s_list_context={0};
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create dm thread
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint32 MMIDM_CreateThread(void);

/*****************************************************************************/
//  Description : register Call back function
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_registerCallbacks(void);


/*****************************************************************************/
//  Description : FUMO_Plugin_init
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error FUMO_Plugin_init(void* inParam);

/*****************************************************************************/
//	Description : FUMO_Plugin_term
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void FUMO_Plugin_term(void);

/*****************************************************************************/
//  Description : LAWMO_Plugin_init
//  Global resource dependence : none
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL VDM_Error LAWMO_Plugin_init(const uint8 *inDMTreeURI);

/*****************************************************************************/
//  Description : ui 
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL IBOOL DM_ConfirmPartiallyLockCB(VDM_Handle_t inLAWMOInstance,void *inDmcContext);

/*****************************************************************************/
//  Description : LAWMO confirm unlock cb
//  Global resource dependence : none
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL IBOOL DM_ConfirmUnLockCB(VDM_Handle_t inLAWMOInstance,void *inDmcContext);

/*****************************************************************************/
//  Description : LAWMO Factory reset cb
//  Global resource dependence : none
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL IBOOL DM_ConfirmFactoryResetCB(VDM_Handle_t inLAWMOInstance,void *inDmcContext);

/*****************************************************************************/
//  Description : ui 
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL IBOOL DM_FumoConfirmUpdate(
      VDM_Handle_t  inFumoInstance,
      void*         inContext);

/*****************************************************************************/
//  Description : ui 
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL IBOOL DM_FumoExecuteUpdate(
    UTF8CStr        inUpdatePkgPath,
    VDM_Handle_t    inFumoInstance,
    void*           inDmcContext,
    IU32*           outFumoResult);
/*****************************************************************************/
//  Description : SCOMO_Plugin_init
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error SCOMO_Plugin_init(void* inParam);

/*****************************************************************************/
//  Description : handle confirm install action
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoConfirmInstallation(
                                         VDM_Handle_t   inScomoDPInstance,
                                         void*           inDmcContext,
                                         UTF8CStr        inOperationPath
                                        );

/*****************************************************************************/
//  Description : VDMC_PluginMng_init
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error VDMC_PluginMng_init(void);


/*****************************************************************************/
//	Description : get dm model
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceModel(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm man
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceMan(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm OEM
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceOEM(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm Language
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceLanguage(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceVersion(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm DevImei
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceImei(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);
/*****************************************************************************/
//	Description : get dm firmware version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceFwV(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);


/*****************************************************************************/
//	Description : get dm software version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceSwV(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm Hardware version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceHwV(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);
/*****************************************************************************/
//	Description : get dm profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDmProfile(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get empty info
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetEmptyInfo(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);
/*****************************************************************************/
//	Description : get dm gprs cmnet apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetAPN(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm gprs apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsAPN(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm gprs cmwap proxyaddr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMWapProxyaddr(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm gprs cmwap port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMWapPortNum(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm mms mmsc
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetMMSMmsc(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm mms profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetMMSProfile(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm wap conprofile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetWAPConprofile(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);
/*****************************************************************************/
//	Description : get dm wap start page
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetWAPStarPage(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);


/*****************************************************************************/
//	Description : get dm pim addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetPIMAddr(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm pim addressbook url
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetPIMAddressBookURL(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm pim calendarurl
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetPIMCalendarURL(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);


/*****************************************************************************/
//	Description : get dm srvaddr url
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetSrvAddrURL(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get vdm info string
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetVdmInfoString(
                           char * string,   //in
                           uint32 offset,   //in
                           void * buffer,   //in/out
                           uint32 bLength,  //in
                           uint32*vLength   //in/out
                           );
/*****************************************************************************/
//  Description : init dm session
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_initiateSession(DM_SESSION_TYPE type,char* msg_body, uint32 msg_size);

/*****************************************************************************/
//  Description : open dm alert win
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL void DM_OpenDMAlerthandleWin(
                                MMIDM_ALERT_TYPE_E mode, 
                                const uint8* text,
                                 uint16 len,
                                 uint32 max_timer
                                 );
/*!*******************************************************************************
 * Register routines to store and retrieve DL info to allow object download resume.
 * \ref VDM_DL_registerResume().
 *
 * \param	getFunc Function to get the stored data
 *
 * \param	setFunc Function to store the relevant data
 *
********************************************************************************/
LOCAL VDM_Error DM_DLResumeSetFunc(IU32 offset, UTF8Str eTag);

/*!*******************************************************************************
 * Function used to get the stored DL info to resume the download if possible.
 *
 *
 * \param	offset		The byte offset which was downloaded so far.
 *
 * \param	eTag		The eTag of the object from the previous download.
 *						Memory allocated to this parameter should be done via
 *						RDM_malloc and the RDM library is responsible to free it.
 *
 * \return	\ref RDM_ERR_defs "An error code" (RDM_ERR_OK if no error)
********************************************************************************/
LOCAL VDM_Error DM_DlResumeGetFunc(IU32* offset, UTF8Str* eTag);

/*****************************************************************************/
//	Description : DM_LawmoSetLockState
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void DM_LawmoSetLockState(E_VDM_LAWMO_STATE_t state);

/*****************************************************************************/
//	Description : get resume file name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL char* DM_GetResumeFileName(MMIDM_RESUME_TYPE type);

/*****************************************************************************/
//	Description : LAWMO_Plugin_term
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void LAWMO_Plugin_term(void);


/*****************************************************************************/
//  Description : 发送消息给app task
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void SendSignalToMMITask(uint16 sig_id,uint16 event_id, void *data_ptr);

/*****************************************************************************/
//  Description :  SCOMO ready to install
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
//  Return:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoReadyToInstall(
                                    VDM_Handle_t    inScomoDPInstance,
                                   void* inDmcContext,
                                   UTF8CStr inOperationPath,
                                   E_VDM_SCOMO_RESULT_t *outResult
                                   );

/*****************************************************************************/
//  Description :  SCOMO confirm uninstallation
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
//  Return:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoConfirmUninstallation(
                                         VDM_Handle_t   inScomoDCInstance,
                                          void*         inDmcContext
                                          );
/*****************************************************************************/
//  Description :  SCOMO ready to remove 
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
//  Return:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoReadyToRemove(
                                  VDM_Handle_t inScomoDCInstance,
                                  void* inDmcContext,
                                  E_VDM_SCOMO_RESULT_t *outResult
                                  );

/*****************************************************************************/
//  Description :  SCOMO plugin initiate DC
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN SCOMO_Plugin_initDCs(SCOMO_Plugin_t* pPlugin);

/*****************************************************************************/
//  Description :  resume SCOMO Download Session
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
//  Return:
/*****************************************************************************/
LOCAL VDM_Error DM_ResumeScomoDLSess(void);

/*****************************************************************************/
//	Description :  get SCOMO result code
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL int DM_GetScomoResultCode(SCOMO_Result_Code code);

/*****************************************************************************/
//	Description :  set DP install result
//	Global resource dependence : 
//	Author: jian.ma
//	Note:
//	Return:
/*****************************************************************************/
LOCAL VDM_Error DM_SetDPInstallResult(
                                      SCOMO_Result_Code result,
                                      int suiteID,
                                      const MMIDM_SCOMO_APP_T * dp_header
                                      );

/*****************************************************************************/
//	Description :  remove DC from Inventory
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL VDM_Error DM_RemoveDCFromInventory(int suiteID);

/*****************************************************************************/
//	Description : SCOMO Plugin create
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL SCOMO_Plugin_t* SCOMO_Plugin_create(void);

/*****************************************************************************/
//	Description : SCOMO Plugin term
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SCOMO_Plugin_term(void);

/*****************************************************************************/
//	Description : dm session state change function
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void DM_SessionStateChange(
                                E_VDM_SessionType_t     inType,      
                                E_VDM_SessionState_t    inState,
                                RDM_Error               inLastError, 
                                UTF8CStr                inDataStr,
                                VDM_SessionContext_t**  inContext
                                );

/*****************************************************************************/
//	Description : get dm profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetDmProfile(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm gprs cmnet apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetAPN(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);
                                

/*****************************************************************************/
//	Description : set dm gprs apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsAPN(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : get dm gprs user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsUsername(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : set dm gprs user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsUsername(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : get dm gprs pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsPw(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : get dm gprs pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsPw(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm gprs cmwap proxyaddr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMWapProxyaddr(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm gprs cmwap port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMWapPortNum(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm mms mmsc
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetMMSMmsc(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);
/*****************************************************************************/
//	Description : set dm mms profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetMMSProfile(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm wap conprofile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetWAPConprofile(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm wap start page
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetWAPStarPage(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm pim addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetPIMAddr(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm pim addressbook url
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetPIMAddressBookURL(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : set dm pim calendarurl
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetPIMCalendarURL(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : get dm gprs cmnet user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetUsername(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : set dm gprs cmnet user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetUsername(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : get dm gprs cmnet pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNePw(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : set dm gprs cmnet pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetPw(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : get dm gprs cmnet prixt addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetProxyaddr(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : set dm gprs cmnet prixt addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetProxyaddr(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//	Description : get dm gprs cmnet port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetPortNum(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength);

/*****************************************************************************/
//	Description : set dm gprs cmnet port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetPortNum(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : get dm streaming name
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingName(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming name
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingName(void *inContext, IU32 inOffset,const void* inData,
                              IU32 inDataLen, IU32 inTotalSize);
                              


/*****************************************************************************/
//  Description : get dm streaming name
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingProxy(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming Proxy
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingProxy(void *inContext, IU32 inOffset,const void* inData,
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//  Description : get dm streaming name
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingProxyPort(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming Proxy
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingProxyPort(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//  Description : get dm streaming nap id
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingNapID(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming nap id
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingNapID(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//  Description : get dm streaming net info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingNetInfo(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming Net Info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingNetInfo(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//  Description : get dm streaming Min UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingMinUDPPort(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming Min UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingMinUDPPort(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);

/*****************************************************************************/
//  Description : get dm streaming Max UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingMaxUDPPort(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength);

/*****************************************************************************/
//  Description : set dm streaming Max UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingMaxUDPPort(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize);
#endif
/*****************************************************************************/
//  Description : check is runing in backgroup
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_IsBackgroupRun(void);
#ifdef DM_SUPPORT_DFRESH
/*****************************************************************************/
//  Description : return the device lock status code
//  Global resource dependence : 
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceLockStatus(void* context, IU32 offset, void* buffer,IU32  bLength, IU32* vLength);
#endif
/*****************************************************************************/
// 	Description : to set choice list info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_SetChoiceListInfo(VDM_MMI_ListContext_t *list_constext_p);


//define dm init tree date table
LOCAL const DM_TREE_ITEM_T  s_dm_initdata_tab[] = 
{
    {"./DevInfo/Mod",                       DM_GetDeviceModel,          PNULL,                      PNULL},//m
    {"./DevInfo/Man",                       DM_GetDeviceMan,            PNULL,                      PNULL},//m
    {"./DevInfo/OEM",                       DM_GetDeviceOEM,            PNULL,                      PNULL},//o
    {"./DevInfo/Lang",                      DM_GetDeviceLanguage,       PNULL,                      PNULL},//o
    {"./DevInfo/DmV",                       DM_GetDeviceVersion,        PNULL,                      PNULL},//o
    {"./DevInfo/DevId",                     DM_GetDeviceImei,           PNULL,                      PNULL},//m
    {"./DevDetail/FwV",                     DM_GetDeviceFwV,            PNULL,                      PNULL},//o
    {"./DevDetail/SwV",                     DM_GetDeviceSwV,            PNULL,                      PNULL},//m
    {"./DevDetail/HwV",                     DM_GetDeviceHwV,            PNULL,                      PNULL},//o
    //DM
    {"./Settings/DM/ConnProfile",           DM_GetDmProfile,            DM_SetDmProfile,            PNULL}, //m
    //GPRS_CMNet配置信息
    {"./Settings/GPRS/CMNet/APN",           DM_GetGprsCMNetAPN,         DM_SetGprsCMNetAPN,         PNULL}, //m
    {"./Settings/GPRS/CMNet/UserName",      DM_GetGprsCMNetUsername,    DM_SetGprsCMNetUsername,    PNULL}, //m
    {"./Settings/GPRS/CMNet/PassWord",      DM_GetGprsCMNePw,           DM_SetGprsCMNetPw,          PNULL}, //m
    {"./Settings/GPRS/CMNet/Proxyaddr",     DM_GetGprsCMNetProxyaddr,   DM_SetGprsCMNetProxyaddr,   PNULL}, //m
    {"./Settings/GPRS/CMNet/ProxyPortNbr",  DM_GetGprsCMNetPortNum,     DM_SetGprsCMNetPortNum,     PNULL}, //m
    //GPRS_CMWap配置信息
    {"./Settings/GPRS/CMWap/APN",           DM_GetGprsAPN,              DM_SetGprsAPN,              PNULL}, //m
    {"./Settings/GPRS/CMWap/UserName",      DM_GetGprsUsername,         DM_SetGprsUsername,         PNULL}, //m
    {"./Settings/GPRS/CMWap/PassWord",      DM_GetGprsPw,               DM_SetGprsPw,               PNULL}, //m
    {"./Settings/GPRS/CMWap/Proxyaddr",     DM_GetGprsCMWapProxyaddr,   DM_SetGprsCMWapProxyaddr,   PNULL}, //m
    {"./Settings/GPRS/CMWap/ProxyPortNbr",  DM_GetGprsCMWapPortNum,     DM_SetGprsCMWapPortNum,     PNULL}, //m
    //MMS配置信息
    {"./Settings/MMS/MMSC",                 DM_GetMMSMmsc,              DM_SetMMSMmsc,              PNULL}, //m
    {"./Settings/MMS/ConnProfile",          DM_GetMMSProfile,           DM_SetMMSProfile,           PNULL}, //m
    //WAP2.0
    {"./Settings/WAP/ConnProfile",          DM_GetWAPConprofile,        DM_SetWAPConprofile,        PNULL}, //m
    {"./Settings/WAP/StartPage",            DM_GetWAPStarPage,          DM_SetWAPStarPage,          PNULL}, //m
#ifdef PIM_SUPPORT
    //PIM配置信息
    {"./Settings/PIM/Addr",                 DM_GetPIMAddr,              DM_SetPIMAddr,              PNULL}, //m
    {"./Settings/PIM/AddressBookURI",       DM_GetPIMAddressBookURL,    DM_SetPIMAddressBookURL,    PNULL}, //m
    {"./Settings/PIM/CalendarURI",          DM_GetPIMCalendarURL,       DM_SetPIMCalendarURL,       PNULL}, //m
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT     //STREAMING配置信息
    {"./Settings/Streaming/Name",           DM_GetStreamingName,        DM_SetStreamingName,        PNULL}, //m
    {"./Settings/Streaming/Proxy",          DM_GetStreamingProxy,       DM_SetStreamingProxy,       PNULL}, //m
    {"./Settings/Streaming/ProxyPort",      DM_GetStreamingProxyPort,   DM_SetStreamingProxyPort,   PNULL}, //m
    {"./Settings/Streaming/ConnProfile",    DM_GetStreamingNapID,       DM_SetStreamingNapID,       PNULL}, //m
    {"./Settings/Streaming/NetInfo",        DM_GetStreamingNetInfo,     DM_SetStreamingNetInfo,     PNULL}, //m
    {"./Settings/Streaming/MinUdpPort",     DM_GetStreamingMinUDPPort,  DM_SetStreamingMinUDPPort,  PNULL}, //m
    {"./Settings/Streaming/MaxUdpPort",     DM_GetStreamingMaxUDPPort,  DM_SetStreamingMaxUDPPort,  PNULL}, //m
#endif
   {"./DMAcc/TDAcc/AppAddr/SrvAddr/Addr",  DM_GetSrvAddrURL,           PNULL,                      PNULL}, //o
   {"./DMAcc/TDAcc",  DM_GetSrvAddrURL,           PNULL,                      PNULL}, //o
#ifdef DM_SUPPORT_DFRESH   
    {"./LAWMO/State",                       DM_GetDeviceLockStatus,     PNULL,                      PNULL},//m
#endif    
//    0
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/*!*******************************************************************************
* Perform MMI initialization. This will be called before any other MMI
* function is called. This MUST NOT cause anything to be displayed.
*
* \return  TRUE if initialization was successful, FALSE if not.
********************************************************************************/
PUBLIC IBOOL VDM_MMI_PL_init(VDM_MMI_Observer_t* inObserver)
{
    if (PNULL == inObserver)
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_init fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_877_112_2_18_2_14_14_551,(uint8*)"");
        return FALSE;
    }
    SCI_MEMCPY(&s_observer, inObserver, sizeof(VDM_MMI_Observer_t));
    return TRUE;
}


/*!*******************************************************************************
* Perform MMI termination. No MMI functions will be called after this has been
* called, except for perhaps \ref VDM_MMI_PL_init() to reinitialize the MMI
* system
********************************************************************************/
PUBLIC void VDM_MMI_PL_term(void)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_term inInfoType "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_891_112_2_18_2_14_14_552,(uint8*)"");
    SCI_MEMSET(&s_observer, 0, sizeof(VDM_MMI_Observer_t));
}

/*****************************************************************************/
//  Description : get Observer data ptr
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC VDM_MMI_Observer_t* MMIDM_GetObserverData(void)
{
    return &s_observer;
}

/*!*******************************************************************************
* Display an information message.
* 
* \param   inScreenContext     Context of the message screen to be displayed.
* \param   inInfoType          Whether this is a message from the DM server,
*                              or a message notifying DM Engine has been 
*                              initialized / shutting down.
*
* \note    in case of a timeout event, timeout callback should be called.
*
* \return  E_VDM_MMI_Result_OK on success or an MMI Error result.
 ********************************************************************************/
E_VDM_MMI_Result_t VDM_MMI_PL_infoMessage(
    VDM_MMI_ScreenContext_t*    inScreenContext, 
    E_VDM_MMI_InfoType_t        inInfoType)
{
    E_VDM_MMI_Result_t      result = E_VDM_MMI_Result_OK;

    //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_infoMessage inInfoType=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_921_112_2_18_2_14_14_553,(uint8*)"d",inInfoType);
    DM_OpenDMAlerthandleWin(MMIDM_ALERT_PL_DISPLAY, inScreenContext->displayText, (uint16)strlen((char*)inScreenContext->displayText),inScreenContext->maxDisplayTime);
    
    return result;
}


/*!
 *******************************************************************************
 * Prompt user to confirm / deny a session.
 * 
 * \param   inScreenContext     Context of the screen to be displayed.
 * \param   inDefaultCommand    Which command (confirm/deny), if any, should 
 *                              be selected by default;
 *
 * \note    One of observer's callbacks should be called to notify vDM engine 
 *          upon user response, cancellation, or timeout.
 *
 * \return  E_VDM_MMI_Result_OK on success or an MMI Error result.
 *******************************************************************************
 */
E_VDM_MMI_Result_t VDM_MMI_PL_confirmationQuery(
    VDM_MMI_ScreenContext_t*    inScreenContext, 
    E_VDM_MMI_ConfirmCmd_t      inDefaultCommand)
{
    E_VDM_MMI_Result_t      result = E_VDM_MMI_Result_OK;
    MMIDM_PL_ALERT_TYPE  alert_type = DM_PL_ALERT_NONE;

    //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_infoMessage inDefaultCommand = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_948_112_2_18_2_14_14_554,(uint8*)"d",inDefaultCommand);
    alert_type = DM_PL_ALERT_1101;
    MMINV_WRITE(MMINV_DM_ALERT_TYPE,&alert_type);
    //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_confirmationQuery alert_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_951_112_2_18_2_14_14_555,(uint8*)"d", alert_type);
    DM_OpenDMAlerthandleWin(MMIDM_ALERT_PL_CONFIRM, inScreenContext->displayText, (uint16)strlen((char*)inScreenContext->displayText),inScreenContext->maxDisplayTime);

    return result;
}


/*!
 *******************************************************************************
 * Prompt user to enter text.
 * 
 * \param   inScreenContext     Context of the screen to be displayed.
 * \param   inInputContext      Context of the input field. 
 *
 * \note    One of observer's callbacks should be called to notify vDM engine 
 *          upon user response, cancellation, or timeout.
 *
 * \return  E_VDM_MMI_Result_OK on success or an MMI Error result.
 *******************************************************************************
 */
E_VDM_MMI_Result_t VDM_MMI_PL_inputQuery(
    VDM_MMI_ScreenContext_t*    inScreenContext,
    VDM_MMI_InputContext_t*     inInputContext)
{
    E_VDM_MMI_Result_t      result = E_VDM_MMI_Result_OK;
	
    //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_inputQuery maxLength = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_976_112_2_18_2_14_14_556,(uint8*)"d",inInputContext->maxLength);

    return result;
}


/*!
 *******************************************************************************
 * Prompt user to select one or more item from a list.
 * 
 * \param   inScreenContext     Context of the screen to be displayed.
 * \param   inListContext       Context of the list, including whether a single
 *                              selection or a multiple selection is allowed.
 *
 * \note    One of observer's callbacks should be called to notify vDM engine 
 *          upon user response, cancellation, or timeout.
 *
 * \return  E_VDM_MMI_Result_OK on success or an MMI Error result.
 *******************************************************************************
 */
E_VDM_MMI_Result_t VDM_MMI_PL_choiceListQuery(
    VDM_MMI_ScreenContext_t*    inScreenContext, 
    VDM_MMI_ListContext_t*      inListContext)
{
    E_VDM_MMI_Result_t  result = E_VDM_MMI_Result_OK;

    
    //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_choiceListQuery isMultipleSelection = %d, itemsCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1002_112_2_18_2_14_14_557,(uint8*)"dd",inListContext->isMultipleSelection,inListContext->itemsCount);
	
    if (inListContext->itemsCount >= DM_ALERT_CHOICE_MAX_NUM)
	{
		result = E_VDM_MMI_Result_TooManyOptions;
    }
    else
    {
        if (MMIDM_SetChoiceListInfo(inListContext))
        {
            if (inListContext->isMultipleSelection)
            {	
                DM_OpenDMAlerthandleWin(MMIDM_ALERT_MULTI_CHOICE,inScreenContext->displayText, (uint16)strlen((char*)inScreenContext->displayText),inScreenContext->maxDisplayTime);
            }
            else
            {	
                DM_OpenDMAlerthandleWin(MMIDM_ALERT_SINGLE_CHOICE,inScreenContext->displayText, (uint16)strlen((char*)inScreenContext->displayText),inScreenContext->maxDisplayTime);
            }
        }
        else
        {
            result = E_VDM_MMI_Result_Error;
        }
    }
    return result;
}

/*****************************************************************************/
// 	Description : to get choice list info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC VDM_MMI_ListContext_t* MMIDM_GetChoiceListInfo(void)
{
    return &s_list_context;
}

/*****************************************************************************/
// 	Description : to set choice list info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_SetChoiceListInfo(VDM_MMI_ListContext_t *list_constext_p)
{
    BOOLEAN result = FALSE;
    int  len= strlen((char*)list_constext_p->items);
    s_list_context.items = SCI_ALLOCA(strlen((char*)list_constext_p->items)+1);
    if (s_list_context.items)
    {
        SCI_MEMCPY(s_list_context.items,list_constext_p->items,len);
        s_list_context.defaultSelection = list_constext_p->defaultSelection;
        s_list_context.isMultipleSelection = list_constext_p->isMultipleSelection;
        s_list_context.itemsCount = list_constext_p->itemsCount;
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
// 	Description : to clean choice list info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CleanChoiceListInfo(void)
{
    if (s_list_context.items)
    {
        SCI_FREE(s_list_context.items);
        s_list_context.items = PNULL;
    }
    SCI_MEMSET(&s_list_context,0,sizeof(s_list_context));
}
/*!
 *******************************************************************************
 * Display progress information to the user.
 * 
 * \param   inCurrentProgress   Current progress status.
 * \param   inMaxProgress       Maximum progress value.
 *******************************************************************************
 */
PUBLIC void VDM_MMI_PL_updateProgress(
    IU32    inCurrentProgress, 
    IU32    inMaxProgress)
{
    MMIDM_DOWNLOAD_PROCESS_T* download_process_ptr=PNULL;
    download_process_ptr = SCI_ALLOCA(sizeof(MMIDM_DOWNLOAD_PROCESS_T));
    if (PNULL!=download_process_ptr)
    {
        SCI_MEMSET(download_process_ptr,0x00,sizeof(MMIDM_DOWNLOAD_PROCESS_T));

        download_process_ptr->inCurrentProgress = inCurrentProgress;
        download_process_ptr->inMaxProgress = inMaxProgress;
    
        //SCI_TRACE_LOW:"MMIDM==> VDM_MMI_PL_updateProgress inCurrentProgress = %d, inMaxProgress=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1090_112_2_18_2_14_14_558,(uint8*)"dd",inCurrentProgress,inMaxProgress);

        if (DM_CANCEL != MMIDM_GetDlState())
        {
            MMIDM_SetDlState(DM_RUN);
            SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_DWNLD_PROGRESS,download_process_ptr);
        }
    }
}

/*****************************************************************************/
//	Description : dm session state change function
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void DM_SessionStateChange(
                                E_VDM_SessionType_t     inType,      
                                E_VDM_SessionState_t    inState,
                                RDM_Error               inLastError, 
                                UTF8CStr                inDataStr,
                                VDM_SessionContext_t**  inContext
                                )
{
    
    MMIDM_SESSION_CHANGE_T* session_change_ptr=PNULL;
    session_change_ptr = SCI_ALLOC_APP(sizeof(MMIDM_SESSION_CHANGE_T));
    if (PNULL !=session_change_ptr)
    {
        SCI_MEMSET(session_change_ptr,0x00,sizeof(MMIDM_SESSION_CHANGE_T));

        session_change_ptr->inType = inType;/*lint !e64*/
        session_change_ptr->inState = inState;/*lint !e64*/
        session_change_ptr->error = inDataStr;/*lint !e64*/
    }

    //SCI_TRACE_LOW:"MMIDM==> DM_SessionStateChange inType=%d,inState=%d,inLastError=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1124_112_2_18_2_14_14_559,(uint8*)"ddd",inType,inState,inLastError);
    if (VDM_SessionType_DL == inType)
    {
        if(VDM_SessionState_Started ==inState)
        {
            MMIDM_SetDlState(DM_RUN);
        }
        else if (VDM_SessionState_Aborted == inState)
        {
            if (RDM_ERR_COMMS_FATAL ==inLastError||RDM_ERR_COMMS_NON_FATAL == inLastError)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_NETWORK_DISCONNECT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,1,PNULL);/*lint !e64*/
            }
        }
    }
    if (VDM_SessionType_DM == inType)
    {
        if (VDM_SessionState_Complete ==inState)
        {
            if (DM_SESSION_CLIENT == s_task_session_type &&DM_NONE == MMIDM_GetDlState())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_UN_SEARCH_PACKAGE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,1,PNULL);/*lint !e64*/
            }
        }
    }
    if (PNULL !=session_change_ptr)
    {
        SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_SESSION_CHANGE,session_change_ptr);
    }
}
/*****************************************************************************/
//  Description : free all mo instance
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_FreeAllMOInstance(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_FreeAllMOInstance"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1160_112_2_18_2_14_14_560,(uint8*)"");

    FUMO_Plugin_term();
    SCOMO_Plugin_term();
    LAWMO_Plugin_term();
    VDM_MMI_PL_term();
    
    SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_FreeAllMOInstance exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1168_112_2_18_2_14_14_561,(uint8*)"");
}

/*****************************************************************************/
//  Description : start dm 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC DM_START_RESULT_E MMIDM_StartVDM(DM_SESSION_TYPE type,char* msg_body, uint32 msg_size)
{
#ifdef DM_SUPPORT_REDBENCH   
    uint32                  handle = 0;
    VDM_Error               result = FALSE;
    //uint16                  ret_value = 0;	
	    MMIDM_PL_ALERT_TYPE  alert_type = DM_PL_ALERT_NONE;
	    //SCI_TRACE_LOW:"MMIDM==> MMIDM_StartVDM start"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1182_112_2_18_2_14_14_562,(uint8*)"");
	    MMIDM_SetDmState(DM_START);
	    MMIDM_SetDlState(DM_NONE);
	    
	    s_task_session_type = type;
	
	   if(type != DM_SESSION_FUMO_RESUME && type != DM_SESSION_SCOMO_RESUME && type != DM_SESSION_CLIENT)
	   {
		    
	   	alert_type = DM_PL_ALERT_NONE;
	       MMINV_WRITE(MMINV_DM_ALERT_TYPE,&alert_type);
	       //SCI_TRACE_LOW:"MMIDM==> MMIDM_StartVDM alert_type %d"
	       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1193_112_2_18_2_14_14_563,(uint8*)"d", alert_type);
	   
	    }
	
	    // Step 1: Application initialization.
	    // In multi threaded environments, open a thread for the vDM engine.
	    handle = MMIDM_CreateThread();
	
	    // Step 2: Create vDM instance.
	    // vDM should be created as early as possible.
	    // Note that the already existing thread (if created) is passed
	    // as a parameter to vDM.
	    result = VDM_create(handle);/*lint !e64*/
	    if( result != VDM_ERR_OK)
	    {
	        //SCI_TRACE_LOW:"MMIDM==> Failed to create vDM: error 0x%x"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1208_112_2_18_2_14_14_564,(uint8*)"d", result);
	        return DM_START_DONE;
	    }
	    if (MMIDM_GetLogState())  //for test debug
	    {
	       MMIDM_UTL_Logger_setDefaultLevel(E_VDM_LOGLEVEL_Debug);
	    }
	
	    MMIDM_Config_setMaxObjSize(DM_CLIENT_PL_MAX_OBJ_SIZE);
	    MMIDM_Config_setMaxMsgSize(DM_CLIENT_PL_MAX_MSG_SIZE);
	    if (MMIDM_GetLogState())
	    {
	        MMIDM_Config_setEncodeWBXMLMsg(FALSE);
	    }
	    MMIDM_Config_setCheckDDVersion(FALSE);
	    MMIDM_Config_setMaxNetRetries(DM_CLIENT_PL_MAX_RETRIE_TIMES);
	
	    if(MMIDM_GetIdasdecType())//8801 需要 /ophone不需要
	    {
	        MMIDM_ConfigSetSessionIDAsDec(TRUE); 
	    }
	    MMIDM_ConfigSetNotificationVerificationMode(E_VDM_NotificationVerificationMode_Disabled);
	    //Register DL resume mechanism
	    MMIDM_RegisterResume(DM_DlResumeGetFunc, DM_DLResumeSetFunc);
	    MMIDM_RegisterSessionStateObserver(DM_SessionStateChange);
	    // Step 3: Start vDM
	    // Once configured, start the vDM engine.
	
	    result = VDM_start();
	    if (result != VDM_ERR_OK)
	    {
	        //SCI_TRACE_LOW:"MMIDM==> Failed to start vDM: error 0x%x"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1239_112_2_18_2_14_15_565,(uint8*)"d", result);
	        return DM_START_DONE;
	    }
	
	    // Step 4: Register vDM callback functions.
	    result = DM_registerCallbacks();
	    if (result != VDM_ERR_OK)
	    {
	        //SCI_TRACE_LOW:"MMIDM==> Failed to register callbacks: error 0x%x"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1247_112_2_18_2_14_15_566,(uint8*)"d", result);
	        return DM_START_DONE;
	    }
	
	    // Step 5:  Initialize vDM plug-ins.
	    result = VDMC_PluginMng_init();
	    if (result != VDM_ERR_OK)
	    {
	        //SCI_TRACE_LOW:"MMIDM==> Failed to initialize plug-ins: error 0x%x"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1255_112_2_18_2_14_15_567,(uint8*)"d", result);
	        return DM_START_DONE;
	    }
	
	    // Step 6:
		// This is a fictitious step that initiates a session with the DM server.
		// Normally this would happen following a notification from a UDP listener,
		// a WAP listener, a user action, etc.
		//
	
		result = DM_initiateSession(type,msg_body,msg_size);
		if (result != VDM_ERR_OK)
		{
			//SCI_TRACE_LOW:"MMIDM==> MMIDM_StarVDM Failed to trigger session: error 0x%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1268_112_2_18_2_14_15_568,(uint8*)"d", result);
			if (DM_SESSION_REMOVE == type || DM_SESSION_ADD ==type)
	        {
	            return DM_START_DONE;
	        }
	        else
	        {
			    return DM_START_FAIL;
	        }
		}
	    if (DM_SESSION_REMOVE == type)
	    {
	        return DM_START_DONE;
	    }
	        
		//
	// Step 7: Start vDM event loop (single threaded environments only)
	// In single threaded environments, this is the place to start the
	// vDM event loop. In multi threaded environments, the event loop is run
	// from another thread (dmc_vdmThreadFunc)
	// This sample application executes a single session and then exits.
	//


    result = VDM_run();

	if (result != VDM_ERR_OK)
	{
		//SCI_TRACE_LOW:"MMIDM==> Failed to run vDM: error 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1296_112_2_18_2_14_15_569,(uint8*)"d", result);
		return DM_START_FAIL;
	}
    MMIDM_SetDmState(DM_RUN);
 	//SCI_TRACE_LOW:"MMIDM==> MMIDM_StarVDM end"
 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1300_112_2_18_2_14_15_570,(uint8*)"");
#endif
	return DM_START_SUCC;
}

/*****************************************************************************/
//	Description : create dm thread
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL uint32 MMIDM_CreateThread(void)
{
	//SCI_TRACE_LOW:"MMIDM==> MMIDM_CreateThread"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1311_112_2_18_2_14_15_571,(uint8*)"");
	return MMIDM_GetDmTaskID();
}
/*****************************************************************************/
//	Description : register Call back function
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_registerCallbacks(void)
{
	VDM_Error	 result = VDM_ERR_OK;
	uint32		 i = 0;

	//SCI_TRACE_LOW:"MMIDM==> DM_registerCallbacks"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1323_112_2_18_2_14_15_572,(uint8*)"");
	
	for (i=0;i<ARR_SIZE(s_dm_initdata_tab);i++)
	{
		MMIDM_Tree_registerExternalStorage(s_dm_initdata_tab[i].inPath,
										 s_dm_initdata_tab[i].inReadFunc,
										 s_dm_initdata_tab[i].inWriteFunc,
										 s_dm_initdata_tab[i].inContext);
	}

	return result;
}


#ifdef DM_SUPPORT_DFRESH
PUBLIC VDM_Tree_ReadExternalCB DM_GET_TREE_READFUNC(char* path)
{
    uint32		 i = 0;
	for (i=0;i<ARR_SIZE(s_dm_initdata_tab);i++)
	{
	  if(!strcmp((char*)s_dm_initdata_tab[i].inPath,path))
	    return s_dm_initdata_tab[i].inReadFunc;

	}
    return (VDM_Tree_ReadExternalCB)PNULL;
}

PUBLIC VDM_Tree_WriteExternalCB DM_GET_TREE_WRITEFUNC(char* path)
{
    uint32		 i = 0;
    
	SCI_TRACE_LOW("MMIDM ==> DM_GET_TREE_WRITEFUNC : %s", path);
    
	for (i=0;i<ARR_SIZE(s_dm_initdata_tab);i++)
	{
	  if(!strcmp((char*)s_dm_initdata_tab[i].inPath,path))
	    return s_dm_initdata_tab[i].inWriteFunc;

	}
    return (VDM_Tree_WriteExternalCB)PNULL;
}
#endif
/*****************************************************************************/
//	Description : VDMC_PluginMng_init
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error VDMC_PluginMng_init(void)
{
	VDM_Error result = VDM_ERR_OK;
	//SCI_TRACE_LOW:"MMIDM==> VDMC_PluginMng_init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1369_112_2_18_2_14_15_573,(uint8*)"");

	result = FUMO_Plugin_init((void*)"./FwUpdate");
	if(result != VDM_ERR_OK)
	{
		//SCI_TRACE_LOW:"MMIDM==> Failed to initialize FUMO plugin: error 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1374_112_2_18_2_14_15_574,(uint8*)"d", result);
		return result;
	}

	result = SCOMO_Plugin_init((void*)"./SCOMO");
	if(result != VDM_ERR_OK)
	{
		//SCI_TRACE_LOW:"MMIDM==> Failed to initialize SCOMO plugin: error 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1381_112_2_18_2_14_15_575,(uint8*)"d", result);
		return result;
	}

	result = LAWMO_Plugin_init((void*)"./LAWMO");
	if(result != VDM_ERR_OK)
	{
		//SCI_TRACE_LOW:"MMIDM==> Failed to initialize LAWMO plugin: error 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1388_112_2_18_2_14_15_576,(uint8*)"d", result);
		return result;
	}

	return result;
}
/*****************************************************************************/
//	Description : FUMO_Plugin_init
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error FUMO_Plugin_init(void* inParam)
{
	VDM_Error result = VDM_ERR_OK;
	UTF8CStr inDMTreeURI = (UTF8CStr)inParam;
	VDM_FUMO_Callbacks_t callbacks ={0};
	//SCI_TRACE_LOW:"MMIDM==> FUMO_Plugin_init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1403_112_2_18_2_14_15_577,(uint8*)"");
	callbacks.confirmDownloadCB = NULL;
	callbacks.confirmUpdateCB = DM_FumoConfirmUpdate;
	callbacks.executeUpdateCB = DM_FumoExecuteUpdate;

    if (PNULL== s_fumo_instance)
    {
        s_fumo_instance = MMIDM_FUMO_createInstance(
		                    inDMTreeURI,
		                    &callbacks,
		                    NULL);
    }
	return result;
}
/*****************************************************************************/
//	Description : FUMO_Plugin_term
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void FUMO_Plugin_term(void)
{
    //SCI_TRACE_LOW:"MMIDM==> FUMO_Plugin_term"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1423_112_2_18_2_14_15_578,(uint8*)"");
    if (PNULL != s_fumo_instance)
    {

        MMIDM_FUMO_destroyInstance(&s_fumo_instance);

        s_fumo_instance = PNULL;
    }
}
/*****************************************************************************/
//	Description : fumo confirm update  
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL IBOOL DM_FumoConfirmUpdate(
	  VDM_Handle_t	inFumoInstance,
	  void* 		inContext)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_FumoConfirmUpdate"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1440_112_2_18_2_14_15_579,(uint8*)"");
	//UI提示  
	DM_OpenDMAlerthandleWin(MMIDM_ALERT_FUMO_CONFIRM,PNULL,0,0);

	return FALSE;
}

/*****************************************************************************/
//	Description : fumo execute update 
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL IBOOL DM_FumoExecuteUpdate(
	UTF8CStr		inUpdatePkgPath,
	VDM_Handle_t	inFumoInstance,
	void*			inDmcContext,
	IU32*			outFumoResult)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_FumoExecuteUpdate"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1457_112_2_18_2_14_15_580,(uint8*)"");
	return FALSE;
}
/*****************************************************************************/
//	Description : SCOMO_Plugin_init
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error SCOMO_Plugin_init(void* inParam )
{
	VDM_Error result = VDM_ERR_OK;
	VDM_SCOMO_DP_Callbacks_t dpCallbacks = {0};
	VDM_SCOMO_DC_Callbacks_t dcCallbacks = {0};
	UTF8CStr inDMTreeURI = (UTF8CStr)inParam;
    SCOMO_Plugin_t* instance = SCOMO_Plugin_create();

	//SCI_TRACE_LOW:"MMIDM==> SCOMO_Plugin_init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1472_112_2_18_2_14_15_581,(uint8*)"");
    
	dpCallbacks.confirmInstallCB = DM_OnScomoConfirmInstallation;
	dpCallbacks.executeInstallCB = DM_OnScomoReadyToInstall;

	dcCallbacks.confirmRemoveCB = DM_OnScomoConfirmUninstallation;
	dcCallbacks.executeRemoveCB = DM_OnScomoReadyToRemove;
	
	result = MMIDM_SCOMO_create(inDMTreeURI);
	if ( result != VDM_ERR_OK )
	{
		result = VDM_ERR_UNSPECIFIC;
	}
	else
	{
		instance->scomoDP = MMIDM_SCOMO_DP_createInstance(
		(UTF8Str)DP_ID,
		&dpCallbacks,
        NULL);
        
        if (instance->scomoDP == NULL)
        {
            result = VDM_ERR_UNSPECIFIC;
            return result;
        }
        
	    if(SCOMO_Plugin_initDCs(instance))
        {
            SCOMO_DC_t* list_ptr = instance->dcList;
            while (list_ptr)
	        {
		        list_ptr->handle = MMIDM_SCOMO_DC_createInstance(
			        list_ptr->id,
			        &dcCallbacks,
			        instance);
		        if (list_ptr->handle == NULL)
		        {
			        result = VDM_ERR_UNSPECIFIC;
                    return result;
		        }
		        list_ptr = list_ptr->next;
	        }
        }
	}
	return result;
}

/*****************************************************************************/
//	Description : handle confirm install action
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoConfirmInstallation(VDM_Handle_t   inScomoDPInstance,
										void*			inDmcContext,
										UTF8CStr		inOperationPath
										)
{   
    SCI_TRACE_LOW("MMIDM==> DM_OnScomoConfirmInstallation inOperationPath %s",inOperationPath);

    MMIDM_RemoveResumeFile(RESUME_TYPE_SCOMO);
    MMIDM_ScomoInstall(MMIDM_IsBackgroupRun());    
#if 0 //下载完直接安装即可
    if(MMIDM_IsBackgroupRun())
    {
        //backgroud run
        MMIDM_RemoveResumeFile(RESUME_TYPE_SCOMO);
        MMIDM_ScomoInstall(TRUE);        
    }
    else
    {
        //提示用户 
        DM_OpenDMAlerthandleWin(MMIDM_ALERT_SCOMO_CONFIRM,PNULL,0,0);
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//	Description : get dm model
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceModel(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetDeviceModel "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1577_112_2_18_2_14_15_585,(uint8*)"");
	string = MMIDM_GetModel();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : get dm man
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceMan(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> VDMC_GetVdmMan "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1592_112_2_18_2_14_15_586,(uint8*)"");
	string = MMIDM_GetMan();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : get dm OEM
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceOEM(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> VDMC_GetVdmOEM "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1606_112_2_18_2_14_15_587,(uint8*)"");
	string = MMIDM_GetOEM();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : get dm Language
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceLanguage(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> VDMC_GetVdmLanguage "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1620_112_2_18_2_14_15_588,(uint8*)"");
	string = MMIDM_GetLanguage();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/

}

/*****************************************************************************/
//	Description : get dm version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceVersion(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> VDMC_GetVdmMan "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1636_112_2_18_2_14_15_589,(uint8*)"");
	string = MMIDM_GetDMVersion();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : get dm DevImei
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceImei(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> VDMC_GetDevImei "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1651_112_2_18_2_14_15_590,(uint8*)"");
	string = DM_GetDevImeiInfo();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : get dm firmware version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceFwV(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetDeviceFwV "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1665_112_2_18_2_14_15_591,(uint8*)"");
	string = MMIDM_GetFirmwareVersion();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}


/*****************************************************************************/
//	Description : get dm software version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceSwV(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetDeviceSwV "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1681_112_2_18_2_14_15_592,(uint8*)"");
	string = MMIDM_GetSoftwareVersion();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : get dm Hardware version
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDeviceHwV(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetDeviceHwV "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1696_112_2_18_2_14_15_593,(uint8*)"");
	string = MMIDM_GetHardwareVersion();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : get empty info
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetEmptyInfo(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char string[] ="";

	//SCI_TRACE_LOW:"MMIDM==> DM_GetStringNULL "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1711_112_2_18_2_14_15_594,(uint8*)"");

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : get dm profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetDmProfile(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetDmProfile "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1725_112_2_18_2_14_15_595,(uint8*)"");
	string = MMIDM_GetDmProfile();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : get dm profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetDmProfile(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetDmProfile "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1738_112_2_18_2_14_16_596,(uint8*)"");
    if(MMIDM_SetDmProfile((char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1745_112_2_18_2_14_16_597,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmnet apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetAPN(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMNetAPN "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1759_112_2_18_2_14_16_598,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_APN,DM_CMNET);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}


/*****************************************************************************/
//	Description : set dm gprs cmnet apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetAPN(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMNetAPN "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1773_112_2_18_2_14_16_599,(uint8*)"");
    if(MMIDM_SetGprsInfo(DM_WAP_APN,(char*)inData,inDataLen,DM_CMNET))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1780_112_2_18_2_14_16_600,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmnet user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetUsername(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMNetUsername "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1794_112_2_18_2_14_16_601,(uint8*)"");

    string = MMIDM_GetGPRSInfo(DM_WAP_USER_NAME,DM_CMNET);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : set dm gprs cmnet user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetUsername(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMNetUsername "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1807_112_2_18_2_14_16_602,(uint8*)"");
    if(MMIDM_SetGprsInfo(DM_WAP_USER_NAME,(char*)inData,inDataLen,DM_CMNET))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1814_112_2_18_2_14_16_603,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmnet pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNePw(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMNePw "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1828_112_2_18_2_14_16_604,(uint8*)"");

	string = MMIDM_GetGPRSInfo(DM_WAP_PW,DM_CMNET);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm gprs cmnet pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetPw(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMNetPw "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1842_112_2_18_2_14_16_605,(uint8*)"");
    if(MMIDM_SetGprsInfo(DM_WAP_PW,(char*)inData,inDataLen,DM_CMNET))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1849_112_2_18_2_14_16_606,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmnet prixt addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetProxyaddr(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMNetProxyaddr "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1863_112_2_18_2_14_16_607,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_PROXY_ADDR,DM_CMNET);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : set dm gprs cmnet prixt addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetProxyaddr(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMNetProxyaddr "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1875_112_2_18_2_14_16_608,(uint8*)"");
    if(MMIDM_SetGprsInfo(DM_WAP_PROXY_ADDR,(char*)inData,inDataLen,DM_CMNET))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1882_112_2_18_2_14_16_609,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmnet port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMNetPortNum(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMNetPortNum "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1896_112_2_18_2_14_16_610,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_PORT_NUM,DM_CMNET);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : set dm gprs cmnet port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMNetPortNum(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMNetUsername "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1908_112_2_18_2_14_16_611,(uint8*)"");
    if(MMIDM_SetGprsInfo(DM_WAP_PORT_NUM,(char*)inData,inDataLen,DM_CMNET))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1915_112_2_18_2_14_16_612,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsAPN(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsAPN "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1929_112_2_18_2_14_16_613,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_APN,DM_CMWAP);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//	Description : set dm gprs apn
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsAPN(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsAPN "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1941_112_2_18_2_14_16_614,(uint8*)"");

    if(MMIDM_SetGprsInfo(DM_WAP_APN,(char*)inData,inDataLen,DM_CMWAP))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1949_112_2_18_2_14_16_615,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//	Description : get dm gprs user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsUsername(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsUsername "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1962_112_2_18_2_14_16_616,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_USER_NAME,DM_CMWAP);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm gprs user name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsUsername(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsUsername "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1975_112_2_18_2_14_16_617,(uint8*)"");

    if(MMIDM_SetGprsInfo(DM_WAP_USER_NAME,(char*)inData,inDataLen,DM_CMWAP))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1983_112_2_18_2_14_16_618,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsPw(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsPw "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_1997_112_2_18_2_14_16_619,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_PW,DM_CMWAP);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm gprs pw
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsPw(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsPw "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2010_112_2_18_2_14_16_620,(uint8*)"");

    if(MMIDM_SetGprsInfo(DM_WAP_PW,(char*)inData,inDataLen,DM_CMWAP))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2018_112_2_18_2_14_16_621,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmwap proxyaddr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMWapProxyaddr(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMWapProxyaddr "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2032_112_2_18_2_14_16_622,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_PROXY_ADDR,DM_CMWAP);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm gprs cmwap proxyaddr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMWapProxyaddr(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMWapProxyaddr "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2045_112_2_18_2_14_16_623,(uint8*)"");

    if(MMIDM_SetGprsInfo(DM_WAP_PROXY_ADDR,(char*)inData,inDataLen,DM_CMWAP))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2053_112_2_18_2_14_16_624,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm gprs cmwap port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetGprsCMWapPortNum(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetGprsCMWapPortNum "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2067_112_2_18_2_14_16_625,(uint8*)"");
	string = MMIDM_GetGPRSInfo(DM_WAP_PORT_NUM,DM_CMWAP);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm gprs cmwap port num
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetGprsCMWapPortNum(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetGprsCMWapPortNum "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2080_112_2_18_2_14_16_626,(uint8*)"");

    if(MMIDM_SetGprsInfo(DM_WAP_PORT_NUM,(char*)inData,inDataLen,DM_CMWAP))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2088_112_2_18_2_14_16_627,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm mms mmsc
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetMMSMmsc(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetMMSMmsc "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2102_112_2_18_2_14_16_628,(uint8*)"");

	string = MMIDM_GetMMSCInfo(DM_MMS_MMSC);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm mms mmsc
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetMMSMmsc(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize) 
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetMMSMmsc "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2116_112_2_18_2_14_16_629,(uint8*)"");

    if(MMIDM_SetMMSCInfo(DM_MMS_MMSC,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2124_112_2_18_2_14_16_630,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//	Description : get dm mms profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetMMSProfile(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetMMSProfile "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2137_112_2_18_2_14_16_631,(uint8*)"");
	string = MMIDM_GetMMSCInfo(DM_MMS_CONN_PROFILE);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm mms profile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetMMSProfile(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetMMSProfile "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2150_112_2_18_2_14_16_632,(uint8*)"");

    if(MMIDM_SetMMSCInfo(DM_MMS_CONN_PROFILE,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2158_112_2_18_2_14_16_633,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//	Description : get dm wap conprofile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetWAPConprofile(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetWAPConprofile "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2172_112_2_18_2_14_16_634,(uint8*)"");
	string = DM_GetWAPInfo(DM_WAP_APN);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm wap conprofile
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetWAPConprofile(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetWAPConprofile "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2185_112_2_18_2_14_16_635,(uint8*)"");

    if(DM_SetWAPInfo(DM_WAP_APN,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2193_112_2_18_2_14_16_636,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//	Description : get dm wap start page
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetWAPStarPage(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;

	//SCI_TRACE_LOW:"MMIDM==> DM_GetWAPStarPage "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2206_112_2_18_2_14_16_637,(uint8*)"");
	string = DM_GetWAPInfo(DM_WAP_HOME_PAGE);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//	Description : set dm wap start page
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetWAPStarPage(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetWAPStarPage "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2219_112_2_18_2_14_17_638,(uint8*)"");

    if(DM_SetWAPInfo(DM_WAP_HOME_PAGE,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2227_112_2_18_2_14_17_639,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//  Description : get dm pim addr
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetPIMAddr(void* context, IU32 offset, void* buffer,
                            IU32  blength, IU32* vlength)
{
#ifdef PIM_SUPPORT
    char  string[MMIPIM_MAX_URL_LEN + 1] = {0};
    
    SCI_TRACE_LOW("MMIDM==> DM_GetPIMAddr ");
    MMIDM_GetPIMInfo(DM_PIM_ADDR, string, MMIPIM_MAX_URL_LEN);

    return DM_GetVdmInfoString(string,offset,buffer,blength,vlength);/*lint !e64*/
#else
    return VDM_ERR_OK;
#endif

}

/*****************************************************************************/
//	Description : set dm pim addr
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetPIMAddr(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetPIMAddr "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2254_112_2_18_2_14_17_641,(uint8*)"");

    if(MMIDM_SetPIMInfo(DM_PIM_ADDR,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2262_112_2_18_2_14_17_642,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//	Description : get dm pim addressbook url
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetPIMAddressBookURL(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
#ifdef PIM_SUPPORT
    char string[MMIPIM_MAX_DATABASE_LEN + 1] = {0};

    SCI_TRACE_LOW("MMIDM==> DM_GetPIMAddressBookURL ");
    MMIDM_GetPIMInfo(DM_PIM_ADDRESSBOOKURL, string, MMIPIM_MAX_DATABASE_LEN);
    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
#else
    return VDM_ERR_OK;
#endif

}

/*****************************************************************************/
//	Description : set dm pim addressbook url
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetPIMAddressBookURL(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetPIMAddressBookURL "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2288_112_2_18_2_14_17_644,(uint8*)"");

    if(MMIDM_SetPIMInfo(DM_PIM_ADDRESSBOOKURL,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2296_112_2_18_2_14_17_645,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//	Description : get dm pim calendarurl
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetPIMCalendarURL(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
#ifdef PIM_SUPPORT
    char string[MMIPIM_MAX_DATABASE_LEN + 1] = {0};

    SCI_TRACE_LOW("MMIDM==> DM_GetPIMCalendarURL ");
    MMIDM_GetPIMInfo(DM_PIM_CALENDARURL, string, MMIPIM_MAX_DATABASE_LEN);
    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
#else
    return VDM_ERR_OK;
#endif
}

/*****************************************************************************/
//	Description : get dm srvaddr url
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetSrvAddrURL(void* context, IU32 offset, void* buffer,
							IU32  bLength, IU32* vLength)
{
	char* string = PNULL;
    
	//SCI_TRACE_LOW:"MMIDM==> DM_GetSrvAddrURL "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2324_112_2_18_2_14_17_647,(uint8*)"");
	string = MMIDM_GetSrvAddURL();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

#ifdef DM_SUPPORT_DFRESH
LOCAL VDM_Error DM_GetDeviceLockStatus(void* context, IU32 offset, void* buffer,IU32  bLength, IU32* vLength)
{
	char* string = PNULL;
    
	//SCI_TRACE_LOW:"MMIDM==> DM_GetDeviceLockStatus"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2335_112_2_18_2_14_17_648,(uint8*)"");
	string = MMIDM_GetLockStatusCode();

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
#endif
/*****************************************************************************/
//	Description : set dm pim calendarurl
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetPIMCalendarURL(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
	//SCI_TRACE_LOW:"MMIDM==> DM_SetPIMCalendarURL "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2348_112_2_18_2_14_17_649,(uint8*)"");

    if(MMIDM_SetPIMInfo(DM_PIM_CALENDARURL,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2356_112_2_18_2_14_17_650,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : get dm streaming name
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingName(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingName "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2370_112_2_18_2_14_17_651,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_NAME);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}

/*****************************************************************************/
//  Description : set dm streaming name
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingName(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingName "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2383_112_2_18_2_14_17_652,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_NAME,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2391_112_2_18_2_14_17_653,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//  Description : get dm streaming Proxy
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingProxy(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingProxy "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2405_112_2_18_2_14_17_654,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_PROXY);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//  Description : set dm streaming Proxy
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingProxy(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingProxy "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2417_112_2_18_2_14_17_655,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_PROXY,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2425_112_2_18_2_14_17_656,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}

/*****************************************************************************/
//  Description : get dm streaming proxy port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingProxyPort(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingProxyPort "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2439_112_2_18_2_14_17_657,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_PROXYPORT);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//  Description : set dm streaming proxy port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingProxyPort(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingProxyPort "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2451_112_2_18_2_14_17_658,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_PROXYPORT,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2459_112_2_18_2_14_17_659,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//  Description : get dm streaming nap id
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingNapID(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingNapID "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2472_112_2_18_2_14_17_660,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_APN);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//  Description : set dm streaming nap id
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingNapID(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingNapID "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2484_112_2_18_2_14_17_661,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_APN,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2492_112_2_18_2_14_17_662,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//  Description : get dm streaming net info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingNetInfo(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingNetInfo "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2505_112_2_18_2_14_17_663,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_NETINFO);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//  Description : set dm streaming Net Info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingNetInfo(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingNetInfo "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2517_112_2_18_2_14_17_664,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_NETINFO,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2525_112_2_18_2_14_17_665,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//  Description : get dm streaming Min UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingMinUDPPort(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingMinUDPPort "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2538_112_2_18_2_14_17_666,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_MIN_UDP_PORT);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//  Description : set dm streaming Min UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingMinUDPPort(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingMinUDPPort "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2550_112_2_18_2_14_17_667,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_MIN_UDP_PORT,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2558_112_2_18_2_14_17_668,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
/*****************************************************************************/
//  Description : get dm streaming Max UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetStreamingMaxUDPPort(void* context, IU32 offset, void* buffer,
                            IU32  bLength, IU32* vLength)
{
    char* string = PNULL;
    
    //SCI_TRACE_LOW:"MMIDM==> DM_GetStreamingMaxUDPPort "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2571_112_2_18_2_14_17_669,(uint8*)"");
    string = MMIDM_GetStreamingInfo(DM_ST_MAX_UDP_PORT);

    return DM_GetVdmInfoString(string,offset,buffer,bLength,vLength);/*lint !e64*/
}
/*****************************************************************************/
//  Description : set dm streaming Max UDP Port
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL VDM_Error DM_SetStreamingMaxUDPPort(void *inContext, IU32 inOffset,const void* inData, 
                              IU32 inDataLen, IU32 inTotalSize)
{
    //SCI_TRACE_LOW:"MMIDM==> DM_SetStreamingName "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2583_112_2_18_2_14_17_670,(uint8*)"");

    if(MMIDM_SetStreamingInfo(DM_ST_MAX_UDP_PORT,(char*)inData,inDataLen))
    {
        return VDM_ERR_OK;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_ERR_BUFFER_OVERFLOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2591_112_2_18_2_14_17_671,(uint8*)"");
        return VDM_ERR_BUFFER_OVERFLOW;
    }
}
#endif
/*****************************************************************************/
//	Description : get vdm info string
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_GetVdmInfoString(
                           char * string,   //in
                           uint32 offset,   //in
                           void * buffer,   //in/out
                           uint32 bLength,  //in
                           uint32*vLength   //in/out
                           )
{
	uint32		cLength = 0;

	if (PNULL == string)
    {
        return VDM_ERR_BUFFER_OVERFLOW;
    }
    /* Store the total length of the string. */
	*vLength = strlen(string);

	/* Work out how many bytes to copy, fail if not enough space */
	cLength = ( offset >= *vLength ) ? 0 : (*vLength - offset);
	if ( cLength > bLength ) 
	{
		cLength = bLength;
		return VDM_ERR_BUFFER_OVERFLOW;
	}

	/* Copy the bytes. */
	if ( cLength > 0 )
	{
		SCI_MEMCPY(buffer, string + offset, cLength);
	}
    return VDM_ERR_OK;
}

/*****************************************************************************/
//	Description : init dm session
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL VDM_Error DM_initiateSession(DM_SESSION_TYPE type,char* msg_body, uint32 msg_size)
{
	VDM_Error	result = VDM_ERR_OK;
    E_VDM_FUMO_RESULT_t fumo_result = E_VDM_FUMO_RESULT_client_error; 

	//SCI_TRACE_LOW:"MMIDM==>  DM_initiateSession type =%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2642_112_2_18_2_14_17_672,(uint8*)"d",type);
	switch(type) 
	{
		case DM_SESSION_USER:
			 result = MMIDM_TriggerDMSession(NULL, NULL, NULL, 0, NULL);
			 break;
		case DM_SESSION_SERVER:
			 result = MMIDM_TriggerNIADMSession(msg_body,msg_size,NIA_Plugin_handleAlert,NULL);/*lint !e64*/
			 break;
		case DM_SESSION_CLIENT:
			 result = MMIDM_FUMO_triggerSession(s_fumo_instance,NULL,0,E_VDM_FUMO_Client_device);
			 break;
		case DM_SESSION_FUMO_RESUME:
			 result = MMIDM_FUMO_SessionController_triggerDL(s_fumo_instance,E_VDM_FUMO_Session_dlAndUpdate);
			 break;
        case DM_SESSION_SCOMO_RESUME:
			 result = MMIDM_SCOMO_DP_SessionHandler_DL_trigger(s_scomoInstance->scomoDP,E_VDM_SCOMO_DP_Session_dlAndInstall);
             break;
        case DM_SESSION_FOTA:
            if (1 == msg_size)//E_FOTA_SUCCESS
            {
                fumo_result = E_VDM_FUMO_RESULT_successful;
            }
            else //E_FOTA_UNCOMPATIBLE
            {
                fumo_result = E_VDM_FUMO_RESULT_client_error;
            }
            result = MMIDM_FUMO_setUpdateResult(s_fumo_instance,fumo_result);
            break;
        case DM_SESSION_REMOVE:
            result = DM_RemoveDCFromInventory(msg_size);
            break;
        case DM_SESSION_ADD:
            result = DM_SetDPInstallResult(SCOMO_Successful,msg_size,&s_dp_header);
            MMIDM_CleanInstallFile();
            break;
		default:
			break;
	}

	return result;
}
extern PUBLIC void MMIDM_SetUiMode(E_VDM_NIA_UIMode_t ui_mode);
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
	VDM_SessionContext_t*	inSessionContext)
{
	VDM_Error	result = VDM_ERR_OK;
	
	//SCI_TRACE_LOW:"MMIDM==>  NIA_Plugin_handleAlert inUIMode =%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2698_112_2_18_2_14_17_673,(uint8*)"d",inUIMode);

	//s_UIMode = inUIMode;
    MMIDM_SetUiMode(inUIMode);
	switch(inUIMode) 
	{
		case E_VDM_NIA_UIMode_NotSpecified:
		case E_VDM_NIA_UIMode_Background:
			MMIDM_NotifyNIASessionProceed(); 
			break;
		case E_VDM_NIA_UIMode_Informative:
			//UI提示
			DM_OpenDMAlerthandleWin(MMIDM_ALERT_NIA_DISPLAY,inVendorSpecificData,inVendorSpecificDataLength,0);
			break;
		case E_VDM_NIA_UIMode_UI:
			//给用户一个提示界面
			DM_OpenDMAlerthandleWin(MMIDM_ALERT_NIA_CONFIRM,inVendorSpecificData,inVendorSpecificDataLength,0);
			break;
		default:
			break;
	}
	return result;
}


/*!*******************************************************************************
 * Function used to get the stored DL info to resume the download if possible.
 *
 *
 * \param	offset		The byte offset which was downloaded so far.
 *
 * \param	eTag		The eTag of the object from the previous download.
 *						Memory allocated to this parameter should be done via
 *						RDM_malloc and the RDM library is responsible to free it.
 *
 * \return	\ref RDM_ERR_defs "An error code" (RDM_ERR_OK if no error)
********************************************************************************/
LOCAL VDM_Error DM_DlResumeGetFunc(IU32* offset, UTF8Str* eTag)
{
	uint32				read_size = 0;
	MMIFILE_HANDLE		file_handle = 0;
	wchar*				filename_ptr = PNULL;
	MMIFILE_ERROR_E 	error = SFS_NO_ERROR;
	wchar				full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	uint16				full_path_len = MMIFILE_FULL_PATH_MAX_LEN; 
	char				*pResumeTag = PNULL;
	VDM_Error			result = VDM_ERR_OK;
	uint32				len =0;

	pResumeTag = SCI_ALLOC_APP(VDM_ETAG_LEN+1);//this will be freed in VDM lib
    //SCI_TRACE_LOW:"MMIDM==> DM_DlResumeGetFunc %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2747_112_2_18_2_14_18_674,(uint8*)"s",s_resume_file);

    if (PNULL == pResumeTag)
	{
		//SCI_TRACE_LOW:"MMIDM==> DM_DlResumeGetFunc: Failed to allocate memory."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2751_112_2_18_2_14_18_675,(uint8*)"");
        SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
		return result;
	}
	SCI_MEMSET(pResumeTag,0x00,(VDM_ETAG_LEN + 1));
	*offset = 0;
	*eTag = pResumeTag;/*lint !e64*/
	filename_ptr = SCI_ALLOC_APP((strlen(s_resume_file)+1) * sizeof(wchar));
	len = (uint32)strlen(s_resume_file)+1;
	SCI_MEMSET(filename_ptr,0,(len * sizeof(wchar)));

	MMIAPICOM_StrToWstr((uint8*)s_resume_file,filename_ptr);
	if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
	{
        if (!MMIAPIFMM_IsFileExist(full_path_arr,full_path_len))
        {							
		    SCI_FREE(filename_ptr);
            SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
            return result;
        }
		file_handle = MMIAPIFMM_CreateFile(full_path_arr,SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);

		if(0 == file_handle)
		{
			//if (PNULL != filename_ptr)
			{							
				SCI_FREE(filename_ptr);
			}
			//SCI_TRACE_LOW:"MMIDM==>  DM_DlResumeGetFunc: Failed to open %s."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2779_112_2_18_2_14_18_676,(uint8*)"s",s_resume_file);
            SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
			return result;
		}
		error = MMIAPIFMM_ReadFile(
			file_handle,
			&s_vdm_resume,
			sizeof(s_vdm_resume),
			&read_size,
			NULL
			);
		//SCI_TRACE_LOW:"MMIDM==>  DM_DlResumeGetFunc error =%d,read_size=%d,sizeof(s_vdm_resume) size=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2790_112_2_18_2_14_18_677,(uint8*)"ddd",error,read_size,sizeof(s_vdm_resume));
//		if (SFS_ERROR_NONE != error)
//		{
//			SCI_TRACE_LOW("MMIDM==> DM_DlResumeGetFunc:  failed to read *offset=%d, eTag=%s",*offset,*eTag);
//			result = VDM_ERR_INTERNAL;
//		}
// 		else
		{
			if(strlen(s_vdm_resume.eTag)!=0)
			{
				strcpy((char*)pResumeTag, s_vdm_resume.eTag);
			}
			*offset = s_vdm_resume.offset;
			//SCI_TRACE_LOW:"MMIDM==>  DM_DlResumeGetFunc pResumeTag = %s, offset = %d."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2803_112_2_18_2_14_18_678,(uint8*)"sd", pResumeTag, *offset);
		}
		MMIAPIFMM_CloseFile(file_handle);
	}
    else
    {
        SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
    }
	//if (PNULL != filename_ptr)
	{
		SCI_FREE(filename_ptr);
	}
	return VDM_ERR_OK;
}

/*!*******************************************************************************
 * Register routines to store and retrieve DL info to allow object download resume.
 * \ref VDM_DL_registerResume().
 *
 * \param	getFunc Function to get the stored data
 *
 * \param	setFunc Function to store the relevant data
 *
********************************************************************************/
LOCAL VDM_Error DM_DLResumeSetFunc(IU32 offset, UTF8Str eTag)
{
	uint32				write_size = 0;
	MMIFILE_HANDLE		file_handle = 0;
	wchar*				filename_ptr = PNULL;
	MMIFILE_ERROR_E 	error = SFS_NO_ERROR;
	wchar				full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	uint16				full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	//int 				readlen = 0; 
	VDM_Error			result = VDM_ERR_OK;
	uint32 				len = 0;

	filename_ptr = SCI_ALLOC_APP((strlen(s_resume_file)+1) * sizeof(wchar));
    if (PNULL == filename_ptr)
    {
        return VDM_ERR_MEMORY;
    }
	len = (uint32)strlen(s_resume_file)+1;
	SCI_MEMSET(filename_ptr,0,(len* sizeof(wchar)));

    //SCI_TRACE_LOW:"MMIDM==> DM_DLResumeSetFunc %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2846_112_2_18_2_14_18_679,(uint8*)"s",s_resume_file);
	MMIAPICOM_StrToWstr((uint8*)s_resume_file,filename_ptr);
	if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
	{
		file_handle = MMIAPIFMM_CreateFile(full_path_arr,SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);

		if(0 ==file_handle)
		{						
		    SCI_FREE(filename_ptr);
			//SCI_TRACE_LOW:"MMIDM==>  DM_DLResumeSetFunc open file fail"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2855_112_2_18_2_14_18_680,(uint8*)"");
            SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
			return result;
		}
		SCI_MEMSET(&s_vdm_resume, 0x00, sizeof(DM_RESUME_DATA_STRUCT));
		s_vdm_resume.offset = offset;
		if(eTag && strlen((char*)eTag)>0)
		{
		       len = strlen((char*)eTag);
			SCI_MEMCPY(s_vdm_resume.eTag,eTag,len);
		}
		error = MMIAPIFMM_WriteFile(
			file_handle,
			&s_vdm_resume,
			sizeof(s_vdm_resume),
			&write_size,
			NULL
			);
		//SCI_TRACE_LOW:"MMIDM==>  DM_DLResumeSetFunc error =%d,write_size=%d,sizeof(s_vdm_resume) size=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2873_112_2_18_2_14_18_681,(uint8*)"ddd",error,write_size,sizeof(s_vdm_resume));

//		if (SFS_ERROR_NONE != error) //write_size < sizeof(s_vdm_resume) ||
//		{
//			SCI_TRACE_LOW("MMIDM==>  DM_DLResumeSetFunc VDM_ERR_STORAGE_WRITE ");
//			result = VDM_ERR_STORAGE_WRITE;
// 		}
	   
		MMIAPIFMM_CloseFile(file_handle);
	}
    else
    {
        SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
    }
    SCI_FREE(filename_ptr);
	return result;
}

/*****************************************************************************/
//	Description : remove resume file
//	Global resource dependence : 
//	Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC int MMIDM_RemoveResumeFile(MMIDM_RESUME_TYPE type)
{
	MMIFILE_ERROR_E 	error = SFS_NO_ERROR;
	wchar				full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	uint16				full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	//MMIFILE_HANDLE		file_handle = 0;
	wchar*				filename_ptr = PNULL;	
	char *              resume_file_ptr = PNULL;
    int result = VDM_ERR_OK;
	uint32				len =0;

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_RemoveResumeFile "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2906_112_2_18_2_14_18_682,(uint8*)"");
    
    resume_file_ptr = DM_GetResumeFileName(type);

	filename_ptr = SCI_ALLOC_APP((strlen(resume_file_ptr)+1) * sizeof(wchar));
	len= (uint32)strlen(resume_file_ptr)+1;
	SCI_MEMSET(filename_ptr,0,(len * sizeof(wchar)));

	MMIAPICOM_StrToWstr((uint8*)resume_file_ptr,filename_ptr);
	if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
	{
		if(MMIAPIFMM_IsFileExist(full_path_arr,full_path_len))
		{
			error= MMIAPIFMM_DeleteFile(full_path_arr, NULL);
		}
	}
    //SCI_TRACE_LOW:"MMIDM==>  MMIDM_RemoveResumeFile: DL resume file %s result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2922_112_2_18_2_14_18_683,(uint8*)"sd", resume_file_ptr,error);
    SCI_FREE(filename_ptr);
	return result;
}
/*****************************************************************************/
//	Description : delete all resume files
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_DeleteAllResumeFile(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_DeleteAllResumeFile "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2932_112_2_18_2_14_18_684,(uint8*)"");
    
    if (DM_CANCEL == MMIDM_GetDlState())
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_DeleteAllResumeFile "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2936_112_2_18_2_14_18_685,(uint8*)"");
        MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO);
        MMIDM_RemoveResumeFile(RESUME_TYPE_SCOMO);
        MMIDM_RemoveResumeFile(RESUME_TYPE_SCOMO_DATA);
        MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO_DATA);
        MMIDM_CleanResumeNv();
    }

    SCI_MEMSET(&s_vdm_resume,0,sizeof(DM_RESUME_DATA_STRUCT));
}

/*****************************************************************************/
//	Description : get resume file name
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL char* DM_GetResumeFileName(MMIDM_RESUME_TYPE type)
{
    char * resume_file = PNULL;

    switch(type) 
    {
        case RESUME_TYPE_FUMO:
            resume_file = DM_CLIENT_PL_RESUME_FUMO_FILE;
    	    break;
        case RESUME_TYPE_FUMO_DATA:
            resume_file = DM_FUMO_UPDATE_FILE;
            break;
        case RESUME_TYPE_SCOMO:
            resume_file = DM_CLIENT_PL_RESUME_SCOMOFILE;
    	    break;
        case RESUME_TYPE_SCOMO_DATA:
            resume_file = DM_SCOMO_UPDATE_FILE;
    	    break;
        case RESUME_TYPE_CMCC_MENU:
            resume_file = DM_CLIENT_PL_RESUME_FLEX_FILE;
    	    break;
        default:
            break;
    }
    return resume_file;
}
/*****************************************************************************/
//	Description : whether resume file is exist or not
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsResumeFileExist(MMIDM_RESUME_TYPE type)
{
	//MMIFILE_ERROR_E 	error = SFS_NO_ERROR;
	wchar				full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	uint16				full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	//MMIFILE_HANDLE		file_handle = 0;
	wchar*				filename_ptr = PNULL;	
	BOOLEAN 			result = FALSE;
    char*               resume_file_ptr = PNULL;
	uint32					len=0;

    //SCI_TRACE_LOW:"MMIDM==> DM_IsResumeFileExist "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2992_112_2_18_2_14_18_686,(uint8*)"");

    resume_file_ptr = DM_GetResumeFileName(type);
    if (PNULL == resume_file_ptr)
    {
        return FALSE;
    }
    //SCI_TRACE_LOW:"MMIDM==> %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_2999_112_2_18_2_14_18_687,(uint8*)"s",resume_file_ptr);

	len = (uint32)((strlen(resume_file_ptr)+1)* sizeof(wchar));
	filename_ptr = SCI_ALLOC_APP(len);
	SCI_MEMSET(filename_ptr,0,len);

	MMIAPICOM_StrToWstr((uint8*)resume_file_ptr,filename_ptr);
	if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
	{
		if(MMIAPIFMM_IsFileExist(full_path_arr,full_path_len))
		{
			result = TRUE;
		}
	}
    SCI_FREE(filename_ptr);
	return result;
}

/*****************************************************************************/
//	Description : send message to app task
//	Global resource dependence : 
//	Author: jian.ma
//	Note:
//	Return:
/*****************************************************************************/
LOCAL void SendSignalToMMITask(uint16 sig_id,uint16 event_id, void *data_ptr)
{
    MmiAppDMMsgS *sig_ptr = PNULL;

    MmiCreateSignal(sig_id, sizeof(MmiAppDMMsgS), (MmiSignalS**)&sig_ptr);

    sig_ptr->SignalSize = sizeof(MmiAppDMMsgS);
    sig_ptr->Sender = P_APP;
    sig_ptr->event_id = event_id;
    if (PNULL != data_ptr)
    {
        sig_ptr->param = data_ptr;
    }
    MMI_SEND_SIGNAL(sig_ptr, P_APP)
    //SCI_TRACE_LOW:"MMIDM==>  SendSignalToMMITask , sig_id is %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3036_112_2_18_2_14_18_688,(uint8*)"d",sig_id);
}


/*****************************************************************************/
//	Description : LAWMO confirm partially lock cb
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL IBOOL DM_ConfirmPartiallyLockCB(VDM_Handle_t inLAWMOInstance,void *inDmcContext)
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_ConfirmPartiallyLockCB To do Lock operation"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3046_112_2_18_2_14_18_689,(uint8*)"");
    DM_LawmoSetLockState(E_VDM_LAWMO_STATE_PartiallyLocked);
    SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_LOCK_DEVICE,PNULL);
	return TRUE;
}
#ifdef DM_SUPPORT_DFRESH
/*****************************************************************************/
//	Description : LAWMO confirm lock
//	Global resource dependence : none
//	Author:mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC IBOOL DM_ConfirmPartiallyLock(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_ConfirmPartiallyLock do Lock operation"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3058_112_2_18_2_14_18_690,(uint8*)"");
    SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_LOCK_DEVICE,PNULL);
	return TRUE;
}
#endif
/*****************************************************************************/
//	Description : LAWMO confirm unlock cb
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL IBOOL DM_ConfirmUnLockCB(VDM_Handle_t inLAWMOInstance,void *inDmcContext)
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_ConfirmUnLockCB do UnLock operation"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3069_112_2_18_2_14_18_691,(uint8*)"");
    DM_LawmoSetLockState(E_VDM_LAWMO_STATE_UnLocked);
    SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_UNLOCK_DEVICE,inDmcContext);
	return TRUE;
}
#ifdef DM_SUPPORT_DFRESH
/*****************************************************************************/
//	Description : LAWMO confirm unlock
//	Global resource dependence : none
//	Author:mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC IBOOL DM_ConfirmUnLock(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_ConfirmUnLock do UnLock operation"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3081_112_2_18_2_14_18_692,(uint8*)"");
    SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_UNLOCK_DEVICE,PNULL);
	return TRUE;
}
#endif
/*****************************************************************************/
//	Description : LAWMO Factory reset cb
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL IBOOL DM_ConfirmFactoryResetCB(VDM_Handle_t inLAWMOInstance,void *inDmcContext)
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_ConfirmFactoryResetCB To do FactoryReset operation"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3092_112_2_18_2_14_18_693,(uint8*)"");
	MMIDM_ResetFactorySetting();
    s_is_need_reset = TRUE;
	return TRUE;
}
#ifdef DM_SUPPORT_DFRESH
/*****************************************************************************/
//	Description : LAWMO confirm factoryreset
//	Global resource dependence : none
//	Author:mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC IBOOL DM_ConfirmFactoryReset(void)
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_ConfirmFactoryReset To do FactoryReset operation"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3104_112_2_18_2_14_18_694,(uint8*)"");
	MMIDM_ResetFactorySetting();
    s_is_need_reset = TRUE;
	return TRUE;
}
#endif
/*****************************************************************************/
//	Description : is need reset
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedReset(void)
{
    return s_is_need_reset;
}
/*****************************************************************************/
//	Description : LAWMO_Plugin_init
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL VDM_Error LAWMO_Plugin_init(const uint8 *inDMTreeURI)
{	
	VDM_Error result = VDM_ERR_OK;
	VDM_LAWMO_Callbacks_t placeholders = {0};	

	placeholders.confirmPartiallyLockCB = DM_ConfirmPartiallyLockCB;
	placeholders.confirmUnLockCB = DM_ConfirmUnLockCB;
	placeholders.confirmFactoryResetCB = DM_ConfirmFactoryResetCB;

	//SCI_TRACE_LOW:"MMIDM==>  LAWMO_Plugin_init Enter, LAWMO_Plugin_init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3131_112_2_18_2_14_18_695,(uint8*)"");
    if (PNULL==s_lawmo)
    {
        s_lawmo = MMIDM_LAWMO_createInstance(
		inDMTreeURI,	// LAWMO root URI
		&placeholders,
		NULL);	//no context - we're using static data instead
    }
	//SCI_TRACE_LOW:"MMIDM==>  LAWMO_Plugin_init = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3139_112_2_18_2_14_18_696,(uint8*)"d", s_lawmo);
#ifndef WIN32
	if(s_lawmo == NULL)
	{
		result = VDM_ERR_UNSPECIFIC;		
	}
#endif
	return result;
}
/*****************************************************************************/
//	Description : LAWMO_Plugin_term
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void LAWMO_Plugin_term(void)
{	
	if(PNULL != s_lawmo)
	{
		MMIDM_LAWMO_destroyInstance(&s_lawmo);
	}
	//SCI_TRACE_LOW:"MMIDM==>  LAWMO_Plugin_term "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3158_112_2_18_2_14_18_697,(uint8*)"");
}

/*****************************************************************************/
//	Description : LAWMO_Plugin_term
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void DM_LawmoSetLockState(E_VDM_LAWMO_STATE_t state)
{
	VDM_Error result= FALSE;;
	result = MMIDM_LAWMO_setLockState(s_lawmo,state);
	//SCI_TRACE_LOW:"MMIDM==>  DM_LawmoSetLockState: result= 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3169_112_2_18_2_14_18_698,(uint8*)"d", result);
}


/*****************************************************************************/
//	Description :  SCOMO ready to install
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoReadyToInstall(VDM_Handle_t	inScomoDPInstance,
								   void* inDmcContext,
								   UTF8CStr inOperationPath,
								   E_VDM_SCOMO_RESULT_t *outResult)
{	 
	
	return TRUE;
}

/*****************************************************************************/
//	Description :  SCOMO confirm uninstallation
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoConfirmUninstallation(
										    VDM_Handle_t	inScomoDCInstance,
										    void*			inDmcContext
										  )
{
	//SCI_TRACE_LOW:"MMIDM==>  SCOMO confirmUninstall"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3197_112_2_18_2_14_18_699,(uint8*)"");
	return FALSE;
}
/*****************************************************************************/
//	Description :  SCOMO ready to remove 
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL IBOOL DM_OnScomoReadyToRemove(
								  VDM_Handle_t inScomoDCInstance,
								  void* inDmcContext,
								  E_VDM_SCOMO_RESULT_t *outResult
								  )
{
	//SCI_TRACE_LOW:"MMIDM==>  DM_OnScomoReadyToRemove"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3211_112_2_18_2_14_19_700,(uint8*)"");
	return FALSE;
}

/*****************************************************************************/
//	Description :  SCOMO plugin initiate DC
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL BOOLEAN SCOMO_Plugin_initDCs(SCOMO_Plugin_t* pPlugin)
{
	IU32 ioLen = 0;
	VDM_Error err = VDM_ERR_OK;
	char *dcNames = NULL;
	char *p = PNULL;
	char *s = PNULL;
	char nameBuff[MAX_ID_LEN] = {0};
	char dcId[MAX_ID_LEN] = {0};
	int len = 0;
	SCOMO_DC_t *node = NULL;
    //IBOOL bSuccess = FALSE;
    int   id = 0;
    if (PNULL == pPlugin)
    {
	    //SCI_TRACE_LOW:"MMIDM==> SCOMO_Plugin_initDCsall PNULL == pPlugin"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3235_112_2_18_2_14_19_701,(uint8*)"");
        return FALSE;
    }
    MMIDM_InitScomoIdToTab();
	pPlugin->dcCount = 0;
	pPlugin->dcList = NULL;
	
	// traverse inventory to get the number and related info of DCs 
	err = MMIDM_SCOMO_PL_INV_getAllComponents((UTF8CStr)SCOMO_ROOT, NULL, &ioLen);
	if (VDM_ERR_BUFFER_OVERFLOW == err && ioLen > 0)
	{
		dcNames = SCI_ALLOC_APP(ioLen+1);
		SCI_MEMSET(dcNames,0,(ioLen+1));

		err = MMIDM_SCOMO_PL_INV_getAllComponents((UTF8CStr)SCOMO_ROOT, (IU8*)dcNames, &ioLen);
		if (VDM_ERR_OK == err)
		{
			dcNames[ioLen] = '\0';
		}
		else
		{
			SCI_FREE(dcNames);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	//SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_initDCsall name: %s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3265_112_2_18_2_14_19_702,(uint8*)"s", dcNames);
	s = dcNames;

	do
	{
		p = strchr(s, '/');
		if (p)
		{
			len = p - s;
			strncpy(nameBuff, s, len);
			nameBuff[len] = '\0';
			s = p + 1;
		}
		else
		{
			strcpy(nameBuff, s);
		}
			
		//SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_initDCsall name"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3283_112_2_18_2_14_19_703,(uint8*)"");
		// get component id from name
		ioLen = sizeof(dcId) - 1;
		err = MMIDM_SCOMO_PL_INV_getComponentId((UTF8CStr)SCOMO_ROOT,
							(UTF8CStr)nameBuff, (IU8*)dcId, &ioLen);
		if (VDM_ERR_OK == err && ioLen > 0)
		{
			dcId[ioLen] = '\0';
		}
		else
		{
			continue;
		}

		//SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_initDCsall id: %s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3297_112_2_18_2_14_19_704,(uint8*)"s", dcId);
        id = atoi(dcId);//VDM_PL_atoIU32(dcId,DM_RADIX_TYPE,&bSuccess)
		//SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_initDCsall name id=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3299_112_2_18_2_14_19_705,(uint8*)"d",id);
        MMIDM_AddScomoIdToTab(id);

		// add dc node to list
		node = (SCOMO_DC_t *)SCI_ALLOC_APP(sizeof(SCOMO_DC_t));
        SCI_MEMSET(node,0,sizeof(SCOMO_DC_t))
		node->id = MMIDM_UTL_utf8dup((UTF8CStr)dcId);
		node->next = pPlugin->dcList;
		pPlugin->dcList = node;
		pPlugin->dcCount++;
        //SCI_FREE(node);
	}while (p);

	SCI_FREE(dcNames);
    
    return TRUE;
}

/*****************************************************************************/
//	Description :  SCOMO DP session handle download trigger
//	Global resource dependence : 
//	Author: 
//	Note:
//	Return:
/*****************************************************************************/
//extern VDM_Error VDM_SCOMO_DP_SessionHandler_DL_trigger(
//														VDM_SCOMO_DP_Instance_t* inInstance,
//														E_VDM_SCOMO_DP_Session_t inSessionType
//														)
//{
//
//	VDM_Error	   result = VDM_ERR_OK;
//	SCI_TRACE_LOW("MMIDM==>  VDM_SCOMO_DP_SessionHandler_DL_trigger inSessionType = %d",inSessionType);
//
//	return result;
//}

/*****************************************************************************/
//	Description :  resume SCOMO Download Session
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL VDM_Error DM_ResumeScomoDLSess(void)
{

	SCOMO_Plugin_t* instance = s_scomoInstance;
	//SCI_TRACE_LOW:"MMIDM==> DM_ResumeScomoDLSess"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3343_112_2_18_2_14_19_706,(uint8*)"");
	return VDM_SCOMO_DP_SessionHandler_DL_trigger(instance->scomoDP,
				E_VDM_SCOMO_DP_Session_dlAndInstall);
}

/*****************************************************************************/
//	Description :  get SCOMO result code
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL int DM_GetScomoResultCode(SCOMO_Result_Code code)
{
	int resultCode = 0;
	switch (code)
	{
		case SCOMO_Successful:
			resultCode = (int)E_VDM_SCOMO_RESULT_successful;
			break;
		case SCOMO_Client_Error:
			resultCode = (int)E_VDM_SCOMO_RESULT_client_error;
			break;
		case SCOMO_User_Cancelled:
			resultCode = (int)E_VDM_SCOMO_RESULT_download_cancelled;
			break;
		case SCOMO_Download_Failed:
			resultCode = (int)E_VDM_SCOMO_RESULT_download_failed;
			break;
		case SCOMO_Download_Failed_Memory:
			resultCode = (int)E_VDM_SCOMO_RESULT_dl_failed_device_out_of_memory;
			break;
		case SCOMO_Install_Failed:
			resultCode = (int)E_VDM_SCOMO_RESULT_install_failed;
			break;
		case SCOMO_Install_Failed_Memory:
			resultCode = (int)E_VDM_SCOMO_RESULT_install_failed_device_out_of_memory;
			break;
		case SCOMO_Not_Implemented:
			resultCode = (int)E_VDM_SCOMO_RESULT_not_implemented;
			break;
		case SCOMO_Undefined_Error:
			resultCode = (int)E_VDM_SCOMO_RESULT_undefined_error;
			break;
		case SCOMO_Alternate_Download_Server_Error:
			resultCode = (int)E_VDM_SCOMO_RESULT_dl_server_error;
			break;
		case SCOMO_Alternate_Download_Server_Unavailable:
			resultCode = (int)E_VDM_SCOMO_RESULT_dl_server_unavailable;
			break;
		default:
			//SCI_TRACE_LOW:"MMIDM==>  DM_GetScomoResultCode: Unknow SCOMO result code!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3392_112_2_18_2_14_19_707,(uint8*)"");
			break;
	}
	return resultCode;
}

/*****************************************************************************/
//	Description :  set DP install result
//	Global resource dependence : 
//	Author: jian.ma
//	Note:
//	Return:
/*****************************************************************************/
LOCAL VDM_Error DM_SetDPInstallResult(
                                      SCOMO_Result_Code scomo_result,
                                      int suiteID,
                                      const MMIDM_SCOMO_APP_T * dp_header
                                      )
{
	VDM_Error ret = VDM_ERR_UNSPECIFIC;
    VDM_Error add_ret = VDM_ERR_UNSPECIFIC;
    VDM_Error del_ret = VDM_ERR_UNSPECIFIC;
   // MMIFILE_ERROR_E 	error = SFS_NO_ERROR;
   // wchar				full_path_arr[MMIFILE_FULL_PATH_MAX_LEN] = {0};
  //  uint16				full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
   // MMIFILE_HANDLE		file_handle = 0;
   // wchar*				filename_ptr = PNULL;	
    int32               result = VDM_ERR_OK;
    VDM_SCOMO_DC_Callbacks_t dcCallbacks = {0};
    SCOMO_DC_t* node = NULL;
    char dcId [MAX_ID_LEN] ={0};
	SCOMO_Plugin_t* instance = PNULL;
	VDM_Handle_t dpInstance = PNULL;
	int code = DM_GetScomoResultCode(scomo_result);
    
	//SCI_TRACE_LOW:"MMIDM==> DM_SetDPInstallResult: result = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3425_112_2_18_2_14_19_708,(uint8*)"d", result);

    if (PNULL == s_scomoInstance->scomoDP)
    {
	    //SCI_TRACE_LOW:"MMIDM==> DM_SetDPInstallResult PNULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3429_112_2_18_2_14_19_709,(uint8*)"");
        return VDM_ERR_OK;
    }
	if (SCOMO_Successful == scomo_result &&PNULL!=dp_header)/*lint !e774 */
	{
        instance = s_scomoInstance;
	    dpInstance = instance->scomoDP;
        MMIDM_IU32toa(suiteID,dcId,DM_RADIX_TYPE);
        //SCI_TRACE_LOW:"MMIDM==>  MMIDM_IU32toa"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3437_112_2_18_2_14_19_710,(uint8*)"");
        // add to inventory
		ret = MMIDM_SCOMO_PL_INV_addComponent((UTF8CStr)SCOMO_ROOT,
			(UTF8CStr)dcId, (UTF8CStr)dp_header->name,
			(UTF8CStr)DP_ID, (UTF8CStr)dp_header->version,
 			NULL, NULL, (UTF8CStr)DM_SCOMO_INSTALL_TREE_PATH);
        //SCI_TRACE_LOW:"MMIDM==>  MMIDM_SCOMO_PL_INV_addComponent ret=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3443_112_2_18_2_14_19_711,(uint8*)"d",ret);


        if (VDM_ERR_OK != ret)
		{
			if (VDM_ERR_NODE_ALREADY_EXISTS == ret)//component already exists
			{
				// delete component in inventory first
				del_ret = MMIDM_SCOMO_PL_INV_deleteComponent((UTF8CStr)SCOMO_ROOT, (UTF8CStr) dcId);
                //SCI_TRACE_LOW:"MMIDM==>  VDM_ERR_NODE_ALREADY_EXISTS del_ret =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3452_112_2_18_2_14_19_712,(uint8*)"d",del_ret);

				// add again
				add_ret = MMIDM_SCOMO_PL_INV_addComponent((UTF8CStr)SCOMO_ROOT,
					(UTF8CStr) dcId, (UTF8CStr) dp_header->name,
					(UTF8CStr) DP_ID, (UTF8CStr) dp_header->version,
					NULL, NULL, (UTF8CStr)DM_SCOMO_INSTALL_TREE_PATH);
                //SCI_TRACE_LOW:"MMIDM==>  VDM_ERR_NODE_ALREADY_EXISTS ret =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3459_112_2_18_2_14_19_713,(uint8*)"d",ret);
	            if (VDM_ERR_OK != add_ret)
			    {
                    code = (int)E_VDM_SCOMO_RESULT_client_error;
				    //SCI_TRACE_LOW:"MMIDM==> DM_SetDPInstallResult Failed to add component to inventory! error = 0x%x"
				    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3463_112_2_18_2_14_19_714,(uint8*)"d", add_ret);
			    }
            }
		}
        else //(VDM_ERR_OK == ret ||VDM_ERR_OK ==add_ret)
        {            
            //SCI_TRACE_LOW:"MMIDM==> DM_SetDPInstallResult added to inventory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3469_112_2_18_2_14_19_715,(uint8*)"");
            // get DC instance and add to the DC list                
            dcCallbacks.confirmRemoveCB = DM_OnScomoConfirmUninstallation;
            dcCallbacks.executeRemoveCB = DM_OnScomoReadyToRemove;
            node = SCI_ALLOC_APP(sizeof(SCOMO_DC_t));
            SCI_MEMSET(node,0,sizeof(SCOMO_DC_t));
            node->id = MMIDM_UTL_utf8dup((UTF8CStr)dcId);
            node->next = instance->dcList;
            node->handle = MMIDM_SCOMO_DC_createInstance(
                node->id, &dcCallbacks, instance);
            //SCI_TRACE_LOW:"MMIDM==> DM_SetDPInstallResult created instance"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3479_112_2_18_2_14_19_716,(uint8*)"");
            instance->dcList = node;
            instance->dcCount++;
            //SCI_FREE(node);
            MMIDM_AddScomoIdToTab(suiteID);
        }
        ret = MMIDM_SCOMO_DP_setInstallationResult(dpInstance, (E_VDM_SCOMO_RESULT_t)code);
        //SCI_TRACE_LOW:"MMIDM==>  MMIDM_SCOMO_DP_setInstallationResult ret =0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3486_112_2_18_2_14_19_717,(uint8*)"d",ret);
	}
	else
	{
		ret = VDM_ERR_BAD_INPUT;
	}
    //SCI_TRACE_LOW:"MMIDM==>  MMIDM_SCOMO_DP_setInstallationResult ret 1=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3492_112_2_18_2_14_19_718,(uint8*)"d",ret);
	return ret;
}

/*****************************************************************************/
//	Description :  remove DC from Inventory
//	Global resource dependence : 
//	Author: jianhui.luo
//	Note:
//	Return:
/*****************************************************************************/
LOCAL VDM_Error DM_RemoveDCFromInventory(int suiteID)
{
	
	VDM_Error ret = VDM_ERR_OK;
	//SCOMO_Plugin_t *instance = s_scomoInstance;
	SCOMO_DC_t *p = PNULL;
	SCOMO_DC_t *q = PNULL;
    char dcId [MAX_ID_LEN] ={0};
    //SCI_TRACE_LOW:"MMIDM==>  DM_RemoveDCFromInventory %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3509_112_2_18_2_14_19_719,(uint8*)"d",suiteID);

    if (PNULL == s_scomoInstance||PNULL== s_scomoInstance->dcList)
    {
        //SCI_TRACE_LOW:"MMIDM==>  DM_RemoveDCFromInventory PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3513_112_2_18_2_14_19_720,(uint8*)"");
        return VDM_ERR_INVALID_CALL;
    }
    MMIDM_IU32toa(suiteID,dcId,DM_RADIX_TYPE);

    if(!VDM_PL_strcmp((const char *) s_scomoInstance->dcList->id, dcId))
    {
        p = s_scomoInstance->dcList;
		MMIDM_SCOMO_DC_deleteFromInventory(p->handle);//remove the DC from inventory
		MMIDM_Tree_writeToPersistentStorage();
		MMIDM_SCOMO_DC_destroyInstance(&(p->handle));//destroy th DC instance
		SCI_FREE(p->id);
		VDM_PL_freeAndNullify(p);
		s_scomoInstance->dcCount--;
        s_scomoInstance->dcList = s_scomoInstance->dcList->next;
        MMIDM_DelScomoIdToTab(suiteID);
		//SCI_TRACE_LOW:"MMIDM==> DM_RemoveDCFromInventory: Selected DC has been removed.1"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3529_112_2_18_2_14_19_721,(uint8*)"");
    }
    else
    {
         p = s_scomoInstance->dcList->next;
         q = s_scomoInstance->dcList;

	    while (p)
	    {
            //SCI_TRACE_LOW:"MMIDM==>  DM_RemoveDCFromInventory p=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3538_112_2_18_2_14_19_722,(uint8*)"d",p);
		    if (!VDM_PL_strcmp((const char *) p->id, dcId))//found the matching DC
            {
			    break;
            }
		    q = p;
		    p = p->next;
	    }
	    
	    if (p)
	    {
		    q->next = p->next;//remove the node from list
		    MMIDM_SCOMO_DC_deleteFromInventory(p->handle);//remove the DC from inventory
		    MMIDM_Tree_writeToPersistentStorage();
		    MMIDM_SCOMO_DC_destroyInstance(&(p->handle));//destroy th DC instance
		    SCI_FREE(p->id);
		    VDM_PL_freeAndNullify(p);
		    s_scomoInstance->dcCount--;
            MMIDM_DelScomoIdToTab(suiteID);
		    //SCI_TRACE_LOW:"MMIDM==> DM_RemoveDCFromInventory: Selected DC has been removed.2"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3557_112_2_18_2_14_19_723,(uint8*)"");
        }
	    else
	    {
		    //SCI_TRACE_LOW:"MMIDM==> DM_RemoveDCFromInventory: Cannot find selected DC in inventory."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3561_112_2_18_2_14_19_724,(uint8*)"");
		    ret = VDM_ERR_INVALID_CALL;
	    }
    }


	return ret;
}

/*****************************************************************************/
//	Description : SCOMO Plugin create
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL SCOMO_Plugin_t* SCOMO_Plugin_create(void)
{	
    if ( PNULL == s_scomoInstance)
    {
        s_scomoInstance = SCI_ALLOC_APP(sizeof(SCOMO_Plugin_t));
        if (PNULL != s_scomoInstance)
        {
            SCI_MEMSET(s_scomoInstance,0,sizeof(SCOMO_Plugin_t));
        }
    }
    //SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_create"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3584_112_2_18_2_14_19_725,(uint8*)"");
	return s_scomoInstance;
}

/*****************************************************************************/
//	Description : SCOMO Plugin term
//	Global resource dependence : none
//	Author:jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SCOMO_Plugin_term(void)
{
    SCOMO_DC_t *temp = NULL;
	SCOMO_Plugin_t* instance = s_scomoInstance;
    SCOMO_DC_t* list = PNULL;
    //SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_term"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3597_112_2_18_2_14_19_726,(uint8*)"");
	if (PNULL!=instance)
    {
        list = instance->dcList;
        while(list)
	    {
		    #if 0 //@CR248730 2011.06.27  MMIDM_SCOMO_destroy 已经会释放
		    if (list->handle)
		    {
			    MMIDM_SCOMO_DC_destroyInstance(&(list->handle));
		    }
		    #endif
		    if (list->id)
		    {
			    SCI_FREE(list->id);
		    }

		    temp = list;
		    list = list->next;
            //SCI_TRACE_LOW:"MMIDM==> SCOMO_Plugin_term temp=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3616_112_2_18_2_14_19_727,(uint8*)"d",temp);
		    if(temp)/*lint !e774*/
            {
                SCI_FREE(temp);
            }
	    }
        //SCI_TRACE_LOW:"MMIDM==> SCOMO_Plugin_term destroy dp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3622_112_2_18_2_14_19_728,(uint8*)"");
	    #if 0 //@CR248730 2011.06.27 MMIDM_SCOMO_destroy 已经会释放
	    if (instance->scomoDP)
	    {
	        //SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_term instance->scomoDP instance->scomoDP %x"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3626_112_2_18_2_14_19_729,(uint8*)"d",instance->scomoDP);
		    MMIDM_SCOMO_DP_destroyInstance(&(instance->scomoDP));
	    }
	    #endif
        //SCI_TRACE_LOW:"MMIDM==> SCOMO_Plugin_term 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3630_112_2_18_2_14_19_730,(uint8*)"");
	    MMIDM_SCOMO_destroy();

        //SCI_TRACE_LOW:"MMIDM==> SCOMO_Plugin_term 3"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3633_112_2_18_2_14_19_731,(uint8*)"");
	    SCI_FREE(instance);
    }
	s_scomoInstance = NULL;
	//SCI_TRACE_LOW:"MMIDM==>  SCOMO_Plugin_term done"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3637_112_2_18_2_14_19_732,(uint8*)"");
}

/*****************************************************************************/
//  Description : get DP header length
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VDM_PL_getDPHeaderLength(void)
{
    //SCI_TRACE_LOW:"MMIDM==>  MMIDM_GetDPHeaderLength =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3646_112_2_18_2_14_19_733,(uint8*)"d",sizeof(MMIDM_APP_PAKEAGE_T));
	return 0;//(uint32)sizeof(MMIDM_APP_PAKEAGE_T);
}

/*****************************************************************************/
//  Description : set download waht
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void VDM_PL_Set_Download_What(IU32 downloadType)
{
    //SCI_TRACE_LOW:"MMIDM==> downloadType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3656_112_2_18_2_14_19_734,(uint8*)"d",downloadType);
    switch(downloadType) 
    {
        case RESUME_TYPE_FUMO:
            s_resume_file = DM_CLIENT_PL_RESUME_FUMO_FILE;
    	    break;
        case RESUME_TYPE_SCOMO:
            s_resume_file = DM_CLIENT_PL_RESUME_SCOMOFILE;
    	    break;
        case RESUME_TYPE_CMCC_MENU:
            s_resume_file = DM_CLIENT_PL_RESUME_FLEX_FILE;
    	    break;
        default:
            break;
    }
    MMINV_WRITE(MMINV_DM_RESUME_TYPE,&downloadType);
    
}
/*****************************************************************************/
//  Description : DM task send signal to MMI task
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SendSignalToMMITask(uint16 sig_id,uint16 event_id, void *data_ptr)
{
    SendSignalToMMITask(sig_id,event_id,data_ptr);
}

/*****************************************************************************/
//  Description : open dm alert win
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
LOCAL void DM_OpenDMAlerthandleWin(
                                MMIDM_ALERT_TYPE_E mode, 
                                const uint8* text,
                                 uint16 len,
                                 uint32 max_timer
                                 )
{

    MMIDM_NOTIFY_ALERT_T *notify_alert_ptr = PNULL;
    notify_alert_ptr= SCI_ALLOCA(sizeof(MMIDM_NOTIFY_ALERT_T));
    if (PNULL != notify_alert_ptr)
    {
        SCI_MEMSET(notify_alert_ptr,0x00,sizeof(MMIDM_NOTIFY_ALERT_T));
        notify_alert_ptr->type = mode;
        notify_alert_ptr->string = text;/*lint !e605*/
        notify_alert_ptr->str_len = len;
        notify_alert_ptr->max_timer = max_timer;
        SendSignalToMMITask(APP_DM_MESSAGE_IND,MSG_DM_NOTIFY_ALERT,notify_alert_ptr);
    }
}

/*****************************************************************************/
//  Description : set dp header
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetDPHeader(void *header,BOOLEAN is_old_version)
{
    MMIDM_APP_PAKEAGE_OLD_T  *old_header_ptr = PNULL;
    MMIDM_APP_PAKEAGE_T      *header_ptr = PNULL;
    uint32 len = 0;
    if (PNULL == header)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetDPHeader PNULL == header"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3719_112_2_18_2_14_20_735,(uint8*)"");
        return;
    }
    SCI_MEMSET(&s_dp_header,0,sizeof(MMIDM_SCOMO_APP_T));
    if(is_old_version)
    {
        old_header_ptr = (MMIDM_APP_PAKEAGE_OLD_T*)header;
	len = (uint32)strlen((char*)old_header_ptr->name);
        SCI_MEMCPY(s_dp_header.name,old_header_ptr->name,len);
	len = strlen((char*)old_header_ptr->version);	
        SCI_MEMCPY(s_dp_header.version,old_header_ptr->version,len);
    }
    else
    {
        header_ptr = (MMIDM_APP_PAKEAGE_T*)header;
        
        //@CR121389
        //MMIAPICOM_WstrToStr(header_ptr->name,s_dp_header.name);
        GUI_WstrToUTF8(s_dp_header.name, (((DM_STRING_LENGTH+1)<<1) - 1), header_ptr->name, MMIAPICOM_Wstrlen(header_ptr->name));

        MMIAPICOM_WstrToStr(header_ptr->version,s_dp_header.version);
    }
}

extern PUBLIC E_VDM_NIA_UIMode_t MMIDM_GetUiMode(void);
/*****************************************************************************/
//  Description : check is running in backgroup
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_IsBackgroupRun(void)
{
	BOOLEAN result = FALSE;
    E_VDM_NIA_UIMode_t		inUIMode;
    inUIMode = MMIDM_GetUiMode();
    if(E_VDM_NIA_UIMode_Background == inUIMode ||E_VDM_NIA_UIMode_NotSpecified == inUIMode )
	{
		result = TRUE;
	}
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_IsBackgroupRun s_UIMode = %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3753_112_2_18_2_14_20_736,(uint8*)"dd",inUIMode, result);
	return result;
}
/*****************************************************************************/
//  Description : check is need display progress win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:alert 1101 need display progress win
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedDisplayProgresswin(void)
{
    BOOLEAN result = FALSE;
    MMIDM_PL_ALERT_TYPE  alert_type = DM_PL_ALERT_NONE;
    uint ret_value = 0;
    MMINV_READ(MMINV_DM_ALERT_TYPE,&alert_type, ret_value);//mary
    if (DM_PL_ALERT_1101 == alert_type)
    {
        result = TRUE;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_IsNeedDisplayProgresswin alert_type = %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_MMI_3770_112_2_18_2_14_20_737,(uint8*)"dd",alert_type, result);
    return result;
}
#endif
