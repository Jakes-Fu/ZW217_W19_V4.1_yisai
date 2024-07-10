

MINCPATH = MS_Ref/source/codec/midi_opt/inc  MS_Ref/export/inc \
  MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH +=$(L1_INCPATH) 
 
MSRCPATH = MS_Ref/source/codec/midi_opt/src

SOURCES  = midi_opt_adp.c midi_opt_decode.c midi_opt_api.c midi_opt_soundbank.c\
           midi_loop.s midi_modenv.s midi_modlfo.s midi_vibrato.s midi_volenv.s






