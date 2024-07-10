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
*	2005/01/14  Neil.Jing	support runtime LCD��Font size setting
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
#define STRING_TABLE_POSITION 3  /*mb_u_int32  ����*/
#endif 

#ifndef INPUT_MAX_LENGTH
#define INPUT_MAX_LENGTH  1024 /*��������¼������ַ���*/
#endif  

#ifndef URL_MAX_LENGTH
#define URL_MAX_LENGTH  1024  /*url��ַ����󳤶�*/
#endif  

#ifndef CARD_MAX_LENGTH
#define CARD_MAX_LENGTH 20  /*card���ֵĳ���*/
#endif

#ifndef IS_IN_LOADING_STATE 
#define IS_IN_LOADING_STATE 0  /*����״̬*/
#endif 

#ifndef LOAD_OK_STATE 
#define LOAD_OK_STATE 1  /*���سɹ�״̬*/
#endif 

#ifndef LOAD_SUCCEED 
#define LOAD_SUCCEED 1  /*���سɹ�*/
#endif 
#ifndef LOAD_FAIL 
#define LOAD_FAIL 0  /*����ʧ��*/
#endif

#ifndef URL_HISTORY_LENGTH 
#define URL_HISTORY_LENGTH 6  /*��ʷ��ջ����*/
#endif 

#ifndef MAX_VARIABLE_NUMBER 
#define MAX_VARIABLE_NUMBER 20 /*WBXML�ĵ����ܰ�������������*/
#endif 

#ifndef MAX_TAG_NUMBER 
#define MAX_TAG_NUMBER 300  /*WBXML�ĵ����ܰ��������TAG��*/
#endif 

#ifndef MAX_ATTRIBUTE_NUMBER 
#define MAX_ATTRIBUTE_NUMBER 400   /*WBXML�ĵ����ܰ��������������*/
#endif

#ifndef MAX_CONTENT_NUMBER 
#define MAX_CONTENT_NUMBER 300  /*WBXML�ĵ����ܰ�����������ʾTAG��*/
#endif 

#ifndef MAX_IMAGE_NUMBER 
#define MAX_IMAGE_NUMBER 20  /*WBXML�ĵ����ܰ��������ͼƬ��*/
#endif 

#ifndef MAX_TEXT_LINE 
#define MAX_TEXT_LINE 500  /*��ǰ�����������*/
#endif 

#ifndef INDECK_HISTORY_LENGTH 
#define INDECK_HISTORY_LENGTH 6  /*Deck����ʷ��¼��ջ����󳤶�*/
#endif 

#ifndef MAX_STACK_NUMBER 
#define MAX_STACK_NUMBER 50  /*�洢��ջ��*/
#endif

#ifndef SMALLLEST_TIMEINTEVAL 
#define SMALLLEST_TIMEINTEVAL 3  /*��С��ʱ����*/
#endif 

#ifndef MAX_TAGNAME_LENGTH
#define MAX_TAGNAME_LENGTH  100  /*��ǩ���ֳ���*/
#endif

#ifndef DOWNLOAD_TIMEOUT_TTL 
#define DOWNLOAD_TIMEOUT_TTL 3000  /*����ʱ�䳤��*/
#endif

#ifndef MAX_SCRIPTBUF_LENGTH
#define MAX_SCRIPTBUF_LENGTH    1096  /*script �洢�ռ䳤��*/
#endif

#ifndef MAX_IMAGE_BUFFER_LENGTH
#define MAX_IMAGE_BUFFER_LENGTH  4096*3  /*ͼƬ�Ĵ洢����*/
#endif

#ifndef LENGTH_OF_VARIABLENAME
#define LENGTH_OF_VARIABLENAME   40  /*�������ֵĳ���*/
#endif

#ifndef VARIABLE_MAX_NUM
#define VARIABLE_MAX_NUM 12  /*��������*/ 
#endif

#ifndef NAME_MAX_LEN
#define NAME_MAX_LEN 64  /*name���Եĳ���*/  
#endif

#ifndef VALUE_MAX_LEN
#define VALUE_MAX_LEN INPUT_MAX_LENGTH  /*value���Եĳ���*/ 
#endif

/********************************************************************************************
*                               struct  declare
********************************************************************************************/
/*WAP BROWSER��������Ϣ�ṹ Add by Neil Jing For Spreadtrum*/
typedef struct _BrowserContext
{
	UInt16	WapWidth;			//wapҳ����ʾ����Ŀ��
	UInt16	WapHeight;			//wapҳ����ʾ����ĸ߶�
	UInt16	CharWidth;			//���ֽ��ַ��Ŀ��
	UInt16	CharsPerLine;		//ÿ���ַ���
	UInt16	LineHeight;			//ÿ�и߶�
	UInt16	LinesPerPage;		//ÿ��������
	UInt16	WBXMLLength;		//WBXML���ݵĳ���
}BrowserContext;

/*����*/
typedef struct _Variable
{
	char Name[NAME_MAX_LEN];  /*������*/
	char Value[VALUE_MAX_LEN];  /*����ֵ*/
} Variable;
/*�����ṹ*/
typedef struct _VariableStruct
{
	Variable Var[VARIABLE_MAX_NUM];  /*�����ṹ*/
	Int16 CurVar;  /*��ǰ����*/
	Int16 HeadVar;  /*ͷ����*/
	Int16 TailVar;  /*β����*/
} VariableStruct;
/*��ǩ��Ϣ���ڵ�ṹ����*/
typedef struct Tag_Info_Tree_Node_Tag
{
	Int16 RealTag;  /*Tag&0x3f,0xff��ʾ���ı�*/
	Int16 TagAttributesBegin;  /*���Կ�ʼ�±�*/
	Int16 TagAttributesEnd;  /*���Խ����±�*/
	Int16 TagContentBegin;  /*���ݿ�ʼ�±꣬���ı�Ϊ��WBXML�е��±꣬�Ա�ǩΪ����TEXT_LIST�е��±�*/
	Int16 TagContentEnd;  /*���ݽ����±꣬���ı�Ϊ��WBXML�е��±꣬�Ա�ǩΪ����TEXT_LIST�е��±�*/
	Int16 Parent;  /*���ڵ��±�*/
	Int16 NextBrother;  /*��һ���ֵܽڵ���±�*/
	Int16 FirstSon;  /*��һ���ӽڵ���±�*/
	char TagTextAttribute;  /*�ַ�������*/
							/***********************************************
							*  char NotUse;                                *
							*  7  6  5      4  3  2  1    0                *
							*  |  |  |      |  |  |  |    |                *
							*  br em strong i  b  u  big small             *
	***********************************************/
	char nouse;
	short eventhappen;  /*���ʱ��*/
} Tag_Info_Tree_Node;
/*�ı���Ϣ�ṹ����*/
typedef struct Text_List_Item_Tag
{
	Int16 RefElementIndex;  /*�������ContentList�±�*/
	char TextAttribute;  /*�ַ�������*/
						 /***********************************************
						 *  char NotUse;                                *
						 *  7  6  5      4  3  2  1    0                *
						 *  |  |  |      |  |  |  |    |                *
						 *  br em strong i  b  u  big small             *
	***********************************************/
	unsigned char *LineText;  /*�ַ���*/
	short PosX;  /*��ȡλ��*/
	short LineNum;  /*����*/
} Text_List_Item;
/*�ı���Ϣ�ṹ����*/
typedef struct Content_List_Item_Tag
{ 
	Int16  TextAttribute;  /*�ַ�������*/
						  /***********************************************
						  *  char NotUse;                                *
						  *  7  6  5      4  3  2  1    0                *
						  *  |  |  |      |  |  |  |    |                *
						  *  br em strong i  b  u  big small             *
	***********************************************/
	Int16 interval;  /*ʱ����*/
	Int16 ReferenceNode;  /*����������ڵ�*/
} Content_List_Item;
/*�ı���Ϣ�ṹ����*/
typedef struct Attribute_Item_Tag
{
	Int16 AttributeTag;  /*����*/
	Int16 AttributeBegin; /*�ַ������Կ�ʼ*/
	Int16 AttributeEnd;  /*�ַ������Կ�ʼ*/
} Attribute_Item;
/*��ʷ��ַ�ṹ����*/
typedef struct URL_History_Item_Tag
{
	char url[URL_MAX_LENGTH];  /*url��ַ*/
	Int16 Node;  /*��ؽڵ�*/
} URL_History_Item;
/*�����ṹ����*/
typedef struct Variable_Item_Tag
{
	Int16 VariableNameBegin;  /*��������ʼ*/
	Int16 VariableNameEnd;  /*����������*/
	char VariableValue[INPUT_MAX_LENGTH];  /*����ֵ�ַ���*/
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
/*ͼ����ʾ�ڵ�*/
typedef struct Image_Display_Item_Tag
{
	char ImageTag;  /*0x04ͼ���־*/
	char ImageArrayIndex;  /*ͼ��������±�1�ֽ�*/
	unsigned char ImageWidth;  /*ͼ����1�ֽ�*/
	unsigned char ImageHeight;  /*ͼ��߶�1�ֽ�*/
	unsigned char* ImageData;  /*ͼ�����ݿ�ʼ�±�2�ֽ�*/
	char StringEnd;  /*0x00�ַ�������־*/
    char nouse[3];
} Image_Display_Item;
/*�����ַ����ṹ*/
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
