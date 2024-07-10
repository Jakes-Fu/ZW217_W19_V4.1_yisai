/******************************************************************************
 ** File Name:    UBOT_scsi_sbc.h                                             *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file include basic micro and data prototype base on    *
 **               <Information Technology SCSI-3 Block Commands (SBC) 1.2>    *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _UCOM_UBOT_SCSI_SBC_H_
#define _UCOM_UBOT_SCSI_SBC_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

#include "usb_drv.h"
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
// SCSI-3 Block Commands (SBC) 65
//D -               DIRECT ACCESS DEVICE (SBC) Device column key
//.T -              SEQUENTIAL ACCESS DEVICE (SSC) blank = reserved
//. L -             PRINTER DEVICE (SSC) not blank = allowed
//. .P -            PROCESSOR DEVICE (SPC)
//. . W -           WRITE ONCE READ MULTIPLE DEVICE (SBC)
//. . .R -          CD DEVICE (MMC)
//. . . S -         SCANNER DEVICE (SGC)
//. . . .O -        OPTICAL MEMORY DEVICE (SBC)
//. . . . M -       MEDIA CHANGER DEVICE (SMC)
//. . . . .C -      COMMUNICATION DEVICE (SSC)
//. . . . . A -     STORAGE ARRAY DEVICE (SCC)
//. . . . . .E -    ENCLOSURE SERVICES DEVICE (SES)
//DTLPWRSOMCAE Description

#define UCOM_UBOT_SBC_CHANGE_DEFINITION         0x40  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_COMPARE                   0x39  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_COPY                      0x18  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_COPY_AND_VERIFY           0x3A  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_ERASE_10                  0x2C  //O 6.2.1
#define UCOM_UBOT_SBC_ERASE_12                  0xAC  //O 6.2.2
#define UCOM_UBOT_SBC_FORMAT_UNIT               0x04  //O 6.1.1
#define UCOM_UBOT_SBC_INQUIRY                   0x12  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_LOCK_UNLOCK_CACHE         0x36  //O 6.1.2
#define UCOM_UBOT_SBC_LOG_SELECT                0x4C  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_LOG_SENSE                 0x4D  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_MEDIUM_SCAN               0x38  //O 6.2.3
#define UCOM_UBOT_SBC_MODE_SELECT_6             0x15  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_MODE_SELECT_10            0x55  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_MODE_SENSE_6              0x1A  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_MODE_SENSE_10             0x5A  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_MOVE_MEDIUM               0xA5  //O SMC
#define UCOM_UBOT_SBC_OBSOLETE_01               0x01  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_31               0x31  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_b1               0xB1  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_30               0x30  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_b0               0xB0  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_32               0x32  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_b2               0xB2  //OB 3.3.4
#define UCOM_UBOT_SBC_Obsolete_0b               0x0B  //OB 3.3.4
#define UCOM_UBOT_SBC_PERSISTENT_IN             0x5E  //O1 ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_PERSISTENT_OUT            0x5F  //O1 ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_PRE_FETCH                 0x34  //O 6.1.3
#define UCOM_UBOT_SBC_PREVENT_ALLOW_MEDIUM_REMOVAL \
                                           0x1E  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_READ_6                    0x08  //O 6.1.4
#define UCOM_UBOT_SBC_READ_10                   0x28  //M 6.1.5
#define UCOM_UBOT_SBC_READ_12                   0xA8  //O 6.2.4
#define UCOM_UBOT_SBC_READ_BUFFER               0x3C  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_READ_CAPACITY             0x25  //M 6.1.6
#define UCOM_UBOT_SBC_READ_DEFECT_DATA_10       0x37  //O 6.1.7
#define UCOM_UBOT_SBC_READ_DEFECT_DATA_12       0xB7  //O 6.2.5
#define UCOM_UBOT_SBC_READ_ELEMENT_STATUS       0xB8  //O SMC
#define UCOM_UBOT_SBC_READ_GENERATION           0x29  //O 6.2.6
#define UCOM_UBOT_SBC_READ_LONG                 0x3E  //O 6.1.8
#define UCOM_UBOT_SBC_READ_UPDATED_BLOCK        0x2D  //O 6.2.7
#define UCOM_UBOT_SBC_REASSIGN_BLOCKS           0x07  //O 6.1.9
#define UCOM_UBOT_SBC_RECEIVE_DIAGNOSTIC_RESULTS \
                                                0x1C  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_RELEASE_6                 0x17  //O2 ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_RELEASE_10                0x57  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_REQUEST_SENSE             0x03  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_RESERVE_6                 0x16  //O2 ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_RESERVE_10                0x56  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_SEEK_10                   0x2B  //O 6.1.12
#define UCOM_UBOT_SBC_SEND_DIAGNOSTIC           0x1D  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_SET_LIMITS_10             0x33  //O 6.1.13
#define UCOM_UBOT_SBC_SET_LIMITS_12             0xB3  //O 6.2.8
#define UCOM_UBOT_SBC_START_STOP_UNIT           0x1B  //O 6.1.14
#define UCOM_UBOT_SBC_SYNCHRONIZE_CACHE         0x35  //O 6.1.15
#define UCOM_UBOT_SBC_TEST_UNIT_READY           0x00  //M ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_UPDATE_BLOCK              0x3D  //O 6.2.9
#define UCOM_UBOT_SBC_VERIFY_10                 0x2F  //O 6.2.10
#define UCOM_UBOT_SBC_VERIFY_12                 0xAF  //O 6.2.11
#define UCOM_UBOT_SBC_WRITE_6                   0x0A  //O 6.1.17
#define UCOM_UBOT_SBC_WRITE_10                  0x2A  //M 6.1.18
#define UCOM_UBOT_SBC_WRITE_12                  0xAA  //O 6.2.13
#define UCOM_UBOT_SBC_WRITE_AND_VERIFY_10       0x2E  //O 6.2.14
#define UCOM_UBOT_SBC_WRITE_AND_VERIFY_12       0xAE  //O 6.2.15
#define UCOM_UBOT_SBC_WRITE_BUFFER              0x3B  //O ANSI NCITS 301 SPC
#define UCOM_UBOT_SBC_WRITE_LONG                0x3F  //O 0
#define UCOM_UBOT_SBC_READ_TOC_PMA_ATIP         0x43  //M {MMC Proposed}

#define UCOM_UBOT_SBC_MAX                       0xFF  
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
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcRead10 (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcWrite10 (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcReadCapacity (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcVerify10 (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcMediaRemove (UCOM_UBOT_CBD_U *pCBD);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcStartStop (UCOM_UBOT_CBD_U *pCBD);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
