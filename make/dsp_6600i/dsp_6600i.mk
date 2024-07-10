include Makefile.rules
.PHONEY:all
all:
	
	@$(DEL_FILE)  ./DSP/Source/DSP_6600I/dsp_dl_6600i_dualsim.bin
	@$(DEL_FILE)  ./DSP/Source/DSP_6600I/dsp_dl_6600i_dualsim_quo.bin

	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600I_dualsim.c
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6600I_dualsim_quo.c

	cd ./DSP/Source/DSP_6600I;make  clean_all;make  -f  makefile_IDualsim_quo   quo
	cd ./DSP/Source/DSP_6600I;make  clean_all;make  -f  makefile_Idualsim aero2