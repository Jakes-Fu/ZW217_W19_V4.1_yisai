/******************************************************************************
 ** File Name:    usb_drv.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _USB_DRV_H_
#define _USB_DRV_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

//#include "sci_types.h"
#include "os_api.h"
//#include "usb200.h"
//#include "isr_drvapi.h"
#include "ucom_api.h"
//#include "tb_hal.h"
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
#define USB_SPREADTRUM_VENDER_ID            0x1782
#define USB_SPREADTRUM_CHIP_ID              0x4d00
#define USB_SPREADTRUM_PID_TWOINTERFACE     0x3d00
#define USB_SPREADTRUM_VCOM_MASS_STORAGE_ID        0x3d01  // at+diag+udisk
#define USB_SPREADTRUM_CDROM_MODEM_ID       0x3d02  // modem + cdrom
#define USB_SPREADTRUM_DATA_DEBUG_MODEM_ID  0x3d03  // modem + cp trace + diag + at
#define USB_SPREADTRUM_CALIB_ID             0x4d02  // calib u2s diag

#define USB_SPREADTRUM_MASS_STORAGE_ID    0x4d01
#define USB_SPREADTRUM_VCOM_ID            0x4d00
#define USB_SPREADTRUM_PC_CAMERA_ID       0x4d04
//
#define USB_VID_SPRD (0x1782)
#define USB_VID_RDA_DOWNLOAD (0x0525)

#define USB_PID_RDA_DOWNLOAD (0xa4a7)
#define USB_PID_SERIALS (0x4d10)
#define USB_PID_RNDIS_AND_SERIALS (0x4d11)
#define USB_PID_NPI_SERIAL (0x4d12)
#define USB_PID_ECM_AND_SERIALS (0x4d13)
#define USB_PID_ECM_ACM_SERIAL (0x4d15)

//UWS6121E
#define USB_PID_6121E_SERIALS (0x4d17)
#define USB_PID_6121E_ACM_SERIALS (0x4d18)
#define USB_PID_6121E_RNDIS_AND_SERIALS (0x4d19)
#define USB_PID_6121E_ECM_AND_SERIALS (0x4d20)
#define USB_PID_6121E_NPI_SERIAL (0x4d30)
//

/**
 * usb debounce time in milliseconds before enumarating
 */
#define CONFIG_USB_DETECT_DEBOUNCE_TIME 600

enum usb_detect_mode
{
    USB_DETMODE_CHARGER = 0,
    USB_DETMODE_AON = 1,
};
//#define USB_MICROSOFT_PID_TWOINTERFACE    0x0001    // at, diag

#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
#define USB_WORD_SWAP_ENDIAN(x)   ((x)&0xFFFF)
#else
#define USB_WORD_SWAP_ENDIAN(x)   (((((x) & 0x0FF) << 8) | (((x) >> 8) & 0x0FF))&0xFFFF) /*lint !e572*/
#endif

#define USB_DWORD_SWAPENDIAN(x){\
        (((x & 0x0FF00)<< 8)|((x & 0x0FF)<<24) |\
         ((x & 0xFF000000)>>24)|((x & 0xFF0000)>>8))}


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
#if 0
typedef enum
{
    USB_SUCCESS = 0,
    USB_CONNECTING,
    USB_OTHER
} USB_ERROR_E;

typedef union
{
    struct
    {
#ifdef CHIP_ENDIAN_LITTLE
        uint   index   :8;
        uint   type    :8;
#else
        uint   type    :8;
        uint   index   :8;
#endif
    } bmDescriptor;
    short wValue;
} USB_HWORD_VALUE_U;

typedef enum
{
    USB_DEFAULT_POWER = 0,
    USB_BUS_POWER     = 0,
    USB_SELF_POWER    = 1
} USB_POWER_E;

typedef enum
{
    USB_ERR_INACK_TIMEOUT   = 0x0,
    USB_ERR_INVALID_CLASSCMD,
    USB_ERR_INVALID_VENDORCMD,
    USB_ERR_BSD_GETDEV_INFO,
    USB_ERR_BSD_OPENDEV    ,
    USB_ERR_BSD_CLOSEDEV   ,
    USB_ERR_PARAM_ERR ,
    USB_ERR_MAX
} USB_ERROR_TYPE_E;
#endif

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
PUBLIC void drvUsbInit(void);
PUBLIC void drvUsbEnable(uint32 debounce);
PUBLIC void drvUsbSetAttach(uint32 attach);
PUBLIC void UCOM_SetWorkMode(uint8 service_type, uint8 calibmode);
PUBLIC BOOLEAN drvUsbIsConnected(void);
/** 
 *@brief usb resetart
*/
void drvUsbRestart();
/**

 * @brief usb stop
 */
void drvUsbStop();



/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
