# *************************************************************************************************************************************
#  大部分公共的配置选项在 uws6121e_watch_base_config.cfg 中，
#  All the common configuration is set in uws6121e_watch_base_config.cfg 
# *************************************************************************************************************************************
MMI_MULTI_SIM_SYS = SINGLE                             ### Platform Support Multi Sim Card Number
                                                     # Option1:SINGLE    #define MMI_MULTI_SIM_SYS_SINGLE
                                                     # Option2:DUAL      #define MMI_MULTI_SIM_SYS_DUAL
include uws6121e_watch_base_config.cfg
include 217sw_patch.mk
# *************************************************************************************************************************************
# 如果有差异的话，需要从uws6121e_watch_base_config.cfg提取出来，放到各自project_xxx.mk的这段注释的后面，
# 注意：
#    (1)目前不支持有重复宏定义，比如SRAM_SIZE = 128MBIT，然后后面有出现一处SRAM_SIZE = 256MBIT，两种不同的定义。
#    (2)这段注释的后面的宏定义不能和uws6121e_watch_base_config.cfg 中的重复，如果有重复需要从uws6121e_watch_base_config.cfg 中拿出来。
#    (3)RVCT_VERSION = RVCT41 或者 RVCT_VERSION = V504 不再出现在project_xxx.mk中，需要编译的时候手动在CMD中设置set CC_VER=RVCT41
#       或者set CC_VER=V504，如果不设置的话，在makefile.rules中会默认设置成RVCT_VERSION = RVCT41。
# Notice:
#    (1). Do not support multi-definition of one micro, such as define SRAM_SIZE = 128MBIT at first, then define SRAM_SIZE = 256MBIT in the end.
#    (2). The micro delow this comments should not re-defined with the micros in uws6121e_watch_base_config.cfg. If there happened re-defined, you
#         should seprate the micro from  uws6121e_watch_base_config.cfg and add it to individual project_xx.mk
#    (3). The difinition of RVCT_VERSION is not set anymore. You should set environment value in CMD terminator, such as set CC_VER=V504.
#         If you do not set CC_VER , the default value RVCT_VERSION = RVCT41 will be set in makefile.rules.
# *************************************************************************************************************************************
# override for 128*128 LCD size
MAINLCD_DEV_SIZE = 240X240                           ### Resolution Of Main LCD Device,  MAINLCD_SIZE is the size of MMI
                                                     # Option1:128X160    #define MAINLCD_DEV_SIZE_128X160
                                                     # Option2:160X128    #define MAINLCD_DEV_SIZE_128X160
                                                     # Option3:176X220    #define MAINLCD_DEV_SIZE_176X220
                                                     # Option4:220X176    #define MAINLCD_DEV_SIZE_220X176
                                                     # Option5:240X320    #define MAINLCD_DEV_SIZE_240X320
                                                     # Option6:320X240    #define MAINLCD_DEV_SIZE_320X240

MAINLCD_SIZE = 240X320                               ### Resolution Of Main LCD:
                                                     # Option1:240X320    #define MAINLCD_SIZE_240X320
                                                     # Option2:176X220    #define MAINLCD_SIZE_176X220
                                                     # Option3:128x160    #define MAINLCD_SIZE_128X160

MMI_RES_DIR = mmi_res_240x240_child_circular         ### resource directory
                                                     # Option1:mmi_res_128x160
                                                     # Option2:mmi_res_176x220
                                                     # Option3:mmi_res_240x320
                                                     # Option4:mmi_res_240x320_mini
                                                     # Option5:mmi_res_240x240_child_circular
MMI_DIR  = MS_MMI_Main															 

FLASH_SIZE = 128MBIT                         		 ### FLASH_SIZE SIZE
                                                     # Option1:16MBIT     #define FLASH_SIZE_16MBIT
                                                     # Option2:32MBIT     #define FLASH_SIZE_32MBIT
                                                     # Option3:64MBIT     #define FLASH_SIZE_64MBIT
                                                     # Option4:128MBIT    #define FLASH_SIZE_128MBIT
						                             # Option5:128MBITX64MBIT    #define FLASH_SIZE_128MBITX64MBIT_OLD
													 # Option6:128MBITX64MBIT_NEW #define FLASH_SIZE_128MBITX64MBIT_NEW

MULTI_FLASH_SUPPORT = FALSE		

#FLASH_SIZE_128MBITX32MBIT = TRUE				 	 # Option1: TRUE FLASH(INTERNAL 16MB + EXTERNAL 4MB)	
													 # Option2: FALSE FLASH(INTERNAL 16MB ONLY)	
											 										 
                                                     
SRAM_SIZE = 128MBIT                                   ### SRAM SIZE
                                                     # Option1:16MBIT     #define SRAM_SIZE_16MBIT
                                                     # Option2:32MBIT     #define SRAM_SIZE_32MBIT
                                                     # Option3:64MBIT     #define SRAM_SIZE_64MBIT
                                                     # Option4:128MBIT     #define SRAM_SIZE_128MBIT
                                                     # Option4:256MBIT     #define SRAM_SIZE_256MBIT
													 
AP_CP_RAM_SIZE = 40X88                               ##APXCP RAM SIZE 
                                                     # Option1:40MBITX88MBIT     #define AP_CP_RAM_SIZE_40X88
													 # Option1:24MBITX104MBIT     #define AP_CP_RAM_SIZE_24X104
                                                   

_USE_3_LINE_2_DATD_ = FALSE                          ### lcd 3 line 2 date support
                                                     # Option2:FALSE
													 
													 
PRODUCT_CONFIG = uws6121e                            ### MS_Customize/source/product/config;scf file;nv;resource
                                                     # default: project name 

PRODUCT_BASELIB_DIR = $(PROJECT)                     ### Dir name for base lib in MS_Code\lib\
                                                     # default: project name 
                                                     

													 
BT_LOG_MENU_SUPPORT = FALSE                          ### BT LOG Menu Select Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE 
                                                     
                                                     
PLATFORM = UWS6121E                                    ### Chip Platform Definition:
                                                     # Option1:SC6530     #define PLATFORM_SC6530
                                                     # Option2:SC6531EFM  #define PLATFORM_SC6531EFM
                                                     # Option2:UIX8910    #define PLATFORM_UIX8910
													 # Option3:UWS6121E   #define PLATFORM_UWS6121E
                                                     
CHIP_VER = UWS6121E                                     ### Chip version:
                                                     # Option1:6800H1     #define CHIP_VER_6800H1
                                                     # Option2:6800H3     #define CHIP_VER_6800H3
	                                                 # Option3:6531EFM    #define CHIP_VER_6531EFM												 
                                                     # Option4:UIX8910MPW    #define CHIP_VER_UIX8910MPW
													 # Option5:UIX8910    #define CHIP_VER_UIX8910
													 # Option5:UIS8910C    #define CHIP_VER_UIS8910C
                                                     # Option6:UIS8910FF    #define CHIP_VER_UIS8910FF
                                                     # Option7:UWS6121E    #define CHIP_VER_UWS6121E

#modem bin folder is defined by two configures: MODEM_BIN_FILE/MODEM_PROJECT_FOLDER													 
MODEM_BIN_FILE = UWS6121E_MODEM                          ### modem bin cache FOLDER
                                                     # Option1:UWS6121E_MODEM
													 
MODEM_PROJECT_FOLDER = UWS6121_cat1bis_volte_gnss      ### modem PROJECT cache FOLDER
                                                     # Option1:UWS6121_cat1bis_volte_gnss
													 # Option2:UWS6121_cat1bis_volte_nognss
													 
													 
FM_S_ANT_SUPPORT = FALSE                              ###
                                                     # Option0:TRUE     
                                                     # Option1:FALSE                                                    

						     													 
BZP_NO_RESOURCE = TRUE								## resource not zip when bzp_support
													# Option1:TRUE
													# Option1:FALSE                                                

BT_PBAP_SUPPORT = FALSE                              ### Switch Of PBAP Profile Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE
													 
AUTOTEST_DEVICE_SUPPORT = TRUE                      ### AUTOTEST_DEVICE_SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE
													 
UMEM_SUPPORT = TRUE                                 ### Switch Of nor flash udisk support
                                                     # Option1:TRUE
                                                     # Option2:FALSE	

UDISK_PC_CONNECT = TRUE                             ### Norflash disk pc access support UMEM_SUPPORT MUST set TRUE
                                                     # Option1:TRUE
                                                     # Option2:FALSE     
													 
SIM_LOCK_SUPPORT = FALSE                             ### SIM Lock Support 
                                                     # Option1:TRUE
                                                     # Option2:FALSE													 
													
SIMULATE_FLASH2_SUPPORT = FALSE

CHIP_ECO_SUPPORT = TRUE

POP_MDY_SUPPORT = TRUE

SECURE_BOOT_SUPPORT = TRUE			     ### Secure Boot Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SECURITY_DATA_SUPPORT = FALSE			     ### Security Data Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE

VIRTUAL_VIBRATE_FUNC = FALSE                         ### VIRTUAL VIBRATE FUNCTION
                                                     # Option0:TRUE     
                                                     # Option1:FALSE 


RF_BAND_SELFADAPTIVE = FALSE                    		### OPEN/CLOSE RF BAND SELFADAPTIVE FUNCTION
													# Option0:TRUE     
													# Option1:FALSE 
CHGMNG_PSE_SUPPORT = FALSE                    		### OPEN/CLOSE CHGMNG_PSE_SUPPORT
													# Option0:TRUE     

DEBUG_VERSION  = TRUE                                ### DEBUG_VERSION:
                                                     # Option1:TRUE
                                                     # Option2:FALSE


RELEASE_FLAG = FALSE                                  ### RELEASE_FLAG
                                                     # Option1:TRUE
                                                     # Option2:FALSE 


                                                     
BZP_SUPPORT  = FALSE                                  ###new memory management 
                                                     # Option1:TRUE
                                                     # Option2:FALSE 	
											 
													 
BLUETOOTH_SUPPORT = NONE                             ### Bluetooth Chip Type: 
                                                     # Option1:NONE      #define BLUETOOTH_SUPPORT_NONE
                                                     # Option2:CSR_BC4_6 #define BLUETOOTH_SUPPORT_CRS_BC4_6
                                                     # Option3:CSR_BC4_6_LOW_MEM: low mem and code size , only support OPP HFG
                                                     # Option4:RTL8723   Realtek RTL8723AS wifi/bt combo chip
                                                     # Option5:IS1662    ISSC IS1662S bt chip
                                                     # Option6:SPRD_BT			#define BLUETOOTH_SUPPORT_SPRD_BT
 

UI_P3D_SUPPORT = TRUE

ROTATE_2D_SUPPORT = TRUE

MMI_3DUI_SUPPORT = TRUE

ANALOG_CLK_SUPPORT = TRUE

IKEYBACKUP_SUPPORT = FALSE

UI_MULTILAYER_SUPPORT = TRUE

BROWSER_SUPPORT = FALSE

BROWSER_SUPPORT_DORADO = FALSE

BROWSER_OPERA_START_PAGE = FALSE

OPERA_MINI_SUPPORT = NONE

DL_SUPPORT = FALSE

MMS_SUPPORT = NONE

VOICE_PROCESS_NEW = TRUE

CVS_ENABLE = TRUE

CVSRCD_ENABLE = TRUE

AMR_ENABLE = TRUE

VOIP_SUPPORT = TRUE

AUD_ENCODE_FADE_INOUT = TRUE

DATACOUNTER_SUPPORT = FALSE

SALES_SUPPORT = FALSE

DC_WATCH_UI_SUPPORT= TRUE

HW_6121_pre_w217 = FALSE

AUDIO_CODEC_TYPE = ES8311                            ### audio codec selection
                                                     # Option1:ES8311     #define ES8311
                                                     # Option2:ES8374     #define ES8374

AUDIO_EXTPA_TYPE = AW87390                              ### audio codec extpa selection
                                                     # Option1:AW87390      #define AW87390
                                                     # Option2:GPIO         #define GPIO
												 
TOUCHPANEL_TYPE	= MULTITP                               ### Touch panel type
                                                     # Option1:HW            #define TOUCHPANEL_TYPE_HW
                                                     # Option2:NONE          #define TOUCHPANEL_TYPE_NONE
                                                     # Option3:MULTITP       #define TOUCHPANEL_TYPE_MULTITP

TPC_TYPE = NONE                                      ### tpc sampling type
                                                     # Option1:SINGLE  #define TPC_TYPE_SINGLE
                                                     # Option2:DOUBLE  #define TPC_TYPE_DOUBLE
                                                     # Option3:NONE    #define TPC_TYPE_NONE

TP_EXCHANGE = NONE                                   ### tp channel exchange type
                                                     # Option1:XY      #define TP_EXCHANGE_XY
                                                     # Option2:NONE    #define TP_EXCHANGE_NONE

CAP_TP_SUPPORT = BL6133                                ### Capacitive Touch panel chip
                                                     # Option0:NONE          #define CAP_TP_SUPPORT_NONE
                                                     # Option1:FT5206        #define CAP_TP_SUPPORT_FT5206
                                                     # Option1:CY8C21434     #define CAP_TP_SUPPORT_CY8C21434

TOUCH_PANEL_CTRL_SUPPORT  = TRUE                      ###touch panel only affect ctrl
                                                     # Option1:TRUE
                                                     # Option2:FALSE

TOUCH_PANEL_HWICON_SUPPORT = NONE                    ### touch panel hw support
                                                     # Option1:DIAL_PB_SMS_MAINMENU         #define TOUCH_PANEL_HWICON_SUPPORT_DIAL_PB_SMS_MAINMENU
                                                     # Option2:MAINMENU_PB_SMS_CAMERA_MP4   #define TOUCH_PANEL_HWICON_SUPPORT_MAINMENU_PB_SMS_CAMERA_MP4
                                                     # Option3:MAINMENU_SMS_PB_DIAL_MP3     #define TOUCH_PANEL_HWICON_SUPPORT_MAINMENU_SMS_PB_DIAL_MP3
                                                     # Option4:VIRTUAL_2KEY                 #define TOUCH_PANEL_HWICON_SUPPORT_VIRTUAL_2KEY
                                                     # Option5:NONE

TP_VK_SUPPORT = FALSE                                ### touch panel virtual key driver support
                                                     # Option1:TRUE
                                                     # Option2:FALSE
                                                     # Option2:GPIO         #define GPIO=======
                                                     # Option2:GPIO         #define GPIO

PMIC_CHARGER_DETECT_SUPPORT = FALSE                  ### support charger type detect by pmic
                                                     # Option1:FALSE      #6121E is not support right now
                                                     # Option2:TRUE       #for extend in the future

AT_CMD_OP_SUPPORT = TRUE                             ### support ATC cmd to test daps function
                                                     # Option1:FALSE      not support
                                                     # Option2:TRUE       support

HTTP2_SUPPORT = TRUE                                 ### HTTP2 Profile Support:
                                                     # Option1:FALSE

GPS_SUPPORT = TRUE                                   ### Switch Of GPS Module Support
                                                     # Option1:FALSE      not support
                                                     # Option2:TRUE       support

GPS_CHIP_VER = GREENEYE2                             ### Chip version:
                                                     # Option1:GREENEYE2     #define GPS_CHIP_VER_GREENEYE2
                                                     # Option2:NONE

GPS_TRANS_VIA = SIPC                                 ### GPS COMM PORT
                                                     # Option1:NONE
                                                     # Option1:UART
                                                     # Option1:SIPC

GNSS_MODE = GPS_BD_GAL                               ### Switch Of GPS Mode
                                                     # Option1:GPS_BD
                                                     # Option2:GPS_BD_GLO_GAL
                                                     # Option3:GPS_BD_GAL

#GPS_TCARD_LOG_SUPPORT = FALSE                       ### Switch Of GPS TCARD LOG SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE

GPS_MINI_LOG_SUPPORT = TRUE                          ### Switch Of GPS TCARD LOG SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE

GPS_AGPS_SUPPORT = QX                                ### Switch GPS AGPS SUPPORT:
                                                     # Option1:NONE      #define GPS_AGPS_SUPPORT_NONE
                                                     # Option2:QX #define GPS_AGPS_SUPPORT_QX
                                                     # Option3:SPRD_AGPS #define GPS_AGPS_SUPPORT_SPRD_AGPS

SPRD_ENGPC_SUPPORT = TRUE                            ### Switch ENGPC Module Support
                                                     # Option1:TRUE      #define SPRD_ENGPC_SUPPORT
                                                     # Option2:FALSE

ENGPC_W217 = TRUE                                    ### Switch ENGPC_W217 Support
                                                     # Option1:TRUE      #define ENGPC_W217
                                                     # Option2:FALSE

LCD_INSTALL_ANGLE = 90
