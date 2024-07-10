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

//ѡ������OpenSetFun��������ѡ�񱳾��򵯳��������ô��ڵ�
typedef struct 
{
	void (*OpenSetFun)(void); 
}OpenSet; 

//����RADIOѡ���б�ѡ������ֵ��Χ��0-(num-1)
typedef struct 
{
	uint8 num;                            //RADIOѡ�����
	MMI_TEXT_ID_T* item_text_id;          //RADIOѡ����������
	uint16 (*GetSetting)(void);           //��ȡ����ֵ�����ص���0-(num-1)
	void (*SaveSetting)(uint16 value_id); //��������ֵ	
}SelectList; 

typedef struct
{
	MMI_TEXT_ID_T set_name_id;//������������
	SetType type;             //��������
	void *pitem;              //ָ��OpenSet/SelectList�ṹ��
}ListItem;

//���ڴ���һ������Ϊwin_title_id����item_num��������Ĵ���
typedef struct 
{
	MMI_TEXT_ID_T win_title_id; //��������
	uint16 item_num;            //���������
	ListItem *plistitem;        //���������� 
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


