/*************************************************************************
** File Name:      watch_record_api.h                                                                        *
** Author:         jiaxin.sun                                                                                        *
** Date:           3/16/2020                                                                                       *
** Description:     record api                                                                                     *
*************************************************************************
*************************************************************************/

#ifndef _WATCH_RECORD_API_H_
#define _WATCH_RECORD_API_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmirecord_export.h"
#include "mmicom_panel.h"
#include "mmi_filemgr.h"
#include "mmisrvrecord_export.h"
#include "mmifilearray_export.h"

#define RECORD_BACKGROUND_RIGHT DP2PX_VALUE(240)
#define RECORD_BACKGROUND_BOTTOM DP2PX_VALUE(240)

typedef enum
    {
        MMIVOICE_RECORD_STATE_NORMAL,
        MMIVOICE_RECORD_STATE_RECORDING,
        MMIVOICE_RECORD_STATE_PAUSE,
        MMIVOICE_RECORD_STATE_LIST,
        MMIVOICE_RECORD_STATE_MAX
    } MMIVOICE_RECORD_STATE_E;

typedef enum
    {
        WATCH_RECORD_PLAY_STATE_NORMAL,
        WATCH_RECORD_PLAY_STATE_PLAYING,
        WATCH_RECORD_PLAY_STATE_PAUSE,
        WATCH_RECORD_PLAY_STATE_DELETE_QUERY,
        WATCH_RECORD_PLAY_STATE_DELETING,
        WATCH_RECORD_PLAY_STATE_MAX
    } WATCH_RECORD_PLAY_STATE_E;

 /*****************************************************************************/
//  Description : open option window
//  Para : none
//  return : none
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenOptList(void);
 /*****************************************************************************/
//  Description : open record file window
//  Para : none
//  return : none
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenFileList(void);
  /*****************************************************************************/
//  Description : open record play window
//  Para : none
//  return : none
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenPlayWin(void);
  /*****************************************************************************/
//  Description : get file list current index
//  Para : none
//  return : current index
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC uint32 WATCHRECORD_GetFileListCurIndex(void);
 /*****************************************************************************/
//  Description : get file list total number
//  Para : none
//  return : totle number
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC uint32 WATCHRECORD_GetFileListNum(void);

/*****************************************************************************/
//  Description : open wating window before record list
//  Para : none
//  return : none
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenWaitingWin(void);

/*****************************************************************************/
//  Description : get file list file array pointer
//  Para : none
//  return : file array pointer
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY WATCHRECORD_GetFileArray(void);

/*****************************************************************************/
//  Description : Stop Play
//  [int]:none
//  [out]:none
//  [return]:none
//  Author:Jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_StopPlay(void);
/*****************************************************************************/
//  Description : Stop Record
//  [in]:isExit -TRUE:exit record window
//                     FALSE:no exit
//  Author:Jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_StopRecord(BOOLEAN isExit);

/*****************************************************************************/
//  Description : Stop Record Or Play
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_StopRecordOrPlay(void);

/*****************************************************************************/
//  Description : WATCHRECORD_GetPlayStatus
//  [int]:none
//  [out]:none
//  [return]:none
//  Author:Jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC WATCH_RECORD_PLAY_STATE_E WATCHRECORD_GetPlayStatus(void);
PUBLIC void WATCHRECORD_ClockStart(void);
/*****************************************************************************/
//  Description : WATCHRECORD_GetCurPlayName
//  [int]:none
//  [out]:filename
//  [return]:none
//  Author:Jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHRECORD_GetCurPlayName(wchar *name);
/*****************************************************************************/
//  Description : WATCHRECORD_OpenPlayWinEx
//  [int]:filename
//  [out]:none
//  [return]:none
//  Author:Jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenPlayWinEx(wchar *name);
#endif
