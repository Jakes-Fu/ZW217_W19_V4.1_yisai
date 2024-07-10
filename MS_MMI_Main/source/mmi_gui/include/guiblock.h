/*! @file guiblock.h
@brief 此文件介绍了block控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date 06/11/2007       
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Block Block

(暂无)

*******************************************************************************/

/*! @addtogroup blockGrp Block
@brief block控件
@details
BLOCK的概念，是对LCD屏幕的一层抽象，它将屏幕分层，按照不同的内容，或者不同的应用需求，
放置在不同的BLOCK层上，然后由硬件将它们的内容进行合并，刷新到屏幕上。如DC应用，
拍摄的内容显示在主层上，选项菜单则在OSD层上，这两层的内容互不影响，但硬件在最终
输出时，会将主层和OSD层的内容进行合并。\n
由于每个BLOCK都有它自己的buffer，所以用户在绘制时，不会影响到其他BLOCK的内容，
可以减少刷新动作，提高效率。\n
除BLOCK外，还有一个与之相似的概念：LAYER。它是对主LCD的主BLOCK的一个抽象，其功能
与BLOCK类似，只是BLOCK最终结果输出到屏幕上，而LAYER的输出结果是在主LCD的主BLOCK上。
@{
*******************************************************************************/


#ifndef _GUIBLOCK_H_
#define _GUIBLOCK_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guilcd.h"

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
/*! @def GUI_UILAYER_TOTAL_LAYER_COUNT
@brief  逻辑层块的个数
*/
#define GUI_UILAYER_TOTAL_LAYER_COUNT       100//200 菜单使用了缓冲区机制，不需要很多图层了，裁剪一半 
//#ifdef UI_LAYER_SUPPORT_LOW_MEMORY
//#define GUI_UILAYER_TOTAL_LAYER_COUNT       20     
//#else
//#define GUI_UILAYER_TOTAL_LAYER_COUNT       200     
//#endif

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
        
/*! @enum GUIBLOCK_ID_E
@brief BLOCK ID
*/
typedef enum
{
	GUI_BLOCK_0    = 0,             /*!< 0号BLOCK */
	GUI_BLOCK_MAIN = GUI_BLOCK_0,   /*!< 主BLOCK *///main block
	GUI_BLOCK_1,                    /*!< 1号BLOCK */
	GUI_BLOCK_2,                    /*!< 2号BLOCK */
	GUI_BLOCK_OSD  = GUI_BLOCK_2,   /*!< OSD BLOCK *///osd block
    GUI_BLOCK_RESERVED,             /*!< 保留 *///reserved
    GUI_BLOCK_RESERVED_2,           /*!< 保留 */
	GUI_BLOCK_MAX,
    //后面是多图层的block_id
    GUI_UILAYER_MAX = (GUI_BLOCK_MAX+GUI_UILAYER_TOTAL_LAYER_COUNT)
} GUIBLOCK_ID_E;


/*! @enum GUIBLOCK_TYPE_E
@brief BLOCK类型
*/
typedef enum
{
    GUIBLOCK_TYPE_OSD = 0,          /*!< OSD模式 @note 用该类型可以通过关键色来达到透明的效果 */
    GUIBLOCK_TYPE_COMMON,           /*!< 普通模式 */
	GUIBLOCK_TYPE_MAX
} GUIBLOCK_TYPE_E;


/*! @enum GUIBLOCK_PRIORITY_E
@brief BLOCK优先级
@details
当一个LCD有多个BLOCK时，优先级用于指定这些BLOCK的合并的顺序
*/
typedef enum
{
	GUIBLOCK_PRIORITY_LOWEST = 0,   /*!< 最低级 */
	GUIBLOCK_PRIORITY_0 = GUIBLOCK_PRIORITY_LOWEST, /*!< 0级 */
	GUIBLOCK_PRIORITY_1,            /*!< 1级 */
	GUIBLOCK_PRIORITY_2,            /*!< 2级 */
	GUIBLOCK_PRIORITY_3,            /*!< 3级 */
	GUIBLOCK_PRIORITY_4,            /*!< 4级 */
	GUIBLOCK_PRIORITY_5,            /*!< 5级 */
	GUIBLOCK_PRIORITY_6,            /*!< 6级 */
	GUIBLOCK_PRIORITY_7,            /*!< 7级 */
	GUIBLOCK_PRIORITY_HIGHEST = GUIBLOCK_PRIORITY_7 /*!< 最高级 */
} GUIBLOCK_PRIORITY_E;


/*! @enum GUIBLOCK_RESOLUTION_E
@brief BLOCK buffer格式
*/
typedef enum
{
	GUIBLOCK_RESOLUTION_888 = 0,    /*!< ARGB888格式 */
	GUIBLOCK_RESOLUTION_666 = 1,    /*!< RGB666格式 */
	GUIBLOCK_RESOLUTION_565 = 2,    /*!< RGB565格式 */
	GUIBLOCK_RESOLUTION_MAX
} GUIBLOCK_RESOLUTION_E;


/*! @struct GUIBLOCK_INFO_T
@brief BLOCK信息
*/
typedef struct 
{	
    BOOLEAN         is_enable;      /*!< BLOCK是否开启 */
    GUI_COLOR_T     *init_buf_ptr;  /*!< BLOCK初始buffer地址 */
    uint32          offset;         /*!< BLOCK当前偏移量 */
    GUI_RECT_T      rect;           /*!< 区域值 */
    BLOCKCFG_T      config;         /*!< 配置信息 */
} GUIBLOCK_INFO_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief BLOCK初始化
@author Jassmine
@param lcd_id [in] LCD ID
@return 是否调用成功
@note
该方法只初始化GUI_BLOCK_MAIN
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Init(
                             LCD_ID_E  lcd_id
                             );

/***************************************************************************//*!
@brief  注册BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note
该方法初始化除GUI_BLOCK_MAIN外的BLOCK
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Register(
                                 GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                 );



/***************************************************************************//*!
@brief 销毁BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note 
只能销毁除GUI_BLOCK_MAIN外的BLOCK
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Destroy(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                );



/***************************************************************************//*!
@brief 重置BLOCK的buffer
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@param buffer_ptr  [in] 新buffer地址
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetBuffer(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr,   //in:
                                  GUI_COLOR_T       *buffer_ptr     //in:
                                  );


/***************************************************************************//*!
@brief 查询指定的BLOCK是否开启
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return BLOCK是否开启
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_IsEnable(
                                 GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                 );



/***************************************************************************//*!
@brief 开启指定BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Enable(
                               GUI_LCD_DEV_INFO     *lcd_dev_ptr
                               );



/***************************************************************************//*!
@brief 禁止指定BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Disable(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                );



/***************************************************************************//*!
@brief 转换BLOCK ID
@author Jassmine
@param lcd_id   [in] LCD ID
@param block_id [in] 原BLOCK ID
@return 转换后的BLOCK ID
@note 
*******************************************************************************/
PUBLIC uint32 GUIBLOCK_ConvertBlockId(
                                      LCD_ID_E          lcd_id, //in
                                      GUIBLOCK_ID_E     block_id//in
                                      );



/***************************************************************************//*!
@brief 设置BLOCK的区域
@author Jassmine
@param block_rect  [in] 区域值
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note 
现在只能设置GUI_MAIN_LCD_ID和GUI_TV_ID的优先级
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetRect(
                                GUI_RECT_T          block_rect, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                );



/***************************************************************************//*!
@brief 清除BLOCK的区域
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note 
现在只能设置GUI_MAIN_LCD_ID和GUI_TV_ID的优先级
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_ClearRect(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                                  );


/***************************************************************************//*!
@brief 设置BLOCK的优先级
@author Jassmine
@param lcd_dev_ptr    [in] LCD信息
@param block_priority [in] 优先级
@return 是否调用成功
@note 
现在只能设置GUI_MAIN_LCD_ID和GUI_TV_ID的优先级
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetPriority(
                                    GUI_LCD_DEV_INFO        *lcd_dev_ptr,   //in:
                                    GUIBLOCK_PRIORITY_E     block_priority  //in:
                                    );



/***************************************************************************//*!
@brief 设置指定BLOCK的类型
@author Jassmine
@param alpha       [in] 透明度
@param color_key   [in] 关键色
@param block_type  [in] BLOCK类型
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note
关键色color_key只有在OSD模式下才有效，可以通过关键色来透过某些区域，而alpha是整体透明
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetType(
                                uint32              alpha,      //in:
                                GUI_COLOR_T         color_key,  //in:
                                GUIBLOCK_TYPE_E     block_type, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                );

PUBLIC BOOLEAN GUIBLOCK_ResetType(
                                GUIBLOCK_TYPE_E     block_type, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                );

/***************************************************************************//*!
@brief 重置主BLOCK信息
@author Jassmine
@return 
@note 
*******************************************************************************/
PUBLIC void GUIBLOCK_ResetMainType(void);


/***************************************************************************//*!
@brief 用指定颜色清除BLOCK buffer的内容
@author Jassmine
@param color       [in] 颜色值
@param lcd_dev_ptr [in] LCD信息
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_ClearByColor(
                                     GUI_COLOR_T        color,
                                     GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                     );



/***************************************************************************//*!
@brief 注册block并设置相关信息
@author Jassmine
@param alpha            [in] 透明度
@param display_rect     [in] 显示区域
@param color_key        [in] 关键色
@param lcd_dev_ptr      [in] LCD信息
@param block_type       [in] BLOCK类型
@param display_priority [in] 优先级
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetHalfTransparent(
                                           uint32               alpha,
                                           GUI_RECT_T           display_rect,
                                           GUI_COLOR_T          color_key,
                                           GUI_LCD_DEV_INFO     *lcd_dev_ptr,
                                           GUIBLOCK_TYPE_E      block_type,
                                           GUIBLOCK_PRIORITY_E  display_priority
                                           );


/***************************************************************************//*!
@brief 获取指定LCD的buffer
@author Jassmine
@param lcd_dev_ptr [in] LCD信息
@return buffer地址
@note
如果lcd_dev_ptr的block_id为GUI_BLOCK_MAIN，那么该方法将获取layer系统中主层的buffer，
而非LCD的buffer。\n
如果想要获取LCD的buffer，可以通过方法\link GUILCD_GetInfo() \endlink
*******************************************************************************/
PUBLIC GUI_COLOR_T* GUIBLOCK_GetBuf(
                                    GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                    );

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
