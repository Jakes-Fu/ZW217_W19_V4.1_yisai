include Makefile.rules
.PHONEY:all
all:
	@$(DEL_FILE)  ./DSP/Source/DSP_HR3/dsp_dl_rocky_quo518.bin
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600R_quorum518_a3.c
	cd ./DSP/Source/DSP_HR3;make  clean_all;make  -f  makefile_rocky_quo518 quo518