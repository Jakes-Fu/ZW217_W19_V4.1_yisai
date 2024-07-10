/*! @file guilcd.h
@brief ���ļ���װ��LCD��صķ���
@author Jassmine
@version 1.0
@date 06/11/2007    
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Lcd Lcd

(����)

*******************************************************************************/

/*! @addtogroup lcdGrp Lcd
@brief lcdģ��
@details
��װLCD��صķ���
- ��ȡLCD�������Ϣ������Ļ��ߵ�
- ˢ����Ļ
- ����Ļ��ת��ص���Ϣ\n
������������������Ļ������Ƕ�Ϊ��׼�ģ������ĽǶ�Ϊ0�Ⱥ�180�ȣ�Ĭ�������ĸ߶ȴ�
�ڿ�ȣ�ͬ���أ�����Ƕ�Ϊ90�Ȼ�270��ʱ������Ϊ�Ǻ����������Ŀ�ȴ��ڸ߶ȡ����磬
�����Ļ�ĳ�ʼ�Ƕ�Ϊ0��180��ƽ̨����Ϊ��Ϊ������
@{
*******************************************************************************/


#ifndef _GUILCD_H_
#define _GUILCD_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "dal_img.h"
#include "mmk_type.h"
#include "guigraph.h"
#include "dal_display.h"

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
/*! @def GUI_LCD_ID_E
@brief LCD ID
*/
#define GUI_LCD_ID_E        LCD_ID_E

/*! @def GUI_LCD_0
@brief 0��LCD
*/
#define GUI_LCD_0           LCD_ID_0
/*! @def GUI_MAIN_LCD_ID
@brief ������������0������
*/
#define GUI_MAIN_LCD_ID     LCD_ID_0
/*! @def GUI_LCD_1
@brief 1����
*/
#define GUI_LCD_1           LCD_ID_1
/*! @def GUI_SUB_LCD_ID
@brief ������������1������
*/
#define GUI_SUB_LCD_ID      LCD_ID_1
/*! @def GUI_LCD_2
@brief 2����
*/
#define GUI_LCD_2           LCD_ID_2
/*! @def GUI_LCD_3
@brief 3����
*/
#define GUI_LCD_3           LCD_ID_3
/*! @def GUI_LCD_4
@brief 4����
*/
#define GUI_LCD_4           LCD_ID_4
/*! @def GUI_LCD_5
@brief 5����
*/
#define GUI_LCD_5           LCD_ID_5
/*! @def GUI_LCD_6
@brief 6����
*/
#define GUI_LCD_6           LCD_ID_6
/*! @def GUI_LCD_7
@brief 7����
*/
#define GUI_LCD_7           LCD_ID_7
/*! @def GUI_TV_ID
@brief TV�����
*/
#define GUI_TV_ID           TV_ID
/*! @def GUI_LCD_MAX
@brief LCD����
*/
#define GUI_LCD_MAX         MAX_LCD_ID
        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @struct GUILCD_COORDINATE_PARAM_T
@brief ���������Ϣ
*/
typedef struct
{
    GUI_RECT_T  rect;           /*!< �������� */
    LCD_ANGLE_E angle;          /*!< �Ƕ� */
    uint16      lcd_width;      /*!< ���� *///logical or physical screen width 
    uint16      lcd_height;     /*!< ���� *///logical or physical screen height 
}GUILCD_COORDINATE_PARAM_T;

//ˢ��ģʽ
typedef enum
{
    GUILCD_INVALIDATE_MODE_NORMAL,  //����ˢ��
    GUILCD_INVALIDATE_MODE_OSD,     //OSDˢ������ģʽ�£�����ֱ�Ӵ����ײ��ͼ���ϳɣ����Լ���һ��buffer

    GUILCD_INVALIDATE_MODE_MAX
}GUILCD_INVALIDATE_MODE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief LCD��ʼ��
@author Jassmine
@param lcd_id [in] LCD ID
@return �Ƿ���óɹ�
@note 
����ֻ�ܳ�ʼ����������������\link #GUI_MAIN_LCD_ID \endlink��\link #GUI_SUB_LCD_ID \endlink
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_Init(
                           LCD_ID_E     lcd_id
                           );

/***************************************************************************//*!
@brief ��ȡLCD��Ϣ
@author Jassmine
@param lcd_id [in] LCD ID
@param lcd_info_ptr [in/out] ָ��LCD��Ϣ��ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_GetInfo(
                              LCD_ID_E      lcd_id,
                              LCD_INFO_T    *lcd_info_ptr
                              );

/***************************************************************************//*!
@brief ��ȡ������buffer��ַ
@author Jassmine
@return ������buffer��ַ
@note 
�÷�����ȡ����LCD buffer
*******************************************************************************/
PUBLIC GUI_COLOR_T* GUILCD_GetMainLcdBufPtr(void);

/*****************************************************************************/
// Description : set brush mode
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
//PUBLIC BOOLEAN GUILCD_SetBrushMode(
//                                   LCD_ID_E             lcd_id,
//                                   LCD_BRUSH_MODE_E     mode
 //                                  );

/***************************************************************************//*!
@brief ��ȡָ��LCD�ĳ�ʼ�Ƕ�
@author Jassmine
@param lcd_id [in] LCD ID
@return LCD�ĳ�ʼ�Ƕ�
@note 
*******************************************************************************/
PUBLIC LCD_ANGLE_E GUILCD_GetInitAngle(
                                       LCD_ID_E     lcd_id
                                       );


/***************************************************************************//*!
@brief ��ȡָ��LCD�ĵ�ǰ�Ƕ�
@author Jassmine
@param lcd_id [in] LCD ID
@return LCD�ĵ�ǰ�Ƕ�
@note 
*******************************************************************************/
PUBLIC LCD_ANGLE_E  GUILCD_GetLogicAngle(
                                         LCD_ID_E   lcd_id
                                         );

/***************************************************************************//*!
@brief ����ָ��LCD�ĵ�ǰ�Ƕ�
@author Jassmine
@param lcd_id [in] LCD ID
@param logic_angle [in] LCD�ĵ�ǰ�Ƕ�
@return 
@note 
*******************************************************************************/
PUBLIC void GUILCD_SetLogicAngle(
                                 LCD_ID_E       lcd_id,
                                 LCD_ANGLE_E    logic_angle
                                 );

/***************************************************************************//*!
@brief ˢ��ָ��������Ļ��
@author Jassmine
@param lcd_id [in] LCD ID
@param rect [in] ��Ҫˢ�µ�����
@param update_block_set [in] ��Ҫ���µ�BLOCK��������TVOUT
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_InvalidateRect(
                                     LCD_ID_E       lcd_id,             //in:
                                     GUI_RECT_T     rect,               //in:
                                     uint32         update_block_set    //in:for tv out
                                     );


/***************************************************************************//*!
@brief ˢ��������Ļ
@author Jassmine
@param lcd_id [in] LCD ID
@param update_block_set [in] ��Ҫ���µ�BLOCK��������TVOUT
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_Invalidate(
                                 LCD_ID_E   lcd_id,             //in:
                                 uint32     update_block_set    //in:for tv out
                                 );

/***************************************************************************//*!
@brief ˢ��ָ��������Ļ�ϣ���������
@author Jassmine
@param lcd_id [in] LCD ID
@param rect [in] ��Ҫˢ�µ�����
@param update_block_set [in] ��Ҫ���µ�BLOCK��������TVOUT
@return �Ƿ���óɹ�
@note 
�÷�����\link GUILCD_InvalidateRect() \endlink���������ڣ��÷�����������
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_InvalidateRectOnly(
                                         LCD_ID_E       lcd_id,             //in:
                                         GUI_RECT_T     rect,               //in:
                                         uint32         update_block_set    //in:for tv out
                                         );

/***************************************************************************//*!
@brief ��ȡ��ǰ�߼����Ŀ��
@author Jassmine
@param lcd_id [in] LCD ID
@param logic_width_ptr [in/out] ָ���߼�����ȵ�ָ��
@param logic_height_ptr [in/out] ָ���߼����߶ȵ�ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_GetLogicWidthHeight(
                                          LCD_ID_E      lcd_id,             //in:
                                          uint16        *logic_width_ptr,   //in/out:may PNULL
                                          uint16        *logic_height_ptr   //in/out:may PNULL
                                          );


/***************************************************************************//*!
@brief ������ת�ǶȻ�ȡ��Ļ���
@author Jassmine
@param lcd_id [in] LCD ID
@param lcd_angle [in] ��ת�Ƕ�
@param logic_width_ptr [in/out] ָ���ȵ�ָ��
@param logic_height_ptr [in/out] ָ��߶ȵ�ָ��
@return 
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_GetWidthHeightByAngle(
                                            LCD_ID_E    lcd_id,             //in:
                                            LCD_ANGLE_E lcd_angle,          //in:
                                            uint16      *logic_width_ptr,   //in/out:may PNULL
                                            uint16      *logic_height_ptr   //in/out:may PNULL
                                            );

/***************************************************************************//*!
@brief ��Ļ�Ƿ��ں���״̬
@author Jassmine
@param lcd_id [in] LCD ID
@return ��Ļ�Ƿ��ں���״̬
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_IsLandscape(
                                  LCD_ID_E  lcd_id
                                  );

/***************************************************************************//*!
@brief ���߼��ǶȺ�����ת��Ϊ����ǶȺ�����
@author Jassmine
@param lcd_id [in] LCD ID
@param logic_rect_ptr [in] �߼�����
@param logic_angle_ptr [in] �߼��Ƕ�
@param physical_rect_ptr [in/out] ��������
@param physical_angle_ptr [in/out] ����Ƕ�
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_ConvertRectAndAngle(
                                          LCD_ID_E      lcd_id,             //in:
                                          GUI_RECT_T    *logic_rect_ptr,    //in:
                                          LCD_ANGLE_E   *logic_angle_ptr,   //in:
                                          GUI_RECT_T    *physical_rect_ptr, //in/out:may PNULL
                                          LCD_ANGLE_E   *physical_angle_ptr //in/out:may PNULL
                                          );

/***************************************************************************//*!
@brief ���ݽǶ�ת������
@author Jassmine
@param key_code_ptr [in/out] ����ԭ����ֵ�������ת����İ���ֵ
@param lcd_id [in] LCD ID
@param logic_angle [in] �Ƕ�
@return 
@note 
�÷�������ֻ����ת�����������������������Ҫת��
*******************************************************************************/
PUBLIC void GUILCD_ConvertKeyCode(
                                  uint32        *key_code_ptr,  //in/out:
                                  LCD_ID_E      lcd_id,         //in:
                                  LCD_ANGLE_E   logic_angle     //in:current angle
                                  );

/***************************************************************************//*!
@brief ��ȡ��Ļ��ת�ĽǶ�
@author Jassmine
@param lcd_id [in] LCD ID
@return ��Ļ����ڳ�ʼʱ����ת�Ƕ�
@note 
*******************************************************************************/
PUBLIC LCD_ANGLE_E GUILCD_GetRotateAngle(
                                         LCD_ID_E   lcd_id  //in:
                                         );

/***************************************************************************//*!
@brief ���ݵ�ǰ��ת�Ƕȣ�ת����Ļ�����
@author Jassmine
@param x_ptr [in/out] ����ԭ��Ļx����㣬���ת�����x�����
@param y_ptr [in/out] ����ԭ��Ļy����㣬���ת�����y�����
@param lcd_id [in] LCD ID
@return 
@note 
*******************************************************************************/
PUBLIC void GUILCD_ConvertTpPoint(
                                  uint32    *x_ptr, //in/out:
                                  uint32    *y_ptr, //in/out:
                                  LCD_ID_E  lcd_id  //in:
                                  );
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
/***************************************************************************//*!
@brief ���ݵ�ǰ��ת�Ƕȣ�ת����Ļ�����
@author zhenpeng.yao
@param x_ptr [in/out] ����ԭ��Ļx����㣬���ת�����x�����
@param y_ptr [in/out] ����ԭ��Ļy����㣬���ת�����y�����
@param lcd_id [in] LCD ID
@return 
@note support left and right wear,we can get the display angle from LCD_GetWearType
*******************************************************************************/
PUBLIC void GUILCD_ConvertTpPointEx(
                                  uint32    *x_ptr, //in/out:
                                  uint32    *y_ptr, //in/out:
                                  LCD_ID_E  lcd_id  //in:
                                  );
#endif
                                  

/***************************************************************************//*!
@brief ���ݵ�ǰ��ת�Ƕȣ�ת��������Ϣ
@author robert.wang
@param lcd_id [in] LCD ID
@param logic_coordinate_ptr [in] ָ��ԭ������Ϣ��ָ�룬����Ϊ��
@param physical_coordinate_ptr [in/out] ָ��ת�����������Ϣ��ָ�룬����Ϊ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_ConvertLogicToPhysicalCoordinate(
                                          LCD_ID_E      lcd_id,             //in:
                                          GUILCD_COORDINATE_PARAM_T  *logic_coordinate_ptr, //in:
                                          GUILCD_COORDINATE_PARAM_T  *physical_coordinate_ptr //out:
                                          );

/*****************************************************************************/
// Description : set invalidate mode
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_SetInvalidateMode(
                                        GUILCD_INVALIDATE_MODE_E mode
                                        );

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
