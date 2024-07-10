/******************************************************************************

                  版权所有 (C), 2001-2011, 扬讯科技

 ******************************************************************************
  文 件 名   : MexMenuCtrl.c
  版 本 号   : 1.0
  作    者   : zhangguangsheng
  生成日期   : 2011年12月28日
  最近修改   :
  功能描述   : 针对mex独立入口菜单隐藏和二级菜单控制操作
  函数列表   :
  修改历史   :
  1.日    期   : 2011年12月28日
    作    者   : zhangguangsheng
    修改内容   : 创建文件

******************************************************************************/
#ifdef __MET_MEX_SUPPORT__

#include "MMIDataType.h"
#include "mexlib.h"
#include "GlobalConstants.h"
#include "fs_type.h"
#if (MEX_MTK_CODE_VERSION == 0x1012)
#include "Mex_res.h"
#endif


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
 
#if (MEX_MTK_CODE_VERSION >= 0x1112)
	
#include "mmi_rp_native_app_package_data.h"
#else
#include "mmi_rp_app_metmex_def.h"		//36_10A

static U16 g_mex_nitems = 0;
#endif
#define MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU	31
#define MAX_MEX_APP_MENU_ITEMS_FOR_HIDEMENU		(MAX_SUB_MENUS - 10)
U16 Mex_App_MENUID_List[MAX_MEX_APP_MENU_ITEMS_FOR_HIDEMENU] = {0};

#define mex_app_size (sizeof(g_mex_menu_ctrl)/sizeof(app_control))
#define mex_app_res_size (sizeof(Mex_App_List)/sizeof(mex_app_res))



typedef struct  
{
	S8* 	app_name;
	BOOL 	is_hiden;
	BOOL 	is_second_menu_hiden;
}app_control;





static app_control g_mex_menu_ctrl[] = 
{
	/*客户要求加的应用{{{*/
	#ifdef __MEX_QQ_SUPPORT__
	{"qq", false, true },
	#endif	
	#ifdef __MEX_MSN_SUPPORT__
	{"msn", false, true },	
	#endif
	/*客户要求加的应用}}}*/
	{"angrybirds", true, true },
	{"ddz", true, true },
	{"shsl", true, true },
	{"fruit", true, true },
	{"zwzjs", true, true },
	{"cqz", true, true },
	//{"csjjs", true, true },
	{"rocket", true, true },
	//{"TalkingCat", true, true },
	//{"mxsqy", true, true },	
	{"dzt", true, true },	

};

#if (MEX_MTK_CODE_VERSION >= 0x1112)
	
	typedef struct
	{
		char mex_name[20];
		char packet_name[30];
	}mex_app2native_packet_name;
typedef struct  
{
	S8* app_name;
}mex_app_res;

	
#define mex_app2packet_size (sizeof(g_mex_app2packet)/sizeof(mex_app2native_packet_name))

static mex_app_res Mex_App_List[] = 
{
	/*客户要求加的应用{{{*/
	#ifdef __MEX_QQ_SUPPORT__
	{"qq"},
	#endif
	#ifdef __MEX_MSN_SUPPORT__
	{"msn"},
	#endif	
	/*客户要求加的应用}}}*/
	{"angrybirds"},
	{"ddz"},
	{"shsl"},
	{"fruit"},
	{"zwzjs"},
	{"cqz"},
	{"rocket"},
	{"dzt"},
	
};	

mex_app2native_packet_name g_mex_app2packet[] =
{
	{"mex_qq",		"native.mtk.qq"},
	{"LighterHD",	"native.mtk.lighter"},
	{"msn",			"native.mtk.msn"},
	{"fetion",		"native.mtk.fetion"},
	{"AngryBirds",	"native.mtk.abird"},
	{"appgetter",	"native.mtk.mex_merig3g"},
	{"HDcqz",		"native.mtk.HDcqz"},
	{"dzbk",		"native.mtk.DMC"},
	{"settime",		"native.mtk.SETTIME"},
	{"jpfckb",		"native.mtk.JPFCKB"},
	{"AreaInquire",	"native.mtk.aq"	},
        {"fruit",       "native.mtk.qsg"},
        {"cqz",     "native.mtk.cqz"},
        {"tom",     "native.mtk.tommao"},
        {"zwzjs",       "native.mtk.zwdzjs"},
        {"ddz",     "native.mtk.ddz"},
        {"shsl",        "native.mtk.shsl"},
        {"csjjs",      "native.mtk.csjjs"},
        {"rocket",      "native.mtk.blfc"},
        {"mxsqy",       "native.mtk.mxsqy"},   
        {"dzt",       "native.mtk.dzt"},  
};
#else
typedef struct  
{
	S8* app_name;
	U16 met_mex_menu_id;
	U16 met_mex_menu_string;
	U16 met_mex_menu_icon;
}mex_app_res;

static mex_app_res Mex_App_List[] = 
{
	/*客户要求加的应用{{{*/
	#ifdef __MEX_QQ_SUPPORT__
	{"qq", MET_MEX_QQ_MENUID, E_MSI_QQ_MENU_STRING,E_MII_MEX_QQ_ICON },
	#endif
	#ifdef __MEX_MSN_SUPPORT__
	{"msn", MET_MEX_MSN_MENUID, E_MSI_MSN_MENU_STRING,E_MII_MEX_MSN_ICON },
	#endif	
	/*客户要求加的应用}}}*/
	{"angrybirds", MET_MEX_FNDXN_MENUID, E_MSI_FNDXN_MENU_STRING,E_MII_MEX_FNDXN_ICON },
	{"ddz", MET_MEX_DDZ_MENUID, E_MSI_DDZ_MENU_STRING,E_MII_MEX_DDZ_ICON },
	{"shsl", MET_MEX_SHSL_MENUID, E_MSI_SHSL_MENU_STRING,E_MII_MEX_SHSL_ICON },
	{"fruit", MET_MEX_QSG_MENUID, E_MSI_QSG_MENU_STRING,E_MII_MEX_QSG_ICON},
	{"zwzjs", MET_MEX_ZWDZJS_MENUID, E_MSI_ZWDZJS_MENU_STRING,E_MII_MEX_ZWDZJS_ICON},
	{"cqz", MET_MEX_CQZ_MENUID, E_MSI_CQZ_MENU_STRING, E_MII_MEX_CQZ_ICON},
	//{"csjjs", MET_MEX_CSJJS_MENUID, E_MSI_CSJJS_MENU_STRING, E_MII_MEX_CSJJS_ICON},
	{"rocket", MET_MEX_BLFC_MENUID, E_MSI_BLFC_MENU_STRING, E_MII_MEX_BLFC_ICON},
	//{"TalkingCat", MET_MEX_TOMMAO_MENUID, E_MSI_HDTOMMAO_MENU_STRING, E_MII_MEX_TOMMAO_ICON},
	//{"mxsqy", MET_MEX_MXSQY_MENUID, E_MSI_MXSQY_MENU_STRING, E_MII_MEX_MXSQY_ICON},
	{"dzt", MET_MEX_DZT_MENUID, E_MSI_DZT_MENU_STRING, E_MII_MEX_DZT_ICON},
	
};	

/*---------------------------END-----------------------------------*/

#endif /*(MEX_MTK_CODE_VERSION >= 0x1112)*/

void mex_parse_hiden_config_file(void)
{
	FS_HANDLE pFile;
	UINT filesize = 0;
	wchar_t cfgFileName[100] = { 0 };
	char content[300] = {0};
	uint32 readed_size = 0;
	char *strsearch = NULL;
	char search_targ[MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU] = {0};
	int i =0;
	extern int32 MexConfig_GetPhoneConfig(E_Mex_Phone_Config cfg);

	/*********try to get the file from T-CARD*********/
	mex_wcscpy( cfgFileName, (const wchar_t*)MexConfig_GetPhoneConfig(E_MPC_SystemRootDir ) );
	mex_wcscat( cfgFileName, L"\\MexSys.cfg" );
	pFile = FS_Open( cfgFileName, FS_READ_ONLY );
	
	if(pFile < 0 )
	{
		mex_log(MEX_LOG_SYSTEM," parse_hiden_config_file open file err code:%d",pFile);
		
        if(!(bool) MexConfig_GetPhoneConfig(E_MPC_Usb_MassStorage))
        {
            for( i = 0; i < mex_app_size; i++)
        	{
                g_mex_menu_ctrl[i].is_hiden = TRUE;

				/*客户要求加的应用{{{*/
				#if (MEX_MTK_CODE_VERSION < 0x1112)
				#ifdef __MEX_QQ_SUPPORT__
				if( MET_MEX_QQ_MENUID == Mex_App_List[i].met_mex_menu_id )
				{
					g_mex_menu_ctrl[i].is_hiden = FALSE;
				}
				#endif
				
				#ifdef __MEX_MSN_SUPPORT__
				if( MET_MEX_MSN_MENUID == Mex_App_List[i].met_mex_menu_id )
				{
					g_mex_menu_ctrl[i].is_hiden = FALSE;
				}
				#endif	
				#endif
				/*客户要求加的应用}}}*/
            }
        }
	
		return;
		
	}

	FS_GetFileSize(pFile,&filesize);
	FS_Read(pFile, (void *)content, filesize,&readed_size);
	FS_Close(pFile);
	
	mex_strupr(content);
	for( i = 0; i < mex_app_size; i++ )
	{
		strcpy(search_targ,"\"");
		strcat(search_targ, g_mex_menu_ctrl[i].app_name);
		strcat(search_targ, "\"");
		mex_strupr(search_targ);
		if((strsearch = strstr(content, search_targ))!= NULL)
		{
			g_mex_menu_ctrl[i].is_hiden = FALSE;
		}
		else
		{
			g_mex_menu_ctrl[i].is_hiden = TRUE;
		}

		/*客户要求加的应用{{{*/
		#if (MEX_MTK_CODE_VERSION < 0x1112)
		#ifdef __MEX_QQ_SUPPORT__
		if( MET_MEX_QQ_MENUID == Mex_App_List[i].met_mex_menu_id )
		{
			g_mex_menu_ctrl[i].is_hiden = FALSE;
		}
		#endif
		
		#ifdef __MEX_MSN_SUPPORT__
		if( MET_MEX_MSN_MENUID == Mex_App_List[i].met_mex_menu_id )
		{
			g_mex_menu_ctrl[i].is_hiden = FALSE;
		}
		#endif
		#endif

		/*客户要求加的应用}}}*/
		
		
		strcpy(search_targ,"\'");
		strcat(search_targ, g_mex_menu_ctrl[i].app_name);
		strcat(search_targ, "\'");
		mex_strupr(search_targ);

		if((strsearch = strstr(content, search_targ))!= NULL)
		{
			g_mex_menu_ctrl[i].is_second_menu_hiden = FALSE;
		}
		else
		{
			g_mex_menu_ctrl[i].is_second_menu_hiden = TRUE;
		}	
		
	}

}
                                                                                                                                                                                            
BOOL mex_is_second_menu_hiden(S8 * current_app_name)
{
	U8 i = 0;
	S8 *strsearch = NULL;
	S8 search_targ[MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU] = {0};	

	mex_strupr(current_app_name);
	for(i = 0; i < mex_app_size; i++)
	{
	
		strcpy(search_targ, g_mex_menu_ctrl[i].app_name);
		mex_strupr(search_targ);
		
		if((strsearch = strstr(current_app_name, search_targ))!= NULL)
		{
			if( FALSE == g_mex_menu_ctrl[i].is_second_menu_hiden )
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		
	}

	return TRUE;

}

#if (MEX_MTK_CODE_VERSION >= 0x1112)
MMI_BOOL mex_check_ifMenu_hide(U8 index)
{
	U16 i,j = 0;
	char mex_app_name[30] = {0};
	char mex_packet_name[30] = {0};
	char mtk_packet_name[30] = {0};
	extern const srv_nativeappfactory_package_struct native_app_package_table[];

	/*---------------------------------------
		find packet name's mex app name
	--------------------------------------*/
	/*use the native packet name to find whether in the list "g_mex_app2packet" */
	for(i = 0; i < mex_app2packet_size; i++)
	{
		strcpy(mtk_packet_name, native_app_package_table[index].package_name);
		strcpy(mex_packet_name,	g_mex_app2packet[i].packet_name);
		mex_strupr(mex_packet_name);
		mex_strupr(mtk_packet_name);
 		if(strcmp(mex_packet_name,mtk_packet_name) == 0)
		{
			strcpy(mex_app_name, g_mex_app2packet[i].mex_name);
			break;
		}
	}
	/*if not find the packet name in g_mex_app2packet, it means it doesnot belong to mex, it's MTK app, show it*/
	if(i >= mex_app2packet_size)
	{
		return MMI_FALSE;
	}

	/**-------------------------------------
		check if this mex app should show or not in mainmenu
	----------------------------------------**/
	/*we found the app in list, and get its mex name*/

	for(j = 0;j<mex_app_size; j++)
	{
		char hide_name_tmp[30] = {0};
		strcpy(hide_name_tmp,g_mex_menu_ctrl[j].app_name);
		mex_strupr(hide_name_tmp);
		mex_strupr(mex_app_name);
		if(strcmp(hide_name_tmp,mex_app_name) == 0)
		{
			return g_mex_menu_ctrl[j].is_hiden;
		}
		
	}
	/*entry menu is not in hidden control, show it*/
	return MMI_FALSE;
}


#else	//(MEX_MTK_CODE_VERSION < 0x1112)

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if (MEX_MTK_CODE_VERSION == 0x1012)
static U16 g_mex_nitems = 0;
#define MEX_APP_NAME_DYNAMIC_MENU	31
#define MEX_APP_DYNAMIC_MENU_ITEM   (MAX_SUB_MENUS - 10)
U16 Mex_App_MENUID_List[MEX_APP_DYNAMIC_MENU_ITEM] = {0};

//=======================================================

typedef struct  
{
	char app_name[MEX_APP_NAME_DYNAMIC_MENU];
	BOOL is_hiden;
	BOOL is_s_hiden;   //second menu
	U16 met_mex_menu_id;
	U16 met_mex_menu_string;
	U16 met_mex_menu_icon;
}mex_app_res;

mex_app_res Mex_App_List[] = 
{
	{"AngryBirds",true, true, MET_MEX_FNDXN_MENUID, E_MSI_FNDXN_MENU_STRING,E_MII_MEX_FNDXN_ICON },
	{"ddz",true, true, MET_MEX_DDZ_MENUID, E_MSI_DDZ_MENU_STRING,E_MII_MEX_DDZ_ICON },
	{"shsl",true, true, MET_MEX_SHSL_MENUID, E_MSI_SHSL_MENU_STRING,E_MII_MEX_SHSL_ICON },
	{"fruit",true, true, MET_MEX_QSG_MENUID, E_MSI_QSG_MENU_STRING,E_MII_MEX_QSG_ICON},
	{"zwzjs",true, true, MET_MEX_ZWDZJS_MENUID, E_MSI_ZWDZJS_MENU_STRING,E_MII_MEX_ZWDZJS_ICON},
	{"cqz",true, true, MET_MEX_CQZ_MENUID, E_MSI_CQZ_MENU_STRING, E_MII_MEX_CQZ_ICON},
	{"csjjs",true, true, MET_MEX_CSJJS_MENUID, E_MSI_CSJJS_MENU_STRING, E_MII_MEX_CSJJS_ICON},
	{"rocket",true, true, MET_MEX_BLFC_MENUID, E_MSI_BLFC_MENU_STRING, E_MII_MEX_BLFC_ICON},
	{"tom",true, true, MET_MEX_TOMMAO_MENUID, E_MSI_HDTOMMAO_MENU_STRING, E_MII_MEX_TOMMAO_ICON},
	{"mxsqy",true, true, MET_MEX_MXSQY_MENUID, E_MSI_MXSQY_MENU_STRING, E_MII_MEX_MXSQY_ICON},
};

#define MEX_APP_RES_SIZE (sizeof(Mex_App_List)/sizeof(mex_app_res))

void mex_parse_hiden_config_file(void)
{
	int32* pFile = NULL;
	int filesize = 0;
	bool bOpenPlatform = false;
	wchar_t cfgFileName[100] = { 0 };
	char content[300] = {0};
	uint32 readed_size = 0;
	char *strsearch = NULL;
	char hideentry_tag[20] = {0};
	char secondmenu_tag[20] = {0};
	int i =0;
	extern	const wchar_t* mex_GetSystemRootDir(void);

	pFile = (int32*)mex_fopenw_ext(L"MexSys.cfg",L"rb",mex_GetSystemRootDir(), NULL,false);


	if(pFile == NULL )
	{
		mex_log(MEX_LOG_SYSTEM,"try to open config file in T-card failed!code:%d",pFile);
		return;
	}

	filesize = mex_GetFileSize(pFile);
	if(0==mex_fread(content, 1,filesize ,pFile))
		{
		mex_log(MEX_LOG_SYSTEM,"read config file failed! read out for NULL");
		mex_fclose(pFile);
			return;
		}
	
	mex_fclose(pFile);
	
	mex_strupr(content);
	for(i = 0; i<MEX_APP_RES_SIZE; i++)
	{
		memset(hideentry_tag,0x00,20);
		strcpy(hideentry_tag,"\"");
		strcat(hideentry_tag, Mex_App_List[i].app_name);
		strcat(hideentry_tag, "\"");
		mex_strupr(hideentry_tag);
		if(((strsearch = strstr(content, hideentry_tag))!= NULL) && mmi_frm_get_hilite_hdlr(Mex_App_List[i].met_mex_menu_id))
		{
			Mex_App_List[i].is_hiden = FALSE;
		}
		else
		{
			Mex_App_List[i].is_hiden = TRUE;
		}

		memset(hideentry_tag,0x00,20);
		strcpy(hideentry_tag,"\'");
		strcat(hideentry_tag, Mex_App_List[i].app_name);
		strcat(hideentry_tag, "\'");
		mex_strupr(hideentry_tag);
	
		if((strsearch = strstr(content, hideentry_tag))!= NULL)
		{
			Mex_App_List[i].is_s_hiden = FALSE;
		}
		else
		{
			Mex_App_List[i].is_s_hiden = TRUE;
		}	

		mex_log(MEX_LOG_SYSTEM,"parse_hiden_config_file::Mex_App_List[%d].app_name:%s, is_hiden:%d, is_s_hiden:%d", i, Mex_App_List[i].app_name, Mex_App_List[i].is_hiden, Mex_App_List[i].is_s_hiden);
	}
}

U16 mex_app_menu_add(U16 n_items, U16 *string_ids, U16 *icon_ids)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	//U16 mex_i = 0;
	U16 mex_j = 0;
	U16 mex_current_item = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	g_mex_nitems = 0;

	mex_parse_hiden_config_file();

	for(mex_j = 0; mex_j < MEX_APP_RES_SIZE; mex_j++ )
	{
		if(!Mex_App_List[mex_j].is_hiden)
		{
			for( mex_current_item = n_items; mex_current_item > 0; mex_current_item-- )
			{
				string_ids[mex_current_item] = string_ids[mex_current_item - 1];
				icon_ids[mex_current_item] = icon_ids[mex_current_item - 1];
			}

			string_ids[0] = Mex_App_List[mex_j].met_mex_menu_string;
			icon_ids[0] = Mex_App_List[mex_j].met_mex_menu_icon;	
			n_items ++;
			Mex_App_MENUID_List[g_mex_nitems] = Mex_App_List[mex_j].met_mex_menu_id ;
			g_mex_nitems++;
		}
	}

    return n_items;
}

U16 mex_get_g_mex_nitems(void)
{
	return g_mex_nitems;
}

BOOL mex_is_second_menu_hiden(const char * current_app_name)
{
	U8 i = 0;
	S8 *strsearch = NULL;
	char * dot_locatin = NULL;
	S8 t_app_name[MEX_APP_NAME_DYNAMIC_MENU] = {0};	

	dot_locatin = strstr(current_app_name, ".mex");

	if(dot_locatin == NULL)
	{
		return TRUE;
	}
	
	//strcpy(t_app_name, current_app_name);

	strncpy(t_app_name, current_app_name, (dot_locatin - current_app_name));
	
	mex_strupr(t_app_name);
	for(i = 0; i < MEX_APP_RES_SIZE; i++)
	{
		mex_strupr(Mex_App_List[i].app_name);
		if((strsearch = strstr(t_app_name, Mex_App_List[i].app_name))!= NULL)
		{
			mex_log(1,"mex_is_second_menu_hiden::Mex_App_List[%d].app_name:%s, current_app_name:%s, t_app_name:%s, is_hiden:%d, is_s_hiden:%d", i, Mex_App_List[i].app_name, current_app_name, t_app_name, Mex_App_List[i].is_hiden, Mex_App_List[i].is_s_hiden);
			return Mex_App_List[i].is_s_hiden;
		}
	}
	return TRUE;
}
#else

#if defined(MT6235B) 
U16 mex_app_menu_add(U16 n_items, U8 **string_ids, U16 *icon_ids)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U16 mex_i = 0;
	U16 mex_j = 0;
	U16 mex_current_item = 0;
	S8	ctrl_app_name[MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU] = {0};
	S8	list_app_name[MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU] = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mex_parse_hiden_config_file();

	g_mex_nitems = 0;

	for(mex_j = 0; mex_j < mex_app_res_size; mex_j++ )
	{
		Mex_App_MENUID_List[mex_j] = 0;
	}

	for(mex_i = 0; mex_i<mex_app_size; mex_i++)
	{
		strcpy(ctrl_app_name, g_mex_menu_ctrl[mex_i].app_name);
		mex_strupr(ctrl_app_name);

		for(mex_j = 0; mex_j < mex_app_res_size; mex_j++ )
		{
			strcpy(list_app_name, Mex_App_List[mex_j].app_name);
			mex_strupr(list_app_name);
			
			if(FALSE == g_mex_menu_ctrl[mex_i].is_hiden )
			{
				//if( 0 == strcmp(g_mex_menu_ctrl[mex_i].app_name, Mex_App_List[mex_j].app_name))
				if( 0 == strcmp(ctrl_app_name, list_app_name) )
				{
					for( mex_current_item = n_items; mex_current_item > 0; mex_current_item-- )
					{
						string_ids[mex_current_item] = string_ids[mex_current_item - 1];
						icon_ids[mex_current_item] = icon_ids[mex_current_item - 1];
					}

					string_ids[0] = (U8*) GetString(Mex_App_List[mex_j].met_mex_menu_string);
					icon_ids[0] = Mex_App_List[mex_j].met_mex_menu_icon;	
					n_items ++;
					Mex_App_MENUID_List[g_mex_nitems] = Mex_App_List[mex_j].met_mex_menu_id ;
					g_mex_nitems++;
					
					break;

				}
			}

		}

	}

    return n_items;

}

#else	//defined (MT6235B))

U16 mex_app_menu_add(U16 n_items, U16 *string_ids, U16 *icon_ids)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U16 mex_i = 0;
	U16 mex_j = 0;
	U16 mex_current_item = 0;
	S8	ctrl_app_name[MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU] = {0};
	S8	list_app_name[MAX_MEX_APP_NAME_LENGTH_FOR_HIDEMENU] = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mex_parse_hiden_config_file();

	g_mex_nitems = 0;

	for(mex_j = 0; mex_j < mex_app_res_size; mex_j++ )
	{
		Mex_App_MENUID_List[mex_j] = 0;
	}

	for(mex_i = 0; mex_i<mex_app_size; mex_i++)
	{
		strcpy(ctrl_app_name, g_mex_menu_ctrl[mex_i].app_name);
		mex_strupr(ctrl_app_name);

		for(mex_j = 0; mex_j < mex_app_res_size; mex_j++ )
		{
			strcpy(list_app_name, Mex_App_List[mex_j].app_name);
			mex_strupr(list_app_name);
			
			if(FALSE == g_mex_menu_ctrl[mex_i].is_hiden )
			{
				//if( 0 == strcmp(g_mex_menu_ctrl[mex_i].app_name, Mex_App_List[mex_j].app_name))
				if( 0 == strcmp(ctrl_app_name, list_app_name) )
				{
					for( mex_current_item = n_items; mex_current_item > 0; mex_current_item-- )
					{
						string_ids[mex_current_item] = string_ids[mex_current_item - 1];
						icon_ids[mex_current_item] = icon_ids[mex_current_item - 1];
					}

					string_ids[0] = Mex_App_List[mex_j].met_mex_menu_string;
					icon_ids[0] = Mex_App_List[mex_j].met_mex_menu_icon;	
					n_items ++;
					Mex_App_MENUID_List[g_mex_nitems] = Mex_App_List[mex_j].met_mex_menu_id ;
					g_mex_nitems++;
					
					break;

				}
			}

		}

	}

    return n_items;

}

#endif	//defined (MT6235A))

U16 mex_get_g_mex_nitems(void)
{
	return g_mex_nitems;
}

#endif  //(MEX_MTK_CODE_VERSION == 0x1012)
/*add by dzw 20111215 }}}*/



#endif	//defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)


#endif	//(MEX_MTK_CODE_VERSION >= 0x1112)

#endif //__MET_MEX_SUPPORT__
