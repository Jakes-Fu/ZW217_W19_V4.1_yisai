/*******************************************************************
* Parameter List
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef PARAMLIST_H
#define PARAMLIST_H

#include "windowhandle.h"
#include "paramlistdef.h"

typedef int (*PARAM_NOTIFY)(int, void*);

void InitParams(PARAM_DATA *params);
void DestroyParams(PARAM_DATA *params);
int IsParamModified(PARAM_DATA *params);
int SaveParams(PARAM_DATA *params, TCHAR* filename);
int LoadParams(PARAM_DATA *params, TCHAR* filename);

class CParamList:public CWindow
{
public:
	int open(HINSTANCE hInst, PARAM_DATA *params, PARAM_NOTIFY notify, PARAM_DESC *info, int varidx, HWND hwndParent);
	~CParamList();
	int showVariable(int varidx);
private:
	int listVariables(int selected);
	void refresh();
	void showValue();
	void saveValue();
	void revert();
	int getIndex();
	int browseForPath(char* buffer);
	int browseForFile(char* buffer);
	int idx;
	int iTab;
	int iPageCount;
	int iBaseLine;
	int *pages;
	bool onCreate();
	bool onClose();
	bool onScroll(int dlgid, int code);
	bool onCommand(int dlgid, int code);
	bool onNotify(LPNMHDR pnmh);
	PARAM_DATA *params;
	PARAM_NOTIFY notify;
	HWND hTree;
	PARAM_DESC *info;
	CWindow* parent;
};

#endif
