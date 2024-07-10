/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/
#include "mpvo.h"

typedef int (*PFN_MP_SEND_CMD)(char* cmd);
typedef void (*PFN_MP_EXIT)(char* how);

#define COLORKEY 0x00000010

#define MF_LOAD_DLL 1
#define MF_NATIVE_WINDOW 2

class CMPlayer : public CWindow
{
public:
	CMPlayer():fNoThread(false),dll(0),flags(0) { mppath = "mplayer"; }
	~CMPlayer();
	int Load();
	int Unload();
	int Play(char* filename, HWND hwndParent);
	int Stop();
	int Command(char* cmd);
	int Read(char* buf, int bufsize);
	void RefreshVideo();
	SHELL_PARAM mpx;
	int state;
	int flags;
	bool fNoThread;
	CString options;
	CString mppath;
	CString playingfile;
private:
	CDllHelper* dll;
	vo_functions_t* vo;
	static void WaitLaunchDone(void* ptrme);
	bool onPosChange();
	bool onClose();
	HANDLE hThread;
	PFN_MP_SEND_CMD mp_send_command;
	PFN_MP_EXIT mp_exit;
};
