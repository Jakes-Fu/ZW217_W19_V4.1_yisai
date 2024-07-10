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
#ifndef _UCOM_UBOT_SCSI_SPC_H_
#define _UCOM_UBOT_SCSI_SPC_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"
#include "ucom_common.h"
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
#define UCOM_UBOT_SPC_INQUIRY                   0x12  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_RELEASE_10                0x57  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_REQUEST_SENSE             0x03  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_RESERVE_10                0x56  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_SEND_DIAGNOSTIC           0x1D  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_TEST_UNIT_READY           0x00  //M ANSI NCITS 301 SPC

#define UCOM_UBOT_SPC_MODE_SELECT_6             0x15  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_MODE_SELECT_10            0x55  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_MODE_SENSE_6              0x1A  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SPC_MODE_SENSE_10             0x5A  //O ANSI NCITS 301 SPC


#define UCOM_UBOT_SPC_READ_FORMAT_CAPACITIES    0x23  //M {MMC Proposed}
#define UCOM_UBOT_SPC_READ_TOC_PMA_ATIP         0x43  //M {MMC Proposed}

#define UCOM_UBOT_MMC_REPORT_KEY         	   0xA4  //M {MMC Proposed}
#define UCOM_UBOT_MMC_GET_CONFIGURATION         0x46  //M {MMC Proposed}
#define UCOM_UBOT_MMC_GET_EVENT_STATUS     	   0x4A  //M {MMC Proposed}
#define UCOM_UBOT_MMC_READ_SUB_CHANNEL          0x42  //M {MMC Proposed}
#define UCOM_UBOT_MMC_READ_DISK_INFORMATION    0x51  //M {MMC Proposed}
#define UCOM_UBOT_MMC_GET_PERFORMANCE   0xAC  //M {MMC Proposed}
#define UCOM_UBOT_MMC_SYNC_CACHE   0x35  //M {MMC Proposed}

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
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcInquiry (UCOM_UBOT_CBD_U *pCBD); 
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcTestUintReady (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcRequestSense (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcReadTpa (UCOM_UBOT_CBD_U *pCBD);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReportKey (UCOM_UBOT_CBD_U *pCBD);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiGetConfiguration (UCOM_UBOT_CBD_U *pCBD);


/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiGetEventStatus (UCOM_UBOT_CBD_U *pCBD);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReadSubChannel (UCOM_UBOT_CBD_U *pCBD);


/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReadDiskInformation (UCOM_UBOT_CBD_U *pCBD);


/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiGetPerformance (UCOM_UBOT_CBD_U *pCBD);


/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSyncCache (UCOM_UBOT_CBD_U *pCBD);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReadFormatCapacites (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcModeSense6 (UCOM_UBOT_CBD_U *pCBD);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcModeSense10(UCOM_UBOT_CBD_U *pCBD);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
