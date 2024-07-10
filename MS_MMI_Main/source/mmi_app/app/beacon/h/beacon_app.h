/****************************************************************************
** File Name:      mmiphs_app.h                                                *
** Author:                                                                 *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshengqi         Create
** 
****************************************************************************/
#ifndef  _MMI_BEACON_APP_H_    
#define  _MMI_BEACON_APP_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "beacon_common.h"
#include "beacon_id.h"
#include "beacon_menutable.h"
#include "beacon_nv.h"
#include "beacon_text.h"
#include "beacon_image.h"
#include "beacon_anim.h"
#include "zdt_beacon_uart.h"
#include "zdt_beacon_parse_at.h"

#define BEACONAPP_TRACE            BEACON_Trace

typedef struct
{
       uint8 *        str;
	uint32         len; 
} MMI_BEACON_DATA_T;

typedef struct
{
	SIGNAL_VARS
       MMI_BEACON_DATA_T data;
} MMI_BEACON_SIG_T ;


extern void MMIBEACON_AppInit(void);
extern void MMIBEACON_RegWinIdNameArr(void);
extern void MMIBEACON_InitModule(void);
extern void MMIBEACON_RegMenuGroup(void);
extern void MMIBEACON_RegNv(void);
extern BOOLEAN  MMIBEACON_SendSigTo_APP(BEACON_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);

#ifdef __cplusplus
    }
#endif

#endif
