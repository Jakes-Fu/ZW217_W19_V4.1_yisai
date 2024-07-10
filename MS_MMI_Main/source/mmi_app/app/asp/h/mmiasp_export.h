/****************************************************************************
** File Name:      mmiasp_export.h                                          *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe fuction of ANSWER PHONE    *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/
#ifndef _MMIASP_API_H
#define _MMIASP_API_H
#include "mmiasp_cortrol_layer.h"


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIASP_SetRecordRingState   MMIAPIASP_SetRecordRingState
#define MMIASP_GetRecordRingState   MMIAPIASP_GetRecordRingState

#endif //MMI_FUNC_COMPATIBLE_SUPPORT


/*****************************************************************************/
//  Description :  init asp mudule
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_InitModule(void);

/*****************************************************************************/
//  Description : create wait timer
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIASP_CreateWaitTimer(void);

/*****************************************************************************/
//  Description : stop wati timer
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopWaitTimer(void);

/*****************************************************************************/
//  Description :  stop call answer when call finished
//  Global resource dependence :none
//  Author:
//  Note: 该函数可能在答录时用户接听了电话时调用，而MMIAPIASP_RealseCall则是挂断整个电话时调用
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopCallAnswer(void);

/*****************************************************************************/
//  Description : 进入答录机
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 检查密码保护
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_EnterAsp(void);

/*****************************************************************************/
//  Description : whether asp is turn on
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsMachineTurnOn(void);

/*****************************************************************************/
//  Description : 答录机是否在工作状态
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsMachineWorking(void);

/*****************************************************************************/
//  Description : reset factory setting
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : CLOSE 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_CloseRecordingWin(void);

/*****************************************************************************/
//  Description : open 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_OpenRecordingWin(void);

/*****************************************************************************/
//  Description : 获取固定铃音名称
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIASP_GetFixedRingNameByID(uint8 id);

/*****************************************************************************/
//  Description : 答录提示语播放完毕后处理
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_HandlePlayAudioEnd(void);

/*****************************************************************************/
//  Description :  此函数专供RealseCall时调用
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 只在挂断整个电话时调用
/*****************************************************************************/
PUBLIC void MMIAPIASP_RealseCall(void);

/*****************************************************************************/
//  Description :  电话是否被主人接听
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 此函数供cc挂断电话时判断用户是否接听电话调用
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsCallSpeakedByOwner(void);

/*****************************************************************************/
//  Description : 选择答录语
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_SelectAnswerRing(PRINGSELECT_CALLBACKFUNC callback_fun_ptr);

/*****************************************************************************/
//  Description : 答录机在状态栏需要显示的图标
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  MMI_IMAGE_ID_T MMIAPIASP_GetIdleImageID(void);

/*****************************************************************************/
//  Description : 是否正在播答录留言
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsPlayingMusic(void);

/*****************************************************************************/
//  Description : 当前是否处在答录电话，且在接听状态
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_lsAspCallSpeaking(void);

/*****************************************************************************/
//  Description : open 播放界面的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   此函数专供从外部直接进入到播放界面(不是从留言列表界面进入的）
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_OpenPlayWinFromExternal(uint16 node_index);

/*****************************************************************************/
//  Description : 获取新的留言条目数..
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC uint32 MMIAPIASP_GetNewRecordNum(void);

/*****************************************************************************/
//  Description : 获取最新的新留言
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetLastestUnreadRecord(
                                                MMIASP_MSG_INFO_T* msg_info_ptr, // out
                                                uint16*            node_index_ptr// out
                                                );
/*****************************************************************************/
//  Description : 自动播放答录机留言
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 检查密码保护
/*****************************************************************************/
PUBLIC void MMIAPIASP_AutoPlayRecordMessages(void);

/*****************************************************************************/
//  Description : post message to the win recording when plug out sd card
//  Global resource dependence : 
//  Author: koki.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_SDCardPlugOut(void);

/*****************************************************************************/
//  Description : 设置答录语为录制答录状态
//  Global resource dependence : none
//  Author: koki.gao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_SetRecordRingState(BOOLEAN  is_record_ring);

/*****************************************************************************/
//  Description :获取 设置答录语是否为录制答录状态
//  Global resource dependence : none
//  Author: koki.gao
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIASP_GetRecordRingState(void);

/*****************************************************************************/
//  Description : 通话中立即开启答录
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_WorkOneTime(void);

/*****************************************************************************/
//  Description : 是否已到答录等待时间
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_IsWaitTimeout(DPARAM param);

/*****************************************************************************/
//  Description : 开始答录
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIASP_StartAnswerCall(void);

/*****************************************************************************/
//  Description : 进入答录机
//  Global resource dependence : none
//  Author: kun.liu
//  Note: 不检查密码保护
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_EnterIntoAsp(void);

/*****************************************************************************/
//  Description : 自动播放答录留言
//  Global resource dependence : none
//  Author: kun.liu
//  Note: 不检查密码保护
/*****************************************************************************/
PUBLIC void MMIAPIASP_UnLockAutoPlayRecordMessages(void);

/*****************************************************************************/
//  Description : open 播放界面的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   此函数专供从外部直接进入到播放界面(不是从留言列表界面进入的）
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_UnLockOpenPlayWinFromExternal(uint16 node_index);

/*****************************************************************************/
//  Description : 设置通话答录时间到的状态
//  Global resource dependence :none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_SetAspRecordTimeOutState(BOOLEAN  asp_record_state);

/*****************************************************************************/
//  Description : 获得通话答录时间到的状态
//  Global resource dependence :none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetAspRecordTimeOutState(void);

/*****************************************************************************/
//  Description :  来第二个电话保持并应答时关掉答录
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIASP_StopAspRecording(void);

/*****************************************************************************/
//   Description : 通过号码获取名字
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetNameByNumber(
                                      uint8*          call_num_ptr,//in
                                      MMI_STRING_T*   name_ptr //out
                                      );

#endif

