
#ifndef  __MET_MEX_ID_H__    
#define  __MET_MEX_ID_H__

#ifdef MET_MEX_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#define WIN_ID_DEF(win_id)          win_id

	typedef enum
	{
		MET_MEX_WIN_ID_START = (MMI_MODULE_MET_MEX << 16),    

#include "met_mex_id.def"

		MET_MEX_MAX_WIN_ID
	} MET_MEX_WINDOW_ID_E;

	// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
	typedef enum
	{
		MET_MEX_CTRL_ID_ID_START = MET_MEX_MAX_WIN_ID, 

		MET_MEX_INPUT_CTRL_ID,	     //输入法的窗口界面
		MET_MEX_SSCPLATFORM_TEXTBOX_CTRL_ID,//SSC 平台的TextBox
        MET_MEX_PLATFORM_MODE_FORM_CTRL_ID,
		MET_MEX_LOG_NAME_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_LABEL_CTRL_ID, //日志
		MET_MEX_PLATFORM_ENGINEERMODE_LOG_NAME_SETLIST_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_LOG_CONTENT_LABEL_CTRL_ID,
		MET_MEX_VENDER_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_VENDER_LABEL_CTRL_ID,//渠道号
		MET_MEX_PLATFORM_ENGINEERMODE_VENDER_TEXTBOX_CTRL_ID,

		MET_MEX_PHONEUA_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_LABEL_CTRL_ID,//机型号
		MET_MEX_PLATFORM_ENGINEERMODE_PHONEUA_TEXTBOX_CTRL_ID,

		MET_MEX_SCREEN_WIDTH_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_LABEL_CTRL_ID,//屏幕宽
		MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_WIDTH_TEXTBOX_CTRL_ID,

		MET_MEX_SCREEN_HEIGHT_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_LABEL_CTRL_ID,//屏幕高
		MET_MEX_PLATFORM_ENGINEERMODE_SCREEN_HEIGHT_TEXTBOX_CTRL_ID,

		MET_MEX_APP_NAME_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_LABEL_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_APP_NAME_TEXTBOX_CTRL_ID,
		
		MET_MEX_APP_NUMBER_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_LABEL_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_APP_NUMBER_TEXTBOX_CTRL_ID,

		MET_MEX_START_TEST_FORM_CTRL_ID,
		MET_MEX_PLATFORM_ENGINEERMODE_APP_START_TEST_LABEL_CTRL_ID,
		
		MET_MEX_APP_SETNAME_CTRL_ID,

		MET_MEX_IDLE_SETTING_LIST_CTRL_ID,//idle setting list (for FAUI)
		
		MET_MEX_SELECT_SIM_CTRL_ID,//SIM_SELECT
		MET_MEX_SECOND_MENU_CTRL_ID,//mex游戏二级菜单
		MET_MEX_ABOUT_STATIC_TEXT_CTRL_ID,

	}MET_MEX_CTRL_ID_T;
	
	typedef enum
	{
		ID_MEXGAME_START,  
	    ID_MEXGAME_P15,  
	    ID_MEXGAME_ABOUT,
		
		MEXGAME_ID_MENU_MAX
	}MEXGAME_ID_MENU_E;
	
#undef WIN_ID_DEF


	/**---------------------------------------------------------------------------*
	**                         Compiler Flag                                     *
	**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // MET_MEX_SUPPORT
#endif // __MET_MEX_ID_H__

