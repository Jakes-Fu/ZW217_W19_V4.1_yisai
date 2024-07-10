/******************************************************************************
 ** File Name:    usb20_drvapi.h                                                 *
 ** Author:       Xuepeng Zhang                                               *
 ** DATE:         10/11/2010                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 10/11/2010    Xuepeng Zhang     Create.                                     *
 ******************************************************************************/
#ifndef _USB20_DRVAPI_H_
#define _USB20_DRVAPI_H_
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

typedef struct usb_transfer {
    void (*callback) (uint8 ep, struct usb_transfer *xfer);
    void *ctx;
    void *buf;
    uint32 length;
    uint32 actual;
    int status;
    uint8 zlp;
} usb_xfer_t;

/**
 * @brief udc polling mode start transfer
 *
 * @param ep_   the endpoint address
 * @param xfer  the usb transfer struct, caller should keep the memory
 * @return
 *      - (-1)  fail
 *      - 0     success
 */
int udc_polling_start_xfer(uint8 ep_, usb_xfer_t *xfer);

/**
 * @brief udc polling mode cancel a usb transfer
 *
 * @param ep_   the endpoint address
 * @param xfer  the usb transfer is transfering
 */
void udc_polling_cancel_xfer(uint8 ep_, usb_xfer_t *xfer);

/**
 * @brief polling the usb controller to process interrupt
 */
void udc_polling_handler();

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

