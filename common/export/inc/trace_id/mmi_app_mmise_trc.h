/******************************************************************************
 ** File Name:      mmi_app_mmise_trc.h                                         *
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
//trace_id:195
#ifndef _MMI_APP_MMISE_TRC_H_
#define _MMI_APP_MMISE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CRYSTAL_CUBE_815_112_2_18_2_37_23_0 "****** %d"
#define CRYSTAL_CUBE_821_112_2_18_2_37_23_1 "56789 time is %d"
#define CRYSTAL_CUBE_832_112_2_18_2_37_23_2 "===== %d"
#define CRYSTAL_CUBE_838_112_2_18_2_37_23_3 "1234 time is %d"
#define CRYSTAL_CUBE_857_112_2_18_2_37_23_4 "CCUBE: on_tpscroll_up,x=%d,y=%d"
#define CRYSTAL_CUBE_893_112_2_18_2_37_23_5 "CCUBE: on_tpscroll_down,x=%d,y=%d"
#define CRYSTAL_CUBE_929_112_2_18_2_37_23_6 "CCUBE: on_tpscroll_move,x=%d,y=%d"
#define CRYSTAL_CUBE_1034_112_2_18_2_37_23_7 "CCUBE:  fling_velocity=%f"
#define CRYSTAL_CUBE_1047_112_2_18_2_37_23_8 "CCUBE:  angle is %d"
#define CRYSTAL_CUBE_1060_112_2_18_2_37_24_9 "CCUBE: scroll start_angle is %d"
#define CRYSTAL_CUBE_1064_112_2_18_2_37_24_10 "CCUBE: scroll start_angle is equal 30 degree"
#define CRYSTAL_CUBE_1085_112_2_18_2_37_24_11 "CCUBE: scroll tp is down , stop immediately"
#define CRYSTAL_CUBE_1102_112_2_18_2_37_24_12 "CCUBE: scroll angle =%d, start anim"
#define CRYSTAL_CUBE_1121_112_2_18_2_37_24_13 "CCUBE: scroll anim frame=%d"
#define CRYSTAL_CUBE_1154_112_2_18_2_37_24_14 "CCUBE : not move angle=%d"
#define CRYSTAL_CUBE_1170_112_2_18_2_37_24_15 "CCUBE : angle is on position"
#define CRYSTAL_CUBE_1194_112_2_18_2_37_24_16 "CCUBE: 2 scroll anim frame=%d"
#define CRYSTAL_CUBE_1214_112_2_18_2_37_24_17 "CCUBE: scroll end: t_frm=%d,c_frm=%d,damp_angle_offset=%d,start_angle=%d,geometry_angle=%d"
#define CRYSTAL_CUBE_1228_112_2_18_2_37_24_18 "CCUBE: sendnotify MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND"
#define CRYSTAL_CUBE_1343_112_2_18_2_37_24_19 "CCUBE: crystalcube_calc_func type=%d"
#define CRYSTAL_CUBE_1463_112_2_18_2_37_24_20 "UNKNOWN calc type is %x"
#define CRYSTAL_CUBE_1541_112_2_18_2_37_25_21 "CCUBE: ComputeCurrentVelocity %d:  (%d,%d)"
#define CRYSTAL_CUBE_1609_112_2_18_2_37_25_22 "CCUBE: ComputeCurrentVelocity result=%f"
#define CYCLONE_723_112_2_18_2_37_27_23 "cyclone_calc_highlight_frame begin %d"
#define CYCLONE_744_112_2_18_2_37_27_24 "cyclone_calc_highlight_frame end %d , angle=%d"
#define CYCLONE_756_112_2_18_2_37_27_25 "calc_anim_frame begin %d"
#define CYCLONE_768_112_2_18_2_37_27_26 "calc_anim_frame end %d , angle=%d"
#define CYCLONE_779_112_2_18_2_37_27_27 "chang_res_by_angle start angle=%d"
#define CYCLONE_809_112_2_18_2_37_27_28 "chang_res_by_angle end i=%d , angle=%d"
#define CYCLONE_872_112_2_18_2_37_27_29 "cyclone_calc_rotate_frame begin %d"
#define CYCLONE_921_112_2_18_2_37_27_30 "cyclone_calc_rotate_frame end %d , angle=%d"
#define CYCLONE_935_112_2_18_2_37_27_31 "cyclone_calc_scroll_frame begin %d, angle=%d"
#define CYCLONE_943_112_2_18_2_37_28_32 "cyclone_calc_scroll_frame end %d , angle=%d"
#define CYCLONE_956_112_2_18_2_37_28_33 "cyclone_calc_scroll_anim_frame begin %d"
#define CYCLONE_1006_112_2_18_2_37_28_34 "cyclone_calc_scroll_anim_frame cur_catch_frame=%d catch_start_angle=%d anim_angle=%d"
#define CYCLONE_1014_112_2_18_2_37_28_35 "g_cyclone_scroll.inertia_angle=%d"
#define CYCLONE_1037_112_2_18_2_37_28_36 "cyclone_calc_scroll_anim_frame end %d , angle=%d"
#define CYCLONE_1050_112_2_18_2_37_28_37 "CYCLONE: crystalcube_calc_func type=%d"
#define MMISE_CALCULATION_99_112_2_18_2_37_28_38 "[CalulateTaskHandle] receive UNKNOWN Signal, Code is %x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MMISE_TRC)
TRACE_MSG(CRYSTAL_CUBE_815_112_2_18_2_37_23_0,"****** %d")
TRACE_MSG(CRYSTAL_CUBE_821_112_2_18_2_37_23_1,"56789 time is %d")
TRACE_MSG(CRYSTAL_CUBE_832_112_2_18_2_37_23_2,"===== %d")
TRACE_MSG(CRYSTAL_CUBE_838_112_2_18_2_37_23_3,"1234 time is %d")
TRACE_MSG(CRYSTAL_CUBE_857_112_2_18_2_37_23_4,"CCUBE: on_tpscroll_up,x=%d,y=%d")
TRACE_MSG(CRYSTAL_CUBE_893_112_2_18_2_37_23_5,"CCUBE: on_tpscroll_down,x=%d,y=%d")
TRACE_MSG(CRYSTAL_CUBE_929_112_2_18_2_37_23_6,"CCUBE: on_tpscroll_move,x=%d,y=%d")
TRACE_MSG(CRYSTAL_CUBE_1034_112_2_18_2_37_23_7,"CCUBE:  fling_velocity=%f")
TRACE_MSG(CRYSTAL_CUBE_1047_112_2_18_2_37_23_8,"CCUBE:  angle is %d")
TRACE_MSG(CRYSTAL_CUBE_1060_112_2_18_2_37_24_9,"CCUBE: scroll start_angle is %d")
TRACE_MSG(CRYSTAL_CUBE_1064_112_2_18_2_37_24_10,"CCUBE: scroll start_angle is equal 30 degree")
TRACE_MSG(CRYSTAL_CUBE_1085_112_2_18_2_37_24_11,"CCUBE: scroll tp is down , stop immediately")
TRACE_MSG(CRYSTAL_CUBE_1102_112_2_18_2_37_24_12,"CCUBE: scroll angle =%d, start anim")
TRACE_MSG(CRYSTAL_CUBE_1121_112_2_18_2_37_24_13,"CCUBE: scroll anim frame=%d")
TRACE_MSG(CRYSTAL_CUBE_1154_112_2_18_2_37_24_14,"CCUBE : not move angle=%d")
TRACE_MSG(CRYSTAL_CUBE_1170_112_2_18_2_37_24_15,"CCUBE : angle is on position")
TRACE_MSG(CRYSTAL_CUBE_1194_112_2_18_2_37_24_16,"CCUBE: 2 scroll anim frame=%d")
TRACE_MSG(CRYSTAL_CUBE_1214_112_2_18_2_37_24_17,"CCUBE: scroll end: t_frm=%d,c_frm=%d,damp_angle_offset=%d,start_angle=%d,geometry_angle=%d")
TRACE_MSG(CRYSTAL_CUBE_1228_112_2_18_2_37_24_18,"CCUBE: sendnotify MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND")
TRACE_MSG(CRYSTAL_CUBE_1343_112_2_18_2_37_24_19,"CCUBE: crystalcube_calc_func type=%d")
TRACE_MSG(CRYSTAL_CUBE_1463_112_2_18_2_37_24_20,"UNKNOWN calc type is %x")
TRACE_MSG(CRYSTAL_CUBE_1541_112_2_18_2_37_25_21,"CCUBE: ComputeCurrentVelocity %d:  (%d,%d)")
TRACE_MSG(CRYSTAL_CUBE_1609_112_2_18_2_37_25_22,"CCUBE: ComputeCurrentVelocity result=%f")
TRACE_MSG(CYCLONE_723_112_2_18_2_37_27_23,"cyclone_calc_highlight_frame begin %d")
TRACE_MSG(CYCLONE_744_112_2_18_2_37_27_24,"cyclone_calc_highlight_frame end %d , angle=%d")
TRACE_MSG(CYCLONE_756_112_2_18_2_37_27_25,"calc_anim_frame begin %d")
TRACE_MSG(CYCLONE_768_112_2_18_2_37_27_26,"calc_anim_frame end %d , angle=%d")
TRACE_MSG(CYCLONE_779_112_2_18_2_37_27_27,"chang_res_by_angle start angle=%d")
TRACE_MSG(CYCLONE_809_112_2_18_2_37_27_28,"chang_res_by_angle end i=%d , angle=%d")
TRACE_MSG(CYCLONE_872_112_2_18_2_37_27_29,"cyclone_calc_rotate_frame begin %d")
TRACE_MSG(CYCLONE_921_112_2_18_2_37_27_30,"cyclone_calc_rotate_frame end %d , angle=%d")
TRACE_MSG(CYCLONE_935_112_2_18_2_37_27_31,"cyclone_calc_scroll_frame begin %d, angle=%d")
TRACE_MSG(CYCLONE_943_112_2_18_2_37_28_32,"cyclone_calc_scroll_frame end %d , angle=%d")
TRACE_MSG(CYCLONE_956_112_2_18_2_37_28_33,"cyclone_calc_scroll_anim_frame begin %d")
TRACE_MSG(CYCLONE_1006_112_2_18_2_37_28_34,"cyclone_calc_scroll_anim_frame cur_catch_frame=%d catch_start_angle=%d anim_angle=%d")
TRACE_MSG(CYCLONE_1014_112_2_18_2_37_28_35,"g_cyclone_scroll.inertia_angle=%d")
TRACE_MSG(CYCLONE_1037_112_2_18_2_37_28_36,"cyclone_calc_scroll_anim_frame end %d , angle=%d")
TRACE_MSG(CYCLONE_1050_112_2_18_2_37_28_37,"CYCLONE: crystalcube_calc_func type=%d")
TRACE_MSG(MMISE_CALCULATION_99_112_2_18_2_37_28_38,"[CalulateTaskHandle] receive UNKNOWN Signal, Code is %x")
END_TRACE_MAP(MMI_APP_MMISE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MMISE_TRC_H_

