/******************************************************************************
 ** File Name:      mmi_app_livewallpaper_trc.h                                         *
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
//trace_id:190
#ifndef _MMI_APP_LIVEWALLPAPER_TRC_H_
#define _MMI_APP_LIVEWALLPAPER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_LIVEWALLPAPER_327_112_2_18_2_31_48_0 "LivewallpaperInstanceGetBgWidthAndHeight bg_width:%d,bg_height:%d"
#define MMI_LIVEWALLPAPER_448_112_2_18_2_31_48_1 "LiveWallPaperFrame:There's no live wallpaper init funtion!"
#define MMI_LIVEWALLPAPER_453_112_2_18_2_31_48_2 "LiveWallPaperFrame:Can't find the valid live wallpaper!"
#define MMI_LIVEWALLPAPER_474_112_2_18_2_31_49_3 "LiveWallPaperFrame:There's no live wallpaper DynamicConstruct funtion!"
#define MMI_LIVEWALLPAPER_479_112_2_18_2_31_49_4 "LiveWallPaperFrame:Play Init need instance ptr!"
#define MMI_LIVEWALLPAPER_501_112_2_18_2_31_49_5 "LiveWallPaperFrame:There's no live wallpaper Destroy funtion!"
#define MMI_LIVEWALLPAPER_506_112_2_18_2_31_49_6 "LiveWallPaperFrame:Destroy need instance ptr!"
#define MMI_LIVEWALLPAPER_528_112_2_18_2_31_49_7 "LiveWallPaperFrame:There's no live wallpaper HandleEvent funtion!"
#define MMI_LIVEWALLPAPER_533_112_2_18_2_31_49_8 "LiveWallPaperFrame:HandleEvent need instance ptr!"
#define MMI_LIVEWALLPAPER_593_112_2_18_2_31_49_9 "LivewallpaperSourceDataInit Faild:current_instance_ptr:%x"
#define MMI_LIVEWALLPAPER_598_112_2_18_2_31_49_10 "LivewallpaperSourceDataInit Faild:Frame is not init!"
#define MMI_LIVEWALLPAPER_622_112_2_18_2_31_49_11 "LivewallpaperStartCalculate Faild:is_initialized:%d"
#define MMI_LIVEWALLPAPER_800_112_2_18_2_31_49_12 "LivewallpaperCalculate Faild:dest buffer:%x"
#define MMI_LIVEWALLPAPER_837_112_2_18_2_31_49_13 "LivewallpaperCalculate Faild:src buffer:%x"
#define MMI_LIVEWALLPAPER_856_112_2_18_2_31_49_14 "LivewallpaperCalculate Time:%d"
#define MMI_LIVEWALLPAPER_862_112_2_18_2_31_49_15 "LivewallpaperCalculate Faild:the buffer is PNULL!"
#define MMI_LIVEWALLPAPER_923_112_2_18_2_31_49_16 "@@Livewallpaper UdiskMount:%d \n"
#define MMI_LIVEWALLPAPER_1096_112_2_18_2_31_50_17 "MMIAPILIVEWALLPAPER_SetLiveWallPaperType error!"
#define MMI_LIVEWALLPAPER_AQUARIUM_603_112_2_18_2_31_54_18 "Ben, AquariumConstruct, Fish_srcARGB_temp buffer failed"
#define MMI_LIVEWALLPAPER_AQUARIUM_812_112_2_18_2_31_54_19 "Ben, MMIIDLE_Aquarium_CreateImageLayer buf Alloca error, type = %d ms"
#define MMI_LIVEWALLPAPER_AQUARIUM_911_112_2_18_2_31_54_20 "Ben, SUNSHINE buffer merge failed"
#define MMI_LIVEWALLPAPER_AQUARIUM_987_112_2_18_2_31_54_21 "Bubbling buffer failed"
#define MMI_LIVEWALLPAPER_AQUARIUM_1083_112_2_18_2_31_55_22 "Food buffer failed"
#define MMI_LIVEWALLPAPER_GALAXY_128_112_2_18_2_32_2_23 "[Star] GalaxyUpdate-----1-----time = %d "
#define MMI_LIVEWALLPAPER_GALAXY_147_112_2_18_2_32_2_24 "[Star] GalaxyUpdate-----2-----time = %d "
#define MMI_LIVEWALLPAPER_GIRLSWING_712_112_2_18_2_32_4_25 "YOON, cost_time girl_swing = %d ms"
#define MMI_LIVEWALLPAPER_LIGHTGRID_299_112_2_18_2_32_6_26 "SaveLightGridTPPoint  entry"
#define MMI_LIVEWALLPAPER_POP_862_112_2_18_2_32_16_27 "[POP_POS]---------x = %d"
#define MMI_LIVEWALLPAPER_POP_881_112_2_18_2_32_16_28 "[LIVE_WALLPAPER]---------fg_pos_info.x = %d"
#define MMI_LIVEWALLPAPER_POP_882_112_2_18_2_32_16_29 "[LIVE_WALLPAPER]---------TPMove_oft.x = %d"
#define MMI_LIVEWALLPAPER_RAINBOWCITY_218_112_2_18_2_32_18_30 "Livewallpaper:rainbowcity_CreateImageLayer Faild!"
#define MMI_LIVEWALLPAPER_RAINBOWCITY_363_112_2_18_2_32_18_31 "Livewallpaper:rainbowcityDrawBg Faild"
#define MMI_LIVEWALLPAPER_SAKURA_510_112_2_18_2_32_21_32 "--Rotate Image before-- angle:%d  "
#define MMI_LIVEWALLPAPER_SAKURA_523_112_2_18_2_32_21_33 "--Rotate Image after-- angle:%d  "
#define MMI_LIVEWALLPAPER_SAKURA_617_112_2_18_2_32_21_34 "[MMI_Rotate_Sakura_Buffer Fail] - angle: %d\n"
#define MMI_LIVEWALLPAPER_SEAWORLD_483_112_2_18_2_32_25_35 "--Rotate Image before-- angle:%d  "
#define MMI_LIVEWALLPAPER_SEAWORLD_496_112_2_18_2_32_25_36 "--Rotate Image after-- angle:%d  "
#define MMI_LIVEWALLPAPER_SEAWORLD_655_112_2_18_2_32_25_37 "[MMI_Rotate_Fish_Buffer Fail] - angle: %d\n"
#define MMI_LIVEWALLPAPER_SWEETTREE_1369_112_2_18_2_32_30_38 "SweetTreeConstruct()"
#define MMI_LIVEWALLPAPER_SWEETTREE_1613_112_2_18_2_32_31_39 "SweetTreeDrawBg()"
#define MMI_LIVEWALLPAPER_SWEETTREE_1718_112_2_18_2_32_31_40 "MMIIDLE_ChangeGirlSwayStrongeLevel()"
#define MMI_LIVEWALLPAPER_SWEETTREE_1740_112_2_18_2_32_31_41 "MMIIDLE_ChangeBirdState()"
#define MMI_LIVEWALLPAPER_SWEETTREE_1765_112_2_18_2_32_31_42 "MMIIDLE_ChangeSquirrelState"
#define MMI_LIVEWALLPAPER_SWEETTREE_1787_112_2_18_2_32_31_43 "MMIIDLE_ChangeButterflyState"
#define MMI_LIVEWALLPAPER_SWEETTREE_1810_112_2_18_2_32_31_44 "MMIIDLE_ChangeRabbitState"
#define MMI_LIVEWALLPAPER_SWEETTREE_1828_112_2_18_2_32_31_45 "CalcTPVavlidRectOfGirlSwing"
#define MMI_LIVEWALLPAPER_SWEETTREE_1859_112_2_18_2_32_31_46 "enter MMIIDLE_GirlSwing"
#define MMI_LIVEWALLPAPER_SWEETTREE_1908_112_2_18_2_32_32_47 "exit MMIIDLE_GirlSwing"
#define MMI_LIVEWALLPAPER_SWEETTREE_1920_112_2_18_2_32_32_48 "MMIIDLE_DisplayBirdofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_1952_112_2_18_2_32_32_49 "MMIIDLE_DisplayBirdNestofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_1976_112_2_18_2_32_32_50 "MMIIDLE_DisplaySquirrelofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2002_112_2_18_2_32_32_51 "DisplayStandRabbitOfSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2028_112_2_18_2_32_32_52 "MMIIDLE_DisplayButterflyofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2054_112_2_18_2_32_32_53 "MMIIDLE_PlayBirdTouchedAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2115_112_2_18_2_32_32_54 "MMIIDLE_PlayBirdSwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2154_112_2_18_2_32_32_55 "MMIIDLE_PlayBirdGoNestAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2198_112_2_18_2_32_32_56 "MMIIDLE_PlayBirdFlyAwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2238_112_2_18_2_32_32_57 "MMIIDLE_PlayBirdFlyBackAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2278_112_2_18_2_32_32_58 "MMIIDLE_PlayButterflySwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2317_112_2_18_2_32_32_59 "MMIIDLE_PlayButterflyFlyAwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2358_112_2_18_2_32_32_60 "MMIIDLE_PlaySquirrelCaveAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2416_112_2_18_2_32_33_61 "MMIIDLE_PlaySquirrelSwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2456_112_2_18_2_32_33_62 "MMIIDLE_PlaySquirrelRunAwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2510_112_2_18_2_32_33_63 "MMIIDLE_PlaySquirrelRunBackAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2565_112_2_18_2_32_33_64 "MMIIDLE_PlayButterflyFlyBackAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2605_112_2_18_2_32_33_65 "MMIIDLE_PlayRabbitRunningAwayAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2646_112_2_18_2_32_33_66 "MMIIDLE_PlayRabbitRunningBackAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2686_112_2_18_2_32_33_67 "MMIIDLE_PlayRabbitTouchedAnimofSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_2742_112_2_18_2_32_33_68 "MMIIDLE_PlayGirlSwayLegAnim"
#define MMI_LIVEWALLPAPER_SWEETTREE_2805_112_2_18_2_32_33_69 "SweetTreeCalculate"
#define MMI_LIVEWALLPAPER_SWEETTREE_2978_112_2_18_2_32_34_70 "SweetTreeHandleTpPressDown"
#define MMI_LIVEWALLPAPER_SWEETTREE_3032_112_2_18_2_32_34_71 "MMIIDLE_RestParamOfSweetTreeLoseFocus"
#define MMI_LIVEWALLPAPER_SWEETTREE_3055_112_2_18_2_32_34_72 "FreeAllBufferOfSweetTree"
#define MMI_LIVEWALLPAPER_SWEETTREE_3240_112_2_18_2_32_34_73 "SweetTreeDestruct"
#define MMI_LIVEWALLPAPER_SWEETTREE_3253_112_2_18_2_32_34_74 "SweetTreeHandleEvent"
#define MMI_LIVEWALLPAPER_WINDMILL_365_112_2_18_2_32_35_75 "WINDMILLGetMic_dB, start sampling_type = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_371_112_2_18_2_32_35_76 "WINDMILLGetMic_dB, s_is_mic_sample_enb = %d,mic_db = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_388_112_2_18_2_32_35_77 "WINDMILLEnableMicSample, s_is_mic_sample_enb = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_401_112_2_18_2_32_35_78 "[MMIWINDMILL]:WINDMILLMicCallBack event=%d"
#define MMI_LIVEWALLPAPER_WINDMILL_430_112_2_18_2_32_35_79 "[MMIWINDMILL]:WINDMILLRequestVirtualHandle audio_srv_handle  entry audio_srv_handle =%d"
#define MMI_LIVEWALLPAPER_WINDMILL_440_112_2_18_2_32_36_80 "[MMIWINDMILL]:WINDMILLRequestVirtualHandle audio_srv_handle =%d"
#define MMI_LIVEWALLPAPER_WINDMILL_452_112_2_18_2_32_36_81 "[MMIWINDMILL]:WINDMILLRequestVirtualHandle result =%d"
#define MMI_LIVEWALLPAPER_WINDMILL_486_112_2_18_2_32_36_82 "[LIVEWALLPAPER] WINDMILLConstruct 1"
#define MMI_LIVEWALLPAPER_WINDMILL_614_112_2_18_2_32_36_83 "[LIVEWALLPAPER]  WHILE-----------IN"
#define MMI_LIVEWALLPAPER_WINDMILL_619_112_2_18_2_32_36_84 "[LIVEWALLPAPER]  WHILE-----------OUT"
#define MMI_LIVEWALLPAPER_WINDMILL_642_112_2_18_2_32_36_85 "[LIVEWALLPAPER] windmill layer construct failed   count = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_726_112_2_18_2_32_36_86 "[WHOLE_TIME], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_805_112_2_18_2_32_36_87 "[YOON_CALC], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_818_112_2_18_2_32_36_88 "WINDMILLCalculate, s_is_mic_sample_enb = %d,"
#define MMI_LIVEWALLPAPER_WINDMILL_839_112_2_18_2_32_36_89 "harry, db_pre = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_859_112_2_18_2_32_36_90 "harry, s_wing_speed = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_879_112_2_18_2_32_36_91 "harry, windmill_wing_angle = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_880_112_2_18_2_32_36_92 "harry, windmill_sun_1_angle = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_881_112_2_18_2_32_36_93 "harry, windmill_sun_2_angle = %d"
#define MMI_LIVEWALLPAPER_WINDMILL_900_112_2_18_2_32_36_94 "[YOON_MEMCPY], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_921_112_2_18_2_32_36_95 "[YOON_ROTATE], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_1078_112_2_18_2_32_37_96 "[YOON_DRAW], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_1082_112_2_18_2_32_37_97 "[YOON_TOTAL], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_1245_112_2_18_2_32_37_98 "[YOON_SHADOW_BAR], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_1281_112_2_18_2_32_37_99 "[YOON_SHADOW_WING], CALC cost time = %d ms"
#define MMI_LIVEWALLPAPER_WINDMILL_1309_112_2_18_2_32_37_100 "[YOON_SHADOW_CENTRE], CALC cost time = %d ms"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_LIVEWALLPAPER_TRC)
TRACE_MSG(MMI_LIVEWALLPAPER_327_112_2_18_2_31_48_0,"LivewallpaperInstanceGetBgWidthAndHeight bg_width:%d,bg_height:%d")
TRACE_MSG(MMI_LIVEWALLPAPER_448_112_2_18_2_31_48_1,"LiveWallPaperFrame:There's no live wallpaper init funtion!")
TRACE_MSG(MMI_LIVEWALLPAPER_453_112_2_18_2_31_48_2,"LiveWallPaperFrame:Can't find the valid live wallpaper!")
TRACE_MSG(MMI_LIVEWALLPAPER_474_112_2_18_2_31_49_3,"LiveWallPaperFrame:There's no live wallpaper DynamicConstruct funtion!")
TRACE_MSG(MMI_LIVEWALLPAPER_479_112_2_18_2_31_49_4,"LiveWallPaperFrame:Play Init need instance ptr!")
TRACE_MSG(MMI_LIVEWALLPAPER_501_112_2_18_2_31_49_5,"LiveWallPaperFrame:There's no live wallpaper Destroy funtion!")
TRACE_MSG(MMI_LIVEWALLPAPER_506_112_2_18_2_31_49_6,"LiveWallPaperFrame:Destroy need instance ptr!")
TRACE_MSG(MMI_LIVEWALLPAPER_528_112_2_18_2_31_49_7,"LiveWallPaperFrame:There's no live wallpaper HandleEvent funtion!")
TRACE_MSG(MMI_LIVEWALLPAPER_533_112_2_18_2_31_49_8,"LiveWallPaperFrame:HandleEvent need instance ptr!")
TRACE_MSG(MMI_LIVEWALLPAPER_593_112_2_18_2_31_49_9,"LivewallpaperSourceDataInit Faild:current_instance_ptr:%x")
TRACE_MSG(MMI_LIVEWALLPAPER_598_112_2_18_2_31_49_10,"LivewallpaperSourceDataInit Faild:Frame is not init!")
TRACE_MSG(MMI_LIVEWALLPAPER_622_112_2_18_2_31_49_11,"LivewallpaperStartCalculate Faild:is_initialized:%d")
TRACE_MSG(MMI_LIVEWALLPAPER_800_112_2_18_2_31_49_12,"LivewallpaperCalculate Faild:dest buffer:%x")
TRACE_MSG(MMI_LIVEWALLPAPER_837_112_2_18_2_31_49_13,"LivewallpaperCalculate Faild:src buffer:%x")
TRACE_MSG(MMI_LIVEWALLPAPER_856_112_2_18_2_31_49_14,"LivewallpaperCalculate Time:%d")
TRACE_MSG(MMI_LIVEWALLPAPER_862_112_2_18_2_31_49_15,"LivewallpaperCalculate Faild:the buffer is PNULL!")
TRACE_MSG(MMI_LIVEWALLPAPER_923_112_2_18_2_31_49_16,"@@Livewallpaper UdiskMount:%d \n")
TRACE_MSG(MMI_LIVEWALLPAPER_1096_112_2_18_2_31_50_17,"MMIAPILIVEWALLPAPER_SetLiveWallPaperType error!")
TRACE_MSG(MMI_LIVEWALLPAPER_AQUARIUM_603_112_2_18_2_31_54_18,"Ben, AquariumConstruct, Fish_srcARGB_temp buffer failed")
TRACE_MSG(MMI_LIVEWALLPAPER_AQUARIUM_812_112_2_18_2_31_54_19,"Ben, MMIIDLE_Aquarium_CreateImageLayer buf Alloca error, type = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_AQUARIUM_911_112_2_18_2_31_54_20,"Ben, SUNSHINE buffer merge failed")
TRACE_MSG(MMI_LIVEWALLPAPER_AQUARIUM_987_112_2_18_2_31_54_21,"Bubbling buffer failed")
TRACE_MSG(MMI_LIVEWALLPAPER_AQUARIUM_1083_112_2_18_2_31_55_22,"Food buffer failed")
TRACE_MSG(MMI_LIVEWALLPAPER_GALAXY_128_112_2_18_2_32_2_23,"[Star] GalaxyUpdate-----1-----time = %d ")
TRACE_MSG(MMI_LIVEWALLPAPER_GALAXY_147_112_2_18_2_32_2_24,"[Star] GalaxyUpdate-----2-----time = %d ")
TRACE_MSG(MMI_LIVEWALLPAPER_GIRLSWING_712_112_2_18_2_32_4_25,"YOON, cost_time girl_swing = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_LIGHTGRID_299_112_2_18_2_32_6_26,"SaveLightGridTPPoint  entry")
TRACE_MSG(MMI_LIVEWALLPAPER_POP_862_112_2_18_2_32_16_27,"[POP_POS]---------x = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_POP_881_112_2_18_2_32_16_28,"[LIVE_WALLPAPER]---------fg_pos_info.x = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_POP_882_112_2_18_2_32_16_29,"[LIVE_WALLPAPER]---------TPMove_oft.x = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_RAINBOWCITY_218_112_2_18_2_32_18_30,"Livewallpaper:rainbowcity_CreateImageLayer Faild!")
TRACE_MSG(MMI_LIVEWALLPAPER_RAINBOWCITY_363_112_2_18_2_32_18_31,"Livewallpaper:rainbowcityDrawBg Faild")
TRACE_MSG(MMI_LIVEWALLPAPER_SAKURA_510_112_2_18_2_32_21_32,"--Rotate Image before-- angle:%d  ")
TRACE_MSG(MMI_LIVEWALLPAPER_SAKURA_523_112_2_18_2_32_21_33,"--Rotate Image after-- angle:%d  ")
TRACE_MSG(MMI_LIVEWALLPAPER_SAKURA_617_112_2_18_2_32_21_34,"[MMI_Rotate_Sakura_Buffer Fail] - angle: %d\n")
TRACE_MSG(MMI_LIVEWALLPAPER_SEAWORLD_483_112_2_18_2_32_25_35,"--Rotate Image before-- angle:%d  ")
TRACE_MSG(MMI_LIVEWALLPAPER_SEAWORLD_496_112_2_18_2_32_25_36,"--Rotate Image after-- angle:%d  ")
TRACE_MSG(MMI_LIVEWALLPAPER_SEAWORLD_655_112_2_18_2_32_25_37,"[MMI_Rotate_Fish_Buffer Fail] - angle: %d\n")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1369_112_2_18_2_32_30_38,"SweetTreeConstruct()")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1613_112_2_18_2_32_31_39,"SweetTreeDrawBg()")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1718_112_2_18_2_32_31_40,"MMIIDLE_ChangeGirlSwayStrongeLevel()")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1740_112_2_18_2_32_31_41,"MMIIDLE_ChangeBirdState()")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1765_112_2_18_2_32_31_42,"MMIIDLE_ChangeSquirrelState")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1787_112_2_18_2_32_31_43,"MMIIDLE_ChangeButterflyState")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1810_112_2_18_2_32_31_44,"MMIIDLE_ChangeRabbitState")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1828_112_2_18_2_32_31_45,"CalcTPVavlidRectOfGirlSwing")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1859_112_2_18_2_32_31_46,"enter MMIIDLE_GirlSwing")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1908_112_2_18_2_32_32_47,"exit MMIIDLE_GirlSwing")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1920_112_2_18_2_32_32_48,"MMIIDLE_DisplayBirdofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1952_112_2_18_2_32_32_49,"MMIIDLE_DisplayBirdNestofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_1976_112_2_18_2_32_32_50,"MMIIDLE_DisplaySquirrelofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2002_112_2_18_2_32_32_51,"DisplayStandRabbitOfSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2028_112_2_18_2_32_32_52,"MMIIDLE_DisplayButterflyofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2054_112_2_18_2_32_32_53,"MMIIDLE_PlayBirdTouchedAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2115_112_2_18_2_32_32_54,"MMIIDLE_PlayBirdSwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2154_112_2_18_2_32_32_55,"MMIIDLE_PlayBirdGoNestAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2198_112_2_18_2_32_32_56,"MMIIDLE_PlayBirdFlyAwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2238_112_2_18_2_32_32_57,"MMIIDLE_PlayBirdFlyBackAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2278_112_2_18_2_32_32_58,"MMIIDLE_PlayButterflySwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2317_112_2_18_2_32_32_59,"MMIIDLE_PlayButterflyFlyAwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2358_112_2_18_2_32_32_60,"MMIIDLE_PlaySquirrelCaveAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2416_112_2_18_2_32_33_61,"MMIIDLE_PlaySquirrelSwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2456_112_2_18_2_32_33_62,"MMIIDLE_PlaySquirrelRunAwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2510_112_2_18_2_32_33_63,"MMIIDLE_PlaySquirrelRunBackAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2565_112_2_18_2_32_33_64,"MMIIDLE_PlayButterflyFlyBackAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2605_112_2_18_2_32_33_65,"MMIIDLE_PlayRabbitRunningAwayAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2646_112_2_18_2_32_33_66,"MMIIDLE_PlayRabbitRunningBackAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2686_112_2_18_2_32_33_67,"MMIIDLE_PlayRabbitTouchedAnimofSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2742_112_2_18_2_32_33_68,"MMIIDLE_PlayGirlSwayLegAnim")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2805_112_2_18_2_32_33_69,"SweetTreeCalculate")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_2978_112_2_18_2_32_34_70,"SweetTreeHandleTpPressDown")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_3032_112_2_18_2_32_34_71,"MMIIDLE_RestParamOfSweetTreeLoseFocus")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_3055_112_2_18_2_32_34_72,"FreeAllBufferOfSweetTree")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_3240_112_2_18_2_32_34_73,"SweetTreeDestruct")
TRACE_MSG(MMI_LIVEWALLPAPER_SWEETTREE_3253_112_2_18_2_32_34_74,"SweetTreeHandleEvent")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_365_112_2_18_2_32_35_75,"WINDMILLGetMic_dB, start sampling_type = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_371_112_2_18_2_32_35_76,"WINDMILLGetMic_dB, s_is_mic_sample_enb = %d,mic_db = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_388_112_2_18_2_32_35_77,"WINDMILLEnableMicSample, s_is_mic_sample_enb = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_401_112_2_18_2_32_35_78,"[MMIWINDMILL]:WINDMILLMicCallBack event=%d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_430_112_2_18_2_32_35_79,"[MMIWINDMILL]:WINDMILLRequestVirtualHandle audio_srv_handle  entry audio_srv_handle =%d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_440_112_2_18_2_32_36_80,"[MMIWINDMILL]:WINDMILLRequestVirtualHandle audio_srv_handle =%d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_452_112_2_18_2_32_36_81,"[MMIWINDMILL]:WINDMILLRequestVirtualHandle result =%d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_486_112_2_18_2_32_36_82,"[LIVEWALLPAPER] WINDMILLConstruct 1")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_614_112_2_18_2_32_36_83,"[LIVEWALLPAPER]  WHILE-----------IN")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_619_112_2_18_2_32_36_84,"[LIVEWALLPAPER]  WHILE-----------OUT")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_642_112_2_18_2_32_36_85,"[LIVEWALLPAPER] windmill layer construct failed   count = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_726_112_2_18_2_32_36_86,"[WHOLE_TIME], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_805_112_2_18_2_32_36_87,"[YOON_CALC], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_818_112_2_18_2_32_36_88,"WINDMILLCalculate, s_is_mic_sample_enb = %d,")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_839_112_2_18_2_32_36_89,"harry, db_pre = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_859_112_2_18_2_32_36_90,"harry, s_wing_speed = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_879_112_2_18_2_32_36_91,"harry, windmill_wing_angle = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_880_112_2_18_2_32_36_92,"harry, windmill_sun_1_angle = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_881_112_2_18_2_32_36_93,"harry, windmill_sun_2_angle = %d")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_900_112_2_18_2_32_36_94,"[YOON_MEMCPY], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_921_112_2_18_2_32_36_95,"[YOON_ROTATE], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_1078_112_2_18_2_32_37_96,"[YOON_DRAW], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_1082_112_2_18_2_32_37_97,"[YOON_TOTAL], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_1245_112_2_18_2_32_37_98,"[YOON_SHADOW_BAR], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_1281_112_2_18_2_32_37_99,"[YOON_SHADOW_WING], CALC cost time = %d ms")
TRACE_MSG(MMI_LIVEWALLPAPER_WINDMILL_1309_112_2_18_2_32_37_100,"[YOON_SHADOW_CENTRE], CALC cost time = %d ms")
END_TRACE_MAP(MMI_APP_LIVEWALLPAPER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_LIVEWALLPAPER_TRC_H_

