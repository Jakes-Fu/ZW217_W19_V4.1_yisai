#include "sci_types.h"

#ifdef ENGTD_SUPPORT
#include "mmiengtd_menutable.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiengtd_win.h"
//#include "mmiengtd_main.h"
#include "mmiengtd_id.h"
#include "window_parse.h"
#include "mmiengtd_nv.h"
//#include "mmiengtd_export.h"
#include "mmiengtd_internal.h"

//#define MMIENG_MAX_APN_LEN 16

WINDOW_TABLE(MMIENGTD_MAINMENU_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)ENGTDMainMenuWinHandleMsg),
	WIN_ID(MMIENGTD_MAINMENU_WIN_ID),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MENU_ENGTD, MMIENGTD_MAINMENU_CTRL_ID),
	END_WIN
};

/*the length of eng nv*/
LOCAL uint16 mmiengtd_nv_len_tab[] =
{
    2,  //@shangke add   MMINV_ENG_THRESHOLD
    4,  //@shangke add   MMINV_ENG_PERIODS
    1,  //@shangke add    MMINV_ENG_TIMER
    MMIENG_MAX_APN_LEN*sizeof(uint8),
    sizeof(MMIENG_VT_T)
};
/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENGTD_AppInit(void)
{
//	g_mmieng_app.ProcessMsg = AppHandleEngMsg;
//	g_mmieng_app.component_type = CT_APPLICATION;

    //×¢²á²Ëµ¥
    MMIENGTD_RegMenuGroup();
    MMIENGTD_RegWinIdNameArr();

//    MMIENG_InitRoamParam();//@shangke add for read NV
}

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENGTD_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_ENGTD, mmiengtd_nv_len_tab, ARR_SIZE(mmiengtd_nv_len_tab));
}

/********************************************************************************
 NAME:			MMIAPIENGTD_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENGTD_StartEngineerMenu(void)
{
	return MMK_CreateWin((uint32*)MMIENGTD_MAINMENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENGTD_InitModule(void)
{
    MMIAPIENGTD_RegNv();
    MMIENGTD_RegWinIdNameArr();
}


#else
/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENGTD_AppInit(void)
{
}

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENGTD_RegNv(void)
{
}

/********************************************************************************
 NAME:			MMIAPIENGTD_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
BOOLEAN MMIAPIENGTD_StartEngineerMenu(void)
{
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENGTD_InitModule(void)
{
}

#endif
