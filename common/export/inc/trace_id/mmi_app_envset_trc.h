/******************************************************************************
 ** File Name:      mmi_app_envset_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:175
#ifndef _MMI_APP_ENVSET_TRC_H_
#define _MMI_APP_ENVSET_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIENVSET_2078_112_2_18_2_18_58_0 "MMIENVSET_EnvName_WriteInNV change edit len form %d to %d"
#define MMIENVSET_2118_112_2_18_2_18_58_1 "MMIENVSET_EnvName_WriteInNV nv_item_id = %d"
#define MMIENVSET_2223_112_2_18_2_18_58_2 "MMIENVSET_HandleDeleteItem nv_item_id = %d"
#define MMIENVSET_2290_112_2_18_2_18_58_3 "MMIENVSET_HandleAddItem nv_item_id = %d"
#define MMIENVSET_2585_112_2_18_2_18_59_4 "MMIAPIENVSET_GetPlayRingModeId mode_id=%d"
#define MMIENVSET_2612_112_2_18_2_18_59_5 "MMIAPIENVSET_GetMuteState s_mute_state=%d"
#define MMIENVSET_2624_112_2_18_2_18_59_6 "MMIAPIENVSET_SetMuteState s_mute_state=%d"
#define MMIENVSET_2651_112_2_18_2_18_59_7 "MMIAPIENVSET_GetModeOptValue result = %d"
#define MMIENVSET_2884_112_2_18_2_18_59_8 "GetEnvSetOptValue:set_opt is error!"
#define MMIENVSET_2892_112_2_18_2_18_59_9 "GetEnvSetOptValue return_value=%d mode_id=%d,set_opt=%d"
#define MMIENVSET_2925_112_2_18_2_19_0_10 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_2938_112_2_18_2_19_0_11 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_2950_112_2_18_2_19_0_12 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_2963_112_2_18_2_19_0_13 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_2975_112_2_18_2_19_0_14 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_2987_112_2_18_2_19_0_15 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_2999_112_2_18_2_19_0_16 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3010_112_2_18_2_19_0_17 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3020_112_2_18_2_19_0_18 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3032_112_2_18_2_19_0_19 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3045_112_2_18_2_19_0_20 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3056_112_2_18_2_19_0_21 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3069_112_2_18_2_19_0_22 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3081_112_2_18_2_19_0_23 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3093_112_2_18_2_19_0_24 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3105_112_2_18_2_19_0_25 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3117_112_2_18_2_19_0_26 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3129_112_2_18_2_19_0_27 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3141_112_2_18_2_19_0_28 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3152_112_2_18_2_19_0_29 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3164_112_2_18_2_19_0_30 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3176_112_2_18_2_19_0_31 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3188_112_2_18_2_19_0_32 "SetEnvSetOptValue fial set_opt =%d,set_value = %d"
#define MMIENVSET_3194_112_2_18_2_19_0_33 "SetEnvSetOptValue:set_opt is error!"
#define MMIENVSET_3200_112_2_18_2_19_0_34 "SetEnvSetOptValue nv_item_id = %d"
#define MMIENVSET_3249_112_2_18_2_19_0_35 "MMIAPIENVSET_GetActiveModeOptValue result=%d "
#define MMIENVSET_4059_112_2_18_2_19_2_36 "PlayCallRingByListItem:call_ring_type is error!"
#define MMIENVSET_4164_112_2_18_2_19_2_37 "MMIENVSET_PlayMsgAlarmRingByListItem:msg_ring_type is error!"
#define MMIENVSET_4218_112_2_18_2_19_2_38 "MMIENVSET_PlayRingByListItem:ring_type is error!"
#define MMIENVSET_4264_112_2_18_2_19_2_39 "MMIENVSET_VibraTimerOut: the ring_type is %d !"
#define MMIENVSET_4323_112_2_18_2_19_2_40 "MMIENVSET_ResetEnvSetSetting nv_item_id = %d"
#define MMIENVSET_4334_112_2_18_2_19_3_41 "MMIENVSET_ResetEnvSetSetting nv_item_id = %d"
#define MMIENVSET_4383_112_2_18_2_19_3_42 "MMIENVSET_StartWinOnetimeVibTimer %d"
#define MMIENVSET_4601_112_2_18_2_19_3_43 "MMIAPIENVSET_SetMoreCallRingInfo nv_item_id = %d"
#define MMIENVSET_4640_112_2_18_2_19_3_44 "MMIAPIENVSET_SetMoreMsgRingInfo nv_item_id = %d"
#define MMIENVSET_4671_112_2_18_2_19_3_45 "MMIAPIENVSET_SetFixedCallRingId nv_item_id = %d"
#define MMIENVSET_4702_112_2_18_2_19_3_46 "MMIAPIENVSET_SetFixedMsgRingId fail ring_id=%d"
#define MMIENVSET_4705_112_2_18_2_19_3_47 "MMIAPIENVSET_SetFixedMsgRingId nv_item_id = %d"
#define MMIENVSET_4726_112_2_18_2_19_3_48 "MMIAPIENVSET_GetModeValue cur_mode = %d"
#define MMIENVSET_4730_112_2_18_2_19_3_49 "MMIAPIENVSET_GetModeValue PNULL == mode_setting_ptr"
#define MMIENVSET_4737_112_2_18_2_19_3_50 "MMIAPIENVSET_GetModeValue nv_item_id = %d, return_value = %d"
#define MMIENVSET_4752_112_2_18_2_19_3_51 "MMIAPIENVSET_GetModeValue nv_item_id = %d, return_value = %d"
#define MMIENVSET_4785_112_2_18_2_19_3_52 "ReadMoreRing mod_id = %d, index = %d, dual_sys = %d, offset = %d"
#define MMIENVSET_4790_112_2_18_2_19_3_53 "ReadMoreRing readed_size = %d,offset = %d"
#define MMIENVSET_4795_112_2_18_2_19_3_54 "ReadMoreRing read file error"
#define MMIENVSET_4800_112_2_18_2_19_3_55 "ReadMoreRing set file pointer error offset = %d"
#define MMIENVSET_4806_112_2_18_2_19_3_56 "ReadMoreRing createfile error"
#define MMIENVSET_4834_112_2_18_2_19_4_57 "WriteMoreRing mod_id = %d, index = %d, dual_sys = %d, offset = %d"
#define MMIENVSET_4843_112_2_18_2_19_4_58 "WriteMoreRing readed_size = %d,offset = %d"
#define MMIENVSET_4848_112_2_18_2_19_4_59 "WriteMoreRing read file error"
#define MMIENVSET_4853_112_2_18_2_19_4_60 "WriteMoreRing set file pointer error offset = %d"
#define MMIENVSET_4859_112_2_18_2_19_4_61 "WriteMoreRing createfile error"
#define MMIENVSET_4938_112_2_18_2_19_4_62 "MMIENVSET_CreateMoreRingFile: create file  error"
#define MMIENVSET_4944_112_2_18_2_19_4_63 "MMIENVSET_CreateMoreRingFile: get device error"
#define MMIENVSET_4970_112_2_18_2_19_4_64 "MMIENVSET GetEnhanceRingState %d"
#define MMIENVSET_4990_112_2_18_2_19_4_65 "MMIAPIENVSET_GetEnhanceRingState %d"
#define MMIENVSET_5164_112_2_18_2_19_4_66 "MMIAPIENVSET_ResetActModeOtherRingSet it already reset mode_id = %d"
#define MMIENVSET_5199_112_2_18_2_19_4_67 "MMIAPIENVSET_SetModeValue cur_mode = %d"
#define MMIENVSET_5205_112_2_18_2_19_4_68 "MMIAPIENVSET_SetModeValue PNULL == mode_setting_ptr"
#define MMIENVSET_WINTAB_1208_112_2_18_2_19_7_69 " the menu id is error:HandleEnvMainOptMenuWindow"
#define MMIENVSET_WINTAB_1439_112_2_18_2_19_7_70 " the menu id is error:HandleEnvMainOptMenuWindow"
#define MMIENVSET_WINTAB_2219_112_2_18_2_19_9_71 "HandleRingVolumeWindow:menu_id is error\n!"
#define MMIENVSET_WINTAB_2263_112_2_18_2_19_9_72 "HandleAllRingVolWindow msg_id = %d"
#define MMIENVSET_WINTAB_2411_112_2_18_2_19_9_73 "HandleAllRingVolWindow s_cur_vol = %d "
#define MMIENVSET_WINTAB_2665_112_2_18_2_19_10_74 "mary HandleAllRingTypeWindow %d"
#define MMIENVSET_WINTAB_4117_112_2_18_2_19_13_75 "SetVolumeLabelcontent set_opt = %d,s_cur_vol = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ENVSET_TRC)
TRACE_MSG(MMIENVSET_2078_112_2_18_2_18_58_0,"MMIENVSET_EnvName_WriteInNV change edit len form %d to %d")
TRACE_MSG(MMIENVSET_2118_112_2_18_2_18_58_1,"MMIENVSET_EnvName_WriteInNV nv_item_id = %d")
TRACE_MSG(MMIENVSET_2223_112_2_18_2_18_58_2,"MMIENVSET_HandleDeleteItem nv_item_id = %d")
TRACE_MSG(MMIENVSET_2290_112_2_18_2_18_58_3,"MMIENVSET_HandleAddItem nv_item_id = %d")
TRACE_MSG(MMIENVSET_2585_112_2_18_2_18_59_4,"MMIAPIENVSET_GetPlayRingModeId mode_id=%d")
TRACE_MSG(MMIENVSET_2612_112_2_18_2_18_59_5,"MMIAPIENVSET_GetMuteState s_mute_state=%d")
TRACE_MSG(MMIENVSET_2624_112_2_18_2_18_59_6,"MMIAPIENVSET_SetMuteState s_mute_state=%d")
TRACE_MSG(MMIENVSET_2651_112_2_18_2_18_59_7,"MMIAPIENVSET_GetModeOptValue result = %d")
TRACE_MSG(MMIENVSET_2884_112_2_18_2_18_59_8,"GetEnvSetOptValue:set_opt is error!")
TRACE_MSG(MMIENVSET_2892_112_2_18_2_18_59_9,"GetEnvSetOptValue return_value=%d mode_id=%d,set_opt=%d")
TRACE_MSG(MMIENVSET_2925_112_2_18_2_19_0_10,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_2938_112_2_18_2_19_0_11,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_2950_112_2_18_2_19_0_12,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_2963_112_2_18_2_19_0_13,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_2975_112_2_18_2_19_0_14,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_2987_112_2_18_2_19_0_15,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_2999_112_2_18_2_19_0_16,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3010_112_2_18_2_19_0_17,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3020_112_2_18_2_19_0_18,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3032_112_2_18_2_19_0_19,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3045_112_2_18_2_19_0_20,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3056_112_2_18_2_19_0_21,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3069_112_2_18_2_19_0_22,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3081_112_2_18_2_19_0_23,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3093_112_2_18_2_19_0_24,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3105_112_2_18_2_19_0_25,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3117_112_2_18_2_19_0_26,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3129_112_2_18_2_19_0_27,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3141_112_2_18_2_19_0_28,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3152_112_2_18_2_19_0_29,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3164_112_2_18_2_19_0_30,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3176_112_2_18_2_19_0_31,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3188_112_2_18_2_19_0_32,"SetEnvSetOptValue fial set_opt =%d,set_value = %d")
TRACE_MSG(MMIENVSET_3194_112_2_18_2_19_0_33,"SetEnvSetOptValue:set_opt is error!")
TRACE_MSG(MMIENVSET_3200_112_2_18_2_19_0_34,"SetEnvSetOptValue nv_item_id = %d")
TRACE_MSG(MMIENVSET_3249_112_2_18_2_19_0_35,"MMIAPIENVSET_GetActiveModeOptValue result=%d ")
TRACE_MSG(MMIENVSET_4059_112_2_18_2_19_2_36,"PlayCallRingByListItem:call_ring_type is error!")
TRACE_MSG(MMIENVSET_4164_112_2_18_2_19_2_37,"MMIENVSET_PlayMsgAlarmRingByListItem:msg_ring_type is error!")
TRACE_MSG(MMIENVSET_4218_112_2_18_2_19_2_38,"MMIENVSET_PlayRingByListItem:ring_type is error!")
TRACE_MSG(MMIENVSET_4264_112_2_18_2_19_2_39,"MMIENVSET_VibraTimerOut: the ring_type is %d !")
TRACE_MSG(MMIENVSET_4323_112_2_18_2_19_2_40,"MMIENVSET_ResetEnvSetSetting nv_item_id = %d")
TRACE_MSG(MMIENVSET_4334_112_2_18_2_19_3_41,"MMIENVSET_ResetEnvSetSetting nv_item_id = %d")
TRACE_MSG(MMIENVSET_4383_112_2_18_2_19_3_42,"MMIENVSET_StartWinOnetimeVibTimer %d")
TRACE_MSG(MMIENVSET_4601_112_2_18_2_19_3_43,"MMIAPIENVSET_SetMoreCallRingInfo nv_item_id = %d")
TRACE_MSG(MMIENVSET_4640_112_2_18_2_19_3_44,"MMIAPIENVSET_SetMoreMsgRingInfo nv_item_id = %d")
TRACE_MSG(MMIENVSET_4671_112_2_18_2_19_3_45,"MMIAPIENVSET_SetFixedCallRingId nv_item_id = %d")
TRACE_MSG(MMIENVSET_4702_112_2_18_2_19_3_46,"MMIAPIENVSET_SetFixedMsgRingId fail ring_id=%d")
TRACE_MSG(MMIENVSET_4705_112_2_18_2_19_3_47,"MMIAPIENVSET_SetFixedMsgRingId nv_item_id = %d")
TRACE_MSG(MMIENVSET_4726_112_2_18_2_19_3_48,"MMIAPIENVSET_GetModeValue cur_mode = %d")
TRACE_MSG(MMIENVSET_4730_112_2_18_2_19_3_49,"MMIAPIENVSET_GetModeValue PNULL == mode_setting_ptr")
TRACE_MSG(MMIENVSET_4737_112_2_18_2_19_3_50,"MMIAPIENVSET_GetModeValue nv_item_id = %d, return_value = %d")
TRACE_MSG(MMIENVSET_4752_112_2_18_2_19_3_51,"MMIAPIENVSET_GetModeValue nv_item_id = %d, return_value = %d")
TRACE_MSG(MMIENVSET_4785_112_2_18_2_19_3_52,"ReadMoreRing mod_id = %d, index = %d, dual_sys = %d, offset = %d")
TRACE_MSG(MMIENVSET_4790_112_2_18_2_19_3_53,"ReadMoreRing readed_size = %d,offset = %d")
TRACE_MSG(MMIENVSET_4795_112_2_18_2_19_3_54,"ReadMoreRing read file error")
TRACE_MSG(MMIENVSET_4800_112_2_18_2_19_3_55,"ReadMoreRing set file pointer error offset = %d")
TRACE_MSG(MMIENVSET_4806_112_2_18_2_19_3_56,"ReadMoreRing createfile error")
TRACE_MSG(MMIENVSET_4834_112_2_18_2_19_4_57,"WriteMoreRing mod_id = %d, index = %d, dual_sys = %d, offset = %d")
TRACE_MSG(MMIENVSET_4843_112_2_18_2_19_4_58,"WriteMoreRing readed_size = %d,offset = %d")
TRACE_MSG(MMIENVSET_4848_112_2_18_2_19_4_59,"WriteMoreRing read file error")
TRACE_MSG(MMIENVSET_4853_112_2_18_2_19_4_60,"WriteMoreRing set file pointer error offset = %d")
TRACE_MSG(MMIENVSET_4859_112_2_18_2_19_4_61,"WriteMoreRing createfile error")
TRACE_MSG(MMIENVSET_4938_112_2_18_2_19_4_62,"MMIENVSET_CreateMoreRingFile: create file  error")
TRACE_MSG(MMIENVSET_4944_112_2_18_2_19_4_63,"MMIENVSET_CreateMoreRingFile: get device error")
TRACE_MSG(MMIENVSET_4970_112_2_18_2_19_4_64,"MMIENVSET GetEnhanceRingState %d")
TRACE_MSG(MMIENVSET_4990_112_2_18_2_19_4_65,"MMIAPIENVSET_GetEnhanceRingState %d")
TRACE_MSG(MMIENVSET_5164_112_2_18_2_19_4_66,"MMIAPIENVSET_ResetActModeOtherRingSet it already reset mode_id = %d")
TRACE_MSG(MMIENVSET_5199_112_2_18_2_19_4_67,"MMIAPIENVSET_SetModeValue cur_mode = %d")
TRACE_MSG(MMIENVSET_5205_112_2_18_2_19_4_68,"MMIAPIENVSET_SetModeValue PNULL == mode_setting_ptr")
TRACE_MSG(MMIENVSET_WINTAB_1208_112_2_18_2_19_7_69," the menu id is error:HandleEnvMainOptMenuWindow")
TRACE_MSG(MMIENVSET_WINTAB_1439_112_2_18_2_19_7_70," the menu id is error:HandleEnvMainOptMenuWindow")
TRACE_MSG(MMIENVSET_WINTAB_2219_112_2_18_2_19_9_71,"HandleRingVolumeWindow:menu_id is error\n!")
TRACE_MSG(MMIENVSET_WINTAB_2263_112_2_18_2_19_9_72,"HandleAllRingVolWindow msg_id = %d")
TRACE_MSG(MMIENVSET_WINTAB_2411_112_2_18_2_19_9_73,"HandleAllRingVolWindow s_cur_vol = %d ")
TRACE_MSG(MMIENVSET_WINTAB_2665_112_2_18_2_19_10_74,"mary HandleAllRingTypeWindow %d")
TRACE_MSG(MMIENVSET_WINTAB_4117_112_2_18_2_19_13_75,"SetVolumeLabelcontent set_opt = %d,s_cur_vol = %d")
END_TRACE_MAP(MMI_APP_ENVSET_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ENVSET_TRC_H_

