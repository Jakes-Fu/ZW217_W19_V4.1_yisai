
MCFLAG_OPT 		= -D__APCS_SWST -D__APCS_INTERWORK -D__TARGET_FPU_NONE -D__BIG_ENDIAN -DTX_TIMER_INT_BY_TICK_DECREASE --thumb

MASMFLAG_OPT	= -PD "TX_TIMER_INT_PER_TICK SETL {FALSE}"  -PD "TX_ASSERT_DEBUG SETL {TRUE}"  -PD "RTOS_ROM SETL {FALSE}" \
                 -PD "TX_CHECK_STACK SETL {TRUE}" -PD "TASK_ANALYZER SETL {FALSE}" -PD "DEEP_SLEEP_NEED_EMPTY_FIFO SETL {FALSE}"

ifeq ($(strip $(STACK_CHECK_SUPPORT)),	FALSE)
MASMFLAG_OPT	= -PD "TX_TIMER_INT_PER_TICK SETL {FALSE}"  -PD "TX_ASSERT_DEBUG SETL {TRUE}"  -PD "RTOS_ROM SETL {FALSE}" \
                -PD "TX_CHECK_STACK SETL {TRUE}" -PD "TASK_ANALYZER SETL {FALSE}" -PD "DEEP_SLEEP_NEED_EMPTY_FIFO SETL {FALSE}"

endif

ifeq ($(strip $(SRAM_SIZE)),	16MBIT)
MCFLAG_OPT   += -DRTOS_LOW_MEMORY
endif


ifeq ($(strip $(SRAM_SIZE)),	32MBIT)
MCFLAG_OPT   += -DRTOS_LOW_MEMORY
endif


MINCPATH =  RTOS/source/threadx/thumb  RTOS/source/include

MSRCPATH =  RTOS/source/src/c RTOS/source/threadx/thumb

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
MINCPATH += RTOS/source/include/anti
MINCPATH += MS_Ref/source/efs/inc/anti
MINCPATH += chip_drv/chip_module/efuse/uws6121e/anti
MSRCPATH += RTOS/source/src/c/anti
endif

SOURCES     	= tx_tcr.s tx_tcs.s tx_tic.s tx_timin.s tx_ts.s tx_tsb.s tx_tsr.s \
					tx_td.c tx_tda.c tx_tdel.c tx_ti.c tx_tide.c tx_tig.c tx_timch.c tx_timcr.c tx_timd.c tx_timeg.c \
					tx_times.c tx_timi.c tx_timig.c txe_ba.c tx_tpch.c tx_tprch.c txe_bpc.c tx_tr.c tx_tra.c tx_trel.c \
					txe_bpd.c tx_tsa.c txe_bpig.c tx_tse.c tx_tsle.c txe_bpp.c tx_tsus.c tx_tt.c tx_tte.c txe_br.c \
					tx_tto.c tx_tts.c tx_ttsc.c txe_byta.c tx_twa.c txe_bytc.c txe_bytd.c txe_bytg.c txe_bytp.c \
					txe_bytr.c txe_efc.c txe_efd.c txe_efg.c txe_efig.c txe_efs.c txe_mc.c txe_md.c txe_mg.c txe_mig.c \
					txe_mp.c txe_mpri.c txe_qc.c txe_qd.c txe_qf.c txe_qfs.c txe_qig.c txe_qp.c txe_qr.c txe_qs.c \
					txe_sc.c txe_sd.c txe_sg.c txe_sig.c txe_sp.c txe_spri.c txe_taa.c txe_tc.c txe_tda.c txe_tdel.c \
					txe_tig.c txe_timd.c txe_timi.c txe_tmch.c txe_tmcr.c txe_tpch.c txe_tra.c txe_trel.c txe_trpc.c \
					txe_tsa.c txe_tt.c txe_ttsc.c txe_twa.c tx_ba.c tx_bpc.c tx_bpcle.c tx_bpd.c tx_bpi.c tx_bpig.c \
					tx_bpp.c tx_br.c tx_byta.c tx_bytc.c tx_bytcl.c tx_bytd.c tx_byti.c tx_bytig.c tx_bytpp.c tx_bytr.c \
					tx_byts.c tx_efc.c tx_efcle.c tx_efd.c tx_efg.c tx_efi.c tx_efig.c tx_efs.c tx_global_define.c \
					tx_ihl.c tx_ike.c tx_mc.c tx_mcle.c tx_md.c tx_mg.c tx_mi.c tx_mig.c tx_mp.c tx_mpc.c tx_mpri.c \
					tx_qc.c tx_qcle.c tx_qd.c tx_qf.c tx_qfs.c tx_qi.c tx_qig.c tx_qp.c tx_qr.c tx_qs.c tx_sc.c \
					tx_scle.c tx_sd.c tx_sg.c tx_si.c tx_sig.c tx_sp.c tx_spri.c tx_ta.c tx_taa.c tx_tc.c tx_global_define.c \
					tx_threadx_stack_error_handler.c tx_threadx_stack_error_notify.c \
					threadx_mem.c threadx_os.c threadx_rst.c os_api.c \
					sci_log.c server.c os_param.c sci_compile_date.c \
					sci_api.c rtos_version.c  threadx_appmem.c

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
SOURCES  += base41.c  \
            cyphertext.c 
endif