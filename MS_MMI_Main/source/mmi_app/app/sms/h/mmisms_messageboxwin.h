/*************************************************************************
 ** File Name:      mmisms_receive.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about receiving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2012/07/12      liming.deng      Create.                              *
*************************************************************************/
#ifndef _MMISMS_MESSAGEBOXWIN_H_
#define _MMISMS_MESSAGEBOXWIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
//#include "mmisms_app.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
//#include "mmisms_order.h"
#include "guitab.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mail_export.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#include "mmiota_id.h"
#ifdef MMS_SUPPORT 
#include "mmimms_text.h"
#endif
#include "mmicc_export.h"
#include "guiurl.h"
#include "mmk_app.h"
#include "mmisms_commonui.h"
#include "mmipub.h"
#include "mmisms_order.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/


PUBLIC void MMISMS_EnterSmsBox(
    MMISMS_BOX_TYPE_E        box_type
);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
