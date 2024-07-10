/*****************************************************************************
** File Name:      ctrlsoftkey_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLSOFTKEY_EXPORT_H_
#define _CTRLSOFTKEY_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"

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
/*! @def BORDER_NULL
@brief û�б߿�
*/
#define BORDER_NULL     		0

/*! @def GUISOFTKEY_BUTTON_NUM
@brief softkeyһ������button��������
*/
#define GUISOFTKEY_BUTTON_NUM  	3 // softkeyһ������button��������

/*! @def LEFT_BUTTON
@brief ��button������
*/
#define LEFT_BUTTON     		0 // ��button������

/*! @def MIDDLE_BUTTON
@brief �м�button������
*/
#define MIDDLE_BUTTON   		1 // ��button������

/*! @def RIGHT_BUTTON
@brief ��button������
*/
#define RIGHT_BUTTON    		2 // ��button������

/*! @def INVALID_BUTTON
@brief ��Чbutton������
*/
#define INVALID_BUTTON    		4 // ��Чbutton������

/*! @typedef GUI_SOFTKEY_BG_TYPE_T
@brief softkey����ģʽ��Ϣ
*/
typedef uint32  GUI_SOFTKEY_BG_TYPE_T;

/*! @def GUI_SOFTKEY_BG_NULL
@brief û�б���
*/
#define GUI_SOFTKEY_BG_NULL         0

/*! @def GUI_SOFTKEY_BG_IMAGE_ONLY
@brief ����ֻ��ͼƬ
*/
#define GUI_SOFTKEY_BG_IMAGE_ONLY   1

/*! @def GUI_SOFTKEY_BG_COLOR_ONLY
@brief ����ֻ����ɫ
*/
#define GUI_SOFTKEY_BG_COLOR_ONLY   2

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @def GUISOFTKEY_STYLE_NULL
@brief softkey�޷��
*/
#define GUISOFTKEY_STYLE_NULL		0

/*! @typedef GUISOFTKEY_INDEX
@brief softkey���
*/
typedef uint16  GUISOFTKEY_INDEX;


/*! @typedef GUI_BUTTON_FRAME_TYPE_T
@brief softkey�ܹ�ģʽ��Ϣ
@note ����ṹ������Ŀǰ��֧�������
*/
typedef enum
{
    BUTTON_FRAME_RECT,  
    BUTTON_FRAME_3D,   
    BUTTON_FRAME_IMAGE, 
    BUTTON_FRAME_NULL   
}GUI_BUTTON_FRAME_TYPE_T; 

/*!  @typedef GUISK_DATA_TYPE_E
@brief softkey��ʾģʽ
@note Ŀǰֻ��ʾ�����е�һ�֣���֧������ͬʱ��ʾ
*/
typedef enum
{
    GUISK_DATA_TEXT_ID,     /*!< ��ʾ�ı� */ 
	GUISK_DATA_TEXT_BUFFER, /*!< ��ʾ�ı� */  
    GUISK_DATA_IMAGE_ID,    /*!< ��ʾicon */  

    GUISK_DATA_MAX,
}GUISK_DATA_TYPE_E;

/*! @union GUISK_CONTENT_DATA_U
@brief softkey��ʾ����Ϣ
@note ֻ��ʾ����һ������
*/
typedef union
{
    MMI_TEXT_ID_T               text_id;    /*!< text_id */
    MMI_STRING_T                text_buffer;/*!< text_buffer */
    MMI_IMAGE_ID_T              image_id;   /*!< image_id */
}GUISK_CONTENT_DATA_U;

/*! @struct GUISK_CONTENT_T
@brief  �ؼ�������������Ϣ
*/
typedef struct
{
    GUISK_DATA_TYPE_E data_type;    /*!< �ı�id����buffer */ 
    GUISK_CONTENT_DATA_U   data_u;  /*!< �ı���Ϣ */ 
} GUISK_CONTENT_T;

/*! @struct GUI_BUTTON_T
@brief  softkey �� button����
*/
typedef struct
{
    GUISK_CONTENT_T             content;        /*!< �ؼ��ı���Ϣ */ 
    MMI_IMAGE_ID_T              icon_down_id;   /*!< ����ͼƬid */ 
    MMI_IMAGE_ID_T              icon_up_id;     /*!< �ָ�ͼƬid */ 
	MMI_IMAGE_ID_T              gray_icon_id;   /*!< �һ�ͼƬid */  
    BOOLEAN                     is_pressed;     /*!< �Ƿ�ѡ�� */ 
    BOOLEAN	                    is_show;        /*!< �Ƿ���ʾ */      
    BOOLEAN                     is_gray;        /*!< �Ƿ�һ� */       
	BOOLEAN	                    is_long_press;  /*!< �Ƿ񳤰� */ 		 
	GUI_RECT_T                  rect;           /*!< ���� */ 
}GUI_BUTTON_T;

typedef enum
{
    GUISOFTKEY_BUTTONSTYLE_NONE,

    //pda��3��button���
	GUISOFTKEY_BUTTONSTYLE_1BUTTON,
    GUISOFTKEY_BUTTONSTYLE_2BUTTON,
    GUISOFTKEY_BUTTONSTYLE_3BUTTON,

	GUISOFTKEY_BUTTONSTYLE_MAX
}GUISOFTKEY_BUTTONSTYLE_E;

typedef enum
{
    // ��ͨ���
    GUISOFTKEY_STYLE_BAR,

    // button���
    GUISOFTKEY_STYLE_BUTTON,

	GUISOFTKEY_STYLE_MAX
}GUISOFTKEY_STYLE_E;

/*! @struct GUISOFTKEY_IMG_T
@brief  softkey������Ϣ
*/
typedef struct 
{
	MMI_IMAGE_ID_T      bg_img;         /*!< ����ͼƬ */
    MMI_IMAGE_ID_T      left_up_img;    /*!< �����upͼƬ */
    MMI_IMAGE_ID_T      left_down_img;  /*!< ���������ͼƬ */
    MMI_IMAGE_ID_T      mid_up_img;     /*!< �м����upͼƬ */
    MMI_IMAGE_ID_T      mid_down_img;   /*!< �м������ͼƬ */
    MMI_IMAGE_ID_T      right_up_img;   /*!< �Ҽ�upͼƬ */
    MMI_IMAGE_ID_T      right_down_img; /*!< ���������ͼƬ */
} GUISOFTKEY_IMG_T;

typedef struct
{
    MMI_TEXT_ID_T		leftsoft_id;    /*!< ����� */
	MMI_TEXT_ID_T		midsoft_id;     /*!< �м���� */
	MMI_TEXT_ID_T		rightsoft_id;   /*!< ����� */  
} GUISOFTKEY_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
