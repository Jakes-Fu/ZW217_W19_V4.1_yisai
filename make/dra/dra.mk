MCFLAG_OPT   	= -Otime 

MINCPATH =  MS_Ref/source/codec/dra/inc MS_Ref/source/codec/dra/inc_export MS_Ref/export/inc \
                    MS_Ref/source/aud_exp/eq/inc MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc \
                    MS_Ref/source/codec/dra/src/DRA_Inc

MINCPATH += chip_drv/export/inc/outdated
MINCPATH += $(L1_INCPATH)
 	
MSRCPATH =  MS_Ref/source/codec/dra/src  MS_Ref/source/codec/dra/src/DRA_Src

SOURCES  =  dra_adp.c  DRA_BitHandle.c  DRA_Decode.c  DRA_FFT_New.c \
                    DRA_FFTTAB_New.c  DRA_IMDCT.c  DRA_Paser.c  DRA_Process.c  \
                    DRA_ReQuantize.c  DRA_Stream.c  DRA_StreamTable.c DRA_Table.c \
                    DRA_UtilityASM.s  DRA_WindowASM.s  imdct128_s1.s  imdct128_s2.s \
                    imdct128_s3.s  imdct1024_s1.s  imdct1024_s2.s  imdct1024_s3.s \
                    imdct1024_s4.s  imdct1024_s5.s imdct_post.s  imdct_pre.s  imdct_reorder.s
