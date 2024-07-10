
/*****************************************************************************
** File Name:      mmivt_export.h                                                   *
** Author:                                                                   *
** Date:           12/7/2005                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/7/2005       bin.ji       Create
******************************************************************************/

#ifndef _MMIVT_EXPORT_H_
#define _MMIVT_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmifmm_export.h"//@zhaohui
#include "mmipb_export.h"

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


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIVT_RESULT_SUCCESS,  //ACTION执行成功
    MMIVT_RESULT_INVALID_ACTION,   //非法的ACTION
    MMIVT_RESULT_ERROR_STATE,  //ACTION执行时，状态错误
    MMIVT_RESULT_WRONG_APPLET, //UIDIAL和UIREMOTEINCOMING执行时，存在非法的applet
    MMIVT_RESULT_NULL_APPLET, //UI ACTION执行时，applet不存在
    MMIVT_RESULT_ERROR_PARM,   //非法参数
    MMIVT_RESULT_START_PS_FAILED, //电话呼出时启动ps失败
    MMIVT_RESULT_FAILED,   //ACTION执行失败，具体原因不明
    MMIVT_RESULT_MAX
        
}MMIVT_RESULT_E;


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : request a videomode call
//	Global resource dependence : 
//  Author: jibin
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_ReqVideoModeCall(MN_DUAL_SYS_E dual_sys, uint8 *tele_num, uint8 tele_len);

/*****************************************************************************/
//  Description : init msg handle func & local variable
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVT_Init(void);

/*****************************************************************************/
//  Description : reg VT MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVT_InitModule(void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtCalling
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:当前是否进行VT通信
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtCalling (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtConnecting
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否已经处于VT通话状态
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtConnecting (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtMoCalling
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否处于VT呼出中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtMoCalling (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtMtIncoming
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否处于VT来电中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtMtIncoming (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVTIncomingMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:是否是VT call的setup消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVTIncomingMsg (uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : MMIVT_GetCallDirect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_GetCallPbBCDNumber(MMIPB_BCD_NUMBER_T *pb_number_ptr);

/*****************************************************************************/
//  Description : MMIAPIVT_ReleaseVTCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_ReleaseVTCallByRedkey(void);

/*****************************************************************************/
//  Description : MMIAPIVT_SendUserInput
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_SendUserInput(uint32 msg_id);

/*****************************************************************************/
//  Description : MMIAPIVT_IsFallBackCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsFallBackCall (MN_CALLED_NUMBER_T num);

/*****************************************************************************/
//  Description : MMIAPIVT_ConnectCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_ConnectCall (void);
/*****************************************************************************/
//  Description : MMIAPIVT_GetCallName
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_GetCallName( MMI_STRING_T* out_name_str_ptr);

/*****************************************************************************/
//  Description : MMIAPIVT_SendMsgToApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 该接口目前用于接收耳机插拔消息
/*****************************************************************************/
PUBLIC void MMIAPIVT_SendMsgToApplet(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : MMIAPIVT_SetRecordAudioType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:工程模式：VT录制中录制混音项可以在工程模式里面修改 0:混音 1:远端音 2:本端音
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetRecordAudioType(uint16 record_audio_type);

/*****************************************************************************/
//  Description : MMIAPIVT_GetRecordAudioType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:工程模式：VT录制中录制混音项可以在工程模式里面修改 0:混音 1:远端音 2:本端音
/*****************************************************************************/
PUBLIC uint16 MMIAPIVT_GetRecordAudioType(void);

/*****************************************************************************/
//  Description : MMIAPIVT_ActiveVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 让VT applet获得焦点，返回值TRUE：代表VT applet存在，FALSE：表示applet不存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_ActiveVT(void);

/*****************************************************************************/
//  Description : MMIAPIVT_SetIsEPDefault
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 工程模式下设置默认声音模式是否为耳机
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetIsEPDefault(BOOLEAN is_default_earphone);

/*****************************************************************************/
//  Description : MMIAPIVT_GetIsEPDefault
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 工程模式下获得默认声音模式是否为耳机
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_GetIsEPDefault(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif



