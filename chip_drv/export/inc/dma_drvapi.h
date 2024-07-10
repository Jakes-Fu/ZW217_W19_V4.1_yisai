/******************************************************************************
 ** File Name:      dma_drvapi.h                                                 *
 ** Author:         andy.chen                                                 *
 ** DATE:           07/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic interfaces of dma.             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/28/2010     andy.chen        Create.                                   *
 ******************************************************************************/
#ifndef _DMA_DRVAPI_H_
#define _DMA_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                          Micro Define
**---------------------------------------------------------------------------*/
//data width
typedef enum _DATAWIDTH
{
    DATAWIDTH_BYTE = 0,
    DATAWIDTH_HALFWORD,
    DATAWIDTH_WORD,
    DATAWIDTH_MAX
} DATAWIDTH_E;

/**---------------------------------------------------------------------------*
 **                      Function  Prototype
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    dma memcpy
//  Global resource dependence:
//  Author:         hao.liu
//  input param     des        : New buffer
//                  src        : Buffer to copy from
//                  count      : Number of characters to copy in bytes
//  return value               : param error means timeout
//                             : 0xffffffff means available channel
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_Memcpy (void *des, void *src, uint32 count);

/*****************************************************************************/
//  Description:    dma memset
//  Global resource dependence:
//  Author:         hao.liu
//  input param     des        : New buffer
//                  value      : poniter of value to set
//                  count      : Number of characters to copy in bytes
//  return value               : param error means timeout
//                             : 0xffffffff means available channel
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_Memset (void *des, void *value, uint32 count, DATAWIDTH_E flag);


#ifdef   __cplusplus
}
#endif

#endif //_DMA_DRVAPI_H_

/* End Of File  */


