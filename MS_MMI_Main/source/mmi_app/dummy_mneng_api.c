/***************************************************************************
 ** File Name:      mneng_api.c                                            *
 ** Author:         William Qian                                           *
 ** Date:           01/08/2006                                             *
 ** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved     *
 ** Description:    It contains api realization between mmi and mntl       *
****************************************************************************
 **                        Edit History                                    *
 ** -----------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                            *
 ** 01/08/2006     William Qian     Create.                                *
****************************************************************************/

#include "mn_type.h"
//#include "scttypes.h"
//#include "ctypes.h"
//#include "gsm_gprs.h"
//#include "ps_eng.h"
//#include "layer1_engineering.h"
#include "mnclassmark_api.h"
#include "diag.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Get the phone info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E MAIN_GetPhoneInfo(
										   MN_DUAL_SYS_E		dual_sys,	//IN:
										   MN_ENG_PHONE_INFO_T	*phone_info	//OUT:
										   );

/**--------------------------------------------------------------------------*
 **                         Functions                                        *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Get the phone info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetPhoneInfo (
                            MN_ENG_PHONE_INFO_T *phone_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the phone info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
LOCAL MN_RETURN_RESULT_E MAIN_GetPhoneInfo(
										   MN_DUAL_SYS_E		dual_sys,	//IN:
										   MN_ENG_PHONE_INFO_T	*phone_info	//OUT:
										   )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the cell basic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetCellBasicInfo (
                            MN_ENG_CELL_BASIC_INFO_T *cell_basic_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the cell idle info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetCellIdleInfo (
                            MN_ENG_CELL_IDLE_INFO_T *cell_idle_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the scell control info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetSCellCtrlInfo (
                            MN_ENG_SCELL_CTRL_INFO_T *scell_ctrl_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the ncell control info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetNCellCtrlInfo (
                            MN_ENG_NCELL_CTRL_INFO_T *ncell_ctrl_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the scell traffic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetSCellTrafInfo (
                            MN_ENG_SCELL_TRAF_INFO_T *scell_traf_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the ncell traffic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetNCellTrafInfo (
                            MN_ENG_NCELL_TRAF_INFO_T *ncell_traf_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get Layer1 Monitor item 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetLayer1MonItem (
                            MN_ENG_LAYER1_MONITOR_ITEM_T *mn_layer1_mon
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the GPRS info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetGPRSInfo (
                            MN_ENG_GPRS_INFO_T *mn_gprs_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the power sweep info 
//	Global resource dependence : none
//  Author: Yu.Sun
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetPowerSweepInfo(
							MN_ENG_POWER_SWEEP_INFO_T 	*power_sweep_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : register ps diagnosis command routine
//	Global resource dependence : none
//  Author: shijun
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_RegisterCmdRoutine (
                            void
                            )
{

	return 0;
}


/*****************************************************************************/
// 	Description : Get the phone info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetPhoneInfoEx(
                            MN_DUAL_SYS_E       dual_sys,
                            MN_ENG_PHONE_INFO_T *phone_info
                            )
{

	return 0;
}
#if 0
/*****************************************************************************/
// 	Description : Get the cell basic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetCellBasicInfoEx(
                            MN_DUAL_SYS_E            dual_sys,
                            MN_ENG_CELL_BASIC_INFO_T *cell_basic_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the cell idle info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetCellIdleInfoEx(
                            MN_DUAL_SYS_E           dual_sys,
                            MN_ENG_CELL_IDLE_INFO_T *cell_idle_info
                            )
{

	return 0;
}
#endif
/*****************************************************************************/
// 	Description : Get the scell control info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetSCellCtrlInfoEx(
                            MN_DUAL_SYS_E            dual_sys,
                            MN_ENG_SCELL_CTRL_INFO_T *scell_ctrl_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the ncell control info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetNCellCtrlInfoEx(
                            MN_DUAL_SYS_E            dual_sys,
                            MN_ENG_NCELL_CTRL_INFO_T *ncell_ctrl_info
                            )
{

	return 0;
}
#if 0
/*****************************************************************************/
// 	Description : Get the scell traffic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetSCellTrafInfoEx(
                            MN_DUAL_SYS_E            dual_sys,
                            MN_ENG_SCELL_TRAF_INFO_T *scell_traf_info
                            )
{

	return 0;
}
#endif
/*****************************************************************************/
// 	Description : Get the ncell traffic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetNCellTrafInfoEx(
                            MN_DUAL_SYS_E            dual_sys,
                            MN_ENG_NCELL_TRAF_INFO_T *ncell_traf_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the GPRS info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetGPRSInfoEx(
                            MN_DUAL_SYS_E dual_sys,
                            MN_ENG_GPRS_INFO_T *mn_gprs_info
                            )
{

	return 0;
}

/*****************************************************************************/
// 	Description : Get the power sweep info 
//	Global resource dependence : none
//  Author: Yu.Sun
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNENG_GetPowerSweepInfoEx(
                            MN_DUAL_SYS_E dual_sys,
                            MN_ENG_POWER_SWEEP_INFO_T *power_sweep_info
                            )
{

	return 0;
}


#ifdef  BROWSER_SUPPORT_NONE
PUBLIC BOOLEAN MMIAPIBROWSER_IsRunning(void)
{
		return FALSE;	
}
#endif


