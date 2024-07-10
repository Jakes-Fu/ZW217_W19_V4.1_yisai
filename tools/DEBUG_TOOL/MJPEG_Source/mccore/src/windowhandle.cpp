/*******************************************************************
* Lite Window Handling Framework
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef CONFIG_WIN9X
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#include "windowhandle.h"

#define MTABLE_ALLOC_AHEAD 0xf

//////////////////////////////////////////////////////////////////
// hash table implementation
//////////////////////////////////////////////////////////////////
#define HASH_MAX_BYTES (1024 * 512)
class CHashTable
{
public:
	CHashTable():count(0),maxTableSize(HASH_MAX_BYTES / sizeof(void*)),tableSize(0),low(0),high(0) {};
	int Add(int key, void* value)
	{
		if (count == 0) {
			// first element
			low = key;
			high = key;
			table = (void**)malloc(sizeof(void*));
			table[0] = value;
			tableSize = 1;
		} else if (key < low) {
			// expanding the hash table from the lower direction
			unsigned int newSize = high - key + 1;
			if (newSize > maxTableSize) return -1;
			void** newtable = (void**)malloc(newSize * sizeof(void*));
			memset(newtable, 0, (low - key) * sizeof(void*));
			memcpy(newtable + low - key, table, tableSize * sizeof(void*));
			free(table);
			table = newtable;
			table[0] = value;
			tableSize = newSize;
			low = key;
		} else if (key > high) {
			unsigned int newSize = key - low + 1;
			if (newSize > maxTableSize) return -1;
			high = key;
			table = (void**)realloc(table, (high - low + 1) * sizeof(void*));
			memset(table + tableSize, 0, (newSize - tableSize) * sizeof(void*));
			table[key - low] = value;
			tableSize = newSize;
		} else {
			table[key - low] = value;
		}
		char buf[128];
		wsprintf(buf, "Count = %d\tKey = %x\tLow = %x\tHigh = %x\t", count + 1, key, low, high);
		MessageBox(0, buf, 0, MB_OK);
		return ++count;
	}
	bool Remove(int key)
	{
		if (key >= low && key <= high) {
			void* val = table[key - low];
			table[key - low] = 0;
			if (--count == 0) {
				free(table);
				low = 0;
				high = 0;
			}
			return true;
		} else {
			return false;
		}
	}
	void* Lookup(int key)
	{
		if (key >= low && key <= high) {
			return table[key - low];
		} else {
			return 0;
		}
	}
	int low;
	int high;
	int count;
	unsigned int tableSize;
	unsigned int maxTableSize;
protected:
	void** table;
};

class CHashTableMain
{
public:
	CHashTableMain():hashcount(1)
	{
		hashes = (CHashTable**)malloc(sizeof(CHashTable*));
		hashes[0] = new CHashTable;
	}
	~CHashTableMain()
	{
		for (int i = 0; i < hashcount; i++) delete hashes[i];
		free(hashes);
	}
	bool Add(int key, void* value)
	{
		for (int i = 0; i < hashcount; i++) {
			if (hashes[i]->Add(key, value) > 0) return true;
		}
		hashes = (CHashTable**)realloc(hashes, (hashcount + 1) * sizeof(CHashTable*));
		hashes[hashcount] = new CHashTable;
		hashes[hashcount]->Add(key, value);
		hashcount++;
		return false;
	}
	bool Remove(int key)
	{
		for (int i = 0; i < hashcount; i++) {
			if (hashes[i]->Remove(key)) return true;
		}
		return false;
	}
	void* Lookup(int key)
	{
		for (int i = 0; i < hashcount; i++) {
			void* val = hashes[i]->Lookup(key);
			if (val) return val;
		}
		return 0;
	}
	int hashcount;
	CHashTable** hashes;
};

//////////////////////////////////////////////////////////////////
// matching table implementation
//////////////////////////////////////////////////////////////////

#define MTABLE_ALLOC_AHEAD 0xf

mtable* CMatchTable::add(int id) 
{
	for (idx=0;idx<count && (table+idx)->id<id;idx++) {
		if ((table+idx)->id==id) return table+idx;
	}
	count++;
	table=(mtable*)realloc(table,((count+MTABLE_ALLOC_AHEAD)&(0xffffffff-MTABLE_ALLOC_AHEAD))*sizeof(mtable));
	if (!table) {
		count=0;
		return NULL;
	}
	for (int i=count-1;i>idx;i--) {
		*(table+i)=*(table+i-1);
	}
	(table+idx)->id=id;
	(table+idx)->data=NULL;
	return table+idx;
}

int CMatchTable::remove(int id) 
{
	if (!match(id)) return 0;
	release();
	for (int i = idx; i < count - 1; i++)
		*(table + i) = *(table + i + 1);
	table = (mtable*)realloc(table, (--count) * sizeof(mtable));
	if (idx == count) idx--;
	return 1;
}

mtable* CMatchTable::match(int id) 
{
	if (count==0) return NULL;
	if (id<(table+idx)->id) {
		for (int i=idx-1;i>=0;i--) {
			if (id==(table+i)->id) {
				idx=i;
				return (table+idx);
			}
		}
	} else if (id>(table+idx)->id) {
		for (int i=idx+1;i<count;i++) {
		if (id==(table+i)->id) {
				idx=i;
				return (table+idx);
			}
		}
	} else {
		return (table+idx);
	}
	return table+idx;
}

mtable* CMatchTable::finematch(int id) 
{
	if (count==0) return NULL;
	if (id<(table+idx)->id) {
		for (int i=idx-1;i>=0;i--) {
			if (id==(table+i)->id) {
				idx=i;
				return (table+idx);
			}
		}
	} else if (id>(table+idx)->id) {
		for (int i=idx+1;i<count;i++) {
		if (id==(table+i)->id) {
				idx=i;
				return (table+idx);
			}
		}
	} else {
		return (table+idx);
	}
	return NULL;
}

void CMatchTable::reset() 
{
	if (table) {
		for (idx=0;idx<count;idx++) release();
		free(table);
	}
	table=NULL;
	idx=0;
	count=0;
}

void CMatchTable::release()
{
}

//////////////////////////////////////////////////////////////////
// window handle class implementation
//////////////////////////////////////////////////////////////////

static int CALLBACK DlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	mtable *ptrEntry;
	bool fRet;
	static CMatchTable dlgtable;
	CWindow* wnd;
	ptrEntry=dlgtable.finematch((int)hwnd);
	if (!ptrEntry) {
        if (msg!=WM_INITDIALOG) return false;
		ptrEntry=dlgtable.add((int)hwnd);
		if (!ptrEntry) return false;
		ptrEntry->data=(void*)lParam;
		wnd = (CWindow*)lParam;
		wnd->hwnd=hwnd;
		wnd->onCreate();
	} else {
		wnd = (CWindow*)ptrEntry->data;
	}
	fRet = wnd->DefMsgHandler(msg,wParam,lParam);
	if (msg == WM_CLOSE) {
		if (!fRet) {
			EndDialog(hwnd, 0);
			wnd->hwnd = 0;
			dlgtable.remove((int)hwnd);
			return TRUE;
		}
	}
	return fRet;
}

static LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	mtable *ptrEntry;
	bool fProcessed=false;
	static CMatchTable wndtable;

	ptrEntry=wndtable.finematch((int)hwnd);
	if (!ptrEntry) {
		if (msg!=WM_CREATE) return -1;
		ptrEntry=wndtable.add((int)hwnd);
		if (!ptrEntry) return -1;
		ptrEntry->data=((LPCREATESTRUCT)lParam)->lpCreateParams;
		((CWindow*)ptrEntry->data)->hwnd=hwnd;
		if (((CWindow*)ptrEntry->data)->onCreate()) return 0;
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	fProcessed=((CWindow*)ptrEntry->data)->DefMsgHandler(msg,wParam,lParam);
	CWindow* wnd = ((CWindow*)ptrEntry->data);
	if (msg==WM_DESTROY) {
		wndtable.remove((int)hwnd);
		if (wnd->wndParent) {
			wnd->wndParent->SubWndDestroyed(wnd);
		}
	}
	if (fProcessed) return 0;
	if (msg == WM_CLOSE) {
		wnd->Destroy();
		return 0;
	} else {
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
}

bool CWindow::DefMsgHandler(UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg){
	case WM_MOUSEMOVE:
		return onMouseMove(lParam,wParam);
	case WM_MOUSELEAVE:
		return onMouseLeave();
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		return onMouseClick(wParam);
	case WM_TIMER:
		return onTimer(wParam);
	case WM_COMMAND:
		switch (HIWORD(wParam)) {
		case BN_CLICKED:
			if (onClick(LOWORD(wParam))) return true;
			break;
		case CBN_SELCHANGE:
			if (onSelChange(LOWORD(wParam))) return true;
			break;
		}
		return onCommand(LOWORD(wParam),HIWORD(wParam));
	case WM_NOTIFY:
		return onNotify((LPNMHDR)lParam);
	case WM_HSCROLL:
	case WM_VSCROLL:
		return onScroll(lParam?GetDlgCtrlID((HWND)lParam):0,LOWORD(wParam));
	case WM_SIZE:
		return onSize(LOWORD(lParam), HIWORD(lParam));
	case WM_MOVE:
		return onMove();
	case WM_PAINT:
		return onPaint();
	case WM_SHOWWINDOW:
		return onShow((wParam!=0));
	case WM_CLOSE:
		return onClose();
	case WM_DESTROY:
		return onDestroy();
	case WM_KEYUP:
		return onKeyPress(wParam);
	case WM_WINDOWPOSCHANGED:
		return onPosChange();
	default:
		return false;
	}
	return true;
}

void CWindow::StretchItemFull(HWND hwndItem)
{
	RECT rect;
    GetClientRect(hwnd, &rect);
	MoveWindow(hwndItem,0,0,rect.right-rect.left,rect.bottom-rect.top,true);
	UpdateWindow(hwnd);
}

void CWindow::RegWndClass()
{
	WNDCLASS wc;
	if (!GetClassInfo(hInstance,"CWindowClass",&wc)) {
		ZeroMemory(&wc,sizeof(WNDCLASS));
		wc.style = CS_HREDRAW|CS_VREDRAW |CS_DBLCLKS ;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.hInstance = hInstance;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszClassName = _T("CWindowClass");
		wc.hCursor = LoadCursor(NULL,IDC_ARROW);
		RegisterClass(&wc);
	}
}

HWND CWindow::Create(int style, int xstyle, HWND hwndParent, WNDCLASS& wc)
{
	wndParent = 0;
	if (!hwnd) {
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.hInstance = hInstance;
		RegisterClass(&wc);
		hwnd = CreateWindowEx(xstyle,wc.lpszClassName,NULL,style,CW_USEDEFAULT,0,CW_USEDEFAULT,0,hwndParent,NULL,hInstance,this);
	}
	return hwnd;
}

HWND CWindow::Create(int style, int xstyle, CWindow* parent, WNDCLASS& wc)
{
	wndParent = parent;
	if (!hwnd) {
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.hInstance = hInstance;
		RegisterClass(&wc);
		hwnd = CreateWindowEx(xstyle,wc.lpszClassName,NULL,style,CW_USEDEFAULT,0,CW_USEDEFAULT,0,parent ? parent->hwnd : 0,NULL,hInstance,this);
	}
	return hwnd;
}

HWND CWindow::Create(int style, int xstyle, CWindow* parent)
{
	wndParent = parent;
	if (!hwnd) {
		RegWndClass();
		hwnd=CreateWindowEx(xstyle,_T("CWindowClass"),NULL,style,CW_USEDEFAULT,0,CW_USEDEFAULT,0,parent ? parent->hwnd : 0,NULL,hInstance,this);
	}
	return hwnd;
}

HWND CWindow::Create(int style, int xstyle, HWND hwndParent)
{
	wndParent = 0;
	if (!hwnd) {
		RegWndClass();
		hwnd=CreateWindowEx(xstyle,_T("CWindowClass"),NULL,style,CW_USEDEFAULT,0,CW_USEDEFAULT,0,hwndParent,NULL,hInstance,this);
	}
	return hwnd;
}

HWND CWindow::CreateDlg(HINSTANCE hInst, int dlgid, CWindow* parent) {
	wndParent = parent;
	if (hInst) hInstance = hInst;
	if (!hwnd) {
		hwnd = CreateDialogParam(GetModuleHandle(NULL), MAKEINTRESOURCE(dlgid), parent ? parent->hwnd : 0, DlgProc, (LPARAM)this);
	}
	return hwnd;
}

HWND CWindow::CreateDlg(HINSTANCE hInst, int dlgid, HWND hwndParent) {
	wndParent = 0;
	if (hInst) hInstance = hInst;
	if (!hwnd) {
		hwnd = CreateDialogParam(GetModuleHandle(NULL), MAKEINTRESOURCE(dlgid), hwndParent, DlgProc, (LPARAM)this);
	}
	return hwnd;
}

int CWindow::ShowDlg(HINSTANCE hInst, int dlgid, HWND hwndParent, CWindow* parent) {
	if ((wndParent = parent)) {
		hwndParent = parent->hwnd;
	}
	if (!hwnd) {
		if (!hInst) hInst = GetModuleHandle(NULL);
		hInstance = hInst;
		return DialogBoxParam(hInst, MAKEINTRESOURCE(dlgid), hwndParent, DlgProc, (LPARAM)this);
	} else {
		return -1;
	}
}

void CWindow::Move(int x,int y,int w,int h){
	RECT rc;
	GetWindowRect(hwnd,&rc);
	if (x==-1) x=rc.left;
	if (y==-1) y=rc.top;
	if (w==-1) w=rc.right-rc.left;
	if (h==-1) h=rc.bottom-rc.top;
	MoveWindow(hwnd,x,y,w,h,true);
}

void CWindow::StretchFull()
{
	RECT rc;
	HWND hwndParent=GetParentWindow();
	if (hwndParent) {
		GetClientRect(hwndParent,&rc);
		Move(0,0,rc.right-rc.left,rc.bottom-rc.top);
	}
}

void CWindow::GetWindowPos(int* left, int* top, int* width, int* height)
{
	RECT rc;
	GetWindowRect(hwnd,&rc);
	if (left) *left=rc.left;
	if (top) *top=rc.top;
	if (width) *width=rc.right-rc.left;
	if (height) *height=rc.bottom-rc.top;
}

void CWindow::GetBorderSize() {
	RECT rcWindow;
	RECT rcClient;
	GetWindowRect(hwnd,&rcWindow);
	GetClientRect(hwnd,&rcClient);
	borderWidth=(WORD)((rcWindow.right-rcWindow.left)-(rcClient.right-rcClient.left));
	borderHeight=(WORD)((rcWindow.bottom-rcWindow.top)-(rcClient.bottom-rcClient.top));
}


void CWindow::SetFont(LPCTSTR lfFaceName,int lfCharSet,int lfHeight,int lfWeight)
{
	LOGFONT lf;
	HFONT hfont;
	memset(&lf,0,sizeof(lf));
	lf.lfCharSet=lfCharSet;
	lf.lfHeight=-lfHeight;
	lf.lfWeight=lfWeight;
	hfont = CreateFontIndirect(&lf); 
	SendMessage(hwnd,WM_SETFONT,(WPARAM)hfont,0);
}

bool CWindow::SetAlpha(int value)
{
#if _WIN32_WINNT >= 0x0500
	SetLayeredWindowAttributes(hwnd, 0, value, LWA_ALPHA);
	return true;
#else
	return false;
#endif
}

int CWindow::ShowFileDialog(int type, OPENFILENAME* pofn)
{
	int fSuccess;
	TCHAR curdir[MAX_PATH];

	GetCurrentDirectory(sizeof(curdir),curdir);

	pofn->lStructSize = sizeof(OPENFILENAME);
	pofn->hInstance = GetModuleHandle(NULL);
	pofn->hwndOwner = hwnd;
	
	if (type == 0){
		pofn->Flags |= OFN_CREATEPROMPT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
		fSuccess=GetOpenFileName(pofn);
	} else {
		pofn->Flags |= OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
		fSuccess=GetSaveFileName(pofn);
	}
	SetCurrentDirectory(curdir);
	return fSuccess;
}

int CWindow::GetShowState()
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	if (!GetWindowPlacement(hwnd, &wp)) return 0;
	return wp.showCmd;
}

int CWindow::Ask(int id, int type) {
	TCHAR buf[256];
	if (LoadString(hInstance, id, buf, sizeof(buf)/sizeof(TCHAR)))
		return MessageBox(hwnd,buf,_T("Confirm"),type|MB_ICONQUESTION);
	else
		return 0;
}

int CWindow::MsgBox(int id, TCHAR* caption) {
	TCHAR buf[256];
	if (LoadString(hInstance, id, buf, sizeof(buf)/sizeof(TCHAR)))
		return MessageBox(hwnd,buf,caption?caption:_T("Information"),MB_OK|MB_ICONINFORMATION);
	else
		return 0;
}

int CListView::AddColumns(COL_INFO cols[])
{
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	for (int i = 0; cols[i].text; i++) {
		lvc.pszText = cols[i].text;
		lvc.cx = cols[i].width;
		if (ListView_InsertColumn(hwnd, i, &lvc)!=i) return -1;
	}
	return 0;
}

int CListView::InsertItem(int item)
{
	LVITEM lvi;
	lvi.mask = 0;
	lvi.iItem = item;
	lvi.iSubItem = 0;
	return SendMessage(hwnd, LVM_INSERTITEM, 0, (LPARAM)&lvi)==item ? 0 : -1;
}

int CListView::SetItem(int item, TCHAR* items[])
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = item;
	for (int i = 0; items[i]; i++) {
		lvi.iSubItem = i;
		lvi.pszText = items[i];
		if (!SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)&lvi)) return -1;
	}
	return 0;
}

int CListView::AddItem(TCHAR* items[])
{
	int count = GetCount();
	if (InsertItem(count) || SetItem(count, items)) return -1;
	return count;
}

int CListView::GetCount()
{
	return SendMessage(hwnd, LVM_GETITEMCOUNT, 0, 0);
}

int CListView::RemoveItem(int item)
{
	return SendMessage(hwnd, LVM_DELETEITEM, item, 0) ? 0 : -1;
}

int CListView::GetSelection(int item)
{
	return SendMessage(hwnd, LVM_GETITEMSTATE, item, LVIS_SELECTED);
}

int CListView::SetSelection(int item, int fSel)
{
	LV_ITEM lvi;
	lvi.mask = LVIF_STATE;
	lvi.stateMask = LVIS_SELECTED;
	lvi.state = fSel ? LVIS_SELECTED : 0;
	return SendMessage(hwnd, LVM_SETITEMSTATE, item, (LPARAM)&lvi) ? 0 : -1;
}

int CListView::GetFocus()
{
	for (int i = GetCount() - 1; i >= 0; i--) {
		if (SendMessage(hwnd, LVM_GETITEMSTATE, i, LVIS_FOCUSED)) return i;
	}
	return -1;
}

int CListView::SetListFocus(int item)
{
	LV_ITEM lvi;
	lvi.mask = LVIF_STATE;
	lvi.stateMask = LVIS_FOCUSED;
	lvi.state = LVIS_FOCUSED;
	return SendMessage(hwnd, LVM_SETITEMSTATE, item, (LPARAM)&lvi) ? 0 : -1;
}

void CListView::Reset()
{
	SendMessage(hwnd, LVM_DELETEALLITEMS ,0 ,0);
}

int CListView::GetText(int item, int subitem, char* buffer, int bufsize)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = buffer;
	lvi.cchTextMax = bufsize - 1;
	lvi.iItem = item;
	lvi.iSubItem = subitem;
	return SendMessage(hwnd, LVM_GETITEM, 0, (LPARAM)&lvi) ? 0 : -1;
}

int CListView::SetText(int item, int subitem, char* buffer)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = buffer;
	lvi.cchTextMax = 0;
	lvi.iItem = item;
	lvi.iSubItem = subitem;
	return SendMessage(hwnd, LVM_SETITEMTEXT, item, (LPARAM)&lvi) ? 0 : -1;
}
