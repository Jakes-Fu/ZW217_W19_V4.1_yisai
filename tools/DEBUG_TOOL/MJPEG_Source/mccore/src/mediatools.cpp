/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include "datatypes.h"
#include "transcode.h"
#include "processpil.h"
#include "mediatools.h"
#include "common.h"
#include "preference.h"
#include "resource.h"
#ifdef _MEDIAINFO
#include "MediaInfoDLL.h"
#endif

CMediaTool::CMediaTool()
{
#ifdef _MEDIAINFO
	handle = MediaInfo_New();
	MediaInfo_Option(handle, "Internet", "No");
#endif
}

CMediaTool::~CMediaTool()
{
#ifdef _MEDIAINFO
	MediaInfoA_Delete(handle);
#endif
}

int CMediaInfo::Run(int action, T_ITEM* item)
{
#ifdef _MEDIAINFO
	if (!pref.GetValueBool(PI_MEDIAINFO)) return 0;

	// an workaround for MediaInfo MP3 crash bug
	char *p = strrchr(item->filename, '.');
	if (p && !stricmp(p + 1, "mp3")) {
		return 0;
	}

	CLogger logger;
	if (action == 0) {
		MediaInfo_Open(handle, item->filename);
		result = MediaInfo_Inform(handle, 0);
		MediaInfoA_Close(handle);
	} else if (action == 1) {
		const char *p;
		MediaInfo_Open(handle, item->filename);
		p = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, "Format", MediaInfo_Info_Text, MediaInfo_Info_Name);
		if (p && *p && !item->info.container) {
			item->info.container = strdup(p);
		}
		item->info.duration = atoi(MediaInfo_Get(handle, MediaInfo_Stream_General, 0, "PlayTime", MediaInfo_Info_Text, MediaInfo_Info_Name));
		item->info.audio.bitrate = atoi(MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, "BitRate", MediaInfo_Info_Text, MediaInfo_Info_Name));
		item->info.audio.samplerate = atoi(MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, "SamplingRate", MediaInfo_Info_Text, MediaInfo_Info_Name));
		item->info.audio.channels = atoi(MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, "Channel(s)", MediaInfo_Info_Text, MediaInfo_Info_Name));
		p = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, "Codec", MediaInfo_Info_Text, MediaInfo_Info_Name);
		if (p && *p && !item->info.audio.codec) {
			item->info.audio.codec = strdup(p);
		}
		p = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, "BitRate_Mode", MediaInfo_Info_Text, MediaInfo_Info_Name);
		item->info.audio.bitratemode = !strcmp(p, "CBR") ? 1 : (!strcmp(p, "VBR") ? 2 : 0);
		item->info.video.bitrate = atoi(MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, "BitRate", MediaInfo_Info_Text, MediaInfo_Info_Name));
		item->info.video.width = atoi(MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, "Width", MediaInfo_Info_Text, MediaInfo_Info_Name));
		item->info.video.height = atoi(MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, "Height", MediaInfo_Info_Text, MediaInfo_Info_Name));
		item->info.video.fps = (float)atof(MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, "FrameRate", MediaInfo_Info_Text, MediaInfo_Info_Name));
		p = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, "Codec", MediaInfo_Info_Text, MediaInfo_Info_Name);
		if (p && *p && !item->info.video.codec) {
			item->info.video.codec = strdup(p);
		}
		MediaInfoA_Close(handle);
	}
#endif
	return 0;
}

int CShibatch::Run(int action, T_ITEM* item)
{
	SHELL_PARAM sp;
	CString destfile;
	CString cmd;
	CLogger logger;
	destfile = item->filename;
	destfile += ".out";
	memset(&sp, 0, sizeof(sp));
	cmd.Format("\"%s\"", pref.GetValueStr(PI_SB_PATH));
	if (pref.GetValueBool(PI_SB_2PASS)) {
		cmd += " --twopass";
	}
	if (pref.GetValueInt(PI_SB_SAMPLE_RATE)) {
		CString rate = pref.GetValueEnum(PI_SB_SAMPLE_RATE);
		cmd += " --rate ";
		cmd += rate.Left(rate.Find("Hz"));
	}
	if (pref.GetValueInt(PI_SB_BITS)) {
		cmd += " --bits 16";
	}
	if (pref.GetValueInt(PI_SB_NORMAL)) {
		cmd += " --normalize";
	}
	if (pref.GetValueInt(PI_SB_DITHER) > 0) {
		cmd.AppendFormat(" --dither %d", pref.GetValueInt(PI_SB_DITHER) - 1);
	}
	cmd.AppendFormat(" \"%s\" \"%s\"", item->filename, destfile.GetBuffer());
	sp.flags = SHELL_REDIRECT_STDOUT | SHELL_ALLOC | SHELL_READ_STDOUT_ALL;
	sp.pchCommandLine = cmd.GetBuffer();
	pfnEvent(MC_MSG_CMD, sp.pchCommandLine);
	logger.Log(IDS_RUNNING, GetName());
	if (ShellExec(&sp)) return -1;
	ShellRead(&sp, 0);
	result = sp.buffer;
	ShellClean(&sp);
	if (FileExist(destfile.GetBuffer())) {
		if (!DeleteFile(item->filename) || !MoveFile(destfile.GetBuffer(), item->filename)) return -1;
		return 0;
	} else {
		return -1;
	}
}

#define MP_EXE "mplayer.exe"
#define MP_DLL "mplayer.dll"
#define MSG_STR "${length},${audio_bitrate},${samplerate},${channels},${video_bitrate},${width},${height},${fps}"

int CMPParser::Run(int action, T_ITEM* item)
{
	CString cmd;
	SHELL_PARAM sp;
	HANDLE hPipeRead, hPipeWrite;
	DWORD bytes;
	CDllHelper dll;
	int ret = 0;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 4096)) return -1;

	cmd.Format("\"%s%s\" \"%s\" -really-quiet -vo null -ao null -msg-pipe %d -parse-only 1 -playing-msg %s",
		pref.GetValueStr(PI_MP_PATH),
		pref.GetValueBool(PI_MP_DLL) ? MP_DLL : MP_EXE,
		item->filename,
		hPipeWrite,
		MSG_STR);

	if (pref.GetValueBool(PI_MP_DLL)) {
		if (!dll.LoadDLL(pref.GetValueStr(PI_MP_PATH), MP_DLL)) {
			ret = -1;
			goto exit;
		}
		if (dll.StartMain(cmd.GetBuffer())) {
			ret = -1;
			goto exit;
		}
	} else {
		memset(&sp, 0, sizeof(sp));
		sp.pchCommandLine = cmd.GetBuffer();
		if (ShellExec(&sp)) {
			ret = -1;
			goto exit;
		}
	}
	for (int i = 0; (!PeekNamedPipe(hPipeRead,0,0,0,&bytes,0) || bytes == 0) && i < 20; i++)
		Sleep(50);
	if (bytes == 0) {
		CloseHandle(hPipeRead);
		CloseHandle(hPipeWrite);
		return -1;
	}

	do {
		int size;
		int i;
		char* p;
		char* s;
		if (!ReadFile(hPipeRead, &size, sizeof(size), &bytes, 0)) break;
		p = (char*)malloc(size + 1);
		if (!ReadFile(hPipeRead, p, size, &bytes, 0) || bytes == 0) {
			free(p);
			break;
		}
		p[bytes] = 0;
		int* arg[] = {&item->info.duration, &item->info.audio.bitrate, &item->info.audio.samplerate, &item->info.audio.channels, &item->info.video.bitrate, &item->info.video.width, &item->info.video.height, 0};
		for (i = 0, s = strtok(p, ","); arg[i] && s; i++, s = strtok(NULL, ",")) {
			*arg[i] = atoi(s);
		}
		if (s) {
			item->info.video.fps = (float)atof(s);
		}
		item->info.audio.bitrate *= 8;
		item->info.video.bitrate *= 8;
		free(p);
	} while (0);
	if (pref.GetValueBool(PI_MP_DLL)) {
		dll.Wait(1000);
		dll.Unload();
	} else {
		ShellWait(&sp, 1000);
		ShellTerminate(&sp);
		ShellClean(&sp);
	}

exit:
	CloseHandle(hPipeRead);
	CloseHandle(hPipeWrite);
	return ret;
}