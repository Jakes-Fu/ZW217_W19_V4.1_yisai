/******************************************************************************
 ** File Name:    sc6531efm_eic_cfg.h                                             *
 ** Author:       lichd                                                       *
 ** DATE:         6/14/2011                                                   *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **                                    *
 ******************************************************************************/
#ifndef _SC6531EFM_EIC_CFG_H_
#define _SC6531EFM_EIC_CFG_H_

#include "sci_types.h"
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

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
// Base address for EIC&SIC
#define EIC_DBNC_REG_BASE                    ( 0x51108500 )
#define EIC_LATCH_REG_BASE                   ( 0x8a001080 )
#define EIC_ASYNC_REG_BASE                   ( 0x8a0010A0 )
#define EIC_SYNC_REG_BASE                    ( 0x8a0010C0 )

#define EICA_DBNC_REG_BASE                   ( 0x82001200 )
#define EICA_START_NUM                       16
#define EICA_CHGR_INT                        (0 + EICA_START_NUM)
#define EICA_PBINT                           (1 + EICA_START_NUM)
#define EICA_AUD_HEAD_BUTTON               	 (2 + EICA_START_NUM)
#define EICA_CHGR_CV_STATUS                  (3 + EICA_START_NUM)
#define EICA_AUD_HEAD_INSERT                 (4 + EICA_START_NUM)
#define EICA_VCHG_OVI                        (5 + EICA_START_NUM)
#define EICA_VBAT_OVI                        (6 + EICA_START_NUM)
#define EICA_AUD_HEAD_INSERT2                (7 + EICA_START_NUM)
#define EICA_BATDET_OK                       (8 + EICA_START_NUM)
#define EICA_KEY2_7S_EXT_RSTN                (9 + EICA_START_NUM)

#define EIC_DBNC_SIZE    16
#define EIC_DBNCA_SIZE   16
#define EIC_LATCH_SIZE   8
#define EIC_ASYNC_SIZE   8
#define EIC_SYNC_SIZE    8

#define EICA_DBNC_SIZE   16
#define EICA_LATCH_SIZE  8


#define EIC_ID(x)          (x)
#define EIC_LATCH_ID(x)    (EIC_DBNC_SIZE+EIC_DBNCA_SIZE+x)

#define LATCH_UART0_RXD 0
#define LATCH_UART0_CTS 1
#define LATCH_UART1_RXD 2

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif // End _SC6531EFM_EIC_CFG_H_

