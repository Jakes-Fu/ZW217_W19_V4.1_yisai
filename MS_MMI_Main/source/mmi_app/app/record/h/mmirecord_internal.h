/*************************************************************************
** File Name:      mmirecord_internal.h                                  *
** Author:         bin.ji                                                *
** Date:           3/2/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about record          *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 3/2/2005     bin.ji             Create.                              *
*************************************************************************/

#ifndef _MMIRECORD_INTERNAL_H_
#define _MMIRECORD_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "efs.h"
#include "mmi_nv.h"
#include "tb_dal.h"
#include "mmi_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "record.h"
#include "mmirecord_export.h"
//#include "mmimultim.h"
#include "mmidisplay_data.h"
#include "mmirecord_position.h"
//#include "mmimultim_text.h"
#include "mmifmm_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define _MMIRECORD_DEBUG

#define MMIRECORD_MAX_FILE_NAME_LEN          256

#define MMIRECORD_ALL                   "*.wav"
#define MMIRECORD_DEFAULT_SUFFIX_LEN    6

//#define MMIRECORD_DIR                   MMIMULTIM_RING_DEFAULT_DIR//"Record"
#define MMIRECORD_DEFAULT_DIR_LEN       15

//#define MMILEAVEWORD_DIR                MMIMULTIM_RING_DEFAULT_DIR//"Leav"
#define MMIRECORD_FILEEXT               ".wav"

#define MMIRECORD_MAX_NUM               200
#define MMIRECORD_FIND_MAX_NUM          200

#define MMILEAVEWORD_MAX_NUM            50//100

#define MMIRECORD_MIN_NEED_SPACE        20000//2000    //录音最小需要空间
#define MMIRECORD_UDISK_MIN_NEED_SPACE        100000    //录音最小需要空间

#define MMIRECORD_MEMSTR_MAX_LEN        50
#define MMIRECORD_DISPLAY_DATE_LEN      50
#define MMIRECORD_DISPLAY_TIME_LEN      50
#define MMIRECORD_DISPLAY_TYPE_LEN      50



#define MMIRECORD_TRANSPARENT_COLOR       8
#define MMIRECORD_STORAGE_MAX             2 //udisk, sd card


#define MMIRECORD_INVALID_FILEINDEX       0xFFFF

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 
//对应底层的结构RECORD_WRITE_ERR_SIG_T
typedef struct
{
    BOOLEAN is_header_err;   
}MMIRECORD_WRITE_ERR_SIG_T;

typedef struct 
{
    MMIFILE_DEVICE_E    dev;
    uint32              record_file_id;
    uint32              call_file_id;
    MMISRVAUD_RING_FMT_E  file_formattype;//andy.he_record
}MMIRECORD_SETTING_INFO_T;

typedef enum
{
    BUTTON_STATE_DISABLE,
    BUTTON_STATE_UNSELECTED,
    BUTTON_STATE_SELECTED,
    BUTTON_STATE_MAX
}MMIRECORD_BUTTON_STATE_E;

//录音音量的类型 
typedef enum
{
    MMIRECORD_NORMAL_VOL,    //普通录音
    MMIRECORD_INCALL_VOL,    //电话录音
    MMIRECORD_OTHER_VOL     //其他录音
}MMIRECORD_VOLUME_TYPE;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//      Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_DeleteTempFile(void);

/*****************************************************************************/
//  Description : 开始录音（通常情况）
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIRECORD_StartRecordNormal(BOOLEAN is_reRecord);

/*****************************************************************************/
//  Description : 暂停录音
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_PauseRecord(void);

/*****************************************************************************/
//  Description : 继续录音
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_ResumeRecord(void);

/*****************************************************************************/
//  Description : 播放录音（通常情况）
//  Global resource dependence :                                
//      Author: bin.ji
//  Note:
/*****************************************************************************/
MMIRECORD_RESULT_E MMIRECORD_PlayNormal(void);

/*****************************************************************************/
//  Description : 停止播放录音
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_StopPlay(void);

/*****************************************************************************/
//  Description : 继续播放录音
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_ResumePlay(void);

/*****************************************************************************/
//  Description : 暂停播放录音
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_PausePlay(void);

/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordStorage(MMIFILE_DEVICE_E storage);

/*****************************************************************************/
//  Description : display record
//  Global resource dependence : none
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateScreen(BOOLEAN is_full_paint);

/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note: only called by this module
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_PlayCurrent(void);

/*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_Exit(void);

/*****************************************************************************/
//  Description : Register record menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_RegMenuGroup(void);

/*****************************************************************************/
//  Description : set record status
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordStatus(MMIRECORD_STATE_E current_state);

/*****************************************************************************/
//  Description : clear current record file name
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_ClearCurrentRecordFileName(void);

/*****************************************************************************/
//  Description : is allowed to delete current record?
//  Global resource dependence : s_is_cur_file_allow_deleted
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_GetCurFileDeleteFlag(void);

/*****************************************************************************/
//  Description : update button status
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatus(void);

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Constructor(void);

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Destructor(void);

/*****************************************************************************/
//  Description : open record file
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_OpenRecordFileList(void);

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORD_GetRecordFileName(void);

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORD_GetRecordFilePath(void);

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SendFileViaBT(void);

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SendFileViaMMS(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void UpdateTotalRecordTime(void);


/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: andy.he
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordFileFormatType(MMISRVAUD_RING_FMT_E filetype);//andy.he_record 

/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none 
//  Author:andy.he
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIRECORD_GetRecordFileFormatType(void);//andy.he_record

/*****************************************************************************/
//  Description : get current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_GetPlayHandle(void);
 
/*****************************************************************************/
//  Description : get current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_GetRecordHandle(void);
 
/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_SetPlayHandle(MMISRV_HANDLE_T handle);
 
/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_SetRecordHandle(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_StopRecord(void);
#endif
