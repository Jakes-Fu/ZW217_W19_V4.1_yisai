#################################################307gerneral有且开着，217无，暂按307的补充####################################################
#todo:不确定mmi_app\kernel source\resource
APP_PRODUCT_DM = FALSE                                ### APP FOR 3G PS SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE


BBM_ROLE_SUPPORT = NONE                             ###BLE & BT & modem
                                                    # option1:NONE
                                                    # option2:MASTER_ONLY
                                                    # option3:SLAVER_ONLY
                                                    # option4:MASTER_SLAVER_BOTH

BT_BLE_SUPPORT = FALSE                              ### Switch Of BLE Support
                                                    # Option1:TRUE
                                                    # Option2:FALSE
#todo:不确定
CAP_TP_SUPPORT = NONE                               ### Capacitive Touch panel chip
                                                    # Option0:NONE          #define CAP_TP_SUPPORT_NONE
                                                    # Option1:FT5206        #define CAP_TP_SUPPORT_FT5206
                                                    # Option1:CY8C21434     #define CAP_TP_SUPPORT_CY8C21434

DAPS_IPERF_SUPPORT = FALSE                           ### IPERF Profile Support:
                                                    # Option1:FALSE

HTTP2_SUPPORT = TRUE                                ### HTTP2 Profile Support:
                                                    # Option1:FALSE

#todo:不确定
#MEMORY_LAYOUT_FOR_128M = TRUE                      ### memory layout 128M

ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
MMS_SUPPORT = NONE                                  ### MMS SW Supplier:
                                                    # Option1:NONE     #define MMS_SUPPORT_NONE
                                                    # Option2:DORADO    #define MMS_SUPPORT_DORADO
endif


MET_MEX_CQZ_SUPPORT = FALSE                         ### MET_MEX_CQZ_SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MET_MEX_TOMCAT_SUPPORT = FALSE                      ### MET MEX TOMCAT SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE
#todo:1h00有
MMI_MULTI_SIM_SYS = SINGLE                           ### Platform Support Multi Sim Card Number
                                                     # Option1:SINGLE    #define MMI_MULTI_SIM_SYS_SINGLE
                                                     # Option2:DUAL      #define MMI_MULTI_SIM_SYS_DUAL

MMI_NOT_SUPPORT_USSD_SS_INCC = TRUE                 #ussd and ss is not support in dial panel for watch
#Option1:TRUE not support ussd and ss request from dial panel
#Option2:FALSE support ussd and ss request from dial panel

#mmi_app\kernel\
MN_RPC_SUPPORT = TRUE                               ### MN_RPC Support remote process call between AP and CP:
                                                    # Option1:TRUE
                                                    # Option2:FALSE

MMI_VOLTE_SUPPORT = TRUE                            ### 4G SUPPORT
                                                    # Option1:TRUE
                                                    # Option2: FALSE

#硬件
MOCOR_SUPPORT_ARGB888_MODE = FALSE                   #Open/Close ARGB Mode On CAMERA Preview

GUIF_PRGBOX_AUTOHIDE = TRUE

QQ_SUPPORT = NONE                                    ### QQ Support

QWERTY_KEYPAD = NONE                                 ### QWERTY_KEYPAD TYPE ???????????????? simulator

#307 ms_ref有相关文件与217不同
RESETFACTORY_SETTING_SIMPLE = FALSE                   ### reset factory settings
                                                     # Option1: TRUE: erase AP running nv, CP running nv, C:
                                                     # Option2: FALSE: deleted data by every application, adjusted by project


UAL_NETWORKMGR_SUPPORT = FALSE                        ###enable ual network manger
                                                     # Option1:TRUE
                                                     # Option2:FALSE

UI_MULTILAYER_SUPPORT = TRUE                         ### ui layer support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

OMADL_SUPPORT = FALSE                                 ### OMA download Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE
                                                     # work for XML_SUPPORT = TRUE && HTTP_SUPPORT = TRUE

######################################################child base有217无#######################################################################################
PDA_UI_DROPDOWN_WIN = TRUE    #TODO:DROPDOWN = NOTIFY_ONLY

######################################################217宏值与307不同，暂按307设置###########################################################################

BROWSER_ALL_RUNNING_SUPPORT = FALSE                  ### Browser running at the same time Supplier:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

BROWSER_OPERA_START_PAGE = FALSE                     ### Browser SW Supplier:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

#todo:!!!!!!!!!!!打开关联700多报错!!!!!!!!!
CMCC_VCARD_SUPPORT = FALSE                            ### Switch VCARD Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSECMCC_VCARD_SUPPORT = TRUE

EBOOK_SUPPORT = FALSE                                 ### EBOOK Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE
FONT_HAN_SIMP_ONLY = TRUE                            ### FONT SIMP Only:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

GAME_SUPPORT = FALSE                                  ### game Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

IKEYBACKUP_SUPPORT = FALSE                           ### 1-key backup Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

IM_UDB_SUPPORT   = TRUE                             ### Pre-filled Udb Support, only support when IM_ENGINE is T9 and umem_support set TRUE
                                                     # Option1:TRUE
                                                     # Option2:FALSE

KING_MOVIE_SUPPORT = NONE                             ### kingmovie play:
                                                     # Option1:STAND
                                                     # Option2:EXTEND
                                                     # Option3:ALL
                                                     # Option4:NONE

MID_SOFTKEY_USE_IMG_SUPPORT = FALSE                  ### MID_SOFTKEY_USE_IMG:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MMI_3DUI_SUPPORT = TRUE                              ### algorithm collection(sin cos)
                                                     # Option1:TRUE
                                                     # Option2:FALSE
#mmi_app\kernel\
MMI_CHECK_MEMINFO_SUPPORT = TRUE                     ### Check Memory Overrun Info Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MMI_COMMON_IDLE_SUPPORT = TRUE                       ### Common Idle Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MMI_DISPLAY_SIMP_CHINESE_SUPPORT = FALSE              ### DISPLAY SIMPLE CHINESE SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MMI_PREFER_PLMN_SUPPORT = FALSE                       ### Prefer network
                                                     # Option1: TRUE
                                                     # Option2: FALSE

MP3_EQ_SUPPORT = FALSE                                ### MP3 EQ Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

NANDBOOT_SUPPORT = FALSE                             ### Boot From Nandflash Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SCREENSAVER_SUPPORT = FALSE                           ### screensaver support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SMS_CHAT_SUPPORT = TRUE                             ### CHATMODE SUPPORT Support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE
#217与307 2h11宏值不同，暂按307
#*************************************************************************
# Feature Options FOR CUSTOMER DEFINED
# *************************************************************************

SFR_SUPPORT = FALSE                                  ### DM Register R
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SFR_SUPPORT_CTCC = FALSE                             ### DM CTCC
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SFR_SUPPORT_CTCC_SMS = FALSE                         ### DM CTCC_SMS_SFR
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SFR_SUPPORT_CMCC = FALSE                             ### DM CMCC
                                                     # Option1:TRUE support CMTT self register
                                                     # Option2:FALSE not support CMTT self register
#2h11 end

SIM_PLUG_IN_SUPPORT = NONE                          ### Platform Support SIM HOT PLUGIN
                                                     # Option1:HARDWARE
                                                     # Option2:LOGIC
                                                     # Option3:NONE

TIANLONG_MOCOR = FALSE                                ### Switch between Advanced and Mocor PS:
                                                     # Option1:TRUE

UI_P3D_SUPPORT = TRUE                                ### UI P3D Effect Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE


WMA_BACKGROUND_SUPPORT = FALSE                        ### WMA BACKGROUND SUPPORT
                                                     # Option1:FALSE

WMA_SEEK_SUPPORT = FALSE                              ### WMA Seek Function Support:
                                                     # Option1:FALSE

WMA_SUPPORT = FALSE                                   ### WMA Decoder Support:
                                                     # Option1:FALSE

UAL_SUPPORT = TRUE                                   ### Platform Support UAL
                                                     # Option1:TRUE
                                                     # Option2:FALSE

UAL_TELE_SUPPORT = TRUE                              ### ual_tele
                                                     # Option1:TRUE
                                                     # Option2:FALSE
MODEM_SUPPORT = RDA                                 ### modem support
                                                     # Option1:RDA
                                                     # Option2:SPRD
UAL_WIFI_SUPPORT = TRUE                              ### ual_wifi
                                                     # Option1:TRUE
                                                     # Option2:FALSE

UAL_GNSS_SUPPORT = TRUE                            ### ual gnss
                                                    # Option1:TRUE
                                                    # Option2:FALSE

UAL_RF_AM_SUPPORT = TRUE                             ### ual RF AM
                                                    # Option1:TRUE
                                                    # Option2:FALSE


DYNAMIC_WATCHFACE_SUPPORT = FALSE#TRUE                   ###DYNAMIC WATCHFACE SUPPORT
                                                    # Option1:TRUE
                                                    # Option2:FALSE

WATCHFACE_MANAGE_SUPPORT = FALSE#TRUE                     ###WATCHFACE_MANAGE support
                                                    # Option1:TRUE
                                                    # Option2:FALSE

WATCHFACE_CTRL_EDIT_SUPPORT = FALSE                 ###ifdef WATCHFACE CTRL EDIT SUPPORT
                                                    # Option1:TRUE
                                                    # Option2:FALSE

UAL_AUDIOPLAYER_SUPPORT = TRUE                     ### ual audioplayer
                                                    # Option1:TRUE
                                                    # Option2:FALSE

UAL_KV_SUPPORT = TRUE                               ### ual kv
                                                    # Option1:TRUE
                                                    # Option2:FALSE

UAL_DEVICE_SUPPORT = TRUE                           ### ual device
                                                    # Option1:TRUE
                                                    # Option2:FALSE

AUDIOPLAYER_DEMOAPP_SUPPORT = TRUE                  ### 工程模式下audioplayer demo app
                                                    # Option1:TRUE
                                                    # Option2:FALSE

SCREEN_SHAPE = CIRCULAR                             ###SCREEN_SHAPE support
                                                    # Option1:CIRCULAR
                                                    # Option2:SQUARE
APP_STORE_SUPPORT  = FALSE                         ### APP_STORE support
                                                    # Option1:TRUE
                                                    # Option2:FALSE

DYNAMIC_C_APP_SUPPORT = FALSE                       ### APP_STORE support
                                                    # Option1:TRUE
                                                    # Option2:FALSE

JS_APP_SUPPORT = FALSE                              #support JS APP
                                                    # Option1:TRUE
                                                    # Option2:FALSE


JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT = FALSE          ### JAVASCRIPT_ECO_ENVIRONMENT
                                                    # Option1:TRUE
                                                    # Option2:FALSE

FM_SUPPORT = NONE                                   ### FM Function Support:
                                                    # Option0:NONE      #define FM_SUPPORT_NONE
                                                    # Option1:CL6012X   #define FM_SUPPORT_CL6012X
                                                    # Option2:NMI601    #define FM_SUPPORT_NMI601
                                                    # Option3:SPRD_V0   #define SPRD_V0
                                                    # Option4:AUTO      #define AUTO

MMI_GPRS_SWITCH_SUPPORT = TRUE                      ### GPRS SWITCH SUPPORT
                                                    # Option1:TRUE
                                                    # Option2:FALSE

DV_UDISK_SUPPORT = TRUE

ENG_BAND_FREQ_SUPPORT = TRUE                      ### ENG BAND FREQ SUPPORT
                                                    # Option1:TRUE
                                                    # Option2:FALSE

#UTIT framework
UTEST_SUPPORT = FALSE                               ### UTIT support:
                                                    # Option1: PE   Run UT test on watch / phone(not support)
                                                    # Option2: PC   Run UT test on PC(TODO: Run IT on PC)
                                                    # Option3: FALSE
WF_USE_SINGLELAYER = TRUE                           #Watchface use multilayers(FALSE) or single layer(TRUE),
                                                    # Option1:TRUE
                                                    # Option2:FALSE

UI_LAYER_SUPPORT_LOW_MEMORY = TRUE                  # UI Layer do not use double buffer in low memory model.