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
	extern Int8 ProcessEnter(short);  /*处理enter*/
	extern Int8 ProcessInput(short ,char* );  /*处理input*/
	extern short GetElementTitle(short contentindex,char * str,UInt16 len);  /*获得标题*/
	extern void ProcessWhenDisplay(void);  /*显示之前，进行判断*/
	extern char  BeginTimer(Int16 inteval);  /*开始计时*/
	extern Int16 FormatCardLineNew(Int16 CurNode);  /*处理要显示card的文本行*/
	extern void AddItemToUrlhistory(char * cururl);  /*把url压入历史栈*/ 
	extern void ProcessBack(void);  /*向后浏览*/
	extern void ProcessForward(void);  /*向前浏览*/
	extern	void GetInputType(short contentindex,
		inputattr* inputunit);/*获取input元素的属性结构*/
	extern Int8   ProcessTimerEvent(); 
	extern unsigned char * GetPostDataPointer();
	extern short GetPostFlag();
	extern void ResetPostFlag();
	void WapCoreLoad(UInt16 wapWidth,UInt16 wapHeight,UInt16 fontSize,UInt16 maxWBXMLLength);/*初始化wap*/
	void WapCoreUnload(void);/*卸载wap*/
	Int16 WapGetNumberofLines(void);/*获取内容行数*/
	void WapSetNumberofImages(Int16 number);/*设置有效图片数*/
	Int16 WapGetNumberofImages(void);/*获取图像数目*/
	Int16 WapGetNumberofLines(void);/*获取行数*/
	int ProcessLoadedContent(char * buffer,int length);/*处理下载文档*/
	unsigned char GetImageLoadedState(unsigned char  imageindex);/*获取图像下载状态*/
	unsigned char GetImageWidth(unsigned char  imageindex);/*获取图像宽度*/
	unsigned char  GetImageHeight(unsigned char  imageindex);/*获取图像高度*/
	unsigned char * GetWbmpDataBegin(unsigned char  imageindex);/*获取图像缓冲区地址*/
	short  GetImageIndex(short contentindex);/*获取图像索引*/
	Text_List_Item * GetListItem(int index);/*获取显示行*/
	int GetNodeType(int elementindex);/*获取节点tag类型,传入contentindex*/
	unsigned char * GetGHref(void);/*获取当前url*/
	void SetGHref(unsigned char * href);/*设置当前url*/
	void ClearImageState(int imageindex);/*清除图像下载状态*/
	void SetImageData(int imageindex,int offset,unsigned char * data,int len);/*保存下载的图像*/
	int GetImageHref(int imageindex,unsigned char * href);/*生成图像下载的url*/
	unsigned char * GetImageBufferBase(void);/*获取图像缓冲区大小*/
	void WapSetPageType(char type);/*设置文档类型*/
	int  WapGetPageType(void);/*获取文档类型*/
	unsigned char * GetDocBuffer(void);/*获取文档数据指针*/
	int GetDocLength(void);/*获取文档长度*/
	Int16 FormatCardLineDefault(void);/*重新布局*/
	int checkimagetype(unsigned char * buf);/*检查图像类型1:bmp;2:gif;3:jpg;4:wbmp*/
	unsigned char * GetImageBuffer(unsigned char  imageindex);/*获取下载图像的首地址*/
	int GetImagelen(unsigned char  imageindex);/*获取下载图像的长度*/
#ifdef __cplusplus
}
#endif

#endif  /*_DWML_H_*/
