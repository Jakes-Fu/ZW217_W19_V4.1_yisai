/*******************************************************************
* Lite Window Handling Framework
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef WINDOWSHANDLE_H
#define WINDOWSHANDLE_H
#include <commdlg.h>
#include <commctrl.h>
#include <richedit.h>
#include <tchar.h>

#ifdef _USRDLL
#define EXPORTCLASS __declspec(dllexport)
#else
#define EXPORTCLASS
#endif

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
int CALLBACK DlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

typedef struct {
	int id;
	void* data;
} mtable;

class CMatchTable {
public:
	mtable* add(int id);
	int remove(int id);
	mtable* match(int id);
	mtable* finematch(int id);
	void reset();
	virtual void release();
	CMatchTable() {reset();}
	~CMatchTable() {reset();}
	mtable* table;
	int count;
	int idx;
};

class CWindow
{
public:
	void Move(int x,int y,int w,int h);
	void StretchFull();
	void StretchItemFull(HWND hwndItem);
	void Hide() {ShowWindow(hwnd,SW_HIDE);}
	void Show()	{ShowWindow(hwnd,SW_SHOW);}
	void SetForeground() {SetForegroundWindow(hwnd);}
	void Minimize() {ShowWindow(hwnd,SW_MINIMIZE);}
	void Maximize() {ShowWindow(hwnd,SW_MAXIMIZE);}
	void Restore() {ShowWindow(hwnd,SW_RESTORE);}
	void Focus() {SetFocus(hwnd);}
	void Copy() {SendMessage(hwnd, WM_COPY, 0, 0);}
	void Paste() {SendMessage(hwnd, WM_PASTE, 0, 0);}
	void Cut () {SendMessage(hwnd, WM_CUT, 0, 0);}
	void Undo() {SendMessage(hwnd, EM_UNDO, 0, 0);}
	void Redo() {SendMessage(hwnd, EM_REDO, 0, 0);}
	void Clear() {SendMessage(hwnd, WM_SETTEXT,0,0);}
	void SetCaption(LPCTSTR lpText) {SetWindowText(hwnd,lpText);}
	void Close() {
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
	void Destroy() {
		if (!hwnd) return;
		HWND hwndToDestroy = hwnd;
		hwnd = 0;
		DestroyWindow(hwndToDestroy);
	}
	int Ask(TCHAR* text, int type) {
		return MessageBox(hwnd,text,_T("Confirm"),type|MB_ICONQUESTION);
	}
	int Ask(int id, int type);
	int MsgBox(TCHAR* text, TCHAR* caption) {
		return MessageBox(hwnd,text,caption?caption:_T("Information"),MB_OK|MB_ICONINFORMATION);
	}
	int MsgBox(int id, TCHAR* caption);
	long GetWindowFlag(int nIndex) {
		return GetWindowLong(hwnd,nIndex);
	}
	void SetWindowFlag(int nIndex, long dwFlag) {
		long curflag=GetWindowFlag(nIndex);
		SetWindowLong(hwnd,nIndex,curflag|dwFlag);
	}
	void UnsetWindowFlag(int nIndex, long dwFlag) {
		long curflag=GetWindowFlag(nIndex);
		SetWindowLong(hwnd,nIndex,curflag&(~dwFlag));
	}
	void GetWindowPos(int* left, int* top, int* width, int* height);
	HWND GetParentWindow() {
		return (HWND)GetWindowLong(hwnd,GWL_HWNDPARENT);
	}
	virtual bool onShow(bool fShow){return false;}
	virtual bool onCreate(){return false;}
	virtual bool onSize(int width, int height){return false;}
	virtual bool onMove(){return false;}
	virtual bool onPosChange(){return false;}
	virtual bool onScroll(int dlgid, int code){return false;}
	virtual bool onCommand(int dlgid, int code){return false;}
	virtual bool onClick(int dlgid) { return false; }
	virtual bool onKeyPress(int vkey) { return false; }
	virtual bool onSelChange(int dlgid) { return false; }
	virtual bool onNotify(LPNMHDR pnmh){return false;}
	virtual bool onPaint(){return false;}
	virtual bool onClose(){ return false; }
	virtual bool onDestroy() {return true;}
	virtual bool onMouseMove(DWORD pos, int vkey){return false;}
	virtual bool onMouseLeave(){return false;}
	virtual bool onMouseClick(DWORD flags){return false;}
	virtual bool onTimer(int id){return false;}
	virtual bool SubWndDestroyed(CWindow* wndSub) {return false;}
	virtual HWND Create(int style, int xstyle, HWND hwndParent, WNDCLASS& wc);
	virtual HWND Create(int style, int xstyle, CWindow* parent, WNDCLASS& wc);
	virtual HWND Create(int style, int xstyle, HWND hwndParent);
	virtual HWND Create(int style, int xstyle, CWindow* parent);
	virtual HWND CreateDlg(HINSTANCE hInst, int dlgid, HWND hwndParent);
	virtual HWND CreateDlg(HINSTANCE hInst, int dlgid, CWindow* parent);
	virtual int ShowDlg(HINSTANCE hInst, int dlgid, HWND hwndParent, CWindow* parent);
	LRESULT SendMsg(UINT Msg,WPARAM wParam,LPARAM lParam) {
		return SendMessage(hwnd,Msg,wParam,lParam);
	}
	LRESULT SendDlgMsg(int dlgid,UINT Msg,WPARAM wParam,LPARAM lParam) {
		return SendDlgItemMessage(hwnd,dlgid,Msg,wParam,lParam);
	}
	LRESULT PostDlgMsg(int dlgid,UINT Msg,WPARAM wParam,LPARAM lParam) {
		return PostMessage(GetHandle(dlgid),Msg,wParam,lParam);
	}
	HWND GetHandle(int dlgid) {
		return GetDlgItem(hwnd,dlgid);
	}
	bool IsItemChecked(int dlgid) {
		return (SendDlgItemMessage(hwnd,dlgid,BM_GETCHECK,0,0)==BST_CHECKED);
	}
	bool IsMenuChecked(UINT uId) {
		return ((GetMenuState(GetMenu(hwnd),uId,MF_BYCOMMAND) & MF_CHECKED)!=0);
	}
	void CheckMenu(UINT uId) {
		CheckMenuItem(GetMenu(hwnd),uId,MF_BYCOMMAND|MF_CHECKED);
	}
	void UncheckMenu(UINT uId) {
		CheckMenuItem(GetMenu(hwnd),uId,MF_BYCOMMAND|MF_UNCHECKED);
	}
	void MoveCenter(int w, int h) {
		RECT rc;
		GetWindowRect(hwnd,&rc);
		if (!w) w=rc.right-rc.left;
		if (!h) h=rc.bottom-rc.top; 
		MoveWindow(hwnd,(GetSystemMetrics(SM_CXSCREEN)-w)/2,
			(GetSystemMetrics(SM_CYSCREEN)-h)/2,
			w,h,true);
	}
	TCHAR* GetItemText(int dlgid, TCHAR *buffer) {
		int ret;
		HWND hwndItem=GetDlgItem(hwnd,dlgid);
		if ((ret=SendMessage(hwndItem,WM_GETTEXTLENGTH,0,0))<=0)
			return NULL;
		if (buffer) free(buffer);
		buffer=(TCHAR*)malloc(ret+1);
		SendMessage(hwndItem,WM_GETTEXT,ret+1,(LPARAM)buffer);
		return buffer;
	}
	void SetEnabled(int fEnable) { EnableWindow(hwnd,fEnable); }
	void GetBorderSize();
	int ShowFileDialog(int type, OPENFILENAME* pofn);
	void SetFont(LPCTSTR lfFaceName,int lfCharSet,int lfHeight,int lfWeight);
	bool DefMsgHandler(UINT msg,WPARAM wParam,LPARAM lParam);
	int GetShowState();
	bool SetAlpha(int value);
	CWindow():hwnd(NULL){ hInstance = GetModuleHandle(0); }
	~CWindow(){ Destroy(); }
	HWND hwnd;
	WORD borderWidth;
	WORD borderHeight;
	CWindow* wndParent;
protected:
	void RegWndClass();
	HMODULE hInstance;
};

typedef struct {
	TCHAR* text;
	int width;
} COL_INFO;

class CListView
{
public:
	int AddColumns(COL_INFO cols[]);
	int InsertItem(int item);
	int AddItem(TCHAR* items[]);
	int SetItem(int item, TCHAR* items[]);
	int GetText(int item, int subitem, char* buffer, int bufsize);
	int SetText(int item, int subitem, char* buffer);
	int RemoveItem(int item);
	int GetCount();
	int GetSelection(int item);
	int SetSelection(int item, int fSel);
	int GetCheck(int item)
	{
		return ListView_GetCheckState(hwnd, item);
	}
	void SetCheck(int item, int fCheck)
	{
		 ListView_SetCheckState(hwnd, item, fCheck);
	}
	int GetFocus();
	int SetListFocus(int item);
	void Reset();
	void SetExtendedStype(DWORD style)
	{
		ListView_SetExtendedListViewStyle(hwnd, style);
	}
	int Scroll(int x, int y)
	{
		return SendMessage(hwnd, LVM_SCROLL, x, y);
	}
	HWND hwnd;
};
#endif
