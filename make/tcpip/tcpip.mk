# + TCPIP module START
#----------------------------------------------------------------------------#
#                                DAPS COMMON                                 #
#----------------------------------------------------------------------------#
MINCPATH      = DAPS/export/inc

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH   += chip_drv/export/inc
endif


ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)

MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT

#----------------------------------------------------------------------------#
#                               TCPIP6                                        #
#----------------------------------------------------------------------------#
ifeq ($(strip $(TCPIP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS
MCFLAG_OPT   += -DINCLUDE_TCP -DNB_CONNECT -DFULL_ICMP -DIP_FRAGMENTS \
                -DIP_ROUTING -DMULTI_HOMED -DDNS_CLIENT  \
                -DPUTC_UART -DRAM_BUFFER -DASYNC_SUPPORT \
                -DPKTOUTPUT_ENABLE -DBUFF_OPTIMIZE -DWTCP_OPTIMIZE \
                -DINCLUDE_ARP -DPING_APP -DEAPOL_ENABLE

ifeq ($(strip $(VM_VLX)), TRUE)
MCFLAG_OPT   += -DVETHER_ENABLE
endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_MUX_ENABLE_
endif

ifeq ($(strip $(TCPIP_MODEM_ONLY_SUPPORT)), TRUE)
MCFLAG_OPT   += -DMODEM_ONLY
endif


ifeq ($(strip $(PLATFORM)), SC6530)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6600R)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
else
ifeq ($(strip $(PLATFORM)), SC8501C)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
else
ifeq ($(strip $(PLATFORM)), SC7702)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif
ifeq ($(strip $(PLATFORM)), SC7703A)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif
endif
endif

# 7701 dusing hardware PPP encode/decode module
ifeq ($(strip $(HWPPP_USB)), TRUE)
MCFLAG_OPT   += -DPPP_HARD_SUPPORT
endif
# 7702 data card using hardware PPP encode/decode module
ifeq ($(strip $(USE_NEW_USB_FRAMEWORK)), TRUE)
MCFLAG_OPT   += -DPPP_HARD_SUPPORT
endif

# here just adaptive to 09a platform
ifneq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   += -DNEW_USBPORT
endif
endif

MINCPATH += $(ATC_INCPATH)
MINCPATH += $(L4_INCPATH)
MINCPATH += $(SIM_INCPATH)

ifeq ($(strip $(PRODUCT_DM)), TRUE)
MINCPATH     += atc_3g/export/inc
else
MINCPATH     += BASE/atc/export/inc
endif

MINCPATH     += DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
		DAPS/source/tcpip6/src/ppp_mp/inc \
                DAPS/source/tcpip6/src/armport \
                DAPS/source/tcpip6/src/autoip4 \
				DAPS/source/tcpip6/src/crypt/rsa \
				DAPS/source/tcpip6/src/crypt/rc5 \
                DAPS/source/tcpip6/src/crypt/bf \
                DAPS/source/tcpip6/src/crypt \
                DAPS/source/tcpip6/src/dhcpv6 \
                DAPS/source/tcpip6/src/ftp \
                DAPS/source/tcpip6/src/ip \
                DAPS/source/tcpip6/src/ipsec \
                DAPS/source/tcpip6/src/ipv6 \
                DAPS/source/tcpip6/src/misclib \
                DAPS/source/tcpip6/src/modem \
                DAPS/source/tcpip6/src/mschap \
                DAPS/source/tcpip6/src/natrt \
                DAPS/source/tcpip6/src/ftp \
                DAPS/source/tcpip6/src/net \
                DAPS/source/tcpip6/src/snmp \
                DAPS/source/tcpip6/src/tcp \
                DAPS/source/tcpip6/src/telnet \
                DAPS/source/tcpip6/src/tftp \
                BASE/l4/export/inc \
                BASE/sim/export/inc \
                mux/include \
                MS_Customize/export/inc

ifeq ($(strip $(VM_VLX)), TRUE)
MINCPATH     += vlx_plugin/export/nk
endif
                
MINCPATH     += chip_drv/export/inc/outdated \
                chip_drv/chip_plf/uws6121e

MINCPATH     +=$(PS_INCPATH)

MSRCPATH     += DAPS/source/tcpip6/src \
                DAPS/source/tcpip6/src/allports \
                DAPS/source/tcpip6/src/armport \
                DAPS/source/tcpip6/src/autoip4 \
                DAPS/source/tcpip6/src/cli \
                DAPS/source/tcpip6/src/crypteng \
                DAPS/source/tcpip6/src/dhcpv6 \
                DAPS/source/tcpip6/src/ftp \
                DAPS/source/tcpip6/src/igmp3 \
                DAPS/source/tcpip6/src/ip \
                DAPS/source/tcpip6/src/ipmc \
                DAPS/source/tcpip6/src/ipsec \
                DAPS/source/tcpip6/src/ipv6 \
                DAPS/source/tcpip6/src/misclib \
                DAPS/source/tcpip6/src/mld \
                DAPS/source/tcpip6/src/modem \
                DAPS/source/tcpip6/src/mschap \
                DAPS/source/tcpip6/src/natrt \
                DAPS/source/tcpip6/src/net \
                DAPS/source/tcpip6/src/ppp_mp/src \
                DAPS/source/tcpip6/src/snmp \
                DAPS/source/tcpip6/src/tcp \
                DAPS/source/tcpip6/src/telnet \
                DAPS/source/tcpip6/src/tftp

ifeq ($(strip $(PLATFORM)), UWS6121E)
MSRCPATH     += DAPS/source/tcpip_ipcBear/src
endif
				
SOURCES      += allports.c timeouts.c \
                ininit.c inmain.c intimer.c inport.c mem_check.c userdata.c uartutil.c         \
				osport.c tcpip_bearer.c tcpip_ps.c xip_sec.c autoip.c upnp.c cli.c cli_group_nt.c         \
				cli_mod.c cli_nt.c \
				ce_mod.c ce_nt.c ciphers.c crypteng.c cu_aes.c cu_app.c cu_bf.c cu_des.c       \
				cu_hmac.c cu_md5.c cu_niche.c cu_null.c cu_rsa.c cu_sha1.c                     \
                create_sockets.c dhcpv6_loop.c dhcpv6_port.c dhcpv6_utils.c dhcpv6_bld_msg.c \
                dhcpv6_mod.c dhcpv6_bld_send_msg.c dhcpv6_nt.c dhcpv6_process_msg.c dhcpv6_server.c \
                igmp3.c \
                et_arp.c icmp.c iface.c ip.c ip_reasm.c ipdemux.c ipmc.c ipnet.c ipport.c      \
                ipraw.c iproute.c ipstart.c pmtu.c rtbtree.c udp.c \
                igmp.c igmp2.c igmp_cmn.c igmp_mod.c igmp_nt.c ipmc46.c ipopt.c msf.c          \
                u_mctest.c \
                callbacks.c classify.c classify_sp.c crypto.c inet.c iniche.c iniche_intf.c    \
                ipcksum.c ipsec.c ipsec_id.c ipsec_mod.c ipsec_nt.c ipsecip6.c ipsecmgmt.c     \
                ipsecmisc.c ipsecrst.c ipsecsession.c log.c mgmt.c mgmtapi.c myaddrs.c         \
                os_iniche.c packet.c pkt_iniche.c process.c replay.c route.c spd.c sync_api.c  \
                timer.c util.c valnames.c \
                icmp6.c ip6.c ip62mac.c ip6_pcb.c ip6ext.c ip6frags.c ip6mc.c ip6route.c       \
                ipv6_mod.c ipv6_nt.c nd.c tcp6.c udp6.c udpsock6.c \
                app_tcp_if.c ascii_utils.c async_gethost.c async_sock.c cb.c                   \
				cksum.c console_mod.c dhcsetup.c do_script.c  genlist.c gio.c in_utils.c       \
				nextcarg.c parseip.c profiler.c reshost.c strilib.c strlib.c syslog.c          \
				syslog_mod.c syslog_nt.c task.c tcp_echo.c tcpcksum.c tcpip_debug.c ttyio.c    \
				udp_echo.c user_nt.c userpass.c mld.c mld_cmn.c mldv2.c \
                nat_mod.c nat_nt.c natfrag.c natftp.c natport.c nattl.c natutil.c \
                app_ping.c chainbuff_nt.c dc_mod.c dc_nt.c dhcpclnt.c dhcputil.c diag_mod.c    \
                diag_nt.c dnsc_nt.c dnsclnt.c ifmap.c macloop.c netif.c netmain_mod.c          \
				netmain_nt.c ping.c pktalloc.c q.c q_txcnf.c slip.c slipif.c tx_flowctrl.c     \
				udp_open.c       \
                in_pcb.c nptcp.c rawsock.c sockcall.c socket.c socket2.c soselect.c tcp_in.c   \
                tcp_out.c tcp_subr.c tcp_timr.c tcp_usr.c tcp_zio.c tcpport.c       \
                tcpsack.c udpsock.c \
                tcpip_version.c                       
ifeq ($(strip $(TELNET_SUPPORT)), TRUE)
SOURCES += comport.c rfc2217.c telerr.c telnet.c telnet_mod.c telnet_nt.c telparse.c  telport.c
endif
ifeq ($(strip $(TFTP_SUPPORT)), TRUE)				
SOURCES += tftp_mod.c tftp_nt.c tftpcli.c tftpsrv.c tftpudp.c tftputil.c
endif	
ifeq ($(strip $(FTP_SUPPORT)), TRUE)
SOURCES +=	ftp_mod.c  ftp_nt.c ftpclnt.c ftpcprn.c  ftpssock.c    
endif				
SOURCES +=  ppp_mp_api.c ppp_action_ctrl.c ppp_action_func.c	ppp_entry.c	ppp_event_ctrl.c ppp_event_func.c	\
           ppp_frame_receive_ctrl.c	ppp_frame_receive_func.c ppp_frame_send_ctrl.c	ppp_frame_send_func.c	\
           ppp_lcp_ctrl.c ppp_lcp_func.c	ppp_main_func.c ppp_ncp_ctrl.c ppp_ncp_func.c ppp_packet_receive_ctrl.c \
 	     		 ppp_packet_receive_func.c ppp_packet_send_ctrl.c	ppp_packet_send_func.c ppp_timer_ctrl.c ppp_timer_func.c \
           ppp_auth_chap_ctrl.c ppp_auth_chap_func.c ppp_auth_pap_ctrl.c ppp_auth_pap_func.c ppp_cl_md5.c \
           ppp_utils.c ppp_mem.c ppp_ipv6cp_ctrl.c ppp_ipv6cp_func.c ppp_io.c vjcomp.c
ifeq ($(strip $(MUX_SUPPORT)), TRUE)				
SOURCES += uart_mux.c 
endif				
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES      += tcpip_ipcBear.c 
endif				
				
endif

else
MINCPATH   += DAPS/export/inc/tcpip
#----------------------------------------------------------------------------#
#                               TCPIP                                        #
#----------------------------------------------------------------------------#
ifeq ($(strip $(TCPIP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS
MCFLAG_OPT   += -DINCLUDE_TCP -DNB_CONNECT -DFULL_ICMP -DIP_FRAGMENTS \
                -DIP_ROUTING -DMULTI_HOMED -DDNS_CLIENT -DMAC_LOOPBACK \
                -DPUTC_UART -DRAM_BUFFER -DASYNC_SUPPORT \
                -DPKTOUTPUT_ENABLE -DBUFF_OPTIMIZE -DWTCP_OPTIMIZE \
                -DUSE_PPP -DPPP_CHARIO -DUSE_COMPORT -DPPP_DNS \
                -DINCLUDE_ARP -DDHCP_CLIENT -DPING_APP -DEAPOL_ENABLE 

ifeq ($(strip $(VM_VLX)), TRUE)
MCFLAG_OPT   += -DVETHER_ENABLE
endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_MUX_ENABLE_
endif

ifeq ($(strip $(TCPIP_MODEM_ONLY_SUPPORT)), TRUE)
MCFLAG_OPT   += -DMODEM_ONLY
endif


ifeq ($(strip $(PLATFORM)), SC6530)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6600R)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif

# here just adaptive to 09a platform
ifneq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   += -DNEW_USBPORT
endif
endif

ifeq ($(strip $(PRODUCT_DM)), TRUE)
MINCPATH     += atc_3g/export/inc
else
MINCPATH     += BASE/atc/export/inc
endif

MINCPATH     += DAPS/source/tcpip/inc \
                DAPS/source/tcpip/src/h \
                DAPS/source/tcpip/src/armport \
                DAPS/source/tcpip/src/crypt \
                DAPS/source/tcpip/src/mppp \
                DAPS/source/tcpip/src/net \
                DAPS/source/tcpip/src/tcp \
                DAPS/source/tcpip/src/tftp \
                DAPS/source/tcpip/src/test \
                BASE/l4/export/inc \
                BASE/sim/export/inc \
                mux/include \
                MS_Customize/export/inc

ifeq ($(strip $(VM_VLX)), TRUE)
MINCPATH     += vlx_plugin/export/nk
endif
                
MINCPATH     += chip_drv/export/inc/outdated

MINCPATH     +=$(PS_INCPATH)

MSRCPATH     += DAPS/source/tcpip/src \
                DAPS/source/tcpip/src/allports \
                DAPS/source/tcpip/src/armport \
                DAPS/source/tcpip/src/crypt \
                DAPS/source/tcpip/src/ip \
                DAPS/source/tcpip/src/misclib \
                DAPS/source/tcpip/src/mppp \
                DAPS/source/tcpip/src/net \
                DAPS/source/tcpip/src/tcp \
                DAPS/source/tcpip/src/tftp \
                DAPS/source/tcpip/src/test

ifeq ($(strip $(PLATFORM)), UWS6121E)
MSRCPATH     += DAPS/source/tcpip_ipcBear/src
endif

SOURCES      += allports.c timeouts.c tk_misc.c \
                ccksum.c mem_check.c osport.c pclink_decode.c ppp_port.c startup_prep.c        \
                tcpip_bearer.c tcpip_pclink.c tcpip_ps.c uart_mux.c uartutil.c xip_sec.c       \
                md4.c md5.c tcpip_vether.c \
                et_arp.c icmp.c iface.c ip.c ip_reasm.c ipdemux.c ipnet.c ipraw.c iproute.c    \
                ipstart.c pmtu.c rtbtree.c udp.c \
                app_ping.c app_tcp_if.c async_sock.c bsdsock.c dhcsetup.c genlist.c in_utils.c \
                memdev.c memio.c memwrap.c menulib.c menus.c msring.c netmain.c nextcarg.c     \
                nrmenus.c nvfsio.c nvparms.c parseip.c pcycles.c profiler.c rawiptst.c         \
                reshost.c rfsim.c rttest.c soperr.c strilib.c strlib.c task.c tcp_echo.c       \
                tcpcksum.c tcpip_debug.c testmenu.c ttyio.c udp_echo.c userpass.c \
                chap.c ifppp.c ipcp.c lcp.c ppp_dhcp.c ppp_loop.c pppchar.c pppfsm.c pppmenu.c \
                pppsys.c upap.c vjcomp.c \
                dhcpclnt.c dhcputil.c dnsclnt.c eapol.c macloop.c netif.c ping.c pktalloc.c    \
                q.c q_txcnf.c tx_flowctrl.c udp_open.c \
                in_pcb.c nptcp.c rawsock.c sockcall.c socket.c socket2.c soselect.c tcp_in.c   \
                tcp_menu.c tcp_out.c tcp_subr.c tcp_timr.c tcp_usr.c tcp_zio.c tcpport.c       \
                tcpsack.c udpsock.c \
                tcpip_version.c ipconflict_chk.c async_gethost.c \
                modem_throughup.c faketcp_throughup.c pclink_utility.c
				
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES      += tcpip_ipcBear.c 
endif				
endif

endif
# - TCPIP module END
         
