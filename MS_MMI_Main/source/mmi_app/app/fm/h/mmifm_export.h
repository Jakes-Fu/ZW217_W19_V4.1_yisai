/*****************************************************************************
** File Name:      mmifm.h                                                   *
** Author:         baokun.yin                                               *
** Date:           2008-12-16                                                 *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is header of fm                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008-12-16      Liqing Peng      Creat
******************************************************************************/

#ifndef _MMI_FM_H_
#define _MMI_FM_H_

/**--------------------------------------------------------------------------*
 **                         Include files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef WIN32
#include "mmifm_win32_simu.h"
#else
#include "fm_drv.h"
#endif


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         CONSTANT DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define MMIFM_DEFAULT_VOL                   FM_VOL_LEVEL_3
#define MMIFM_AUTOSEARCH_TIMER              1500

//FM search frequency result
typedef enum
{
    MMIFM_AUTOSEARCH_FAIL = 0,
    MMIFM_AUTOSEARCH_CONTINUE,              // get one valid FM station, go next
    MMIFM_AUTOSEARCH_END,
    MMIFM_AUTOSEARCH_NOT_COMPLETE           // has not get valid station yet
}MMIFM_AUTOSEARCH_RESULT_E;
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
#ifdef FM_RDS_SUPPORT
typedef enum
{
    FM_RDS_STATUS_OPEN,
    FM_RDS_STATUS_CLOSE
}MMIFM_RDS_STATUS_E;
#endif

//FM app status
typedef enum
{
    FM_PLAYING,
    FM_SUSPENDED,
    FM_STOP
}MMIFM_STATUS_E;


//FM's sensitivity
typedef enum
{
    FM_SENSITI_LOW,
    FM_SENSITI_MEDIUM,
    FM_SENSITI_HIGH
}MMIFM_SENSI_E;

typedef struct  
{
    MMIFM_STATUS_E              status;         //current status
    uint16                      cur_freq;       //current freq
}MMIENGFM_CUR_INFO_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open play FM main window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_OpenMainWin(void);

/*****************************************************************************/
//  Description : init FM application
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_Init(void);

/*****************************************************************************/
//  Description : Close FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Stop(void);

/*****************************************************************************/
//  Description : Pause FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Pause(BOOLEAN suspend_by_fm);

/*****************************************************************************/
//  Description : Continue FM play, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_Resume(BOOLEAN resume_by_fm);

/*****************************************************************************/
//  Description : get FM's status, play, stop or suspended
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_STATUS_E MMIAPIFM_GetStatus(void);

/*****************************************************************************/
//  Description : register fm module nv len and max item
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_RegFMNv(void);

/*****************************************************************************/
//  Description : intial FM module  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_InitModule(void);


/*****************************************************************************/
//  Description : play FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_EngTestPlay(void);

/*****************************************************************************/
//  Description : stop FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_EngTestStop(void);

/*****************************************************************************/
//  Description : set FM sound volume
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_SetVolume(uint16 vol);

/*****************************************************************************/
//  Description : handle headset plug in and plug out action
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_HeadsetInd(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : return freq name of FM playing now
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_GetCurFreqName(MMI_STRING_T *str_ptr, uint16 max_name_len);

/*****************************************************************************/
//  Description : Close FM play and exit fm windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Exit(void);

/*****************************************************************************/
//     Description : get sum size of all fm nv itme
//    Global resource dependence : 
//  Author: jian.ma
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFM_GetSumNvSize(void);

/*****************************************************************************/
//  Description : adjust FM sound volume
//  Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_AdjustVolume(int32 step);

/*****************************************************************************/
//  Description : set fm digital type
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_SetFmDigitalType(void);
#if 0
/*****************************************************************************/
//  Description : Play FM
//  Global resource dependence : 
//  Author:
//  Note: Do not used any more,because audio module be updated.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_PlayInputFreq(uint16 input_freq,uint32 volume);
#endif
/*****************************************************************************/
//  Description : Create Alarm FM Channel List
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_CreateAlarmChannelList(ADD_DATA param);
/*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMRECORD_Exit(void);
/*****************************************************************************/
// 	Description : auto search is open
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsOpened(void);
/*****************************************************************************/
//  Description : Stop FM Record for CC
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopRecord(void);
/*****************************************************************************/
//  Description : Stop FM Auto Search
//  Global resource dependence : 
//  Author:langhua.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopAutoSearch(void);
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Start FM timer record 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_StartFMTimerRecord(void);
#endif
/*****************************************************************************/
//  Description : Create FM Timer Record Info Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFM_CreateFMTimerRecordInfoWin(void);
/*****************************************************************************/
//  Description : reset timer fm info
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMAPIIFM_Reset(void);
/*****************************************************************************/
//  Description : API Play FM
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_Play(MMISRV_HANDLE_T audio_handle,uint16 input_freq);

/*****************************************************************************/
//  Description : 判断fm是否active,供外部使用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmActive(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsFmRecordActive(void);
/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIAPIFM_GetChannelName(uint16 fre);
/*****************************************************************************/
//  Description : get fm audio current device mode
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
//PUBLIC AUDIO_DEVICE_MODE_TYPE_E MMIAPIFM_GetFMCurrentDeviceMode(void);
/*****************************************************************************/
//  Description : preview fm frequency status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsPreviewFM(void);

PUBLIC void MMIAPIFM_AutosearchChannel(void);
/*****************************************************************************/
//  Description : Get FM play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIFM_GetPlayHandle(void);

/*****************************************************************************/
//  Description : Check Headset Status
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_CheckHeadsetStatusWithoutPrompt(void);
/*****************************************************************************/
//  Description : Adjust FM channel
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_AdjustChannel(int32 step);
/*****************************************************************************/
//  Description : Auot search one FM channel
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_SearchValidFreq(int32 direct);

/*****************************************************************************/
//  Description : Get current FM channel infomation
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIENGFM_CUR_INFO_T* MMIAPIFM_GetFMCurrentInfo(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMI_FM_H_
