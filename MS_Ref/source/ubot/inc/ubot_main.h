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
 ** 3/25/2005      Daniel.Ding     Create.                                    *
 ******************************************************************************/
#ifndef _UBOT_MAIN_H_
    #define _UBOT_MAIN_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "ubot_common.h"
#include "gadget.h"
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
typedef struct fsg_dev {
	/* lock protects: state, all the req_busy's, and cbbuf_cmnd */
	USB_GADGET_T	*gadget;

	USB_EP_T		*ep0;		// Handy copy of gadget->ep0
	USB_REQUEST_T	*ep0req;	// For control responses

	USB_REQUEST_T	*outreq;	// For bulk out responses
	USB_REQUEST_T	*inreq;		// For bulk in responses

	USB_REQUEST_T	*intreq;	// For interrupt responses
	struct fsg_buffhd	*intr_buffhd;

	USB_EP_T		*bulk_in;
	USB_EP_T		*bulk_out;
	USB_EP_T		*intr_in;
}FSG_DEV_T;
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This is USLA's main body .
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_init (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_exit (void);

PUBLIC uint32 UBOT_Ep0Queue(FSG_DEV_T *fsg);

extern uint8 g_UBOT_MaxLUN [];
extern UBOT_MANAGER_T g_UBOT_manager ;
//extern UBOT_FUNCTION g_UBOT_function[];


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
