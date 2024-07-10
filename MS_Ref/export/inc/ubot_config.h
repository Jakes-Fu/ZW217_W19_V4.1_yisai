/******************************************************************************
 ** File Name:    common.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _UBOT_CONFIG_H_
    #define _UBOT_CONFIG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "ubot_common.h"
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
#define UBOT_DISK_CNT_MAX			4

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct
{
	PUSB_DEVICE_DESCRIPTOR_T 				pdev_desc;
	PUBOT_CONFIG_DESC_T		 				pubot_cfg_desc;
	PUSB_STRING_LANGUAGE_DESCRIPTOR_T		pusb_str_lang_desc;
	PUSB_STRING_MANUFACTURER_DESCRIPTOR_T	pusb_str_manf_desc;
	PUSB_STRING_PRODUCT_DESCRIPTOR_T		pusb_str_prod_desc;
	PUSB_STRING_SERIALNUMBER_DESCRIPTOR_T	pusb_str_serialnum_desc;	
	uint32									disk_count;	
	PUBOT_DEVICE_INF_T						pdisk_info;
	
}UBOT_CFG_INFO_T, *PUBOT_CFG_INFO_T;
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN   ubot_config_init();
/*****************************************************************************/
//  Description:    
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC void    ubot_config_exit();
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_CustomerConfig (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_SetDiskHidden (uint8 *dev_name, BOOLEAN state);

/*****************************************************************************/
//  Description:    Function get the disk hidden state
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_GetDiskHiddenState (uint8 *dev_name);

/*****************************************************************************/
//  Description:    Get configure information 
//  Global resource dependence: 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC PUBOT_CFG_INFO_T UBOT_GetCfgInfo(void);

/*****************************************************************************/
//  Description:    Set configure descriptor according to speed 
//  Global resource dependence: 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void UBOT_SetCfgDescriptor(uint8 IsFS);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
