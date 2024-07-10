/*****************************************************************************
** File Name:      watch_notifycenter.h                                      *
** Author:         longwei.qiao                                              *
** Date:           12/01/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe notifycenter export interface*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2021       longwei.qiao         Create                                  *
******************************************************************************/
#ifndef _WATCH_NOTIFYCENTER_H_
#define _WATCH_NOTIFYCENTER_H_

#include "watch_notifycenter_shortcutwin.h"
#include "watch_notifycenter_text.h"
#include "watch_notifycenter_image.h"

#define NOTIFYCENTER_APPNAME_LEN        (30)
#define NOTIFYCENTER_TITLE_LEN          (30)
#define NOTIFYCENTER_TIME_LEN           (20)
#define NOTIFYCENTER_SHORTDESC_LEN      (60)
#define NOTIFYCENTER_DETAILDESC_LEN     (255)
typedef uint32  NOTIFY_HANDLE_T;

typedef struct {
    MMI_IMAGE_ID_T  notify_image;
    wchar    app_name_wstr[NOTIFYCENTER_APPNAME_LEN + 1];
    wchar    title_wstr[NOTIFYCENTER_TITLE_LEN + 1];
    wchar    time_wstr[NOTIFYCENTER_TIME_LEN + 1];
    wchar    short_desc_wstr[NOTIFYCENTER_SHORTDESC_LEN + 1];
    wchar    detail_desc_wstr[NOTIFYCENTER_DETAILDESC_LEN + 1];
    void    (*cb_NotifyRead)(NOTIFY_HANDLE_T notify_handle);
    void    (*cb_NotifyEnterApp)(NOTIFY_HANDLE_T notify_handle);
}NOTIFYCENTER_NOTIFY_INFO_T;

typedef struct _NOTIFY_NODE_T{
  NOTIFY_HANDLE_T               notify_handle;
  NOTIFYCENTER_NOTIFY_INFO_T    notify_info;
  struct _NOTIFY_NODE_T         *next;
}NOTIFY_NODE_T;

typedef struct _NOTIFY_LIST_T{
    uint32 total_num;
    NOTIFY_NODE_T *notify_head;//head notify node
    NOTIFY_NODE_T *notify_tail;//tail notify node
}NOTIFY_LIST_T;

typedef enum
{
    NOTIFYCENTER_SUCCESS,
    NOTIFYCENTER_INPUT_ERROR,
    NOTIFYCENTER_MALLOC_ERROR,
    NOTIFYCENTER_ERROR,
    NOTIFYCENTER_MAX
}NOTIFYCENTER_RESULT_E;

/**--------------------------------------------------------------------------*
**                          PUBLIC FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get notify handle by position from notify list
//  Parameter: [In] position: the position of node in list
//             [Out] None
//             [Return] NOTIFY_HANDLE_T:the handle of notify
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC NOTIFY_HANDLE_T NotifyCenter_GetHandleByPos ( uint32 position );

/*****************************************************************************/
//  Description : Get notify node by handle from notify list
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [Out] None
//             [Return] NOTIFY_NODE_T:notify node
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC NOTIFY_NODE_T * NotifyCenter_GetNode( NOTIFY_HANDLE_T notify_handle );

/*****************************************************************************/
//  Description : Add notify info into notify list
//  Parameter: [In] p_notify_info: notify info
//             [Out] None
//             [Return] NOTIFY_HANDLE_T:the handle of notify
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC uint32 NotifyCenter_GetTotalNum( void );

/*****************************************************************************/
//  Description : Add notify info into notify list
//  Parameter: [In] p_notify_info: notify info
//             [Out] None
//             [Return] NOTIFY_HANDLE_T:the handle of notify
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC NOTIFY_HANDLE_T NotifyCenter_Add(NOTIFYCENTER_NOTIFY_INFO_T *p_notify_info);

/*****************************************************************************/
//  Description : Read notify by notify handle
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:This API will call app cb_NotifyRead when notify read
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_Read(NOTIFY_HANDLE_T notify_handle);

/*****************************************************************************/
//  Description : Modify notify info by notify handle
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [In] p_notify_info: notify info
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:1. This API will call app cb_NotifyRead when notify read
//       2. This API will delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_Modify(NOTIFY_HANDLE_T notify_handle, NOTIFYCENTER_NOTIFY_INFO_T *p_notify_info);

/*****************************************************************************/
//  Description : Delete notify by handle from notify list
//  Parameter: [In] NOTIFY_HANDLE_T:the handle of notify
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:This API just delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_Delete(NOTIFY_HANDLE_T notify_handle);

/*****************************************************************************/
//  Description : Delete all notify node from notify list
//  Parameter: [In]  None
//             [Out] None
//             [Return] NOTIFYCENTER_RESULT_E: result
//  Author: tingting.zheng
//  Note:This API just delete notify node from notify list
/*****************************************************************************/
PUBLIC NOTIFYCENTER_RESULT_E NotifyCenter_DeleteAll( void );

/*****************************************************************************/
//  Description : Check wheather there is notify
//  Parameter: [In]  None
//             [Out] None
//             [Return] BOOLEAN:TRUE,FALSE
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN NotifyCenter_IsExist( void );

/*****************************************************************************/
//  Description : notifycenterlist win
//  Global resource dependence : none
//  Author:dandan.cao
//  Note:
/*****************************************************************************/

PUBLIC void NotifyCenter_EnterListWin(void);
/*****************************************************************************/
//  Description : notifycenterdetail win
//  Global resource dependence : none
//  Author:dandan.cao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_EnterDetailWin(NOTIFY_HANDLE_T notify_handle);

/*****************************************************************************/
//  Description : Init notifycenter module
//  Global resource dependence : none
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_NotifyCenter_InitModule(void);
#endif
