
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
WIFI_WAPI_ENABLE = TRUE
MCFLAG_OPT += -DWLAN_SUPPORT
#MCFLAG_OPT += -DWAPI_TEST
#MCFLAG_OPT += -DWAPI_TEST_CER

ATH_SUPPLICANT_SRC = DAPS/source/wifi/ar6302/host/wpav4
ATH_HOSTAPD_SRC = DAPS/source/wifi/ar6302/host/hostapd-0.6.8
ATH_DRIVER_SRC = DAPS/source/wifi/ar6302

MINCPATH =  $(ATH_DRIVER_SRC)/firmware \
						$(ATH_DRIVER_SRC)/host/bmi/include \
						$(ATH_DRIVER_SRC)/host/include \
						$(ATH_DRIVER_SRC)/host/os/sprd_os/include \
						$(ATH_DRIVER_SRC)/host/os/sprd_os/utility/flag \
						$(ATH_DRIVER_SRC)/host/reorder \
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
						$(ATH_DRIVER_SRC)/host/tools/mboxping \
						$(ATH_SUPPLICANT_SRC)/include/wpa \
						$(ATH_DRIVER_SRC)/host/wapi/ECC2.2-2008 \
						$(ATH_DRIVER_SRC)/host/wapi/libiwnwai_asue \
						$(ATH_DRIVER_SRC)/host/wapi/libiwnwai_asue/include \
						$(ATH_DRIVER_SRC)/host/wapi/sms4 \
						$(ATH_HOSTAPD_SRC)
						

MSRCPATH =  $(ATH_SUPPLICANT_SRC)/src \
						$(ATH_SUPPLICANT_SRC)/src/_private \
						$(ATH_DRIVER_SRC)/host/wapi/ECC2.2-2008 \
						$(ATH_DRIVER_SRC)/host/wapi/libiwnwai_asue \
						$(ATH_DRIVER_SRC)/host/wapi/sms4 \
						$(ATH_HOSTAPD_SRC)
						
SOURCES			= common.c	\
							config.c	\
						  driver_ar6k.c	\
						  events.c	\
						  md5.c	\
						  rc4.c	\
						  sha1.c	\
						  wpa.c	\
						  wpa_api.c	\
						  wpa_main.c	\
						  wpa_supplicant.c	\
						  aes.c	\
						  aes_wrap.c 
				  
ifeq ($(strip $(WIFI_WAPI_ENABLE)),	TRUE)
SOURCES			+= wapi_interface.c \
						  ecc.c \
						  hmac.c \
						  cert.c \
						  wapi_common.c \
						  interface.c \
						  wapi.c \
						  sms4c.c \
						  wpi_pcrypt.c
endif						  
