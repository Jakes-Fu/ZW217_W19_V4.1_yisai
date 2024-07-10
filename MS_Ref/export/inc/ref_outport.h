/******************************************************************************
 ** File Name:    ref_outport.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2006                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2006      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _REF_OUTPORT_H_
    #define _REF_OUTPORT_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
//REF ERROR CODE
#define REF_AT_BASE         0X80000000
#define REF_AT_INVLID_PARA  REF_AT_BASE + 1
#define REF_AT_INVLID_CMD   REF_AT_BASE + 2


  typedef enum
{
    SEND_PACKET_END = 0x00,
    SEND_PACKET_CUSTOM0,

    SEND_PACKET_CUSTOM1
} SEND_PACKET;

typedef enum
{
    REF_IOCTL_SET_CHIP_SDRAM_CLK,
    REF_IOCTL_SET_RFOUTPUT,
    REF_IOCTL_GET_RFOUTPUT,
    REF_IOCTL_SET_DC_DSPTICK,
    REF_IOCTL_SET_DC_CAPSOF,
    REF_IOCTL_CHK_DC_CAPTICK,

} REF_IOCTL_CMD_E;

#ifdef RF_HW_SELFADAPTIVE_ENABLE
typedef enum
{
    PH_HW_VER_00,
    PH_HW_VER_01,
    PH_HW_VER_02,
    PH_HW_VER_03,
    PH_HW_VER_04,
    PH_HW_VER_05,
    PH_HW_VER_06,
    PH_HW_VER_07,
    PH_HW_MAX
}PH_HW_VERSION;
#endif

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function delay some ticks .
// Global resource dependence :
// Author :         Daniel.ding
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_TickDelay(uint32 ticks);

/*****************************************************************************/
//  Description:    OS_CheckStackRegion
//  Global resource dependence: 
//  Author:        Younger.yang
//  Note:           This function  provide system stack region
/*****************************************************************************/

BOOLEAN OS_CheckStackRegion(uint32 addr);

/*****************************************************************************/
//  Description:    OS_CheckCodeRegion
//  Global resource dependence: 
//  Author:        Younger.yang
//  Note:           This function  provide system code region
/*****************************************************************************/
BOOLEAN OS_CheckCodeRegion(uint32 addr);

/*****************************************************************************/

 /*****************************************************************************/
// Description : This function send a char to uart or usb virtual com at assert debug
// Global resource dependence :
// Author :      weihua.wang
// Note :           
/*****************************************************************************/
PUBLIC void OS_DebugPutChar(uint8 ch);

/******************************************************************************/
// Description:   Handler AT Command
// Dependence:    None
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC uint32 REF_HandleATCommand(uint8* at_string);

/*****************************************************************************/
// Description : This function send a char to uart or usb virtual com at assert debug
// Global resource dependence :
// Author :      weihua.wang
// Note :           
/*****************************************************************************/
PUBLIC void OS_DebugPutChar(uint8 ch);

/*****************************************************************************/
// Description : This function scanf a char from uart receive register at debug.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC int OS_DebugGetChar(void);
 
/*****************************************************************************/
//  Description:    The OS_DumpPrint function is used to print string by 
//                  serial port.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC  void OS_DumpFlush(void);
PUBLIC void XF_SavePrintTraceToMem(
    char * string,
    uint32 size,
    uint32 type    
);

/******************************************************************************/
// Description:   REF_SetModeForRatSwitch
// Dependence:    None
// Note: 
//     net_mode: 0  -2G,    1   -3G
/******************************************************************************/

PUBLIC uint32 REF_SetModeForRatSwitch(uint32 net_mode);
/*****************************************************************************/
//  Description:    REF_RestoreFactorySetting
//  Global resource dependence: 
//  Author:        
//  Note:          add for REF reset during mmi reset factory setting 
/*****************************************************************************/

PUBLIC uint32 REF_RestoreFactorySetting(void);
/*****************************************************************************/
//  Description: REF_ioctl
//  Note:        data_ptr should be ready before call it       
/*****************************************************************************/
PUBLIC uint32 REF_ioctl(uint32 cmd, void * data_ptr);

PUBLIC void REF_7sResetEnable(BOOLEAN enable);

typedef enum
{
    OSI_RESET_NORMAL,
    OSI_RESET_FORCE_DOWNLOAD,
    OSI_RESET_CALIBRATION_MODE,
} osiResetMode_t;

void osiReset(osiResetMode_t mode);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
