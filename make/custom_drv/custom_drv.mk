DRIVERPATH        =  MS_Customize/source/product/driver

CONFIGPATH        =  MS_Customize/source/product/config
COMMONPATH        =  MS_Customize/source/common
MOTION_SENSOR_DRV =  FALSE

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
MCFLAG_OPT		+=-DSPRD_SUPPORT_MCEX
endif

ifneq ($(findstring $(strip $(PLATFORM)),UWS6121E SC6531EFM),)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6600R)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif

ifneq ($(findstring $(strip $(PLATFORM)),UWS6121E SC6531EFM),)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif



ifeq ($(strip $(PLATFORM)),UWS6121E)
PLATFORM_TEMP = uws6121e
endif


MINCPATH =    $(DRIVERPATH)/fm/inc \
              $(DRIVERPATH)/fm\
              $(DRIVERPATH)/gpio/inc\
              $(DRIVERPATH)/motionsensor/inc\
              $(DRIVERPATH)/norflash/inc\
              $(DRIVERPATH)/spiflash/inc\
              $(DRIVERPATH)/ubot\
              $(DRIVERPATH)/dc/atv_tlg1120 \
              $(DRIVERPATH)/dc/atv_qn8610 \
              $(DRIVERPATH)/dc/atv_nmi601 \
              MS_Ref/source/audioservice/inc \
              MS_Ref/source/audioservice/inc_export \
              MS_Ref/source/aud_config/inc_export \
              MS_Ref/source/aud_dev/inc \
              MS_Ref/source/lcd/inc \
              $(DRIVERPATH)/norflash/normcp_info \
              $(MMI_DIR)/source/mmi_app/kernel/h \
              CMMB/export/inc\
              MS_Ref/export/inc \
              MS_Ref/source/image_proc/src/img_proc/inc \
              MS_Ref/source/gps/green_eye2/ge2_libgps_mp/gps_lib/inc \
              connectivity/BT/export/inc

ifeq ($(strip $(DEV_MULTIUSB_SUPPORT)), TRUE)
MINCPATH +=    $(DRIVERPATH)/multi_usb
endif

MINCPATH +=   chip_drv/chip_module/lcdc/$(strip $(PLATFORM_TEMP)) 
MINCPATH += chip_drv/chip_plf/$(strip $(PLATFORM_TEMP))


MINCPATH +=   chip_drv/export/inc/outdated	    	  
MINCPATH +=$(PS_INCPATH) $(L1_INCPATH)
MSRCPATH =    $(COMMONPATH) \
              $(DRIVERPATH)/sdram \
              $(DRIVERPATH)/nandflash \
              $(DRIVERPATH)/norflash \
              $(DRIVERPATH)/spiflash \
              $(DRIVERPATH)/dc \
              $(DRIVERPATH)/tp \
              $(DRIVERPATH)/lcd \
              $(DRIVERPATH)/seine_lcd \
              $(DRIVERPATH)/autofocus \
              $(DRIVERPATH)/gpio \
              $(DRIVERPATH)/bt \
              $(DRIVERPATH)/ocr/src \
              $(DRIVERPATH)/ubot\
              $(DRIVERPATH)/power \
              $(DRIVERPATH)/motionsensor\
              $(DRIVERPATH)/dualbattery

ifeq ($(strip $(DEV_MULTIUSB_SUPPORT)), TRUE)
MSRCPATH +=    $(DRIVERPATH)/multi_usb
endif

MSRCPATH  += $(CONFIGPATH)/$(strip $(PROD_CONFIG))
MINCPATH  += $(CONFIGPATH)/$(strip $(PROD_CONFIG))

# ******************** COMMON ******************** 
ifneq ($(strip $(MODEM_PLATFORM)), TRUE)								
SOURCES	     = 	adc_parameter.c      \
              	audio_cfg.c          \
              	current_test_prod.c  \
              	production_test.c    \
              	lcd_backlight.c      \
              	prod_param.c         \
              	gpio_prod.c          \
              	gpio_ext_drv.c       \
              	gpio_ext_cfg.c       \
              	mem_prod.c           \
              	ubot_cfg_info.c	     \
              	sensor_cfg.c         \
              	dc_product_cfg.c     \
              	lcm_prod.c           \
              	tcpip_config.c       \
              	power_prod.c
else
SOURCES	     =	adc_parameter.c      \
              	audio_cfg.c          \
              	current_test_prod.c  \
              	production_test.c    \
              	prod_param.c         \
              	gpio_prod.c          \
              	gpio_ext_drv.c       \
              	gpio_ext_cfg.c       \
              	mem_prod.c           \
              	ubot_cfg_info.c	     \
              	tcpip_config.c       \
              	power_prod.c
endif

ifeq ($(strip $(IMG_EDITER_SUPPORT)), TRUE)
SOURCES +=     img_proc_cfg.c
endif

# ANALOG TV  DRIVER 


ifeq ($(strip $(ATV_SUPPORT)), TLG1120)
MSRCPATH  += $(DRIVERPATH)/dc/atv_tlg1120 

SOURCES	  += atv_tlg1120.c \
             I2Csample.c \
             tlg1120Api.c \
             tlg1120App.c \
             tlg1120Customize.c \
             tlg1120Mfg.c
             
endif


ifeq ($(strip $(ATV_SUPPORT)), RDA5888)
	MINCPATH	+=$(DRIVERPATH)/dc/atv_rda5888
	MSRCPATH	+=$(DRIVERPATH)/dc/atv_rda5888
	SOURCES	  += atv_rda5888.c \
	             rda5888drv.c \
	             rda5888app.c \
	             rda5888freqtbl.c \
	             rda5888fm.c \
	             rda5888sprd.c
endif

ifeq ($(strip $(ATV_SUPPORT)), QN8610)
MSRCPATH  += $(DRIVERPATH)/dc/atv_qn8610

SOURCES	  += atv_qn8610.c \
             qn8610i2c.c \
             qmaio.c \
             qmafunc.c \
             qmadriver.c \
             fmdriver.c \
             qmachmap.c
             
endif

ifeq ($(strip $(ATV_SUPPORT)), NMI601)
MSRCPATH  += $(DRIVERPATH)/dc/atv_nmi601
NMI601_CHIP_DRV = TRUE
SOURCES   += atv_nmi601.c 
endif

              	
ifeq ($(strip $(FM_SUPPORT)), NMI601)
MSRCPATH  += $(DRIVERPATH)/dc/atv_nmi601
NMI601_CHIP_DRV = TRUE
SOURCES   += fm_nmi601.c 
endif

ifeq ($(strip $(NMI601_CHIP_DRV)), TRUE)
MSRCPATH  += $(DRIVERPATH)/dc/atv_nmi601
SOURCES   += nmiapi.c \
             nmidrv_atv.c \
             nmidrv_fm.c \
             nmidrv_i2c.c \
             nmiasic.c \
             nmiport.c            
endif

ifneq ($(strip $(MODEM_PLATFORM)), TRUE) 
# IMG SENSOR  DRIVER 
SOURCES	   += sensor_ov7670.c \
              sensor_ov5640.c \
              sensor_ov2655.c \
              sensor_ov7675.c  \
              sensor_ov7690.c  \
              sensor_ov2640.c \
              sensor_ov7676.c \
              sensor_gc0310.c \
              sensor_gc0309.c \
              sensor_gc0307.c \
              sensor_gt2005.c \
              sensor_ov2659.c \
              sensor_sp0838.c \
              sensor_sp0a38.c \
              sensor_bf3703.c \
              sensor_bf3903.c \
              sensor_bf30a2.c \
              sensor_gc6113.c \
              sensor_gc2015.c \
              sensor_gc032A.c \
              sensor_NT99250.c \
	      sensor_gc6133.c \
              sensor_gc2145.c

              
# LCD DRIVER

SOURCES  +=  tft_st7789v2.c   \
             tft_GC9305.c     \
			 tft_GC9306.c     \
			 tft_GC9307.c     \
			 tft_JD9851.c     \
			 tft_GC9a01.c
			 
			 
ifeq ($(strip $(ADAPT_ALL_LCM)), TRUE)                              
SOURCES  +=  tft_s6d0139.c    \
             tft_hx8312a.c    \
             tft_ILI9320.c    \
             tft_ILI9328.c    \
             tft_ILI9325.c    \
             tft_ILI9326.c    \
             tft_spfd5408.c   \
             tft_HX8346.c     \
             tft_LGDP4531.c   \
             tft_LGDP4532.c   \
             tft_SSD1228.c    \
             tft_R61505U.c    \
             tft_hx8347.c     \
             tft_HX8347D.c    \
             tft_SPFD54124A.c \
             tft_SPFD5420A.c  \
             tft_R61509V.c    \
             tft_R61581.c     \
             tft_R61529.c     \
             tft_hx8357.c     \
             tft_ILI9335.c    \
             tft_ILI9325B8.c  \
             tft_ST7781RB8.c  \
             tft_S6D04H0A.c   \
             tft_HX8347G.c    \
             tft_ILI9342.c    \
             tft_RM68040.c    \
             tft_HX8347G_spi.c \
             tft_ILI9338B.c   \
             tft_ILI9225B.c   \
             tft_hx8352C.c    \
             tft_ILI9481.c    \
             tft_ILI9486.c    \
             tft_HX8340B.c    \
             tft_ST7789H2.c   \
             tft_GC9304C.c
ifeq ($(strip $(MAINLCD_SIZE)),128X64)            
			 SOURCES += fstn_nt7538.c
endif
endif

ifeq ($(strip $(SCREEN_CAPTURE_SUPPORT)), TRUE)
SOURCES   += 	lcd_diag.c    
endif

ifeq ($(strip $(AUTOTEST_DEVICE_SUPPORT)), TRUE)
SOURCES   +=	sensor_parallel_dummy.c \
	            sensor_spi_dummy.c 
endif
endif

# NORFLASH+SRAM SPEC
SOURCES   +=		
								
ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
SOURCES   +=    HX8347_SEINE_drv.c \
                ILI9325_SEINE_drv.c \
                SPFD5420A_SEINE_drv.c \
                R61581_SEINE_drv.c \
                R61509v_SEINE_drv.c
endif

ifeq ($(strip $(DEV_MULTIUSB_SUPPORT)), TRUE)
SOURCES	  +=	sc900770.c
endif

# PROD CFG COMMON FILE
ifneq ($(strip $(MODEM_PLATFORM)), TRUE) 
SOURCES   +=	prod_cfg.c    	\
           		freq_cfg.c  	\
           		prod_outport.c  \
           		sfs_config.c    \
           		keymap_cfg.c 	\
           		gpio_cfg.c 	    \
           		pinmap_cfg.c 	\
           		os_cfg.c		\
           		lcm_cfg_info.c	\
           		display_cfg.c 	  
ifeq ($(strip $(MSDC_CARD_SUPPORT)), TRUE)
	SOURCES    +=   scm_config.c
endif
ifdef LOW_MEMORY_SUPPORT
    MCFLAG_OPT       +=    -DDISPLAY_LOW_MEMORY
endif
else
SOURCES   +=	prod_cfg.c    	\
           		prod_outport.c  \
           		keymap_cfg.c 	\
           		gpio_cfg.c 		\
           		pinmap_cfg.c 	\
           		sfs_config.c    \
           		os_cfg.c	
endif

SOURCES	  +=	power_cfg.c sleep_cfg.c

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
SOURCES   +=	mtv_cfg.c
endif			

ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)
SOURCES    +=	sdua_config.c demod_config.c
endif			

ifeq ($(strip $(SPLOAD_ENABLE)), TRUE)
SOURCES    += spload_api.c
endif			


# ******************** FEATURE ******************** 

ifeq ($(strip $(ZDT_NFC_SUPPORT)), TRUE)
MINCPATH	+=	$(DRIVERPATH)/zdt_nfc
MINCPATH	+=	$(DRIVERPATH)/zdt_nfc/app
MINCPATH	+=	$(DRIVERPATH)/zdt_nfc/iso
MINCPATH	+=	$(DRIVERPATH)/zdt_nfc/tool
MSRCPATH	+=	$(DRIVERPATH)/zdt_nfc
MSRCPATH	+=	$(DRIVERPATH)/zdt_nfc/app
MSRCPATH	+=	$(DRIVERPATH)/zdt_nfc/iso
MSRCPATH	+=	$(DRIVERPATH)/zdt_nfc/tool
SOURCES		+=	zdt_nfc.c \
                zdt_nfc_i2c.c \
                zdt_nfc_uart.c \
                apdu.c \
                mifare.c \
                nfcc.c \
                snci_lib.c \
                tag.c \
                buffer.c \
                nfc_des.c \
                nfc_list.c \
                MyTool.c \
                sm4.c \
                CardReader.c \
                CardSimulation.c \
                FirmwareBank1.c \
                FirmwareBank2.c \
                SerialCommand.c \
                FirmwareUpdate.c
endif

ifeq ($(strip $(ZDT_HSENSOR_SUPPORT)), TRUE)
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor
SOURCES		+=	zdt_hsensor.c 
ifeq ($(strip $(ZDT_HSENSOR_TYPE)), AUTO)
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/sensor
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/factory
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/MEMS_INT
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_hr
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_spo2
MINCPATH    	+=  	$(DRIVERPATH)/zdt_hsensor/vchr02
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/sensor
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/factory
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/MEMS_INT
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_hr
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_spo2
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/vchr02
SOURCES		+=	pah800x_main.c \
   		                   pah800x_bsp.c \
   		                   pah_800x.c \
   		                   pah800x_factory_test.c \
   		                   zdt_hsensor_pah8009.c \
    		                   zdt_hsensor_hrs3300.c \
				   module_heart_vc_int.c \
				   vcHr02Hci.c
endif
ifeq ($(strip $(ZDT_HSENSOR_TYPE)), HRS3300)
SOURCES		+=	zdt_hsensor_hrs3300.c
endif
ifeq ($(strip $(ZDT_HSENSOR_TYPE)), VCHR02)
MINCPATH    	+=  	$(DRIVERPATH)/zdt_hsensor/vchr02
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/vchr02
SOURCES		+=	module_heart_vc_int.c \
   		                   vcHr02Hci.c 
endif
ifeq ($(strip $(ZDT_HSENSOR_TYPE)), PAH8009)
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/sensor
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/factory
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/MEMS_INT
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_hr
MINCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_spo2
MINCPATH    	+=  	$(DRIVERPATH)/zdt_hsensor/vchr02
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/sensor
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/factory
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/MEMS_INT
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_hr
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/algorithm/pah_spo2
MSRCPATH	+=	$(DRIVERPATH)/zdt_hsensor/vchr02
SOURCES		+=	pah800x_main.c \
   		                   pah800x_bsp.c \
   		                   pah_800x.c \
   		                   pah800x_factory_test.c \
   		                   zdt_hsensor_pah8009.c
endif

ifeq ($(strip $(ZDT_HSENSOR_SUPPORT_TW)), TRUE)
SOURCES		+=	zdt_tw_t1711.c
endif
endif

ifeq ($(strip $(ZDT_SUPPORT_TW_GD609)), TRUE)
MSRCPATH	+=	$(DRIVERPATH)/temperature
SOURCES		+=	zdt_tw_gd609.c
endif

ifeq ($(strip $(ZDT_SUPPORT_TW_FM78102)), TRUE)
MSRCPATH	+=	$(DRIVERPATH)/temperature
SOURCES		+=	zdt_tw_fm78102.c
endif

ifeq ($(strip $(ZDT_SUPPORT_BEACON)), TRUE)
MSRCPATH	+=	$(DRIVERPATH)/beacon
SOURCES		+=	zdt_beacon_uart.c zdt_beacon_parse_at.c
endif


ifeq ($(strip $(ZDT_GSENSOR_SUPPORT)), TRUE)
MSRCPATH	+=	$(DRIVERPATH)/zdt_gsensor
SOURCES		+=	zdt_gsensor.c \
                zdt_gsensor_da213.c \
                zdt_gsensor_qmax981.c \
                qmax981_algo.c
endif



#ifeq ($(strip $(GPS_SUPPORT)), TRUE)
#MSRCPATH	+=	$(DRIVERPATH)/gps

#ifneq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
#MSRCPATH	+=	MS_Customize/source/product/driver/gps/sirf MS_Customize/source/product/driver/gps/sirf/prgflash/src
#SOURCES		+=	gps_com.c \
#                gps_nmea.c \
#                gps_sirf.c
#else
#MINCPATH  += BASE/l4/export/inc
#endif

#SOURCES		+=	gps_cfg.c \
#                gps_diag.c \
#                gps_drv.c
                
#include make/custom_drv/gps.mk
								
#endif

SOURCES		+= bt_prod.c
ifneq ($(strip $(BLUETOOTH_SUPPORT)), NONE)
SOURCES		+= bt_cfg.c
endif

ifeq ($(strip $(UMEM_SUPPORT)), TRUE)
MCFLAG_OPT	+= -DUMEM_SUPPORT 
SOURCES		+=umem_cfg.c
endif


ifeq ($(strip $(TOUCHPANEL_TYPE)), MULTITP)
MINCPATH	+= $(DRIVERPATH)/tp/inc \
               $(DRIVERPATH)/tp   

MSRCPATH    += $(DRIVERPATH)/tp/src 
SOURCES     += tp_multi_srv.c     \
                tp_multi_drv.c   \
                tp_multi_cfg.c       
ifeq ($(strip $(CAP_TP_SUPPORT)), FT5206)
SOURCES     += tp_ft5206.c  
endif
ifeq ($(strip $(CAP_TP_SUPPORT)), FT6336)
SOURCES     += tp_ft6336.c  
endif
endif

ifeq ($(strip $(CAP_TP_SUPPORT)), BL6133)

MINCPATH    += $(DRIVERPATH)/tp/bl6133/inc

MSRCPATH    += $(DRIVERPATH)/tp/bl6133 
                
SOURCES     += bl_ts.c \
               bl_fw_update.c
endif



#ifeq ($(strip $(OCR_SUPPORT)), TRUE)
#MCFLAG_OPT	+=	-DPRODUCT_DRIVER_OCR_DRIVER_SUPPORT 
#MINCPATH	+=	MS_Customize/source/product/driver/ocr/inc \
#                                Third-party/ocr/include
#					
#SOURCES		+=	ocr_driver.c	\
#                thocr_lib.c
#endif


MSRCPATH +=   $(DRIVERPATH)/fm \
              $(DRIVERPATH)/fm/src
SOURCES		+=	fm_drv.c
ifneq ($(strip $(FM_SUPPORT)), NONE)
SOURCES		+=	fm_cfg.c
endif
			
ifeq ($(strip $(FM_SUPPORT)), CL6012X)
SOURCES		+=	fm_cl6012x.c
endif

ifeq ($(strip $(FM_SUPPORT)), KT0812G)
SOURCES		+=	fm_kt0812g.c
endif

ifeq ($(strip $(FM_SUPPORT)), BK1080)
SOURCES		+=	fm_bk1080.c
endif

ifeq ($(strip $(FM_SUPPORT)), SPRD_V0)
SOURCES		+=	fm_sprd.c \
                  fm_dut.c  
endif

ifeq ($(strip $(FM_SUPPORT)), AUTO)
SOURCES		+=	fm_sprd.c \
                  fm_dut.c  \
                  fm_bk1080.c
endif

ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
SOURCES		+=  sdram_prod.c
else
ifeq ($(strip $(FLASH_TYPE)),   SPI)
MSRCPATH    +=  $(DRIVERPATH)/spiflash/sf_device
SOURCES     += spiflash_cfg.c \
               giga.c   \
                macronix.c       \
                winbond.c      \
                berg.c      \
                fidelix_fm25m.c
                
ifeq ($(strip $(PLATFORM)),    UWS6121E)
SOURCES     += spiflash_v2.c  \
			   spi_flash_common_v2.c
else
SOURCES     += spiflash.c   \
			   spi_flash_common.c
endif
else
SOURCES		+=  flash_cfg.c	 \
                intel_drv.c	 \
                intel_m18_drv.c \
                spansion_drv.c   \
                normcp_spec.c
endif
endif

ifeq ($(strip $(ZDT_RFID_SPI_SUPPORT)), TRUE)
MSRCPATH	+=	$(DRIVERPATH)/rfid/si24r2e
SOURCES		+=	rfid_si24r2e.c 
endif	
                
ifneq ($(strip $(MV_SUPPORT)), NONE)
MSRCPATH	+=	$(DRIVERPATH)/mv/src
MINCPATH	+=	MS_Customize/source/product/driver/mv/inc
SOURCES		+=	magicvoice_app.c
endif

ifeq ($(strip $(NORBOOT_NAND_UDISK_SUPPORT)), TRUE)
MCFLAG_OPT	+=-DPRODUCT_DRIVER_NAND_UDISK 
endif	
		
ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), FALSE)
MCFLAG_OPT	+=-DPRODUCT_DRIVER_MTV_ENABLE
endif
endif					

ifneq ($(strip $(MOTION_SENSOR_TYPE)),)				   
ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE)
MOTION_SENSOR_DRV = TRUE
MSRCPATH +=   $(DRIVERPATH)/motionsensor
	   
MSRCPATH +=   $(DRIVERPATH)/motionsensor/accelerometer

ifeq ($(strip $(MOTION_SENSOR_TYPE)), ST_LIS35DE)
SOURCES     += msensor_lis35de.c
endif

ifeq ($(strip $(MOTION_SENSOR_TYPE)), MEMS_DTOS)
SOURCES     += msensor_dtos.c
endif

ifeq ($(strip $(MOTION_SENSOR_TYPE)), TS1003)
SOURCES     += msensor_ts1003.c
endif

ifeq ($(strip $(MOTION_SENSOR_TYPE)), ADXL346)
SOURCES     += msensor_adxl346.c
endif

ifeq ($(findstring QMA7981,$(MOTION_SENSOR_TYPE)), QMA7981)
MCFLAG_OPT  += -DMOTION_SENSOR_TYPE_QST_QMA7981
SOURCES     += msensor_qmaX981.c
endif

ifeq ($(findstring DA215S,$(MOTION_SENSOR_TYPE)), DA215S)
MCFLAG_OPT  += -DMOTION_SENSOR_TYPE_MIRAMEMS_DA215S
SOURCES     += msensor_mir3da.c
endif

endif  ##MOTION_SENSOR_TYPE != NONE
endif  ##MOTION_SENSOR_TYPE != ''

ifneq ($(strip $(MOTION_SENSOR_TYPE_OPTICAL)),)
ifneq ($(strip $(MOTION_SENSOR_TYPE_OPTICAL)), NONE)
MSRCPATH +=   $(DRIVERPATH)/motionsensor
MOTION_SENSOR_DRV = TRUE
MSRCPATH +=   $(DRIVERPATH)/motionsensor/optical
			   
ifeq ($(strip $(MOTION_SENSOR_TYPE_OPTICAL)), SCM013)
SOURCES	 +=  msensor_scm013.c
endif

ifeq ($(strip $(MOTION_SENSOR_TYPE_OPTICAL)), LTR501)
SOURCES	 +=  msensor_ltr501.c
endif
endif  ##MOTION_SENSOR_TYPE_OPTICAL != NONE
endif  ##MOTION_SENSOR_TYPE_OPTICAL != ''

ifneq ($(strip $(MOTION_SENSOR_TYPE_OFN)),)
ifneq ($(strip $(MOTION_SENSOR_TYPE_OFN)), NONE)
MSRCPATH +=   $(DRIVERPATH)/motionsensor/ofn
MSRCPATH +=   $(DRIVERPATH)/motionsensor
MOTION_SENSOR_DRV = TRUE
			   
ifeq ($(strip $(MOTION_SENSOR_TYPE_OFN)), TRULY_A320)
SOURCES	 +=  msensor_truly_a320.c
endif
endif  ##MOTION_SENSOR_TYPE_OFN != NONE
endif  ##MOTION_SENSOR_TYPE_OFN != ''

ifeq ($(strip $(MOTION_SENSOR_DRV)), TRUE)
SOURCES     += msensor_drv.c \
			   msensor_cfg.c
endif
			   
ifneq ($(strip $(WIFI_SUPPORT)),NONE) 
  MINCPATH += $(DRIVERPATH)/wifi/wifisupp
  MSRCPATH += $(DRIVERPATH)/wifi/wifisupp
  SOURCES  += wifisupp_api.c wifisupp_main.c 
  ifneq ($(strip $(WIFI_SUPPORT)),UNISOC_RX)
    SOURCES  += wifi_drv.c wifisupp_eut.c
  endif
 
  ifeq ($(strip $(WIFI_SUPPORT)),AR6302)
  MSRCPATH    += $(DRIVERPATH)/wifi/ar6302
  SOURCES     += athr_wifi_adapter.c wifisupp_adap_atheros.c wifisupp_eut_ar6302.c wifi_cfg_ar6302.c 
  endif  

  ifeq ($(strip $(WIFI_SUPPORT)),UNISOC_RX)
  MSRCPATH    += $(DRIVERPATH)/wifi/uni6121e
  SOURCES     += uni6121e_wifi.c uni6121e_wifi_wcn.c wifisupp_adap_unisoc.c 
  endif 

  ifeq ($(strip $(WIFI_SUPPORT)),CSR6030)
  MCFLAG_OPT += -DIMPLEMENT_ETHER_INTERFACE -DCSP_SPI_SHARE_WITH_OTHER_DEVICE -DCSR_WIFI_6030 \
                 -DCSR_WIFI_NME_ENABLE -DCSR_THROUGHPUT_TEST_TOOL -DCSR_WIFI_PTEST_ENABLE
  #CSR_LOG:=TRUE
  ifeq ($(strip $(CSR_LOG)),TRUE)
    MCFLAG_OPT += -DCSR_LOG_ENABLE -DCSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER 
  endif
  MINCPATH += Third-party/wifi/CSR6030/include \
              Third-party/wifi/CSR6030/frw_csr/include \
              Third-party/wifi/CSR6030/frw_csr/include/platform
  MSRCPATH    += $(DRIVERPATH)/wifi/csr6030/frw_csr \
              += $(DRIVERPATH)/wifi/csr6030
  SOURCES     += csr_spi_master.c \
                 csr_debug_layer.c \
                 csr_ip_ether_sef.c \
                 csr_sfs_filesystem.c \
                 csr_time_utc.c \
                 csr_time.c \
                 csr_framework_ext.c \
                 bt_wifi_transport.c \
                 csr_spt_log_porting.c \
                 wifisupp_adap_csr.c \
                 wifisupp_adap_csr_sef.c \
                 wifisupp_eut_csr6030.c \
		 wifi_cfg_csr6030.c \
                 wifisupp_adap_csr_config.c
  endif
  
  ifeq ($(findstring CSR6027,$(WIFI_SUPPORT)), CSR6027)
  MINCPATH += Third-party/wifi/CSR6027/include \
              Third-party/wifi/CSR6027/frw_csr/include \
              Third-party/wifi/CSR6027/frw_csr/include/platform
  MSRCPATH += $(DRIVERPATH)/wifi/csr6027/frw_csr \
              $(DRIVERPATH)/wifi/csr6027
  SOURCES  += csr_debug_layer.c \
              csr_sfs_filesystem.c \
              csr_time_utc.c \
              csr_time.c \
              csr_framework_ext.c \
              csr_spi_master.c \
              csr_ip_ether_sef.c \
              bt_wifi_transport.c \
              wifi_cfg_csr6027.c \
              wifisupp_adap_csr.c \
              wifisupp_adap_csr_sef.c \
              wifisupp_eut_csr6027.c \
              wifisupp_adap_csr_config.c
  MCFLAG_OPT += -DCSR_WIFI_NME_ENABLE -DIMPLEMENT_ETHER_INTERFACE \
                -DCSR_THROUGHPUT_TEST_TOOL -DCSP_SPI_SHARE_WITH_OTHER_DEVICE 
  #CSR_LOG:=TRUE
  ifeq ($(strip $(CSR_LOG)),TRUE)
    MCFLAG_OPT  += -DCSR_LOG_ENABLE -DCSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER
  endif
  endif  
    
  ifeq ($(strip $(WIFI_VENDOR)), REALTEK)
  MSRCPATH += $(DRIVERPATH)/wifi/rtl8723
  SOURCES	 += rtl8723_wifi_adapter.c wifisupp_adap_realtek.c wifisupp_eut_realtek.c wifi_cfg_realtek.c
  endif  
    
endif #WIFI_SUPPORT==NONE

ifneq ($(strip $(SUBLCD_DEV_SIZE)), NONE)
MCFLAG_OPT	+=-DPRODUCT_DRIVER_SUB_LCD_ENABLE
endif	

ifeq ($(strip $(MULTI_SIM_SYS)), QUAD)
SOURCES     += sim_cfg.c
endif

ifeq ($(strip $(MULTI_SIM_SYS)), TRI)
ifneq ($(findstring $(strip $(PLATFORM)),UWS6121E SC6531EFM),)
SOURCES     += sim_cfg.c
endif
endif

ifeq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
SOURCES +=  dualbat_prod_cfg.c\
            dualbat_prod_aw3312.c              
endif

ifeq ($(strip $(TF_LOAD_SUPPORT)), TRUE)
MINCPATH	+=	fdl_bootloader/tf_fdl/inc
MSRCPATH +=  $(DRIVERPATH)/tf_load/src
SOURCES	 +=  tf_load.c \
			 tf_cfg.c
endif

ifneq ($(strip $(MODEM_PLATFORM)), TRUE)
SOURCES     += sensor_prod.c
endif

 SOURCES     += nv_hwversion_adpter.c \
 		nv_adaptive_cfg.c
# ******************** PLATFORM ******************** 

# ******************** PROJECT ******************** 
