MCFLAG_OPT = -DJBED_ARM -DUSB_LOG -DUSB_VUART

MINCPATH += $(PS_INCPATH) $(L1_INCPATH) 

#$(ATC_INCPATH)

MINCPATH +=  build/$(PROJECT)_builddir/tmp  

MINCPATH += l4/export/inc 
MINCPATH += CAF/Template/include 
MINCPATH += ms_ref/export/inc 
MINCPATH += chip_drv/export/inc/outdated 
MINCPATH += sim/export/inc 

MINCPATH += $(MMI_DIR)/source/resource
MINCPATH += $(MMI_DIR)/source/mmi_gui/include 
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/include
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include 
MINCPATH += $(MMI_DIR)/source/mmi_utility/h 

MINCPATH += $(MMI_DIR)/source/mmi_app/common/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/custom/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/idle/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fmm/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mp3/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/audio/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pb/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/setting/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/theme/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cc/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/bt/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/phone/h 

MINCPATH +=	$(MMI_DIR)/source/flashUI/device/h $(MMI_DIR)/source/flashUI/task/h \
            $(MMI_DIR)/source/flashUI/mal/h $(MMI_DIR)/source/mmi_app/app/flash_win/h
            
MSRCPATH += $(MMI_DIR)/source/flashUI/device/c $(MMI_DIR)/source/flashUI/task/c \
            $(MMI_DIR)/source/flashUI/mal/c 
	

SOURCES += FlashTask_main.c FlashTask_msg.c FlashTask_player.c \
		amuse_config_device.c FlashTask_MAL.c mal_phone_context.c mal_phone_func.c mal_phone_interface.c
		

SOURCES += amuse_dbitmap_device.c amuse_display_device.c amuse_file_device.c amuse_math.c amuse_host_api.c \
			amuse_key_device.c amuse_memory_device.c amuse_network_device.c amuse_fscommand_device.c	\
			amuse_sound_device.c amuse_thread_device.c amuse_time_device.c amuse_jpeg_device.c	amuse_jpeg_hw_decoder.c


