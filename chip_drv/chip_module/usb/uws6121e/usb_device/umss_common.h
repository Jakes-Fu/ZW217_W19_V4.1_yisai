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
#ifndef _UBOT_COMMON_H_
#define _UBOT_COMMON_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "sci_types.h"


#include "os_api.h"
#include "usb200.h"
#include "usb_drv.h"
#include "bsd.h"
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
//"USB Mass Storage Class Bulk Only Transport 1.0"

#define NV_IMEI_ID			5
#define MAX_ID_LENGTH 		9
#define MAX_IMEI_LENGTH		(MAX_ID_LENGTH - 1)

#define USB_STR_MANUFACTURER_DESCRIPTOR_MIN 24
#define USB_STR_SERIALNUMBER_DESCRIPTOR_MIN 24

#define UMSS_SCSI_VENDER_STR_LEN        8
#define UMSS_SCSI_PRODUCT_ID_STR_LEN    16

#define UMSS_MAX_DISK_NUM 		4

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/


typedef enum {
    UMSS_DEV_NO_PRESENT,
	UMSS_DEV_HIDDEN,
    UMSS_DEV_PRESENT,
    UMSS_DEV_OPENED,
    UMSS_DEV_CLOSE
}UMSS_DEV_STATE_U;


typedef struct _umss_device{
    uint8               *dev_name;
    const uint8         *vendor;
    const uint8         *product;
    BSD_HANDLE          dev_handle;
    uint32              page_size;
    uint32              page_number;
    uint32              cache_number; /*cache number, 0: no cache*/
    UMSS_DEV_STATE_U    dev_state;
	uint8    			is_Hidden;

}UMSS_DEV_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC UMSS_DEV_T *  umss_CustomerConfig (void);
PUBLIC BOOLEAN UBOT_SetDiskHidden (uint8 *dev_name, BOOLEAN state);
PUBLIC BOOLEAN UBOT_GetDiskHiddenState (uint8 *dev_name);
PUBLIC void  umss_CustomerDeconfig (void);
PUBLIC UMSS_DEV_T * umss_GetActiveDiskDev (void);
PUBLIC uint8 umss_GetActiveDiskNum (void);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
