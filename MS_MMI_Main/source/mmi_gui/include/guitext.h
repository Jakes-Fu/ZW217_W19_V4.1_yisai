#ifndef _GUITEXT_H_
#define _GUITEXT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guictrl.h"
#include "guiprgbox.h"
#include "mmitheme_text.h"

#include "ctrltext_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUITEXT_GetString               CTRLTEXT_GetString
#define GUITEXT_SetString               CTRLTEXT_SetString
#define GUITEXT_SetRect                 CTRLTEXT_SetRect
#define GUITEXT_SetMargin               CTRLTEXT_SetMargin
#define GUITEXT_SetMarginEx             CTRLTEXT_SetMarginEx
#define GUITEXT_SetLineSpace            CTRLTEXT_SetLineSpace
#define GUITEXT_SetBorder               CTRLTEXT_SetBorder
#define GUITEXT_SetBg                   CTRLTEXT_SetBg
#define GUITEXT_SetFont                 CTRLTEXT_SetFont
#define GUITEXT_GetCurTag               CTRLTEXT_GetCurTag
#define GUITEXT_SetTagType              CTRLTEXT_SetTagType
#define GUITEXT_SetAlign                CTRLTEXT_SetAlign
#define GUITEXT_SetAutoScroll           CTRLTEXT_SetAutoScroll
#define GUITEXT_SetCircularHandle       CTRLTEXT_SetCircularHandle
#define GUITEXT_SetHandleTpMsg          CTRLTEXT_SetHandleTpMsg
#define GUITEXT_IsDisplayPrg            CTRLTEXT_IsDisplayPrg
#define GUITEXT_IsSlide                 CTRLTEXT_IsSlide
#define GUITEXT_SetClipboardEnabled     CTRLTEXT_SetClipboardEnabled
#define GUITEXT_GetPosInfo              CTRLTEXT_GetPosInfo
#define GUITEXT_GetHighlightInfo        CTRLTEXT_GetHighlightInfo
//zmt add start
#define GUITEXT_SetResetTopDisplay      CTRLTEXT_SetResetTopDisplay
//zmt add end
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
