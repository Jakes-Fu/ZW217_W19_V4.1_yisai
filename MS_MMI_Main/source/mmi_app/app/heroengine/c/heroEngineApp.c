#ifdef HERO_ENGINE_SUPPORT
#ifndef _MMI_HERO_APP_C_
#define _MMI_HERO_APP_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "Mn_type.h"
#include "mmisms_app.h"
#include "block_mem.h"
#include "mmipub.h"
#include "heroEngineExport.h"
#include "heroEngineInternal.h"
#include "heroEngineId.h"
#include "mmienvset_export.h"
#include "mmi_nv.h"  
#include "mmiudisk_export.h"
#include "heroAppApi.h" //modify 20181117

#if 1//def __HERO_ENGINE_OVERSEA__
#include "heroenginedata_eim.h"
#endif
#if defined(__HR_STONE_INROM__)||defined(__HR_SUDOKU_INROM__)||defined(__HR_DODGE_INROM__)
#include "heroenginegamedata.h"
#endif
#include "mmifmm_export.h"

#if defined(FIRM_HMGS_FULL_ROM)||defined(FIRM_HMGS_MIN_ROM)
#include "heroEnginedata_hmgs.h"
#endif
#ifdef HERO_APP_SMART_OPEN_INROM  //modify20181105
#include "heroSmart.h"
#endif
#ifdef HERO_ENGINE_FAMILY_INROM_SUPPORT
#include "tfamily.h"
#endif
#ifdef HERO_ENGINE_STVIDEO_SUPPORT_INROM
#include "heroStvideo.h"
#endif

#if defined(HERO_ENGINE_WSTORE_INROM_SUPPORT)&&defined(HERO_APP_WSTORE_OPEN)
#include "heroengineWstore.h"
#endif

#define FIRM_LIST_IND 3//"D", do not change
#define FIRM_NES3_IND 5//"F", do not change
#define FIRM_NES1_IND 6//"G", do not change
#define FIRM_NES2_IND 7//"H", do not change
#define FIRM_KER_IND  8//"I", do not change
#define FIRM_GML_IND  9//"J", do not change
#define FIRM_INDEX_BG 10//"K", do not change

/*
 “1”前台不显示wap
 “2”前台显示wap，
 “3”后台不显示wap，
 “4”后台显示wap
 SPR平台只支持模式“1”
 */
#define STARTUP_CLOCK_UI_MODE 1
char heroAppEntry[100] = {0}; 
#ifdef HERO_ENGINE_CLOCK_OPEN
char strHoFile[256];
#endif

hr_screen_angle_t g_hr_screen_angle = {0};


const GUIMENU_GROUP_T hero_menu_table[] = 
{
    {NULL},
};

LOCAL uint16 hero_nv_len[] =
{
	sizeof(HERO_APP_ENVINFO_T),
	sizeof(int8),
	NVRAM_HERO_APP_CACHE_SIZE, //modify 20181117
	sizeof(hero_smart_reminder_nv) //modify 20181120
#ifdef HERO_APP_SHORTCUT_SUPPORT
	,sizeof(heroShortcutMenuSt)
#endif
#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
	,NVRAM_OLALA_SLIM_CACHE_SIZE
#endif
};

const GUIMENU_GROUP_T hero_app_menu_table[] = 
{
    {NULL},
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN MMIHEROAPP_checkStorage(uint8 grade);
LOCAL void MMIHEROAPP_clearAllreg(void);

#if defined(__MMI_HERO_SUPPORT_SYSBRW__) && !defined(BROWSER_SUPPORT)
//如果要自定义网址的话，请按照调用下面的语句就可以实现:wap_start_browser(2, (PU8)L"http://www.baidu.com");
//char hr_browser_url[240] = "http://google.cn/";//BVH105026http://google.cn/
char hr_browser_url_asc[1024] = "http://www.baidu.com/";
static char hr_browser_url[240+1000] =
{
0x4D, 0x52, 0x50, 0x47, 0x01, 0x01, 0x00, 0x00, 0xAF, 0x04, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 
0x75, 0x72, 0x6C, 0x2E, 0x6D, 0x72, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x72, 0x6C, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x38, 0x64, 0x31, 0x33, 0x34, 0x61, 0x37, 0x65, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x12, 0x27, 0x00, 0x00, 0x56, 0x3F, 0x41, 0xAC, 0x73, 0x6B, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x75, 0x72, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static BOOLEAN hr_add_url(char* url)
{
	if(NULL == url || 0 == strlen(url) || ((strlen(url) + (5 + 1 + 1 + 1) + 240) > sizeof(hr_browser_url)))
		return FALSE;

	SCI_MEMSET(hr_browser_url+240,0,sizeof(hr_browser_url)-240);

	sprintf(hr_browser_url+240, "%s,%s;", "99999",url);

	return TRUE;
}

// url 需要asc编码
PUBLIC BOOLEAN wap_start_browser2(unsigned char option, const unsigned char *url)
{
	BOOLEAN recode = FALSE;
	MMIHR_Trace(("wap_start_browser2() url: %s", url));

	#if defined(HERO_ENGINE_OVERSEA_SUPPORT)
	MMIHEROAPP_setCurApp(HERO_APP_OSBROWSER);
	#else	
	MMIHEROAPP_setCurApp(HERO_APP_CHBROWSER);
	#endif
	//MMIHEROAPP_setCurApp(HERO_APP_BROWSER);
	MMIHEROAPP_clearAllreg();

#ifdef MMI_RES_ORIENT_BOTH
    g_hr_screen_angle.lcd_angle = LCD_ANGLE_0;
    g_hr_screen_angle.win_angle = WIN_SUPPORT_ANGLE_0;
    MMK_SetScreenAngle(LCD_ANGLE_0);
#endif

	MMITHEME_DestroyMenuItemCache();

	if(FALSE == MMIHEROAPP_checkStorage(0))
	{
		return FALSE;
	}

	switch (option)
	{
		case 2:// WAP_BROWSER_GOTO_URL:
		{
			#if !defined(WIN32)
			if(0 != url)
			{
				SCI_MEMSET(hr_browser_url_asc,0,sizeof(hr_browser_url_asc));
				//mmi_ucs2_to_asc(hr_browser_url_asc,(char*)url);
				strcpy(hr_browser_url_asc, (char*)url);
				hr_add_url(hr_browser_url_asc);
				hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
				hr_mainInstallRomExtern((uint8 *) hr_browser_url, sizeof(hr_browser_url));
			#if defined(HERO_ENGINE_OVERSEA_SUPPORT)
				recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tosbrw -i 920009 -n tosbrw.too", HERO_APP_OSBROWSER);
			#else
				recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tchbrw -i 920008 -n tchbrw.too", HERO_APP_CHBROWSER);
			#endif	
				//recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Browser -i 920009 -n tosbrw.too", HERO_APP_BROWSER);
				MMIHR_Trace(("wap_start_browser2,%s,%s",hr_browser_url_asc,hr_browser_url));
			}
			else
			{
				hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
			#if defined(HERO_ENGINE_OVERSEA_SUPPORT)
				recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tosbrw -i 920009 -n tosbrw.too", HERO_APP_OSBROWSER);
			#else
				recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tchbrw -i 920008 -n tchbrw.too", HERO_APP_CHBROWSER);
			#endif
				//recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Browser -i 920009 -n tosbrw.too", HERO_APP_BROWSER);
			}
			#endif
		}
	}
	return recode;
}

typedef struct AddBookmarkWinData
{
	char name[64];
	char url[256];
}BookMarkData_T;
// url 需要unicode编码
PUBLIC BOOLEAN wap_add_bookmark(const char* name, const char* url)
{
	BOOLEAN recode = FALSE;
	int ret;
	int fHandle;
	BookMarkData_T* pBookMarkData = (BookMarkData_T *)SCI_ALLOC(sizeof(BookMarkData_T));
	
	MMIHR_Trace(("wap_add_bookmark() name: %s, url: %s", name, url));
	do
	{
		if(NULL == pBookMarkData)
		{
			break;
		}
		SCI_MEMSET(pBookMarkData, 0, sizeof(BookMarkData_T));
		if(0 != name)
		{
			SCI_MEMCPY(pBookMarkData->name, name, 64);
		}
		if(0 != url)
		{
			SCI_MEMCPY(pBookMarkData->url, url, 256);
			hr_str_convert_endian((char*) pBookMarkData->url);
		}
		else
		{
			break;
		}
		//SFS_CreateDirectory(L"E:\\heroEngineSpr\\import");
		fHandle = SFS_CreateFile(L"E:\\heroEngineSpr\\import\\brwBK.dat", SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, PNULL, PNULL);
    	if(fHandle != 0)
        {
            SFS_SetFilePointer(fHandle, 0, SFS_SEEK_END);
            SFS_WriteFile(fHandle, (void*)pBookMarkData, sizeof(BookMarkData_T), &ret, PNULL);
            SFS_CloseFile(fHandle);
			fHandle = 0;
        } 
		//fHandle = hr_open("/import/brwBK.dat", HR_FILE_RDWR);
		//if(FFS_INVALID_HANDLE == fHandle)
		//{
		//	MMIHR_Trace(("wap_add_bookmark() FFS_INVALID_HANDLE == fHandle"));
		//	break;
		//}
		//ret = hr_seek(fHandle, 0, HR_SEEK_END);
		//MMIHR_Trace(("wap_add_bookmark() hr_seek ret = %d", ret));
		//ret = hr_write(fHandle, (void *)pBookMarkData, sizeof(BookMarkData_T));
		//MMIHR_Trace(("wap_add_bookmark() hr_write ret = %d", ret));
		recode = TRUE;
	}while(0);

	//if(FFS_INVALID_HANDLE != fHandle)
	//{
	//	hr_close(fHandle);
	//}
	if(NULL != pBookMarkData)
	{
		SCI_FREE(pBookMarkData);
	}
	return recode;
}

#endif//__MMI_HERO_SUPPORT_SYSBRW__

static int hr_get_smainm_title_buf(int StringId)
{
	MMI_STRING_T textStr = {0};
	static char gSmainmBuf[128] = {0};

	memset(gSmainmBuf, 0, sizeof(gSmainmBuf));

	MMI_GetLabelTextByLang(StringId, &textStr);
	if(textStr.wstr_ptr != NULL && textStr.wstr_len > 0)
	{
		GUI_WstrToUTF8(gSmainmBuf, sizeof(gSmainmBuf) - 1, (wchar *)textStr.wstr_ptr, textStr.wstr_len);
	}
	return (int)gSmainmBuf;
}

PUBLIC BOOLEAN MMIHEROAPP_AppEntry(HERO_APP_APPENTRY_E index)
{
	BOOLEAN recode = FALSE;
	static char gHeroAppEntryTempBuf[128] = {0};

	MMIHR_Trace(("MMIHEROAPP_AppEntry() index: %d", index));

	memset(gHeroAppEntryTempBuf, 0, sizeof(gHeroAppEntryTempBuf));

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
	{
		extern void hr_initFirstMenuId(void);
		hr_initFirstMenuId();
	}
#endif

#if 0//defined(MAINLCD_SUPPORT_480X800)
	MMIAPIMENU_CloseMainMenu();
#endif

	MMIHEROAPP_setCurApp(index);
	MMIHEROAPP_clearAllreg();

#ifdef MMI_RES_ORIENT_BOTH
    g_hr_screen_angle.lcd_angle = LCD_ANGLE_0;
    g_hr_screen_angle.win_angle = WIN_SUPPORT_ANGLE_0;
    MMK_SetScreenAngle(LCD_ANGLE_0);
#endif

	MMITHEME_DestroyMenuItemCache();

#if !defined(WIN32)
	switch(index)
	{
#ifdef HERO_APP_BROWSER_OPEN
	#if defined(HERO_ENGINE_OVERSEA_SUPPORT)
	case HERO_APP_OSBROWSER:
		if(MMIHEROAPP_checkStorage(0))
	{
	hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tosbrw -i 920009 -n tosbrw.too", index);	
	}
	#else
	case HERO_APP_CHBROWSER:
		if(MMIHEROAPP_checkStorage(0))
	{
	hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tchbrw -i 920008 -n tchbrw.too", index);	
	}
	#endif
    	break;
#endif

#ifdef HERO_APP_NEWS_OPEN
	case HERO_APP_NEWS:
    	if(MMIHEROAPP_checkStorage(0))
	{
	hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Tvnews -i 920037 -n tvnews.too", index);
	}
    	break;
#endif

#ifdef HERO_APP_SMART_OPEN
	case HERO_APP_SMART:
#ifdef HERO_APP_SMART_OPEN_INROM
	hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hr_smart, sizeof(hr_smart));
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Smart -i 920057 -n tsmart.too", index);	
#else
	if(MMIHEROAPP_checkStorage(0))
	{
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Smart -i 920057 -n tsmart.too", index);	
	}
#endif
    break;
#endif

#ifdef HERO_ENGINE_FAMILY_SUPPORT
	case HR_APPID_FAMILY:
	#ifdef HERO_ENGINE_FAMILY_INROM_SUPPORT
		hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)tfamily, sizeof(tfamily));
		recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Family -i 920300 -n tfamily.too -p 101", index);	
	#else
		if(MMIHEROAPP_checkStorage(0))
		{
			hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
			recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Family -i 920300 -n tfamily.too -p 101", index);	
		}
	#endif
		break;
#ifdef HERO_ENGINE_FAMILY_CONTROL_SUPPORT
	case HR_APPID_FAMILY_BGRUN:
	#ifdef HERO_ENGINE_FAMILY_INROM_SUPPORT
		hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)tfamily, sizeof(tfamily));
	#else
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	#endif
		recode = MMIHEROAPP_HrAppRun((void*)"-s 5 -t Family -i 920301 -n tfamily.too", index);	
		break;
#endif
#endif

#ifdef HERO_APP_SUDOKU_OPEN
	case HERO_APP_SUDOKU:
    	if(MMIHEROAPP_checkStorage(0))
	{
	#ifdef __HR_SUDOKU_INROM__
	hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hr_sudoku, sizeof(hr_sudoku));
	#else
	hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	#endif
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Sudoku -i 920016 -n tsudoku.too", index);	
	}
    	break;
#endif

#ifdef HERO_APP_STONE_OPEN
	case HERO_APP_STONE:
    	if(MMIHEROAPP_checkStorage(0))
	{
	#ifdef __HR_STONE_INROM__
	hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hr_stone, sizeof(hr_stone));
	#else
	hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	#endif
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Stone -i 920020 -n tstone.too", index);	
	}
    	break;
#endif

#ifdef HERO_APP_DODGE_OPEN
	case HERO_APP_TDODGE:
    	if(MMIHEROAPP_checkStorage(0))
	{
	#ifdef __HR_DODGE_INROM__
	hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hr_dodge, sizeof(hr_dodge));
	#else
	hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
	#endif
	recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Dodge -i 920034 -n tdodge.too", index);	
	}
    	break;
#endif


#ifdef HERO_APP_READER_OPEN
	case HERO_APP_TREADER:
		if(MMIHEROAPP_checkStorage(0))
		{
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Reader -i 920029 -n treader.too", index);	
		}
		break;
#endif

#ifdef HERO_APP_WEATHER_OPEN
	case HERO_APP_TWEATHER:
			if(MMIHEROAPP_checkStorage(0))
		{
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t Weather -i 920044 -n tweath.too -p 1", index);	
		}
		break;
#endif

#ifdef HERO_APP_TIME_OPEN
	case HERO_APP_TIME:
			if(MMIHEROAPP_checkStorage(0))
		{
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t smainm -i 400002 -n smainm.too -p 1", index);	
		}
		break;
#endif

#ifdef HERO_ENGINE_HMGS_SUPPORT
	case HERO_APP_HMGS:
		if(MMIHEROAPP_checkStorage(0))
		{
		#if defined(FIRM_HMGS_FULL_ROM)||defined(FIRM_HMGS_MIN_ROM)
		hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hr_hmgs, sizeof(hr_hmgs));
		#else
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		#endif
		sprintf(gHeroAppEntryTempBuf, "-s 0 -h %d -i 920008 -n tcall5.too", hr_get_smainm_title_buf(TXT_HERO_APP_HMGS));
		recode = MMIHEROAPP_HrAppRun((void*)gHeroAppEntryTempBuf, index); 
		}
		break;
#endif

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
	case HERO_APP_TTS:
		//if(MMIHEROAPP_checkStorage(0))
		{
			hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
			recode = MMIHEROAPP_HrAppRun((void*)"-s 4 -t smainm -i 400002 -n smainm.too", index); 
		}
		break;
#endif

#ifdef HERO_APP_SIMUGAME_OPEN
	case HERO_APP_NES:
		#ifndef HERO_APP_SIMUGAME_FIRMED
		if(MMIHEROAPP_checkStorage(0))
		#endif
		{
		#ifdef HERO_APP_SIMUGAME_FIRMED
			hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hero_nes, sizeof(hero_nes));
		#else
			hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		#endif
			recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t nes -i 813105 -n nes.too", index); 
		}
		break;
#endif
#ifdef HERO_ENGINE_FM_SUPPORT
	case HERO_APP_FM:
			if(MMIHEROAPP_checkStorage(0))
		{
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		recode = MMIHEROAPP_HrAppRun((void*)"-s 0 -t tfm -i 920032 -n tfm.too -p 1", index);	
		}
		break;
#endif
#ifdef HERO_ENGINE_STVIDEO_SUPPORT
	case HERO_APP_STVIDEO:
#if !defined(HERO_ENGINE_STVIDEO_SUPPORT_INROM)
			if(MMIHEROAPP_checkStorage(0))
#endif				
		{
		#ifdef HERO_ENGINE_STVIDEO_SUPPORT_INROM
			hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)hero_stvideo, sizeof(hero_stvideo));
		#else
		hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));	
		#endif
	       sprintf(gHeroAppEntryTempBuf, "-s 0 -h %d -i 920309 -n stvideo.too", hr_get_smainm_title_buf(TXT_HERO_APP_STVIDEO));
	       recode = MMIHEROAPP_HrAppRun((void*)gHeroAppEntryTempBuf, index); 	
		}
		break;
#endif

        #if defined(HERO_APP_WSTORE_OPEN)//wuxx add 20230914
	case HERO_APP_WSTORE:
		#if !defined(HERO_ENGINE_WSTORE_INROM_SUPPORT)
		if(MMIHEROAPP_checkStorage(0))//内置应用不需要判断T卡
		#endif
		{
		#ifdef HERO_ENGINE_WSTORE_INROM_SUPPORT
			hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)wstore, sizeof(wstore));
		#else
			hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		#endif
			sprintf(gHeroAppEntryTempBuf, "-s 0 -t Store -i 920226 -n twstore.too -r 0", hr_get_smainm_title_buf(TXT_HERO_APP_WSTORE));
			recode = MMIHEROAPP_HrAppRun((void*)gHeroAppEntryTempBuf, index); 	
		}
		break;
	case HERO_APP_WSTORE_MANAGE:
		#if !defined(HERO_ENGINE_WSTORE_INROM_SUPPORT)
		if(MMIHEROAPP_checkStorage(0))//内置应用不需要判断T卡
		#endif
		{
		#ifdef HERO_ENGINE_WSTORE_INROM_SUPPORT
			hr_mainInstallRomFull((uint8 *)hr_tmainm, sizeof(hr_tmainm), (uint8 *)wstore, sizeof(wstore));
		#else
			hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
		#endif
			sprintf(gHeroAppEntryTempBuf, "-s 0 -t Store -i 920226 -n twstore.too -r 1", hr_get_smainm_title_buf(TXT_HERO_APP_WSTORE_MANAGE));
			recode = MMIHEROAPP_HrAppRun((void*)gHeroAppEntryTempBuf, index); 	
		}
		break;	
        #endif//wuxx add 20230914
#endif

    default:
        MMIHR_Trace(("HERO invalid entry index = %d", index));
        break;
	}
#endif
	return recode;
}


LOCAL BOOLEAN MMIHEROAPP_checkStorage(uint8 grade)
{
	int plmnStatus = 0;
    //判断sim卡
	if((0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
       #if defined(FIRM_HMGS_FULL_ROM)||defined(FIRM_HMGS_MIN_ROM)
			  && (MMIHEROAPP_GetCurApp()!=HERO_APP_HMGS)
       #endif
		) 
    {
    		MMIPUB_OpenAlertWinByTextId(NULL,TXT_SMS_NO_SIM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);	
		return FALSE;
    }

	if(!MMIAPIPDP_PsIsPdpLinkExist())
	{
		MMIPUB_OpenAlertWinByTextId(NULL,TXT_SMS_NO_SIM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);	
		return FALSE;
	}

	plmnStatus = MMIAPIPHONE_GetPlmnStatus(MMIAPISET_GetActiveSim());
	 if(MMIAPISET_GetFlyMode() || !MMIPHONE_IsSimOk(MMIAPISET_GetActiveSim()) || (PLMN_NO_SERVICE == plmnStatus) || (PLMN_EMERGENCY_ONLY == plmnStatus) || (PLMN_EMERGENCY_SIM_INVALID == plmnStatus))
	 {
	 	MMIPUB_OpenAlertWinByTextId(NULL,TXT_SMS_NO_SIM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);  
	  	return FALSE;
	 }
	   
//    if ((TRUE != MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_UDISK, HERO_MMIFILE_DEVICE_UDISK_LEN))
//        && (TRUE != MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD, HERO_MMIFILE_DEVICE_SDCARD_LEN))
//#ifdef DUAL_TCARD_SUPPORT
//        && (TRUE != MMIAPIFMM_GetDeviceStatus(HERO_MMIFILE_DEVICE_SDCARD2, HERO_MMIFILE_DEVICE_SDCARD2_LEN))
//#endif
//        )
    if(MMIAPIUDISK_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)) != FALSE)    
	{		
        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
			MMIPUB_OpenAlertWinByTextId(NULL,TXT_NO_SD_CARD_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);
        }
		return FALSE;
	}
    else
    {
        return TRUE;
	}
}

PUBLIC int32 MMIHEROAPP_saveMrApp(uint8 *data,uint32 len)
{
	return MMIHEROAPP_savePack(data, len);
}


PUBLIC int32 MMIHEROAPP_hrSMSfilter(MN_SMS_ALPHABET_TYPE_E  alphabet_type, MN_SMS_USER_VALID_DATA_T *data, MN_CALLED_NUMBER_T *number)
{
	int32 result, recode = 1;
	
	result = MMIHEROAPP_SMSfilter(alphabet_type, data, number);

	if(result == HR_SUCCESS)
		recode = 0;
	else if(result == HR_FAILED)
		recode = -1;
	else
		recode = 1; 

	return recode;
}


PUBLIC void MMIHEROAPP_initModule(void)
{
	MMI_RegMenuGroup(MMI_MODULE_HEROENGINE, hero_menu_table);
	MMI_RegModuleNv(MMI_MODULE_HEROENGINE, hero_nv_len, sizeof(hero_nv_len) / sizeof(uint16));
	MMI_RegMenuGroup(MMI_MODULE_HEROAPP, hero_app_menu_table);
}

PUBLIC BOOLEAN MMIHEROAPP_closeHeroApp(void)
{
	MMIHR_Trace(("MMIHEROAPP_closeHeroApp(), %d", g_hr_vmBackRun));
#ifdef HR_BINDTO_VIRTUAL_WIN
	if(g_hr_vmBackRun != HERO_APP_MAX)
	{
		MMIHEROAPP_exitMrApp(FALSE);
	}
	else
#endif
	if(MMK_IsOpenWin(MMI_HERO_APP_BASE_WIN_ID))
	{
		MMIHEROAPP_setAllReady(FALSE);
		if(MMIHEROAPP_closeAllWins())
		{
			return TRUE;
		}
	}


    MMIHEROAPP_StopStatusIconTimer();
    #if !defined(PLATFORM_UIX8910)
    MAIN_SetIdleHeroappState(FALSE); /*lint !e718 !e18*/
    #endif
		
	return FALSE;
}
PUBLIC BOOLEAN MMIHEROAPP_appIsActived(void)
{
	if(s_hr_allReady
#ifdef HR_BINDTO_VIRTUAL_WIN
		|| (g_hr_vmBackRun != HERO_APP_MAX)
#endif
		)
	{
		return TRUE;
	}

	return FALSE;
}

PUBLIC void MMIHEROAPP_setPhoneState(e_hero_phone_mod mod, int32 state)
{
	if(mod < HERO_APP_MD_MAXNUM)
	{
		g_hr_phoneModState[mod] = state;
	}
}


PUBLIC BOOLEAN  MMIHEROAPP_InitSetting(void)
{
	MMIHEROAPP_initApn();

#ifdef HERO_ENGINE_SUPPORT
	{
		static char heroNetworkIsInit = 0;
		if(!heroNetworkIsInit)
		{  
		#if !defined(WIN32)
			extern void hero_app_network_init();
			hero_app_network_init();
			heroNetworkIsInit = 1;
		#endif
		}
	}   
#endif        
	return TRUE;
}

PUBLIC BOOLEAN MMIHEROAPP_activeMrapp2Front(HERO_APP_APPENTRY_E app, void *param)
{
	BOOLEAN ret = FALSE;

    MMIHR_Trace(("HERO MMIHEROAPP_activeMrapp2Front app = %d", app));
	if(MMIHEROAPP_appIsActived())
	{
		ret = MMIHEROAPP_HrAppRun(NULL, app);

        /*应用由后台激活到前台时去除状态栏图标*/
        if(TRUE == ret)
        {
		#if !defined(PLATFORM_UIX8910)
            MAIN_SetIdleHeroappState(FALSE); /*lint !e718 !e18*/
		#endif
        }

    }
	else
	{
		ret = MMIHEROAPP_AppEntry(app);
	}
	
	return ret;
}

LOCAL void MMIHEROAPP_clearAllreg(void)
{
	#if !defined(WIN32)
	hr_mainInstallRomExtern(0,0);
	hr_mainInstallRomFull(0,0,0,0);
	#endif
}

PUBLIC BOOLEAN MMIHEROAPP_ProcessShow(void)
{
	MMI_STRING_T  s_text = {0};
	BOOLEAN ret = TRUE;
#ifdef __HERO_CFG_FEATURE_HANDSETID_EX__
	uint8   temp_buf1[64] = {0};
	uint16  temp_buf2[64] = {0};
	uint8  temp_bufin[34] = {0};
#else
	uint8   temp_buf1[32] = {0};
    uint16  temp_buf2[32] = {0};
#endif
	uint32 temp_time=15000;
#ifdef __HERO_CFG_FEATURE_HANDSETID_EX__
	hr_setting_get_handsetID_ex(temp_bufin,34,NULL,NULL,NULL);
	MMIHR_Trace(("MMIHEROAPP_ProcessShow() manu:%s handset:%s", HERO_ENGINE_MANUFACTORY, temp_bufin));
#else
	MMIHR_Trace(("MMIHEROAPP_ProcessShow() manu:%s handset:%s", HERO_ENGINE_MANUFACTORY, HERO_ENGINE_HANDSET));
#endif
	SCI_MEMSET(temp_buf1, 0, sizeof(temp_buf1));
	SCI_MEMSET(temp_buf2, 0, sizeof(temp_buf2));
#ifdef __HERO_CFG_FEATURE_HANDSETID_EX__
	sprintf((char*)temp_buf1, "%s  %s", HERO_ENGINE_MANUFACTORY, temp_bufin);
#else
    sprintf((char*)temp_buf1, "%s  %s", HERO_ENGINE_MANUFACTORY, HERO_ENGINE_HANDSET);
#endif
	s_text.wstr_len = MMIHEROAPP_GB2UCS(temp_buf2, temp_buf1, strlen(temp_buf1)); /*lint !e64*/
	s_text.wstr_ptr = temp_buf2;
	
	MMIPUB_OpenAlertWinByTextPtr(&temp_time,&s_text,NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);	
	return ret;
}

// 20190430
PUBLIC BOOLEAN MMIHEROAPP_DsmVerShow(void)
{
	MMI_STRING_T  s_text = {0};
	BOOLEAN ret = TRUE;
	uint8   temp_buf1[32] = {0};
    uint16  temp_buf2[32] = {0};
	uint32 temp_time=15000;
	//extern int hero_app_get_vm_plat_code();

	MMIHR_Trace(("MMIHEROAPP_DsmVerShow() hero_get_dsm_app_version:%d", hero_get_dsm_app_version()));
	MMIHR_Trace(("MMIHEROAPP_DsmVerShow() hero_app_get_vm_plat_code:%d", hero_app_get_vm_plat_code()));
	SCI_MEMSET(temp_buf1, 0, sizeof(temp_buf1));
	SCI_MEMSET(temp_buf2, 0, sizeof(temp_buf2));
    sprintf((char*)temp_buf1, "DSM:%d,VM:%d", hero_get_dsm_app_version(), hero_app_get_vm_plat_code());
	s_text.wstr_len = MMIHEROAPP_GB2UCS(temp_buf2, temp_buf1, strlen(temp_buf1)); /*lint !e64*/
	s_text.wstr_ptr = temp_buf2;

	MMIPUB_OpenAlertWinByTextPtr(&temp_time,&s_text,NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);	
	return ret;
}

PUBLIC BOOLEAN MMIHEROAPP_ProcessEntrStr(
									   uint8* str_ptr, //[IN] input string pointer
									   uint16 str_len  //[IN] input string length
									   )
{ 
	BOOLEAN recode = FALSE;
	MMIHR_Trace(("MMIHEROAPP_ProcessEntrStr(), %d, %s", str_len, str_ptr));

	if(strncmp((char*)str_ptr, "*#110807#", str_len) == 0
       && str_len == strlen("*#110807#"))
	{
	    MMIHEROAPP_setCurApp(HERO_ENGINE_TEST);
		recode = MMIHEROAPP_HrAppRun((void*)"%hero_gm.hoo", HERO_ENGINE_TEST);	
	}
	else if(strncmp((char*)str_ptr, "*#551188#", str_len) == 0
             && str_len == strlen("*#551188#"))
	{  
		//MMIHEROAPP_ProcessShow();
		MMIHERO_ShowFactoryInfoWin();
		recode = TRUE;
	}
	else if(strncmp((char*)str_ptr, "*#550088#", str_len) == 0
		  && str_len == strlen("*#550088#"))
	{	
		MMIHEROAPP_DsmVerShow();
		recode = TRUE;
	}

	return (recode);
}
PUBLIC BOOLEAN MMIHEROAPP_BLMemCb(BLOCK_MEM_ID_E block_id, BLOCK_MEM_RELATION_STATUS_E relation_info)
{
    MMIHR_Trace(("HERO MMIHEROAPP_BLMemCb block_id = %d relation_info = %d", block_id, relation_info));
	if(BORROW_FROM_CURRENT_BLOCK == relation_info)
    {
		MMIHEROAPP_closeHeroApp();
		MMIHR_Trace(("HERO close END"));
	}
	 
	return TRUE;
}

PUBLIC BOOLEAN MMIHEROAPP_activeMrapp(void)
{
    MMIHR_Trace(("HERO MMIHEROAPP_activeMrapp g_hr_vmBackRun = %d", g_hr_vmBackRun));
	return MMIHEROAPP_activeMrapp2Front(g_hr_vmBackRun, NULL);
}
	
PUBLIC HERO_APP_APPENTRY_E MMIHEROAPP_GetCurHungApp(void)
{
	MMIHR_Trace(("MMIHEROAPP_GetCurHungApp, %d", g_hr_vmBackRun));
	return (HERO_APP_APPENTRY_E)g_hr_vmBackRun;
}


#ifdef __HR_CFG_SHORTCUT_SUPPORT__
void hr_scut_start_app(void* param1, void *param2)
{
    int32 appId = *(int32 *)param1;
    uint32 menuId = *(uint32 *)param2;
    BOOLEAN ret = FALSE;
    HR_SHORTCUT_APPINFO_T appInfo = {0};

    MMIHEROAPP_setDefFilePath();

    if(HR_SUCCESS == hr_scut_read_data(appId, &appInfo))  
    {
        hr_scut_refresh_icon(menuId, appInfo);
    
        SCI_MEMSET(heroAppEntry, 0x00, sizeof(heroAppEntry));
        sprintf(heroAppEntry, "*J,%s", appInfo.start_data);

        ret = MMIHEROAPP_HrAppRun((void*)heroAppEntry, appId);
    }

    MMIHR_Trace(("[HERO] hr_scut_start_app appId = %u heroAppEntry = %s ret = %d", appId, heroAppEntry, ret));
}
#endif

void hero_get_install_app_list(char *outBuf, int outBufLen)
{
	int pos = 0;

	if(outBuf == NULL)
	{
		return;
	}
	memset(outBuf, 0, outBufLen);

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
		{
			pos += sprintf(outBuf + pos, "%d,", HR_TTS_MENU_ENTRY);
			if(pos + 10 >= outBufLen)
			{
				return;
			}
		}
#endif

#if defined(HEROENGINE_TTS_SUPPORT)
	{
		pos += sprintf(outBuf + pos, "%d,", HR_SIMPLE_TTS_ENTRY);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_BROWSER_OPEN
	#if defined(HERO_ENGINE_OVERSEA_SUPPORT)
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_OSBROWSER);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
	#else
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_CHBROWSER);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
	#endif
#endif

#ifdef HERO_APP_NEWS_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_NEWS);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_SMART_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_SMART);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_ENGINE_FAMILY_SUPPORT
	{
		pos += sprintf(outBuf + pos, "%d,", HR_APPID_FAMILY);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#ifdef HERO_ENGINE_FAMILY_CONTROL_SUPPORT
	{
		pos += sprintf(outBuf + pos, "%d,", HR_APPID_FAMILY_BGRUN);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif
#endif

#ifdef HERO_APP_SUDOKU_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_SUDOKU);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_STONE_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_STONE);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_DODGE_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_TDODGE);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif


#if  1//def HERO_APP_READER_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_TREADER);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_WEATHER_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_TWEATHER);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_TIME_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_TIME);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_APP_WSTORE_OPEN
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_WSTORE);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_ENGINE_HMGS_SUPPORT
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_HMGS);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif
#ifdef HERO_ENGINE_FM_SUPPORT
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_FM);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif

#ifdef HERO_ENGINE_STVIDEO_SUPPORT
	{
		pos += sprintf(outBuf + pos, "%d,", HERO_APP_STVIDEO);
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
#endif
	if(hero_get_dsm_app_version() >= 1025)
	{
		if(pos + 35 >= outBufLen)
		{
			return;
		}
		pos += sprintf(outBuf + pos, "%s,", "560001,560002,560015,560016,560017");
		if(pos + 10 >= outBufLen)
		{
			return;
		}
	}
}

static char gHeroAppEntryBuf[128] = {0};
void hero_app_open_app(int appid, char *entry)
{
	if(entry != NULL && strlen(entry) > 0)
	{
		memset(gHeroAppEntryBuf, 0, sizeof(gHeroAppEntryBuf));
		strncpy(gHeroAppEntryBuf, entry, sizeof(gHeroAppEntryBuf) - 1);

		if(MMIHEROAPP_GetVMState() != VM_STOP)
		{
			return;
		}
		MMIHEROAPP_clearAllreg();
		MMITHEME_DestroyMenuItemCache();

		MMIHEROAPP_setCurApp(appid);
		if(MMIHEROAPP_checkStorage(0))
		{
			hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
			MMIHEROAPP_HrAppRun((void*)gHeroAppEntryBuf, appid);
		}
	}
}

#ifdef HERO_APP_SHORTCUT_SUPPORT
#include "mmi_menutable.h"

static heroShortcutMenuSt gHeroShortcutMenuData = {0};
static int hr_read_shortcut_data(heroShortcutMenuSt *pMenuData)
{
    MN_RETURN_RESULT_E return_value = 0;

	if(pMenuData == NULL)
	{
		return HR_FAILED;
	}

    MMINV_READ(NVRAM_EF_SHORTCUT_MENU_LID, pMenuData, return_value);   
	if(return_value != MN_RETURN_SUCCESS)
	{
		return HR_FAILED;
	}

	if(pMenuData->magic != HR_SHORCUT_MAGIC)
	{
		memset(pMenuData, 0, sizeof(heroShortcutMenuSt));
		pMenuData->magic = HR_SHORCUT_MAGIC;
		return hr_write_shortcut_data(pMenuData);
	}
	return HR_SUCCESS;
}

static int hr_write_shortcut_data(heroShortcutMenuSt *pMenuData)
{
	if(pMenuData == NULL)
	{
		return HR_FAILED;
	}
	pMenuData->magic = HR_SHORCUT_MAGIC;
	MMINV_WRITE(NVRAM_EF_SHORTCUT_MENU_LID, pMenuData);
	memcpy(&gHeroShortcutMenuData, pMenuData, sizeof(heroShortcutMenuSt));
	return HR_SUCCESS;
}

static heroShortcutMenuSt *hero_get_shortcut_menu()
{
	heroShortcutMenuSt tempMenuData = {0};
	if(gHeroShortcutMenuData.magic != HR_SHORCUT_MAGIC)
	{
		if(hr_read_shortcut_data(&tempMenuData) == HR_SUCCESS)
		{
			memcpy(&gHeroShortcutMenuData, &tempMenuData, sizeof(heroShortcutMenuSt));
		}
	}
	return &gHeroShortcutMenuData;
}

void hero_init_shortcut_menu(MMI_CTRL_ID_T ctrl_id)
{
	int index = 0;
	MMI_MENU_ID_T menu_id = 0;
	MMI_MENU_GROUP_ID_T group_id = 0;
	heroShortcutMenuSt *pMenu = hero_get_shortcut_menu();

	GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
	for(index = 0; index < HERO_SHORTCUT_MENU_COUNT; index++)
	{
		heroShortcutMenuItemSt *pMenuItem = &(pMenu->item[index]);
		if(pMenuItem->isShow && pMenuItem->appid != 0 && strlen(pMenuItem->entry) > 0 && MMIAPICOM_Wstrlen(pMenuItem->menuName) > 0)
		{
			MMI_STRING_T stk_str = {0};
			char tempNameBuf[64] = {0};

			GUIMENU_SetItemVisible(ctrl_id, group_id, ID_HERO_APP_EMPTY_1 + index, TRUE);

			// str menu name
			memcpy(tempNameBuf, pMenuItem->menuName, MIN(sizeof(pMenuItem->menuName), sizeof(tempNameBuf)));
			hr_str_convert_endian(tempNameBuf);
			stk_str.wstr_ptr = (wchar*)tempNameBuf;
			stk_str.wstr_len=MMIAPICOM_Wstrlen(stk_str.wstr_ptr);
			GUIMENU_SetStaticItem(MAINMENUWIN_SECOND_MENU_CTRL_ID, group_id, ID_HERO_APP_EMPTY_1 + index, &stk_str, IMAGE_NULL);
		}
		else
		{
			GUIMENU_SetItemVisible(ctrl_id, group_id, ID_HERO_APP_EMPTY_1 + index, FALSE);
		}
	}
}

void hero_shortcut_menu_event(MMI_MENU_ID_T menu_id)
{
	if(menu_id >= ID_HERO_APP_EMPTY_1 && menu_id < ID_HERO_APP_EMPTY_1 + HERO_SHORTCUT_MENU_COUNT)
	{
		int itemIndex = 0;
		heroShortcutMenuItemSt *pMenuItem = NULL;
		heroShortcutMenuSt *pMenu = hero_get_shortcut_menu();

		pMenuItem = &(pMenu->item[menu_id - ID_HERO_APP_EMPTY_1]);
		if(pMenuItem->appid != 0 && strlen(pMenuItem->entry) > 0 && MMIAPICOM_Wstrlen(pMenuItem->menuName) > 0)
		{
			MMIHEROAPP_clearAllreg();
			MMITHEME_DestroyMenuItemCache();

			MMIHEROAPP_setCurApp(pMenuItem->appid);
			if(MMIHEROAPP_checkStorage(0))
			{
				hr_mainInstallRom((uint8 *)hr_tmainm, sizeof(hr_tmainm));
				MMIHEROAPP_HrAppRun((void*)pMenuItem->entry, pMenuItem->appid);	
			}
		}
	}
}

int32 hr_get_shortcut_data_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	heroShortcutMenuSt *menuData = hero_get_shortcut_menu();
	if(output == NULL || output_len == NULL)
	{
		return HR_FAILED;
	}
	*output = (uint8*)menuData;
	*output_len = sizeof(heroShortcutMenuSt);
	return HR_SUCCESS;
}

int32 hr_set_shortcut_data_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	heroShortcutMenuSt *menuData = (heroShortcutMenuSt*)input;
	if(input == NULL || input_len != sizeof(heroShortcutMenuSt))
	{
		return HR_FAILED;
	}
	return hr_write_shortcut_data(menuData);
}
#endif

#ifdef HERO_APP_WSTORE_OPEN
PUBLIC void HeroWstore_AppEntry(void)
{
	MMIHEROAPP_AppEntry(HERO_APP_WSTORE);
}
PUBLIC void HeroWstoreManage_AppEntry(void)
{
	MMIHEROAPP_AppEntry(HERO_APP_WSTORE_MANAGE);
}

#endif

#endif

