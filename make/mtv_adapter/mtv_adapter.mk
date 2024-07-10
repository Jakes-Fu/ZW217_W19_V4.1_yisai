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
                chip_drv/export/inc/outdated BASE/l4/export/inc BASE/sim/export/inc\
                sim/export/inc CMMB/source/dtl/inc CMMB/source/mtv_service/inc $(PS_INCPATH)
                
MSRCPATH      = CMMB/source/mtv_adapter/src CMMB/source/mbbms/src 

SOURCES       =  cmmb_task.c  mtv_fs.c  mtv_setting.c \
                mtv_common_func.c
ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), FALSE)
SOURCES       += dac.c apdac.c hydac.c tv_firmware.c unzip_tv_firmware.c icc_mcex.c
endif                

ifeq ($(strip $(MBBMS_SUPPORT)),TRUE)
SOURCES       += sdp.c\
                mbbms_sg_internal.c mbbms_sg.c mbbms_sg_api.c mbbms_sgdu_parse.c mbbms_sub_parse.c \
 		base64.c mbbms_gba.c mbbms_sub.c mikey_v.c mikey_err.c mikey_ext.c mikey_ext_keyid.c \
		mikey_hdr.c mikey_id.c mikey_kemac.c mikey_rand.c mikey_t.c mikey.c dtl_rfvt.c
#SOURCES       +=  mua_cmd.c

ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), FALSE)
SOURCES       += sdua_drv.c
else
MINCPATH      += CMMB/source/demodal/inc
SOURCES       += ua_drv.c
endif		
endif



