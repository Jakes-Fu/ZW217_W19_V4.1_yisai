/********************************************************************
	FILE:	
		APPLE_OS.h
	DESCRIPTION:
		apple interface for os, included by os
	AUTHOR:		
		
	DATE:
		
*********************************************************************/
#ifndef APPLE_SMD_H
#define APPLE_SMD_H

#include "std.h"
typedef void (*FuncPtr) (void);
typedef struct _SXM_MenuItem
{
	int index;
	int title_str_type;//0:ID;1:ucs2
	unsigned int title;
	FuncPtr pEntry;
}SXM_MenuItem;

typedef struct _SXM_LISTCTRL_NODE
{
	unsigned short parent_menu_index;
	unsigned short menu_index;
    unsigned short itemType;    // itemtype
	unsigned int appid;         // itemtype = 1时为appid，为2时为malloc出来的url地址
	void *title;
	void *args;
}SXM_LISTCTRL_NODE;

typedef struct _SXM_CONFIG_DATA
{
	unsigned int dwID;
	unsigned int dwUpdate;
}SXM_CONFIG_DATA;

typedef struct _SXM_LISTCTRL
{
	SXM_LISTCTRL_NODE *node;
	unsigned int number_of_items;
	unsigned int active_menu_index;
	unsigned int active_item;

	SXM_CONFIG_DATA *cfg;
	unsigned int number_of_configs;

	unsigned int entry_appid;
}SXM_LISTCTRL;

typedef struct _SXM_Display_Menu 
{
	unsigned int app_id;
	unsigned char* tilte_str;
	unsigned int item_count;
	SXM_MenuItem *pItems;
}SXM_Display_Menu;
#define MAX_SUB_MENUS               (60)

/////////////////////////////////////////////////////////////////////////////////////
// List
////////////////////////////////////////////////////////////////////////////////////
//listitem子元素类型
typedef  enum 
{
	OEM_LISTCTL_DATA_TEXT_NONE = 0x0,
		OEM_LISTCTL_DATA_TEXT_BUFFER,
		OEM_LISTCTL_DATA_TEXT_IMAGEID,
		OEM_LISTCTL_DATA_TEXT_IMAGEBUFFER,
}LISTCTL_DATA_TYPE;
void OEM_ListCtl_Destroy();

//list整体分格
typedef enum
{
    OEM_LIST_TYPE_NONE,
		OEM_LIST_RADIOLIST, 							/*!<  single selected list */
		OEM_LIST_CHECKLIST, 							/*!<  multi selected list   */
		OEM_LIST_TEXTLIST,  							/*!<  text list */
		OEM_LIST_TYPE_MAX
}OEM_LIST_TYPE;


typedef enum
{
	/* 以下都为单行item, check为多选, radio为单选, number会在item前增加数字 */
    OEM_LIST_ITEM_STYLE_ONE_LINE_TEXT,
	OEM_LIST_ITEM_STYLE_ONE_LINE_ICON_TEXT,
	OEM_LIST_ITEM_STYLE_ONE_LINE_TWO_ICON_TEXT,
	OEM_LIST_ITEM_STYLE_ONE_LINE_NUMBER_TEXT,
	OEM_LIST_ITEM_STYLE_ONE_LINE_ICON_TWO_TEXT,
	OEM_LIST_ITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT,
	OEM_LIST_ITEM_STYLE_ONE_LINE_CHECK,
	OEM_LIST_ITEM_STYLE_ONE_LINE_RADIO
}OEM_LIST_ITEM_STYLE;

enum
{
	ID_NUM1=0,
	ID_NUM2,
	ID_NUM3,
	ID_NUM4,
	ID_NUM5,
	ID_NUM6,
	ID_NUM7,
	ID_NUM8,
	ID_NUM9,
	ID_NUM10,
	ID_NUM11,
	ID_NUM12,
	ID_NUM13,
	ID_NUM14,
	ID_NUM15,
	ID_NUM16,
	ID_NUM17,
	ID_NUM18,
	ID_NUM19,
	ID_NUM20,
	
	
};
typedef enum
{
SXH_UMVCHAT,
	SXH_TV,
	SXH_TV1,
	SXH_TV2,
	SXH_TV3,
	SXH_RADIO,
	SXH_ONLINEGAME,
	SXH_WEATHER,
	SXH_MUSIC,
	SXH_VIDELDL,
	SXH_BOOK,
}STARTAPPID;
typedef	signed long				S32;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#endif /* APPLE_SMD_H */ 

