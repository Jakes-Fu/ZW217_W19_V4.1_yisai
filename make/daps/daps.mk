# + DAPS module START
#----------------------------------------------------------------------------#
#                                DAPS COMMON                                 #
#----------------------------------------------------------------------------#

MINCPATH      = DAPS/export/inc \
                DAPS/source/common/inc \
                DAPS/source/common/inc/string \
                DAPS/source/common/inc/encrypt \
                DAPS/source/common/inc/mem_debug \
                DAPS/source/common/inc/mime_type

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif

MSRCPATH      = DAPS/source/common/src \
                DAPS/source/common/src/string \
                DAPS/source/common/src/encrypt \
                DAPS/source/common/src/mem_debug \
                DAPS/source/common/src/list \
                DAPS/source/common/src/timer \
                DAPS/source/common/src/time \
                DAPS/source/common/src/mime_type \
                DAPS/source/common/src/mem                 

SOURCES       = cfl_char.c cfl_wchar.c cfl_string.c\
                cfl_base64.c cfl_aescbc.c \
                mime_type.c cfl_mem_debug.c cfl_list.c cfl_timer.c cfl_time.c cfl_md5.c cfl_mem.c

#----------------------------------------------------------------------------#
#                            DL_MALLOC                                       #
#----------------------------------------------------------------------------#
MCFLAG_OPT   += -DDL_MALLOC_SUPPORT

MSRCPATH     += DAPS/source/common/src/dl_malloc

SOURCES      += dl_malloc.c dlmalloc_api.c 

# + DAPS sub-module start
# All DAPS sub-module configurations should be added bellow

#----------------------------------------------------------------------------#
#                            LIBWBXML                                        #
#----------------------------------------------------------------------------#
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/wbxml_parser/inc

MSRCPATH     += DAPS/source/wbxml_parser/src

#SOURCES      += wbxml_base64.c wbxml_buffers.c wbxml_charset.c wbxml_elt.c \
#                wbxml_lists.c wbxml_mem.c wbxml_parser.c wbxml_tables.c





#----------------------------------------------------------------------------#
#                                HSM                                         #
#----------------------------------------------------------------------------# 
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/hsm/inc

MSRCPATH     += DAPS/source/hsm/src 

SOURCES      += Cplus.c Qf.c Qhsm.c QhsmTran.c



#----------------------------------------------------------------------------#
#                                   SSL                                      #
#----------------------------------------------------------------------------#
ifeq ($(strip $(SSL_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS -DTHREADX_OS

MINCPATH     += DAPS/export/inc \
	        DAPS/source/matrix_ssl/src/core \
	        DAPS/source/matrix_ssl/src/crypto \
	        DAPS/source/matrix_ssl/src/crypto/digest \
	        DAPS/source/matrix_ssl/src/crypto/keyformat \
	        DAPS/source/matrix_ssl/src/crypto/math \
	        DAPS/source/matrix_ssl/src/crypto/prng \
	        DAPS/source/matrix_ssl/src/crypto/symmetric \
	        DAPS/source/matrix_ssl/src/crypto/pubkey \
	        DAPS/source/matrix_ssl/src/matrixssl \
	        DAPS/source/matrix_ssl/src/sslwrapper/h 
	        

MSRCPATH     += DAPS/source/matrix_ssl/src/core \
          DAPS/source/matrix_ssl/src/core/POSIX \
	        DAPS/source/matrix_ssl/src/crypto \
	        DAPS/source/matrix_ssl/src/crypto/digest \
	        DAPS/source/matrix_ssl/src/crypto/keyformat \
	        DAPS/source/matrix_ssl/src/crypto/math \
	        DAPS/source/matrix_ssl/src/crypto/prng \
	        DAPS/source/matrix_ssl/src/crypto/pubkey \
	        DAPS/source/matrix_ssl/src/crypto/symmetric \
	        DAPS/source/matrix_ssl/src/matrixssl \
	        DAPS/source/matrix_ssl/src/sslwrapper/c 

SOURCES      += ssl_alert.c ssl_algo.c ssl_algoprocess.c ssl_api.c hmacsha_api.c ssl_cert.c \
		ssl_event.c ssl_handshake.c ssl_machine.c ssl_pdu.c ssl_pdusupport.c \
		ssl_session.c   matrixssl.c \
		   hmac.c md2.c matrix_md4.c matrix_md5.c sha1.c sha224.c sha256.c sha384.c sha512.c \
		asn1.c base64.c x509.c pstm.c pstm_montgomery_reduce.c pstm_mul_comba.c pstm_sqr_comba.c \
		prng.c yarrow.c dh.c ecc.c pkcs.c pubkey.c rsa.c aes.c aesGCM.c arc4.c des3.c rc2.c seed.c rc5.c \
		corelib.c psmalloc.c osdep.c sslEncode.c sslDecode.c psk.c cipherSuite.c tls.c prf.c matrixsslApi.c sslv3.c
endif
#----------------------------------------------------------------------------#
#                                   WTLS                                     #
#----------------------------------------------------------------------------#
ifeq ($(strip $(WSP_SUPPORT)), TRUE)
ifeq ($(strip $(SSL_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS 

MINCPATH     += DAPS/export/inc \
          DAPS/source/matrix_ssl/src/core \
          DAPS/source/matrix_ssl/src/core/POSIX \
	        DAPS/source/matrix_ssl/src/crypto \
	        DAPS/source/matrix_ssl/src/crypto/digest \
	        DAPS/source/matrix_ssl/src/crypto/keyformat \
	        DAPS/source/matrix_ssl/src/crypto/math \
	        DAPS/source/matrix_ssl/src/crypto/prng \
	        DAPS/source/matrix_ssl/src/crypto/pubkey \
	        DAPS/source/matrix_ssl/src/crypto/symmetric \
	        DAPS/source/matrix_ssl/src/matrixssl \
	        DAPS/source/matrix_wtls/inc        

MSRCPATH     += DAPS/source/matrix_wtls/src 

SOURCES      += wtls_alert.c wtls_algo.c wtls_algoprocess.c wtls_api.c wtls_cert.c \
		wtls_event.c wtls_handshake.c wtls_machine.c wtls_pdu.c wtls_pdusupport.c \
		wtls_session.c 
endif
endif
#----------------------------------------------------------------------------#
#                           NetManager                                       #
#----------------------------------------------------------------------------#
ifeq ($(strip $(HTTP_SUPPORT)), TRUE)
MINCPATH     += DAPS/source/netmanager/inc \
				DAPS/source/netmanager/netmgr/h \
				DAPS/source/netmanager/nettrans/h   
 
MSRCPATH     += DAPS/source/netmanager/netmgr/c \
				DAPS/source/netmanager/nettrans/c    
				
SOURCES      += netmgr_api.c netmgr_host.c netmgr_instance.c netmgr_request.c netmgr_url.c nettrans_api.c nettrans_connection.c\
				nettrans_msg.c nettrans_sock.c
endif

#----------------------------------------------------------------------------#
#                                  HTTP                                      #
#----------------------------------------------------------------------------#
ifeq ($(strip $(HTTP2_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/http2/inc \
                DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/common/inc \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/http2/src

SOURCES      += http_api.c http_auth.c http_cache.c http_cache_api.c \
                http_chunk.c http_cookie_api.c http_date_parser.c http_event.c \
                http_header.c http_hsm.c http_request.c http_response.c http_serialize.c \
                http_session.c http_signal.c http_util.c 

MINCPATH     += DAPS/source/http2/inc/nghttp2
				
MSRCPATH     += DAPS/source/http2/src/nghttp2

SOURCES      += nghttp2_pq.c nghttp2_map.c nghttp2_queue.c \
				nghttp2_frame.c \
				nghttp2_buf.c \
				nghttp2_stream.c nghttp2_outbound_item.c \
				nghttp2_session.c nghttp2_submit.c \
				nghttp2_helper.c \
				nghttp2_npn.c \
				nghttp2_hd.c nghttp2_hd_huffman.c nghttp2_hd_huffman_data.c \
				nghttp2_version.c \
				nghttp2_priority_spec.c \
				nghttp2_option.c \
				nghttp2_callbacks.c \
				nghttp2_mem.c \
				nghttp2_http.c \
				nghttp2_rcbuf.c \
				nghttp2_debug.c
else
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/http/inc \
                DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/common/inc \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/http/src

SOURCES      += http_api.c http_auth.c http_cache.c http_cache_api.c \
                http_chunk.c http_cookie_api.c http_date_parser.c http_event.c \
                http_header.c http_hsm.c http_request.c http_response.c http_serialize.c \
                http_session.c http_signal.c http_util.c 
endif

#----------------------------------------------------------------------------#
#                                  dhcps                                       #
#----------------------------------------------------------------------------#
ifeq ($(strip $(DHCPS_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/dhcp4_server/inc \
                DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
				DAPS/source/tcpip6/src/armport \
                DAPS/source/common/inc \
                DAPS/source/common/inc/mem_debug \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/dhcp4_server/src

SOURCES      += arpping.c dhcpd.c dhcps_api.c dhcps_common.c dhcps_files.c \
                dhcps_leases.c dhcps_packet.c dhcps_socket.c static_leases.c
endif

#----------------------------------------------------------------------------#
#                                  WSP                                       #
#----------------------------------------------------------------------------#
ifeq ($(strip $(WSP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/wsp/inc \
                DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/common/inc \
                DAPS/source/common/inc/mem_debug \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/wsp/src

SOURCES      += wsp_api.c wsp_auth.c wsp_cache.c wsp_event.c \
                wsp_header.c wsp_hsm.c wsp_ocstr.c wsp_request.c \
                wsp_response.c wsp_serialize.c wsp_session.c wsp_signal.c \
                wtp_api.c wtp_hsm.c wtp_pdu.c
endif

#----------------------------------------------------------------------------#
#                                  WSP  header decoder                       #
#----------------------------------------------------------------------------#
ifeq ($(strip $(WSP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS 

MINCPATH     += DAPS/source/wsp_header_decoder/inc \
				DAPS/source/common/inc \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/wsp_header_decoder/src

SOURCES      += wsp_list.c wsp_octstr.c wsp_parse.c wsp_header_api.c wsp_headers.c wsp_strings.c

endif

#----------------------------------------------------------------------------#
#                                XML( Expat )                                #
#----------------------------------------------------------------------------#   
ifeq ($(strip $(XML_SUPPORT)), TRUE) 
MCFLAG_OPT   += -DXML_SUPPORT

MINCPATH     += DAPS/source/xml_expat/inc \
                DAPS/source/common/inc/string
                
MSRCPATH     += DAPS/source/xml_expat/src \
                DAPS/source/common/src/string
                		  
SOURCES      += xmlexpatif.c xmlparse.c \
                xmlrole.c xmltok.c xmltest.c  
endif              


#----------------------------------------------------------------------------#
#                                OMA Download                                #
#----------------------------------------------------------------------------#                
ifeq ($(strip $(OMADL_SUPPORT)), TRUE)

MCFLAG_OPT   += -D_RTOS -DODM_DEBUG -DDUA_DEBUG

MINCPATH     += DAPS/source/omadl/inc \
                DAPS/export/inc

MSRCPATH     += DAPS/source/omadl/src 

SOURCES      += conn_interface_manager.c  content_handler_manager.c omadl_serialize.c \
                dd_xml_handler.c  dua.c  generic_content_handler.c  \
                http_interface.c  odm.c  omadl_util.c
ifeq ($(strip $(WSP_SUPPORT)), TRUE)				
SOURCES      +=	wsp_interface.c
endif
endif          

#----------------------------------------------------------------------------#
#                                OMA DRM                                     #
#----------------------------------------------------------------------------#                
ifneq ($(strip $(DRM_SUPPORT)),)
ifneq ($(strip $(DRM_SUPPORT)), NONE)
	MCFLAG_OPT   += -DDRM_SUPPORT
	ifeq ($(strip $(CHIP_VER)), 6600L7)
	MCFLAG_OPT   += -DDRM_IRAM_ENABLE
	else
	ifeq ($(strip $(CHIP_VER)), 6600L2)
	MCFLAG_OPT   += -DDRM_IRAM_ENABLE
	else
	ifeq ($(strip $(CHIP_VER)), 6600L1)
	MCFLAG_OPT   += -DDRM_IRAM_ENABLE
	else
	ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
	MCFLAG_OPT   += -DDRM_LITTLEBIAN_ENABLE
	endif
	endif
	endif
	endif
    ifeq ($(strip $(DRM_SUPPORT)), V1)
    MCFLAG_OPT   += -DDRM_SURPORT_FL_TYPE
    MCFLAG_OPT   += -DDRM_SURPORT_CD_TYPE
	MCFLAG_OPT   += -DDRM_SURPORT_SD_TYPE
	endif
    ifeq ($(strip $(DRM_SUPPORT)), FL)
    MCFLAG_OPT   += -DDRM_SURPORT_FL_TYPE
	endif
    ifeq ($(strip $(DRM_SUPPORT)), CD)
    MCFLAG_OPT   += -DDRM_SURPORT_CD_TYPE
	endif
    ifeq ($(strip $(DRM_SUPPORT)), SD)
    MCFLAG_OPT   += -DDRM_SURPORT_SD_TYPE
	endif

  MINCPATH     +=$(PS_INCPATH)
  
  #MINCPATH += $(ATC_INCPATH)
  #MINCPATH += $(L4_INCPATH)
  #MINCPATH += $(SIM_INCPATH) 
    
	MINCPATH     += DAPS/source/omadrm/inc \
	                BASE/l4/export/inc \
                    BASE/sim/export/inc \
	                MS_Ref/export/inc
    
    MSRCPATH     += DAPS/source/omadrm/src
    
    SOURCES      += drm_dh.c drm_io.c drm_io_cfg.c drm_io_impl_fl_cd.c \
                    drm_io_impl_sd.c drm_common.c \
                    drm_xml.c drm_api.c drm_sfs.c
endif
endif


#----------------------------------------------------------------------------#
#                                  STREAMING                                 #
#----------------------------------------------------------------------------#
ifndef LOW_MEMORY_SUPPORT
    LOW_MEMORY_SUPPORT = NONE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), FALSE)
    LOW_MEMORY_SUPPORT = NONE
endif

ifneq ($(strip $(LOW_MEMORY_SUPPORT)), NONE)
    STRM_SUPPORT_LOW_MEMORY = BOTH
endif

ifeq ($(strip $(STRM_SUPPORT_LOW_MEMORY)), BOTH)
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict
else
    ifeq ($(strip $(STRM_SUPPORT_LOW_MEMORY)), BUFFER)
        MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    endif
    
    ifeq ($(strip $(STRM_SUPPORT_LOW_MEMORY)), CONFLICT)
        MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT    #streaming support low memory, conflict
    endif
endif

ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS

MINCPATH     += DAPS/source/http/inc \
                DAPS/source/Streaming/inc \
                DAPS/export/inc \
                MS_Ref/export/inc
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH     += DAPS/source/Streaming/src

SOURCES      += strm_rtp.c strm_sdp.c strm_api.c strm_serialize.c strm_buffer.c \
                strm_rtsp.c strm_hsm.c strm_test_ref.c
endif                
#----------------------------------------------------------------------------#
#                                 SDP                                        #
#----------------------------------------------------------------------------#
#ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS -DOS_THREADX -DDAPS_SDP_ONLY -D_CTYPE_DISABLE_MACROS

MINCPATH     += DAPS/source/ims/inc \
                DAPS/source/ims/src/sdp \
                DAPS/source/ims/src/sdp/codecs \
                DAPS/source/ims/src/common/cutils \
                DAPS/source/ims/src/common/cbase \
                DAPS/source/ims/src/common/adapters \
                DAPS/source/ims/src/common/adapters/threadx \
                DAPS/source/ims/src/common/ceutils \
                DAPS/source/ims/src/common/ares \
                DAPS/source/ims/src/common/config \
                DAPS/source/ims/src/common/config/arch \
                DAPS/source/ims/src/common/config/os \
                DAPS/source/ims/src/common/ccore \
                DAPS/source/ims/src/common/ccore/netdrivers \
                DAPS/source/ims/src/common/ccore/memdrivers \
                DAPS/source/ims/src/common/config/tool \
                DAPS/source/ims/src/ads \
                DAPS/source/ims/src/sip/common \
                DAPS/source/ims/src/sip/stackMgr \
                DAPS/source/ims/src/sip/message \
                DAPS/source/ims/src/sip/mid \
                DAPS/source/ims/src/sip/parser \
                DAPS/source/ims/src/sip/pubclient \
                DAPS/source/ims/src/sip/regclient \
                DAPS/source/ims/src/sip/resolver \
                DAPS/source/ims/src/sip/subscription \
                DAPS/source/ims/src/sip/transaction \
                DAPS/source/ims/src/sip/transaclient \
                DAPS/source/ims/src/sip/transmitter \
                DAPS/source/ims/src/sip/authenticator \
                DAPS/source/ims/src/sip/compartment \
                DAPS/source/ims/src/sip/call \
                DAPS/source/ims/src/sip/transport
                 

MSRCPATH     += DAPS/source/ims/src/sdp \
                DAPS/source/ims/src/manager \
                DAPS/source/ims/src/sdp \
                DAPS/source/ims/src/sdp/codecs \
                DAPS/source/ims/src/common/cutils \
                DAPS/source/ims/src/common/cbase \
                DAPS/source/ims/src/common/adapters \
                DAPS/source/ims/src/common/adapters/threadx \
                DAPS/source/ims/src/common/adapters/pclock \
                DAPS/source/ims/src/common/ceutils \
                DAPS/source/ims/src/common/ares \
                DAPS/source/ims/src/common/config \
                DAPS/source/ims/src/common/config/arch \
                DAPS/source/ims/src/common/config/os \
                DAPS/source/ims/src/common/ccore \
                DAPS/source/ims/src/common/ccore/netdrivers \
                DAPS/source/ims/src/common/ccore/memdrivers \
                DAPS/source/ims/src/common/config/tool \
                DAPS/source/ims/src/ads \

                                 
SOURCES      += rvsdpattr.c rvsdpbadsyntax.c rvsdpbandwidth.c rvsdpconnection.c \
                rvsdpcrypto.c rvsdpemail.c rvsdpglobals.c rvsdpinit.c rvsdpkey.c \
                rvsdpkeymgmt.c rvsdplist.c rvsdpmedia.c rvsdpmediagroup.c \
                rvsdpmegaco.c rvsdpmsg.c rvsdpmsgparse.c rvsdpmsgreparse.c rvsdporigin.c \
                rvsdpphone.c rvsdpprecondition.c rvsdpprsutils.c rvsdprtcp.c rvsdprtpmap.c \
                rvsdpsesstime.c rvsdpstrings.c rvsdpcodec_amr.c rvsdpcodec_dtmf.c \
                rvsdpcodec_g722.c rvsdpcodec_g7231.c rvsdpcodec_g726.c rvsdpcodec_g729.c \
                rvsdpcodec_h261.c rvsdpcodec_h263.c rvsdpcodec_mp4a_latm.c rvsdpcodec_mp4v_es.c \
                rvsdpcodec_pcma.c rvsdpcodec_pcmu.c rvsdpcodecparseutils.c sdp_api.c rvsdpcodecs.c \
                rvalloc.c rvcbase.c rvglobals.c AdsRpool.c rvmemory.c rvtimerengine.c \
                rvqueue.c rvccore.c rvbase64.c rvtimer.c rvccoreglobals.c AdsRa.c rvlog.c rvosmem.c \
                rvobjlist.c rvthread.c rvstdio.c rvpqueue.c rvobjpool.c rvselect.c rvtimestamp.c \
                rvmutex.c rvsemaphore.c rvlock.c rv64ascii.c rvtm.c rvsocket.c rvhost.c rvtls.c \
                rvares.c rvloglistener.c rvclock.c AdsCopybits.c rvadlock.c rvadmutex.c rvadsema4.c \
                rvadthread.c rvadtstamp.c rvthreadtls.c rvportrange.c rvnet2host.c \
                rvadclock.c rvrandomgenerator.c 
#endif                

#----------------------------------------------------------------------------#
#                                  RTSP                                      #
#----------------------------------------------------------------------------#
ifeq ($(strip $(RTSP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MCFLAG_OPT += -DIPV6_SUPPORT
endif

MINCPATH     += DAPS/source/http/inc \
                DAPS/source/rtsp/inc \
                DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/common/inc \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/rtsp/src

SOURCES      += rtsp_adapter.c rtsp_api.c rtsp_auth.c rtsp_cookie.c rtsp_event.c rtsp_header.c \
		rtsp_host.c rtsp_hsm.c rtsp_method.c rtsp_options_hsm.c rtsp_request.c rtsp_response.c \
		rtsp_serialize.c rtsp_setparam_hsm.c rtsp_signal.c rtsp_util.c
endif

# All DAPS sub-module configurations should be added above
# - DAPS sub-module end
# All DAPS sub-module configurations should be added above
# - DAPS sub-module end



#----------------------------------------------------------------------------#
#                                DAPS start                                  #
#----------------------------------------------------------------------------#

MINCPATH     += DAPS/source/common/inc \
                DAPS/source/http/inc \
                DAPS/source/netmanager/inc
		
MSRCPATH     += DAPS/source/common/src
	
SOURCES      += daps_start.c

#----------------------------------------------------------------------------#
#                                  RTP                                       #
#----------------------------------------------------------------------------#
ifeq ($(strip $(RTP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS -DOS_THREADX

MINCPATH     += DAPS/source/ims/inc \
				DAPS/source/ims/src/rtpRtcp/rtp \
				DAPS/source/ims/src/common/cutils \
				DAPS/source/ims/src/common/ccore \
				DAPS/source/ims/src/common/config \
				DAPS/source/ims/src/common/config/arch \
				DAPS/source/ims/src/common/config/os \
				DAPS/source/ims/src/common/config/tool \
				DAPS/source/ims/src/common/ccore/netdrivers \
                DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/common/inc \
                DAPS/source/common/inc/mem_debug \
                DAPS/export/inc 

MSRCPATH     += DAPS/source/ims/src/common/cutils \
				DAPS/source/ims/src/rtpRtcp/rtp \
				DAPS/source/ims/src/manager

SOURCES      += rtp_api.c rtp_mgr.c bitfield.c rtcp.c rtp.c  \
                rtputil.c rvrtpbuffer.c rtp_test.c rvntptime.c rvaddress.c rvtime.c rv64bits.c
endif

#----------------------------------------------------------------------------#
#                                  RTP_Payload                               #
#----------------------------------------------------------------------------#
ifeq ($(strip $(RTP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS -DOS_THREADX

MINCPATH     += DAPS/source/rtp_payload/inc \
                DAPS/export/inc \
                DAPS/source/common/inc \
                DAPS/source/common/inc/mem_debug

MSRCPATH     += DAPS/source/rtp_payload/src

SOURCES      += amr_unpack.c h263_unpack.c h264_unpack.c mpeg4_unpack.c

endif  

#----------------------------------------------------------------------------#
#                                  SNTP			                                 #
#----------------------------------------------------------------------------#

ifeq ($(strip $(SNTP_SUPPORT)), TRUE)
MCFLAG_OPT   += -DSNTP_SUPPORT

MINCPATH     += DAPS/source/sntp/inc \
                DAPS/export/inc

MSRCPATH     += DAPS/source/sntp/src

SOURCES      += sntp.c sntp_dst.c sntp_unit.c sntp_api.c

endif  
#----------------------------------------------------------------------------#
#                                  OAUTH	                                 #
#----------------------------------------------------------------------------#
ifeq ($(strip $(OAUTH_SUPPORT)), TRUE)
MINCPATH     += DAPS/source/oauth/inc \
                DAPS/export/inc \
                MS_MMI_Main/source/mmi_app/app/netproxy/h

MSRCPATH     += DAPS/source/oauth/src

SOURCES      += oauth.c cJSONParser.c

endif  
#----------------------------------------------------------------------------#
#                                  iperf   			        	             #
#----------------------------------------------------------------------------#
MINCPATH     += DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/common/inc \
                DAPS/export/inc/ \
				DAPS/source/iperf/inc \
				MSL/l4/inc 

MSRCPATH     += DAPS/source/iperf/src

SOURCES		 += daps_iperf.c
# - DAPS module END
         