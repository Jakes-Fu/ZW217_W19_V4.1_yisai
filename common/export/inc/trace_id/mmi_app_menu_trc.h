/******************************************************************************
 ** File Name:      mmi_app_menu_trc.h                                         *
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
//trace_id:192
#ifndef _MMI_APP_MENU_TRC_H_
#define _MMI_APP_MENU_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MAINMENU_WIN_1130_112_2_18_2_32_50_0 "Create MAINMENU_MOVEOPTMENU_WIN_TAB!"
#define MAINMENU_WIN_1135_112_2_18_2_32_50_1 "Create MAINMENU_OPTMENU_WIN_TAB!"
#define MAINMENU_WIN_3134_112_2_18_2_32_54_2 "HandleMainSecondMenuListWinMsg, msg_id = 0x%02x"
#define MAINMENU_WIN_3356_112_2_18_2_32_54_3 "HandleMainMenuOptWinMsg msg_id = %d"
#define MAINMENU_WIN_3422_112_2_18_2_32_55_4 "HandleMainMenuMoveOptWinMsg msg_id = %d"
#define MMIMENU_CRYSTALCUBE_1165_112_2_18_2_33_0_5 "GetCubePageMenuNextItemIndex: PNULL!"
#define MMIMENU_CRYSTALCUBE_2288_112_2_18_2_33_2_6 "HandleCubePageMenuCancelKey not move state!"
#define MMIMENU_CRYSTALCUBE_2599_112_2_18_2_33_3_7 "CCUBE: HandleCubePageTpDown cubepage_state =%d,x=%d,y=%d"
#define MMIMENU_CRYSTALCUBE_2659_112_2_18_2_33_3_8 "CCUBE: HandleCubePageTpMove cubepage_state =%d,x=%d,y=%d"
#define MMIMENU_CRYSTALCUBE_2741_112_2_18_2_33_3_9 "CCUBE: HandleCubePageTpUp cubepage_state =%d,x=%d,y=%d"
#define MMIMENU_CRYSTALCUBE_2822_112_2_18_2_33_3_10 "CCUBE: MSG_CTL_OPEN"
#define MMIMENU_CRYSTALCUBE_2885_112_2_18_2_33_3_11 "CCUBE: MSG_CTL_GET_FOCUS"
#define MMIMENU_CRYSTALCUBE_2965_112_2_18_2_33_3_12 "CCUBE: MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND"
#define MMIMENU_CRYSTALCUBE_3003_112_2_18_2_33_4_13 "CCUBE: MMICRYSTALCUBE_MSG_DISPLAY_IND"
#define MMIMENU_CRYSTALCUBE_3047_112_2_18_2_33_4_14 "CCUBE: MMICRYSTALCUBE_MSG_ANIM_FRAME_IND"
#define MMIMENU_CRYSTALCUBE_3081_112_2_18_2_33_4_15 "CCUBE: MMICRYSTALCUBE_MSG_ROTATE_END_IND"
#define MMIMENU_CRYSTALCUBE_3088_112_2_18_2_33_4_16 "CCUBE: MMICRYSTALCUBE_MSG_ANIM_END_IND"
#define MMIMENU_CRYSTALCUBE_3095_112_2_18_2_33_4_17 "CCUBE: MMICRYSTALCUBE_MSG_PARAMINIT_CNF"
#define MMIMENU_CRYSTALCUBE_3122_112_2_18_2_33_4_18 "CCUBE: MMICRYSTALCUBE_MSG_PARAM_RESET_CNF"
#define MMIMENU_CRYSTALCUBE_3147_112_2_18_2_33_4_19 "CCUBE: MMICUBEPAGE_MSG_TP_CMD_CNF"
#define MMIMENU_CRYSTALCUBE_3166_112_2_18_2_33_4_20 "CCUBE: MMICUBEPAGE_MSG_TP_SCROOL_FRAME_IND"
#define MMIMENU_CRYSTALCUBE_3201_112_2_18_2_33_4_21 "CCUBE: MMICRYSTALCUBE_MSG_SRCOLLANIM_IND"
#define MMIMENU_CRYSTALCUBE_3241_112_2_18_2_33_4_22 "CCUBE: MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND"
#define MMIMENU_CRYSTALCUBE_3248_112_2_18_2_33_4_23 "CCUBE: MMICUBEPAGE_MSG_SRCOLLANIM_END_IND,%d,%d"
#define MMIMENU_CRYSTALCUBE_3277_112_2_18_2_33_4_24 "CCUBE: MMICRYSTALCUBE_MSG_PARAMRELEASE_CNF"
#define MMIMENU_CRYSTALCUBE_3293_112_2_18_2_33_4_25 "CCUBE: MSG_CTL_LOSE_FOCUS"
#define MMIMENU_CRYSTALCUBE_3316_112_2_18_2_33_4_26 "CCUBE: MSG_CTL_CLOSE"
#define MMIMENU_CUBE_1222_112_2_18_2_33_7_27 "== CubeMenuRotate == layer is not active"
#define MMIMENU_CUBE_1369_112_2_18_2_33_8_28 "== CubeMenuEnter == layer is not active"
#define MMIMENU_CUBE_1774_112_2_18_2_33_8_29 "HandleCubeMenuEnterAnim not support type! %d"
#define MMIMENU_CYCLONE_1383_112_2_18_2_33_12_30 "CalculateAngleAndMenuItem  velocity_x =%i"
#define MMIMENU_CYCLONE_1402_112_2_18_2_33_12_31 "CalculateAngleAndMenuItem  velocity_y =%i"
#define MMIMENU_CYCLONE_1501_112_2_18_2_33_12_32 "CalculateAngleAndMenuItem  scroll_anim_start_angle =%d,catch_angle=%d,inertia_angle=%d"
#define MMIMENU_CYCLONE_1502_112_2_18_2_33_12_33 "CalculateAngleAndMenuItem  inertia_end_angle =%d,catch_stop_angle=%d,cur_item_index=%d"
#define MMIMENU_CYCLONE_1503_112_2_18_2_33_12_34 "CalculateAngleAndMenuItem  item_index_backup_in_scroll_anim =%d"
#define MMIMENU_CYCLONE_1703_112_2_18_2_33_13_35 "HandleCycloneMenuCancelKey not move state!"
#define MMIMENU_CYCLONE_1823_112_2_18_2_33_13_36 "HandleCycloneTpDown cyclone_state =%d,x=%d,y=%d"
#define MMIMENU_CYCLONE_1880_112_2_18_2_33_13_37 "HandleCycloneTpMove cyclone_state =%d,x=%d,y=%d tick=%d"
#define MMIMENU_CYCLONE_1913_112_2_18_2_33_13_38 "HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,x=%d,start_x=%d"
#define MMIMENU_CYCLONE_1914_112_2_18_2_33_13_39 "HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,y=%d,start_y=%d"
#define MMIMENU_CYCLONE_1934_112_2_18_2_33_13_40 "HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,x=%d,start_x=%d"
#define MMIMENU_CYCLONE_1935_112_2_18_2_33_13_41 "HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,y=%d,start_y=%d"
#define MMIMENU_CYCLONE_1954_112_2_18_2_33_13_42 "HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,x=%d,start_x=%d"
#define MMIMENU_CYCLONE_1955_112_2_18_2_33_13_43 "HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,y=%d,start_y=%d"
#define MMIMENU_CYCLONE_1972_112_2_18_2_33_13_44 "HandleCycloneTpUp cyclone_state =%d,x=%d,y=%d"
#define MMIMENU_CYCLONE_2046_112_2_18_2_33_13_45 "CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_OPEN"
#define MMIMENU_CYCLONE_2086_112_2_18_2_33_13_46 "CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_GET_FOCUS"
#define MMIMENU_CYCLONE_2121_112_2_18_2_33_13_47 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_ANIM_FRAME_IND,state=%d"
#define MMIMENU_CYCLONE_2139_112_2_18_2_33_13_48 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_ANIM_END_IND"
#define MMIMENU_CYCLONE_2150_112_2_18_2_33_13_49 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_ROTATE_FRAME_IND"
#define MMIMENU_CYCLONE_2163_112_2_18_2_33_13_50 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND"
#define MMIMENU_CYCLONE_2184_112_2_18_2_33_13_51 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_TP_SCROOL_IND"
#define MMIMENU_CYCLONE_2188_112_2_18_2_33_14_52 "MMICYCLONE_MSG_TP_SCROOL_IND %d"
#define MMIMENU_CYCLONE_2208_112_2_18_2_33_14_53 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_SRCOLLANIM_IND"
#define MMIMENU_CYCLONE_2211_112_2_18_2_33_14_54 "MMICYCLONE_MSG_SRCOLLANIM_IND %d"
#define MMIMENU_CYCLONE_2229_112_2_18_2_33_14_55 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_PARAMINIT_CNF"
#define MMIMENU_CYCLONE_2256_112_2_18_2_33_14_56 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_PARAM_RESET_CNF"
#define MMIMENU_CYCLONE_2281_112_2_18_2_33_14_57 "CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_PARAMRELEASE_CNF"
#define MMIMENU_CYCLONE_2294_112_2_18_2_33_14_58 "CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_LOSE_FOCUS"
#define MMIMENU_CYCLONE_2311_112_2_18_2_33_14_59 "CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_CLOSE"
#define MMIMENU_GO_SE_454_112_2_18_2_33_16_60 "DataTypeAdapt   data_type = %d"
#define MMIMENU_GO_SE_596_112_2_18_2_33_16_61 "MMIMENU_Go_Draw_Unfold_Page     ERROR   cur_frame_num=%d"
#define MMIMENU_GO_SE_1259_112_2_18_2_33_17_62 "== calcfuncSphere == Input param cur_page_info error"
#define MMIMENU_GO_SE_1265_112_2_18_2_33_17_63 "== calcfuncSphere == Input param next_page_info error"
#define MMIMENU_GO_SE_1271_112_2_18_2_33_17_64 "== calcfuncSphere == Input param bg_window_buf error"
#define MMIMENU_GO_SE_1277_112_2_18_2_33_17_65 "== calcfuncSphere == Input param cur_frame_num=%d & total_frame_num=%d error"
#define MMIMENU_GO_SE_1283_112_2_18_2_33_17_66 "== calcfuncSphere == Input param total_frame_num=%d error"
#define MMIMENU_GO_SE_1289_112_2_18_2_33_17_67 "== calcfuncSphere == Input param direction=%d error"
#define MMIMENU_GO_SE_1295_112_2_18_2_33_17_68 "== calcfuncSphere == Input param bg_window_size.w=%d error"
#define MMIMENU_GO_SE_1301_112_2_18_2_33_17_69 "== calcfuncSphere == Input param bg_window_size.h=%d error"
#define MMIMENU_GO_SE_1307_112_2_18_2_33_17_70 "== calcfuncSphere == Input param cur_page_info->total_col_num=%d error"
#define MMIMENU_GO_SE_1313_112_2_18_2_33_17_71 "== calcfuncSphere == Input param cur_page_info->total_row_num=%d error"
#define MMIMENU_GO_SE_1321_112_2_18_2_33_17_72 "== calcfuncSphere == Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_1330_112_2_18_2_33_17_73 "== calcfuncSphere == Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_1355_112_2_18_2_33_17_74 "== calcfuncSphere == SE_Create error"
#define MMIMENU_GO_SE_1364_112_2_18_2_33_17_75 "== calcfuncSphere == alloca mem error"
#define MMIMENU_GO_SE_1377_112_2_18_2_33_17_76 "== calcfuncSphere == SE_Render to get param error"
#define MMIMENU_GO_SE_1392_112_2_18_2_33_17_77 "== calcfuncSphere == Rotation count total_frame_num error"
#define MMIMENU_GO_SE_1405_112_2_18_2_33_17_78 "== calcfuncSphere == Rotation count total_frame_num error"
#define MMIMENU_GO_SE_1423_112_2_18_2_33_17_79 "== calcfuncSphere == target layer error"
#define MMIMENU_GO_SE_1454_112_2_18_2_33_18_80 "== SlideMenuMoveSphere == SE_Render error"
#define MMIMENU_GO_SE_1507_112_2_18_2_33_18_81 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info error"
#define MMIMENU_GO_SE_1512_112_2_18_2_33_18_82 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param next_page_info error"
#define MMIMENU_GO_SE_1517_112_2_18_2_33_18_83 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_buf error"
#define MMIMENU_GO_SE_1522_112_2_18_2_33_18_84 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error"
#define MMIMENU_GO_SE_1527_112_2_18_2_33_18_85 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param total_frame_num=%d error"
#define MMIMENU_GO_SE_1532_112_2_18_2_33_18_86 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param direction=%d error"
#define MMIMENU_GO_SE_1537_112_2_18_2_33_18_87 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_size.w=%d error"
#define MMIMENU_GO_SE_1542_112_2_18_2_33_18_88 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_size.h=%d error"
#define MMIMENU_GO_SE_1547_112_2_18_2_33_18_89 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->total_col_num=%d error"
#define MMIMENU_GO_SE_1552_112_2_18_2_33_18_90 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->total_row_num=%d error"
#define MMIMENU_GO_SE_1559_112_2_18_2_33_18_91 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_1567_112_2_18_2_33_18_92 "MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_1798_112_2_18_2_33_18_93 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info error"
#define MMIMENU_GO_SE_1803_112_2_18_2_33_18_94 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param next_page_info error"
#define MMIMENU_GO_SE_1808_112_2_18_2_33_18_95 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_buf error"
#define MMIMENU_GO_SE_1813_112_2_18_2_33_18_96 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error"
#define MMIMENU_GO_SE_1818_112_2_18_2_33_18_97 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param total_frame_num=%d error"
#define MMIMENU_GO_SE_1823_112_2_18_2_33_18_98 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param direction=%d error"
#define MMIMENU_GO_SE_1828_112_2_18_2_33_18_99 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_size.w=%d error"
#define MMIMENU_GO_SE_1833_112_2_18_2_33_18_100 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_size.h=%d error"
#define MMIMENU_GO_SE_1838_112_2_18_2_33_18_101 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->total_col_num=%d error"
#define MMIMENU_GO_SE_1843_112_2_18_2_33_18_102 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->total_row_num=%d error"
#define MMIMENU_GO_SE_1850_112_2_18_2_33_18_103 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_1858_112_2_18_2_33_18_104 "MMIMENU_Go_Effect_Pegtop_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_2048_112_2_18_2_33_19_105 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info error"
#define MMIMENU_GO_SE_2053_112_2_18_2_33_19_106 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param next_page_info error"
#define MMIMENU_GO_SE_2058_112_2_18_2_33_19_107 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_buf error"
#define MMIMENU_GO_SE_2063_112_2_18_2_33_19_108 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error"
#define MMIMENU_GO_SE_2068_112_2_18_2_33_19_109 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param total_frame_num=%d error"
#define MMIMENU_GO_SE_2073_112_2_18_2_33_19_110 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param direction=%d error"
#define MMIMENU_GO_SE_2078_112_2_18_2_33_19_111 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_size.w=%d error"
#define MMIMENU_GO_SE_2083_112_2_18_2_33_19_112 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_size.h=%d error"
#define MMIMENU_GO_SE_2088_112_2_18_2_33_19_113 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->total_col_num=%d error"
#define MMIMENU_GO_SE_2093_112_2_18_2_33_19_114 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->total_row_num=%d error"
#define MMIMENU_GO_SE_2100_112_2_18_2_33_19_115 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_GO_SE_2108_112_2_18_2_33_19_116 "MMIMENU_Go_Effect_Turntable_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
#define MMIMENU_ICON_1910_112_2_18_2_33_30_117 "ReadIconMenuItemNum error!"
#define MMIMENU_ICON_2506_112_2_18_2_33_31_118 "InitIconMenu enter!"
#define MMIMENU_ICON_2589_112_2_18_2_33_31_119 "InitIconMenu out!"
#define MMIMENU_ICON_2804_112_2_18_2_33_31_120 "== DisplayIconMenuScrollGroove == prgbox ctrl is PNULL"
#define MMIMENU_ICON_2962_112_2_18_2_33_32_121 "ConfirmMoveMenuItem not move state!"
#define MMIMENU_ICON_3175_112_2_18_2_33_32_122 "IconMenuModifyRect Error! style = %d"
#define MMIMENU_ICON_3330_112_2_18_2_33_32_123 "HandleIconMenuEnterAnim not support type! %d"
#define MMIMENU_ICON_4715_112_2_18_2_33_35_124 "HandleIconMenuCancelKey not move state!"
#define MMIMENU_ICON_5233_112_2_18_2_33_36_125 "HandleIconMenuOk msg_id = %x"
#define MMIMENU_ICON_5986_112_2_18_2_33_38_126 "HandleIconMenuMove %d, %d!"
#define MMIMENU_ICON_6238_112_2_18_2_33_38_127 "== HandleSecondMenuTimer == timer id %d is error"
#define MMIMENU_ICON_6912_112_2_18_2_33_40_128 "HandleSecMenuTpDown %d, %d"
#define MMIMENU_ICON_7132_112_2_18_2_33_40_129 "AdjuestIconMenuOffsetY 1!"
#define MMIMENU_ICON_7153_112_2_18_2_33_40_130 "AdjuestIconMenuOffsetY 2!"
#define MMIMENU_ICON_7194_112_2_18_2_33_40_131 "HandleIconTpUp %d"
#define MMIMENU_ICON_7210_112_2_18_2_33_40_132 "HandleSecMenuTpUp %f"
#define MMIMENU_ICON_7668_112_2_18_2_33_41_133 "Do nothing"
#define MMIMENU_ICON_8290_112_2_18_2_33_42_134 "RenderRollEffect() SE_Render fail! ret = %d"
#define MMIMENU_ISTYLE_2095_112_2_18_2_33_48_135 "[GUIMENU:DrawPageLayer]Error handle %d"
#define MMIMENU_ISTYLE_2286_112_2_18_2_33_48_136 "mmimenu_istyle: MovePageLayer: Can not move right in search page"
#define MMIMENU_ISTYLE_6201_112_2_18_2_33_56_137 "HandleTimer: the move_type is error!"
#define MMIMENU_ISTYLE_6281_112_2_18_2_33_56_138 "HandleIStyleMenuCancelKey not move state!"
#define MMIMENU_ISTYLE_6348_112_2_18_2_33_57_139 "HandleIStyleMenuAutoMove:continue_move_type %d is error!"
#define MMIMENU_ISTYLE_6424_112_2_18_2_33_57_140 "AutoSlidePage Error! move type = %d"
#define MMIMENU_ISTYLE_7425_112_2_18_2_33_59_141 "HandleIStyleMenuMove!"
#define MMIMENU_ISTYLE_10473_112_2_18_2_34_7_142 "HandleIStyleMenuMove!"
#define MMIMENU_ISTYLE_12142_112_2_18_2_34_10_143 "mmimenu_istyle: CreateSearchPageLayer page_layer_handle[1] block id is %d"
#define MMIMENU_ISTYLE_12186_112_2_18_2_34_10_144 "mmimenu_istyle: BltSearchPage offset_x is %d"
#define MMIMENU_ISTYLE_12209_112_2_18_2_34_10_145 "mmimenu_istyle: ReleaseSearchPage menu_ctrl_ptr is null"
#define MMIMENU_ISTYLE_12273_112_2_18_2_34_10_146 "mmimenu_istyle: IsListResponseTpMSGInSearchPage form ptr is null"
#define MMIMENU_ISTYLE_12285_112_2_18_2_34_10_147 "mmimenu_istyle: IsListResponseTpMSGInSearchPage child form ptr is null"
#define MMIMENU_ISTYLE_12445_112_2_18_2_34_11_148 "mmimenu_istyle.c: SearchPageHookFunc msg_id is %d"
#define MMIMENU_ISTYLE_12449_112_2_18_2_34_11_149 "mmimenu_istyle.c: SearchPageHookFunc menu_ctrl_ptr is null"
#define MMIMENU_ISTYLE_12545_112_2_18_2_34_11_150 "mmimenu_istyle: SearchPageHookFunc: move point x cross slide_start_point"
#define MMIMENU_ISTYLE_12677_112_2_18_2_34_11_151 "mmimenu_istyle: SearchPageHookFunc tp up not found any valid ctrl to handle this msg"
#define MMIMENU_OPTIONPAGE_1122_112_2_18_2_34_14_152 "HandleOptionMenuOk msg_id = %x"
#define MMIMENU_OPTIONPAGE_1170_112_2_18_2_34_14_153 "HandleOptMenuCancelKey not move state!"
#define MMIMENU_OPTIONPAGE_1480_112_2_18_2_34_15_154 "GetOptionMenuNextItemIndex: PNULL!"
#define MMIMENU_OPTIONPAGE_2625_112_2_18_2_34_17_155 "OptMenuModifyRect Error! style = %d"
#define MMIMENU_OPTIONPAGE_2768_112_2_18_2_34_17_156 "== DisplayOptionMenuScrollGroove == prgbox ctrl is PNULL"
#define MMIMENU_OPTIONPAGE_3378_112_2_18_2_34_19_157 "HandleOptMenuTimer: the move_type is error!"
#define MMIMENU_OPTIONPAGE_3428_112_2_18_2_34_19_158 "HandleOptMenuCancelKey not move state!"
#define MMIMENU_OPTIONPAGE_3485_112_2_18_2_34_19_159 "HandleOptMenuAutoMove:continue_move_type %d is error!"
#define MMIMENU_OPTIONPAGE_4793_112_2_18_2_34_22_160 "HandleOptionPageTpUp result = %d, is_in_menu_ctrl = %d"
#define MMIMENU_POPUP_1211_112_2_18_2_34_27_161 "current item is in menu!"
#define MMIMENU_POPUP_1284_112_2_18_2_34_28_162 "PopmenuHandleTpUp %f"
#define MMIMENU_POPUP_1441_112_2_18_2_34_28_163 "current item is in menu!"
#define MMIMENU_POPUP_1833_112_2_18_2_34_29_164 "current item is in menu!"
#define MMIMENU_POPUP_2377_112_2_18_2_34_30_165 "ResetPopmenuDisplayItem 1 Error!"
#define MMIMENU_POPUP_2398_112_2_18_2_34_30_166 "ResetPopmenuDisplayItem 2 Error!"
#define MMIMENU_POPUP_4040_112_2_18_2_34_33_167 "== CalculateContextMenuRect == create button failed"
#define MMIMENU_QBTHEME_1846_112_2_18_2_34_40_168 "[GUIMENU:DrawPageLayer]Error handle %d"
#define MMIMENU_QBTHEME_4247_112_2_18_2_34_45_169 "[GO_MENU]   DisplaySlidePageMenu "
#define MMIMENU_QBTHEME_5587_112_2_18_2_34_48_170 "HandleTimer: the move_type is error!"
#define MMIMENU_QBTHEME_5707_112_2_18_2_34_48_171 "HandleQbthemeMenuCancelKey not move state!"
#define MMIMENU_QBTHEME_5770_112_2_18_2_34_48_172 "HandleQbthemeMenuAutoMove:continue_move_type %d is error!"
#define MMIMENU_QBTHEME_5835_112_2_18_2_34_48_173 "AutoSlidePage Error! move type = %d"
#define MMIMENU_QBTHEME_6655_112_2_18_2_34_50_174 "HandleIStyleMenuMove!"
#define MMIMENU_QBTHEME_7249_112_2_18_2_34_51_175 "[GO_MENU]HandleSlidePageTpUp current time  = %d "
#define MMIMENU_QBTHEME_8258_112_2_18_2_34_53_176 "HandleQbthemeSetWinMsg  msg_id = %x"
#define MMIMENU_SECOND_1025_112_2_18_2_34_58_177 "== MMIMENU_DisplayScrollGroove == prgbox ctrl is PNULL"
#define MMIMENU_SECOND_1232_112_2_18_2_34_58_178 "ResetSecMenuDisplayItem 1 Error!"
#define MMIMENU_SECOND_1253_112_2_18_2_34_58_179 "ResetSecMenuDisplayItem 2 Error!"
#define MMIMENU_SECOND_1488_112_2_18_2_34_59_180 "HandleSecMenuTpDown %d, %d"
#define MMIMENU_SECOND_1536_112_2_18_2_34_59_181 "current item is in menu!"
#define MMIMENU_SECOND_1657_112_2_18_2_34_59_182 "current item is in menu!"
#define MMIMENU_SECOND_1718_112_2_18_2_34_59_183 "HandleSecMenuTpUp %d"
#define MMIMENU_SECOND_1733_112_2_18_2_34_59_184 "HandleSecMenuTpUp %f"
#define MMIMENU_SECOND_1927_112_2_18_2_35_0_185 "current item is in menu!"
#define MMIMENU_SECOND_3943_112_2_18_2_35_4_186 "== HandleSecondMenuTimer == timer id %d is error"
#define MMIMENU_SECOND_4018_112_2_18_2_35_4_187 "HandleIconMenuOk msg_id = %x"
#define MMIMENU_SLIDE_PAGE_1695_112_2_18_2_35_8_188 "[GUIMENU:DrawSlideMenuPageLayer]Error handle %d"
#define MMIMENU_SLIDE_PAGE_2693_112_2_18_2_35_10_189 "[GUIMENU:DrawSlideMenuPageLayer]Error handle %d"
#define MMIMENU_SLIDE_PAGE_2983_112_2_18_2_35_10_190 "== SlideMenuMoveSphere == need not draw"
#define MMIMENU_SLIDE_PAGE_2992_112_2_18_2_35_10_191 "== SlideMenuMoveSphere == get layer 0 error"
#define MMIMENU_SLIDE_PAGE_3019_112_2_18_2_35_10_192 "== SlideMenuMoveSphere == SE_Create error"
#define MMIMENU_SLIDE_PAGE_3027_112_2_18_2_35_10_193 "== SlideMenuMoveSphere == alloca mem error"
#define MMIMENU_SLIDE_PAGE_3039_112_2_18_2_35_10_194 "== SlideMenuMoveSphere == SE_Render to get param error"
#define MMIMENU_SLIDE_PAGE_3076_112_2_18_2_35_10_195 "== SlideMenuMoveSphere == target layer error"
#define MMIMENU_SLIDE_PAGE_3117_112_2_18_2_35_11_196 "== SlideMenuMoveSphere == SE_Render error"
#define MMIMENU_SLIDE_PAGE_3721_112_2_18_2_35_12_197 "HandleSlidePageMenuOk msg_id = %x"
#define MMIMENU_SLIDE_PAGE_3805_112_2_18_2_35_12_198 "ConfirmSlidePageMenuItem not move state!"
#define MMIMENU_SLIDE_PAGE_4260_112_2_18_2_35_13_199 "GetSlideMenuNextItemIndex: PNULL!"
#define MMIMENU_SLIDE_PAGE_5014_112_2_18_2_35_14_200 "ReadSlideMenuPageItemInfo Read Nv Error! page_num = %d, item count = %d"
#define MMIMENU_SLIDE_PAGE_5795_112_2_18_2_35_16_201 "SlideMenuModifyRect Error! style = %d"
#define MMIMENU_SLIDE_PAGE_6691_112_2_18_2_35_18_202 "HandleSlideMenuEnterAnim not support type! %d"
#define MMIMENU_SLIDE_PAGE_7399_112_2_18_2_35_20_203 "HandleSlidePageMenuTimer: the move_type is error!"
#define MMIMENU_SLIDE_PAGE_7551_112_2_18_2_35_20_204 "HandleSlidePageMenuCancelKey not move state!"
#define MMIMENU_SLIDE_PAGE_7618_112_2_18_2_35_20_205 "HandleSlidePageMenuAutoMove:continue_move_type %d is error!"
#define MMIMENU_SLIDE_PAGE_7789_112_2_18_2_35_20_206 "AutoSlidePage Error! move type = %d"
#define MMIMENU_SLIDE_PAGE_9267_112_2_18_2_35_24_207 "HandleSlidePageMenuMove!"
#define MMIMENU_SLIDE_PAGE_10856_112_2_18_2_35_27_208 "HandleSlidePageTpUp result = %d, is_in_menu_ctrl = %d"
#define MMIMENU_SLIDE_PAGE_GO_1042_112_2_18_2_35_31_209 "slide menu not support DisplayActiveIconItem, error!"
#define MMIMENU_SLIDE_PAGE_GO_1052_112_2_18_2_35_31_210 "slide menu not support DisplayEnterMenuAnim, error!"
#define MMIMENU_SLIDE_PAGE_GO_1063_112_2_18_2_35_31_211 "slide menu not support DisplaySwitchMenuAnim, error!"
#define MMIMENU_SLIDE_PAGE_GO_1338_112_2_18_2_35_32_212 "Do nothing"
#define MMIMENU_SLIDE_PAGE_GO_2316_112_2_18_2_35_34_213 "s_go_item_page_info handle block_id== PNULL"
#define MMIMENU_SLIDE_PAGE_GO_2373_112_2_18_2_35_34_214 "[GO_MENU]MMIMENU_DrawGOMenuLayer current time  = %d "
#define MMIMENU_SLIDE_PAGE_GO_2534_112_2_18_2_35_34_215 "[GO_MENU]MMIMENU_GO_Effect_calc calcfunc cost time = %d         STYLE = %d      frame = %d"
#define MMIMENU_SLIDE_PAGE_GO_3195_112_2_18_2_35_36_216 "GetSlideMenuNextItemIndex: PNULL!"
#define MMIMENU_SLIDE_PAGE_GO_4226_112_2_18_2_35_38_217 "[GO_MENU]   DisplaySlidePageMenu "
#define MMIMENU_SLIDE_PAGE_GO_4532_112_2_18_2_35_39_218 "[GO_MENU]HandleSlidePageMenuTimer current time  = %d "
#define MMIMENU_SLIDE_PAGE_GO_4658_112_2_18_2_35_39_219 "[GO_MENU]GOMenuSlideOverDraw-------direction = %d"
#define MMIMENU_SLIDE_PAGE_GO_5183_112_2_18_2_35_40_220 "[JINX_GO]menu_ctrl_ptr->fling_velocity   =  %d"
#define MMIMENU_SLIDE_PAGE_GO_5371_112_2_18_2_35_40_221 "[GO_MENU]GOMenuSlideOverDraw-------direction = %d"
#define MMIMENU_SLIDE_PAGE_GO_5692_112_2_18_2_35_41_222 "[GO_MENU]HandleSlidePageTpUp current time  = %d "
#define MMIMENU_SLIDE_PAGE_GO_5745_112_2_18_2_35_41_223 "HandleSlidePageTpUp result = %d, is_in_menu_ctrl = %d"
#define MMIMENU_SYNCHRONIZE_759_112_2_18_2_35_44_224 "[Mmimenu_synchronize].IsDevEnoughSpace: free_spaced_high = %d, free_space_low = %d"
#define MMIMENU_SYNCHRONIZE_808_112_2_18_2_35_44_225 "[Mmimenu_synchronize].IsItemExist: item existed!"
#define MMIMENU_SYNCHRONIZE_832_112_2_18_2_35_44_226 "[Mmimenu_synchronize].IsValidDyMenuInfo: Run info invalid."
#define MMIMENU_SYNCHRONIZE_852_112_2_18_2_35_44_227 "[Mmimenu_synchronize].IsValidDyMenuInfo: icon info invalid."
#define MMIMENU_SYNCHRONIZE_919_112_2_18_2_35_44_228 "[Mmimenu_synchronize].ReadDevMenuData:file_dead_ptr->dy_menu_num = %d, file_dead_ptr->check_info = %d"
#define MMIMENU_SYNCHRONIZE_991_112_2_18_2_35_44_229 "[Mmimenu_synchronize].GetAllDyMenuData: input is null"
#define MMIMENU_SYNCHRONIZE_1081_112_2_18_2_35_44_230 "[Mmimenu_synchronize].StoreInfoData: input error"
#define MMIMENU_SYNCHRONIZE_1380_112_2_18_2_35_45_231 "[Mmimenu_synchronize].WriteDataInfo: input error"
#define MMIMENU_SYNCHRONIZE_1434_112_2_18_2_35_45_232 "[Mmimenu_synchronize].InsertDyItem: input is null!"
#define MMIMENU_SYNCHRONIZE_1442_112_2_18_2_35_45_233 "[Mmimenu_synchronize].InsertDyItem: page_max_cnt = %d, item_max_cnt = %d,"
#define MMIMENU_SYNCHRONIZE_1912_112_2_18_2_35_46_234 "[Mmimenu_synchronize].DecodeIconData: No Memory!"
#define MMIMENU_SYNCHRONIZE_2443_112_2_18_2_35_47_235 "[Mmimenu_synchronize].DeleteDevFile: fail..."
#define MMIMENU_SYNCHRONIZE_2517_112_2_18_2_35_47_236 "[Mmimenu_synchronize].WriteDataToFile: ERROR SIZE!"
#define MMIMENU_SYNCHRONIZE_2548_112_2_18_2_35_47_237 "[Mmimenu_synchronize].WriteDataToFile: fail"
#define MMIMENU_SYNCHRONIZE_2603_112_2_18_2_35_48_238 "[Mmimenu_synchronize].ReadDataFromFile: ERROR SIZE!"
#define MMIMENU_SYNCHRONIZE_2611_112_2_18_2_35_48_239 "[Mmimenu_synchronize].ReadDataFromFile: No Memory!"
#define MMIMENU_SYNCHRONIZE_2976_112_2_18_2_35_48_240 "[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_func_valid = %d"
#define MMIMENU_SYNCHRONIZE_2977_112_2_18_2_35_48_241 "[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_para1_valid = %d"
#define MMIMENU_SYNCHRONIZE_2978_112_2_18_2_35_48_242 "[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_para2_valid = %d"
#define MMIMENU_SYNCHRONIZE_3001_112_2_18_2_35_48_243 "DisplayDyMenuIconInFile: input is null!"
#define MMIMENU_SYNCHRONIZE_3354_112_2_18_2_35_49_244 "[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_get_run_info = %d"
#define MMIMENU_SYNCHRONIZE_3709_112_2_18_2_35_50_245 "DisplayDyMenuIconInFile: sucess!"
#define MMIMENU_SYNCHRONIZE_3719_112_2_18_2_35_50_246 "Display default icon --- IMAGE_MAINMENU_ICON_DYNAMIC_DEFAUL : sucess!"
#define MMIMENU_SYNCHRONIZE_3775_112_2_18_2_35_50_247 "MMIMENU_DyAppendItemStringArray: input is error!"
#define MMIMENU_SYNCHRONIZE_3783_112_2_18_2_35_50_248 "MMIMENU_DyAppendItemStringArray: input text is too long!"
#define MMIMENU_SYNCHRONIZE_3801_112_2_18_2_35_50_249 "MMIMENU_DySetItemString: input is null!"
#define MMIMENU_SYNCHRONIZE_3813_112_2_18_2_35_50_250 "MMIMENU_DySetItemString: input is error!"
#define MMIMENU_SYNCHRONIZE_3833_112_2_18_2_35_50_251 "MMIMENU_DyGetItemString: input is null!"
#define MMIMENU_SYNCHRONIZE_3840_112_2_18_2_35_50_252 "MMIMENU_DyGetItemString: input is null!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MENU_TRC)
TRACE_MSG(MAINMENU_WIN_1130_112_2_18_2_32_50_0,"Create MAINMENU_MOVEOPTMENU_WIN_TAB!")
TRACE_MSG(MAINMENU_WIN_1135_112_2_18_2_32_50_1,"Create MAINMENU_OPTMENU_WIN_TAB!")
TRACE_MSG(MAINMENU_WIN_3134_112_2_18_2_32_54_2,"HandleMainSecondMenuListWinMsg, msg_id = 0x%02x")
TRACE_MSG(MAINMENU_WIN_3356_112_2_18_2_32_54_3,"HandleMainMenuOptWinMsg msg_id = %d")
TRACE_MSG(MAINMENU_WIN_3422_112_2_18_2_32_55_4,"HandleMainMenuMoveOptWinMsg msg_id = %d")
TRACE_MSG(MMIMENU_CRYSTALCUBE_1165_112_2_18_2_33_0_5,"GetCubePageMenuNextItemIndex: PNULL!")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2288_112_2_18_2_33_2_6,"HandleCubePageMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2599_112_2_18_2_33_3_7,"CCUBE: HandleCubePageTpDown cubepage_state =%d,x=%d,y=%d")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2659_112_2_18_2_33_3_8,"CCUBE: HandleCubePageTpMove cubepage_state =%d,x=%d,y=%d")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2741_112_2_18_2_33_3_9,"CCUBE: HandleCubePageTpUp cubepage_state =%d,x=%d,y=%d")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2822_112_2_18_2_33_3_10,"CCUBE: MSG_CTL_OPEN")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2885_112_2_18_2_33_3_11,"CCUBE: MSG_CTL_GET_FOCUS")
TRACE_MSG(MMIMENU_CRYSTALCUBE_2965_112_2_18_2_33_3_12,"CCUBE: MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3003_112_2_18_2_33_4_13,"CCUBE: MMICRYSTALCUBE_MSG_DISPLAY_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3047_112_2_18_2_33_4_14,"CCUBE: MMICRYSTALCUBE_MSG_ANIM_FRAME_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3081_112_2_18_2_33_4_15,"CCUBE: MMICRYSTALCUBE_MSG_ROTATE_END_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3088_112_2_18_2_33_4_16,"CCUBE: MMICRYSTALCUBE_MSG_ANIM_END_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3095_112_2_18_2_33_4_17,"CCUBE: MMICRYSTALCUBE_MSG_PARAMINIT_CNF")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3122_112_2_18_2_33_4_18,"CCUBE: MMICRYSTALCUBE_MSG_PARAM_RESET_CNF")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3147_112_2_18_2_33_4_19,"CCUBE: MMICUBEPAGE_MSG_TP_CMD_CNF")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3166_112_2_18_2_33_4_20,"CCUBE: MMICUBEPAGE_MSG_TP_SCROOL_FRAME_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3201_112_2_18_2_33_4_21,"CCUBE: MMICRYSTALCUBE_MSG_SRCOLLANIM_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3241_112_2_18_2_33_4_22,"CCUBE: MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3248_112_2_18_2_33_4_23,"CCUBE: MMICUBEPAGE_MSG_SRCOLLANIM_END_IND,%d,%d")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3277_112_2_18_2_33_4_24,"CCUBE: MMICRYSTALCUBE_MSG_PARAMRELEASE_CNF")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3293_112_2_18_2_33_4_25,"CCUBE: MSG_CTL_LOSE_FOCUS")
TRACE_MSG(MMIMENU_CRYSTALCUBE_3316_112_2_18_2_33_4_26,"CCUBE: MSG_CTL_CLOSE")
TRACE_MSG(MMIMENU_CUBE_1222_112_2_18_2_33_7_27,"== CubeMenuRotate == layer is not active")
TRACE_MSG(MMIMENU_CUBE_1369_112_2_18_2_33_8_28,"== CubeMenuEnter == layer is not active")
TRACE_MSG(MMIMENU_CUBE_1774_112_2_18_2_33_8_29,"HandleCubeMenuEnterAnim not support type! %d")
TRACE_MSG(MMIMENU_CYCLONE_1383_112_2_18_2_33_12_30,"CalculateAngleAndMenuItem  velocity_x =%i")
TRACE_MSG(MMIMENU_CYCLONE_1402_112_2_18_2_33_12_31,"CalculateAngleAndMenuItem  velocity_y =%i")
TRACE_MSG(MMIMENU_CYCLONE_1501_112_2_18_2_33_12_32,"CalculateAngleAndMenuItem  scroll_anim_start_angle =%d,catch_angle=%d,inertia_angle=%d")
TRACE_MSG(MMIMENU_CYCLONE_1502_112_2_18_2_33_12_33,"CalculateAngleAndMenuItem  inertia_end_angle =%d,catch_stop_angle=%d,cur_item_index=%d")
TRACE_MSG(MMIMENU_CYCLONE_1503_112_2_18_2_33_12_34,"CalculateAngleAndMenuItem  item_index_backup_in_scroll_anim =%d")
TRACE_MSG(MMIMENU_CYCLONE_1703_112_2_18_2_33_13_35,"HandleCycloneMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_CYCLONE_1823_112_2_18_2_33_13_36,"HandleCycloneTpDown cyclone_state =%d,x=%d,y=%d")
TRACE_MSG(MMIMENU_CYCLONE_1880_112_2_18_2_33_13_37,"HandleCycloneTpMove cyclone_state =%d,x=%d,y=%d tick=%d")
TRACE_MSG(MMIMENU_CYCLONE_1913_112_2_18_2_33_13_38,"HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,x=%d,start_x=%d")
TRACE_MSG(MMIMENU_CYCLONE_1914_112_2_18_2_33_13_39,"HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,y=%d,start_y=%d")
TRACE_MSG(MMIMENU_CYCLONE_1934_112_2_18_2_33_13_40,"HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,x=%d,start_x=%d")
TRACE_MSG(MMIMENU_CYCLONE_1935_112_2_18_2_33_13_41,"HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,y=%d,start_y=%d")
TRACE_MSG(MMIMENU_CYCLONE_1954_112_2_18_2_33_13_42,"HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,x=%d,start_x=%d")
TRACE_MSG(MMIMENU_CYCLONE_1955_112_2_18_2_33_13_43,"HandleCycloneTpMove last_anim_stop_angle =%d,scroll_angle=%d,y=%d,start_y=%d")
TRACE_MSG(MMIMENU_CYCLONE_1972_112_2_18_2_33_13_44,"HandleCycloneTpUp cyclone_state =%d,x=%d,y=%d")
TRACE_MSG(MMIMENU_CYCLONE_2046_112_2_18_2_33_13_45,"CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_OPEN")
TRACE_MSG(MMIMENU_CYCLONE_2086_112_2_18_2_33_13_46,"CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_GET_FOCUS")
TRACE_MSG(MMIMENU_CYCLONE_2121_112_2_18_2_33_13_47,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_ANIM_FRAME_IND,state=%d")
TRACE_MSG(MMIMENU_CYCLONE_2139_112_2_18_2_33_13_48,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_ANIM_END_IND")
TRACE_MSG(MMIMENU_CYCLONE_2150_112_2_18_2_33_13_49,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_ROTATE_FRAME_IND")
TRACE_MSG(MMIMENU_CYCLONE_2163_112_2_18_2_33_13_50,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND")
TRACE_MSG(MMIMENU_CYCLONE_2184_112_2_18_2_33_13_51,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_TP_SCROOL_IND")
TRACE_MSG(MMIMENU_CYCLONE_2188_112_2_18_2_33_14_52,"MMICYCLONE_MSG_TP_SCROOL_IND %d")
TRACE_MSG(MMIMENU_CYCLONE_2208_112_2_18_2_33_14_53,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_SRCOLLANIM_IND")
TRACE_MSG(MMIMENU_CYCLONE_2211_112_2_18_2_33_14_54,"MMICYCLONE_MSG_SRCOLLANIM_IND %d")
TRACE_MSG(MMIMENU_CYCLONE_2229_112_2_18_2_33_14_55,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_PARAMINIT_CNF")
TRACE_MSG(MMIMENU_CYCLONE_2256_112_2_18_2_33_14_56,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_PARAM_RESET_CNF")
TRACE_MSG(MMIMENU_CYCLONE_2281_112_2_18_2_33_14_57,"CYCLONE: HandleCycloneMenuSpecialMsg MMICYCLONE_MSG_PARAMRELEASE_CNF")
TRACE_MSG(MMIMENU_CYCLONE_2294_112_2_18_2_33_14_58,"CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_LOSE_FOCUS")
TRACE_MSG(MMIMENU_CYCLONE_2311_112_2_18_2_33_14_59,"CYCLONE: HandleCycloneMenuSpecialMsg MSG_CTL_CLOSE")
TRACE_MSG(MMIMENU_GO_SE_454_112_2_18_2_33_16_60,"DataTypeAdapt   data_type = %d")
TRACE_MSG(MMIMENU_GO_SE_596_112_2_18_2_33_16_61,"MMIMENU_Go_Draw_Unfold_Page     ERROR   cur_frame_num=%d")
TRACE_MSG(MMIMENU_GO_SE_1259_112_2_18_2_33_17_62,"== calcfuncSphere == Input param cur_page_info error")
TRACE_MSG(MMIMENU_GO_SE_1265_112_2_18_2_33_17_63,"== calcfuncSphere == Input param next_page_info error")
TRACE_MSG(MMIMENU_GO_SE_1271_112_2_18_2_33_17_64,"== calcfuncSphere == Input param bg_window_buf error")
TRACE_MSG(MMIMENU_GO_SE_1277_112_2_18_2_33_17_65,"== calcfuncSphere == Input param cur_frame_num=%d & total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1283_112_2_18_2_33_17_66,"== calcfuncSphere == Input param total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1289_112_2_18_2_33_17_67,"== calcfuncSphere == Input param direction=%d error")
TRACE_MSG(MMIMENU_GO_SE_1295_112_2_18_2_33_17_68,"== calcfuncSphere == Input param bg_window_size.w=%d error")
TRACE_MSG(MMIMENU_GO_SE_1301_112_2_18_2_33_17_69,"== calcfuncSphere == Input param bg_window_size.h=%d error")
TRACE_MSG(MMIMENU_GO_SE_1307_112_2_18_2_33_17_70,"== calcfuncSphere == Input param cur_page_info->total_col_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1313_112_2_18_2_33_17_71,"== calcfuncSphere == Input param cur_page_info->total_row_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1321_112_2_18_2_33_17_72,"== calcfuncSphere == Input param cur_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_1330_112_2_18_2_33_17_73,"== calcfuncSphere == Input param next_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_1355_112_2_18_2_33_17_74,"== calcfuncSphere == SE_Create error")
TRACE_MSG(MMIMENU_GO_SE_1364_112_2_18_2_33_17_75,"== calcfuncSphere == alloca mem error")
TRACE_MSG(MMIMENU_GO_SE_1377_112_2_18_2_33_17_76,"== calcfuncSphere == SE_Render to get param error")
TRACE_MSG(MMIMENU_GO_SE_1392_112_2_18_2_33_17_77,"== calcfuncSphere == Rotation count total_frame_num error")
TRACE_MSG(MMIMENU_GO_SE_1405_112_2_18_2_33_17_78,"== calcfuncSphere == Rotation count total_frame_num error")
TRACE_MSG(MMIMENU_GO_SE_1423_112_2_18_2_33_17_79,"== calcfuncSphere == target layer error")
TRACE_MSG(MMIMENU_GO_SE_1454_112_2_18_2_33_18_80,"== SlideMenuMoveSphere == SE_Render error")
TRACE_MSG(MMIMENU_GO_SE_1507_112_2_18_2_33_18_81,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info error")
TRACE_MSG(MMIMENU_GO_SE_1512_112_2_18_2_33_18_82,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param next_page_info error")
TRACE_MSG(MMIMENU_GO_SE_1517_112_2_18_2_33_18_83,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_buf error")
TRACE_MSG(MMIMENU_GO_SE_1522_112_2_18_2_33_18_84,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1527_112_2_18_2_33_18_85,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1532_112_2_18_2_33_18_86,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param direction=%d error")
TRACE_MSG(MMIMENU_GO_SE_1537_112_2_18_2_33_18_87,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_size.w=%d error")
TRACE_MSG(MMIMENU_GO_SE_1542_112_2_18_2_33_18_88,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_size.h=%d error")
TRACE_MSG(MMIMENU_GO_SE_1547_112_2_18_2_33_18_89,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->total_col_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1552_112_2_18_2_33_18_90,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->total_row_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1559_112_2_18_2_33_18_91,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_1567_112_2_18_2_33_18_92,"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_1798_112_2_18_2_33_18_93,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info error")
TRACE_MSG(MMIMENU_GO_SE_1803_112_2_18_2_33_18_94,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param next_page_info error")
TRACE_MSG(MMIMENU_GO_SE_1808_112_2_18_2_33_18_95,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_buf error")
TRACE_MSG(MMIMENU_GO_SE_1813_112_2_18_2_33_18_96,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1818_112_2_18_2_33_18_97,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1823_112_2_18_2_33_18_98,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param direction=%d error")
TRACE_MSG(MMIMENU_GO_SE_1828_112_2_18_2_33_18_99,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_size.w=%d error")
TRACE_MSG(MMIMENU_GO_SE_1833_112_2_18_2_33_18_100,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_size.h=%d error")
TRACE_MSG(MMIMENU_GO_SE_1838_112_2_18_2_33_18_101,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->total_col_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1843_112_2_18_2_33_18_102,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->total_row_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_1850_112_2_18_2_33_18_103,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_1858_112_2_18_2_33_18_104,"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_2048_112_2_18_2_33_19_105,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info error")
TRACE_MSG(MMIMENU_GO_SE_2053_112_2_18_2_33_19_106,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param next_page_info error")
TRACE_MSG(MMIMENU_GO_SE_2058_112_2_18_2_33_19_107,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_buf error")
TRACE_MSG(MMIMENU_GO_SE_2063_112_2_18_2_33_19_108,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_2068_112_2_18_2_33_19_109,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param total_frame_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_2073_112_2_18_2_33_19_110,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param direction=%d error")
TRACE_MSG(MMIMENU_GO_SE_2078_112_2_18_2_33_19_111,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_size.w=%d error")
TRACE_MSG(MMIMENU_GO_SE_2083_112_2_18_2_33_19_112,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_size.h=%d error")
TRACE_MSG(MMIMENU_GO_SE_2088_112_2_18_2_33_19_113,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->total_col_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_2093_112_2_18_2_33_19_114,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->total_row_num=%d error")
TRACE_MSG(MMIMENU_GO_SE_2100_112_2_18_2_33_19_115,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_GO_SE_2108_112_2_18_2_33_19_116,"MMIMENU_Go_Effect_Turntable_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error")
TRACE_MSG(MMIMENU_ICON_1910_112_2_18_2_33_30_117,"ReadIconMenuItemNum error!")
TRACE_MSG(MMIMENU_ICON_2506_112_2_18_2_33_31_118,"InitIconMenu enter!")
TRACE_MSG(MMIMENU_ICON_2589_112_2_18_2_33_31_119,"InitIconMenu out!")
TRACE_MSG(MMIMENU_ICON_2804_112_2_18_2_33_31_120,"== DisplayIconMenuScrollGroove == prgbox ctrl is PNULL")
TRACE_MSG(MMIMENU_ICON_2962_112_2_18_2_33_32_121,"ConfirmMoveMenuItem not move state!")
TRACE_MSG(MMIMENU_ICON_3175_112_2_18_2_33_32_122,"IconMenuModifyRect Error! style = %d")
TRACE_MSG(MMIMENU_ICON_3330_112_2_18_2_33_32_123,"HandleIconMenuEnterAnim not support type! %d")
TRACE_MSG(MMIMENU_ICON_4715_112_2_18_2_33_35_124,"HandleIconMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_ICON_5233_112_2_18_2_33_36_125,"HandleIconMenuOk msg_id = %x")
TRACE_MSG(MMIMENU_ICON_5986_112_2_18_2_33_38_126,"HandleIconMenuMove %d, %d!")
TRACE_MSG(MMIMENU_ICON_6238_112_2_18_2_33_38_127,"== HandleSecondMenuTimer == timer id %d is error")
TRACE_MSG(MMIMENU_ICON_6912_112_2_18_2_33_40_128,"HandleSecMenuTpDown %d, %d")
TRACE_MSG(MMIMENU_ICON_7132_112_2_18_2_33_40_129,"AdjuestIconMenuOffsetY 1!")
TRACE_MSG(MMIMENU_ICON_7153_112_2_18_2_33_40_130,"AdjuestIconMenuOffsetY 2!")
TRACE_MSG(MMIMENU_ICON_7194_112_2_18_2_33_40_131,"HandleIconTpUp %d")
TRACE_MSG(MMIMENU_ICON_7210_112_2_18_2_33_40_132,"HandleSecMenuTpUp %f")
TRACE_MSG(MMIMENU_ICON_7668_112_2_18_2_33_41_133,"Do nothing")
TRACE_MSG(MMIMENU_ICON_8290_112_2_18_2_33_42_134,"RenderRollEffect() SE_Render fail! ret = %d")
TRACE_MSG(MMIMENU_ISTYLE_2095_112_2_18_2_33_48_135,"[GUIMENU:DrawPageLayer]Error handle %d")
TRACE_MSG(MMIMENU_ISTYLE_2286_112_2_18_2_33_48_136,"mmimenu_istyle: MovePageLayer: Can not move right in search page")
TRACE_MSG(MMIMENU_ISTYLE_6201_112_2_18_2_33_56_137,"HandleTimer: the move_type is error!")
TRACE_MSG(MMIMENU_ISTYLE_6281_112_2_18_2_33_56_138,"HandleIStyleMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_ISTYLE_6348_112_2_18_2_33_57_139,"HandleIStyleMenuAutoMove:continue_move_type %d is error!")
TRACE_MSG(MMIMENU_ISTYLE_6424_112_2_18_2_33_57_140,"AutoSlidePage Error! move type = %d")
TRACE_MSG(MMIMENU_ISTYLE_7425_112_2_18_2_33_59_141,"HandleIStyleMenuMove!")
TRACE_MSG(MMIMENU_ISTYLE_10473_112_2_18_2_34_7_142,"HandleIStyleMenuMove!")
TRACE_MSG(MMIMENU_ISTYLE_12142_112_2_18_2_34_10_143,"mmimenu_istyle: CreateSearchPageLayer page_layer_handle[1] block id is %d")
TRACE_MSG(MMIMENU_ISTYLE_12186_112_2_18_2_34_10_144,"mmimenu_istyle: BltSearchPage offset_x is %d")
TRACE_MSG(MMIMENU_ISTYLE_12209_112_2_18_2_34_10_145,"mmimenu_istyle: ReleaseSearchPage menu_ctrl_ptr is null")
TRACE_MSG(MMIMENU_ISTYLE_12273_112_2_18_2_34_10_146,"mmimenu_istyle: IsListResponseTpMSGInSearchPage form ptr is null")
TRACE_MSG(MMIMENU_ISTYLE_12285_112_2_18_2_34_10_147,"mmimenu_istyle: IsListResponseTpMSGInSearchPage child form ptr is null")
TRACE_MSG(MMIMENU_ISTYLE_12445_112_2_18_2_34_11_148,"mmimenu_istyle.c: SearchPageHookFunc msg_id is %d")
TRACE_MSG(MMIMENU_ISTYLE_12449_112_2_18_2_34_11_149,"mmimenu_istyle.c: SearchPageHookFunc menu_ctrl_ptr is null")
TRACE_MSG(MMIMENU_ISTYLE_12545_112_2_18_2_34_11_150,"mmimenu_istyle: SearchPageHookFunc: move point x cross slide_start_point")
TRACE_MSG(MMIMENU_ISTYLE_12677_112_2_18_2_34_11_151,"mmimenu_istyle: SearchPageHookFunc tp up not found any valid ctrl to handle this msg")
TRACE_MSG(MMIMENU_OPTIONPAGE_1122_112_2_18_2_34_14_152,"HandleOptionMenuOk msg_id = %x")
TRACE_MSG(MMIMENU_OPTIONPAGE_1170_112_2_18_2_34_14_153,"HandleOptMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_OPTIONPAGE_1480_112_2_18_2_34_15_154,"GetOptionMenuNextItemIndex: PNULL!")
TRACE_MSG(MMIMENU_OPTIONPAGE_2625_112_2_18_2_34_17_155,"OptMenuModifyRect Error! style = %d")
TRACE_MSG(MMIMENU_OPTIONPAGE_2768_112_2_18_2_34_17_156,"== DisplayOptionMenuScrollGroove == prgbox ctrl is PNULL")
TRACE_MSG(MMIMENU_OPTIONPAGE_3378_112_2_18_2_34_19_157,"HandleOptMenuTimer: the move_type is error!")
TRACE_MSG(MMIMENU_OPTIONPAGE_3428_112_2_18_2_34_19_158,"HandleOptMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_OPTIONPAGE_3485_112_2_18_2_34_19_159,"HandleOptMenuAutoMove:continue_move_type %d is error!")
TRACE_MSG(MMIMENU_OPTIONPAGE_4793_112_2_18_2_34_22_160,"HandleOptionPageTpUp result = %d, is_in_menu_ctrl = %d")
TRACE_MSG(MMIMENU_POPUP_1211_112_2_18_2_34_27_161,"current item is in menu!")
TRACE_MSG(MMIMENU_POPUP_1284_112_2_18_2_34_28_162,"PopmenuHandleTpUp %f")
TRACE_MSG(MMIMENU_POPUP_1441_112_2_18_2_34_28_163,"current item is in menu!")
TRACE_MSG(MMIMENU_POPUP_1833_112_2_18_2_34_29_164,"current item is in menu!")
TRACE_MSG(MMIMENU_POPUP_2377_112_2_18_2_34_30_165,"ResetPopmenuDisplayItem 1 Error!")
TRACE_MSG(MMIMENU_POPUP_2398_112_2_18_2_34_30_166,"ResetPopmenuDisplayItem 2 Error!")
TRACE_MSG(MMIMENU_POPUP_4040_112_2_18_2_34_33_167,"== CalculateContextMenuRect == create button failed")
TRACE_MSG(MMIMENU_QBTHEME_1846_112_2_18_2_34_40_168,"[GUIMENU:DrawPageLayer]Error handle %d")
TRACE_MSG(MMIMENU_QBTHEME_4247_112_2_18_2_34_45_169,"[GO_MENU]   DisplaySlidePageMenu ")
TRACE_MSG(MMIMENU_QBTHEME_5587_112_2_18_2_34_48_170,"HandleTimer: the move_type is error!")
TRACE_MSG(MMIMENU_QBTHEME_5707_112_2_18_2_34_48_171,"HandleQbthemeMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_QBTHEME_5770_112_2_18_2_34_48_172,"HandleQbthemeMenuAutoMove:continue_move_type %d is error!")
TRACE_MSG(MMIMENU_QBTHEME_5835_112_2_18_2_34_48_173,"AutoSlidePage Error! move type = %d")
TRACE_MSG(MMIMENU_QBTHEME_6655_112_2_18_2_34_50_174,"HandleIStyleMenuMove!")
TRACE_MSG(MMIMENU_QBTHEME_7249_112_2_18_2_34_51_175,"[GO_MENU]HandleSlidePageTpUp current time  = %d ")
TRACE_MSG(MMIMENU_QBTHEME_8258_112_2_18_2_34_53_176,"HandleQbthemeSetWinMsg  msg_id = %x")
TRACE_MSG(MMIMENU_SECOND_1025_112_2_18_2_34_58_177,"== MMIMENU_DisplayScrollGroove == prgbox ctrl is PNULL")
TRACE_MSG(MMIMENU_SECOND_1232_112_2_18_2_34_58_178,"ResetSecMenuDisplayItem 1 Error!")
TRACE_MSG(MMIMENU_SECOND_1253_112_2_18_2_34_58_179,"ResetSecMenuDisplayItem 2 Error!")
TRACE_MSG(MMIMENU_SECOND_1488_112_2_18_2_34_59_180,"HandleSecMenuTpDown %d, %d")
TRACE_MSG(MMIMENU_SECOND_1536_112_2_18_2_34_59_181,"current item is in menu!")
TRACE_MSG(MMIMENU_SECOND_1657_112_2_18_2_34_59_182,"current item is in menu!")
TRACE_MSG(MMIMENU_SECOND_1718_112_2_18_2_34_59_183,"HandleSecMenuTpUp %d")
TRACE_MSG(MMIMENU_SECOND_1733_112_2_18_2_34_59_184,"HandleSecMenuTpUp %f")
TRACE_MSG(MMIMENU_SECOND_1927_112_2_18_2_35_0_185,"current item is in menu!")
TRACE_MSG(MMIMENU_SECOND_3943_112_2_18_2_35_4_186,"== HandleSecondMenuTimer == timer id %d is error")
TRACE_MSG(MMIMENU_SECOND_4018_112_2_18_2_35_4_187,"HandleIconMenuOk msg_id = %x")
TRACE_MSG(MMIMENU_SLIDE_PAGE_1695_112_2_18_2_35_8_188,"[GUIMENU:DrawSlideMenuPageLayer]Error handle %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_2693_112_2_18_2_35_10_189,"[GUIMENU:DrawSlideMenuPageLayer]Error handle %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_2983_112_2_18_2_35_10_190,"== SlideMenuMoveSphere == need not draw")
TRACE_MSG(MMIMENU_SLIDE_PAGE_2992_112_2_18_2_35_10_191,"== SlideMenuMoveSphere == get layer 0 error")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3019_112_2_18_2_35_10_192,"== SlideMenuMoveSphere == SE_Create error")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3027_112_2_18_2_35_10_193,"== SlideMenuMoveSphere == alloca mem error")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3039_112_2_18_2_35_10_194,"== SlideMenuMoveSphere == SE_Render to get param error")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3076_112_2_18_2_35_10_195,"== SlideMenuMoveSphere == target layer error")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3117_112_2_18_2_35_11_196,"== SlideMenuMoveSphere == SE_Render error")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3721_112_2_18_2_35_12_197,"HandleSlidePageMenuOk msg_id = %x")
TRACE_MSG(MMIMENU_SLIDE_PAGE_3805_112_2_18_2_35_12_198,"ConfirmSlidePageMenuItem not move state!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_4260_112_2_18_2_35_13_199,"GetSlideMenuNextItemIndex: PNULL!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_5014_112_2_18_2_35_14_200,"ReadSlideMenuPageItemInfo Read Nv Error! page_num = %d, item count = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_5795_112_2_18_2_35_16_201,"SlideMenuModifyRect Error! style = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_6691_112_2_18_2_35_18_202,"HandleSlideMenuEnterAnim not support type! %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_7399_112_2_18_2_35_20_203,"HandleSlidePageMenuTimer: the move_type is error!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_7551_112_2_18_2_35_20_204,"HandleSlidePageMenuCancelKey not move state!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_7618_112_2_18_2_35_20_205,"HandleSlidePageMenuAutoMove:continue_move_type %d is error!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_7789_112_2_18_2_35_20_206,"AutoSlidePage Error! move type = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_9267_112_2_18_2_35_24_207,"HandleSlidePageMenuMove!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_10856_112_2_18_2_35_27_208,"HandleSlidePageTpUp result = %d, is_in_menu_ctrl = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_1042_112_2_18_2_35_31_209,"slide menu not support DisplayActiveIconItem, error!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_1052_112_2_18_2_35_31_210,"slide menu not support DisplayEnterMenuAnim, error!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_1063_112_2_18_2_35_31_211,"slide menu not support DisplaySwitchMenuAnim, error!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_1338_112_2_18_2_35_32_212,"Do nothing")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_2316_112_2_18_2_35_34_213,"s_go_item_page_info handle block_id== PNULL")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_2373_112_2_18_2_35_34_214,"[GO_MENU]MMIMENU_DrawGOMenuLayer current time  = %d ")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_2534_112_2_18_2_35_34_215,"[GO_MENU]MMIMENU_GO_Effect_calc calcfunc cost time = %d         STYLE = %d      frame = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_3195_112_2_18_2_35_36_216,"GetSlideMenuNextItemIndex: PNULL!")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_4226_112_2_18_2_35_38_217,"[GO_MENU]   DisplaySlidePageMenu ")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_4532_112_2_18_2_35_39_218,"[GO_MENU]HandleSlidePageMenuTimer current time  = %d ")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_4658_112_2_18_2_35_39_219,"[GO_MENU]GOMenuSlideOverDraw-------direction = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_5183_112_2_18_2_35_40_220,"[JINX_GO]menu_ctrl_ptr->fling_velocity   =  %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_5371_112_2_18_2_35_40_221,"[GO_MENU]GOMenuSlideOverDraw-------direction = %d")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_5692_112_2_18_2_35_41_222,"[GO_MENU]HandleSlidePageTpUp current time  = %d ")
TRACE_MSG(MMIMENU_SLIDE_PAGE_GO_5745_112_2_18_2_35_41_223,"HandleSlidePageTpUp result = %d, is_in_menu_ctrl = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_759_112_2_18_2_35_44_224,"[Mmimenu_synchronize].IsDevEnoughSpace: free_spaced_high = %d, free_space_low = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_808_112_2_18_2_35_44_225,"[Mmimenu_synchronize].IsItemExist: item existed!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_832_112_2_18_2_35_44_226,"[Mmimenu_synchronize].IsValidDyMenuInfo: Run info invalid.")
TRACE_MSG(MMIMENU_SYNCHRONIZE_852_112_2_18_2_35_44_227,"[Mmimenu_synchronize].IsValidDyMenuInfo: icon info invalid.")
TRACE_MSG(MMIMENU_SYNCHRONIZE_919_112_2_18_2_35_44_228,"[Mmimenu_synchronize].ReadDevMenuData:file_dead_ptr->dy_menu_num = %d, file_dead_ptr->check_info = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_991_112_2_18_2_35_44_229,"[Mmimenu_synchronize].GetAllDyMenuData: input is null")
TRACE_MSG(MMIMENU_SYNCHRONIZE_1081_112_2_18_2_35_44_230,"[Mmimenu_synchronize].StoreInfoData: input error")
TRACE_MSG(MMIMENU_SYNCHRONIZE_1380_112_2_18_2_35_45_231,"[Mmimenu_synchronize].WriteDataInfo: input error")
TRACE_MSG(MMIMENU_SYNCHRONIZE_1434_112_2_18_2_35_45_232,"[Mmimenu_synchronize].InsertDyItem: input is null!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_1442_112_2_18_2_35_45_233,"[Mmimenu_synchronize].InsertDyItem: page_max_cnt = %d, item_max_cnt = %d,")
TRACE_MSG(MMIMENU_SYNCHRONIZE_1912_112_2_18_2_35_46_234,"[Mmimenu_synchronize].DecodeIconData: No Memory!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2443_112_2_18_2_35_47_235,"[Mmimenu_synchronize].DeleteDevFile: fail...")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2517_112_2_18_2_35_47_236,"[Mmimenu_synchronize].WriteDataToFile: ERROR SIZE!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2548_112_2_18_2_35_47_237,"[Mmimenu_synchronize].WriteDataToFile: fail")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2603_112_2_18_2_35_48_238,"[Mmimenu_synchronize].ReadDataFromFile: ERROR SIZE!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2611_112_2_18_2_35_48_239,"[Mmimenu_synchronize].ReadDataFromFile: No Memory!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2976_112_2_18_2_35_48_240,"[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_func_valid = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2977_112_2_18_2_35_48_241,"[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_para1_valid = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_2978_112_2_18_2_35_48_242,"[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_para2_valid = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3001_112_2_18_2_35_48_243,"DisplayDyMenuIconInFile: input is null!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3354_112_2_18_2_35_49_244,"[Mmimenu_synchronize].MMIMENU_DyListGetRunInfo: is_get_run_info = %d")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3709_112_2_18_2_35_50_245,"DisplayDyMenuIconInFile: sucess!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3719_112_2_18_2_35_50_246,"Display default icon --- IMAGE_MAINMENU_ICON_DYNAMIC_DEFAUL : sucess!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3775_112_2_18_2_35_50_247,"MMIMENU_DyAppendItemStringArray: input is error!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3783_112_2_18_2_35_50_248,"MMIMENU_DyAppendItemStringArray: input text is too long!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3801_112_2_18_2_35_50_249,"MMIMENU_DySetItemString: input is null!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3813_112_2_18_2_35_50_250,"MMIMENU_DySetItemString: input is error!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3833_112_2_18_2_35_50_251,"MMIMENU_DyGetItemString: input is null!")
TRACE_MSG(MMIMENU_SYNCHRONIZE_3840_112_2_18_2_35_50_252,"MMIMENU_DyGetItemString: input is null!")
END_TRACE_MAP(MMI_APP_MENU_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MENU_TRC_H_

