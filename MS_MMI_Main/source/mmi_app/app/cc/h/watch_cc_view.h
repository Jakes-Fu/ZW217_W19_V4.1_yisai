/*****************************************************************************
** File Name:      watch_cc_view.h                                           *
** Author:         yuming.yang                                               *
** Date:           02/12/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw cc module interface             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 02/2020               yuming.yang            Create                       *
******************************************************************************/

#ifndef _WATCH_CC_VIEW_H_
#define _WATCH_CC_VIEW_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmicc_internal.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/

typedef struct _MMICC_DISPINFO_T
{
    MMI_IMAGE_ID_T      incall_portrait;
    wchar               disp_name_buf[MMICC_PHONE_NUM_MAX_LEN + 2];
    uint16              disp_name_len;
    uint8               tel_num_len; //bug 1366954
}MMICC_DISPINFO_T;


/*****************************************************************************/
//  Description : Open mo call query window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallQueryWin_Enter(void* param);

/*****************************************************************************/
//  Description : Close mo call query window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallQueryWin_Exit(void);

/*****************************************************************************/
//  Description : Open mo calling window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallingWin_Enter(void* param);

/*****************************************************************************/
//  Description : Close mo calling window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallingWin_Exit(void);

/*****************************************************************************/
//  Description : Open mo or mt connected window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_ConnectedWin_Enter(void* param);

/*****************************************************************************/
//  Description : Close mo or mt connected window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_ConnectedWin_Exit(void);

/*****************************************************************************/
//  Description : Open mt incoming window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MtCallingWin_Enter(void* param);

/*****************************************************************************/
//  Description : Close mt incoming window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MtCallingWin_Exit(void);

/*****************************************************************************/
//  Description : open call ended tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_CallEnded_NoteWin_Enter(void);

/*****************************************************************************/
//  Description : record saved tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_RecordSaved_NoteWin_Enter(void);

/*****************************************************************************/
//  Description : open call ended and record saved tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_EndedAndSaved_NoteWin_Enter(void);

/*****************************************************************************/
//  Description : open sms send and call reject tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SentAndRejected_NoteWin_Enter(void);

/*****************************************************************************/
//  Description : mo call error tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallError_NoteWin_Enter(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : memory is full tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MemoryFull_NoteWin_Enter(void);

/*****************************************************************************/
//  Description : Reject Mt call By Sms
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_RejectMtBySms(void);

/*****************************************************************************/
//  Description : make call api for watch
//  Parameter: [In] call info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchAPICC_MakeCall(MN_DUAL_SYS_E dual_sys,
                                       uint8 *tele_num,
                                       uint8  tele_len,
                                       MMI_STRING_T* disp_ptr,
                                       MMI_STRING_T* name_ptr,
                                       CC_CALL_SIM_TYPE_E call_sim_type,
                                       uint32 call_type,
                                       MN_SUBADDR_T* sub_addr
                                       );

/*****************************************************************************/
//  Description : send dtmf query window
//  Parameter: [In] dtmf_info
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SendDtmf_QueryWin_Enter(uint8* p_dtmf_str,uint8 dtmf_len);

/*****************************************************************************/
//  Description : sending dtmf tip window
//  Parameter: [In] dtmf_info
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SendDtmf_NoteWin_Enter(uint8* p_dtmf_str,uint8 dtmf_len);

/*****************************************************************************/
//  Description :  thermal close business note win
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]NONE
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_Thermal_Win_Enter(void);

#endif
