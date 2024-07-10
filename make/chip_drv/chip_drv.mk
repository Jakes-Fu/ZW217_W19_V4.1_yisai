
MCFLAG_OPT      = 

MASMFLAG_OPT    = 


ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MCFLAG_OPT  += -DCHIP_DRV_UIX8910
MCFLAG_OPT  += -DARM_ARCH_V7
MCFLAG_OPT  += -D_SIO_WAKEUP_ARM
endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
PLATFORM_TEMP = uws6121e
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

MINCPATH =  MS_Ref/source/aud_config/inc_export \
            Third-party/xsr_stlmp/include \
            $(ATC_INCPATH) $(L1_INCPATH) \
            BASE/logsave/export/inc BASE/PS/export/inc 

ifeq ($(strip $(PLATFORM)),    UWS6121E)

MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/export/inc
MINCPATH += chip_drv/chip_plf/export/inc
MINCPATH += chip_drv/chip_lib/inc
MINCPATH += chip_drv/chip_plf
MINCPATH += MS_Customize/source/product/config/$(strip $(PLATFORM_TEMP))
MINCPATH += chip_drv/chip_module/mmu
MINCPATH += chip_drv/chip_module/audio
MINCPATH += chip_drv/chip_module/smc/uws6121e
MINCPATH += chip_drv/chip_module/lzma/uws6121e
MINCPATH += chip_drv/chip_module/sys_mail/$(strip $(PLATFORM_TEMP))    
MINCPATH += chip_drv/chip_module/sys_mail
MINCPATH += chip_drv/chip_plf/uws6121e
MINCPATH += chip_drv/chip_module/efuse

MSRCPATH += chip_drv/chip_module/lzma/uws6121e
MINCPATH += chip_drv/chip_module/chip/uws6121e

MINCPATH += chip_drv/chip_module/smc/uws6121e/ddr_timing
MINCPATH += chip_drv/chip_module/eic
MINCPATH += chip_drv/chip_module/eic/uws6121e
MSRCPATH += chip_drv/chip_plf/common/src
MSRCPATH += chip_drv/chip_module/adc
MSRCPATH += chip_drv/chip_module/adc/r6p0
MSRCPATH += chip_drv/chip_module/global_ctl
MSRCPATH += chip_drv/chip_module/adi
MSRCPATH += chip_drv/chip_module/adi/uws6121e
MSRCPATH += chip_drv/chip_module/analog
MSRCPATH += chip_drv/chip_module/analog/sc2720
MSRCPATH += chip_drv/chip_module/cardlog
MSRCPATH += chip_drv/chip_module/busmonitor
MSRCPATH += chip_drv/chip_module/busmonitor/uws6121e
MSRCPATH += chip_drv/chip_module/charge/uws6121e
MSRCPATH += chip_drv/chip_module/charge/v7
MSRCPATH += chip_drv/chip_module/chip
MSRCPATH += chip_drv/chip_module/clock
MSRCPATH += chip_drv/chip_module/dma
MSRCPATH += chip_drv/chip_module/dsp
MSRCPATH += chip_drv/chip_module/dsp/v5
MSRCPATH += chip_drv/chip_module/smc
MSRCPATH += chip_drv/chip_module/smc/uws6121e
MSRCPATH += chip_drv/chip_module/freq
MSRCPATH += chip_drv/chip_module/freq/v5
MSRCPATH += chip_drv/chip_module/gea
MSRCPATH += chip_drv/chip_module/gea/v5
MSRCPATH += chip_drv/chip_module/gpio
MSRCPATH += chip_drv/chip_module/i2c
MSRCPATH += chip_drv/chip_module/i2c/uws6121e
MSRCPATH += chip_drv/chip_module/i2c/gpio_simu
MSRCPATH += chip_drv/chip_module/init
MSRCPATH += chip_drv/chip_module/int
MSRCPATH += chip_drv/chip_module/int/uws6121e
MSRCPATH += chip_drv/chip_module/kpd
MSRCPATH += chip_drv/chip_module/kpd/uws6121e
MSRCPATH += chip_drv/chip_module/lcdc
MSRCPATH += chip_drv/chip_module/lcdc/v5
MSRCPATH += chip_drv/chip_module/lcdc/uws6121e
MSRCPATH += chip_drv/chip_module/ldo
MSRCPATH += chip_drv/chip_module/ldo/v5
MSRCPATH += chip_drv/chip_module/misc
MSRCPATH += chip_drv/chip_module/norflash
MSRCPATH += chip_drv/chip_module/pcm
MSRCPATH += chip_drv/chip_module/pcm/v5
MSRCPATH += chip_drv/chip_module/pin
MSRCPATH += chip_drv/chip_module/pwm
MSRCPATH += chip_drv/chip_module/pwm/v5
MSRCPATH += chip_drv/chip_module/rtc
MSRCPATH += chip_drv/chip_module/rotation
MSRCPATH += chip_drv/chip_module/rotation/uws6121e
MSRCPATH += chip_drv/chip_module/rtc/uws6121e
MSRCPATH += chip_drv/chip_module/sdio
MSRCPATH += chip_drv/chip_module/sdio/uws6121e
MSRCPATH += chip_drv/chip_module/sim
MSRCPATH += chip_drv/chip_module/sim/v5
MSRCPATH += chip_drv/chip_module/spi
MSRCPATH += chip_drv/chip_module/spi/uws6121e
MSRCPATH += chip_drv/chip_module/spi/gpio_simu
MSRCPATH += chip_drv/chip_module/sfc
MSRCPATH += chip_drv/chip_module/sfc/uws6121e
MSRCPATH += chip_drv/chip_module/sfc/sfc_common
MSRCPATH += chip_drv/chip_module/sys_timer
MSRCPATH += chip_drv/chip_module/sys_timer/dummy
MSRCPATH += chip_drv/chip_module/timer
MSRCPATH += chip_drv/chip_module/tpc
MSRCPATH += chip_drv/chip_module/tpc/r4p0
MSRCPATH += chip_drv/chip_module/uart
MSRCPATH += chip_drv/chip_module/usb
MSRCPATH += chip_drv/chip_module/watchdog
MSRCPATH += chip_drv/chip_module/watchdog/v5
MSRCPATH += chip_drv/chip_module/eic
MSRCPATH += chip_drv/chip_module/eic/uws6121e
MSRCPATH += chip_drv/chip_module/audio
MSRCPATH += chip_drv/chip_module/audio/dai/aif
MSRCPATH += chip_drv/chip_module/audio/codec/es83xx
MSRCPATH += chip_drv/chip_module/fm/v0
MSRCPATH += chip_drv/chip_module/osc
MSRCPATH += chip_drv/chip_module/osc/uws6121e
MSRCPATH += chip_drv/chip_module/sys_mail
MSRCPATH += chip_drv/chip_module/ifc/apifc
MSRCPATH += chip_drv/chip_module/spinlock/uws6121e
MSRCPATH += chip_drv/chip_module/pagespy
MSRCPATH += chip_drv/chip_module/thermal

MCFLAG_OPT += -DSPRD_AUDIO_USE_V1

ifeq ($(strip $(CHIP_VER)),    UIX8910MPW)
MINCPATH += chip_drv/chip_plf/uix8910mpw
MSRCPATH += chip_drv/chip_module/uart/uix8910mpw
MSRCPATH += chip_drv/chip_module/timer/uix8910mpw
MSRCPATH += chip_drv/chip_plf/uix8910mpw
MSRCPATH += chip_drv/chip_module/chip/uix8910mpw
MSRCPATH += chip_drv/chip_module/clock/uix8910mpw
MSRCPATH += chip_drv/chip_module/pin/uix8910mpw
MSRCPATH += chip_drv/chip_module/dma/uix8910mpw
MSRCPATH += chip_drv/chip_module/gpio/uix8910mpw
else
MINCPATH += chip_drv/chip_plf/uws6121e
MSRCPATH += chip_drv/chip_module/uart/uws6121e
MSRCPATH += chip_drv/chip_module/timer/uws6121e
MSRCPATH += chip_drv/chip_plf/uws6121e
MSRCPATH += chip_drv/chip_module/chip/uws6121e
MSRCPATH += chip_drv/chip_module/clock/uws6121e
MSRCPATH += chip_drv/chip_module/pin/uws6121e
MSRCPATH += chip_drv/chip_module/dma/uws6121e
MSRCPATH += chip_drv/chip_module/gpio/uws6121e
MSRCPATH += chip_drv/chip_module/pagespy/uws6121e
MSRCPATH += chip_drv/chip_module/smc/uws6121e
endif

ifneq ($(strip $(FPGA_VERIFICATION)), TRUE)
MCFLAG_OPT     += -DSDIO_HIGH_SPEED_UP_TO_48MHZ  
endif

MINCPATH += chip_drv/chip_module/charge

endif #platform UWS6121E

ifeq ($(strip $(PLATFORM)),    UWS6121E)

SOURCES +=                        adi_phy_uws6121e.c                    \
                        lzma_phy_uws6121e.c              \
                        analog_hal.c                    \
                        global_ctl_hal.c                \
                        sc6531efm_global_ctl.c              \
                        busmonitor_phy_uws6121e.c             \
                        busmonitor_hal.c                \
                        cardlog_pal.c                   \
                        cardlog_uart.c                  \
                        charge_uws6121e.c                 \
                        chg_phy_v7.c                    \
                        chip_phy_uws6121e.c                \
                        mem_dump.c                      \
                        clock.c                         \
                        uws6121e_clock_cfg.c               \
                        hash.c                          \
                        uws6121e_dma_cfg.c                 \
                        dma_hal.c                       \
                        rotation_phy.c                  \
                        dma_phy_uws6121e.c                  \
                        dsp_phy_v5.c                    \
                        dsp_hal.c                       \
                        smc_phy_uws6121e.c                  \
                        freq_hal.c                      \
                        freq_phy_v5.c                   \
                        sc6531efm_freq_cfg.c                \
                        gea_phy_v5.c                    \
                        gpio_hal.c                      \
                        gpio_phy_uws6121e.c                   \
                        uws6121e_gpio_cfg.c                \
                        uws6121e_i2c_cfg.c                 \
                        i2c_hal.c                       \
                        i2c_phy_uws6121e.c                    \
                        i2c_phy_gpio_simu.c             \
                        tx_illdb.s                      \
                        boot_mode.c                     \
                        chip_drv_init.c                 \
                        int_hal.c                       \
                        int_hal_hisr.c                  \
                        int_phy_uws6121e.c                  \
                        uws6121e_int_cfg.c                 \
                        int_gic.s                       \
                        fiq_misc_drv.c                  \
                        kpd_phy_uws6121e.c                    \
                        lcm_phy_v5.c                    \
                        lcd_if_spi.c                    \
                        lcd_if_hal.c                    \
                        hal_gouda.c                    \
                        ldo_cfg.c                       \
                        ldo_hal.c                       \
                        asm_cvt.c                       \
                        mcu_reboot.c                    \
                        misc.s                          \
                        retarget.c                      \
                        retarget_heap.c                 \
                        sio.c                           \
                        osi_fifo.c                      \
                        stack.s                         \
                        threadx_assert.c                \
                        pcm_drv_v5.c                    \
                        pinmap.c                        \
                        pwm_phy_v5.c                    \
                        rtc_phy_uws6121e.c                    \
                        sdio_dummy.c                    \
                        sim_drv.c                       \
                        spi_hal.c                       \
                        spi_phy_uws6121e.c                    \
                        sc6531efm_spi_cfg.c                 \
                        sys_timer_phy_dummy.c              \
                        timer_drv.c                     \
                        timer_hal.c                     \
                        timer_phy_uws6121e.c                  \
                        uws6121e_timer_cfg.c               \
                        uart_hal.c                      \
                        uart_phy_uws6121e.c                 \
                        sc6531efm_audio_cfg.c               \
                        uws6121e_eic_cfg.c             \
                        eic_hal.c                       \
                        eic_phy_uws6121e.c                    \
                        watchdog_hal.c                  \
                        watchdog_phy_v5.c               \
                        uws6121e_mem_cfg.c             \
                        sfc_phy_uws6121e_v2.c                    \
                        sfc_hal_v2.c                       \
                        cpu_endian.c                    \
                        adc_phy_r6p0.c                  \
                        fm_phy_v0.c                        \
                        osc_phy_uws6121e.c                \
                        osc_hal.c                        \
                        hal_sdmmc.c                     \
                        analog_phy_w217.c             \
                        md_ipc.c                        \
                        nv_ipc.c                        \
						ifc_hal.c									\
						ifc_manager_cfg.c					\
                        spinlock_phy_uws6121e.c          \
                        pmic_26M_clock.c                          \
                        load_modem.c                       \
                        load_lzma_drv.c                       \
                        fmd.c                              \
                        clk_div.c                      \
                        pagespy_dmc.c                  \
                        pagespy_dmc_phy_uws6121e.c     \
						ddr.c							\
						Thermal_phy_w217.c

ifneq ($(strip $(MODEM_SYS_IN_AP_SIDE)), TRUE)
SOURCES += uws6121e_stub.c
MINCPATH += BASE/l4/export/inc  \
            BASE/layer1/export/inc  \
            BASE/PS/export/inc \
            BASE/sim/export/inc

endif


MSRCPATH += chip_drv/chip_module/ifc/apifc
MSRCPATH += chip_drv/chip_module/ifc/aonifc
MSRCPATH += chip_drv/chip_module/ifc/lpsifc
MSRCPATH += chip_drv/chip_module/ifc/ifc_manager

MINCPATH += chip_drv/chip_module/charge
MINCPATH += MS_Ref/source/efs/inc
MINCPATH += lib/modem/$(PROJECT)
MINCPATH += lib/modem/$(PROJECT)/cp_info
MINCPATH += lib/modem/$(PROJECT)/zsp_info

endif #platform UWS6121E

ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)

    SOURCES +=              norless_dummy.c

else
MCFLAG_OPT     += -DNOR_SRAM_PLATFORM
MASMFLAG_OPT    += -PD "NOR_SRAM_PLATFORM SETL {TRUE}"
ifeq ($(strip $(CHIP_VER)), 6531)
    SOURCES +=              smc_hal.c 
else ifeq ($(strip $(CHIP_VER)), 6531EFM)
    SOURCES +=              smc_hal.c 
else ifneq ($(findstring $(strip $(CHIP_VER)),UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF),)
    SOURCES +=              smc_hal.c 
else
    SOURCES +=              emc_hal.c 
endif

    SOURCES +=              flash.c

endif

ifeq ($(strip $(PLATFORM)),    UWS6121E)
MINCPATH += chip_drv/chip_plf/common/inc
MINCPATH += chip_drv/chip_plf/uws6121e
MINCPATH += chip_drv/chip_module/usb/uws6121e/inc
MSRCPATH += chip_drv/chip_module/usb/uws6121e/usb_device
	SOURCES += 	    udc_dwc.c \
			    udc.c \
			    usb_composite_device.c \
			    f_serial.c \
			    usb_platform_uws6121e.c \
			    drv_usb.c \
			    drv_debug_userial.c

ifeq ($(strip $(USB_ENTER_CALI_USE_ROM_API)), TRUE)
SOURCES +=		vcom_rom_userial.c
else
SOURCES +=		vcom_u2s.c
endif

ifeq ($(strip $(USB_MASS_STORAGE_SUPPORT)), TRUE)
SOURCES +=		    f_mass_storage.c	\
			    msg_dev.c	\
			    umss_config.c
endif
else # PLATFORM != UWS6121E
ifeq ($(strip $(USB_PROTOCOL_SUPPORT)), USB20)
SOURCES             +=     udc_polling.c usb20_app_config.c vcom_usb20.c usb_common.c
endif

ifeq ($(strip $(USB_PROTOCOL_SUPPORT)), USB11)
SOURCES             +=     vcom_usb.c usb_drv_e.c usb_drv_i.c
endif
endif  # end of PLATFORM == UWS6121E

ifeq ($(strip $(GPIO_SIMULATE_SPI_SUPPORT)), TRUE)
SOURCES             +=             spi_phy_gpio_simu.c
endif

ifeq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
SOURCES +=    dualbat_hal.c
endif


SOURCES             +=            spi_hal_com.c

###AUDIO ### rui.huang add ##
ifeq ($(strip $(PLATFORM)),    UWS6121E)

## audio_codec
SOURCES             +=      audio_codec_es83xx.c

ifeq ($(strip $(AUDIO_CODEC_TYPE)),           ES8374)
SOURCES             +=      es8374_i2c.c
else
SOURCES             +=      es8311_i2c.c
endif

ifeq ($(strip $(AUDIO_EXTPA_TYPE)),           AW87390)
SOURCES             +=      extpa_drv_aw87390.c
else
#ifeq ($(strip $(AUDIO_EXTPA_TYPE)),          GPIO)
SOURCES             +=      extpa_drv_gpio.c
endif


## audio_dai
SOURCES             +=      audio_aif_phy.c            \
                            audio_ifc_phy.c            \
                            audio_dai_aif.c

SOURCES             +=  audio_hal.c


## include
MINCPATH             += chip_drv/chip_module/audio/codec/es83xx 
MINCPATH             += chip_drv/chip_module/audio/dai/aif

endif   # PLATFORM == UWS6121E
####AUDIO END#####


