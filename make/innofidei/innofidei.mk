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

MCFLAG_OPT 		+= -DINNO_FW_26M 

C_MACRO += -DDEMOD_HW_INNOFIDEI
C_MACRO += -UDEMOD_HW_SIANO

MINCPATH      = rtos/export/inc MS_Ref/export/inc chip_drv/export/inc cmmb/source/dtl/inc cmmb/export/inc Third-party/innofidei/inc\
                CMMB/source/demodal/inc  chip_drv/export/inc/outdated MS_Customize/export/inc 
                

MSRCPATH      = Third-party/innofidei/src

SOURCES       = INNO_Communication.c INNO_Operation.c INNO_OS.c INNO_API.c

#demod al
MSRCPATH      += CMMB/source/demodal/src
SOURCES       += demod.c

#mtv_api   
MINCPATH  += CMMB/source/mbbmsca_inno/h CMMB/source/mtv/h CMMB/source/mbbms/inc CMMB/source/misc l4/export/inc
MSRCPATH  += CMMB/source/mbbmsca_inno/c  CMMB/source/mbbms/src 
SOURCES   += ca_adapter.c mua_cmd.c


              

ifeq ($(strip $(MBBMS_SUPPORT)),TRUE)
SOURCES       += 
                
endif



