#ifdef ASP_SUPPORT

//[[define image infomation
//RES_ADD_IMG(IMAGE_ASP_RECORD_BUTTON_FOCUS,"\\MMI_RES_##\\IMAG\\IMAGE_ASP_RECORD_BUTTON_FOCUS.bmp",IMG_CMP_BMP_16_565_RLE,0)
//RES_ADD_IMG(IMAGE_ASP_RECORD_BUTTON,"\\MMI_RES_##\\IMAG\\IMAGE_ASP_RECORD_BUTTON.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(IMAGE_SECMENU_ICON_CALL_RECORD_UNREAD,"\\MMI_RES_##\\IMAG\\IMAGE_SECMENU_ICON_CALL_RECORD_UNREAD.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ASP_ICON_CALL_RECORD_READ,"\\MMI_RES_##\\IMAG\\IMAGE_ASP_ICON_CALL_RECORD_READ.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_IDLE_ICON_CALL_RECORD,          "\\MMI_RES_##\\IMAG\\IMAGE_IDLE_ICON_CALL_RECORD.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_IDLE_ICON_CALL_RECORD_FULL,  "\\MMI_RES_##\\IMAG\\IMAGE_IDLE_ICON_CALL_RECORD_FULL.bmp",IMG_CMP_BMP_16_565_RLE,0)
//RES_ADD_IMG(IMAGE_ASP_RECORDING,  "\\MMI_RES_##\\IMAG\\IMAGE_ASP_RECORDING.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_CALLRECORD_PLAY,  "\\MMI_RES_##\\IMAG\\IMAGE_CALLRECORD_PLAY.gif", IMG_CMP_GIF_SCI, 0)
#ifdef MMI_PDA_SUPPORT
RES_ADD_IMG(IMAGE_ASP_MENU_AUDIO_CALL,"\\MMI_RES_##\\IMAG\\Optionsmenu_icon\\IMAGE_ASP_MENU_AUDIO_CALL.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ASP_MENU_REPLAY_SMS,"\\MMI_RES_##\\IMAG\\Optionsmenu_icon\\IMAGE_ASP_MENU_REPLAY_SMS.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

//]]define image infomation


//[[define anim infomation
//]]define anim infomation
//[[define asp infomation
RES_ADD_STRING(TXT_MMIASP_ANSWER_PHONE,"Answer phone")
//RES_ADD_STRING(TXT_MMIASP_ANSWER_RECORD,"Answer record")       //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_ANSWER_AUDIO,"Answer phone salutation") //replaced by common ID TXT_COMM_ANSWER
RES_ADD_STRING(TXT_MMIASP_MSG_TIME,"Answer phone message duration")
RES_ADD_STRING(TXT_MMIASP_WAIT_TIME,"Waiting time for answer phone")
RES_ADD_STRING(TXT_MMIASP_SPEAKER,"Speaker")
RES_ADD_STRING(TXT_MMIASP_MSG_SAVE_PATH,"Message save location")
//RES_ADD_STRING(TXT_MMIASP_DEFAULT_AUDIO,"Default answer phone salutation")       //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_MORE_AUDIO,"More answer phone salutation")        //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_1_MINUTE,"1 minute")  //replaced by common ID TXT_COMM_1_MINUTE
RES_ADD_STRING(TXT_MMIASP_3_MINUTES,"3 minutes")
//RES_ADD_STRING(TXT_MMIASP_5_MINUTES,"5 minutes")  //replaced by common ID TXT_COMM_5_MINUTES
//RES_ADD_STRING(TXT_MMIASP_10_MINUTES,"10 minutes") //replaced by common ID TXT_COMM_10MINUTES
//RES_ADD_STRING(TXT_MMIASP_1_SECOND,"1 second")  //replaced by common ID TXT_COMM_1_SECOND
//RES_ADD_STRING(TXT_MMIASP_5_SECONDS,"5 seconds") //replaced by common ID TXT_COMM_10_SECONDS
//RES_ADD_STRING(TXT_MMIASP_10_SECONDS,"10 seconds") //replaced by common ID TXT_COMM_10_SECONDS
RES_ADD_STRING(TXT_MMIASP_15_SECONDS,"15 seconds")
//RES_ADD_STRING(TXT_MMIASP_20_SECONDS,"20 seconds") //replaced by common ID TXT_COMM_20_SECONDS
RES_ADD_STRING(TXT_MMIASP_NEW_MSG,"New message")
//RES_ADD_STRING(TXT_MMIASP_OPTION,"Option")      //case3£ºdefined but not used
RES_ADD_STRING(TXT_MMIASP_LINKMAN,"linkman")
//RES_ADD_STRING(TXT_MMIASP_PHONE_NUMBER,"phone number")  //replaced by common ID TXT_PHONE_NUMBER
//RES_ADD_STRING(TXT_MMIASP_TOTAL_TIME,"total time")    //replaced by common ID TXT_TOTAL_TIME
//RES_ADD_STRING(TXT_MMIASP_UNKNOW_NUM,"unknow num")    //replaced by common ID TXT_UNKNOWN_NUM
//RES_ADD_STRING(TXT_MMIASP_OPEN,"Start")       //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_DEV_U,"dev u")     //replaced by common ID TXT_COMMON_UDISK
//RES_ADD_STRING(TXT_MMIASP_DEV_SD,"dev sd")  //replaced by common ID  TXT_COMMON_SD_CARD
//RES_ADD_STRING(TXT_MMIASP_CLOSE,"Close")        //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_DIAL_CALL,"call")       //replaced by common ID TXT_CALL
//RES_ADD_STRING(TXT_MMIASP_SEND_MESSAGE,"Send Message")  //replaced by common ID  TXT_SEND_MESSAGE
//RES_ADD_STRING(TXT_MMIASP_DELETE,"Delete")     //replaced by common ID TXT_DELETE
//RES_ADD_STRING(TXT_MMIASP_MARK,"Mark") //replaced by common ID STXT_MARK
//RES_ADD_STRING(TXT_MMIASP_SEND,"Send")  //replaced by common ID TXT_SEND
//RES_ADD_STRING(TXT_MMIASP_SETTING,"Setting")        //replaced by common ID STXT_MAIN_SETTING
//RES_ADD_STRING(TXT_MMIASP_DIAL_IPCALL,"IP Call")      //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_SEND_SMS,"Send SMS")     //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_SEND_MMS,"Send MMS")     //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_CANCEL_MARK,"Unmark")     //replaced by common ID TXT_CANCEL_MARK
//RES_ADD_STRING(TXT_MMIASP_MARKALL,"Mark All") //replaced by common ID TXT_MARK_ALL
//RES_ADD_STRING(TXT_MMIASP_CANCEL_MARKALL,"Unmark All")     //replaced by common ID TXT_CANCEL_MARK_ALL
//RES_ADD_STRING(TXT_MMIASP_SEND_BLUETOOTH,"Bluetooth")       //case3£ºdefined but not used
RES_ADD_STRING(TXT_MMIASP_RING_FIXED,"fixed ring")
RES_ADD_STRING(TXT_MMIASP_MORE_RING,"more ring")
//RES_ADD_STRING(TXT_MMIASP_MAKE_SURE_DELETE,"make sure delete")         //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_SYS_FILE_ATTAINT,"system Failure")       //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_EMPTY_LIST,"empty list")             //replaced by common ID TXT_COMMON_COMMON_EMPTY_LIST
//RES_ADD_STRING(TXT_MMIASP_ANSWER_AUDIO_TIME,"Answer phone salutation time")        //case3£ºdefined but not used
//RES_ADD_STRING(TXT_MMIASP_NOT_SET_OBJECT,"not set object")      //case3£ºdefined but not used
RES_ADD_STRING(TXT_MMIASP_GOTO_ANSWER_FAIL,"go to answer fail")
RES_ADD_STRING(TXT_MMIASP_RECORD_ANSWER_WORD,"Record a answer wordl")
//RES_ADD_STRING(TXT_MMIASP_ACCEPT_CALL, "answer call")     //replaced by common ID TXT_COMMON_ANSWER_CALL
RES_ADD_STRING(TXT_MMIASP_PLAY_RECORD, "play record")
//RES_ADD_STRING(TXT_MMIASP_OPEN_MACHINE, "Open record")     //case3£ºdefined but not used
RES_ADD_STRING(TXT_ASP_RECORDING, "RECORDING")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_1, "Answer Ring 1")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_2, "Answer Ring 2")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_3, "Answer Ring 3")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_4, "Answer Ring 4")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_5, "Answer Ring 5")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_6, "Answer Ring 6")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_7, "Answer Ring 7")
RES_ADD_STRING(TXT_ASP_ANSWER_RING_8, "Answer Ring 8")
RES_ADD_STRING(TXT_ASP_NO_FREE_SPACE, "No enough space for recoding")
//RES_ADD_STRING(TXT_ASP_LIST_FULL,           "List full, unable to recording")       //case3£ºdefined but not used
RES_ADD_STRING(TXT_ANSWER_PHONE_SIM1, "sim1 answer_phone")
RES_ADD_STRING(TXT_ANSWER_PHONE_SIM2, "sim2 answer_phone")
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
RES_ADD_STRING(TXT_ANSWER_PHONE_SIM3, "sim3 answer_phone")
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
RES_ADD_STRING(TXT_ANSWER_PHONE_SIM4, "sim4 answer_phone")
#endif
RES_ADD_STRING(TXT_MMIASP_NEW_MULT_MSGS, "new messages")
RES_ADD_STRING(TXT_MMIASP_AUTOPLAY_MSGS, "Autoplay messages")
RES_ADD_STRING(TXT_MMIASP_MSG_NUM_IS_FULL, "Record message num is full")
RES_ADD_STRING(TXT_MMIASP_DIR_IS_FULL, "Dir is full, unable to record")
RES_ADD_STRING(TXT_MMIASP_ANSWER_AUDIOHELP, "user setting help")
RES_ADD_STRING(TXT_ASP_UDISK_USING, "U Disk is in use, failed to ASP")
RES_ADD_STRING(TXT_ASP_NO_SPACE_GOTO_ANSWER_FAIL, "Not enough space,fail to activate answering")
//RES_ADD_STRING(TXT_ASP_NOT_SUPPORT_CURRENT_FORMAT, "The current format does not support the answering words,choose to record wav format")    //case3£ºdefined but not used
    

//]]define string infomation
//[[define data infomation
//]]define data infomation
#endif
