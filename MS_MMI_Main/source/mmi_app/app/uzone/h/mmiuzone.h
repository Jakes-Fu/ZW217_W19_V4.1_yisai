/*****************************************************************************
** File Name:      mmiuzone.h                                                *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to descirbe uzone                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica             Create
******************************************************************************/

#ifndef _MMIUZONE_H_
#define _MMIUZONE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

#include "mmk_type.h"
#include "guistring.h"

#include "mmiuzone_internal.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get the string state
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC GUISTR_STATE_T MMIUZONE_GetStringState(void);


/*****************************************************************************/
// Description : get the string state
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC GUISTR_STYLE_T MMIUZONE_GetStringStyle(void);

 /*****************************************************************************/
//  Description : release uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_ReleaseLayers(void);

 /*****************************************************************************/
//  Description : create uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
 PUBLIC BOOLEAN MMIUZONE_CreateLayers(void);


 /*****************************************************************************/
//  Description : init uzone system
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_Init(MMI_WIN_ID_T win_id);


 /*****************************************************************************/
//  Description : destroy uzone system
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_Destroy(void);


/*****************************************************************************/
//  Description : create uzone system
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_Create(MMI_HANDLE_T applet_handle);


/*****************************************************************************/
// Description : callback function of item click
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_ItemAction(uint16 item_index);

/*****************************************************************************/
//  Description : get pointer to uzone running info
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIUZONE_RUNNING_INFO* MMIUZONE_GetRunningInfo(void);



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIUZONE_H_
