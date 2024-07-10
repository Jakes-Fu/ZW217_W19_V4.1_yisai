/*****************************************************************************
** File Name:      mmiqbtheme_export.c                                        *
** Author:         cheng.luo                                                   *
** Date:           2011/09                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/09/17     cheng.luo       Create.                                     *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmiqbtheme_export.h"
#include "mmiqbtheme_internal.h"
#include "mmiqbtheme_nv.h"
//#include "mmiset.h"
//#include "mmiset_export.h"
#include "mmifmm_export.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T       g_mmiqbtheme_app = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL uint8 s_qbtheme_icon_bg = 0;
LOCAL uint8 s_qbtheme_icon = 0;
LOCAL uint8 s_qbtheme_font = 0;
LOCAL wchar* s_qbtheme_cur_theme = PNULL;
LOCAL wchar* s_qbtheme_cur_wallpaper = PNULL;

LOCAL BOOLEAN s_qbtheme_is_need_transform = TRUE;
LOCAL BOOLEAN s_qbtheme_force_transform = FALSE;

/**--------------------------------------------------------------------------*
**                         EXTERNAL FUNCTION DEFINITION                     *
**--------------------------------------------------------------------------*/

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_InitModule
//  Description : 	MMIAPIQBTHEME_InitModule
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_InitModule(void)
{
    MMIQBTHEME_RegQbThemeNv();
    MMIQBTHEME_RegWinIdNameArr();
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_Init
//  Description : 	MMIAPIQBTHEME_Init
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIQBTHEME_Init(void)
{
	MN_RETURN_RESULT_E ret = MN_RETURN_FAILURE;
	SFS_ERROR_E        sfs_ret = SFS_ERROR_NONE;
	
	//	MMIQBTHEME_ManageInit();
	sfs_ret = SFS_CreateDirectory(QBTHEME_ROOT_PATH);
	if (SFS_ERROR_HAS_EXIST == sfs_ret || SFS_ERROR_NONE == sfs_ret)
	{
		MMIAPIFMM_SetAttr(QBTHEME_ROOT_PATH, MMIAPICOM_Wstrlen(QBTHEME_ROOT_PATH),\
			TRUE, FALSE, FALSE, FALSE);
	}
	
	g_mmiqbtheme_app.ProcessMsg = MMIQBTHEME_HandleRefInstallMsg;
	MMIQBTHEME_RegBrowserCB();
	MMIFW_SetUdiskMountListener(MMIQBTHEME_UdiskMountCallback, TRUE);
	
	if (PNULL == s_qbtheme_cur_theme)
	{
		s_qbtheme_cur_theme = SCI_ALLOCA(sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
	}
	if (PNULL != s_qbtheme_cur_theme)
	{	
		SCI_MEMSET(s_qbtheme_cur_theme, 0, sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
		MMINV_READ(QBTHEME_NV_CUR_THEME, s_qbtheme_cur_theme, ret);
		if(MN_RETURN_SUCCESS != ret)
		{
			MMINV_WRITE(QBTHEME_NV_CUR_THEME, s_qbtheme_cur_theme); 
		}
	}
	
	if (PNULL == s_qbtheme_cur_wallpaper)
	{
		s_qbtheme_cur_wallpaper = SCI_ALLOCA(sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
	}
	if (PNULL != s_qbtheme_cur_wallpaper)
	{
		SCI_MEMSET(s_qbtheme_cur_wallpaper, 0, sizeof(wchar) * (QBTHEME_NAME_EN_MAX_LEN + 1));
		MMINV_READ(QBTHEME_NV_WALLPAPER, s_qbtheme_cur_wallpaper, ret);
		if (MN_RETURN_SUCCESS != ret)
		{
			MMIAPICOM_Wstrcpy(s_qbtheme_cur_wallpaper, L"");
			MMINV_WRITE(QBTHEME_NV_WALLPAPER, s_qbtheme_cur_wallpaper);
		}		
	}
	
	MMINV_READ(QBTHEME_NV_MAINMENU_ICON, &s_qbtheme_icon,ret );
	if (MN_RETURN_SUCCESS != ret)
	{
		s_qbtheme_icon = SYSTEM_DEFAULT;
		MMINV_WRITE(QBTHEME_NV_MAINMENU_ICON, &s_qbtheme_icon);
	}
	
	MMINV_READ(QBTHEME_NV_ICON_BG, &s_qbtheme_icon_bg, ret);
	if(MN_RETURN_SUCCESS != ret)
	{
		s_qbtheme_icon_bg = DEFAULT_NONE;
		MMINV_WRITE(QBTHEME_NV_ICON_BG, &s_qbtheme_icon_bg);		
	}

	MMINV_READ(QBTHEME_NV_FONT, &s_qbtheme_font, ret);
	if(MN_RETURN_SUCCESS != ret)
	{
		s_qbtheme_font = SYSTEM_DEFAULT;
		MMINV_WRITE(QBTHEME_NV_FONT, &s_qbtheme_font);		
	}

	// test by cheng,luo
	//MMIAPIQBTHEME_StyleStart();
}

/********************************^***********************************/
//  Interface:		MMIQBTHEME_GetCurIconType
//  Description : 	MMIQBTHEME_GetCurIconType
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint8 MMIQBTHEME_GetCurIconType()
{
	return s_qbtheme_icon;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetCurIconBgType
//  Description : 	MMIQBTHEME_GetCurIconBgType
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint8 MMIQBTHEME_GetCurIconBgType()
{
	return s_qbtheme_icon_bg;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetCurFont
//  Description : 	MMIQBTHEME_GetCurFont
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC uint8 MMIQBTHEME_GetCurFontType()
{
	return s_qbtheme_font;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_SetCurIconType
//  Description : 	MMIQBTHEME_SetCurIconType
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_SetCurIconType(uint8 type)
{
	s_qbtheme_icon = type;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_SetCurIconBgType
//  Description : 	MMIQBTHEME_SetCurIconBgType
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_SetCurIconBgType(uint8 type)
{
	s_qbtheme_icon_bg = type;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_SetCurFontType
//  Description : 	MMIQBTHEME_SetCurFontType
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_SetCurFontType(uint8 type)
{
	s_qbtheme_font = type;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetCurWallpaper
//  Description : 	MMIQBTHEME_GetCurWallpaper
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC wchar* MMIQBTHEME_GetCurWallpaper(void)
{
	return s_qbtheme_cur_wallpaper;
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_IsIconHaveBg
//  Description : 	MMIAPIQBTHEME_IsIconHaveBg
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPIQBTHEME_IsIconHaveBg(void)
{
	BOOLEAN bret = TRUE;
	
	if (STYLE_IS_SUPPORT_QBTHEME
		&& DEFAULT_NONE == s_qbtheme_icon_bg)
	{
		bret = FALSE;
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetCurrentTheme
//  Description : 	MMIQBTHEME_GetCurrentTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC wchar* MMIQBTHEME_GetCurrentTheme(void)
{
	return s_qbtheme_cur_theme;
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_IsDefaultTheme
//  Description : 	MMIAPIQBTHEME_IsDefaultTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPIQBTHEME_IsDefaultTheme(void)
{
	BOOLEAN bret = TRUE;
	
	if (STYLE_IS_SUPPORT_QBTHEME
		&& 0 != MMIAPICOM_Wstrlen(s_qbtheme_cur_theme))
	{
		bret = FALSE;
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_SetNeedTransform
//  Description : 	MMIQBTHEME_SetNeedTransform
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_SetNeedTransform(
										BOOLEAN is_transform
										)
{
	s_qbtheme_is_need_transform = is_transform;
}

/*******************************************************************/
//  Interface:		MMIAPIQBTHEME_GetNeedTransform
//  Description : 	MMIAPIQBTHEME_GetNeedTransform
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPIQBTHEME_GetNeedTransform(void)
{
	return s_qbtheme_is_need_transform;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_SetForceTransform
//  Description : 	MMIQBTHEME_SetForceTransform
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIQBTHEME_SetForceTransform(
										 BOOLEAN is_force
										 )
{
	s_qbtheme_force_transform = is_force;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_GetForceTransform
//  Description : 	MMIQBTHEME_GetForceTransform
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_GetForceTransform(void)
{
	return s_qbtheme_force_transform;
}

/*******************************************************************/
//  Interface:		MMIQBTHEME_IsEnableStyle
//  Description : 	MMIQBTHEME_IsEnableStyle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIQBTHEME_IsSupport(
                                    MMISET_IDLE_STYLE_E style
                                    )
{
    return ((MMISET_IDLE_QBTHEME == style)||(MMISET_IDLE_ISTYLE == style));
}
