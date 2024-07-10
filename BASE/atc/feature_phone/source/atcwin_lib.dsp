# Microsoft Developer Studio Project File - Name="atc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=atc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "atcwin_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "atcwin_lib.mak" CFG="atc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "atc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "atc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "atc"
# PROP Scc_LocalPath "..\..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "atc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "atc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /I "../../../../MS_Ref/export/inc" /I "../../../../common/export/inc" /I "../../../../RTOS/export/inc" /I "../../../../MS_Customize/export/inc" /I "../../../../chip_drv/export/inc" /I "../../../../DAPS/export/inc" /I "../../../../char_lib/export/inc" /I "../../../../DAPS/source/wbxml_parser/inc" /I "../../../../DAPS/source/wsp_header_decoder/inc" /I "../../../../DAPS/source/xml_expat/inc" /I "../../../../l4/export/inc" /I "../../../../PARSER/export/inc" /I "../../../../sim/export/inc" /I "../../../../Third-party/cstar/include" /I "../../../../Third-party/S9/include" /I "../../../../Third-party/InterSound4/h" /I "../../../../Third-party/InterSound4/Lib" /I "../../../../logsave/export/inc" /I "../../../../Third-party/hw/include" /I "../../../../PS/export/inc" /I "../../../../Layer1/export/inc" /I "../../../../atc/export/inc" /D "JBED_ARM" /D "USB_VUART" /D "MUSIC_PLAYER_SUPPORT" /D "BROWSER_GZIP_SUPPORT" /D "CALCULATOR_SUPPORT" /D "EBOOK_SUPPORT" /D "GAME_SUPPORT" /D "GAME_BOXMAN_SUPPORT" /D "KEY_RING_THROUGH_RECEIVER" /D "TP_RING_THROUGH_RECEIVER" /D "CALL_TIMES_SUPPORT" /D "TTS_FILE_RES" /D "CAMERA_SUPPORT" /D "SEARCH_SUPPORT" /D "COUNTEDTIME_SUPPORT" /D "WORLD_CLOCK_SUPPORT" /D "GAME_LINK_SUPPORT" /D "ENG_SUPPORT" /D "CARD_LOG" /D "IM_ENGLISH_SUPPORT" /D "TOUCH_PANEL_SUPPORT" /D "BLUETOOTH_HFG_PLAY_MUSIC_SUPPORT" /D "JAVA_OTA_SUPPORT" /D "P3D_WHEN_MP3BG_SUPPORT" /D "FPGA_6800H_POWER_ON" /D "PLATFORM_SC6800H" /D "AUDIO_SC6800H" /D "DC_SC6800H" /D "MV_EFFECT_SUPPORT" /D "CAMERA_SENSOR_ANGLE_0" /D "CAMERA_SCREEN_PORTRAIT" /D "BURST_VOICE_EVERY" /D "BROWSER_TOOLBAR" /D "QQ_SUPPORT_2009" /D "AAC_A2DP_SUPPORT" /D "AAC_BACKGROUND_SUPPORT" /D "AAC_SEEK_SUPPORT" /D "AAC_SUPPORT" /D "AMR_A2DP_SUPPORT" /D "AMR_BACKGROUND_SUPPORT" /D "AMR_SEEK_SUPPORT" /D "AMR_SUPPORT" /D "ASP_SUPPORT" /D "ASSERT_TYPE_SUPPORT" /D "ASSERT_TYPE_SUPPORT_ASSERT" /D "ATC_SUPPORT" /D "ATC_SUPPORT_FULL" /D "ATV_SUPPORT" /D "ATV_TYPE" /D "ATV_TYPE_TLG1120" /D "AVI_DEC_SUPPORT" /D "AVI_ENC_SUPPORT" /D "BB_DRAM_TYPE" /D "BB_DRAM_TYPE_32MB_16BIT" /D "BLUETOOTH_SUPPORT" /D "BLUETOOTH_SUPPORT_CSR_BC4_6" /D "BMP_DEC_SUPPORT" /D "BMP_DEC_SUPPORT_BMP_SW" /D "BROWSER_SUPPORT" /D "BROWSER_SUPPORT_DORADO" /D "BT_A2DP_SAMPLERATE_RECONFIG_SUPPORT" /D "BT_A2DP_SUPPORT" /D "BT_AVRCP_SUPPORT" /D "BT_FTP_SERVER_SUPPORT" /D "BT_OPP_SUPPORT" /D "CHIP_ENDIAN" /D "CHIP_MARK_NONE" /D "CHIP_VER" /D "CHIP_VER_6800H1" /D "CMOS_SENSOR_TYPE" /D "CMOS_SENSOR_TYPE_OV7670" /D "CMOS_SUB_SENSOR_TYPE_NONE" /D "DISPLAY_PIXELDEPTH_MAX" /D "DISPLAY_PIXELDEPTH_MAX_32" /D "DOLPHIN_SUPPORT" /D "EQ_EXE_ON_CHIP" /D "EXPRESS_PROCESS_IN_DSP" /D "FDN_SUPPORT" /D "FLV_DEC_SUPPORT" /D "FM_SUPPORT_NONE" /D "FONT_TYPE_SUPPORT" /D "FONT_TYPE_SUPPORT_VECTOR" /D "GIF_DEC_SUPPORT" /D "GIF_DEC_SUPPORT_GIF_SW" /D "GPRSMPDP_SUPPORT" /D "GPRS_SUPPORT" /D "GZIP_DEC_SUPPORT" /D "GZIP_DEC_SUPPORT_GZIP_SW" /D "H264_DEC_SUPPORT" /D "HALFRATE_SUPPORT" /D "HOME_LOCATION_INQUIRE_SUPPORT" /D "HTTP_SUPPORT" /D "IM_ENGINE" /D "IM_ENGINE_CSTAR" /D "IM_HANDWRITING" /D "IM_HANDWRITING_HANWANG" /D "IM_SIMP_CHINESE_SUPPORT" /D "JAVA_SUPPORT" /D "JAVA_SUPPORT_IA" /D "JPG_DEC_SUPPORT" /D "JPG_DEC_SUPPORT_JPG_SW" /D "JPG_DEC_TYPE_SUPPORT" /D "JPG_DEC_TYPE_SUPPORT_BASELINE_PROGRESSIVE" /D "JPG_ENC_SUPPORT" /D "JPG_ENC_SUPPORT_JPG_SW" /D "KEYPAD_TYPE" /D "KEYPAD_TYPE_TYPICAL_KEYPAD" /D "MAINLCD_DEV_SIZE" /D "MAINLCD_DEV_SIZE_240X320" /D "MAINLCD_LOGIC_ANGLE" /D "MAINLCD_LOGIC_ANGLE_0" /D "MAINLCD_SIZE" /D "MAINLCD_SIZE_240X320" /D "MAINMENU_STYLE" /D "MAINMENU_STYLE_ICON_TEXT" /D "MAIN_LCD_DISPLAY_LAYER_NUM" /D "MAIN_LCD_DISPLAY_LAYER_NUM_1" /D "MIDI_A2DP_SUPPORT" /D "MIDI_BACKGROUND_SUPPORT" /D "MIDI_DECODER_OPTIMIZED" /D "MIDI_SUPPORT"
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../build/sc6800h_sp6801h_builddir/lib/atc.lib"

!ENDIF 

# Begin Target

# Name "atc - Win32 Release"
# Name "atc - Win32 Debug"
# Begin Group "atc_source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\c\at_common.c
NODEP_CPP_AT_CO=\
	".\c\at_common.h"\
	".\c\cmddef.h"\
	".\c\os_api.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc.c
NODEP_CPP_ATC_C=\
	".\c\atc.h"\
	".\c\atc_basic_cmd.h"\
	".\c\atc_common.h"\
	".\c\atc_gprs_modem.h"\
	".\c\atc_gsm.h"\
	".\c\atc_gsm_sms.h"\
	".\c\atc_gsm_ss.h"\
	".\c\atc_gsm_stk.h"\
	".\c\atc_malloc.h"\
	".\c\atc_phonebook.h"\
	".\c\atc_port.h"\
	".\c\atc_ppp.h"\
	".\c\atc_tcp_if.h"\
	".\c\bridgelayer.h"\
	".\c\dal_audio.h"\
	".\c\dal_chr.h"\
	".\c\dal_power.h"\
	".\c\env_atc_signal.h"\
	".\c\mn_api.h"\
	".\c\mnclassmark_api.h"\
	".\c\sci_service.h"\
	".\c\sig_code.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_audio_data.c
NODEP_CPP_ATC_A=\
	".\c\dal_audio.h"\
	".\c\midi_1.dat"\
	".\c\midi_10.dat"\
	".\c\midi_11.dat"\
	".\c\midi_12.dat"\
	".\c\midi_13.dat"\
	".\c\midi_14.dat"\
	".\c\midi_15.dat"\
	".\c\midi_16.dat"\
	".\c\midi_17.dat"\
	".\c\midi_18.dat"\
	".\c\midi_19.dat"\
	".\c\midi_2.dat"\
	".\c\midi_20.dat"\
	".\c\midi_21.dat"\
	".\c\midi_22.dat"\
	".\c\midi_23.dat"\
	".\c\midi_24.dat"\
	".\c\midi_25.dat"\
	".\c\midi_26.dat"\
	".\c\midi_27.dat"\
	".\c\midi_28.dat"\
	".\c\midi_29.dat"\
	".\c\midi_3.dat"\
	".\c\midi_30.dat"\
	".\c\midi_31.dat"\
	".\c\midi_32.dat"\
	".\c\midi_33.dat"\
	".\c\midi_34.dat"\
	".\c\midi_35.dat"\
	".\c\midi_36.dat"\
	".\c\midi_37.dat"\
	".\c\midi_38.dat"\
	".\c\midi_39.dat"\
	".\c\midi_4.dat"\
	".\c\midi_40.dat"\
	".\c\midi_41.dat"\
	".\c\midi_42.dat"\
	".\c\midi_43.dat"\
	".\c\midi_44.dat"\
	".\c\midi_45.dat"\
	".\c\midi_5.dat"\
	".\c\midi_6.dat"\
	".\c\midi_7.dat"\
	".\c\midi_8.dat"\
	".\c\midi_9.dat"\
	".\c\smaf_1.dat"\
	".\c\smaf_10.dat"\
	".\c\smaf_11.dat"\
	".\c\smaf_12.dat"\
	".\c\smaf_13.dat"\
	".\c\smaf_14.dat"\
	".\c\smaf_15.dat"\
	".\c\smaf_16.dat"\
	".\c\smaf_17.dat"\
	".\c\smaf_18.dat"\
	".\c\smaf_19.dat"\
	".\c\smaf_2.dat"\
	".\c\smaf_20.dat"\
	".\c\smaf_21.dat"\
	".\c\smaf_22.dat"\
	".\c\smaf_23.dat"\
	".\c\smaf_24.dat"\
	".\c\smaf_25.dat"\
	".\c\smaf_26.dat"\
	".\c\smaf_27.dat"\
	".\c\smaf_28.dat"\
	".\c\smaf_29.dat"\
	".\c\smaf_3.dat"\
	".\c\smaf_30.dat"\
	".\c\smaf_4.dat"\
	".\c\smaf_5.dat"\
	".\c\smaf_6.dat"\
	".\c\smaf_7.dat"\
	".\c\smaf_8.dat"\
	".\c\smaf_9.dat"\
	".\c\wave.dat"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_basic_cmd.c
NODEP_CPP_ATC_B=\
	".\c\admm.h"\
	".\c\admm_II.h"\
	".\c\admm_III.h"\
	".\c\atc.h"\
	".\c\atc_basic_cmd.h"\
	".\c\atc_common.h"\
	".\c\atc_gsm_sms.h"\
	".\c\atc_gsm_stk.h"\
	".\c\aud_gen.h"\
	".\c\aud_proc_exp.h"\
	".\c\audio_api.h"\
	".\c\audio_prod.h"\
	".\c\custom_tone_adp.h"\
	".\c\dal_chr.h"\
	".\c\dal_power.h"\
	".\c\dal_time.h"\
	".\c\dtmf_tone_adp.h"\
	".\c\efs.h"\
	".\c\env_atc_signal.h"\
	".\c\gpio_drv.h"\
	".\c\layer1_audio.h"\
	".\c\layer1_engineering.h"\
	".\c\mnclassmark_api.h"\
	".\c\nv_item_id.h"\
	".\c\power.h"\
	".\c\prod_param.h"\
	".\c\ps_eng.h"\
	".\c\ref_outport.h"\
	".\c\run_mode.h"\
	".\c\sci_service.h"\
	".\c\tasks_id.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_common.c
NODEP_CPP_ATC_CO=\
	".\c\atc_basic_cmd.h"\
	".\c\atc_common.h"\
	".\c\dal_audio.h"\
	".\c\efs.h"\
	".\c\env_atc_signal.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_gprs_modem.c
NODEP_CPP_ATC_G=\
	".\c\app_tcp_if.h"\
	".\c\atc_gprs_modem.h"\
	".\c\atc_plus_gprs.h"\
	".\c\atc_tcp_if.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm.c
NODEP_CPP_ATC_GS=\
	".\c\atc_basic_cmd.h"\
	".\c\atc_common.h"\
	".\c\atc_gsm.h"\
	".\c\atc_gsm_ss.h"\
	".\c\atc_gsm_stk.h"\
	".\c\audio_api.h"\
	".\c\env_atc_signal.h"\
	".\c\layer1_engineering.h"\
	".\c\sci_api.h"\
	".\c\sig_code.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm_sms.c
NODEP_CPP_ATC_GSM=\
	".\c\atc.h"\
	".\c\atc_common.h"\
	".\c\atc_gsm_sms.h"\
	".\c\sci_array.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm_ss.c
NODEP_CPP_ATC_GSM_=\
	".\c\atc_common.h"\
	".\c\atc_gsm.h"\
	".\c\atc_gsm_ss.h"\
	".\c\atc_gsm_stk.h"\
	".\c\audio_api.h"\
	".\c\audio_prod.h"\
	".\c\env_atc_signal.h"\
	".\c\sci_codec.h"\
	".\c\sig_code.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm_stk.c
NODEP_CPP_ATC_GSM_S=\
	".\c\atc_common.h"\
	".\c\atc_gsm_ss.h"\
	".\c\atc_gsm_stk.h"\
	".\c\env_atc_signal.h"\
	".\c\sig_code.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_info.c
NODEP_CPP_ATC_I=\
	".\c\atc.h"\
	".\c\atc_basic_cmd.h"\
	".\c\atc_gprs_modem.h"\
	".\c\atc_gsm.h"\
	".\c\atc_gsm_sms.h"\
	".\c\atc_gsm_ss.h"\
	".\c\atc_gsm_stk.h"\
	".\c\atc_phonebook.h"\
	".\c\atc_plus_gprs.h"\
	".\c\sci_types.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_lex.c
NODEP_CPP_ATC_L=\
	".\c\atc_common.h"\
	".\c\atc_def.h"\
	".\c\atc_malloc.h"\
	".\c\atc_yacc.h"\
	".\c\sci_types.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_main.c
NODEP_CPP_ATC_M=\
	".\c\atc_port.h"\
	".\c\env_atc_signal.h"\
	".\c\sci_api.h"\
	".\c\tasks_id.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_malloc.c
NODEP_CPP_ATC_MA=\
	".\c\atc_malloc.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_phonebook.c
NODEP_CPP_ATC_P=\
	".\c\atc.h"\
	".\c\atc_common.h"\
	".\c\atc_phonebook.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_plus_gprs.c
NODEP_CPP_ATC_PL=\
	".\c\atc.h"\
	".\c\atc_basic_cmd.h"\
	".\c\atc_plus_gprs.h"\
	".\c\atc_tcp_if.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_public_phone.c
NODEP_CPP_ATC_PU=\
	".\c\atc_public_phone.h"\
	".\c\mmi_pt.h"\
	".\c\mn.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_sleep.c
NODEP_CPP_ATC_S=\
	".\c\atc_sleep.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_test_win32.c
NODEP_CPP_ATC_T=\
	".\c\atc.h"\
	".\c\atc_common.h"\
	".\c\atc_def.h"\
	".\c\atc_malloc.h"\
	
# End Source File
# Begin Source File

SOURCE=.\c\atc_version.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_yacc.c
NODEP_CPP_ATC_Y=\
	".\c\atc.h"\
	".\c\atc_common.h"\
	".\c\atc_malloc.h"\
	".\c\sci_types.h"\
	
# End Source File
# End Group
# End Target
# Begin Group "build"

# PROP Default_Filter ""
# End Group
# End Project
