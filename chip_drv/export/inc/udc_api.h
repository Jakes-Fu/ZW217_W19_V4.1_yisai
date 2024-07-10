/******************************************************************************
 ** File Name:    UPM_api.h                                                    *
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
#ifndef _UDC_API_H_
#define _UDC_API_H_
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
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
// USB EPx ID
#define USB2011_EP0_IN                      0
#define USB2011_EP0_OUT                     1

#if !defined(PLATFORM_UWS6121E)

#define USB_EP1                         2
#define USB_EP2                         3
#define USB_EP3                         4
#define USB_EP4                         5
#define USB_EPx_NUMBER                  6

#endif // !PLATFORM_UWS6121E


enum sx_udc_plug_e
{
    USB_VBUS_PLUG_IN,
    USB_VBUS_PLUG_OUT,
    USB_HOST_PLUG_IN,
};

typedef enum sx_udc_cmd_e
{
    SX_UDC_P_ENABLE     = 1,    /* Pull-up enable        */
    SX_UDC_P_DISABLE    = 2,    /* Pull-up disable       */
    SX_UDC_P_RESET      = 3,    /* UDC reset, in case of */
} SX_UDC_CMD_E;


typedef struct sx_udc_mach_info
{
    void (*udc_command) (SX_UDC_CMD_E);
    void (*vbus_draw) (uint32 ma);
    uint32 vbus_pin;
    uint32 vbus_pin_inverted;
} SX_UDC_MACH_INFO_T;

#define	  ENABLE_NONE						(0)
#define		ENABLE_EPIN_NAKINT		(BIT_0)
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Call this function to set current USB connect state ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/

PUBLIC uint32  UDC_Init (void);

PUBLIC uint32 UDC_Exit (void);

PUBLIC void UDC_ClkReset (void);

PUBLIC uint32 UDC_GetConnect (void);

PUBLIC uint32 UDC_GetHostReq (void);

PUBLIC void UDC_PowerSet (uint32 power);

PUBLIC void UDC_Config (void);







PUBLIC BOOLEAN USB_DisableAllEndPointDMA (void);

PUBLIC void UDC_Reset (void);

PUBLIC void UDC_UCOMIntCtrl (uint32 endp_num, BOOLEAN is_able, uint32 ep_type);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
