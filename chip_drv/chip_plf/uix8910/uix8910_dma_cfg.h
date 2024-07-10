/*******************************************************************************
 ** File Name:      sc6531efm_dma_cfg.h                                          *
 ** Author:         andy.chen                                                  *
 ** Date:           08/20/2010                                                 *
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
 ** 2010.08        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/


#ifndef __UIX8910_DMA_CFG_H__
#define __UIX8910_DMA_CFG_H__

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "../export/inc/dma_drv_internal.h"
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
// DMA MAX CH NUMBER
#if defined(FPGA_SUPPORT_UIX8910)
#define DMA_CH_NUM                      3
#else
#define DMA_CH_NUM                      6
#endif
#define DMA_CH_FULLTYPE_NUM                      0

#define DMA_SOFT_BLOCK_WAITTIME         (0x0f)
#define DMA_HARD_BLOCK_WAITTIME         (0x0f)
#define DMA_MAX_TRANSSIZE               (1<<25)    //2^25byte
#define DMA_MAX_BURSTSIZE               (1<<16)    //2^16byte
#define DMA_LIST_SIZE_MAX               (10)//max length used in both linklist and soflist mode

// DMA user id
#define DMA_SOFT0                      0x00
#define DMA_UART0_RX                   (0x00)
#define DMA_UART0_TX                   (0x01)
#define DMA_UART1_RX                   (0x02)
#define DMA_UART1_TX                   (0x03)
#define DMA_UART2_RX                   (0x04)
#define DMA_UART2_TX                   (0x05)
#define DMA_ZSPUART_RX               (0x06)
#define DMA_ZSPUART_TX               (0x07)

#define DMA_IIS_TX                     0x05
#define DMA_IIS_RX                     0x06

#define DMA_SIM_TX                    0x07
#define DMA_SIM_RX                    0x08
#define DMA_VB_AD0                    0x09
#define DMA_VB_AD1                    0x0A
#define DMA_VB_DA0                    0x0B
#define DMA_VB_DA1                    0x0C

#define DMA_USB_EP0                   0x0D
#define DMA_USB_EP1                   0x0E
#define DMA_USB_EP2                   0x0F
#define DMA_USB_EP3                   0x10
#define DMA_USB_EP4                   0x11
#define DMA_USB_EP5                   0x12

#define DMA_SPI0_TX                   0x13
#define DMA_SPI0_RX                   0x14
#define DMA_SPI1_TX                   0x15
#define DMA_SPI1_RX                   0x16

#define DMA_GEA                       0x17

#define DMA_SDIO_WR                   0x18
#define DMA_SDIO_RD                   0x19

#define DMA_FMARK_LCM                 0x1A
#define DMA_ROTATION                  0x1B


#define DMA_INVALID_REQ_ID				0xFFFFFFFF


// endian issue
#define DMA_ENDIAN_SELECT


/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
//dma channel type

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/

/*******************************************************************************
 **                        Function prototype                                  *
 ******************************************************************************/
/******************************************************************************/
//  Description:    get dma channel type
//  Global resource dependence:
//  Author:         andy.chen
//  Note:           channel:dma channel number,0-31
//  Return value:   DMA_CHN_TYPE_HARD/DMA_CHN_TYPE_SOFT
/******************************************************************************/
PUBLIC DMA_CHN_TYPE_E DMA_CFG_GetChnType (uint32 channel);

/******************************************************************************/
//  Description:    get dma channel user id
//  Global resource dependence:
//  Author:         
//  Note:           
//  Return value:   
/******************************************************************************/
PUBLIC uint32 DMA_CFG_GetMReqID(DMA_CHN_REQ_LOGIC_ID_E logic_req_id);

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif  //__SC6531EFM_DMA_CFG_H__
// End

