/******************************************************************************
**  File Name:      mmiatv_menutable.h                                        *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_MENUTABLE_H_
#define _MMIATV_MENUTABLE_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmiatv_text.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _TABLEID,
    
typedef enum
{
    MMIATV_MENU_LABEL_START = (MMI_MODULE_ATV << 16),
            
#include "mmiatv_menu_wintable.def"
        
    MMIATV_MENUTABLE_MAX
} MMIATV_MENU_LABEL_E;

#undef MENU_DEF    

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register atv menu group
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_RegMenuWin(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIATV_MENUTABLE_H_ */
#endif


