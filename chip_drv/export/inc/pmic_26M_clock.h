/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _PMIC_26M_CLOCK_H_
#define _PMIC_26M_CLOCK_H_

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define PMIC_26M_USED_BY_ADC            (BIT_0)
#define PMIC_26M_USED_BY_AUDIO          (BIT_1)
#define PMIC_26M_USED_BY_WCN            (BIT_2)

// =============================================================================
//  TYPES
// =============================================================================


PUBLIC void PMIC_26M_status_init (void);
PUBLIC void PMIC_26M_open (uint32 open_flag);
PUBLIC void PMIC_26M_close (uint32 close_flag);
PUBLIC void PMIC_26M_get_status(
    uint32         *ana_ctrl_reg32, 
    uint32         *rf_ana_26M_ctrl,
    uint32         *status);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif

#endif

