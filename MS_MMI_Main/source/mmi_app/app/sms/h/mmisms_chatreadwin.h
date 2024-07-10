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
#ifndef _MMISMS_CHATREADWIN_H_
#define _MMISMS_CHATREADWIN_H_

#ifdef MMI_SMS_CHAT_SUPPORT

//#define MMI_BLACKLIST_SUPPORT
//#define MMI_SMS_MOVE_SUPPORT
//#define MMI_SMS_COPY_SUPPORT
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
#include "mmi_textfun.h"
//#include "mmienvset.h"

#include "mn_type.h"
#include "mmk_app.h"
#include "mmi_position.h"
#include "mmicom_string.h"
#include "guictrl_api.h"

#include "guianim.h"
#include "guiedit.h"
#include "mmi_custom_define.h"
#include "gui_ucs2b_converter.h"
#include "guires.h"
#include "mmicom_extract.h"
#include "guictrl_api.h"
#include "mmimms_export.h"
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif
#include "mmisms_app.h"
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
#ifdef MMIMMS_SMS_IN_1_SUPPORT
typedef struct
{
    MMISMS_CHAT_INSERT_TYPE insert_type;
    MMIFMM_FILE_TYPE_E   file_type;
    MMIFILE_FILE_INFO_T *file_info_ptr;
} MMISMS_CHAT_INSERT_GLOBLE_T;
#endif


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : get chat content , delete time
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ChatWinGetItemContent(MMI_STRING_T *content);

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_CHAT_INSERT_TYPE MMISMS_GetChatEditInsertOpType(void);

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatEditInsertOpType(MMISMS_CHAT_INSERT_TYPE insert_type);
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : to get phone number
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetPhoneNumber(uint8 *number_ptr, uint16 src_len, wchar *focus_num_ptr, uint16 dest_len);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
