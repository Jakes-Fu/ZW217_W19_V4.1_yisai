include Makefile.rules

TARGET = release_level_d0.a
TARGET_PATH = lib/$(strip $(PRODUCT_BASELIB_DIR))

OBJECTS = tx_tcr.o tx_tcs.o tx_tic.o tx_timin.o tx_ts.o tx_tsb.o tx_tsr.o \
          tx_td.o tx_tda.o tx_tdel.o tx_ti.o tx_tide.o tx_tig.o tx_timch.o tx_timcr.o tx_timd.o tx_timeg.o \
          tx_times.o tx_timi.o tx_timig.o txe_ba.o tx_tpch.o tx_tprch.o txe_bpc.o tx_tr.o tx_tra.o tx_trel.o \
          txe_bpd.o tx_tsa.o txe_bpig.o tx_tse.o tx_tsle.o txe_bpp.o tx_tsus.o tx_tt.o tx_tte.o txe_br.o \
          tx_tto.o tx_tts.o tx_ttsc.o txe_byta.o tx_twa.o txe_bytc.o txe_bytd.o txe_bytg.o txe_bytp.o \
          txe_bytr.o txe_efc.o txe_efd.o txe_efg.o txe_efig.o txe_efs.o txe_mc.o txe_md.o txe_mg.o txe_mig.o \
          txe_mp.o txe_mpri.o txe_qc.o txe_qd.o txe_qf.o txe_qfs.o txe_qig.o txe_qp.o txe_qr.o txe_qs.o \
          txe_sc.o txe_sd.o txe_sg.o txe_sig.o txe_sp.o txe_spri.o txe_taa.o txe_tc.o txe_tda.o txe_tdel.o \
          txe_tig.o txe_timd.o txe_timi.o txe_tmch.o txe_tmcr.o txe_tpch.o txe_tra.o txe_trel.o txe_trpc.o \
          txe_tsa.o txe_tt.o txe_ttsc.o txe_twa.o tx_ba.o tx_bpc.o tx_bpcle.o tx_bpd.o tx_bpi.o tx_bpig.o \
          tx_bpp.o tx_br.o tx_byta.o tx_bytc.o tx_bytcl.o tx_bytd.o tx_byti.o tx_bytig.o tx_bytpp.o tx_bytr.o \
          tx_byts.o tx_efc.o tx_efcle.o tx_efd.o tx_efg.o tx_efi.o tx_efig.o tx_efs.o tx_global_define.o \
          tx_ihl.o tx_ike.o tx_mc.o tx_mcle.o tx_md.o tx_mg.o tx_mi.o tx_mig.o tx_mp.o tx_mpc.o tx_mpri.o \
          tx_qc.o tx_qcle.o tx_qd.o tx_qf.o tx_qfs.o tx_qi.o tx_qig.o tx_qp.o tx_qr.o tx_qs.o tx_sc.o \
          tx_scle.o tx_sd.o tx_sg.o tx_si.o tx_sig.o tx_sp.o tx_spri.o tx_ta.o tx_taa.o tx_tc.o tx_global_define.o

OBJECTS += ../../../../build/$(PROJECT)_builddir/obj/dm_2g_ps/rrdm.o
OBJECTS += ../../../../build/$(PROJECT)_builddir/obj/tdd_nas/nas_swth_main.o 
OBJECTS += ../../../../build/$(PROJECT)_builddir/obj/tdd_nas/nas_swth_signal_conversion.o 
OBJECTS += ../../../../build/$(PROJECT)_builddir/obj/tdd_nas/nas_swth_struct_common.o
OBJECTS += ../../../../build/$(PROJECT)_builddir/obj/tdd_bcfe/rrc_bcfe_proc_cell_srch.o

$(strip $(TARGET)):
	@$(PERL) make/perl_script/mk_dir.pl $(strip $(TARGET_PATH)) ./
	@$(PERL) make/perl_script/rm_file.pl $(strip $(TARGET)) $(strip $(TARGET_PATH))/
	cd build/$(PROJECT)_builddir/obj/rtos_kal &&  $(AR) $(ARFLAGS) ../../../../$(TARGET_PATH)/$(TARGET)  $(OBJECTS)

.PHONY:clean
clean:
	@$(PERL) make/perl_script/rm_file.pl $(strip $(TARGET)) $(strip $(TARGET_PATH))/