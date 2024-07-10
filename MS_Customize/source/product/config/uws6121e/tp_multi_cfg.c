/******************************************************************************
 ** File Name:      tp_multi_cfg.c                                            *
 ** DATE:           2011.03.19                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.03.19                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#ifdef __cplusplus
extern   "C" 
{
#endif

#include "sci_types.h"
#include "os_api.h"

#include "tp_multi_drv.h"
#if defined (CAP_TP_SUPPORT_FT5206)
#include "tp_ft5206.h"
#endif
#if defined (CAP_TP_SUPPORT_FT6336)
#include "tp_ft6336.h"
#endif
#if defined (CAP_TP_SUPPORT_BL6133)
extern  TPC_OPERATION_T tpc_bl6133_ops;
#endif


/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define TPM_I2C_PORT    4

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
LOCAL const TPC_OPERATION_T * TPC_OPS_TAB[]=
{
#if defined (CAP_TP_SUPPORT_FT5206)
    &tpc_ft5206_ops,
#endif
#if defined (CAP_TP_SUPPORT_FT6336)
    &tpc_ft6336_ops,
#endif
#if defined (CAP_TP_SUPPORT_BL6133)
    &tpc_bl6133_ops,
#endif
    PNULL
};
/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Name:           TPC_GetOpsTab
//  Description:    This function is used to get capactive tp information table    
//  Note:           
/*****************************************************************************/
PUBLIC TPC_OPERATION_T ** TPC_GetOpsTab(void)
{
    return (TPC_OPERATION_T **)TPC_OPS_TAB;
}

/*****************************************************************************/
//  Name:           TPC_GetOpsTabLen
//  Description:    This function is used to get capactive tp information table    
//  Note:           
/*****************************************************************************/
PUBLIC uint32 TPC_GetOpsTabLen(void)
{
	return (uint32)(sizeof(TPC_OPS_TAB)/sizeof(TPC_OPERATION_T*));
}


/*****************************************************************************/
//  Name:           TPC_GetCtrlPort
//  Description:    get the ctrl port between Capacitve tp and BB control path
//  Note:           it is i2c port id right now
//                   if on the 8800g platform, SDA/SCL1 is gpio86,gpio87
/*****************************************************************************/
PUBLIC uint8 TPC_GetCtrlPort(void)
{
    uint8 port = TPM_I2C_PORT; //  use port2 (SDA/SCL 1), simulate GPIO
    //SCI_TRACE_LOW:"TPM_GetCtrlPort %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_MULTI_CFG_82_112_2_18_0_27_15_214,(uint8*)"d", port);
    return port;
}



#ifdef __cplusplus
}
#endif

/******************************************************************************/

