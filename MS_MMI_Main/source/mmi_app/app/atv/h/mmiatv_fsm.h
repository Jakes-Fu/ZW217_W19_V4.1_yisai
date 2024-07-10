/*****************************************************************************
** File Name:      mmiatv_fsm.h                                                   *
** Author:                                                                   *
** Date:           2010/3/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIATV_FSM_H_
#define _MMIATV_FSM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmiatv_export.h"
#include "mmisrvaud_api.h"
#include "dal_recorder.h"
#include "mmisrv_fsm.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
//状态机接收的消息
typedef enum
{     
		ATV_OPEN_REQ = FSMSRV_USER_REQ,
		ATV_CLOSE_REQ, 
        ATV_PLAY_REQ,   		
        ATV_SEARCH_REQ,
		ATV_STOP_SEARCH_REQ,
		ATV_SEARCH_CNF,
		ATV_RECORD_REQ,
		ATV_STOP_RECORD_REQ,
		ATV_RECORD_END,
        ATV_STOP_REQ, 
		ATV_CHN_CNF,
		ATV_RELEASE_REQ,	
        ATV_REQ_MSG_MAX
} ATV_FSM_MSG_E; 

//状态机信息结构
typedef struct  
{    
    MMISRV_HANDLE_T             srv_handle;             //audio service handle 
	uint32						search_cur_num;
	uint32                      search_total_num;
	uint32						search_valid_num;
	uint32						service_id; 
	uint8						signal_level;
	BOOLEAN                     is_close;
}ATV_FSM_T;

typedef struct  
{
	uint32 search_cur_num;
	uint32 search_total_num;
	uint32 chn_id;
	uint32 chn_is_valid;
	uint32 search_valid_num;
}ATV_CHN_INFO_T;

/*****************************************************************************/
//  Description : MMIATV_IsRecording
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsRecording(void); 

/*****************************************************************************/
//  Description : MMIATV_IsPlaying
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsPlaying(void); 

/*****************************************************************************/
//  Description : MMIATV_IsSearching
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsSearching(void);

/*****************************************************************************/
//  Description : MMIATV_IsSettingCHN
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsSettingCHN(void);

/*****************************************************************************/
// 	Description : get atv signal level 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
/*****************************************************************************/
PUBLIC uint8 MMIATV_GetTVSignalLevel(void);   

/*****************************************************************************/
// 	Description : get search current num
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetSearchCurrentNum(void);

/*****************************************************************************/
// 	Description : get search total num
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetSearchTotalNum(void);
/*****************************************************************************/
// 	Description : handle app msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIATV_HandleSysMsg(
                                       PWND     app_ptr,    //pointer of application
                                       uint16   msg_id,     //the identify of message
                                       DPARAM   param       //the param of the message
                                       );

/*****************************************************************************/
//  Description : 终止视频播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMClose(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMRelease(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : MMIATV_FSMStopRecord
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMStopRecord(FSMSRV_HANDLE   atv_handle,
									BOOLEAN is_stop_now);

/*****************************************************************************/
//  Description : MMIATV_FSMRecord
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMRecord(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : MMIATV_FSMStop
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMStop(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMStopSearch(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : MMIATV_FSMSearch
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMSearch(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : 发起视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_FSMPlay(FSMSRV_HANDLE   atv_handle, uint32 service_id );

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIATV_FSMOpen(FSMSRV_HANDLE   atv_handle);

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC  FSMSRV_HANDLE  MMIATV_FSMCreate(void);

/*****************************************************************************/
// 	Description : get current play service id
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetCurrentServiceID(void);

/*****************************************************************************/
// 	Description : set current play service id
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetCurrentServiceID(uint32 service_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_GetAlign8Offset(
                                       uint32 *width_ptr, //[IN/OUT]
                                       uint32 *height_ptr, //[IN/OUT]
                                       uint32 *x_offset_ptr, //[IN/OUT]
                                       uint32 *y_offset_ptr //[IN/OUT]                                                         
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: yanxianzhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_GetAlign4Offset(
 									  uint32 *width_ptr, //[IN/OUT]
									  uint32 *height_ptr, //[IN/OUT]
									  uint32 *x_offset_ptr, //[IN/OUT]
									  uint32 *y_offset_ptr //[IN/OUT]                                                         
									  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T  MMIATV_GetAudioHandle(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
