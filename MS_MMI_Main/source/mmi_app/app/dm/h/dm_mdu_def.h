#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))


//[[define image infomation
RES_ADD_IMG(IMAGE_DM_SOFTWARE_UPDATE,"\\MMI_RES_##\\IMAG\\IMAGE_DM_SOFTWARE_UPDATE.png",IMG_CMP_PNG_ARGB_RLE,0)

//]]define image infomation





//[[define anim infomation

//]]define anim infomation





//[[define string infomation
RES_ADD_STRING(TXT_DM_SETTING,"DM setting")
//RES_ADD_STRING(TXT_DM_DLSESSION_PROGRESS,"Downloading Pls wait...")  //replaced by common ID TXT_COMMON_DOWNLOADING_WAIT
RES_ADD_STRING(TXT_DM_SOFTWARE_UPDATE_SERVICE,"Software update service")
RES_ADD_STRING(TXT_DM_DOWNLOAD_UPDATE,"The software updating might take 10 minutes. During the updating, all the functions will be unavailable, including emergency call.")
RES_ADD_STRING(TXT_DM_SOFTWARE_VERSION,"Software version")
RES_ADD_STRING(TXT_DM_SOFTWARE_UPDATE,"Software update")
RES_ADD_STRING(TXT_DM_INSTALL_UPDATE_PAG,"Install application package")
RES_ADD_STRING(TXT_DM_INSTALL_APP_PAG,"Install application package")
//RES_ADD_STRING(TXT_DM_SEARCHING_WAIT,"Searching , please wait")  //replaced by common ID TXT_COMMON_SEARCHING
RES_ADD_STRING(TXT_DM_UN_SEARCH_PACKAGE,"Unserach latest version software package")
RES_ADD_STRING(TXT_DM_DL_CONTINUE,"Download intermit,continue download")
RES_ADD_STRING(TXT_DM_DL_FINISH,"Download finish,install")
RES_ADD_STRING(TXT_DM_UPDATE_NOW,"Install now")
RES_ADD_STRING(TXT_DM_UPDATE_LATER,"Install later")
//RES_ADD_STRING(TXT_DM_INSTALL_NOW,"Install now")    //replaced by common ID TXT_YES
//RES_ADD_STRING(TXT_DM_INSTALL_LATER,"Install later")  //replaced by common ID TXT_COMM_NO
RES_ADD_STRING(TXT_DM_SERVICE_UNREG,"Dm service un register")
RES_ADD_STRING(TXT_DM_SERVICE_RUN,"Dm service on run")
RES_ADD_STRING(TXT_DM_PUSH,"Dm push in")
RES_ADD_STRING(TXT_DM_DL_WAIT,"Update package download,pls wait")
//RES_ADD_STRING(TXT_DM_INSTALL_FAIL,"Update package fail")  //replaced by common ID TXT_UPDATE_FAIL
RES_ADD_STRING(TXT_DM_INSTALL_SUCC,"Update package succ,handset reset")
RES_ADD_STRING(TXT_DM,"DM session")
RES_ADD_STRING(TXT_DM_DL,"DL session")
//RES_ADD_STRING(TXT_DM_START,"Dm Start")        //replaced by common ID TXT_COMMON_START
//RES_ADD_STRING(TXT_DM_FINISHED,"Finished")    //replaced by common ID  STXT_FINISH 
//RES_ADD_STRING(TXT_DM_ERROR,"Error")    //replaced by common ID  TXT_ERROR 
//RES_ADD_STRING(TXT_DM_DOWNLOAD_SUCCEED,"Download succeed")    //replaced by common ID TXT_DOWNLOAD_SUCCEED
//RES_ADD_STRING(TXT_DM_DOWNLOAD_FAILED,"Download failed")    //replaced by common ID TXT_DOWNLOAD_FAILED 
RES_ADD_STRING(TXT_DM_APN_SET,"APN set")
RES_ADD_STRING(TXT_DM_MON_SET,"Mon set")
RES_ADD_STRING(TXT_DM_DEV_VERSION_SET,"Software version")
//RES_ADD_STRING(TXT_DM_IMEI_SET,"IMEI")   //replaced by common ID TXT_PHONE_IMEI
RES_ADD_STRING(TXT_DM_MAN_SET,"Man set")
RES_ADD_STRING(TXT_DM_CNNECT_SERVER,"Connect server")
RES_ADD_STRING(TXT_DM_EXT_SERVICE,"High level service")
RES_ADD_STRING(TXT_DM_EXT_SERVICES,"Accept High level service")
RES_ADD_STRING(TXT_DM_NETWORK_DISCONNECT,"Network disconnect")
RES_ADD_STRING(TXT_DM_HANDSET_LOCK,"Handset lock")
//RES_ADD_STRING(TXT_DM_DEBUG_MODE,"Debug mode")  //replaced by common ID TXT_COMM_DEBUG_MODE
//RES_ADD_STRING(TXT_DM_DEBUG_MODE_OPEN,"Open")   //replaced by common ID TXT_OPEN_COMN
//RES_ADD_STRING(TXT_DM_DEBUG_MODE_CLOSE,"Close")  //replaced by common ID TXT_COMM_CLOSE
RES_ADD_STRING(TXT_DM_IDASDEC_TYPE,"Idasdec type")
RES_ADD_STRING(TXT_DM_SMS_REG,"Self register")
RES_ADD_STRING(TXT_DM_CHANGE_SERVER,"Change server")
RES_ADD_STRING(TXT_DM_SPREADTRUM,"spreadtrum")
RES_ADD_STRING(TXT_DM_LG,"LG")
RES_ADD_STRING(TXT_DM_NEWPOST,"Newpost")
RES_ADD_STRING(TXT_DM_LOG_SWITCH,"Dm lib log switch")
RES_ADD_STRING(TXT_DM_FOTA_SWITCH,"FOTA test")
RES_ADD_STRING(TXT_DM_SEND,"Send dm message")
RES_ADD_STRING(TXT_DM_TREE_SWITCH,"Tree switch")
RES_ADD_STRING(TXT_DM_DELAY_TIME,"Delay timer ")
RES_ADD_STRING(TXT_DM_DELAY_TIME_1,"After one hour ")
RES_ADD_STRING(TXT_DM_DELAY_TIME_2,"After two hour")
RES_ADD_STRING(TXT_DM_DELAY_TIME_4,"After four hour")
RES_ADD_STRING(TXT_DM_DELAY_TIME_24,"After twenty-Four hour")
RES_ADD_STRING(TXT_DM_DOWNLOAD_UPDATE_ALERT,"New software package exist,update?")
RES_ADD_STRING(TXT_DM_SWITCH,"Dm reg switch")
RES_ADD_STRING(TXT_DM_RESET,"Dm reset")
RES_ADD_STRING(TXT_DM_SW_UPDATE_ALERT,"Update phone version?")
RES_ADD_STRING(TXT_DM_EXIT,"Dm on the exit,pls try later!")
//RES_ADD_STRING(TXT_DM_DOWNLOAD_NO_SPACE,"Disk space insufficient")  //replaced by common ID TXT_COMM_NO_SPACE
RES_ADD_STRING(TXT_DM_FILE_PARSE_FIAL,"File parse fail!")
RES_ADD_STRING(TXT_DM_SRV_ADDR,"Server addr")
RES_ADD_STRING(TXT_DM_SELF_REG_NUMBER,"Self reg number")
RES_ADD_STRING(TXT_DM_SELF_REGPORT_NUMBER,"Self reg port number")
RES_ADD_STRING(TXT_DM_NOT_SUPPORT_INSTALL,"Not support java install1")
RES_ADD_STRING(TXT_DM_SEL_SIM,"DM Service sim setting")
RES_ADD_STRING(TXT_DM_SIMSEL_RESET_TIP,"DM Will not work until reset the phone, reset it now?")
RES_ADD_STRING(TXT_DM_SIMSEL_DMRUN_TIP,"DM is running pls try it later?")
RES_ADD_STRING(TXT_DM_MAIN_SIM,"Main Sim")
RES_ADD_STRING(TXT_DM_SCOMO_INSTALL, "Scomo Install")
RES_ADD_STRING(TXT_DM_SCOMO_FILE_ABSENT,"Can't find SCOMO file")
//]]define string information





//[[define data infomation

//]]define data infomation

#endif
