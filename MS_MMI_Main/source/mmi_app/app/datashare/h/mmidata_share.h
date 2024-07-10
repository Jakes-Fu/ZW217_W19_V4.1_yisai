

#ifndef __MMIDATA_SHARE_H__
#define __MMIDATA_SHARE_H__

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
// 	Description : active pdp 
//	Global resource dependence : 
//    Author: liang.wu1
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUSBSHARE_ConnectPDP(void);

/*****************************************************************************/
// 	Description : MMIUSBSHARE_Disable
//	Global resource dependence : 
//    Author: liang.wu1
//	Note:
/*****************************************************************************/
PUBLIC void MMIUSBSHARE_Disable(void);

/*****************************************************************************/
// 	Description : MMIUSBSHARE_IsRunning
//	Global resource dependence : 
//    Author: liang.wu1
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUSBSHARE_IsRunning(void);
PUBLIC BOOLEAN MMIUSBSHARE_EnterConnect(void);
PUBLIC void MMIUSBSHARE_Disconnect(void);
#ifdef   __cplusplus
    }
#endif

#endif
