/************************************************************************
* 版权所有 (C)2010,扬讯科技。
*
* 文件名称： // MexMain.c
* 文件标识：
* 内容摘要： // 平台入口，各个应用的独立入口
*
************************************************************************/

#if defined(MEX_TARGET_SERVER)
#include "MMIDataType.h"

#include "MexLib.h"

#include "globaldefs.h"
#include "GlobalConstants.h"
#include "GlobalMenuItems.h"
#include "SettingResDef.h"
#if (MEX_MTK_CODE_VERSION < 0x1112)
#include "HistoryGprot.h"
#include "FrameworkStruct.h"
#endif

#include "SettingProfile.h"
#include "CommonScreens.h"
#if (MEX_MTK_CODE_VERSION < 0x0952)
#include "wgui_status_icons.h"
#endif

#include "DataAccountGProt.h"

#include "Med_utility.h"
#if (MEX_MTK_CODE_VERSION >= 0x1032)
#include "dataaccountcuigprot.h"
#include "mmi_rp_app_metmex_def.h"
#include "mex_app_id.h"
#else
#include "Mex_Res.h"
#endif
#include "MexLib_Def.h"
#include "MexLib_Std.h"
#include "MexLib_Os.h"
#include "MexLib_Gui.h"
#include "MexLib_Phs.h"
#include "MexLib_Soc.h"
#include "MexInterface.h"


#if (MEX_MTK_CODE_VERSION >= 0x1108)
#include "l4c_nw_cmd.h"
#include "wgui_include.h"
#include "Wgui_categories_util.h"
#include "TimerEvents.h"
#include "wgui_inline_edit.h" 

#endif
#if (MEX_MTK_CODE_VERSION >= 0x1112)
#include "wgui_categories_util.h"
#include "menucui.h"
#include "inlinecuiGprot.h"
#include "browser_api.h"

#include "mmi_rp_native_app_package_data.h"
#endif

#if defined ( __MET_MEX_SUPPORT__ ) && (defined (MT6236) || defined (MT6235B))

#if (MEX_MTK_CODE_VERSION >= 0x1032) && (MEX_MTK_CODE_VERSION < 0x1200)
//关闭播放声音时底层整流器,减少声音播放耗时,若需要定义该宏为1,否则为0
#define     _MEX_DISABLE_AUDIO_COMPENSATION_    (1)
//是否忽略底层播放声音延时处理,减少播放声音耗时,若需要定义该宏为1,否则为0
#define     _MEX_DISABLE_SLEEP_WHEN_SOUNDPLAY_  (1)

//标志按键音是否执行
bool bDoAudio_Compensation = true;
void Mex_Set_Disable_Audio_Compensation(bool bFlag);
bool Mex_Get_Disable_Audio_Compensation(void);
//标志播放声音是延时是否执行
bool bSoundPlaySleepIsRun = true;
void Mex_Set_SoundPlaySleep_Status(bool bFlag);
bool Mex_Get_soundPlaySleep_Status(void);
#endif
#endif


void Mex_Exit_MainScreen(void);
U8 Mex_DeleteEntry( void *param);
void Mex_Pause_System(void);
void Mex_ShutDownExtSystem(void);
void Mex_ShutDownAllSystem(void);
void Mex_Entry_QQ(void);
void Mex_Entry_MainScreen(void);
#if (MEX_MTK_CODE_VERSION < 0x1112)
void Mex_PlatForm_EntryEngineerMode(void);
#else
void mex_engine(void);
#endif
extern void MexConfig_SetTinyLoadingPage( bool bShowColorBar, mex_color cBgColor,mex_color cFontColor, uint8 uFontPos, const wchar_t * szLoadStr );
extern void MexConfig_SetManLoadingPage( bool bShowColorBar, mex_color cBgColor, mex_color cFontColor, uint8 uFontPos, const wchar_t * szLoadStr );

extern bool MexSystem_IsSysKernalRunning(void);

static int32 Mex_GetAppIdByFileName( wchar_t *szAppName );

void Mex_SetCurrentEntryAppInfo(char *pAppName, int32 iAppID);
int32 MexConfig_GetPhoneConfig(E_Mex_Phone_Config cfg);

#ifdef MEX_SEL_DATAACCOUNT_SUPPORT

extern void Mex_Entry_Menu_RegisterMenuFunc( MexFuncPtr pStartFunc, MexFuncPtr pAboutFunc );
extern void Mex_Entry_Menu_MenuList( void );


extern bool Mex_EntrySelectDateAccountScreen(void);
extern void Mex_Select_Date_Account(void);
extern void Mex_Entry_ApnSelectScreen(void);

#endif

extern bool g_bNeedShowLoadingPage;
#if MEX_MTK_CODE_VERSION >= 0x1108
extern wgui_inline_item wgui_inline_items[MAX_INLINE_ITEMS];
#endif


#define MEXMANAGER_LAUNCH_APP_BY_MEXSTARTUP_FILE	1	//(!MEX_BUILD_FINAL_USER)删除，直接全部改成MexStartup.cfg控制
#define MEXMANAGER_LAUNCH_APP_BY_MEXSINSTALL_FILE	(!MEX_BUILD_FINAL_USER)

#ifdef __J2ME__
void mex_jam_stop_confirm(void);
extern void jam_enter_terminate_mode(void) ;/* this function is call by MMI */
extern kal_bool jam_is_teminate_mode(void);
extern void mmi_java_entry_terminating(MMI_BOOL idle);
#endif/*__J2ME__*/

//#define SRV_FMGR_CARD_DRV           FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)

static void Mex_Entry_Screen_Internal(FuncPtr newEntryHandler)
{
	int8 ret;
	bool bSysKernalInited = false;
	static bool bEntrySelectDataAccount = false;

	extern int16 MexConfig_GetAppMemId(void);
	extern bool g_bBackStartMode;

	extern void MexInterface_Conflict_Suspend(void);
	extern void MexLib_Os_Init_File_Pool(void);

	uint16 uStringId;
#if 0
	//无T卡，进3G至尊宝，提示空间不足，格式化手机后，提示还是如此
	if(FS_NO_ERROR != FS_GetDevStatus((U8)SRV_FMGR_CARD_DRV ,FS_MOUNT_STATE_ENUM))
	 {
	  	DisplayPopup((PU8) GetString(STR_GLOBAL_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, TRUE, 1000, WARNING_TONE);
   		return;
	}
#endif
	#ifdef __J2ME__
	// java  冲突处理
	 if(!jam_is_teminate_mode())
	  {
	   mex_jam_stop_confirm();
		return ;
	 }
	#endif
	assert(newEntryHandler != NULL);
#if MEXMANAGER_LAUNCH_APP_BY_MEXSTARTUP_FILE
	{
		FS_HANDLE pFile;
		wchar_t cfgFileName[40] = { 0 };

		mex_wcscpy( cfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_SystemRootDir) );
		mex_wcscat( cfgFileName, L"\\MexStartup.cfg" );

		pFile = FS_Open( cfgFileName, FS_READ_ONLY );
		if (pFile > 0)
		{
			int16 i;
			static char szApp[40] = { 0 };
			uint32 iRead;
            memset(szApp, 0, 40);
			FS_Read( pFile, szApp, 40, &iRead );
			FS_Close( pFile );
			for (i = mex_strlen(szApp) - 1; i > 0; i--)
			{
				if (szApp[i] == '\r' || szApp[i] == '\n' || szApp[i] == ' ' || szApp[i] == '\t' || szApp[i] == 26)
					szApp[i] = 0;
			}
			if (mex_strlen(szApp) > 0)
			{
				mex_strcat( szApp, ".mex" );
				Mex_SetCurrentEntryAppInfo( szApp , 0);
                mex_strcat( szApp, ":" );
				MexSystem_SetSystemInitParam( szApp );

			}
		}
	}
#endif

#if MEXMANAGER_LAUNCH_APP_BY_MEXSINSTALL_FILE
#ifdef MEX_INSTALL_RUN_PACKAGE_SUPPORT
	if (MexSystem_GetSystemInitParam() == NULL)
	{
		FS_HANDLE pFile;
		wchar_t cfgFileName[40] = { 0 };

		mex_wcscpy( cfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_SystemRootDir) );
		mex_wcscat( cfgFileName, L"\\MexInstall.cfg" );

		pFile = FS_Open( cfgFileName, FS_READ_ONLY );
		if (pFile > 0)
		{
			int16 i;
			static char szPackage[100] = { 0 };
			uint32 iRead;
			memset(szPackage, 0, 100);
			FS_Read( pFile, szPackage, 100, &iRead );
			FS_Close( pFile );
			for (i = mex_strlen(szPackage) - 1; i > 0; i--)
			{
				if (szPackage[i] == '\r' || szPackage[i] == '\n' || szPackage[i] == ' ' || szPackage[i] == '\t' || szPackage[i] == 26)
					szPackage[i] = 0;
			}
			Mex_Entry_InstallAndRunPackage(mex_str2nwcs(cfgFileName, 40 * 2, szPackage));
			return;
		}
	}
#endif
#endif

    #if defined(__MEX_G7_UI_BOOTUP__)
    if(MexSystem_GetSystemInitParam()==NULL ||(mex_stricmp((char*)MexSystem_GetSystemInitParam(),(char*)"AdExplorer.mex:")==0))
    {
      EntryNewScreen(E_MSI_MAIN,Mex_Exit_MainScreen, newEntryHandler, NULL);
	  entry_full_screen();
	  SetDelScrnIDCallbackHandler( E_MSI_MAIN, (HistoryDelCBPtr)Mex_DeleteEntry );
	}
	else
	{
	  if(MexConfig_GetPhoneConfig(E_MPC_Tcard_State))
	  {
         EntryNewScreen(E_MSI_MAIN,Mex_Exit_MainScreen, newEntryHandler, NULL);
	     entry_full_screen();
	     SetDelScrnIDCallbackHandler( E_MSI_MAIN, (HistoryDelCBPtr)Mex_DeleteEntry );  
	  }
	  else
	  {//T 卡不存在，启动除平台的其它应用
        EntryNewScreen(E_MSI_MAX,Mex_Exit_MainScreen, newEntryHandler, NULL);
	    entry_full_screen();
	  }
	}
	#else
	EntryNewScreen(E_MSI_MAIN,Mex_Exit_MainScreen, newEntryHandler, NULL);
	entry_full_screen();
	SetDelScrnIDCallbackHandler( E_MSI_MAIN, (HistoryDelCBPtr)Mex_DeleteEntry );
	#endif

	//if (mmi_frm_group_is_present(E_MSI_BASE))
	//{
	//	mmi_frm_group_close(E_MSI_BASE);
	//}
	//mmi_frm_group_create(GRP_ID_ROOT, E_MSI_BASE, NULL, NULL);
	//mmi_frm_group_enter(E_MSI_BASE, MMI_FRM_NODE_SMART_CLOSE_FLAG);

	//if (!mmi_frm_scrn_enter(
	//	E_MSI_BASE,
	//	E_MSI_MAIN,
	//	Mex_Exit_MainScreen,
	//	newEntryHandler,
	//	MMI_FRM_FULL_SCRN))
	//{
	//	mex_log(1, "entry scr err!!!");
	//	return;
	//}

	#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
	if(!bEntrySelectDataAccount)
	{
		if(Mex_EntrySelectDateAccountScreen())
		{
			bEntrySelectDataAccount = true;
			return;
		}

	}
	else
	{
		bEntrySelectDataAccount = false;
	}
	#endif
	g_bBackStartMode = false;

#if defined ( __MET_MEX_SUPPORT__ ) && (defined (MT6236) || defined (MT6235B)) 
#if (MEX_MTK_CODE_VERSION >= 0x1032) && (MEX_MTK_CODE_VERSION < 0x1200)
//关闭播放声音时,底层整流器,减少声音播放耗时

    Mex_Set_Disable_Audio_Compensation(false);
    Mex_Set_SoundPlaySleep_Status(false);
#endif
#endif

	//中断其他冲突应用
	MexInterface_Conflict_Suspend();
    

#if defined(__MEX_G7_UI_BOOTUP__)
    if(MexSystem_GetSystemInitParam()==NULL ||(mex_stricmp((char*)MexSystem_GetSystemInitParam(),(char*)"AdExplorer.mex:")==0))
    {
       ; 
	}
    else
    {
       if ( MexConfig_GetPhoneConfig(E_MPC_PauseBgMusicOnLaunch) )
	  {
		mdi_audio_suspend_background_play();
	  }
	}
#else
	//Pause audio paly
	if ( MexConfig_GetPhoneConfig(E_MPC_PauseBgMusicOnLaunch) )
	{
		mdi_audio_suspend_background_play();
	}
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1112)
	MexLib_Os_Init_File_Pool();
#endif
	bSysKernalInited = MexSystem_IsSysKernalRunning();
	ret = MexSystem_Init();


	switch(ret)
	{
	case E_MEE_OK:
		{
			return;
		}
		break;
	case E_MEE_USBMS:
		{
			uStringId = E_MSI_USB_MASSSTORAGE;
			//str_pop = (U8 *)"\xF7\x8B\xD4\x62\xFA\x51\x55\x0\x53\x0\x42\x0\xE\x54\x8D\x51\xDB\x8F\x65\x51\xB\x7A\x8F\x5E\x1\xFF\0"; //"请拔出USB后再进入程序!";
		}
		break;
	case E_MEE_NOCARD:
		{
			uStringId = E_MSI_NO_TFLASH;
			//str_pop = (U8 *)"\xF7\x8B\xD2\x63\x65\x51\x58\x5B\xA8\x50\x61\x53\xE\x54\x8D\x51\xDB\x8F\x65\x51\xB\x7A\x8F\x5E\x1\xFF\0";//mex_twcs2str(L"请插入存储卡后再进入程序!");
		}
		break;
	case E_MEE_SPACESHORTAGE:
		{
			uStringId = E_MSI_NOT_ENOUGH_SPACE;
			//str_pop = (U8 *)"\x58\x5B\xA8\x50\x61\x53\x7A\x7A\xF4\x95\xD\x4E\xB3\x8D\x2C\x0\xE0\x65\xD5\x6C\xDB\x8F\x65\x51\xB\x7A\x8F\x5E\x1\xFF\0";//mex_twcs2str(L"存储卡空间不足,无法进入程序!");
		}
		break;
	case E_MEE_INTMEMINITERR:
	case E_MEE_EXTMEMINITERR:
	case E_MEE_RESMEMINITERR:
		{
			uStringId = E_MSI_OUT_OF_MEMORY;
			//str_pop = (U8 *)"\x85\x51\x58\x5B\xD\x4E\xB3\x8D\x2C\x0\xF7\x8B\x73\x51\xED\x95\x0\x4E\x9B\x4E\xB\x7A\x8F\x5E\x1\xFF\0";//mex_twcs2str(L"内存不足,请关闭一些程序!");
		}
		break;
	case E_MEE_PREPARETFLASHERR:
		{
			uStringId = E_MSI_UNABLE_READ_FILE;
			//str_pop = (U8 *)"\xE0\x65\xD5\x6C\xFB\x8B\xD6\x53\x58\x5B\xA8\x50\x61\x53\x87\x65\xF6\x4E\x1\xFF\0";//mex_twcs2str(L"无法读取存储卡文件!");
		}
		break;
	case E_MEE_STARTUPTINYERR:
		{
			uStringId = E_MSI_INIT_FAILED;
			//str_pop = (U8 *)"\x2F\x54\xA8\x52\xB\x7A\x8F\x5E\x31\x59\x25\x8D\x1\xFF\0";//mex_twcs2str(L"启动程序失败!");
		}
		break;
	}
	
	#if defined(MEX_G7_UI_BOOT)
	Mex_Set_UI_Status(E_MEX_UI_STYLE_DEFAULT);
	#endif

	//启动失败后释放资源
	if (!bSysKernalInited)
		Mex_ShutDownAllSystem();
	GoBackHistory();
	//显示 mex无法启动!
	if (MexConfig_GetAppMemId() >= 0 && ret == E_MEE_INTMEMINITERR)
		mmi_frm_appmem_prompt_to_release_mem(MexConfig_GetAppMemId(), E_MII_MAIN_ICON, MexConfig_GetPhoneConfig(E_MPC_MaxIntMem), newEntryHandler );
	else
		DisplayPopup((PU8)GetString((U16)uStringId),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);

	g_bNeedShowLoadingPage = false;		//不再显示启动加载页面
}

#if defined(__MEX_G7_UI_BOOTUP__)
extern void Mex_Entry_PlatForm(void);
extern void SetG7UIInitParam(void);
void Mex_Entry_Screen_Interal_ext(void)
{
   //SetG7UIInitParam();
   Mex_Entry_Screen_Internal( Mex_Entry_PlatForm );
}
#endif
#if (MEX_MTK_CODE_VERSION >= 0x1112)
MMI_ID Mex_PlatForm_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_PlatForm(void);
#if defined(__MEX_G7_UI_BOOTUP__)	
	if(true == MexSystem_IsSysRunning())
	{
		mmi_frm_history_back_to_idle();
	}
	else
#endif		
	
	Mex_Entry_PlatForm();

	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
MMI_ID Mex_Abird_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_FNDXN(void);
	Mex_Entry_FNDXN();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
MMI_ID Mex_Cqz_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_CQZ(void);
	Mex_Entry_CQZ();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Zwdzjs_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_ZWDZJS(void);
        Mex_Entry_ZWDZJS();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Ddz_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_DDZ(void);
        Mex_Entry_DDZ();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Shsl_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_SHSL(void);
        Mex_Entry_SHSL();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Csjjs_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_CSJJS(void);
        Mex_Entry_CSJJS();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Blfc_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_BLFC(void);
        Mex_Entry_BLFC();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Mxsqy_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_MXSQY(void);
        Mex_Entry_MXSQY();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
    MMI_ID Mex_Dzt_Launch(void *param, U32 param_size)
    {
        extern void Mex_Entry_DZT(void);
        Mex_Entry_DZT();
        if (MexSystem_IsSysKernalRunning())
            return MMI_RET_OK;
        else
            return MMI_RET_ERR;
    }
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
MMI_ID Mex_Fruit_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_QSG(void);
	Mex_Entry_QSG();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_FETION_SUPPORT__)
MMI_ID Mex_Fetion_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_Fetion(void);
	Mex_Entry_Fetion();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_AQ_SUPPORT__)
MMI_ID Mex_Aq_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_Aq(void);
	Mex_Entry_Aq();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_HTC_TQ_SUPPORT__)
MMI_ID Mex_HTC_TQ_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_HTC_TQ(void);
	Mex_Entry_HTC_TQ();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_QQ_SUPPORT__)
MMI_ID Mex_QQ_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_QQ(void);
	Mex_Entry_QQ();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_MSN_SUPPORT__)
MMI_ID Mex_MSN_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_MSN(void);
	Mex_Entry_MSN();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif




#if defined(__MEX_Lighter_SUPPORT__)
MMI_ID Mex_Lighter_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_Lighter(void);
	Mex_Entry_Lighter();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_MLSZ_SUPPORT__)
MMI_ID Mex_Aq_MLSZ(void *param, U32 param_size)
{
	extern void Mex_Entry_MLSZ(void);
	Mex_Entry_MLSZ();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
MMI_ID Mex_MLSZ_Launch (void *param, U32 param_size)
{
	return Mex_Aq_MLSZ(param, param_size);
}
#endif

#if defined(__MEX_HDtqyb_SUPPORT__)
MMI_ID Mex_HDtqyb_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_HDtqyb(void);
	Mex_Entry_HDtqyb();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_MvBol_SUPPORT__)
MMI_ID Mex_MvBol_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_MvBol(void);
	Mex_Entry_MvBol();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_HDcqz_SUPPORT__)
MMI_ID Mex_HDcqz_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_HDcqz(void);
	Mex_Entry_HDcqz();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_SETTIME_SUPPORT__)
MMI_ID Mex_SETTIME_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_SETTIME(void);
	Mex_Entry_SETTIME();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_JJFB_SUPPORT__)
MMI_ID Mex_JJFB_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_JJFB(void);
	Mex_Entry_JJFB();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_JPFCKB_SUPPORT__)
MMI_ID Mex_JPFCKB_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_JPFCKB(void);
	Mex_Entry_JPFCKB();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_GANJILIFE_SUPPORT__)
MMI_ID Mex_GANJILIFE_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_GANJILIFE(void);
	Mex_Entry_GANJILIFE();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_MEXSQAPP_SUPPORT__)
MMI_ID Mex_MEXSQAPP_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_MEXSQAPP(void);
	Mex_Entry_MEXSQAPP();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif


#if defined(__MEX_RENREN_SUPPORT__)
MMI_ID Mex_Renren_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_Renren(void);
	Mex_Entry_Renren();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_KAIXIN_SUPPORT__)
MMI_ID Mex_Kaixin_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_Kaixin(void);
	Mex_Entry_Kaixin();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__MEX_TAOBAO_SUPPORT__)
MMI_ID Mex_Taobao_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_Taobao(void);
	Mex_Entry_Taobao();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#if defined(__3611A__)//__MEX_MENU_HIDE_SUPPORT__
MMI_ID Mex_Hdmao_Launch(void *param, U32 param_size)
{
	extern void Mex_Entry_HDMAO(void);
	Mex_Entry_HDMAO();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
#endif

#endif
/************************************************************************
 *进入mex主界面
 ************************************************************************/

void Mex_Entry_PlatForm(void)
{
	MexSystem_SetSystemInitParam( NULL );
	Mex_SetCurrentEntryAppInfo( "" , E_MSVT_Normal ); // Fix bug 12655
	MexConfig_SetTinyLoadingPage( true,
								  MEX_COLOR_BLACK,
								  MEX_COLOR_WHITE,
								  MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								  (const wchar_t *)"\x1D\x52\xCB\x59\x16\x53\x2E\x0\x2E\x0\x2E\x0\x2E\x0\x2E\x0\x2E\x0\0" );		//初始化......
	MexConfig_SetManLoadingPage( true,
								 MEX_COLOR_BLACK,
								 MEX_COLOR_WHITE,
								 MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								 (const wchar_t *)"\xF7\x8B\xD\x7A\x19\x50\x2E\x0\x2E\x0\x2E\x0\x2E\x0\x2E\x0\x2E\x0\0" );		//请稍候......
	Mex_Entry_Screen_Internal( Mex_Entry_PlatForm );

}

void Mex_Entry_MainScreen(void)
{
#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
	Mex_Entry_Menu_RegisterMenuFunc( Mex_Entry_PlatForm, Mex_Entry_ApnSelectScreen );
	Mex_Entry_Menu_MenuList();
#else
	Mex_Entry_PlatForm();
#endif

}
/************************************************************************
 * 退出主界面
 ************************************************************************/
void Mex_Exit_MainScreen(void)
{
#if defined ( __MET_MEX_SUPPORT__ ) && (defined (MT6236) || defined (MT6235B)) 
#if (MEX_MTK_CODE_VERSION >= 0x1032) && (MEX_MTK_CODE_VERSION < 0x1200)
    //恢复播放声音时,底层整流器打开
    Mex_Set_Disable_Audio_Compensation(true);
    Mex_Set_SoundPlaySleep_Status(true);
#endif
#endif
	Mex_Pause_System();
}

bool Mex_IsMexQQMessageRunning(void);

/************************************************************************
* 界面被中断后的回调
************************************************************************/
U8 Mex_DeleteEntry(void *param)
{
	extern int32 MexConfig_GetPhoneConfig(E_Mex_Phone_Config cfg);
	if( MexConfig_GetPhoneConfig(E_MPC_DeleteScreenHotShutdown) )
	{
		Mex_ShutDownExtSystem();
	}
	else
	{
		Mex_Pause_System();
	}
    //msn qq不在后台运行，退出所以后台
	if (!Mex_GetAppIdByFileName(L"MSG_backrun.mex") && !Mex_IsMexQQMessageRunning())
	{
		Mex_ShutDownAllSystem();
	}

#if 0
	// 日日康挂QQ进TB问题[Bug #11072]
	{
		int32 iMexDlId = Mex_GetAppIdByFileName( L"MexDownloader.mex" );
		if (iMexDlId != 0)
		{
			int32 iAppId = MexSystem_GetCurrentApp();
			MexSystem_SetCurrentApp(iMexDlId);
			mex_Exit();
			MexSystem_SetCurrentApp(iAppId);
		}
	}
#endif

#if defined ( __MET_MEX_SUPPORT__ ) && (defined (MT6236) || defined (MT6235B)) 
#if (MEX_MTK_CODE_VERSION >= 0x1032) && (MEX_MTK_CODE_VERSION < 0x1200)
    //恢复播放声音时,底层整流器打开
    Mex_Set_Disable_Audio_Compensation(true);
    Mex_Set_SoundPlaySleep_Status(true);
#endif
#endif


	//Resume audio play
	if ( MexConfig_GetPhoneConfig(E_MPC_PauseBgMusicOnLaunch) )
	{
		mdi_audio_resume_background_play();
	}


	//Mex_ClearProtocolEventHandler_conflict();
	Mex_ClearProtocolEventHandler_onexit();
	return 0;
}

void Mex_ExitExtSystem_EntryIdle(void)
{
	extern void EntryIdleScreen(void);
	MexSystem_Shutdown( E_MPS_HotShutdown );
	DeleteNScrId( E_MSI_MAIN );
	GoBackHistory();
}

void Mex_ExitAllSystem_EntryIdle(void)
{
	extern void EntryIdleScreen(void);
	MexSystem_Shutdown(E_MPS_ColdShutdown);
	EntryIdleScreen();
}
/************************************************************************
*退出平台的接口
************************************************************************/
void Mex_Exit_System( bool bExitAll )
{
	DisplayConfirm(
		STR_GLOBAL_YES,
		IMG_GLOBAL_YES,
		STR_GLOBAL_NO,
		IMG_GLOBAL_NO,
		(UI_string_type)GetString((U16)E_MSI_QUIT_MESSAGE), //是否要退出应用
		NULL,
		WARNING_TONE);
	/* Register function for left soft key */
	if( bExitAll )
		SetLeftSoftkeyFunction(Mex_ExitAllSystem_EntryIdle, KEY_EVENT_UP);
	else
		SetLeftSoftkeyFunction(Mex_ExitExtSystem_EntryIdle, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

/************************************************************************
*暂停平台
************************************************************************/
void Mex_Pause_System(void)
{
	if( MexSystem_IsSysRunning() && (!MexSystem_IsSysBackRunning()))
	{
		MexSystem_Shutdown( E_MPS_Pause );
	}
}
/************************************************************************
* 退出平台
************************************************************************/
void Mex_ShutDownAllSystem(void)
{
	if( MexSystem_IsSysKernalRunning() )
	{
		MexSystem_Shutdown( E_MPS_ColdShutdown );
		DeleteNScrId( E_MSI_MAIN );
	}
}
/************************************************************************
* 退出平台
************************************************************************/
void Mex_ShutDownExtSystem(void)
{
	if( MexSystem_IsSysRunning() )
	{
		MexSystem_Shutdown( E_MPS_HotShutdown );
		DeleteNScrId( E_MSI_MAIN );
	}
}

#if defined ( __MET_MEX_SUPPORT__ ) && (defined (MT6236) || defined (MT6235B)) 
#if (MEX_MTK_CODE_VERSION >= 0x1032) && (MEX_MTK_CODE_VERSION < 0x1200)
//关闭播放声音时,底层整流器,减少声音播放耗时,若需要定义该宏为1,否则为0
void Mex_Set_Disable_Audio_Compensation(bool bFlag)
{
#if _MEX_DISABLE_AUDIO_COMPENSATION_
    mex_log( MEX_LOG_SYSTEM,"call Mex_Set_Disable_Audio_Compensation(%d)",bFlag);
    bDoAudio_Compensation= bFlag;
#endif
}

bool Mex_Get_Disable_Audio_Compensation(void)
{
    return bDoAudio_Compensation;
}
void Mex_Set_SoundPlaySleep_Status(bool bFlag)
{
#if _MEX_DISABLE_SLEEP_WHEN_SOUNDPLAY_
    mex_log( MEX_LOG_SYSTEM,"call Mex_Set_SoundPlaySleep_Status(%d)",bFlag);
    bSoundPlaySleepIsRun= bFlag;
#endif
}
bool Mex_Get_soundPlaySleep_Status()
{
    return bSoundPlaySleepIsRun;
}
#endif
#endif



/************************************************************************
 * 菜单选项被高亮选中时的回调
 ************************************************************************/
void Mex_HighLight_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_MainScreen, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/************************************************************************
 * 该函数随mmitask启动
 ************************************************************************/
void Mex_InitialzeAll(void)
{
	extern void MexConfig_AllocKermem(void);

#if defined(__MEX_ROMGUARDIAN_SUPPORT__)
{
	extern void MexSystem_MMITaskInit(void);
	MexSystem_MMITaskInit();
}
#endif
		

#if (MEX_MTK_CODE_VERSION < 0x1032)

	SetHiliteHandler(MET_MEX_MAIN, Mex_HighLight_Menu);
#ifdef __MEX_MERIG_YZH_SUPPORT__
	SetHiliteHandler(MET_MEX_MENU_YZH,Mex_HighLight_Menu);
#endif

#if defined(__MEX_QQ_SUPPORT__)
{
	extern void Mex_HighLight_QQ_Menu(void);

	#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__
		extern void Mex_HighLight_QQ2_Menu(void);
	#endif/*__MEX_QQ_DOUBLEMENU_SUPPORT__*/
#ifdef __MEX_QQ_TOOLS_SUPPORT__
	SetHiliteHandler(MET_MEX_QQ_TOOLS_MENUID,Mex_HighLight_QQ_Menu);
	#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__
		SetHiliteHandler(MET_MEX_QQ2_TOOLS_MENUID,Mex_HighLight_QQ2_Menu);
	#endif/*__MEX_QQ_DOUBLEMENU_SUPPORT__*/
#endif
#ifdef __MEX_QQ_SMS_SUPPORT__
	SetHiliteHandler(MET_MEX_QQ_SMS_MENUID,Mex_HighLight_QQ_Menu);
	#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__
		SetHiliteHandler(MET_MEX_QQ2_SMS_MENUID,Mex_HighLight_QQ2_Menu);
	#endif/*__MEX_QQ_DOUBLEMENU_SUPPORT__*/
#endif
}
#endif

#if defined(__MEX_SINA_SUPPORT__)
{
	extern void Mex_HighLight_SINA_Menu(void);
	SetHiliteHandler(MET_MEX_SINA_MENUID, Mex_HighLight_SINA_Menu);
}
#endif
#if defined(__MEX_TQYB_SUPPORT__)
{
	extern void Mex_HighLight_TQYB_Menu(void);
	SetHiliteHandler(MET_MEX_TQYB_MENUID, Mex_HighLight_TQYB_Menu);
}
#endif


#if defined(__MEX_WAMKF_SUPPORT__)||defined(__MEX_KGZW_SUPPORT__)
{
	extern void Mex_HighLight_TB_Menu(void);
#if defined(__MEX_WAMKF_SUPPORT__)
	SetHiliteHandler(MET_MEX_WAMKF_MENUID, Mex_HighLight_TB_Menu);
#endif
#if defined(__MEX_KGZW_SUPPORT__)
	SetHiliteHandler(MET_MEX_KGZW_MENUID, Mex_HighLight_TB_Menu);
#endif
}
#endif

#if defined(__MEX_MSN_SUPPORT__)
{
	extern void Mex_HighLight_MSN_Menu(void);
#ifdef __MEX_MSN_SMS_SUPPORT__
	SetHiliteHandler(MET_MEX_MSN_SMS_MENUID, Mex_HighLight_MSN_Menu);
#endif
#ifdef __MEX_MSN_PHB_SUPPORT__
	SetHiliteHandler(MET_MEX_MSN_PHB_MENUID, Mex_HighLight_MSN_Menu);
#endif
}
#endif

#if defined(__MEX_LSZT_SUPPORT__)||defined(__MEX_ZTWY_SUPPORT__)
{
	extern void Mex_HighLight_ZT_Menu(void);
#if defined(__MEX_LSZT_SUPPORT__)
	SetHiliteHandler(MET_MEX_LSZT_MENUID, Mex_HighLight_ZT_Menu);
#endif
#if defined(__MEX_ZTWY_SUPPORT__)
	SetHiliteHandler(MET_MEX_ZTWY_MENUID, Mex_HighLight_ZT_Menu);
#endif
}
#endif

#if defined(__MEX_FETION_SUPPORT__)
	{
		extern void Mex_HighLight_Fetion_Menu(void);
#if defined(__MEX_FETION_SMS_SUPPORT__)
		SetHiliteHandler(MET_MEX_FETION_SMS_MENUID, Mex_HighLight_Fetion_Menu);
#endif
#if defined(__MEX_FETION_TOOLS_SUPPORT__)
		SetHiliteHandler(MET_MEX_FETION_TOOLS_MENUID, Mex_HighLight_Fetion_Menu);
#endif
	}
#endif


#if defined(__MEX_WMSP_SUPPORT__)||defined(__MEX_TCJY_SUPPORT__)
{
				extern void Mex_HighLight_WoChat_Menu(void);
#if defined(__MEX_WMSP_SUPPORT__)
				SetHiliteHandler(MET_MEX_WMSP_MENUID, Mex_HighLight_WoChat_Menu);
#endif
#if defined(__MEX_TCJY_SUPPORT__)
				SetHiliteHandler(MET_MEX_TCJY_MENUID, Mex_HighLight_WoChat_Menu);
#endif
}
#endif

#if defined(__MEX_LWSG_SUPPORT__)||defined(__MEX_MFWY_SUPPORT__)
	{
					extern void Mex_HighLight_LWSG_Menu(void);
#if defined(__MEX_LWSG_SUPPORT__)
					SetHiliteHandler(MET_MEX_LWSG_MENUID, Mex_HighLight_LWSG_Menu);
#endif
#if defined(__MEX_MFWY_SUPPORT__)
					SetHiliteHandler(MET_MEX_MFWY_MENUID, Mex_HighLight_LWSG_Menu);
#endif
	}
#endif

#if defined(__MEX_QQNC_SUPPORT__)||defined(__MEX_KXNC_SUPPORT__)
		{
						extern void Mex_HighLight_Farm_Menu(void);
#if defined(__MEX_KXNC_SUPPORT__)
						SetHiliteHandler(MET_MEX_KXNC_MENUID, Mex_HighLight_Farm_Menu);
#endif
#if defined(__MEX_QQNC_SUPPORT__)
						SetHiliteHandler(MET_MEX_QQNC_MENUID, Mex_HighLight_Farm_Menu);
#endif
		}
#endif


#if 1//__MEX_MENU_HIDE_SUPPORT__
{
				extern void Mex_HighLight_ZWDZJS_Menu(void);
#if 1//defined(__MEX_ZWDZJS_SUPPORT__)
				SetHiliteHandler(MET_MEX_ZWDZJS_MENUID, Mex_HighLight_ZWDZJS_Menu);
#endif
#if 0//defined(__MEX_JSDZ_SUPPORT__)
				SetHiliteHandler(MET_MEX_JSDZ_MENUID, Mex_HighLight_ZWDZJS_Menu);
#endif
}
#endif

#if 1//__MEX_MENU_HIDE_SUPPORT__
{
					extern void Mex_HighLight_DDZ_Menu(void);
#if 1//defined(__MEX_DDZ_SUPPORT__)
					SetHiliteHandler(MET_MEX_DDZ_MENUID, Mex_HighLight_DDZ_Menu);
#endif
#if 0//defined(__MEX_MVDDZ_SUPPORT__)
					SetHiliteHandler(MET_MEX_MVDDZ_MENUID, Mex_HighLight_DDZ_Menu);
#endif
}
#endif

#if defined(__MEX_FKCS_SUPPORT__)||defined(__MEX_CYHX_SUPPORT__)
{
					extern void Mex_HighLight_FKCS_Menu(void);
#if defined(__MEX_FKCS_SUPPORT__)
					SetHiliteHandler(MET_MEX_FKCS_MENUID, Mex_HighLight_FKCS_Menu);
#endif
#if defined(__MEX_CYHX_SUPPORT__)
					SetHiliteHandler(MET_MEX_CYHX_MENUID, Mex_HighLight_FKCS_Menu);
#endif
}
#endif

#if defined(__MEX_JSCB_SUPPORT__)||defined(__MEX_KXCD_SUPPORT__)
{
					extern void Mex_HighLight_JSCB_Menu(void);
#if defined(__MEX_JSCB_SUPPORT__)
					SetHiliteHandler(MET_MEX_JSCB_MENUID, Mex_HighLight_JSCB_Menu);
#endif
#if defined(__MEX_KXCD_SUPPORT__)
					SetHiliteHandler(MET_MEX_KXCD_MENUID, Mex_HighLight_JSCB_Menu);
#endif
}
#endif

#if defined(__MEX_CPTZ_SUPPORT__)||defined(__MEX_CPZDJ_SUPPORT__)
	{
					extern void Mex_HighLight_CSJ_Menu(void);
#if defined(__MEX_CPTZ_SUPPORT__)
					SetHiliteHandler(MET_MEX_CPTZ_MENUID, Mex_HighLight_CSJ_Menu);
#endif
#if defined(__MEX_CPZDJ_SUPPORT__)
					SetHiliteHandler(MET_MEX_CPZDJ_MENUID, Mex_HighLight_CSJ_Menu);
#endif
	}
#endif

#if defined(__MEX_GJWZF_SUPPORT__)||defined(__MEX_MFZF_SUPPORT__)
	{
					extern void Mex_HighLight_ZFT_Menu(void);
#if defined(__MEX_GJWZF_SUPPORT__)
					SetHiliteHandler(MET_MEX_GJWZF_MENUID, Mex_HighLight_ZFT_Menu);
#endif
#if defined(__MEX_MFZF_SUPPORT__)
					SetHiliteHandler(MET_MEX_MFZF_MENUID, Mex_HighLight_ZFT_Menu);
#endif
	}

#endif

#if defined(__MEX_YMMH_SUPPORT__)||defined(__MEX_YMDS_SUPPORT__)
	{
					extern void Mex_HighLight_YMDS_Menu(void);
#if defined(__MEX_YMMH_SUPPORT__)
					SetHiliteHandler(MET_MEX_YMMH_MENUID, Mex_HighLight_YMDS_Menu);
#endif
#if defined(__MEX_YMDS_SUPPORT__)
					SetHiliteHandler(MET_MEX_YMDS_MENUID, Mex_HighLight_YMDS_Menu);
#endif
	}
#endif

#if defined(__MEX_SDYY_SUPPORT__)||defined(__MEX_WYJC_SUPPORT__)
{
					extern void Mex_HighLight_Player_Menu(void);
#if defined(__MEX_SDYY_SUPPORT__)
					SetHiliteHandler(MET_MEX_SDYY_MENUID, Mex_HighLight_Player_Menu);
#endif
#if defined(__MEX_WYJC_SUPPORT__)
					SetHiliteHandler(MET_MEX_WYJC_MENUID, Mex_HighLight_Player_Menu);
#endif
	}
#endif

#if defined(__MEX_HDHL_SUPPORT__)||defined(__MEX_YFDDP_SUPPORT__)
	{
					extern void Mex_HighLight_HDHL_Menu(void);
#if defined(__MEX_HDHL_SUPPORT__)
					SetHiliteHandler(MET_MEX_HDHL_MENUID, Mex_HighLight_HDHL_Menu);
#endif
#if defined(__MEX_YFDDP_SUPPORT__)
					SetHiliteHandler(MET_MEX_YFDDP_MENUID, Mex_HighLight_HDHL_Menu);
#endif
	}
#endif


#if defined(__MEX_SGDJT_SUPPORT__)||defined(__MEX_BJLT_SUPPORT__)
{
					extern void Mex_HighLight_HXZ_Menu(void);
#if defined(__MEX_SGDJT_SUPPORT__)
					SetHiliteHandler(MET_MEX_SGDJT_MENUID, Mex_HighLight_HXZ_Menu);
#endif
#if defined(__MEX_BJLT_SUPPORT__)
					SetHiliteHandler(MET_MEX_BJLT_MENUID, Mex_HighLight_HXZ_Menu);
#endif
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
				extern void Mex_HighLight_FNDXN_Menu(void);
#if 1//defined(__MEX_FNDXN_SUPPORT__)
				SetHiliteHandler(MET_MEX_FNDXN_MENUID, Mex_HighLight_FNDXN_Menu);
#endif
#if 0//defined(__MEX_ABIRD_SUPPORT__)
				SetHiliteHandler(MET_MEX_ABIRD_MENUID, Mex_HighLight_FNDXN_Menu);
#endif
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_QSG_Menu(void);
				SetHiliteHandler(MET_MEX_QSG_MENUID, Mex_HighLight_QSG_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_SHSL_Menu(void);
				SetHiliteHandler(MET_MEX_SHSL_MENUID, Mex_HighLight_SHSL_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__)
{
                extern void Mex_HighLight_CSJJS_Menu(void);
				SetHiliteHandler(MET_MEX_CSJJS_MENUID, Mex_HighLight_CSJJS_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_BLFC_Menu(void);
				SetHiliteHandler(MET_MEX_BLFC_MENUID, Mex_HighLight_BLFC_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_MXSQY_Menu(void);
				SetHiliteHandler(MET_MEX_MXSQY_MENUID, Mex_HighLight_MXSQY_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_DZT_Menu(void);
				SetHiliteHandler(MET_MEX_DZT_MENUID, Mex_HighLight_DZT_Menu);
}
#endif

#if defined(__MEX_AQ_SUPPORT__)
{
                extern void Mex_HighLight_Aq_Menu(void);
				SetHiliteHandler(MET_MEX_AQ_MENUID, Mex_HighLight_Aq_Menu);
}
#endif

#if defined(__MEX_HTC_TQ_SUPPORT__)
{
                extern void Mex_HighLight_HTC_TQ_Menu(void);
				SetHiliteHandler(MET_MEX_HTC_TQ_MENUID, Mex_HighLight_HTC_TQ_Menu);
}
#endif

#if defined(__MEX_Lighter_SUPPORT__)
{
                extern void Mex_HighLight_Lighter_Menu(void);
				SetHiliteHandler(MET_MEX_Lighter_MENUID, Mex_HighLight_Lighter_Menu);
}
#endif
#if defined(__MEX_MLSZ_SUPPORT__)
{
                extern void Mex_HighLight_MLSZ_Menu(void);
				SetHiliteHandler(MET_MEX_MLSZ_MENUID, Mex_HighLight_MLSZ_Menu);
}
#endif
#if defined(__MEX_HDtqyb_SUPPORT__)
{
                extern void Mex_HighLight_HDtqyb_Menu(void);
				SetHiliteHandler(MET_MEX_HDtqyb_MENUID, Mex_HighLight_HDtqyb_Menu);
}
#endif
#if defined(__MEX_MvBol_SUPPORT__)
{
                extern void Mex_HighLight_MvBol_Menu(void);
		SetHiliteHandler(MET_MEX_MvBol_MENUID, Mex_HighLight_MvBol_Menu);
}
#endif
#if defined(__MEX_HDcqz_SUPPORT__)
{
                extern void Mex_HighLight_HDcqz_Menu(void);
				SetHiliteHandler(MET_MEX_HDcqz_MENUID, Mex_HighLight_HDcqz_Menu);
}
#endif
#if defined(__MEX_DMC_SUPPORT__)
{
                extern void Mex_HighLight_DMC_Menu(void);
				SetHiliteHandler(MET_MEX_DMC_MENUID, Mex_HighLight_DMC_Menu);
}
#endif
#if defined(__MEX_SETTIME_SUPPORT__)
{
                extern void Mex_HighLight_SETTIME_Menu(void);
				SetHiliteHandler(MET_MEX_SETTIME_MENUID, Mex_HighLight_SETTIME_Menu);
}
#endif
#if defined(__MEX_JJFB_SUPPORT__)
{
                extern void Mex_HighLight_JJFB_Menu(void);
				SetHiliteHandler(MET_MEX_JJFB_MENUID, Mex_HighLight_JJFB_Menu);
}
#endif
#if defined(__MEX_JPFCKB_SUPPORT__)
{
                extern void Mex_HighLight_JPFCKB_Menu(void);
				SetHiliteHandler(MET_MEX_JPFCKB_MENUID, Mex_HighLight_JPFCKB_Menu);
}
#endif
#if defined(__MEX_GANJILIFE_SUPPORT__)
{
                extern void Mex_HighLight_GANJILIFE_Menu(void);
				SetHiliteHandler(MET_MEX_GANJILIFE_MENUID, Mex_HighLight_GANJILIFE_Menu);
}
#endif
#if defined(__MEX_MEXSQAPP_SUPPORT__)
{
                extern void Mex_HighLight_MEXSQAPP_Menu(void);
				SetHiliteHandler(MET_MEX_MEXSQAPP_MENUID, Mex_HighLight_MEXSQAPP_Menu);
}
#endif

#if defined(__MEX_RENREN_SUPPORT__)
{
                extern void Mex_HighLight_Renren_Menu(void);
				SetHiliteHandler(MET_MEX_RENREN_MENUID, Mex_HighLight_Renren_Menu);
}
#endif
#if defined(__MEX_KAIXIN_SUPPORT__)
{
                extern void Mex_HighLight_Kaixin_Menu(void);
				SetHiliteHandler(MET_MEX_KAIXIN_MENUID, Mex_HighLight_Kaixin_Menu);
}
#endif
#if defined(__MEX_TAOBAO_SUPPORT__)
{
                extern void Mex_HighLight_Taobao_Menu(void);
				SetHiliteHandler(MET_MEX_TAOBAO_MENUID, Mex_HighLight_Taobao_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_CQZ_Menu(void);
				SetHiliteHandler(MET_MEX_CQZ_MENUID, Mex_HighLight_CQZ_Menu);
}
#endif
#if 1//__MEX_MENU_HIDE_SUPPORT__
{
                extern void Mex_HighLight_TOMMAO_Menu(void);
				SetHiliteHandler(MET_MEX_TOMMAO_MENUID, Mex_HighLight_TOMMAO_Menu);
}
#endif
#if defined(__MEX_BULLETSFLY_SUPPORT__)
{
                extern void Mex_HighLight_BULLETSFLY_Menu(void);
				SetHiliteHandler(MET_MEX_BULLETSFLY_MENUID, Mex_HighLight_BULLETSFLY_Menu);
}
#endif

#endif /* MEX_MTK_CODE_VERSION */


	MexConfig_AllocKermem();

#ifdef MEX_ST_SUPPORT
{
	extern void MexStInit(void);
	MexStInit();
}
#endif

{
	extern void MexLib_Os_Init_File_Pool(void);
	MexLib_Os_Init_File_Pool();
}

#if defined(__MEX_DMC_SUPPORT__)
{
	extern void Mex_HighLight_DMC_Menu(void);
	extern void DMC_TimerCallback(void);
#if !(defined(__MTK_TARGET__)) && (MEX_MTK_CODE_VERSION >= 0x1032)
	SetHiliteHandler(0, Mex_HighLight_DMC_Menu);
#else
	SetHiliteHandler(MET_MEX_DMC_MENUID, Mex_HighLight_DMC_Menu);
#endif
	DMC_TimerCallback();
}
#endif


}


#ifdef MEX_INSTALL_RUN_PACKAGE_SUPPORT

typedef enum
{
	E_MIS_STATE_IDLE,
	E_MIS_STATE_UNZIP,
	E_MIS_STATE_INSTALL,
    E_MIS_STATE_OK,
    E_MIS_STATE_FAIL,
	E_MIS_STATE_MAX
} Mex_Install_State;

typedef struct
{
	bool            m_bCanPaint;
	int32           m_iHintStrWidth;       // 提示字串宽
	int32           m_iHintStrHeight;      // 提示字串高
	int32           m_iHintStrNbChar;
	uint8           m_iNbDot;              // dot 数量
	uint8           m_eState;              // 安装状态
    int32           m_iPercent;            // 进度百分比
	char            m_szAppName[MEX_SHORT_FILE_NAME_MAX_LENGTH];
    int32           m_hPatchInst;
} Mex_Install_Context;

static Mex_Install_Context g_MexInstallContext = { 0 };

static mex_color Mex_Entry_Install_GetBarColor(int32 iTotalWidth, int32 iProgWidth)
{
	int32 i, segWidth;
	int16 iRed[6]   = { 0xF8, 0xF8, 0xB8, 0x00, 0x70, 0xF8 };
	int16 iGreen[6] = { 0x20, 0xDE, 0xE0, 0x60, 0x28, 0x20 };
	int16 iBlue[6]  = { 0x00, 0x00, 0x10, 0xC8, 0xE8, 0x00 };
	int16 R, G, B, perLeft;
	segWidth = iTotalWidth / 5;
	i = iProgWidth / segWidth;
	perLeft = iProgWidth - i * segWidth;
	R = (iRed[i+1] * perLeft + iRed[i] * (segWidth - perLeft)) / segWidth;
	G = (iGreen[i+1] * perLeft + iGreen[i] * (segWidth - perLeft)) / segWidth;
	B = (iBlue[i+1] * perLeft + iBlue[i] * (segWidth - perLeft)) / segWidth;
	return ((R >> 3 << 11) | (G >> 2 << 5) | (B >> 3));
}

static void Mex_Entry_Install_Paint(uint16 *pBuffer, uint16 iWidth, uint16 iHeight)
{
    int16 i;
    int16 x1, y1;
    int16 x2, y2;
    char szHint[] = "正在安装，请稍候...";

    x1 = iWidth / 10;
    x2 = iWidth - x1;
    y1 = iHeight / 2;
    y2 = y1 + 20;

    switch (g_MexInstallContext.m_eState)
    {
    case E_MIS_STATE_IDLE:
        {
            // Draw bg
            mex_memset16(pBuffer, MEX_COLOR_BLACK, iWidth * iHeight);
            g_MexInstallContext.m_iNbDot = 1;
            g_MexInstallContext.m_iHintStrNbChar = strlen(szHint);
            mex_GetStringDimension(
                mex_tstr2wcs(szHint),
                &g_MexInstallContext.m_iHintStrWidth,
                &g_MexInstallContext.m_iHintStrHeight,
                MEX_FONTATTR_NORMAL,
                MEX_FONTSIZE_MEDIUM);
            mex_DrawFillRect(x1, y1, x2, y2, MEX_COLOR_WHITE);
        }
    	break;
    case E_MIS_STATE_UNZIP:
    case E_MIS_STATE_INSTALL:
        {
            int16 iProgWidth;
            mex_color barColor;
            iProgWidth = x1 + (x2 - x1) * g_MexInstallContext.m_iPercent / 100;
            if (iProgWidth > x2 - 2)
                iProgWidth = x2 - 2;
            barColor = Mex_Entry_Install_GetBarColor(x2 - x1, iProgWidth - x1);
            mex_DrawFillRect(x1 + 2, y1 + 2, iProgWidth, y2 - 2, barColor);
        }
        break;
    case E_MIS_STATE_OK:
        break;
    }

    for (i = 0; i < 3 - g_MexInstallContext.m_iNbDot; i++)
        szHint[g_MexInstallContext.m_iHintStrNbChar - 1 - i] = '\0';
    x1 = (iWidth - g_MexInstallContext.m_iHintStrWidth) / 2;
    y1 = y1 - g_MexInstallContext.m_iHintStrHeight - 8;
    x2 = x1 + g_MexInstallContext.m_iHintStrWidth;
    y2 = y1 + g_MexInstallContext.m_iHintStrHeight;
    mex_DrawFillRect(x1, y1, x2, y2, MEX_COLOR_BLACK);
    mex_DrawString(
        mex_tstr2wcs(szHint),
        x1,
        y1,
        MEX_COLOR_WHITE,
        MEX_FONTATTR_NORMAL,
        MEX_FONTSIZE_MEDIUM);
    g_MexInstallContext.m_iNbDot++;
    if (g_MexInstallContext.m_iNbDot > 3)
        g_MexInstallContext.m_iNbDot = 1;

}

static void Mex_Entry_Install_Cleanup(void)
{
    if (g_MexInstallContext.m_hPatchInst != NULL)
    {
        mex_MP_Cleanup(g_MexInstallContext.m_hPatchInst);
        g_MexInstallContext.m_hPatchInst = NULL;
    }
}

static void Mex_Entry_Install_Finish_CB(bool bIsOk)
{
    extern void Mex_Entry_Install_Ok(void);
    wchar_t *szInsAppName;
    mex_log(MEX_LOG_SYSTEM, "I: II finish res:%d", bIsOk);
    // Get App Name
    szInsAppName = mex_MP_GetCurPatchAppName(g_MexInstallContext.m_hPatchInst);
    mex_wcs2nstr(g_MexInstallContext.m_szAppName, MEX_SHORT_FILE_NAME_MAX_LENGTH, szInsAppName);
    // Cleanup
    Mex_Entry_Install_Cleanup();

    if (bIsOk)
    {
        Mex_Entry_Install_Ok();
    }
    DeleteScreenIfPresent(E_MSI_INSTALL_APP_SCREEN);
}


static void Mex_Entry_Install_Progress_CB(Mex_Install_Info *pInfo)
{
	uint16 *pBuffer;
	uint16 iWidth, iHeight;

    if (pInfo->m_iTotalNbUpdate == 0 && pInfo->m_iTotalNbDelete == 0)
    {
        if (pInfo->m_iTotalByteToUnc == 0 && pInfo->m_iNbBytesUnc == 0)
        {
            g_MexInstallContext.m_eState = E_MIS_STATE_IDLE;
        }
        else
        {
            g_MexInstallContext.m_eState = E_MIS_STATE_UNZIP;
            g_MexInstallContext.m_iPercent = pInfo->m_iNbBytesUnc * 100 / pInfo->m_iTotalByteToUnc;
            g_MexInstallContext.m_iPercent /= 2;
        }
    }
    else
    {
        g_MexInstallContext.m_eState = E_MIS_STATE_INSTALL;
        g_MexInstallContext.m_iPercent = pInfo->m_iNbUpdated * 100 / pInfo->m_iTotalNbUpdate;
        g_MexInstallContext.m_iPercent /= 2;
        g_MexInstallContext.m_iPercent += 50;
    }
    mex_BeginPaint();
    mex_GetActScreenBuffer(&pBuffer, &iWidth, &iHeight);
    Mex_Entry_Install_Paint(pBuffer, iWidth, iHeight);
	mex_EndPaint();
}


static void Mex_Entry_Install_CheckTCard(void)
{
    //检测存储卡...
    bool bTCardOk = (bool) MexConfig_GetPhoneConfig(E_MPC_Tcard_State);
	if (!bTCardOk)
	{
		mex_log(MEX_LOG_SYSTEM, "E: II TCard");
		Mex_Entry_Install_Cleanup();
		if (g_MexInstallContext.m_bCanPaint)
		{
			GoBackHistory();
		}
		else
		{
			DeleteScreenIfPresent(E_MSI_INSTALL_APP_SCREEN);
		}
	}
}


static void Mex_Entry_Install_Exit(void)
{
	g_MexInstallContext.m_bCanPaint = false;
    Mex_Entry_Install_CheckTCard();
}

static void Mex_Entry_Install_GetFocus(void)
{
	Mex_Install_Info InstInfo = { 0 };
	EntryNewScreen(E_MSI_INSTALL_APP_SCREEN, Mex_Entry_Install_Exit, Mex_Entry_Install_GetFocus, NULL);
    SetDelScrnIDCallbackHandler(E_MSI_INSTALL_APP_SCREEN, (HistoryDelCBPtr)Mex_Entry_Install_Cleanup);
    g_MexInstallContext.m_bCanPaint = true;
	Mex_Entry_Install_CheckTCard();
	Mex_Entry_Install_Progress_CB(&InstInfo); // Draw bg
}

static void Mex_Entry_Install_Ok(void)
{
    char szParam[MEX_SHORT_FILE_NAME_MAX_LENGTH];
    mex_log(MEX_LOG_MANAGER, "I: II app: %s", g_MexInstallContext.m_szAppName);
    strcpy(szParam, g_MexInstallContext.m_szAppName);
    strcat(szParam, ".mex");
	Mex_SetCurrentEntryAppInfo(szParam, E_MSVT_Normal);
	strcat(szParam, ":");
    assert(sizeof(szParam) > strlen(szParam));
	MexSystem_SetSystemInitParam(szParam);
	MexConfig_SetTinyLoadingPage( false,
								  MEX_COLOR_MEDIUMPURPLE,
								  MEX_COLOR_WHITE,
								  MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								  (const wchar_t *)"\x63\x6B\x28\x57\x1D\x52\xCB\x59\x16\x53\xFB\x7C\xDF\x7E\x2E\x0\x2E\x0\x2E\x0\0" ); 	//正在初始化系统...
	MexConfig_SetManLoadingPage( false,
								 MEX_COLOR_MEDIUMPURPLE,
								 MEX_COLOR_WHITE,
								 MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								 (const wchar_t *)"\x63\x6B\x28\x57\xA0\x52\x7D\x8F\x94\x5E\x28\x75\x0B\x7A\x8F\x5E\x2E\x00\x2E\x00\x2E\x00\x00" );		//正在加载应用程序...
	Mex_Entry_Screen_Internal( Mex_Entry_Install_Ok );
}

static void Mex_Entry_Install_Thread(void)
{
    extern int32 PatchLib_MainLoop(int32 hPatch);
	Mex_Entry_Install_CheckTCard();
    if (g_MexInstallContext.m_bCanPaint &&
        g_MexInstallContext.m_hPatchInst != NULL)
    {
    	mex_Phs_Set_Backlight(E_MPHS_SHORT_TIME);
        PatchLib_MainLoop(g_MexInstallContext.m_hPatchInst);
    }
    if (g_MexInstallContext.m_hPatchInst != NULL)
    {
        StartTimer(MexConfig_GetMMITimerId(), 5, Mex_Entry_Install_Thread);
    }
}

// 安装游戏包并运行游戏
// 参数1: szPackageName 游戏包的全路径
// 返回: 无
void Mex_Entry_InstallAndRunPackage(const wchar_t *szPackageName)
{
    mex_log(MEX_LOG_SYSTEM, "I: II pak: %s", mex_twcs2str(szPackageName));
    assert(g_MexInstallContext.m_hPatchInst == NULL);

    g_MexInstallContext.m_hPatchInst = mex_MP_ProcessPatchFile(szPackageName, Mex_Entry_Install_Progress_CB, Mex_Entry_Install_Finish_CB);

    if (g_MexInstallContext.m_hPatchInst != NULL)
    {
        // 检测是否需要安装游戏包?
        bool bNeedInstall = mex_MP_IsPatchNeeded(szPackageName);
        if (bNeedInstall)
        {
            EntryNewScreen(E_MSI_INSTALL_APP_SCREEN, Mex_Entry_Install_Exit, Mex_Entry_Install_GetFocus, NULL);
            SetDelScrnIDCallbackHandler(E_MSI_INSTALL_APP_SCREEN, (HistoryDelCBPtr)Mex_Entry_Install_Cleanup);
            g_MexInstallContext.m_bCanPaint = true;
            //create sys dir first
            mex_CreateDir_ext_recur( mex_GetSystemRootDir() );
            mex_CreateDir_ext_recur( mex_GetAppRootDir() );
            //create timer
            StartTimer(MexConfig_GetMMITimerId(), 5, Mex_Entry_Install_Thread);
        }
        else
        {
            wchar_t *szInsAppName;
            mex_log(MEX_LOG_MANAGER, "I: II ok");
            // Get App Name
            szInsAppName = mex_MP_GetCurPatchAppName(g_MexInstallContext.m_hPatchInst);
            mex_wcs2nstr(g_MexInstallContext.m_szAppName, MEX_SHORT_FILE_NAME_MAX_LENGTH, szInsAppName);
            // Cleanup
            Mex_Entry_Install_Cleanup();
            Mex_Entry_Install_Ok();
        }
    }
    else
    {
        extern int32 mex_MP_GetLastError(void);
        mex_log(MEX_LOG_SYSTEM, "I: II err:%d", mex_MP_GetLastError());
    }
}

extern int32* mex_fopenw_org_internal(const wchar_t *filename, const wchar_t *mode);

bool Mex_Entry_Install_DeleteApp(const wchar_t *szPackageName)
{
    wchar_t szAppName[40] = { 0 };
    uint8 uAppNameLen = 0;
    int32 iOldPos = 0;
    int32 *file = mex_fopenw_org_internal(szPackageName, L"rb");
    if (file == NULL)
        return false;
    mex_fseek(file, -4, SEEK_END);
    mex_fread(&iOldPos, 4, 1, file);
    if (iOldPos > 0)
    {
        mex_fseek(file, iOldPos, SEEK_SET);
        mex_fseek(file, 2, SEEK_CUR);	//skip app version
        mex_fread(&uAppNameLen, 1, 1, file);
        if (uAppNameLen > 0 && uAppNameLen < 38)
        {
            mex_fread(szAppName, 1, uAppNameLen, file);
            mex_log(MEX_LOG_SYSTEM, "I: Delete App:%s", mex_twcs2str(szAppName));
        }
    }
    mex_fclose(file);

    if (szAppName[0] != L'\0')
    {
        wchar_t szFullPath[MEX_FILE_NAME_MAX_LENGTH] = { 0 };
        mex_wcscpy(szFullPath, mex_GetAppRootDir());
        mex_wcscat(szFullPath, L"\\");
        mex_wcscat(szFullPath, szAppName);
        mex_RemoveDir_ext(szFullPath);
        mex_log(MEX_LOG_SYSTEM, "I: Delete App Dir:%s", mex_twcs2str(szFullPath));
        return true;
    }
    return false;
}


void Mex_Entry_Install_Test(void)
{
    FS_HANDLE pFile;
    wchar_t cfgFileName[40] = { 0 };

    mex_wcscpy( cfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_SystemRootDir) );
    mex_wcscat( cfgFileName, L"\\MexInstall.cfg" );

    pFile = FS_Open( cfgFileName, FS_READ_ONLY );
    if (pFile > 0)
    {
        int16 i;
        static char szPackage[100] = { 0 };
        uint32 iRead;
        memset(szPackage, 0, 100);
        FS_Read( pFile, szPackage, 100, &iRead );
        FS_Close( pFile );
        for (i = mex_strlen(szPackage) - 1; i > 0; i--)
        {
            if (szPackage[i] == '\r' || szPackage[i] == '\n' || szPackage[i] == ' ' || szPackage[i] == '\t' || szPackage[i] == 26)
                szPackage[i] = 0;
        }
        Mex_Entry_InstallAndRunPackage(mex_str2nwcs(cfgFileName, 40 * 2, szPackage));
        return;
    }
    mex_log(MEX_LOG_SYSTEM, "E: no MexInstall.cfg");
}


#endif /* MEX_INSTALL_RUN_PACKAGE_SUPPORT */

static char *tempstr = NULL;
static U8 *mex_temp_path = NULL;

int32 iMexEntrySecondHighLightItemIndex = 0;
wchar_t szMexEntrySecondPlatName[20] = {0};

//记录当前独立入口程序名及Id
char pCurrEntryAppName[200] = { 0 };
int32 iCurrEntryAppID = 0;


#ifdef MEX_QA_MODE
//int g_mex_entry_second_limit = 30;
int g_mex_entry_second_limit = 1;
extern void Mex_Entry_Second_DeleteCfgFile(void);
#endif
extern MexFuncPtr pMexEntrySecondStartFunc;
extern MexFuncPtr pMexEntrySecondAboutFunc;
extern void Mex_Entry_Second_GetPlatFormName( wchar_t * szPlatName );
extern void Mex_Entry_Second_RegisterMenuFunc( MexFuncPtr pStartFunc, MexFuncPtr pAboutFunc );
extern bool Mex_Entry_Second_CheckCfgFile(void);
extern void Mex_Entry_Second_Make_Record(	int32 iLimitCount, bool bOnceADay );


void Mex_Entry_Second_Confirm_Handler(void)
{
	if( iMexEntrySecondHighLightItemIndex == 0 )
	{
		if( pMexEntrySecondStartFunc != NULL )
		{
			(*pMexEntrySecondStartFunc)();
		}
	}
	else if( iMexEntrySecondHighLightItemIndex == 1 )
	{
		Mex_Entry_MainScreen();
	}
	else if( iMexEntrySecondHighLightItemIndex == 2 )
	{
		if( pMexEntrySecondAboutFunc != NULL )
		{
			(*pMexEntrySecondAboutFunc)();
		}
	}
	#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
	else if(iMexEntrySecondHighLightItemIndex == 3)
	{
		extern void Mex_Entry_ApnSelectScreen(void);
		Mex_Entry_ApnSelectScreen();
	}
	#endif

}

void Mex_Entry_Second_Highlight_Handler(S32 index)
{
	iMexEntrySecondHighLightItemIndex = index;
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(Mex_Entry_Second_Confirm_Handler);
}

//进入独立入口菜单下的二级菜单
void Mex_Entry_Second_MenuList( void )
{
		U8 * guiBuffer;
		S16 item_offset = 0;
		U16 list_of_icons[4] = { IMG_GLOBAL_L1,
								IMG_GLOBAL_L2,
								IMG_GLOBAL_L3,
			#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
								IMG_GLOBAL_L4
			#endif
								};
		U8 *inputBuffer;
		U16 inputBufferSize;

		EntryNewScreen(E_MSI_SECOND_MENULIST,NULL,Mex_Entry_Second_MenuList,NULL);

		guiBuffer = GetCurrGuiBuffer(E_MSI_SECOND_MENULIST);
		inputBuffer = GetCurrNInputBuffer(E_MSI_SECOND_MENULIST, &inputBufferSize);
		InitializeCategory57Screen();
		iMexEntrySecondHighLightItemIndex = 1;		//每次默认停留在第二项

		RegisterHighlightHandler(Mex_Entry_Second_Highlight_Handler);

		Mex_Entry_Second_GetPlatFormName( szMexEntrySecondPlatName );
#if (MEX_MTK_CODE_VERSION < 0x1112)
		SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\x2F\x54\xA8\x52\0"));
		SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)(szMexEntrySecondPlatName));
		SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\x73\x51\x8E\x4E\0"));
		#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
		SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\x51\x7F\xDC\x7E\xBE\x8B\x6E\x7F\0"));
		#endif
#else
		// TODO:
		assert(0);
#endif

#if (MEX_MTK_CODE_VERSION < 0x1112)
		ShowCategory57Screen(
			0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, item_offset,list_of_icons,
			wgui_inline_items,
			iMexEntrySecondHighLightItemIndex,//animList.themeId,
			guiBuffer
			);
#else
		// TODO:
		assert(0);
#endif

		MMI_title_string = (UI_string_type)("\x9F\x52\xFD\x80\xDC\x83\x55\x53\0");	//功能菜单
		draw_title();

		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetCategory57LeftSoftkeyFunction(Mex_Entry_Second_Confirm_Handler);
		DisableCategory57ScreenDone();
		SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);

}


#ifdef MEX_SEL_DATAACCOUNT_SUPPORT

MexFuncPtr pMexEntryMenuStartFunc;
MexFuncPtr pMexEntryMenuSettingFunc;

void Mex_Entry_Menu_RegisterMenuFunc( MexFuncPtr pStartFunc, MexFuncPtr pSettingFunc )
{
	pMexEntryMenuStartFunc = pStartFunc;
	pMexEntryMenuSettingFunc = pSettingFunc;
}
void Mex_Entry_Menu_Confirm_Handler(void)
{
	if( iMexEntrySecondHighLightItemIndex == 0 )
	{
		if( pMexEntryMenuStartFunc != NULL )
		{
			(*pMexEntryMenuStartFunc)();
		}
	}
	else if( iMexEntrySecondHighLightItemIndex == 1 )
	{
		if( pMexEntryMenuSettingFunc != NULL )
		{
			(*pMexEntryMenuSettingFunc)();
		}
	}

}

void Mex_Entry_Menu_Highlight_Handler(S32 index)
{
	iMexEntrySecondHighLightItemIndex = index;
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(Mex_Entry_Menu_Confirm_Handler);
}

void Mex_Entry_Menu_MenuList( void )
{
		U8 * guiBuffer;
		S16 item_offset = 0;
		U16 list_of_icons[2] = { IMG_GLOBAL_L1,IMG_GLOBAL_L2};
		U8 *inputBuffer;
		U16 inputBufferSize;

		EntryNewScreen(E_MSI_SECOND_MENULIST,NULL,Mex_Entry_Menu_MenuList,NULL);

		guiBuffer = GetCurrGuiBuffer(E_MSI_SECOND_MENULIST);
		inputBuffer = GetCurrNInputBuffer(E_MSI_SECOND_MENULIST, &inputBufferSize);
		InitializeCategory57Screen();
		iMexEntrySecondHighLightItemIndex = 0;

		RegisterHighlightHandler(Mex_Entry_Menu_Highlight_Handler);

		//Mex_Entry_Second_GetPlatFormName( szMexEntrySecondPlatName );
		SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\x2F\x54\xA8\x52\0"));
		//SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)(szMexEntrySecondPlatName));
		SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\x51\x7F\xDC\x7E\xBE\x8B\x6E\x7F\0"));

		ShowCategory57Screen(
			0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, item_offset,list_of_icons,
			wgui_inline_items,
			iMexEntrySecondHighLightItemIndex,//animList.themeId,
			guiBuffer
			);

		MMI_title_string = (UI_string_type)("\x2F\x54\xA8\x52\x9\x90\x79\x98\0");
		draw_title();

		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetCategory57LeftSoftkeyFunction(Mex_Entry_Menu_Confirm_Handler);
		DisableCategory57ScreenDone();
		SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);

}

#endif


//记录当前独立入口信息
void Mex_SetCurrentEntryAppInfo(char *pAppName, int32 iAppID)
{
#ifdef MEX_USE_ASSERT
	{
		int iLen = strlen(pAppName);
		assert(iLen == 0 || (iLen > 4 && mex_stricmp(pAppName + iLen - 4, ".mex") == 0));
	}
#endif

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

//__MEX_MENU_HIDE_SUPPORT__
#if defined(__MEX_WAMKF_SUPPORT__)  || defined(__MEX_KGZW_SUPPORT__)    ||  \
    defined(__MEX_FETION_SUPPORT__) || defined(__MEX_WMSP_SUPPORT__)    ||   \
    defined(__MEX_TCJY_SUPPORT__)   || defined(__MEX_QQ_SUPPORT__)      ||   \
    defined(__MEX_MSN_SUPPORT__)     || defined(__MEX_SINA_SUPPORT__)   || \
    defined(__MEX_BULLETSFLY_SUPPORT__)     ||   \
    defined(__MEX_JSCB_SUPPORT__)   || defined(__MEX_KXCD_SUPPORT__)  || \
    defined(__MEX_AQ_SUPPORT__)
   
#ifdef MAX_APP_PATH_NUM
#undef MAX_APP_PATH_NUM
#endif
#ifdef MAX_APP_NAME_NUM
#undef MAX_APP_NAME_NUM
#endif
#ifdef MAX_APP_DESCRIBEINFO_NUM
#undef MAX_APP_DESCRIBEINFO_NUM
#endif
#ifdef MAX_APP_INITPARAM_NUM
#undef MAX_APP_INITPARAM_NUM
#endif

#define MAX_APP_PATH_NUM            31
#define MAX_APP_NAME_NUM            31
#define MAX_APP_DESCRIBEINFO_NUM    7
#define MAX_APP_INITPARAM_NUM       31
static wchar_t g_AppPath[MAX_APP_PATH_NUM+1] = {0};
static char g_AppName[MAX_APP_NAME_NUM+1] = {0};
static wchar_t g_DesctribeInfo[MAX_APP_DESCRIBEINFO_NUM+1] = {0};
static wchar_t g_InitParam[MAX_APP_INITPARAM_NUM+1] = {0};
static uint32 g_iAppid = 0;


/********************************************************
	second menu entry list struct in MTK 11A
*********************************************************/
#if (MEX_MTK_CODE_VERSION >= 0x1112)
static cui_inline_item_display_only_struct g_mex_second_menu_list_button[] =
{
	STR_GLOBAL_CANCEL,
	STR_GLOBAL_CANCEL,
};
static const cui_inline_set_item_struct g_mex_second_menu_list_inline[] = 
{
	{CUI_INLINE_ITEM_ID_BASE+0,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_second_menu_list_button},
	{CUI_INLINE_ITEM_ID_BASE+1,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_second_menu_list_button},
	{CUI_INLINE_ITEM_ID_BASE+2,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_second_menu_list_button},
};
static const cui_inline_item_softkey_struct g_mex_second_menu_list_sofkey =
{
	{{CUI_INLINE_SOFTKEY_DEFAULT_VALUE, 0},
	{CUI_INLINE_SOFTKEY_DEFAULT_VALUE, 0},
	{STR_GLOBAL_CANCEL, 0}}        
};

static const cui_inline_struct g_mex_second_menu_list_scrn = 
{
	(sizeof(g_mex_second_menu_list_inline)/sizeof(cui_inline_set_item_struct)),
	NULL,
	0,
	CUI_INLINE_SCREEN_LOOP|CUI_INLINE_SCREEN_DISABLE_DONE,
	(cui_inline_item_softkey_struct*)&g_mex_second_menu_list_sofkey,
	(const cui_inline_set_item_struct*)&g_mex_second_menu_list_inline,
};
static int8 iMexSecondMenuItem = 0;
static int8 iMexSecondMenuCUI = 0;
static void mex_second_menu_select_handle(void)
{	
	extern void Mex_Entry_App_About(void);
	extern void Mex_DirectEntry_App(void);
	assert(iMexSecondMenuItem < 3);
	switch(iMexSecondMenuItem)
	{
		case 0:
			{
                            Mex_DirectEntry_App();
                            cui_inline_close(iMexSecondMenuCUI);
                        }   
			break;
		case 1:
                        {
			    Mex_Entry_MainScreen();
                            cui_inline_close(iMexSecondMenuCUI);
                        }
			break;
		case 2:
			Mex_Entry_App_About();
			break;
		default:
			break;
	}
}
static MMI_RET mex_second_menu_list_proc(mmi_event_struct* evt)
{
	cui_event_inline_common_struct* inline_evt = (cui_event_inline_common_struct*) evt;
	cui_event_inline_notify_struct* notify_evt = (cui_event_inline_notify_struct*) evt;

	switch (inline_evt->evt_id)
	{
	case EVT_ID_CUI_INLINE_ABORT:
		cui_inline_close(inline_evt->sender_id);
		break;
	case EVT_ID_CUI_INLINE_SET_KEY:
		{
			cui_event_inline_set_key_struct *inline_evt = (cui_event_inline_set_key_struct *)evt;
			U32 item = inline_evt->item_id-CUI_INLINE_ITEM_ID_BASE;
			if(item < 3)
			{
				iMexSecondMenuItem = item;
				ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
				SetLeftSoftkeyFunction(mex_second_menu_select_handle, KEY_EVENT_UP);
			}
		}
		break;
	default:
		break;
	}
	return MMI_RET_OK;
}
void mex_second_menu_list_entry(void)
{
        MMI_ID mex_second_menu_list_gid;
        MMI_ID mex_second_menu_list_inline_id;

        mex_second_menu_list_gid = mmi_frm_group_create(GRP_ID_ROOT, 
                               GRP_ID_AUTO_GEN, 
                               mex_second_menu_list_proc, NULL);
        mmi_frm_group_enter(mex_second_menu_list_gid, MMI_FRM_NODE_SMART_CLOSE_FLAG);

        mex_second_menu_list_inline_id = cui_inline_create(mex_second_menu_list_gid, &g_mex_second_menu_list_scrn);
		cui_inline_set_title_string(mex_second_menu_list_inline_id,(UI_string_type)&g_DesctribeInfo);
        cui_inline_set_item_displayonly(mex_second_menu_list_inline_id,CUI_INLINE_ITEM_ID_BASE+0, (PU8)"\x2F\x54\xA8\x52\0", NULL);
        cui_inline_set_item_displayonly(mex_second_menu_list_inline_id,CUI_INLINE_ITEM_ID_BASE+1, (PU8)"\xF3\x81\x0A\x5C\x9D\x5B\x33\x00\x47\x00\0", NULL);
        cui_inline_set_item_displayonly(mex_second_menu_list_inline_id,CUI_INLINE_ITEM_ID_BASE+2, (PU8)"\x73\x51\x8E\x4E\0", NULL);
        cui_inline_run(mex_second_menu_list_inline_id);
		iMexSecondMenuCUI = mex_second_menu_list_inline_id;

}

#endif

void Mex_Make_Second_Menu_Record(void)
{
#ifndef MEX_QA_MODE
	//30天每天一次
	Mex_Entry_Second_Make_Record( 30, true );
#else
	Mex_Entry_Second_Make_Record( g_mex_entry_second_limit, true );
#endif
}

void Mex_Delete_App_About()
{
	if (mex_temp_path != NULL)
	{
		MexInterface_Med_Free(mex_temp_path);
		mex_temp_path = NULL;
	}
}

//进入菜单里面关于
void Mex_Entry_App_About(void)
{
	U8 *guiBuffer_p;
	int16 iAppVersion=0;
	wchar_t * szAboutStr = NULL;
	uint32 length = 0;
	wchar_t *tmp= NULL;
        wchar_t  app_version[10] = {0};
	extern char* MexSystem_GetSystemVersion(void);
	extern char* MexSystem_GetPatchVersion(void);
	extern bool mex_GetAppInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion);
	extern int32 MexConfig_GetAppMemSize(void);

	mex_GetAppInfoFromFile( g_AppPath, mex_tstr2wcs(g_AppName), false, NULL, &iAppVersion );


	if (mex_temp_path == NULL)
		mex_temp_path = (U8 *)med_alloc_ext_mem(2*200);
	if (mex_temp_path == NULL)
		return;
	mex_wsnprintf( (wchar_t*)mex_temp_path, 200, "MEX V:%s_%s\n",
		MexSystem_GetSystemVersion(),mex_strcmp(MexSystem_GetPatchVersion(),"")==0?"0":MexSystem_GetPatchVersion());

	#if 0 //YSS add,去掉关于菜单中的版本显示
	mex_wcscat((wchar_t*)mex_temp_path,g_DesctribeInfo);
	mex_wcscat((wchar_t*)mex_temp_path,(wchar_t*)"\x48\x72\x2c\x67\x1a\xff");
	#endif
	
	/*mex_wsnprintf(app_version,10,"%d\n",iAppVersion);
	mex_wcscat((wchar_t*)mex_temp_path,app_version);*/
	length = (uint32)mex_wcslen((wchar_t*)mex_temp_path);
	tmp = (wchar_t*)(mex_temp_path+length*2);
    
        mex_wsnprintf(tmp, 200-length*2, "Build: %s\n(C) 2010 MeTek \n",MexSystem_GetBulidDate() );
	mex_log(MEX_LOG_SYSTEM,"%s==%s",mex_twcs2str((wchar_t*)mex_temp_path),tmp);
	EntryNewScreen(E_MSI_SECOND_ABOUT, NULL, Mex_Entry_App_About, NULL);
	SetDelScrnIDCallbackHandler( E_MSI_SECOND_ABOUT, (HistoryDelCBPtr)Mex_Delete_App_About);
	guiBuffer_p = GetCurrGuiBuffer(E_MSI_SECOND_ABOUT);
	ShowCategory74Screen(0,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,(U8 *)mex_temp_path,mex_wcslen((U16 *)mex_temp_path),guiBuffer_p);
	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


//可以参考该函数为其他APP 添加独立入口
void Mex_DirectEntry_App(void);

void Mex_Entry_App_Start(void)
{
    wchar_t loadinfo[MAX_APP_DESCRIBEINFO_NUM + 9] = {0};
#if (defined(__MEX_G7_UI_BOOTUP__))&&(MEX_MTK_CODE_VERSION >= 0x1112)
		if(true == MexSystem_IsSysRunning())
		{
			Mex_ShutDownAllSystem();
		}
#endif	
	Mex_SetCurrentEntryAppInfo( g_AppName, g_iAppid);

	MexConfig_SetTinyLoadingPage( false,
								  MEX_COLOR_DARKBLUE,
								  MEX_COLOR_WHITE,
								  MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								  (const wchar_t *)"\x63\x6B\x28\x57\x1D\x52\xCB\x59\x16\x53\xFB\x7C\xDF\x7E\x2E\x0\x2E\x0\x2E\x0\0" ); 	//正在初始化系统...
    mex_wcscpy(loadinfo,(const wchar_t *)"\x63\x6B\x28\x57\xA0\x52\x7D\x8F\x0\x0"); //正在加载
    mex_wcscat(loadinfo,(const wchar_t *)g_DesctribeInfo);                          //"应用描述"
    mex_wcscat(loadinfo,(const wchar_t *)(const wchar_t *)"\x2E\x0\x2E\x0\x2E\x0\x0");  //...
    MexConfig_SetManLoadingPage( false,
								 MEX_COLOR_DARKBLUE,
								 MEX_COLOR_WHITE,
								 MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								 loadinfo );
    Mex_Entry_Screen_Internal( Mex_DirectEntry_App );

	//延时为平台取得imsi，争取时间
	(*gui_start_timer)( 5000, Mex_Make_Second_Menu_Record );
}

void Mex_DirectEntry_App(void)
{
	MexSystem_SetSystemInitParam(g_InitParam);
	Mex_Entry_App_Start();
}
/*add by dzw 20111215 */
extern BOOL mex_is_second_menu_hiden(S8 * current_app_name);

void Mex_Second_Menu_Entry_App(const wchar_t *pathname,const char *mexfilename,const char *initparam,const wchar_t* describeinfo,int32 iAppid)

{
#if 0//(MEX_MTK_CODE_VERSION >= 0x1112)
	extern bool mex_second_menu_isshown(char* name);
#else
char curr_app_name[MAX_APP_NAME_NUM+1] = {0};
BOOL is_second_menu_hiden = TRUE;
#endif

    assert(pathname != NULL && mexfilename != NULL && describeinfo != NULL && initparam != NULL);
    assert(mex_strlen(pathname) < MAX_APP_PATH_NUM && mex_strlen(pathname) < MAX_APP_NAME_NUM && mex_wcslen(describeinfo) < MAX_APP_DESCRIBEINFO_NUM && mex_wcslen(g_InitParam) < MAX_APP_INITPARAM_NUM);
    g_iAppid = iAppid;
    mex_wcscpy(g_AppPath,pathname);
    mex_strcpy(g_AppName,mexfilename);
    mex_wcscpy(g_DesctribeInfo,describeinfo);
    mex_wcscpy(g_InitParam,(const wchar_t*)initparam);
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	if(!mex_is_second_menu_hiden(g_AppName))
	{
		mex_second_menu_list_entry();
	}
#else
	mex_strcpy(curr_app_name,mexfilename);
	is_second_menu_hiden = mex_is_second_menu_hiden(curr_app_name);
	
	if( Mex_Entry_Second_CheckCfgFile() || !is_second_menu_hiden )
	{
		Mex_Entry_Second_RegisterMenuFunc( Mex_DirectEntry_App, Mex_Entry_App_About );
		Mex_Entry_Second_MenuList();
	}
#endif 
	else
	{
#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
		Mex_Entry_Menu_RegisterMenuFunc( Mex_DirectEntry_App, Mex_Entry_ApnSelectScreen );
		Mex_Entry_Menu_MenuList();
#else
		Mex_DirectEntry_App();
#endif
	}
}

#endif

/*************************  QQ独立入口  ********************************/
#ifdef __MEX_QQ_SUPPORT__
// QQ 独立入口，可变:)
void Mex_Entry_QQ(void)
{

#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__
    Mex_Second_Menu_Entry_App(L"Mex_QQ","Mex_QQ.mex","Mex_QQ.mex:1",(const wchar_t *)"\x51\x0\x51\x0",E_MSVT_QQ);
#else/*__MEX_QQ_DOUBLEMENU_SUPPORT__*/
    Mex_Second_Menu_Entry_App(L"Mex_QQ","Mex_QQ.mex","Mex_QQ.mex:",(const wchar_t *)"\x51\x0\x51\x0",E_MSVT_QQ);
#endif/*__MEX_QQ_DOUBLEMENU_SUPPORT__*/
}

// qq独立入口 HighLight
void Mex_HighLight_QQ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_QQ, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__

	// QQ2  入口
	void Mex_Entry_QQ2(void)
{
    Mex_Second_Menu_Entry_App(L"Mex_QQ","Mex_QQ.mex","Mex_QQ.mex:2",(const wchar_t *)"\x51\x0\x51\x0",E_MSVT_QQ);
}
// QQ2  HighLight
	void Mex_HighLight_QQ2_Menu(void)
	{
		ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

		/* Register function for left soft key */
		SetLeftSoftkeyFunction(Mex_Entry_QQ2, KEY_EVENT_UP);

		/* Register function for right soft key */
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	#endif/*__MEX_QQ_DOUBLEMENU_SUPPORT__*/
#endif
/*****************************************************************************/
/***************************天气预报独立入口******************************/
#if defined(__MEX_TQYB_SUPPORT__)
void Mex_Entry_TQYB(void)
{
	MexSystem_SetSystemInitParam("Weather.mex:");
	Mex_SetCurrentEntryAppInfo( "Weather.mex" , E_MSVT_WEATHER);
	Mex_Entry_Screen_Internal( Mex_Entry_TQYB );
}
void Mex_HighLight_TQYB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_TQYB, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif

/*************************  新浪微博独立入口  ********************************/
#ifdef __MEX_SINA_SUPPORT__

// SINA 独立入口，可变:)
void Mex_Entry_SINA(void)
{
    Mex_Second_Menu_Entry_App(L"weibo","weibo.mex","weibo.mex:",(const wchar_t*)"\xB0\x65\x6A\x6D\xAE\x5F\x5A\x53",E_MSVT_WEIBO);
}


// SINA独立入口 HighLight
void Mex_HighLight_SINA_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_SINA, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

#endif

/*****************************************************************************/
/***************************征途独立入口******************************/
#if defined(__MEX_LSZT_SUPPORT__)||defined(__MEX_ZTWY_SUPPORT__)
void Mex_Entry_ZT(void)
{
	MexSystem_SetSystemInitParam("mobile_zt.mex:");
	Mex_SetCurrentEntryAppInfo( "mobile_zt.mex" , E_MSVT_ZT);
	Mex_Entry_Screen_Internal( Mex_Entry_ZT );

}
void Mex_HighLight_ZT_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_ZT, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/
/*************************  TB独立入口  ********************************/
#if defined(__MEX_WAMKF_SUPPORT__)||defined(__MEX_KGZW_SUPPORT__)
// QQ 独立入口，可变:)
void Mex_Entry_TB(void)
{
    Mex_Second_Menu_Entry_App(L"MEX_TB","MEX_TB.mex","MEX_TB.mex:",(const wchar_t*)"\xA6\x9E\x4B\x51\xAF\x75",E_MSVT_TB);
}


// TB独立入口 HighLight
void Mex_HighLight_TB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_TB, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/*****************************************************************************/

/*************************  SMS独立入口  ********************************/
#ifdef __MEX_SMS_SUPPORT__
void Mex_Entry_SMS_Make_Record(void)
{
#ifndef MEX_QA_MODE
	//30天每天一次
	Mex_Entry_Second_Make_Record( 30, true );
#else
	Mex_Entry_Second_Make_Record( g_mex_entry_second_limit, true );
#endif
}

void Mex_Delete_SMS_About(void*pParam)
{
	if (mex_temp_path != NULL)
	{
		MexInterface_Med_Free(mex_temp_path);
		mex_temp_path = NULL;
	}
}

//进入菜单里面的SMS关于
void Mex_Entry_SMS_About(void)
{
	U8 *guiBuffer_p;
	int16 iSMSVersion=0;
	wchar_t * szAboutStr = NULL;

	extern char* MexSystem_GetSystemVersion(void);
	extern char* MexSystem_GetPatchVersion(void);
	extern bool mex_GetAppInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion);
	extern int32 MexConfig_GetAppMemSize(void);

	mex_GetAppInfoFromFile( L"imsms", L"imsms.mex", false, NULL, &iSMSVersion );

	if (mex_temp_path == NULL)
		mex_temp_path = (U8 *)med_alloc_ext_mem(2*1024);
	if (mex_temp_path == NULL)
		return;

	mex_wsnprintf( (wchar_t*)mex_temp_path, 1024, "MEX V:%s_%s\nSMS版本:%d\nBuild: %s\n(C) 2010 MeTek \n",
		MexSystem_GetSystemVersion(),MexSystem_GetPatchVersion(),iSMSVersion,MexSystem_GetBulidDate() );

	EntryNewScreen(E_MSI_SECOND_ABOUT, NULL, Mex_Entry_SMS_About, NULL);
	SetDelScrnIDCallbackHandler( E_MSI_SECOND_ABOUT, (HistoryDelCBPtr)Mex_Delete_SMS_About);
	guiBuffer_p = GetCurrGuiBuffer(E_MSI_SECOND_ABOUT);
	ShowCategory74Screen(0,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,(U8 *)mex_temp_path,mex_wcslen((U16 *)mex_temp_path),guiBuffer_p);
	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


//实际进入SMS界面的入口
//可以参考该函数为其他APP 添加独立入口
void Mex_Entry_SMS_DirectEntry(void)
{
	MexSystem_SetSystemInitParam("imsms.mex:");
	Mex_SetCurrentEntryAppInfo( "imsms.mex" , E_MSVT_SMS);

	MexConfig_SetTinyLoadingPage( false,
								  MEX_COLOR_MEDIUMPURPLE,
								  MEX_COLOR_WHITE,
								  MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								  (const wchar_t *)"\x63\x6B\x28\x57\x1D\x52\xCB\x59\x16\x53\xFB\x7C\xDF\x7E\x2E\x0\x2E\x0\x2E\x0\0" );		//正在初始化系统...
	MexConfig_SetManLoadingPage( false,
								 MEX_COLOR_MEDIUMPURPLE,
								 MEX_COLOR_WHITE,
								 MEX_FONTPOS_VMIDDLE|MEX_FONTPOS_HCENTER,
								 (const wchar_t *)"\x63\x6B\x28\x57\xA0\x52\x7D\x8F\xED\x77\xE1\x4F\xB1\x7B\x2E\x0\x2E\x0\x2E\x0\0" );		//正在加载短信箱...
	Mex_Entry_Screen_Internal( Mex_Entry_SMS_DirectEntry );

	//延时为平台取得imsi，争取时间
	(*gui_start_timer)( 5000, Mex_Entry_SMS_Make_Record );
}

// SMS 独立入口，可变:)
void Mex_Entry_SMS(void)
{
	if( Mex_Entry_Second_CheckCfgFile() )
	{
		Mex_Entry_Second_RegisterMenuFunc( Mex_Entry_SMS_DirectEntry, Mex_Entry_SMS_About );
		Mex_Entry_Second_MenuList();
	}
	else
	{
#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
		Mex_Entry_Menu_RegisterMenuFunc( Mex_Entry_SMS_DirectEntry, Mex_Entry_ApnSelectScreen );
		Mex_Entry_Menu_MenuList();
#else
		Mex_Entry_SMS_DirectEntry();
#endif
	}
}

// SMS独立入口 HighLight
void Mex_HighLight_SMS_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_SMS, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/*****************************************************************************/


/*************************  MSN独立入口  ********************************/
#ifdef __MEX_MSN_SUPPORT__
// msn 独立入口?
void Mex_Entry_MSN(void)
{
    Mex_Second_Menu_Entry_App(L"MSN","MSN.mex","MSN.mex:",(const wchar_t*)"\x4D\x00\x53\x00\x4E\x00",E_MSVT_MSN);
}

// TB独立入口 HighLight
void Mex_HighLight_MSN_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_MSN, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/*****************************************************************************/


bool Mex_Main_Is_File_Exist( const wchar_t * szFileName, const wchar_t * szAppName, bool bInSystemDir )
{
	int32 *file;
	extern const wchar_t* MexSystem_GetSystemRootDir(void);
	extern const wchar_t* MexSystem_GetAppRootDir(void);

	if (bInSystemDir)
	{
		file = (int32*)mex_fopenw_ext( szFileName, L"rb", MexSystem_GetSystemRootDir(), NULL, false );
	}
	else
	{
		wchar_t pFullPath[MEX_FILE_NAME_MAX_LENGTH];
		mex_wcscpy( pFullPath, MexSystem_GetAppRootDir() );
		mex_wcscat( pFullPath, L"\\" );
		mex_wcscat( pFullPath, szAppName );
		file = (int32*)mex_fopenw_ext( szFileName, L"rb", pFullPath, NULL, false );
	}

	if( file != NULL )
	{
		mex_fclose(file);
		return true;
	}

	return false;
}

bool Mex_Imsms_Is_File_All_Exist(void)
{
	if( !Mex_Main_Is_File_Exist( L"imsms.mex", L"imsms", false ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_KERUIM.mel", NULL, true ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_KERCOM.mel", NULL, true ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_UIL1.mel", NULL, true ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_UIC1.mel", NULL, true ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_UIR1.mel", NULL, true ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_UIR2.mel", NULL, true ) )
		return false;
	if( !Mex_Main_Is_File_Exist( L"MexLib_UIT1.mel", NULL, true ) )
		return false;

	return true;
}

bool Mex_Imsms_Check_New_Msg_Open(void)
{
	int32 *file;
	int32 iSize = 0;
	extern const wchar_t* MexSystem_GetSystemRootDir(void);

	file = (int32*)mex_fopenw_ext( L"MexMessage.cfg", L"rb", MexSystem_GetSystemRootDir(), NULL, false );

	if( file != NULL )
	{
		iSize = mex_GetFileSize( file );
		mex_fclose(file);
	}

	return ( iSize > 0 );
}

//检查imsms是否可以运行，接管查看短信
bool Mex_Imsms_Can_Run(void)
{
	return ( Mex_Imsms_Check_New_Msg_Open() && Mex_Imsms_Is_File_All_Exist() );
}

//实际进入SMS 新短信 界面的入口
void Mex_Imsms_DirectEntry(void)
{
	MexSystem_SetSystemInitParam( "imsms.mex:newmsg" );
	Mex_SetCurrentEntryAppInfo( "imsms.mex" , E_MSVT_SMS);
	Mex_Entry_Screen_Internal( Mex_Imsms_DirectEntry );
}

// SMS读取短信入口，返回1 mex接管，返回0 系统处理
int32 Mex_Imsms_Read_New_SMS(void)
{
	//todo : 是否可以运行 的判断
	if( Mex_Imsms_Can_Run() )
	{
		Mex_Imsms_DirectEntry();
		return 1;
	}
	return 0;
}



extern int8 g_MexLogFilter;

void Mex_ChangeLogFilter(void)
{
	if(g_MexLogFilter == 0)
	{
		g_MexLogFilter = -1;
	}
	else
	{
		g_MexLogFilter = 0;
	}
	GoBackHistory();
}


void Mex_DeleteSSCHandleMexPlatForm(void *param)
{
	if (tempstr != NULL)
	{
		MexInterface_Med_Free(tempstr);
		tempstr = NULL;
	}
	if (mex_temp_path != NULL)
	{
		MexInterface_Med_Free(mex_temp_path);
		mex_temp_path = NULL;
	}
}

typedef struct
{
	S32 iLogOnOffHiliteState;
#ifdef MEX_QA_MODE
	U8 pVenderIDBuf[20];
	U8 pPhoneUA[20];
	//#if MEX_SCREEN_ESPECIAL_VERSION
	U8 pScreenWidth[20];
	U8 pScreenHeight[20];
	//#endif
	//#ifdef MEX_ST_SUPPORT
	U8 pSTTime[20];
	//#endif
	//#if defined(MEX_QQ_ESPECIAL_VERSION)
	U8 pEntrySecondLimit[20];
	//#endif
	U8 pMexAppName[50];
	U8 pMexAppID[20];
#endif
	S32 iHighLightItemIndex;
}Mex_EngineerMode_Struct;

static Mex_EngineerMode_Struct engmode = {0};
static bool g_engmode_changed = false;


S8 *MexOnOffText[2];

#if MEX_SCREEN_ESPECIAL_VERSION

void Mex_SaveScreenConfig(uint16 uWidth,uint16 uHeight)
{
	extern const wchar_t * mex_GetSystemRootDir(void);
	wchar_t szFullPath[100];
	int32* file = mex_fopenw_ext( L"sysScreen.cfg", L"wb", mex_GetSystemRootDir(), NULL, false );
	if( file != NULL )
	{
		mex_fwrite( &uWidth, 2, 1, file );
		mex_fwrite( &uHeight, 2, 1, file );
		mex_fclose(file);
	}

	mex_wcscpy( szFullPath, mex_GetSystemRootDir() );
	mex_wcscat( szFullPath, L"\\MexManager.mer" );
	mex_remove_ext( szFullPath );
}

bool Mex_CopyScreenManager(uint16 uWidth,uint16 uHeight)
{
	extern const wchar_t * mex_GetSystemRootDir(void);
	extern void* MexSystem_Alloc(int32 size);
	extern void MexSystem_Free(void *ptr);

	wchar_t porgFullFilePath[MEX_FILE_NAME_MAX_LENGTH];
	int32 *dstFile = NULL,*orgFile = NULL;
	void * pBuf = NULL;
	int32 iTotalSize = 0, iReadSize = 0 , iOnceSize = 1024*10;
	wchar_t * pLastIndex;
	int32 iLen,i;

	mex_wsnprintf( porgFullFilePath, MEX_FILE_NAME_MAX_LENGTH, "Manager\\%dX%d\\MexManager.mex",
					uWidth,
					uHeight);

	orgFile = mex_fopenw_ext( porgFullFilePath, L"rb", mex_GetSystemRootDir(), NULL, false );
	if( orgFile == NULL )
		goto PutError;
	dstFile = mex_fopenw_ext( L"MexManager.mex", L"wb", mex_GetSystemRootDir(), NULL, false );
	if( dstFile == NULL )
		goto PutError;

	pBuf = MexInterface_Med_Alloc(iOnceSize);
	while (pBuf == NULL)
	{
		pBuf = (uint8*)MexInterface_Med_Alloc( iOnceSize );
		if (pBuf == NULL)
			iOnceSize = iOnceSize * 3 / 4;
	}

	iTotalSize = mex_GetFileSize( orgFile );

	while( iReadSize < iTotalSize )
	{
		int32 iSize = iTotalSize - iReadSize;
		if( iSize > iOnceSize)
		{
			mex_fread( pBuf, iOnceSize, 1, orgFile);
			mex_Sleep( 3 );
			mex_fwrite( pBuf, iOnceSize, 1, dstFile);
			iReadSize += iOnceSize;
		}
		else
		{
			mex_fread( pBuf, iSize, 1, orgFile);
			mex_fwrite( pBuf, iSize, 1, dstFile);
			iReadSize += iSize;
		}
	}


	mex_fclose( dstFile );
	mex_fclose( orgFile );
	mex_wsnprintf( porgFullFilePath, MEX_FILE_NAME_MAX_LENGTH, "Manager\\%dX%d\\MexManager.dll",
					uWidth,
					uHeight);

	orgFile = mex_fopenw_ext( porgFullFilePath, L"rb", mex_GetSystemRootDir(), NULL, false );
	if( orgFile == NULL )
		goto PutError;
	dstFile = mex_fopenw_ext( L"MexManager.dll", L"wb", mex_GetSystemRootDir(), NULL, false );
	if( dstFile == NULL )
		goto PutError;

	iReadSize = 0;
	iTotalSize = mex_GetFileSize( orgFile );

	while( iReadSize < iTotalSize )
	{
		int32 iSize = iTotalSize - iReadSize;
		if( iSize > iOnceSize)
		{
			mex_fread( pBuf, iOnceSize, 1, orgFile);
			mex_Sleep( 3 );
			mex_fwrite( pBuf, iOnceSize, 1, dstFile);
			iReadSize += iOnceSize;
		}
		else
		{
			mex_fread( pBuf, iSize, 1, orgFile);
			mex_fwrite( pBuf, iSize, 1, dstFile);
			iReadSize += iSize;
		}
	}

	MexInterface_Med_Free(pBuf);
	mex_fclose( dstFile );
	mex_fclose( orgFile );
	return true;
PutError:
	if( dstFile != NULL )
		mex_fclose( dstFile );
	if( orgFile != NULL )
		mex_fclose( orgFile );
	if( pBuf != NULL )
		MexInterface_Med_Free(pBuf );
	return false;
}


#endif

void Mex_SaveEngineerMode(void)
{
#ifdef MEX_QA_MODE
	extern int32 iMex_QA_VenderID;
	extern char pMex_QA_PhoneUA[10];
	extern void Mex_Test_UpdateCurrAppInfo(U8* pAppName, U8* pAppId);


	#ifdef MEX_ST_SUPPORT
	//extern int32
	int32 iMex_St_time ;

	extern void UpdateStTime(int);

	#endif
#endif
	if(engmode.iLogOnOffHiliteState)
	{
		g_MexLogFilter = 0;
	}
	else
	{
		g_MexLogFilter = -1;
	}

#ifdef MEX_QA_MODE
	mex_wcs2nstr(pMex_QA_PhoneUA,10,(const wchar_t*)engmode.pPhoneUA);
	iMex_QA_VenderID = mex_atoi( mex_twcs2str((wchar_t *)engmode.pVenderIDBuf) );
	#if MEX_SCREEN_ESPECIAL_VERSION
	{
		extern uint16 g_now_screen_width;
		extern uint16 g_now_screen_height;
		int32 iScreenWidth,iScreenHeight;
		iScreenWidth = mex_atoi( mex_twcs2str((wchar_t *)engmode.pScreenWidth) );
		iScreenHeight = mex_atoi( mex_twcs2str((wchar_t *)engmode.pScreenHeight) );
		if( iScreenWidth != g_now_screen_width || iScreenHeight != g_now_screen_height )
		{
			g_now_screen_width = iScreenWidth;
			g_now_screen_height = iScreenHeight;
			Mex_SaveScreenConfig( g_now_screen_width, g_now_screen_height );
			Mex_CopyScreenManager( g_now_screen_width, g_now_screen_height );
		}
	}
	#endif
	#ifdef MEX_ST_SUPPORT
	iMex_St_time = mex_atoi( mex_twcs2str((wchar_t *)engmode.pSTTime) );
	UpdateStTime(iMex_St_time);
	#endif

	#if defined(MEX_QQ_ESPECIAL_VERSION)
	g_mex_entry_second_limit = mex_atoi( mex_twcs2str((wchar_t *)engmode.pEntrySecondLimit) );
	Mex_Entry_Second_DeleteCfgFile();
	#endif

	Mex_Test_UpdateCurrAppInfo(engmode.pMexAppName,engmode.pMexAppID);
#endif
	GoBackHistory();
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_OK,0,1000,SUCCESS_TONE);


}

void MexEngineerModeHightHdlr(S32 index)
{
	engmode.iHighLightItemIndex = index;


#if defined(MEX_QA_MODE)
	if(engmode.iHighLightItemIndex == 18)
	{
		//Mex_Test_EntryApp();
		extern void Mex_Test_EntryApp(void);
		extern void Mex_Test_UpdateCurrAppInfo(U8* pAppName, U8* pAppId);

		Mex_Test_UpdateCurrAppInfo(engmode.pMexAppName,engmode.pMexAppID);
		change_left_softkey( STR_GLOBAL_GO_TO, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(Mex_Test_EntryApp, KEY_EVENT_UP);
		redraw_left_softkey();
	}
#endif

/*	extern int32 iMex_QA_VenderID;
	extern char pMex_QA_PhoneUA[10];
	extern int32 g_Mex_St_time;

	if( index == 3 )
	{
		g_engmode_changed |= mex_atoi(mex_twcs2str((const wchar_t *)engmode.pVenderIDBuf)) != iMex_QA_VenderID;
	}
	else if( index == 5 )
	{
		g_engmode_changed |= mex_strcmp( pMex_QA_PhoneUA, mex_twcs2str(engmode.pPhoneUA)) != 0;
	}
	else if( index == 7 )
	{
		g_engmode_changed |= mex_atoi(mex_twcs2str((const wchar_t *)engmode.pSTTime)) != g_Mex_St_time;
	}
	else if( index == 9 )
	{
		g_engmode_changed |= mex_atoi(mex_twcs2str((const wchar_t *)engmode.pEntrySecondLimit)) != g_mex_entry_second_limit;
	}


	if( g_engmode_changed )
	{
		change_left_softkey( STR_GLOBAL_DONE, IMG_GLOBAL_OK);
		change_right_softkey(0, 0);
		SetLeftSoftkeyFunction(Mex_SaveEngineerMode, KEY_EVENT_UP);
		SetRightSoftkeyFunction(NULL, KEY_EVENT_UP);
	}
	else
	{
		change_left_softkey( 0, 0);
		change_right_softkey(0, 0);
		SetLeftSoftkeyFunction(NULL, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction(NULL, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
		SetRightSoftkeyFunction(NULL, KEY_EVENT_UP);
	}

	redraw_left_softkey();
	redraw_right_softkey();
	*/
}

void Mex_PlatForm_ExitEngineerMode(void)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history Scr;
    S16 nHistory = 0;
    U16 inputBufferSize;    /* added for inline edit history */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Scr.scrnID = E_MSI_ENGINEERMODE;
    CloseCategory57Screen();
#if (MEX_MTK_CODE_VERSION < 0x1112)
    Scr.entryFuncPtr = Mex_PlatForm_EntryEngineerMode;
#else
    Scr.entryFuncPtr = mex_engine;
#endif
    mmi_ucs2cpy((S8*) Scr.inputBuffer, (S8*) & nHistory);
    GetCategory57History(Scr.guiBuffer);
    inputBufferSize = (U16) GetCategory57DataSize();    /* added for inline edit history */
    GetCategory57Data((U8*) Scr.inputBuffer);           /* added for inline edit history */
    AddNHistory(Scr, inputBufferSize);                  /* added for inline edit history */
}

#if (MEX_MTK_CODE_VERSION < 0x1112)

//mex平台工程模式
void Mex_PlatForm_EntryEngineerMode(void)
{
	U8 *guiBuffer;
	U8 *inputBuffer;
	U16 inputBufferSize;
	U8	nMenuItems;

	S8 inline_item_index = 0;

//去掉某些宏控制
//防止高亮项索引受宏影响
	U16 icon_list[] = {
		IMG_GLOBAL_L1,0
#ifdef MEX_QA_MODE
		,IMG_GLOBAL_L2,0
		,IMG_GLOBAL_L3,0
		//#if MEX_SCREEN_ESPECIAL_VERSION
		,IMG_GLOBAL_L4,0
		,IMG_GLOBAL_L5,0
		//#endif
	//	#ifdef MEX_ST_SUPPORT
		,IMG_GLOBAL_L6,0
	//	#endif
	//	#if defined(MEX_QQ_ESPECIAL_VERSION)
		,IMG_GLOBAL_L7,0
	//	#endif
		,IMG_GLOBAL_L8,0
		,IMG_GLOBAL_L9,0
		,IMG_GLOBAL_L10
#endif
		};

	MexOnOffText[0] = GetString(STR_GLOBAL_OPEN); //打开
	MexOnOffText[1] = GetString(STR_GLOBAL_CLOSE);//关闭

#ifdef MEX_QA_MODE
	mex_wsnprintf((wchar_t *)engmode.pVenderIDBuf,5,"%d",MexConfig_GetPhoneConfig(E_MPC_MexVenderID));
	mex_wsnprintf((wchar_t *)engmode.pPhoneUA,5,"%s",(char *)MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA));

	#if MEX_SCREEN_ESPECIAL_VERSION
	{
		extern uint16 g_now_screen_width;
		extern uint16 g_now_screen_height;
		mex_wsnprintf((wchar_t *)engmode.pScreenWidth,5,"%d",g_now_screen_width);
		mex_wsnprintf((wchar_t *)engmode.pScreenHeight,5,"%d",g_now_screen_height);
	}
	#endif
	#ifdef MEX_ST_SUPPORT
	{
		extern int32 g_Mex_St_time;
		mex_wsnprintf((wchar_t *)engmode.pSTTime,5,"%d",g_Mex_St_time);
	}
	#endif
	#if defined(MEX_QQ_ESPECIAL_VERSION)
		mex_wsnprintf((wchar_t *)engmode.pEntrySecondLimit,5,"%d",g_mex_entry_second_limit);
	#endif
#endif

	engmode.iLogOnOffHiliteState = (g_MexLogFilter == 0);
	g_engmode_changed = false;

	nMenuItems = sizeof(icon_list) / sizeof(U16);

	EntryNewScreen(E_MSI_ENGINEERMODE,Mex_PlatForm_ExitEngineerMode,NULL,NULL);

	guiBuffer = GetCurrGuiBuffer(E_MSI_ENGINEERMODE);
	inputBuffer = GetCurrNInputBuffer(E_MSI_ENGINEERMODE, &inputBufferSize);

    InitializeCategory57Screen();

    RegisterHighlightHandler(MexEngineerModeHightHdlr);

	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\xE5\x65\xD7\x5F\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(&(wgui_inline_items[inline_item_index]), 2, (U8 **)MexOnOffText, &(engmode.iLogOnOffHiliteState));

#ifdef MEX_QA_MODE

	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\x20\x6E\x53\x90\xF7\x53\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pVenderIDBuf),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\x3A\x67\x8B\x57\xF7\x53\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pPhoneUA),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));
//#if MEX_SCREEN_ESPECIAL_VERSION

	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\x4F\x5C\x55\x5E\xBD\x5B\xA6\x5E\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pScreenWidth),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\x4F\x5C\x55\x5E\xD8\x9A\xA6\x5E\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pScreenHeight),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

//#endif

//#ifdef MEX_ST_SUPPORT
	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\xFA\x51\x27\x8D\xDF\x7E\xA1\x8B\xF6\x65\xF4\x95\x28\x0\x6\x52\x9F\x94\x29\x0\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pSTTime),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));
//#endif

//#if defined(MEX_QQ_ESPECIAL_VERSION)
	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\x8C\x4E\xA7\x7E\xDC\x83\x55\x53\x50\x96\x36\x52\x29\x59\x70\x65\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pEntrySecondLimit),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));
//#endif


	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\xEC\x72\xCB\x7A\x65\x51\xE3\x53\xB\x7A\x8F\x5E\xD\x54\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pMexAppName),50,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_ALPHANUMERIC_LOWERCASE));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

	inline_item_index++;

	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]), (U8*)"\xEC\x72\xCB\x7A\x65\x51\xE3\x53\xB\x7A\x8F\x5E\x16\x7F\xF7\x53\0");

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&(wgui_inline_items[inline_item_index]),(U8*)(engmode.pMexAppID),20,MexConfig_ConvertMtkDef(E_M2MDC_INPUT_NUMERIC));
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

	inline_item_index++;
	SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]), (U8*)"\x4B\x6D\xD5\x8B\xEC\x72\xCB\x7A\x65\x51\xE3\x53\0");


#endif

    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, nMenuItems, inputBuffer);
    }

	ShowCategory57Screen(
        E_MSI_MENU_STRING,
        E_MII_MEX_PLATFORM_STATUS_ICON,
        STR_GLOBAL_DONE,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nMenuItems,
        icon_list,
        wgui_inline_items,
         engmode.iHighLightItemIndex,
        guiBuffer);


    SetCategory57RightSoftkeyFunctions(Mex_SaveEngineerMode, GoBackHistory);
}
#endif

void SSCHandleMexPlatFormExit(void)
{
	Mex_DeleteSSCHandleMexPlatForm((void *)NULL);
}
void SSCHandleMexPlatForm(void)
{
	U8 *guiBuffer_p;
	int16 iDownloaderVersion=0;
	int16 iTinyManagerVersion=0;
	int16 iFullManagerVersion=0;
	int16 iAppGetterVersion=0;

	extern char* MexSystem_GetSystemVersion(void);
	extern char* MexSystem_GetPatchVersion(void);
	extern bool mex_GetAppInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion);
	extern int32 MexConfig_GetAppMemSize(void);
	extern int32 MexSystem_GetSdkVersion(void);

#if (MEX_MTK_CODE_VERSION >= 0x1112)
	MexLib_Os_Init_File_Pool();
#endif
	mex_GetAppInfoFromFile( L"", L"MexDownloader.mex", true, NULL, &iDownloaderVersion );
	mex_GetAppInfoFromFile( L"", L"MexManagerTiny.mex", true, NULL, &iTinyManagerVersion );
	mex_GetAppInfoFromFile( L"", L"MexManager.mex", true, NULL, &iFullManagerVersion );
	mex_GetAppInfoFromFile( L"MexAppGetter", L"MexAppGetter.mex", false, NULL, &iAppGetterVersion );
    if (tempstr == NULL)
    	tempstr = (char *)MexInterface_Med_Alloc(2*1024);
	if (tempstr == NULL)
		goto MEMERR;
	memset(tempstr,0,2*1024);

    if (mex_temp_path == NULL)
    	mex_temp_path = (U8 *)MexInterface_Med_Alloc(2*1024);
	if (mex_temp_path == NULL)
		goto MEMERR;
	memset(mex_temp_path,0,2*1024);

	sprintf(tempstr,"MEX V:%s_%s\nSDK V: %d\n渠道号:%d\n机型:%s\n机器编号:%d\nBuild: %s\n(C) 2010 MeTek \nLogEnable %d\n\nDownloader %d\nTiny %d\nManager %d\nAppGetter %d\nAppMEM %d\nIntMem %d\nExtMem %d\nReservedMem %d",
		MexSystem_GetSystemVersion(),MexSystem_GetPatchVersion(),MexSystem_GetSdkVersion(),MexConfig_GetPhoneConfig(E_MPC_MexVenderID) ,MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA),MexConfig_GetPhoneConfig(E_MPC_MexMachineID),MexSystem_GetBulidDate(),(g_MexLogFilter != 0),
		iDownloaderVersion, iTinyManagerVersion, iFullManagerVersion, iAppGetterVersion, MexConfig_GetAppMemSize(), MexConfig_GetPhoneConfig(E_MPC_MaxIntMem), MexConfig_GetPhoneConfig(E_MPC_MaxExtMem), MexConfig_GetPhoneConfig(E_MPC_MaxReservedMem) );

	mex_str2nwcs((wchar_t*)mex_temp_path,1024,(char *)tempstr);
	MexInterface_Med_Free(tempstr);
	tempstr = NULL;

	EntryNewScreen(E_MSI_SSCHANDLE, SSCHandleMexPlatFormExit, SSCHandleMexPlatForm, NULL);

	SetDelScrnIDCallbackHandler( E_MSI_MAIN, (HistoryDelCBPtr)Mex_DeleteSSCHandleMexPlatForm);

	guiBuffer_p = GetCurrGuiBuffer(E_MSI_SSCHANDLE);
	ShowCategory74Screen(0,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,(U8 *)mex_temp_path,mex_wcslen((U16 *)mex_temp_path),guiBuffer_p);

#if (MEX_MTK_CODE_VERSION >= 0x1112)
    {
	    SetLeftSoftkeyFunction(mex_engine, KEY_EVENT_UP);
    }
#else   
	SetLeftSoftkeyFunction(Mex_PlatForm_EntryEngineerMode, KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	return;

MEMERR:

	if (tempstr != NULL)
	{
		MexInterface_Med_Free(tempstr);
		tempstr = NULL;
	}
	if (mex_temp_path != NULL)
	{
		MexInterface_Med_Free(mex_temp_path);
		mex_temp_path = NULL;
	}
}
int32 iHighLightItemIndex = 0;

void HighlightItemHandler(S32 index)
{
	extern void Mex_RestoreSystem(void);
	iHighLightItemIndex = index;
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(Mex_RestoreSystem);
}
extern void Mex_RestoreSystem(void);
//系统恢复重建入口
#if (MEX_MTK_CODE_VERSION >= 0x1112)
static cui_inline_item_display_only_struct g_mex_engine_restore_button[] =
{
    STR_GLOBAL_CANCEL,
    STR_GLOBAL_CANCEL,
};
static const cui_inline_set_item_struct g_mex_engine_restore_inline[] = 
{
    {CUI_INLINE_ITEM_ID_BASE+0,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_engine_restore_button},
    {CUI_INLINE_ITEM_ID_BASE+1,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_engine_restore_button},
    {CUI_INLINE_ITEM_ID_BASE+2,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_engine_restore_button},
    {CUI_INLINE_ITEM_ID_BASE+3,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_engine_restore_button},
    {CUI_INLINE_ITEM_ID_BASE+4,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,0,(void*)&g_mex_engine_restore_button},
    
};
static const cui_inline_item_softkey_struct g_mex_engine_restore_inline_sofkey =
{
    {{CUI_INLINE_SOFTKEY_DEFAULT_VALUE, 0},
     {CUI_INLINE_SOFTKEY_DEFAULT_VALUE, 0},
     {STR_GLOBAL_CANCEL, 0}}        
};

static const cui_inline_struct g_mex_engine_restore_inline_scrn = 
{
    (sizeof(g_mex_engine_restore_inline)/sizeof(cui_inline_set_item_struct)),
    NULL,
    0,
    CUI_INLINE_SCREEN_LOOP|CUI_INLINE_SCREEN_DISABLE_DONE,
    (cui_inline_item_softkey_struct*)&g_mex_engine_restore_inline_sofkey,
    (const cui_inline_set_item_struct*)&g_mex_engine_restore_inline,
};
static MMI_RET mex_engine_restore_proc(mmi_event_struct* evt)
{
    cui_event_inline_common_struct* inline_evt = (cui_event_inline_common_struct*) evt;
    cui_event_inline_notify_struct* notify_evt = (cui_event_inline_notify_struct*) evt;

    switch (inline_evt->evt_id)
    {
        case EVT_ID_CUI_INLINE_ABORT:
            cui_inline_close(inline_evt->sender_id);
            break;
        case EVT_ID_CUI_INLINE_SET_KEY:
            {
                cui_event_inline_set_key_struct *inline_evt = (cui_event_inline_set_key_struct *)evt;
                U32 item = inline_evt->item_id-CUI_INLINE_ITEM_ID_BASE;
                if(item < 5)
                {
                    iHighLightItemIndex = item;
                    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
                    SetLeftSoftkeyFunction(Mex_RestoreSystem, KEY_EVENT_UP);
                }
            }
            break;
        default:
            break;
    }
    return MMI_RET_OK;
}
#endif
void SSCHandleMexPlatFormRestore(void)
{
	//U8 itemListPtrs[4][100];
	U8 * guiBuffer;
	S16 item_offset = 0;
	U16 list_of_icons[8] = { IMG_GLOBAL_L1,IMG_GLOBAL_L2,IMG_GLOBAL_L3,IMG_GLOBAL_L4,IMG_GLOBAL_L5};
	U8 *inputBuffer;
	U16 inputBufferSize;
/*
	mex_str2nwcs((wchar_t*)itemListPtrs[0],50,mex_twcs2str("简单修复"));
	mex_str2nwcs((wchar_t*)itemListPtrs[1],50,mex_twcs2str("重建平台管理程序"));
	mex_str2nwcs((wchar_t*)itemListPtrs[2],50,mex_twcs2str("重建系统库文件"));
	mex_str2nwcs((wchar_t*)itemListPtrs[3],50,mex_twcs2str("重建平台"));
  */
#if (MEX_MTK_CODE_VERSION < 0x1112)

	EntryNewScreen(E_MSI_SSCHANDLE_RESTORE,NULL,SSCHandleMexPlatFormRestore,NULL);

	guiBuffer = GetCurrGuiBuffer(E_MSI_SSCHANDLE_RESTORE);
	inputBuffer = GetCurrNInputBuffer(E_MSI_SSCHANDLE_RESTORE, &inputBufferSize);
	InitializeCategory57Screen();

	RegisterHighlightHandler(HighlightItemHandler);

	SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\x80\x7B\x55\x53\xEE\x4F\xD\x59\0"));
	SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\xCD\x91\xFA\x5E\xE8\x6C\x8C\x51\x68\x88\0"));
	SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\xCD\x91\xFA\x5E\x73\x5E\xF0\x53\xA1\x7B\x6\x74\xB\x7A\x8F\x5E\0"));
	SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\xCD\x91\xFA\x5E\xFB\x7C\xDF\x7E\x93\x5E\x87\x65\xF6\x4E\0"));
	SetInlineItemDisplayOnly(&wgui_inline_items[item_offset++],(U8 *)("\xCD\x91\xFA\x5E\x73\x5E\xF0\x53\0"));
	 ShowCategory57Screen(
		0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, item_offset,list_of_icons,
		wgui_inline_items,
		iHighLightItemIndex,//animList.themeId,
		guiBuffer
		);

	 ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	 SetCategory57LeftSoftkeyFunction(Mex_RestoreSystem);
	 DisableCategory57ScreenDone();
	 SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
#else
    {
        MMI_ID mex_eng_restore_gid;
        MMI_ID mex_eng_restore_inline_id;

        mex_eng_restore_gid = mmi_frm_group_create(GRP_ID_ROOT, 
                               GRP_ID_AUTO_GEN, 
                               mex_engine_restore_proc, NULL);
        mmi_frm_group_enter(mex_eng_restore_gid, MMI_FRM_NODE_SMART_CLOSE_FLAG);

        mex_eng_restore_inline_id = cui_inline_create(mex_eng_restore_gid, &g_mex_engine_restore_inline_scrn);
        cui_inline_set_item_displayonly(mex_eng_restore_inline_id,CUI_INLINE_ITEM_ID_BASE+0, (PU8)"\x80\x7B\x55\x53\xEE\x4F\xD\x59\0", NULL);
        cui_inline_set_item_displayonly(mex_eng_restore_inline_id,CUI_INLINE_ITEM_ID_BASE+1, (PU8)"\xCD\x91\xFA\x5E\xE8\x6C\x8C\x51\x68\x88\0", NULL);
        cui_inline_set_item_displayonly(mex_eng_restore_inline_id,CUI_INLINE_ITEM_ID_BASE+2, (PU8)"\xCD\x91\xFA\x5E\x73\x5E\xF0\x53\xA1\x7B\x6\x74\xB\x7A\x8F\x5E\0", NULL);
        cui_inline_set_item_displayonly(mex_eng_restore_inline_id,CUI_INLINE_ITEM_ID_BASE+3, (PU8)"\xCD\x91\xFA\x5E\xFB\x7C\xDF\x7E\x93\x5E\x87\x65\xF6\x4E\0", NULL);
        cui_inline_set_item_displayonly(mex_eng_restore_inline_id,CUI_INLINE_ITEM_ID_BASE+4, (PU8)"\xCD\x91\xFA\x5E\x73\x5E\xF0\x53\0", NULL);
        cui_inline_run(mex_eng_restore_inline_id);

    }

#endif
}

void Mex_DisplayPopup_Done(void)
{
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_OK,0,1000,SUCCESS_TONE);
}

//注册状态栏触屏事件
void Mex_RegisterStatusIconPenEventHdlr(void)
{
#ifdef __MMI_TOUCH_SCREEN__

	//wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_MEX_PLATFORM,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_MainScreen);
	#ifdef __MEX_QQ_SUPPORT__
	{
		#if (MEX_MTK_CODE_VERSION < 0x0952)
			wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_MEX_MASTER_QQ,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_QQ);
			#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__
			wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_MEX_SLAVE_QQ,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_QQ2);
			#else
			wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_MEX_SLAVE_QQ,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_QQ);
			#endif
		#else
            #if !defined(__MMI_VUI_HOMESCREEN__)
            	wgui_status_icon_bar_register_pen_event_handler(STATUS_ICON_MEX_MASTER_QQ,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_QQ);
				#ifdef __MEX_QQ_DOUBLEMENU_SUPPORT__
				wgui_status_icon_bar_register_pen_event_handler(STATUS_ICON_MEX_SLAVE_QQ,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_QQ2);
				#else
				wgui_status_icon_bar_register_pen_event_handler(STATUS_ICON_MEX_SLAVE_QQ,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_QQ);
				#endif
			#endif
		#endif
	}
	#endif


	//wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_MEX_PLATFORM,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_MainScreen);
	#ifdef __MEX_MSN_SUPPORT__
	{
		#if (MEX_MTK_CODE_VERSION < 0x0952)
		wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_MEX_MSN,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_MSN);
		#else
            #if !defined(__MMI_VUI_HOMESCREEN__)
            wgui_status_icon_bar_register_pen_event_handler(STATUS_ICON_MEX_MSN,WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,Mex_Entry_MSN);
			#endif
		#endif
	}
	#endif

#endif
}

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

//原执行函数void (*f) (void)
void* g_pOrgFunc = NULL;

void Mex_DisplayConfirm_SetOrgFunc(void * orgFunc)
{
	g_pOrgFunc = orgFunc;
}

void Mex_DisplayConfirm_Tip( void * confirmFunc, UI_string_type tipString )
{
	DisplayConfirm(
		STR_GLOBAL_YES,
		IMG_GLOBAL_YES,
		STR_GLOBAL_NO,
		IMG_GLOBAL_NO,
		tipString,
		NULL,
		WARNING_TONE);

	SetLeftSoftkeyFunction((void(*)(void))confirmFunc, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void Mex_QQExit_And_ModeSwitch(void)
{
	int32 iCurrentApp = MexSystem_GetCurrentApp();
	int32 iQQId = Mex_GetAppIdByFileName(L"Mex_QQ.mex");
	int32 iQQMessageId = Mex_GetAppIdByFileName(L"Mex_QQ_Message.mex");
	if( iQQId != NULL )
	{
		MexSystem_SetCurrentApp( iQQId );
		mex_Exit();
	}

	if( iQQMessageId != NULL )
	{
		MexSystem_SetCurrentApp( iQQMessageId );
		mex_Exit();
	}
	MexSystem_SetCurrentApp( iCurrentApp );

	if( g_pOrgFunc != NULL )
	{
		MexFuncPtr func = (MexFuncPtr)g_pOrgFunc;
		(*func)();
		g_pOrgFunc = NULL;
	}
}

void Mex_ModeSwitch_New_Hangler(void)
{
	Mex_DisplayConfirm_Tip( (void*)Mex_QQExit_And_ModeSwitch, (UI_string_type)GetString((U16)E_MSI_MODE_QUIT_QQ_TIP) );
}

MexFuncPtr curr_confirmFunc = NULL ;

static void Mex_ShutDownBackRunningApp(void)
{
	Mex_ShutDownAllSystem();
	if(curr_confirmFunc)
	{
		curr_confirmFunc();
	}
}
//和其他应用程序无法共存时，进入该应用程序需退出挂机程序，请调用该函数

//检测是否存在挂机，并弹出提示退出
//参数为点击确定后会进入的函数
//返回值表示是否弹出了提示框

bool Mex_CheckBackRunningAndDisplayConfirm(MexFuncPtr confirmFunc)
{

	// 检测QQ
	 if( Mex_IsMexQQMessageRunning()||Mex_IsMexQQRunning() )
	 {
	 	curr_confirmFunc = confirmFunc;

		Mex_DisplayConfirm_Tip((void *)Mex_ShutDownBackRunningApp,(UI_string_type)"\xA8\x60\x0\x97\x81\x89\x48\x51\x0\x90\xFA\x51\x51\x0\x51\x0\0");
		return true;
	 }
	 // 如果有其他的挂机程序 请在这里添加检测代码


	 else if( MexSystem_IsSysKernalRunning()&& (confirmFunc!=NULL))
	 {
		curr_confirmFunc = confirmFunc;
		Mex_DisplayConfirm_Tip((void *)Mex_ShutDownBackRunningApp,(UI_string_type)"\x85\x51\x58\x5B\x0D\x4E\xB3\x8D\x0C\xFF\x74\x65\x06\x74\x85\x51\x58\x5B\x1F\xFF\x00");
		return true;
	 }
	 else if( MexSystem_IsSysKernalRunning() )
	 {
	 	Mex_ShutDownAllSystem();
	 }
	 return false;
}


#ifdef MEX_SEL_DATAACCOUNT_SUPPORT

#define MMI_DATA_ACCOUNT_SIM1  1
#define MMI_DATA_ACCOUNT_SIM2  2
#ifdef __MET_MEX_SIM_PLUS__
#define MMI_DATA_ACCOUNT_SIM3  3
#define MMI_DATA_ACCOUNT_SIM4  4
#endif/*__MET_MEX_SIM_PLUS__*/
extern U8 g_sim_info;
int8 mode;
int8 sim = E_MEX_SIM_1;
int32 accountId;
int32 defualt_account = 0;

extern void MexSoc_SaveDataAccountConfig(int32 dataAccount, int8 apn, int8 sim, int8 bAlwaysAak);
extern void Mex_Entry_ApnSelectScreen(void);

//仅询问一次接入点
static void Mex_SelectDataAccountSuccess(void)
{
	//检测网络模块是否在运行
	extern bool MexSoc_IsModuleRun(void);
	if(MexSoc_IsModuleRun())
	{
		extern void MexSoc_CloceNetWork(int32 dataAccount, int8 sim);
		MexSoc_CloceNetWork(accountId , sim);
	}
	MexSoc_SaveDataAccountConfig(accountId,mode,sim,0);
	GoBackToHistory(E_MSI_APN_SELECT_SCREEN);
	GoBackHistory();
}
//每次都询问接入点
static void Mex_SelectDataAccountSuccessAlwaysAsk(void)
{
	//检测网络模块是否在运行
	extern bool MexSoc_IsModuleRun(void);
	if(MexSoc_IsModuleRun())
	{
		extern void MexSoc_CloceNetWork(int32 dataAccount, int8 sim);
		MexSoc_CloceNetWork(accountId , sim);
	}
	MexSoc_SaveDataAccountConfig(accountId,mode,sim,1);
	GoBackToHistory(E_MSI_APN_SELECT_SCREEN);
	GoBackHistory();
}
//重新进行设置
static void Mex_SelectDataAccountFail(void)
{
	//DeleteScreenIfPresent(SCR_CONFIRM_SCREEN);
	GoBackToHistory(E_MSI_APN_SELECT_SCREEN);
	GoBackHistory();
	//Mex_Select_Date_Account();
	Mex_Entry_ApnSelectScreen();
}


static void Mex_ShowConfirmDialog( void *yesconfirmFunc, void *noconfirmFunc, UI_string_type tipString )
{
	DisplayConfirm(
		STR_GLOBAL_YES,
		IMG_GLOBAL_YES,
		STR_GLOBAL_NO,
		IMG_GLOBAL_NO,
		tipString,
		NULL,
		WARNING_TONE);

	SetLeftSoftkeyFunction((void(*)(void))yesconfirmFunc, KEY_EVENT_UP);
	SetRightSoftkeyFunction((void(*)(void))noconfirmFunc, KEY_EVENT_UP);
}

static void mex_data_account_cb(uint32 account_id)
{
	char apn[100];
    int8 mode_dummy = 0;
	GoBackToHistory(E_MSI_APN_SELECT_SCREEN);
	if(g_sim_info == MMI_DATA_ACCOUNT_SIM2)
	{
			sim = E_MEX_SIM_2;
			accountId = MexInterface_always_ask_encode_data_account(account_id,
				MexConfig_ConvertMtkDef(E_M2MDC_ALWAYS_ASK_SIM_INFO_2),1,0,mode_dummy);
	}
#ifdef __MET_MEX_SIM_PLUS__
	else if(g_sim_info == MMI_DATA_ACCOUNT_SIM3)
	{
			sim = E_MEX_SIM_3;
			accountId = MexInterface_always_ask_encode_data_account(account_id,
				MexConfig_ConvertMtkDef(E_M2MDC_ALWAYS_ASK_SIM_INFO_3),1,0,mode_dummy);
	}
	else if(g_sim_info == MMI_DATA_ACCOUNT_SIM4)
	{
			sim = E_MEX_SIM_4;
			accountId = MexInterface_always_ask_encode_data_account(account_id,
				MexConfig_ConvertMtkDef(E_M2MDC_ALWAYS_ASK_SIM_INFO_4),1,0,mode_dummy);
	}
#endif/*__MET_MEX_SIM_PLUS__*/
	else
	{
			sim = E_MEX_SIM_1;
			accountId = MexInterface_always_ask_encode_data_account(account_id,
				MexConfig_ConvertMtkDef(E_M2MDC_ALWAYS_ASK_SIM_INFO_1),1,0,mode_dummy);
	}

	if(MexInterface_GetApn(accountId,apn,100))
	{
		int ret = mex_strcmp(apn,"cmwap");
		if(!ret)
		{
			mode = E_MNM_CMWAP;		//移动代理
		}
		else
		{
			mode = E_MNM_CMNET;		//直连
		}


		//设置成功!是否设置为默认接入点?
		Mex_ShowConfirmDialog( (void *)Mex_SelectDataAccountSuccess,
							   (void *)Mex_SelectDataAccountSuccessAlwaysAsk,
							   (UI_string_type)"\xBE\x8B\x6E\x7F\x10\x62\x9F\x52\x21\x0\x2F\x66\x26\x54\xBE\x8B\x6E\x7F\x3A\x4E\xD8\x9E\xA4\x8B\xA5\x63\x65\x51\xB9\x70\x3F\x0\0" );
	}
	else
	{
		//设置失败,是否重新设置?
		Mex_ShowConfirmDialog( (void *)Mex_SelectDataAccountFail,
							   (void *)GoBackHistory,
							   (UI_string_type)"\xBE\x8B\x6E\x7F\x31\x59\x25\x8D\x2C\x0\x2F\x66\x26\x54\xCD\x91\xB0\x65\xBE\x8B\x6E\x7F\x3F\x0\0" );
	}

}

#if (MEX_MTK_CODE_VERSION >= 0x1032)
static mmi_ret met_mex_dtcnt_cui_proc(mmi_event_struct *evt) 
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    cui_dtcnt_select_event_struct  *event;
    U32 data_acoount_id;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    event = (cui_dtcnt_select_event_struct*)evt;

    switch (evt->evt_id)
    {
        case CUI_DTCNT_SELECT_EVENT_RESULT_OK:
			/*
				mex_data_account_cb(cbm_get_original_account(event->account_id));  
*/
				mex_data_account_cb(event->account_id);
                cui_dtcnt_select_close(event->sender_id);
            break;

        case CUI_DTCNT_SELECT_EVENT_RESULT_CANCEL:
        case CUI_DTCNT_SELECT_EVENT_CLOSE:
            cui_dtcnt_select_close(event->sender_id);      
            break;
        case CUI_DTCNT_SELECT_EVENT_RESULT_FAILED:
             break;
        default:
            break;
	}
    return MMI_RET_OK;
}

void mmi_mex_entry_network_data_account_cui(void)
{
    cui_dtcnt_select_run_struct para;
    MMI_ID ret_id;
    mmi_id da_grp_id;
	
    para.account_id = MMI_DTCNT_INVALID_ID;
    para.AppMenuID = E_MSI_MAIN;
    para.app_id = defualt_account;
    para.bearers = DATA_ACCOUNT_BEARER_ALL;
    para.sim_selection = CUI_DTCNT_SELECT_DUAL_SIM;
    para.type = CUI_DTCNT_SELECT_TYPE_NORMAL;
    para.option = DTCNT_SELECT_BEARER_SWITCH | DTCNT_SELECT_ALWAYS_ASK | DTCNT_SELECT_SIM;
    para.icon_id = NULL;
    para.str_id = STR_GLOBAL_DATA_ACCOUNT;
    ret_id = mmi_frm_group_create(GRP_ID_ROOT, E_MSI_APN_SELECT_SCREEN, met_mex_dtcnt_cui_proc, NULL);

    if (!ret_id)
    {
       mmi_display_popup((UI_string_type)get_string(STR_GLOBAL_NOT_AVAILABLE),MMI_EVENT_FAILURE);
       return;
    }
    ret_id = mmi_frm_group_enter(E_MSI_APN_SELECT_SCREEN, MMI_FRM_NODE_SMART_CLOSE_FLAG);
    if (!ret_id)
    {
        mmi_display_popup((UI_string_type)get_string(STR_GLOBAL_NOT_AVAILABLE),MMI_EVENT_FAILURE);
        return;
    }

    da_grp_id = cui_dtcnt_select_create(E_MSI_APN_SELECT_SCREEN);
	
	
    if(da_grp_id != GRP_ID_INVALID)
    {
        cui_dtcnt_select_set_param(da_grp_id, &para);
        cui_dtcnt_select_run(da_grp_id);
    }
    else
    {
        mmi_frm_group_close(E_MSI_APN_SELECT_SCREEN);
    }
}
#endif/*MEX_MTK_CODE_VERSION*/


void Mex_Select_Date_Account(void)
{
#if (MEX_MTK_CODE_VERSION >= 0x1032)
	mmi_mex_entry_network_data_account_cui();
#else /*MEX_MTK_CODE_VERSION*/
#if (MEX_MTK_CODE_VERSION < 0x0932)
//如果在在MexLib.h文件定义了MEX_SEL_DATAACCOUNT_SUPPORT宏，需要将下面的这个函数调用的最后一个参数去掉
	mmi_dtcnt_select_account((FuncPtrU32)mex_data_account_cb, E_MSI_APN_SELECT_SCREEN, DATA_ACCOUNT_BEARER_GPRS | DATA_ACCOUNT_BEARER_WIFI ,defualt_account);
#else
    mmi_dtcnt_select_account((FuncPtrU32)mex_data_account_cb,
                                    E_MSI_APN_SELECT_SCREEN,
                                    DATA_ACCOUNT_BEARER_ALL,
                                    defualt_account,
                                    MMI_DTCNT_INVALID_ID,
                                    DTCNT_SELECT_BEARER_SWITCH | DTCNT_SELECT_ALWAYS_ASK | DTCNT_SELECT_SIM);
#endif/*MEX_MTK_CODE_VERSION*/
#endif
}

bool Mex_EntrySelectDateAccountScreen(void)
{
	int32 dataAccount;
	int8 sim;
	int8 apn;
	int8 bAlwaysAak;
	char str_apn[100];
	extern bool MexSoc_CheckDataAccount(int32 *dataAccount, int8 *apn, int8 *sim, int8 *bAlwaysAak);
	if(MexSoc_CheckDataAccount(&dataAccount, &apn, &sim, &bAlwaysAak))
	{

		if(MexInterface_GetApn(dataAccount,str_apn,100))
		{
			if(bAlwaysAak)
			{
				defualt_account = dataAccount;
				Mex_Entry_ApnSelectScreen();
				return true;
			}
			return false;
		}

	}
	defualt_account = 0;
	Mex_Entry_ApnSelectScreen();
	return true;

}

#endif


void Mex_Entry_ApnSelectScreen(void)
{
#ifdef MEX_SEL_DATAACCOUNT_SUPPORT
	//欢迎使用至尊宝3G网络设置向导！
#define STR_SELECT_DATAACCOUNT_TIP "\x22\x6B\xCE\x8F\x7F\x4F\x28\x75\xF3\x81\xA\x5C\x9D\x5B\x33\x0\x47\x0\x51\x7F\xDC\x7E\xBE\x8B\x6E\x7F\x11\x54\xFC\x5B\x1\xFF\0"


	EntryNewScreen(E_MSI_APN_SELECT_SCREEN,NULL, Mex_Entry_ApnSelectScreen, NULL);
	ShowCategory74Screen(0,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,(U8 *)STR_SELECT_DATAACCOUNT_TIP,mex_wcslen((U16 *)STR_SELECT_DATAACCOUNT_TIP),NULL);
	SetLeftSoftkeyFunction(Mex_Select_Date_Account, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#endif
}


#if defined(MEX_QA_MODE)
static char currAppNameParam[50] = "Mex_QQ.mex:";   //默认选项为QQ
static char currAppName[50] = "Mex_QQ.mex";         //默认选项为QQ
static int32 currAppId = E_MSVT_QQ;

void Mex_Test_UpdateCurrAppInfo(U8* pAppName, U8* pAppId)
{
	mex_strcpy(currAppName, mex_twcs2str((wchar_t *)pAppName));
	mex_strcpy(currAppNameParam, currAppName);
	mex_strcat(currAppNameParam, ":");
	currAppId = mex_atoi( mex_twcs2str((wchar_t *)pAppId) );
}

void Mex_Test_EntryApp(void)
{
 	MexSystem_SetSystemInitParam( currAppNameParam );
	Mex_SetCurrentEntryAppInfo( currAppName , currAppId);
	Mex_Entry_Screen_Internal( Mex_Test_EntryApp );
}

int32 Mex_Test_CheckCurrAppId(char *appInitParam )
{
	if( mex_strstr( appInitParam,currAppNameParam ) != NULL)
	{
		return currAppId;
	}

	return 0;
}

#endif


/***************************飞信独立入口******************************/
#if defined(__MEX_FETION_SUPPORT__)


// Fetion 独立入口，可变:)
void Mex_Entry_Fetion(void)
{
    Mex_Second_Menu_Entry_App(L"fetion","fetion.mex","fetion.mex:",(const wchar_t*)"\xde\x98\xe1\x4f",E_MSVT_FETION);
}


// Fetion独立入口 HighLight
void Mex_HighLight_Fetion_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Fetion, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


#endif
/****************************************************************************/

/***************************沃米独立入口******************************/
#if defined(__MEX_WMSP_SUPPORT__)||defined(__MEX_TCJY_SUPPORT__)
// WoChat 独立入口，可变:)
void Mex_Entry_WoChat(void)
{
    Mex_Second_Menu_Entry_App(L"mex_wochat","mex_wochat.mex","mex_wochat.mex:",(const wchar_t*)"\x83\x6C\x73\x7C\xC6\x89\x91\x98",E_MSVT_WOCHAT);
}


// WoChat独立入口 HighLight
void Mex_HighLight_WoChat_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_WoChat, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

#endif
	/****************************************************************************/

/***************************乱舞三国独立入口******************************/
#if defined(__MEX_LWSG_SUPPORT__)||defined(__MEX_MFWY_SUPPORT__)
void Mex_Entry_LWSG(void)
{
	MexSystem_SetSystemInitParam("lwsg.mex:");
	Mex_SetCurrentEntryAppInfo( "lwsg.mex" , E_MSVT_LWSG);
	Mex_Entry_Screen_Internal( Mex_Entry_LWSG );
}
void Mex_HighLight_LWSG_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_LWSG, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/

/***************************农场独立入口******************************/
#if defined(__MEX_QQNC_SUPPORT__)||defined(__MEX_KXNC_SUPPORT__)
void Mex_Entry_Farm(void)
{
	MexSystem_SetSystemInitParam("farm.mex:");
	Mex_SetCurrentEntryAppInfo( "farm.mex" , E_MSVT_FARM);
	Mex_Entry_Screen_Internal( Mex_Entry_Farm );
}
void Mex_HighLight_Farm_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Farm, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/

/***************************植物大战僵尸独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
void Mex_Entry_ZWDZJS(void)
{
    Mex_Second_Menu_Entry_App(L"ZWDZJS","zwzjs.mex","zwzjs.mex:",(const wchar_t*)"\x0D\x69\x69\x72\x27\x59\x18\x62\xF5\x50\x38\x5C\x00",E_MSVT_ZWDZJS);
}
void Mex_HighLight_ZWDZJS_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_ZWDZJS, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/


/***************************斗地主独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
void Mex_Entry_DDZ(void)
{
   // Mex_Second_Menu_Entry_App(L"DDZ","ddz.mex","ddz.mex:",(const wchar_t*)"\x06\x72\x11\x7B\x09\x4E\xFD\x56\x97\x65\x30\x57\x3B\x4E\x00",E_MSVT_DDZ);
    Mex_Second_Menu_Entry_App(L"DDZ","ddz.mex","ddz.mex:",(const wchar_t*)"\x06\x72\x11\x7B\x09\x4E\xFD\x56\x97\x65\x30\x57\x3B\x4E\x00",E_MSVT_DDZ);
}
#if MEX_GPRS_ALLOW
void Mex_Entry_DDZH(void)
{
mmi_display_popup_confirm(
		(UI_string_type) GetString(STR_GLOBAL_YES),
		(U8*) GetImage(IMG_GLOBAL_YES), 
		(UI_string_type) GetString(STR_GLOBAL_NO),
		(U8*) GetImage(IMG_GLOBAL_NO), 
		(UI_string_type) GetString(E_MSI_GPRS_ALLOW_STRING),
		MMI_EVENT_QUERY);

    SetLeftSoftkeyFunction(Mex_Entry_DDZ, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP); 
}
#endif
void Mex_HighLight_DDZ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	#if MEX_GPRS_ALLOW
	SetLeftSoftkeyFunction(Mex_Entry_DDZH, KEY_EVENT_UP);
        #else
	SetLeftSoftkeyFunction(Mex_Entry_DDZ, KEY_EVENT_UP);
	#endif
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/


/***************************反恐cs独立入口******************************/
#if defined(__MEX_FKCS_SUPPORT__)||defined(__MEX_CYHX_SUPPORT__)
void Mex_Entry_FKCS(void)
{
	MexSystem_SetSystemInitParam("fkcs.mex:");
	Mex_SetCurrentEntryAppInfo( "fkcs.mex" , E_MSVT_FKCS);
	Mex_Entry_Screen_Internal( Mex_Entry_FKCS );
}
void Mex_HighLight_FKCS_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_FKCS, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/

/***************************金山词霸独立入口******************************/
#if defined(__MEX_JSCB_SUPPORT__)||defined(__MEX_KXCD_SUPPORT__)

// jscb 独立入口，可变:)
void Mex_Entry_JSCB(void)
{
    Mex_Second_Menu_Entry_App(L"jscb","jscb.mex","jscb.mex:",(const wchar_t*)"\xD1\x91\x71\x5C\xCD\x8B\x38\x97",E_MSVT_JSCB);
}


// JSCB独立入口 HighLight
void Mex_HighLight_JSCB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_JSCB, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

#endif
/***************************大众点评独立入口******************************/
#if defined(__MEX_DZDP_SUPPORT__)
// 大众点评
void Mex_Entry_DZDP(void)  
{
    Mex_Second_Menu_Entry_App(L"DZDP","DZDP.mex","DZDP.mex:",(const wchar_t*)"\x59\x27\x4F\x17\x70\xB9\x8B\xC4",E_MSVT_DZDP);
}

// 大众点评 HighLight
void Mex_HighLight_DZDP_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_DZDP, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_DZDP_SUPPORT__*/
/****************************************************************************/

/***************************彩票独立入口******************************/
#if defined(__MEX_CPTZ_SUPPORT__)||defined(__MEX_CPZDJ_SUPPORT__)
void Mex_Entry_CSJ(void)
{
	MexSystem_SetSystemInitParam("csj.mex:");
	Mex_SetCurrentEntryAppInfo( "csj.mex" , E_MSVT_CSJ);
	Mex_Entry_Screen_Internal( Mex_Entry_CSJ );
}
void Mex_HighLight_CSJ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_CSJ, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/

/***************************找房独立入口******************************/
#if defined(__MEX_GJWZF_SUPPORT__)||defined(__MEX_MFZF_SUPPORT__)
void Mex_Entry_ZFT(void)
{
	MexSystem_SetSystemInitParam("zft.mex:");
	Mex_SetCurrentEntryAppInfo( "zft.mex" , E_MSVT_ZFT);
	Mex_Entry_Screen_Internal( Mex_Entry_ZFT );
}
void Mex_HighLight_ZFT_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_ZFT, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/


/***************************幽默大师独立入口******************************/
#if defined(__MEX_YMMH_SUPPORT__)||defined(__MEX_YMDS_SUPPORT__)
void Mex_Entry_YMDS(void)
{
	MexSystem_SetSystemInitParam("ymds.mex:");
	Mex_SetCurrentEntryAppInfo( "ymds.mex" , E_MSVT_YMDS);
	Mex_Entry_Screen_Internal( Mex_Entry_YMDS );
}
void Mex_HighLight_YMDS_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_YMDS, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/

/***************************播放器独立入口******************************/
#if defined(__MEX_SDYY_SUPPORT__)||defined(__MEX_WYJC_SUPPORT__)
void Mex_Entry_Player(void)
{
	MexSystem_SetSystemInitParam("player.mex:");
	Mex_SetCurrentEntryAppInfo( "player.mex" , E_MSVT_PLAYER);
	Mex_Entry_Screen_Internal( Mex_Entry_Player );
}
void Mex_HighLight_Player_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Player, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/


/***************************红豆婚恋独立入口******************************/
#if defined(__MEX_HDHL_SUPPORT__)||defined(__MEX_YFDDP_SUPPORT__)
void Mex_Entry_HDHL(void)
{
	MexSystem_SetSystemInitParam("hdhl.mex:");
	Mex_SetCurrentEntryAppInfo( "hdhl.mex" , E_MSVT_HDHL);
	Mex_Entry_Screen_Internal( Mex_Entry_HDHL );
}
void Mex_HighLight_HDHL_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_HDHL, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/


/***************************话匣子独立入口******************************/
#if defined(__MEX_SGDJT_SUPPORT__)||defined(__MEX_BJLT_SUPPORT__)
void Mex_Entry_HXZ(void)
{
	MexSystem_SetSystemInitParam("hxz.mex:");
	Mex_SetCurrentEntryAppInfo( "hxz.mex" , E_MSVT_HXZ);
	Mex_Entry_Screen_Internal( Mex_Entry_HXZ );
}
void Mex_HighLight_HXZ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_HXZ, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif
/****************************************************************************/

/***************************赶集生活独立入口******************************/
#if 0 
void Mex_Entry_GANJILIFE(void)
{
	MexSystem_SetSystemInitParam("ganjilife.mex:");
	Mex_SetCurrentEntryAppInfo( "ganjilife.mex" , E_MSVT_GANJILIFE);
	Mex_Entry_Screen_Internal( Mex_Entry_GANJILIFE );
}
void Mex_HighLight_GANJILIFE_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_GANJILIFE, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_GANJILIFE_SUPPORT__*/
/****************************************************************************/

/***************************通用社区入口******************************/
#if defined(__MEX_MEXSQAPP_SUPPORT__)
void Mex_Entry_MEXSQAPP(void)
{
	MexSystem_SetSystemInitParam("MexSqApp.mex:");
	Mex_SetCurrentEntryAppInfo( "MexSqApp.mex" , E_MSVT_MEXSQAPP);
	Mex_Entry_Screen_Internal( Mex_Entry_MEXSQAPP );
}
void Mex_HighLight_MEXSQAPP_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_MEXSQAPP, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_MEXSQAPP_SUPPORT__*/
/****************************************************************************/

/***************************自动对时入口******************************/
#if 0
void Mex_Entry_SETTIME(void)
{
	MexSystem_SetSystemInitParam("settime.mex:");
	Mex_SetCurrentEntryAppInfo( "settime.mex" , E_MSVT_SETTIME);
	Mex_Entry_Screen_Internal( Mex_Entry_SETTIME );
}
void Mex_HighLight_SETTIME_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_SETTIME, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_SETTIME_SUPPORT__*/
/****************************************************************************/

/***************************机甲风暴入口******************************/
#if 0
void Mex_Entry_JJFB(void)
{
	MexSystem_SetSystemInitParam("jjfb.mex:");
	Mex_SetCurrentEntryAppInfo( "jjfb.mex" , E_MSVT_JJFB);
	Mex_Entry_Screen_Internal( Mex_Entry_JJFB );
}
void Mex_HighLight_JJFB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_JJFB, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_JJFB_SUPPORT__*/
/****************************************************************************/

/***************************极品飞车入口******************************/
#if 0
void Mex_Entry_JPFCKB(void)
{
	MexSystem_SetSystemInitParam("jpfckb.mex:");
	Mex_SetCurrentEntryAppInfo( "jpfckb.mex" , E_MSVT_JPFCKB);
	Mex_Entry_Screen_Internal( Mex_Entry_JPFCKB );
}
void Mex_HighLight_JPFCKB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_JPFCKB, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_JPFCKB_SUPPORT__*/
/****************************************************************************/


/***************************愤怒的小鸟独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// FNDXN 独立入口，可变:)
void Mex_Entry_FNDXN(void)
{
    Mex_Second_Menu_Entry_App(L"AngryBirds","AngryBirds.mex","AngryBirds.mex:",(const wchar_t*)"\x24\x61\x12\x60\x84\x76\x0F\x5C\x1F\x9E",E_MSVT_FNDXN);
}

// FNDXN独立入口 HighLight
void Mex_HighLight_FNDXN_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_FNDXN, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif

/***************************愤怒的小鸟:里约版独立入口******************************/
#if defined(__MEX_FNDXN_RIO_SUPPORT__)
void Mex_Entry_FNDXN_RIO(void)
{
	MexSystem_SetSystemInitParam("Rio.mex:");
	Mex_SetCurrentEntryAppInfo( "Rio.mex" , E_MSVT_FNDXN_RIO);
	Mex_Entry_Screen_Internal( Mex_Entry_FNDXN_RIO );
}
void Mex_HighLight_FNDXN_RIO_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_FNDXN_RIO, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_FNDXN_RIO_SUPPORT__*/

/****************************************************************************/

/***************************Tom cat独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// tom cat 独立入口，可变:)
void Mex_Entry_HDMAO(void)
{
    Mex_Second_Menu_Entry_App(L"Tom Cat","tom.mex","tom.mex:",(const wchar_t*)"\x48\x00\x44\x00\x2B\x73\x00",E_MSVT_HDCAT);
}

// Tom cat独立入口 HighLight
void Mex_HighLight_TOMMAO_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_HDMAO, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_TOMMAO_SUPPORT__*/
/****************************************************************************/


/***************************吹裙子独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// CQZ 独立入口，可变:)
void Mex_Entry_CQZ(void)
{
    Mex_Second_Menu_Entry_App(L"CQZ","cqz.mex","cqz.mex:",(const wchar_t*)"\x39\x54\xD9\x88\x50\x5B\x00",E_MSVT_CQZ);
}

// CQZ独立入口 HighLight
void Mex_HighLight_CQZ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_CQZ, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_CQZ_SUPPORT__*/
/****************************************************************************/

/***************************切水果独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// QSG 独立入口，可变:)
void Mex_Entry_QSG(void)
{
    Mex_Second_Menu_Entry_App(L"fruit","fruit.mex","fruit.mex:",(const wchar_t*)"\x07\x52\x34\x6C\x9C\x67\x00",E_MSVT_FRUITSLICE);
}

// QSG独立入口 HighLight
void Mex_HighLight_QSG_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_QSG, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_QSG_SUPPORT__*/
/****************************************************************************/

/***************************深海狩猎独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// SHSL独立入口，可变:)
void Mex_Entry_SHSL(void)
{
    Mex_Second_Menu_Entry_App(L"shsl","shsl.mex","shsl.mex:",(const wchar_t*)"\xF1\x6D\x77\x6D\xE9\x72\x0E\x73\x00",E_MSVT_SHSL);
}

// SHSL独立入口 HighLight
void Mex_HighLight_SHSL_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_SHSL, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_SHSL_SUPPORT__*/
/****************************************************************************/

/***************************CS狙击手独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// CSJJS 独立入口，可变:)
void Mex_Entry_CSJJS(void)
{
    Mex_Second_Menu_Entry_App(L"csjjs","csjjs.mex","csjjs.mex:",(const wchar_t*)"\x43\x00\x53\x00\xD9\x72\xFB\x51\x4B\x62\x00",E_MSVT_CSJJS);
}

// CSJJS独立入口 HighLight
void Mex_HighLight_CSJJS_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_CSJJS, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_CSJJS_SUPPORT__*/
/****************************************************************************/

/***************************暴力飞车独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// BLFC 独立入口，可变:)
void Mex_Entry_BLFC(void)
{
    Mex_Second_Menu_Entry_App(L"rocket","rocket.mex","rocket.mex:",(const wchar_t*)"\xB4\x66\x9B\x52\xDE\x98\x66\x8F\x00",E_MSVT_BLFC);
}

// BLFC独立入口 HighLight
void Mex_HighLight_BLFC_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_BLFC, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_BLFC_SUPPORT__*/
/****************************************************************************/

/***************************明星三缺一独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// MXSQ 独立入口，可变:)
void Mex_Entry_MXSQY(void)
{
    Mex_Second_Menu_Entry_App(L"mxsqy","mxsqy.mex","mxsqy.mex:",(const wchar_t*)"\x0E\x66\x1F\x66\x09\x4E\x3A\x7F\x00\x4E\x00",E_MSVT_MXSQY);
}

// QSG独立入口 HighLight
void Mex_HighLight_MXSQY_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_MXSQY, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_MXSQY_SUPPORT__*/
/****************************************************************************/

/***************************丢纸团独立入口******************************/
#if 1//__MEX_MENU_HIDE_SUPPORT__
// MXSQ 独立入口，可变:)
void Mex_Entry_DZT(void)
{
    Mex_Second_Menu_Entry_App(L"dzt","dzt.mex","dzt.mex:",(const wchar_t*)"\x22\x4E\xB8\x7E\xE2\x56\x00",E_MSVT_DZT);
}

// QSG独立入口 HighLight
void Mex_HighLight_DZT_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_DZT, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_DZT_SUPPORT__*/
/****************************************************************************/


/***************************号码卫士独立入口******************************/
#if defined(__MEX_AQ_SUPPORT__)
// AQ 独立入口，可变:)
void Mex_Entry_Aq(void)
{
    Mex_Second_Menu_Entry_App(L"AreaInquire","AreaInquire.mex","AreaInquire.mex:",(const wchar_t*)"\x52\x07\x6C\x34\x67\x9C",E_MSVT_AQ);
}

// QSG独立入口 HighLight
void Mex_HighLight_Aq_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Aq, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_AQ_SUPPORT__*/
/****************************************************************************/

/***************************人人网独立入口******************************/
#if defined(__MEX_RENREN_SUPPORT__)
// 人人网独立入口，可变:)
#define MY_RENREN_SEARCH_URL		L"http://3g.renren.com"
void Mex_Entry_Renren(void)
{
    extern int wap_start_browser();
   // Mex_Second_Menu_Entry_App(L"AreaInquire","AreaInquire.mex","AreaInquire.mex:",(const wchar_t*)"\x52\x07\x6C\x34\x67\x9C",E_MSVT_AQ);    
    wap_start_browser(WAP_BROWSER_GOTO_URL, (const kal_uint8*)MY_RENREN_SEARCH_URL);
}

// 人人网独立入口 HighLight
void Mex_HighLight_Renren_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Renren, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_RENREN_SUPPORT__*/
/****************************************************************************/

/***************************开心网独立入口******************************/
#if defined(__MEX_KAIXIN_SUPPORT__)
// 开心网独立入口，可变)
#define MY_KAIXIN_SEARCH_URL		L"http://wap.kaixin001.com"
void Mex_Entry_Kaixin(void)
{
    extern int wap_start_browser();
    //Mex_Second_Menu_Entry_App(L"AreaInquire","AreaInquire.mex","AreaInquire.mex:",(const wchar_t*)"\x52\x07\x6C\x34\x67\x9C",E_MSVT_AQ);
    wap_start_browser(WAP_BROWSER_GOTO_URL, (const kal_uint8*)MY_KAIXIN_SEARCH_URL);
}

// 开心网独立入口 HighLight
void Mex_HighLight_Kaixin_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Kaixin, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_KAIXIN_SUPPORT__*/
/****************************************************************************/

/***************************淘宝网独立入口******************************/
#if defined(__MEX_TAOBAO_SUPPORT__)
// 淘宝网独立入口，可变:)
#define MY_TAOBAO_SEARCH_URL		L"http://m.taobao.com"
void Mex_Entry_Taobao(void)
{
    extern int wap_start_browser();
   // Mex_Second_Menu_Entry_App(L"AreaInquire","AreaInquire.mex","AreaInquire.mex:",(const wchar_t*)"\x52\x07\x6C\x34\x67\x9C",E_MSVT_AQ);
    wap_start_browser(WAP_BROWSER_GOTO_URL, (const kal_uint8*)MY_TAOBAO_SEARCH_URL);
}

//淘宝网独立入口 HighLight
void Mex_HighLight_Taobao_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_Taobao, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_TAOBAO_SUPPORT__*/

/****************************************************************************/

/***************************叫子弹飞独立入口******************************/
#if defined(__MEX_BULLETSFLY_SUPPORT__)
// BULLETSFLY 独立入口，可变:)
void Mex_Entry_BULLETSFLY(void)
{
    Mex_Second_Menu_Entry_App(L"Bulletsfly","Bulletsfly.mex","Bulletsfly.mex:",(const wchar_t*)"\x53\xEB\x5B\x50\x5F\x39\x98\xDE",E_MSVT_BULLETSFLY);

}

// BULLETSFLY独立入口 HighLight
void Mex_HighLight_BULLETSFLY_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_BULLETSFLY, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_BULLETSFLY_SUPPORT__*/
/****************************************************************************/



/***************************魔力骰子独立入口******************************/
#if defined(__MEX_MLSZ_SUPPORT__)
void Mex_Entry_MLSZ(void)
{
	MexSystem_SetSystemInitParam("mlsz.mex:");
	Mex_SetCurrentEntryAppInfo( "mlsz.mex" , E_MSVT_MLSZ);
	Mex_Entry_Screen_Internal( Mex_Entry_MLSZ );
}
void Mex_HighLight_MLSZ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_MLSZ, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_MLSZ_SUPPORT__*/
/****************************************************************************/

/***************************赶集生活独立入口******************************/
#if defined(__MEX_GANJILIFE_SUPPORT__)
void Mex_Entry_GANJILIFE(void)
{
	MexSystem_SetSystemInitParam("ganjilife.mex:");
	Mex_SetCurrentEntryAppInfo( "ganjilife.mex" , E_MSVT_GANJILIFE);
	Mex_Entry_Screen_Internal( Mex_Entry_GANJILIFE );
}
void Mex_HighLight_GANJILIFE_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_GANJILIFE, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_GANJILIFE_SUPPORT__*/
/****************************************************************************/

/***************************通用社区入口******************************/
#if 0
void Mex_Entry_MEXSQAPP(void)
{
	MexSystem_SetSystemInitParam("MexSqApp.mex:");
	Mex_SetCurrentEntryAppInfo( "MexSqApp.mex" , E_MSVT_MEXSQAPP);
	Mex_Entry_Screen_Internal( Mex_Entry_MEXSQAPP );
}
void Mex_HighLight_MEXSQAPP_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_MEXSQAPP, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_MEXSQAPP_SUPPORT__*/
/****************************************************************************/

/***************************自动对时入口******************************/
#if defined(__MEX_SETTIME_SUPPORT__)
void Mex_Entry_SETTIME(void)
{
	
	Mex_Second_Menu_Entry_App(L"settime","settime.mex","settime.mex:",(const wchar_t*)"\xEA\x81\xA8\x52\xF9\x5B\xF6\x65",E_MSVT_SETTIME);
}
void Mex_HighLight_SETTIME_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_SETTIME, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_SETTIME_SUPPORT__*/
/****************************************************************************/

/***************************机甲风暴入口******************************/
#if defined(__MEX_JJFB_SUPPORT__)
void Mex_Entry_JJFB(void)
{
	MexSystem_SetSystemInitParam("jjfb.mex:");
	Mex_SetCurrentEntryAppInfo( "jjfb.mex" , E_MSVT_JJFB);
	Mex_Entry_Screen_Internal( Mex_Entry_JJFB );
}
void Mex_HighLight_JJFB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_JJFB, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_JJFB_SUPPORT__*/
/****************************************************************************/

/***************************极品飞车入口******************************/
#if defined(__MEX_JPFCKB_SUPPORT__)
void Mex_Entry_JPFCKB(void)
{
	Mex_Second_Menu_Entry_App(L"jpfckb","jpfckb.mex","jpfckb.mex:",(const wchar_t*)"\x81\x67\xC1\x54\xDE\x98\x66\x8F",E_MSVT_JPFCKB);
}
void Mex_HighLight_JPFCKB_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_JPFCKB, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_JPFCKB_SUPPORT__*/
/****************************************************************************/



/***************************动感平衡球独立入口******************************/
#if defined(__MEX_MvBol_SUPPORT__)
void Mex_Entry_MvBol(void)
{
	MexSystem_SetSystemInitParam("MvBol.mex:");
	Mex_SetCurrentEntryAppInfo( "MvBol.mex" , E_MSVT_MVBOL);
	Mex_Entry_Screen_Internal( Mex_Entry_MvBol );
}
void Mex_HighLight_MvBol_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_MvBol, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_MvBol_SUPPORT__*/
/****************************************************************************/



/***************************裙角飞扬独立入口******************************/
#if defined(__MEX_HDcqz_SUPPORT__)
void Mex_Entry_HDcqz(void)
{
	Mex_Second_Menu_Entry_App(L"HDcqz","HDcqz.mex","HDcqz.mex:",(const wchar_t*)"\xD9\x88\xD2\x89\xDE\x98\x6C\x62",E_MSVT_HDCQZ);
}
void Mex_HighLight_HDcqz_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_HDcqz, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_HDcqz_SUPPORT__*/
/****************************************************************************/



/***************************HD天气独立入口******************************/
#if defined(__MEX_HDtqyb_SUPPORT__)
void Mex_Entry_HDtqyb(void)
{
	MexSystem_SetSystemInitParam("HDtqyb.mex:");
	Mex_SetCurrentEntryAppInfo( "HDtqyb.mex" , E_MSVT_HDTQYB);
	Mex_Entry_Screen_Internal( Mex_Entry_HDtqyb );
}
void Mex_HighLight_HDtqyb_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_HDtqyb, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_HDtqyb_SUPPORT__*/
/****************************************************************************/



/***************************HTC天气独立入口******************************/
#if defined(__MEX_HTC_TQ_SUPPORT__)
void Mex_Entry_HTC_TQ(void)
{
	MexSystem_SetSystemInitParam("HTC_TQ.mex:");
	Mex_SetCurrentEntryAppInfo( "HTC_TQ.mex" , E_MSVT_HTC_TQ);
	Mex_Entry_Screen_Internal( Mex_Entry_HTC_TQ );
}
void Mex_HighLight_HTC_TQ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_HTC_TQ, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_HTC_TQ_SUPPORT__*/
/****************************************************************************/



/***************************i天气独立入口******************************/
#if defined(__MEX_i_TQ_SUPPORT__)
void Mex_Entry_i_TQ(void)
{
	MexSystem_SetSystemInitParam("i_TQ.mex:");
	Mex_SetCurrentEntryAppInfo( "i_TQ.mex" , E_MSVT_I_TQ);
	Mex_Entry_Screen_Internal( Mex_Entry_i_TQ );
}
void Mex_HighLight_i_TQ_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_i_TQ, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_i_TQ_SUPPORT__*/
/****************************************************************************/



/***************************魔幻打火机独立入口******************************/
#if defined(__MEX_Lighter_SUPPORT__)
void Mex_Entry_Lighter(void)
{
	
	
	Mex_Second_Menu_Entry_App(L"LighterHD","LighterHD.mex","LighterHD.mex:",(const wchar_t*)"\x54\x9B\x7B\x5E\x53\x62\x6B\x70\x3A\x67",E_MSVT_LIGHTER);
}
void Mex_HighLight_Lighter_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_Lighter, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_Lighter_SUPPORT__*/
/****************************************************************************/



/***************************魔幻打火机HD独立入口******************************/
#if defined(__MEX_LighterHD_SUPPORT__)
void Mex_Entry_LighterHD(void)
{
	
	Mex_Second_Menu_Entry_App(L"LighterHD","LighterHD.mex","LighterHD.mex:",(const wchar_t*)"\x54\x9B\x7B\x5E\x53\x62\x6B\x70\x3A\x67",E_MSVT_LIGHTER);
}
void Mex_HighLight_LighterHD_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(Mex_Entry_LighterHD, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
#endif /*End of __MEX_LighterHD_SUPPORT__*/
/****************************************************************************/



int32 Mex_GetCurrEntryAppId(char *pInitParam)
{
	if( mex_strstr(pInitParam,pCurrEntryAppName) != NULL)
	{
		return iCurrEntryAppID;
	}
	else
	{
        mex_log(1, "E: forget to set entry app info!!!");
		assert(0);
		return 0;
	}
}
#ifdef __J2ME__
 
void mex_stop_java(void)
{
	jam_enter_terminate_mode();
	mmi_java_entry_terminating(MMI_FALSE);
}

void mex_jam_stop_confirm(void)
{

DisplayConfirm(
STR_GLOBAL_YES,
IMG_GLOBAL_YES,
STR_GLOBAL_NO,
IMG_GLOBAL_NO,
/*需要退出后台JAVA，是否退出？*/
(UI_string_type)"\x0\x97\x81\x89\x0\x90\xFA\x51\xE\x54\xF0\x53\x4A\x0\x41\x0\x56\x0\x41\x0\xC\xFF\x2F\x66\x26\x54\x0\x90\xFA\x51\x1F\xFF\0",
NULL,
WARNING_TONE);
SetLeftSoftkeyFunction(mex_stop_java, KEY_EVENT_UP);
SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}
#endif


#if defined(__MEX_DMC_SUPPORT__)

typedef struct {
	int iRegStatus;
	int iExitCount;
	int iTimeCount;
	int bAutoStart;
}DMC_REG_FILE;

void Mex_Entry_DMC(void)
{
	
	Mex_Second_Menu_Entry_App(L"dzbk","dzbk.mex","dzbk.mex:",(const wchar_t*)"\x35\x75\x50\x5B\xDD\x4F\x61\x53",E_MSVT_DMC);
}
MMI_ID Mex_DMC_Launch (void *param, U32 param_size)
{
	extern void Mex_Entry_DMC(void);
	Mex_Entry_DMC();
	if (MexSystem_IsSysKernalRunning())
		return MMI_RET_OK;
	else
		return MMI_RET_ERR;
}
void Mex_HighLight_DMC_Menu(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(Mex_Entry_DMC, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void DMC_TimerCallback()
{
	static int iSetTime = 0; //minute
	DMC_REG_FILE head = { 0 };
	int32 *file = NULL;
	wchar_t *pRegFilePath = NULL;

	mex_CreateDir_ext( (const wchar_t*)mex_GetPhoneConfig(E_MPC_MexDir_In_Phone) );
	mex_CreateDir_ext( (const wchar_t*)mex_GetPhoneConfig(E_MPC_SystemRootDir) );
	if( mex_IsDirExist_ext((const wchar_t*)mex_GetPhoneConfig(E_MPC_MexDir_In_Phone)) )
	{
		pRegFilePath = (wchar_t*)mex_GetPhoneConfig(E_MPC_MexDir_In_Phone);
	}
	else if( mex_IsDirExist_ext((const wchar_t*)mex_GetPhoneConfig(E_MPC_SystemRootDir)) )
	{
		pRegFilePath = (wchar_t*)mex_GetPhoneConfig(E_MPC_SystemRootDir);
	}
	else
	{
		return;
	}
	
	file = mex_fopenw_ext( L"dmc_reg.dat", L"rb+", pRegFilePath, NULL, false);
	if(file != NULL)
	{
		//读取注册信息
		mex_fseek(file, 0, SEEK_SET);
		if(mex_fread(&head, sizeof(head), 1, file) != sizeof(head))
		{
			memset(&head, 0, sizeof(head));
		}

		if(head.bAutoStart)
		{
			head.bAutoStart = 0;
		}

		//已注册或过期
		if(head.iRegStatus == 2 || head.iRegStatus == 3)
		{
			mex_fclose(file);
			return;
		}
		//未注册或未检测
		else if(head.iRegStatus == 0 || head.iRegStatus == 1)
		{
			head.iTimeCount += iSetTime;
		}

		//时间判断
		if((head.iExitCount <= 2 && head.iTimeCount >= (head.iExitCount + 1) * 2 * 60)
			|| (head.iExitCount <= 3 && head.iTimeCount >= (int)(6.5 * 24 * 60)))
		{
			if(is_on_idlescreen() || mmi_idle_is_on_idle_screen())  //idle?
			{
				head.bAutoStart = 1;
				head.iTimeCount = 0;
				iSetTime = 30;
			}
			else
			{
				iSetTime = 5;
			}
		}
		else
		{
			iSetTime = 30;
		}

		mex_fseek(file, 0, SEEK_SET);
		mex_fwrite(&head, sizeof(head), 1, file);
		mex_fclose(file);

		if(iSetTime > 0)
			StartTimer(MEX_DMC_TIMER, iSetTime * 60 * 1000, DMC_TimerCallback);
	}

	if(head.bAutoStart)
	{
		mex_Phs_Set_Backlight(E_MPHS_SHORT_TIME);
		Mex_Entry_DMC();
	}
}



#endif
#if (MEX_MTK_CODE_VERSION >= 0x1112)

/****===============================================================================================================****/
#define MEX_ENG_CAPTION_LOG                 CUI_INLINE_ITEM_ID_BASE+0
#define MEX_ENG_STR_SELECT_LOG_TRIGGER      CUI_INLINE_ITEM_ID_BASE+1
#define MEX_ENG_CAPTION_CHANNEL             CUI_INLINE_ITEM_ID_BASE+2
#define MEX_ENG_NUM_EDIT_CHANNEL            CUI_INLINE_ITEM_ID_BASE+3
#define MEX_ENG_CAPTION_MODEL               CUI_INLINE_ITEM_ID_BASE+4
#define MEX_ENG_NUM_EDIT_MODEL              CUI_INLINE_ITEM_ID_BASE+5
#define MEX_ENG_CAPTION_SCR_WIDTH           CUI_INLINE_ITEM_ID_BASE+6
#define MEX_ENG_NUM_EDIT_SCR_WIDTH          CUI_INLINE_ITEM_ID_BASE+7
#define MEX_ENG_CAPTION_SCR_HEIGHT          CUI_INLINE_ITEM_ID_BASE+8
#define MEX_ENG_NUM_EDIT_SCR_HEIGHT         CUI_INLINE_ITEM_ID_BASE+9
#define MEX_ENG_CAPTION_TOTAL_TIME          CUI_INLINE_ITEM_ID_BASE+10
#define MEX_ENG_NUM_EDIT_TOTAL_TIME         CUI_INLINE_ITEM_ID_BASE+11
#define MEX_ENG_CAPTION_SUBMENU_UPDATE      CUI_INLINE_ITEM_ID_BASE+12
#define MEX_ENG_NUM_EDIT_SUBMENU_UPDATE     CUI_INLINE_ITEM_ID_BASE+13
#define MEX_ENG_CAPTION_ENTRY_NAME          CUI_INLINE_ITEM_ID_BASE+14
#define MEX_ENG_STR_EDIT_ENTRY_NAME         CUI_INLINE_ITEM_ID_BASE+15
#define MEX_ENG_CAPTION_ENTRY_NUM           CUI_INLINE_ITEM_ID_BASE+16
#define MEX_ENG_NUM_EDIT_ENTRY_NUM          CUI_INLINE_ITEM_ID_BASE+17
#define MEX_ENG_CAPTION_TEST_ENTRY          CUI_INLINE_ITEM_ID_BASE+18

//初始化inline menu的结构
//不要关注以下结构体中字符串的名称，这个只是初始值，在绘制的时候会重新赋值
static const cui_inline_item_caption_struct g_mex_engine_cap_log = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_channel = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_model = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_scr_width = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_scr_height = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_totaltime = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_submenu_update = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_entry_name = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_caption_struct g_mex_engine_cap_entry_num = 
{
    STR_GLOBAL_OPEN,
};
static const cui_inline_item_display_only_struct g_mex_engine_cap_test_entry = 
{
    STR_GLOBAL_OPEN,
    STR_GLOBAL_OPEN,
};
static const U16 g_mex_engine_select_trigger_str[2] = {STR_GLOBAL_OPEN, 
                                            STR_GLOBAL_OFF};

static const cui_inline_item_select_struct g_mex_engine_select_trigger = 
{
    2, 
    0, (U16*)g_mex_engine_select_trigger_str,
};



static const cui_inline_item_text_edit_struct g_mex_engine_number_textedit = 
{
    0,
    0,
    10,
    IMM_INPUT_TYPE_NUMERIC,
    0,
    NULL    
};
static const cui_inline_item_text_edit_struct g_mex_engine_string_textedit = 
{
    0,
    0,
    25,
    IMM_INPUT_TYPE_ASCII_CHAR,
    0,
    NULL    
};


static const cui_inline_item_softkey_struct g_mex_engine_inline_sofkey =
{
    {{CUI_INLINE_SOFTKEY_DEFAULT_VALUE, 0},
     {CUI_INLINE_SOFTKEY_DEFAULT_VALUE, 0},
     {STR_GLOBAL_CANCEL, 0}}        
};

static const cui_inline_set_item_struct g_mex_engine_inline[] = 
{
    {MEX_ENG_CAPTION_LOG,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L1,(void*)&g_mex_engine_cap_log},
    {MEX_ENG_STR_SELECT_LOG_TRIGGER,CUI_INLINE_ITEM_TYPE_SELECT,0,(void*)&g_mex_engine_select_trigger},
    {MEX_ENG_CAPTION_CHANNEL,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L2,(void*)&g_mex_engine_cap_channel},
    {MEX_ENG_NUM_EDIT_CHANNEL,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_MODEL,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L3,(void*)&g_mex_engine_cap_model},
    {MEX_ENG_NUM_EDIT_MODEL,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_SCR_WIDTH,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L4,(void*)&g_mex_engine_cap_scr_width},
    {MEX_ENG_NUM_EDIT_SCR_WIDTH,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_SCR_HEIGHT,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L5,(void*)&g_mex_engine_cap_scr_height},
    {MEX_ENG_NUM_EDIT_SCR_HEIGHT,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_TOTAL_TIME,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L6,(void*)&g_mex_engine_cap_totaltime},
    {MEX_ENG_NUM_EDIT_TOTAL_TIME,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_SUBMENU_UPDATE,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L7,(void*)&g_mex_engine_cap_submenu_update},
    {MEX_ENG_NUM_EDIT_SUBMENU_UPDATE,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_ENTRY_NAME,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L8,(void*)&g_mex_engine_cap_entry_name},
    {MEX_ENG_STR_EDIT_ENTRY_NAME,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_string_textedit},
    {MEX_ENG_CAPTION_ENTRY_NUM,CUI_INLINE_ITEM_TYPE_CAPTION,IMG_GLOBAL_L9,(void*)&g_mex_engine_cap_entry_num},
    {MEX_ENG_NUM_EDIT_ENTRY_NUM,CUI_INLINE_ITEM_TYPE_TEXT_EDIT,0,(void*)&g_mex_engine_number_textedit},
    {MEX_ENG_CAPTION_TEST_ENTRY,CUI_INLINE_ITEM_TYPE_DISPLAY_ONLY,IMG_GLOBAL_L10,(void*)&g_mex_engine_cap_test_entry},
    
};
//初始化完成
static const cui_inline_struct g_mex_engine_inline_scrn = 
{
    (sizeof(g_mex_engine_inline)/sizeof(cui_inline_set_item_struct)),
    NULL,
    0,
    CUI_INLINE_SCREEN_LOOP|CUI_INLINE_SCREEN_DISABLE_DONE,
    (cui_inline_item_softkey_struct*)&g_mex_engine_inline_sofkey,
    (const cui_inline_set_item_struct*)&g_mex_engine_inline,
};

static void mex_engine_set_default_values(void)
{
	engmode.iLogOnOffHiliteState = (g_MexLogFilter == 0);
	g_engmode_changed = false;
#ifdef MEX_QA_MODE
	mex_wsnprintf((wchar_t *)engmode.pVenderIDBuf,5,"%d",MexConfig_GetPhoneConfig(E_MPC_MexVenderID));
	mex_wsnprintf((wchar_t *)engmode.pPhoneUA,5,"%s",(char *)MexConfig_GetPhoneConfig(E_MPC_MexPhoneUA));

	#if MEX_SCREEN_ESPECIAL_VERSION
	{
		extern uint16 g_now_screen_width;
		extern uint16 g_now_screen_height;
		mex_wsnprintf((wchar_t *)engmode.pScreenWidth,5,"%d",g_now_screen_width);
		mex_wsnprintf((wchar_t *)engmode.pScreenHeight,5,"%d",g_now_screen_height);
	}
	#endif
	#ifdef MEX_ST_SUPPORT
	{
		extern int32 g_Mex_St_time;
		mex_wsnprintf((wchar_t *)engmode.pSTTime,5,"%d",g_Mex_St_time);
	}
	#endif
	#if defined(MEX_QQ_ESPECIAL_VERSION)
		mex_wsnprintf((wchar_t *)engmode.pEntrySecondLimit,5,"%d",g_mex_entry_second_limit);
	#endif
#endif

}    
static void mex_engine_get_values(MMI_ID parent_id)
{
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //LOG TRIGGER
    cui_inline_get_value(parent_id, MEX_ENG_STR_SELECT_LOG_TRIGGER, (void *)&engmode.iLogOnOffHiliteState);

#ifdef MEX_QA_MODE

    //channel number
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_CHANNEL, (void *)engmode.pVenderIDBuf);

    //model 
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_MODEL, (void *)engmode.pPhoneUA);
    
    //screen width
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_SCR_WIDTH, (void *)engmode.pScreenWidth);

    //screen height
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_SCR_HEIGHT, (void *)engmode.pScreenHeight);

    //return after product sale out
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_TOTAL_TIME, (void *)engmode.pSTTime);

    //submenu update per xxxx day
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_SUBMENU_UPDATE, (void *)engmode.pEntrySecondLimit);

    //entry name xxx.mex
    cui_inline_get_value(parent_id, MEX_ENG_STR_EDIT_ENTRY_NAME, (void *)engmode.pMexAppName);

    //entry mex number
    cui_inline_get_value(parent_id, MEX_ENG_NUM_EDIT_ENTRY_NUM, (void *)engmode.pMexAppID);
#endif

}
static void mex_engine_set_hdlr(MMI_ID parent_id)
{
    mex_engine_get_values(parent_id);

    Mex_SaveEngineerMode();
}

static void mex_engine_stop_hdlr(MMI_ID parent_id)
{
    cui_inline_close(parent_id);
}

static MMI_RET mex_engine_proc(mmi_event_struct* evt)
{
    cui_event_inline_common_struct* inline_evt = (cui_event_inline_common_struct*) evt;
    cui_event_inline_notify_struct* notify_evt = (cui_event_inline_notify_struct*) evt;

    switch (inline_evt->evt_id)
    {
    case EVT_ID_CUI_INLINE_SUBMIT:
    case EVT_ID_CUI_INLINE_CSK_PRESS:
        mex_engine_set_hdlr(inline_evt->sender_id);
        mex_engine_stop_hdlr(inline_evt->sender_id);
        break;

    case EVT_ID_CUI_INLINE_ABORT:
        mex_engine_stop_hdlr(inline_evt->sender_id);
        break;
    case EVT_ID_CUI_INLINE_NOTIFY:
        break;
    case EVT_ID_CUI_INLINE_SET_KEY:
        {
            cui_event_inline_set_key_struct *inline_evt = (cui_event_inline_set_key_struct *)evt;
            U32 item = inline_evt->item_id;
            switch(item)
            {

#ifdef MEX_QA_MODE
                case MEX_ENG_CAPTION_TEST_ENTRY:
                    {
                		extern void Mex_Test_EntryApp(void);
                		extern void Mex_Test_UpdateCurrAppInfo(U8* pAppName, U8* pAppId);

                        mex_engine_get_values(inline_evt->sender_id);
                		Mex_Test_UpdateCurrAppInfo(engmode.pMexAppName,engmode.pMexAppID);
                        SetLeftSoftkeyFunction(Mex_Test_EntryApp, KEY_EVENT_UP);
                        SetCenterSoftkeyFunction(Mex_Test_EntryApp, KEY_EVENT_UP);
                    }
                    break;
#endif
            }
        }
        break;
    default:
        break;
    }

    return MMI_RET_OK;
}



void mex_engine(void)
{
    MMI_ID mex_eng_gid;
    MMI_ID mex_eng_inline_id;
    S8* trigger_str[2];
   	//mmi_em_dev_pwm_cntx_struct * pwn_cntx = mmi_em_dev_pwm_cntx_p;

    
    mex_eng_gid = mmi_frm_group_create(GRP_ID_ROOT, 
                               GRP_ID_AUTO_GEN, 
                               mex_engine_proc, NULL);
    mmi_frm_group_enter(mex_eng_gid, MMI_FRM_NODE_SMART_CLOSE_FLAG);

    mex_eng_inline_id = cui_inline_create(mex_eng_gid, &g_mex_engine_inline_scrn);
    //set the init value of items
    mex_engine_set_default_values();
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_LOG,(U8*)"\xE5\x65\xD7\x5F\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_CHANNEL,(U8*)"\x20\x6E\x53\x90\xF7\x53\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_MODEL,(U8*)"\x3A\x67\x8B\x57\xF7\x53\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_SCR_WIDTH,(U8*)"\x4F\x5C\x55\x5E\xBD\x5B\xA6\x5E\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_SCR_HEIGHT,(U8*)"\x4F\x5C\x55\x5E\xD8\x9A\xA6\x5E\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_TOTAL_TIME,(U8*)"\xFA\x51\x27\x8D\xDF\x7E\xA1\x8B\xF6\x65\xF4\x95\x28\x0\x6\x52\x9F\x94\x29\x0\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_SUBMENU_UPDATE,(U8*)"\x8C\x4E\xA7\x7E\xDC\x83\x55\x53\x50\x96\x36\x52\x29\x59\x70\x65\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_ENTRY_NAME,(U8*)"\xEC\x72\xCB\x7A\x65\x51\xE3\x53\xB\x7A\x8F\x5E\xD\x54\0");
    cui_inline_set_item_caption(mex_eng_inline_id,MEX_ENG_CAPTION_ENTRY_NUM,(U8*)"\xEC\x72\xCB\x7A\x65\x51\xE3\x53\xB\x7A\x8F\x5E\x16\x7F\xF7\x53\0");
    cui_inline_set_item_displayonly(mex_eng_inline_id,MEX_ENG_CAPTION_TEST_ENTRY,(U8*)"\x4B\x6D\xD5\x8B\xEC\x72\xCB\x7A\x65\x51\xE3\x53\0",NULL);
    trigger_str[0] = GetString(STR_GLOBAL_OPEN); //打开
	trigger_str[1] = GetString(STR_GLOBAL_CLOSE);//关闭

    cui_inline_set_item_select(mex_eng_inline_id, MEX_ENG_STR_SELECT_LOG_TRIGGER, 2, (U8**)trigger_str, (U8)engmode.iLogOnOffHiliteState);

#ifdef MEX_QA_MODE
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_CHANNEL,(void *)&engmode.pVenderIDBuf);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_MODEL,(void *)&engmode.pPhoneUA);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_SCR_WIDTH,(void *)&engmode.pScreenWidth);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_SCR_HEIGHT,(void *)&engmode.pScreenHeight);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_TOTAL_TIME,(void *)&engmode.pSTTime);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_SUBMENU_UPDATE,(void *)&engmode.pEntrySecondLimit);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_STR_EDIT_ENTRY_NAME,(void *)&engmode.pMexAppName);
    cui_inline_set_value(mex_eng_inline_id, MEX_ENG_NUM_EDIT_ENTRY_NUM,(void *)&engmode.pMexAppID);
#endif

	cui_inline_run(mex_eng_inline_id);

}
#endif /*MTK VERSION >= 1112*/

#endif
/*这样做的原因是减少加载号码卫士两个库的代码，节省空间*/
#ifndef __MEX_AQ_SUPPORT__
int32 met_aq_GetIPPhone(const uint8 *tele_in, uint8 *tele_out, int32 tele_out_len)
{
	return 0;
}
int32 met_aq_SmsWall(uint16 num_len,uint8 *party_num)
{
	return 0;
}
int32 met_aq_PhoneWall(uint16 num_len,U8 *party_num)
{
	return 0;
}
void met_mex_aq_GetAttachedAreaNameAndReset(char* tempBuf)
{
	return;
}
int8 met_mex_aq_NumInq_App_Flag;
U8 met_mex_aq_strCityNameUnicode[20] ={0};
#endif
