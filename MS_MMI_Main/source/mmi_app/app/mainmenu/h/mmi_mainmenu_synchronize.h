/*****************************************************************************
** File Name:      mmimenu_synchronize.h                                      *
** Author:                                                                   *
** Date:           05/20/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2011      jin.wang              Creat
******************************************************************************/
#ifndef _MMIMENU_SYNCHRONIZE_H_
#define _MMIMENU_SYNCHRONIZE_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "graphics_draw.h"
#include "mmitheme_mainmenu.h" 
#include "mmi_mainmenu_data.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIMENU_MAINMENU_STORE_STYLE_NUM ((uint16)GUIMENU_DEVICE_MAX - GUIMENU_DEVICE_SYSTEM)  //�м��ִ洢����
#define DYNAMIC_MAINMENU_DEFAULT_PAGE_INDEX 0xffff
#define DYNAMIC_MAINMENU_DEFAULT_ITEM_INDEX 0xffff
#define DATA_DIR_NAME "mainmenu"
#define DATA_FILE_FIX_PARA1 ".pr1"
#define DATA_FILE_FIX_PARA2 ".pr2"
#define DATA_FILE_FIX_ICON_ORI ".ori"
#define DATA_FILE_FIX_ICON_DEC ".dec"
#define DATA_FILE_FIX_CHECK_INFO "check_info"
#define DATA_FILE_FIX_COUNT "count"
#define DATA_FILE_FIX_DATA "data"
#define DYNAMIC_DEV_SPACE_LIMIT (32000)
#define DATA_FILE_SIZE_CHECK_INFO (32)
#define DATA_FILE_SIZE_COUNT (32)
//������Ŀǰƽ̨֧�ֲ���100����ͼ���޷�����
#define DYNAMIC_MAINMENU_NUM_LIMIT (100)

#define MMIMENU_ICON_BORDER_RESERVE 4

//ĳ���̷��¶�̬�˵����ݵ��ܴ�С��ʵ��Ӧ��ԶԶС�ڴ�ֵ
#define DATA_FILE_SIZE_LIMIT (999999)

#define ICON_DEC_WIDTH (MENUICON_WIDTH-MMIMENU_ICON_BORDER_RESERVE)

#define ICON_DEC_HEIGHT (MENUICON_HEIGHT-MMIMENU_ICON_BORDER_RESERVE)

//����ͼƬ��������ռ��С
#define ICON_FILE_SIZE (ICON_DEC_WIDTH * ICON_DEC_HEIGHT * 4)

//���������ռ�����
#define RUNDATA_FILE_SIZE ICON_FILE_SIZE_LIMIT

//����ͼƬ��������ռ��趨����ʵ��ԶԶС�������ֵ
#define ICON_FILE_SIZE_LIMIT (MENUICON_HEIGHT * MENUICON_WIDTH * 4)
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
enum
{
    GUIMENU_READFILE_NONE_STYLE = 0,   /*!< �� */
    GUIMENU_READFILE_DECODE = 1,       /*!< ��ȡ����ͼƬ */
    GUIMENU_READFILE_ORIG = 2,         /*!< ��ȡԭʼͼƬ */
    GUIMENU_READFILE_BOTH = 1|2,       /*!< ���롢ԭʼ����ȡ */
};
typedef uint32 GUIMENU_READFILE_TYPE;

enum
{
    GUIMENU_GET_INFO_NONE = 0,   /*!< ����ȡ */
    GUIMENU_GET_INFO_PARA = 1,   /*!< ��ȡ�����ļ� */
    GUIMENU_GET_INFO_ICON = 2,   /*!< ��ȡͼ���ļ� */
    GUIMENU_GET_INFO_ALL = 1|2,  /*!< ��ȡ������ͼ���ļ� */
};
typedef uint32 GUIMENU_GET_INFO_T;

/*! @struct  MMIMENU_SYN_VALUE_E
@brief ���ж�̬�˵�����ֵ����
*/
typedef enum
{
	MMIMENU_SYN_VALUE_SUC,
	MMIMENU_SYN_VALUE_NOSPACE,	
	MMIMENU_SYN_VALUE_INVALID,
	MMIMENU_SYN_VALUE_EXIST,
	MMIMENU_SYN_VALUE_ERROR,

	MMIMENU_SYN_VALUE_MAX// ϵͳ����
}MMIMENU_SYN_VALUE_E;

/*! @struct  MMIMENU_SYN_HEAD_T
@brief ���ж�̬�˵����ļ�ϵͳ�еĴ洢��ʽ
*/
typedef struct
{
    uint32 ver_info;                                    /*!< ϵͳ�汾��Ϣ�����ڼ�����ݺϷ��� */
    uint32 check_info;                                  /*!< ��̬�˵��ж��Ƿ���Ҫͬ��*/
    uint32 dy_menu_num;                                 /*!< ��̬�˵����� */
} MMIMENU_SYN_HEAD_T;

// ͼƬ���Ż�������
typedef struct
{
    uint8               *buf_ptr;
    uint8               *target_buf_ptr;
    uint16              width;
    uint16              height;
} MMIMENU_SYN_ICON_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ����һ����̬menu
@author jin.wang
@param dynamic_item_ptr [in] ����item������
@param store_idx [in] ��װ�̷�
@return �����Ƿ�ɹ�
@note
- dynamic_item_ptr�У����²���Ҫ�ṩ
    -# uint16 page_index
    -# uint16 item_index
    -# MMI_MENU_ID_T menu_id
- dynamic_item_ptr�е�dynamic_menu_info��Ҫ��д����
@sa MMIMENU_DyDeleteItem
@sa MMIMENU_DyDeleteItemByFunc
*******************************************************************************/
PUBLIC MMIMENU_SYN_VALUE_E MMIMENU_DyAppendItem(
												GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr,
												GUIMENU_FILE_DEVICE_E store_idx
												);

/***************************************************************************//*!
@brief ɾ��һ����̬item
@author jin.wang
@param dynamic_item_ptr [in] ��ɾ��item������
@return �����Ƿ�ɹ�
@note ��ʵ���Ǹ�����ʾ�ַ�������������
- dynamic_item_ptr�У�����Ҫ�ṩdynamic_menu_info�еĲ�������
    -# wchar text[GUIMENU_DYMAINMENU_STR_MAX_NUM + 1]
    -# DY_MENU_ITEM_LINK_FUNC_T link_function_ptr
@sa MMIMENU_DyAppendItem
@sa MMIMENU_DyDeleteItemByFunc
*******************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyDeleteItem(
    GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr
);

/***************************************************************************//*!
@brief ɾ��ͳһ�������������в˵�
@author jin.wang
@param link_func_ptr [in] ����������ָ��
@return �����Ƿ�ɹ�
@note ����һ���Ӧ�ò�����ͬ����������������java����ʱͳһɾ���˵�
@sa MMIMENU_DyAppendItem
@sa MMIMENU_DyDeleteItem
*******************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyDeleteItemByFunc(
    DY_MENU_ITEM_LINK_FUNC_T link_func_ptr
);

/*****************************************************************************/
// Description : �ͷ����������ڴ�
// Global resource dependence : 
// Author:jin.wang
// Note:���ض�̬�˵�������Ϣ
/*****************************************************************************/
PUBLIC void MMIMENU_DyFreeParmInfo(
    GUIMAINMENU_DY_ITEM_T *dy_item_ptr
);

/*****************************************************************************/
// Description :��̬���˵���ʼ��
// Global resource dependence : 
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyInit(void);

/*****************************************************************************/
// Description :�ж϶�̬�˵��Ƿ���Ҫͬ��
// Global resource dependence : 
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyCheckIsSyn(void);

/*****************************************************************************/
// Description :���ö�̬���˵�λ����Ϣ
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DySaveItemPos(
    uint16                      item_total_num,                         
    BOOLEAN                     is_move_state,                          
    MMIMAINMENU_PAGE_ITEM_INFO_T *page_info_ptr
);

/*****************************************************************************/
// Description : ��ȡ��̬���˵��ܸ���
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC uint32 MMIMENU_DyGetItemCount(void);

/*****************************************************************************/
// Description : ��ȡ���в˵�����
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyGetAllItemData(void);
/*****************************************************************************/
// Description :  ��ȡ��̬�˵�����
// Global resource dependence :
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySetItemData(
    MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr,
    MMIMAINMENU_TYPE_E style,
	BOOLEAN is_organize,                                                //�Ƿ�����֯ģʽ��
	uint16 item_total_num,
    MMI_IMAGE_ID_T select_icon_id
);
/*****************************************************************************/
// Description : ɾ��һ����̬item
// Global resource dependence :
// Author:jin.wang
// Note:�ɹ�����TRUE��ʧ�ܷ���FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyDeleteItemEx(
    CTRLMAINMENU_ITEM_T *dynamic_item_ptr
);

/*****************************************************************************/
// Description : �ͷŶ�̬�˵����������ڴ�
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyFreeData(void);

/*****************************************************************************/
// Description : �ж����������������Ƿ���ȷ
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyIsRunInfoValid(GUIMAINMENU_DYNAMIC_DATA_T *data_ptr);

/*****************************************************************************/
// Description : ����menu_id��ȡslideģʽ�¶�̬�˵���������
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySlideGetRunInfo(
    MMI_MENU_ID_T menu_id,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : ����list_index��ȡlistģʽ�¶�̬�˵���������
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyListGetRunInfo(
    uint16 list_index,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : ����menu_id��ȡslideģʽ�¶�̬�˵�ͼƬ����
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySlideGetIcon(
    MMI_MENU_ID_T menu_id,
    uint8 *icon_info_ptr
);

/*****************************************************************************/
// Description : ����menu_id��ȡslideģʽ�¶�̬�˵���ʾ����(������ͼƬ����)
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySlideGetDispInfo(
    MMI_MENU_ID_T menu_id,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : ����menu_id��ȡlistģʽ�¶�̬�˵���ʾ����
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyListGetDispInfo(
    uint16 list_index,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : ����menu_id��ȡlistģʽ�¶�̬�˵���ʾ����(������ͼƬ����)
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyListGetDispInfoEx(
    uint16 list_index,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : �ж�һ����̬�˵������Ƿ����
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_IsDyItemExist(
    MMI_MENU_ID_T menu_id
);

/*****************************************************************************/
// Description : ���ø���ͼƬid
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_SetSelectedId(
    MMI_IMAGE_ID_T select_icon_id
);

/*****************************************************************************/
// Description : ����text��
// Global resource dependence :
// Author:nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DySetItemStringArray(
										 GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr,
										 GUIMAINMENU_DYNAMIC_STRING_DATA_T *string_arr,
										 uint16 arr_num
										 );

/*****************************************************************************/
// Description : ������ʾ�õ�text
// Global resource dependence :
// Author:nan.ji
// Note:��java��WRE���벻һ��ʱ��Ĭ��������ʾ����Ϊ�������׸�data��
/*****************************************************************************/
PUBLIC void MMIMENU_DySetItemString(
									GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr
									);

/*****************************************************************************/
// Description : ��ȡ��ǰ���Ի�����Ӧ��text
// Global resource dependence :
// Author:nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyGetItemString(
									GUIMAINMENU_DY_ITEM_T *dynamic_item_ptr
									);
/*****************************************************************************/
// Description : �ͷŶ�̬�˵����������ڴ�
// Global resource dependence :
// Author:.nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyDestroy(
                                void
                                );
/*****************************************************************************/
// Description : ���ж�̬menu
// Global resource dependence :
// Author:nan.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyRunItem(
                                 MMI_MENU_GROUP_ID_T group_id,
								 MMI_MENU_ID_T menu_id
                                 );                                
#endif

#endif

