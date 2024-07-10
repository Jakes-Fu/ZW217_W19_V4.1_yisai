MCFLAG_OPT   	= -Otime

MINCPATH =  BASE/Layer1/source/h MS_Ref/export/inc MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH) 	
MINCPATH +=BASE/Layer1/export/inc 
MINCPATH +=MS_Ref/source/codec/aud_dsp_mng/inc
MINCPATH +=BASE/Layer1/source/h BASE/ps/export/inc
MINCPATH +=chip_drv/export/inc/outdated



MSRCPATH =MS_Ref/source/codec/aud_dsp_mng/src


						

SOURCES		=  aud_dsp_interface.c                                                   \
				    mp3_dsp_layer1_cmd_interface.c					\
				    aud_dsp_mng.c                           \
				    amr_dsp_layer1_cmd_interface.c


						    
