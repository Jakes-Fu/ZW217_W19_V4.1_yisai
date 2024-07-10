#ifdef MRAPP_SUPPORT
/****************************************************************************
** File Name:      mmimrapp.c                            
** Author:            qgp                                                      
** Date:            2006/05/15
** Copyright:           
** Description:     
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006	qgp		          Create
** 
****************************************************************************/
#ifndef _MMIMRAPP_C_
#define _MMIMRAPP_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "Mn_type.h"
#include "mmisms_app.h"
#include "block_mem.h"
#include "mmipub.h"
#include "mmimrapp_export.h"
#include "mmimrapp_internal.h"
#include "mmimrapp_id.h"
#include "mmienvset_export.h"
#include "mmi_nv.h"
#include "mmiudisk_export.h"
#include "Mmiidle_statusbar.h"


#ifdef MRAPP_COOKIE_OPEN
#include "mmimrdata_cookie.h"
#include "mmimrdata_config.h"
#endif

#ifdef MRAPP_MRPSTORE_OPEN
#include "mmimrdata_mrpstore.h"
#endif

#ifdef MRAPP_SKYBUDDY_OPEN
#include "mmimrdata_skybuddy.h"
#endif
#include "mmifmm_export.h"



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
char dsmEntry[100] = {0}; 
#ifdef MRAPP_CLOCK_OPEN
char strMrpFile[256];
#endif

mr_screen_angle_t g_screen_angle = {0};


const GUIMENU_GROUP_T mrp_menu_table[] = 
{
    {NULL},
};

LOCAL uint16 mrp_nv_len[] =
{
	sizeof(MRAPP_ENVINFO_T),
	sizeof(int8),
};

#if defined(__MR_CFG_LICENSE_MANAGER__) && !defined(WIN32)
/*********************************************************************************
 * 本地应用权限注册函数列表
 *********************************************************************************/
MR_LAPP_LICENSE_MANAGER_BEGIN()

/* 将你的应用程序参照如下方式添加到下面 */
#ifdef __MR_CFG_ENTRY_MYAPP__	
	MR_LAPP_LICENSE_ENTRY("mrapp", mrapp_license_register)
#endif

MR_LAPP_LICENSE_MANAGER_END();
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN MMIMRAPP_checkStorage(uint8 grade);
LOCAL void MMIMRAPP_clearAllreg(void);

/*****************************************************************************/
// 	Description : mrp应用的入口
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_AppEntry(MRAPP_APPENTRY_E index)
{
	BOOLEAN recode = FALSE;
	MMIMR_Trace(("MMIMRAPP_AppEntry() index: %d", index));

	MMIMRAPP_setCurApp(index);
	MMIMRAPP_clearAllreg();

    g_screen_angle.lcd_angle = LCD_ANGLE_0;
    g_screen_angle.win_angle = WIN_SUPPORT_ANGLE_0;
	
	/*modify by zack@20120803 start*/
#ifdef MMI_RES_ORIENT_BOTH
    MMK_SetScreenAngle(LCD_ANGLE_0);
#endif
	/*modify by zack@20120803 end*/

	MMITHEME_DestroyMenuItemCache();

	switch(index)
	{
#ifdef MRAPP_SDKQQ_OPEN
	case MRAPP_SDKQQ:
	if(MMIMRAPP_checkStorage(0))
	{
        mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
		mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);

		recode = MMIMRAPP_MrAppRun((void*)"*J,qq2008", index);		
	}
	break;
#endif

#ifdef MRAPP_TOOL_OPEN
	case MRAPP_TOOL:
		if(MMIMRAPP_checkStorage(0))
		{
			mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);			
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,tools", index);
		}
		break;
#endif
#ifdef MRAPP_SIMUGAME_OPEN
	case MRAPP_NESGAME:
		if(MMIMRAPP_checkStorage(0))
        {      
    		mr_registerAPP((uint8 *)mr_m0_nesMain, sizeof(mr_m0_nesMain), FIRM_GML_IND);
            mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
    		mr_registerAPP((uint8 *)mr_m0_nes240, sizeof(mr_m0_nes240), FIRM_NES1_IND);
    		//mr_registerAPP((uint8 *)mr_m0_nes240_270, sizeof(mr_m0_nes240_270), FIRM_NES2_IND);
    		//mr_registerAPP((uint8 *)mr_m0_nes240_90, sizeof(mr_m0_nes240_90), FIRM_NES3_IND);
    		recode = MMIMRAPP_MrAppRun((void*)"*J,nes", index);
        }
        break;
#endif
#ifdef MRAPP_ENDICT_OPEN
	case MRAPP_ENDICT:
		if(MMIMRAPP_checkStorage(0))
		{
			mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);			
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);

			recode = MMIMRAPP_MrAppRun((void*)"*J,atyhzd", index);
		}
		break;
#endif

#ifdef MRAPP_CHDICT_OPEN
	case MRAPP_CHDICT:
		if(MMIMRAPP_checkStorage(0))
		{
			mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);			
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);

			recode = MMIMRAPP_MrAppRun((void*)"*J,axhzd", index);
		}
		break;
#endif

#ifdef MRAPP_EBOOK_OPEN
	case MRAPP_EBOOK:
    	if(MMIMRAPP_checkStorage(0))
    	{
            mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
    		mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);

    		recode = MMIMRAPP_MrAppRun((void*)"*J,skyread", index);
    	}
    	break;
#endif

#ifdef MRAPP_COOKIE_OPEN
	case MRAPP_COOKIE:
    	if(MMIMRAPP_checkStorage(0))
    	{
    		mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
            recode = MMIMRAPP_MrAppRun((void*)"*J,games", index);
    	}
    	break;
#endif

#ifdef MRAPP_MRPSTORE_OPEN
	case MRAPP_MRPSTORE:
    	if(MMIMRAPP_checkStorage(0))
    	{
            mr_registerAPP((uint8 *)mr_m0_mrpstore, sizeof(mr_m0_mrpstore), FIRM_GML_IND);
            recode = MMIMRAPP_MrAppRun((void*)"*J,1,0,3,3,5,0,0", index);
    	}
    	break;
#endif

#ifdef MRAPP_SKYBUDDY_OPEN
	case MRAPP_SKYBUDDY:
    	if(MMIMRAPP_checkStorage(0))
    	{
            mr_registerAPP((uint8 *)mr_m0_eim, sizeof(mr_m0_eim), FIRM_GML_IND);
            mr_registerAPP((uint8 *)mr_m0_edg, sizeof(mr_m0_edg), 8);
            recode = MMIMRAPP_MrAppRun((void*)"*J", index);
    	}
    	break;
#endif

#ifdef MRAPP_DDZ_OPEN
	case MRAPP_DDZ:
		mr_registerAPP((uint8 *)mr_m0_ddz, sizeof(mr_m0_ddz), 7);
		recode = MMIMRAPP_MrAppRun((void*)"*H", index);		
#endif

#ifdef MRAPP_MSN_OPEN
	case MRAPP_MSN:
    	if(MMIMRAPP_checkStorage(0))
    	{
#ifdef __MRAPP_OVERSEA__
            mr_registerAPP((uint8 *)mr_m0_mrpstore, sizeof(mr_m0_mrpstore), FIRM_GML_IND);

    		recode = MMIMRAPP_MrAppRun((void*)"*J,1,0,3,3,5,0,0,MSN,203102,msn.mrp", index);
#else
            mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
    		mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);

    		recode = MMIMRAPP_MrAppRun((void*)"*J,msn", index);
#endif
    	}
    	break;
#endif

#ifdef MRAPP_CLOCK_OPEN
	case MRAPP_CLOCK:
    	if(MMIMRAPP_checkStorage(0)) 
    	{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,clockc", index);
    	}
    	break;
#endif  
 
#ifdef MRAPP_NETGAME_OPEN
	case MRAPP_NETGAME:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,maopao", index);
		}
		break;
#endif 

#ifdef MRAPP_QIPAI_OPEN
	case MRAPP_QIPAI:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,mrpcc", index);
		}
		break;
#endif 

#ifdef MRAPP_TIANQI_OPEN
	case MRAPP_TIANQI:
		if(MMIMRAPP_checkStorage(0)) 
		{
			mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,otatq", index);
		}
		break;
#endif 

#ifdef MRAPP_FARM_OPEN
	case MRAPP_FARM:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,atkxnc", index);
		}
		break;
#endif 

#ifdef MRAPP_KAIXIN_OPEN
	case MRAPP_KAIXIN:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);

			recode = MMIMRAPP_MrAppRun((void*)"*J,kx001", index);
		}
		break;
#endif 

#ifdef MRAPP_FETION_OPEN
	case MRAPP_FETION:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);			

			recode = MMIMRAPP_MrAppRun((void*)"*J,otafx3", index);
		}
		break;
#endif 

#ifdef MRAPP_XLWB_OPEN
	case MRAPP_XLWB:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,axlwb", index);
		}
		break;
#endif 

#ifdef SKY_CAT_SUPPORT
	case MRAPP_TALKCAT:
		if(MMIMRAPP_checkStorage(0)) 
		{
    		mr_registerAPP((uint8 *)mr_m0_config, sizeof(mr_m0_config), 18);
			mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), FIRM_GML_IND);
			recode = MMIMRAPP_MrAppRun((void*)"*J,talkcat,2924", index);
		}
		break;
#endif

#ifdef MRAPP_FACEBOOK_OPEN 
	case MRAPP_FACEBOOK:
    	if(MMIMRAPP_checkStorage(0))
    	{
    		mr_registerAPP((uint8 *)mr_m0_mrpstore, sizeof(mr_m0_mrpstore), FIRM_GML_IND);

    		recode = MMIMRAPP_MrAppRun((void*)"*J,1,0,3,3,5,0,0,Facebook,810002,fbook.mrp", index);
        }
    	break;
#endif

#ifdef MRAPP_SKYPE_OPEN
	case MRAPP_SKYPE:
    	if(MMIMRAPP_checkStorage(0))
    	{
    		mr_registerAPP((uint8 *)mr_m0_mrpstore, sizeof(mr_m0_mrpstore), FIRM_GML_IND);

            recode = MMIMRAPP_MrAppRun((void*)"*J,1,0,3,3,5,0,0,Skype,391022,cskype.mrp", index);	
    	}
    	break;
#endif

#ifdef MRAPP_YAHOO_OPEN
	case MRAPP_YAHOO:
    	if(MMIMRAPP_checkStorage(0))
    	{
    		mr_registerAPP((uint8 *)mr_m0_mrpstore, sizeof(mr_m0_mrpstore), FIRM_GML_IND);

            recode = MMIMRAPP_MrAppRun((void*)"*J,1,0,3,3,5,0,0,Yahoo,810000,yahoo.mrp", index);	
    	}
    	break;
#endif

#ifdef MRAPP_TWITTER_OPEN
	case MRAPP_TWITTER:
    	if(MMIMRAPP_checkStorage(0))
    	{
            mr_registerAPP((uint8 *)mr_m0_mrpstore, sizeof(mr_m0_mrpstore), FIRM_GML_IND);
			
            recode = MMIMRAPP_MrAppRun((void*)"*J,1,0,3,3,5,0,0,Twitter,810006,twitter.mrp", index);		
    	}
    	break;
#endif

    default:
        MMIMR_Trace(("mrapp invalid entry index = %d", index));
        break;
	}

	return recode;
}

/*****************************************************************************/
// 	Description :获取存储器状态
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMRAPP_checkStorage(uint8 grade)
{
    if ((TRUE != MMIAPIFMM_GetDeviceStatus(MMIFILE_DEVICE_UDISK, MMIFILE_DEVICE_UDISK_LEN))
        && (TRUE != MMIAPIFMM_GetDeviceStatus(MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN)))
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

/*****************************************************************************/
// 	Description : 保存下载的mrp文件
//	Global resource dependence : 
//  Author: qgp
//  return : 
//0		下载文件是mr 文件，且保存成功
//-1	下载文件是mr文件，但保存失败
// 1	下载文件不是mr 文件
// 2	存储空间不足
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_saveMrApp(uint8 *data,uint32 len)
{
	return MMIMRAPP_savePack(data, len);
}

/*****************************************************************************/
// 	Description : 检查是否是DSM的配置信息。
//	Global resource dependence : 
//  Author: qgp
//  Note : return:
// 0	是DSM配置消息，配置操作成功，需要删除该消息
//-1	是DSM配置消息，配置操作失败，需要删除该消息
// 1	不是DSM配置消息
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_dsmSMSfilter(MN_SMS_ALPHABET_TYPE_E  alphabet_type, MN_SMS_USER_VALID_DATA_T *data, MN_CALLED_NUMBER_T *number)
{
	int32 result, recode = 1;
	
	result = MMIMRAPP_SMSfilter(alphabet_type, data, number);

	if(result == MR_SUCCESS)
		recode = 0;
	else if(result == MR_FAILED)
		recode = -1;
	else
		recode = 1; 

	return recode;
}

/*****************************************************************************/
// 	Description : register mrp module nv len and max item
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIMRAPP_initModule(void)
{
	MMI_RegMenuGroup(MMI_MODULE_MRAPP, mrp_menu_table);
	MMI_RegModuleNv(MMI_MODULE_MRAPP, mrp_nv_len, sizeof(mrp_nv_len) / sizeof(uint16));
}

/*****************************************************************************/
// 	Description : 强制关闭mrp 应用及引擎
//	Global resource dependence : 
//  Author: qgp
//	Note: 当点击触摸屏下方的快捷键来切换菜单时,需要关闭mrp,因
// 为内存共享等原因,否则会产生冲突.
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_closeMrapp(void)
{
	MMIMR_Trace(("MMIMRAPP_closeMrapp(), %d", g_mr_vmBackRun));
#ifdef MR_BINDTO_VIRTUAL_WIN
	if(g_mr_vmBackRun != MRAPP_MAX)
	{
		MMIMRAPP_exitMrApp(FALSE);
	}
	else
#endif
	if(MMK_IsOpenWin(MMIMRAPP_BASE_WIN_ID))
	{
		MMIMRAPP_setAllReady(FALSE);
		if(MMIMRAPP_closeAllWins())
		{
			return TRUE;
		}
	}

    MMIMRAPP_StopStatusIconTimer();
    MAIN_SetIdleMrappState(FALSE); /*lint !e718 !e18*/
		
	return FALSE;
}

/*****************************************************************************/
// 	Description : 查询mr app是否在运行(用于与mr app有内存、网络等资源
//   冲突的模块调用)
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_mrappIsActived(void)
{
	if(s_mr_allReady
#ifdef MR_BINDTO_VIRTUAL_WIN
		|| (g_mr_vmBackRun != MRAPP_MAX)
#endif
		)
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************/
// 	Description : 保存当前手机的必要状态.
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIMRAPP_setPhoneState(e_mrapp_phone_mod mod, int32 state)
{
	if(mod < MRAPP_MD_MAXNUM)
	{
		g_mr_phoneModState[mod] = state;
	}
}

/*****************************************************************************/
// 	Description :  初始化设置
//	Global resource dependence : 
//  Author:qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMRAPP_InitSetting(void)
{
#if defined(__MR_CFG_LICENSE_MANAGER__) && !defined(WIN32)
    extern void mr_license_mgr_initialize(int32);
#endif

	/*2009-10-22 111,17787 add. for overseas*/
	MMIMRAPP_initApn();
	/*2009-10-22 111,17787 add end*/

#if defined(__MR_CFG_LICENSE_MANAGER__) && !defined(WIN32)
	mr_license_mgr_initialize(0);
#endif

	return TRUE;
}

/*****************************************************************************/
// 	Description : 通知应用恢复前台运行(如果app当前没有运行，则起app主菜单)
//	Global resource dependence : 
//  Author: qgp
//	Note: param - no use now.
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_activeMrapp2Front(MRAPP_APPENTRY_E app, void *param)
{
	BOOLEAN ret = FALSE;

    MMIMR_Trace(("mrapp MMIMRAPP_activeMrapp2Front app = %d", app));
	if(MMIMRAPP_mrappIsActived())
	{
		ret = MMIMRAPP_MrAppRun(NULL, app);
        /*应用由后台激活到前台时去除状态栏图标*/
        if(TRUE == ret)
        {
            MAIN_SetIdleMrappState(FALSE); /*lint !e718 !e18*/
        }
    }
	else
	{
		ret = MMIMRAPP_AppEntry(app);
	}
	
	return ret;
}

/*****************************************************************************/
// 	Description : 清空所有apps注册
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
LOCAL void MMIMRAPP_clearAllreg(void)
{
	int i;
	for(i = 0; i < 20; i++)
	{
		mr_registerAPP((uint8 *)NULL, 0, i);
	}
}

/*****************************************************************************/
// 	Description : 查询厂商机型信息
//	Global resource dependence : 
//  Author: zack
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_ProcessShow(void)
{
	MMI_STRING_T  s_text = {0};
	BOOLEAN ret = TRUE;
	uint8   temp_buf1[32] = {0};
    uint16  temp_buf2[32] = {0};
	uint32 temp_time=15000;
	
	MMIMR_Trace(("MMIMRAPP_ProcessShow() manu:%s handset:%s", MRAPP_MANUFACTORY, MRAPP_HANDSET));
	
	SCI_MEMSET(temp_buf1, 0, sizeof(temp_buf1));
	SCI_MEMSET(temp_buf2, 0, sizeof(temp_buf2));
    sprintf((char*)temp_buf1, "%s  %s", MRAPP_MANUFACTORY, MRAPP_HANDSET);
	
	s_text.wstr_len = MMIMRAPP_GB2UCS(temp_buf2, temp_buf1, strlen(temp_buf1)); /*lint !e64*/
	s_text.wstr_ptr = temp_buf2;
	
	MMIPUB_OpenAlertWinByTextPtr(&temp_time,&s_text,NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,NULL);	
	return ret;
}

/*****************************************************************************/
// 	Description : 测试入口
//	Global resource dependence : 
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_ProcessEntrStr(
									   uint8* str_ptr, //[IN] input string pointer
									   uint16 str_len  //[IN] input string length
									   )
{ /*lint !e18 !e516*/
	BOOLEAN recode = FALSE;
	MMIMR_Trace(("MMIMRAPP_ProcessEntrStr(), %d, %s", str_len, str_ptr));

	if(strncmp((char*)str_ptr, "*#220807#", str_len) == 0
       && str_len == strlen("*#220807#"))
	{
	    MMIMRAPP_setCurApp(MRAPP_TEST);
		recode = MMIMRAPP_MrAppRun((void*)"%dsm_gm.mrp", MRAPP_TEST);	
	}
	else if(strncmp((char*)str_ptr, "*#777755999#", str_len) == 0
             && str_len == strlen("*#777755999#"))
	{
		MMIMRAPP_ProcessShow();
		recode = TRUE;
	}

	return (recode);
}

/*****************************************************************************/
// 	Description : bl mem Cross 处理回调(可用于block_mem.c中的内存划分那里)
//	Global resource dependence : 
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_BLMemCb(BLOCK_MEM_ID_E block_id, BLOCK_MEM_RELATION_STATUS_E relation_info)
{
    MMIMR_Trace(("mrapp MMIMRAPP_BLMemCb block_id = %d relation_info = %d", block_id, relation_info));
	if(BORROW_FROM_CURRENT_BLOCK == relation_info)
    {
		MMIMRAPP_closeMrapp();
		MMIMR_Trace(("mr close END"));
	}
	 
	return TRUE;
}

/*****************************************************************************/
// 	Description : 通知应用恢复前台运行(如果app当前没有运行，则起app主菜单)
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_activeMrapp(void)
{
    MMIMR_Trace(("mrapp MMIMRAPP_activeMrapp g_mr_vmBackRun = %d", g_mr_vmBackRun));
	return MMIMRAPP_activeMrapp2Front(g_mr_vmBackRun, NULL);
}
	
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC MRAPP_APPENTRY_E MMIMRAPP_GetCurHungApp(void)
{
	MMIMR_Trace(("MMIMRAPP_GetCurHungApp, %d", g_mr_vmBackRun));
	return (MRAPP_APPENTRY_E)g_mr_vmBackRun;
}

#ifdef MRAPP_CLOCK_OPEN
/*****************************************************************************/
//  Description : netclock timer callback fuction
//  Global resource dependence : 
//  Author: zzh
//  Note:
/*****************************************************************************/
void MMIMRAPP_StartNetClock(void)
{
    int i = 0;
    mr_screeninfo screen;

#ifndef __MR_CFG_VMMEM_CUSTOM__
    if(NOT_USE != BL_GetStatus(BLOCK_MEM_POOL_MRAPP)) /*lint !e64*/
    {
        return;
    }
#endif
   
	for(i = 0; i < 20; i++)
	{
		mr_registerAPP((uint8 *)NULL, 0, i);
	}
    MMIMRAPP_setCurApp(MRAPP_CLOCK);
    MMIMRAPP_setDefFilePath();

    if(MMIMRAPP_checkStorage(0)) 
    {
        mr_getScreenInfo(&screen);
        sprintf(strMrpFile, "app%d%d\\clockc.mrp", screen.width, screen.height);
        MMIMR_Trace(("MRAPP MMIMRAPP_StartNetClock strMrpStr = %s", strMrpFile));
        if(MR_IS_FILE==mr_info(strMrpFile))
        {
            sprintf(strMrpFile, "%%app%d%d\\clockc.mrp,%%%d", screen.width, screen.height, STARTUP_CLOCK_UI_MODE);
            MMIMR_Trace(("MRAPP startup netclock from TCard %s", strMrpFile));
        }
        else
        {
            sprintf(strMrpFile, "*J%%%d", STARTUP_CLOCK_UI_MODE);
            MMIMR_Trace(("MRAPP startup netclock from firm %s", strMrpFile));
        }
        MMIMRAPP_MrAppRun((void*)strMrpFile, MRAPP_CLOCK); 
    }
} 
#endif

#ifdef __MR_CFG_SHORTCUT_SUPPORT__
void mr_scut_start_app(void* param1, void *param2)
{
    int32 appId = *(int32 *)param1;
    uint32 menuId = *(uint32 *)param2;
    BOOLEAN ret = FALSE;
    MR_SHORTCUT_APPINFO_T appInfo = {0};

    MMIMRAPP_setDefFilePath();

    if(MR_SUCCESS == mr_scut_read_data(appId, &appInfo))  
    {
        mr_scut_refresh_icon(menuId, appInfo);
    
        SCI_MEMSET(dsmEntry, 0x00, sizeof(dsmEntry));
        sprintf(dsmEntry, "*J,%s", appInfo.start_data);
#ifndef __MRAPP_OVERSEA__
        mr_registerAPP((uint8 *)mr_m0_cookie, sizeof(mr_m0_cookie), 9);
#endif
        ret = MMIMRAPP_MrAppRun((void*)dsmEntry, appId);
    }

    MMIMR_Trace(("[SKYSHORTCUT] mr_scut_start_app appId = %u dsmEntry = %s ret = %d", appId, dsmEntry, ret));
}
#endif

#endif//#_MMIMRAPP_C_

#endif//#end MRAPP_SUPPORT

