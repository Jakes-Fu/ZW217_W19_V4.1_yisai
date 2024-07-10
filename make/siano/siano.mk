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

MCFLAG_OPT 		+= -DSIANO_CMMB_HW

C_MACRO += -DDEMOD_HW_SIANO
C_MACRO += -UDEMOD_HW_INNOFIDEI


MINCPATH      = rtos/export/inc MS_Ref/export/inc Third-party/siano/sianocommon/incl Third-party/siano/sianoplatforms/incl \
                Third-party/siano/siano_api/incl CMMB/source/dtl/inc CMMB/source/demodal/inc CMMB/export/inc chip_drv/export/inc/outdated MS_Customize/export/inc

MSRCPATH      = Third-party/siano/sianocommon/srce Third-party/siano/sianoplatforms/srce Third-party/siano/siano_api/srce

SOURCES       = smshostliblitecmmb.c smshostliblitecommon.c smshostutils.c osw_event.c osw_init.c osw_memory.c osw_task.c \
                smsspiphy.c smsspicommon.c sianodrv.c sianoapi.c
                
#demod al
MSRCPATH   += CMMB/source/demodal/src
SOURCES    += demod.c

#mtv_api   
MINCPATH    += CMMB/source/mbbmsca/h CMMB/source/mtv/h CMMB/source/mbbms/inc  CMMB/source/misc l4/export/inc
MSRCPATH    += CMMB/source/mbbmsca/c CMMB/source/mbbms/src 
SOURCES     += ca_adapter.c ca_key.c ecm.c mua_cmd.c     


ifeq ($(strip $(MBBMS_SUPPORT)),TRUE)
SOURCES       += 
                
endif



