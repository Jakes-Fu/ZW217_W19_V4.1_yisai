MCFLAG_OPT += -DOS_NO_C_LIB_DEFINES
#MCFLAG_OPT += -DCONFIG_NO_CONFIG_WRITE
MCFLAG_OPT += -DCONFIG_NO_SCAN_PROCESSING
MCFLAG_OPT += -DCONFIG_NO_WPA 
MCFLAG_OPT += -DCONFIG_NO_WPA2
MCFLAG_OPT += -DEAP_SIM
MCFLAG_OPT += -DEAP_PEAP
MCFLAG_OPT += -DEAP_MSCHAPv2
MCFLAG_OPT += -DIEEE8021X_EAPOL
MCFLAG_OPT += -DCONFIG_CRYPTO_INTERNAL
MCFLAG_OPT += -DCONFIG_INTERNAL_LIBTOMMATH
MCFLAG_OPT += -DLTM_FAST
MCFLAG_OPT += -DCONFIG_NO_RANDOM_POOL
MCFLAG_OPT += -DIEEE8021X_EAPOL
#MCFLAG_OPT += -DCONFIG_CTRL_IFACE_UNIX
MCFLAG_OPT += -DPCSC_FUNCS
MCFLAG_OPT += -DCONFIG_DRIVER_NONE
MCFLAG_OPT += -DRTW_WPA_TODO
MCFLAG_OPT += -DCONFIG_NO_STDOUT_DEBUG
MCFLAG_OPT += -DCONFIG_NO_WPA_MSG
MCFLAG_OPT += -DCONFIG_NO_CONFIG_WRITE
MCFLAG_OPT += -DCONFIG_TLS_INTERNAL_CLIENT
MCFLAG_OPT += -DCONFIG_INTERNAL_LIBTOMMATH
#this is temp
MINCPATH = DAPS/source/wifi/eap_sim_stack/wpa_supplicant
MINCPATH += DAPS/source/wifi/eap_sim_stack/wpa_supplicant/dbus
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/common
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/crypto
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/drivers
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_common
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_peer
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eapol_supp
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/l2_packet
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/ap
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/crypto
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/drivers
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_common
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_peer
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_server
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eapol_auth
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/eapol_supp
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/l2_packet
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/p2p
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/radius
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/rsn_supp
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/utils
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/wps
MINCPATH += DAPS/source/wifi/eap_sim_stack/src/tls
MINCPATH += DAPS/source/wifi/rtl8723/os_dep/threadx
MINCPATH += MS_Customize/source/product/driver/wifi/rtl8723
MINCPATH += ms_mmi/source/mmi_kernel/include 
MINCPATH += BASE/l4/export/inc
MINCPATH += BASE/PS/export/inc
MINCPATH += BASE/Layer1/export/inc
MINCPATH += BASE/sim/export/inc

MSRCPATH = MS_Customize/source/product/driver/wifi/rtl8723
MSRCPATH += DAPS/source/wifi/eap_sim_stack/wpa_supplicant
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/common
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/crypto
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/drivers
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_common
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/eap_peer
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/eapol_supp
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/l2_packet
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/utils
MSRCPATH += DAPS/source/wifi/eap_sim_stack/src/tls
#rtw_debug.c
#rtw_eeprom.c
#rtw_ioctl_query.c
#rtw_ioctl_rtl.c
SOURCES	 = config.c notify.c bss.c eap_register.c\
	   config_none.c\
	   wpa_supplicant.c events.c blacklist.c \
	   wpas_glue.c scan.c main_none.c

SOURCES	+= common.c wpa_debug.c wpabuf.c os_none.c eloop_none.c \
	   pcsc_funcs.c base64.c
SOURCES	+= eap_sim.c eap.c eap_methods.c eap_peap.c eap_tls_common.c \
	   mschapv2.c sha1-tlsprf.c eap_mschapv2.c
SOURCES	+= eap_sim_common.c eap_common.c eap_peap_common.c chap.c
SOURCES	+= driver_none.c drivers.c
SOURCES	+= eapol_supp_sm.c
#SOURCES	+= l2_packet_none.c
SOURCES	+= crypto_internal.c aes-internal.c \
	   aes-internal-dec.c \
	   aes-unwrap.c aes-cbc.c aes-internal-enc.c \
	   rc4.c sha1.c\
	   sha1-internal.c fips_prf_internal.c \
	   tls_internal.c ms_funcs.c md5.c \
	   crypto_internal-rsa.c crypto_internal-cipher.c \
	   crypto_internal-modexp.c des-internal.c \
	   sha256-internal.c md5-internal.c md4-internal.c
SOURCES	+= tlsv1_client_read.c tlsv1_client_write.c \
	   tlsv1_common.c tlsv1_client.c \
	   tlsv1_record.c tlsv1_cred.c \
	   x509v3.c asn1.c rsa.c pkcs8.c \
	   pkcs5.c pkcs1.c bignum.c 
