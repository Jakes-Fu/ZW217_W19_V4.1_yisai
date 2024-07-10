
#CREATEFILE :=  $(shell tools\DEBUG_TOOL\BMP2Array\Bin\BTACmd.exe b2a -f  "$(ROOT_DIR)\build\$(PROJECT)\dep\custom_drv"  "$(ROOT_DIR)\build\$(PROJECT)\tmp\init_logo.h.tmp")
#CREATEFILE :=  $(shell $(PERL) ./make/perl_script/diff.pl  build/$(PROJECT)/tmp/init_logo.h  build/$(PROJECT)/tmp/init_logo.h.tmp build/$(PROJECT)/tmp/init_logo.h.cmp)
#CREATEFILE :=  $(shell $(PERL) ./make/perl_script/file_diff_process.pl build/$(PROJECT)/tmp/init_logo.h build/$(PROJECT)/tmp/init_logo.h.tmp  build/$(PROJECT)/tmp/init_logo.h.cmp)

ifeq ($(strip $(SOUNDBANK_MODE)),COMPACT)

    MSRCPATH =  build/$(PROJECT)_builddir/tmp
    SOURCES  =  midi_soundbank_compact.c 
    
else

    MSRCPATH =  MS_Ref/source/soundbank/
    SOURCES  =  midi_soundbank.c 

endif