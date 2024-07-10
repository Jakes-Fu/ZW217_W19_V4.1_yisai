/*******************************************************************************
 ** File Name:      sdslave_phy.h                                              *
 ** Author:         andy.chen                                                  *
 ** Date:           12/29/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.12        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/
#ifndef __SDSLAVE_PHY_H__
#define __SDSLAVE_PHY_H__

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "sci_types.h"

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
//***********************************
//  Debug Function
//***********************************
#define SDSLAVE_PRINT
//#define SDSLAVE_PRINT SCI_TRACE_LOW
#define SDSLAVE_ASSERT SCI_ASSERT

//***********************************
//  IRQ Function
//***********************************
#define SDSLAVE_DISABLE_IRQ SCI_DisableIRQ
#define SDSLAVE_ENABLE_IRQ SCI_RestoreIRQ

// NOTE: WR/RD here means master wr/rd to/from slave
#define SIG_WR_TRANS_CMP        BIT_0
#define SIG_RD_TRANS_CMP        BIT_1
#define SIG_DMA_INT             BIT_2
#define SIG_BUF_RD_RDY          BIT_3
#define SIG_BUF_WR_RDY          BIT_4
#define SIG_WRITE_START         BIT_5
#define SIG_READ_START          BIT_6
#define SIG_OCR_SET             BIT_7
#define SIG_TRANS_CMP           (SIG_WR_TRANS_CMP|SIG_RD_TRANS_CMP)

#define SIG_ERR_CMDCRC          BIT_16
#define SIG_ERR_CMDILL          BIT_17
#define SIG_ERR_DATACRC         BIT_18
#define SIG_ERR_AHB             BIT_19

#define SIG_SDIO_ERR            (SIG_ERR_CMDCRC|SIG_ERR_CMDILL|SIG_ERR_DATACRC|SIG_ERR_AHB)
#define SIG_STATE_ERR           BIT_31

//SDIO OCR VOTAGE BIT
#define SDIO_OCR_20V            (BIT_8)         // 2.0V
#define SDIO_OCR_21V            (BIT_8|BIT_9)   // 2.1V
#define SDIO_OCR_22V            (BIT_9|BIT_10)  // 2.2V
#define SDIO_OCR_23V            (BIT_10|BIT_11) // 2.3V
#define SDIO_OCR_24V            (BIT_11|BIT_12) // 2.4V
#define SDIO_OCR_25V            (BIT_12|BIT_13) // 2.5V
#define SDIO_OCR_26V            (BIT_13|BIT_14) // 2.6V
#define SDIO_OCR_27V            (BIT_14|BIT_15) // 2.7V
#define SDIO_OCR_28V            (BIT_15|BIT_16) // 2.8V
#define SDIO_OCR_29V            (BIT_16|BIT_17) // 2.9V
#define SDIO_OCR_30V            (BIT_17|BIT_18) // 3.0V
#define SDIO_OCR_31V            (BIT_18|BIT_19) // 3.1V
#define SDIO_OCR_32V            (BIT_19|BIT_20) // 3.2V
#define SDIO_OCR_33V            (BIT_20|BIT_21) // 3.3V
#define SDIO_OCR_34V            (BIT_21|BIT_22) // 3.4V
#define SDIO_OCR_35V            (BIT_22|BIT_23) // 3.5V
#define SDIO_OCR_36V            (BIT_23)        // 3.6V

// transfer mode
// BLK/BYTE MODE
#define SDIO_BLK_MODE           1
#define SDIO_BYTE_MODE          0
/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
struct SDSLAVE_PHY_TAG;
typedef struct SDSLAVE_PHY_TAG *SDSLAVE_PHY_HANDLE;
typedef void (*SDSLAVE_CALLBACK) (uint32 msg);

typedef enum SDSLAVE_RST_TYPE_E_TAG
{
    RST_SDIO,
    RST_AHB,
    RST_ALL
}
SDSLAVE_RST_TYPE_E;


/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/

/*******************************************************************************
 **                        Function Definitions                                *
 ******************************************************************************/
PUBLIC void SDSLAVE_PHY_SetOCRReady (SDSLAVE_PHY_HANDLE handle);
PUBLIC uint32 SDSLAVE_PHY_GetOCRVaue (SDSLAVE_PHY_HANDLE handle);
PUBLIC SDSLAVE_PHY_HANDLE SDSLAVE_PHY_Init (SDSLAVE_CALLBACK fun);
PUBLIC void SDSLAVE_PHY_Deinit (SDSLAVE_PHY_HANDLE handle);
PUBLIC void SDSLAVE_PHY_RST (SDSLAVE_PHY_HANDLE handle, SDSLAVE_RST_TYPE_E rst_type);
PUBLIC void SDSLAVE_PHY_CfgVoltage (SDSLAVE_PHY_HANDLE handle, uint32 vol);
PUBLIC void SDSLAVE_PHY_PowrOnOff (SDSLAVE_PHY_HANDLE handle, BOOLEAN is_on);
PUBLIC void SDSLAVE_PHY_SetDmaAddr (SDSLAVE_PHY_HANDLE handle, uint32 addr);
PUBLIC uint32 SDSLAVE_PHY_GetBlkcnt (SDSLAVE_PHY_HANDLE handle);
PUBLIC uint32 SDSLAVE_PHY_GetBlklen (SDSLAVE_PHY_HANDLE handle);
PUBLIC uint32 SDSLAVE_PHY_GetTransMode (SDSLAVE_PHY_HANDLE handle);
PUBLIC void SDSLAVE_PHY_SetCardInt (SDSLAVE_PHY_HANDLE handle);

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif

#endif  //__SDSLAVE_PHY_H__

