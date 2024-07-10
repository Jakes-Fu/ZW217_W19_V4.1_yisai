include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

MMI_RES_DIR := $(strip $(MMI_RES_DIR))

ifeq ($(strip $(WRE_STORE_OVERSEAS)), TRUE)
WRE_PRELOAD_DIR = preloadoverseas
else
WRE_PRELOAD_DIR = preload
endif  

.PHONY:all
all: 
ifeq ($(strip $(WRE_SUPPORT)), TRUE)
	@-mkdir $(MMI_DIR)\source\resource\$(MMI_RES_DIR)\preload\wre
endif
	@echo $(WRE_PRELOAD_DIR)
ifeq ($(strip $(WRE_VIRTUALDISK_SUPPORT)), TRUE)
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img  
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wre  
ifeq ($(strip $(WRE_WREKERNEL_PRELOAD)), TRUE)	

	@xcopy .\wre\sdk\$(WRE_PRELOAD_DIR)\$(strip $(MAINLCD_DEV_SIZE))\wre_img\kernel\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wre  /e /q /y /r
endif	
ifeq ($(strip $(WRE_WREKERNEL_NORFLASH)), TRUE)	

	@xcopy .\wre\sdk\$(WRE_PRELOAD_DIR)\$(strip $(MAINLCD_DEV_SIZE))\wre_img\kernel\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wre  /e /q /y /r
#	@del .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wre\wrekernel.wrd  /s /q
endif	
ifeq ($(strip $(WRE_WRESTORE_PRELOAD)), TRUE)	

	@xcopy .\wre\sdk\$(WRE_PRELOAD_DIR)\$(strip $(MAINLCD_DEV_SIZE))\wre_img\store\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wre  /e /q /y /r
endif	


	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp
ifeq ($(strip $(WRE_FACEBOOK_SUPPORT)), TRUE)  
	@$(ECHO) facebook
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\facebook
	@xcopy .\wre\sdk\preload\$(strip $(MAINLCD_DEV_SIZE))\wre_img\wreapp\facebook\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\facebook\ /y /e /q /r  	
endif
ifeq ($(strip $(WRE_TWITTER_SUPPORT)), TRUE)	
	@$(ECHO) twitter
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\WRETwit
	@xcopy .\wre\sdk\preload\$(strip $(MAINLCD_DEV_SIZE))\wre_img\wreapp\WRETwit\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\WRETwit\ /y /e /q /r  	
endif
ifeq ($(strip $(WRE_WEIBO_SUPPORT)), TRUE)		
	@$(ECHO) weibo
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\sinaweibo
	@xcopy .\wre\sdk\preload\$(strip $(MAINLCD_DEV_SIZE))\wre_img\wreapp\sinaweibo\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\sinaweibo\ /y /e /q /r  	
endif	
ifeq ($(strip $(WRE_CONTRA_SUPPORT)), TRUE)		
	@$(ECHO) contra
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\contra
	@xcopy .\wre\sdk\preload\$(strip $(MAINLCD_DEV_SIZE))\wre_img\wreapp\contra\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\contra\ /y /e /q /r  	
endif	
ifeq ($(strip $(WRE_SUPERMARIO_SUPPORT)), TRUE)		
	@$(ECHO) supermario
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\supermario
	@xcopy .\wre\sdk\preload\$(strip $(MAINLCD_DEV_SIZE))\wre_img\wreapp\supermario\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\supermario\ /y /e /q /r  	
endif	
ifeq ($(strip $(WRE_TSLAND_SUPPORT)), TRUE)		
	@$(ECHO) tsland
	@-mkdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\tsland
	@xcopy .\wre\sdk\preload\$(strip $(MAINLCD_DEV_SIZE))\wre_img\wreapp\tsland\*.* .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img\wreapp\tsland\ /y /e /q /r  	
endif	
	@$(ECHO) end	
	.\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\makevirtualdisk.bat 
	@rmdir .\wre\sdk\$(WRE_PRELOAD_DIR)\wre_virtualdisk\wre_img /s /q
endif

.PHONY:builddir
builddir:
	@echo preload

.PHONY:clean
clean:
	@echo preload
