
# **************************** COMMON *************************
	
ifeq ($(strip $(CHIP_ENDIAN_DEFAULT)),LITTLE)
    ifeq ($(strip $(CHIP_ENDIAN)),BIG)
        MCFLAG_OPT 	  = $(LITTLE_ENDIAN_OPT) -D_LITTLE_ENDIAN -DFDL_ENDIAN_SWITCH
        MASMFLAG_OPT  = $(LITTLE_ENDIAN_OPT)    
        CFLAGS        = $(CPU) ARM926EJ-S -O2 $(APCS) /interwork/ -fy -D_RTOS -D_DEBUG -Ec -W
        AFLAGS        = $(CPU) ARM926EJ-S $(KEEP) $(APCS) /shlclient/interwork/
    else
        MCFLAG_OPT    = -D_LITTLE_ENDIAN
    endif
else
    ifeq ($(strip $(SC8800S_LITTLE_ENDIAN)),TRUE)
        MCFLAG_OPT    = $(BIG_ENDIAN_OPT) -D_BIG_ENDIAN
        MASMFLAG_OPT  = $(BIG_ENDIAN_OPT) 
    endif
endif

ifeq ($(strip $(STACK_CHECK_SUPPORT)),TRUE)
    CFLAGS        := $(subst $(SWST_OPT),,$(CFLAGS))
    AFLAGS        := $(subst $(SWST_OPT),,$(AFLAGS))
endif



# **************************** PLATFORM *************************



MINCPATH =  fdl_bootloader/common/inc \
						fdl_bootloader/fdl1/inc \
						chip_drv/export/inc \
						chip_drv/chip_module/usb/v0 \
						chip_drv/chip_module/int/v0 \
						chip_drv/chip_module/usb/v3 \
                                                chip_drv/chip_plf/export/inc \
						fdl_bootloader/chip_drv/inc 
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH =  fdl_bootloader/common/src \
 						fdl_bootloader/fdl1/src \
 						fdl_bootloader/chip_drv/src 

ifeq ($(strip $(USB_PROTOCOL_SUPPORT)), USB20)
MSRCPATH +=fdl_bootloader/chip_drv/src/usb_boot/usb20
else
MSRCPATH +=fdl_bootloader/chip_drv/src/usb_boot/usb11
endif

SOURCES				= fdl_main.c      \
				  fdl_command.c   \
				  fdl_stdio.c     \
				  sdram_init.c    \
				  dl_engine.c     \
				  drv_usb.c       \
				  fdl_crc.c       \
				  packet.c	  \
				  sio_drv.c       \
				  usb_boot.c      \
				  virtual_com.c	  \
				  init.s					\
				  chip_cfg.c 		\
				  fdl_channel.c                                 


