
#if !defined(__MSGIDDEF_H__)
#define __MSGIDDEF_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// 消息响应函数格式为 void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
// 的消息ID
/////////////////////////////////////////////////////////////////////////////

// 用户改变了程序的语言
#define WM_LANGUAGE_CHANGE     (WM_USER+600)     

// 用户选择不同的信息种类
#define WM_CATEGORY_CHANGE     (WM_USER+601)     

// 条目的内容改变
#define WM_UPDATE_ITEM_CONTENT (WM_USER+602)

// 电话群组改变
#define WM_UPDATE_GROUP_CHANGE (WM_USER+604)     
typedef struct _UIC_HINT
{
	DWORD dwFlag;
	DWORD dwData;
} UIC_HINT, * PUIC_HINT;


/////////////////////////////////////////////////////////////////////////////
// 消息响应函数格式为 LRESULT OnXXX(WPARAM wParam, LPARAM lParam)
// 的消息ID
/////////////////////////////////////////////////////////////////////////////

// 传输结束, wParam表示需要显示的信息的ID
#define WM_TRANSMIT_END     (WM_USER+602)     

// 操作过程指示消息, wParam表过程的状态， lParam 表位置
#define WM_PROGRESS         (WM_USER+603)     
// 过程的状态                                           
const int PROG_BEGIN   = 0;
const int PROG_PROCEED = 1;
const int PROG_END     = 2;
const int PROG_TITLE   = 3;

#endif // __MSGIDDEF_H__