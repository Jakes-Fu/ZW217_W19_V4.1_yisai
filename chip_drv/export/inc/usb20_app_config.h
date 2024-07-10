/******************************************************************************
 ** File Name:    usb_app_config.h                                                    *
 ** Author:       jiayong.yang                                                 *
 ** DATE:         8/31/2010                                                    *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 8/31/2010     jiayong.yang    Create.                                     *
 ******************************************************************************/
#ifndef _USB_APP_CONFIG_H_
#define _USB_APP_CONFIG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

#include "os_api.h"
#include "isr_drvapi.h"
#include "sc_reg.h"


//#include "tb_hal.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
typedef int (* USB_GetDescripotr) (void *setup_, void *buf, unsigned size);
typedef void (* USB_InterfaceCommand) (void *setup);
typedef uint32 (* USB_GetAppCfgDescriptor) (uint8 **para);
typedef void (* USB_SetConfig) (uint8 num);

typedef struct _usb_app_operation_t
{
    USB_GetDescripotr                get_descriptor;
    USB_InterfaceCommand     app_command;
    USB_GetAppCfgDescriptor  get_app_configure_descriptor;
    USB_SetConfig            set_config;
} USB_APP_OP_T;

typedef struct _usb_endp_cfg
{
    uint8 ep_address;
    uint8 ep_type;
    uint16 mps;
    uint32 res2;
} USB_EPCfg_T;

typedef enum
{
    USB_APP_NONE   = 0,
    USB_APP_UCOM   = 1,
    USB_APP_UDISK  = 2,
    USB_APP_VCOM   = 3,
    USB_APP_UPCC   = 4,
    USB_APP_MTP    = 5,
    USB_APP_MAX    = 6
} USB_APP_E;

PUBLIC void USB_SetService (USB_APP_E app_type,void *para);
PUBLIC void usb_parse_configuration (uint8 IsFS);
PUBLIC void UBOT_StartRecv (uint8 ep_id,uint32 *buffer,uint32 len);
PUBLIC uint32 *USB_GetSetupBuffer (void);
PUBLIC void usb_reset_handler (void);
PUBLIC BOOLEAN usb20_continue_transfer (uint8 epno);
PUBLIC void USB_SetEPBusy (uint8 epno);
PUBLIC uint32 usb_GetOutEPMask (uint8 type);
PUBLIC USB_APP_OP_T *USB_GetCurrentAppOperation (void);
PUBLIC USB_EPCfg_T *USB_GetEPCFG (void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
