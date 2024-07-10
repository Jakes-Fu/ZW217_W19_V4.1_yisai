
//[[define image infomation
//RES_ADD_IMG(IMAGE_PHONEBOOK_COMPANY_NUMBER,"\\MMI_RES_##\\IMAG\\IMAGE_PHONEBOOK_COMPANY_NUMBER.png",IMG_CMP_PNG_ARGB_RLE,0)
//]]define image infomation

//[[define anim infomation
//]]define anim infomation

//[[define string infomation
#ifdef PIM_SUPPORT  //Added by wangxiaolin  2009.7.6  
RES_ADD_STRING(TXT_PIM,"PIM")
RES_ADD_STRING(TXT_PIM_SYNC_ENTER,"Sync enter")
RES_ADD_STRING(TXT_PIM_SERVICE_INDEX,"Service index")
RES_ADD_STRING(TXT_PIM_PAY_INDEX,"Pay index")
RES_ADD_STRING(TXT_PIM_SYNC_REGISTER_OR_CANCEL,"Register/Cancel register")
RES_ADD_STRING(TXT_PIM_SYNC_START,"Sync start")
RES_ADD_STRING(TXT_PIM_SYNC_NET_QUERY,"Net query")
//RES_ADD_STRING(TXT_PIM_SYNC_SETTING,"Sync setting")      //replaced by common ID STXT_MAIN_SETTING
RES_ADD_STRING(TXT_PIM_SET_SERVER,"Set server")
//RES_ADD_STRING(TXT_PIM_USER_NAME,"User name")  //replaced by common ID TXT_COMMON_USERNAME
//RES_ADD_STRING(TXT_PIM_PASSWORD,"Password")  //replaced by common ID TXT_COMMON_PASSWORD
//RES_ADD_STRING(TXT_PIM_PORT,"Port")  //replaced by common ID TXT_COMM_PORT
RES_ADD_STRING(TXT_PIM_SERVER_ADDR,"Server address")
//RES_ADD_STRING(TXT_PIM_SYNC_CODEC,"Codec type")  //replaced by common ID TXT_CODING_TYPE
RES_ADD_STRING(TXT_PIM_SYNC_AUTH,"Auth type")
RES_ADD_STRING(TXT_PIM_NET_SETTING,"Net setting")
RES_ADD_STRING(TXT_PIM_APN,"APN")
RES_ADD_STRING(TXT_PIM_GATEWAY,"Gateway setting")
RES_ADD_STRING(TXT_PIM_IP,"IP")
RES_ADD_STRING(TXT_PIM_PROXY,"Proxy")
//RES_ADD_STRING(TXT_PIM_PROXY_ON,"On")    //replaced by common ID TXT_PROXY_ON
//RES_ADD_STRING(TXT_PIM_PROXY_OFF,"Off") // replaced by common ID TXT_COMM_PROXY_OFF
RES_ADD_STRING(TXT_PIM_SYNCML_SETTING,"SyncML setting")
RES_ADD_STRING(TXT_PIM_CONTACT,"Contact")
RES_ADD_STRING(TXT_PIM_CALENDAR,"Calendar")
RES_ADD_STRING(TXT_PIM_WAP_URL,"Wap url")
//RES_ADD_STRING(TXT_PIM_RESET_SETTING,"Reset setting")  //replaced by common ID TXT_RESET_DEFAULT
RES_ADD_STRING(TXT_PIM_CODE_XML,"XML")
RES_ADD_STRING(TXT_PIM_CODE_WBXML,"WBXML")
RES_ADD_STRING(TXT_PIM_AUTH_B64,"Basic")
RES_ADD_STRING(TXT_PIM_AUTH_MD5,"MD5")
//RES_ADD_STRING(TXT_PIM_CONNECTTING,"Connectting")    //replaced by common ID TXT_CONNECTING
RES_ADD_STRING(TXT_PIM_INITIALIZE,"Initializing")
RES_ADD_STRING(TXT_PIM_SEND_DATA,"Sendding data")
RES_ADD_STRING(TXT_PIM_RECEIVE_DATA,"Receiving data")
RES_ADD_STRING(TXT_PIM_SYNC_SUCCESS,"Sync success")
RES_ADD_STRING(TXT_PIM_SYNC_FAIL,"Sync fail")
//RES_ADD_STRING(TXT_PIM_CANCEL,"Cancel")  //replaced by common ID STXT_CANCEL
RES_ADD_STRING(TXT_PIM_SYNC,"Synchronization")
RES_ADD_STRING(TXT_PIM_SERVER_REPLACED,"Sserver replaced:")
RES_ADD_STRING(TXT_PIM_SERVER_ADDED,"Server added:")
RES_ADD_STRING(TXT_PIM_SERVER_DELETED,"Server deleted:")
RES_ADD_STRING(TXT_PIM_PHONE_ADDED,"Phone added:")
RES_ADD_STRING(TXT_PIM_PHONE_REPLACED,"Phone replaced:")
RES_ADD_STRING(TXT_PIM_PHONE_DELETED,"Phone deleted:")
RES_ADD_STRING(TXT_PIM_ITEM,"Item")	 
RES_ADD_STRING(TXT_PIM_PDP_ACTIVE_FAIL,"PDP_active_fail")
RES_ADD_STRING(TXT_PIM_COMMUNICATION_ERROR,"Communication error")
///RES_ADD_STRING(TXT_PIM_NET_ERROR,"Net error")  //replaced by common ID TXT_COMMON_NET_ERROR
RES_ADD_STRING(TXT_PIM_NO_NET,"Net error")
RES_ADD_STRING(TXT_PIM_AUTH_FAIL,"Auth fail")
//RES_ADD_STRING(TXT_PIM_BAD_REQUEST,"Bad request")  //replaced by common ID TXT_COMM_BAD_REQUEST
//RES_ADD_STRING(TXT_PIM_SERVER_IS_BUSY,"Server is busy，try it later")  //replaced by local common ID TXT_PIM_FAIL_503
RES_ADD_STRING(TXT_PIM_TIME_OUT,"Time out ")
RES_ADD_STRING(TXT_PIM_SYNC_ERROR,"Sync error")
//RES_ADD_STRING(TXT_PIM_SERVER_ERROR,"Server error") //replaced by common ID TXT_COMM_SERVER_ERROR
RES_ADD_STRING(TXT_PIM_FAIL_403,"Please try later")
//RES_ADD_STRING(TXT_PIM_FAIL_407,"Please input username & password")     //replaced by common ID TXT_COMMON_INPUT_USERNAME_PASSWORD
RES_ADD_STRING(TXT_PIM_FAIL_408,"Please try later")
RES_ADD_STRING(TXT_PIM_FAIL_420,"Space is not enough")
RES_ADD_STRING(TXT_PIM_FAIL_503,"The server is busy")
RES_ADD_STRING(TXT_PIM_FAIL_510,"Please check the setting of database")
RES_ADD_STRING(TXT_PIM_FAIL_513,"Nonsupport")
RES_ADD_STRING(TXT_PIM_UA_SETTING,"UA setting")
RES_ADD_STRING(TXT_PIM_DEBUG_SERVER_SETTING,"Server setting")
RES_ADD_STRING(TXT_PIM_DELETE_ANCHOR,"Delete anchor")
RES_ADD_STRING(TXT_PIM_SERVER1,"Monternet")
RES_ADD_STRING(TXT_PIM_SERVER2,"218.206.176.241")
RES_ADD_STRING(TXT_PIM_UA_HS_N51,"HS-N51")
RES_ADD_STRING(TXT_PIM_UA_HS_T66,"HS-T66")
RES_ADD_STRING(TXT_PIM_UA_HS_N71,"HS-N71")
RES_ADD_STRING(TXT_PIM_DEBUG_SETTING,"PIM debug setting")
RES_ADD_STRING(TXT_PIM_UPLOAD,"Upload:")
RES_ADD_STRING(TXT_PIM_DOWNLOAD,"Download:")
RES_ADD_STRING(TXT_PIM_STR_SYNC_TYPE_1WFCLI   ,   "One way sync from client")  
RES_ADD_STRING(TXT_PIM_STR_SYNC_TYPE_RFCLI    ,   "Refresh sync from client")  
RES_ADD_STRING(TXT_PIM_STR_SYNC_TYPE_1WFSVR   ,   "One way sync from server")  
RES_ADD_STRING(TXT_PIM_STR_SYNC_TYPE_RFSVR    ,   "Refresh sync from server")  
RES_ADD_STRING(TXT_PIM_STR_SEND_DATA_FAIL     ,   "Send data fail")  
RES_ADD_STRING(TXT_PIM_STR_RECEIVE_DATA_FAIL  ,   "Receive data fail")  
RES_ADD_STRING(TXT_PIM_PHONEBOOK              ,   "Phonebook")  
RES_ADD_STRING(TXT_PIM_STR_WAP_ADDR           ,   "Wap address")  
RES_ADD_STRING(TXT_PIM_STR_WAP_KT_ADDR        ,   "Subscribe/Cancel services addr  ")  
RES_ADD_STRING(TXT_PIM_STR_WAP_CX_ADDR        ,   "Online enquiry addr")  
RES_ADD_STRING(TXT_PIM_STR_WAP_ZF_ADDR        ,   "Charge query addr")  
RES_ADD_STRING(TXT_PIM_ERR_SVR_FAIL           ,   "Server fail")  
RES_ADD_STRING(TXT_PIM_ERR_SYNC_TYPE          ,   " Sync type nonsupport")  
RES_ADD_STRING(TXT_PIM_ERR_CRET_TYPE          ,   " Auth type nonsupport")  
//RES_ADD_STRING(TXT_PIM_ERR_SVR_BUSY           ,   " Server busy")              //replaced by common ID TXT_COMMON_TEXT_SERVER_BUSY
RES_ADD_STRING(TXT_PIM_ERR_SVR_FULL           ,   " Server full")  
RES_ADD_STRING(TXT_PIM_ERR_CLT_FULL           ,   " Clent full")  
RES_ADD_STRING(TXT_PIM_ERR_SVR_DB_FAIL        ,   " Database operate fail")  
RES_ADD_STRING(TXT_PIM_ERR_CRED_MISS          ,   " Credit miss")  
RES_ADD_STRING(TXT_PIM_ERR_CRED_INVALID       ,   " Credit invalid")  
RES_ADD_STRING(TXT_PIM_ERR_ACC_IN_USE         ,   " Account in use")  
RES_ADD_STRING(TXT_PIM_ERR_CMD_INVALID        ,   " Command invalid")  
RES_ADD_STRING(TXT_PIM_ERR_ITEM_TOO_LARGE     ,   " Item too larg")  
RES_ADD_STRING(TXT_PIM_ERR_SVR_INNER_ERR      ,   " Server inner error")  
RES_ADD_STRING(TXT_PIM_ERR_SVR_PROC_FAIL      ,   " Server proc fali")  
RES_ADD_STRING(TXT_PIM_ERR_VER_NOT_SUPP       ,   " Version not support")  
RES_ADD_STRING(TXT_PIM_ERR_SVR_URI_ERR        ,   " Server URI error")  
RES_ADD_STRING(TXT_PIM_ERR_DB_URI_ERR         ,   " Database URI error")  
//RES_ADD_STRING(TXT_PIM_ERR_CONN_FAIL          ,   " Connect fail")     //replaced by common ID TXT_COMMON_LINK_FAILED       
RES_ADD_STRING(TXT_PIM_ERR_CONN_CLOSED        ,   " Connect closed")  
RES_ADD_STRING(TXT_PIM_ERR_NOT_FOUND          ,   " Not found")  
RES_ADD_STRING(TXT_PIM_ERR_NET_FAIL           ,   " Net operation fail")  
//RES_ADD_STRING(TXT_PIM_ERR_USER_CANCEL        ,   " User cancel")    //replaced by common ID TXT_USER_CANCEL  
RES_ADD_STRING(TXT_PIM_ERR_TIMEOUT            ,   " Time out")  
RES_ADD_STRING(TXT_PIM_ERR_PARAM              ,   " Param set error")  
RES_ADD_STRING(TXT_PIM_STR_ERR_SYS_ERROR      ,   " System error")   
//RES_ADD_STRING(TXT_PIM_STR_ERR_UNKNOW         ,   "Error unknow")    //replaced by common ID TXT_UNKNOWN_ERROR   
RES_ADD_STRING(TXT_PIM_SYNC_TYPE,"Synctype")
RES_ADD_STRING(TXT_PIM_STR_SYNC_TYPE_2WAY   ,   "Two way sync")  
RES_ADD_STRING(TXT_PIM_STR_SYNC_TYPE_S2WAY    ,   "Slow two way sync")  
RES_ADD_STRING(TXT_PIM_SYNC_START_VCAL    ,   "Calendar update")  
//RES_ADD_STRING(TXT_PIM_MEMORY_FULL,"Memory full")  //replaced by common ID TXT_COMM_MEMORY_FULL

RES_ADD_STRING(TXT_PIM_CONFIRM_EXIT,"Confirm exit?")
RES_ADD_STRING(TXT_PIM_RESTORE_DEFAULT,"Restore to default?")
//RES_ADD_STRING(TXT_PIM_ITEMS,"items")    //replaced by common ID TXT_ITEMS
RES_ADD_STRING(TXT_PIM_PHONE_TO_SERVER,"Phone to server:")
RES_ADD_STRING(TXT_PIM_SERVER_TO_PHONE,"Server to phone:")
//实际内容较长，在exl表格中添加
RES_ADD_STRING(TXT_PIM_SERVICE_INTRODUCE_CONTENT    ,   "Pim service introduce")  
RES_ADD_STRING(TXT_PIM_UA_DEFAULT,"Default")
RES_ADD_STRING(TXT_PIM_UA_OTHER,"Other")
RES_ADD_STRING(TXT_PIM_UA_CUSTOM,"Custom")
RES_ADD_STRING(TXT_PIM_CUSTOM_MAN,"MAN")
RES_ADD_STRING(TXT_PIM_CUSTOM_OEM,"OEM")
RES_ADD_STRING(TXT_PIM_CUSTOM_MOD,"MOD")
RES_ADD_STRING(TXT_PIM_CUSTOM_UA,"UA")
//MS00216980 cheney
RES_ADD_STRING(TXT_PIM_SERVER_ADD,"pim.monternet.com")
RES_ADD_STRING(TXT_PIM_SERVER_ADD_TEST,"218.206.176.241")
#endif
//]]define string infomation

//[[define data infomation
//]]define data infomation
