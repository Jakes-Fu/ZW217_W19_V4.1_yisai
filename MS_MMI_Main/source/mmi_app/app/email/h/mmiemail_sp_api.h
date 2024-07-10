/***************************************************************************************
** File Name:      mmiemail_sp_api.h                                                   *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail Service Provider. The file contains functions   *
**                 to finish initialize HEmlSPMgr, get HConfigMgr ,                    *
**                 HTransMgr or HStoreMgr from HEmlSPMgr.                              *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAIL_SP_API_H_
#define _MMIEMAIL_SP_API_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "mmiemail_sp_config_api.h"
#include "mmiemail_sp_store_api.h"
#include "mmiemail_sp_transaction_api.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define E_INBOX_INDEX            0x00000001
#define E_OUTBOX_INDEX           0x00000002
#define E_SENTBOX_INDEX          0x00000003
#define E_DRAFT_INDEX            0x00000004
#define E_MAX_INDEX              0xffffffff

#define EMA_SP_ACTIVE_ACCOUNT    0xffffffff

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef void* HEmlSPMgr;

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp, to init handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_InitHandle(
                                    HEmlSPMgr *sp_handle_pptr
                                    );
/**************************************************************************************/
// Description : email sp, delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_DeleteHandle(HEmlSPMgr *sp_handle_pptr);
/**************************************************************************************/
// Description : email sp, get config handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC HConfigMgr MMIEMAIL_SP_GetCfgHandle(HEmlSPMgr sp_handle_ptr);
/**************************************************************************************/
// Description : email sp, to get transe handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC HTransMgr MMIEMAIL_SP_GetTrsHandle(HEmlSPMgr sp_handle_ptr);
/**************************************************************************************/
// Description : to get store handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC HStoreMgr MMIEMAIL_SP_GetStoreHandle(HEmlSPMgr sp_handle_ptr);

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#ifndef EMA_GETTRANS_HANDLE
#define EMA_GETTRANS_HANDLE   MMIEMAIL_SP_GetTrsHandle
#endif

#ifndef EMA_GETSTORE_HANDLE
#define EMA_GETSTORE_HANDLE   MMIEMAIL_SP_GetStoreHandle
#endif

#ifndef EMA_GETCFG_HANDLE
#define EMA_GETCFG_HANDLE     MMIEMAIL_SP_GetCfgHandle
#endif

#endif /* #ifndef _MMIEMAIL_SP_API_H_ */
