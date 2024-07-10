#ifndef _WATCH_NOTIFYCENTER_POSITION_H_
#define _WATCH_NOTIFYCENTER_POSITION_H_
//****************************************Shortcut***********************************//
#define SHORCUTWIN_BG_RECT                DP2PX_RECT(0,0,239,239)
#define SHORCUTWIN_DATE_RECT              DP2PX_RECT(66,0,173,47)
#define SHORCUTWIN_NETWORK_AND_WEEK_RECT  DP2PX_RECT(24,48,215,67)
#define SHORCUTWIN_SHORTCUT_RECT          DP2PX_RECT(0,111,239,207)

#define SHORTCUT_1ROW_MARGIN              DP2PX_VALUE(16)
#define SHORTCUT_2ROW_MARGIN              DP2PX_VALUE(44)
#define SHORTCUT_SPLIT_WIDTH              DP2PX_VALUE(16)
#define SHORTCUT_ICON_WIDTH               DP2PX_VALUE(40)
#define SHORTCUT_ICON_HEIGHT              DP2PX_VALUE(40)
#define SHORTCUT_1ROW_MAX_ICONNUM        (4)
#define SHORTCUT_2ROW_MAX_ICONNUM        (3)

#define SHORTCUTWIN_HEIGHT                DP2PX_VALUE(240)
#define SHORTCUTWIN_WIDTH                 DP2PX_VALUE(240)

#define SHORTCUTWIN_ROIREGION DP2PX_RECT(0,208,239,239)


#define SHORTCUTWIN_INIT_POS (-SHORTCUTWIN_HEIGHT)
#define SHORTCUTWIN_STOP_POS DP2PX_VALUE(0)
#define SHORTCUTWIN_END_POS  (-SHORTCUTWIN_HEIGHT)

//****************************************Notifylist***********************************//
#define NOTIFYLISTWIN_BG_RECT                       DP2PX_RECT(0,0,239,239)
#define NOTIFYLISTWIN_MAINSCREEN_HEIGHT             DP2PX_VALUE(240)
#define NOTIFYLISTWIN_TITLE_HEIGHT                  DP2PX_VALUE(48)
#define NOTIFYLISTWIN_LEFT_MARGIN                   DP2PX_VALUE(0)
#define NOTIFYLISTWIN_TITLE_RECT                    DP2PX_RECT(36,0,204,48)
#define NOTIFYLISTWIN_LABEL_RECT                    DP2PX_RECT(85,176,155,224)
#define NOTIFYLISTWIN_ROIREGION                     DP2PX_RECT(0,208,239,239)
#define NOTIFYLISTWIN_IMAGE_RECT                    DP2PX_RECT(66,60,174,168)
#define NOTIFYLISTWIN_IMAGE_SOFTKEY_REC             DP2PX_RECT(36,168,204,216)
#define NOTIFYLISTWIN_LIST_HEIGHT                   DP2PX_VALUE(60)
#define NOTIFYLISTWIN_INIT_POS                      (SHORTCUTWIN_HEIGHT)
#define NOTIFYLISTWIN_STOP_POS                      DP2PX_VALUE(0)
#define NOTIFYLISTWIN_END_POS                       (SHORTCUTWIN_HEIGHT)
#define NOTIFYLISTWIN_ROIREGION                     DP2PX_RECT(0,0,239,30)
#define NOTIFYDETAILWIN_IMAGE_RECT                  DP2PX_RECT(102,24,138,60)
#define NOTIFY_TEXT_LEFT_MARGIN                     DP2PX_VALUE(36)
#define NOTIFY_STATUSBAR_HEIGHT                     DP2PX_VALUE(24)
#define NOTIFY_NUMBER_HEIGHT                        DP2PX_VALUE(18)
#define NOTIFY_TEXT_MARGIN                          DP2PX_VALUE(20)
#define NOTIFY_APPNAME_AND_TIME_RECT                DP2PX_RECT(0,72,239,90)
#endif

