/****************************************************************************
** File Name:      mmisrv_adapt.h                                               *
** Author:                                                                 *
** Date:           07/26/2012                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the mmi service adapt     *
**                 (为了不破坏mmisrv模块独立性，mmisrv中调用的上层模块某些难以归属的函数声明）                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/26/2012        bin.ji       Create
** 
****************************************************************************/

#ifndef _MMISRV_ADAPT_H_
#define _MMISRV_ADAPT_H_

#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_font.h"
#include "mmisrv_adapt_gui.h"
#include "mmi_base_common.h"

//typedef uint32 MMISRV_HANDLE_T;


/*****************************************************************************/
//  Description : to get the text by label and language
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
extern void MMI_GetLabelTextByLang( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   );

/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
extern const wchar* MMIRES_GetText(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   );    //[out] the data of text 

/*****************************************************************************/
//  Description : get text's letters
//  Global resource dependence : none
//  Author: 
//  Note:   
/*****************************************************************************/
extern void MMIAPIIM_GetTextLetters(uint16 text_unicode, uint16 *letters_ptr);

/*****************************************************************************/
//  Description : get the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
extern void MMIAPISET_GetLanguageType(
    MMISET_LANGUAGE_TYPE_E  *language_type_ptr
    );

#ifndef WIN32
extern uint32 SCI_MEMMOVE(void * _dest_ptr, void * _src_ptr, uint32 _size);
#endif

#ifdef SXH_APPLE_SUPPORT
void APPLE_SRVMGR_Init(void);
#endif


#endif

