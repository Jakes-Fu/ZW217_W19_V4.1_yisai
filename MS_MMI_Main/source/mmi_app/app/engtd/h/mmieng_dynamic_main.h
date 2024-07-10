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
#define MAX_DYNAENG_STR_LENGTH 255 //名字字符串的最大长度
#define MAX_DYNAENG_ARR_SIZE 50 //item 数组的最大长度

 

//结构体中某个item 的内容
typedef struct 
{ 

uint8 item_name[MAX_DYNAENG_STR_LENGTH]; // 名字 
uint32 arr_count; // 数组长度，如果为0，说明是普通变量（值保存在数组0 的位置） 
uint32 arr_value[MAX_DYNAENG_ARR_SIZE];// 数组内容 
uint32 min_value; // 取值范围（最小值） 
uint32 max_value; // 取值范围（最大值）


}ITEM_T; 



typedef struct
{
    uint16  parent_node_id;  //父节点编号
    uint16  node_id;         //节点编号
    uint16  index;           //顺序号
    wchar    name_ptr;        //节点名称
}GUI_MENUTREE_NODE_DATA_T; 





//描述主功能菜单结构
typedef struct 
{
	 int id; //id
	 wchar name[MAX_DYNAENG_STR_LENGTH]; // 名字
}DYNAENG_MAINFUNC_T;

typedef uint32 (*GetEngItemCount)(void); 
typedef void (*GetEngItem)(ITEM_T *item_ptr, uint32 index);
typedef void (*SetEngItem)(ITEM_T *item_ptr, uint32 index);

//描述子功能菜单结构
typedef struct 
{ 
	int parent_id; // 所属主功能的id 
	wchar name[MAX_DYNAENG_STR_LENGTH]; // 名字 
	GetEngItemCount getcountfunc; //获得item 个数的函数 
	GetEngItem getfunc; //获得指定index 的item 内容的函数 
	SetEngItem setfunc; //设置指定index 的item 内容的函数
}DYNAENG_SUBFUNC_T;



typedef struct
{
    uint16 dynaeng_mainfunc_arr_index ;  //main menutable index
    uint16 dynaeng_subfunc_arr_index;         //sub menutable index
    uint8   param_index;                                //index of param list;
    uint16  value_array_index;           //index of param value array
}SELECTED_DATA_T; 


//全局变量

extern  DYNAENG_MAINFUNC_T g_dynaeng_mainfunc_arr[] ;
extern  DYNAENG_SUBFUNC_T   g_dynaeng_subfunc_arr[] ;



void MMIEng_Dynamic_main(void);

#endif

#endif// ENGTD_SUPPORT
