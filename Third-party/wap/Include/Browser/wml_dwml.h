/* $Id: wml_dwml.h V1.0 2001/09/19 */
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
* FILE: wml_dwml.h
* MODULE: parse
*
*  PURPOSE:  Micro define,structure define,global variants or functions declare .
*
* AUTHOR(S): ZhiLi Wang
* GROUP: Browser Group
* DATE CREATED: 2001/09/20
* REFERENCE DOCUMENT ID: Detail Design of WapBrowser
* MODIFICATIONS:
*    Date        userName     Description
*    2001/07/20  ZhiLi Wang   Create this file
*	   2001/09/19  XueSong Wang Modify this file
*********************************************************************************************/
#ifndef _DWML_H_
#define _DWML_H_
/*********************************************************************************************
*									include block
*********************************************************************************************/
#include "wml_commondefines.h"
#include "wml_config.h"
/********************************************************************************************
*                                global variable  
********************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************************************
*                                global functions  declare
********************************************************************************************/
	extern Int8 ProcessEnter(short);  /*����enter*/
	extern Int8 ProcessInput(short ,char* );  /*����input*/
	extern short GetElementTitle(short contentindex,char * str,UInt16 len);  /*��ñ���*/
	extern void ProcessWhenDisplay(void);  /*��ʾ֮ǰ�������ж�*/
	extern char  BeginTimer(Int16 inteval);  /*��ʼ��ʱ*/
	extern Int16 FormatCardLineNew(Int16 CurNode);  /*����Ҫ��ʾcard���ı���*/
	extern void AddItemToUrlhistory(char * cururl);  /*��urlѹ����ʷջ*/ 
	extern void ProcessBack(void);  /*������*/
	extern void ProcessForward(void);  /*��ǰ���*/
	extern	void GetInputType(short contentindex,
		inputattr* inputunit);/*��ȡinputԪ�ص����Խṹ*/
	extern Int8   ProcessTimerEvent(); 
	extern unsigned char * GetPostDataPointer();
	extern short GetPostFlag();
	extern void ResetPostFlag();
	void WapCoreLoad(UInt16 wapWidth,UInt16 wapHeight,UInt16 fontSize,UInt16 maxWBXMLLength);/*��ʼ��wap*/
	void WapCoreUnload(void);/*ж��wap*/
	Int16 WapGetNumberofLines(void);/*��ȡ��������*/
	void WapSetNumberofImages(Int16 number);/*������ЧͼƬ��*/
	Int16 WapGetNumberofImages(void);/*��ȡͼ����Ŀ*/
	Int16 WapGetNumberofLines(void);/*��ȡ����*/
	int ProcessLoadedContent(char * buffer,int length);/*���������ĵ�*/
	unsigned char GetImageLoadedState(unsigned char  imageindex);/*��ȡͼ������״̬*/
	unsigned char GetImageWidth(unsigned char  imageindex);/*��ȡͼ����*/
	unsigned char  GetImageHeight(unsigned char  imageindex);/*��ȡͼ��߶�*/
	unsigned char * GetWbmpDataBegin(unsigned char  imageindex);/*��ȡͼ�񻺳�����ַ*/
	short  GetImageIndex(short contentindex);/*��ȡͼ������*/
	Text_List_Item * GetListItem(int index);/*��ȡ��ʾ��*/
	int GetNodeType(int elementindex);/*��ȡ�ڵ�tag����,����contentindex*/
	unsigned char * GetGHref(void);/*��ȡ��ǰurl*/
	void SetGHref(unsigned char * href);/*���õ�ǰurl*/
	void ClearImageState(int imageindex);/*���ͼ������״̬*/
	void SetImageData(int imageindex,int offset,unsigned char * data,int len);/*�������ص�ͼ��*/
	int GetImageHref(int imageindex,unsigned char * href);/*����ͼ�����ص�url*/
	unsigned char * GetImageBufferBase(void);/*��ȡͼ�񻺳�����С*/
	void WapSetPageType(char type);/*�����ĵ�����*/
	int  WapGetPageType(void);/*��ȡ�ĵ�����*/
	unsigned char * GetDocBuffer(void);/*��ȡ�ĵ�����ָ��*/
	int GetDocLength(void);/*��ȡ�ĵ�����*/
	Int16 FormatCardLineDefault(void);/*���²���*/
	int checkimagetype(unsigned char * buf);/*���ͼ������1:bmp;2:gif;3:jpg;4:wbmp*/
	unsigned char * GetImageBuffer(unsigned char  imageindex);/*��ȡ����ͼ����׵�ַ*/
	int GetImagelen(unsigned char  imageindex);/*��ȡ����ͼ��ĳ���*/
#ifdef __cplusplus
}
#endif

#endif  /*_DWML_H_*/
