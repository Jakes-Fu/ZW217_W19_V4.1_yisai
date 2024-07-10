
MCFLAG_OPT      = 

MASMFLAG_OPT    = 

ifeq ($(strip $(CHIP_VER)),    6531)
MCFLAG_OPT  += -DCHIP_DRV_SC6531
endif

ifeq ($(strip $(CHIP_VER)),    6531EFM)
MCFLAG_OPT  += -DCHIP_DRV_SC6531EFM
endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
PLATFORM_TEMP = uws6121e
endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MCFLAG_OPT  += -DCHIP_DRV_UIX8910
endif

ifeq ($(strip $(STACK_CHECK_SUPPORT)),    FALSE)
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MASMFLAG_OPT    = $(KEEP) $(CODE16) $(LITTLE_ENDIAN_OPT) $(APCS) /shlclient/interwork/
else
MASMFLAG_OPT    = $(KEEP) $(CODE16) $(BIG_ENDIAN_OPT) $(APCS) /shlclient/interwork/
endif
endif

#MCFLAG_OPT         += -S
ifeq ($(strip $(CHIP_ENDIAN)),    LITTLE)
MCFLAG_OPT         += -D_LITTLE_ENDIAN_DEBUG
#for usb modify only , should modify later
endif

MINCPATH =  Third-party/xsr_stlmp/include
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/export/inc
MINCPATH += chip_drv/chip_plf/export/inc

ifeq ($(strip $(CHIP_VER)), 6531)
MINCPATH += chip_drv/chip_lib/inc
else
ifeq ($(strip $(CHIP_VER)), 6531EFM)
MINCPATH += chip_drv/chip_lib/inc
else
ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MINCPATH += chip_drv/chip_lib/inc
else
MINCPATH += chip_drv/chip_module/tpc
MINCPATH += chip_drv/chip_module/tpc/v5
endif
endif
endif

ifeq ($(strip $(PLATFORM)), SC6530)

ifeq ($(strip $(CHIP_VER)), 6531)
MINCPATH += chip_drv/chip_plf/sc6531
MSRCPATH += chip_drv/chip_plf/sc6531
else
MINCPATH += chip_drv/chip_plf/sc6530
MSRCPATH += chip_drv/chip_plf/sc6530
endif
MSRCPATH += chip_drv/chip_module/mmu
MSRCPATH += chip_drv/chip_module/mmu/v0

ifeq ($(strip $(CHIP_VER)), 6531)
MSRCPATH += chip_drv/chip_lib/src
else
MSRCPATH += chip_drv/chip_module/tpc
MSRCPATH += chip_drv/chip_module/tpc/v5
endif

MSRCPATH += chip_drv/chip_module/vfp
MSRCPATH += chip_drv/chip_module/lzma
MSRCPATH += chip_drv/chip_module/lzma/v5

MCFLAG_OPT     += -DVB_DMA_SUPPORT -DSDIO_HIGH_SPEED_UP_TO_48MHZ
MASMFLAG_OPT    += -PD "MMU_ENABLE SETL {TRUE}"
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH += chip_drv/chip_plf/sc6531efm
MSRCPATH += chip_drv/chip_plf/sc6531efm

MSRCPATH += chip_drv/chip_module/mmu
MSRCPATH += chip_drv/chip_module/mmu/v0

MSRCPATH += chip_drv/chip_lib/src

MSRCPATH += chip_drv/chip_module/lzma
MSRCPATH += chip_drv/chip_module/lzma/v5

MCFLAG_OPT     += -DVB_DMA_SUPPORT -DSDIO_HIGH_SPEED_UP_TO_48MHZ
MASMFLAG_OPT    += -PD "MMU_ENABLE SETL {TRUE}"
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH += chip_drv/chip_plf/uws6121e
MINCPATH += MS_Customize/source/product/config/$(strip $(PLATFORM_TEMP))
MINCPATH += chip_drv/chip_plf/sc6531efm
MINCPATH += chip_drv/chip_module/smc/uws6121e
MINCPATH += chip_drv/chip_module/smc/uws6121e/ddr_timing
MINCPATH += chip_drv/chip_module/sys_mail/$(strip $(PLATFORM_TEMP))
MINCPATH += fdl_bootloader/chip_drv/inc/uws6121e
MINCPATH += chip_drv/chip_module/sys_mail
MINCPATH += chip_drv/chip_module/efuse 
MINCPATH += chip_drv/chip_module/chip/uws6121e
MINCPATH += chip_drv/chip_module/pin/uws6121e

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
MINCPATH += build/$(PROJECT)_builddir/tmp
MINCPATH += version
MINCPATH += MS_Ref/source/efs/inc/anti
MINCPATH += RTOS/source/include/anti
MINCPATH += chip_drv/chip_module/efuse/uws6121e/anti
MSRCPATH += chip_drv/chip_module/efuse/uws6121e/anti
endif


MSRCPATH += chip_drv/chip_plf/uws6121e

MSRCPATH += chip_drv/chip_module/mmu
MSRCPATH += chip_drv/chip_module/mmu/arm_v7

MSRCPATH += chip_drv/chip_lib/src

MSRCPATH += chip_drv/chip_module/lzma
MSRCPATH += chip_drv/chip_module/lzma/uws6121e

MSRCPATH += chip_drv/chip_module/sleep
MSRCPATH += chip_drv/chip_module/sleep/uws6121e                      
MSRCPATH += chip_drv/chip_module/efuse                        
MSRCPATH += chip_drv/chip_module/efuse/uws6121e

MCFLAG_OPT     += -DVB_DMA_SUPPORT -DSDIO_HIGH_SPEED_UP_TO_48MHZ
MASMFLAG_OPT    += -PD "MMU_ENABLE SETL {TRUE}"
endif

ifeq ($(strip $(PLATFORM)),    UWS6121E)
SOURCES +=              v7_mmu_cache.s                       \
                        lzma_hal.c                      \
                        lzma_phy_uws6121e.c

SOURCES +=              sleep_asm_uws6121e.s               \
                        sleep_aon_init_uws6121e.s          \
                        sleep_phy_uws6121e.c               \
                        sleep_oswtimer.c                  \
                        sleep_ddr_uws6121e.c               \
                        uws6121e_sleep_cfg.c               \
                        sleep_common.c                  

                      
SOURCES +=              efuse_phy_uws6121e.c                  \
                        efuse_phy_uws6121e_pmic.c                  \
                        efuse_hal.c \
						efuse_api.c 
                        
SOURCES += uws6121e_mmu_config.c

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
SOURCES += antiregl.c
endif

ifeq ($(strip $(VM_SUPPORT)),    TRUE)
SOURCES += mmu_bzp.c  mmu_vm_bzp.c v7_mmu.c
else
SOURCES += mmu.c mmu_vm.c                        
endif

SOURCES += tpc_algorithm.c

endif

