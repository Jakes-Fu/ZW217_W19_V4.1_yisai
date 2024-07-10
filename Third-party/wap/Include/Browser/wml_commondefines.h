/* $Id: ps_actionprocess.h V1.0 2001/09/19 */
/**********************************************************************************************
*    This source code has been made available to you by CORETEK on an AS-IS
*    basis.  Anyone receiving this source is licensed under CORETEK
*    copyrights to use it in any way he or she deems fit, including
*    copying it, modifying it, compiling it, and redistributing it either
*    with or without modifications.
*
*    Any person who transfers this source code or any derivative work
*    must include the CORETEK copyright notice, this paragraph, and the
*    preceding two paragraphs in the transferred software.
*
*    COPYRIGHT   CORETEK  CORPORATION 2001
*    LICENSED MATERIAL  -  PROGRAM PROPERTY OF CORETEK
*********************************************************************************************/

/**********************************************************************************************
* FILE: ps_actionprocess.h
* MODULE: Parse Wml
*
*  PURPOSE:  Macro define,structure define,global variants or functions declare .
*
* AUTHOR(S): ZhiLi Wang
* GROUP: Browser Group
* DATE CREATED: 2001/09/20
* REFERENCE DOCUMENT ID: Detail Design of WapBrowser
* MODIFICATIONS:
*    Date        userName     Description
*   2001/07/20  ZhiLi Wang   Create this file
*	2001/09/19  XueSong Wang Modify this file
*	2005/01/14  Neil.Jing	support runtime LCD、Font size setting
*********************************************************************************************/
#ifndef _COMMONDEFINES_H_
#define _COMMONDEFINES_H_
#include "wml_config.h"
#include "wml_typeswml.h"
/********************************************************************************************
*                               macro  declare
********************************************************************************************/
#define True  (Boolean)1
#define False (Boolean)0

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef STRING_TABLE_POSITION
#define STRING_TABLE_POSITION 3  /*mb_u_int32  编码*/
#endif 

#ifndef INPUT_MAX_LENGTH
#define INPUT_MAX_LENGTH  1024 /*输入框可以录入最大字符数*/
#endif  

#ifndef URL_MAX_LENGTH
#define URL_MAX_LENGTH  1024  /*url地址的最大长度*/
#endif  

#ifndef CARD_MAX_LENGTH
#define CARD_MAX_LENGTH 20  /*card名字的长度*/
#endif

#ifndef IS_IN_LOADING_STATE 
#define IS_IN_LOADING_STATE 0  /*下载状态*/
#endif 

#ifndef LOAD_OK_STATE 
#define LOAD_OK_STATE 1  /*下载成功状态*/
#endif 

#ifndef LOAD_SUCCEED 
#define LOAD_SUCCEED 1  /*下载成功*/
#endif 
#ifndef LOAD_FAIL 
#define LOAD_FAIL 0  /*下载失败*/
#endif

#ifndef URL_HISTORY_LENGTH 
#define URL_HISTORY_LENGTH 6  /*历史堆栈长度*/
#endif 

#ifndef MAX_VARIABLE_NUMBER 
#define MAX_VARIABLE_NUMBER 20 /*WBXML文档中能包含的最大变量数*/
#endif 

#ifndef MAX_TAG_NUMBER 
#define MAX_TAG_NUMBER 300  /*WBXML文档中能包含的最大TAG数*/
#endif 

#ifndef MAX_ATTRIBUTE_NUMBER 
#define MAX_ATTRIBUTE_NUMBER 400   /*WBXML文档中能包含的最大属性数*/
#endif

#ifndef MAX_CONTENT_NUMBER 
#define MAX_CONTENT_NUMBER 300  /*WBXML文档中能包含的最大可显示TAG数*/
#endif 

#ifndef MAX_IMAGE_NUMBER 
#define MAX_IMAGE_NUMBER 20  /*WBXML文档中能包含的最大图片数*/
#endif 

#ifndef MAX_TEXT_LINE 
#define MAX_TEXT_LINE 500  /*当前卡的最大行数*/
#endif 

#ifndef INDECK_HISTORY_LENGTH 
#define INDECK_HISTORY_LENGTH 6  /*Deck内历史记录堆栈的最大长度*/
#endif 

#ifndef MAX_STACK_NUMBER 
#define MAX_STACK_NUMBER 50  /*存储堆栈数*/
#endif

#ifndef SMALLLEST_TIMEINTEVAL 
#define SMALLLEST_TIMEINTEVAL 3  /*最小的时间间隔*/
#endif 

#ifndef MAX_TAGNAME_LENGTH
#define MAX_TAGNAME_LENGTH  100  /*标签名字长度*/
#endif

#ifndef DOWNLOAD_TIMEOUT_TTL 
#define DOWNLOAD_TIMEOUT_TTL 3000  /*下载时间长度*/
#endif

#ifndef MAX_SCRIPTBUF_LENGTH
#define MAX_SCRIPTBUF_LENGTH    1096  /*script 存储空间长度*/
#endif

#ifndef MAX_IMAGE_BUFFER_LENGTH
#define MAX_IMAGE_BUFFER_LENGTH  4096*3  /*图片的存储长度*/
#endif

#ifndef LENGTH_OF_VARIABLENAME
#define LENGTH_OF_VARIABLENAME   40  /*变量名字的长度*/
#endif

#ifndef VARIABLE_MAX_NUM
#define VARIABLE_MAX_NUM 12  /*最大变量数*/ 
#endif

#ifndef NAME_MAX_LEN
#define NAME_MAX_LEN 64  /*name属性的长度*/  
#endif

#ifndef VALUE_MAX_LEN
#define VALUE_MAX_LEN INPUT_MAX_LENGTH  /*value属性的长度*/ 
#endif

/********************************************************************************************
*                               struct  declare
********************************************************************************************/
/*WAP BROWSER上下文信息结构 Add by Neil Jing For Spreadtrum*/
typedef struct _BrowserContext
{
	UInt16	WapWidth;			//wap页面显示区域的宽度
	UInt16	WapHeight;			//wap页面显示区域的高度
	UInt16	CharWidth;			//单字节字符的宽度
	UInt16	CharsPerLine;		//每行字符数
	UInt16	LineHeight;			//每行高度
	UInt16	LinesPerPage;		//每屏的行数
	UInt16	WBXMLLength;		//WBXML数据的长度
}BrowserContext;

/*变量*/
typedef struct _Variable
{
	char Name[NAME_MAX_LEN];  /*变量名*/
	char Value[VALUE_MAX_LEN];  /*变量值*/
} Variable;
/*变量结构*/
typedef struct _VariableStruct
{
	Variable Var[VARIABLE_MAX_NUM];  /*变量结构*/
	Int16 CurVar;  /*当前变量*/
	Int16 HeadVar;  /*头变量*/
	Int16 TailVar;  /*尾变量*/
} VariableStruct;
/*标签信息树节点结构定义*/
typedef struct Tag_Info_Tree_Node_Tag
{
	Int16 RealTag;  /*Tag&0x3f,0xff表示纯文本*/
	Int16 TagAttributesBegin;  /*属性开始下标*/
	Int16 TagAttributesEnd;  /*属性结束下标*/
	Int16 TagContentBegin;  /*内容开始下标，对文本为在WBXML中的下标，对标签为其在TEXT_LIST中的下标*/
	Int16 TagContentEnd;  /*内容结束下标，对文本为在WBXML中的下标，对标签为其在TEXT_LIST中的下标*/
	Int16 Parent;  /*父节点下标*/
	Int16 NextBrother;  /*下一个兄弟节点的下标*/
	Int16 FirstSon;  /*第一个子节点的下标*/
	char TagTextAttribute;  /*字符串属性*/
							/***********************************************
							*  char NotUse;                                *
							*  7  6  5      4  3  2  1    0                *
							*  |  |  |      |  |  |  |    |                *
							*  br em strong i  b  u  big small             *
	***********************************************/
	char nouse;
	short eventhappen;  /*访问标记*/
} Tag_Info_Tree_Node;
/*文本信息结构定义*/
typedef struct Text_List_Item_Tag
{
	Int16 RefElementIndex;  /*相关连的ContentList下标*/
	char TextAttribute;  /*字符串属性*/
						 /***********************************************
						 *  char NotUse;                                *
						 *  7  6  5      4  3  2  1    0                *
						 *  |  |  |      |  |  |  |    |                *
						 *  br em strong i  b  u  big small             *
	***********************************************/
	unsigned char *LineText;  /*字符串*/
	short PosX;  /*读取位置*/
	short LineNum;  /*行数*/
} Text_List_Item;
/*文本信息结构定义*/
typedef struct Content_List_Item_Tag
{ 
	Int16  TextAttribute;  /*字符串属性*/
						  /***********************************************
						  *  char NotUse;                                *
						  *  7  6  5      4  3  2  1    0                *
						  *  |  |  |      |  |  |  |    |                *
						  *  br em strong i  b  u  big small             *
	***********************************************/
	Int16 interval;  /*时间间隔*/
	Int16 ReferenceNode;  /*相关连的树节点*/
} Content_List_Item;
/*文本信息结构定义*/
typedef struct Attribute_Item_Tag
{
	Int16 AttributeTag;  /*属性*/
	Int16 AttributeBegin; /*字符串属性开始*/
	Int16 AttributeEnd;  /*字符串属性开始*/
} Attribute_Item;
/*历史地址结构定义*/
typedef struct URL_History_Item_Tag
{
	char url[URL_MAX_LENGTH];  /*url地址*/
	Int16 Node;  /*相关节点*/
} URL_History_Item;
/*变量结构定义*/
typedef struct Variable_Item_Tag
{
	Int16 VariableNameBegin;  /*变量名开始*/
	Int16 VariableNameEnd;  /*变量名结束*/
	char VariableValue[INPUT_MAX_LENGTH];  /*变量值字符串*/
} Variable_Item;

typedef struct Image_Data_Item_Tag
{
	Int16 hrefbegin;
	Int16 hrefend;
	char hreftype;
	/*0 orige 1 add http:// 2 add https:*/
	char  Loaded;
	char  nouse[2];
	unsigned char* ImageData; 
	UInt16 tid;
	UInt16 len;
} Image_Data_Item;
/*图像显示节点*/
typedef struct Image_Display_Item_Tag
{
	char ImageTag;  /*0x04图象标志*/
	char ImageArrayIndex;  /*图象数组的下标1字节*/
	unsigned char ImageWidth;  /*图像宽度1字节*/
	unsigned char ImageHeight;  /*图象高度1字节*/
	unsigned char* ImageData;  /*图象数据开始下标2字节*/
	char StringEnd;  /*0x00字符串进标志*/
    char nouse[3];
} Image_Display_Item;
/*输入字符串结构*/
typedef struct{
	char name[100];
	Boolean texttype;
	char value[INPUT_MAX_LENGTH];
	char format[100];
	Boolean  emptyok;
	short size;
	short maxlength;
	short tabindex;
	char title[100];
	char accesskey[100];
}inputattr;
#endif /*_COMMONDEFINES_H_*/
