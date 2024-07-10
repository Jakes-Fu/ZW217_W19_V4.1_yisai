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
#ifndef _UBOT_SCSI_SPC_H_
    #define _UBOT_SCSI_SPC_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"
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
#define UBOT_SPC_INQUIRY                   0x12  //M ANSI NCITS 301 SPC
#define UBOT_SPC_RELEASE_10                0x57  //M ANSI NCITS 301 SPC
#define UBOT_SPC_REQUEST_SENSE             0x03  //M ANSI NCITS 301 SPC
#define UBOT_SPC_RESERVE_10                0x56  //M ANSI NCITS 301 SPC
#define UBOT_SPC_SEND_DIAGNOSTIC           0x1D  //M ANSI NCITS 301 SPC
#define UBOT_SPC_TEST_UNIT_READY           0x00  //M ANSI NCITS 301 SPC

#define UBOT_SPC_MODE_SELECT_6             0x15  //O ANSI NCITS 301 SPC
#define UBOT_SPC_MODE_SELECT_10            0x55  //O ANSI NCITS 301 SPC
#define UBOT_SPC_MODE_SENSE_6              0x1A  //O ANSI NCITS 301 SPC
#define UBOT_SPC_MODE_SENSE_10             0x5A  //O ANSI NCITS 301 SPC


#define UBOT_SPC_READ_FORMAT_CAPACITIES    0x23  //M {MMC Proposed}
#define UBOT_SPC_READ_TOC_PMA_ATIP         0x43  //M {MMC Proposed}

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
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcInquiry (UBOT_CBD_U *pCBD); 
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcTestUintReady (UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcRequestSense (UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcReadTpa (UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiReadFormatCapacites (UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcModeSense6 (UBOT_CBD_U *pCBD);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSbcModeSense10(UBOT_CBD_U *pCBD);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
