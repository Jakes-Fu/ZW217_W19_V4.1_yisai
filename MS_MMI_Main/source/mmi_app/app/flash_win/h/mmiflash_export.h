/****************************************************************************
** File Name:      mmiflash_export.h                                                *
** Author:                                                                 *
** Date:           27/05/2011                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011       jesse.yu         Create
** 
****************************************************************************/
#ifndef  _MMI_FLASH_EXPORT_H_    
#define  _MMI_FLASH_EXPORT_H_   


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "Sci_types.h"
#include "Mmk_type.h"
#include "caf.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define FLASH_IDLE_ITEM_STRING_LEN_MAX 40

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

typedef struct
{
    wchar *     wstr_ptr;
    uint16      wstr_len; 
    BOOLEAN     is_display;
}FLASH_IDLE_ITEM_T;


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
PUBLIC void MMIAPIFlash_UpdateIdleItem(void);

PUBLIC uint8* MMIAPIFlash_GetFlashLayerBufPtr(void);

PUBLIC BOOLEAN MMIAPIFlash_FlashIsVisiable(void);

PUBLIC MMI_RESULT_E MMIAPIFlashWin_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

PUBLIC FLASH_IDLE_ITEM_T * MMIAPIFlash_GetFlashIdleItem(void);

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIFlash_UpdateIdleItem         MMIAPIFlash_UpdateIdleItem
#define MMIFlash_GetFlashLayerBufPtr    MMIAPIFlash_GetFlashLayerBufPtr
#define MMIFlash_FlashIsVisiable        MMIAPIFlash_FlashIsVisiable
#define MMIFlashWin_HandleMsg           MMIAPIFlashWin_HandleMsg
#define MMIFlash_GetFlashIdleItem       MMIAPIFlash_GetFlashIdleItem

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

