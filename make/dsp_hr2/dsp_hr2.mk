include Makefile.rules
.PHONEY:all
all:
	@$(DEL_FILE)  ./DSP/Source/DSP_HR2/dsp_dl_rocky_quo.bin
	@$(DEL_FILE)  ./DSP/Source/DSP_HR2/dsp_dl_rocky_aero2.bin
	@$(DEL_FILE)  ./DSP/Source/DSP_HR2/dsp_dl_rocky_quo518.bin
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600R_quorum_a2.c
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600R_aero2_a2.c
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600R_quorum518_a2.c
	cd ./DSP/Source/DSP_HR2;make  clean_all;make  -f  makefile_rocky_aero2 aero2
	cd ./DSP/Source/DSP_HR2;make  clean_all;make  -f  makefile_rocky_quo quo
	cd ./DSP/Source/DSP_HR2;make  clean_all;make  -f  makefile_rocky_quo518 quo518