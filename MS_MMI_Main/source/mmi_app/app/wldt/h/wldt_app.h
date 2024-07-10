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
#ifndef  _MMI_WLDT_APP_H_    
#define  _MMI_WLDT_APP_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "wldt_common.h"
#include "wldt_db.h"
#include "wldt_id.h"
#include "wldt_menutable.h"
#include "wldt_nv.h"
#include "wldt_text.h"
#include "wldt_image.h"
#include "wldt_anim.h"


#ifdef WLDT_NET_SUPPORT
#include "wldt_net.h"

#ifdef WLDT_PLAT_CY_SUPPORT
#include "wldt_cy.h"
#endif
#endif

#define WLDTAPP_TRACE            WLDT_AppTrace

typedef struct
{
       uint8 *        str;
	uint32         len; 
} MMI_WLDT_DATA_T;

typedef struct
{
	SIGNAL_VARS
       MMI_WLDT_DATA_T data;
} MMI_WLDT_SIG_T ;

extern BOOLEAN is_wldt_jt_mode;

extern void MMIWLDT_AppInit(void);
extern void MMIWLDT_RegWinIdNameArr(void);
extern void MMIWLDT_InitModule(void);
extern void MMIWLDT_RegMenuGroup(void);
extern void MMIWLDT_RegNv(void);
extern BOOLEAN  MMIWLDT_SendSigTo_APP(WLDT_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);

#ifdef __cplusplus
    }
#endif

#endif
