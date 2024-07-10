/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef MENC_H
#define MENC_H

#define VER_MAJOR 0
#define VER_MINOR 3
#define VER_REV 7

#define CFGFILE "mediacoder-settings.xml"
#define LISTFILE "mediacoder.m3u"
//@richardGuan
//#define APPNAME "MediaCoder"
#define APPNAME "Mp4 Express"


#define AUDIODIALOG AudioDialogs[iAudCodecDlg]

void MsgBoxStr(UINT uID);

#define PATH_COL 3

typedef enum {
	VIEW_LOG = 0,
	VIEW_CONSOLE,
	VIEW_AUDIO,
	VIEW_VIDEO,
	VIEW_CONTAINER,
} VIEWS;

enum {
	LC_FILENAME = 0, LC_SIZE, LC_TYPE, LC_STATE, LC_FORMAT, LC_FULLPATH, LC_COUNT,
};

class CMenc:public CWindow
{
public:
	int Init();
	int AddItem(T_ITEM* item, CString& alias);
	int AddMultipleFiles(TCHAR* filenames);
	void ShowConsole(int fShow);
	int Encode();
	int DoEncoding();
	void UpdateTitle(TCHAR* text);
	void UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber, WORD displayFlags);
	void UpdateTabbedView();
	CMenc();
	void UpdateTaskStats();
	int GenCommandLines();
	void ShowAudioDialog(int idx);
	void ShowSubDialog(int idx);
	void ShowState(int state);
	void Refresh();
	int SaveQueue(TCHAR* filename);
	int LoadQueue(TCHAR* filename);
	bool onMove();
	TCHAR *caption;
	CListView list;
	bool fConsole;
	CWindow wndStats;
private:
	bool onNotify(LPNMHDR pnmh);
	bool onClick(int dlgid);
	bool onSelChange(int dlgid);
	bool onCommand(int dlgid, int code);
	bool onCreate();
	bool onClose();
	bool onSize();
	bool onShow(bool fShow);
	int SetFormatMode(int mode);
	int CheckFileExistence(TCHAR* filename);
	int GetFileFromList(int idx, TCHAR* buf);
	void ShowFileDetail(int index);
	int EnumDir(TCHAR* dir);
	void AddFolder();
	HANDLE hThread;
	TCHAR *version;
	HWND hWndStatusbar;
	HBITMAP hBmpPlay[2];
	HBITMAP hBmpStart[2];
};

#endif
