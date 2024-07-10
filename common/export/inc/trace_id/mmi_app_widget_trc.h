/******************************************************************************
 ** File Name:      mmi_app_widget_trc.h                                         *
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
//trace_id:239
#ifndef _MMI_APP_WIDGET_TRC_H_
#define _MMI_APP_WIDGET_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIGRID_ALARM_311_112_2_18_3_6_53_0 "MMIGIRD_ALARM DisplayAlarmSwitch event_ptr = 0"
#define MMIGRID_ALARM_316_112_2_18_3_6_53_1 "MMIGIRD_ALARM DisplayAlarmSwitch event_delay_ptr = 0"
#define MMIGRID_ALARM_419_112_2_18_3_6_53_2 "MMIGIRD_ALARM DisplayAlarmSwitch event_ptr = 0"
#define MMIGRID_ALARM_424_112_2_18_3_6_53_3 "MMIGIRD_ALARM DisplayAlarmSwitch event_delay_ptr = 0"
#define MMIGRID_ALARM_628_112_2_18_3_6_54_4 "MMIGIRD_ALARM HandleGridAlarmTpUp event_ptr = 0"
#define MMIGRID_ALARM_633_112_2_18_3_6_54_5 "MMIGIRD_ALARM HandleGridAlarmTpUp event_delay_ptr = 0"
#define MMIGRID_DISPLAY_1044_112_2_18_3_6_56_6 "icon menu: group_id = %d, menu_id = %d"
#define MMIGRID_IDLE_1443_112_2_18_3_7_0_7 "idle_grid_ptr is null"
#define MMIGRID_IDLE_1548_112_2_18_3_7_0_8 "MMIGRID_IDLE CreateEditBgLayer error = %d"
#define MMIGRID_IDLE_1559_112_2_18_3_7_0_9 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_2131_112_2_18_3_7_1_10 "MMIGRID_IDLE MMIAPIGRID_HandleMsg win_id = %x,msg_id = %x"
#define MMIGRID_IDLE_3402_112_2_18_3_7_4_11 "DestroyWidgetItem() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_3535_112_2_18_3_7_4_12 "MMIGRID_IDLE MMIGRID_GetWinRect row_num == %d"
#define MMIGRID_IDLE_3999_112_2_18_3_7_5_13 "MMIGRID CreateThumbViewLayer  == FALSE"
#define MMIGRID_IDLE_4027_112_2_18_3_7_5_14 "MMIGRID ThumbOnePageLayer  == FALSE"
#define MMIGRID_IDLE_4173_112_2_18_3_7_5_15 "MMIGRID_GRID HandleGridTpUp"
#define MMIGRID_IDLE_4436_112_2_18_3_7_6_16 "SaveWidgetInfoToNV() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_4482_112_2_18_3_7_6_17 "GetGridTpPressedItem() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_4531_112_2_18_3_7_6_18 "SetWidgetTPHitType() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_4776_112_2_18_3_7_7_19 "from main menu: group id = %d, menu id = %d"
#define MMIGRID_IDLE_4796_112_2_18_3_7_7_20 "idle_grid_ptr->icon_ptr[%d].icon_info. group_id = %d, menu_id = %d"
#define MMIGRID_IDLE_5065_112_2_18_3_7_7_21 "MMIGRID_GRID HandleGridTpDown"
#define MMIGRID_IDLE_5161_112_2_18_3_7_7_22 "RedrawWidgetItem() item_ptr == PNULL!"
#define MMIGRID_IDLE_5223_112_2_18_3_7_7_23 "HandleGridAutoSlideEvent() PNULL == idle_grid_ptr"
#define MMIGRID_IDLE_5785_112_2_18_3_7_9_24 "SetWidgetActive() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_5897_112_2_18_3_7_9_25 "DestroyElveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_5927_112_2_18_3_7_9_26 "DestroyElveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_5984_112_2_18_3_7_9_27 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6006_112_2_18_3_7_9_28 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6381_112_2_18_3_7_10_29 "InsertShortcut() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6418_112_2_18_3_7_10_30 "RemoveShortcut() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6500_112_2_18_3_7_10_31 "ActiveWidgetItem() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6533_112_2_18_3_7_10_32 "ActiveWidgetItem() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6598_112_2_18_3_7_10_33 "GetWinHandleByWidgetId() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_6626_112_2_18_3_7_10_34 "GetWidgetIdByWinHandle() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_7140_112_2_18_3_7_11_35 "MMIGRID_IDLE DrawSpecificPage wid is focues = %d"
#define MMIGRID_IDLE_7969_112_2_18_3_7_13_36 "ResetAllGridLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_8104_112_2_18_3_7_13_37 "SetChildWinRect window_rect:(%d, %d, %d, %d) is error!"
#define MMIGRID_IDLE_9063_112_2_18_3_7_16_38 "MMIGRID DisplayGridPageThumbView GRAPH_ScaleImage error"
#define MMIGRID_IDLE_9091_112_2_18_3_7_16_39 "MMIGRID CreateThumbViewLayer  == FALSE"
#define MMIGRID_IDLE_9102_112_2_18_3_7_16_40 "MMIGRID DisplayGridPageThumbView tar_buf == 0"
#define MMIGRID_IDLE_9118_112_2_18_3_7_16_41 "MMIGRID ThumbOnePageLayer  == FALSE"
#define MMIGRID_IDLE_9244_112_2_18_3_7_17_42 "MMIGRID_IDLE MMIGRID_InitWidgetsPosition ROW*COL == 0"
#define MMIGRID_IDLE_9249_112_2_18_3_7_17_43 "MMIGRID_IDLE MMIGRID_InitWidgetsPosition row_num == %d"
#define MMIGRID_IDLE_9425_112_2_18_3_7_17_44 "SetDisplayRect ERROR! tp_type = %d"
#define MMIGRID_IDLE_10859_112_2_18_3_7_21_45 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_11293_112_2_18_3_7_22_46 "CreateMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_11442_112_2_18_3_7_22_47 "CreateMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_11480_112_2_18_3_7_22_48 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIGRID_IDLE_11762_112_2_18_3_7_23_49 "DrawSpecificPage() PNULL == idle_grid_ptr!"
#define MMIGRID_IMGVIEWER_398_112_2_18_3_7_24_50 "MMIGRID_IMGVIEWER SetImgAnimData ctrl_idx = %d,photo_index =%d"
#define MMIGRID_IMGVIEWER_400_112_2_18_3_7_24_51 "MMIGRID_IMGVIEWER SetImgAnimData s_imgviewer_info.s_files =%x"
#define MMIGRID_IMGVIEWER_404_112_2_18_3_7_24_52 "MMIGRID_IMGVIEWER SetImgAnimData  MMIAPIFILEARRAY_Read failed"
#define MMIGRID_IMGVIEWER_420_112_2_18_3_7_24_53 "MMIGRID_IMGVIEWER SetImgAnimData  time = %d"
#define MMIGRID_IMGVIEWER_573_112_2_18_3_7_24_54 "MMIGRID_IMGVIEWER win_handlement  time = %d"
#define MMIGRID_IMGVIEWER_575_112_2_18_3_7_24_55 "MMIGRID_IMGVIEWER win_handlement ctrl_index = %x ,result = %d"
#define MMIGRID_IMGVIEWER_768_112_2_18_3_7_25_56 "MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
#define MMIGRID_IMGVIEWER_771_112_2_18_3_7_25_57 "MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
#define MMIGRID_IMGVIEWER_773_112_2_18_3_7_25_58 "MMIGRID_IMGVIEWER UpdateFolderPhoto start time = %d"
#define MMIGRID_IMGVIEWER_784_112_2_18_3_7_25_59 "MMIGRID_IMGVIEWER UpdateFolderPhoto end time = %d"
#define MMIGRID_IMGVIEWER_787_112_2_18_3_7_25_60 "MMIGRID_IMGVIEWER UpdateFolderPhoto file_num = %d"
#define MMIGRID_IMGVIEWER_804_112_2_18_3_7_25_61 "MMIGRID_IMGVIEWER OpenPhotoFolderCallback enter file_num = %d"
#define MMIGRID_IMGVIEWER_810_112_2_18_3_7_25_62 "MMIGRID_IMGVIEWER UpdateFolderPhoto MMIAPIFILEARRAY_Read failed"
#define MMIGRID_IMGVIEWER_828_112_2_18_3_7_25_63 "MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
#define MMIGRID_IMGVIEWER_831_112_2_18_3_7_25_64 "MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
#define MMIGRID_IMGVIEWER_833_112_2_18_3_7_25_65 "MMIGRID_IMGVIEWER MMIAPIFMM_SearchFileInPath start time = %d"
#define MMIGRID_IMGVIEWER_844_112_2_18_3_7_25_66 "MMIGRID_IMGVIEWER MMIAPIFMM_SearchFileInPath end time = %d"
#define MMIGRID_IMGVIEWER_847_112_2_18_3_7_25_67 "MMIGRID_IMGVIEWER OpenPhotoFolderCallback file_num = %d"
#define MMIGRID_PB_664_112_2_18_3_7_28_68 "[mmipb] MakeCall ret %d"
#define MMIGRID_PB_747_112_2_18_3_7_28_69 "[mmipb] MakeCall ret %d"
#define MMIGRID_PB_791_112_2_18_3_7_28_70 "MakeNumberCallByBCD PNULL == bcd_number_ptr"
#define MMIGRID_PB_816_112_2_18_3_7_28_71 "[MMIPB]MakeNumberCallByString number_str_ptr == PNULL !"
#define MMIGRID_PB_1090_112_2_18_3_7_29_72 "[mmipb] MakeCall ret %d"
#define MMIWIDGET_1201_112_2_18_3_7_33_73 "total_num = %d\n"
#define MMIWIDGET_1206_112_2_18_3_7_33_74 "shortcut[%d] = %d\n"
#define MMIWIDGET_1219_112_2_18_3_7_33_75 "active item [%d] = %d, rect = {%d, %d, %d, %d}"
#define MMIWIDGET_1500_112_2_18_3_7_34_76 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2088_112_2_18_3_7_35_77 "DispatchMsgToAllChildWin() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2184_112_2_18_3_7_35_78 "mmiwidget.c MMIWIDGET_Create() ctrl_id = %d is exist!"
#define MMIWIDGET_2369_112_2_18_3_7_35_79 "MMIWIDGET_SetMaxItemByPtr() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2376_112_2_18_3_7_35_80 "MMIWIDGET_SetMaxItemByPtr() max_item_num is zero!"
#define MMIWIDGET_2405_112_2_18_3_7_36_81 "DestroyWidgetItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2429_112_2_18_3_7_36_82 "DestroyAllWidgetItemData() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2463_112_2_18_3_7_36_83 "GetItemPtr() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2496_112_2_18_3_7_36_84 "UpdateAllWidgetItem() PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr!"
#define MMIWIDGET_2549_112_2_18_3_7_36_85 "UpdateAllWidgetItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2848_112_2_18_3_7_36_86 "DisplayWidgetShortcut() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2909_112_2_18_3_7_37_87 "DisplayWidgetShortcut() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_2982_112_2_18_3_7_37_88 "FindWidgetItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3013_112_2_18_3_7_37_89 "IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3049_112_2_18_3_7_37_90 "IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3078_112_2_18_3_7_37_91 "IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3117_112_2_18_3_7_37_92 "CheckTotalWidgetNum() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3162_112_2_18_3_7_37_93 "ChangeWidgetWinRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3206_112_2_18_3_7_37_94 "ResetHitWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3230_112_2_18_3_7_37_95 "ResetOneWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3278_112_2_18_3_7_37_96 "HandleWidgetTpUp() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3285_112_2_18_3_7_37_97 "ResetHitWidgetLayer()00000 widget_id = %d"
#define MMIWIDGET_3343_112_2_18_3_7_37_98 "VibrateWidgetItem() startup"
#define MMIWIDGET_3578_112_2_18_3_7_38_99 "HandleWidgetTpUp() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3824_112_2_18_3_7_38_100 "SaveWidgetInfoToNV() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3895_112_2_18_3_7_39_101 "GetWidgetTpPressedItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3940_112_2_18_3_7_39_102 "ResetWidgetTPMoveInfo() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_3959_112_2_18_3_7_39_103 "SetWidgetTPHitType() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_4569_112_2_18_3_7_40_104 "IsValidShortcutOffset() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_4662_112_2_18_3_7_40_105 "IsValidShortcutOffset() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_4705_112_2_18_3_7_40_106 "HandleWidgetTpDown() hit_widget_id == MMIWIDGET_INVALID_ID!"
#define MMIWIDGET_4790_112_2_18_3_7_40_107 "RedrawWidgetItem() item_ptr == PNULL!"
#define MMIWIDGET_4810_112_2_18_3_7_40_108 "IsValidShortcutOffset() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_4896_112_2_18_3_7_41_109 "HandleWidgetAutoSlideEvent() PNULL == widget_ctrl_ptr"
#define MMIWIDGET_4930_112_2_18_3_7_41_110 "HandleWidgetTpMove() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5091_112_2_18_3_7_41_111 "SetWidgetActive() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5172_112_2_18_3_7_41_112 "SetHitWidgetTopmost() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5318_112_2_18_3_7_41_113 "AddZorderItemTopmost() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5371_112_2_18_3_7_41_114 "SetZorderItemTopMostByWidgetId() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5393_112_2_18_3_7_42_115 "SetZorderItemTopMost() PNULL == widget_ctrl_ptr || PNULL == item_ptr!"
#define MMIWIDGET_5427_112_2_18_3_7_42_116 "RedrawHitIcon() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5487_112_2_18_3_7_42_117 "ClearWidgetRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5507_112_2_18_3_7_42_118 "DestroyWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5531_112_2_18_3_7_42_119 "DestroyElveLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5555_112_2_18_3_7_42_120 "DestroyElveLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5579_112_2_18_3_7_42_121 "DestroyElveLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5603_112_2_18_3_7_42_122 "DestroyElveLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5627_112_2_18_3_7_42_123 "DestroyWallpaperLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5652_112_2_18_3_7_42_124 "DestroyWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5676_112_2_18_3_7_42_125 "DestroyWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5705_112_2_18_3_7_42_126 "CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5780_112_2_18_3_7_42_127 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5803_112_2_18_3_7_42_128 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5826_112_2_18_3_7_42_129 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5850_112_2_18_3_7_42_130 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5874_112_2_18_3_7_42_131 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5898_112_2_18_3_7_43_132 "RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5924_112_2_18_3_7_43_133 "CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_5970_112_2_18_3_7_43_134 "CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6018_112_2_18_3_7_43_135 "CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6068_112_2_18_3_7_43_136 "CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6113_112_2_18_3_7_43_137 "CreateWallpaperLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6169_112_2_18_3_7_43_138 "CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6238_112_2_18_3_7_43_139 "InsertShortcut() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6275_112_2_18_3_7_43_140 "InsertShortcut() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6297_112_2_18_3_7_43_141 "RemoveShortcut() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6337_112_2_18_3_7_43_142 "RemoveShortcut() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6390_112_2_18_3_7_43_143 "GetShortcutItemRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6444_112_2_18_3_7_44_144 "GetShortcutItemRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6497_112_2_18_3_7_44_145 "GetShortcutItemRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6531_112_2_18_3_7_44_146 "FindShortcutInsertPosByPoint() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6569_112_2_18_3_7_44_147 "FindShortcutPosByPoint() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6635_112_2_18_3_7_44_148 "SetWidgetActive() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6664_112_2_18_3_7_44_149 "ActiveWidgetItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6705_112_2_18_3_7_44_150 "ActiveWidgetItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6748_112_2_18_3_7_44_151 "DeactiveWidgetItem() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6779_112_2_18_3_7_44_152 "GetWinHandleByWidgetId() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6807_112_2_18_3_7_44_153 "GetWidgetIdByWinHandle() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_6875_112_2_18_3_7_44_154 "MMIWIDGET_SetShortcutOffset() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_7114_112_2_18_3_7_45_155 "RemoveWidgetItemPtr() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_7220_112_2_18_3_7_45_156 "SlideWidgetOneByOne() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_7444_112_2_18_3_7_46_157 "ChangeAllChildWinRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_7662_112_2_18_3_7_46_158 "BakAllActiveChildWinRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_7693_112_2_18_3_7_46_159 "RestoreAllActiveChildWinRect() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_8237_112_2_18_3_7_47_160 "CreateWidgetPageLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_8289_112_2_18_3_7_47_161 "MMIWIDGET_SetCurPageIndex() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_8394_112_2_18_3_7_47_162 "ResetAllWidgetLayer() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_8528_112_2_18_3_7_48_163 "SetChildWinRect window_rect:(%d, %d, %d, %d) is error!"
#define MMIWIDGET_8631_112_2_18_3_7_48_164 "IsElveWidgetActive() PNULL == widget_ctrl_ptr!"
#define MMIWIDGET_8669_112_2_18_3_7_48_165 "CopyPageLayerToMainLayer() pass = %dms"
#define MMIWIDGET_ACLOCK_387_112_2_18_3_7_49_166 "aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIWIDGET_ACLOCK_539_112_2_18_3_7_49_167 "aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIWIDGET_ACLOCK_647_112_2_18_3_7_50_168 "MMIWIDGET_ACLOCK DisplayAClock rect = %d,%d,%d,%d"
#define MMIWIDGET_ACLOCK_652_112_2_18_3_7_50_169 "MMIWIDGET_ACLOCK DisplayAClock lcd_id = %d"
#define MMIWIDGET_ACLOCK_708_112_2_18_3_7_50_170 "MMIWIDGET_ACLOCK DisplayAClock SCI_ALLOCA error"
#define MMIWIDGET_ACLOCK_722_112_2_18_3_7_50_171 "MMIWIDGET_ACLOCK DisplayAClock GRAPH_DrawClock error"
#define MMIWIDGET_ACLOCK_952_112_2_18_3_7_50_172 "[MMIWIDGET]: StartTimer: the timer has started!"
#define MMIWIDGET_ACLOCK_970_112_2_18_3_7_50_173 "[MMIWIDGET]: StopTimer: the timer has stopped!"
#define MMIWIDGET_AUTOANSWER_LOG_283_112_2_18_3_7_53_174 "MMIASP this number has find a name "
#define MMIWIDGET_CALENDAR_555_112_2_18_3_7_56_175 "MMICALENDAR_GetMonthday error month=%d"
#define MMIWIDGET_CALENDAR_582_112_2_18_3_7_56_176 "MMIWIDGET_Calendar_GetDateRect row >= MMIWIDGET_CALENDAR_LINENUM row=%d"
#define MMIWIDGET_CALENDAR_588_112_2_18_3_7_56_177 "MMIWIDGET_Calendar_GetDateRect col >= MMIWIDGET_CALENDAR_COLNUM col=%d"
#define MMIWIDGET_CALENDAR_631_112_2_18_3_7_56_178 "MMICALENDAR_GetWeekday month < 1 || month > 12, month=%d"
#define MMIWIDGET_CALENDAR_637_112_2_18_3_7_56_179 "MMICALENDAR_GetWeekday day < 1 || day > 31, day=%d"
#define MMIWIDGET_CALENDAR_673_112_2_18_3_7_56_180 "MMIWIDGET_Calendar_GetPosByDate PNULL == calendar_info_ptr || PNULL == row_ptr || PNULL == col_ptr"
#define MMIWIDGET_CALENDAR_897_112_2_18_3_7_56_181 "MMIWIDGET_Calendar_DrawWeeks PNULL == calendar_info_ptr"
#define MMIWIDGET_CALENDAR_1080_112_2_18_3_7_57_182 "MMIWIDGET_Calendar_DrawMonthDate PNULL != calendar_info_ptr || PNULL == rect_ptr"
#define MMIWIDGET_CALENDAR_1313_112_2_18_3_7_57_183 "CALENDAR_GetFirstday:solar_month %d is error!"
#define MMIWIDGET_CALENDAR_1332_112_2_18_3_7_57_184 "MMIWIDGET_Calendar_DrawCurrentDate PNULL != calendar_info_ptr"
#define MMIWIDGET_CALLLOG_248_112_2_18_3_7_58_185 "MMIWIDGET_GetCallLogTextStyle PNULL == style_ptr"
#define MMIWIDGET_CALLLOG_599_112_2_18_3_7_59_186 "MMIWIDGET_DisplayMissLogDetail arraycall_info failed to alloc memory"
#define MMIWIDGET_CALLLOG_751_112_2_18_3_7_59_187 "MMIWIDGET_Calllog_EnterWidgetCLNoPrivacyProtect arraycall_info failed to alloc memory"
#define MMIWIDGET_COUNTEDTIME_503_112_2_18_3_8_1_188 "[MMIWIDGET]: StartTimer: the timer has started!"
#define MMIWIDGET_COUNTEDTIME_520_112_2_18_3_8_1_189 "[MMIWIDGET]: StopTimer: the timer has stopped!"
#define MMIWIDGET_COUNTEDTIME_731_112_2_18_3_8_2_190 "MMIWIDGET_GetTextStyle style_ptr"
#define MMIWIDGET_DCD_389_112_2_18_3_8_4_191 "[MMIDCD]:MMIWIDGETDCD_SetData() enter"
#define MMIWIDGET_DCD_412_112_2_18_3_8_4_192 "[MMIDCD]:MMIWIDGETDCD_SetData() exit"
#define MMIWIDGET_DCD_657_112_2_18_3_8_4_193 "GUITAB_SwitchToNextItem FALSE!"
#define MMIWIDGET_DCD_684_112_2_18_3_8_4_194 "MMIWIDGETDCD_SwitchToNextItem type error! %d"
#define MMIWIDGET_DCD_754_112_2_18_3_8_4_195 "MMICD will show file"
#define MMIWIDGET_DUAL_ACLOCK_288_112_2_18_3_8_7_196 "dual aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIWIDGET_DUAL_ACLOCK_409_112_2_18_3_8_8_197 "dual aclock Convert888To888Or565 (int32)bk_width < start_x || (int32)bk_height < start_y"
#define MMIWIDGET_DUAL_ACLOCK_639_112_2_18_3_8_8_198 "MMIWIDGET_DUAL_ACLOCK DisplayAClock GRAPH_DrawClock error"
#define MMIWIDGET_DUAL_ACLOCK_1053_112_2_18_3_8_9_199 "[MMIWIDGET]: StartTimer: the timer has started!"
#define MMIWIDGET_DUAL_ACLOCK_1069_112_2_18_3_8_9_200 "[MMIWIDGET]: StopTimer: the timer has stopped!"
#define MMIWIDGET_MEMO_324_112_2_18_3_8_13_201 "MMIWIDGET_Memo_ReadMemoFile PNULL == g_edit_str.wstr_ptr"
#define MMIWIDGET_MEMO_331_112_2_18_3_8_13_202 "MMIWIDGET_Memo_ReadMemoFile PNULL == g_memo_edit_str[i].wstr_ptr"
#define MMIWIDGET_MEMO_579_112_2_18_3_8_14_203 "[MMIWIDGET_Memo]: HandleMemoEditWinMsg = before MMIWIDGET_Memo_ReadMemoFile! "
#define MMIWIDGET_MEMO_653_112_2_18_3_8_14_204 "MMIWIDGET_Memo_GetMemoTextStyle style_ptr"
#define MMIWIDGET_MEMO_947_112_2_18_3_8_15_205 "GetCurMemoIdx PNULL == item_ptr !"
#define MMIWIDGET_MP3_803_112_2_18_3_8_17_206 "MMIWIDGET_MP3 HandleWidgetMp3WinMsg MSG_TIMER MMIAPIMP3_GetAudioState = %d"
#define MMIWIDGET_MP3_1548_112_2_18_3_8_18_207 "GetMp3WidgetTextStyle PNULL == style_ptr"
#define MMIWIDGET_MP3_1723_112_2_18_3_8_19_208 "HandleProgressBarTpKey  tp_x=%d error!"
#define MMIWIDGET_MP3_1732_112_2_18_3_8_19_209 "MMIMP3_HandleProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d"
#define MMIWIDGET_MP3_1752_112_2_18_3_8_19_210 "MMIMP3_HandleProgressBarTpKey:yaohua get mp3 current offset result %d is error!"
#define MMIWIDGET_MP3_1758_112_2_18_3_8_19_211 "MMIMP3_HandleProgressBarTpKey:cur_state %d don't handle!"
#define MMIWIDGET_MP3_1781_112_2_18_3_8_19_212 "GetMp3CurrentOffsetByTp, Error: is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!"
#define MMIWIDGET_MP3_1791_112_2_18_3_8_19_213 "GetMp3CurrentOffsetByTp:cur_slide_x_offset = %d,tp_x_offset = %d"
#define MMIWIDGET_MP3_1812_112_2_18_3_8_19_214 "GetMp3CurrentOffsetByTp:is_forward = %d,mp3_total_offset = %ld"
#define MMIWIDGET_MP3_1831_112_2_18_3_8_19_215 "[MMIMP3] ForwardOrBackwardMp3 mp3_offset = %d"
#define MMIWIDGET_MP3_1836_112_2_18_3_8_19_216 "ForwardOrBackwardMp3, cur_state = %d error!"
#define MMIWIDGET_MP3_1863_112_2_18_3_8_19_217 "ForwardOrBackwardMp3:audio seek result = %d"
#define MMIWIDGET_NETSEARCH_325_112_2_18_3_8_20_218 "MMIWIDGET_NetSearch_GetNetSearchTextStyle style_ptr"
#define MMIWIDGET_NETWORK_NAME_205_112_2_18_3_8_21_219 "MMIWIDGET_Network_GetNetWorkTextStyle PNULL == style_ptr"
#define MMIWIDGET_NEWSMS_225_112_2_18_3_8_22_220 "MMIWIDGET_NewSms_GetNewSmsTextStyle PNULL == style_ptr"
#define MMIWIDGET_NEWSMS_514_112_2_18_3_8_23_221 "MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr"
#define MMIWIDGET_NEWSMS_566_112_2_18_3_8_23_222 "MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr"
#define MMIWIDGET_PB_851_112_2_18_3_8_25_223 "mmipb_wintab: HandleInterfaceType storage = %d"
#define MMIWIDGET_PB_894_112_2_18_3_8_25_224 "MMIWIDGETPB_MakeNumberCallByBCD PNULL == bcd_number_ptr"
#define MMIWIDGET_PB_923_112_2_18_3_8_25_225 "[MMIPB]MakeNumberCallByString number_str_ptr == PNULL !"
#define MMIWIDGET_PB_1828_112_2_18_3_8_27_226 "failed to alloc name memory, display empty for exception."
#define MMIWIDGET_SCHEDULE_149_112_2_18_3_8_30_227 "HandleWidgetScheduleWinMsg msg_id = %x"
#define MMIWIDGET_SCHEDULE_162_112_2_18_3_8_30_228 "HandleWidgetScheduleWinMsg PNULL == s_win_param_ptr"
#define MMIWIDGET_SNS_260_112_2_18_3_8_35_229 "[SNS]--------current_frame = %d-----total_frame = %d"
#define MMIWIDGET_SNS_349_112_2_18_3_8_35_230 "HTC_EX, cost_time ONE frame = %d ms"
#define MMIWIDGET_SNS_395_112_2_18_3_8_35_231 "HTC_SLIM, cost_time ONE frame = %d ms"
#define MMIWIDGET_SNS_414_112_2_18_3_8_35_232 "HTC_EX, cost_time ONE frame = %d ms"
#define MMIWIDGET_SNS_1451_112_2_18_3_8_37_233 "renwei --HandleSNSWidgetTpUp  NewEdit---widget_id=%d "
#define MMIWIDGET_SNS_1478_112_2_18_3_8_37_234 "renwei --HandleSNSWidgetTpUp  EnterTextDetail --widget_id=%d,index = %d"
#define MMIWIDGET_SNS_1980_112_2_18_3_8_38_235 "renwei --HandleWidgetEvtFromWre ---win_id=%d "
#define MMIWIDGET_SNS_2015_112_2_18_3_8_38_236 "renwei -HandleWidgetEvtFromWre--ENTER_DETAIL"
#define MMIWIDGET_SNS_2039_112_2_18_3_8_39_237 "renwei -HandleWidgetEvtFromWre--COMMOND_LOGIN:%d"
#define MMIWIDGET_SNS_2045_112_2_18_3_8_39_238 "renwei -HandleWidgetEvtFromWre--default event"
#define MMIWIDGET_SPEED_DIAL_267_112_2_18_3_8_39_239 "HandleWidgetSpeedDialWinMsg msg_id = %x"
#define MMIWIDGET_TIME_295_112_2_18_3_8_42_240 "[time]CreateTimeBoxLayer failed"
#define MMIWIDGET_TIME_375_112_2_18_3_8_42_241 "[time]:DisplayCurrentTime---%02d : %02d ---"
#define MMIWIDGET_TIME_458_112_2_18_3_8_42_242 "DisplayWeekAndDay ERROR! datewstr_len = %d"
#define MMIWIDGET_TIME_576_112_2_18_3_8_42_243 "[weather]:Weather_StopSockTimer"
#define MMIWIDGET_TIME_727_112_2_18_3_8_43_244 "[time]:HandleTimeUpdateTimer---%d/16---"
#define MMIWIDGET_TIME_905_112_2_18_3_8_43_245 "[TIME]:StartTimeUpdateSeTimer"
#define MMIWIDGET_WEATHER_999_112_2_18_3_8_46_246 "HandleWidgetWeatherWinMsg msg_id = %x"
#define MMIWIDGET_WEATHER_2024_112_2_18_3_8_48_247 "Please check time number img size"
#define MMIWIDGET_WEATHER_3151_112_2_18_3_8_50_248 "[weather]:Weather_StopSockTimer"
#define MMIWIDGET_WEATHER_3313_112_2_18_3_8_50_249 "[weather]:HandleTimeUpdateTimer---%d/16---"
#define MMIWIDGET_WEATHER_3495_112_2_18_3_8_51_250 "[weather]:Weather_StartSockTimer"
#define MMIWIDGET_WEATHER_3651_112_2_18_3_8_51_251 "[weather]:DisplayCurrentTime---%02d : %02d ---"
#define MMIWIDGET_WEATHER_3859_112_2_18_3_8_52_252 "MMIWIDGET_Weather_HandleWebKey cur_index = %d"
#define MMIWIDGET_WEATHER_3870_112_2_18_3_8_52_253 "MMIWIDGET_Weather_HandleWebKey cur_index = %d"
#define MMIWIDGET_WEATHER_3890_112_2_18_3_8_52_254 "MMIWIDGET_Weather_HandleWebKey cur_index = %d"
#define MMIWIDGET_WEATHER_3913_112_2_18_3_8_52_255 "MMIWIDGET_Weather_HandleWebKey cur_index = %d"
#define MMIWIDGET_WEATHER2_630_112_2_18_3_8_53_256 "HandleWidgetWeatherWinMsg msg_id = %x"
#define MMIWIDGET_WEATHER2_978_112_2_18_3_8_54_257 "[mmiwidget_weather_two.c]:DisplayCurrentTime---%02d : %02d ---"
#define MMIWIDGET_WEATHER2_1192_112_2_18_3_8_55_258 "[weather]:MMIWEATHER_GetCurTempRangeStr---"
#define MMIWIDGET_WEATHER2_1456_112_2_18_3_8_55_259 "HandleWidgetWeatherTwoTpDownMSG x= %d y= %d"
#define MMIQBTHEME_DOCKBAR_700_112_2_18_3_8_57_260 "pub_win_ptr == 0"
#define MMIQBTHEME_IDLEFUNCTION_159_112_2_18_3_8_59_261 "DrawSpecificPage() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_652_112_2_18_3_9_0_262 "valid grid index = %d"
#define MMIQBTHEME_IDLEFUNCTION_734_112_2_18_3_9_0_263 "MMIWIDGET_GetGridTpPressedItem() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_777_112_2_18_3_9_0_264 "MMIWIDGET_FindWidgetItem() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_799_112_2_18_3_9_0_265 "reset_id = %d"
#define MMIQBTHEME_IDLEFUNCTION_851_112_2_18_3_9_0_266 "MMIWIDGET_InsertShortcut() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_857_112_2_18_3_9_0_267 "widget_id = %d"
#define MMIQBTHEME_IDLEFUNCTION_945_112_2_18_3_9_0_268 "ActiveWidgetItem() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_979_112_2_18_3_9_0_269 "MMIWIDGET_DeactiveWidgetItem() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_984_112_2_18_3_9_0_270 "widget_id = %d"
#define MMIQBTHEME_IDLEFUNCTION_1036_112_2_18_3_9_0_271 "MMIWIDGET_RedrawWidgetItem() item_ptr == PNULL!"
#define MMIQBTHEME_IDLEFUNCTION_1055_112_2_18_3_9_0_272 "SetWidgetActive() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_1100_112_2_18_3_9_1_273 "RemoveShortcut() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_1240_112_2_18_3_9_1_274 "MMIWIDGET_SetChildWinRect window_rect:(%d, %d, %d, %d) is error!"
#define MMIQBTHEME_IDLEFUNCTION_1299_112_2_18_3_9_1_275 "DispatchMsgToAllChildWin() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_1326_112_2_18_3_9_1_276 "MMIWIDGET_GetWinHandleByWidgetId() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEFUNCTION_1354_112_2_18_3_9_1_277 "MMIWIDGET_GetWidgetIdByWinHandle() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_894_112_2_18_3_9_3_278 "MMIGRID_IDLE MMIAPIQBIDLE_HandleMsg win_id = %x,msg_id = %x"
#define MMIQBTHEME_IDLEWIN_1462_112_2_18_3_9_4_279 "DestroyAllWidgetItemData() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_1491_112_2_18_3_9_4_280 "GetItemPtr() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_1571_112_2_18_3_9_5_281 "ResetHitWidgetLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_1600_112_2_18_3_9_5_282 "ResetOneWidgetLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_2157_112_2_18_3_9_6_283 "ResetGridTPMoveInfo() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_2176_112_2_18_3_9_6_284 "SetWidgetTPHitType() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_2395_112_2_18_3_9_6_285 "from main menu: group id = %d, menu id = %d"
#define MMIQBTHEME_IDLEWIN_2408_112_2_18_3_9_6_286 "MMIAPIMENU_GetSlideMenuItemPtr = 0"
#define MMIQBTHEME_IDLEWIN_2461_112_2_18_3_9_6_287 "find  icon_ptr falied"
#define MMIQBTHEME_IDLEWIN_2464_112_2_18_3_9_6_288 "idle_grid_ptr->icon_ptr[%d].icon_info. group_id = %d, menu_id = %d"
#define MMIQBTHEME_IDLEWIN_2480_112_2_18_3_9_6_289 "MMIWIDGET_LayoutGridTblByRect falied"
#define MMIQBTHEME_IDLEWIN_2564_112_2_18_3_9_7_290 "MMIWIDGET_LayoutGridTblByRect falied"
#define MMIQBTHEME_IDLEWIN_2598_112_2_18_3_9_7_291 "MMIAPIMENU_GetSlideMenuItemPtr falied"
#define MMIQBTHEME_IDLEWIN_2651_112_2_18_3_9_7_292 "find icon ptr falied"
#define MMIQBTHEME_IDLEWIN_2660_112_2_18_3_9_7_293 "MMIWIDGET_LayoutGridTblByRect falied"
#define MMIQBTHEME_IDLEWIN_2763_112_2_18_3_9_7_294 "HandleGridAutoSlideEvent() PNULL == idle_grid_ptr"
#define MMIQBTHEME_IDLEWIN_3025_112_2_18_3_9_7_295 "HandleGridTpMove() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_3244_112_2_18_3_9_8_296 "DestroyWallpaperLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_3271_112_2_18_3_9_8_297 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_3293_112_2_18_3_9_8_298 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_3369_112_2_18_3_9_8_299 "CreateMoveLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_3435_112_2_18_3_9_8_300 "CreateMoveLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_3476_112_2_18_3_9_8_301 "DestroyMoveLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_4723_112_2_18_3_9_11_302 "ResetAllGridLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_5027_112_2_18_3_9_12_303 "failed"
#define MMIQBTHEME_IDLEWIN_5088_112_2_18_3_9_12_304 "p_item[%d].lcd_dev_info invalid"
#define MMIQBTHEME_IDLEWIN_5139_112_2_18_3_9_12_305 "GRAPH_ScaleImage failed"
#define MMIQBTHEME_IDLEWIN_5193_112_2_18_3_9_12_306 "GRAPH_ScaleImage failed"
#define MMIQBTHEME_IDLEWIN_5268_112_2_18_3_9_12_307 "DestroyElveLayer() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_IDLEWIN_5316_112_2_18_3_9_12_308 "DrawSpecificPage() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_INIT_899_112_2_18_3_9_16_309 "SaveWidgetInfoToNV() PNULL == idle_grid_ptr!"
#define MMIQBTHEME_ITEM_197_112_2_18_3_9_16_310 "pitem state = moving , type = preview index = %d"
#define MMIQBTHEME_ITEM_204_112_2_18_3_9_16_311 "tp point.x =%d , point.y =%d, pitem display area %d,%d,%d,%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WIDGET_TRC)
TRACE_MSG(MMIGRID_ALARM_311_112_2_18_3_6_53_0,"MMIGIRD_ALARM DisplayAlarmSwitch event_ptr = 0")
TRACE_MSG(MMIGRID_ALARM_316_112_2_18_3_6_53_1,"MMIGIRD_ALARM DisplayAlarmSwitch event_delay_ptr = 0")
TRACE_MSG(MMIGRID_ALARM_419_112_2_18_3_6_53_2,"MMIGIRD_ALARM DisplayAlarmSwitch event_ptr = 0")
TRACE_MSG(MMIGRID_ALARM_424_112_2_18_3_6_53_3,"MMIGIRD_ALARM DisplayAlarmSwitch event_delay_ptr = 0")
TRACE_MSG(MMIGRID_ALARM_628_112_2_18_3_6_54_4,"MMIGIRD_ALARM HandleGridAlarmTpUp event_ptr = 0")
TRACE_MSG(MMIGRID_ALARM_633_112_2_18_3_6_54_5,"MMIGIRD_ALARM HandleGridAlarmTpUp event_delay_ptr = 0")
TRACE_MSG(MMIGRID_DISPLAY_1044_112_2_18_3_6_56_6,"icon menu: group_id = %d, menu_id = %d")
TRACE_MSG(MMIGRID_IDLE_1443_112_2_18_3_7_0_7,"idle_grid_ptr is null")
TRACE_MSG(MMIGRID_IDLE_1548_112_2_18_3_7_0_8,"MMIGRID_IDLE CreateEditBgLayer error = %d")
TRACE_MSG(MMIGRID_IDLE_1559_112_2_18_3_7_0_9,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_2131_112_2_18_3_7_1_10,"MMIGRID_IDLE MMIAPIGRID_HandleMsg win_id = %x,msg_id = %x")
TRACE_MSG(MMIGRID_IDLE_3402_112_2_18_3_7_4_11,"DestroyWidgetItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_3535_112_2_18_3_7_4_12,"MMIGRID_IDLE MMIGRID_GetWinRect row_num == %d")
TRACE_MSG(MMIGRID_IDLE_3999_112_2_18_3_7_5_13,"MMIGRID CreateThumbViewLayer  == FALSE")
TRACE_MSG(MMIGRID_IDLE_4027_112_2_18_3_7_5_14,"MMIGRID ThumbOnePageLayer  == FALSE")
TRACE_MSG(MMIGRID_IDLE_4173_112_2_18_3_7_5_15,"MMIGRID_GRID HandleGridTpUp")
TRACE_MSG(MMIGRID_IDLE_4436_112_2_18_3_7_6_16,"SaveWidgetInfoToNV() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_4482_112_2_18_3_7_6_17,"GetGridTpPressedItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_4531_112_2_18_3_7_6_18,"SetWidgetTPHitType() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_4776_112_2_18_3_7_7_19,"from main menu: group id = %d, menu id = %d")
TRACE_MSG(MMIGRID_IDLE_4796_112_2_18_3_7_7_20,"idle_grid_ptr->icon_ptr[%d].icon_info. group_id = %d, menu_id = %d")
TRACE_MSG(MMIGRID_IDLE_5065_112_2_18_3_7_7_21,"MMIGRID_GRID HandleGridTpDown")
TRACE_MSG(MMIGRID_IDLE_5161_112_2_18_3_7_7_22,"RedrawWidgetItem() item_ptr == PNULL!")
TRACE_MSG(MMIGRID_IDLE_5223_112_2_18_3_7_7_23,"HandleGridAutoSlideEvent() PNULL == idle_grid_ptr")
TRACE_MSG(MMIGRID_IDLE_5785_112_2_18_3_7_9_24,"SetWidgetActive() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_5897_112_2_18_3_7_9_25,"DestroyElveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_5927_112_2_18_3_7_9_26,"DestroyElveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_5984_112_2_18_3_7_9_27,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6006_112_2_18_3_7_9_28,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6381_112_2_18_3_7_10_29,"InsertShortcut() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6418_112_2_18_3_7_10_30,"RemoveShortcut() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6500_112_2_18_3_7_10_31,"ActiveWidgetItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6533_112_2_18_3_7_10_32,"ActiveWidgetItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6598_112_2_18_3_7_10_33,"GetWinHandleByWidgetId() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_6626_112_2_18_3_7_10_34,"GetWidgetIdByWinHandle() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_7140_112_2_18_3_7_11_35,"MMIGRID_IDLE DrawSpecificPage wid is focues = %d")
TRACE_MSG(MMIGRID_IDLE_7969_112_2_18_3_7_13_36,"ResetAllGridLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_8104_112_2_18_3_7_13_37,"SetChildWinRect window_rect:(%d, %d, %d, %d) is error!")
TRACE_MSG(MMIGRID_IDLE_9063_112_2_18_3_7_16_38,"MMIGRID DisplayGridPageThumbView GRAPH_ScaleImage error")
TRACE_MSG(MMIGRID_IDLE_9091_112_2_18_3_7_16_39,"MMIGRID CreateThumbViewLayer  == FALSE")
TRACE_MSG(MMIGRID_IDLE_9102_112_2_18_3_7_16_40,"MMIGRID DisplayGridPageThumbView tar_buf == 0")
TRACE_MSG(MMIGRID_IDLE_9118_112_2_18_3_7_16_41,"MMIGRID ThumbOnePageLayer  == FALSE")
TRACE_MSG(MMIGRID_IDLE_9244_112_2_18_3_7_17_42,"MMIGRID_IDLE MMIGRID_InitWidgetsPosition ROW*COL == 0")
TRACE_MSG(MMIGRID_IDLE_9249_112_2_18_3_7_17_43,"MMIGRID_IDLE MMIGRID_InitWidgetsPosition row_num == %d")
TRACE_MSG(MMIGRID_IDLE_9425_112_2_18_3_7_17_44,"SetDisplayRect ERROR! tp_type = %d")
TRACE_MSG(MMIGRID_IDLE_10859_112_2_18_3_7_21_45,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_11293_112_2_18_3_7_22_46,"CreateMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_11442_112_2_18_3_7_22_47,"CreateMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_11480_112_2_18_3_7_22_48,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IDLE_11762_112_2_18_3_7_23_49,"DrawSpecificPage() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIGRID_IMGVIEWER_398_112_2_18_3_7_24_50,"MMIGRID_IMGVIEWER SetImgAnimData ctrl_idx = %d,photo_index =%d")
TRACE_MSG(MMIGRID_IMGVIEWER_400_112_2_18_3_7_24_51,"MMIGRID_IMGVIEWER SetImgAnimData s_imgviewer_info.s_files =%x")
TRACE_MSG(MMIGRID_IMGVIEWER_404_112_2_18_3_7_24_52,"MMIGRID_IMGVIEWER SetImgAnimData  MMIAPIFILEARRAY_Read failed")
TRACE_MSG(MMIGRID_IMGVIEWER_420_112_2_18_3_7_24_53,"MMIGRID_IMGVIEWER SetImgAnimData  time = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_573_112_2_18_3_7_24_54,"MMIGRID_IMGVIEWER win_handlement  time = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_575_112_2_18_3_7_24_55,"MMIGRID_IMGVIEWER win_handlement ctrl_index = %x ,result = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_768_112_2_18_3_7_25_56,"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x")
TRACE_MSG(MMIGRID_IMGVIEWER_771_112_2_18_3_7_25_57,"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x")
TRACE_MSG(MMIGRID_IMGVIEWER_773_112_2_18_3_7_25_58,"MMIGRID_IMGVIEWER UpdateFolderPhoto start time = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_784_112_2_18_3_7_25_59,"MMIGRID_IMGVIEWER UpdateFolderPhoto end time = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_787_112_2_18_3_7_25_60,"MMIGRID_IMGVIEWER UpdateFolderPhoto file_num = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_804_112_2_18_3_7_25_61,"MMIGRID_IMGVIEWER OpenPhotoFolderCallback enter file_num = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_810_112_2_18_3_7_25_62,"MMIGRID_IMGVIEWER UpdateFolderPhoto MMIAPIFILEARRAY_Read failed")
TRACE_MSG(MMIGRID_IMGVIEWER_828_112_2_18_3_7_25_63,"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x")
TRACE_MSG(MMIGRID_IMGVIEWER_831_112_2_18_3_7_25_64,"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x")
TRACE_MSG(MMIGRID_IMGVIEWER_833_112_2_18_3_7_25_65,"MMIGRID_IMGVIEWER MMIAPIFMM_SearchFileInPath start time = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_844_112_2_18_3_7_25_66,"MMIGRID_IMGVIEWER MMIAPIFMM_SearchFileInPath end time = %d")
TRACE_MSG(MMIGRID_IMGVIEWER_847_112_2_18_3_7_25_67,"MMIGRID_IMGVIEWER OpenPhotoFolderCallback file_num = %d")
TRACE_MSG(MMIGRID_PB_664_112_2_18_3_7_28_68,"[mmipb] MakeCall ret %d")
TRACE_MSG(MMIGRID_PB_747_112_2_18_3_7_28_69,"[mmipb] MakeCall ret %d")
TRACE_MSG(MMIGRID_PB_791_112_2_18_3_7_28_70,"MakeNumberCallByBCD PNULL == bcd_number_ptr")
TRACE_MSG(MMIGRID_PB_816_112_2_18_3_7_28_71,"[MMIPB]MakeNumberCallByString number_str_ptr == PNULL !")
TRACE_MSG(MMIGRID_PB_1090_112_2_18_3_7_29_72,"[mmipb] MakeCall ret %d")
TRACE_MSG(MMIWIDGET_1201_112_2_18_3_7_33_73,"total_num = %d\n")
TRACE_MSG(MMIWIDGET_1206_112_2_18_3_7_33_74,"shortcut[%d] = %d\n")
TRACE_MSG(MMIWIDGET_1219_112_2_18_3_7_33_75,"active item [%d] = %d, rect = {%d, %d, %d, %d}")
TRACE_MSG(MMIWIDGET_1500_112_2_18_3_7_34_76,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2088_112_2_18_3_7_35_77,"DispatchMsgToAllChildWin() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2184_112_2_18_3_7_35_78,"mmiwidget.c MMIWIDGET_Create() ctrl_id = %d is exist!")
TRACE_MSG(MMIWIDGET_2369_112_2_18_3_7_35_79,"MMIWIDGET_SetMaxItemByPtr() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2376_112_2_18_3_7_35_80,"MMIWIDGET_SetMaxItemByPtr() max_item_num is zero!")
TRACE_MSG(MMIWIDGET_2405_112_2_18_3_7_36_81,"DestroyWidgetItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2429_112_2_18_3_7_36_82,"DestroyAllWidgetItemData() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2463_112_2_18_3_7_36_83,"GetItemPtr() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2496_112_2_18_3_7_36_84,"UpdateAllWidgetItem() PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr!")
TRACE_MSG(MMIWIDGET_2549_112_2_18_3_7_36_85,"UpdateAllWidgetItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2848_112_2_18_3_7_36_86,"DisplayWidgetShortcut() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2909_112_2_18_3_7_37_87,"DisplayWidgetShortcut() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_2982_112_2_18_3_7_37_88,"FindWidgetItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3013_112_2_18_3_7_37_89,"IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3049_112_2_18_3_7_37_90,"IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3078_112_2_18_3_7_37_91,"IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3117_112_2_18_3_7_37_92,"CheckTotalWidgetNum() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3162_112_2_18_3_7_37_93,"ChangeWidgetWinRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3206_112_2_18_3_7_37_94,"ResetHitWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3230_112_2_18_3_7_37_95,"ResetOneWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3278_112_2_18_3_7_37_96,"HandleWidgetTpUp() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3285_112_2_18_3_7_37_97,"ResetHitWidgetLayer()00000 widget_id = %d")
TRACE_MSG(MMIWIDGET_3343_112_2_18_3_7_37_98,"VibrateWidgetItem() startup")
TRACE_MSG(MMIWIDGET_3578_112_2_18_3_7_38_99,"HandleWidgetTpUp() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3824_112_2_18_3_7_38_100,"SaveWidgetInfoToNV() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3895_112_2_18_3_7_39_101,"GetWidgetTpPressedItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3940_112_2_18_3_7_39_102,"ResetWidgetTPMoveInfo() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_3959_112_2_18_3_7_39_103,"SetWidgetTPHitType() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_4569_112_2_18_3_7_40_104,"IsValidShortcutOffset() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_4662_112_2_18_3_7_40_105,"IsValidShortcutOffset() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_4705_112_2_18_3_7_40_106,"HandleWidgetTpDown() hit_widget_id == MMIWIDGET_INVALID_ID!")
TRACE_MSG(MMIWIDGET_4790_112_2_18_3_7_40_107,"RedrawWidgetItem() item_ptr == PNULL!")
TRACE_MSG(MMIWIDGET_4810_112_2_18_3_7_40_108,"IsValidShortcutOffset() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_4896_112_2_18_3_7_41_109,"HandleWidgetAutoSlideEvent() PNULL == widget_ctrl_ptr")
TRACE_MSG(MMIWIDGET_4930_112_2_18_3_7_41_110,"HandleWidgetTpMove() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5091_112_2_18_3_7_41_111,"SetWidgetActive() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5172_112_2_18_3_7_41_112,"SetHitWidgetTopmost() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5318_112_2_18_3_7_41_113,"AddZorderItemTopmost() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5371_112_2_18_3_7_41_114,"SetZorderItemTopMostByWidgetId() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5393_112_2_18_3_7_42_115,"SetZorderItemTopMost() PNULL == widget_ctrl_ptr || PNULL == item_ptr!")
TRACE_MSG(MMIWIDGET_5427_112_2_18_3_7_42_116,"RedrawHitIcon() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5487_112_2_18_3_7_42_117,"ClearWidgetRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5507_112_2_18_3_7_42_118,"DestroyWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5531_112_2_18_3_7_42_119,"DestroyElveLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5555_112_2_18_3_7_42_120,"DestroyElveLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5579_112_2_18_3_7_42_121,"DestroyElveLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5603_112_2_18_3_7_42_122,"DestroyElveLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5627_112_2_18_3_7_42_123,"DestroyWallpaperLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5652_112_2_18_3_7_42_124,"DestroyWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5676_112_2_18_3_7_42_125,"DestroyWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5705_112_2_18_3_7_42_126,"CreateWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5780_112_2_18_3_7_42_127,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5803_112_2_18_3_7_42_128,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5826_112_2_18_3_7_42_129,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5850_112_2_18_3_7_42_130,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5874_112_2_18_3_7_42_131,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5898_112_2_18_3_7_43_132,"RunChildWidgetProc() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5924_112_2_18_3_7_43_133,"CreateWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_5970_112_2_18_3_7_43_134,"CreateWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6018_112_2_18_3_7_43_135,"CreateWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6068_112_2_18_3_7_43_136,"CreateWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6113_112_2_18_3_7_43_137,"CreateWallpaperLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6169_112_2_18_3_7_43_138,"CreateWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6238_112_2_18_3_7_43_139,"InsertShortcut() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6275_112_2_18_3_7_43_140,"InsertShortcut() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6297_112_2_18_3_7_43_141,"RemoveShortcut() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6337_112_2_18_3_7_43_142,"RemoveShortcut() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6390_112_2_18_3_7_43_143,"GetShortcutItemRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6444_112_2_18_3_7_44_144,"GetShortcutItemRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6497_112_2_18_3_7_44_145,"GetShortcutItemRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6531_112_2_18_3_7_44_146,"FindShortcutInsertPosByPoint() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6569_112_2_18_3_7_44_147,"FindShortcutPosByPoint() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6635_112_2_18_3_7_44_148,"SetWidgetActive() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6664_112_2_18_3_7_44_149,"ActiveWidgetItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6705_112_2_18_3_7_44_150,"ActiveWidgetItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6748_112_2_18_3_7_44_151,"DeactiveWidgetItem() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6779_112_2_18_3_7_44_152,"GetWinHandleByWidgetId() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6807_112_2_18_3_7_44_153,"GetWidgetIdByWinHandle() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_6875_112_2_18_3_7_44_154,"MMIWIDGET_SetShortcutOffset() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_7114_112_2_18_3_7_45_155,"RemoveWidgetItemPtr() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_7220_112_2_18_3_7_45_156,"SlideWidgetOneByOne() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_7444_112_2_18_3_7_46_157,"ChangeAllChildWinRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_7662_112_2_18_3_7_46_158,"BakAllActiveChildWinRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_7693_112_2_18_3_7_46_159,"RestoreAllActiveChildWinRect() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_8237_112_2_18_3_7_47_160,"CreateWidgetPageLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_8289_112_2_18_3_7_47_161,"MMIWIDGET_SetCurPageIndex() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_8394_112_2_18_3_7_47_162,"ResetAllWidgetLayer() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_8528_112_2_18_3_7_48_163,"SetChildWinRect window_rect:(%d, %d, %d, %d) is error!")
TRACE_MSG(MMIWIDGET_8631_112_2_18_3_7_48_164,"IsElveWidgetActive() PNULL == widget_ctrl_ptr!")
TRACE_MSG(MMIWIDGET_8669_112_2_18_3_7_48_165,"CopyPageLayerToMainLayer() pass = %dms")
TRACE_MSG(MMIWIDGET_ACLOCK_387_112_2_18_3_7_49_166,"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIWIDGET_ACLOCK_539_112_2_18_3_7_49_167,"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIWIDGET_ACLOCK_647_112_2_18_3_7_50_168,"MMIWIDGET_ACLOCK DisplayAClock rect = %d,%d,%d,%d")
TRACE_MSG(MMIWIDGET_ACLOCK_652_112_2_18_3_7_50_169,"MMIWIDGET_ACLOCK DisplayAClock lcd_id = %d")
TRACE_MSG(MMIWIDGET_ACLOCK_708_112_2_18_3_7_50_170,"MMIWIDGET_ACLOCK DisplayAClock SCI_ALLOCA error")
TRACE_MSG(MMIWIDGET_ACLOCK_722_112_2_18_3_7_50_171,"MMIWIDGET_ACLOCK DisplayAClock GRAPH_DrawClock error")
TRACE_MSG(MMIWIDGET_ACLOCK_952_112_2_18_3_7_50_172,"[MMIWIDGET]: StartTimer: the timer has started!")
TRACE_MSG(MMIWIDGET_ACLOCK_970_112_2_18_3_7_50_173,"[MMIWIDGET]: StopTimer: the timer has stopped!")
TRACE_MSG(MMIWIDGET_AUTOANSWER_LOG_283_112_2_18_3_7_53_174,"MMIASP this number has find a name ")
TRACE_MSG(MMIWIDGET_CALENDAR_555_112_2_18_3_7_56_175,"MMICALENDAR_GetMonthday error month=%d")
TRACE_MSG(MMIWIDGET_CALENDAR_582_112_2_18_3_7_56_176,"MMIWIDGET_Calendar_GetDateRect row >= MMIWIDGET_CALENDAR_LINENUM row=%d")
TRACE_MSG(MMIWIDGET_CALENDAR_588_112_2_18_3_7_56_177,"MMIWIDGET_Calendar_GetDateRect col >= MMIWIDGET_CALENDAR_COLNUM col=%d")
TRACE_MSG(MMIWIDGET_CALENDAR_631_112_2_18_3_7_56_178,"MMICALENDAR_GetWeekday month < 1 || month > 12, month=%d")
TRACE_MSG(MMIWIDGET_CALENDAR_637_112_2_18_3_7_56_179,"MMICALENDAR_GetWeekday day < 1 || day > 31, day=%d")
TRACE_MSG(MMIWIDGET_CALENDAR_673_112_2_18_3_7_56_180,"MMIWIDGET_Calendar_GetPosByDate PNULL == calendar_info_ptr || PNULL == row_ptr || PNULL == col_ptr")
TRACE_MSG(MMIWIDGET_CALENDAR_897_112_2_18_3_7_56_181,"MMIWIDGET_Calendar_DrawWeeks PNULL == calendar_info_ptr")
TRACE_MSG(MMIWIDGET_CALENDAR_1080_112_2_18_3_7_57_182,"MMIWIDGET_Calendar_DrawMonthDate PNULL != calendar_info_ptr || PNULL == rect_ptr")
TRACE_MSG(MMIWIDGET_CALENDAR_1313_112_2_18_3_7_57_183,"CALENDAR_GetFirstday:solar_month %d is error!")
TRACE_MSG(MMIWIDGET_CALENDAR_1332_112_2_18_3_7_57_184,"MMIWIDGET_Calendar_DrawCurrentDate PNULL != calendar_info_ptr")
TRACE_MSG(MMIWIDGET_CALLLOG_248_112_2_18_3_7_58_185,"MMIWIDGET_GetCallLogTextStyle PNULL == style_ptr")
TRACE_MSG(MMIWIDGET_CALLLOG_599_112_2_18_3_7_59_186,"MMIWIDGET_DisplayMissLogDetail arraycall_info failed to alloc memory")
TRACE_MSG(MMIWIDGET_CALLLOG_751_112_2_18_3_7_59_187,"MMIWIDGET_Calllog_EnterWidgetCLNoPrivacyProtect arraycall_info failed to alloc memory")
TRACE_MSG(MMIWIDGET_COUNTEDTIME_503_112_2_18_3_8_1_188,"[MMIWIDGET]: StartTimer: the timer has started!")
TRACE_MSG(MMIWIDGET_COUNTEDTIME_520_112_2_18_3_8_1_189,"[MMIWIDGET]: StopTimer: the timer has stopped!")
TRACE_MSG(MMIWIDGET_COUNTEDTIME_731_112_2_18_3_8_2_190,"MMIWIDGET_GetTextStyle style_ptr")
TRACE_MSG(MMIWIDGET_DCD_389_112_2_18_3_8_4_191,"[MMIDCD]:MMIWIDGETDCD_SetData() enter")
TRACE_MSG(MMIWIDGET_DCD_412_112_2_18_3_8_4_192,"[MMIDCD]:MMIWIDGETDCD_SetData() exit")
TRACE_MSG(MMIWIDGET_DCD_657_112_2_18_3_8_4_193,"GUITAB_SwitchToNextItem FALSE!")
TRACE_MSG(MMIWIDGET_DCD_684_112_2_18_3_8_4_194,"MMIWIDGETDCD_SwitchToNextItem type error! %d")
TRACE_MSG(MMIWIDGET_DCD_754_112_2_18_3_8_4_195,"MMICD will show file")
TRACE_MSG(MMIWIDGET_DUAL_ACLOCK_288_112_2_18_3_8_7_196,"dual aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIWIDGET_DUAL_ACLOCK_409_112_2_18_3_8_8_197,"dual aclock Convert888To888Or565 (int32)bk_width < start_x || (int32)bk_height < start_y")
TRACE_MSG(MMIWIDGET_DUAL_ACLOCK_639_112_2_18_3_8_8_198,"MMIWIDGET_DUAL_ACLOCK DisplayAClock GRAPH_DrawClock error")
TRACE_MSG(MMIWIDGET_DUAL_ACLOCK_1053_112_2_18_3_8_9_199,"[MMIWIDGET]: StartTimer: the timer has started!")
TRACE_MSG(MMIWIDGET_DUAL_ACLOCK_1069_112_2_18_3_8_9_200,"[MMIWIDGET]: StopTimer: the timer has stopped!")
TRACE_MSG(MMIWIDGET_MEMO_324_112_2_18_3_8_13_201,"MMIWIDGET_Memo_ReadMemoFile PNULL == g_edit_str.wstr_ptr")
TRACE_MSG(MMIWIDGET_MEMO_331_112_2_18_3_8_13_202,"MMIWIDGET_Memo_ReadMemoFile PNULL == g_memo_edit_str[i].wstr_ptr")
TRACE_MSG(MMIWIDGET_MEMO_579_112_2_18_3_8_14_203,"[MMIWIDGET_Memo]: HandleMemoEditWinMsg = before MMIWIDGET_Memo_ReadMemoFile! ")
TRACE_MSG(MMIWIDGET_MEMO_653_112_2_18_3_8_14_204,"MMIWIDGET_Memo_GetMemoTextStyle style_ptr")
TRACE_MSG(MMIWIDGET_MEMO_947_112_2_18_3_8_15_205,"GetCurMemoIdx PNULL == item_ptr !")
TRACE_MSG(MMIWIDGET_MP3_803_112_2_18_3_8_17_206,"MMIWIDGET_MP3 HandleWidgetMp3WinMsg MSG_TIMER MMIAPIMP3_GetAudioState = %d")
TRACE_MSG(MMIWIDGET_MP3_1548_112_2_18_3_8_18_207,"GetMp3WidgetTextStyle PNULL == style_ptr")
TRACE_MSG(MMIWIDGET_MP3_1723_112_2_18_3_8_19_208,"HandleProgressBarTpKey  tp_x=%d error!")
TRACE_MSG(MMIWIDGET_MP3_1732_112_2_18_3_8_19_209,"MMIMP3_HandleProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d")
TRACE_MSG(MMIWIDGET_MP3_1752_112_2_18_3_8_19_210,"MMIMP3_HandleProgressBarTpKey:yaohua get mp3 current offset result %d is error!")
TRACE_MSG(MMIWIDGET_MP3_1758_112_2_18_3_8_19_211,"MMIMP3_HandleProgressBarTpKey:cur_state %d don't handle!")
TRACE_MSG(MMIWIDGET_MP3_1781_112_2_18_3_8_19_212,"GetMp3CurrentOffsetByTp, Error: is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!")
TRACE_MSG(MMIWIDGET_MP3_1791_112_2_18_3_8_19_213,"GetMp3CurrentOffsetByTp:cur_slide_x_offset = %d,tp_x_offset = %d")
TRACE_MSG(MMIWIDGET_MP3_1812_112_2_18_3_8_19_214,"GetMp3CurrentOffsetByTp:is_forward = %d,mp3_total_offset = %ld")
TRACE_MSG(MMIWIDGET_MP3_1831_112_2_18_3_8_19_215,"[MMIMP3] ForwardOrBackwardMp3 mp3_offset = %d")
TRACE_MSG(MMIWIDGET_MP3_1836_112_2_18_3_8_19_216,"ForwardOrBackwardMp3, cur_state = %d error!")
TRACE_MSG(MMIWIDGET_MP3_1863_112_2_18_3_8_19_217,"ForwardOrBackwardMp3:audio seek result = %d")
TRACE_MSG(MMIWIDGET_NETSEARCH_325_112_2_18_3_8_20_218,"MMIWIDGET_NetSearch_GetNetSearchTextStyle style_ptr")
TRACE_MSG(MMIWIDGET_NETWORK_NAME_205_112_2_18_3_8_21_219,"MMIWIDGET_Network_GetNetWorkTextStyle PNULL == style_ptr")
TRACE_MSG(MMIWIDGET_NEWSMS_225_112_2_18_3_8_22_220,"MMIWIDGET_NewSms_GetNewSmsTextStyle PNULL == style_ptr")
TRACE_MSG(MMIWIDGET_NEWSMS_514_112_2_18_3_8_23_221,"MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr")
TRACE_MSG(MMIWIDGET_NEWSMS_566_112_2_18_3_8_23_222,"MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr")
TRACE_MSG(MMIWIDGET_PB_851_112_2_18_3_8_25_223,"mmipb_wintab: HandleInterfaceType storage = %d")
TRACE_MSG(MMIWIDGET_PB_894_112_2_18_3_8_25_224,"MMIWIDGETPB_MakeNumberCallByBCD PNULL == bcd_number_ptr")
TRACE_MSG(MMIWIDGET_PB_923_112_2_18_3_8_25_225,"[MMIPB]MakeNumberCallByString number_str_ptr == PNULL !")
TRACE_MSG(MMIWIDGET_PB_1828_112_2_18_3_8_27_226,"failed to alloc name memory, display empty for exception.")
TRACE_MSG(MMIWIDGET_SCHEDULE_149_112_2_18_3_8_30_227,"HandleWidgetScheduleWinMsg msg_id = %x")
TRACE_MSG(MMIWIDGET_SCHEDULE_162_112_2_18_3_8_30_228,"HandleWidgetScheduleWinMsg PNULL == s_win_param_ptr")
TRACE_MSG(MMIWIDGET_SNS_260_112_2_18_3_8_35_229,"[SNS]--------current_frame = %d-----total_frame = %d")
TRACE_MSG(MMIWIDGET_SNS_349_112_2_18_3_8_35_230,"HTC_EX, cost_time ONE frame = %d ms")
TRACE_MSG(MMIWIDGET_SNS_395_112_2_18_3_8_35_231,"HTC_SLIM, cost_time ONE frame = %d ms")
TRACE_MSG(MMIWIDGET_SNS_414_112_2_18_3_8_35_232,"HTC_EX, cost_time ONE frame = %d ms")
TRACE_MSG(MMIWIDGET_SNS_1451_112_2_18_3_8_37_233,"renwei --HandleSNSWidgetTpUp  NewEdit---widget_id=%d ")
TRACE_MSG(MMIWIDGET_SNS_1478_112_2_18_3_8_37_234,"renwei --HandleSNSWidgetTpUp  EnterTextDetail --widget_id=%d,index = %d")
TRACE_MSG(MMIWIDGET_SNS_1980_112_2_18_3_8_38_235,"renwei --HandleWidgetEvtFromWre ---win_id=%d ")
TRACE_MSG(MMIWIDGET_SNS_2015_112_2_18_3_8_38_236,"renwei -HandleWidgetEvtFromWre--ENTER_DETAIL")
TRACE_MSG(MMIWIDGET_SNS_2039_112_2_18_3_8_39_237,"renwei -HandleWidgetEvtFromWre--COMMOND_LOGIN:%d")
TRACE_MSG(MMIWIDGET_SNS_2045_112_2_18_3_8_39_238,"renwei -HandleWidgetEvtFromWre--default event")
TRACE_MSG(MMIWIDGET_SPEED_DIAL_267_112_2_18_3_8_39_239,"HandleWidgetSpeedDialWinMsg msg_id = %x")
TRACE_MSG(MMIWIDGET_TIME_295_112_2_18_3_8_42_240,"[time]CreateTimeBoxLayer failed")
TRACE_MSG(MMIWIDGET_TIME_375_112_2_18_3_8_42_241,"[time]:DisplayCurrentTime---%02d : %02d ---")
TRACE_MSG(MMIWIDGET_TIME_458_112_2_18_3_8_42_242,"DisplayWeekAndDay ERROR! datewstr_len = %d")
TRACE_MSG(MMIWIDGET_TIME_576_112_2_18_3_8_42_243,"[weather]:Weather_StopSockTimer")
TRACE_MSG(MMIWIDGET_TIME_727_112_2_18_3_8_43_244,"[time]:HandleTimeUpdateTimer---%d/16---")
TRACE_MSG(MMIWIDGET_TIME_905_112_2_18_3_8_43_245,"[TIME]:StartTimeUpdateSeTimer")
TRACE_MSG(MMIWIDGET_WEATHER_999_112_2_18_3_8_46_246,"HandleWidgetWeatherWinMsg msg_id = %x")
TRACE_MSG(MMIWIDGET_WEATHER_2024_112_2_18_3_8_48_247,"Please check time number img size")
TRACE_MSG(MMIWIDGET_WEATHER_3151_112_2_18_3_8_50_248,"[weather]:Weather_StopSockTimer")
TRACE_MSG(MMIWIDGET_WEATHER_3313_112_2_18_3_8_50_249,"[weather]:HandleTimeUpdateTimer---%d/16---")
TRACE_MSG(MMIWIDGET_WEATHER_3495_112_2_18_3_8_51_250,"[weather]:Weather_StartSockTimer")
TRACE_MSG(MMIWIDGET_WEATHER_3651_112_2_18_3_8_51_251,"[weather]:DisplayCurrentTime---%02d : %02d ---")
TRACE_MSG(MMIWIDGET_WEATHER_3859_112_2_18_3_8_52_252,"MMIWIDGET_Weather_HandleWebKey cur_index = %d")
TRACE_MSG(MMIWIDGET_WEATHER_3870_112_2_18_3_8_52_253,"MMIWIDGET_Weather_HandleWebKey cur_index = %d")
TRACE_MSG(MMIWIDGET_WEATHER_3890_112_2_18_3_8_52_254,"MMIWIDGET_Weather_HandleWebKey cur_index = %d")
TRACE_MSG(MMIWIDGET_WEATHER_3913_112_2_18_3_8_52_255,"MMIWIDGET_Weather_HandleWebKey cur_index = %d")
TRACE_MSG(MMIWIDGET_WEATHER2_630_112_2_18_3_8_53_256,"HandleWidgetWeatherWinMsg msg_id = %x")
TRACE_MSG(MMIWIDGET_WEATHER2_978_112_2_18_3_8_54_257,"[mmiwidget_weather_two.c]:DisplayCurrentTime---%02d : %02d ---")
TRACE_MSG(MMIWIDGET_WEATHER2_1192_112_2_18_3_8_55_258,"[weather]:MMIWEATHER_GetCurTempRangeStr---")
TRACE_MSG(MMIWIDGET_WEATHER2_1456_112_2_18_3_8_55_259,"HandleWidgetWeatherTwoTpDownMSG x= %d y= %d")
TRACE_MSG(MMIQBTHEME_DOCKBAR_700_112_2_18_3_8_57_260,"pub_win_ptr == 0")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_159_112_2_18_3_8_59_261,"DrawSpecificPage() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_652_112_2_18_3_9_0_262,"valid grid index = %d")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_734_112_2_18_3_9_0_263,"MMIWIDGET_GetGridTpPressedItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_777_112_2_18_3_9_0_264,"MMIWIDGET_FindWidgetItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_799_112_2_18_3_9_0_265,"reset_id = %d")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_851_112_2_18_3_9_0_266,"MMIWIDGET_InsertShortcut() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_857_112_2_18_3_9_0_267,"widget_id = %d")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_945_112_2_18_3_9_0_268,"ActiveWidgetItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_979_112_2_18_3_9_0_269,"MMIWIDGET_DeactiveWidgetItem() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_984_112_2_18_3_9_0_270,"widget_id = %d")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1036_112_2_18_3_9_0_271,"MMIWIDGET_RedrawWidgetItem() item_ptr == PNULL!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1055_112_2_18_3_9_0_272,"SetWidgetActive() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1100_112_2_18_3_9_1_273,"RemoveShortcut() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1240_112_2_18_3_9_1_274,"MMIWIDGET_SetChildWinRect window_rect:(%d, %d, %d, %d) is error!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1299_112_2_18_3_9_1_275,"DispatchMsgToAllChildWin() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1326_112_2_18_3_9_1_276,"MMIWIDGET_GetWinHandleByWidgetId() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEFUNCTION_1354_112_2_18_3_9_1_277,"MMIWIDGET_GetWidgetIdByWinHandle() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_894_112_2_18_3_9_3_278,"MMIGRID_IDLE MMIAPIQBIDLE_HandleMsg win_id = %x,msg_id = %x")
TRACE_MSG(MMIQBTHEME_IDLEWIN_1462_112_2_18_3_9_4_279,"DestroyAllWidgetItemData() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_1491_112_2_18_3_9_4_280,"GetItemPtr() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_1571_112_2_18_3_9_5_281,"ResetHitWidgetLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_1600_112_2_18_3_9_5_282,"ResetOneWidgetLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2157_112_2_18_3_9_6_283,"ResetGridTPMoveInfo() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2176_112_2_18_3_9_6_284,"SetWidgetTPHitType() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2395_112_2_18_3_9_6_285,"from main menu: group id = %d, menu id = %d")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2408_112_2_18_3_9_6_286,"MMIAPIMENU_GetSlideMenuItemPtr = 0")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2461_112_2_18_3_9_6_287,"find  icon_ptr falied")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2464_112_2_18_3_9_6_288,"idle_grid_ptr->icon_ptr[%d].icon_info. group_id = %d, menu_id = %d")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2480_112_2_18_3_9_6_289,"MMIWIDGET_LayoutGridTblByRect falied")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2564_112_2_18_3_9_7_290,"MMIWIDGET_LayoutGridTblByRect falied")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2598_112_2_18_3_9_7_291,"MMIAPIMENU_GetSlideMenuItemPtr falied")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2651_112_2_18_3_9_7_292,"find icon ptr falied")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2660_112_2_18_3_9_7_293,"MMIWIDGET_LayoutGridTblByRect falied")
TRACE_MSG(MMIQBTHEME_IDLEWIN_2763_112_2_18_3_9_7_294,"HandleGridAutoSlideEvent() PNULL == idle_grid_ptr")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3025_112_2_18_3_9_7_295,"HandleGridTpMove() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3244_112_2_18_3_9_8_296,"DestroyWallpaperLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3271_112_2_18_3_9_8_297,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3293_112_2_18_3_9_8_298,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3369_112_2_18_3_9_8_299,"CreateMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3435_112_2_18_3_9_8_300,"CreateMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_3476_112_2_18_3_9_8_301,"DestroyMoveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_4723_112_2_18_3_9_11_302,"ResetAllGridLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_5027_112_2_18_3_9_12_303,"failed")
TRACE_MSG(MMIQBTHEME_IDLEWIN_5088_112_2_18_3_9_12_304,"p_item[%d].lcd_dev_info invalid")
TRACE_MSG(MMIQBTHEME_IDLEWIN_5139_112_2_18_3_9_12_305,"GRAPH_ScaleImage failed")
TRACE_MSG(MMIQBTHEME_IDLEWIN_5193_112_2_18_3_9_12_306,"GRAPH_ScaleImage failed")
TRACE_MSG(MMIQBTHEME_IDLEWIN_5268_112_2_18_3_9_12_307,"DestroyElveLayer() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_IDLEWIN_5316_112_2_18_3_9_12_308,"DrawSpecificPage() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_INIT_899_112_2_18_3_9_16_309,"SaveWidgetInfoToNV() PNULL == idle_grid_ptr!")
TRACE_MSG(MMIQBTHEME_ITEM_197_112_2_18_3_9_16_310,"pitem state = moving , type = preview index = %d")
TRACE_MSG(MMIQBTHEME_ITEM_204_112_2_18_3_9_16_311,"tp point.x =%d , point.y =%d, pitem display area %d,%d,%d,%d")
END_TRACE_MAP(MMI_APP_WIDGET_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WIDGET_TRC_H_

