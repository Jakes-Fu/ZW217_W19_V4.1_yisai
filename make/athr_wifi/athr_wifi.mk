
ifeq ($(strip $(PLATFORM)),	SC6600R)
MCFLAG_OPT   	= -DDISABLE_OPT -D_RECONSTRUCTION_ -DWMFAPI_NO_DRM \
			-DWMA_OPT_STRMDEC_ARM -DWMA_OPT_INVERSQUAN_LOWRATE_ARM -DWMA_OPT_LPCSTD_ARM -DENABLE_LPC \
			-D_Embedded_x86 -DBITSTREAM_CORRUPTION_ASSERTS -DWMA_OPT_SUBFRAMERECON_ARM -DWMA_DEBUG -DENABLE_ALL_ENCOPT \
			-DWMA_OPT_AURECON_ARM -DBUILD_INTEGER -D_RTOS -DWMA_OPT_HUFFDEC_ARM -D_ARM_ -DWMA_OPT_FFT_ARM -D_DEBUG \
			-DBIG_ENDIAN -DBUILD_WMASTD -D_WMF_ -DWMAAPI_NO_DRM_STREAM -DWMA_V9 -D_RAM_ALLOC_ -DWMAAPI_NO_DRM 

MASMFLAG_OPT   = 
endif

ifeq ($(strip $(PLATFORM)),	SC6800)
MCFLAG_OPT   	= DDISABLE_OPT -D_RECONSTRUCTION_ \
					-DWMFAPI_NO_DRM -DWMA_OPT_STRMDEC_ARM -DWMA_OPT_INVERSQUAN_LOWRATE_ARM -DWMA_OPT_LPCSTD_ARM -DENABLE_LPC \
					-D_Embedded_x86 -DBITSTREAM_CORRUPTION_ASSERTS -DWMA_OPT_SUBFRAMERECON_ARM -DWMA_DEBUG -DENABLE_ALL_ENCOPT \
					-DWMA_OPT_AURECON_ARM -DBUILD_INTEGER -D_RTOS -DWMA_OPT_HUFFDEC_ARM -D_ARM_ -DWMA_OPT_FFT_ARM -D_DEBUG \
					-DBIG_ENDIAN -DBUILD_WMASTD -D_WMF_ -D_REF_SC6800_ -DWMAAPI_NO_DRM_STREAM -DWMA_V9 -D_RAM_ALLOC_ -DWMAAPI_NO_DRM

MASMFLAG_OPT   = 
endif

MCFLAG_OPT += -DWLAN_HEADERS

ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT += -DWORDS_LITTLEENDIAN
endif

MCFLAG_OPT += -DSUPPORT_CHINAMOBILE_SPEC
MCFLAG_OPT += -DWAPI_ENABLE


ATH_DRIVER_SRC = DAPS/source/wifi/ar6302

MINCPATH =  $(ATH_DRIVER_SRC) \
						$(ATH_DRIVER_SRC)/firmware \
						$(ATH_DRIVER_SRC)/host/bmi/include \
						$(ATH_DRIVER_SRC)/host/hif/spi/sprd_os \
						$(ATH_DRIVER_SRC)/host/htc2/AR6000 \
						$(ATH_DRIVER_SRC)/host/htc2 \
						$(ATH_DRIVER_SRC)/host/include \
						$(ATH_DRIVER_SRC)/host/os/sprd_os/include \
						$(ATH_DRIVER_SRC)/host/os/sprd_os/utility/flag \
						$(ATH_DRIVER_SRC)/host/spistack/src/busdriver \
						$(ATH_DRIVER_SRC)/host/spistack/src/hcd/athspi_common \
						$(ATH_DRIVER_SRC)/host/spistack/src/include \
						$(ATH_DRIVER_SRC)/host/spistack/src/include/sprd_os \
						$(ATH_DRIVER_SRC)/host/wlan/include \
						$(ATH_DRIVER_SRC)/host/wmi \
						MS_Customize/source/product/driver/wifi/ar6302 \
						$(ATH_DRIVER_SRC)/include \
						$(ATH_DRIVER_SRC)/include/AR6002 \
						$(ATH_DRIVER_SRC)/include/AR6002/hw2.0 \
						$(ATH_DRIVER_SRC)/host/tools/mboxping 						
                        						

MSRCPATH =  $(ATH_DRIVER_SRC)/host/bmi/src \
						$(ATH_DRIVER_SRC)/host/hif/spi/sprd_os \
						$(ATH_DRIVER_SRC)/host/htc2/AR6000 \
						$(ATH_DRIVER_SRC)/host/htc2 \
						$(ATH_DRIVER_SRC)/host/miscdrv \
						$(ATH_DRIVER_SRC)/host/os/sprd_os \
						$(ATH_DRIVER_SRC)/host/os/sprd_os/utility/flag \
						$(ATH_DRIVER_SRC)/host/spistack/src/busdriver \
						$(ATH_DRIVER_SRC)/host/spistack/src/busdriver/sprd_os \
						$(ATH_DRIVER_SRC)/host/spistack/src/hcd/athspi_common \
						$(ATH_DRIVER_SRC)/host/spistack/src/hcd/sprd_spi_hcd \
						$(ATH_DRIVER_SRC)/host/spistack/src/lib \
						$(ATH_DRIVER_SRC)/host/spistack/src/lib/sprd_os \
						$(ATH_DRIVER_SRC)/host/wmi \
						$(ATH_DRIVER_SRC)/host/wlan/src \
						$(ATH_DRIVER_SRC)/host/tools/mboxping \
						$(ATH_DRIVER_SRC)/host/tools/SPI_testing \
						$(ATH_DRIVER_SRC)/tcmd
						
SOURCES			= bmi.c \
							hif.c \
							htc.c \
							htc_recv.c \
							htc_send.c \
							htc_services.c \
							ar6k.c \
							ar6k_events.c \
							common_drv.c \
							credit_dist.c \
							ar6000_drv.c \
							netbuf.c \
							ath_flag.c \
							sdio_bus.c \
							sdio_bus_events.c \
							sdio_bus_misc.c \
							sdio_function.c \
							sdio_bus_os.c \
							ath_spi_hcd.c \
							sprd_spi_hw.c \
							sdio_lib_c.c \
							sdio_lib_os.c \
							wmi.c \
							wlan_utils.c \
							athtestcmd.c \
							getotp_long.c
