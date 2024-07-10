/****************************************************************************
** File Name:      mmiacc_winmain.c                            
** Author:                                                                 
** Date:            2004/08/24
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to process msg to main window of accessory
**                      including
                         long distande area code serching, 
                         calendar, 
                         simulating file system. 
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 08/24/2004     taul.zhu          Create
** 06/02/2006     bruce.chi			modify
****************************************************************************/
#define _MMIACC_WINMAIN_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "window_parse.h"
#include "mmiidle_export.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"/*lint -e766*/
#include "mmi_menutable.h"
//#include "mmiacc_winmain.h"
//#include "mmialarm_export.h"
#include "mmiwclk_export.h"
#include "mmi_nv.h"
//#include "mmigame.h"
#ifdef GAME_SUPPORT
#include "mmigame_export.h"
#endif
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
//#include "win.def"
#include "mmi_textfun.h"
#include "mmitv_out.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_appmsg.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"

#include "mmiudisk_internal.h"

#include "mmiacc.h"
#include "mmiacc_id.h"
#include "mmiacc_event.h"
//#include "mmiacc_image.h"
//#include "mmiacc_text.h"
#include "mmiacc_menutable.h"
#include "mmiacc_nv.h"


#include "mmifm_export.h"

#include "mmibt_export.h"// baokun add

#ifdef BLUETOOTH_SUPPORT
//#include "mmibt_menutable.h"
#endif

#include "mmi_event_api.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_steps.h"
#include "watch_heartrate.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/	

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                           */
/*---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                       function bodies  -- global
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:      MMIAPIACC_Init
// 	Description : init data used in accessory
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIACC_Init(void)
{
    BOOLEAN result = TRUE;
	
    result &= MMIAPIWCLK_Init();
#ifdef GAME_SUPPORT
    result &=  MMIAPIGAME_InitSetting();
#endif
	
	result &= MMIFW_SetTimeUpdateListerner(MMIALM_TimeUpdateCallback, TRUE);

    return result;
}

/*****************************************************************************/
//  FUNCTION:      MMIAPIACC_InitModule
// 	Description :
//	Global resource dependence : 
//  Author:        haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIACC_InitModule(void)
{
    MMIACC_RegMenuGroup();  
    MMIACC_RegNv();   
    MMIACC_RegWinIdNameArr();
}
