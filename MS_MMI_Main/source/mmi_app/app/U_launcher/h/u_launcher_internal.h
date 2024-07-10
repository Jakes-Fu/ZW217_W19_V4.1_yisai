/*****************************************************************************
** File Name:      u_launcher_internal.h                                      *
** Author: kyle.jin         kyle                                              *
** Date:           14/02/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe go menu se                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012      kyle            Creat
******************************************************************************/
#ifndef _U_LAUNCHER_INTERNAL_H_
#define _U_LAUNCHER_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "guistring.h"
#include "graphics_3d_rotate.h"
#include "guimenu.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GO_DEFAULT_COEFFCIENT 40

/*****************************************************************
**   U �����㷨�ӿں��������
**
******************************************************************/
typedef struct GO_MAINMENU_ICON_INFO_TAG
{
    uint32* icon_buf;//ͼ��BUF��ַ
    IMGREF_POINT_T icon_offset;//ͼ���������Ļ���Ͻǵ�ƫ��
    IMGREF_SIZE_T icon_size;//ͼ���С
    uint16 item_row_num;//�ڼ���
    uint16 item_col_num;//�ڼ��� 
}GO_MAINMENU_ICON_INFO_T;

typedef struct GO_MAINMENU_PAGE_INFO_TAG
{
    uint8 total_row_num;//һ�����м���
    uint8 total_col_num;//һ�����м���
    uint16 icon_num;//һ��ͼ�����Ŀ
    GO_MAINMENU_ICON_INFO_T* icon_info;//ͼ������
}GO_MAINMENU_PAGE_INFO_T;

typedef enum
{
    GO_MOVE_NONE = 0,//��
    GO_MOVE_LEFT,//���󻬶�
    GO_MOVE_RIGHT,//���һ���
    GO_MOVE_UP,//���ϻ���
    GO_MOVE_DOWN,//���»���
    GO_MOVE_MAX
}GO_MOVE_DIRECTION_E;

typedef struct GO_rota_data_t {
	
   uint16	axis_x;

   uint16	axis_y;
															
   uint16	axis_z;
   
}GO_ROTA_DATA_T;

//�㷨��������Ϣ
typedef struct GO_CALC_DATA_INFO{
    S3D_DATA_TYPE_E input_type;
    S3D_DATA_TYPE_E output_type;
    S3D_EXPANDPTR_T* param;
}GO_CALC_DATA_INFO_T;

//�㷨��������:
typedef BOOLEAN (*go_calc_func)(GO_MAINMENU_PAGE_INFO_T *,GO_MAINMENU_PAGE_INFO_T *,GO_MAINMENU_PAGE_INFO_T *,uint16 ,uint16 ,IMGREF_SIZE_T ,void* ,GO_MOVE_DIRECTION_E ,FIX16_POINT_T*,GO_CALC_DATA_INFO_T*);

//ÿ����Ч��������Ϣ
typedef struct GO_CALC_STYLE_INFO{
    go_calc_func calc_func;//�㷨����
    GUIMENU_GO_SLIDE_TYPE_E slide_type;//֧�ֵĻ�������
    uint16 calc_time;//ÿ��Ч�������ʱ��
}GO_CALC_STYLE_INFO_T;

/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/


#endif/*_U_LAUNCHER_INTERNAL_H_*/