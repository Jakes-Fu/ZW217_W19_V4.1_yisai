MCFLAG_OPT   	+= -D_CALIBRATION_VER_0009_
MCFLAG_OPT   	+= -D_RXLEVEL_PRECISION_
MCFLAG_OPT   	+= -D_RTOS
MCFLAG_OPT   	+= -D_DEBUG
MCFLAG_OPT   	+= -D_BT_USE_SFS


MCFLAG_OPT	    += -D_BT_SBC_ENCODER_ARM

ifneq ($(strip $(DRM_SUPPORT)),)  
   ifneq ($(strip $(DRM_SUPPORT)), NONE)    
        MCFLAG_OPT		+=  -DDRM_SUPPORT_$(strip $(DRM_SUPPORT))  -DDRM_SUPPORT
   else
        MCFLAG_OPT		+=  -DDRM_SUPPORT_NONE
   endif
endif

ifeq ($(strip $(PLATFORM)), SC6530)
	   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif					

ifeq ($(strip $(PLATFORM)), SC6531EFM)
	   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
	   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
	   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
   MCFLAG_OPT += -DBT_SBC_DSP_ENCODE
endif	

ifeq ($(strip $(PLATFORM)), SC8800G)
   MCFLAG_OPT += -DBT_SBC_ARM_ENCODE
endif

ifeq ($(strip $(CHIP_VER)), 6531)
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif

ifeq ($(strip $(CHIP_VER)), 6531EFM)
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E)" ""
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif

ifeq ($(strip $(BT_HCI_TRANS)),BCSP)
   MCFLAG_OPT	+= -DSPRD_BCSP		
endif

MINCPATH =  BASE/Layer1/export/inc \
			common/export/inc \
			$(MMI_DIR)/source/mmi_app/app/bt/h \
			MS_Ref/export/inc \
			chip_drv/export/inc \
			RTOS/export/inc \
			chip_drv/source/h \
			connectivity/BT/export/inc \
			chip_drv/chip_plf \
			chip_drv/export/inc \
			chip_drv/export/inc/outdated \
			MS_Ref/source/bt_pal/inc
	
ifeq ($(strip $(BLUETOOTH_BT_PS_V3X)),TRUE)
    #MINCPATH += Connectivity/BT/BT_PS_V3X/bluesdk/inc
    #MINCPATH += Connectivity/BT/BT_PS_V3X/src    
else
    #MINCPATH += Connectivity/BT/BT_PS_V4X/bluesdk/inc
    #MINCPATH += Connectivity/BT/BT_PS_V4X/src
endif

ifeq ($(strip $(CHIP_VER)), 6531)
    MINCPATH += chip_drv/chip_plf/sc6531
endif

ifeq ($(strip $(CHIP_VER)), 6531EFM)
    MINCPATH += chip_drv/chip_plf/sc6531efm
endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
    MINCPATH += chip_drv/chip_plf/uws6121e
endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E)" ""
    MINCPATH += chip_drv/chip_plf/uws6121e
endif

MSRCPATH = 	MS_Ref/source/bt_pal/os \
			MS_Ref/source/bt_pal/low_level \
			MS_Ref/source/bt_pal/sbc


#add for DRM 
MINCPATH += BASE/l4/export/inc BASE/sim/export/inc $(PS_INCPATH)

SOURCES		= uart.c timer.c os_timer.c

ifeq ($(strip $(BT_SCO_OVER)),UART)
    SOURCES     +=  bluecall_uart.c
endif
				  			
ifeq ($(strip $(BT_A2DP_SUPPORT)),TRUE)
    SOURCES     +=  polyphase.c polyphase_synthesis.c polyphase_analysis.c sbc_dec.c kernel_sbc.c sbc_pal.c 
    
    ifeq ($(strip $(CHIP_VER)),6531) 
    	SOURCES += sbc_enc.c
    endif 
    ifeq ($(strip $(CHIP_VER)),6531EFM) 
    	SOURCES += sbc_enc.c
    endif

    ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
    	SOURCES += sbc_enc.c
    endif

    ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E)" ""
    	SOURCES += sbc_enc.c
    endif

endif
