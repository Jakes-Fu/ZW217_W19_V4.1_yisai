/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/
#include <windows.h>
#include <stdio.h>
#include "processpil.h"
#include "windowhandle.h"
#include "datatypes.h"
#include "transcode.h"
#include "mplayerwrapper.h"
#include "paramlistdef.h"
#include "preference.h"

extern PFN_EVENT_HANDLER pfnEvent;
#define EVENT pfnEvent

extern "C" vo_functions_t video_out_dx;
extern "C" VO_INFO vo_info;
extern "C" HWND vo_parent;
extern "C" int vo_style;
extern "C" int vo_colorkey;
extern "C" int vo_border;
extern "C" int vo_ontop;

extern "C" {
void SetAddrPtr(vo_functions_t* vo);
uint32_t Directx_ManageDisplay();
}
extern "C" HWND hWndVideo;
static CMPlayer* me = 0;

extern "C" int vo_event(int code)
{
	switch (code) {
	case 0:
		me->state = STATE_IDLE;
		me->Close();
		break;
	}
	return 0;
}

int CMPlayer::Load()
{
	return 0;
	if (dll) return 0;
	if (!dll) dll = new CDllHelper;
	if (!dll) return -1;

	CString path;
	path.Format("%smplayer.dll", mppath.GetBuffer());
	dll->LoadDLL(path.GetBuffer());
	
	mp_send_command = (PFN_MP_SEND_CMD)dll->GetAddr("mp_send_command");
	mp_exit = (PFN_MP_EXIT)dll->GetAddr("exit_player");
	VO_INFO* p_vo_info = (VO_INFO*)dll->GetAddr("vo_info");
	if (!p_vo_info || !mp_send_command || !mp_exit) {
		delete dll;
		return -1;
	} else {
		memcpy(&vo_info, p_vo_info, sizeof(VO_INFO));
	}
	vo = (vo_functions_t*)dll->GetAddr("video_out_export");
	if (vo) {
		SetAddrPtr(vo);
	}
	return 0;
}

int CMPlayer::Unload()
{
	if (dll) {
		delete dll;
		dll = 0;
	}
	return 0;
}

DWORD WINAPI PlayThread(CMPlayer *player)
{
	Sleep(500);
	if (hWndVideo)
		ShowWindow(hWndVideo, SW_SHOW);
	player->state=STATE_PLAYING;
	ShellWait(&player->mpx,-1);
	ShellClean(&player->mpx);
	player->state=STATE_IDLE;
	player->Close();
	return 0;
}

int CMPlayer::Command(char* cmd)
{
	if (flags & MF_LOAD_DLL) {
		mp_send_command(cmd);
	} else {
#ifdef WIN32
		DWORD ret;
		CString cmdstr(cmd);
		cmdstr += '\n';
		if (!WriteFile((HANDLE)mpx.fdWrite,cmdstr.GetBuffer(),cmdstr.GetLength(),&ret,NULL)) return -1;
#else
		if (write(mpx.fdWrite,cmd,strlen(cmd)) <= 0) return -1;
		write(mpx.fdWrite,"\n",1);
#endif
	}
	return 0;
}

int CMPlayer::Read(TCHAR* buf, int bufsize)
{
	int bytes=0;
#ifdef WIN32
	int interval = 10;
	for (; (!PeekNamedPipe((HANDLE)mpx.fdRead,0,0,0,(LPDWORD)&bytes,0) || bytes == 0) && interval < 100; interval += 5) Sleep(interval);
	if (bytes == 0) return -1;
	ReadFile((HANDLE)mpx.fdRead,buf,bufsize-1,(LPDWORD)&bytes,NULL);
#else
	bytes=read(mpx.fdRead,buf,bufsize-1);
#endif
	*(buf+bytes)=0;
	return bytes;
}

int CMPlayer::Stop()
{
	if (state==STATE_PLAYING) {
		Command("pause\n");
		Command("quit\n");
		if (flags & MF_LOAD_DLL) {
			if (dll->Wait(1000) != WAIT_OBJECT_0) {
				dll->Stop();
			}
		} else {
			ShellWait(&mpx, 1000);
			ShellTerminate(&mpx);
			hThread=NULL;
		}
		state = STATE_IDLE;
		return 0;
	}
	return -1;
}

bool CMPlayer::onPosChange()
{
	Directx_ManageDisplay();
	return true;
}

void CMPlayer::RefreshVideo()
{
	if (flags & MF_LOAD_DLL) {
		Directx_ManageDisplay();
	} else {
		Command("refresh_vo\n");
	}
}

int CMPlayer::Play(char* filename, HWND hwndParent)
{
	DWORD id;
	CString cmd;

	playingfile = filename;
	if (state==STATE_PLAYING) {
		cmd.Format("loadfile \"%s\"\n", filename);
		Command(cmd.GetBuffer());
		return 0;
	}
	Load();
	memset(&mpx,0,sizeof(mpx));
	cmd.Format("\"%smplayer.%s\" \"%s\" -slave -quiet -colorkey %d ",
		mppath.GetBuffer(), flags & MF_LOAD_DLL ? "dll" : "exe", filename, COLORKEY);

	if (!options.IsEmpty()) {
		cmd += options;
	}

	if (flags & MF_NATIVE_WINDOW) {
		if (!hwnd) {
			WNDCLASS   wc;
			HBRUSH colorbrush = CreateSolidBrush(COLORKEY);
			memset(&wc, 0, sizeof(wc));
			wc.style         =  CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wc.hbrBackground =  colorbrush;
			wc.lpszClassName =  "MPlayerVideoWindow";
			vo_style = hwndParent?WS_CHILD:WS_OVERLAPPEDWINDOW|WS_SIZEBOX|WS_VISIBLE;
			hWndVideo = Create(vo_style,0,hwndParent, wc);
		}
		//cmd += " -nokeepaspect";
	} else if (hwnd) {
		Destroy();
		hWndVideo = 0;
	}
	if (!(flags & MF_LOAD_DLL)) {
		if (hWndVideo)
			cmd.AppendFormat(" -wid %d", hWndVideo);
		mpx.pchCommandLine=cmd.GetBuffer();
		mpx.flags=SHELL_REDIRECT_STDIN;
		EVENT(MC_MSG_CMD, mpx.pchCommandLine);
		if (ShellExec(&mpx)) {
			MessageBox(NULL,"MPlayer not found",NULL,MB_OK|MB_ICONINFORMATION);
			return -1;
		}
		if (mpx.fdRead) {
			DWORD bytes;
			while (PeekNamedPipe((HANDLE)mpx.fdRead, 0, 0, 0, &bytes, 0) && bytes == 0);
			{
				char buf[256];
				while (PeekNamedPipe((HANDLE)mpx.fdRead, 0, 0, 0, &bytes, 0) && bytes > 0) {
					ReadFile((HANDLE)mpx.fdRead, buf, sizeof(buf), &bytes, 0);
					Sleep(100);
				}
			}
		}
		if (fNoThread) return 0;
		hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PlayThread,(void*)this,0,&id);
	} else {
		//DeleteObject(colorbrush);
		if (Load()) return -1;

		if (hwndParent) vo_border = 0;
		vo_parent = hwndParent;
		vo_ontop = 1;
		vo_colorkey = COLORKEY;

		cmd += " -vo export";
		me = this;
		dll->StartMain(cmd.GetBuffer());
		//hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)WaitLaunchDone,(void*)this,0,&id);
		state = STATE_PLAYING;
		Directx_ManageDisplay();
	}
	return 0;
}

CMPlayer::~CMPlayer()
{
	if (dll) delete dll;
}

bool CMPlayer::onClose()
{
	if (state == STATE_PLAYING) {
		Stop();
		state=STATE_IDLE;
	}
	if (flags & MF_LOAD_DLL) {
		delete dll;
		dll = 0;
	}
	Destroy();
	hWndVideo = 0;
	return true;
}
