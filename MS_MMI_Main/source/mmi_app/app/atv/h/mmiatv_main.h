/******************************************************************************
**  File Name:      mmiatv_main.h                                             *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_MAIN_H_
#define _MMIATV_MAIN_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmiatv_data.h"
#include "dal_recorder.h"
#include "mmibt_export.h"
#include "mmicom_panel.h"
#include "mmiatv_fsm.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIATV_SIGNAL_LEVEL_MAX     4   //ATV信号强度最大值
#define MMIATV_OSD_ALPHA                   230     //OSD透明度
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


//signal structure
typedef struct mtv_stimi_sig_to_mmi_tag
{
    SIGNAL_VARS                 //signal
    DRECORDER_REPORT_DATA_U  data;    //data
} ATV_CALLBACK_SIG_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : stop current action
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ExitATV(FSMSRV_HANDLE atv_handle);
/*****************************************************************************/
// 	Description : start search service
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_StartSearchService(FSMSRV_HANDLE atv_handle);

/*****************************************************************************/
// 	Description : stop search service
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_StopSearchService(FSMSRV_HANDLE atv_handle);


/*****************************************************************************/
// 	Description : get signal iamge_id for status bar
//	Global resource dependence :
//  Author: xin.li
//	Note: level 0-5
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIATV_GetTVStatusImageID(uint8 signal);

/*****************************************************************************/
// 	Description : get tip text id
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIATV_GetTipTextID(void);

/*****************************************************************************/
// 	Description : turn volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_TurnVolume(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIATV_RequestHandle(void);
            
/*****************************************************************************/
// 	Description : save current record file name
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIATV_SaveRecordFileName(void);     

/*****************************************************************************/
// 	Description : get current saved record file name
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC wchar* MMIATV_GetRecordFileName(void);
   
/*****************************************************************************/
// 	Description : save last played service id to nv
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SaveLastPlayServiceID(uint32 serviceId);

/*****************************************************************************/
// 	Description : get last played service id from nv
//	Global resource dependence : none
//  Author: xin.li
//	Note: if get success return TRUE, else return FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_GetLastPlayServiceID(uint32 *id_ptr);

/*****************************************************************************/
// 	Description : update current play index by the service id
//	Global resource dependence : none
//  Author: xin.li
//	Note: 通过service_id更新当前播放索引值，如初始进入播放、从收藏列表播放
/*****************************************************************************/
PUBLIC uint32 MMIATV_UpdateCurrentPlayIndexByServiceId(uint32 service_id);

/*****************************************************************************/
// 	Description : play previous service
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_PlayPreService(FSMSRV_HANDLE atv_handle ,
									uint32 *cur_index_ptr
									);
/*****************************************************************************/
// 	Description : play next service
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_PlayNextService(FSMSRV_HANDLE atv_handle,
									 uint32 *cur_index_ptr);

/*****************************************************************************/
// 	Description : exit atv
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ReleaseATV(FSMSRV_HANDLE atv_handle );

/*****************************************************************************/
// 	Description : enter atv, init data, setting, ref etc.
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_InitATV(FSMSRV_HANDLE atv_handle,
							  uint32 *service_id_ptr);

/*****************************************************************************/
// 	Description : add a new service node to service list
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_AddServiceNodeByID(uint32 cur_num, uint32 service_id);

/******************************************************************************/
// Description:analog tv start auto search channel dummy callback
// Author: aoke.hu
// Note:       
/******************************************************************************/ 
#ifdef WIN32
PUBLIC void MMIATV_WIN32SearchMsg(
                                  DRECORDER_PREVIEW_PARAM_T const *p_preview_param);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* _MMIATV_MAIN_H_ */
#endif /* ATV_SUPPORT */
