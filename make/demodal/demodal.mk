MCFLAG_OPT 		+= -DSPRD_SUPPORT_MCEX -DHPATH=$(PROJECT)

MINCPATH      = rtos/export/inc MS_Ref/export/inc CMMB/source/demodal/inc
                
ifeq ($(strip $(DEMOD_HW)), SIANO)
MINCPATH      += Third-party/siano/siano_api/incl                
endif

ifeq ($(strip $(DEMOD_HW)), INNOFIDEI)
MINCPATH      += Third-party/innofidei/inc                
endif

MSRCPATH      = CMMB/source/demodal/src

SOURCES       = demod.c
                
ifeq ($(strip $(MBBMS_SUPPORT)),TRUE)
SOURCES       += 
                
endif



