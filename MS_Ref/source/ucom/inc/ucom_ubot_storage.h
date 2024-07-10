/******************************************************************************
 ** File Name:    UBOT_storage.h                                              *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005      Daniel.Ding     Create.                                    *
 ******************************************************************************/
#ifndef _UCOM_UBOT_STORAGE_
#define _UCOM_UBOT_STORAGE_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "ucom_common.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_UBOT_ClearCBW (void);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_UBOT_ClearCSW (void);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcReady (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCBW (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCSW (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcDataIn (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcDataOut (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCmdError (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcExeError (uint32 param,void * ptr);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcDisconnect (uint32 param,void * ptr);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCacheFull (uint32 param,void * ptr);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ProcCBD (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_DEVICE_STATE_E UCOM_UBOT_CheckDevState (uint8 id);

/*****************************************************************************/
//  Description: rom disk customconfig and get lun number    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
extern void UCOM_UBOT_CustomerConfig (void);
extern void UCOM_UBOT_GetCDROMBlockNum(uint8  lun);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
