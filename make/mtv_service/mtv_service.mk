ifeq ($(SCREEN_TEST_SUPPORT), TRUE)
MCFLAG_OPT		= -DSCREEN_TEST
endif

ifeq ($(strip $(PLATFORM)),SC6600R)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600R
MASMFLAG_OPT	+= -PD "CHIP_DRV_SC6800D SETL {FALSE}" -PD "CHIP_DRV_SC6600R SETL {TRUE}" -PD "CHIP_DRV_SC6600H SETL {FALSE}" -PD "CHIP_DRV_SC6600M SETL {FALSE}"
endif

MCFLAG_OPT 		+= -DSPRD_SUPPORT_MCEX -DHPATH=$(PROJECT)

ifeq ($(strip $(CMMB_DAC_TYPE)), AP)
MCFLAG_OPT 		+= -DDAC_TYPE_AP
endif

ifeq ($(strip $(CMMB_DAC_TYPE)),HYBRID)
MCFLAG_OPT 		+= -DDAC_TYPE_HYBRID
endif

ifeq ($(strip $(CMMB_DAC_TYPE)),V)
MCFLAG_OPT 		+= -DDAC_TYPE_V
endif

MINCPATH      = CMMB/source/mtv_adapter/inc MS_Code/common/export/inc chip_drv/export/inc MS_Code/MS_Customize/export/inc \
                CMMB/source/os_al/include/KD  $(MMI_DIR)/Source/MMI_App/Kernel/h MS_Ref/export/inc CMMB/source/mbbms/inc \
                CMMB/export/inc CMMB/source/proxycall/inc l4/export/inc ms_ref/source/image_proc/src/png_decode/inc  \
                sim/export/inc ps/export/inc CMMB/source/dtl/inc $(MMI_DIR)/source/mmi_app/app/pdp/h CMMB/source/mtv_service/inc \
				CMMB/source/os_al/include/KD BASE/l4/export/inc
MSRCPATH      = CMMB/source/mtv_service/src 

SOURCES       = mtv_service.c mtv_service_sub.c mtvservice_main.c  mtvservice_player.c mtvservice_mbbms.c mtvservice_callback.c mtvservice_simu.c




