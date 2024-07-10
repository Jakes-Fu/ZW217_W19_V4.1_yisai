#ifndef _MMI_LIVEWALLPAPER_OPTION_H_
#define _MMI_LIVEWALLPAPER_OPTION_H_

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "mmi_id.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_position.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
//#include "mmiset_export.h"
#include "ui_layer.h"
#include "mmitheme_update.h"

#include "guilabel.h"
#include "guiform.h"
#include "guiownerdraw.h"
#include "mmidisplay_data.h"

typedef enum
{
	TYPE_OPEN_SET,
	TYPE_SELECT_LIST
}SetType;

//选择后调用OpenSetFun，可用于选择背景或弹出其它设置窗口等
typedef struct 
{
	void (*OpenSetFun)(void); 
}OpenSet; 

//弹出RADIO选项列表供选择，设置值范围：0-(num-1)
typedef struct 
{
	uint8 num;                            //RADIO选项个数
	MMI_TEXT_ID_T* item_text_id;          //RADIO选项名称数组
	uint16 (*GetSetting)(void);           //获取设置值，返回的是0-(num-1)
	void (*SaveSetting)(uint16 value_id); //保存设置值	
}SelectList; 

typedef struct
{
	MMI_TEXT_ID_T set_name_id;//设置类型名称
	SetType type;             //设置类型
	void *pitem;              //指向OpenSet/SelectList结构体
}ListItem;

//用于创建一个名字为win_title_id，有item_num个设置项的窗口
typedef struct 
{
	MMI_TEXT_ID_T win_title_id; //窗口名字
	uint16 item_num;            //设置项个数
	ListItem *plistitem;        //设置项数组 
}OPTIONLIST;

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMI_CreateOptionWin(OPTIONLIST *plist);

#ifdef   __cplusplus
    }
#endif

#endif  //_MMI_LIVEWALLPAPER_OPTION_H_


