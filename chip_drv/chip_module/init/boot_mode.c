/******************************************************************************
 ** File Name:      boot_mode.c                                               *
 ** Author:         Jeff Li                                                   *
 ** DATE:           31/08/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the logic interfaces called during boot,*
 **                 including reset mode setting, initialization etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 31/08/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "os_param.h"
#include "cmddef.h"
#include "chip_internal.h"
#include "threadx_assert.h"
#include "sci_log.h"
#include "tb_drv.h"
#include "iram_mgr.h"
#include "boot_drvapi.h"
#include "mmu_drvapi.h"
#if defined (PLATFORM_SC6800H) ||defined (PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "analog_drvapi.h"
#include "../analog/analog_phy.h"
#include "../chip/chip_phy.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
extern const uint32 g_exception_mode_flag;

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN wdg_debug_mode = SCI_FALSE;

#ifdef FPGA_VERIFICATION
LOCAL uint32 fpga_rest_mode = 0;
#endif

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function handles several kinds of watchdog reset occ-
//                  asions such as MCU-reset, system halted, and makes a dist-
//                  inction between debug mode and release mode.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
LOCAL void _WDGResetHandler ();
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function handles 7S software reset.
// Author :
// Note :
/*****************************************************************************/
LOCAL void _7sResetHandler ();
#endif

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function handles several kinds of watchdog reset occ-
//                  asions such as MCU-reset, system halted, and makes a dist-
//                  inction between debug mode and release mode.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
LOCAL void _WDGResetHandler ()
{
    uint8 assert_mode;

    if (CHIP_PHY_IsWDGRstByMCU (g_exception_mode_flag))
    {
        // In case of watchdog reset caused by MCU-reset, return directly
        return;
    }
    else
    {
        // In case of watchdog reset caused by system-halted, set the corresp-
        // onding mode
        BOOT_SetResetMode (WATCHDOG_RESET_MODE);
    }

    OSPARAM_GetAssertMode (&assert_mode);

    if (SCI_ASSERT_DEBUG_MODE != assert_mode)
    {
        return;
    }
    else
    {
        MMU_Enable();

        SetWatchdogMode (SCI_TRUE);
        SCI_InitLogSwitch (SCI_FALSE);
        SCI_EnterAssertMode ();

        CHIP_PHY_WDGAssertInit ();

        SCI_PASSERT (0, ("watchdog timeout restart assert %d", BOOT_GetResetMode()));/*assert verified*/
    }
}

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function handles 7S software reset.
// Author :
// Note :
/*****************************************************************************/
LOCAL void _7sResetHandler ()
{
	uint8 assert_mode;

	CHIP_PHY_Clear7SRstSWFlag();

	BOOT_SetResetMode (SEVENSEC_RESET_MODE);

	OSPARAM_GetAssertMode (&assert_mode);
       return;//bringup_tmp,for debug
       #if 0
	if (SCI_ASSERT_DEBUG_MODE != assert_mode)
	{
		return;
	}
	else
	{
		MMU_Enable();

		SetWatchdogMode (SCI_TRUE);
		SCI_InitLogSwitch (SCI_FALSE);
		SCI_EnterAssertMode ();

		CHIP_PHY_WDGAssertInit ();

		SCI_PASSERT (0, ("7s PowerKey Reset assert %d", BOOT_GetResetMode()));/*assert verified*/
	}
       #endif
}
#endif

/*****************************************************************************/
//  Description:    Necessary operation for chip initialization at the reset
//                  point.
//  Author:         Jeff.Li
//  Note:           It is called before __main
/*****************************************************************************/
PUBLIC void __BOOT_InitModule ()
{
    CHIP_PHY_BootInitModule ();
}

/*****************************************************************************/
//  Description :    This function switches IRAM control to ARM.
//  Author :         Jeff.Li
//  Note :           It is called before __main
/*****************************************************************************/
PUBLIC void __BOOT_IRAM_EN ()
{
    CHIP_PHY_BootIramEn ();
}

/*****************************************************************************/
//  Description:    Pure assert interface. Assert when software reset by set-
//                  ting PC to 0 unexpectly.
//  Author:         Jeff.Li
//  Note:           It is called before __main
/*****************************************************************************/
PUBLIC void __PureAssert (void)
{
    SCI_PASSERT (0, ("Unexpectly PC set to 0"));/*assert verified*/
}

#ifdef ARM_CORE_POWER_DOMAIN
/*****************************************************************************/
//  Description:    Arm mode should have svc mode when pc to 0.
//
//  Author:         henry.he
//  Note:           It is called before __main
/*****************************************************************************/
PUBLIC void __ArmModeAssert (void)
{
    SCI_PASSERT (0, ("ARM should have svc mode"));/*assert verified*/
}
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function handles several kinds of reset occasions such
//                  as watchdog reset, power up reset.
// Author :         Jeff.Li
// Note :           It is called before __main
/*****************************************************************************/
PUBLIC void __BOOT_ResetHandler (void)
{

#if !defined(FPGA_SUPPORT_UIX8910)    /*KU095*/
    if (CHIP_PHY_IsResetByExtReset())
    {
    	CHIP_PHY_ClearExtRstFlag();
        BOOT_SetResetMode (EXTERNAL_RESET_MODE);
    }
    else if (CHIP_PHY_IsResetByWatchDog ())
    {
        _WDGResetHandler();
    }
	else if (CHIP_PHY_IsResetByHW7S())
	{
		CHIP_PHY_Clear7SRstSWFlag();
		//BOOT_SetResetMode (SEVENSEC_RESET_MODE);
		CHIP_PHY_SetHwRstFlag_InBoot(SEVENSEC_RESET_MODE);
	}
	else if (CHIP_PHY_IsResetBySW7S())
    {
        _7sResetHandler();
    }
    else
    {
        //reset by power up
        return;
    }
#else
    //reset by power up
    return;
#endif
}
#else
/*****************************************************************************/
// Description :    This function handles several kinds of reset occasions such
//                  as watchdog reset, power up reset.
// Author :         Jeff.Li
// Note :           It is called before __main
/*****************************************************************************/
PUBLIC void __BOOT_ResetHandler (void)
{
    if (CHIP_PHY_IsResetByPowerUp ())
    {
        // In case of power up reset, return directly
        return;
    }
    else if (CHIP_PHY_IsResetByExtReset())
    {
        BOOT_SetResetMode (EXTERNAL_RESET_MODE);
    }
    else if (CHIP_PHY_IsResetByWatchDog ())
    {
        _WDGResetHandler();
    }
    else
    {
        // It is impossible to enter this branch
    }
}
#endif

/*****************************************************************************/
//  Description:    After normal power on, the HW_RST flag should be reset in
//                  order to judge differrent reset conditions between normal
//                  power on reset and watchdog reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_ResetHWFlag (void)
{
    CHIP_PHY_ResetHWFlag (g_exception_mode_flag);
}

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetWDGHWFlag (WDG_HW_FLAG_T type)
{
    return CHIP_PHY_GetWDGHWFlag (type);
}

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
    CHIP_PHY_SetWDGHWFlag (type, val);
}

/*****************************************************************************/
//  Description:    After normal power on, the HW_RST flag should be reset in
//                  order to judge differrent reset conditions between normal
//                  power on reset and watchdog reset.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_InitResetMode (void)
{
    CHIP_PHY_ResetHWFlag (g_exception_mode_flag);
}

/*****************************************************************************/
//  Description:   Get reset mode
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetResetMode (void)
{
#ifdef FPGA_VERIFICATION
	return fpga_rest_mode;
#else
    return CHIP_PHY_GetWDGHWFlag (TYPE_RESET);
#endif
}

/*****************************************************************************/
//  Description:    Set reset mode
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetResetMode (uint32 val)
{
#ifdef FPGA_VERIFICATION
	fpga_rest_mode = val;
#else
    CHIP_PHY_SetWDGHWFlag (TYPE_RESET, val);
#endif
}

/*****************************************************************************/
//  Description:   Get BL status flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetBLStatus (void)
{
#if defined (PLATFORM_SC8800G)

    return CHIP_PHY_GetWDGHWFlag (TYPE_BACKLIGHT);

#elif defined (PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)

#ifndef FPGA_VERIFICATION
    return ANA_GetLCMBackLightStatus();
#else
    return 0;
#endif

#else

    if(SC6800H == CHIP_GetChipType())  // 6800H and following chip
    {
        return FALSE;
    }
    else  // 6800H3
    {
        return ANA_GetLCMBackLightStatus();
    }

#endif
}

/*****************************************************************************/
//  Description:    Set BL status flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetBLStatus (uint32 val)
{
#if defined(PLATFORM_SC8800G) || defined(PLATFORM_UWS6121E)
    CHIP_PHY_SetWDGHWFlag (TYPE_BACKLIGHT, val);
#endif
}

/*****************************************************************************/
//  Description:   Get DSP exception status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetDSPExceptionStatus (void)
{
    return CHIP_PHY_GetWDGHWFlag (TYPE_DSPEXCEPTION);
}

/*****************************************************************************/
//  Description:    Set DSP exception status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetDSPExceptionStatus (uint32 val)
{
    CHIP_PHY_SetWDGHWFlag (TYPE_DSPEXCEPTION, val);
}

/*****************************************************************************/
//  Description:   Get USB connect status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetUSBConnectStatus (void)
{
    return CHIP_PHY_GetWDGHWFlag (TYPE_USB);
}

/*****************************************************************************/
//  Description:    Set USB connect status Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetUSBConnectStatus (uint32 val)
{
    CHIP_PHY_SetWDGHWFlag (TYPE_USB, val);
}


/*****************************************************************************/
//  Description:    This fuction returns the HW_RST value written before reset.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
//PUBLIC uint32 BOOT_GetHWFlag (void)
//{
//    return CHIP_PHY_GetHWFlag ();
//}

/*****************************************************************************/
//  Description:    Gets the flag.
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetWatchdogMode (void)
{
    return wdg_debug_mode;
}

/*****************************************************************************/
//  Description:    Sets the flag when watchdog resets the system in debug mode.
//  Author:
/*****************************************************************************/
PUBLIC void SetWatchdogMode (BOOLEAN mode)
{
    wdg_debug_mode = mode;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
