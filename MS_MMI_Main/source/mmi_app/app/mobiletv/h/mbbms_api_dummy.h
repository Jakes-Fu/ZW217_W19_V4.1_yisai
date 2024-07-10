/****************************************************************************
** File Name:      mbbms_api_dummy.h                                          *
** Author:                                                          *
** Date:           2008-12                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mtv application.       *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008                  Create
****************************************************************************/

#ifndef _MBBMS_API_DUMMY_H
#define _MBBMS_API_DUMMY_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "os_api.h"
#include "mbbms_sub.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
 #if 0
typedef enum
{

	MBBMS_SG_PRASE_SUCC= ((0x0A9 << 8)|1),
	MBBMS_SG_PARSE_FAIL,
	MBBMS_SG_SG_RCV_IND,
	MBBMS_MMI_GBA_RES,
	MBBMS_MMI_GBA_STOP_RES,
	MBBMS_MMI_MRK_RES,
	MBBMS_MMI_MRK_STOP_RES,
	MBBMS_MSK_SUB_SIGNAL,

	MBBMS_SIGNAL_MAX
	
}MBBMS_SIGNAL_CODE_E;

#endif
        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
// Description : Get the MBBMS MTV state: Open, cancel, suspending. 
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
//PUBLIC void MBBMS_Set_Operation_Status(MBBMS_OPERATION_E ope_status);

/*****************************************************************************/
// Description : play
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
//#ifndef WIN32
//uint32 CMMB_PlayNetworkByFrq(uint32 frequency, uint32 service_id, uint32 audio_language);
//#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MBBMS_API_DUMMY_H*/