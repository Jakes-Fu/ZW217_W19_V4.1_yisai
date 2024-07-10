/******************************************************************************
 ** File Name:      fm_cfg.c                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           04/19/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of FM*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2006     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 



/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_customize_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "fm_cfg.h"

#if defined (FM_SUPPORT_CL6012X)
	#include "fm_cl6012x.h"
#endif

#if defined(ATV_SUPPORT_NMI601) && defined(FM_SUPPORT_NMI601)
    extern PUBLIC FM_OPERATION_T g_NMI600_ops;
#endif

#if defined(FM_SUPPORT_BK1080)
    extern PUBLIC FM_OPERATION_T g_BK1080_ops;
#endif

#if defined (FM_SUPPORT_SPRD_V0)
extern PUBLIC FM_OPERATION_T g_fm_sprd_ops;
#endif

#if defined(FM_SUPPORT_AUTO)
    extern PUBLIC FM_OPERATION_T g_BK1080_ops;
    extern PUBLIC FM_OPERATION_T g_fm_sprd_ops;
#endif

//#if defined (PLATFORM_SC6531EFM)
extern  FM_OPERATION_T s_fm_operations_def;
//#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#if defined(FM_SUPPORT_BK1080) || defined(FM_SUPPORT_AUTO)
#define FM_CFG_I2C_PORT     4
#else
#define FM_CFG_I2C_PORT     0 
#endif
/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 // Customer need to add FM probe into below table
LOCAL const FM_OPERATION_T * FM_OPS_TAB[]=
{
#if defined (FM_SUPPORT_CL6012X)
    &g_CL6012X_ops,
#endif

#if defined(ATV_SUPPORT_NMI601) && defined(FM_SUPPORT_NMI601)
	&g_NMI600_ops,
#endif

#if defined(FM_SUPPORT_BK1080)
    &g_BK1080_ops,
#endif

#if defined (FM_SUPPORT_SPRD_V0)
    //&g_fm_sprd_ops,
    &s_fm_operations_def,
#endif

#if defined(FM_SUPPORT_AUTO)
    &g_BK1080_ops,
    &g_fm_sprd_ops,
#endif


	PNULL					// END	
};
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to get FM information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC FM_OPERATION_T ** FM_GetOpsTab(void)
{
	return (FM_OPERATION_T **)FM_OPS_TAB;
}

/*****************************************************************************/
//  Description:    This function is used to get FM information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_GetOpsTabLen(void)
{
	return (uint32)(sizeof(FM_OPS_TAB)/sizeof(FM_OPERATION_T*));
}

/*****************************************************************************/
//  Description:    get the ctrl port between FM and BB control path
//  Author:         
//  Note:           it is i2c port id right now
/*****************************************************************************/
PUBLIC uint8 FM_GetCtrlPort(void)
{
    uint8 port = FM_CFG_I2C_PORT;
    //SCI_TRACE_LOW:"FM_GetCtrlPort %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_CFG_94_112_2_18_0_26_25_33,(uint8*)"d", port);
    return port;
}

/*****************************************************************************/
//  Description:    function  used to set fm clk
//  Author:
//  Note:           SCI_TRUE: set clk  , SCI_FALSE: resore clk
/*****************************************************************************/
#if !defined (FM_SUPPORT_SPRD_V0) // it means we didnot use SPRD internal FM, use external FM chip
#include "pinmap.h"
#include "sc_reg.h"/*lint -esym(766, chip_drv\export\inc\outdated\sc_reg.h)*/
LOCAL uint32 reg_value;
#endif
PUBLIC void FM_ConfigClk (BOOLEAN param)
{
    // device use nmi601, Config at nmiport.c
#if  0  //(!defined (FM_SUPPORT_SPRD_V0)) // it means we didnot use SPRD internal FM, use external FM chip
	if(param)
	{	
		reg_value = *(volatile uint32*)PIN_IISMCK_REG;	
		*(volatile uint32*)PIN_IISMCK_REG = ( PIN_SLP_O | PIN_SPX_EN | PIN_SPO_X | PIN_FUNC_1 | PIN_FPX_EN | PIN_DS_1 | PIN_FPC_EN);
	}
	else
	{
		*(volatile uint32*)PIN_IISMCK_REG = reg_value;
	}
      SCI_TRACE_LOW("FM_ConfigClk %x",*(volatile uint32*)PIN_IISMCK_REG);
#endif
}

/*****************************************************************************/
//  Description:    function  used to set fm power
//  Author:
//  Note:           /*SCI_TRUE: enable power  , SCI_FALSE: disable power*/
/*****************************************************************************/
PUBLIC void FM_ConfigPower (BOOLEAN param)
{
    // device use nmi601, Config at nmiport.c
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  
