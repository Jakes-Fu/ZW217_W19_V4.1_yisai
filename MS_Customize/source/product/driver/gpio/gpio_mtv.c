/******************************************************************************
 ** File Name:      pinmap_sp7320.c                                          *
 ** Author:         Richard.Yang                                              *
 ** DATE:           03/08/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the pin usage and initiate value of     *
 **                 SP7100B                                                   *
 **                GPIO used information( SP7100B ), please see following     *
 **                GPIO define                                                *     
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2004     Richard.Yang     Create.                                   *
 ** 03/18/2004     Lin.liu          Modify for SP7100B                        *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "gpio_drv.h"
#include "cmddef.h"
#include "tb_dal.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "scm_api.h"
#include "upm_api.h"
#include "lcd_backlight.h"
#include "deep_sleep.h"
#include "ref_outport.h"
#include "kd.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


extern KDuint32 DTL_LCM_GPIO;
BOOLEAN _GPIO_PROD_SetVal(GPIO_PROD_ID_E id, BOOLEAN value);

/*****************************************************************************/
//  Description:    mtv int enable
//  Input      :    
//  Return     :    None
//  Author     :    hanbing.zhang
//	Note       :
/*****************************************************************************/
void GPIO_MTVIntEnable(KDboolean is_enable)
{
    GPIO_EnableIntCtl(DTL_LCM_GPIO);
}
 
/*****************************************************************************/
//  Description:    mtv reset
//  Input      :
//  Return     :    None
//  Author     :    hanbing.zhang
//	Note       :
/*****************************************************************************/
void GPIO_MTVReset(KDboolean is_tvreset)
{
    _GPIO_PROD_SetVal(GPIO_PROD_MTV_RESET_ID, (KDboolean)!is_tvreset);
}

/*****************************************************************************/
//  Description:    mtv standby
//  Input      :
//  Return     :    None
//  Author     :    hanbing.zhang
//	Note       :
/*****************************************************************************/
void GPIO_MTVStandby(KDboolean is_tvstby)
{
    _GPIO_PROD_SetVal(GPIO_PROD_MTV_STANDBY_ID, (KDboolean)!is_tvstby);
}

/*****************************************************************************/
//  Description:    mtv lcd bypass
//  Input      :
//  Return     :    None
//  Author     :    hanbing.zhang
//	Note       :
/*****************************************************************************/
void GPIO_MTVLCDBypass(KDboolean is_bypass)
{
    _GPIO_PROD_SetVal(GPIO_PROD_MTV_LCDBYPASS_ID, (KDboolean)!is_bypass);
}

/*****************************************************************************/
//  Description:    mtv send int
//  Input      :
//  Return     :    None
//  Author     :    hanbing.zhang
//	Note       :
/*****************************************************************************/
void GPIO_MTVSendInt(void)
{
    _GPIO_PROD_SetVal(GPIO_PROD_MTV_INT_OUT_ID, 1);
    _GPIO_PROD_SetVal(GPIO_PROD_MTV_INT_OUT_ID, 0);
}

/*****************************************************************************/
//  Description:    mtv softswitch control
//  Input      :
//  Return     :    None
//  Author     :    hanbing.zhang
//	Note       :
/*****************************************************************************/
void GPIO_MTVSoftSwitchON(KDboolean is_on)
{
    _GPIO_PROD_SetVal(GPIO_PROD_MTV_SOFTSWITCH_ID, (KDboolean)!is_on); //0µ¼Í¨

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

// End of tb_comm.c
