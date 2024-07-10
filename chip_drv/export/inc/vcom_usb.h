/******************************************************************************
 ** File Name:    vcom_usb.h                                                    *
 ** Author:       weihua.wang                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005      weihua.wang     Create.                                     *
 ******************************************************************************/
#ifndef _VCOM_USB_H_
#define _VCOM_USB_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"

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
#define VCOM_DEV_NO_PRESENT (0x00)
#define VCOM_DEV_PRESENT    (0x01)
#define VCOM_DEV_READY      (0x02)
#define VCOM_DEV_BUSY       (0x03)
#define VCOM_DEV_MAX        (0x04)
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
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_usb_boot (void);

/*****************************************************************************/
//  Description:    complete the handshake and receive data handle
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           -1:got none from fifo, else return the received data
/*****************************************************************************/
PUBLIC int32 VCOM_GetChar (void);

/*****************************************************************************/
//  Description:    pull down usb ldo
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_disconnect (void);

/*****************************************************************************/
//  Description:    get usb virtual com state
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       none
//  Note: ret value:VCOM_DEV_NO_PRESENT
//                  VCOM_DEV_PRESENT
//                  VCOM_DEV_READY
//                  VCOM_DEV_BUSY
/*****************************************************************************/
PUBLIC uint32 VCOM_GetDevState (void);

/*****************************************************************************/
//  Description:    put char to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint8 nchar: char to be sent
//  Note:           when this func is called, it's indicated that current irs is disable
//                  , no interrupt can happen,
/*****************************************************************************/
PUBLIC void VCOM_PutTxChar (uint8 nchar);

/*****************************************************************************/
//  Description:    put chars to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint8* data_ptr: chars to be sent
//                  int32 len: total data size to be sent
//  Note:           called when dump memory only! to reduce time, 0x7e will be convert here
/*****************************************************************************/
PUBLIC void VCOM_PutTxChars (uint8* data_ptr, int32 len);

/*****************************************************************************/
//  Description:    indicating send char will be finished, remain char should be flushed
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_Flush (void);
PUBLIC void VCOM_Open(BOOLEAN ready);
#if VCOM1_SUPPORT
PUBLIC void VCOM1_PutTxChars (uint8* data_ptr, int32 len);
PUBLIC void VCOM1_Flush (void);
PUBLIC uint32 VCOM1_GetDevState (void);
PUBLIC void VCOM1_Open(BOOLEAN ready);
PUBLIC void VCOM1_PutTxChar(uint8 nchar);
#endif

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
