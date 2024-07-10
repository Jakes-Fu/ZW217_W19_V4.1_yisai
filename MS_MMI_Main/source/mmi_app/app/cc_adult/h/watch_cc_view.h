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

typedef struct _MMICC_CALLINFO_T
{
    uint8          tele_num[MMICC_PHONE_NUM_MAX_LEN];
    uint8          tele_len;
}MMICC_CALLINFO_T;

typedef struct _MMICC_DISPINFO_T
{
    MMI_IMAGE_ID_T      incall_portrait;
    wchar               disp_name_buf[MMICC_PHONE_NUM_MAX_LEN + 2];
    uint16              disp_name_len;
    uint8               tel_num_len; //bug 1366954
}MMICC_DISPINFO_T;

typedef struct _MMICC_WANT_MO_CALL_T
{
    BOOLEAN        is_want_mo_call;
    uint8          tele_num[MMICC_PHONE_NUM_MAX_LEN];
    uint8          tele_len;
}MMICC_WANT_MO_CALL_T;

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
PUBLIC void WatchCC_ConnectedWin_Enter(void);

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
//  Description : open set volume window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SetVolumeWin_Enter(void);

/*****************************************************************************/
//  Description : Close set volume window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SetVolumeWin_Exit(void);

/*****************************************************************************/
//  Description : call tip window
//  Parameter: [In] text_id, image_id
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_NoteWin_Enter(MMI_TEXT_ID_T text_id, MMI_IMAGE_ID_T image_id);

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

#endif
