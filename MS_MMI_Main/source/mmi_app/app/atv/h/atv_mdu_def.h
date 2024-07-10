#ifdef ATV_SUPPORT

//[[define image infomation
#ifdef MMI_PDA_SUPPORT
RES_ADD_IMG(IMAGE_ATV_ADD_TO_FAVORITES,"\\MMI_RES_##\\IMAG\\Optionsmenu_icon\\IMAGE_ATV_ADD_TO_FAVORITES.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ATV_ADJUST_BRIGHTNESS,"\\MMI_RES_##\\IMAG\\Optionsmenu_icon\\IMAGE_ATV_ADJUST_BRIGHTNESS.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_ATV_CHANNEL_LIST,"\\MMI_RES_##\\IMAG\\Optionsmenu_icon\\IMAGE_ATV_CHANNEL_LIST.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
//]]define image infomation

//[[define anim infomation
//]]define anim infomation

//[[define string infomation
#if 1
RES_ADD_STRING(TXT_ATV_ANALOGTV,                        "Analog TV")
//RES_ADD_STRING(TXT_ATV_SNAPSHOT_FILE,                   "Picture ")       //case3£∫defined but not used
//RES_ADD_STRING(TXT_ATV_SNAPSHOT_SAVE_IN_UDISK,          "saved to Udisk Photos dir!")     //case3£∫defined but not used
//RES_ADD_STRING(TXT_ATV_SNAPSHOT_SAVE_IN_SDCARD,         "saved to SD card Photos dir!")    //case3£∫defined but not used
//RES_ADD_STRING(TXT_ATV_NOSPACE,                         "No enough spcce!")       //case3£∫defined but not used
//RES_ADD_STRING(TXT_ATV_ADD_FAVORITE,                    "Favorite")    //replaced by common ID TXT_ADD_FAVORITE
//RES_ADD_STRING(TXT_ATV_PLAY,                            "Play")                  //case3£∫defined but not used
//RES_ADD_STRING(TXT_ATV_SERVICE_LIST,                    "Service list")      //replaced by common ID TXT_COMMON_COMMON_CHANNEL_LIST
RES_ADD_STRING(TXT_ATV_RECORD_LIST,                     "Record list")
//RES_ADD_STRING(TXT_ATV_FAVORITE_SERVICE_LIST,           "My favorite list")    //replaced by common ID TXT_MAINMENU_FAVORITE 
RES_ADD_STRING(TXT_ATV_ADD_FAVORITE_SERVICE_OK,         "Add favorite service success!")
RES_ADD_STRING(TXT_ATV_FAVORITE_SERVICE_FULL,           "favorite list is full!")
RES_ADD_STRING(TXT_ATV_FAVORITE_SERVICE_HAS_EXIST,           "The program is existed!")
//RES_ADD_STRING(TXT_ATV_ADD_FAVORITE_SERVICE,            "Add to favorite")    //replaced by common ID TXT_ADD_FAVORITE_SERVICE
RES_ADD_STRING(TXT_ATV_SEARCH_SERVICE,                  "Search sercice")
//RES_ADD_STRING(TXT_ATV_SEARCHING,                       "Searching...")  //replaced by common ID TXT_COMMON_SEARCHING
RES_ADD_STRING(TXT_ATV_SELECT_REGION,                   "Select country or region")
//RES_ADD_STRING(TXT_ATV_SELECT_REGION_OK,                "Select OK!")     //case3£∫defined but not used
//RES_ADD_STRING(TXT_ATV_BRIGHTNESS,                      "Adjust brightness(5)")    //replaced by common ID TXT_COMMON_COMMON_BRIGHTNESS(5)
RES_ADD_STRING(TXT_ATV_HELP,                            "Help(*)")
RES_ADD_STRING(TXT_ATV_HELP_DETAIL,                     "Help detail")
RES_ADD_STRING(TXT_ATV_HELP_DETAIL_NO_KEY,              "Help detail")
RES_ADD_STRING(TXT_ATV_HELP_DETAIL_NO_RECORD,              "Help detail")
RES_ADD_STRING(TXT_ATV_WEAK_SIGNAL,                     "Weak signal!")
//RES_ADD_STRING(TXT_ATV_EDIT,                            "Edit") //replaced by common ID TXT_EDIT
//RES_ADD_STRING(TXT_ATV_DELETE,                          "Delete")         //replaced by common ID TXT_DELETE
//RES_ADD_STRING(TXT_ATV_DELETE_QUERY,                    "Conform to delete?")    //replaced by common ID TXT_COMMON_CONFIRM_DELETE
//RES_ADD_STRING(TXT_ATV_DELETE_ALL,                      "Delete all")      //replaced by common ID TXT_DELALL
//RES_ADD_STRING(TXT_ATV_DELETE_ALL_QUERY,                "Conform to delete all?")    //replaced by common ID TXT_DELETE_ALL_QUERY
//RES_ADD_STRING(TXT_ATV_DELETE_OK,                       "Delte OK!")       //case3£∫defined but not used
RES_ADD_STRING(TXT_ATV_OPEN_FAIL,                       "Open ATV fail!")
//RES_ADD_STRING(TXT_ATV_PLAY_ERROR,                      "Play error!") //replaced by common ID TXT_COMM_PLAY_ERROR
RES_ADD_STRING(TXT_ATV_SEARCH_ERROR,                    "Search program error!")
RES_ADD_STRING(TXT_ATV_RECORD_DEVICE,                   "Record store device")
RES_ADD_STRING(TXT_ATV_START_RECORD_ERROR,              "Start record error")
RES_ADD_STRING(TXT_ATV_STOP_RECORD_FISRT,               "Stop record please")
RES_ADD_STRING(TXT_ATV_RECORD_SAVED,                    "Saved as")
RES_ADD_STRING(TXT_ATV_RECORD_FILE_TYPE,                "Record file type")


//tv region ∂‘”¶ATV_REGION_E
RES_ADD_STRING(TXT_ATV_REGION_CHINA,                    "China")        
RES_ADD_STRING(TXT_ATV_REGION_HONGKONG,                 "Hongkong")
RES_ADD_STRING(TXT_ATV_REGION_SHENZHEN,                 "Shenzhen")
RES_ADD_STRING(TXT_ATV_REGION_CAMBODIA,                 "Cambodia")
RES_ADD_STRING(TXT_ATV_REGION_CANADA,                   "Canada")
RES_ADD_STRING(TXT_ATV_REGION_CHILE,                    "Chile")
RES_ADD_STRING(TXT_ATV_REGION_AFGHANISTAN,              "Afghanistan")
RES_ADD_STRING(TXT_ATV_REGION_ARGENTINA,                "Argentina")
RES_ADD_STRING(TXT_ATV_REGION_BRAZIL,                   "Brazil")
RES_ADD_STRING(TXT_ATV_REGION_INDONESIA,                "Indonesia")
RES_ADD_STRING(TXT_ATV_REGION_JAPAN,                    "Japan")
RES_ADD_STRING(TXT_ATV_REGION_KOREA,                    "Korea")
RES_ADD_STRING(TXT_ATV_REGION_LAOS,                     "Laos")
RES_ADD_STRING(TXT_ATV_REGION_MALAYSIA,                 "Malaysia")
//RES_ADD_STRING(TXT_ATV_REGION_MEXICO,                   "Mexico")          //replaced by common ID TXT_COMMON_WORLD_MEXICO
RES_ADD_STRING(TXT_ATV_REGION_MYANMAR,                  "Myanmar")
RES_ADD_STRING(TXT_ATV_REGION_PHILIPPINES,              "Philippines")
//RES_ADD_STRING(TXT_ATV_REGION_SINGAPORE,                "Singapore")    //replaced by common ID TXT_SINGAPORE
RES_ADD_STRING(TXT_ATV_REGION_SOUTHAFRICA,              "South Africa")
RES_ADD_STRING(TXT_ATV_REGION_TAIWAN,                   "Taiwan")
RES_ADD_STRING(TXT_ATV_REGION_THAILAND,                 "Thailand")
RES_ADD_STRING(TXT_ATV_REGION_TURKEY,                   "Turkey")
RES_ADD_STRING(TXT_ATV_REGION_UAE,                      "UAE")
RES_ADD_STRING(TXT_ATV_REGION_UK,                       "UK")
RES_ADD_STRING(TXT_ATV_REGION_USA,                      "USA")
RES_ADD_STRING(TXT_ATV_REGION_VENEZUELA,                "Venezuela")
RES_ADD_STRING(TXT_ATV_REGION_VIETNAM,                  "Vietnam")
RES_ADD_STRING(TXT_ATV_REGION_WESTERNEUROPE,            "West Europe")
RES_ADD_STRING(TXT_ATV_REGION_SPAIN,                    "Spain")
RES_ADD_STRING(TXT_ATV_REGION_PORTUGAL,                 "Portugal")
RES_ADD_STRING(TXT_ATV_REGION_PAKISTAN,                 "Pakistan")
RES_ADD_STRING(TXT_ATV_REGION_INDIA,                    "India")
RES_ADD_STRING(TXT_ATV_REGION_AUSTRALIA,                "Australia")
RES_ADD_STRING(TXT_ATV_REGION_PARAGURY,                 "Paraguay")
RES_ADD_STRING(TXT_ATV_REGION_URUGUAY,                  "Uruguay")
RES_ADD_STRING(TXT_ATV_REGION_BULGARIA,                 "Bulgaria")
RES_ADD_STRING(TXT_ATV_REGION_CONGO,                    "Congo")
RES_ADD_STRING(TXT_ATV_REGION_EGYPT,                    "Egypt")
RES_ADD_STRING(TXT_ATV_REGION_GERMANY,                  "Germany")
RES_ADD_STRING(TXT_ATV_REGION_IRAQ,                     "Iraq")
RES_ADD_STRING(TXT_ATV_REGION_IRAN,                     "Iran")
RES_ADD_STRING(TXT_ATV_REGION_HUNGRY,                   "Hungary")
RES_ADD_STRING(TXT_ATV_REGION_LIBYA,                    "Libya")
RES_ADD_STRING(TXT_ATV_REGION_LEBANON,                  "Lebanon")
RES_ADD_STRING(TXT_ATV_REGION_POLAND,                   "Poland")
RES_ADD_STRING(TXT_ATV_REGION_ROMANIA,                  "Romania")
RES_ADD_STRING(TXT_ATV_REGION_SANDIARABIA,              "Saudi Arabia")
RES_ADD_STRING(TXT_ATV_REGION_TUNISIA,                  "Tunisia")
RES_ADD_STRING(TXT_ATV_REGION_RUSSIA,                   "Russia")
#endif
//]]define string infomation

#endif


