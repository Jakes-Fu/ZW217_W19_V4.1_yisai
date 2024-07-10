include Makefile.rules
.PHONEY:all
all:
	
	@$(DEL_FILE)  ./DSP/Source/DSP_HR/dsp_dl_rocky_aero2.bin
	@$(DEL_FILE)  ./DSP/Source/DSP_HR/dsp_dl_rocky_quo.bin
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600R_aero2.c
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600R_quorum.c
	cd ./DSP/Source/DSP_HR;make  clean_all;make  -f  makefile_rocky_aero2 aero2
	cd ./DSP/Source/DSP_HR;make  clean_all;make  -f  makefile_rocky_quo quo