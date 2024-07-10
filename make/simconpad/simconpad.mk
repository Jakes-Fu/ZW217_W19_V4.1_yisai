include Makefile.rules
.PHONEY:execute
execute:
	-rm -f ./MS_MMI/source/winsimulator_tp/win_platform/bin/simconpad.dll
	-msdev ./sim/source/test/SIMConPad/SIMConPad.dsp /MAKE "SIMControlPad - Win32 Debug" /REBUILD