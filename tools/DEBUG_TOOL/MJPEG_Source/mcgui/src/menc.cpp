/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote products
*    derived from this software without specific prior permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#include "config.h"
#include <commctrl.h>
#include <shlobj.h>
#include <stdio.h>
#include <time.h>
#include "atlstr.h"
#include "resource.h"
#include "common.h"
#include "menc.h"
#include "oleidl.h"
//#include "httpget.h"
#include "revision.h"
#include "optiondialogs.h"
#include "cfilename.h"
#include "lang.h"

#define CONSOLE_CAPTION "MediaCoder Console Window"
#define SPLASH_SHOW_TIME 2000
#define SHOWSTATS(show) ParamNotify(PI_STATS, (void*)(show?1:2))

enum { IS_READY = 0, IS_DONE, IS_SKIPPED, IS_ERROR };
static TCHAR *itemstate[] = {"Ready", "Done", "Skipped", "Error"};

void RegisterDropWindow(HWND hwnd);
void UnregisterDropWindow(HWND hwnd);
VOID CALLBACK StatsTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

int UpdateTime();

int MakeDir(TCHAR* dirname);
void GlobalUninit();

HINSTANCE hInst;
TCHAR apppath[MAX_PATH];
HICON hAppIcon;

CMenc menc;
DWORD idThread;
COptionDialog* optionDialog[OD_COUNT];

int iAudCodecDlg;
int iOptionDialog;
int iQueueItemIndex;

int EventHandler(MC_EVENT event, void* arg);
int ShowStats();
#define EVENT EventHandler
static MC_STATE* mcstate;

class CFileInfo:public CWindow {
public:
	void Refresh(int index);
private:
	bool onCreate();
	bool onCommand(int dlgid, int code);
	void SetText(int dlgid, CString& str);
};

CFileInfo fileinfo;

class CSplash:public CWindow {
private:
	bool onPaint() {
		
		HDC hdc,hdcmem;
		HBITMAP hbm;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		hdcmem=CreateCompatibleDC(hdc);
		hbm=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SPLASH));
		SelectObject(hdcmem,hbm);
		BitBlt(hdc,0,0,372,340,hdcmem,0,0,SRCCOPY);
		DeleteObject(hbm);
		DeleteDC(hdcmem);
		EndPaint(hwnd, &ps);
		
		return true;		
	}
	bool onMouseClick(DWORD flags) { return onTimer(0); }
	bool onTimer(int id) { 
		KillTimer(hwnd,id);
#ifndef CONFIG_WIN9X
		for (int i = 255; i>=5; i-=10) {
			SetLayeredWindowAttributes(hwnd, 0, i, LWA_ALPHA);
			Sleep(10);
		}
#endif
		Destroy();
		return true; 
	}
public:
	void Open(int timeout, CWindow* parent) {
#ifndef CONFIG_WIN9X
		Create(WS_POPUP|WS_BORDER,WS_EX_TOPMOST|WS_EX_LAYERED, parent);
		SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
#else
		Create(WS_POPUP|WS_BORDER,WS_EX_TOPMOST, parent);
#endif
		MoveCenter(374,345);
		Show();
		onPaint();
#ifndef CONFIG_WIN9X
		for (int i = 15; i<=255; i+=20) {
			SetLayeredWindowAttributes(hwnd, 0, i, LWA_ALPHA);
			Sleep(10);
		}
#endif
		if (timeout) SetTimer(hwnd,0,timeout,0);
	}
};

CSplash splash;

#define NOW GetTickCount()

static char* mirrors[] = {"mediacoder.sourceforge.net", "www.rarewares.org/mediacoder", 0};
static int alivemirror = -1;
static HANDLE hThreadCheckUpdate = 0;

void VisitWebsite(int mirrorid)
{
	CString url ;
	url.Format("http://%s/", mirrors[mirrorid]);
	ShellExecute(menc.hwnd, "open", url.GetBuffer(), 0, 0, SW_SHOW);
}

void EnqueueItem(T_ITEM* item, CString& alias)
{
	menc.AddItem(item, alias);
}

void ListEnums(int idx, HWND hCtrl, UINT msg)
{
	TCHAR *p;
	for (int i = 0; *(p = mcGetPrefValueEnumDefs(idx, i)); i++) {
		if (*p == '\t') p++;
		SendMessage(hCtrl, msg, 0, (LPARAM)p);
	}
}

void MsgBoxStr(UINT uID)
{
	CString str;
	str.Format(uID);
	EVENT(MC_MSG_POPUP, str.GetBuffer());
}

void RefreshGui()
{
	menc.Refresh();
}

HWND CreateSBar(HWND hwndParent,TCHAR *initialText,int nrOfParts,int ID)
{
	#define SBarParts 3
    HWND hWndStatusbar;
	int   ptArray[SBarParts]={100,250,-1};   // Array defining the number of parts/sections

	hWndStatusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_BORDER|SBARS_SIZEGRIP,initialText,hwndParent,ID);

	if(hWndStatusbar) {
		SendMessage(hWndStatusbar,
					SB_SETPARTS,
					SBarParts,
					(LPARAM)ptArray);
	}
    return hWndStatusbar;
}

int ParamNotify(int idx, void* value)
{
	int iac=-1;

	switch (idx) {
	case PI_SHOW_CONSOLE:
		menc.ShowConsole(mcGetPrefValueInt(PI_SHOW_CONSOLE));
		if (mcGetPrefValueInt(PI_SHOW_CONSOLE)) {
			if (mcGetPrefValueInt(PI_AUTO_BITRATE)) {
				MessageBox(0, "Auto-bitrate is only available when console window is closed.", "Notify", MB_OK|MB_ICONINFORMATION);
				mcSetPrefValue(PI_AUTO_BITRATE,0);
			}
		}
		break;
	case PI_TEMP_DIR:
	case PI_DEST_DIR:
		{
		TCHAR *p=mcGetPrefValueStr(idx);
		int len=strlen(p);
        if (len>0 && *(p+len-1)!='\\') *(LPWORD)(p+len)='\\';
		} break;
	case PI_STATS:
		switch ((int)value) {
		case 0:
		case 2:
			menc.wndStats.Close();
			break;
		case 1:
			if (!menc.wndStats.hwnd) {
				int x,y,w,h;
				menc.GetWindowPos(&x, &y, &w, &h);
				menc.wndStats.CreateDlg(0, IDD_STATS, &menc);
				menc.onMove();
				menc.wndStats.Show();
				menc.SetForeground();
			}
			break;
		}
		return 0;
	case PI_FACE_ALPHA:
#ifndef CONFIG_WIN9X
		if ((int)value == 255) {
			menc.UnsetWindowFlag(GWL_EXSTYLE, 0x80000);
		} else {
			menc.SetWindowFlag(GWL_EXSTYLE, 0x80000);
			menc.SetAlpha((int)value);
		}
#endif
		return 0;
	default:
		if (idx > PI_PAGE_VCODEC) {
			mcGetToolCommand(TL_VIDEO_CODEC, 0);
			optionDialog[OD_VIDEO]->Refresh();
		} else if (idx > PI_PAGE_ADEC) {
			optionDialog[OD_AUDIO]->Refresh();
			optionDialog[OD_AUDIO]->Refresh();
		}
	}
	mcApplyParam();
	if (iOptionDialog >= 0 && optionDialog[iOptionDialog]) optionDialog[iOptionDialog]->Refresh();
	RefreshGui();
	menc.SendDlgMsg(IDC_OUTPUT_FOLDER,WM_SETTEXT,0,(LPARAM)mcGetPrefValueInt(PI_DEST_DIR));
	return 0;
}

int ShutDownNT(int fPowerOff)
{
	HANDLE hProcessHandle,hTokenHandle;
	LUID tmpLuid;
	TOKEN_PRIVILEGES tkpNew;
	TOKEN_PRIVILEGES tkpPrevious;
	DWORD lBufferNeeded;

	hProcessHandle = GetCurrentProcess();
	if (!OpenProcessToken(hProcessHandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenHandle)) return -1;

	//Get the LUID for the shutdown privilege
	LookupPrivilegeValue("", "SeShutdownPrivilege", &tmpLuid);
	tkpNew.PrivilegeCount = 1; // One privilege to set
	tkpNew.Privileges[0].Luid = tmpLuid;
	tkpNew.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Enable the shutdown privilege in the access token of this process.
	lBufferNeeded = 0;
	AdjustTokenPrivileges(hTokenHandle, false, &tkpNew, sizeof(tkpPrevious), &tkpPrevious, &lBufferNeeded);

	// Force a Reboot/Poweroff (no option to save files to cancel out)
	ExitWindowsEx(fPowerOff?(EWX_FORCE|EWX_SHUTDOWN|EWX_POWEROFF):(EWX_FORCE|EWX_SHUTDOWN), 0xffff);
	return 0;
}

TCHAR* SetFileExt(TCHAR* filename, TCHAR* fileext)
{
	TCHAR *p;
	p=strrchr(filename,'.');
	if (p)
		strcpy(p+1,fileext);
	else
		strcat(filename,fileext);
	return filename;
}

DWORD WINAPI EncodeThread(CMenc *enc)
{
	return enc->DoEncoding();
}

int EventHandler(MC_EVENT event, void* arg)
{
	switch (event) {
	case MC_WRITE_CONSOLE:
		if (mcGetPrefValueInt(PI_VIEW) == VIEW_CONSOLE)
			menc.SendDlgMsg(IDC_INFO,WM_SETTEXT,0,(LPARAM)arg);
		break;
	case MC_UPDATE_STATE:
		menc.ShowState((int)arg);
		break;
	case MC_UPDATE_TITLE: 
		menc.UpdateTitle((TCHAR*)arg);
		break;
	case MC_UPDATE_TIME:
		UpdateTime();
		break;
	case MC_UPDATE_STATS:
		ShowStats();
		break;
	case MC_UPDATE_PROGRESS:
		SendDlgItemMessage(menc.hwnd,IDC_PROGRESS,PBM_SETPOS,(WPARAM)arg,0);
		break;
	case MC_MSG_POPUP:
		MessageBox(menc.hwnd, (TCHAR*)arg, APPNAME, MB_OK|MB_ICONINFORMATION);
		break;
	case MC_MSG_ALERT:
		MessageBox(menc.hwnd, (TCHAR*)arg, "Alert", MB_OK|MB_ICONEXCLAMATION);
		break;
	case MC_MSG_YESNO:
		return MessageBox(0, (LPCTSTR)arg, "Confirm", MB_YESNO|MB_ICONQUESTION);
	case MC_MSG_YESNOCANCEL:
		return MessageBox(0, (LPCTSTR)arg, "Confirm", MB_YESNOCANCEL|MB_ICONQUESTION);
	case MC_MSG_INFO: {
		DWORD bytes;
		HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
		WriteFile(hStdout, (TCHAR*)arg, strlen((TCHAR*)arg), &bytes, 0);
		} break;
	case MC_MSG_CMD: {
		DWORD bytes;
		if (mcGetPrefValueInt(PI_SHOW_CONSOLE) && arg) {
			HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
			int len = strlen((TCHAR*)arg);
			WriteFile(hStdout,"\n# ",3,&bytes,0);
			WriteFile(hStdout, arg, len,&bytes,0);
			WriteFile(hStdout,"\n\n",2,&bytes,0);
		}
		} break;
	case MC_UPDATE_LOG:
		if (mcGetPrefValueInt(PI_VIEW) == 0) {
			menc.SendDlgMsg(IDC_INFO, WM_SETTEXT, 0, (LPARAM)mcGetLogText(0));
		}
		break;
	case MC_JOB_START: {
		T_ITEM* item = (T_ITEM*)arg;
		if (iQueueItemIndex == 0) {
			menc.list.Scroll(0, -(menc.list.GetCount() * 12));
		}
		menc.list.SetSelection(iQueueItemIndex, 1);
		menc.list.SetListFocus(iQueueItemIndex);
		RefreshGui();
		menc.UpdateStatusBar(item->filename,2,0);
		//ShowWindow(menc.GetHandle(IDC_FILENAME), SW_SHOW);
		} break;
	case MC_JOB_FINISH:	{
		char *p = itemstate[IS_SKIPPED];
		switch (mcstate->state) {
		case STATE_STOPPING:
			p = itemstate[IS_READY];
			break;
		case STATE_ENCODING:
			p = itemstate[IS_DONE];
			break;
		case STATE_ERROR:
			p = itemstate[IS_ERROR];
			break;
		}
        menc.list.SetText(iQueueItemIndex, LC_STATE, p);
		menc.SendDlgMsg(IDC_PROGRESSALL,PBM_SETPOS, mcstate->stats.fileprocessed * 100 / mcstate->totalfiles ,0);
		menc.UpdateTaskStats();
		if (iQueueItemIndex >= 3)
			menc.list.Scroll(0, 12);
		iQueueItemIndex++;
		} break;
	default:
		return ER_BYPASSED;
	}
	return ER_OK;
}

void CMenc::UpdateTitle(TCHAR* text)
{
	if (text) {
		TCHAR buf[256];
		sprintf(buf, "%s - %s", caption, text);
		SetCaption(buf);
	} else {
		SetCaption(caption);
	}
}

int CMenc::DoEncoding()
{
	int iSingleFile=0;
	int state;

	if (mcGetPrefValueInt(PI_DEL_ORIGINAL)) {
		if (Ask("You have chosen to delete original files after transcoding. Are you sure you want this?", MB_YESNO) == IDNO)
			return 0;
	}

	iQueueItemIndex = 0;
	EVENT(MC_WRITE_CONSOLE, 0);
	SendDlgMsg(IDC_TRANSCODE,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBmpStart[1]);
	SendDlgMsg(IDC_PROGRESSALL,PBM_SETPOS,0,0);
	HWND hwndStats = wndStats.hwnd;
	if (hwndStats) {
		SendDlgItemMessage(hwndStats,IDC_STAT_POS,WM_SETTEXT,0,0);
		SendDlgItemMessage(hwndStats,IDC_STAT_FRAME,WM_SETTEXT,0,0);
		SendDlgItemMessage(hwndStats,IDC_STAT_FPS,WM_SETTEXT,0,0);
		SendDlgItemMessage(hwndStats,IDC_STAT_ESTTIME,WM_SETTEXT,0,0);
		SendDlgItemMessage(hwndStats,IDC_STAT_ESTSIZE,WM_SETTEXT,0,0);
		SendDlgItemMessage(hwndStats,IDC_STAT_AB,WM_SETTEXT,0,0);
		SendDlgItemMessage(hwndStats,IDC_STAT_VB,WM_SETTEXT,0,0);
	}
	mcstate->totalfiles = list.GetCount();
	if (mcstate->totalfiles <= 0) return -1;
	UpdateTaskStats();

	for (int i = list.GetCount() - 1; i >= 0; i--)
		list.SetSelection(i, 0);

	state = mcStartTranscode();
	UpdateTaskStats();
	EVENT(MC_WRITE_CONSOLE, 0);
	if (mcGetPrefValueInt(PI_STATS) == 0) {
		SHOWSTATS(0);
	}
	SendDlgMsg(IDC_TRANSCODE,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBmpStart[0]);
	if (mcstate->state == STATE_SHUTDOWN) {
		mcSaveParam(CFGFILE);
		ShutDownNT(1);
		Close();
	}
	ShowState(mcstate->state);
	Refresh();
	SetCaption(caption);
	SendDlgMsg(IDC_PROGRESS,PBM_SETPOS,0,0);
	SendDlgMsg(IDC_PROGRESSALL,PBM_SETPOS,0,0);
	return 0;
}

void CMenc::UpdateTaskStats()
{
	TCHAR buf[16];
	HWND hwndStats = wndStats.hwnd;
	if (!hwndStats) return;
	sprintf(buf,"%d",mcstate->totalfiles);
	SendDlgItemMessage(hwndStats,IDC_STAT_TOTAL_FILES,WM_SETTEXT,0,(LPARAM)buf);
	sprintf(buf,"%d",mcstate->stats.encodedFiles);
	SendDlgItemMessage(hwndStats,IDC_STAT_ENCODED_FILES,WM_SETTEXT,0,(LPARAM)buf);
	sprintf(buf,"%d",mcstate->stats.skippedFiles);
	SendDlgItemMessage(hwndStats,IDC_STAT_SKIPPED_FILES,WM_SETTEXT,0,(LPARAM)buf);
	sprintf(buf,"%d",mcstate->stats.errorFiles);
	SendDlgItemMessage(hwndStats,IDC_STAT_ERROR_FILES,WM_SETTEXT,0,(LPARAM)buf);
	sprintf(buf,"%d",mcstate->stats.originalKB);
	SendDlgItemMessage(hwndStats,IDC_STAT_ENCODED_KB,WM_SETTEXT,0,(LPARAM)buf);
}

int UpdateTime()
{
	TCHAR buf[16];
	HWND hwndStats = menc.wndStats.hwnd;
	DWORD tm = GetTickCount();
	if (!hwndStats) return 0;
	int sec = mcstate->stats.dwElapsed >> 10;
	sprintf(buf,"%d:%02d", sec / 60, sec % 60);
	SendDlgItemMessage(hwndStats,IDC_STAT_ELAPSED,WM_SETTEXT,0,(LPARAM)buf);
	sec = (tm - mcstate->stats.dwEncStartTime)>>10;
	sprintf(buf,"%d:%02d", sec / 60, sec % 60);
	SendDlgItemMessage(hwndStats,IDC_STAT_TOTALTIME,WM_SETTEXT,0,(LPARAM)buf);
	float percent = (mcstate->stage == ST_PROCESSING_AUDIO) ? mcstate->astats.percent : mcstate->vstats->percent;
	if (percent) {
		sec = (int)(mcstate->stats.dwElapsed / percent * (100 - percent)) >> 10;
		if (sec > 0) {
			sprintf(buf,"%dmins", sec / 60);
			SendDlgItemMessage(hwndStats,IDC_STAT_ESTTIME,WM_SETTEXT,0,(LPARAM)buf);
		}
	}
	return 0;
}

int UpdateVideoStats()
{
	TCHAR buf[128], *p;
	MP_STATS* vstats = mcstate->vstats;
	HWND hwndStats = menc.wndStats.hwnd;

	p = buf;
	if (menc.GetShowState() != SW_SHOWMINIMIZED)
		p += sprintf(p, "%s - ", menc.caption);
	sprintf(p, mcstate->totalpass > 1 ? "%4.1f%% done (pass %d of %d)" : "%4.1f%% done", 
		vstats->percent,
		mcstate->currentpass,
		mcstate->totalpass);
	menc.SetCaption(buf);

	if (hwndStats) {
		sprintf(buf, "%d:%02d", vstats->pos / 60, vstats->pos % 60);
		SendDlgItemMessage(hwndStats,IDC_STAT_POS,WM_SETTEXT,0,(LPARAM)buf);
		sprintf(buf, "%df", vstats->frame);
		SendDlgItemMessage(hwndStats,IDC_STAT_FRAME,WM_SETTEXT,0,(LPARAM)buf);
		sprintf(buf, "%4.1ffps", vstats->fps);
		SendDlgItemMessage(hwndStats,IDC_STAT_FPS,WM_SETTEXT,0,(LPARAM)buf);
		sprintf(buf, "%dmins", vstats->remaintime);
		SendDlgItemMessage(hwndStats,IDC_STAT_ESTTIME,WM_SETTEXT,0,(LPARAM)buf);
		sprintf(buf, "%dMB", vstats->estsize  >> 10);
		SendDlgItemMessage(hwndStats,IDC_STAT_ESTSIZE,WM_SETTEXT,0,(LPARAM)buf);
		sprintf(buf, "%dKbps", vstats->abitrate);
		SendDlgItemMessage(hwndStats,IDC_STAT_AB,WM_SETTEXT,0,(LPARAM)buf);
		sprintf(buf, "%dKbps", vstats->vbitrate);
		SendDlgItemMessage(hwndStats,IDC_STAT_VB,WM_SETTEXT,0,(LPARAM)buf);
	}
	return 0;
}

int CMenc::Encode()
{
	if (list.GetCount()<=0) return -1;

	T_ITEM* item = mcGetNextItem(0);
	int i = 0;
	int readyitems = 0;
	for (; item; item = mcGetNextItem(item), i++) {
		if (!(item->flags & IF_DONE)) readyitems++;
		list.SetSelection(i, 0);
	}
	if (!readyitems) {
		if (Ask(IDS_NO_READY_ITEM, MB_YESNO) == IDYES) {
			for (i = 0, item = mcGetNextItem(0); item; item = mcGetNextItem(item), i++) {
				item->flags &= ~IF_DONE;
				list.SetText(i, LC_STATE, itemstate[IS_READY]);
			}
		} else {
			MsgBox(IDS_NO_ITEM, 0);
			return -1;
		}
	}

	if (mcGetPrefValueInt(PI_STATS) == 0) {
		SHOWSTATS(1);
	}

	DWORD id;
	hThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)EncodeThread,(void*)this,0,&id);
	return 0;
}

static TCHAR** exts;

int CMenc::EnumDir(TCHAR* pchDir)
{
	WIN32_FIND_DATA* findData;
	CString fullpath;
	HANDLE hFind;
	int ret = 0;

	findData=(WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));
	fullpath.Format("%s\\*.*",pchDir);
	memset(findData->cFileName, 0, MAX_PATH);
	hFind = FindFirstFile(fullpath.GetBuffer(),findData);
	if (hFind == INVALID_HANDLE_VALUE) return 1;
	do {
		if (findData->cFileName[0]=='.' && (findData->cFileName[1]==0 || *(WORD*)(findData->cFileName+1)=='.')) continue;
		fullpath.Format("%s\\%s",pchDir,findData->cFileName);
		if (findData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			ret=0;
			if (mcGetPrefValueInt(PI_RECURSE)) ret=EnumDir(fullpath.GetBuffer());
			if (ret)
				break;
			else
				continue;
		}
		TCHAR* p=strrchr(findData->cFileName,'.');
		if (p++) {
			for (int i=0; exts[i]; i++) {
				if (!stricmp(p, exts[i])) {
					T_ITEM* item = mcMakeItem(fullpath.GetBuffer());
					CString alias;
					AddItem(item, alias);
					break;
				}
			}
		}
	} while (FindNextFile(hFind,findData));
	FindClose(hFind);
	free(findData);
	return ret;
}

void CMenc::AddFolder()
{
	TCHAR buf[MAX_PATH];
	if (!BrowseForPath(buf)) return;
	TCHAR *incfiletypes=strdup(mcGetPrefValueStr(PI_FILE_TYPES));
	TCHAR *p; 
	buf[strlen(buf)-1]=0;
	int i;
	for (i = 1, p = incfiletypes; *p; p++) {
		if (*p == ';') i++;
	}
	exts = (TCHAR**)malloc((i+ 1) * sizeof(TCHAR*));
	i = 0;
	for (TCHAR* p=strtok(incfiletypes,";"); p != 0; p=strtok(0,";")) {
		exts[i++] = p;
	}
	exts[i] = 0;
	EnumDir(buf);
	free(incfiletypes);
	free(exts);
}

void CMenc::ShowSubDialog(int idx)
{
	/*
	if (iOptionDialog>=0 && optionDialog[iOptionDialog]) optionDialog[iOptionDialog]->Hide();
	optionDialog[idx]->Refresh();
	optionDialog[idx]->Show();
	iOptionDialog=idx;
	*/
}

void CMenc::ShowAudioDialog(int idx)
{
}

bool CMenc::onNotify(LPNMHDR pnmh)
{
	int idx;
	switch (pnmh->code) {
	case LVN_ITEMACTIVATE:
		onClick(IDC_PLAY);
		break;
	case NM_CLICK:
		optionDialog[OD_SUMMARY]->Refresh();
		break;
	case TCN_SELCHANGE:
		idx = TabCtrl_GetCurSel(pnmh->hwndFrom);
		switch (pnmh->idFrom) {
		case IDC_TAB:
			ShowSubDialog(idx);
			break;
		case IDC_TAB_AUDIO:
			ShowAudioDialog(idx);
			if (mcstate->state==STATE_IDLE) {
				mcSetPrefValue(PI_AENC,(void*)idx);
				optionDialog[OD_AUDIO]->Refresh();
				RefreshGui();
			}
			break;
		case IDC_TAB_VIEW:
			mcSetPrefValue(PI_VIEW, (void*)idx);
			RefreshGui();
			break;
		}
		break;
	}
	return true;
}

bool CFileInfo::onCommand(int dlgid, int code)
{
	switch (dlgid) {
	case IDCANCEL:
		Close();
		break;
	default:
		return false;
	}
	return true;
}

void CFileInfo::SetText(int dlgid, CString& str)
{
	SendDlgMsg(dlgid, WM_SETTEXT, 0, (LPARAM)str.GetBuffer());
}

bool CFileInfo::onCreate()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 14;                      // request a 12-pixel-height font
	lf.lfWeight = FW_NORMAL;
	strcpy(lf.lfFaceName, "Courier New");
	HFONT hFont=CreateFontIndirect(&lf);
	SendDlgMsg(IDC_DETAIL, WM_SETFONT, (WPARAM)hFont, 0);
	return true;
}

void CFileInfo::Refresh(int index)
{
	int i = 0;
	T_ITEM* item = mcGetItemByIndex(index);
	if (!item) return;
	CString info;
	mcGetMediaInfo(item);
	SetText(IDC_EDIT1, (info = item->filename));
	SetText(IDC_EDIT3, (info = item->info.tags.title));
	SetText(IDC_EDIT4, (info = item->info.tags.artist));
	SetText(IDC_EDIT5, (info = item->info.tags.album));
	if (item->info.tags.year) {
		info.Format("%d", item->info.tags.year);
		SetText(IDC_EDIT6, info);
	}
	if (item->info.track) {
		info.Format("%d", item->info.track);
		SetText(IDC_EDIT7, info);
	} else if (item->info.tags.track) {
		info.Format("%d", item->info.tags.track);
		SetText(IDC_EDIT7, info);
	}
	if (item->info.chapter) {
		info.Format("%d", item->info.chapter);
		SetText(IDC_EDIT8, info);
	}
	SetText(IDC_EDIT9, (info = item->info.tags.genre));
	SetText(IDC_EDIT10, (info = item->info.tags.comment));
	if (item->info.filesize > 0) {
		info.Format("%d KB", item->info.filesize);
		SetText(IDC_EDIT13, info);
	}
	info.Format("%d ms", item->info.begin);
	SetText(IDC_EDIT11, info);
	if (item->info.end) {
		info.Format("%d ms", item->info.end - item->info.begin);
		SetText(IDC_EDIT12, info);
	} else if (item->info.duration) {
		info.Format("%d ms", item->info.duration);
		SetText(IDC_EDIT12, info);
	}
	SetText(IDC_EDIT14, (info = item->info.discroot));
	char *p = mcRunMediaTool(MT_MEDIAINFO, 0, item);
	if (p) {
		info = p;
		SetText(IDC_DETAIL, info);
	}
}

void GetFormatString(T_ITEM* item, CString& str)
{
	if (item->flags & IF_PRIVATE_FORMAT) {
		if (item->target.container != CF_DEFAULT) {
			str += mcGetToolName(TL_CONTAINER, 0, item->target.container);
			str += '/';
		}
		if (item->target.videoformat != VC_NONE) {
			str += mcGetToolName(TL_VIDEO_CODEC, 0, item->target.videoformat);
			str += '/';
		}
		if (item->target.audioformat != AC_NONE) {
			str += mcGetToolName(TL_AUDIO_ENCODER, 0, item->target.audioformat);
		}
		if (str.Right(1) == '/') {
			str.Delete(str.GetLength() - 1, 1);
		}
	}
}

void CMenc::ShowFileDetail(int index)
{
	if (index < 0) return;
	fileinfo.CreateDlg(0, IDD_FILEINFO, this);
	fileinfo.Refresh(index);
}

bool CMenc::onCommand(int dlgid, int code)
{
	if (code == EN_CHANGE) {
		if (dlgid == IDC_OUTPUT_FOLDER) {
			TCHAR *p = mcGetPrefValueStr(PI_DEST_DIR);
			int len = SendDlgMsg(IDC_OUTPUT_FOLDER,WM_GETTEXT,256,(LPARAM)p);
			if (len == 0) {
				*p = 0;
			} else {
				p += len;
				if (*(p - 1) != '\\') *(WORD*)p = '\\';
			}
			return true;
		}
	}
	return false;
}

bool CMenc::onSelChange(int dlgid)
{
	return onClick(dlgid);
}

static const int menuid[] = {IDM_OPT_DELETE_ORIGINAL, IDM_OPT_POWEROFF, 0};
static const int prefid[] = {PI_DEL_ORIGINAL, PI_POWEROFF};

void CMenc::Refresh()
{
	mcApplyParam();
	for (int i = 0; menuid[i]; i++) {
		if (mcGetPrefValueInt(prefid[i])) {
			if (!IsMenuChecked(menuid[i])) {
				CheckMenu(menuid[i]);
			}
		} else {
			if (IsMenuChecked(menuid[i])) {
				UncheckMenu(menuid[i]);
			}
		}
	}
	
	CString statestr;
	if (mcstate->state != STATE_ENCODING) {
		if (mcGetPrefValueInt(PI_CONTAINER) == 0) {
		if (mcGetPrefValueInt(PI_VCODEC) == VC_NONE) {
			statestr.Format("Audio Only: %s", mcGetToolName(TL_AUDIO_ENCODER, 1, mcGetPrefValueInt(PI_AENC)));
		} else {
			statestr.Format("Video: %s Audio: %s ",
				mcGetToolName(TL_VIDEO_CODEC, 0, mcGetPrefValueInt(PI_VCODEC)),
				mcGetToolName(TL_AUDIO_ENCODER, 0, mcGetPrefValueInt(PI_AENC))
			);
		}
	} else {
		if (mcGetPrefValueInt(PI_VCODEC) == VC_NONE) {
			statestr.Format("%s (%s)",
				mcGetToolName(TL_CONTAINER, 0, mcGetPrefValueInt(PI_CONTAINER)),
				mcGetToolName(TL_AUDIO_ENCODER, 0, mcGetPrefValueInt(PI_AENC))
			);
		} else {
			statestr.Format("%s (%s + %s)",
				mcGetToolName(TL_CONTAINER, 0, mcGetPrefValueInt(PI_CONTAINER)),
				mcGetToolName(TL_VIDEO_CODEC, 0, mcGetPrefValueInt(PI_VCODEC)),
				mcGetToolName(TL_AUDIO_ENCODER, 0, mcGetPrefValueInt(PI_AENC))
			);
		}
	}
	UpdateStatusBar(statestr.GetBuffer(),2,0);
	}
	TabCtrl_SetCurSel(GetHandle(IDC_TAB_VIEW), mcGetPrefValueInt(PI_VIEW));
	UpdateTabbedView();
	optionDialog[OD_SUMMARY]->Refresh();
	optionDialog[OD_AUDIO]->Refresh();
	optionDialog[OD_VIDEO]->Refresh();
}

void CMenc::UpdateTabbedView()
{
	TCHAR* p = 0;
	switch (mcGetPrefValueInt(PI_VIEW)) {
	case VIEW_LOG:
		SendDlgMsg(IDC_INFO, WM_SETTEXT, 0, 0);
		SendDlgMsg(IDC_INFO, EM_REPLACESEL, 0, (LPARAM)mcGetLogText(0));
		//SendDlgMsg(IDC_INFO, EM_SCROLLCARET, 0, 0);
		return;
	case VIEW_AUDIO:
		p = mcGetToolCommand(TL_AUDIO_ENCODER, 0);
		break;
	case VIEW_VIDEO: {
		CString s;
		for (int i = 0; i < 3; i++) {
			p = mcGetToolCommand(TL_VIDEO_CODEC, i);
			if (!p) break;
			if (i > 0) s += "\r\n";
			s += p;
			s += "\r\n";
		}
		SendDlgMsg(IDC_INFO,WM_SETTEXT,0,(LPARAM)((i == 0) ? 0 : s.GetBuffer()));
		return;
		}
	case VIEW_CONTAINER:
		p = mcGetToolCommand(TL_CONTAINER, 0);
		break;
	}
	SendDlgMsg(IDC_INFO,WM_SETTEXT,0,(LPARAM)p);
}

// mode: 0 - toggle, 1 - set private, 2 - set public
int CMenc::SetFormatMode(int mode)
{
	for (int i = list.GetCount() - 1; i>=0; i--) {
		T_ITEM* item;
		if (list.GetSelection(i)) {
			{
				item = mcGetItemByIndex(i);
			}
			if (item) {
				switch (mode) {
				case 0:
					if (item->flags & IF_PRIVATE_FORMAT)
						item->flags &= ~IF_PRIVATE_FORMAT;
					else
						item->flags |= IF_PRIVATE_FORMAT;
					break;
				case 1:
					item->flags |= IF_PRIVATE_FORMAT;
					break;
				case 2:
					item->flags &= ~IF_PRIVATE_FORMAT;
					break;
				}
				if (item->flags & IF_PRIVATE_FORMAT) {
					CString str;
					item->target.audioformat = (AUDIO_CODEC)mcGetPrefValueInt(PI_AENC);
					item->target.videoformat = (VIDEO_CODEC)mcGetPrefValueInt(PI_VCODEC);
					item->target.container = (CONTAINER_FORMAT)mcGetPrefValueInt(PI_CONTAINER);
					item->target.videoenc = (VIDEO_ENCODER)mcGetPrefValueInt(PI_VENC);
					GetFormatString(item, str);
					list.SetText(i, LC_FORMAT, str.GetBuffer());
				} else {
					list.SetText(i, LC_FORMAT, 0);
				}
			}
		}
	}
	
	return 0;
}

T_ITEM* GetSelectedItem()
{
	int i = menc.list.GetFocus();
	if (i >= 0) {
		T_ITEM* item = mcGetItemByIndex(i);
		if (item) {
			mcGetMediaInfo(item);
		}
		return item;
	} else {
		return 0;
	}
}

bool CMenc::onClick(int dlgid)
{
	switch (dlgid) {
	case IDC_PLAY:
	case IDM_PLAY: {
		T_ITEM* item = GetSelectedItem();
		if (item) {
			mcPlayerStart(item, 0, 0, 0, 0);
		}
		} break;
	case IDM_REMOVE_ALL_ITEMS:
	case IDC_CLEAR:
		if (mcstate->state != STATE_ENCODING) {
			list.Reset();
			mcRemoveAllItems();
		}
		break;
	case IDM_REMOVE_ITEM:
	case IDC_REMOVE:
		if (mcstate->state != STATE_ENCODING) {
			for (int i = list.GetCount() - 1; i>=0; i--) {
				if (list.GetSelection(i)) {
					T_ITEM* item = mcGetItemByIndex(i);
					if (!item) break;
					if (mcRemoveItem(item) < 0) break;
					list.RemoveItem(i);
					list.SetSelection(i, 1);
				}
			}
			SetFocus(list.hwnd);
		}
		break;
	case IDM_SET_STATE_DONE: {
		T_ITEM* item = mcGetNextItem(0);
		for (int i = 0; item; item = mcGetNextItem(item), i++) {
			if (list.GetSelection(i)) {
				item->flags |= IF_DONE;
				list.SetText(i, LC_STATE, itemstate[IS_DONE]);
			}
		}
		} break;
	case IDM_SET_STATE_UNDONE: {
		T_ITEM* item = mcGetNextItem(0);
		for (int i = 0; item; item = mcGetNextItem(item), i++) {
			if (list.GetSelection(i)) {
				item->flags &= ~IF_DONE;
				list.SetText(i, LC_STATE, itemstate[IS_READY]);
			}
		}
		} break;
	case IDM_RESET_STATE: {
		T_ITEM* item = mcGetNextItem(0);
		for (int i = 0; item; item = mcGetNextItem(item), i++) {
			item->flags &= ~IF_DONE;
			list.SetText(i, LC_STATE, itemstate[IS_READY]);
		}
		} break;
	case IDC_ADDDIR:
	case IDM_ADDFOLDER:
		if (mcstate->state != STATE_ENCODING) {
			AddFolder();
		}
		break;
	case IDM_SETOUTPUT:
	case IDC_SETOUTPUT:
		if (!BrowseForPath(mcGetPrefValueStr(PI_DEST_DIR))) return true;
		SendDlgMsg(IDC_OUTPUT_FOLDER,WM_SETTEXT,0,(LPARAM)mcGetPrefValueStr(PI_DEST_DIR));
		break;
	case IDC_OPEN_OUTPUT:
		ShellExecute(menc.hwnd, "open", mcGetPrefValueStr(PI_DEST_DIR), 0, 0, SW_SHOW);
		break;
	case IDC_SETTEMP:
		if (!BrowseForPath(mcGetPrefValueStr(PI_TEMP_DIR))) return true;
		SendDlgMsg(IDC_TEMP_FOLDER,WM_SETTEXT,0,(LPARAM)mcGetPrefValueStr(PI_DEST_DIR));
		break;
	case IDC_ADDFILE:
	case IDM_ADDFILE:
		if (mcstate->state != STATE_ENCODING) {
			OPENFILENAME ofn;
			int i;
			TCHAR buf[2048],*p,*q,*s;
			*buf=0;
			memset(&ofn, 0, sizeof(ofn));
			// generate file extension filter
			p = buf + 4;
			ofn.lpstrFilter = p;
			p += sprintf(p,"Media Files");
			*(p++)=0;
			FILE_TYPE* ft;
			for (i = 0; ; i++) {
				ft = mcGetFileTypeByIndex(i);
				if (!ft->fileext) break;
				p += sprintf(p,"*.%s;", ft->fileext);
			}
			*(p-1)=0;
			for (i = 0; ; i++) {
				ft = mcGetFileTypeByIndex(i);
				if (!ft->fileext) break;
				p += sprintf(p, "%s (%s)", ft->desc, ft->fileext);
				*(p++) = 0;
				p += sprintf(p,"*.%s", ft->fileext);
				*(p++) = 0;
			}
			memcpy(p,"All Files\0*.*\0",15);
			// show file dialog
			ofn.lpstrFile = buf;
			ofn.nMaxFile = sizeof(buf);
			ofn.Flags = OFN_ALLOWMULTISELECT;
			ofn.lpstrTitle="Add File";
			if (!ShowFileDialog(0, &ofn)) return true;
			// add selected files
			s=strchr(buf,0);
			if (*(WORD*)s) {
				p=s;
				do {
					for (q=++p; *q; q++);
					if (*(s-1)!='\\') *(s++)='\\';
					strcpy(s,p);
					T_ITEM* item = mcMakeItem(buf);
					CString alias;
					AddItem(item, alias);
					p=q;
				} while (*(WORD*)p);
			} else {
				T_ITEM* item = mcMakeItem(buf);
				CString alias;
				AddItem(item, alias);
			}
		}
		break;
	case IDC_ADV_SETTINGS:
		mcShowConfigDialog(CD_AUDIO_ENC, ParamNotify);
		break;
	case IDM_ENCODEAUDIO:
		optionDialog[OD_AUDIO]->SendDlgMsg(IDC_CODECS,CB_SETCURSEL,2,0);
		mcSetPrefValue(PI_VCODEC, (void*)VC_NONE);
		RefreshGui();
	case IDM_ENCODE:
	case IDC_TRANSCODE:
		if (mcGetState()==STATE_ENCODING) {
			mcSetState(STATE_STOPPING);
		} else {
			Encode();
		}
		break;
	case IDM_STOP_ENCODE:
		mcSetState(STATE_STOPPING);
		break;
	case IDC_SKIP:
		if (mcGetState()==STATE_ENCODING) mcSetState(STATE_SKIPPING);
		break;
	case IDM_PLAY_ENCODED:
		mcPreviewStart(0, 0, 0, 0);
		break;
	case IDM_STOP_PLAY:
		mcPlayerStop();
		break;
	case IDM_CONSOLE:
		ShowConsole((int)mcSetPrefValue(PI_SHOW_CONSOLE, (void*)(1 - mcGetPrefValueInt(PI_SHOW_CONSOLE))));
		SetForegroundWindow(hwnd);
		break;
	case IDM_TOGGLE_STATS:
		if (wndStats.hwnd) {
			SHOWSTATS(0);
		} else {
			SHOWSTATS(1);
			if (mcGetState() == STATE_ENCODING) UpdateTaskStats();
		}
		break;
	case IDM_FILE_DETAIL:
		ShowFileDetail(list.GetFocus());
		break;
	case IDM_CLEARLOG:
		mcSetLogText("");
		UpdateTabbedView();
		break;
	case IDM_AUDIO_ENCODER_VER: {
		int id = mcGetPrefValueInt(PI_AENC);
		TCHAR *ver = mcGetToolVersion(TL_AUDIO_ENCODER, id);
		if (ver) {
			MsgBox(ver, "Audio encoder version");
		} else {
			CString str;
			str.Format(IDS_NO_ENCODER_VER, mcGetToolName(TL_AUDIO_ENCODER, 1, id));
			MsgBox(str.GetBuffer(), 0);
		}
		} break;
	case IDM_VIDEO_ENCODER_VER: {
		int id = mcGetPrefValueInt(PI_VENC);
		TCHAR *ver = mcGetToolVersion(TL_VIDEO_ENCODER, id);
		if (ver) {
			MsgBox(ver, "Video encoder version");
		} else {
			CString str;
			str.Format(IDS_NO_ENCODER_VER, mcGetToolName(TL_VIDEO_ENCODER, 1, id));
			MsgBox(str.GetBuffer(), 0);
		}
		} break;
	case IDM_MUXER_VER: {
		int id = mcGetPrefValueInt(PI_CONTAINER);
		TCHAR *ver = mcGetToolVersion(TL_CONTAINER, id);
		if (ver) {
			MsgBox(ver, "Container muxer version");
		} else {
			CString str;
			str.Format(IDS_NO_ENCODER_VER, mcGetToolName(TL_CONTAINER, 1, id));
			MsgBox(str.GetBuffer(), 0);
		}
		} break;
	case IDM_HELP_DOC:
		ShellExecute(hwnd, "open", "doc\\guide.htm", 0, 0, SW_SHOW);
		break;
	case IDM_EXIT:
		onClose();
		break;
	case IDC_PREF:
	case IDM_PREFERENCE:
		mcShowConfigDialog(CD_PREF, ParamNotify);
		break;
	case IDM_SWITCH_AUDIO:
		mcPlayerCommand("switch_audio");
		break;
	case IDM_LEFT_CHANNEL:
		mcPlayerCommand("switch_audio 1");
		break;
	case IDM_RIGHT_CHANNEL:
		mcPlayerCommand("switch_audio 2");
		break;
	case IDM_STEREO:
		mcPlayerCommand("switch_audio 0");
		break;
	case IDM_LEFT25_RIGHT75:
		mcPlayerCommand("switch_audio 5");
		break;
	case IDM_LEFT50_RIGHT50:
		mcPlayerCommand("switch_audio 4");
		break;
	case IDM_LEFT75_RIGHT25:
		mcPlayerCommand("switch_audio 3");
		break;
	case IDM_PAUSE:
		mcPlayerCommand("pause");
		break;
	case IDM_STOP:
		mcPlayerCommand("quit");
		break;
	case IDM_INC_VOL:
		mcPlayerCommand("volume 1");
		break;
	case IDM_DEC_VOL:
		mcPlayerCommand("volume -1");
		break;
	case IDM_MUTE:
		mcPlayerCommand("mute");
		break;
	case IDM_FULLSCREEN:
		mcPlayerCommand("vo_fullscreen");
		break;
	case IDM_ONTOP:
		mcPlayerCommand("vo_ontop");
		break;
	case IDM_WA_CONFIG:
		mcShowConfigDialog(CD_DSP_PLUGIN, 0);
		break;
	case IDM_REVERT_ALL:
		mcLoadParam(0, 1);
		DeleteFile(CFGFILE);
		ParamNotify(0, 0);
		break;
	case IDM_SAVEASDEFAULT:
		mcSaveParam(CFGFILE);
		UpdateStatusBar("Saved as default",0,0);
		break;
	case IDM_LOADQUEUE: {
		TCHAR buf[MAX_PATH];
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		buf[0]=0;
		ofn.lpstrFile = buf;
		ofn.nMaxFile = sizeof(buf);
		ofn.lpstrFilter = "M3U List\0*.m3u\0Text File List\0*.lst;*.txt\0";
		ofn.lpstrTitle = "Load a list";
		if (!ShowFileDialog(0,&ofn)) return true;
		if (LoadQueue(buf) > 0) {
			UpdateStatusBar("Queue loaded",0,0);
		}
		} break;
	case IDM_SAVEQUEUE: {
		TCHAR buf[MAX_PATH];
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		buf[0] = 0;
		ofn.lpstrFile = buf;
		ofn.nMaxFile = sizeof(buf);
		ofn.lpstrFilter = "M3U List\0*.m3u\0";
		ofn.lpstrTitle = "Save a list";
		if (!ShowFileDialog(1,&ofn)) return true;
		if (!strstr(buf, ".xml")) strcat(buf,".xml");
		if (SaveQueue(buf) > 0) {
			UpdateStatusBar("Queue saved",0,0);
		}
		} break;
	case IDM_LOAD_PRESET: {
		TCHAR buf[MAX_PATH];
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		buf[0]=0;
		ofn.lpstrFile = buf;
		ofn.nMaxFile = sizeof(buf);
		ofn.lpstrFilter = "Presets\0*.xml\0";
		ofn.lpstrTitle = "Load a Preset";
        ofn.lpstrInitialDir = buf + 16;
		sprintf((TCHAR*)ofn.lpstrInitialDir, "%spresets", apppath);
		if (!ShowFileDialog(0,&ofn)) return true;
		if (!mcLoadParam(buf, 1)) {
			Refresh();
			UpdateStatusBar("Preset loaded",0,0);
		}
		} break;
	case IDM_SAVE_PRESET: {
		TCHAR buf[MAX_PATH];
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		buf[0] = 0;
		ofn.lpstrFile = buf;
		ofn.nMaxFile = sizeof(buf);
		ofn.lpstrFilter = "Presets\0*.xml\0";
		ofn.lpstrTitle = "Save a Preset";
        ofn.lpstrInitialDir = buf + 16;
		sprintf((TCHAR*)ofn.lpstrInitialDir, "%spresets", apppath);
		if (!ShowFileDialog(1,&ofn)) return true;
		if (!strstr(buf, ".xml")) strcat(buf,".xml");
		if (!mcSaveParam(buf)) {
			UpdateStatusBar("Preset saved",0,0);
		}
		} break;
	case IDM_SET_FORMAT_PUBLIC:
		SetFormatMode(2);
		break;
	case IDM_SET_FORMAT_PRIVATE:
		SetFormatMode(1);
		break;
	case IDM_TOGGLE_FORMAT:
		SetFormatMode(0);
		break;
	case IDM_VERSION: {
		MsgBox(version,0);
		} break;
	case IDM_VISIT_WEBSITE1:
	case IDM_VISIT_WEBSITE2:
		VisitWebsite((dlgid == IDM_VISIT_WEBSITE1) ? 0 : 1);
		break;
	case IDM_SPLASH:
		splash.Open(0, this);
		break;
	case IDM_OPT_POWEROFF:
	case IDM_OPT_DELETE_ORIGINAL: {
		for (int i = 0; ; i++) {
			if (menuid[i] == dlgid) {
				if (IsMenuChecked(dlgid)) {
					mcSetPrefValueInt(prefid[i], 0);
					UncheckMenu(dlgid);
				} else {
					mcSetPrefValueInt(prefid[i], 1);
					CheckMenu(dlgid);
				}
				break;
			}
		}
		} break;
	case IDM_SHOW_TIME:
		if (IsMenuChecked(dlgid)) {
			((CDlgPreview*)optionDialog[OD_PREVIEW])->SetOsd(0);
			((CDlgPreview*)optionDialog[OD_PREVIEW2])->SetOsd(0);
			UncheckMenu(dlgid);
		} else {
			((CDlgPreview*)optionDialog[OD_PREVIEW])->SetOsd(2);
			((CDlgPreview*)optionDialog[OD_PREVIEW2])->SetOsd(2);
			CheckMenu(dlgid);
		}
		break;
	default:
		return false;
	}
	return true;
}

int CMenc::Init()
{
	HWND hwndTab;
	int i;
	RECT rc;

	for (i = 0; i < OD_COUNT; i++) {
		switch (i) {
		case OD_VIDEO:
			hwndTab=GetHandle(IDC_FRAME_VIDEO);
			optionDialog[OD_VIDEO]=new CDlgVideoOption(hwndTab);
			break;
		case OD_AUDIO:
			hwndTab=GetHandle(IDC_FRAME_AUDIO);
			optionDialog[OD_AUDIO]=new CDlgAudio(hwndTab);
			break;
		case OD_SUMMARY:
			hwndTab=GetHandle(IDC_SRC_INFO);
			optionDialog[OD_SUMMARY] = new CDlgSummary(hwndTab);
			break;
		case OD_PREVIEW:
			hwndTab=GetHandle(IDC_FRAME_PREVIEW);
			optionDialog[OD_PREVIEW] = new CDlgPreview(hwndTab);
			((CDlgPreview*)optionDialog[OD_PREVIEW])->mode = 0;
			break;
		case OD_PREVIEW2:
			hwndTab=GetHandle(IDC_FRAME_PREVIEW2);
			optionDialog[OD_PREVIEW2] = new CDlgPreview(hwndTab);
			((CDlgPreview*)optionDialog[OD_PREVIEW2])->mode = 1;
			break;
		}
		GetClientRect(hwndTab, &rc);
		int width, height;
		optionDialog[i]->GetWindowPos(0, 0, &width, &height);
		optionDialog[i]->Move((rc.right - rc.left - width) / 2, 4 + (rc.bottom - rc.top - height) / 2, -1, -1);
		optionDialog[i]->Refresh();
		optionDialog[i]->Show();
	}

	ShowSubDialog(0);
	SetCaption(caption);

	return 0;
}

void CMenc::ShowState(int state)
{
	TCHAR* s = "Idle";

	switch (state) {
	case STATE_ENCODING:
		switch (mcstate->stage) {
		case ST_PROCESSING_AUDIO:
			s = "Encoding audio";
			break;
		case ST_PROCESSING_VIDEO:
			s = "Encoding video";
			break;
		case ST_MUXING:
			s = "Merging";
			break;
		}
		break;
	case STATE_PLAYING:
		s = "Playing";
		break;
	case STATE_SHUTDOWN:
		s = "Shutting down";
		break;
	}
	UpdateStatusBar(s,0,0);
}

bool CMenc::onCreate()
{
	static COL_INFO cols[]={{lngFileName, 155}, {lngFileSize, 56}, {lngFileType, 125}, {lngState, 50}, {lngTargetFormat, 85}, {lngFullPath, 140}, {0}};
	static const int bmp[]={IDB_ADDFILE, IDB_ADDFOLDER, IDB_ADDTRACK, IDB_REMOVE, IDB_CLEAR, IDB_PLAY, IDB_SKIP, IDB_PREF, IDB_START};
	static const int buttons[]={IDC_ADDFILE, IDC_ADDDIR, IDC_ADDTRACK, IDC_REMOVE, IDC_CLEAR, IDC_PLAY, IDC_SKIP, IDC_PREF, IDC_TRANSCODE, 0};

	SetClassLongPtr(hwnd,GCL_HICON,(LONG)hAppIcon); 

	for (int i = 0; buttons[i]; i++)
		SendDlgMsg(buttons[i],BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(LoadBitmap(hInst,MAKEINTRESOURCE(bmp[i]))));
	
	hBmpPlay[0] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_PLAY));
	hBmpPlay[1] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_STOPPLAY));
	hBmpStart[0] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_START));
	hBmpStart[1] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_STOP));

	list.hwnd = GetHandle(IDC_FILELIST);
	list.SetExtendedStype(LVS_EX_FULLROWSELECT);
	list.AddColumns(cols);
	SendDlgMsg(IDC_PROGRESS,PBM_SETRANGE,true,MAKELONG(0,100));
	RegisterDropWindow(hwnd);
	hWndStatusbar=CreateSBar(hwnd,NULL,3,300);
	ParamNotify(PI_FACE_ALPHA, (void*)mcGetPrefValueInt(PI_FACE_ALPHA));
	ShowState(mcstate->state);

	if (mcGetPrefValueInt(PI_SHOW_CONSOLE))
		ShowConsole(1);

	return true;
}

int CMenc::LoadQueue(TCHAR* filename)
{
	TCHAR *p, *nl;
	T_ITEM itemdata;
	TCHAR *text = 0;
	CString alias;
	int count = 0;
	bool m3u = false;
	if (LoadTextFile(filename, &text, 0) <= 0) return -1;
	p = text;
	if (!strncmp(p, "#EXTM3U", 7)) {
		m3u = true;
		p += 7;
	}
	memset(&itemdata, 0, sizeof(T_ITEM));
	for (; p; p = nl) {
		while (*(p++) != '\n');
		nl = strchr(p, '\r');
		if (!nl) nl = strchr(p, '\n');
		if (nl) *nl = 0;
		if (!*p) continue;
		if (m3u && *p == '#') {
			p++;
			if (!strncmp(p, "EXTINF:", 7)) {
				p = strchr(p + 7, ',');
				if (!p) continue;
				alias = ++p;
			} else if (!strncmp(p, "EXTINF_FLAG:", 12)) {
				p += 12;
				itemdata.flags = atoi(p);
			} else if (!strncmp(p, "EXTINF_TAG:", 11)) {
				char **ptag[] = {&itemdata.info.tags.artist, &itemdata.info.tags.title, &itemdata.info.tags.genre, &itemdata.info.tags.comment, 0};
				char *s;
				itemdata.info.tags.valid = 1;
				p += 11;
				for (int i = 0; ptag[i]; i++) {
					if (!(s = strchr(p, ';'))) break;
					*s = 0;
					if (*p) *ptag[i] = strdup(p);
					p = s + 1;
				}
				itemdata.info.tags.year = atoi(p);
				p = strchr(p, ';');
				if (!p) continue;
				itemdata.info.tags.track = atoi(++p);
				p = strchr(p, ';');
				if (!p) continue;
				itemdata.info.tags.trackcount = atoi(++p);
				p = strchr(p, ';');
				if (!p) continue;
			} else if (!strncmp(p, "EXTINF_TIME:", 12)) {
				p += 12;
				itemdata.info.begin = atoi(p);
				p = strchr(p, ';');
				if (!p) continue;
				itemdata.info.end = atoi(++p);
				p = strchr(p, ';');
				if (!p) continue;
				itemdata.info.track = atoi(++p);
				p = strchr(p, ';');
				if (!p) continue;
				itemdata.info.chapter = atoi(++p);
				p = strchr(p, ';');
				if (!p) continue;
				itemdata.info.discroot = strdup(++p);
			} else if (!strncmp(p, "EXTINF_FMT:", 11)) {
				p += 11;
				itemdata.flags |= IF_PRIVATE_FORMAT;
				if ((p = strtok(p, ";"))) itemdata.target.container = (CONTAINER_FORMAT)atoi(p);
				if ((p = strtok(0, ";"))) itemdata.target.audioformat = (AUDIO_CODEC)atoi(p);
				if ((p = strtok(0, ";"))) itemdata.target.videoformat = (VIDEO_CODEC)atoi(p);
				if ((p = strtok(0, ";"))) itemdata.target.videoenc = (VIDEO_ENCODER)atoi(p);
			}
			continue;
		}
		T_ITEM* item = mcMakeItem(p);
		memcpy(item, &itemdata, sizeof(T_ITEM));
		memset(&itemdata, 0, sizeof(T_ITEM));
		item->filename = strdup(p);
		item->info.filesize = RetrieveFileSize(item->filename);
		item->info.filetype = mcIdentifyFileType(item->filename);
		AddItem(item, alias);
		count++;
	}
	free(text);
	return count;
}

int CMenc::SaveQueue(TCHAR* filename)
{
	int count = list.GetCount();
	if (count>0) {
		FILE *fp=fopen(filename,"w");
		if (fp>0) {
			int i = 0;
			fprintf(fp, "#EXTM3U\n");
			for (T_ITEM* item = mcGetNextItem(0); item; item = mcGetNextItem(item), i++) {
				TCHAR buf[MAX_PATH];
				list.GetText(i, LC_FILENAME, buf, sizeof(buf));
				fprintf(fp,"#EXTINF:%d,%s\n", item->info.duration, buf);
				fprintf(fp,"#EXTINF_FLAG:%d\n", item->flags & ~IF_INFO_RETRIEVED);
				if (item->info.tags.valid) {
					fprintf(fp,"#EXTINF_TAG:%s;%s;%s;%s;%s;%d;%d;%d\n",
						item->info.tags.artist ? item->info.tags.artist : "",
						item->info.tags.title ? item->info.tags.title : "",
						item->info.tags.album ? item->info.tags.album : "",
						item->info.tags.genre ? item->info.tags.genre : "",
						item->info.tags.comment ? item->info.tags.comment : "",
						item->info.tags.year, item->info.tags.track, item->info.tags.trackcount);
				}
				if (item->info.track || item->info.begin || item->info.begin || item->info.discroot) {
					fprintf(fp,"#EXTINF_TIME:%d;%d;%d;%d",
						item->info.begin, item->info.end, item->info.track, item->info.chapter);
					if (item->info.discroot)
						fprintf(fp,";%s",item->info.discroot);
					fprintf(fp, "\n");
				}
				if (item->flags & IF_PRIVATE_FORMAT) {
					fprintf(fp,"#EXTINF_FMT:%d;%d;%d;%d\n",
						item->target.container, item->target.audioformat, item->target.videoenc, item->target.videoformat);
				}
				fprintf(fp,"%s\n", item->filename);
			}
			fclose(fp);
		}
	}
	return count;
}

bool CMenc::onClose()
{
	if (mcGetState() == STATE_ENCODING) {
		if (Ask(IDS_EXIT_CONFIRM, MB_YESNO) == IDNO) return true;
	}
	mcPlayerStop();
	mcSetState(STATE_STOPPING);
	ShowConsole(0);
	Hide();
	WaitForSingleObject(hThread,3000);
	// save encoding list
	DeleteFile(LISTFILE);
	if (mcGetPrefValueInt(PI_SAVE_LIST)) {
		SaveQueue(LISTFILE);
	}
	UnregisterDropWindow(hwnd);
	GlobalUninit();
	Destroy();
	PostQuitMessage(0);
	return true;
}

void CMenc::ShowConsole(int fShow)
{
	TCHAR buf[512];
	if (fShow) {
		if (fConsole || !AllocConsole()) return;
#ifndef CONFIG_WIN9X
		SetConsoleTitle(CONSOLE_CAPTION);
		MoveWindow(GetConsoleWindow(),0,0,700,540,true);
#endif
		sprintf(buf, "%s\n\nProcessor information:\n%s\n\n", version, mcstate->cpucaps.cpuFullName);
		EVENT(MC_MSG_INFO, buf);
		fConsole=true;
	} else if (fConsole) {
		FreeConsole();
		fConsole=false;
	}
}

int CMenc::AddMultipleFiles(TCHAR* filenames)
{
	int count = 0;
	TCHAR *p = filenames;
	do {
		TCHAR *s = p;
		if (*p == '\"') {
			s++;
			for (p++; *p && *p!= '\"'; p++);
			if (*p) {
				*(p++) = 0;
			}
		} else {
			for (; *p && *p!= ' '; p++);
		}
		T_ITEM* item = mcMakeItem(s);
		CString alias;
		AddItem(item, alias);
		count++;
	} while (*(p++) == ' ');
	return count;
}

int CMenc::AddItem(T_ITEM* item, CString& alias)
{
	DWORD filesize = 0;
	int count = 0;
	int type = item->info.filetype->type;
	int track = 1;
	int trackcount = 1;
	CString discroot;


	do {
		if (alias.IsEmpty()) {
			CFileName fn;
			fn = item->filename;
			fn.GetName(alias);
		}
		if (!mcAddItem(item)) {
			TCHAR* items[LC_COUNT + 1];
			CString format;
			TCHAR buf[64], *p = buf;
			items[LC_FILENAME] = alias.GetBuffer();
			if (item->info.filesize >= 10240) {
				items[LC_SIZE] = p;
				p += sprintf(p, "%d MB", item->info.filesize >> 10);
			} else if (item->info.filesize > 0) {
				items[LC_SIZE] = p;
				p += sprintf(p, "%d KB", item->info.filesize);
			} else {
				items[LC_SIZE] = "";
			}
			items[LC_TYPE] = item->info.filetype ? item->info.filetype->desc : "";
			items[LC_STATE] = itemstate[item->flags & IF_DONE ? 1 : 0];
			items[LC_FULLPATH] = item->filename;
			GetFormatString(item, format);
			items[LC_FORMAT] = format.GetBuffer();
			items[LC_COUNT] = 0;
			list.AddItem(items);
		}
	} while (++track <= trackcount);
	return 0;
}

void CMenc::UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber, WORD displayFlags)
{
    SendMessage(hWndStatusbar,SB_SETTEXT,partNumber | displayFlags,(LPARAM)lpszStatusString);
}

bool CMenc::onMove()
{
	if (wndStats.hwnd) {
		int x,y,w,h;
		int sh;
		GetWindowPos(&x, &y, &w, &h);
		wndStats.GetWindowPos(0, 0, 0, &sh);
		wndStats.Move(x + w, y + h - sh, -1, -1);
	}
	return true;
}

bool CMenc::onSize()
{
	onTimer(1);
	return true;
}

static int ShowStats()
{
	TCHAR buf[128];
	int percent = 0;
	switch (mcstate->stage) {
	case ST_PROCESSING_AUDIO: {
		int length = mcstate->astats.length / 1000;
		int pos = mcstate->astats.pos / 1000;
		percent = (int)mcstate->astats.percent;
		if (menc.GetShowState() == SW_SHOWMINIMIZED) {
			if (length) {
				sprintf(buf, "%d%% (%d/%d)", percent, pos, length);
				menc.UpdateTitle(buf);
			}
		} else {
			if (length) {
				if (mcstate->stage == ST_EXTRACTING_PCM) {
					sprintf(buf,"extracting PCM (%3.1f%% done)", mcstate->astats.percent);					
				} else {
					sprintf(buf,"transcoding audio (%3.1f%% done)", mcstate->astats.percent);
				}
				menc.UpdateTitle(buf);
			}
			HWND hwndStats = menc.wndStats.hwnd;
			if (hwndStats) {
				sprintf(buf,"%ds",pos);
				SendDlgItemMessage(hwndStats,IDC_STAT_POS,WM_SETTEXT,0,(LPARAM)buf);
				sprintf(buf,"%dK", (int)(mcstate->astats.bytes>>10));
				SendDlgItemMessage(hwndStats,IDC_STAT_FRAME,WM_SETTEXT,0,(LPARAM)buf);
				if (mcstate->astats.speed > 0) {
					sprintf(buf,"%d.%dX", mcstate->astats.speed / 10, mcstate->astats.speed % 10);
					SendDlgItemMessage(hwndStats,IDC_STAT_FPS,WM_SETTEXT,0,(LPARAM)buf);
				}
			}
		}
		UpdateTime();
		} break;
	case ST_PROCESSING_VIDEO:
		percent = (int)mcstate->vstats->percent;
		if (mcstate->vstats) {
			if (menc.GetShowState() == SW_SHOWMINIMIZED) {
				char buf[32];
				sprintf(buf,"%4.1f%% done (pass %d of %d)",
					mcstate->vstats->percent,
					mcstate->currentpass,
					mcstate->totalpass);
				menc.SetCaption(buf);
			} else {
				UpdateVideoStats();
			}
			UpdateTime();
		}
		break;
	default:
		return -1;
	}
	if (percent >= 0 && percent <= 100) {
		char buf[16];
		sprintf(buf, "%d%%", percent);
		menc.list.SetText(iQueueItemIndex, LC_STATE, buf);
		menc.SendDlgMsg(IDC_PROGRESS, PBM_SETPOS, percent, 0);
	}
	return 0;
}

CMenc::CMenc()
{
	CString str;
	iOptionDialog=-1;
	//@richardGuan 6.6
	//str.Format("%s %d.%d.%d",APPNAME,VER_MAJOR,VER_MINOR,VER_REV);
	str.Format("%s",APPNAME);
#ifdef CONFIG_WIN9X
	str += " (Win9x)";
#endif
	caption=strdup(str.GetBuffer());
	str.Format("Mp4 Express Version1.0.7 (Based on MediaCoder Version %d.%d.%d build %d)\n",
		 VER_MAJOR, VER_MINOR, VER_REV, BUILD_NO);
	//@richardGuan 6.6
	//str.Format("%s Version %d.%d.%d build %d (built on %s)\n",
	//	APPNAME, VER_MAJOR, VER_MINOR, VER_REV, BUILD_NO, BUILD_DATE);
	
#ifndef STANDALONE
	TCHAR *p;
	mcGetBuildInfo(&p);
	str.AppendFormat("Based on MediaCoder core library build %d (built on %s)\n", mcGetBuildInfo(0), p);
#endif
	//@richardGuan 6.6
	//str += "This is a free software written by Stanley Huang <reachme@citiz.net>. Enjoy!";
	version=strdup(str.GetBuffer());
}

int GetAppPath(TCHAR *buf,int bufsize)
{
	TCHAR *p;
	p=buf+GetModuleFileName(GetModuleHandle(NULL),buf,bufsize);
	while(p>buf && *(--p)!='\\');
	*(++p)=0;
	return p-buf;
}

void DropInCallback(DROPFILES *dropfiles)
{
	WORD* unames = (WORD*)((TCHAR*)dropfiles + dropfiles->pFiles);
	TCHAR* anames;
	int len, i;
	for (i = 0, len = 0; ; i++) {
		if (!unames[i]) {
            anames = (TCHAR*)malloc(len*2);
			memset(anames, 0, len*2);
			WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)(unames + i - len), len, anames, len*2, 0, 0);
			T_ITEM* item = mcMakeItem(anames);
			CString alias;
			menc.AddItem(item, alias);
			free(anames);
			len = 0;
			if (!unames[i+1]) break;
		} else {
			len++;
		}
	}
}

void GlobalInit()
{
	idThread = GetCurrentThreadId();
	{
		WSADATA wsaData;
		WSAStartup( MAKEWORD( 1, 1 ), &wsaData );
	}
	GetAppPath(apppath,sizeof(apppath));
	SetCurrentDirectory(apppath);

	mcstate = mcInit(0 , &EventHandler);
	if (!mcstate) {
		menc.MsgBox("Fatal error", 0);
		PostQuitMessage(0);
		return;
	}

	mcLoadParam(CFGFILE, 0);

	hAppIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
}

bool CMenc::onShow(bool fShow)
{
	SendDlgMsg(IDC_OUTPUT_FOLDER,WM_SETTEXT,0,(LPARAM)mcGetPrefValueStr(PI_DEST_DIR));
	Refresh();	
	return true;
}

void GlobalUninit()
{
	mcUninit();
	mcSaveParam(CFGFILE);
	WSACleanup();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	struct HACCEL__ * hAccelTable=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCEL));
	hInst = hInstance;

	GlobalInit();

	if (mcGetPrefValueInt(PI_SHOW_SPLASH))
		splash.Open(SPLASH_SHOW_TIME, 0);

	if (!menc.CreateDlg(0, IDD_MAIN, (HWND)0) || menc.Init()) return 0;

	if (*lpCmdLine) {
		menc.AddMultipleFiles(lpCmdLine);
	} else if (mcGetPrefValueInt(PI_SAVE_LIST)) {
		menc.LoadQueue(LISTFILE);
	}
	mcPostInit(menc.hwnd);

	// show CPU speed
	{
		CString str = mcstate->cpucaps.vendor;
		if (mcstate->cpucaps.freq > 1000) {
			int spd = (mcstate->cpucaps.freq + 100) / 100;
			str.AppendFormat(" @ %d.%d Ghz", spd / 10, spd % 10);
		} else {
			str.AppendFormat(" @ %d Mhz", mcstate->cpucaps.freq);
		}
		menc.UpdateStatusBar(str.GetBuffer(),1,0);
	}
	menc.Show();
	menc.SetForeground();
	ParamNotify(PI_STATS, (void*)mcGetPrefValueInt(PI_STATS));
	
	while (GetMessage(&msg,0,0,0)) {
		if (!TranslateAccelerator(menc.hwnd,hAccelTable,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
