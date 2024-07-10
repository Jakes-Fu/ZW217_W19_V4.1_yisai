/*******************************************************************************
 ** File Name:      sc6531efm_dma_cfg.c                                          *
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

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "sci_types.h"
#include "sc6531efm_dma_cfg.h"
//#include "dma_drv_internal.h"

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

/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/
LOCAL CONST DMA_CHN_TYPE_E s_dma_chn_type[DMA_CH_NUM] =
{
    DMA_CHN_TYPE_SOFT , //channel 0  default type
    DMA_CHN_TYPE_HARD , //channel 1  default type
    DMA_CHN_TYPE_HARD , //channel 2  default type
    DMA_CHN_TYPE_HARD , //channel 3  default type
    DMA_CHN_TYPE_HARD , //channel 4  default type
    DMA_CHN_TYPE_HARD , //channel 5  default type
    DMA_CHN_TYPE_HARD , //channel 6  default type
    DMA_CHN_TYPE_HARD , //channel 7  default type
    DMA_CHN_TYPE_HARD , //channel 8  default type
    DMA_CHN_TYPE_HARD , //channel 9  default type
    DMA_CHN_TYPE_HARD , //channel 10 default type
    DMA_CHN_TYPE_HARD , //channel 11 default type
    DMA_CHN_TYPE_HARD , //channel 12 default type
    DMA_CHN_TYPE_HARD , //channel 13 default type
    DMA_CHN_TYPE_HARD , //channel 14 default type
    DMA_CHN_TYPE_HARD , //channel 15 default type
    DMA_CHN_TYPE_HARD , //channel 16 default type
    DMA_CHN_TYPE_HARD , //channel 17 default type
    DMA_CHN_TYPE_HARD , //channel 18 default type
    DMA_CHN_TYPE_HARD , //channel 19 default type
    DMA_CHN_TYPE_HARD , //channel 20 default type
    DMA_CHN_TYPE_HARD , //channel 21 default type
    DMA_CHN_TYPE_HARD , //channel 22 default type
    DMA_CHN_TYPE_HARD , //channel 23 default type
    DMA_CHN_TYPE_HARD , //channel 24 default type
    DMA_CHN_TYPE_HARD , //channel 25 default type
    DMA_CHN_TYPE_HARD ,//channel 26 default type
    DMA_CHN_TYPE_HARD  //channel 27 default type
};

LOCAL CONST DMA_CHN_REQ_INFO_T s_dma_chn_req_info[] = 
{
	{DMA_UART0_TX_CHN_REQ,	DMA_UART0_TX},
	{DMA_UART0_RX_CHN_REQ,	DMA_UART0_RX},
	{DMA_UART1_TX_CHN_REQ,	DMA_UART1_TX},
	{DMA_UART1_RX_CHN_REQ,	DMA_UART1_RX},
	{DMA_SPI0_TX_CHN_REQ,	DMA_SPI0_TX},
	{DMA_SPI0_RX_CHN_REQ,	DMA_SPI0_RX},
	{DMA_SPI1_TX_CHN_REQ,	DMA_SPI1_TX},
	{DMA_SPI1_RX_CHN_REQ,	DMA_SPI1_RX},
	{DMA_IIS0_TX_CHN_REQ,	DMA_IIS_TX},
	{DMA_IIS0_RX_CHN_REQ,	DMA_IIS_RX},
	{DMA_SIM0_TX_CHN_REQ,	DMA_SIM_TX},
	{DMA_SIM0_RX_CHN_REQ,	DMA_SIM_RX},
	{DMA_VBDA0_CHN_REQ,		DMA_VB_DA0},
	{DMA_VBDA1_CHN_REQ,		DMA_VB_DA1},
	{DMA_VBAD0_CHN_REQ,		DMA_VB_AD0},
	{DMA_VBAD1_CHN_REQ,		DMA_VB_AD1},
	{DMA_USBD0_CHN_REQ,		DMA_USB_EP0},
	{DMA_USBD1_CHN_REQ,		DMA_USB_EP1},
	{DMA_USBD2_CHN_REQ,		DMA_USB_EP2},
	{DMA_USBD3_CHN_REQ,		DMA_USB_EP3},
	{DMA_USBD4_CHN_REQ,		DMA_USB_EP4},
	{DMA_USBD5_CHN_REQ,		DMA_USB_EP5},
	{DMA_FMARK_CHN_REQ,		DMA_FMARK_LCM},
	{DMA_GEA_CHN_REQ,		DMA_GEA},
	{DMA_SDIO_WR_CHN_REQ,	DMA_SDIO_WR},
	{DMA_SDIO_RD_CHN_REQ,	DMA_SDIO_RD},
	{DMA_ROTATION_CHN_REQ,	DMA_ROTATION}
};

/*******************************************************************************
 **                        Function Definitions                                *
 ******************************************************************************/
/******************************************************************************/
//  Description:    get dma channel type
//  Global resource dependence:
//  Author:         andy.chen
//  Note:           channel:dma channel number,0-31
//  Return value:   DMA_CHN_TYPE_HARD/DMA_CHN_TYPE_SOFT
/******************************************************************************/
PUBLIC DMA_CHN_TYPE_E DMA_CFG_GetChnType (uint32 channel)
{
    return s_dma_chn_type[channel];
}
/******************************************************************************/
//  Description:    get dma channel user id
//  Global resource dependence:
//  Author:         
//  Note:           
//  Return value:   
/******************************************************************************/
PUBLIC uint32 DMA_CFG_GetMReqID(DMA_CHN_REQ_LOGIC_ID_E logic_req_id)
{
	uint32 i,size;
	
	size = sizeof(s_dma_chn_req_info)/sizeof(s_dma_chn_req_info[0]);
	
	for(i=0;i<size;i++)
	{
		if( s_dma_chn_req_info[i].logic_id == logic_req_id )
		{
			return s_dma_chn_req_info[i].req_id;
		}
	}
	
	return DMA_INVALID_REQ_ID;
}
/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif

