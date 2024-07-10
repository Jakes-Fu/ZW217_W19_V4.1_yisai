

MINCPATH =  MS_Ref/source/codec/cat_mvoice/inc MS_Ref/export/inc MS_Ref/source/codec/record/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc 
 

ifeq ($(strip $(MULTI_SIM_SYS)), TRI)
MINCPATH  += trisim/Layer1/export/inc  trisim/Layer1/source/h 
else
MINCPATH  += Layer1/export/inc  Layer1/source/h 
endif

MSRCPATH =  MS_Ref/source/codec/cat_mvoice/src

SOURCES	 = fft.c SCF_api.c SCF_role.c SCF_win.c fft_arm.s SCF_log2.s SCF_polar.s cat_mvoice.c   


			
