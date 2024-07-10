/*****************************************************************************
** File Name:      mmieng_Dynamic_main.h                                        *
** Author:           sword kim                                                             *
** Date:              2007-3-15                                               *
** Copyright:      Lenovo mobile, Incorporated. All Rights Reserved.         *
** Description:    This file is used to eng  mode dynamic param            *
******************************************************************************/
#ifdef ENGTD_SUPPORT
#ifndef _MMIENG_DYNAMIC_MAIN_H_
#define _MMIENG_DYNAMIC_MAIN_H_

#include "window_parse.h"
#include "mmk_app.h" 
#include "guimenu.h"
#include "guilistbox.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmitheme_pubwin.h"
#include "mmipub.h"
#include "mmidisplay_data.h"
#include "guimsgbox.h"
#include "mmi_textfun.h"
#include "mmi_id.h"
#include "mmieng_text.h"

#define MMIENGTD_EDITBOX_HEIGHT 20
#define MAX_DYNAENG_STR_LENGTH 255 //�����ַ�������󳤶�
#define MAX_DYNAENG_ARR_SIZE 50 //item �������󳤶�

 

//�ṹ����ĳ��item ������
typedef struct 
{ 

uint8 item_name[MAX_DYNAENG_STR_LENGTH]; // ���� 
uint32 arr_count; // ���鳤�ȣ����Ϊ0��˵������ͨ������ֵ����������0 ��λ�ã� 
uint32 arr_value[MAX_DYNAENG_ARR_SIZE];// �������� 
uint32 min_value; // ȡֵ��Χ����Сֵ�� 
uint32 max_value; // ȡֵ��Χ�����ֵ��


}ITEM_T; 



typedef struct
{
    uint16  parent_node_id;  //���ڵ���
    uint16  node_id;         //�ڵ���
    uint16  index;           //˳���
    wchar    name_ptr;        //�ڵ�����
}GUI_MENUTREE_NODE_DATA_T; 





//���������ܲ˵��ṹ
typedef struct 
{
	 int id; //id
	 wchar name[MAX_DYNAENG_STR_LENGTH]; // ����
}DYNAENG_MAINFUNC_T;

typedef uint32 (*GetEngItemCount)(void); 
typedef void (*GetEngItem)(ITEM_T *item_ptr, uint32 index);
typedef void (*SetEngItem)(ITEM_T *item_ptr, uint32 index);

//�����ӹ��ܲ˵��ṹ
typedef struct 
{ 
	int parent_id; // ���������ܵ�id 
	wchar name[MAX_DYNAENG_STR_LENGTH]; // ���� 
	GetEngItemCount getcountfunc; //���item �����ĺ��� 
	GetEngItem getfunc; //���ָ��index ��item ���ݵĺ��� 
	SetEngItem setfunc; //����ָ��index ��item ���ݵĺ���
}DYNAENG_SUBFUNC_T;



typedef struct
{
    uint16 dynaeng_mainfunc_arr_index ;  //main menutable index
    uint16 dynaeng_subfunc_arr_index;         //sub menutable index
    uint8   param_index;                                //index of param list;
    uint16  value_array_index;           //index of param value array
}SELECTED_DATA_T; 


//ȫ�ֱ���

extern  DYNAENG_MAINFUNC_T g_dynaeng_mainfunc_arr[] ;
extern  DYNAENG_SUBFUNC_T   g_dynaeng_subfunc_arr[] ;



void MMIEng_Dynamic_main(void);

#endif

#endif// ENGTD_SUPPORT
