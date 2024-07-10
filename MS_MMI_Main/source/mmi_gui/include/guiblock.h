/*! @file guiblock.h
@brief ���ļ�������block�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date 06/11/2007       
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Block Block

(����)

*******************************************************************************/

/*! @addtogroup blockGrp Block
@brief block�ؼ�
@details
BLOCK�ĸ���Ƕ�LCD��Ļ��һ�����������Ļ�ֲ㣬���ղ�ͬ�����ݣ����߲�ͬ��Ӧ������
�����ڲ�ͬ��BLOCK���ϣ�Ȼ����Ӳ�������ǵ����ݽ��кϲ���ˢ�µ���Ļ�ϡ���DCӦ�ã�
�����������ʾ�������ϣ�ѡ��˵�����OSD���ϣ�����������ݻ���Ӱ�죬��Ӳ��������
���ʱ���Ὣ�����OSD������ݽ��кϲ���\n
����ÿ��BLOCK�������Լ���buffer�������û��ڻ���ʱ������Ӱ�쵽����BLOCK�����ݣ�
���Լ���ˢ�¶��������Ч�ʡ�\n
��BLOCK�⣬����һ����֮���Ƶĸ��LAYER�����Ƕ���LCD����BLOCK��һ�������书��
��BLOCK���ƣ�ֻ��BLOCK���ս���������Ļ�ϣ���LAYER��������������LCD����BLOCK�ϡ�
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
@brief  �߼����ĸ���
*/
#define GUI_UILAYER_TOTAL_LAYER_COUNT       100//200 �˵�ʹ���˻��������ƣ�����Ҫ�ܶ�ͼ���ˣ��ü�һ�� 
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
	GUI_BLOCK_0    = 0,             /*!< 0��BLOCK */
	GUI_BLOCK_MAIN = GUI_BLOCK_0,   /*!< ��BLOCK *///main block
	GUI_BLOCK_1,                    /*!< 1��BLOCK */
	GUI_BLOCK_2,                    /*!< 2��BLOCK */
	GUI_BLOCK_OSD  = GUI_BLOCK_2,   /*!< OSD BLOCK *///osd block
    GUI_BLOCK_RESERVED,             /*!< ���� *///reserved
    GUI_BLOCK_RESERVED_2,           /*!< ���� */
	GUI_BLOCK_MAX,
    //�����Ƕ�ͼ���block_id
    GUI_UILAYER_MAX = (GUI_BLOCK_MAX+GUI_UILAYER_TOTAL_LAYER_COUNT)
} GUIBLOCK_ID_E;


/*! @enum GUIBLOCK_TYPE_E
@brief BLOCK����
*/
typedef enum
{
    GUIBLOCK_TYPE_OSD = 0,          /*!< OSDģʽ @note �ø����Ϳ���ͨ���ؼ�ɫ���ﵽ͸����Ч�� */
    GUIBLOCK_TYPE_COMMON,           /*!< ��ͨģʽ */
	GUIBLOCK_TYPE_MAX
} GUIBLOCK_TYPE_E;


/*! @enum GUIBLOCK_PRIORITY_E
@brief BLOCK���ȼ�
@details
��һ��LCD�ж��BLOCKʱ�����ȼ�����ָ����ЩBLOCK�ĺϲ���˳��
*/
typedef enum
{
	GUIBLOCK_PRIORITY_LOWEST = 0,   /*!< ��ͼ� */
	GUIBLOCK_PRIORITY_0 = GUIBLOCK_PRIORITY_LOWEST, /*!< 0�� */
	GUIBLOCK_PRIORITY_1,            /*!< 1�� */
	GUIBLOCK_PRIORITY_2,            /*!< 2�� */
	GUIBLOCK_PRIORITY_3,            /*!< 3�� */
	GUIBLOCK_PRIORITY_4,            /*!< 4�� */
	GUIBLOCK_PRIORITY_5,            /*!< 5�� */
	GUIBLOCK_PRIORITY_6,            /*!< 6�� */
	GUIBLOCK_PRIORITY_7,            /*!< 7�� */
	GUIBLOCK_PRIORITY_HIGHEST = GUIBLOCK_PRIORITY_7 /*!< ��߼� */
} GUIBLOCK_PRIORITY_E;


/*! @enum GUIBLOCK_RESOLUTION_E
@brief BLOCK buffer��ʽ
*/
typedef enum
{
	GUIBLOCK_RESOLUTION_888 = 0,    /*!< ARGB888��ʽ */
	GUIBLOCK_RESOLUTION_666 = 1,    /*!< RGB666��ʽ */
	GUIBLOCK_RESOLUTION_565 = 2,    /*!< RGB565��ʽ */
	GUIBLOCK_RESOLUTION_MAX
} GUIBLOCK_RESOLUTION_E;


/*! @struct GUIBLOCK_INFO_T
@brief BLOCK��Ϣ
*/
typedef struct 
{	
    BOOLEAN         is_enable;      /*!< BLOCK�Ƿ��� */
    GUI_COLOR_T     *init_buf_ptr;  /*!< BLOCK��ʼbuffer��ַ */
    uint32          offset;         /*!< BLOCK��ǰƫ���� */
    GUI_RECT_T      rect;           /*!< ����ֵ */
    BLOCKCFG_T      config;         /*!< ������Ϣ */
} GUIBLOCK_INFO_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief BLOCK��ʼ��
@author Jassmine
@param lcd_id [in] LCD ID
@return �Ƿ���óɹ�
@note
�÷���ֻ��ʼ��GUI_BLOCK_MAIN
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Init(
                             LCD_ID_E  lcd_id
                             );

/***************************************************************************//*!
@brief  ע��BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note
�÷�����ʼ����GUI_BLOCK_MAIN���BLOCK
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Register(
                                 GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                 );



/***************************************************************************//*!
@brief ����BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note 
ֻ�����ٳ�GUI_BLOCK_MAIN���BLOCK
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Destroy(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                );



/***************************************************************************//*!
@brief ����BLOCK��buffer
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@param buffer_ptr  [in] ��buffer��ַ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetBuffer(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr,   //in:
                                  GUI_COLOR_T       *buffer_ptr     //in:
                                  );


/***************************************************************************//*!
@brief ��ѯָ����BLOCK�Ƿ���
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return BLOCK�Ƿ���
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_IsEnable(
                                 GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                 );



/***************************************************************************//*!
@brief ����ָ��BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Enable(
                               GUI_LCD_DEV_INFO     *lcd_dev_ptr
                               );



/***************************************************************************//*!
@brief ��ָֹ��BLOCK
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Disable(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                );



/***************************************************************************//*!
@brief ת��BLOCK ID
@author Jassmine
@param lcd_id   [in] LCD ID
@param block_id [in] ԭBLOCK ID
@return ת�����BLOCK ID
@note 
*******************************************************************************/
PUBLIC uint32 GUIBLOCK_ConvertBlockId(
                                      LCD_ID_E          lcd_id, //in
                                      GUIBLOCK_ID_E     block_id//in
                                      );



/***************************************************************************//*!
@brief ����BLOCK������
@author Jassmine
@param block_rect  [in] ����ֵ
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note 
����ֻ������GUI_MAIN_LCD_ID��GUI_TV_ID�����ȼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetRect(
                                GUI_RECT_T          block_rect, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                );



/***************************************************************************//*!
@brief ���BLOCK������
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note 
����ֻ������GUI_MAIN_LCD_ID��GUI_TV_ID�����ȼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_ClearRect(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                                  );


/***************************************************************************//*!
@brief ����BLOCK�����ȼ�
@author Jassmine
@param lcd_dev_ptr    [in] LCD��Ϣ
@param block_priority [in] ���ȼ�
@return �Ƿ���óɹ�
@note 
����ֻ������GUI_MAIN_LCD_ID��GUI_TV_ID�����ȼ�
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetPriority(
                                    GUI_LCD_DEV_INFO        *lcd_dev_ptr,   //in:
                                    GUIBLOCK_PRIORITY_E     block_priority  //in:
                                    );



/***************************************************************************//*!
@brief ����ָ��BLOCK������
@author Jassmine
@param alpha       [in] ͸����
@param color_key   [in] �ؼ�ɫ
@param block_type  [in] BLOCK����
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note
�ؼ�ɫcolor_keyֻ����OSDģʽ�²���Ч������ͨ���ؼ�ɫ��͸��ĳЩ���򣬶�alpha������͸��
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
@brief ������BLOCK��Ϣ
@author Jassmine
@return 
@note 
*******************************************************************************/
PUBLIC void GUIBLOCK_ResetMainType(void);


/***************************************************************************//*!
@brief ��ָ����ɫ���BLOCK buffer������
@author Jassmine
@param color       [in] ��ɫֵ
@param lcd_dev_ptr [in] LCD��Ϣ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_ClearByColor(
                                     GUI_COLOR_T        color,
                                     GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                     );



/***************************************************************************//*!
@brief ע��block�����������Ϣ
@author Jassmine
@param alpha            [in] ͸����
@param display_rect     [in] ��ʾ����
@param color_key        [in] �ؼ�ɫ
@param lcd_dev_ptr      [in] LCD��Ϣ
@param block_type       [in] BLOCK����
@param display_priority [in] ���ȼ�
@return �Ƿ���óɹ�
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
@brief ��ȡָ��LCD��buffer
@author Jassmine
@param lcd_dev_ptr [in] LCD��Ϣ
@return buffer��ַ
@note
���lcd_dev_ptr��block_idΪGUI_BLOCK_MAIN����ô�÷�������ȡlayerϵͳ�������buffer��
����LCD��buffer��\n
�����Ҫ��ȡLCD��buffer������ͨ������\link GUILCD_GetInfo() \endlink
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
