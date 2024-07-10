
MCFLAG_OPT += -DWLAN_HEADERS

ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT += -DWORDS_LITTLEENDIAN
MCFLAG_OPT += -DSUPPORT_CHINAMOBILE_SPEC
MCFLAG_OPT += -DWAPI_ENABLE
WIFI_WAPI_ENABLE = TRUE
MCFLAG_OPT += -DWLAN_SUPPORT
endif

MCFLAG_OPT += -DWLAN_SUPPORT

## MCFLAG_OPT used for RTL8723
MCFLAG_OPT += -DCONFIG_DEBUG_RTL871X
MCFLAG_OPT += -DCONFIG_RTL8192C
MCFLAG_OPT += -DCONFIG_SPI_HCI
MCFLAG_OPT += -DCONFIG_LITTLE_ENDIAN
MCFLAG_OPT += -DPLATFORM_OS_THREADX
MCFLAG_OPT += -DCONFIG_80211N_HT
#MCFLAG_OPT += -DDISABLE_BB_RF
MCFLAG_OPT += -DHAL_RF_ENABLE
MCFLAG_OPT += -DHAL_BB_ENABLE
MCFLAG_OPT += -DHAL_MAC_ENABLE
#MCFLAG_OPT += -DMAC_LOOPBACK
#MCFLAG_OPT += -DTEST_SCAN
#WPA PSK
MCFLAG_OPT += -DINCLUDE_WPA_PSK
MCFLAG_OPT += -DRTL_WPA2
MCFLAG_OPT += -DWORK_QUEUE
MCFLAG_OPT += -DTX_SHORTCUT
MCFLAG_OPT += -DRX_SHORTCUT
MCFLAG_OPT += -DCONFIG_NATIVEAP_MLME
#MCFLAG_OPT += -DSUPPORT_TX_DMA
#MCFLAG_OPT += -DSUPPORT_RX_DMA
#MCFLAG_OPT += -DINDI_THREAD
#MCFLAG_OPT += -DXMIT_THREAD
#MCFLAG_OPT += -DCONFIG_MP_INCLUDED
#MCFLAG_OPT += -DCONFIG_RECV_REORDERING_CTRL
MCFLAG_OPT += -DCONFIG_IPS
MCFLAG_OPT += -DCONFIG_LPS
MCFLAG_OPT += -DRTL8188E
MCFLAG_OPT += -DCONFIG_FROM_CONFIG_FILE
MCFLAG_OPT += -DCONFIG_EXT_CLK
#MCFLAG_OPT += -DRATE_ADAPTIVE_SUPPORT
#MCFLAG_OPT += -DPOWER_TRAINING_ACTIVE

#MCFLAG_OPT += -DDEV_BUS_USB_INTERFACE
#usb hci
#MCFLAG_OPT += -DUSB_TX_AGGREGATION_92C
#MCFLAG_OPT += -DUSB_RX_AGGREGATION_92C
#MCFLAG_OPT += -DRTL8192CU_FW_DOWNLOAD_ENABLE
#MCFLAG_OPT += -DCONFIG_SKB_COPY #for ampdu
#MCFLAG_OPT += -DCONFIG_R871X_TEST
#MCFLAG_OPT += -DCONFIG_NATIVEAP_MLME
#MCFLAG_OPT += -DRTL8191C_FPGA_NETWORKTYPE_ADHOC
#MCFLAG_OPT += -DCONFIG_SKB_COPY
#usb hci
## MCFLAG_OPT used for RTL8723

RTL_DRIVER_SRC = DAPS/source/wifi/rtl8723

MINCPATH = MS_Customize/source/product/driver/wifi/rtl8723
MINCPATH += DAPS/source/wifi/rtl8723/include
MINCPATH += DAPS/source/wifi/rtl8723/include/byteorder
MINCPATH += DAPS/source/wifi/rtl8723/include/os_dep
MINCPATH += DAPS/source/wifi/rtl8723/include/os_dep/os_threadx
MINCPATH += DAPS/source/wifi/rtl8723/hal/rtl8189e
MINCPATH += DAPS/source/wifi/rtl8723/hal
#this is temp
MINCPATH += DAPS/source/wifi/rtl8723/hal/rtl8189e/rtl8189es
MINCPATH += DAPS/source/wifi/rtl8723/os_dep/threadx/gspi
MINCPATH += DAPS/source/wifi/rtl8723/os_dep/threadx
MINCPATH += DAPS/source/wifi/rtl8723/wpa_supplicant

MSRCPATH = MS_Customize/source/product/driver/wifi/rtl8723
MSRCPATH += DAPS/source/wifi/rtl8723/core
MSRCPATH += DAPS/source/wifi/rtl8723/hal/rtl8189e
MSRCPATH += DAPS/source/wifi/rtl8723/hal
MSRCPATH += DAPS/source/wifi/rtl8723/hal/rtl8189e/rtl8189es
MSRCPATH += DAPS/source/wifi/rtl8723/os_dep
MSRCPATH += DAPS/source/wifi/rtl8723/os_dep/threadx
MSRCPATH += DAPS/source/wifi/rtl8723/os_dep/threadx/gspi
MSRCPATH += DAPS/source/wifi/rtl8723/wpa_supplicant
#rtw_debug.c
#rtw_eeprom.c
#rtw_ioctl_query.c
#rtw_ioctl_rtl.c
SOURCES	 = ieee80211.c rtw_cmd.c rtw_io.c rtw_intfs.c\
	   rtw_ioctl_set.c rtw_mlme.c rtw_mlme_ext.c \
	   rtw_pwrctrl.c rtw_recv.c rtw_xmit.c \
	   rtw_security.c rtw_sta_mgt.c rtw_wlan_util.c rtw_debug.c

SOURCES	+= hal_efuse.c hal_pwrseq_cmd.c hal_mp.c hal_mp_ioctl.c hal_cmd.c hal_cam.c
SOURCES	+= rtl8188e_dm.c rtl8188e_efuse.c \
	   rtl8188e_fw.c rtl8188e_phycfg.c rtl8188e_rf6052.c rtl8188e_pwrseq.c \
	   rtl8188e_rxdesc.c \
	   rtl8188es_desc.c rtl8188es_gen.c rtl8188es_sw.c \
	   rtl8188es_hwimg.c rtl8188es_isr.c rtl8188es_btc.c \
	   odm_RegConfig8188E.c HalHWImg8188E_BB.c HalHWImg8188E_MAC.c HalHWImg8188E_RF.c \
	   Hal8188ERateAdaptive.c

SOURCES	+= threadx_intfs.c threadx_ioctl.c \
	   gspi_rx.c gspi_tx.c gspi_io.c gspi_ops.c

SOURCES	+= supp_psk.c 1x_kmsm_aes.c 1x_kmsm_hmac.c 1x_md5c.c \
	   1x_rc4.c

