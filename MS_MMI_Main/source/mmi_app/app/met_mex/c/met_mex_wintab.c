#include "mmi_app_met_mex_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "window_parse.h"
#include "mmk_app.h" 
#include "guimenu.h"
#include "guilcd.h"
#include "mmi_menutable.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmipub.h"
#include "mmi_resource.h"
#include "guilabel.h"
#include "met_mex_id.h"
#include "met_mex_text.h"
#include "met_mex_image.h"
#include "met_mex_osbridge.h"
#include "lcd_cfg.h"
#include "mex_app_id.h"

#include "MexLib.h"
#include "MexLib_Std.h"
#include "MexLib_Os.h"
#include "MexLib_Gui.h"
#include "MexConfig.h"

#if defined(MEX_PLATFORM__FOR_SPRD6800H)
#include "mmi_default.h"
#include "guidropdownlist.h"
#endif


#ifdef MEX_PLATFORM__FOR_SPRD6800H
#include "guiform.h"
#include "guisetlist.h"
#include "mmialarm_id.h"
#include "guiim_base.h"
#endif
#ifdef FM_SUPPORT
#include "Mmifm_export.h"
#endif
#if (MEX_SPRD_CODE_VERSION >= 0x09AFFFF)
#include "guitext.h"
#else
#include "guitextbox.h"
#endif
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
#include "mmikl_export.h"
#include "mmiset_export.h"
#else
#include "mmi_keylock.h"
#endif
#include "mmi_default.h"
#include "mmk_timer.h"


uint8 MetMex_GetMMITimerId(void);
void MetMex_StartTimer(uint8 timerid, uint32 time_out, MexFuncPtr funcPtr,bool is_period);
void MetMex_StopTimer(uint8 timerid);
void MetMex_SpreadStartupInit(void);
PUBLIC  BOOLEAN MetMex_ConflictPromt(void);

// Display Popup
PUBLIC void MetMex_SetPopupKeyHandler(void* hKeyLeft, void* hKeyRight);
PUBLIC void MetMex_DisplayPopup(const wchar_t *str1, uint16 str1_len, const wchar_t *str2, uint16 str2_len);
PUBLIC void MetMex_DisplayPopupById(MMI_TEXT_ID_T text_id1, MMI_TEXT_ID_T text_id2, MMI_IMAGE_ID_T image_id);

typedef void (*MexEntryFuncPtr) (void);
MexEntryFuncPtr g_mex_entry_func = NULL;
extern void SetMetMexMainInitParam(void);
#if MEX_SCREEN_ESPECIAL_VERSION && defined(MEX_PLATFORM__FOR_SPRD6600L)
extern void MexSystem_LoadSpacialScreenConfig(void);
#endif
extern char* MexSystem_GetSystemVersion(void);
extern char* MexSystem_GetPatchVersion(void);
extern char* MexSystem_GetBulidDate(void);
extern int32 MexConfig_GetAppMemSize(void);
extern void Mex_Key_Listener_Key_Handle(uint16 uiKeyCode, uint16 uiKeyEvent);
extern void MexSystem_Handler_Pen_Down(int32 x, int32 y);
extern void MexSystem_Handler_Pen_Move(int32 x, int32 y);
extern void MexSystem_Handler_Pen_Up(int32 x, int32 y);
extern void MexConfig_SetTinyLoadingPage( bool bShowColorBar, mex_color cBgColor,mex_color cFontColor, uint8 uFontPos, const wchar_t * szLoadStr );
extern void MexConfig_SetManLoadingPage( bool bShowColorBar, mex_color cBgColor, mex_color cFontColor, uint8 uFontPos, const wchar_t * szLoadStr );
extern bool MexSystem_IsSysKernalRunning(void);
extern void MexSystem_SetSystemInitParam( char * pInitParam );
extern void MexConfig_MapSimCard(void);
extern bool MexInterface_EnableLcdRotate(bool flag);
extern int32 MexConfig_GetPhoneConfig(E_Mex_Phone_Config cfg);
extern int8 MexSystem_Init(void);
extern void MexSystem_Shutdown( E_Mex_PlatformShutdown eShutdownMode);	
extern bool MexSystem_IsSysRunning(void);
extern int8 MexConfig_EntrySystemApp(E_MEX_Entry_System_App app, void *pParam);
extern int32 MexConfig_ConvertSprdDef(E_SYS2MEX_Def_Convert param);
extern void MexLib_Os_Init_File_Pool(void);
extern void Mex_SaveScreenConfig(uint16 uWidth,uint16 uHeight);

//记录当前独立入口程序名及Id
char pCurrEntryAppName[200] = { 0 };
int32 iCurrEntryAppID = 0;
#define MEX_MAX_LOAD_STRING_LEN  50
wchar_t g_szManLoadStr[MEX_MAX_LOAD_STRING_LEN]={0};//Manager的loadString
wchar_t g_szTinyLoadStr[MEX_MAX_LOAD_STRING_LEN]={0};//Tiny manager的loadString
bool g_mex_bGotoMainMenu = false;//是否要到main menu画面
bool g_bMexTPLock = false;//mex平台触屏锁控制

bool g_bIsWaitKeyUpEvent = false;
bool g_bIsWaitTPUpEvent = false;

#define AllRow 30
#define AllCol 15
char g_Game_NeedSecondMenu[AllRow][AllCol];//数组用于存放mexsys.cfg 里的需要二级菜单的游戏标识
char g_Game_NeedHiddenMenu[AllRow][AllCol];//数组用于存放mexsys.cfg 里的需要动态二级菜单的游戏标识
int16 iRowTotal_s = 0;//iRowTotal_s存放二维数组g_Game_NeedSecondMenu的列数，也即mexsys.cfg 里游戏名称的总数
int16 iRowTotal_h = 0;//iRowTotal_h存放二维数组g_Game_NeedHiddenMenu的列数，也即mexsys.cfg 里游戏名称的总数
int16 iStartDoubleQQ = 1;//判断开启的是哪个QQ

int16 g_szIndexArray[20] = {0};//存储hid_menu_data表里匹配的索引值

//动态二级菜单结构体
typedef struct
{
	char CmpKeyword[20];
	char AppName[20];
	uint32 AppId;
     //MMI_STRING_T MenuName;
    char charMenuName[20];//char *charMenuName;
}mex_hidden_menu_data;

#define game_count 17

#ifdef MEX_BIGEND
mex_hidden_menu_data hid_menu_data[game_count] = 
{
	{ "tb",		 "MEX_TB.mex",		 E_MSVT_TB, 		"\x62\x11\x72\x31\x9E\xA6\x51\x4B\x98\xCE\x00" },
	{ "aq",		 "AreaInquire.mex",  E_MSVT_AQ, 		"\x53\xF7\x78\x01\x53\x6B\x58\xEB\x00\x0D\x00\x0A\x00" },
	{ "zt",		 "mobile_zt.mex", 	 E_MSVT_ZT, 		"\x5F\x81\x90\x14\x00" },
	{ "fruit",	 "fruit.mex", 		 E_MSVT_FRUITSLICE, "\x52\x07\x6C\x34\x67\x9C\x00"},
	{ "weibo",	 "weibo.mex", 		 E_MSVT_WEIBO, 		"\x65\xB0\x6D\x6A\x5F\xAE\x53\x5A\x00" },	
	{ "weather", "weather.mex", 	 E_MSVT_WEATHER, 	"\x59\x29\x6C\x14\x98\x84\x62\xA5\x00" },
	{ "wochat",	 "mex_wochat.mex",   E_MSVT_WOMIVIDEO, 	"\x6C\x83\x7C\x73\x89\xC6\x98\x91\x00" },
	{ "tom",	 "tom.mex", 		 E_MSVT_TOMCAT, 	"\x00\x74\x00\x6F\x00\x6D\x73\x2B\x00" },
	{ "msn",	 "MSN.mex", 		 E_MSVT_MSN, 		"\x00\x4D\x00\x53\x00\x4E\x00" }, 
	{ "fetion",	 "fetion.mex", 	 	 E_MSVT_FETION, 	"\x98\xDE\x4F\xE1\x00"},
	{ "angrybirds",	"AngryBirds.mex",E_MSVT_FNDXN, 		"\x61\x24\x60\x12\x76\x84\x5C\x0F\x9E\x1F\x00"},
	{ "cqz",	 "cqz.mex", 		 E_MSVT_CQZ, 		"\x54\x39\x88\xD9\x5B\x50\x00" },
	{ "jscb",	 "jscb.mex", 		 E_MSVT_JSCB, 		"\x91\xD1\x5C\x71\x8B\xCD\x97\x38\x00" },
	{ "shsl",	 "shsl.mex",		 E_MSVT_SHSL,		"\x6D\xF1\x6D\x77\x72\xE9\x73\x0E\x00" },
	{ "ddz",	 "ddz.mex",		 	 E_MSVT_DDZ,		"\x4E\x09\x56\xFD\x65\x97\x57\x30\x4E\x3B\x00" },
	{ "zwzjs",	 "zwzjs.mex",		 E_MSVT_ZWDZJS,		"\x69\x0D\x72\x69\x62\x18\x50\xF5\x5C\x38\x00" },
	{ "dzdp",	 "dzdp.mex",		 E_MSVT_DZDP,		"\x59\x27\x4F\x17\x70\xB9\x8B\xC4\x00"}
};
#else
mex_hidden_menu_data hid_menu_data[game_count] = 
{
	{ "tb",		 "MEX_TB.mex",		 E_MSVT_TB, 		"\x11\x62\x31\x72\xA6\x9E\x4B\x51\xCE\x98\x00"},
	{ "aq",		 "AreaInquire.mex",  E_MSVT_AQ, 		"\xF7\x53\x01\x78\x6B\x53\xEB\x58\x00" },
	{ "zt",		 "mobile_zt.mex", 	 E_MSVT_ZT, 		"\x81\x5F\x14\x90\x00" },
	{ "fruit",	 "fruit.mex", 		 E_MSVT_FRUITSLICE, "\x07\x52\x34\x6C\x9C\x67\x00" },
	{ "weibo",	 "weibo.mex", 		 E_MSVT_WEIBO, 		"\xB0\x65\x6A\x6D\xAE\x5F\x5A\x53\x00" },	
	{ "weather", "weather.mex", 	 E_MSVT_WEATHER, 	"\x29\x59\x14\x6C\x84\x98\xA5\x62\x00" },
	{ "wochat",	 "mex_wochat.mex",   E_MSVT_WOMIVIDEO, 	"\x83\x6C\x73\x7C\xC6\x89\x91\x98\x00" },
	{ "tom",	 "tom.mex", 		 E_MSVT_TOMCAT, 	"\x74\x00\x6F\x00\x6D\x00\x2B\x73\x00" },
	{ "msn",	 "MSN.mex", 		 E_MSVT_MSN, 		"\x4D\x00\x53\x00\x4E\x00\x00" }, 
	{ "fetion",	 "fetion.mex", 	 	 E_MSVT_FETION, 	"\xDE\x98\xE1\x4F\x00" },
	{ "angrybirds",	"AngryBirds.mex",E_MSVT_FNDXN, 		"\x24\x61\x12\x60\x84\x76\x0F\x5C\x1F\x9E\x00" },
	{ "cqz",	 "cqz.mex", 		 E_MSVT_CQZ, 		"\x39\x54\xD9\x88\x50\x5B\x00" },
	{ "jscb",	 "jscb.mex", 		 E_MSVT_JSCB, 		"\xD1\x91\x71\x5C\xCD\x8B\x38\x97\x00" },
	{ "shsl",	 "shsl.mex",		 E_MSVT_SHSL,		"\xF1\x6D\x77\x6D\xE9\x72\x0E\x73\x00"  },
	{ "ddz",	 "ddz.mex",		 	 E_MSVT_DDZ,		"\x09\x4E\xFD\x56\x97\x65\x30\x57\x3B\x4E\x00" },
	{ "zwzjs",	 "zwzjs.mex",		 E_MSVT_ZWDZJS,		"\x0D\x69\x69\x72\x18\x62\xF5\x50\x38\x5C\x00" },
	{ "dzdp",	 "dzdp.mex",		 E_MSVT_DZDP,		"\x27\x59\x17\x4F\xB9\x70\xC4\x8B\x00" }
};
#endif
LOCAL const MMI_IMAGE_ID_T g_mexmenu_second_icon[] =
{
    //item icon image id
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_1,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_2,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_3,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_4,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_5,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_6,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_7,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_8,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_9,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_10,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_11,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_12,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_13,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_14,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_15,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_16,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_17,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_18,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_19,
	(MMI_IMAGE_ID_T)IMAGE_NUMBER_20
};

#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
//MEX application message register by leofeng 2011/04/20
MMI_APPLICATION_T    g_mex_app = {0};
#endif
#ifdef AUTO_APP_TEST
wchar AppNameString[200] = { 0 };
wchar AppNumber[20]={0};
#endif
//增加mex 游戏二级菜单
LOCAL MMI_RESULT_E	HandleMexGameMenuWindow(
									  MMI_WIN_ID_T win_id, 
									  MMI_MESSAGE_ID_E msg_id, 
									  DPARAM param
									  );
LOCAL MMI_RESULT_E HandleMexGameAboutWinMsg(
									  MMI_WIN_ID_T win_id,
									  MMI_MESSAGE_ID_E msg_id,
									  DPARAM param
									  );

//the window for mex game SECOND MENU
WINDOW_TABLE( MEXGAME_MAIN_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMexGameMenuWindow ),    
    WIN_ID( MET_MEX_GAME_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_MEX_SECONDMENU_TITLE),
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#if defined(MEX_PLATFORM__FOR_SPRD6600L) 
	CREATE_LISTBOX_CTRL(MMI_LISTBOX_LEFT, MMI_CLIENT_RECT_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM,
	GUILIST_TEXTLIST_E, MET_MEX_SECOND_MENU_CTRL_ID),
#elif defined(MEX_PLATFORM__FOR_SPRD6800H) 	
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MET_MEX_SECOND_MENU_CTRL_ID),
#endif
    END_WIN
};

//the window for ABOUT information in SECOND MENU
WINDOW_TABLE( MEXGAME_ABOUT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMexGameAboutWinMsg ),    
    WIN_ID( MET_MEX_GAME_ABOUT_WIN_ID),
    WIN_TITLE(TXT_MEX_SECONDMENU_ABOUT),
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
    CREATE_TEXT_CTRL(MET_MEX_ABOUT_STATIC_TEXT_CTRL_ID),
#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
	CREATE_TEXTBOX_CTRL(
		0,
		MMI_STATUSBAR_HEIGHT + MMI_TITLE_HEIGHT,
		MMI_MAINSCREEN_RIGHT_MAX_PIXEL,
		MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-1,
		MAIN_LCD_ID,
		MET_MEX_ABOUT_STATIC_TEXT_CTRL_ID
	),    
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMetMexWinMsg( 
									  MMI_WIN_ID_T win_id, 
									  MMI_MESSAGE_ID_E msg_id, 
									  DPARAM param
									  );


WINDOW_TABLE( MET_MEX_MAIN_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)HandleMetMexWinMsg ),
#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)        
#if defined(MMI_PDA_SUPPORT) || (MEX_SPRD_CODE_VERSION == 0x10A6530)
    WIN_HIDE_STATUS,
#endif
#endif
	WIN_ID( MET_MEX_MAIN_WIN_ID ),
	END_WIN
};
extern void Mex_SetEntryParam(char* pInitParam,char* pMexName,E_Mex_Special_Version_Type eSVT,char* pLoadStr);
//记录当前独立入口信息
void Mex_SetCurrentEntryAppInfo(char *pAppName, int32 iAppID)
{
	mex_strcpy(pCurrEntryAppName,pAppName);
	iCurrEntryAppID = iAppID;
}

//获取当前独立入口信息
void Mex_GetCurrentEntryAppInfo(char *pAppName, uint32 iAppNameLen, int32 *pAppID)
{
	if (pAppName != NULL)
	{
		mex_strncpy(pAppName,pCurrEntryAppName,iAppNameLen);
	}
	if (pAppID != NULL)
	{
		*pAppID = iCurrEntryAppID;
	}
}

int32 Mex_GetCurrEntryAppId(char *pInitParam)
{
	if( mex_strstr(pInitParam,pCurrEntryAppName) != NULL)
	{
		return iCurrEntryAppID;
	}
	else
	{
		mex_log(MEX_LOG_SYSTEM,"forget to set entry app info!!!");
		return 0;	
	}
}


//-----------定义SCC Platform 相关信息------------
//Mex Platfrom win func
LOCAL MMI_RESULT_E HandleSSCMexPlatform( 
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);
//mex engineer mode win func
LOCAL MMI_RESULT_E HandleMexPlatformEngineerMode( 
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id, 
	DPARAM param
	);

//SSC PLATFORM WIN Table
WINDOW_TABLE( MET_MEX_SSC_PLATFORM_WIN_TAB) = 
{
	WIN_FUNC ((uint32)HandleSSCMexPlatform),
	WIN_ID (MET_MEX_SSC_PLATFORM_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_MET_MEX_PLATFORM_TITLE),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
	CREATE_TEXT_CTRL(MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID),
#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
	CREATE_TEXTBOX_CTRL(
	0,
	MMI_STATUSBAR_HEIGHT+MMI_TITLE_HEIGHT,
	MMI_MAINSCREEN_RIGHT_MAX_PIXEL,
	MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-1,
	MAIN_LCD_ID,
	MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID
	),

#endif
	END_WIN  
};

extern int8 g_MexLogFilter;//log 状态
bool g_bExitAll = false;//标示是不是退出所有的App
void MexSystem_CloseMainWin(void);

#if defined(MEX_PLATFORM__FOR_SPRD6800H)

#define MEX_ENG_GAP              10
#define MEX_ENG_COMMON_HEIGHT    (MMI_DEFAULT_FONT_HEIGHT+10) 
#define MEX_ENG_COMMON_LENGTH    (5*MMI_DEFAULT_FONT_HEIGHT)	
#if 0
//mex engineer mode win 
WINDOW_TABLE( MET_MEX_PLATFORM_ENGINEERMODE_WIN_TAB) = 
{
	WIN_FUNC ((uint32)HandleMexPlatformEngineerMode),
	WIN_ID (MET_MEX_ENGINEER_MODE_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_MEX_PLT_ENG_TITLE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID),
#if (MEX_SCREEN_ESPECIAL_VERSION)
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID),
#endif
	CREATE_DROPDOWNLIST_CTRL(2,GUIDNLIST_SHOW_DOWNWARD,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID),
	CREATE_EDIT_DIGITAL_CTRL(6,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID),
	CREATE_EDIT_DIGITAL_CTRL(6,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID),
#if (MEX_SCREEN_ESPECIAL_VERSION)
	CREATE_EDIT_DIGITAL_CTRL(3,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID),
	CREATE_EDIT_DIGITAL_CTRL(3,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID),
#endif
	END_WIN  
};
#endif
WINDOW_TABLE( MET_MEX_PLATFORM_ENGINEERMODE_WIN_TAB) = 
{
	WIN_FUNC ((uint32)HandleMexPlatformEngineerMode),
	WIN_ID (MET_MEX_ENGINEER_MODE_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_STATUSBAR,
	WIN_TITLE(TXT_MEX_PLT_ENG_TITLE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_LOG_NAME_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID,MET_MEX_LOG_NAME_FORM_CTRL_ID),
		CHILD_SETLIST_CTRL(TRUE,MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID,MET_MEX_LOG_NAME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_VENDER_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID,MET_MEX_VENDER_FORM_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE,6,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,MET_MEX_VENDER_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_PHONEUA_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID,MET_MEX_PHONEUA_FORM_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE,6,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,MET_MEX_PHONEUA_FORM_CTRL_ID),

#if (MEX_SCREEN_ESPECIAL_VERSION)
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_SCREEN_WIDTH_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID,MET_MEX_SCREEN_WIDTH_FORM_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE,3,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,MET_MEX_SCREEN_WIDTH_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_SCREEN_HEIGHT_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID,MET_MEX_SCREEN_HEIGHT_FORM_CTRL_ID),
	   CHILD_EDIT_DIGITAL_CTRL(TRUE,3,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,MET_MEX_SCREEN_HEIGHT_FORM_CTRL_ID),
#endif
#ifdef AUTO_APP_TEST
		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_APP_NAME_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_LABEL_CTRL_ID,MET_MEX_APP_NAME_FORM_CTRL_ID),
		CHILD_EDIT_TEXT_CTRL(TRUE,50,MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,MET_MEX_APP_NAME_FORM_CTRL_ID),
		
		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_APP_NUMBER_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_LABEL_CTRL_ID,MET_MEX_APP_NUMBER_FORM_CTRL_ID),
		CHILD_EDIT_DIGITAL_CTRL(TRUE,5,MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,MET_MEX_APP_NUMBER_FORM_CTRL_ID),

		CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MET_MEX_START_TEST_FORM_CTRL_ID,MET_MEX_PLATFORM_MODE_FORM_CTRL_ID),
		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID,MET_MEX_START_TEST_FORM_CTRL_ID),
#endif
	END_WIN  
};

#elif defined(MEX_PLATFORM__FOR_SPRD6600L)

#define MEX_PLT_ENG_CLIENT_HEIGHT (MMI_MAINSCREEN_BOTTOM_MAX_PIXEL + MMI_STATUSBAR_HEIGHT + MMI_TITLE_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)//可以显示的高度
#define MEX_PLT_ENG_CENTER_X (MMI_MAINSCREEN_RIGHT_MAX_PIXEL >> 1)													//中心位置 x
#define MEX_PLT_ENG_CENTER_Y (MEX_PLT_ENG_CLIENT_HEIGHT >> 1)														//中心位置 y
#define MEX_MIN(x,y) ((x)<(y)?(x):(y))
#define MEX_PLT_ENG_LINE_DISTANCE (MEX_MIN(10,((MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_STATUSBAR_HEIGHT - MMI_TITLE_HEIGHT - 3*MMI_DEFAULT_FONT_HEIGHT) >> 1)))//行距
#define MEX_PLT_ENG_COMMON_LABEL_LENGTH (2*MMI_DEFAULT_FONT_HEIGHT + 22)											//通用Label长度
#define MEX_PLT_ENG_COMMON_LABEL_X1 (MEX_PLT_ENG_CENTER_X - MEX_PLT_ENG_COMMON_LABEL_LENGTH - 1)					//通用Label X1
#define MEX_PLT_ENG_COMMON_LABEL_X2 (MEX_PLT_ENG_CENTER_X - 1)														//通用Label X2
#ifdef AUTO_APP_TEST
#if defined(MAINLCD_SIZE_320X240) && defined(AUTO_APP_TEST)
#define MEX_PLT_ENG_BEGIN_Y 25
#else
#define MEX_PLT_ENG_BEGIN_Y 48
#endif
#else
#define MEX_PLT_ENG_BEGIN_Y ((2*MEX_PLT_ENG_CENTER_Y - 5*MMI_DEFAULT_FONT_HEIGHT - 4*MEX_PLT_ENG_LINE_DISTANCE) >> 1)//y的起始位置
#endif
#define MEX_PLT_ENG_ARROW_LEFT_RECT		{MEX_PLT_ENG_CENTER_X+1,MEX_PLT_ENG_BEGIN_Y,\
	MEX_PLT_ENG_CENTER_X+11,MEX_PLT_ENG_BEGIN_Y+MMI_DEFAULT_FONT_HEIGHT}		//左箭头rect(箭头和label空格一个dot)
#define MEX_PLT_ENG_LOG_LABEL_X1		(MEX_PLT_ENG_CENTER_X + 12)													//log Label x1
#define MEX_PLT_ENG_LOG_LABEL_X2		(MEX_PLT_ENG_LOG_LABEL_X1 + 2*MMI_DEFAULT_FONT_HEIGHT)						//log Label x2
#define MEX_PLT_ENG_ARROW_RIGHT_RECT	{MEX_PLT_ENG_LOG_LABEL_X2+1,MEX_PLT_ENG_BEGIN_Y,\
	MEX_PLT_ENG_LOG_LABEL_X2+11,MEX_PLT_ENG_BEGIN_Y+MMI_DEFAULT_FONT_HEIGHT}	//右箭头rect

#define MEX_PLT_ENG_EDITBOX_X1			(MEX_PLT_ENG_CENTER_X+1)													//Editbox x1
#define MEX_PLT_ENG_EDITBOX_X2			(MEX_PLT_ENG_EDITBOX_X1 + MEX_PLT_ENG_LOG_LABEL_X2-MEX_PLT_ENG_CENTER_X+10)	//Editbox x2
#if defined(MAINLCD_SIZE_320X240) && defined(AUTO_APP_TEST)
#define MEX_PLT_ENG_FONT_HEIGHT_N(line)  ((MMI_DEFAULT_FONT_HEIGHT-3)*line)
#define MEX_PLT_ENG_LINE_DISTANCE_N(line) ((MEX_PLT_ENG_LINE_DISTANCE-7)*line)
#else
#define MEX_PLT_ENG_FONT_HEIGHT_N(line)  (MMI_DEFAULT_FONT_HEIGHT*line)
#define MEX_PLT_ENG_LINE_DISTANCE_N(line) (MEX_PLT_ENG_LINE_DISTANCE*line)
#endif

//mex engineer mode win 
WINDOW_TABLE( MET_MEX_PLATFORM_ENGINEERMODE_WIN_TAB) = 
{
	WIN_FUNC ((uint32)HandleMexPlatformEngineerMode),
	WIN_ID (MET_MEX_ENGINEER_MODE_WIN_ID),
#if !(defined(MAINLCD_SIZE_320X240) && defined(AUTO_APP_TEST))
	WIN_STATUSBAR,
#endif
	WIN_TITLE(TXT_MEX_PLT_ENG_TITLE),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y,
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(1),
	GUILABEL_CENTER,
	COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID),
	CREATE_LABEL_CTRL(	MEX_PLT_ENG_LOG_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y,
	MEX_PLT_ENG_LOG_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(1),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID),

	CREATE_LABEL_CTRL(	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(1)+MEX_PLT_ENG_LINE_DISTANCE_N(1),
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(2)+MEX_PLT_ENG_LINE_DISTANCE_N(1),
	GUILABEL_CENTER,
	COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID),


	CREATE_DIGITALEDITBOX_CTRL(
	MEX_PLT_ENG_EDITBOX_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(1)+MEX_PLT_ENG_LINE_DISTANCE_N(1),
	MEX_PLT_ENG_EDITBOX_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(2)+MEX_PLT_ENG_LINE_DISTANCE_N(1),
	6,
	MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID
	),
	CREATE_LABEL_CTRL(
	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(2)+MEX_PLT_ENG_LINE_DISTANCE_N(2),
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(3)+MEX_PLT_ENG_LINE_DISTANCE_N(2),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID),

	CREATE_DIGITALEDITBOX_CTRL(
	MEX_PLT_ENG_EDITBOX_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(2)+MEX_PLT_ENG_LINE_DISTANCE_N(2),
	MEX_PLT_ENG_EDITBOX_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(3)+MEX_PLT_ENG_LINE_DISTANCE_N(2),
	6,
	MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID
	),

#if MEX_SCREEN_ESPECIAL_VERSION
	CREATE_LABEL_CTRL(
	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(3)+MEX_PLT_ENG_LINE_DISTANCE_N(3),
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(4)+MEX_PLT_ENG_LINE_DISTANCE_N(3),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID),

	CREATE_DIGITALEDITBOX_CTRL(
	MEX_PLT_ENG_EDITBOX_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(3)+MEX_PLT_ENG_LINE_DISTANCE_N(3),
	MEX_PLT_ENG_EDITBOX_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(4)+MEX_PLT_ENG_LINE_DISTANCE_N(3),
	6,
	MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID
	),

	CREATE_LABEL_CTRL(
	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(4)+MEX_PLT_ENG_LINE_DISTANCE_N(4),
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(5)+MEX_PLT_ENG_LINE_DISTANCE_N(4),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID),

	CREATE_DIGITALEDITBOX_CTRL(
	MEX_PLT_ENG_EDITBOX_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(4)+MEX_PLT_ENG_LINE_DISTANCE_N(4),
	MEX_PLT_ENG_EDITBOX_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(5)+MEX_PLT_ENG_LINE_DISTANCE_N(4),
	6,
	MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID
	),
#endif
#ifdef AUTO_APP_TEST
	CREATE_LABEL_CTRL(
	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(5)+MEX_PLT_ENG_LINE_DISTANCE_N(5),
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(6)+MEX_PLT_ENG_LINE_DISTANCE_N(5),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_LABEL_CTRL_ID),

    CREATE_LABEL_CTRL(
    MEX_PLT_ENG_EDITBOX_X1,//120,//MEX_PLT_ENG_EDITBOX_X1+50,
    MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(5)+MEX_PLT_ENG_LINE_DISTANCE_N(5), 
    MEX_PLT_ENG_EDITBOX_X2,//182,//MEX_PLT_ENG_EDITBOX_X2, 
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(6)+MEX_PLT_ENG_LINE_DISTANCE_N(5),
    GUILABEL_LEFT, 
    COMMON_IMAGE_NULL,
    MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID),

	CREATE_LABEL_CTRL(
	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(6)+MEX_PLT_ENG_LINE_DISTANCE_N(6),
	MEX_PLT_ENG_COMMON_LABEL_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(7)+MEX_PLT_ENG_LINE_DISTANCE_N(6),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_LABEL_CTRL_ID),


	CREATE_DIGITALEDITBOX_CTRL(
	MEX_PLT_ENG_EDITBOX_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(6)+MEX_PLT_ENG_LINE_DISTANCE_N(6),
	MEX_PLT_ENG_EDITBOX_X2,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(7)+MEX_PLT_ENG_LINE_DISTANCE_N(6),
	6,
	MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID),

	CREATE_LABEL_CTRL(
	MEX_PLT_ENG_COMMON_LABEL_X1,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(7)+MEX_PLT_ENG_LINE_DISTANCE_N(7),
	MEX_PLT_ENG_COMMON_LABEL_X2+50,
	MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(8)+MEX_PLT_ENG_LINE_DISTANCE_N(7),
	GUILABEL_CENTER,COMMON_IMAGE_NULL,
	MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID),
#endif
	END_WIN  
};

#endif
//----------End ----定义SCC Platform 相关信息------------

//==============FAUI IDLE Setting Win Start==================
#if defined(MET_MEX_SUPPORT)&&defined(MEX_G7_UI_SUPPORT)
extern MMI_WIN_ID_T  g_idle_win;
BOOLEAN g_mex_bIsFauiIdle = FALSE;
BOOLEAN g_mex_bIsUSBUsed = FALSE;//USB正在用
#define MEX_FAUI_CFG_NAME		L"MexFaui.cfg"  //开机启动配置文件
extern uint16 MetMex_GetIdleSetting(void);
void MetMex_TryResetFauiEntryParam(void)
{
	if(g_mex_bIsFauiIdle)
	{
	   g_mex_entry_func = (MexEntryFuncPtr)SetMetMexMainInitParam;
	}
}
void MetMex_SetUSBState(BOOLEAN bValue)
{
	g_mex_bIsUSBUsed = bValue;
}
BOOLEAN MetMex_TryOpenFauiIdle(void)
{	
	MetMex_GetIdleSetting();
	if((TRUE ==g_mex_bIsFauiIdle)
		&&(FALSE == g_mex_bIsUSBUsed)
		)
	{
		extern void MetMexOpenWin(void);
	
		if(FALSE == MMK_IsOpenWin(MET_MEX_MAIN_WIN_ID))
		{
			if( MexConfig_GetPhoneConfig(E_MPC_Tcard_State) && MexConfig_GetPhoneConfig(E_MPC_Tcard_FreeSpace) < MexConfig_GetPhoneConfig(E_MPC_MinFreeSpace))//空间不够
			{
				MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
				g_mex_bIsFauiIdle = FALSE;
			}
			else
			{
				MetMexOpenWin();
				return TRUE;
			}
		}
	}
	return FALSE;
}
MMI_WIN_ID_T MetMex_GetIdleWinId(void)
{
	if((TRUE == g_mex_bIsFauiIdle)
		&&(FALSE == g_mex_bIsUSBUsed)
		&&(TRUE ==MMK_IsOpenWin(MET_MEX_MAIN_WIN_ID)
		&&(MexConfig_GetPhoneConfig(E_MPC_Tcard_FreeSpace) >= MexConfig_GetPhoneConfig(E_MPC_MinFreeSpace)))//空间要足够
		)
	{
		return MET_MEX_MAIN_WIN_ID;
	}
	return g_idle_win;
}
//设置开机启动。0:system,1:faui
void MetMex_SetIdleSetting(uint16 uValue)
{
	SFS_HANDLE pFile = 0;
	wchar_t szFauiCfgFileName[40] = { 0 };
	if(TRUE == g_mex_bIsUSBUsed)
		return;
	mex_wcscpy( szFauiCfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_MexDir_In_Phone) );
	mex_wcscat( szFauiCfgFileName, L"\\" );
	mex_wcscat( szFauiCfgFileName, MEX_FAUI_CFG_NAME );
	pFile= SFS_CreateFile((const uint16 *)szFauiCfgFileName,SFS_MODE_CREATE_ALWAYS|SFS_MODE_READ|SFS_MODE_WRITE,NULL,NULL);
	if(pFile != 0)
	{
		uint8 uWriteValue = (uint8)uValue;
		uint32 uWritten = 0;
		SFS_WriteFile(pFile,&uWriteValue,sizeof(uint8),&uWritten,NULL);
		SFS_CloseFile(pFile);
		g_mex_bIsFauiIdle =(uValue == 1);
	}	
}
uint16 MetMex_GetIdleSetting(void)
{
	SFS_HANDLE pFile = 0;
	wchar_t szFauiCfgFileName[40] = { 0 };
	if(TRUE == g_mex_bIsUSBUsed)
		return 0;
	
	mex_wcscpy( szFauiCfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_MexDir_In_Phone) );
//#endif
	mex_wcscat( szFauiCfgFileName, L"\\" );
	mex_wcscat( szFauiCfgFileName, MEX_FAUI_CFG_NAME );

	pFile= SFS_CreateFile((const uint16 *)szFauiCfgFileName,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,NULL,NULL);
	if(pFile != 0)
	{
		uint8 uReadValue = 0;
		uint32 uReaded =0;
		SFS_ReadFile(pFile,&uReadValue,sizeof(uint8),&uReaded,NULL);
		SFS_CloseFile(pFile);
		if((sizeof(uint8) == uReaded )&&(1 == uReadValue))
		{
			//读取成功
			g_mex_bIsFauiIdle = TRUE;
			return 1;
		}
	}
	g_mex_bIsFauiIdle  = FALSE;
	return 0;//如果配置文件不存在，也关闭FAUI
}

LOCAL MMI_RESULT_E HandleMetMexIdleSetting( 
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id, 
	DPARAM param
	)
{
	MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			uint16 uOldItem =MetMex_GetIdleSetting();
			uint32 uIndex = 0;
			wchar* strItem[2]={L"System",L"Faui"};
			GUILIST_SetMaxItem(MET_MEX_IDLE_SETTING_LIST_CTRL_ID,2, FALSE );
			for(;uIndex<2;uIndex++)
			{
				GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
				GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
				item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
				item_t.item_data_ptr = &item_data;

				item_data.softkey_id[0] = TXT_COMMON_OK;
				item_data.softkey_id[1] = TXT_NULL;
				item_data.softkey_id[2] = STXT_RETURN;

				item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
#if defined(MEX_PLATFORM__FOR_SPRD6600L) 
				MMIAPICOM_Wstrncpy(item_data.item_content[0].item_data.text_buffer.wstr,strItem[uIndex],MMIAPICOM_Wstrlen(strItem[uIndex])); /*lint !e605*/ 
#elif defined(MEX_PLATFORM__FOR_SPRD6800H) 
				item_data.item_content[0].item_data.text_buffer.wstr_ptr = strItem[uIndex]; /*lint !e605*/      
#endif
				item_data.item_content[0].item_data.text_buffer.wstr_len =   MMIAPICOM_Wstrlen(strItem[uIndex]);

				GUILIST_AppendItem(MET_MEX_IDLE_SETTING_LIST_CTRL_ID, &item_t);
			}
			//set selected item
			GUILIST_SetSelectedItem(MET_MEX_IDLE_SETTING_LIST_CTRL_ID, uOldItem, TRUE);
			//set current item
			GUILIST_SetCurItemIndex(MET_MEX_IDLE_SETTING_LIST_CTRL_ID, uOldItem);
			MMK_SetAtvCtrl(win_id,MET_MEX_IDLE_SETTING_LIST_CTRL_ID);
		}
		break;
	case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:
		//case MSG_NOTIFY_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
		{
			uint16                  cur_selection   =   0;
			uint16 uOldItem =MetMex_GetIdleSetting();
			//get the selected item
			GUILIST_GetSelectedItemIndex(MET_MEX_IDLE_SETTING_LIST_CTRL_ID,&cur_selection,1);
			if(cur_selection != uOldItem)
			{
				//切换idle设置时，要判断空间是否足够
				if( (cur_selection > 0)  //开机启动想切换到FAUI 
					&&MexConfig_GetPhoneConfig(E_MPC_Tcard_State) && (MexConfig_GetPhoneConfig(E_MPC_Tcard_FreeSpace) < MexConfig_GetPhoneConfig(E_MPC_MinFreeSpace)))//空间不够
				{
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
					break;
				}
				MetMex_SetIdleSetting(cur_selection);
			}

		}
		MMK_CloseWin( win_id );
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}
WINDOW_TABLE( MET_MEX_IDLE_SETTING_WIN_TAB ) = 
{
	WIN_FUNC( (uint32)HandleMetMexIdleSetting),    
	WIN_ID( MET_MEX_IDLE_SETTING_WIN_ID ),
	WIN_TITLE( TXT_MET_MEX_FAUI_IDLE_SETTING ),
#if defined(MEX_PLATFORM__FOR_SPRD6600L) 
	CREATE_LISTBOX_CTRL(MMI_LISTBOX_LEFT, MMI_CLIENT_RECT_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM,
	GUILIST_RADIOLIST_E, MET_MEX_IDLE_SETTING_LIST_CTRL_ID),
#elif defined(MEX_PLATFORM__FOR_SPRD6800H) 
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MET_MEX_IDLE_SETTING_LIST_CTRL_ID),
#endif
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};
void MetMex_OpenIdleSettingWin(void)
{
	if(MMIAPIUDISK_UdiskIsRun())
	{
		//u disk runing
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		return;
	}  
// 	if(FALSE == MexConfig_GetPhoneConfig(E_MPC_Tcard_State))//无T卡
// 	{
// 		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MET_MEX_NOCARD_OR_NOTSUPPORT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
// 		return;
// 	}
	MMK_CreateWin((uint32*)MET_MEX_IDLE_SETTING_WIN_TAB,PNULL);
}
//判断是否显示 “显示设置”
void MetMex_UpdateDiaplaySettingMenu(MMI_CTRL_ID_T ctrl_id)
	{

	BOOLEAN bVisible = (g_mex_bIsFauiIdle == FALSE);
	GUIMENU_SetItemVisible(ctrl_id,SLIDE_MENU_SETTINGS_ICON,ID_SETTINGS_DISPLAY,bVisible);
	GUIMENU_SetItemVisible(ctrl_id,MENU_SETTINGS_ICON,ID_SETTINGS_DISPLAY,bVisible);
#ifdef MAINLCD_SIZE_320X480
	GUIMENU_SetItemVisible(ctrl_id,MENU_SLIDE_TOOLS,ID_SETTINGS_DISPLAY,bVisible);
#endif
#ifndef CMCC_UI_STYLE
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
	GUIMENU_SetItemVisible(ctrl_id,MENU_SETTINGS,ID_SETTINGS_DISPLAY,bVisible);
#endif
#endif

	}

#endif
//==============FAUI IDLE Setting Win End  ==================



//================================================= MetMexSys_ begin
void MetMexSys_Init(void);
void MetMexSys_Pause(void);
void MetMexSys_Resume(void);
void MetMexSys_Exit(void);

//================================================= MetMexSys_ end


//=========================================================Inputbox begin

/*****************************************************************************/
//  Description : Handle_Met_Mex_InputWindow
//       
//  Global resource dependence : 
//   Author:       Wen.Liu
//   RETRUN:   
//  Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E  Handle_Met_Mex_InputBox(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id, 
	DPARAM param);
//===========================================================Inputbox end

//===========================================================os_lock begin
void mex_Pen_lock(void);
void mex_Pen_unlock(void);
void mex_Key_Lock(void);
void mex_Key_unLock(void);
void mex_ReStartKeyLock(void);
//===========================================================os_lock end
static int32 Mex_GetAppIdByFileName( wchar_t *szAppName )
{
	int32 iAppId = 0;
	while ((iAppId = mex_GetNextAppId(iAppId)) != 0)
	{
		wchar_t *szNowAppFileName ;
		mex_GetAppInfoFromId( iAppId, NULL, NULL, NULL, NULL, &szNowAppFileName );
		if ( mex_wcscmp( szNowAppFileName, szAppName ) == 0 )
			return iAppId;
	}
	return 0;
}

//当前tomcat前台是否在运行
bool Mex_IsMexTomcatRunning(void)
{
	int32 iAppId = Mex_GetAppIdByFileName(L"tom.mex");
	return ( iAppId != 0 );
}

//当前cqz前台是否在运行
bool Mex_IsMexCQZRunning(void)
{
	int32 iAppId = Mex_GetAppIdByFileName(L"cqz.mex");
	return ( iAppId != 0 );
}
void Mex_ShowWaiting(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T box = { 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1 };
    GUISTR_STYLE_T str_style = { 0 };
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T string = { 0 };
    wchar wait_string[28] = { 0 };
    GUI_RECT_T rect = { 0 };
    uint16 logic_lcd_width = 0;
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;

    str_style.align = ALIGN_HVMIDDLE;
    str_style.font = SONG_FONT_26;
    str_style.font_color = MMI_WHITE_COLOR;


    MMIAPISET_GetLanguageType(&lang_type);

    if (lang_type == MMISET_LANGUAGE_SIMP_CHINESE)
    {
#ifdef MEX_BIGEND
        MMIAPICOM_Wstrcpy(wait_string, "\x8B\xF7\x7A\x0D\x54\x0E\x00\x2E\x00\x2E\x00\x2E\x00");
#else
        MMIAPICOM_Wstrcpy(wait_string, "\xF7\x8B\x0D\x7A\x19\x50\x2E\x00\x2E\x00\x2E\x00\x00");
#endif
    }
    else if (lang_type == MMISET_LANGUAGE_TRAD_CHINESE)
    {
#ifdef MEX_BIGEND
        MMIAPICOM_Wstrcpy(wait_string, "\x8A\xCB\x7A\x0D\x5F\x8C\x00\x2E\x00\x2E\x00\x2E\x00");
#else
        MMIAPICOM_Wstrcpy(wait_string, "\xCB\x8A\x0D\x7A\x8C\x5F\x2E\x00\x2E\x00\x2E\x00\x00");
#endif
    }
    else
    {
        MMIAPICOM_StrToWstr("Please wait...", wait_string);
    }
    string.wstr_ptr = wait_string;
    string.wstr_len = MMIAPICOM_Wstrlen(wait_string);

    LCD_FillRect(&lcd_dev_info, box, 0);
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info, 
                               &box,
                               &box,
                               &string,
                               &str_style,
                               state,
                               GUISTR_TEXT_DIR_AUTO
                               );
    MMITHEME_SetUpdateDelayCount(0);
    MMITHEME_UpdateRect();
}
LOCAL MMI_RESULT_E HandleMetMexWinMsg( 
									  MMI_WIN_ID_T win_id, 
									  MMI_MESSAGE_ID_E msg_id, 
									  DPARAM param
									  )
{
	BOOLEAN result = TRUE;

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			// Mex System Init
			mex_log(MEX_LOG_SYSTEM, "I: sys init"); // open window & sys init
#ifdef UI_P3D_SUPPORT
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
			MMI_Enable3DMMI(FALSE);            
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
			MMI_Disable3DEffect(MMI_3D_EFFECT_MOD);
#endif
#endif
	        MMISRVAUD_ReqVirtualHandle("MET_MEX WAIT", MMISRVAUD_PRI_NORMAL);
			MetMexSys_Init();
		}
		break;
	case MSG_FULL_PAINT:
		{
			// Mex System Full Paint
		}
		break;
	case MSG_LOSE_FOCUS:
		{
			// Mex System Pause
			mex_log(MEX_LOG_SYSTEM, "I: sys pause"); // sys pause
			MetMexSys_Pause();
		}
		break;
	case MSG_GET_FOCUS:
		{
#ifdef MEX_G7_UI_SUPPORT	
			if((!g_mex_bIsFauiIdle)//非Faui启动画面，但是已经打开FAUI了，则强制关闭
				&&(g_mex_entry_func == (MexEntryFuncPtr)SetMetMexMainInitParam))
			{	
				MexSystem_Shutdown( E_MPS_ColdShutdown );
				MMK_HandleRedKeyProcess();
				break;
			}		
#endif
			// Mex System Resume
			MetMexSys_Resume();
			mex_log(MEX_LOG_SYSTEM, "I: sys resume ok"); // sys resume
		}
		break;
	case MSG_CLOSE_WINDOW:
		{
			// Mex System Shutdown
			MetMexSys_Exit();
#ifdef UI_P3D_SUPPORT
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
			MMI_Enable3DMMI(TRUE);            
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
			MMI_Enable3DEffect(MMI_3D_EFFECT_MOD);
#endif  
#endif
	        MMISRVAUD_FreeVirtualHandle("MET_MEX WAIT");
			MMIDEFAULT_AllowTurnOffBackLight(TRUE); //允许系统关闭背光灯
			mex_log(MEX_LOG_SYSTEM, "I: sys exit ok"); // sys exit
		}
		break;

	case MSG_KEYUP_OK:
	case MSG_KEYPRESSUP_OK:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_OK,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_OK:
		g_bIsWaitKeyUpEvent = true;
		Mex_Key_Listener_Key_Handle(KEY_OK,KEY_PRESSED);
		break;
	case MSG_KEYUP_CANCEL:
	case MSG_KEYPRESSUP_CANCEL:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;                  
			Mex_Key_Listener_Key_Handle(KEY_CANCEL,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_CANCEL:
		g_bIsWaitKeyUpEvent = true;
		Mex_Key_Listener_Key_Handle(KEY_CANCEL,KEY_PRESSED);
		break;
	case MSG_KEYUP_UP:
	case MSG_KEYPRESSUP_UP:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
		    Mex_Key_Listener_Key_Handle(KEY_UP,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_UP:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_UP,KEY_PRESSED);
		break;
	case MSG_KEYUP_DOWN:
	case MSG_KEYPRESSUP_DOWN:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_DOWN,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_DOWN:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_DOWN,KEY_PRESSED);
		break;
	case MSG_KEYUP_LEFT:
	case MSG_KEYPRESSUP_LEFT:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_LEFT,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_LEFT:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_LEFT,KEY_PRESSED);
		break;
	case MSG_KEYUP_RIGHT:
	case MSG_KEYPRESSUP_RIGHT:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_RIGHT,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_RIGHT:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_RIGHT,KEY_PRESSED);
		break;
	case MSG_KEYUP_MIDDLE:
		break;
	case MSG_KEYDOWN_MIDDLE:
		break;
	case MSG_KEYUP_WEB:
	case MSG_KEYPRESSUP_WEB:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_WEB,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_WEB:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_WEB,KEY_PRESSED);
		break;
	case MSG_KEYUP_0:
	case MSG_KEYPRESSUP_0:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_0,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_0:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_0,KEY_PRESSED);
		break;
	case MSG_KEYUP_1:
	case MSG_KEYPRESSUP_1:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_1,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_1:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_1,KEY_PRESSED);
		break;
	case MSG_KEYUP_2:
	case MSG_KEYPRESSUP_2:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_2,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_2:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_2,KEY_PRESSED);
		break;
	case MSG_KEYUP_3:
	case MSG_KEYPRESSUP_3:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_3,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_3:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_3,KEY_PRESSED);
		break;
	case MSG_KEYUP_4:
	case MSG_KEYPRESSUP_4:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_4,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_4:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_4,KEY_PRESSED);
		break;
	case MSG_KEYUP_5:
	case MSG_KEYPRESSUP_5:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_5,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_5:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_5,KEY_PRESSED);
		break;
	case MSG_KEYUP_6:
	case MSG_KEYPRESSUP_6:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_6,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_6:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_6,KEY_PRESSED);
		break;
	case MSG_KEYUP_7:
	case MSG_KEYPRESSUP_7:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_7,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_7:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_7,KEY_PRESSED);
		break;
	case MSG_KEYUP_8:
	case MSG_KEYPRESSUP_8:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_8,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_8:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_8,KEY_PRESSED);
		break;
	case MSG_KEYUP_9:
	case MSG_KEYPRESSUP_9:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_9,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_9:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_9,KEY_PRESSED);
		break;
	case MSG_KEYUP_STAR:
#if (MEX_SPRD_CODE_VERSION <= 0x09AFFFF)
	case MSG_KEYPRESSUP_START:
#else
	case MSG_KEYPRESSUP_STAR:
#endif
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_STAR,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_STAR:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_STAR,KEY_PRESSED);
		break;
	case MSG_KEYUP_HASH:
	case MSG_KEYPRESSUP_HASH:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_HASH,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_HASH:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_HASH,KEY_PRESSED);
		break;
    case MSG_KEYUP_UPSIDE:
	case MSG_KEYPRESSUP_UPSIDE:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_UPSIDEKEY,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_UPSIDE:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_UPSIDEKEY,KEY_PRESSED);
		break;
    case MSG_KEYUP_DOWNSIDE:
	case MSG_KEYPRESSUP_DOWNSIDE:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_DOWNSIDEKEY,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_DOWNSIDE:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_DOWNSIDEKEY,KEY_PRESSED);
		break;
	case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
		break;

		
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_KEYUP_A:
	case MSG_KEYPRESSUP_A:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_A,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_A:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_A,KEY_PRESSED);
		break;
	case MSG_KEYUP_B:
	case MSG_KEYPRESSUP_B:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_B,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_B:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_B,KEY_PRESSED);
		break;
	case MSG_KEYUP_C:
	case MSG_KEYPRESSUP_C:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_C,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_C:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_C,KEY_PRESSED);
		break;
	case MSG_KEYUP_D:
	case MSG_KEYPRESSUP_D:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_D,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_D:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_D,KEY_PRESSED);
		break;
	case MSG_KEYUP_E:
	case MSG_KEYPRESSUP_E:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_E,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_E:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_E,KEY_PRESSED);
		break;
	case MSG_KEYUP_F:
	case MSG_KEYPRESSUP_F:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_F,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_F:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_F,KEY_PRESSED);
		break;
	case MSG_KEYUP_G:
	case MSG_KEYPRESSUP_G:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_G,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_G:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_G,KEY_PRESSED);
		break;
	case MSG_KEYUP_H:
	case MSG_KEYPRESSUP_H:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_H,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_H:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_H,KEY_PRESSED);
		break;
	case MSG_KEYUP_I:
	case MSG_KEYPRESSUP_I:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_I,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_I:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_I,KEY_PRESSED);
		break;
	case MSG_KEYUP_J:
	case MSG_KEYPRESSUP_J:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_J,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_J:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_J,KEY_PRESSED);
		break;
	case MSG_KEYUP_K:
	case MSG_KEYPRESSUP_K:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_K,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_K:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_K,KEY_PRESSED);
		break;
	case MSG_KEYUP_L:
	case MSG_KEYPRESSUP_L:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_L,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_L:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_L,KEY_PRESSED);
		break;
	case MSG_KEYUP_M:
	case MSG_KEYPRESSUP_M:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_M,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_M:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_M,KEY_PRESSED);
		break;
	case MSG_KEYUP_N:
	case MSG_KEYPRESSUP_N:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_N,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_N:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_N,KEY_PRESSED);
		break;
	case MSG_KEYUP_O:
	case MSG_KEYPRESSUP_O:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_O,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_O:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_O,KEY_PRESSED);
		break;
	case MSG_KEYUP_P:
	case MSG_KEYPRESSUP_P:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_P,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_P:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_P,KEY_PRESSED);
		break;
	case MSG_KEYUP_Q:
	case MSG_KEYPRESSUP_Q:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_Q,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_Q:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_Q,KEY_PRESSED);
		break;
	case MSG_KEYUP_R:
	case MSG_KEYPRESSUP_R:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_R,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_R:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_R,KEY_PRESSED);
		break;
	case MSG_KEYUP_S:
	case MSG_KEYPRESSUP_S:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_S,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_S:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_S,KEY_PRESSED);
		break;
	case MSG_KEYUP_T:
	case MSG_KEYPRESSUP_T:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_T,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_T:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_T,KEY_PRESSED);
		break;
	case MSG_KEYUP_U:
	case MSG_KEYPRESSUP_U:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_U,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_U:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_U,KEY_PRESSED);
		break;
	case MSG_KEYUP_V:
	case MSG_KEYPRESSUP_V:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_V,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_V:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_V,KEY_PRESSED);
		break;
	case MSG_KEYUP_W:
	case MSG_KEYPRESSUP_W:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_W,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_W:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_W,KEY_PRESSED);
		break;
	case MSG_KEYUP_X:
	case MSG_KEYPRESSUP_X:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_X,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_X:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_X,KEY_PRESSED);
		break;
	case MSG_KEYUP_Y:
	case MSG_KEYPRESSUP_Y:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_Y,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_Y:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_Y,KEY_PRESSED);
		break;
	case MSG_KEYUP_Z:
	case MSG_KEYPRESSUP_Z:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_Z,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_Z:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_Z,KEY_PRESSED);
		break;
    case MSG_KEYUP_SPACE:
	case MSG_KEYPRESSUP_SPACE:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_SPACE,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_SPACE:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_SPACE,KEY_PRESSED);
		break;
	case MSG_KEYUP_DEL:
	case MSG_KEYPRESSUP_DEL:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_DEL,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_DEL:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_DEL,KEY_PRESSED);
		break;
	case MSG_KEYUP_CTRL:
	case MSG_KEYPRESSUP_CTRL:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_CTRL,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_CTRL:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_CTRL,KEY_PRESSED);
		break;
	case MSG_KEYUP_SHIFT:
	case MSG_KEYPRESSUP_SHIFT:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_SHIFT,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_SHIFT:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_SHIFT,KEY_PRESSED);
		break;
	case MSG_KEYUP_QUESTION:
	case MSG_KEYPRESSUP_QUESTION:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_QUESTION,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_QUESTION:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_QUESTION,KEY_PRESSED);
		break;
	case MSG_KEYUP_PERIOD:
	case MSG_KEYPRESSUP_PERIOD:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_PERIOD,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_PERIOD:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_PERIOD,KEY_PRESSED);
		break;
	case MSG_KEYUP_COMMA:
	case MSG_KEYPRESSUP_COMMA:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_COMMA,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_COMMA:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_COMMA,KEY_PRESSED);
		break;
	case MSG_KEYUP_EXCLAMATION:
	case MSG_KEYPRESSUP_EXCLAMATION:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_EXCLAMATION,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_EXCLAMATION:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_EXCLAMATION,KEY_PRESSED);
		break;
	case MSG_KEYUP_AT:
	case MSG_KEYPRESSUP_AT:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_AT,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_AT:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_AT,KEY_PRESSED);
		break;
	case MSG_KEYUP_FN:
	case MSG_KEYPRESSUP_FN:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_FN,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_FN:
		g_bIsWaitKeyUpEvent = true;        
		Mex_Key_Listener_Key_Handle(KEY_FN,KEY_PRESSED);
		break;
#endif
		
#ifdef TOUCH_PANEL_SUPPORT  
	case MSG_TP_PRESS_DOWN:
		if (g_bMexTPLock)
			break;
		g_bIsWaitTPUpEvent = true;        
		MexSystem_Handler_Pen_Down(MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
		break;

	case MSG_TP_PRESS_UP:
		if (g_bMexTPLock)
			break;
		if(g_bIsWaitTPUpEvent == true)
		{
			g_bIsWaitTPUpEvent = false; 
			MexSystem_Handler_Pen_Up(MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
		}
		break;

	case MSG_TP_PRESS_MOVE:
		if (g_bMexTPLock)
			break;
		MexSystem_Handler_Pen_Move(MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
		break;
#endif

#ifdef MEX_G7_UI_SUPPORT
	case MSG_KEYDOWN_RED:
		if(g_mex_bIsFauiIdle)//如果FAUI 被设置为Idle界面了
		{
			g_bIsWaitKeyUpEvent = true;  
			Mex_Key_Listener_Key_Handle(KEY_RED,KEY_PRESSED);
		}
		else
		{
			result = FALSE;
		}
		break;
	case MSG_KEYUP_RED:
		if((g_mex_bIsFauiIdle)&&(g_bIsWaitKeyUpEvent))
		{
			g_bIsWaitKeyUpEvent = false; 
			Mex_Key_Listener_Key_Handle(KEY_RED,KEY_RELEASED);
		}
		else
		{
			result = FALSE;
		}
		break;
	case MSG_KEYDOWN_FLIP://合上
		if(g_mex_bIsFauiIdle)
			MexSystem_NotifySlideStatus(0,0);
		else
			return FALSE;
		break;
	case MSG_KEYUP_FLIP://打开
		if(g_mex_bIsFauiIdle)
			MexSystem_NotifySlideStatus(1,0);
		else
			return FALSE;
		break;
	
#endif
    case MSG_KEYUP_MENU:
	case MSG_KEYPRESSUP_MENU:
		if(g_bIsWaitKeyUpEvent == true)
		{
			g_bIsWaitKeyUpEvent = false;
			Mex_Key_Listener_Key_Handle(KEY_OK,KEY_RELEASED);
		}
		break;
	case MSG_KEYDOWN_MENU:
		g_bIsWaitKeyUpEvent = true;
		Mex_Key_Listener_Key_Handle(KEY_OK,KEY_PRESSED);
		break;

	default:
		result = FALSE;
		break;
	}

	return result;
}

//设置Manger和ting Manager的loadStr
static void SetLoadStr(const char* pTinyLoadStr,const char* pManLoadStr)
{
	mex_memset16(g_szTinyLoadStr,0,MEX_MAX_LOAD_STRING_LEN);
	mex_memset16(g_szManLoadStr,0,MEX_MAX_LOAD_STRING_LEN);
	if(NULL == pTinyLoadStr)
		mex_str2nwcs(g_szTinyLoadStr,MEX_MAX_LOAD_STRING_LEN<<1,"正在初始化系统...");//NULL 则提示：正在初始化
	else
		mex_str2nwcs(g_szTinyLoadStr,MEX_MAX_LOAD_STRING_LEN<<1,pTinyLoadStr);

	mex_str2nwcs(g_szManLoadStr,MEX_MAX_LOAD_STRING_LEN<<1,pManLoadStr);
	MexConfig_SetTinyLoadingPage( true,
		MEX_COLOR_DARKBLUE,
		MEX_COLOR_WHITE,
		MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
		(const wchar_t *)g_szTinyLoadStr);	

	MexConfig_SetManLoadingPage( true,
		MEX_COLOR_DARKBLUE, 
		MEX_COLOR_WHITE,
		MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
		(const wchar_t *)g_szManLoadStr);	
}
#if MEX_SOC_GPRS_SIM_SELECT
bool g_select_sim = false;
LOCAL MMI_RESULT_E HandleMexSIMSelectWinMsg(                                
										 MMI_WIN_ID_T   win_id,                                        
                                         MMI_MESSAGE_ID_E	msg_id, 
                                         DPARAM				param
                                         );
//显示使用SIM卡拨出的选项                                                
WINDOW_TABLE( MET_MEX_SELECT_SIM_WIN_TAB ) = 
{
        WIN_FUNC((uint32)HandleMexSIMSelectWinMsg),    
        WIN_ID(MET_MEX_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
		CREATE_LISTBOX_CTRL(0, MMI_TITLE_HEIGHT, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, (MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT), 
		    GUILIST_RADIOLIST_E,
		    MET_MEX_SELECT_SIM_CTRL_ID),
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
		CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MET_MEX_SELECT_SIM_CTRL_ID),
#endif
#ifdef MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif
//进入MEX,并设置初始化参数
static void Mex_EntryByParam(void* entryFunc)
{
#if MEX_SOC_GPRS_SIM_SELECT
	 if(MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
	 {
		g_mex_entry_func = (MexEntryFuncPtr)entryFunc;
		MMK_CreateWin(MET_MEX_SELECT_SIM_WIN_TAB, NULL);
#if defined(MET_MEX_SUPPORT)&&defined(MEX_G7_UI_SUPPORT)
		 if(MMK_IsOpenWin(MET_MEX_MAIN_WIN_ID))
		 {
			 MMK_HandleRedKeyProcess();
			 return;
		 }
#endif
	 }
	 else
	 {
	  g_mex_entry_func = (MexEntryFuncPtr)entryFunc;
#if defined(MET_MEX_SUPPORT)&&defined(MEX_G7_UI_SUPPORT)
		 if(MMK_IsOpenWin(MET_MEX_MAIN_WIN_ID))
		 {
			 MMK_HandleRedKeyProcess();
			 return;
		 }
#endif
	 MMK_CreateWin(MET_MEX_MAIN_WIN_TAB, NULL);
	 }
#else
	g_mex_entry_func = (MexEntryFuncPtr)entryFunc;  /*lint !e611 */
#if defined(MET_MEX_SUPPORT)&&defined(MEX_G7_UI_SUPPORT)
	if(MMK_IsOpenWin(MET_MEX_MAIN_WIN_ID))
	{
		MMK_HandleRedKeyProcess();
		return;
	}
#endif
	MMK_CreateWin((uint32*)MET_MEX_MAIN_WIN_TAB, NULL);
#endif
}

#if MEX_SOC_GPRS_SIM_SELECT
LOCAL MMI_RESULT_E HandleMexSIMSelectWinMsg(                                
										 MMI_WIN_ID_T   win_id,                                        
                                         MMI_MESSAGE_ID_E	msg_id, 
                                         DPARAM				param
                                         )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			list_index = 0;  
    MN_DUAL_SYS_E        dual_sys = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	//动态添加LIST ITEM
	GUILIST_SetMaxItem(MET_MEX_SELECT_SIM_CTRL_ID, MN_DUAL_SYS_MAX, FALSE );//max item 2
#if defined(MEX_PLATFORM__FOR_SPRD6600L)
	MMIAPISET_AppendDualSimName(MET_MEX_SELECT_SIM_CTRL_ID, TXT_COMMON_OK, MN_DUAL_SYS_MAX); 
#elif defined(MEX_PLATFORM__FOR_SPRD6800H)
    MMIAPISET_AppendMultiSimName(MET_MEX_SELECT_SIM_CTRL_ID, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SIM_OK);
#endif
	MMK_SetAtvCtrl(MET_MEX_SELECT_SIM_WIN_ID, MET_MEX_SELECT_SIM_CTRL_ID);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527                   
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        list_index = GUILIST_GetCurItemIndex(MET_MEX_SELECT_SIM_CTRL_ID);
		if(GUILIST_IsCurrItemInvalid(MET_MEX_SELECT_SIM_CTRL_ID))
        {
            break;
        }
        if (0 == list_index)
        {
            dual_sys = MN_DUAL_SYS_1;
			g_select_sim=false;
        }
        else 
        {
            dual_sys = MN_DUAL_SYS_2;
			g_select_sim=TRUE;
        }
        
        //SCI_TraceLow:"HandleMexSIMSelectWinMsg dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MET_MEX_WINTAB_1763_112_2_18_2_36_0_4,(uint8*)"d", dual_sys);
        

        MMK_CloseWin(win_id);//返回上一级
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:        
        MMK_CloseWin(win_id);//返回上一级
        break;    
    default:		
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

PUBLIC MN_DUAL_SYS_E Mex_Get_Select_sim(void)
{
  MN_DUAL_SYS_E s_dual_sys = MN_DUAL_SYS_1;
if ((!MMIPHONE_IsSimOk(MN_DUAL_SYS_1)) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
	 {
		 s_dual_sys = MN_DUAL_SYS_2;
	 }
else if (MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && (!MMIPHONE_IsSimOk(MN_DUAL_SYS_2)))
	 {
		 s_dual_sys = MN_DUAL_SYS_1;
	 }
else if(MMIPHONE_IsSimOk(MN_DUAL_SYS_1) && MMIPHONE_IsSimOk(MN_DUAL_SYS_2))
	{
      if(g_select_sim)
      {  
	   s_dual_sys = MN_DUAL_SYS_2;
	  }
	  else
	  {
	   s_dual_sys = MN_DUAL_SYS_1;
	  }
	}
	 //SCI_TraceLow:"Mex_Get_Select_sim,s_dual_sys=%d"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MET_MEX_WINTAB_1803_112_2_18_2_36_1_5,(uint8*)"d",s_dual_sys);
	 return s_dual_sys;
}
#endif
/*****************************************************************************/
//  Description : MetMexEnterSecondMenu
//       
//  Global resource dependence : 
//   Author:        tingtao.tang
//   RETRUN:   
//  Note:   
/*****************************************************************************/
void MetMexEnterSecondMenu(ADD_DATA app_id)
{
	MMK_CreateWin((uint32 *)MEXGAME_MAIN_MENU_WIN_TAB, app_id);	
}

LOCAL void AppendListItembyBuffer(MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T image_id, MMI_STRING_T text_buffer)
{
	GUILIST_ITEM_T item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};

	item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[0].item_data.image_id = image_id;

	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_buffer.wstr_ptr;
	item_data.item_content[1].item_data.text_buffer.wstr_len = text_buffer.wstr_len;

	GUILIST_AppendItem(ctrl_id, &item_t);
}

LOCAL void AppendListItembyId(MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T image_id, MMI_TEXT_ID_T text_id)
{
	GUILIST_ITEM_T item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};

	item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[0].item_data.image_id = image_id;

	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[1].item_data.text_id = text_id;

	GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : HandleMexGameMenuWindow
//       
//  Global resource dependence : 
//   Author:        tingtao.tang
//   RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMexGameMenuWindow(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  )
{
    uint32          list_index  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T	ctrl_id	 = MET_MEX_SECOND_MENU_CTRL_ID;
	int16 i = 0;
	int16 j = 0;
	int16 k = 0;
	MMI_STRING_T MenuName={0}; 
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUILIST_SetMaxItem(ctrl_id, iRowTotal_h+3, FALSE );//max item: 3+iRowTotal_h
		AppendListItembyId(ctrl_id, IMAGE_NUMBER_1, TXT_MEX_SECONDMENU_START);
		AppendListItembyId(ctrl_id, IMAGE_NUMBER_2, TXT_MEX_SECONDMENU_MERIG3G);
		AppendListItembyId(ctrl_id, IMAGE_NUMBER_3, TXT_MEX_SECONDMENU_ABOUT);
		
		//添加动态二级菜单项
		for(i=0; i<iRowTotal_h; i++)
		{
			for(j=0; j<game_count; j++)
			{
				if( 0 == mex_stricmp(g_Game_NeedHiddenMenu[i], hid_menu_data[j].CmpKeyword) )
				{
					MenuName.wstr_ptr = (wchar *)hid_menu_data[j].charMenuName;
	                MenuName.wstr_len = MMIAPICOM_Wstrlen((const wchar*)hid_menu_data[j].charMenuName);
					AppendListItembyBuffer(ctrl_id, g_mexmenu_second_icon[3+k], MenuName);
					k++;
					break;
				}	
			}
		}

      MMK_SetAtvCtrl(win_id, ctrl_id);
      break;
      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
      list_index = GUILIST_GetCurItemIndex(ctrl_id);
      switch(list_index) 
      {
      case ID_MEXGAME_START://游戏启动菜单项
          {
		  	  //判断要启动的游戏id
			  switch((uint32)MMK_GetWinAddDataPtr(MMK_ConvertIdToHandle(win_id)))
			  {
#ifdef MET_MEX_QQ_SUPPORT
				case E_MSVT_QQ:
					{
						extern void MetMexQQOpenWin_Direct(void);
						MetMexQQOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_AQ_SUPPORT
				case E_MSVT_AQ:
					{
						extern void MetMexAQOpenWin_Direct(void);
						MetMexAQOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_ZT_SUPPORT
				case E_MSVT_ZT:
					{
						extern void MetMexZTOpenWin_Direct(void);
						MetMexZTOpenWin_Direct();
					}
					break;
#endif		
#ifdef MET_MEX_FRUITNINJA_SUPPORT
				case E_MSVT_FRUITSLICE:
					{
						extern void MetMexFruitNinjaOpenWin_Direct(void);
						MetMexFruitNinjaOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_WEIBO_SUPPORT
				case E_MSVT_WEIBO:
					{
						extern void MetMexWeiboOpenWin_Direct(void);
						MetMexWeiboOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_WEATHER_SUPPORT
				case E_MSVT_WEATHER:
					{
						extern void MetMexWeatherOpenWin_Direct(void);
						MetMexWeatherOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_WOMIVIDEO_SUPPORT
				case E_MSVT_WOMIVIDEO:
					{
						extern void MetMexWomiVideoOpenWin_Direct(void);
						MetMexWomiVideoOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_TB_SUPPORT
				case E_MSVT_TB:
					{
						extern void MetMexTBOpenWin_Direct(void);
						MetMexTBOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
				case E_MSVT_QQ:
					{
						if(1 == iStartDoubleQQ) //开启的是QQ1
						{
							extern void MetMexQQ1OpenWin_Direct(void);
							MetMexQQ1OpenWin_Direct();
						}	
						else if(2 == iStartDoubleQQ)
						{
							extern void MetMexQQ2OpenWin_Direct(void);
							MetMexQQ2OpenWin_Direct();
						}
					}
					break;
#endif
#ifdef MET_MEX_MSN_SUPPORT
				case E_MSVT_MSN:
					{
						extern void MetMexMSNOpenWin_Direct(void);
						MetMexMSNOpenWin_Direct();
					}
					break;
#endif 
#ifdef MET_MEX_FETION_SUPPORT
				case E_MSVT_FETION:
					{
						extern void MetMexFETIONOpenWin_Direct(void);
						MetMexFETIONOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_AB_SUPPORT
				case E_MSVT_FNDXN:
					{
						extern void MetMexABOpenWin_Direct(void);
						MetMexABOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_TOMCAT_SUPPORT
				case E_MSVT_TOMCAT:
					{
						extern void MetMexTomCatOpenWin_Direct(void);
						MetMexTomCatOpenWin_Direct();
					}
					break;
#endif 
#ifdef MET_MEX_CQZ_SUPPORT
				case E_MSVT_CQZ:
					{
						extern void MetMexCQZOpenWin_Direct(void);
						MetMexCQZOpenWin_Direct();
					}
					break;
#endif
#ifdef MET_MEX_JSCB_SUPPORT
				case E_MSVT_JSCB:
					{
						extern void MetMexJSCBOpenWin_Direct(void);
						MetMexJSCBOpenWin_Direct();
					}
					break;
#endif
                 default:
					break;
                   

				}
          }
          break;
		  
      case ID_MEXGAME_P15://至尊宝3G启动菜单项
          {
#ifdef MET_MEX_MERIG_SUPPORT
			  extern void MetMexOpenWin(void);
			  MetMexOpenWin();
#endif
          }
          break;
		  
      case ID_MEXGAME_ABOUT://游戏关于菜单项
	  	  {
          	MMK_CreateWin((uint32 *)MEXGAME_ABOUT_WIN_TAB, PNULL);
      	  }
		  break;
          
      default:
			{
				int16 index = 0;
				int16 idex = 0;
				int16 jdex = 0;
				int16 kdex = 0;
				
				extern void MetMexEntryOpenWin(char *AppName,int id);
				
				for(idex=0; idex<iRowTotal_h; idex++)
				{
					for(jdex=0; jdex<game_count; jdex++)
					{
						if( 0 == mex_stricmp(g_Game_NeedHiddenMenu[idex], hid_menu_data[jdex].CmpKeyword) )
						{
							g_szIndexArray[kdex++] = jdex;
							break;
						}
					}
					if(jdex == game_count)
						mex_log(MEX_LOG_SYSTEM, "warning! %s not exist in Table.", g_Game_NeedHiddenMenu[idex]);
				}
					
				index = g_szIndexArray[(list_index - 3)];

				MetMexEntryOpenWin(hid_menu_data[index].AppName, hid_menu_data[index].AppId);
			}
          break;
      }
      break;
      
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
          
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : the window for boxman help information
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMexGameAboutWinMsg(
											  MMI_WIN_ID_T win_id,
											  MMI_MESSAGE_ID_E msg_id,
											  DPARAM param
											  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
   // MMI_STRING_T string = {0};
	MMI_CTRL_ID_T ctrl_id = MET_MEX_ABOUT_STATIC_TEXT_CTRL_ID;

    switch(msg_id)
    {
    case MSG_FULL_PAINT:
  	  {
  		  char tempStr[1024] = {0};
	      wchar showStr[1024] = {0};
		  MMI_STRING_T textString = {0};
		  
  		  sprintf(tempStr,"MEX V:%s_%s\nBuild: %s\n(C) 2010 MeTek",
  			  MexSystem_GetSystemVersion(),MexSystem_GetPatchVersion(),MexSystem_GetBulidDate());  

	  	  mex_str2nwcs(showStr,2048,tempStr);
		  textString.wstr_ptr = showStr;
		  textString.wstr_len = MMIAPICOM_Wstrlen(showStr);
		  MMK_SetAtvCtrl(win_id, ctrl_id);
			{
#if (MEX_SPRD_CODE_VERSION >= 0x09AFFFF)
				  GUI_FONT_T font = CAF_FONT_20;
				  GUITEXT_SetFont(ctrl_id,&font,PNULL);
				  GUITEXT_SetString(ctrl_id,textString.wstr_ptr,textString.wstr_len,FALSE);
#else
				  GUI_RECT_T titleRect = MMI_TITLE_RECT_UNDER_STB;
				  GUIWIN_SetTitleRect(win_id, titleRect); 
				  GUITEXTBOX_SetText(ctrl_id,&textString,FALSE);
#endif
			}
	    }

  	  break;
	  
	case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
      MMK_CloseWin(win_id);
      break;
      
    default:
      recode = MMI_RESULT_FALSE;
      break;
    }

    return recode;
}

//解析mexsys.cfg文件数据到g_Game_NeedSecondMenu或g_Game_NeedHiddenMenu中
void Mex_ParseSecondMenuCfgFile(void)
{
	int32 *file = NULL;
	wchar_t pFullPath[30] = {0};
	char ch;
	int16 sign_count_s = 0;//用于二级菜单的单引号的计数
	int16 sign_count_h = 0;//用于动态二级菜单的双引号的计数
	int16 row_s = 0;//二级菜单的排
	int16 col_s = 0;//二级菜单的列
	int16 row_h = 0;//动态二级菜单的排
	int16 col_h = 0;//动态二级菜单的列
	
	mex_wcscpy( pFullPath, (wchar_t *)MexConfig_GetPhoneConfig(E_MPC_SystemRootDir) );
	mex_memset(g_Game_NeedSecondMenu, 0, AllRow*AllCol);
	mex_memset(g_Game_NeedHiddenMenu, 0, AllRow*AllCol);
	
	file = mex_fopenw_ext( L"mexsys.cfg", L"rb", pFullPath, NULL, FALSE);
	if(file == NULL)
		return;

	//获取需要开启二级菜单或动态二级菜单的应用标识
	while(mex_fread(&ch,1,1,file))
	{
		if('\'' == ch)
		{
			sign_count_s++;
			
			if(2 == sign_count_s)
			{
				sign_count_s = 0;
				row_s++;
				col_s = 0;
			}	
		}
		else if(1 == sign_count_s)//从发现第一个单引号算起，将应用标识存入到数组里
		{
			*(*(g_Game_NeedSecondMenu + row_s) + col_s) = ch;
			col_s++;
		}
		else if('\"' == ch)
		{
			sign_count_h++;

			if(2 == sign_count_h)
			{
				sign_count_h = 0;
				row_h++;
				col_h = 0;
			}
		}
		else if(1 == sign_count_h)
		{
			*(*(g_Game_NeedHiddenMenu + row_h) + col_h) = ch;
			col_h++;
		}
	}
	
	iRowTotal_s = row_s; //需要进二级菜单总的游戏个数
	iRowTotal_h = row_h; //需要进动态二级菜单总的游戏个数

	mex_fclose(file);
}	

//查找g_Game_NeedSecondMenu数组中是否有和pGameName相同的字符串
BOOLEAN Mex_JudgeEnterSecondMenu(char *pGameName)
{
	int16 i;
	BOOLEAN Is_Entering = FALSE;

	Mex_ParseSecondMenuCfgFile();//解析mexsys.cfg文件

	//是否进二级菜单的判断
	for(i=0; i<iRowTotal_s; i++)
	{
		if(0 == mex_strcmp(pGameName, *(g_Game_NeedSecondMenu + i)))
		{
			Is_Entering = TRUE;
			return Is_Entering;
		}
	}

	return Is_Entering;
}
/*
*函数：Mex_SetEntryParam
*描述：设置独立入口参数
*
*pInitParam [in] :初始化参数，例如添加QQ独立入口时，参数设置为[Mex_QQ.mex:]
*pMexName	[in] :app名，例如[Mex_QQ.mex]
*eSVT		[in]:App或游戏代码号，参考E_Mex_Special_Version_Type
*pLoadStr   [in]:进入app时的提示信息，例如：正在加载QQ...
*
*返回值：无
*
*/
void Mex_SetEntryParam(char* pInitParam,char* pMexName,E_Mex_Special_Version_Type eSVT,char* pLoadStr)
{
	MexSystem_SetSystemInitParam(pInitParam);
	if(pMexName)
		Mex_SetCurrentEntryAppInfo( pMexName , eSVT);
	if(pLoadStr)
		SetLoadStr(NULL,pLoadStr);	
}
//进入至尊宝3G初始参数设置
void SetMetMexMainInitParam(void)
{
	Mex_SetEntryParam(NULL,NULL,E_MSVT_Normal,"请稍候......");
#ifdef MEX_G7_UI_SUPPORT
	if(g_mex_bIsFauiIdle)//如果 FAUI设置为IDLE WIN
	{
		Mex_SetEntryParam("AdExplorer.mex:","AdExplorer.mex",E_MSVT_FAUI,"请稍候......");
	}
#endif
}

//进入至尊宝3G
void MetMexOpenWin(void)
{
	Mex_EntryByParam((void*)SetMetMexMainInitParam);  /*lint !e611 */
}

/******************************************************************************/
//万能独立入口
#ifdef AUTO_APP_TEST
char g_test_entry1[20] = {0};
char g_test_entry2[50] = {0};
char *src =":";
int g_test_entry_id = 0;
static void SetMetMexTestEntryInitParam(void)
{
	Mex_SetEntryParam(g_test_entry2,g_test_entry1,g_test_entry_id,"正在加载...");
}
void MetMexTestEntryOpenWin(char *AppName,int id)
{	
	g_test_entry_id = id;
	strcpy(g_test_entry1,AppName);
	strcpy(g_test_entry2,AppName);
	strcat(g_test_entry2,src);
	Mex_EntryByParam((void*)SetMetMexTestEntryInitParam);  /*lint !e611 */
}
#endif
/******************************************************************************/
//*****************************************************************************
//用于动态二级菜单的公共应用入口
char g_entry1[20] = {0};
char g_entry2[50] = {0};
char *src2 =":";
int g_entry_id = 0;
static void SetMetMexEntryInitParam(void)
{
	Mex_SetEntryParam(g_entry2,g_entry1,g_entry_id,"正在加载...");
}
void MetMexEntryOpenWin(char *AppName,int id)
{	
	g_entry_id = id;
	strcpy(g_entry1,AppName);
	strcpy(g_entry2,AppName);
	strcat(g_entry2,src2);
	Mex_EntryByParam((void*)SetMetMexEntryInitParam);/*lint !e611 */
}
//******************************************************************************
//====================QQ 独立入口添加=========Begin=============
#ifdef MET_MEX_QQ_SUPPORT 
static void SetMetMexQQInitParam(void)
{
	Mex_SetEntryParam("Mex_QQ.mex:","Mex_QQ.mex",E_MSVT_QQ,"正在加载QQ...");
}
//进入MEX_QQ
void MetMexQQOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexQQInitParam);  /*lint !e611 */
}
#endif

#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
static void SetMetMexQQ1InitParam(void)
{
	Mex_SetEntryParam("Mex_QQ.mex:1","Mex_QQ.mex",E_MSVT_QQ,"正在加载QQ...");
}
//进入MEX_QQ
void MetMexQQ1OpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexQQ1InitParam);  /*lint !e611 */
}
static void SetMetMexQQ2InitParam(void)
{
	Mex_SetEntryParam("Mex_QQ.mex:2","Mex_QQ.mex",E_MSVT_QQ,"正在加载QQ...");
}
//进入MEX_QQ
void MetMexQQ2OpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexQQ2InitParam);  /*lint !e611 */
}
#endif
//====================QQ 独立入口添加=========End=============

//====================TB 独立入口添加=========Begin=============
#ifdef MET_MEX_TB_SUPPORT
static void SetMetMexTBInitParam(void)
{
	Mex_SetEntryParam("MEX_TB.mex:","MEX_TB.mex",E_MSVT_TB,"正在加载我爱麦克疯...");
}
//进入MEX_TB
void MetMexTBOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexTBInitParam);  /*lint !e611 */
}
#endif
//====================TB 独立入口添加=========End=============


//====================号码卫士独立入口添加=========Begin=============
#ifdef MET_MEX_AQ_SUPPORT
static void SetMetMexAQInitParam(void)
{
	Mex_SetEntryParam("AreaInquire.mex:","AreaInquire.mex",E_MSVT_AQ,"正在加载号码卫士...");
}
//进入号码卫士
void MetMexAQOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexAQInitParam);  /*lint !e611 */
}
#endif
//====================号码卫士独立入口添加=========End=============

//====================征途 独立入口添加=========Begin=============
#ifdef MET_MEX_ZT_SUPPORT
static void SetMetMexZTInitParam(void)
{
	Mex_SetEntryParam("mobile_zt.mex:","mobile_zt.mex",E_MSVT_ZT,"正在加载绿色征途...");
}
//进入MEX_TB
void MetMexZTOpenWin_Direct(void)
{	
	if(MetMex_ConflictPromt())
	{
	   return;
	}
	Mex_EntryByParam((void*)SetMetMexZTInitParam);  /*lint !e611 */
}
#endif
//====================征途 独立入口添加=========End=============
//====================切水果 独立入口添加=========Begin=============
#ifdef MET_MEX_FRUITNINJA_SUPPORT
static void SetMetMexFruitNinjaInitParam(void)
{
	Mex_SetEntryParam("fruit.mex:","fruit.mex",E_MSVT_FRUITSLICE,"正在加载水果忍者...");
}
//进入切水果
void MetMexFruitNinjaOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexFruitNinjaInitParam);  /*lint !e611 */
}
#endif
//====================切水果 独立入口添加=========End=============
//====================新浪微博 独立入口添加=========Begin=============
#ifdef MET_MEX_WEIBO_SUPPORT
static void SetMetMexWeiboInitParam(void)
{
	Mex_SetEntryParam("weibo.mex:","weibo.mex",E_MSVT_WEIBO,"正在加载新浪微博...");
}
//进入WEIBO
void MetMexWeiboOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexWeiboInitParam);  /*lint !e611 */
}
#endif
//====================新浪微博 独立入口添加=========End=============
//====================天气预报 独立入口添加=========Begin=============
#ifdef MET_MEX_WEATHER_SUPPORT
static void SetMetMexWeatherInitParam(void)
{
	Mex_SetEntryParam("weather.mex:","weather.mex",E_MSVT_WEATHER,"正在加载天气预报...");
}
//进入WEATHER
void MetMexWeatherOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexWeatherInitParam);  /*lint !e611 */
}
#endif
//====================天气预报 独立入口添加=========End=============
//====================沃米视频聊天 独立入口添加=========Begin=============
#ifdef MET_MEX_WOMIVIDEO_SUPPORT
static void SetMetMexWomiVideoInitParam(void)
{
	Mex_SetEntryParam("mex_wochat.mex:","mex_wochat.mex",E_MSVT_WOMIVIDEO,"正在加载沃米视频...");
}
//进入MEX_TB
void MetMexWomiVideoOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexWomiVideoInitParam);  /*lint !e611 */
}
#endif
//====================沃米视频聊天 独立入口添加=========End=============


//====================TOM猫 独立入口添加=========Begin=============
#ifdef MET_MEX_TOMCAT_SUPPORT
static void SetMetMexTomCatInitParam(void)
{
	Mex_SetEntryParam("tom.mex:","tom.mex",E_MSVT_TOMCAT,"正在加载TOM猫...");
}
//进入TOM CAT
void MetMexTomCatOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexTomCatInitParam);  /*lint !e611 */
}
#endif
//====================TOM猫 独立入口添加=========End=============


//====================MSN  独立入口添加========Begin=============

#ifdef MET_MEX_MSN_SUPPORT
static void SetMetMexMSNInitParam(void)
{
	Mex_SetEntryParam("MSN.mex:","MSN.mex",E_MSVT_MSN,"正在加载MSN...");
}
//进入MEX_MSN
void MetMexMSNOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexMSNInitParam);  /*lint !e611 */
}
#endif
//====================MSN  独立入口添加========end=============

//====================fetion 立入口添加=========Begin=============
#ifdef MET_MEX_FETION_SUPPORT
static void SetMetMexFETIONInitParam(void)
{
	Mex_SetEntryParam("fetion.mex:","fetion.mex",E_MSVT_FETION,"正在加载FETION...");
}
//进入MEX_FETION
void MetMexFETIONOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexFETIONInitParam);  /*lint !e611 */
}
#endif
//====================fetion独立入口添加=========End=============
//====================愤怒的小鸟 独立入口添加=========Begin=============
#ifdef MET_MEX_AB_SUPPORT
static void SetMetMexABInitParam(void)
{
	Mex_SetEntryParam("AngryBirds.mex:","AngryBirds.mex",E_MSVT_FNDXN,"正在加载愤怒的小鸟...");
}
//进入AngryBirds
void MetMexABOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexABInitParam);  /*lint !e611 */
}
#endif
//====================愤怒的小鸟 独立入口添加==========End=============

//====================吹裙子 独立入口添加=========Begin=============
#ifdef MET_MEX_CQZ_SUPPORT
static void SetMetMexCQZInitParam(void)
{
	Mex_SetEntryParam("cqz.mex:","cqz.mex",E_MSVT_CQZ,"正在加载吹裙子...");
}
//进入吹裙子
void MetMexCQZOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexCQZInitParam);  /*lint !e611 */
}
#endif
//====================吹裙子 独立入口添加==========End=============

//====================金山词霸 独立入口添加=========Begin=============
#ifdef MET_MEX_JSCB_SUPPORT
static void SetMetMexJSCBInitParam(void)
{
	Mex_SetEntryParam("jscb.mex:","jscb.mex",E_MSVT_JSCB,"正在加载金山词霸...");
}
//金山词霸
void MetMexJSCBOpenWin_Direct(void)
{	
	Mex_EntryByParam((void*)SetMetMexJSCBInitParam);  /*lint !e611 */
}
#endif
//====================金山词霸 独立入口添加==========End=============
//======================================================================
//===============以下为含有二级菜单的应用入口===Begin=======
//======================================================================
//进入QQ
#ifdef MET_MEX_QQ_SUPPORT
void MetMexQQOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("qq") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_QQ));
	}
	else
	{
		MetMexQQOpenWin_Direct();
	}
}
#endif
//进入AQ
#ifdef MET_MEX_AQ_SUPPORT
void MetMexAQOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("aq") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_AQ));
	}
	else
	{
		MetMexAQOpenWin_Direct();
	}
}
#endif
//进入ZT
#ifdef MET_MEX_ZT_SUPPORT
void MetMexZTOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("zt") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_ZT));
	}
	else
	{
		MetMexZTOpenWin_Direct();
	}
}
#endif
//进入切水果
#ifdef MET_MEX_FRUITNINJA_SUPPORT
void MetMexFruitNinjaOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("fruit") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_FRUITSLICE));
	}
	else
	{
		MetMexFruitNinjaOpenWin_Direct();
	}
}
#endif
//进入新浪微博
#ifdef MET_MEX_WEIBO_SUPPORT
void MetMexWeiboOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("weibo") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_WEIBO));
	}
	else
	{
		MetMexWeiboOpenWin_Direct();
	}
}
#endif
//进入天气预报
#ifdef MET_MEX_WEATHER_SUPPORT
void MetMexWeatherOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("weather") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_WEATHER));
	}
	else
	{
		MetMexWeatherOpenWin_Direct();
	}
}
#endif
//进入沃米视频聊天
#ifdef MET_MEX_WOMIVIDEO_SUPPORT
void MetMexWomiVideoOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("womi") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_WOMIVIDEO));
	}
	else
	{
		MetMexWomiVideoOpenWin_Direct();
	}
}
#endif
//进入TB
#ifdef MET_MEX_TB_SUPPORT
void MetMexTBOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("tb") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_TB));
	}
	else
	{
		MetMexTBOpenWin_Direct();
	}
}
#endif
//进入双QQ
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
void MetMexQQ1OpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("qq1") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_QQ));
		iStartDoubleQQ = 1;//开启的是QQ1
	}
	else
	{
		MetMexQQ1OpenWin_Direct();
	}
}
void MetMexQQ2OpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("qq2") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_QQ));
		iStartDoubleQQ = 2;//开启的是QQ2
	}
	else
	{
		MetMexQQ2OpenWin_Direct();
	}
}
#endif
//进入MSN
#ifdef MET_MEX_MSN_SUPPORT
void MetMexMSNOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("msn") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_MSN));
	}
	else
	{
		MetMexMSNOpenWin_Direct();
	}
}
#endif 
//进入飞信
#ifdef MET_MEX_FETION_SUPPORT
void MetMexFETIONOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("fetion") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_FETION));
	}
	else
	{
		MetMexFETIONOpenWin_Direct();
	}
}
#endif
//进入小鸟应用
#ifdef MET_MEX_AB_SUPPORT
void MetMexABOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("angrybirds") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_FNDXN));
	}
	else
	{
		MetMexABOpenWin_Direct();
	}
}
#endif
//进入Tom猫
#ifdef MET_MEX_TOMCAT_SUPPORT
void MetMexTomCatOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("tom") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_TOMCAT));
	}
	else
	{
		MetMexTomCatOpenWin_Direct();
	}
}
#endif 
//进入吹裙子
#ifdef MET_MEX_CQZ_SUPPORT
void MetMexCQZOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("cqz") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_CQZ));
	}
	else
	{
		MetMexCQZOpenWin_Direct();
	}
}
#endif
//进入金山词霸
#ifdef MET_MEX_JSCB_SUPPORT
void MetMexJSCBOpenWin(void)
{
	if( TRUE == Mex_JudgeEnterSecondMenu("jscb") )
	{
		MetMexEnterSecondMenu((ADD_DATA)(E_MSVT_JSCB));
	}
	else
	{
		MetMexJSCBOpenWin_Direct();
	}
}
#endif
//======================================================================
//==================含有二级菜单的应用入口===End=============
//======================================================================
//================================================= MetMexSys_ begin

#define MEXMANAGER_LAUNCH_APP_BY_MEXSTARTUP_FILE	(!MEX_BUILD_FINAL_USER)

void MetMexSys_Init(void)
{
#if (MEX_SDK_VER >= 1018)
	MexConfig_MapSimCard();    
#endif
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
	// FIXME:  此处强制禁止进行LCD 旋转处理,以后需要改进 
	MexInterface_EnableLcdRotate(false);
#endif

	Mex_ShowWaiting();

#ifdef FM_SUPPORT
    if( FM_PLAYING == MMIAPIFM_GetStatus()) //进入平台暂停fm
    {
#if (MEX_SPRD_CODE_VERSION >= 0x09A1113) 
	    MMIAPIFM_Pause(TRUE);
#else
        MMIAPIFM_Pause();
#endif
    }
#endif
	if(g_mex_entry_func != NULL)
		g_mex_entry_func();
#if MEXMANAGER_LAUNCH_APP_BY_MEXSTARTUP_FILE
	{
		SFS_HANDLE pFile;
		wchar_t cfgFileName[40] = { 0 };

		mex_wcscpy( cfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_SystemRootDir) );
		mex_wcscat( cfgFileName, L"\\MexStartup.cfg" );

		pFile = SFS_CreateFile((const uint16 *)cfgFileName, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, NULL, NULL);
		if (pFile != 0)
		{
			int16 i;
			char szApp[40] = { 0 };
			uint32 iRead;
			SFS_ReadFile((SFS_HANDLE)pFile, szApp, 40, &iRead, NULL);
			SFS_CloseFile( pFile );
			for (i = mex_strlen(szApp) - 1; i > 0; i--)
			{
				if (szApp[i] == '\r' || szApp[i] == '\n' || szApp[i] == ' ' || szApp[i] == '\t' || szApp[i] == 26)
					szApp[i] = 0;
			}
			if (mex_strlen(szApp) > 0)
			{
				mex_strcat( szApp, ".mex:" );
				MexSystem_SetSystemInitParam( szApp );
			}
		}
	}
#endif

	{
        //准备进入Mex
#ifdef MEX_FORBID_DOWNLOAD        
        int8 nResult = E_MEE_OK;
#define TEMP_PATH_LEN 256
#define TEMP_FILENAME_LEN 128
        wchar_t *t_path = SCI_ALLOC((TEMP_PATH_LEN + TEMP_FILENAME_LEN) * 2);
        uint32 t_file_handle = 0;
        if(NULL != t_path)
        {
            extern char * MexSystem_GetSystemInitParam(void);
            wchar_t *t_filename = t_path + TEMP_PATH_LEN;
            SFS_FIND_DATA_T t_find_data;
            // 根路径
            MMIAPICOM_Wstrcpy(t_path,mex_GetAppRootDir());
            MMIAPICOM_Wstrcat(t_path,L"\\");
            // 文件名
            if(NULL != MexSystem_GetSystemInitParam())
            {
                MMIAPICOM_StrToWstr(MexSystem_GetSystemInitParam(),t_filename);
            }
            else
            {
                MMIAPICOM_StrToWstr("MexAppGetter.mex",t_filename);
            }
            if(L':' == t_filename[MMIAPICOM_Wstrlen(t_filename) - 1])
            {
                t_filename[MMIAPICOM_Wstrlen(t_filename) - 1] = 0;
            }
            else if(L':' == t_filename[MMIAPICOM_Wstrlen(t_filename) - 2])  // Mex_QQ.mex:1的情况
            {
                t_filename[MMIAPICOM_Wstrlen(t_filename) - 2] = 0;
            }
            // 程序路径
            MMIAPICOM_Wstrcat(t_path,t_filename);
            t_path[MMIAPICOM_Wstrlen(t_path) - 4] = 0;
            //完整路径
            MMIAPICOM_Wstrcat(t_path,L"\\");
            MMIAPICOM_Wstrcat(t_path,t_filename);
            if(0 != (t_file_handle = SFS_FindFirstFile(t_path,&t_find_data)))
            {
                SFS_FindClose(t_file_handle);
            }
            SCI_FREE(t_path);
        }
        if(0 == t_file_handle)
        {
            nResult = E_MEE_PREPARETFLASHERR;
        }
        if(E_MEE_PREPARETFLASHERR != nResult)
        {
            nResult = MexSystem_Init();
        }
#else
        int8 nResult = MexSystem_Init();
#endif
		if(E_MEE_OK != nResult)//进入失败
		{

			MMI_TEXT_ID_T textID = 0;
			switch(nResult)
			{
			case E_MEE_USBMS:
				textID = TXT_COMMON_UDISK_USING;//u盘使用中
				break;
			case E_MEE_NOCARD:
				textID = TXT_MET_MEX_NOCARD_OR_NOTSUPPORT;//无t卡
				break;
			case E_MEE_SPACESHORTAGE: //空间不足
				textID = TXT_NO_SPACE;
				break;
			case E_MEE_INTMEMINITERR:		//Internal内存初始化失败
			case E_MEE_EXTMEMINITERR:		//External内存初始化失败	
			case E_MEE_RESMEMINITERR:		//Reserved内存初始化失败

				textID  = TXT_MET_MEX_MEM_INIT_ERR;//内存初始化失败!
				break;
			case E_MEE_PREPARETFLASHERR:		//T卡文件加载失败

				textID  = TXT_MET_MEX_FLASH_FILE_ERR;//T卡文件加载失败!
				break;
			case E_MEE_STARTUPTINYERR:		//启动ManagerTiny失败

				textID  = TXT_MET_MEX_TINY_STAT_ERR;//启动ManagerTiny失败!
				break;
			default:
				break;
			}//end switch
			if(textID != 0)
			{
				MMK_CloseWin(MET_MEX_MAIN_WIN_ID);
				MMIPUB_OpenAlertWinByTextId(PNULL,textID,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#ifdef MEX_G7_UI_SUPPORT	
				if(g_mex_bIsFauiIdle)
				{
					g_mex_bIsFauiIdle = FALSE;
				}		
#endif		
			}	
		}
	}
}
void MetMexSys_Pause(void)
{
	MexSystem_Shutdown(E_MPS_Pause);
}

void MetMexSys_Resume(void)
{
	MetMexSys_Init();
}

void MetMexSys_Exit(void)
{
	if (MexSystem_IsSysKernalRunning())
	{
		MexSystem_Shutdown(E_MPS_ColdShutdown);
	}
	g_mex_entry_func = NULL;//重置进入函数指针
}

void MetMexSys_ShutDownAll(void)
{
    
	if(MMK_IsOpenWin(MET_MEX_INPUT_WIN_ID))
	{
		MMK_CloseWin(MET_MEX_INPUT_WIN_ID);
	}
	
	if( MexSystem_IsSysKernalRunning() )
	{
		MexSystem_Shutdown( E_MPS_ColdShutdown );
	}
}

void MexSystem_CloseMainWin(void)//关闭平台窗口
{

	if(MMK_IsOpenWin(MET_MEX_MAIN_WIN_ID) 
		&&(MET_MEX_MAIN_WIN_ID == MMK_GetFocusWinId()))
	{
		MMK_CloseWin(MET_MEX_MAIN_WIN_ID);
	}
}
//-----------For Exit--Mex Platform--------------------------

void Mex_ExitExtSystem_EntryIdle(void)
{
	MexSystem_Shutdown( E_MPS_HotShutdown );
	MexConfig_EntrySystemApp( E_MESA_IDLE_SCREEN,NULL);
}

void Mex_ExitAllSystem_EntryIdle(void)
{
	MexSystem_Shutdown( E_MPS_ColdShutdown );
	MexConfig_EntrySystemApp( E_MESA_IDLE_SCREEN,NULL);
}


//确认退出Mex平台？
void Mex_Exit_System( bool bExitAll )
{
	void *hKeyLeft = NULL;
	if(MET_MEX_MAIN_WIN_ID != MMK_GetFocusWinId())
		return;
	if ( bExitAll )
		hKeyLeft = (void *)Mex_ExitAllSystem_EntryIdle;  /*lint !e611 */
	else
		hKeyLeft = (void *)Mex_ExitExtSystem_EntryIdle;  /*lint !e611 */
	//打开提示窗口
	MetMex_SetPopupKeyHandler( hKeyLeft, NULL );
	MetMex_DisplayPopupById( TXT_MET_MEX_QUIT_MSG, 0, 0 );
}
//------------exit mex platform ---------end

//================================================= MetMexSys_ end



//=============================================================== MetMex Timer Begin

typedef struct
{
	MexFuncPtr      m_pCallBack;
	uint8           m_iRefNum;
	uint8           m_iTimerId;
	SCI_TIMER_PTR   m_pTimer;
} MetMex_Timer;

typedef enum
{
	E_MTI_START, // dummy timer id
	E_MTI_MMI,
	E_MTI_STOP_VIBRATOR_TIMER,//停止震动的Timer
	E_MTI_ST_START,			//开始ST
	E_MTI_RECORD,
	E_MTI_NET_RECONNECT,
    E_MIT_PLAYAMR_CALLBACK,

	E_MTI_MAX
} E_Mex_Timer_Id;

LOCAL MetMex_Timer g_MexTimers[E_MTI_MAX] = { 0 };
LOCAL char *g_MexTimerNames[E_MTI_MAX] = {
	"Mex_Dummy_Timer",
	"Mex_MMI_Timer",
	"Mex_Stop_Vibrator_Timer",
	"Mex_ST_Start_Timer",
	"Mex_Record_Timer"
	,"Mex_NetReconect_Timer"
	,"Mex_PlayAmr_Timer"
};

LOCAL void MetMex_TimerCallBack( uint32 input )
{
	MmiTimerExpiryS *sendSignal = NULL;
	MetMex_Timer *pMexTimer = (MetMex_Timer *) input;

	// create the signal to send requesting task
	MmiCreateSignal(E_MSSC_MMI_TIMER_EXPIRY, sizeof(MmiTimerExpiryS), (MmiSignalS**)&sendSignal);

	sendSignal->Sender = MexConfig_ConvertSprdDef(E_M2MDC_MET_MEX_SEND);
	sendSignal->timerId = pMexTimer->m_iTimerId;
	sendSignal->ref_num = pMexTimer->m_iRefNum;

	MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);

}
uint8 MetMex_GetMMITimerId(void)
{
	return E_MTI_MMI;
}
uint8 MetMex_GetVibratorTimerId(void)
{
	return E_MTI_STOP_VIBRATOR_TIMER;
}
uint8 MetMex_GetSTTimer(void)
{
	return E_MTI_ST_START;
}
uint8 MetMex_GetRecordTimerId(void)
{
	return E_MTI_RECORD;
}
uint8 MetMex_GetNetReconnectTimerId(void)
{
	return E_MTI_NET_RECONNECT;
}

uint8 MetMex_GetPlayAmrTimerId(void)
{
    return E_MIT_PLAYAMR_CALLBACK;
}

//Start Mex timer
void MetMex_StartTimer(uint8 timerid, uint32 time_out, MexFuncPtr funcPtr,bool is_period)
{
	MetMex_Timer *pMexTimer = &g_MexTimers[timerid];
	if(!(timerid > E_MTI_START && timerid < E_MTI_MAX))
	{
        mex_log(MEX_LOG_SYSTEM,"try to start a unknown timer %d",timerid);
		return;
	}

	if (pMexTimer->m_pTimer == NULL)
	{
		char *pName = g_MexTimerNames[timerid];
		pMexTimer->m_pTimer = SCI_CreateTimer(pName, MetMex_TimerCallBack, (uint32)pMexTimer, time_out, false);
	}

	if (SCI_IsTimerActive(pMexTimer->m_pTimer))
	{
		SCI_DeactiveTimer(pMexTimer->m_pTimer);
	}

	SCI_ChangeTimer(pMexTimer->m_pTimer, MetMex_TimerCallBack, time_out);
	pMexTimer->m_pCallBack = funcPtr;
	pMexTimer->m_iTimerId = timerid;
	pMexTimer->m_iRefNum++;
	SCI_ActiveTimer(pMexTimer->m_pTimer);
    SCI_Sleep(1);

}

//Stop Mex timer
void MetMex_StopTimer(uint8 timerid)
{
	MetMex_Timer *pMexTimer = &g_MexTimers[timerid];
	if(!(timerid > E_MTI_START && timerid < E_MTI_MAX))
	{
        mex_log(MEX_LOG_SYSTEM,"try to stop a unknown timer %d",timerid);
		return;
	}

	if (pMexTimer->m_pTimer != NULL)
	{
		if (SCI_IsTimerActive(pMexTimer->m_pTimer))
		{
			SCI_DeactiveTimer(pMexTimer->m_pTimer);
		}
		SCI_DeleteTimer(pMexTimer->m_pTimer);
		memset(pMexTimer, 0, sizeof(MetMex_Timer));
	}
}
uint32 g_mexsoc_task_priority = SCI_APP_PRI(0);

void MetMex_SpreadStartupInit(void)
{
	//出货统计开机初始化
#if defined MEX_ST_SUPPORT
	extern void MexStInit(void);
	MexStInit();
#endif

    g_mexsoc_task_priority = SCI_APP_PRI(0);/*lint !e506 */

	//初始化OS File Pool
	MexLib_Os_Init_File_Pool();

#if (MEX_SPRD_CODE_VERSION < 0x09A1109)
	{
		extern MMI_RESULT_E MexInterface_HandlePsAndRefMsg(PWND app_ptr, uint16 msg_id, DPARAM param);        
		g_mex_app.ProcessMsg = MexInterface_HandlePsAndRefMsg;
	}
#endif

}
void MetMex_DispatchSignal(MmiTimerExpiryS* signal_ptr)
{
	switch (signal_ptr->SignalCode)
	{
	case E_MSSC_MMI_TIMER_EXPIRY:
		{
			uint8 timerid = signal_ptr->timerId;
			uint8 ref_num = signal_ptr->ref_num;
			MetMex_Timer *pMexTimer = &g_MexTimers[timerid];
			if (pMexTimer->m_iTimerId == timerid &&
				pMexTimer->m_iRefNum == ref_num)
			{
				pMexTimer->m_pCallBack();
			}
			else
			{
				mex_log(MEX_LOG_SYSTEM, "I: The timer(timerName:%s timerId = %d, ref_num = %d) was stopped", g_MexTimerNames[timerid], timerid, ref_num);
			}
		}
		break;
	case E_MSSC_MDI_SOUND_PLAYEND_IND:
		{
			extern void mex_HandleSoundPlayEnd(void);
			mex_HandleSoundPlayEnd();
		}
		break;
	case E_MSSC_MDI_AUDIO_CREATEFILEHANDLEEND:
		{
			void mex_audio_createFileHandleEnd(void);
			mex_audio_createFileHandleEnd();
		}
		break;
	case E_MSSC_MDI_AUDIO_PLAYEND_IND:
		{
			void mex_stream_audio_PlayEnd(void);
			mex_stream_audio_PlayEnd();
		}
        break;
	case E_MSSC_PHB_ADD_IND:
		{
			extern void MexPhs_AddPhbEntryExRsp(void *info);			
			MexPhs_AddPhbEntryExRsp((void *)signal_ptr);
		}
		break;
	case E_MSSC_PHB_UPDATE_IND:
		{
			extern void MexPhs_UpdatePhbEntryRsp(void *info);			
			MexPhs_UpdatePhbEntryRsp((void *)signal_ptr);
		}
		break;
	case E_MSSC_PHB_REMOVE_IND:
		{
			extern void MexPhs_RemovePhbEntryRsp(void *info);			
			MexPhs_RemovePhbEntryRsp((void *)signal_ptr);
		}
		break;        
		//receive soc msg
	case E_MSSC_SOC_EVENT:
		{
			extern void MexSoc_DealSocMsgEvent(void *info);
			MexSoc_DealSocMsgEvent((void*)signal_ptr);

		}
		break;
		//receive soc dns msg
	case E_MSSC_SOC_DNS_EVENT:
		{
			extern void MexSoc_GetHostByNameEventHandle(void *info);
			MexSoc_GetHostByNameEventHandle((void*)signal_ptr);

		}
		break;
	case E_MSSC_MDI_PCM_STOP_IND:
		{
			extern void mex_pcm_stop(void);
			mex_pcm_stop();
		}
		break;
	default:
		mex_log(MEX_LOG_SYSTEM,"mex got unknown msg %x",signal_ptr->SignalCode);
		break;
	}
}

//=============================================================== MetMex Timer End

//=========================================== Inputbox begin===============================


typedef struct
{
	bool                bCanChange;              
	bool                bIsFirm;              //is confrim input
	uint32              iMaxNum;              //max input len
	uint32              iTitleLen;            //title len
	int32               iAppId;               //current mex app id
	GUIEDIT_TYPE_E      uInput_type;          //type of input box 
	wchar_t             pInputTitle[30];      //tltle
	MMI_STRING_T        sText;                //input content
	uint8 *             pContent;             //init context
	Mex_Input_Func	    pInput_CallBack;      //app call back

}MEX_EDIT_WIN_INFO_T;

MEX_EDIT_WIN_INFO_T s_mex_edit_win_info = {0};


#if defined(MEX_PLATFORM__FOR_SPRD6800H)

WINDOW_TABLE(MET_MEX_INPUT_WIN_TAB) = 
{
	WIN_PRIO(WIN_ONE_LEVEL),
	WIN_FUNC ((uint32)Handle_Met_Mex_InputBox),
	WIN_ID (MET_MEX_INPUT_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_NULL),
#if defined(PDA_UI_SUPPORT_ANDROID)
	WIN_STYLE(WS_HAS_SOFTKEY),
#elif defined(MMI_PDA_SUPPORT)
	WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_EDIT_TEXT_CTRL(200,MET_MEX_INPUT_CTRL_ID),
	END_WIN  
};

//add by huang 2012-04-13 start
uint32 MET_MEX_INPUT_WIN_TAB4[] = 
{
	WIN_PRIO(WIN_ONE_LEVEL),
	WIN_FUNC ((uint32)Handle_Met_Mex_InputBox),
	WIN_ID (MET_MEX_INPUT_WIN_ID),
	WIN_STATUSBAR,
	WIN_TITLE(TXT_NULL),
#if defined(PDA_UI_SUPPORT_ANDROID)
	WIN_STYLE(WS_HAS_SOFTKEY),
#elif defined(MMI_PDA_SUPPORT)
	WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
	CREATE_EDIT_PASSWORD_CTRL(200,MET_MEX_INPUT_CTRL_ID),
	END_WIN  
};

//add by huang 2012-04-13 stop




#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
/*****************************************************************************/
// 	Description : check the string whether is digital
//	Global resource dependence : 
//  Author: Wen.Liu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckDigitalStr(
							  wchar* str_ptr,  // the pointer of the string
							  uint16 str_len   // the string length
							  )
{
	wchar*  check_str_ptr = str_ptr; // the pointer of the input string
	BOOLEAN recode        = TRUE;  // return code
	uint16  i             = NULL;  // temp

	if((PNULL == str_ptr)||(str_len == 0))
	{
        mex_log(MEX_LOG_SYSTEM,"invalid string to check");
        return false;
	}

	// check the input string
	for(i = 0; i < str_len; i++)
	{
		// whether the current char is digital
		if (((*check_str_ptr >= 0x30) && (*check_str_ptr <= 0x39)) ||
			('.' == *check_str_ptr))
		{
			check_str_ptr++;
		}
		else
		{
			recode = FALSE;
			break;
		}

	}

	return (recode);


}



#define MET_MEX_INPUT_MODIFY_POS	12

uint32 * MET_MEX_INPUT_WIN_TAB;

MMI_STRING_T   password_input_string = {0};


uint32 MET_MEX_INPUT_WIN_TAB1[] = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_TITLE( TXT_NULL),
	WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
	WIN_FUNC((uint32) Handle_Met_Mex_InputBox ),    
	WIN_ID( MET_MEX_INPUT_WIN_ID ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
	CREATE_TEXTEDITBOX_CTRL(MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP, 
	MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM, 
	200,200,
	IM_MULTITAP_TYPE |IM_DIGITAL_TYPE,
	IM_DIGITAL_MODE, 
	MET_MEX_INPUT_CTRL_ID),
	END_WIN
};

uint32 MET_MEX_INPUT_WIN_TAB2[] = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_TITLE( TXT_NULL ),
	WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
	WIN_FUNC((uint32) Handle_Met_Mex_InputBox ),    
	WIN_ID( MET_MEX_INPUT_WIN_ID ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
	CREATE_DIGITALEDITBOX_CTRL(MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP, 
	MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM, 
	50, 
	MET_MEX_INPUT_CTRL_ID),
	END_WIN
};

uint32 MET_MEX_INPUT_WIN_TAB3[] = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_TITLE( TXT_NULL),
	WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
	WIN_FUNC((uint32) Handle_Met_Mex_InputBox ),    
	WIN_ID( MET_MEX_INPUT_WIN_ID ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
	CREATE_TEXTEDITBOX_CTRL(MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP, 
	MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT, MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM, 
	200,200,
	IM_DEFAULT_ALL_INPUT_MODE_SET,
	IM_SMART_MODE, 
	MET_MEX_INPUT_CTRL_ID),
	END_WIN
};

#endif
//调用输入框的函数
//---------------------------------------------
//参数1: 输入框的标题，unicode 码
//参数2: 输入框的类型，见枚举定义
//参数3: 输入框的最大允许输入长度
//参数4: 是否为密码输入框
//参数5: 是否允许切换输入法
//参数6: 输入的字符串存储的buf，不可为NULL
//参数7: 被回调的函数指针
//---------------------------------------------


bool mex_Spread_InputBox_Create(  wchar_t* pInputTitle, E_Mex_Input_Type iInputType, uint32 iMaxNum,
								bool bIsPassword, bool bCanChange, 
								uint8 * pContent, Mex_Input_Func pInputFunc )
{
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if defined(MEX_PLATFORM__FOR_SPRD6800H)

	MMI_HANDLE_T win_handle = PNULL;
	MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
	MMI_CTRL_ID_T edit_ctrl_id = MET_MEX_INPUT_CTRL_ID;
	GUIEDIT_INIT_DATA_T 		edit_init = {0};
	MMI_CONTROL_CREATE_T		edit_ctrl = {0};

	SCI_MEMSET(&s_mex_edit_win_info, 0, sizeof(MEX_EDIT_WIN_INFO_T));

	if(pInputTitle != PNULL)
	{
		s_mex_edit_win_info.iTitleLen = mex_wcslen(pInputTitle);
		mex_memcpy(s_mex_edit_win_info.pInputTitle, pInputTitle, mex_wcslen(pInputTitle)*sizeof(wchar_t));
	}		
	if(pContent != PNULL)
	{ 
		s_mex_edit_win_info.pContent = (uint8 *)mex_malloc( (iMaxNum+1)<<1 );  /*lint !e611 */
		mex_memcpy( (void *)s_mex_edit_win_info.pContent, (const void *)pContent, iMaxNum<<1);  /*lint !e611 */
	}
	else
	{
		s_mex_edit_win_info.pContent = NULL;
	}
	s_mex_edit_win_info.iAppId = mex_GetCurrentApp();
	s_mex_edit_win_info.iMaxNum = iMaxNum;
	s_mex_edit_win_info.bCanChange = bCanChange;
	s_mex_edit_win_info.pInput_CallBack = pInputFunc;

	if(bIsPassword) //password edit box
		s_mex_edit_win_info.uInput_type  = GUIEDIT_TYPE_PASSWORD; 
	else if(E_MITT_NUMERIC == iInputType && !bCanChange)//digital edit box can not change input type
		s_mex_edit_win_info.uInput_type  = GUIEDIT_TYPE_DIGITAL; 
	else
		s_mex_edit_win_info.uInput_type  = GUIEDIT_TYPE_TEXT;


	//create edit window
	win_table_create.applet_handle = MMK_GetFirstAppletHandle();
	win_table_create.parent_win_handle = 0;
	//modify by huang 2012-04-13 start 
	if (s_mex_edit_win_info.uInput_type  == GUIEDIT_TYPE_PASSWORD)
	{
				win_table_create.win_table_ptr = MET_MEX_INPUT_WIN_TAB4;
		
				win_handle =  MMK_CreateWin((uint32*)MET_MEX_INPUT_WIN_TAB4,PNULL);
	}
	else
	{
			win_table_create.win_table_ptr = MET_MEX_INPUT_WIN_TAB;

			//win_handle = MMK_CreateWinTable(&win_table_create);
			win_handle =  MMK_CreateWin((uint32*)MET_MEX_INPUT_WIN_TAB,PNULL);
	}
	//modify by huang 2012-04-13 stop
	if(s_mex_edit_win_info.iTitleLen > 0)//set title
		GUIWIN_SetTitleText(win_handle,s_mex_edit_win_info.pInputTitle,s_mex_edit_win_info.iTitleLen,FALSE);

	//edit init data
	edit_init.both_rect = MMITHEME_GetWinClientBothRect(win_handle);
	edit_init.type		  = s_mex_edit_win_info.uInput_type;//type of edit box

	//create edit control
	edit_ctrl.ctrl_id			= edit_ctrl_id;
	edit_ctrl.guid				= SPRD_GUI_EDITBOX_ID;
	edit_ctrl.init_data_ptr 	= &edit_init;
	edit_ctrl.parent_win_handle = win_handle;
	MMK_CreateControl(&edit_ctrl);

#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
	s_mex_edit_win_info.pInput_CallBack = pInputFunc;
	s_mex_edit_win_info.iAppId = mex_GetCurrentApp();
	s_mex_edit_win_info.uInput_type = MMK_CREATE_DIGITALEDITBOX;
	if(pContent != PNULL)
	{  
		s_mex_edit_win_info.pContent = (uint8 *)mex_malloc( (iMaxNum+1)<<1 );
		mex_memcpy( (void *)s_mex_edit_win_info.pContent, (const void *)pContent, iMaxNum<<1 );
	}
	else
	{
		s_mex_edit_win_info.pContent = NULL;
	}
	if(bIsPassword)//密码框
	{
		s_mex_edit_win_info.uInput_type = MMK_CREATE_PASSWORDEDITBOX;
		MET_MEX_INPUT_WIN_TAB = MET_MEX_INPUT_WIN_TAB1;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS]=MMK_CREATE_TEXTEDITBOX;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+5]=iMaxNum;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+6]=iMaxNum;

		password_input_string.wstr_ptr = mex_malloc((iMaxNum+1)<<1);

	}
	else if(E_MITT_NUMERIC == iInputType  && !bCanChange)//数字编辑框不允许书改变输入法
	{
		s_mex_edit_win_info.uInput_type =MMK_CREATE_DIGITALEDITBOX;
		MET_MEX_INPUT_WIN_TAB=MET_MEX_INPUT_WIN_TAB2;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS]=s_mex_edit_win_info.uInput_type;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+5]=iMaxNum;
	}
	else
	{
		s_mex_edit_win_info.uInput_type =MMK_CREATE_TEXTEDITBOX;
		MET_MEX_INPUT_WIN_TAB=MET_MEX_INPUT_WIN_TAB3;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS]=s_mex_edit_win_info.uInput_type;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+5]=iMaxNum;
		MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+6]=iMaxNum;

		if(iInputType == E_MITT_NUMERIC)//数字
		{
			MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+8]=IM_DIGITAL_MODE;
			IM_SetCurInputMode(IM_DIGITAL_MODE);
		}
		else if( iInputType == E_MITT_ALPHANUMERIC_UPPERCASE || iInputType== E_MITT_ALPHANUMERIC_LOWERCASE)//字母或数字
		{
			MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+8]=IM_MULTITAP_MODE;
			IM_SetCurInputMode(IM_MULTITAP_MODE);
		}

		else if(iInputType == E_MITT_SM_STROKE)//汉语笔画
		{
			MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+8]=IM_STROKE_MODE;
			IM_SetCurInputMode(IM_STROKE_MODE);
		}

		else if(iInputType == E_MITT_SM_PINYIN ||iInputType == E_MITT_SM_MULTITAP_PINYIN)// 拼音
		{
			MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+8]=IM_SMART_MODE;  
			IM_SetCurInputMode(IM_SMART_MODE);
		}

		else 
		{
			MET_MEX_INPUT_WIN_TAB[MET_MEX_INPUT_MODIFY_POS+8]=IM_ENGLISH_MODE;  // 英文
			IM_SetCurInputMode(IM_ENGLISH_MODE);
		}

	}

	MMK_CreateWin(MET_MEX_INPUT_WIN_TAB,NULL);

	if(pInputTitle)
	{

		GUIWIN_SetTitleText(MET_MEX_INPUT_WIN_ID,(wchar*)pInputTitle,(uint16)MMIAPICOM_Wstrlen((wchar*)(pInputTitle)),FALSE);

	}

	MMK_SetAtvCtrl(MET_MEX_INPUT_WIN_ID,MET_MEX_INPUT_CTRL_ID);

	if(s_mex_edit_win_info.uInput_type == MMK_CREATE_DIGITALEDITBOX)
	{
		if (s_mex_edit_win_info.pContent && CheckDigitalStr((uint16 *)s_mex_edit_win_info.pContent,(uint16)MMIAPICOM_Wstrlen(( const wchar* )s_mex_edit_win_info.pContent)))
		{  
			GUIEDITBOX_SetDigitalEditBoxStringInfo(
				MET_MEX_INPUT_CTRL_ID,
				(uint16 *)s_mex_edit_win_info.pContent,
				(uint16)MMIAPICOM_Wstrlen(( const wchar* )s_mex_edit_win_info.pContent)
				);
		}

	}
	else if(s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
	{
		if (s_mex_edit_win_info.pContent)
		{
			mex_memset16(password_input_string.wstr_ptr,'*',(uint16)MMIAPICOM_Wstrlen(( const wchar* )s_mex_edit_win_info.pContent));
			password_input_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(( const wchar* )s_mex_edit_win_info.pContent);

			GUIEDITBOX_SetTextEditBoxStringInfo(
				MET_MEX_INPUT_CTRL_ID,
				password_input_string.wstr_ptr,
				password_input_string.wstr_len
				);
			mex_wcscpy(password_input_string.wstr_ptr,(uint16 *)s_mex_edit_win_info.pContent);

		}

	}
	else
	{
		if (s_mex_edit_win_info.pContent)
		{ 
			GUIEDITBOX_SetTextEditBoxStringInfo(
				MET_MEX_INPUT_CTRL_ID,
				(uint16 *)s_mex_edit_win_info.pContent,
				(uint16)MMIAPICOM_Wstrlen(( const wchar* )s_mex_edit_win_info.pContent)
				);
		}

	}
#endif	
	return true;

#else
	mex_log(MEX_LOG_SYSTEM,"create input box wrong");
    return false;
#endif

}

#if defined(MEX_PLATFORM__FOR_SPRD6600L)
uint8 g_RefreshTimer = 0;

void Refresh_Input_Buffer(MMI_WIN_ID_T win_id)
{
	bool bNeedUpdate = false;
	GUI_EDITBOX_CTRL_T *s_editbox_ptr;//当前编辑控件EDITBOX指针
	IM_TXT_INFO_T text_info = { 0 };
	int32 i=0,j=0;
#if (MEX_SPRD_CODE_VERSION<0x09A1109)
	MMIAPIIM_GetTextInfo(&text_info);
	//获取当前编辑框控件
	s_editbox_ptr = (GUI_EDITBOX_CTRL_T *)MMIAPIIM_GetEditboxPtr();


	if (s_editbox_ptr->cur_cursor_pos<text_info.text_len && text_info.text_len!=password_input_string.wstr_len)
	{
		if (text_info.text_len>password_input_string.wstr_len)//为增加
		{
			for(j=password_input_string.wstr_len;j>=s_editbox_ptr->cur_cursor_pos;j--)
				password_input_string.wstr_ptr[j]=password_input_string.wstr_ptr[j-1];

		}
		else
		{
			for(j=s_editbox_ptr->cur_cursor_pos;j<password_input_string.wstr_len;j++)
				password_input_string.wstr_ptr[j]=password_input_string.wstr_ptr[j+1];

		}
		password_input_string.wstr_ptr[text_info.text_len] =L'\0';
		bNeedUpdate = true;
	}

	if(!s_editbox_ptr->is_shade||password_input_string.wstr_len<text_info.text_len)//转换字符
	{ 
		for(i=0;i<text_info.text_len;i++)
		{

			if ('*' != text_info.text_ptr[i])
			{
				if (s_editbox_ptr->is_shade && i == s_editbox_ptr->cur_cursor_pos - 1)
					continue;
				password_input_string.wstr_ptr[i] = text_info.text_ptr[i];
				text_info.text_ptr[i] = '*';
			}
			else //如果输入的刚好是*的情况
			{
				if (((s_editbox_ptr->cur_cursor_pos - 1)  == i)
					&&(password_input_string.wstr_len < text_info.text_len))
				{
					password_input_string.wstr_ptr[i] = text_info.text_ptr[i];
				}
			}
		}
		password_input_string.wstr_len = text_info.text_len;
		password_input_string.wstr_ptr[text_info.text_len] =L'\0';
		bNeedUpdate = true;
	}

	if (bNeedUpdate)
	{
		if(s_editbox_ptr->str_ptr!=NULL && text_info.text_ptr!=NULL)
		{ 
			mex_wcscpy(s_editbox_ptr->str_ptr,text_info.text_ptr);
			MMK_UpdateScreen();
		}

	}
#else

	//获取当前编辑框控件
	s_editbox_ptr = (GUI_EDITBOX_CTRL_T *)MMIAPIIM_GetEditboxPtr();

	if((password_input_string.wstr_len!=s_editbox_ptr->cur_str_len)||(!s_editbox_ptr->is_shade))//转换字符
	{ 
		if (s_editbox_ptr->cur_str_len>password_input_string.wstr_len)//为增加
		{
			for(j=password_input_string.wstr_len;j>=s_editbox_ptr->cur_cursor_pos;j--)
				password_input_string.wstr_ptr[j]=password_input_string.wstr_ptr[j-1]; 
		}
		else if(s_editbox_ptr->cur_str_len<password_input_string.wstr_len)
		{
			for(j=s_editbox_ptr->cur_cursor_pos;j<password_input_string.wstr_len;j++)
				password_input_string.wstr_ptr[j]=password_input_string.wstr_ptr[j+1];
		}

		for(i=0;i<s_editbox_ptr->cur_str_len;i++)
		{
			if (L'*' != s_editbox_ptr->str_ptr[i])
			{
				password_input_string.wstr_ptr[i] = s_editbox_ptr->str_ptr[i];
				if ((s_editbox_ptr->is_shade)&&((s_editbox_ptr->cur_cursor_pos - 1)  == i))//如果当前字符处于阴影状态则不转换
					continue;
				else
                {
                    s_editbox_ptr->str_ptr[i] = L'*';
                    bNeedUpdate = true;
                }
			}
			else //如果输入的刚好是*的情况
			{
				if (((s_editbox_ptr->cur_cursor_pos - 1)  == i)
					&&(password_input_string.wstr_len < s_editbox_ptr->cur_str_len))
				{
					password_input_string.wstr_ptr[i] = s_editbox_ptr->str_ptr[i];
				}
			}
		}

		if(password_input_string.wstr_len!=s_editbox_ptr->cur_str_len)
		{
		    password_input_string.wstr_len = s_editbox_ptr->cur_str_len;
		    password_input_string.wstr_ptr[s_editbox_ptr->cur_str_len] =L'\0';
		    bNeedUpdate = true;
		}
	}

	if (bNeedUpdate )
	{
		extern PUBLIC void GUIEDITBOX_DrawEditBoxForMex(GUI_EDITBOX_CTRL_T* editbox_ptr,BOOLEAN is_draw_all,BOOLEAN is_full_paint );
		//  MMK_UpdateScreen();
		// MMK_SendMessage(MMK_GetCtrlHandleByWin(win_id, MET_MEX_INPUT_CTRL_ID), MSG_CTL_PAINT, PNULL );
		GUIEDITBOX_DrawEditBoxForMex(s_editbox_ptr,TRUE,TRUE);
	}
#endif

}
#endif
/*****************************************************************************/
//  Description : Handle_Met_Mex_InputWindow
//       
//  Global resource dependence : 
//   Author:       Wen.Liu
//   RETRUN:   
//  Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E  Handle_Met_Mex_InputBox(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id, 
	DPARAM param)
{
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
	MMI_RESULT_E			recode = MMI_RESULT_TRUE;
	//MMI_STRING_T			text = {0};
	MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MET_MEX_INPUT_CTRL_ID);
	int32 iOldApp;	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		//注:editbox的参数设置顺序不可随意调换!!!
		switch(s_mex_edit_win_info.uInput_type)
		{
		case GUIEDIT_TYPE_DIGITAL:
			//GUIEDIT_SetMaxLen(edit_ctrl_handle, s_mex_edit_win_info.iMaxNum);
			//GUIEDIT_SetString(edit_ctrl_handle, mex_tstr2wcs(s_mex_edit_win_info.pContent), mex_strlen(s_mex_edit_win_info.pContent));
			GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
		#if (MEX_SPRD_CODE_VERSION >= 0x10A1137)	
            GUIEDIT_SetImTag(edit_ctrl_handle, GUIIM_TAG_PURE_NUM);
		#endif	
			break;

		case GUIEDIT_TYPE_PASSWORD:
			{
				BOOLEAN is_disp_im_icon = TRUE; 

				GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);  /*lint !e655*/
				//GUIEDIT_SetMaxLen(edit_ctrl_handle, s_mex_edit_win_info.iMaxNum);
				GUIEDIT_SetStyle(edit_ctrl_handle, GUIEDIT_STYLE_MULTI);//多行
				GUIEDIT_SetAlign(edit_ctrl_handle, ALIGN_LEFT);//靠左上起
				GUIEDIT_SetDispImIcon(edit_ctrl_handle, &is_disp_im_icon, PNULL);//显示输入法的icon
				//GUIEDIT_SetString(edit_ctrl_handle, mex_tstr2wcs(s_mex_edit_win_info.pContent), mex_strlen(s_mex_edit_win_info.pContent));
			}
			break;

			//case MMIQQ_TM_ALL:
		default:
			GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_SET_ALL, GUIIM_TYPE_MULTITAP);
			//GUIEDIT_SetMaxLen(edit_ctrl_handle, s_mex_edit_win_info.iMaxNum);
			//GUIEDIT_SetString(edit_ctrl_handle, mex_tstr2wcs(s_mex_edit_win_info.pContent), mex_strlen(s_mex_edit_win_info.pContent));
			break;
		}
		GUIEDIT_SetMaxLen(edit_ctrl_handle, s_mex_edit_win_info.iMaxNum);
		GUIEDIT_SetString(edit_ctrl_handle, (uint16 *)(s_mex_edit_win_info.pContent), (uint16)MMIAPICOM_Wstrlen(( const wchar* )s_mex_edit_win_info.pContent));
		MMK_SetAtvCtrl(win_id, edit_ctrl_handle);
		break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:
	case MSG_APP_OK:
#if defined(PDA_UI_SUPPORT_ANDROID) || defined(MMI_PDA_SUPPORT)
	case MSG_APP_MENU:
#endif 
		s_mex_edit_win_info.bIsFirm = true;//confrim input
		GUIEDIT_GetString(edit_ctrl_handle,  &s_mex_edit_win_info.sText);
		MMK_CloseWin(win_id);	
		break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:

		s_mex_edit_win_info.bIsFirm = false;//cancel input


		MMK_CloseWin(win_id);//返回上一级
		break;
	case MSG_GET_FOCUS:
		break;

	case MSG_CLOSE_WINDOW:

		if(s_mex_edit_win_info.pInput_CallBack != PNULL)//处理数据
		{
			//s_mex_edit_win_info.sText.wstr_ptr[s_mex_edit_win_info.sText.wstr_len] = '\0';
			iOldApp = mex_GetCurrentApp();
			mex_SetCurrentApp( s_mex_edit_win_info.iAppId );
		#ifdef MEX_G7_UI_SUPPORT
			mex_SetManCurAppId(s_mex_edit_win_info.iAppId);
		#endif
			s_mex_edit_win_info.pInput_CallBack((uint8 *)(s_mex_edit_win_info.sText.wstr_ptr), s_mex_edit_win_info.bIsFirm);
			mex_SetCurrentApp( iOldApp );	
		#ifdef MEX_G7_UI_SUPPORT
			mex_SetManCurAppId(iOldApp);
		#endif
		}

		if(s_mex_edit_win_info.pContent!=NULL)
		{
			iOldApp = mex_GetCurrentApp();
			mex_SetCurrentApp( s_mex_edit_win_info.iAppId );
		#ifdef MEX_G7_UI_SUPPORT
			mex_SetManCurAppId(s_mex_edit_win_info.iAppId);
		#endif
			mex_free(s_mex_edit_win_info.pContent);  /*lint !e611 */
			mex_SetCurrentApp( iOldApp );	
		#ifdef MEX_G7_UI_SUPPORT
			mex_SetManCurAppId(iOldApp);
		#endif
		}
		break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

	return recode;

#elif defined(MEX_PLATFORM__FOR_SPRD6600L)

	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

	MMI_CTRL_ID_T    ctrl_id= MET_MEX_INPUT_CTRL_ID;
	// MMI_STRING_T   input_string = {0};
	MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MET_MEX_INPUT_CTRL_ID);
	int32 iOldApp;

	extern GUI_EDITBOX_CTRL_T* editbox_ptr;

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		if(s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
		{
			g_RefreshTimer = MMK_CreateTimer(200, true);
		}	

		break;
	case MSG_TIMER:
		if(s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
		{
			Refresh_Input_Buffer(win_id);
		}	
		break;
#if (MEX_SPRD_CODE_VERSION>=0x09A1109)	
	case MSG_GET_FOCUS:
		if((s_mex_edit_win_info.uInput_type==MMK_CREATE_PASSWORDEDITBOX) && (g_RefreshTimer==0))
		{
			g_RefreshTimer = MMK_CreateTimer(200, true);
		}	
		break;
	case MSG_LOSE_FOCUS:
		if((s_mex_edit_win_info.uInput_type==MMK_CREATE_PASSWORDEDITBOX) && (g_RefreshTimer!=0))
		{
			MMK_StopTimer(g_RefreshTimer);
			g_RefreshTimer = 0;
		}
		break;
#endif
	case MSG_FULL_PAINT:
		break;
	case MSG_KEYUP_1:
	case MSG_KEYUP_2:
	case MSG_KEYUP_3:
	case MSG_KEYUP_4:
	case MSG_KEYUP_5:
	case MSG_KEYUP_6:
	case MSG_KEYUP_7:
	case MSG_KEYUP_8:
	case MSG_KEYUP_9:
	case MSG_KEYUP_0:

		if (s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
		{
			//Refresh_Input_Buffer(win_id);
		}

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:
	case MSG_APP_OK:

		s_mex_edit_win_info.bIsFirm = true;//confrim input

		//to get the input number
		if(s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
		{
			password_input_string.wstr_ptr[password_input_string.wstr_len]='\0';
		}
		else if ( s_mex_edit_win_info.uInput_type == MMK_CREATE_DIGITALEDITBOX)
		{
			GUIEDITBOX_GetDigitalEditBoxInfo(ctrl_id, &s_mex_edit_win_info.sText);
		} 
		else
		{
			GUIEDITBOX_GetTextEditBoxInfo(ctrl_id, &s_mex_edit_win_info.sText);
		}  

		MMK_CloseWin(win_id);
		break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:

		s_mex_edit_win_info.bIsFirm = false;//cancel input

		if(s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
		{
			if (password_input_string.wstr_len>0)
			{
				password_input_string.wstr_len--;
			}

			mex_log(MEX_LOG_SYSTEM,"passwordlen:%d",password_input_string.wstr_len);
		}	

		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:

		if (s_mex_edit_win_info.pInput_CallBack != NULL)
		{
			iOldApp = mex_GetCurrentApp();
			mex_SetCurrentApp( s_mex_edit_win_info.iAppId );

			if (s_mex_edit_win_info.uInput_type == MMK_CREATE_PASSWORDEDITBOX)
			{
				s_mex_edit_win_info.pInput_CallBack((uint8 *)password_input_string.wstr_ptr,s_mex_edit_win_info.bIsFirm);

				MMK_StopTimer(g_RefreshTimer);

			} 
			else
			{ 
                wchar *wstr_buf = mex_malloc(s_mex_edit_win_info.sText.wstr_len * 2 + 2);
                if(PNULL != wstr_buf)
                {
                    memset(wstr_buf,0,s_mex_edit_win_info.sText.wstr_len * 2 + 2);
                    memcpy(wstr_buf,s_mex_edit_win_info.sText.wstr_ptr,s_mex_edit_win_info.sText.wstr_len * 2);

    				s_mex_edit_win_info.pInput_CallBack((uint8 *)wstr_buf,s_mex_edit_win_info.bIsFirm);
                    mex_free(wstr_buf);
                }
                else
                {
                    mex_log(MEX_LOG_SYSTEM,"input callback buffer is NULL");
    				s_mex_edit_win_info.pInput_CallBack(NULL,false);
                }

			}
			mex_SetCurrentApp( iOldApp );
		}

		if(password_input_string.wstr_ptr!=NULL)
		{
			iOldApp = mex_GetCurrentApp();
			mex_SetCurrentApp( s_mex_edit_win_info.iAppId );
			mex_free(password_input_string.wstr_ptr);
			password_input_string.wstr_ptr = NULL;
			mex_SetCurrentApp( iOldApp );
		}
		if(s_mex_edit_win_info.pContent!=NULL)
		{
			iOldApp = mex_GetCurrentApp();
			mex_SetCurrentApp( s_mex_edit_win_info.iAppId );
			mex_free(s_mex_edit_win_info.pContent);
			s_mex_edit_win_info.pContent = NULL;
			mex_SetCurrentApp( iOldApp );
		}

		// MMK_CloseWin(win_id);

		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

	return recode;

#endif
}


//================================================Inputbox end======================= 

//-----------------定义SCC Platform 窗口响应函数-----------------------------
LOCAL MMI_RESULT_E HandleSSCMexPlatform( 
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;

	switch(msg_id)
	{
	case MSG_FULL_PAINT:
		{
			char tempStr[1024] = {0};
			wchar showStr[1024] = {0};
			int16 iDownloaderVersion	=0;
			int16 iTinyManagerVersion	=0;
			int16 iFullManagerVersion	=0;
			int16 iAppGetterVersion		=0;
			MMI_STRING_T textString		= {0};

			mex_GetAppInfoFromFile( L"", L"MexDownloader.mex", true, NULL, &iDownloaderVersion );
			mex_GetAppInfoFromFile( L"", L"MexManagerTiny.mex", true, NULL, &iTinyManagerVersion );
			mex_GetAppInfoFromFile( L"", L"MexManager.mex", true, NULL, &iFullManagerVersion );
			mex_GetAppInfoFromFile( L"MexAppGetter", L"MexAppGetter.mex", false, NULL, &iAppGetterVersion );

			sprintf(tempStr,"MEX V:%s_%s\n渠道号:%d\n机型:%s\n机器编号:%d\nBuild: %s\n(C) 2010 MeTek \nLogEnable %d\nDownloader %d\nTiny %d\nManager %d\nAppGetter %d\nAppMEM %d\nIntMem %d\nExtMem %d\nReservedMem %d",
				MexSystem_GetSystemVersion(),MexSystem_GetPatchVersion(),MexConfig_GetPhoneConfig(E_MPC_MexVenderID) ,(char *)MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA),MexConfig_GetPhoneConfig(E_MPC_MexMachineID),MexSystem_GetBulidDate(),(g_MexLogFilter != 0 ? 1 : 0),
				iDownloaderVersion, iTinyManagerVersion, iFullManagerVersion, iAppGetterVersion, MexConfig_GetAppMemSize(), MexConfig_GetPhoneConfig(E_MPC_MaxIntMem), MexConfig_GetPhoneConfig(E_MPC_MaxExtMem), MexConfig_GetPhoneConfig(E_MPC_MaxReservedMem) );
			mex_str2nwcs(showStr,2048,tempStr);
			textString.wstr_ptr =showStr;
			textString.wstr_len = MMIAPICOM_Wstrlen(showStr);

			MMK_SetAtvCtrl(win_id,MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID);
			{			
#if (MEX_SPRD_CODE_VERSION >= 0x09AFFFF)
				GUI_FONT_T font = CAF_FONT_20;
				GUITEXT_SetFont(MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID,&font,PNULL);
				GUITEXT_SetString(MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID,textString.wstr_ptr,textString.wstr_len,FALSE);
#else
				GUI_RECT_T titleRect = MMI_TITLE_RECT_UNDER_STB;
				GUIWIN_SetTitleRect(win_id, titleRect); 
				GUITEXTBOX_SetText(MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID,&textString,FALSE);
#endif
			}

		}
		break;
	case MSG_CLOSE_WINDOW:
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:	
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527                   
#endif
#if defined(PDA_UI_SUPPORT_ANDROID) || defined(MMI_PDA_SUPPORT)
	case MSG_APP_MENU:
#endif 
	case MSG_APP_OK:
	case MSG_CTL_OK:
		MMK_CreateWin((uint32*)MET_MEX_PLATFORM_ENGINEERMODE_WIN_TAB,PNULL);
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}
//into mex Engineer Mode
PUBLIC BOOLEAN MetMex_ShowPlatformEngineerMode(void)
{
	return MMK_CreateWin((uint32*)MET_MEX_SSC_PLATFORM_WIN_TAB,PNULL);
}
//mex engineer mode win func
#if (defined(MEX_PLATFORM__FOR_SPRD6600L))
#ifdef AUTO_APP_TEST
LOCAL MMI_RESULT_E  HandleAppNameWinMsg(
                                        MMI_WIN_ID_T		win_id, 	
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM				param
                                        );

WINDOW_TABLE(APP_NAMEWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleAppNameWinMsg ),
    WIN_ID( MMIAPP_NAMEWIN_ID ),
    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXTEDITBOX_CTRL(
					MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT, MMI_EDITBOX_FULLSCREEN_CLIENT_TOP-1, MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT ,
					MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM,
					20, 20, 
					IM_DEFAULT_ALL_INPUT_MODE_SET, 
					IM_CHARACTER_MODE,
					MET_MEX_APP_SETNAME_CTRL_ID
                    ), 
     END_WIN
}; 
LOCAL MMI_RESULT_E  HandleAppNameWinMsg(
                                        MMI_WIN_ID_T		win_id, 	
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM				param
                                        )
{
    MMI_RESULT_E                    recode = MMI_RESULT_TRUE;
    MMI_STRING_T          str = {0}; 
    MMI_CTRL_ID_T                   current_ctrl_id = MET_MEX_APP_SETNAME_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIEDITBOX_SetTextEditBoxStringInfo(
                current_ctrl_id,
                AppNameString,
                MMIAPICOM_Wstrlen(AppNameString)           
                );
        GUIEDITBOX_EnableDisplayRect(current_ctrl_id, FALSE);
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
	    GUIWIN_SetTitleTextId(win_id, TXT_MEX_APP_NAME, FALSE); 
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif  //TOUCH_PANEL_SUPPORT //IGNORE9527                      
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:        
	case MSG_APP_WEB:
        // get string info
        SCI_MEMSET(AppNameString,0,200);
        GUIEDITBOX_GetTextEditBoxInfo(current_ctrl_id, &str);
        MMI_WSTRNCPY(
            AppNameString, 100,
            str.wstr_ptr, str.wstr_len,
            str.wstr_len);
        MMK_CloseWin(win_id);
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
LOCAL void EditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);

    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}
#endif
#endif
LOCAL MMI_RESULT_E HandleMexPlatformEngineerMode( 
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id, 
	DPARAM param
	)
{
	MMI_RESULT_E		recode = MMI_RESULT_TRUE;

#if 0 //defined(MEX_PLATFORM__FOR_SPRD6800H)
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T          bk_rect      = MMITHEME_GetClientRect();
	MMI_STRING_T        string = {0};
	MMI_TEXT_ID_T       item_arry[2] = {TXT_OPEN, TXT_CLOSE};
#if (MEX_SPRD_CODE_VERSION == 0x10A1116)
	MMI_STRING_T      item_str[2]={0};
#else
	GUIDROPDOWNLIST_ITEM_T item_str[2] = {0};
#endif
	GUI_BOTH_RECT_T     both_client_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id)); 
	GUI_POINT_T         c_center = {0};
	GUI_RECT_T          t_rect = {0} ;
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			int32 i = 0;
			//显示Label
			MMI_STRING_T logStr = {0};
			MMI_STRING_T verderStr = {0};
			MMI_STRING_T phoneUAStr = {0};
			GUI_BG_T        edit_bg = {0};
			GUI_BG_T        edit_bg1 = {0};
#if (MEX_SCREEN_ESPECIAL_VERSION)
			GUI_BG_T        edit_bgWidth = {0};
			GUI_BG_T        edit_bgHeight = {0};
#endif
			//GUI_BORDER_T    edit_border = {0};

			char *pVenderStr = (char*)SCI_ALLOC(20);

			if(SCI_NULL == pVenderStr)
				break;
			SCI_MEMSET(pVenderStr,0,20);


			if (MMITHEME_IsMainScreenLandscape())
			{
				c_center.x = (both_client_rect.h_rect.right-both_client_rect.h_rect.left)/2;
				c_center.y = (both_client_rect.h_rect.bottom-both_client_rect.h_rect.top)/2+both_client_rect.h_rect.top;
				mex_log(MEX_LOG_SYSTEM,"MainScreenLandscape center.x:%d  c_center.y:%d",c_center.x,c_center.y);
			}
			else
			{
				c_center.x = (both_client_rect.v_rect.right-both_client_rect.v_rect.left)/2;
				c_center.y = (both_client_rect.v_rect.bottom-both_client_rect.v_rect.top)/2+both_client_rect.h_rect.top;
				mex_log(MEX_LOG_SYSTEM,"not MainScreenLandscape center.x:%d  c_center.y:%d",c_center.x,c_center.y);
			}



			//设置控件的位置
			t_rect.left = c_center.x-MEX_ENG_GAP/2-MEX_ENG_COMMON_LENGTH;
			t_rect.right = c_center.x-MEX_ENG_GAP/2;
			t_rect.top = c_center.y-2*MEX_ENG_GAP-MEX_ENG_COMMON_HEIGHT/2-2*MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID,&t_rect,FALSE);
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID,&t_rect,FALSE);
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID,&t_rect,FALSE);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID,&t_rect,FALSE);
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID,&t_rect,FALSE);
#endif

			t_rect.left = c_center.x+MEX_ENG_GAP/2;
			t_rect.right = c_center.x+MEX_ENG_GAP/2+MEX_ENG_COMMON_LENGTH;
			t_rect.top = c_center.y-2*MEX_ENG_GAP-MEX_ENG_COMMON_HEIGHT/2-2*MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUIDROPDOWNLIST_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID,&t_rect);
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&t_rect);
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&t_rect);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,&t_rect);
			t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
			t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
			GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,&t_rect);
#endif

			GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_LOG_LABEL,FALSE);
			GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_VENDER_LABEL,FALSE);
			GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_PHONEUA_LABEL,FALSE);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_WIDTH_LABEL,FALSE);
			GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_HEIGHT_LABEL,FALSE);
#endif

			for (i=0;i<2;i++)//set item text
			{
				SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
				MMI_GetLabelTextByLang(item_arry[i], &string);
#if (MEX_SPRD_CODE_VERSION == 0x10A1116)
				item_str[i].wstr_ptr=string.wstr_ptr;
				item_str[i].wstr_len= string.wstr_len;
#else
				MMI_WSTRNCPY( item_str[i].wstr, DROPDOWNLIST_STRING_MAX_NUM, string.wstr_ptr, string.wstr_len, string.wstr_len );
				item_str[i].wstr_len = string.wstr_len;
#endif
			}

			//set item
			GUIDROPDOWNLIST_AppendItemArray( MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID, item_str, 2 );

			//set current selected item
			if(g_MexLogFilter == -1)
				GUIDROPDOWNLIST_SetCurItemIndex( MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID, 0 );
			else
				GUIDROPDOWNLIST_SetCurItemIndex( MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID, 1 );
			//set edit box type
			GUIEDIT_SetStyle(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,GUIEDIT_STYLE_SINGLE);
			GUIEDIT_SetStyle(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,GUIEDIT_STYLE_SINGLE);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			GUIEDIT_SetStyle(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,GUIEDIT_STYLE_SINGLE);
			GUIEDIT_SetStyle(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,GUIEDIT_STYLE_SINGLE);
#endif
			//set edit box back color
			edit_bg.bg_type = GUI_BG_COLOR;
			edit_bg.color   = MMI_WHITE_COLOR;
			edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
			GUIEDIT_SetBg(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&edit_bg);
			edit_bg1.bg_type = GUI_BG_COLOR;
			edit_bg1.color   = MMI_WHITE_COLOR;
			edit_bg1.shape   = GUI_SHAPE_ROUNDED_RECT;
			GUIEDIT_SetBg(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&edit_bg1);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			edit_bgWidth.bg_type = GUI_BG_COLOR;
			edit_bgWidth.color   = MMI_WHITE_COLOR;
			edit_bgWidth.shape   = GUI_SHAPE_ROUNDED_RECT;
			GUIEDIT_SetBg(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,&edit_bgWidth);
			edit_bgHeight.bg_type = GUI_BG_COLOR;
			edit_bgHeight.color   = MMI_WHITE_COLOR;
			edit_bgHeight.shape   = GUI_SHAPE_ROUNDED_RECT;
			GUIEDIT_SetBg(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,&edit_bgHeight);
#endif

			//set border
			/*  
			edit_border.type  = GUI_BORDER_SROUNDED;
			edit_border.width = 2;
			edit_border.color = MMI_BLACK_COLOR;
			GUIEDIT_SetBorder(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&edit_border);
			GUIEDIT_SetBorder(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&edit_border);
			*/
			//set font
			GUIEDIT_SetFontColor(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,MMI_BLACK_COLOR);
			GUIEDIT_SetFontColor(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,MMI_BLACK_COLOR);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			GUIEDIT_SetFontColor(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,MMI_BLACK_COLOR);
			GUIEDIT_SetFontColor(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,MMI_BLACK_COLOR);
#endif
			//set edit box initial str
			sprintf(pVenderStr,"%d",MexConfig_GetPhoneConfig(E_MPC_MexVenderID));
			GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
			SCI_MEMSET(pVenderStr,0,20);
			sprintf(pVenderStr,"%s",MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA));
			GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
#if (MEX_SCREEN_ESPECIAL_VERSION)
			{
				extern uint16 g_now_screen_width;
				extern uint16 g_now_screen_height;
				MexSystem_LoadSpacialScreenConfig();
				SCI_MEMSET(pVenderStr,0,20);
				sprintf(pVenderStr,"%d",g_now_screen_width);
				GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
				SCI_MEMSET(pVenderStr,0,20);
				sprintf(pVenderStr,"%d",g_now_screen_height);
				GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
			}
#endif

			MMK_SetAtvCtrl(win_id,MMK_GetCtrlHandleByWin(win_id, MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID));
			//set edit im is enable tp
			GUIEDIT_SetImTp(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,FALSE);
			GUIEDIT_SetImTp(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,FALSE);
#if (MEX_SCREEN_ESPECIAL_VERSION)
			GUIEDIT_SetImTp(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,FALSE);
			GUIEDIT_SetImTp(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,FALSE);
#endif
			if(pVenderStr)
				SCI_FREE(pVenderStr);
		}
		break;
	case MSG_LCD_SWITCH:
		if (MMITHEME_IsMainScreenLandscape())
		{
			c_center.x = (both_client_rect.h_rect.right-both_client_rect.h_rect.left)/2;
			c_center.y = (both_client_rect.h_rect.bottom-both_client_rect.h_rect.top)/2+both_client_rect.h_rect.top;
			mex_log(MEX_LOG_SYSTEM,"MainScreenLandscape center.x:%d  c_center.y:%d",c_center.x,c_center.y);
		}
		else
		{
			c_center.x = (both_client_rect.v_rect.right-both_client_rect.v_rect.left)/2;
			c_center.y = (both_client_rect.v_rect.bottom-both_client_rect.v_rect.top)/2+both_client_rect.h_rect.top;
			mex_log(MEX_LOG_SYSTEM,"not MainScreenLandscape center.x:%d  c_center.y:%d",c_center.x,c_center.y);
		}

		//设置控件的位置
		//设置控件的位置
		t_rect.left = c_center.x-MEX_ENG_GAP/2-MEX_ENG_COMMON_LENGTH;
		t_rect.top = c_center.y-2*MEX_ENG_GAP-MEX_ENG_COMMON_HEIGHT/2-2*MEX_ENG_COMMON_HEIGHT;
		t_rect.right = c_center.x-MEX_ENG_GAP/2;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID,&t_rect,FALSE);
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID,&t_rect,FALSE);
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID,&t_rect,FALSE);
#if (MEX_SCREEN_ESPECIAL_VERSION)
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID,&t_rect,FALSE);
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUILABEL_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID,&t_rect,FALSE);
#endif

		t_rect.left = c_center.x+MEX_ENG_GAP/2;
		t_rect.right = c_center.x+MEX_ENG_GAP/2+MEX_ENG_COMMON_LENGTH;
		t_rect.top = c_center.y-2*MEX_ENG_GAP-MEX_ENG_COMMON_HEIGHT/2-2*MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUIDROPDOWNLIST_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID,&t_rect);
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&t_rect);
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&t_rect);
#if (MEX_SCREEN_ESPECIAL_VERSION)
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,&t_rect);
		t_rect.top	+= MEX_ENG_GAP + MEX_ENG_COMMON_HEIGHT;
		t_rect.bottom = t_rect.top+MEX_ENG_COMMON_HEIGHT;
		GUIEDIT_SetRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,&t_rect);
#endif
		break;
	case MSG_FULL_PAINT:
		GUIRES_DisplayImg(PNULL, &bk_rect, &bk_rect, win_id, IMAGE_COMMON_BG, &lcd_dev_info);

		break;

	case MSG_CLOSE_WINDOW:
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		{
			uint16 item;
			MMI_STRING_T strTemp1 = {0};
			MMI_STRING_T strTemp2 = {0};
#if (MEX_SCREEN_ESPECIAL_VERSION)
			MMI_STRING_T strWidth = {0};
			MMI_STRING_T strHeight = {0};
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,&strWidth);
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,&strHeight);
#endif

			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&strTemp1);
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&strTemp2);
			//VENDER and PHONEUA number can't be NULL
			if (0 == strTemp1.wstr_len)
			{            
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
				break;
			}
			else if (0 == strTemp2.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
				break;
			}
#if (MEX_SCREEN_ESPECIAL_VERSION)
			else if (0 == strWidth.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID);
				break;
			}
			else if (0 == strHeight.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID);
				break;
			}
#endif
			// set log flag
			item = GUIDROPDOWNLIST_GetCurItemIndex(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);
			if (0 == item) 
				g_MexLogFilter = -1;
			else
				g_MexLogFilter = 0;
#ifdef MEX_QA_MODE
			{
				extern int32 iMex_QA_VenderID;
				extern char pMex_QA_PhoneUA[10];

				iMex_QA_VenderID = atoi(mex_twcs2str(strTemp1.wstr_ptr));
				mex_wcs2nstr(pMex_QA_PhoneUA,strTemp2.wstr_len*sizeof(wchar_t),strTemp2.wstr_ptr);
			}
#endif
#if (MEX_SCREEN_ESPECIAL_VERSION)
			{
				extern uint16 g_now_screen_width;
				extern uint16 g_now_screen_height;
				int32 iScreenWidth = 0,iScreenHeight=0;
				iScreenWidth = mex_atoi( mex_twcs2str((wchar_t *)strWidth.wstr_ptr));
				iScreenHeight = mex_atoi( mex_twcs2str((wchar_t *)strHeight.wstr_ptr) );
				if( iScreenWidth != g_now_screen_width || iScreenHeight != g_now_screen_height )
				{
					g_now_screen_width = iScreenWidth<LCD_WIDTH?iScreenWidth:LCD_WIDTH;
					g_now_screen_height = iScreenHeight<LCD_HEIGHT?iScreenHeight:LCD_HEIGHT;
					Mex_SaveScreenConfig( g_now_screen_width, g_now_screen_height );
				}
			}
#endif
			MMK_CloseWin(win_id);
		}
		break;
	case MSG_APP_WEB:

		break;

	case MSG_KEYUP_UP:
		{

			MMI_CTRL_ID_T activeID = MMK_GetActiveCtrlId(win_id);
			MMI_HANDLE_T  cur_ctrl_handle = 0 ;
			GUIDROPDOWNLIST_CTRL_T* ctrl_ptr =NULL ;

			if ((activeID-MET_MEX_CTRL_ID_ID_START)%2 == 1)//把label 去掉
			{
				activeID+=1;
			}
			if (MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID == activeID)
			{
				//cur_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, activeID) ;
				//ctrl_ptr = (GUIDROPDOWNLIST_CTRL_T*)MMK_GetCtrlPtr(cur_ctrl_handle);
				if (GUIDNLIST_OPEN_STATUS == GUIDROPDOWNLIST_GetCtrlStatus(cur_ctrl_handle))//如果下拉框展开则屏蔽此处的up和down消息
				{
					break;
				}
#if MEX_SCREEN_ESPECIAL_VERSION
				activeID=MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID;
#else		
				activeID=MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID;
#endif

			}
			else
			{
				activeID-=2;
			}
			cur_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, activeID);
			//set current active
			MMK_SetAtvCtrl(win_id,cur_ctrl_handle); 
			MMK_SendMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL); 
		}
		break;
	case MSG_KEYUP_DOWN:
		{
			MMI_CTRL_ID_T activeID = MMK_GetActiveCtrlId(win_id);
			MMI_HANDLE_T  cur_ctrl_handle = 0 ;
			//GUIDROPDOWNLIST_CTRL_T* ctrl_ptr =NULL ;

			if ((activeID-MET_MEX_CTRL_ID_ID_START)%2 == 1)
			{
				activeID+=1;
			}
#if MEX_SCREEN_ESPECIAL_VERSION
			if (activeID == MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID)
#else
			if (activeID == MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID)
#endif
			{
				activeID=MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID;
			}
			else
			{
				cur_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, activeID) ;
//				ctrl_ptr = (GUIDROPDOWNLIST_CTRL_T*)MMK_GetCtrlPtr(cur_ctrl_handle);
				if (GUIDNLIST_OPEN_STATUS == GUIDROPDOWNLIST_GetCtrlStatus(cur_ctrl_handle))//如果下拉框展开则屏蔽此处的up和down消息
				{
					break;
				}
				activeID+=2;
			}
			cur_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, activeID);
			//set current active
			MMK_SetAtvCtrl(win_id,cur_ctrl_handle); 
			MMK_SendMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL); 
		}
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:
		{
			mex_log(MEX_LOG_SYSTEM,"TP PRESS up");
			MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);
		}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
#endif
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
	uint32 i = 0;
	MMI_STRING_T itemStr = {0};
	GUISETLIST_ITEM_INFO_T item_info  ={0};
	MMI_TEXT_ID_T		item_arry[2] = {TXT_OPEN, TXT_CLOSE};
 switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
	   {
		char *pVenderStr = (char*)SCI_ALLOC(20);
		if(SCI_NULL == pVenderStr)
		break;
		SCI_MEMSET(pVenderStr,0,20);
        GUIWIN_SetTitleTextId(win_id,TXT_MEX_PLT_ENG_TITLE,FALSE);

        GUIFORM_SetStyle(MET_MEX_LOG_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_LOG_LABEL,FALSE);
         for(i=0;i<2;i++)
		{
		MMI_GetLabelTextByLang(item_arry[i], &itemStr);
		item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
		item_info.str_info.wstr_len = itemStr.wstr_len;
		item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
		GUISETLIST_AddItem(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID, &item_info);
		}
	    //set current selected item
		if(g_MexLogFilter == -1)
		GUISETLIST_SetCurIndex(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID, 0);
		else
		GUISETLIST_SetCurIndex(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID, 1);
		//GUISETLIST_SetCtrlState(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
				
		GUISETLIST_SetTitleTextId(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID, TXT_MEX_PLT_ENG_LOG_LABEL);
// log name setlist  end
	
		GUIFORM_SetStyle(MET_MEX_VENDER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_VENDER_LABEL,FALSE); 
		sprintf(pVenderStr,"%d",MexConfig_GetPhoneConfig(E_MPC_MexVenderID));
		GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
//vender end
        GUIFORM_SetStyle(MET_MEX_PHONEUA_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_PHONEUA_LABEL,FALSE); 
		SCI_MEMSET(pVenderStr,0,20);
		sprintf(pVenderStr,"%s",(char *)MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA));
		GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
//phoneua end
#if (MEX_SCREEN_ESPECIAL_VERSION)
        GUIFORM_SetStyle(MET_MEX_SCREEN_WIDTH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_WIDTH_LABEL,FALSE);
		{
		extern uint16 g_now_screen_width;
		SCI_MEMSET(pVenderStr,0,20);
		sprintf(pVenderStr,"%d",g_now_screen_width);
		GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
		}
//screen width end
       GUIFORM_SetStyle(MET_MEX_SCREEN_HEIGHT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	   GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID, TXT_MEX_PLT_ENG_HEIGHT_LABEL,FALSE);
		{
		extern uint16 g_now_screen_height;
		SCI_MEMSET(pVenderStr,0,20);
		sprintf(pVenderStr,"%d",g_now_screen_height);
		GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
		}	
//screen height end
#endif
#ifdef AUTO_APP_TEST
		GUIFORM_SetStyle(MET_MEX_APP_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_LABEL_CTRL_ID, TXT_MEX_APP_NAME,FALSE);
       {
	   		MMI_STRING_T str = {0};
			str.wstr_ptr = AppNameString;
			str.wstr_len = MMIAPICOM_Wstrlen(AppNameString);
	   GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,str.wstr_ptr,str.wstr_len);
		}
		GUIFORM_SetStyle(MET_MEX_APP_NUMBER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_LABEL_CTRL_ID, TXT_MEX_APP_NUMBER,FALSE);
		{
			MMI_STRING_T str1 = {0};
			str1.wstr_ptr = AppNumber;
			str1.wstr_len = MMIAPICOM_Wstrlen(AppNumber);
			GUIEDIT_SetString(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,str1.wstr_ptr,str1.wstr_len);
		}
		GUIFORM_SetStyle(MET_MEX_START_TEST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID, TXT_MEX_APP_START_TEST,FALSE);	
#endif 
     MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_MODE_FORM_CTRL_ID);
	 GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
	if(pVenderStr)  /*lint !e774 */
	{
	SCI_FREE(pVenderStr);
	}
	  }
       break;
	   
    case MSG_FULL_PAINT:
		break;
		
	case MSG_CTL_SETLIST_SWITCH:
		   break;
		   
	case MSG_CLOSE_WINDOW:
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
	       break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527                  
#endif 
#if defined(PDA_UI_SUPPORT_ANDROID) || defined(MMI_PDA_SUPPORT)
		case MSG_APP_MENU:
#endif 
	case MSG_APP_OK:
	case MSG_CTL_OK:
		{
			uint16 item;
			MMI_STRING_T strTemp1 = {0};
			MMI_STRING_T strTemp2 = {0};
#ifdef AUTO_APP_TEST
			MMI_STRING_T strTemp3 = {0};
			MMI_STRING_T strTemp4 = {0};	
#endif
#if (MEX_SCREEN_ESPECIAL_VERSION)
			MMI_STRING_T strWidth = {0};
			MMI_STRING_T strHeight = {0};
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,&strWidth);
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,&strHeight);
#endif

			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&strTemp1);
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&strTemp2);

#ifdef AUTO_APP_TEST
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,&strTemp3);
			GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,&strTemp4);

if(0 == strTemp3.wstr_len||0 == strTemp4.wstr_len)
  {
#endif
			//VENDER and PHONEUA number can't be NULL
			if (0 == strTemp1.wstr_len)
			{			 
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
				break;
			}
			else if (0 == strTemp2.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
				break;
			}
#if (MEX_SCREEN_ESPECIAL_VERSION)
			else if (0 == strWidth.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID);
				break;
			}
			else if (0 == strHeight.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID);
				break;
			}
#endif
#ifdef AUTO_APP_TEST
			else if (0 == strTemp3.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID);
			}
			else if (0 == strTemp4.wstr_len)
			{
				MMK_SetAtvCtrl(win_id, MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID); 		
			}
#endif
#ifdef  AUTO_APP_TEST
}
#endif
			// set log flag
			item = GUISETLIST_GetCurIndex(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID);
			if (0 == item) 
				g_MexLogFilter = -1;
			else
				g_MexLogFilter = 0;
#ifdef MEX_QA_MODE
			{
				extern int32 iMex_QA_VenderID;
				extern char pMex_QA_PhoneUA[10];

				iMex_QA_VenderID = atoi(mex_twcs2str(strTemp1.wstr_ptr));
				mex_wcs2nstr(pMex_QA_PhoneUA,strTemp2.wstr_len*sizeof(wchar_t),strTemp2.wstr_ptr);
			}
#endif
#if (MEX_SCREEN_ESPECIAL_VERSION)
			{
				extern uint16 g_now_screen_width;
				extern uint16 g_now_screen_height;
				int32 iScreenWidth = 0,iScreenHeight=0;
				iScreenWidth = mex_atoi( mex_twcs2str((wchar_t *)strWidth.wstr_ptr));
				iScreenHeight = mex_atoi( mex_twcs2str((wchar_t *)strHeight.wstr_ptr) );
				if( iScreenWidth != g_now_screen_width || iScreenHeight != g_now_screen_height )
				{
					g_now_screen_width = iScreenWidth<LCD_WIDTH?iScreenWidth:LCD_WIDTH;
					g_now_screen_height = iScreenHeight<LCD_HEIGHT?iScreenHeight:LCD_HEIGHT;
					Mex_SaveScreenConfig( g_now_screen_width, g_now_screen_height );
				}
			}
#endif
#ifdef AUTO_APP_TEST
			{
				MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);
#ifdef AUTO_APP_TEST
	{
		SCI_MEMSET(AppNameString,0,200);
		SCI_MEMSET(AppNumber,0,20);
		GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,&strTemp3);
		MMI_WSTRNCPY(AppNameString, 100,strTemp3.wstr_ptr, strTemp3.wstr_len,strTemp3.wstr_len);
		GUIEDIT_GetString(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,&strTemp4);
		MMI_WSTRNCPY(AppNumber, 10,strTemp4.wstr_ptr, strTemp4.wstr_len,strTemp4.wstr_len);
	}
#endif	
				if(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID==atciveID)
				 {
				   {
					if(0!=strTemp3.wstr_len&&PNULL!=strTemp4.wstr_len)
					{
						uint16 index_len  = strTemp4.wstr_len;
						uint32 id_num = 0;
						char *appname =(char*)SCI_ALLOC(20);
						if(SCI_NULL == appname)
							break;
						SCI_MEMSET(appname,0,20);
						//将编辑框中的字符串转成ID号
						MMIAPICOM_WstrToNum(strTemp4.wstr_ptr, (uint8)index_len,&id_num);
						MMIAPICOM_WstrToStr(strTemp3.wstr_ptr,(uint8*)appname);
						MetMexTestEntryOpenWin(appname,id_num); 
						if(appname)  /*lint !e774 */
							SCI_FREE(appname);
					}
						
				}
				}
			}
#endif
			MMK_CloseWin(win_id);
		}
				break;
		
				
			case MSG_KEYUP_LEFT:
			case MSG_KEYUP_RIGHT:
				{
					if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID ==MMK_GetActiveCtrlId(win_id) )
					{	
						g_MexLogFilter = (g_MexLogFilter == -1 ? 0 : -1);
						MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);
					}
				}
				break;
		   default:
			   recode = MMI_RESULT_FALSE;
			   break;

 	}
#elif defined(MEX_PLATFORM__FOR_SPRD6600L)

	GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
	GUI_RECT_T rect = MMI_FULL_SCREEN_RECT;
#ifdef AUTO_APP_TEST 
		GUI_RECT_T edit_rect = {120,MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(5)+MEX_PLT_ENG_LINE_DISTANCE_N(5),182,\
		MEX_PLT_ENG_BEGIN_Y+MEX_PLT_ENG_FONT_HEIGHT_N(6)+MEX_PLT_ENG_LINE_DISTANCE_N(5)};
#endif
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
#if !(defined(MAINLCD_SIZE_320X240) && defined(AUTO_APP_TEST)) 
		GUI_RECT_T   title_rect = MMI_TITLE_RECT_UNDER_STB;
#else
		GUI_RECT_T	 title_rect = {0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMI_TITLE_HEIGHT - 1};
#endif
			char *pVenderStr = (char*)SCI_ALLOC(20);
			if(SCI_NULL == pVenderStr)
				break;
			SCI_MEMSET(pVenderStr,0,20);
			GUIWIN_SetTitleRect(win_id, title_rect); //set title position			
			{
				//显示Label
				MMI_STRING_T logStr = {0};
				MMI_STRING_T verderStr = {0};
				MMI_STRING_T phoneUAStr = {0};


				GUIEDITBOX_SetEditBoxToSingle(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, SINGLE_ALIGN_CENTER);
				GUIEDITBOX_EnableDisplayRect(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,TRUE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxStyle(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, 
					SONG_FONT_16,
					MMI_BLUE_COLOR, 
					MMI_DARK_GRAY_COLOR
					);
				GUIEDITBOX_SetEnterFullScreenState(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,FALSE);

				GUIEDITBOX_SetEditBoxToSingle(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, SINGLE_ALIGN_CENTER);
				GUIEDITBOX_EnableDisplayRect(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,TRUE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxStyle(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, 
					SONG_FONT_16,
					MMI_BLUE_COLOR, 
					MMI_DARK_GRAY_COLOR
					);
				GUIEDITBOX_SetEnterFullScreenState(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,FALSE);
				//初始化输入框
#if MEX_SCREEN_ESPECIAL_VERSION
				GUIEDITBOX_SetEditBoxToSingle(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, SINGLE_ALIGN_CENTER);
				GUIEDITBOX_EnableDisplayRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,TRUE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxStyle(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, 
					SONG_FONT_16,
					MMI_BLUE_COLOR, 
					MMI_DARK_GRAY_COLOR
					);
				GUIEDITBOX_SetEnterFullScreenState(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,FALSE);


				GUIEDITBOX_SetEditBoxToSingle(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, SINGLE_ALIGN_CENTER);
				GUIEDITBOX_EnableDisplayRect(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,TRUE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxStyle(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, 
					SONG_FONT_16,
					MMI_BLUE_COLOR, 
					MMI_DARK_GRAY_COLOR
					);
				GUIEDITBOX_SetEnterFullScreenState(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,FALSE);
#endif
#ifdef AUTO_APP_TEST
					EditWinDrawFocusRect(edit_rect,MMI_DARK_GRAY_COLOR);
					GUILABEL_SetTextScroll(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID, FALSE);
					GUILABEL_SetBackgroundColor(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID, MMI_DARK_GRAY_COLOR);		   
					GUILABEL_SetFontParam(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,SONG_FONT_16, MMI_BLUE_COLOR, 0);

					GUIEDITBOX_SetEditBoxToSingle(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, SINGLE_ALIGN_CENTER);
					GUIEDITBOX_EnableDisplayRect(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,TRUE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,	FALSE);
					GUIEDITBOX_SetEditBoxStyle(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, 
					SONG_FONT_16,
					MMI_BLUE_COLOR, 
					MMI_DARK_GRAY_COLOR
					);
					GUIEDITBOX_SetEnterFullScreenState(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,FALSE);
					GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_LABEL_CTRL_ID, TXT_MEX_APP_NAME,FALSE);
					GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_LABEL_CTRL_ID, TXT_MEX_APP_NUMBER,FALSE);
					GUILABEL_SetTextById(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID, TXT_MEX_APP_START_TEST,FALSE);
#endif
				sprintf(pVenderStr,"%d",MexConfig_GetPhoneConfig(E_MPC_MexVenderID));
				GUIEDITBOX_SetDigitalEditBoxStringInfo(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
				SCI_MEMSET(pVenderStr,0,20);
				sprintf(pVenderStr,"%s",(char *)MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA));
				GUIEDITBOX_SetDigitalEditBoxStringInfo(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));



				MMI_GetLabelTextByLang(TXT_MEX_PLT_ENG_LOG_LABEL,&logStr);
				MMI_GetLabelTextByLang(TXT_MEX_PLT_ENG_VENDER_LABEL,&verderStr);
				MMI_GetLabelTextByLang(TXT_MEX_PLT_ENG_PHONEUA_LABEL,&phoneUAStr);

				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID);
				GUILABEL_SetBackgroundColor(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID,MMI_CYAN_COLOR);

				GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID,&logStr,FALSE);
				GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID,&verderStr,FALSE);
				GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID,&phoneUAStr,FALSE);

#if MEX_SCREEN_ESPECIAL_VERSION
				{	
					extern uint16 g_now_screen_width;
					extern uint16 g_now_screen_height;

					MMI_STRING_T pScreenWidth = {0};
					MMI_STRING_T pScreenHeight = {0};
					MMI_GetLabelTextByLang(TXT_MEX_PLT_ENG_WIDTH_LABEL,&pScreenWidth);
					MMI_GetLabelTextByLang(TXT_MEX_PLT_ENG_HEIGHT_LABEL,&pScreenHeight);           
					GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID,&pScreenWidth,FALSE);
					GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID,&pScreenHeight,FALSE);

					SCI_MEMSET(pVenderStr,0,20);
					MexSystem_LoadSpacialScreenConfig();
					sprintf(pVenderStr,"%d",g_now_screen_width);          
					GUIEDITBOX_SetDigitalEditBoxStringInfo(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));
					SCI_MEMSET(pVenderStr,0,20);
					sprintf(pVenderStr,"%d",g_now_screen_height); 
					GUIEDITBOX_SetDigitalEditBoxStringInfo(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,mex_tstr2wcs(pVenderStr),mex_strlen(pVenderStr));                
				}
#endif
#ifdef AUTO_APP_TEST
				GUIEDITBOX_SetDigitalEditBoxStringInfo(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,AppNumber,	MMIAPICOM_Wstrlen(AppNumber));	

#endif

					GUIRES_DisplayImg(PNULL,
					&rect,
					&rect,
					win_id,
					IMAGE_COMMON_BG,
					&lcd_dev_info);
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_TIMER, NULL, 0);//发送刷新timer
			}
			SCI_FREE(pVenderStr);
		}

		break;
	case MSG_TIMER:
		MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
		GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
		GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
#if MEX_SCREEN_ESPECIAL_VERSION
		GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
		GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
#endif
#ifdef AUTO_APP_TEST
			GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
#endif
		MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);
		break;
	case MSG_FULL_PAINT:
		{
			MMI_STRING_T logOpenStr = {0};
			MMI_STRING_T logCloseStr = {0};
			GUI_RECT_T rectDrawImagePos = {2,1,22,21};
			GUI_POINT_T ptArrowLeft ={MEX_PLT_ENG_CENTER_X+1,MEX_PLT_ENG_BEGIN_Y};
			GUI_POINT_T ptArrowRight ={MEX_PLT_ENG_LOG_LABEL_X2+1,MEX_PLT_ENG_BEGIN_Y};
			MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);

			//显示箭头用==============
			uint16 uWidth,uHeight;
			uint16 uArrowWidth = 10;
#ifdef AUTO_APP_TEST			
		   if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID==atciveID) 	   
			GUILABEL_SetBackgroundColor(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,MMI_DARK_YELLOW_COLOR);
		   else
			GUILABEL_SetBackgroundColor(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,MMI_DARK_GRAY_COLOR);	
#endif
			GUIRES_GetImgWidthHeight(&uWidth,&uHeight,IMAGE_LIST_TITLE_ARROW_LEFT_UP,win_id);

			//计算画的位置
			if(uWidth > uArrowWidth )
			{
				rectDrawImagePos.left = (uWidth - uArrowWidth ) >> 1;
				rectDrawImagePos.right = (uWidth + uArrowWidth) >> 1;
			}
			else
			{
				rectDrawImagePos.left = uWidth >> 1;
				rectDrawImagePos.right = uWidth;
			}
			if(uHeight > MMI_DEFAULT_FONT_HEIGHT)
			{
				rectDrawImagePos.top = (uHeight - MMI_DEFAULT_FONT_HEIGHT) >> 1;
				rectDrawImagePos.bottom = (uHeight + MMI_DEFAULT_FONT_HEIGHT) >> 1;
			}
			else
			{
				rectDrawImagePos.top = uHeight  >> 1;
				rectDrawImagePos.bottom = uHeight;
			}
			//==============================

			MMI_GetLabelTextByLang(TXT_MET_MEX_OPEN,&logOpenStr);
			MMI_GetLabelTextByLang(TXT_MET_MEX_CLOSE,&logCloseStr);

			//LCD_FillRect(&lcd_dev_info, rect,MMI_BLACK_COLOR);
            GUIRES_DisplayImg(PNULL,
            &rect,
            &rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
			//IMAGE_LIST_TITLE_ARROW_RIGHT_UP,IMAGE_LIST_TITLE_ARROW_LEFT_UP
			GUIRES_DisplayImg(&ptArrowLeft,
				PNULL,
				&rectDrawImagePos,
				win_id,
				IMAGE_LIST_TITLE_ARROW_LEFT_UP,
				&lcd_dev_info);
			GUIRES_DisplayImg(&ptArrowRight,
				PNULL,
				&rectDrawImagePos,
				win_id,
				IMAGE_LIST_TITLE_ARROW_RIGHT_UP,
				&lcd_dev_info);

			if(g_MexLogFilter != 0)
				GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID,&logOpenStr,FALSE);
			else
				GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID,&logCloseStr,FALSE);
#ifdef AUTO_APP_TEST
			{
				MMI_STRING_T strtemp={0};
				strtemp.wstr_ptr =AppNameString;
				strtemp.wstr_len=MMIAPICOM_Wstrlen(AppNameString);
				GUILABEL_SetText(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,&strtemp,TRUE);
			}
#endif	
		}
		break;
	case MSG_CLOSE_WINDOW:
#ifdef MEX_QA_MODE
		{
			extern int32 iMex_QA_VenderID;
			extern char pMex_QA_PhoneUA[10];
			MMI_STRING_T strTemp = {0};
			wchar wstrTemp[10];
			GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&strTemp);
			if(strTemp.wstr_len != 0)
			{
				SCI_MEM16SET(wstrTemp,0,20);
				MMIAPICOM_Wstrncpy(wstrTemp,strTemp.wstr_ptr,strTemp.wstr_len);
				iMex_QA_VenderID = atoi(mex_twcs2str(wstrTemp));
			}
			else
				iMex_QA_VenderID =0;

			SCI_MEMSET(strTemp.wstr_ptr,0,sizeof(wchar)*strTemp.wstr_len);
			GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&strTemp);
			if(strTemp.wstr_len != 0)
			{
				SCI_MEM16SET(wstrTemp,0,20);
				MMIAPICOM_Wstrncpy(wstrTemp,strTemp.wstr_ptr,strTemp.wstr_len);
				SCI_MEMSET(pMex_QA_PhoneUA,0,sizeof(pMex_QA_PhoneUA));
				mex_wcs2nstr(pMex_QA_PhoneUA,strTemp.wstr_len*sizeof(wchar),wstrTemp);
			}
			else
				SCI_MEMSET(pMex_QA_PhoneUA,0,sizeof(pMex_QA_PhoneUA));
		}
#endif
		break;
		case MSG_CTL_CANCEL:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;
			
#ifdef AUTO_APP_TEST
		case MSG_KEYPRESSUP_WEB:
		case MSG_CTL_MIDSK:
			{
				MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);
				if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID==atciveID)
					{
					MMK_CreateWin((uint32 *)APP_NAMEWIN_TAB, PNULL);
					}
			}
	break;	
	
#endif
		case MSG_APP_WEB:
	case MSG_APP_OK:
	case MSG_CTL_OK:
    {
        MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);

#ifdef AUTO_APP_TEST

        if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID==atciveID)
        {
            MMK_CreateWin((uint32 *)APP_NAMEWIN_TAB, PNULL);
        }
        else
        {
#endif
        {
            MMI_STRING_T strTemp1 = {0};
            MMI_STRING_T strTemp2 = {0};
#ifdef AUTO_APP_TEST
            MMI_STRING_T strTemp5 = {0};
            MMI_STRING_T strTemp6 = {0};	
#endif
            GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,&strTemp1);
            GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,&strTemp2);

#if MEX_SCREEN_ESPECIAL_VERSION
            {
                MMI_STRING_T strTemp3 = {0};
                MMI_STRING_T strTemp4 = {0}; 
                extern uint16 g_now_screen_width;
                extern uint16 g_now_screen_height;
                int32 iScreenWidth,iScreenHeight;

                GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,&strTemp3);
                GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,&strTemp4);   

                iScreenWidth = mex_atoi( mex_twcs2str((wchar_t *)strTemp3.wstr_ptr));
                iScreenHeight = mex_atoi( mex_twcs2str((wchar_t *)strTemp4.wstr_ptr) );
                if( iScreenWidth != g_now_screen_width || iScreenHeight != g_now_screen_height )
                {
                    g_now_screen_width = iScreenWidth;
                    g_now_screen_height = iScreenHeight;
                    Mex_SaveScreenConfig( g_now_screen_width, g_now_screen_height );
                }

            }            
#endif            
#ifdef AUTO_APP_TEST
            GUILABEL_GetText(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,&strTemp5);
            GUIEDITBOX_GetDigitalEditBoxInfo(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,&strTemp6);
				SCI_MEMSET(AppNumber,0,20);
				MMI_WSTRNCPY(AppNumber, 10,strTemp6.wstr_ptr, strTemp6.wstr_len,strTemp6.wstr_len);

            if(0 == strTemp5.wstr_len||0 == strTemp6.wstr_len)
            {
#endif
            if (0 == strTemp1.wstr_len)
            {
                MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID, MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
                MMK_SendMsg(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, MSG_CTL_PAINT, 0 );
            }
            else if (0 == strTemp2.wstr_len)
            {
                MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID, MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
                MMK_SendMsg(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, MSG_CTL_PAINT, 0 );
            }
            else
            {
                MMK_CloseWin(MET_MEX_ENGINEER_MODE_WIN_ID);
            }

#ifdef AUTO_APP_TEST
            }
            else
            {
                if (0 == strTemp1.wstr_len)
                {
                    MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID, MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
                    MMK_SendMsg(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, MSG_CTL_PAINT, 0 );
                }
                else if (0 == strTemp2.wstr_len)
                {
                    MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID, MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);	
                    MMK_SendMsg(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, MSG_CTL_PAINT, 0 );
                }

                else if (0 == strTemp5.wstr_len)
                {
                    MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID, MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID);
                    MMK_SendMsg(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID, MSG_CTL_PAINT, 0 );
                }
                else if (0 == strTemp6.wstr_len)
                {
                    MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID, MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID); 		
                    MMK_SendMsg(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, MSG_CTL_PAINT, 0 );
                }
            }
#endif
#ifdef AUTO_APP_TEST
            {
                if(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID==atciveID)
                {
                    {
                        if(0!=strTemp6.wstr_len&&PNULL!=strTemp5.wstr_len)
                        {
                            uint16 index_len  = strTemp6.wstr_len;
                            uint32 id_num = 0;
                            char *appname =(char*)SCI_ALLOC(20);
                            if(SCI_NULL == appname)
                                break;
                            SCI_MEMSET(appname,0,20);
                            MMIAPICOM_WstrToNum(strTemp6.wstr_ptr, (uint8)index_len,&id_num);
                            MMIAPICOM_WstrToStr(strTemp5.wstr_ptr,appname);
                            MetMexTestEntryOpenWin(appname,id_num); 
                            SCI_FREE(appname);
                        }	
                    }
                }
                else
                {
                    MMK_CloseWin(MET_MEX_ENGINEER_MODE_WIN_ID);
                }
            }
#endif
        }
#ifdef AUTO_APP_TEST
        }
#endif
    }
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:
		{
			int32 PosX = MMK_GET_TP_X(param);
			int32 PosY =  MMK_GET_TP_Y(param);
			GUI_RECT_T rectArraw1 = MEX_PLT_ENG_ARROW_LEFT_RECT;
			GUI_RECT_T rectArraw2 = MEX_PLT_ENG_ARROW_RIGHT_RECT;

			//==============================

			if(((rectArraw1.left < PosX)
				&&(rectArraw1.right > PosX)
				&&(rectArraw1.top < PosY)
				&&(rectArraw1.bottom > PosY))  //压下左键
				||((rectArraw2.left < PosX)
				&&(rectArraw2.right > PosX)
				&&(rectArraw2.top < PosY)
				&&(rectArraw2.bottom > PosY)))//压下右键
			{
				g_MexLogFilter = (g_MexLogFilter == -1 ? 0 : -1);
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);
			}
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_KEYUP_UP:
		{

			MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);
#ifndef AUTO_APP_TEST
			if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);

			}	
			else if(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);

			}
			else
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}

#if MEX_SCREEN_ESPECIAL_VERSION
			if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID);            
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);                
			}
			else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID == atciveID)
			{

				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);            

				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);                 
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}
			else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID == atciveID)
			{

				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID);

				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}
			else if(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);

				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);                 
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);            
			}
			else
			{
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);                 
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);            
			}
#endif            

#else
				if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID);	
					
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
				else if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID == atciveID)
				{
	
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID);
	
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
				else if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID);
									
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
				else if(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID);
	
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
	
				else if(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);
				
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
	
				}
				else if(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
				
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
	
	
				else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID); 		   
	
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
				else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID == atciveID)
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID);
				
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				}
#endif
	
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);
			}
			break;
		case MSG_KEYUP_DOWN:
			{
	
				MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);
				
#ifndef AUTO_APP_TEST
			if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}	
			else if(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}
			else
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
			}

#if MEX_SCREEN_ESPECIAL_VERSION
			if(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID);            
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);                
			}
			else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID == atciveID)
			{

				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID);            

				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);                 
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}
			else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID == atciveID)
			{

				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);

				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);                 
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)STXT_RETURN,  FALSE);
			}
			else
			{
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);                 
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);            
			}
#endif            

#else
		  if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID);
				
				GUILABEL_SetBackgroundColor(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,MMI_DARK_YELLOW_COLOR);
				
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
		  }
			else if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID == atciveID)
			{
	
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID);			 
	
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
			}
			
			else if(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID == atciveID)
			{
	
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID);	
				
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
			}
	
			else if(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID == atciveID)
			{
	
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID);
	
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
			}
		   else  if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID);
				
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
			}	
		   else if(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
			}
		   else if(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
			}
		   else if(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID == atciveID)
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID);
	
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE); 			  
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);				  
			}	
#endif
			MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);	
		}
		break;
	case MSG_KEYUP_LEFT:
	case MSG_KEYUP_RIGHT:
		{
			if(MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID ==MMK_GetActiveCtrlId(win_id) )
			{	
				g_MexLogFilter = (g_MexLogFilter == -1 ? 0 : -1);
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);
			}
		}
		break;
	case MSG_KEYUP_0:
	case MSG_KEYUP_1:
	case MSG_KEYUP_2:
	case MSG_KEYUP_3:
	case MSG_KEYUP_4:
	case MSG_KEYUP_5:
	case MSG_KEYUP_6:
	case MSG_KEYUP_7:
	case MSG_KEYUP_8:
	case MSG_KEYUP_9:
		{
			MMI_CTRL_ID_T atciveID = MMK_GetActiveCtrlId(win_id);
			if((MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID == atciveID)||(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID == atciveID))
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,atciveID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);	
			}	

#if MEX_SCREEN_ESPECIAL_VERSION
			if((MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID == atciveID)||(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID == atciveID))
			{
				MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,atciveID);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,  FALSE);
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);	
			}            
#endif 
#ifdef AUTO_APP_TEST
				if((MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID == atciveID)||(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID == atciveID)||(MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID == atciveID))
				{
					MMK_SetAtvCtrl(MET_MEX_ENGINEER_MODE_WIN_ID,atciveID);
					GUIEDITBOX_SetEditBoxSoftkey(MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID, (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_COMMON_OK, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, (MMI_TEXT_ID_T)TXT_DELETE,	FALSE);
				MMK_PostMsg(MET_MEX_ENGINEER_MODE_WIN_ID, MSG_FULL_PAINT, NULL, 0);	
			}            
#endif 
		}
		break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

#endif
	return recode;

}

//-------------end SSC Platform win func--------------
//======================================================os_lock begin
void mex_Pen_lock(void)
{
	g_bMexTPLock = true;
}

void mex_Pen_unlock(void)
{
	g_bMexTPLock = false;
}

void mex_Key_Lock(void)
{
#ifdef MMI_KEY_LOCK_SUPPORT
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	if(!MMIAPIKL_IsPhoneLocked())
#else
	if(!MMIKL_IsPhoneLocked())
#endif
	{
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
		MMIAPIKL_LockPhone();
#else
		MMIKL_LockPhone();
#endif
	}
#endif
}

void mex_Key_unLock(void)
{
#ifdef MMI_KEY_LOCK_SUPPORT
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	if(MMIAPIKL_IsPhoneLocked())
#else
	if(MMIKL_IsPhoneLocked())
#endif
	{
#if(MEX_SPRD_CODE_VERSION == 0x10A6530) 
	MMIAPIKL_UnlockPhone();
#else
	MMIKL_UnlockPhone(); 
#endif
	}
#endif
}

void mex_ReStartKeyLock(void)
{
	if(MMIAPISET_GetAutoBoardKey()>0)
	{
		MMIDEFAULT_StopAutoKeylockTimer();
		MMIDEFAULT_StartAutoKeylockTimer();
	}
}
//=======================================================os_lock end



//=======================================================Display Popup begin

typedef void (*MetMex_PopupKeyHandler)(void);

typedef struct
{
	void* hKeyLeft;      // left key handler
	void* hKeyRight;     // right key handler
	bool  bKeyDown;      // 是否收到过Key_Down 消息(忽略第一次不成对的按键消息)
} MetMex_Popup_Context;

LOCAL MetMex_Popup_Context g_MetMex_Popup = { 0 };

PUBLIC void MetMex_SetPopupKeyHandler(void* hKeyLeft, void* hKeyRight)
{
	g_MetMex_Popup.hKeyLeft = hKeyLeft;
	g_MetMex_Popup.hKeyRight = hKeyRight;
}

LOCAL MMI_RESULT_E MetMex_PopupHandler(
									   MMI_WIN_ID_T		win_id,    //IN:
									   MMI_MESSAGE_ID_E	msg_id,    //IN:
									   DPARAM				param      //IN:
									   )
{
	MMI_RESULT_E		recode = MMI_RESULT_TRUE;
	GUI_POINT_T point={0};

	GUI_RECT_T left_rect={0};
	GUI_RECT_T right_rect={0};

#ifdef PDA_UI_ANDROID_POP 
		MMIPUB_INFO_T	*win_info_ptr = (MMIPUB_INFO_T *)MMK_GetWinAddDataPtr(win_id);
		win_info_ptr->win_id = win_id;
#endif
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		g_MetMex_Popup.bKeyDown = false;
		break;
	case MSG_CTL_OK:
	case MSG_APP_OK:  
	case MSG_APP_WEB:
		if(g_MetMex_Popup.hKeyLeft != NULL)
		{
			((MetMex_PopupKeyHandler)g_MetMex_Popup.hKeyLeft)();  /*lint !e611 */
		}
		g_MetMex_Popup.bKeyDown = true;
		break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		if(g_MetMex_Popup.hKeyRight != NULL)
		{
			((MetMex_PopupKeyHandler)g_MetMex_Popup.hKeyRight)();  /*lint !e611 */
		}
		g_MetMex_Popup.bKeyDown = true;
		break;
#ifdef TOUCH_PANEL_SUPPORT 		
	case MSG_TP_PRESS_DOWN:        
		left_rect.left = 0;
		left_rect.top = MexConfig_GetPhoneConfig(E_MPC_Screen_Height)-25;
		left_rect.right = MexConfig_GetPhoneConfig(E_MPC_Screen_Width)/5;
		left_rect.bottom = MexConfig_GetPhoneConfig(E_MPC_Screen_Height);

		right_rect.left = 4*MexConfig_GetPhoneConfig(E_MPC_Screen_Width)/5;
		right_rect.top = MexConfig_GetPhoneConfig(E_MPC_Screen_Height)-25;
		right_rect.right = MexConfig_GetPhoneConfig(E_MPC_Screen_Width);
		right_rect.bottom = MexConfig_GetPhoneConfig(E_MPC_Screen_Height);
		if (g_bMexTPLock)
		{
			break;
		}
		point.x = MMK_GET_TP_X(param);
		point.y = MMK_GET_TP_Y(param);
#ifdef PDA_UI_ANDROID_POP
		if(GUI_PointIsInRect(point, win_info_ptr->softkey_rect[LEFT_BUTTON]))
#else
		if(GUI_PointIsInRect(point, left_rect))
#endif
		{
			if(g_MetMex_Popup.hKeyLeft != NULL)
			{
				((MetMex_PopupKeyHandler)g_MetMex_Popup.hKeyLeft)();  /*lint !e611 */
			}
			g_MetMex_Popup.bKeyDown = true;
		}
#ifdef PDA_UI_ANDROID_POP
		 else if(GUI_PointIsInRect(point, win_info_ptr->softkey_rect[RIGHT_BUTTON]))
#else
		else if(GUI_PointIsInRect(point, right_rect))
#endif
		{
			if(g_MetMex_Popup.hKeyRight != NULL)
			{
				((MetMex_PopupKeyHandler)g_MetMex_Popup.hKeyRight)();  /*lint !e611 */
			}
			g_MetMex_Popup.bKeyDown = true;
		}
		break;
#endif

	default:
		recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
		break;
	}

	if (msg_id == MSG_CTL_OK ||
		msg_id == MSG_CTL_CANCEL ||
		(g_MetMex_Popup.bKeyDown && (msg_id == MSG_KEYUP_OK || msg_id == MSG_KEYUP_WEB || msg_id == MSG_KEYUP_CANCEL|| msg_id==MSG_TP_PRESS_DOWN)))
	{
		g_mex_bGotoMainMenu = false;
		MMK_CloseWin(win_id);
	}

	return (recode);

}

PUBLIC void MetMex_DisplayPopup(const wchar_t *str1, uint16 str1_len, const wchar_t *str2, uint16 str2_len)
{
	MMI_STRING_T txt1;
	MMI_STRING_T txt2;

	txt1.wstr_ptr = str1;  /*lint !e605 */
	txt1.wstr_len = str1_len;
	txt2.wstr_ptr = str2;  /*lint !e605 */
	txt2.wstr_len = str2_len;

	MMIPUB_OpenQueryWinByTextPtr(&txt1, &txt2, IMAGE_PUBWIN_QUERY, PNULL, MetMex_PopupHandler);
}

PUBLIC void MetMex_DisplayPopupById(MMI_TEXT_ID_T text_id1, MMI_TEXT_ID_T text_id2, MMI_IMAGE_ID_T image_id)
{
	uint32 time_out = 8000;    // 窗口持续时间
	MMI_IMAGE_ID_T img_id = IMAGE_PUBWIN_QUERY;
	if (image_id != 0)
		img_id = image_id;
	if(text_id1==TXT_MET_MEX_SMS_RECEIVE)
		{
		MMIPUB_OpenAlertWinByTextId(
			PNULL,
			text_id1,
			text_id2,
			img_id,
			PNULL,
			PNULL,
			MMIPUB_SOFTKEY_ONE,
			PNULL);
		}
	else
		{
		MMIPUB_OpenAlertWinByTextId(
			&time_out,
			text_id1,
			text_id2,
			img_id,
			PNULL,
			PNULL,
			MMIPUB_SOFTKEY_OKCANCEL,
			MetMex_PopupHandler);
		}

}


//=======================================================Display Popup end



//=======================================================MetMex Conflict Promt begin
//当前qq后台是否在运行
bool Mex_IsMexQQMessageRunning(void)
{
	int32 iAppId = Mex_GetAppIdByFileName(L"Mex_QQ_Message.mex");
	return ( iAppId != 0 );
}

//当前qq前台是否在运行
bool Mex_IsMexQQRunning(void)
{
	int32 iAppId = Mex_GetAppIdByFileName(L"Mex_QQ.mex");
	return ( iAppId != 0 );
}

PUBLIC  BOOLEAN MetMex_ConflictPromt(void)
{
    if((FALSE == (BOOLEAN)MexSystem_IsSysKernalRunning()))
    {
        return FALSE;
    }
        
	// 检测QQ
	if(Mex_IsMexQQMessageRunning() || Mex_IsMexQQRunning())
	{		
		MetMex_SetPopupKeyHandler((void *) MetMexSys_ShutDownAll, NULL);  /*lint !e611 */
		MetMex_DisplayPopupById(TXT_MET_MEX_QQ_CONFLICT, NULL, 0);
		return TRUE;
	} 
	else // 如果有其他的挂机程序 请在这里添加检测代码
	{
	 
		MetMexSys_ShutDownAll();
		return FALSE;
		
	}
}

//=======================================================MetMex Conflict Promt end


//=======================================================MetMex Conflict Promt begin

PUBLIC void MetMex_SmsReceivePromt(void)
{
#ifdef MEX_G7_UI_SUPPORT
		MexSystem_NotifyRecvSms(0,0,0,0); //向app发送消息：“收到新的短信”
#else
	MetMex_SetPopupKeyHandler(NULL, NULL);
	MetMex_DisplayPopupById(TXT_MET_MEX_SMS_RECEIVE, NULL, 0);
	//mex界面 来短信震动一下
	MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,2000); 
#endif
}

//=======================================================MetMex Conflict Promt end

//=======================================================MetMex Conflict Promt begin

PUBLIC void MetMex_HandleSDCardPlugOut(void)
{
    //SD卡拔出，立即退出平台，关闭所有应用
    MetMexSys_ShutDownAll();
}

//=======================================================MetMex Conflict Promt end


#ifdef MEX_QA_MODE
char currAppNameParam[50] = "Mex_QQ.mex:";   //默认选项为QQ
int32 currAppId = 1;
int32 Mex_Test_CheckCurrAppId(char *appInitParam )
{
	if( mex_strstr( appInitParam,currAppNameParam ) != NULL)
	{
		return currAppId;
	}

	return 0;
}
#endif
#endif // MET_MEX_SUPPORT







/*Edit by script, ignore 6 case. Thu Apr 26 19:00:59 2012*/ //IGNORE9527


/*Edit by script, ignore 13 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
