ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_GPRS_
# MCFLAG_OPT   	+= -D_SUPPORT_SMS_OVER_GPRS_
endif

ifneq ($(strip $(PBCCH_REMOVE_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_PBCCH_
endif

#this macro is also defined in l4.mk,you should turn on/off this macro in the l4.mk and ps.mk at the same time
#MCFLAG_OPT   	+= -D_SUPPORT_CCBS_ 
MCFLAG_OPT   	+= -D_ENH_MEAS_SUPPORT_

# future
# ifeq ($(strip $(TCP_HEADER_COMPRESS_SUPPORT)), TRUE)
# MCFLAG_OPT      += -D_TCP_HEADER_COMPRESS
# endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_MUX_ENABLE_
endif

ifeq ($(strip $(SMART_PAGING_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_SMART_PAGE
endif


#future

MCFLAG_OPT   	+= -D_SUPPORT_DL_RANDOM_FILL_BITS_

ifeq ($(strip $(NETLOCK_SUPPORT)), TRUE)
MCFLAG_OPT += -DNET_LOCK_SUPPORT
endif

MINCPATH =  BASE/PS/export/inc BASE/PS/source/common/h BASE/PS/source/rlc/h BASE/PS/source/sm/h BASE/PS/source/sndcp/h BASE/PS/source/mac/h \
 BASE/PS/source/cc/h BASE/PS/source/dl/h BASE/PS/source/mm/h BASE/PS/source/rr/h BASE/PS/source/sms/h BASE/PS/source/ss/h \
 BASE/PS/source/sct/h BASE/PS/source/gmm/h BASE/Layer1/export/inc BASE/l4/export/inc BASE/PS/source/llc/h \
 BASE/sim/export/inc BASE/PS/source/rra/h char_lib/export/inc chip_drv/export/inc/outdated BASE/PS/source/smc/inc BASE/PS/source/smr/inc

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif

MSRCPATH =  BASE/PS/source/common/c BASE/PS/source/gmm/c BASE/PS/source/llc/c BASE/PS/source/rlc/c BASE/PS/source/sm/c BASE/PS/source/sndcp/c BASE/PS/source/cc/c \
 BASE/PS/source/mm/c BASE/PS/source/rr/c BASE/PS/source/sms/c BASE/PS/source/ss/c BASE/PS/source/sct/c BASE/PS/source/mac/c \
 BASE/PS/source/dl/c BASE/PS/source/rra/c BASE/PS/source/smc/src BASE/PS/source/smr/src PS/source/

ifeq ($(strip $(PLATFORM)),    UWS6121E)

SOURCES = state_machine.c

else
 

SOURCES   = l3func.c  sci_array.c sci_log_gprs_data.c ps_main.c ps_char_pack.c mmgmm_common_ie.c \
            csn_bitop_funcs.c \
            ps_eng.c state_machine.c ps_nas_version.c


ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
SOURCES +=  rrcell_gprs.c rrproc_gprs.c rrmsg_gprs.c rrmsg_pbcch.c
endif


ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
SOURCES     += gprs_classmark.c gmm_classmark.c gmm_encode_decode.c sm_qos.c sm_decode.c sm_encode.c sm_common.c
endif
	
ifeq ($(strip $(TCP_HEADER_COMPRESS_SUPPORT)), TRUE)
SOURCES         += rfc1144.c 			
SOURCES         += rfc2507c.c rfc2507d.c rfc2507lru.c rfc2507req.c rfc2507log.c			
endif


SOURCES         += sct_main.c
SOURCES         += sct_proc.c

SOURCES         += cc_msg.c
SOURCES         += cm_encodeie.c
SOURCES         += cc_main.c
SOURCES         += cc_entity.c

SOURCES         += cc_timer.c
SOURCES         += cc_subproc.c
SOURCES         += cc_signal.c
SOURCES         += cc_netmsg.c

SOURCES         += ss_main.c
SOURCES         += ss_signal.c
SOURCES         += ss_event.c
SOURCES         += ss_msg.c

SOURCES         += smc_common.c
SOURCES         += smc_cs_ctrl.c
SOURCES         += smc_cs_func.c
SOURCES         += smc_main_ctrl.c
SOURCES         += smr_main_control.c
SOURCES         += smr_mo_control.c
SOURCES         += smr_mo_func.c
SOURCES         += smr_mt_control.c
SOURCES         += smr_mt_func.c
SOURCES         += smr_public.c
SOURCES         += sms_common_api.c

SOURCES         += mm_main.c
SOURCES         += mm_api.c
SOURCES         += mm_signal.c
SOURCES         += mm_plmn_select.c
SOURCES         += mm_msg.c
SOURCES         += mm_timer.c
SOURCES         += mm_connection_manage.c
SOURCES         += mm_specific_procedure.c
SOURCES         += mm_gmm_procedure.c

SOURCES         += rrc_main.c
SOURCES         += rrc_search.c
SOURCES         += rrc_idle.c
SOURCES         += rrc_dedicate.c
SOURCES         += rrc_gprs.c
SOURCES         += rrc_common.c
SOURCES         += rrc_signal.c
SOURCES         += rrc_timer.c
SOURCES         += rrcell.c
SOURCES         += rrcell_reselect.c
SOURCES         += rrcell_fast_plmn.c
SOURCES         += rrcell_search_plmn.c
SOURCES         += rrcell_common.c
SOURCES         += rrproc.c
SOURCES         += rrproc_api.c
SOURCES         += rrmsg_decode_dedicate.c
SOURCES         += rrmsg_encode.c
SOURCES         += rrmsg.c

SOURCES         += dl_main.c
SOURCES         += dl_entity.c
SOURCES         += dl_buffer.c
SOURCES         += dl_timer.c
SOURCES         += dl_decode_frame.c

SOURCES         += rr_adapter.c


ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
SOURCES         += gmm_main.c
SOURCES         += gmm_signal.c
SOURCES         += gmm_subproc.c
SOURCES         += gmm_timer.c
SOURCES         += gmm_api.c

SOURCES         += sm_main.c
SOURCES         += sm_entity.c
SOURCES         += sm_signal.c
SOURCES         += sm_timer.c
SOURCES         += sm_subproc.c

SOURCES         += smc_ps_ctrl.c
SOURCES         += smc_ps_func.c

SOURCES         += compress_sdl.c
SOURCES         += sndcp_dc.c
SOURCES         += sndcp_main.c
SOURCES         += sndcp_entity.c
SOURCES         += sndcp_msg.c
SOURCES         += sndcp_pc.c
SOURCES         += sndcp_sapiinf.c 
SOURCES         += sndcp_sdl.c
SOURCES         += sndcp_snpdu.c
SOURCES         += sndcp_xid.c
SOURCES         += xid_sdl.c

SOURCES         += llc_main.c
SOURCES         += llc_entity.c
SOURCES         += llc_cipher.c
SOURCES         += llc_common.c
SOURCES         += llc_decode.c
SOURCES         += llc_encode.c
SOURCES         += llc_engineering.c
SOURCES         += llc_fcs.c
SOURCES         += llc_frame.c
SOURCES         += llc_list.c
SOURCES         += llc_msg.c
SOURCES         += llc_queue.c
SOURCES         += llc_signal.c
SOURCES         += llc_timer.c
SOURCES         += llc_xid.c

SOURCES         += rlc_common.c
SOURCES         += rlc_dbuf.c
SOURCES         += rlc_downlink.c
SOURCES         += rlc_list.c
SOURCES         += rlc_log.c
SOURCES         += rlc_main.c
SOURCES         += rlc_ms_param.c
SOURCES         += rlc_msg.c
SOURCES         += rlc_net_param.c
SOURCES         += rlc_sct.c
SOURCES         += rlc_state_machine.c
SOURCES         += rlc_t4code.c
SOURCES         += rlc_tx_pdu_pool.c
SOURCES         += rlc_uplink.c

SOURCES         += mac_main.c
SOURCES         += mac_state_idle.c
SOURCES         += mac_state_access.c
SOURCES         += mac_state_transfer.c
SOURCES         += mac_common.c
SOURCES         += mac_timer.c
SOURCES         += mac_signal.c
SOURCES         += mac_decode_data_ind.c
SOURCES         += mac_ph.c
SOURCES         += mac_decode_ma.c
SOURCES         += mac_csn_msg.c
SOURCES         += mac_csn_list.c
SOURCES         += mac_csn_common.c

endif
endif
