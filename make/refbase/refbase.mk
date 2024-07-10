
MCFLAG_OPT   	= -DCHIP_SC6600C -D_CALIBRATION_VER_0009_ -DSC6600M -D_RXLEVEL_PRECISION_

ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
MCFLAG_OPT      += -D_ATC_ONLY
endif

ifeq ($(strip $(CARD_LOG)), TRUE)
MCFLAG_OPT      += -DCARD_LOG
endif

ifeq ($(strip $(AUTO_DOWNLOAD_SUPPORT)), TRUE)
MCFLAG_OPT      += -DAUTO_DOWNLOAD_SUPPORT
endif

#MCFLAG_OPT	+= -DTASK_ANALYZER


PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))
	
MINCPATH =  MS_Ref/source/base/inc 					\
			MS_Ref/source/aud_config/inc_export 	\
			MS_Ref/source/mux/inc					\
			MS_Ref/source/isp_service/inc			\
			MS_Ref/source/image_proc/inc \
			BASE/l4/export/inc  							\
			MS_Customize/export/inc 				\
			MS_Ref/export/inc                       \
			chip_drv/export/inc 					\
			chip_drv/chip_plf/export/inc            \
			BASE/sim/export/inc 							\
			chip_drv/chip_module/audio/codec/sprd/v2    \
			$(ATC_INCPATH) 							\
			MS_Customize/source/product/config/$(strip $(PROD_CONFIG))
MINCPATH += chip_drv/export/inc/outdated
MINCPATH +=$(PS_INCPATH) $(L1_INCPATH) 	

ifeq ($(strip $(CARD_LOG)), TRUE)
MINCPATH      += BASE/logsave/export/inc
endif

ifeq ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)
MINCPATH      += chip_drv/chip_module/efuse/uws6121e
endif

MSRCPATH =  MS_Ref/source/base/src

ifeq ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)

endif

SOURCES			= init.c cm.c diag.c shell_test.c doidleonpage.c gb_ucs2.c keypad.c \
					misc_task.c power.c ref_compile_date.c ref_outport.c ref_param.c \
					run_mode.c text_fnt.c ucs2_gb.c watchdog.c  crc16.c \
					task_monitor.c iram_mgr.c task_analyzer.c imei.c  \
					sys_time.c  production_test_current.c char_dev.c ap_srv.c


SOURCES         += auto_download.c

ifeq ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)
SOURCES         += crc32.c \
				   crypto.c \
				   sha256.c \
				   sm3.c  \
				   hal_rom_api.c
endif

ifeq ($(strip $(TOUCHPANEL_TYPE)), HW)
SOURCES         += tp_srv.c
SOURCES         += tp_gesture_drv.c  
endif

ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
SOURCES         += dummy_modem.c  atc_ext.c
endif

ifeq ($(strip $(DSP_USB_LOG)), TRUE)
MINCPATH += chip_drv/chip_plf/export/inc
SOURCES         += dsp_log.c
endif

ifeq ($(strip $(BT_USB_LOG)), TRUE)
MINCPATH += chip_drv/chip_plf/export/inc
SOURCES         += bt_log.c
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH      += chip_drv/chip_plf/sc6531efm
SOURCES         += arm9_shm_operation.c
MCFLAG_OPT      += -DCALI_MODE_PORT_UART0
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH      += chip_drv/chip_plf/uws6121e  \
				chip_drv/chip_module/chip/uws6121e \
				MS_Ref/source/rpc/inc
				
SOURCES         += arm9_shm_operation.c
SOURCES         += modem_log.c
endif

ifeq ($(strip $(SPRD_ENGPC_SUPPORT)), TRUE)

MINCPATH += MS_Ref/source/engpc/server/inc \
			MS_Ref/source/engpc/sprd_fts_inc
endif

ifeq ($(strip $(FOTA_SUPPORT)),REDSTONE)
MINCPATH	+= MS_Ref/source/rsdlsdk/porting/sc6531/inc \
			MS_Ref/source/rsdlsdk/porting/inc \
			CAF/Template/include \
			common/export/inc \
			daps/export/inc/tcpip \
			base/atc/modem/source/h \
			version \
			build/$(PROJECT)_builddir/tmp
MSRCPATH += MS_Ref/source/rsdlsdk/porting/sc6531/src
SOURCES += rs_md5.c \
			rs_dev.c \
			rs_flash_operate.c \
			rs_mem.c \
			rs_notify_user.c \
			rs_param.c \
			rs_sdk_api_ex.c \
			rs_socket.c \
			rs_fs.c \
			rs_std_fun.c \
			rs_system.c \
			rs_thread.c
endif

ifeq ($(strip $(SIM_LOCK_SUPPORT)), TRUE)
ifeq ($(strip $(SIMLOCK_PROTECT_DATA)), TRUE)
MINCPATH   += MS_Ref/source/simlock/inc
endif
endif

