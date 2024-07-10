include Makefile.rules
.PHONEY:all
all:
	
	@rm -f  ./DSP/Source/DSP_6600L1/dsp_dl_6600l_quo518.bin
	@rm -f  ./DSP/Source/mcu/DSP_pdata_sc6600L_quorum518.c

	cd ./DSP/Source/DSP_6600L1;make  clean_all;make -f makefile_6600L_quo518 quo518