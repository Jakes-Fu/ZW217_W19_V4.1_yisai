/*! @file guiref_scale.h
@brief  This file is used to describe t9 text input
@author Jassmine
@version 1.0
@date 2011/02/18
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  2/2007       Jassmine           Create
*******************************************************************************/

/*! @page page_Ref_scale Ref_scale

(暂无)

*******************************************************************************/

/*addtogroup ref_scaleGrp Ref_scale
@brief ref_scale模块
@details ref_scale模块
@{
*******************************************************************************/

#ifndef _GUIREF_SCALE_H_    
#define _GUIREF_SCALE_H_   

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guilcd.h"
#include "guiblock.h"

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
//tv out时需要刷新的block,多种组和之间用或(|)连接
/*! @def GUIREF_SCALE_BLOCK0
@brief 
*/
#define GUIREF_SCALE_BLOCK0         0x01  //0000 0001
/*! @def GUIREF_SCALE_BLOCK1
@brief 
*/
#define GUIREF_SCALE_BLOCK1         0x02  //0000 0010
/*! @def GUIREF_SCALE_BLOCK2
@brief 
*/
#define GUIREF_SCALE_BLOCK2         0x04  //0000 0100

/*---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief when tv out, get block buffer by block id before scaling down
@author Jassmine
@param lcd_id   [in] 
@param block_id [in] 
@return 
@note 
*******************************************************************************/
uint32 GUIREF_GetBlockBufferBeforeScale(
                                        GUI_LCD_ID_E    lcd_id,  //in
                                        GUIBLOCK_ID_E   block_id //in
                                        );


/***************************************************************************//*!
@brief when tv out, set block buffer by block id before scaling down
@author Jassmine
@param lcd_id            [in] 
@param block_id          [in] 
@param block_buffer_addr [in] 
@return 
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIREF_SetBlockBufferBeforeScale(
                                                LCD_ID_E        lcd_id,             //in
                                                GUIBLOCK_ID_E   block_id,           //in
                                                uint32          block_buffer_addr   //in
                                                );


/***************************************************************************//*!
@brief when tv out, clear osd output buffer
@author Jassmine
@param color_key [in] 
@return 
@note 需要对osd buffer填入透明色
*******************************************************************************/
void GUIREF_ClearTvOsdOutputData(
                                 GUI_COLOR_T   color_key
                                 );


/***************************************************************************//*!
@brief when tv out, set block buffer by block id before scaling down
@author Jassmine
@param lcd_id   [in] 
@param block_id [in] 
@return 
@note 
*******************************************************************************/
BLOCKCFG_T* GUIREF_GetBlockCfgBeforeScale(
                                          GUI_LCD_ID_E      lcd_id,  //in
                                          GUIBLOCK_ID_E     block_id //in
                                          );


/***************************************************************************//*!
@brief when tv out, set block config by block id
@author Jassmine
@param lcd_id        [in] 
@param block_id      [in] 
@param block_cfg_ptr [in] 
@return 
@note 
*******************************************************************************/
void GUIREF_SetBlockCfgBeforeScale(
                                   GUI_LCD_ID_E      lcd_id,
                                   GUIBLOCK_ID_E     block_id,
                                   BLOCKCFG_T        *block_cfg_ptr
                                   );


/***************************************************************************//*!
@brief  when tv out, handle scaling down function
@author Jassmine
@param update_block_set [in] 
@param update_rect_ptr  [in] 
@return 
@note 
*******************************************************************************/
void GUIREF_TvScalingDown(
                          uint32        update_block_set,
                          GUI_RECT_T    *update_rect_ptr    //PNULL,update all lcd
                          );


/***************************************************************************//*!
@brief get block set with block id
@author Jassmine
@param block_id [in] 
@return 
@note 
*******************************************************************************/
uint32 GUIREF_GetUpdateBlockSet(
                                GUIBLOCK_ID_E   block_id
                                );


/***************************************************************************//*!
@brief get if is scaling down
@author Jassmine
*******************************************************************************/
BOOLEAN GUIREF_GetIsScaleDown(void);


/***************************************************************************//*!
@brief Set if is scaling down
@author Jassmine
@param is_used [in] 
*******************************************************************************/
void GUIREF_SetIsScaleDown(
                           BOOLEAN  is_used
                           );


/***************************************************************************//*!
@brief get tv out buffer for dc
@author ryan.xu
*******************************************************************************/
PUBLIC uint32* GUIREF_GetTvOutBuffer(void);


/***************************************************************************//*!
@brief 注册TVOUT功能
@author bin.ji
@param row_pixels        [in] 
@param col_pixels        [in] 
@param tvout_rect_memory [in] 
@param tvout_memory1     [in] 
@param tvout_memory2     [in] 
*******************************************************************************/
PUBLIC void GUIREF_RegisterTvOutForApp(
                                       uint32 row_pixels,
                                       uint32 col_pixels,                                       
                                       GUI_COLOR_T *tvout_rect_memory,
                                       GUI_COLOR_T **tvout_memory1,
                                       GUI_COLOR_T **tvout_memory2
                                       );


/***************************************************************************//*!
@brief 关闭TVOUT功能
@author bin.ji
*******************************************************************************/
PUBLIC void GUIREF_UnregisterTvOutForApp(void);


/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
