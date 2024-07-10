/******************************************************************************
 ** File Name:      bt_cfg.c                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           03/13/2008                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of BT    *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/13/2008     Liangwen.Zhen    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/


#include "os_api.h"
#include "bt_abs.h"
#include "priority_system.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                             *
 **---------------------------------------------------------------------------*/
#ifdef BT_SHARE_XTL_SUPPORT
LOCAL BOOLEAN share_crystal = TRUE;
#else
LOCAL BOOLEAN share_crystal = FALSE;
#endif
/***************************************************************************
 
    Function Name:
    Description:  surpport share crystal or not
 
 
    Note:         
    Author:       paul.luo
    Date:       5.21.2009
****************************************************************************/

PUBLIC BOOLEAN   BT_GetCrystCfg(void)
{
    return share_crystal;   
}
/*****************************************************************************/
//  Description:    This function is used to get BT configure information
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void BT_GetProdConfig(BT_CONFIG *config)
{ 
    config->port_num = BT_COM1;
    config->baud_rate = BT_BAUD_3250000;
    
    /*config csr chip pcm_sync pcm_clk*/
    config->pcm_config.config = 0x08a00004; //pcm_sync=8KHz, pcm_clk = 128KHz, Salve
    
    /*config csr chip pcm format*/
    config->pcm_config.format = 0x60;  //16 bit
    
    /*config pcm slot*/
    config->pcm_config.slot_number = 0x0000;  //slot 1
    
    /*config bt crystal*/
    config->bt_crystal_freq = 26000; //26 MHz  
    
   /*Task Priority Config*/
	config->bt_task_priority = PRI_BT_TASK;

}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of bt_cfg.c

