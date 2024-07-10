
MCFLAG_OPT   	=

ifeq ($(strip $(CHIP_ENDIAN_DEFAULT)),LITTLE)
    ifeq ($(strip $(CHIP_ENDIAN)),LITTLE)
        MCFLAG_OPT      += -D_LITTLE_ENDIAN
    endif
else
    ifeq ($(strip $(SC8800S_LITTLE_ENDIAN)),TRUE)
        MCFLAG_OPT      += -D_LITTLE_ENDIAN
        MASMFLAG_OPT    += -PD "_LITTLE_ENDIAN SETL {TRUE}"
    endif
endif

MCFLAG_OPT      += -DFDL2_MODULE

MINCPATH =  fdl_bootloader/common/inc \
            fdl_bootloader/fdl2/inc \
            fdl_bootloader/chip_drv/inc \
            chip_drv/export/inc \
            chip_drv/chip_plf/common/inc \
            chip_drv/chip_module/usb/v0 \
            chip_drv/chip_module/int/v0 \
            chip_drv/chip_module/usb/v3 \
            chip_drv/chip_module/usb/v3 \
            chip_drv/chip_plf/export/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc


MSRCPATH =  fdl_bootloader/common/src \
 						fdl_bootloader/chip_drv/src \
 						fdl_bootloader/fdl2/src     \

ifeq ($(strip $(PLATFORM)),    SC8800G)
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog/v3
MSRCPATH += fdl_bootloader/chip_drv/src/chip/v3
MSRCPATH += fdl_bootloader/chip_drv/src/adi
endif

ifeq ($(strip $(PLATFORM)),    SC6800H)
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog/v0
MSRCPATH += fdl_bootloader/chip_drv/src/chip/v0
endif

ifeq ($(strip $(PLATFORM)),    SC8800H)
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog/v0
MSRCPATH += fdl_bootloader/chip_drv/src/chip/v2
endif

ifeq ($(strip $(PLATFORM)),    SC6600L)
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog
MSRCPATH += fdl_bootloader/chip_drv/src/watchdog/v0
MSRCPATH += fdl_bootloader/chip_drv/src/chip/v0
endif

ifeq ($(strip $(USB_PROTOCOL_SUPPORT)), USB20)
MSRCPATH +=fdl_bootloader/chip_drv/src/usb_boot/usb20
else
MSRCPATH +=fdl_bootloader/chip_drv/src/usb_boot/usb11
endif

MSRCPATH += fdl_bootloader\chip_drv\src\mmu

SOURCES     =       fdl_main.c          \
                    dl_engine.c         \
                    drv_usb.c           \
                    fdl_crc.c           \
                    fdl_stdio.c         \
                    fdl_channel.c       \
                    packet.c            \
                    sio_drv.c           \
                    usb_boot.c          \
                    virtual_com.c       \
                    retarget.c          \
                    flash_command.c     \
                    mcu_command.c       \
                    chip_cfg.c          \
                    init.s              \
                    watchdog_hal.c      \
                    boot_mode.c         \
                    mmu.c               \
                    mmu_asm.s           \
                    nand_controller.c	\
                    fdl_nand.c \
										nand_partition_operation.c \
										fixnv_partition_backup.c




                    
ifeq ($(strip $(PLATFORM)),    SC8800G)
SOURCES     +=      watchdog_phy_v3.c   \
                    chip_phy_v3.c       \
                    adi_phy_v3.c
endif

ifeq ($(strip $(PLATFORM)),    SC6800H)
SOURCES     +=      watchdog_phy_v0.c   \
                    chip_phy_v0.c
endif

ifeq ($(strip $(PLATFORM)),    SC8800H)
SOURCES     +=      watchdog_phy_v0.c   \
                    chip_phy_v2.c
endif

ifeq ($(strip $(PLATFORM)),    SC6600L)
SOURCES     +=      watchdog_phy_v0.c   \
                    chip_phy_v1.c
endif

