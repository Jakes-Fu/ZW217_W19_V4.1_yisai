

//[[define image infomation
//RES_ADD_IMG(IMAGE_MMS_NOTIFY_NEW_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_NOTIFY_NEW_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_MMS_PROGRESS_BAR_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_PROGRESS_BAR_ICON.bmp",IMG_CMP_BMP_16_565_RLE,0)
//RES_ADD_IMG(IMAGE_NOT_READ_MMS_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_NOT_READ_MMS_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_READ_MMS_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_READ_MMS_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_MMS_EDIT_TO,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_TO.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_EDIT_VIDEO,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_VIDEO.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_EDIT_SUBJECT,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_SUBJECT.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifdef MMIMMS_SMS_IN_1_SUPPORT
RES_ADD_IMG(IMAGE_MMS_EDIT_MUSIC,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_MUSIC.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
RES_ADD_IMG(IMAGE_MMS_EDIT_ADD_PIC_OR_VIDEO,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_ADD_PIC_OR_VIDEO.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_MMS_EDIT_ADD_MENU_FOCUS,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_ADD_MENU_FOCUS.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_EDIT_ADD_MENU,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_EDIT_ADD_MENU.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_ACCESSORY_FILE,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_ACCESSORY_FILE.png",IMG_CMP_PNG_ARGB_RLE,0)


//RES_ADD_IMG(IMAGE_MMS_NOW_DOWNLOAD_IDLE,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_NOW_DOWNLOAD_IDLE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_NOW_DOWNLOAD_INBOX,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_NOW_DOWNLOAD_INBOX.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_NOW_DOWNLOAD_INBOX_LOCK,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_NOW_DOWNLOAD_INBOX_LOCK.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_WAITING_DOWNLOAD_INBOX_LOCK,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_WAITING_DOWNLOAD_INBOX_LOCK.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_WAITING_DOWNLOAD_INBOX,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_WAITING_DOWNLOAD_INBOX.png",IMG_CMP_PNG_ARGB_RLE,0)

#if defined( MMI_PDA_SUPPORT)
//RES_ADD_IMG(IMAGE_MMS_PLAY_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_PLAY_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_PAUSE_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_PAUSE_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_MMS_REPLY_SMS_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_REPLY_SMS_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_MMS_DELETE_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_DELETE_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_MMS_REPLY_MMS_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_REPLY_MMS_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)

//RES_ADD_IMG(IMAGE_MMS_REPLY_SMS_GRAY_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_REPLY_SMS_GRAY_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_MMS_DELETE_GRAY_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_DELETE_GRAY_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_MMS_DOWNLOAD_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_DOWNLOAD_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_DOWNLOAD_CANCEL_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_DOWNLOAD_CANCEL_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_MMS_PREVIEW_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_PREVIEW_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_INSERT_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_INSERT_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_SEND_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_SEND_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)

//RES_ADD_IMG(IMAGE_MMS_FORWARD_SMS_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_NOW_DOWNLOAD_IDLE.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_MMS_FORWARD_SMS_GRAY_PDA,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_REPLY_SMS_GRAY_PDA.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#if defined( MMI_PDA_SUPPORT) || !defined( MMI_GUI_STYLE_TYPICAL )
RES_ADD_IMG(IMAGE_MMS_COUNT_LBTN_PR,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_COUNT_LBTN_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_COUNT_LBTN_UN,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_COUNT_LBTN_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_COUNT_RBTN_PR,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_COUNT_RBTN_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_MMS_COUNT_RBTN_UN,"\\MMI_RES_##\\IMAG\\IMAGE_MMS_COUNT_RBTN_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
//]]define image infomation


//[[define anim infomation
//RES_ADD_ANIM(IMAGE_MMS_HAVEDV_ANIM,"\\MMI_RES_##\\ANIM\\IMAGE_MMS_HAVEDV_ANIM",AT_COMPRESS_STREAM,75,0x000000)
//RES_ADD_ANIM(IMAGE_MMS_IMAGE_BIG_ANIM,"\\MMI_RES_##\\ANIM\\IMAGE_MMS_IMAGE_BIG_ANIM",AT_COMPRESS_STREAM,75,0x000000)
//RES_ADD_ANIM(IMAGE_MMS_HAVESOUND_ANIM,"\\MMI_RES_##\\ANIM\\IMAGE_MMS_HAVESOUND_ANIM",AT_COMPRESS_STREAM,75,0x000000)
//]]define anim infomation


//[[define string infomation
#if 1
//RES_ADD_STRING(TXT_MMS_EDIT_SET_SEND_ONTIME_DATE,"Set date")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_SET_SEND_ONTIME_TIME,"Set time")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_EDIT_RECEIVER_NONE,"No receiver")
//RES_ADD_STRING(TXT_MMS_SEND_TOO_LARGE,"Too large")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_ADD_FAIL,"Add fail")  //replaced by common ID TXT_ADD_FAIL
//RES_ADD_STRING(TXT_MMS_INPUT_ERROR,"Input error")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_INSERT_VIDEO,"Insert video")    //replaced by common ID TXT_VIDEO
//RES_ADD_STRING(TXT_MMS_DEL_VIDEO,"Delete video")    //replaced by common ID TXT_VIDEO
//RES_ADD_STRING(TXT_MMS_EDIT_MMS_TO,"To")  //replaced by common ID TXT_COMMON_RECIPIENT
//RES_ADD_STRING(TXT_MMS_EDIT_MMS_CC,"Cc")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_MMS_BCC,"Bcc")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_SET_SEND_ONTIME,"Set send ontime")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_SET_OK,"Set ok")//case 3:defined but not used
//RES_ADD_STRING(STXT_PHONEBOOK,"Phone book")
RES_ADD_STRING(TXT_MMS_EDIT_ADD_TO_FULL,"To full")
//RES_ADD_STRING(TXT_MMS_EDIT_ADD_CC_FULL,"CC full")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_ADD_BCC_FULL,"BCC full")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EMPTY_NUM,"Empty number")         //replaced by common ID TXT_COMMON_EMPTY_NUM
//RES_ADD_STRING(TXT_MMS_RECEIVER_LIST,"Receiver list")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_BCC_LIST,"Bcc list")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_CC_LIST,"Cc list")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ADD_TO,"Add to")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_TO_OPTION,"Option")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_EDIT_TEXT,"Edit text")    //replaced by common ID TXT_TEXT
//RES_ADD_STRING(TXT_MMS_CONTENT_DISPLAY,"Content")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_DISPLAY_OPTION,"Option")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_NO_CONTENT,"No content")    //replaced by common ID TXT_NO_CONTENT
//RES_ADD_STRING(TXT_MMS_IMAGE_LIST,"Image list")
//RES_ADD_STRING(TXT_MMS_AUDIO_LIST,"Audio list")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_VIDEO_LIST,"Video list")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ADD_FAIL,"Add fail")//case 3:defined but not used
//RES_ADD_STRING(TXT_VIDEO_INPUTNAME,"Input name")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SEND_SUCCEED,"Send succeed")  //replaced by common ID TXT_COMM_SEND_SUCCEED
//RES_ADD_STRING(TXT_MMS_SEND_FAILED,"Send failed")  //replaced by common ID  TXT_COMM_SEND_FAILED
//RES_ADD_STRING(TXT_MMS_DOWNLOAD_SUCCEED,"Download succeed")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_DOWNLOAD_FAILED,"Download failed")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SAVED,"Save")  //replaced by common ID TXT_COMM_SAVE_AS_DRAFT
//RES_ADD_STRING(TXT_MMS_NOT_ENOUGH_SPACE,"Not enough space")  //replaced by common ID TXT_NO_SPACE
//RES_ADD_STRING(TXT_MMS_IF_REPLACE,"Replace")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_IMAGE_EMPTY,"No image")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_MUSIC_EMPTY,"No music")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_VIDEO_EMPTY,"No video")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_MMS_TOO_BIG,"MMS is too big")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_IF_DEL,"If Delete")    //replaced by common ID TXT_DELETE_QUERY
#if defined(MMI_SMS_DELALL_SUPPORT)
RES_ADD_STRING(TXT_MMS_DELED,"Deleted")
#endif
//RES_ADD_STRING(TXT_MMS_CANNOT_INSERT_SLIDE,"Cannot insert new slide")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_DEL_SLIDE,"Delete current slide")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_INBOX_EMPTY,"Inbox is empty")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SENTBOX_EMPTY,"Sentbox is empty")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_OUTBOX_EMPTY,"Outbox is empty")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_DRAFTBOX_EMPTY,"Draftbox is empty")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NO_NUMBER,"No number")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_DONE,"Done")    //replaced by common ID TXT_COMPLETE 
//RES_ADD_STRING(TXT_MMS_RECEIVE_MMSNOTI,"Receive MMS notification")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_RECEIVE_MMS,"Receiver new MMS")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NOTIFICATION_PREVIEW,"Has sent a MMS to you.Please download it from MMS center.")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_MMSC,"MMSC")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_GROUP_SEND2,"Send by group")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_USER_DEFINED,"Costomized")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT,"Edit")  //replaced by common ID TXT_EDIT
//RES_ADD_STRING(TXT_MMS_DEL,"Delete")         //replaced by common ID TXT_DELETE
//RES_ADD_STRING(TXT_MMS_DEL_ALL,"Delete all")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ANSER_SMS,"Reply SMS")  //replaced by  common ID TXT_SMS
//RES_ADD_STRING(TXT_MMS_ANSER_MMS,"Reply MMS")  //replaced by common ID TXT_MMS
//RES_ADD_STRING(TXT_MMS_FORWARD,"Forward")  //replaced by common ID TXT_COMMON_FORWARD
//RES_ADD_STRING(TXT_MMS_RESEND,"Resend")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_PICKOUT_NUMBER,"Extract number")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SAVE_IMAGE,"Save picture")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SAVE_MUSIC,"Save sound")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SAVE_VIDEO,"Save video")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ANSWER_PHONE,"CALL")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_DOWNLOAD_MMS,"Download")  //replaced by common ID TXT_COMMON_DOWNLOAD_COMN
//RES_ADD_STRING(TXT_MESSAGE_SETTING_MMS,"MMS settings")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_AUTO_DOWNLOAD,"Retrieve MMS")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_PRIORITY,"Priority")  //replaced by common ID TXT_COMMON_PRIORITY
RES_ADD_STRING(TXT_MMS_DURATION,"Message validity period")
//RES_ADD_STRING(TXT_MMS_SERVICE_SETTING,"Sevice settings")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NETWORK_SETTING,"Network settings")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_SETTING_MMSC,"MMS center")
//RES_ADD_STRING(TXT_MMS_SETTING_GATEWAY,"Gateway")  //replaced by common ID TXT_COMMON_GATEWAY
//RES_ADD_STRING(TXT_MMS_SETTING_PORT,"Port")  //replaced by common ID TXT_COMM_PORT
RES_ADD_STRING(TXT_MMS_ARRIVE_REPORT,"Arrive report")
RES_ADD_STRING(TXT_MMS_READ_REPORT,"Read report")
RES_ADD_STRING(TXT_MMS_ANONYMITY,"Anonymity")
RES_ADD_STRING(TXT_MMS_PLAY_SOUND,"Play sound")
//RES_ADD_STRING(TXT_MMS_ADVERTISING,"Adversting")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_PRIORITY_HIGH,"High priority")         //replaced by common ID TXT_COMMON_HIGH
//RES_ADD_STRING(TXT_MMS_PRIORITY_MID,"Medium priority")  //replaced by common ID TXT_COMMON_MID
//RES_ADD_STRING(TXT_MMS_PRIORITY_LOW,"Low priority")    //replaced by common ID TXT_COMM_LOW
#if defined ( DATA_ROAMING_SUPPORT )
RES_ADD_STRING(TXT_MMS_RETRIEVE_MANUAL,"Retrieve manual")
#endif
//RES_ADD_STRING(TXT_MMS_RETRIEVE_AUTO,"Retrieve auto")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ONE_HOUR,"1 hour")  //replaced by common ID TXT_COMM_1_HOUR
//RES_ADD_STRING(TXT_MMS_SIX_HOUR,"6 hours")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_TWELVE_HOUR,"12 hours") //replaced by common ID TXT_COMM_12_HOURS
//RES_ADD_STRING(TXT_MMS_TWENTY_FOUR_HOUR,"24 hours")  //replaced by common ID TXT_COMM_24_HOURS
//RES_ADD_STRING(TXT_MMS_SEVENTY_TOW_HOUR,"72 hours")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_FORTY_EIGHT_HOUR,"48 hours")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_ONE_DAY,"1 day")//case 3 :defined but not used
RES_ADD_STRING(TXT_MMS_ONE_WEEK,"1 week")
RES_ADD_STRING(TXT_MMS_LONGGEST_DURATION,"Maximum")
//RES_ADD_STRING(TXT_MMS_SET_AS_DEFAULT,"Set as default")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_CHANGE_NAME,"Change name")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_PIECE,"Piece of MMS")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_PLAY,"Play") //REPLACED by common ID TXT_COMMON_PLAY
//RES_ADD_STRING(TXT_MMS_STOP,"Stop")    //replaced by common ID  TXT_PAUSE
//RES_ADD_STRING(TXT_MMS_OPTION,"Option")    //replaced by common ID STXT_OPTION
//RES_ADD_STRING(TXT_MMS_SEND_BY_GROUP,"Send by group")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_CONTENT,"MMS mem")//case 3 :defined but not used
RES_ADD_STRING(TXT_MMS_EDIT_OPTION,"MMS option")
RES_ADD_STRING(TXT_MMS_MMSSETTING,"MMS setting")
//RES_ADD_STRING(TXT_MMS_EDIT_MMS_CONTENT,"Edit content")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_CHOOSE_TO,"choose receiver")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_INBOX_READ,"Read MMS")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_INBOX_READ_NTF,"Read notification")//case 3 :defined but not used
RES_ADD_STRING(TXT_MMS_INBOX_OPTION,"Inbox option")
RES_ADD_STRING(TXT_MMS_SENTBOX_OPTION,"Sentbox option")
RES_ADD_STRING(TXT_MMS_OUTBOX_OPTION,"Outbox option")
//RES_ADD_STRING(TXT_MMS_DRAFTBOX_OPTION,"Draft option")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_CANNOT_SEND_IN_CALL,"Not send duaring call")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_CANNOT_DOWNLOAD_IN_CALL,"Cannot download duaring call")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_SR_ENABLE,"Activate")          //replaced by common ID TXT_COMMON_ENABLE_ON
//RES_ADD_STRING(TXT_MMS_SR_DISABLE,"Deactivate")       //replaced by common ID TXT_CLOSE      
//RES_ADD_STRING(TXT_MMS_RE,"Re:")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_FWD,"Fwd:")//case 3 :defined but not used
RES_ADD_STRING(TXT_MMS_DETAIL,"Message details")
//RES_ADD_STRING(TXT_MMS_DETAIL_TO,"To")  //replaced by common ID TXT_COMMON_TO
//RES_ADD_STRING(TXT_MMS_DETAIL_PRIO,"Priority:")  //replaced by common ID TXT_DETAIL_PRIORITY
//RES_ADD_STRING(TXT_MMS_VIEW_SUBJECT,"Subject:")  //replaced by common ID TXT_INPUT_SUBJECT
//RES_ADD_STRING(TXT_MMS_VIEW_CC,"Cc")//case 3 :defined but not used
RES_ADD_STRING(TXT_MMS_DETAIL_FROM,"From:")
//RES_ADD_STRING(TXT_MMS_DETAIL_SIZE,"Size")  //replaced by common ID TXT_DETAIL_SIZE
RES_ADD_STRING(TXT_MMS_DETAIL_RCV_TIME,"Receive time:")
RES_ADD_STRING(TXT_MMS_DETAIL_SEND_TIME,"Send time:")
//RES_ADD_STRING(TXT_MMS_MMS,"MMS")  //replaced by common ID TXT_MMS
//RES_ADD_STRING(TXT_MMS_INBOX,"Inbox")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_SENTBOX,"Sentbox")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_OUTBOX,"Outbox")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_DRAFTBOX,"Draftbox")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_SETTING,"Message settings")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_SEND,"Sent")  //replaced by common ID TXT_SEND
//RES_ADD_STRING(TXT_MMS_EDIT_CONTENT,"Edit content")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_GROUP_SEND,"Multiple send")//case 3 :defined but not used
//RES_ADD_STRING(TXT_MMS_SAVE,"Save") //replaced by common ID STXT_SAVE
//RES_ADD_STRING(TXT_MMS_SEND_AND_SAVE,"Send and save")//case 3 :defined but not used
//RES_ADD_STRING(TXT_CL_NEW_MMS,"NEW MMS")//case 3 :defined but not used
RES_ADD_STRING(TXT_DELIVERYRPT_RETRIVED,"MMS have been accepted")
RES_ADD_STRING(TXT_DELIVERYRPT_REJECTED,"MMS have been refused")
RES_ADD_STRING(TXT_DELIVERYRPT_DEFERRED,"MMS is defered")
RES_ADD_STRING(TXT_DELIVERYRPT_EXPIRED,"MMS is overdue")
RES_ADD_STRING(TXT_DELIVERYRPT_UNRECOGNISED,"MMS is not admitted")
//RES_ADD_STRING(TXT_MMS_RECV_SUCCES,"Recv success")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_RECV_FAILED,"Recv failed ")    //replaced by common ID TXT_COMMON_RECEIVE_FAIL
//RES_ADD_STRING(TXT_MMS_RECV_CANCEL,"Recv cancel")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_CANCEL_SUCCESS,"Cancel success")
//RES_ADD_STRING(TXT_MMS_NET_FAILED,"Net failed")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_READ_FAILED,"Read failed")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_IF_SEND_READRPT,"If send read report")
//RES_ADD_STRING(TXT_MMS_RECV_INVALID,"Recv invalid period")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_OPENING_MMS,"Opening MMS")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SAVEING_MMS,"Saving MMS")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_EDIT_SUBJECT,"Edit title")
//RES_ADD_STRING(TXT_MMS_EDIT_ADD_TO,"Add")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_DELETE_TO,"delete to address")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_MODIFY_TO,"modify to address")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_INSERT_SLIDE,"Insert slide")
RES_ADD_STRING(TXT_MMS_INSERT_SLIDER,"Insert slide")
RES_ADD_STRING(TXT_MMS_DEL_CUR_SLIDE,"Delete slide")
//RES_ADD_STRING(TXT_MMS_INSERT_IMAGE,"Insert picture")    //replaced by common ID  TXT_IMAGE
#ifdef MMI_GUI_STYLE_MINISCREEN
RES_ADD_STRING(TXT_MMS_EDIT_TEXT,"Text")    //replaced by common ID TXT_TEXT
#else
//RES_ADD_STRING(TXT_MMS_EDIT_TEXT,"Edit text")    //replaced by common ID TXT_TEXT
#endif
//RES_ADD_STRING(TXT_MMS_INSERT_MUSIC,"Insert sound")    //replaced by common ID TXT_AUDIO
//RES_ADD_STRING(TXT_MMS_DEL_IMAGE,"Delete picture")    //replaced by common ID  TXT_IMAGE
//RES_ADD_STRING(TXT_MMS_DEL_MUSIC,"Delete sound")    //replaced by common ID TXT_AUDIO
//RES_ADD_STRING(TXT_MMS_PREV_SLIDE,"Previous slide")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NEXT_SLIDE,"Next slide")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_INPUT_EMAIL,"<Enter email>")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_PREVIEW,"Preview")    //replaced by common ID STXT_PREVIEW
RES_ADD_STRING(TXT_MMS_SET_SLIDE_TIME,"Set slide time")
RES_ADD_STRING(TXT_MMS_SET_SLIDE_TIME_OPT,"Set slide time")
//RES_ADD_STRING(TXT_MMS_INPUT2,"Input")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_DOWNLOADING,"Downloading MMS")
//RES_ADD_STRING(TXT_MMS_SENDING,"Sending")    //replaced by common ID TXT_SENDING
//RES_ADD_STRING(TXT_MMS_INPUT_NUMBER,"<Enter number>")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_MESSAGESPACE,"MMS memory")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_MMSSPACE_TOTAL,"Total")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_SENDING_READRPT,"Sending read report")
#if defined ( PDA_UI_DROPDOWN_WIN ) &&defined( MMS_SUPPORT )
RES_ADD_STRING(TXT_MMS_MEMORYFULL,"MMS Memory will be full")
#endif
//RES_ADD_STRING(TXT_MMS_NEWANDFULL,"New MMS but memory not enough")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_INSERT_RING,"My Ring")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_INSERT_MP3,"MP3")//case 3:defined but not used
RES_ADD_STRING(TXT_CMS_MMS_ADD_TO,"Add addressee")
//RES_ADD_STRING(TXT_CMS_MMS_ADD_GROUP,"Add group")//case 3:defined but not used
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
RES_ADD_STRING(TXT_CMS_MMS_SIGN,"Sign")
#endif
//RES_ADD_STRING(TXT_CMS_MMS_START_SETTING,"Start setting")//case 3:defined but not used
//RES_ADD_STRING(TXT_CMS_MMS_VEDIO,"Vedio")//case 3:defined but not used
//RES_ADD_STRING(TXT_CMS_MMS_INPUT_BOX,"MMS input box")//case 3:defined but not used
//RES_ADD_STRING(TXT_CMS_MMS_SMS_INPUT_BOX,"SMS input box")//case 3:defined but not used
//RES_ADD_STRING(TXT_CMS_MMS_MP3_BIG,"MP3 is too big")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ARRIVED_FIRST_PAGE,"Arrived first page")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ARRIVED_LAST_PAGE,"Arrived last page")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SETTING_USERNAME,"Username")  //replaced by common ID TXT_COMMON_USERNAME
//RES_ADD_STRING(TXT_MMS_SETTING_PASSWORD,"Password")  //replaced by common ID TXT_COMMON_PASSWORD
//RES_ADD_STRING(TXT_MMS_SETTING_APN,"Apn")  //replaced by common ID TXT_COMMON_APN_COMN
//RES_ADD_STRING(TXT_MMS_PROMPT_FAIL,"Fail")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_ANSWER_CALL,"Answer call")
//RES_ADD_STRING(TXT_MMS_CALL_SENDER_EMPTY,"Sender empty, uable to call!")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_CONFIG_INFO,"Config Info")
//RES_ADD_STRING(TXT_MMS_YDMW,"Homepage")  //replaced by common ID TXT_COMMON_HOMEPAGE
RES_ADD_STRING(TXT_MMS_RECEIVE_CONFIG_PUSH,"Receive config push")
//RES_ADD_STRING(TXT_MMS_NO_INFO,"No info")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SET_CONFIG,"Config")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_HOUR,"hours")
RES_ADD_STRING(TXT_MMS_DETAIL_MESSAGEID,"MessageID:")
RES_ADD_STRING(TXT_MMS_READREPORT_TIME,"Read time:")
//RES_ADD_STRING(TXT_MMS_READREPORT_SENTTIME,"Send time:")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_EXPIRED,"MMS Expired")
//RES_ADD_STRING(TXT_MMS_TOO_BIG,"MMS Too Large")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_EXTRACT_STADDR,"Extract Streaming Address")    //replaced by common ID TXT_EXTRACT_STADDR
//RES_ADD_STRING(TXT_MMS_CONNECT_STADDR,"Connect")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NO_STADDR,"No Streaming Address!")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SET_SMIL,"SMIL Setting")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SETTING_GRAPHABOVE,"Graph Above") //case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_SETTING_TEXTABOVE,"Text Above")//case 3:defined but not used
//RES_ADD_STRING(TXT_CMS_MMS_ADD_CC,"Add cc Address")//case 3:defined but not used
//RES_ADD_STRING(TXT_CMS_MMS_ADD_BCC,"Add bcc Address")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_DELETE_CC,"Delete cc Address")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_DELETE_BCC,"Delete Bcc Address")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_MODIFY_CC,"Modify CC Address")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_EDIT_MODIFY_BCC,"Modify Bcc Address")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_HOURS,"hours")
RES_ADD_STRING(TXT_MMS_REACH_MAXSIZE_LIMIT,"Reach max size of mms limit")
//RES_ADD_STRING(TXT_MMS_SAVE_MEDIA,"Save media files")  //replaced by common ID TXT_COMM_SAVE_MEDIA_FILE
//RES_ADD_STRING(TXT_MMS_INSERT,"Insert")  //replaced by common ID STXT_ADD
//RES_ADD_STRING(TXT_MMS_NO_UDISK_TO_RECEIVE_MMS,"MMS receive fail, please check you storage card")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_UDISK_USING_TO_RECEIVE_MMS,"MMS receive fail in udisk using")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_CANNOT_OPMMS_IN_CALL,"can not operation mms in call")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_IMAGE_ERROR,"Image data error!")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NO_MEM,"TXT_MMS_NO_MEM!")  //replaced by common ID TXT_COMMON_NO_MEMORY
//RES_ADD_STRING(TXT_MMS_NO_MEM_WILLCLOSE,"no memory,mms will close!")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_NOT_ENOUGH_SPACE_NO_SAVE,"Space is full,The MMS is not save!")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_USER1,"sim1 mms count")
RES_ADD_STRING(TXT_MMS_USER2,"sim2 mms count")
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
RES_ADD_STRING(TXT_MMS_USER3,"sim3 mms count")
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
RES_ADD_STRING(TXT_MMS_USER4,"sim4 mms count")
#endif
#ifdef MMI_MULTI_SIM_SYS_SINGLE
RES_ADD_STRING(TXT_MMS_USER,"mms count")
#endif
RES_ADD_STRING(TXT_MMS_MMS_MEMORY,"MMS Memory:")
RES_ADD_STRING(TXT_MMS_UNDOWNLOAD_MMS_MEMORY,"Undownload MMS Memory:")
RES_ADD_STRING(TXT_MMS_WAP_PUSH_MEMORY,"WAP Push Memory:")
RES_ADD_STRING(TXT_MMS_OTA_MEMORY,"OTA Memory:")
//RES_ADD_STRING(TXT_MMS_TCARD_FILE_FULL,"T Card File Full")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_IN_CALL,"In Call,MMS Cannot Preview!")
//RES_ADD_STRING(TXT_MMS_PLEASE_EXIT_EDIT,"Please exit MMS edit first!")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_PLEASE_EXIT_PREVIEW,"Please exit MMS preview first!")//case 3:defined but not used

//add by minghu 09-06-10
RES_ADD_STRING(TXT_MMS_SETTING_RECV_OPT,"MMS Seting Receive Option!")
//RES_ADD_STRING(TXT_MMS_SETTTING_SEND_OPT,"MMS Seting Send Option!")   //replaced by common ID TXT_COMM_SEND_OPTION
RES_ADD_STRING(TXT_MMS_RESTORE_DEFAULT,"Restore to default")
//RES_ADD_STRING(TXT_MMS_AOTODOWN_YES,"Active aotodownload")              //replaced by common ID TXT_COMMON_ENABLE_ON
//RES_ADD_STRING(TXT_MMS_AOTODOWN_NO,"Deactive aotodownload")                //replaced by local ID TXT_CLOSE
//RES_ADD_STRING(TXT_MMS_AOTODOWN_TITLE_NAME,"aotodownload title name")  //replaced by common ID TXT_COMMON_AUTO_DOWNLOAD
//RES_ADD_STRING(TXT_MMS_ADVERTISE_YES,"Active advertise")              //replaced by common ID TXT_COMMON_ENABLE_ON
//RES_ADD_STRING(TXT_MMS_ADVERTISE_NO,"Deactive advertise")                    //replaced by local ID TXT_CLOSE
RES_ADD_STRING(TXT_MMS_ADVERTISE_TITLE_NAME,"advertise title name")
//RES_ADD_STRING(TXT_MMS_RICHEDIT_TEXT,"TEXT")    //replaced by common ID TXT_TEXT
//RES_ADD_STRING(TXT_MMS_RICHEDIT_176_TEXT,"TEXT")//case 3:defined but not used
//RES_ADD_STRING(TXT_MMS_RICHEDIT_PIC,"picture")  //replaced by common ID TXT_COMMON_PIC
//RES_ADD_STRING(TXT_MMS_RICHEDIT_VIDEO,"video")    //replaced by common ID TXT_VIDEO
//RES_ADD_STRING(TXT_MMS_RICHEDIT_AODIO,"audio")    //replaced by common ID TXT_AUDIO
RES_ADD_STRING(TXT_MMS_RICHEDIT_MULTI_REPLACE,"replace")
//RES_ADD_STRING(TXT_MMS_RICHEDIT_MULTI_DELETE,"delete")        //replaced by common ID TXT_DELETE
RES_ADD_STRING(TXT_MMS_NEW_MMS,"New MMS")
RES_ADD_STRING(TXT_MMS_MAX_SLIDE,"Too Many Slide")
RES_ADD_STRING(TXT_MMS_INVALID_SIZE,"Too Big Size")
//RES_ADD_STRING(TXT_MMS_NO_MEMORY,"No Memory")  //replaced by common ID TXT_NO_SPACE
//RES_ADD_STRING(TXT_MMS_SAVE_MEDIA_FILE,"Save Media File")  //replaced by common ID TXT_COMM_SAVE_MEDIA_FILE
RES_ADD_STRING(TXT_MMS_SEND_ARRIVE_REPORT,"Arrive Report")
#ifdef MMI_GUI_STYLE_TYPICAL
RES_ADD_STRING(TXT_MMS_EXIT_DOWNLOAD,"Exit Downlad MMS")
#endif
#ifdef DRM_SUPPORT
RES_ADD_STRING(TXT_MMS_DRM_SAVE,"Please save and view it")
#endif
//RES_ADD_STRING(TXT_MMS_IF_EXIT_DOWNLOAD,"if Exit Fownload MMS")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_FULLSPACE,"mms full space")
RES_ADD_STRING(TXT_MMS_FULL_NUMBER,"mms full number")
RES_ADD_STRING(TXT_MMS_RECV_ERROR,"mms notifycation error")
RES_ADD_STRING(TXT_MMS_SETTING_RE_DEFAULT,"Restore to default")
RES_ADD_STRING(TXT_MMS_MULTIFILE_TOO_BIG,"The file is big than 300k,cann't insert!")
//RES_ADD_STRING(TXT_MMS_SEND_NOW_SIZE,"Now Send Size")    //replaced by common ID TXT_SENT
RES_ADD_STRING(TXT_MMS_RECV_NOW_SIZE,"Now Receive Size")
RES_ADD_STRING(TXT_MMS_TOTAL_SIZE,"Total Size")


//RES_ADD_STRING(TXT_MMS_DOWNLOAD_HIDE_WIN,"Hide")    //replaced by common ID TXT_HIDE
RES_ADD_STRING(TXT_MMS_SAVE_FILE_POSITION,"Save On Phone")
//RES_ADD_STRING(TXT_MMS_SAVE_UDISK,"Save On Phone")          //replaced by common ID TXT_COMMON_UDISK
//RES_ADD_STRING(TXT_MMS_SAVE_SD,"Save On Phone")  //replaced by common ID TXT_COMMON_SD_CARD
RES_ADD_STRING(TXT_MMS_TEXT_POS,"Text Position")
RES_ADD_STRING(TXT_MMS_TEXT_POS_UP,"Text Up")
RES_ADD_STRING(TXT_MMS_TEXT_POS_DOWN,"Text Down")

RES_ADD_STRING(TXT_MMS_NEW_MMS_PUSH,"new MMS notification")
RES_ADD_STRING(TXT_MMS_NEW_MMS_PUSHES,"new MMS notifications")

RES_ADD_STRING(TXT_MMS_TEXT_SERVICE_DENIED,"User Is Not Exist")
//RES_ADD_STRING(TXT_MMS_TEXT_NETLINK_FAILED,"Link Failed")        //replaced by common ID TXT_COMMON_LINK_FAILED
RES_ADD_STRING(TXT_MMS_TEXT_FORMAT_CORRUPT,"Format Corrupt")
RES_ADD_STRING(TXT_MMS_TEXT_ADDRESS_UNRESOLVED,"Address Unresolved")
RES_ADD_STRING(TXT_MMS_TEXT_MESSAGE_NOT_FOUND,"Message Not Found")
//RES_ADD_STRING(TXT_MMS_TEXT_SERVER_BUSY,"Server Busy")                      //replaced by common ID TXT_COMMON_TEXT_SERVER_BUSY
RES_ADD_STRING(TXT_MMS_TEXT_NOT_ACCEPTED,"Unaccepted")
RES_ADD_STRING(TXT_MMS_TEXT_UNSUPORTED_MESSAGE,"Unsuported Message")

//RES_ADD_STRING(TXT_MMS_TEXT_BOX_DETAIL,"Detail")    //replaced by common ID STXT_DETAIL
//RES_ADD_STRING(TXT_MMS_TEXT_SIZE_BIG_FIFTY,"More Than 50k")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_TEXT_PREPARE_DOWNLOAD,"Prepare Download")
//RES_ADD_STRING(TXT_MMS_TEXT_LINK_LIST,"Link List")//case 3:defined but not used

RES_ADD_STRING(TXT_MMS_EDIT_WIN_IN_USED,"Edit Used")

//RES_ADD_STRING(TXT_MMS_PREVIEW_NUMBER_SEND_SMS,"Send SMS")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_PREVIEW_NUMBER_SEND_MMS,"Send MMS")
//RES_ADD_STRING(TXT_MMS_PREVIEW_URL_ENTER,"Enter")       //replaced by common ID TXT_BROWSE
//RES_ADD_STRING(TXT_MMS_PREVIEW_URL_ADD_TO_BOOKMARK,"Add To Bookmark")    //replaced by common ID TXT_ADDTO_BOOKMARK
//RES_ADD_STRING(TXT_MMS_PREVIEW_NUMBER,"Add To Bookmark")  //replaced by common ID TXT_COMMON_NUMBER
//RES_ADD_STRING(TXT_MMS_PREVIEW_URL,"Add To Bookmark")  //replaced by common ID TXT_URL_ADRESS
//RES_ADD_STRING(TXT_MMS_PREVIEW_EMAIL,"Email")    //replaced by common ID TXT_EMAIL_BOX
//RES_ADD_STRING(TXT_MMS_PREVIEW_EXTRACT_OPEN_OR_CLOSE,"Extract Open Or Close")    //replaced by common ID TXT_COMMON_CONTENT_EXTRACT
//RES_ADD_STRING(TXT_MMS_PREVIEW_CALL_SENDER,"Call Sender")   // replaced by common ID TXT_CALL_SENDER
#ifdef STREAMING_HS_SUPPORT // chenxiang 20100223
//RES_ADD_STRING(TXT_MMS_PREVIEW_RTSP,"Play")//case 3:defined but not used
#endif // STREAMING_HS_SUPPORT

//RES_ADD_STRING(TXT_MMS_SEND_RETRY,"Send Retry")
//RES_ADD_STRING(TXT_MMS_RECV_RETRY,"Receive Retry")

RES_ADD_STRING(TXT_MMS_WEB_IN_USING,"Web In Using")

RES_ADD_STRING(TXT_MMS_NOW_PLAYING,"MMS Playing")
RES_ADD_STRING(TXT_MMS_NOW_PREVIEWING,"MMS Previewing")

RES_ADD_STRING(TXT_MMS_ATTACHMENT,"Attachment")
RES_ADD_STRING(TXT_MMS_ATTACHMENT_TYPE,"Attachment type")
//RES_ADD_STRING(TXT_MMS_ATTACHMENT_VCARD,"Vcard")        //replaced by common ID TXT_COMMON_VCARD
//RES_ADD_STRING(TXT_MMS_ATTACHMENT_CALENDAR,"Calendar")//case 3:defined but not used
RES_ADD_STRING(TXT_MMS_ATTACHMENT_OTHER,"Other")
RES_ADD_STRING(TXT_MMS_PUSH_SMSC,"SMS Center")

RES_ADD_STRING(TXT_MMS_IF_ADJUST_PIC,"SMS Center")

RES_ADD_STRING(TXT_OTA_SI_CREATED_TIME,"Created time")
RES_ADD_STRING(TXT_OTA_SI_EXPIRED_TIME,"Expired time")

//RES_ADD_STRING(TXT_MMS_SAVE_MULTIFILE_TO,"Save multifile to")  //replaced by common ID TXT_COMM_FILE_SAVE_TO
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
//RES_ADD_STRING(TXT_MMS_CHARACTER_FONT,"Character font")  //replaced by common ID TXT_COMMON_FONT_SIZE
//RES_ADD_STRING(TXT_MMS_CHARACTER_FONT_BIG,"Big")  //replaced by common ID TXT_COMM_FONT_BIG
//RES_ADD_STRING(TXT_MMS_CHARACTER_FONT_NOMAL,"Nomal")  //replaced by common ID TXT_COMMON_NORMAL
//RES_ADD_STRING(TXT_MMS_CHARACTER_FONT_SMALL,"Small")    //replaced by common ID TXT_FONT_SMALL
#endif
//207256
RES_ADD_STRING(TXT_MMS_FILE_NAME_ERROR,"File name error")
//×ª·¢½ûÖ¹:
RES_ADD_STRING(TXT_DRM_FORBIDDEN_FORWARD, "Forward forbidden:")
//for cr228554
RES_ADD_STRING(TXT_MMS_SAVE_TO_DRAFTBOX, "Save to draft box")
//]]define string infomation
#ifdef MMIMMS_SMS_IN_1_SUPPORT
//RES_ADD_STRING(TXT_MMS_SMS_NEW,"New Message")    //replaced by common ID TXT_NEW
RES_ADD_STRING(TXT_MMS_SMS_TEMPLATE,"Templates")
//RES_ADD_STRING(TXT_MMS_SMS_INSERT,"Insert")  //replaced by common ID TXT_COMM_INSERT
RES_ADD_STRING(TXT_MMS_SMS_ADD_THEME,"Add theme")
RES_ADD_STRING(TXT_MMS_SMS_REMOVE_THEME,"Remove theme")
//RES_ADD_STRING(TXT_MMS_SMS_CARBON_COPY,"CC/BCC")  //replaced by common ID TXT_COMM_CC_BCC
RES_ADD_STRING(TXT_MMS_SMS_ADD_CC_COPY,"ADD CC")
RES_ADD_STRING(TXT_MMS_SMS_ADD_BCC_COPY,"ADD BCC")
RES_ADD_STRING(TXT_MMS_SMS_REMOVE_CC_COPY,"Remove CC")
RES_ADD_STRING(TXT_MMS_SMS_REMOVE_BCC_COPY,"Remove BCC")
RES_ADD_STRING(TXT_MMS_SMS_TO_MMS,"Convert to MMS")
RES_ADD_STRING(TXT_MMS_MMS_TO_SMS,"Convert to SMS")
RES_ADD_STRING(TXT_MMS_SMS_DELETE_INSERT_FILE,"Delete Insert")
RES_ADD_STRING(TXT_MMS_REPLACE_PIC,"Replace Image")
RES_ADD_STRING(TXT_MMS_DELETE_PIC,"Remove Image")
RES_ADD_STRING(TXT_MMS_REPLACE_MUSIC,"Replace music")
RES_ADD_STRING(TXT_MMS_DELETE_MUSIC,"Remove music")
RES_ADD_STRING(TXT_MMS_REPLACE_VIDEO,"Replace video")
RES_ADD_STRING(TXT_MMS_DELETE_VIDEO,"Remove video")
RES_ADD_STRING(TXT_MMS_REPLACE_ATTACH,"Replace attachment")
RES_ADD_STRING(TXT_MMS_DELETE_ATTACH,"Remove attachment")
RES_ADD_STRING(TXT_MMS_REMOVE_INVALID_NUMBER,"Discard invalid number")
RES_ADD_STRING(TXT_MMS_TOUCH_TO_INPUT,"Click on to input")
RES_ADD_STRING(TXT_MMS_CC,"CC:")
RES_ADD_STRING(TXT_MMS_BCC,"BCC:")
RES_ADD_STRING(TXT_MMS_CONTENT_EMPTY,"Message content is empty")
#endif
RES_ADD_STRING(TXT_MMS_SMS_TAKE_PIC,"Photograph")
RES_ADD_STRING(TXT_MMS_SMS_TAKE_VIDEO,"Take a video")
#ifdef MMI_RECORD_SUPPORT
RES_ADD_STRING(TXT_MMS_SMS_TAKE_AUDIO,"Record")
#endif
#endif
//added by feng.xiao for MS00207703 20120119
#ifdef DATA_ROAMING_SUPPORT
RES_ADD_STRING(TXT_MMS_SET_RETRIEVE_LOCAL, "Local retrieve mode")
RES_ADD_STRING(TXT_MMS_SET_RETRIEVE_ROAM, "Roam retrieve mode")
RES_ADD_STRING(TXT_MMS_SET_RETRIEVE_MODE_IMMEDIATE, "Retrieve immediate")
//RES_ADD_STRING(TXT_MMS_SET_RETRIEVE_MODE_DEFER, "Retrieve defer")
RES_ADD_STRING(TXT_MMS_SET_RETRIEVE_MODE_DENY, "Retrieve deny")
//RES_ADD_STRING(TXT_MMS_DATA_ROAMING_DISABLED, "MMS Data roaming disabled")//case 3:defined but not used
#endif
//[[define data infomation
//]]define data infomation
