include Makefile.rules
.PHONEY:all
all:
	
	@$(DEL_FILE)  ./DSP/Source/DSP_6600M3-68000A1/main_prom_6800a1_aero2_0.bin
	@$(DEL_FILE)  ./DSP/Source/DSP_6600M3-68000A1/main_prom_6800a1_aero2_1.bin
	@$(DEL_FILE)  ./DSP/Source/DSP_6600M3-68000A1/dsp_dl_6800a1_aero2.bin
	@$(DEL_FILE)  ./DSP/Source/mcu/DSP_pdata_sc6800a1_aero2.c
	cd ./DSP/Source/DSP_6600M3-68000A1;make  clean_all;make  -f  makefile_6800A1_aero2  aero2