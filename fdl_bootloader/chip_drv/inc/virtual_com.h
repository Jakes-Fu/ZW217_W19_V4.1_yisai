/******************************************************************************
 ** File Name:    virtual_com.h                                                    *
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
#ifndef _VIRTUAL_COM_H_
#define _VIRTUAL_COM_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "common.h"
#include "usb_boot.h"
//#include "bsl_main.h"
#include "fdl_crc.h"
#include "sio_drv.h"
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

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct usb_handle_tag
{
    int        sio_state;
    int        pkt_state;
    int        baud_rate;
    int        buf_size;
    int        data_size;
    int        *buf_ptr;
    int        *write_ptr;
    int        *read_ptr;
} usb_handle, *usb_handle_ptr;

typedef enum
{
    USB_DOWNLOAD_OK = 0,
    USB_DOWNLOAD_TIMEOUT,
    USB_DOWNLOAD_ERROR
}USB_DOWNLOAD_STATUS_E;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
char VCOM_GetChar(USB_DOWNLOAD_STATUS_E *d_status_ptr);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_GetDevDescriptor (char bDirection,short wValue,short wIndex ,short wLen);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_VendorCommand (char bDirection,short wValue,short wIndex ,short wLen);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_SetAddress (char bDirection,short wValue,short wIndex ,short wLen);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_ClassCommand (char bDirection,short wValue,short wIndex ,short wLen);

PUBLIC uint8 *UCOM_Get_DevDesc(void);

PUBLIC uint8 *UCOM_Get_CfgDesc(void);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
