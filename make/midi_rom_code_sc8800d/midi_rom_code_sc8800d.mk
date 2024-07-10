
MCFLAG_OPT   	= -Otime -DMP3_SCATTER -DMP3_STAND_ALONE $(CPU) ARM926EJ-S

MINCPATH =  MS_Ref/source/codec/midi/midi_rom_code/src


MSRCPATH =  MS_Ref/source/codec/midi/midi_rom_code/src

SOURCES			= midi_rom_data.c midi_sc_api_rom.c midi_decode.c midi_constant.c midi_volenv.s midi_modenv.s midi_modlfo.s\
							midi_play.c midi_vibrato.s midi_calculation_stereo.c
							
