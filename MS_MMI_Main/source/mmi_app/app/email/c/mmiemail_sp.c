/***************************************************************************************
** File Name:      mmiemail_sp.c                                                       *
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
/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_utils.h"

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef struct ema_spmgr_tag
{
   HTransMgr  trans_msg_ptr;
   HConfigMgr cfg_mgr_ptr;
   HStoreMgr  store_mgr_ptr;
}EMA_SPMGR_T;

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp, to init handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_InitHandle
(
    HEmlSPMgr *sp_handle_pptr
)
{
    int32       result = EMAIL_ERROR;
    EMA_SPMGR_T *sp_mgr_ptr = NULL;
    
    if (NULL == sp_handle_pptr)
    {
        return EMAIL_BADPARAM;
    }
    do 
    {
        sp_mgr_ptr = (EMA_SPMGR_T*)EMA_UTILS_MALLOC(NULL, sizeof(EMA_SPMGR_T));
        EMA_UTILS_BREAKNULL(sp_mgr_ptr, result, "malloc EMA_SPMGR_T failed");
        EMA_UTILS_MEMSET(sp_mgr_ptr, 0x00, sizeof(EMA_SPMGR_T));
        
        result = MMIEMAIL_SP_TRS_InitHandle(&sp_mgr_ptr->trans_msg_ptr, (void*)sp_mgr_ptr/*pstMemmgr*/);
        EMA_UTILS_BREAKIF(result, "EmailASP_TRS_InitHandle failed");
        
        result = MMIEMAIL_SP_Store_InitHandle(&sp_mgr_ptr->store_mgr_ptr, (void*)sp_mgr_ptr/*pstMemmgr*/);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_InitHandle failed");
        
        result = MMIEMAIL_SP_CFG_InitHandle(&sp_mgr_ptr->cfg_mgr_ptr, (void*)sp_mgr_ptr/*pstMemmgr*/);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_InitHandle failed");
        
        *sp_handle_pptr = (HEmlSPMgr)sp_mgr_ptr;
    } while(0);
    
    if (result != EMAIL_SUCCESS)
    {
        if (PNULL != sp_mgr_ptr)
        {
            MMIEMAIL_SP_TRS_DeleteHandle(&sp_mgr_ptr->trans_msg_ptr);
            MMIEMAIL_SP_Store_DeleteHandle(&sp_mgr_ptr->store_mgr_ptr);
            MMIEMAIL_SP_CFG_DeleteHandle(&sp_mgr_ptr->cfg_mgr_ptr);
            
            EMA_UTILS_FREE(NULL, sp_mgr_ptr);
            *sp_handle_pptr = PNULL;
        }
    }
    return result;
}

/**************************************************************************************/
// Description : email sp, delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_DeleteHandle(HEmlSPMgr *sp_handle_pptr)
{
    EMA_SPMGR_T *sp_mgr_ptr = (EMA_SPMGR_T*)(*sp_handle_pptr);
    
    if (PNULL == sp_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EmailASP_DeleteHandle, bad param"));
        return EMAIL_BADPARAM;
    }
    MMIEMAIL_SP_TRS_DeleteHandle(&sp_mgr_ptr->trans_msg_ptr);
    MMIEMAIL_SP_Store_DeleteHandle(&sp_mgr_ptr->store_mgr_ptr);
    MMIEMAIL_SP_CFG_DeleteHandle(&sp_mgr_ptr->cfg_mgr_ptr);
    EMA_UTILS_FREE(NULL, sp_mgr_ptr);
    *sp_handle_pptr = PNULL;
    
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, create handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
//LOCAL int32 SP_CreateHandle
//(
//   HEmlSPMgr *sp_handle_pptr
//);

/**************************************************************************************/
// Description : email sp, get config handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC HConfigMgr MMIEMAIL_SP_GetCfgHandle(HEmlSPMgr sp_handle_ptr)
{
   EMA_SPMGR_T *sp_mgr_ptr = (EMA_SPMGR_T*)sp_handle_ptr;

   if (PNULL == sp_mgr_ptr || PNULL == sp_mgr_ptr->cfg_mgr_ptr)
   {
      return PNULL;
   }

   return sp_mgr_ptr->cfg_mgr_ptr;
}

/**************************************************************************************/
// Description : email sp, to get transe handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC HTransMgr  MMIEMAIL_SP_GetTrsHandle(HEmlSPMgr sp_handle_ptr)
{
   EMA_SPMGR_T *sp_mgr_ptr = (EMA_SPMGR_T*)sp_handle_ptr;

   if (PNULL == sp_mgr_ptr || PNULL == sp_mgr_ptr->trans_msg_ptr)
   {
      return PNULL;
   }

   return sp_mgr_ptr->trans_msg_ptr;
}

/**************************************************************************************/
// Description : email sp, to get store handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC HStoreMgr  MMIEMAIL_SP_GetStoreHandle(HEmlSPMgr sp_handle_ptr)
{
   EMA_SPMGR_T *sp_mgr_ptr = (EMA_SPMGR_T*)sp_handle_ptr;

   if (PNULL == sp_mgr_ptr || PNULL == sp_mgr_ptr->store_mgr_ptr)
   {
      return PNULL;
   }

   return sp_mgr_ptr->store_mgr_ptr;
}