
MCFLAG_OPT   	= -D_RTOS -DCARD_LOG

#if use logsave switch buffer data structe in file sys way,please open the macro, now loop buffer no use the mocro, 
#but android apk catch iq no open the mac in anythiong way
#MCFLAG_OPT += -DLOGIQ_SWITCH

#if open this open, logsave alloc buffer size will decreading half in mode 0, but now use nomal buffer size
#MCFLAG_OPT += -DLOGIQ_MODE0_LOW

#logsave add iq working mode 1, loop write iq data in buffer, no write tf card
#MCFLAG_OPT += -DLOGIQ_MODE1

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MCFLAG_OPT  += -DENDIAN_LITTLE
endif

###########define flow variable jumpe cath iq way is apk catch or file sys catch #####
CATCH_IQ_WAY_OPT = FALSE  

#catch iq use file opration way,so add LOGIQ_FILE mac
ifeq ($(strip $(CATCH_IQ_WAY_OPT)), FALSE)
MCFLAG_OPT  += -DLOGIQ_FILE
endif 
#####################################################################################
MINCPATH =  common/export/inc BASE/PS/export/inc RTOS/export/inc BASE/logsave/export/inc BASE/logsave/source/h BASE/l4/export/inc BASE/sim/export/inc MS_Ref/source/inc MM_Ref/source/drv_sc8800h    \
            Transform/export/inc MS_Customize/export/inc chip_drv/export/inc MS_MMI_Main/source/mmi_app/common/h BASE/Layer1/export/inc MS_Ref/export/inc chip_drv/chip_plf/uws6121e chip_drv/chip_plf/export/inc \
            chip_drv/chip_module/chip/uws6121e
MINCPATH += chip_drv/export/inc/outdated
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif            
MSRCPATH =  BASE/logsave/source/c

SOURCES = logsave_main.c logsave_filemanage.c logsave_config.c logsave_buffer.c logsave_output.c LogIQ_UDP_Send.c

