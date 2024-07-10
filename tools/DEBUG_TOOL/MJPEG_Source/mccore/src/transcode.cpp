/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
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
#include <windows.h>
#include <time.h>
#include "resource.h"
#include "processpil.h"
#include "paramlist.h"
#include "common.h"
#include "transcode.h"
#include "preference.h"
#include "mplayerwrapper.h"
#include "cpuinfo.h"
#include "audioencoders.h"
#include "videoencoders.h"
#include "audiosources.h"
#include "cfilename.h"
#include "audiofilters.h"
#include "containers.h"
#include "mediatools.h"

#ifdef _HTTPD
#include "httpd.h"
#endif
#include "revision.h"

#define NOW GetTickCount()

CAudioEncoder* AudioEncoders[AC_COUNT];
CAudioSource* AudioSources[AUDIO_SRC_COUNT];
CVideoEncoder* VideoEncoders[VE_COUNT];
CContainer* Containers[CF_COUNT];
CONTAINER_TRACKS tracks[MAX_TRACKS];

CAudioSource* audiosrc;
CAudioEncoder* audioenc;
CVideoEncoder* videoenc;
CContainer* container;
#ifdef _HTTPD
CHttpd* httpd = 0;
#endif
CMPlayer player;
MC_STATE inst;
MC_INST_INTERNAL mcinst;

extern HINSTANCE hInst;
extern T_ITEM itemqueue;
extern CVideoEncoder* VideoEncoders[];
extern PARAM_DATA prefParams[];
extern PARAM_DESC prefInfo[];
#ifdef _USRDLL
extern HINSTANCE hDllInst;
#endif

int DefaultEventHandler(MC_EVENT event, void* arg);
static int CheckFileExistence(TCHAR* filename, TASK_STATS* stats);
static int GenCommandLines();

int InitCDInterface( int iInterface);
void UninitCDInterface( void );
int LoadParamNames(PARAM_DATA* params, TCHAR* filename);

PFN_EVENT_HANDLER pfnEvent = &DefaultEventHandler;

static int timerlevel = 0;
static UINT_PTR timer;

int curEvent;
void* curArg;
TCHAR cmd[MAX_PASS_COUNT][1024];
TCHAR* cfgfile;

TCHAR* CDefContainer::GetOutExt()
{
	return (mcinst.fAudioEncoding) ? audioenc->GetOutExt() : videoenc->GetOutExt();
}

static int DefaultEventHandler(MC_EVENT event, void* arg)
{
	curEvent = event;
	curArg = arg;
	SetEvent(mcinst.hEvent);
	return 0;
}

void PopupMsg(int uid)
{
	CString msg;
	msg.LoadString(uid);
	pfnEvent(MC_MSG_POPUP, msg.GetBuffer());
}

void GetAppPath(CString& str)
{
	TCHAR buf[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), buf, sizeof(buf));
	str = buf;
	int i = str.ReverseFind('\\');
	if (i != -1) {
		i++;
		str.Delete(i, str.GetLength() - i);
	}
}

FILE_TYPE filetypes[]={
	{"mp3", FT_MP3, "MPEG 1.0 Layer 3", CT_AUDIO},
	{"mp4", FT_MP4, "MPEG4 Audio/Video", CT_VIDEO},
	{"m4a", FT_M4A, "MPEG4 Audio", CT_AUDIO},
	{"3gp", FT_3GP, "3GPP File", CT_VIDEO},
	{"3g2", FT_3GP, "3GPP2 File", CT_VIDEO},
	{"ogg", FT_OGG, "Ogg/Vorbis", CT_AUDIO},
	{"aac", FT_AAC, "MPEG4 AAC", CT_AUDIO},
	{"mp2", FT_MPA, "MPEG 1.0 Layer 2", CT_AUDIO},
	{"mpa", FT_MPA, "MPEG Audio", CT_AUDIO},
	{"wav", FT_WAV, "Waveform", CT_AUDIO},
	{"pcm", FT_WAV, "Raw PCM", CT_AUDIO},
	{"ra", FT_RA, "RealAudio", CT_AUDIO},
	{"ram", FT_RA, "RealAudio", CT_AUDIO},
	{"amr", FT_AMR, "Raw AMR Stream", CT_AUDIO},
	{"mpc", FT_MPC, "MusePack", CT_AUDIO},
	{"wma", FT_WMA, "Windows Media Audio", CT_AUDIO},
	{"ape", FT_APE, "Monkey's Audio", CT_AUDIO},
	{"apl", FT_APE, "Monkey's Audio", CT_AUDIO},
	{"flac", FT_FLAC, "FLAC Audio", CT_AUDIO},
	{"wv", FT_WV, "WavPack Audio", CT_AUDIO},
	{"cda", FT_CDA, "CD Audio Track", CT_AUDIO},
	{"mpg", FT_MPEG_PS, "MPEG Video", CT_VIDEO},
	{"mpeg", FT_MPEG_PS, "MPEG Video", CT_VIDEO},
	{"mpv", FT_MPEG_PS, "MPEG Video", CT_VIDEO},
	{"mka", FT_MKA, "Matroska Audio", CT_AUDIO},
	{"mkv", FT_MKV, "Matroska Video", CT_VIDEO},
	{"avi", FT_AVI, "Audio Video Interleave", CT_VIDEO},
	{"dat", FT_MPEG_PS, "Video CD Track", CT_VIDEO},
	{"rm", FT_RM, "RealMedia", CT_VIDEO},
	{"rmvb", FT_RM, "RealMedia (VBR)", CT_VIDEO},
	{"mov", FT_MOV, "Quick Time", CT_VIDEO},
	{"wmv", FT_WMV, "Windows Media Video", CT_VIDEO},
	{"asf", FT_WMV, "Windows Media Stream", CT_VIDEO},
	{"vob", FT_MPEG_PS, "DVD/MPEG2", CT_VIDEO},
	{"tp", FT_MPEG_TS, "MPEG2 TS", CT_VIDEO},
	{"ts", FT_MPEG_TS, "MPEG2 TS", CT_VIDEO},
	{"263", FT_263, "Raw H.263 Stream", CT_VIDEO},
	{"264", FT_264, "Raw H.264 Stream", CT_VIDEO},
	{"idx", FT_IDX, "Subtitle", CT_SUBTITLE},
	{"sub", FT_SUB, "Subtitle", CT_SUBTITLE},
	{"cue", FT_CUE, "CUE Sheet", CT_CUE},
	{"m3u", FT_M3U, "CUE Sheet", CT_QUEUE},
	{0},
};

FILE_TYPE ftDVDTrack = {0, FT_MPEG_PS, "DVD Track", CT_VIDEO};
FILE_TYPE ftVCDTrack = {0, FT_MPEG_PS, "Video CD Track", CT_VIDEO};
FILE_TYPE ftCDTrack = {0, FT_CDA, "Audio Track", CT_AUDIO};
FILE_TYPE ftUnknown = {"", FT_UNKNOWN, "Unknown Format", CT_UNKNOWN};

FILE_TYPE* mcGetFileTypeByIndex(int index)
{
	return &filetypes[index];
}

FILE_TYPE* mcIdentifyFileType(TCHAR *filename)
{
	TCHAR ch;
	int i;
	TCHAR *p;
	TCHAR *ext;
	if (strstr(filename, "://")) {
		switch (*(LPDWORD)filename) {
		case ':dvd':
			return &ftDVDTrack;
		case ':dvc':
			return &ftVCDTrack;
		default:
			return &ftCDTrack;
		}
	}
	p = strrchr(filename,'.');
	if (!p) return &ftUnknown;
	ext = strdup(p + 1);
	for (p = ext; (ch = *p); p++) {
		if (ch>='A' && ch<='Z') *p = ch+('a'-'A');
	}
	for (i = 0; filetypes[i].fileext; i++) {
		if (!strcmp(ext, filetypes[i].fileext)) {
			free(ext);
			return &filetypes[i];
		}
	}
	free(ext);
	return &ftUnknown;
}

void PurgeTags(TAGS* tags)
{
	if (tags->title) {
		free(tags->title);
		tags->title = 0;
	}
	if (tags->artist) {
		free(tags->artist);
		tags->artist = 0;
	}
	if (tags->album) {
		free(tags->album);
		tags->album = 0;
	}
	if (tags->genre) {
		free(tags->genre);
        tags->genre = 0;
	}
	if (tags->comment) {
		free(tags->comment);
		tags->comment = 0;
	}
	tags->year = 0;
	tags->valid = 0;
}

int ReadFileTags(T_ITEM* item)
{
	CString cmd;
	SHELL_PARAM sp;
	int bytes;
	if (item->info.filetype->type != CT_AUDIO) return -1;
	PurgeTags(&item->info.tags);
	cmd = "tools\\";
	{
		FILE_TYPE* ft = mcIdentifyFileType(item->filename);
		if (ft && ft->fileext && (!strcmp(ft->fileext, "mp4") || !strcmp(ft->fileext, "m4a"))) {
			cmd += "mp4-tg.exe ";
		} else {
			cmd += "Tag.exe ";
		}
	}
	cmd += '\"';
	cmd += item->filename;
	cmd += '\"';
	memset(&sp,0,sizeof(sp));
	sp.pchCommandLine = cmd.GetBuffer();
	sp.flags = SHELL_REDIRECT_STDOUT | SHELL_ALLOC | SHELL_READ_STDOUT_ALL;
	if (ShellExec(&sp)) return -1;
	bytes = ShellRead(&sp, 1000);
	if (bytes > 0) {
		TCHAR *p,*v;
		item->info.tags.valid=0;
		strtok(sp.buffer,"\n");
		while ( (p=strtok(0,":")) && (v=strtok(0,"\n")) ) {
			TCHAR *t;
			while (*v==' ') v++;
			t=strchr(v,0);
			if ((t--) && *t=='\r') *t=0;
			if (!*v) continue;
			if (!strcmp(p,"Title"))
				item->info.tags.title = strdup(v);
			if (!strcmp(p,"Artist"))
				item->info.tags.artist = strdup(v);
			else if (!strcmp(p,"Album"))
				item->info.tags.album = strdup(v);
			else if (!strcmp(p,"Year"))
				item->info.tags.year = atoi(v);
			else if (!strcmp(p,"Track"))
				item->info.tags.track  =atoi(v);
			//else if (!strcmp(p,"Genre"))
			//	item->info.tags.genre= strdup(v);
			else if (!strcmp(p,"Comment"))
				item->info.tags.comment = strdup(v);
			else
				continue;
			item->info.tags.valid = 1;
		}
	}
	ShellClean(&sp);
	return 0;
}

int WriteFileTags(TCHAR* destfile, T_ITEM* item)
{
	SHELL_PARAM sp;
	CString cmd;
	FILE_TYPE* ft;

	if (!item->info.tags.valid) return 0;
	ft = mcIdentifyFileType(destfile);
	cmd = "tools\\";
	if (!strcmp(ft->fileext, "mp4") || !strcmp(ft->fileext, "m4a")) {
		cmd += "mp4-tg.exe";
	} else {
		cmd += "Tag.exe";
	}
	if (item->info.tags.title) {
		cmd.AppendFormat(" --title \"%s\"", item->info.tags.title);
	}
	if (item->info.tags.artist) {
		cmd.AppendFormat(" --artist \"%s\"", item->info.tags.artist);
	}
	if (item->info.tags.album) {
		cmd.AppendFormat(" --album \"%s\"", item->info.tags.album);
	}
	if (item->info.tags.track) {
		cmd.AppendFormat(" --track %d", item->info.tags.track);
	}
	if (item->info.tags.year) {
		cmd.AppendFormat(" --year %d", item->info.tags.year);
	}
	if (item->info.tags.genre) {
		cmd.AppendFormat(" --genre \"%s\"", item->info.tags.genre);
	}
	if (item->info.tags.comment) {
		cmd.AppendFormat(" --comment \"%s\"", item->info.tags.comment);
	}
	cmd.AppendFormat(" \"%s\"", destfile);
	memset(&sp,0,sizeof(sp));
	sp.pchCommandLine = cmd.GetBuffer();
	pfnEvent(MC_MSG_CMD, sp.pchCommandLine);
	if (ShellExec(&sp)) return -1;
	ShellWait(&sp,-1);
	ShellClean(&sp);
	return 0;
}

TCHAR* GetEncoderVersionInfo(TCHAR* path, TCHAR* options, TCHAR* delimiter)
{
	static CString verinfo;
	SHELL_PARAM sp;
	memset(&sp,0,sizeof(SHELL_PARAM));
	sp.flags = SHELL_REDIRECT_STDOUT | SHELL_READ_STDOUT_ALL | SHELL_ALLOC;
	verinfo.Format("\"%s\"", path);
	if (options) {
		verinfo += ' ';
		verinfo += options;
	}
	sp.pchCommandLine = verinfo.GetBuffer();
	if (!ShellExec(&sp) && ShellRead(&sp, 3000) > 0) {
		char *p = 0;
		if (delimiter) p = strstr(sp.buffer, delimiter);
		if (!p) p = strstr(sp.buffer, "\r\n\r\n");
		if (!p) p = strstr(sp.buffer, "\n\n");
		if (p) *p = 0;
		verinfo = sp.buffer;
		ShellClean(&sp);
		return verinfo.GetBuffer();
	} else {
		verinfo = (TCHAR*)0;
		ShellClean(&sp);
		return 0;
	}
}

void InitRiffHeader(RIFF_HEADER *hdr)
{
	hdr->desc.chunkID='FFIR';
	hdr->desc.chunkSize=0x7ffff024;
	hdr->desc.format='EVAW';
	hdr->fmt.chunkID=' tmf';
	hdr->fmt.chunkSize=16;
	hdr->fmt.wAudioFormat=1;
	hdr->fmt.wChannels=2;
	hdr->fmt.dwSamplesPerSec=44100;
	hdr->fmt.dwAvgBytesPerSec=44100*2*2;
	hdr->fmt.wBlockAlign=2*2;
	hdr->fmt.wBitsPerSample=16;
	hdr->data.chunkID='atad';
	hdr->data.chunkSize=0x7ffff000;
}

void InvalidiateRiffHeader(RIFF_HEADER *hdr)
{
	hdr->fmt.wBlockAlign = hdr->fmt.wBitsPerSample * hdr->fmt.wChannels / 8;
	hdr->fmt.dwAvgBytesPerSec = hdr->fmt.dwSamplesPerSec * hdr->fmt.wBlockAlign;
}

int mcGetEvent(void** parg)
{
	if (WaitForSingleObject(mcinst.hEvent, INFINITE) == WAIT_OBJECT_0) {
		if (parg) *parg = curArg;
		return curEvent;
	}
	return -1;
}

TCHAR* mcGetLogText(int maxsize)
{
	return CLogger::GetLog(maxsize);
}

void mcSetLogText(TCHAR* logtext)
{
	CLogger::SetLog(logtext);
}

int mcPostInit(HWND hwndMain)
{
	CString msg;
	CLogger logger;
	
	InitCDInterface(2);

	if (mcGetPrefValueInt(PI_LOGGING))
		CLogger::Init(mcGetPrefValueStr(PI_LOGFILE));
	else
		CLogger::Init();
	
#ifdef _HTTPD
	// start http deamon if enabled
	if (pref.GetValueBool(PI_HTTPD)) {
		WORD port = pref.GetValueInt(PI_HTTPD_PORT);
		httpd = new CHttpd;
		httpd->SetPort(port);
		msg.Format("Starting httpd on port %d", port);
		logger.Log(msg);
		httpd->Start();
	}
#endif
	logger.Log(IDS_SCAN_PLUGINS);
	mcinst.hwndMain = hwndMain;
	inst.cpucaps.freq = MeasureCPUSpeed(0);
	logger.Log("Initialized");
	return 0;
}

void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	switch (timerlevel) {
	case 0:
		return;
	case 1:
		(*pfnEvent)(MC_UPDATE_TIME, 0);
		break;
	case 2:
		mcInvalidiateStatsData();
		(*pfnEvent)(MC_UPDATE_STATS, 0);
		break;
	}
}

void* mcCreateObject(MC_TOOLS type, int index)
{
	switch (type) {
	case TL_TOOL:
		switch (index) {
		case MT_MEDIAINFO:
			return (new CMediaInfo);
		case MT_SHIBATCH:
			return (new CShibatch);
		}
		break;
	}
	return 0;
}

void mcDestroyObject(MC_TOOLS type, int index, void* ptr)
{
	switch (type) {
	case TL_TOOL:
		switch (index) {
		case MT_MEDIAINFO:
			delete (CMediaInfo*)ptr;
			break;
		case MT_SHIBATCH:
			delete (CShibatch*)ptr;
			break;
		}
		break;
	}
}

MC_STATE* mcInit(TCHAR* pchLangFile, PFN_EVENT_HANDLER pfnEventCallback)
{
	TCHAR *p;
	int i;
	int enumlen = 0;

	memset(&inst, 0, sizeof(MC_STATE));
	MeasureCPUSpeed(1);
	GetCpuCaps(&inst.cpucaps);

	{
		HDC hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
		mcinst.scrw = GetDeviceCaps(hScrDC, HORZRES);
		mcinst.scrh = GetDeviceCaps(hScrDC, VERTRES);
		DeleteDC(hScrDC);
	}

	//modify preference default values
	{
		int i, len = 0;
		for (i = 0; filetypes[i].fileext; i++) {
			len += strlen(filetypes[i].fileext) + 1;
		}
		TCHAR *p = (TCHAR*)malloc(len + 1);
		prefParams[PI_FILE_TYPES].valueDefault = (void*)p;
		for (i = 0; filetypes[i].fileext; i++) {
			p += sprintf(p, "%s;", filetypes[i].fileext);
		}
		*(p - 1) = 0;
	}
	{
		TCHAR buf[MAX_PATH];
		GetTempPath(sizeof(buf), buf);
        prefParams[PI_TEMP_DIR].valueDefault = strdup(buf);
	}
	if (pchLangFile) LoadParamNames(prefParams, pchLangFile);
	InitParams(prefParams);

	if (pfnEventCallback) pfnEvent = pfnEventCallback;

	mcinst.hEvent = CreateEvent(0, FALSE, FALSE, 0);

	inst.state=STATE_IDLE;
	GetAppPath(mcinst.apppath);

	memset(&itemqueue, 0, sizeof(T_ITEM));
	mcinst.item = &itemqueue;

	// create audio encoder objects
	enumlen = 0;
	//@ Liu Kai
	// 
	//for (i = 0; i < AC_COUNT; i++) {
	int j=0;
	for( i = 0; i < AC_COUNT; i++ )
	{
		CAudioEncoder* aenc;
		switch (i) {
		case AC_FAAC:		/*aenc = new CFaac; break;*/ continue;
		case AC_PCM:		aenc = new CPcm; break;
		case AC_COPY:		/*aenc = new CAudioCopy; break;*/ continue;
		case AC_NONE:		aenc = new CAudioNone; break;
		case AC_AMR:		aenc = new CAmr; break;
		default:
			return 0;
		}
		//aenc->UpdateSettings();
		//AudioEncoders[i] = aenc;
		AudioEncoders[j] = aenc;
		j++;
		enumlen += strlen(aenc->GetFullName()) + 1;
	}
	prefParams[PI_AENC].pchEnums = (TCHAR*)malloc(enumlen + 1);
	p = (TCHAR*)prefParams[PI_AENC].pchEnums;
	//for (i = 0; i < AC_COUNT; i++) {
	for (i = 0; i< j; i++) {
		int len = strlen(AudioEncoders[i]->GetFullName()) + 1;
		memcpy(p, AudioEncoders[i]->GetFullName(), len);
		p += len;
	}
	*p = 0;

	// create audio source objects
	enumlen = 0;
	for (i = 0; i<AS_COUNT; i++) {
		CAudioSource* asrc;
		switch (i) {
		case AS_MPLAYER:	asrc = (CAudioSource*) new CMplayerSource; break;
		case AS_WAVE:		asrc = (CAudioSource*) new CWaveFileSource; break;
		//case AS_AMR:		asrc = (CAudioSource*) new CAmrDecoder; break;
		default:
			return 0;
		}
		AudioSources[i] = asrc;
		enumlen += strlen(asrc->GetName()) + 1;
	}
	prefParams[PI_ADEC].pchEnums = (TCHAR*)calloc(enumlen + 1, 1);
	enumlen = 0;
	for (i = 0; i < AS_COUNT; i ++) {
		int len = strlen(AudioSources[i]->GetName()) + 1;
		memcpy(prefParams[PI_ADEC].pchEnums + enumlen, AudioSources[i]->GetName(), len);
		enumlen += len;
	}

	// create video encoder objects
	enumlen = 0;
	for (i = 0; i < VE_COUNT; i ++) {
		CVideoEncoder* venc;
		switch (i) {
		case VE_MENCODER:	venc = new CMencoder; break;
		default:
			return 0;
		}
		VideoEncoders[i] = venc;
		enumlen += strlen(venc->GetName()) + 1;
	}
	p = (TCHAR*)malloc(enumlen + 1);
	prefParams[PI_VENC].pchEnums = p;
	for (i = 0; i < VE_COUNT; i ++) {
		int len = strlen(VideoEncoders[i]->GetName()) + 1;
		memcpy(p, VideoEncoders[i]->GetName(), len);
		p += len;
	}
	*p = 0;

	// create container objects
	enumlen = 0;
	for (i = 0; i < CF_COUNT; i ++) {
		CContainer* cf;
		switch (i) {
		case CF_DEFAULT:	cf = new CDefContainer; break;
		case CF_MP4:		cf = new CMp4box;		break;
		case CF_SPMP4:		cf = new CSpMp4;		break;
		default:
			return 0;
		}
		Containers[i] = cf;
		enumlen += strlen(cf->GetName()) + 1;
	}
	p = (TCHAR*)malloc(enumlen + 1);
	prefParams[PI_CONTAINER].pchEnums = p;
	for (i = 0; i < CF_COUNT; i ++) {
		int len = strlen(Containers[i]->GetName()) + 1;
		memcpy(p, Containers[i]->GetName(), len);
		p += len;
	}
	*p = 0;

	timer = SetTimer(0, 0, 1000, TimerProc);
	return &inst;
}

void mcUninit()
{
	CLogger logger("Uninit");
	mcPlayerStop();
	KillTimer(0, timer);
#ifdef _HTTPD
	if (httpd) {
		logger.Log("Stopping httpd");
		httpd->Stop();
		delete httpd;
		httpd = 0;
	}
#endif
	CloseHandle(mcinst.hEvent);
	UninitCDInterface();
	CLogger::Uninit();
}

int mcLoadParam(TCHAR* cfgfile, int fInit)
{
	if (fInit) {
		InitParams(prefParams);
	}
	if (!FileExist(cfgfile)) return -1;
	return LoadParams(prefParams, cfgfile) ? -1 : 0;
}

int mcSaveParam(TCHAR* cfgfile)
{
	if (SaveParams(prefParams,cfgfile)) {
		return -1;
	} else {
		return 0;
	}
}

void mcApplyParam()
{
	mcinst.item->target.audioformat = (AUDIO_CODEC)pref.GetValueInt(PI_AENC);
	mcinst.item->target.videoenc = (VIDEO_ENCODER)pref.GetValueInt(PI_VENC);
	mcinst.item->target.container = (CONTAINER_FORMAT)pref.GetValueInt(PI_CONTAINER);
	mcinst.item->target.videoformat = (VIDEO_CODEC)pref.GetValueInt(PI_VCODEC);
	VideoEncoders[mcinst.item->target.videoenc]->UpdateSettings();
	int iaf = mcinst.item->target.audioformat;
	if (pref.GetValueBool(PI_QUIET)) {
		AudioEncoders[iaf]->flags |= FLAG_QUIET;
	} else {
		AudioEncoders[iaf]->flags &= ~FLAG_QUIET;
	}
	AudioEncoders[iaf]->UpdateSettings();
}

void SetThreadPriorityBySetting(HANDLE hThread)
{
	static int iPriorities[] = {THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL};
	int i = pref.GetValueInt(PI_PRIORITY);
	SetThreadPriority(hThread, iPriorities[i < 4 ? i : 3]);
}

void SetProcessPriorityBySetting(HANDLE hProcess)
{
#ifdef CONFIG_WIN9X
	static int iPriorities[] = {IDLE_PRIORITY_CLASS, IDLE_PRIORITY_CLASS, IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS};
#else
	static int iPriorities[] = {IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS};
#endif
	int i = pref.GetValueInt(PI_PRIORITY);
	SetPriorityClass(hProcess, iPriorities[i < 4 ? i : 3]);
}

void GetVariable(TCHAR* varNameString, CString& str)
{
	int i;
	CString varname = varNameString;
	CString value;

	switch (varname.GetAt(0)) {
	case 'D':
		if (varname == "DestFile") {
			bool fFormattedName = false;
			// (DestFile)
			if ((mcinst.fNeedWrap && inst.stage != ST_MUXING) || inst.stage == ST_EXTRACTING_PCM) {
				GetVariable("TempFile", value);
			} else if (*pref.GetValueStr(PI_DEST_DIR)) {
				value += pref.GetValueStr(PI_DEST_DIR);
				fFormattedName = (pref.GetValueBool(PI_FORMATTED_NAME) && mcinst.item->info.tags.title);
				if (fFormattedName) {
					QualifyVariables(value, pref.GetValueStr(PI_NAME_FORMAT));
				} else {
					if (strstr(mcinst.item->filename, "://")) {
						GetVariable("SourceFileName", value);
					} else {
						CFileName fn;
						fn = mcinst.item->filename + mcinst.commonPartLen;
						if (pref.GetValueBool(PI_KEEP_DIR)) {
							fn.RemoveDriveLetter();
						} else {
							fn.RemovePathName();
						}
						fn.RemoveExtName();
						value += fn.FileName;
					}
				}
			} else {
				CFileName fn;
				fn = mcinst.item->filename;
				fn.RemoveExtName();
				value += fn.FileName;
			}
			
			// add file name postfix
			if (!fFormattedName) {
				if (mcinst.item->info.track)
					value.AppendFormat(".track%d", mcinst.item->info.track);
				if (mcinst.item->info.chapter)
					value.AppendFormat(".chapter%d", mcinst.item->info.chapter);
			}

			//add file extension
			value += '.';
			switch (inst.stage) {
			case ST_EXTRACTING_PCM:
				value += AudioEncoders[AC_PCM]->GetOutExt();
				break;
			case ST_PROCESSING_AUDIO:
				if (pref.GetValueBool(PI_ALL_AUDIO))
					value.AppendFormat("aid%d.", mcinst.audiochannel);
				else if ((i = pref.GetValueInt(PI_DOWNMIX)))
					value.AppendFormat("%c-ch.", i == 1 ? 'l': ( i == 2 ? 'r' : 'm'));
				value += audioenc->GetOutExt();
				break;
			case ST_PROCESSING_VIDEO:
				if (container->IsExternalMuxer())
					value += videoenc->GetOutExt();
				else
					value += container->GetOutExt();
				break;
			case ST_MUXING:
				value += container->GetOutExt();
				break;
			}
			if (value == mcinst.item->filename) {
				i = value.ReverseFind('.');
				value.Insert(i, "_transcoded");
			}
			value.SpanExcluding(";:/|#%\\");
		} else if (varname == "DestDir") {
			value += pref.GetValueStr(PI_DEST_DIR);
		}
		break;
	case 'S':
		if (varname == "SourceFile") {
			if (inst.stage == ST_PROCESSING_AUDIO) {
				if (mcinst.fUseWaveFile) {
					GetVariable("TempFile", value);
					value.AppendFormat(".%s", AudioEncoders[AC_PCM]->GetOutExt());
				} else if (audioenc->flags & FLAG_PIPE) {
					value.AppendFormat("\\\\.\\pipe\\%s%d.wav", OUTPIPENAME, GetCurrentProcessId());
				} else if (!(audioenc->flags & FLAG_STANDALONE)) {
					value += '-';
				} else {
					value += mcinst.item->filename;
				}
			} else {
				value += mcinst.item->filename;
			}
		} else if (varname == "SourceFileName") {
			CFileName srcfile;
			srcfile = mcinst.item->filename;
			int i = srcfile.FileName.Find("://");
			if (i >= 0) {
				value += srcfile.FileName.Left(i) + '-' + srcfile.FileName.Mid(i + 3);
			} else {
				srcfile.GetName(value);
			}
		} else if (varname == "SampleRate") {
			value.AppendFormat("%d", mcinst.wavhdr.fmt.dwSamplesPerSec);
		} else if (varname == "SampleRateKhz") {
			value.AppendFormat("%d", mcinst.wavhdr.fmt.dwSamplesPerSec / 1000);
		} else if (varname == "SampleBits") {
			value.AppendFormat("%d", mcinst.wavhdr.fmt.wBitsPerSample);
		}
		break;
	case 'T':
		if (varname == "TempFile") {
			value += pref.GetValueStr(PI_TEMP_DIR);
			GetVariable("SourceFileName", value);
		} else if (varname == "TempDir") {
			value += pref.GetValueStr(PI_TEMP_DIR);
		} else if (varname == "Title") {
			value += mcinst.item->info.tags.title ? mcinst.item->info.tags.title : "Unknown title";
		} else if (varname == "Track") {
			if (mcinst.item->info.tags.track) 
				value.AppendFormat("%d",mcinst.item->info.tags.track);
		}
		break;
	case 'V':
		if (varname == "VideoBitrate") {
			if (pref.GetValueBool(PI_AUTO_BITRATE) && mcinst.bitrate)
				value.AppendFormat("%d", (mcinst.bitrate/2/4)+(pref.GetValueInt(PI_VBITRATE)*3/4));
			else
				value.AppendFormat("%d", pref.GetValueInt(PI_VBITRATE));
		}
		break;
	case 'C':
		if (varname == "ChannelNum") {
			value.AppendFormat("%d", mcinst.wavhdr.fmt.wChannels);
		} else if (varname == "Comment") {
			value += mcinst.item->info.tags.comment;
		}
		break;
	case 'A':
		if (varname == "AudioFiles") {
			for (i = 0; i<MAX_TRACKS; i++) {
				if (tracks[i].filename && tracks[i].type == CT_AUDIO) {
					value += tracks[i].filename;
					value += ' ';
				}
			}
			value.Delete(value.GetLength() - 1);
		} else if (varname == "AppPath") {
			value += mcinst.apppath;
		} else if (varname == "Artist") {
			value += mcinst.item->info.tags.artist ? mcinst.item->info.tags.artist : "Unknown artist";
		} else if (varname == "Album") {
			value += mcinst.item->info.tags.album;
		}
		break;
	case 'G':
		if (varname == "Genre") {
			value += mcinst.item->info.tags.genre;
		}
		break;
	case 'Y':
		if (varname == "Year") {
			if (mcinst.item->info.tags.year)
				value.AppendFormat("%d",mcinst.item->info.tags.year);
		}
		break;
	case 'N':
		if (varname.Left(3) == "Now") {
			CString subname = varname.Mid(3);
			time_t now = time(0);
			struct tm *btm = localtime(&now);
			int n = -1;
			if (subname == "Year") {
				value.AppendFormat("%04d", 1900+btm->tm_year);
			} else if (subname == "Month") {
				n = btm->tm_mon;
			} else if (subname == "Day") {
				n = btm->tm_mday;
			} else if (subname == "WeekDay") {
				n = btm->tm_wday;
			} else if (subname == "Hour") {
				n = btm->tm_hour;
			} else if (subname == "Min") {
				n = btm->tm_min;
			} else if (subname == "Sec") {
				n = btm->tm_sec;
			} else {
				n = -1;
			}
			if (n >= 0) {
				value.AppendFormat("%02d", n);
			}
		}
		break;
	}
	if (!value.IsEmpty()) str += value;
}

int EncodeAudioData(char* buf, int len)
{
	int bytes = len;
	MC_AUDIO_FILTER *af;
	// filter audio data if needed
	for (af = mcinst.audiofilter; af; af = af->next) {
		bytes = (*mcinst.audiofilter->pfnAudioDsp)((short*)buf, len >> 1) << 1;
	}

	// send to encoder
	bytes = audioenc->Encode(buf, bytes);
	mcinst.wavhdr.data.chunkSize += bytes;
	return bytes;
}

static int WaitAudioEncoder()
{
	while (inst.state==STATE_ENCODING && 
		(inst.astats.percent = (float)audioenc->GetProgress()) != -1);
	pfnEvent(MC_UPDATE_PROGRESS, (void*)100);
	return 0;
}

static int StartAudioEncoder()
{
	CString str;
	CLogger logger;
	if (mcinst.audiofilter) {
		MC_AUDIO_FILTER* af = mcinst.audiofilter;
		if (af->channels)
			mcinst.wavhdr.fmt.wChannels = af->channels;
		if (af->samplerate)
			mcinst.wavhdr.fmt.dwSamplesPerSec = af->samplerate;
		if (af->bps)
			mcinst.wavhdr.fmt.wBitsPerSample = af->bps;
		mcinst.wavhdr.fmt.dwAvgBytesPerSec = mcinst.wavhdr.fmt.dwSamplesPerSec * mcinst.wavhdr.fmt.wBitsPerSample / 8;
	}
	if (mcinst.wavhdr.fmt.dwSamplesPerSec < (DWORD)audioenc->GetLowestSampleRate() ||
		mcinst.wavhdr.fmt.dwSamplesPerSec > (DWORD)audioenc->GetHighestSampleRate()) {
		logger.Log(IDS_INVALID_SAMPLE_RATE, audioenc->GetFullName());
		return -1;
	}
	str.Format("\nAudio encoder: %s\nSample Rate: %dHz Channels: %d BPS: %d\n\n",
		audioenc->GetFullName(), mcinst.wavhdr.fmt.dwSamplesPerSec, mcinst.wavhdr.fmt.wChannels, mcinst.wavhdr.fmt.wBitsPerSample);
	pfnEvent(MC_MSG_INFO, str.GetBuffer());
	str = "";
	logger.Log(IDS_APPLY_SETTINGS, audioenc->GetFullName());
	QualifyVariables(str, audioenc->cl.GetBuffer());
	logger.Log(IDS_LOAD_AUDIO_ENCODER, audioenc->GetFullName());
	if (audioenc->OpenEncoder(str, mcinst.destfile, &mcinst.wavhdr)) {
		logger.Log(IDS_ERROR_LOADING, audioenc->GetFullName());
		return -1;
	}
	if (!(audioenc->flags & FLAG_RAW_PCM))
		audioenc->Encode((TCHAR*)&mcinst.wavhdr,sizeof(RIFF_HEADER));
	mcinst.wavhdr.data.chunkSize = 0;
	return 0;
}

static void AudioDataPump()
{
	int bytesOut = 0;
	int bytesIn;

	if (mcinst.audioDelay > 0) {
		int padbytes = (int)MS2BYTE(mcinst.audioDelay, mcinst.wavhdr.fmt.dwAvgBytesPerSec);
		memset(mcinst.audiodata, 0, mcinst.audioFrameSize);
		for (int i = 0; 
			i < padbytes - mcinst.audioFrameSize; 
			i += EncodeAudioData(mcinst.audiodata, mcinst.audioFrameSize));
		if (i < padbytes) EncodeAudioData(mcinst.audiodata, padbytes - i);
	} else if (mcinst.audioDelay < 0){
		int padbytes = (int)MS2BYTE(-mcinst.audioDelay, mcinst.wavhdr.fmt.dwAvgBytesPerSec);
		bytesOut = 0;
		for (int i = 0; i < padbytes - mcinst.audioFrameSize; i += bytesOut) {
			bytesOut = audiosrc->read(mcinst.audiodata, mcinst.audioFrameSize);
			if (bytesOut < 0) break;
		}
		if (bytesOut < 0)
			inst.state = STATE_ERROR;
		else
			if (i < padbytes) audiosrc->read(mcinst.audiodata, padbytes - i);
		if (audioenc->framesize) mcinst.audioFrameSize = audioenc->framesize * 2;
	}
	if (inst.state == STATE_ERROR) return;
	while (inst.state==STATE_ENCODING && 
			audiosrc->bytescount < audiosrc->totalbytes &&
			(bytesIn = audiosrc->read(mcinst.audiodata, mcinst.audioFrameSize)) >= 0) {
		bytesOut = EncodeAudioData(mcinst.audiodata, bytesIn);
		if (bytesOut<=0) break;;
	}
}

static int TranscodeAudio()
{
	int ret=0;
	CString msg;
	T_ITEM* itemint = 0;
	int startpos = pref.GetValueInt(PI_STARTPOS) * 1000;
	int length = pref.GetValueInt(PI_ENDPOS) * 1000;
	HANDLE hPumpThread;
	CLogger logger;

	if (mcinst.item->info.begin) startpos += mcinst.item->info.begin;
	if (mcinst.item->info.end) length = mcinst.item->info.end - mcinst.item->info.begin;
	mcinst.audioFrameSize = AUDIO_BLOCK_SIZE;
	mcinst.audioDelay = pref.GetValueInt(PI_AUDIO_DELAY);
	if (pref.GetValueBool(PI_AACP_WORKAROUND)) mcinst.audioDelay -= 100;

	memset(&inst.astats, 0, sizeof(AUDIO_STATS));
	mcinst.item->info.tags.valid = (pref.GetValueBool(PI_TAG) && mcinst.item->info.tags.title);
	inst.stage = ST_PROCESSING_AUDIO;
	mcinst.destfile = "";
	GetVariable("DestFile",mcinst.destfile);
	pfnEvent(MC_UPDATE_STATE, (void*)inst.state);

	audiosrc = AudioSources[pref.GetValueInt(PI_ADEC)];
	if (inst.stage == ST_EXTRACTING_PCM) {
		audiosrc = AudioSources[AS_WAVE];
	}

	mcinst.fUseWaveFile = false;

	// check if the encoder can decode source audio file
	mcinst.fAudioDecodable = false;
	if (audioenc->GetInExts()) {
		TCHAR* exts = strdup(audioenc->GetInExts());
		TCHAR* p = strtok(exts, ";");
		for ( ; p; p = strtok(0, ";")) {
			if (mcinst.item->info.filetype->fileext && !strcmp(mcinst.item->info.filetype->fileext, p)) {
				mcinst.fAudioDecodable = true;
				break;
			}
		}
		free(exts);
	}

	if (((audioenc->flags & FLAG_STANDALONE) && !mcinst.fAudioDecodable) || pref.GetValueBool(PI_FORCE_WAVEFILE) || pref.GetValueBool(PI_SHIBATCH)) {
		CString source;
		mcinst.fUseWaveFile = true;
		pfnEvent(MC_UPDATE_STATE, (void*)inst.state);
		pfnEvent(MC_UPDATE_TITLE, "extracting waveform");
		logger.Log(IDS_EXTRACT_WAVE, mcinst.item->filename);
		GetVariable("SourceFile", source);
		timerlevel = 1;
		if (audiosrc->extract(mcinst.item->filename, source.GetBuffer(), startpos)) return -1;
		if (pref.GetValueBool(PI_SHIBATCH)) {
			// run shibatch tool to process the wave file
			int ret;
			CMediaTool *mt = (CMediaTool*)mcCreateObject(TL_TOOL, MT_SHIBATCH);
			if (mt) {
				T_ITEM item;
				pfnEvent(MC_UPDATE_TITLE, "processing waveform");
				item.filename = source.GetBuffer();
				ret = mt->Run(0, &item);
				pfnEvent(MC_MSG_INFO, mt->result.GetBuffer());
				mcDestroyObject(TL_TOOL, MT_SHIBATCH, mt);
			}
		}
		audiosrc = AudioSources[AS_WAVE];
		itemint = mcMakeItem(source.GetBuffer());
	}

	// opening audio source
	logger.Log(IDS_LOAD_AUDIO_SOURCE, audiosrc->GetName());
	audiosrc->samplerate = 0;
	{
		int low = audioenc->GetLowestSampleRate();
		int high = audioenc->GetHighestSampleRate();
		if (low == high && low != 0) {
			audiosrc->samplerate = low;
		}
	}
	audiosrc->duration = length;
	ret = audiosrc->open(itemint ? itemint : mcinst.item, startpos);
	if (itemint)
		mcFreeItem(itemint);
	if (ret)
		return -1;
	inst.astats.length = length > 0 ? length : (mcinst.item->info.duration - startpos);

	MC_AUDIO_FILTER af;
	mcinst.audiofilter = 0;
	if (audiosrc->hdr.fmt.wChannels != 1) {
		int i = pref.GetValueBool(PI_DOWNMIX);
		if (i) {
			memset(&af, 0, sizeof(af));
			if (pref.GetValueBool(PI_ALL_AUDIO)) {
				af.pfnAudioDsp = dspLeftChannelMono;
				af.channels = 1;
			} else {
				switch (i) {
				case 1: af.pfnAudioDsp = dspLeftChannelMono; break;
				case 2: af.pfnAudioDsp = dspRightChannelMono; break;
				default: af.pfnAudioDsp = dspDownmix; break;
				}
				af.channels = 1;
			}
			mcinst.audiofilter = &af;
		} else if (audioenc->GetMaxChannel() == 1) {
			memset(&af, 0, sizeof(af));
			af.pfnAudioDsp = dspDownmix;
			af.channels = 1;
			mcinst.audiofilter = &af;
		} else if (audioenc->framesize) {
			mcinst.audioFrameSize = audioenc->framesize;
		}
	}

	InvalidiateRiffHeader(&audiosrc->hdr);
	memcpy(&mcinst.wavhdr, &audiosrc->hdr, sizeof(RIFF_HEADER));
	{
		int srate = pref.GetValueInt(PI_FORCE_SRATE);
		if (srate) mcinst.wavhdr.fmt.dwSamplesPerSec = srate;
	}
	msg.Format("\nAudio source: %s\nSample Rate: %dHz Channels: %d BPS: %d\n",
		audiosrc->GetName(), mcinst.wavhdr.fmt.dwSamplesPerSec, mcinst.wavhdr.fmt.wChannels, mcinst.wavhdr.fmt.wBitsPerSample);
	pfnEvent(MC_MSG_INFO, msg.GetBuffer());

	if (ret = StartAudioEncoder()) {
		audiosrc->close();
		return -1;
	}

	if (audioenc->framesize) mcinst.audioFrameSize = audioenc->framesize;
	if (mcinst.audiofilter) {
		if (audiosrc->hdr.fmt.wChannels > mcinst.audiofilter->channels)
			mcinst.audioFrameSize *= (audiosrc->hdr.fmt.wChannels / mcinst.audiofilter->channels);
	}
	audiosrc->framesize = mcinst.audioFrameSize;

	pfnEvent(MC_UPDATE_TITLE, "audio transcoding in progress");

	timerlevel = 2;
	if ((audioenc->flags & FLAG_STANDALONE)) {
		ret = WaitAudioEncoder();
	} else {
		DWORD dwid;
		int timeout = pref.GetValueInt(PI_TIMEOUT);
		mcinst.filterbuf = (char*)malloc(mcinst.audioFrameSize);
		mcinst.audiodata = (char*)malloc(mcinst.audioFrameSize);
		hPumpThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AudioDataPump, 0, 0, &dwid);
		for (;;) {
			unsigned long long bytescount = audiosrc->bytescount;
			if (WaitForSingleObject(hPumpThread, timeout) != WAIT_TIMEOUT) break;
			if (bytescount == audiosrc->bytescount) {
				// the pump is blocked
				TerminateThread(hPumpThread, 0);
				break;
			}
		}
		CloseHandle(hPumpThread);
		free(mcinst.filterbuf);
		free(mcinst.audiodata);
	}
	timerlevel = 0;
	mcinst.wavhdr.desc.chunkSize = mcinst.wavhdr.data.chunkSize + 36;
	audioenc->CloseEncoder(&mcinst.wavhdr);
	logger.Log(IDS_UNLOADED, audioenc->GetFullName());
	audiosrc->close();
	logger.Log(IDS_UNLOADED, audiosrc->GetName());
	if ((audioenc->flags & FLAG_TO_TAG) && pref.GetValueBool(PI_TAG) && !mcinst.fNeedWrap) {
		WriteFileTags(mcinst.destfile.GetBuffer(), mcinst.item);
	}
	if (mcinst.fUseWaveFile && !pref.GetValueBool(PI_KEEP_TEMP)) {
		CString tempfile;
		GetVariable("TempFile", tempfile);
		tempfile.AppendFormat(".%s", AudioEncoders[AC_PCM]->GetOutExt());
		DeleteFile(tempfile.GetBuffer());
	}
	mcinst.fAudioDecodable = false;
	mcinst.fUseWaveFile = false;
	inst.astats.percent = 100;
	pfnEvent(MC_UPDATE_PROGRESS, (void*)&inst.astats);
	return ret;
}

int mcContainerAddFile(TRACK_TYPE type, TCHAR* filename, CF_FLAGS flags)
{
	int i;
	if (!filename) return -1;
	for (i=0;i<MAX_TRACKS;i++) {
		if (!tracks[i].filename) {
			tracks[i].filename=strdup(filename);
			if (type==CT_UNKNOWN) {
				FILE_TYPE* ft = mcIdentifyFileType(filename);
				if (ft) {
					type = ft->type;
				}
			}
			tracks[i].type=type;
			tracks[i].flags=flags;
			return 0;
		}
	}
	return -1;
}

int mcContainerRemoveFile(TCHAR* filename)
{
	int i;
	if (!filename) return -1;
	for (i=0;i<MAX_TRACKS;i++) {
		if (tracks[i].filename && !strcmp(tracks[i].filename,filename)) {
			free(tracks[i].filename);
			tracks[i].filename=0;
			tracks[i].type=CT_UNKNOWN;
			return 0;
		}
	}
	return -1;
}

int mcContainerPurge(int fDelete)
{
	int i;
	CString tmpfile;
	GetVariable("TempFile", tmpfile);
	for (i=0; i<MAX_TRACKS; i++) {
		if (tracks[i].filename) {
			if (fDelete && !strncmp(tracks[i].filename, tmpfile.GetBuffer(), tmpfile.GetLength())) DeleteFile(tracks[i].filename);
			free(tracks[i].filename);
			memset(&tracks[i], 0, sizeof(CONTAINER_TRACKS));
			tracks[i].type=CT_UNKNOWN;
		}
	}
	return 0;
}

int mcContainerMuxFiles(TCHAR* destfile)
{
	if (mcinst.item->target.container == CF_DEFAULT) return -1;
	TCHAR *p = Containers[mcinst.item->target.container]->UpdateSettings(destfile, tracks);
	if (!p) return 0;
	pfnEvent(MC_MSG_CMD, p);
	if (Containers[mcinst.item->target.container]->Mux()) {
		CLogger logger;
		logger.Log(IDS_ERROR_MUXER);
		return -1;
	}
	while (Containers[mcinst.item->target.container]->GetProgress() >= 0) Sleep(100);
	if (pref.GetValueBool(PI_TAG) && mcinst.item->info.tags.title) {
		return WriteFileTags(mcinst.destfile.GetBuffer(), mcinst.item);
	}
	return 0;
}

void QualifyVariables(CString& cmd, TCHAR *in)
{
	TCHAR *q;

	for (q = in; *q; ) {
		if (*q=='$') {
			if (*(q+1)=='(') {
				TCHAR *s = strchr(q+2,')');
				if (s) {
					*s=0;
					GetVariable(q+2, cmd);
					*s=')';
					//bool fQuote=(*q!='\"' && *(s+1)!='\"' && varbuf[0]!='\"' && strchr(varbuf,' '));
					//p += sprintf(p,fQuote?"\"%s\"":"%s",varbuf);
					q = s + 1;
					continue;
				}
			}
		}
		cmd += *(q++);
	}
}

static int MakeDir(TCHAR* dirname)
{
	TCHAR *p=dirname;
	DWORD attr;
	if (dirname[1]==':') p+=2;
	for(;;) {
		p=strchr(p+1,'\\');
		if (!p) {
			return CreateDirectory(dirname,NULL)?0:-1;
		}
		*p=0;
		attr=GetFileAttributes(dirname);
		if (attr == -1) {
			if (!CreateDirectory(dirname,NULL)) return -1;
		} else if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
			return -1;
		}
		*p='\\';
	}
}

int CheckFileExistence(TCHAR* filename, TASK_STATS* stats)
{
	if (filename == 0) return -1;
	if ((mcinst.fNeedWrap || mcinst.fUseWaveFile) && inst.stage != ST_MUXING) {
		return DeleteFile(filename) ? 0 : -1;
	}
	OV_ACTION choice = (OV_ACTION)pref.GetValueInt(PI_OVERWRITE);
	if (FileExist(filename)) {
		CString msg;
		if (choice == OA_SKIP) {
			return 1;
		}
		if (choice == OA_ASK) {
			if (mcinst.ovaction == OA_ASK) {
				msg.Format(IDS_OVERWRITE_CONFIRM, filename);
				switch (pfnEvent(MC_MSG_YESNOCANCEL, msg.GetBuffer())) {
				case IDYES:
					choice = OA_OVERWRITE;
					break;
				case IDNO:
					choice = OA_BACKUP;
					break;
				default:
					choice = OA_SKIP;
					break;
				}
				if (mcinst.item->next) {
					msg.Format(IDS_OVERWRITE_KEEP, filename);
					if (pfnEvent(MC_MSG_YESNO, msg.GetBuffer()) == IDYES) {
						mcinst.ovaction = choice;
					}
				}
			} else {
				choice = mcinst.ovaction;
			}
		}
		switch (choice) {
		case OA_BACKUP: {
			msg = filename;
			int i = msg.ReverseFind('.');
			if (i > 0)
				msg.Insert(i, ".bak");
			else
				msg += ".bak";
			MoveFile(filename, msg.GetBuffer());
			} break;
		case OA_OVERWRITE:
			if (!DeleteFile(filename)) {
				msg.Format(IDS_ERROR_DELETE, filename);
				pfnEvent(MC_MSG_ALERT, msg.GetBuffer());
				choice = OA_SKIP;
			}
			break;
		}
	} else {
		choice = OA_NOTEXIST;
	}
	TCHAR *p;
	if (choice == OA_NOTEXIST && (p=strrchr(filename,'\\')) && pref.GetValueBool(PI_CREATE_DIR)) {
		*p=0;
		MakeDir(filename);
		*p='\\';
	}
	return (choice == OA_SKIP) ? 1 : 0;
}

static int TranscodeVideo()
{
	CString cmd;
	CString msg;
	int ret = 0;
	CLogger logger;
	mcinst.bitrate = 0;
	inst.stage=ST_PROCESSING_VIDEO;
	mcinst.destfile = "";
	GetVariable("DestFile",mcinst.destfile);

	logger.Log(IDS_APPLY_SETTINGS, videoenc->GetName());

	mcinst.item->target.fps = 0;
	if (pref.GetValueInt(PI_FPS)) {
		if (pref.GetValueBool(PI_CONSTANT_FPS) || !mcinst.item->info.video.fps || mcinst.item->info.video.fps > atof(pref.GetValueEnum(PI_FPS))) {
			mcinst.item->target.fps = (float)atof(pref.GetValueEnum(PI_FPS));
		}
	}

	videoenc->UpdateSettings();
	inst.totalpass = videoenc->GetPassCount();
	pfnEvent(MC_UPDATE_STATE, (void*)inst.state);
	//GenCommandLines();
	inst.totalpass = videoenc->GetPassCount();
	for (int i=0; i < inst.totalpass && inst.state==STATE_ENCODING; i++) {
		if (videoenc->OpenEncoder(mcinst.item->filename, mcinst.destfile.GetBuffer())) {
			logger.Log(IDS_ENCODER_FAIL, videoenc->GetName());
			break;
		}

		inst.stats.dwClipStartTime = NOW;
		logger.Log(IDS_LOADED, videoenc->GetName());
		inst.vstats = videoenc->stats;
		inst.currentpass = i + 1;
		cmd.Format("pass %d of %d", inst.currentpass, inst.totalpass);
		pfnEvent(MC_UPDATE_TITLE, cmd.GetBuffer());

		cmd = "";
		QualifyVariables(cmd, videoenc->GetCommand(i));

		//start external program
		if (videoenc->Encode(cmd)) {
			videoenc->CloseEncoder();
			ret = -1;
			logger.Log(IDS_UNLOADED, videoenc->GetName());
			break;
		}
		//wait for encoder to finish
		while ( inst.state == STATE_ENCODING ) {
			if (videoenc->GetProgress() < 0) break;
		}
		mcinst.bitrate = videoenc->stats->vbitrate;
		inst.vstats = 0;
		videoenc->CloseEncoder();
		logger.Log(IDS_UNLOADED, videoenc->GetName());
	}
	if (ret) return ret;
	mcContainerAddFile(CT_VIDEO, mcinst.destfile.GetBuffer(), DUMMY_FLAG);
	return 0;
}

int mcStopTranscode()
{
	if (inst.state == STATE_IDLE) return -1;
	inst.state = STATE_STOPPING;
	return WaitForSingleObject(mcinst.hEvent, 3000) == WAIT_OBJECT_0 ? 0 : -1;
}

int mcStartTranscode()
{
	CString str;
	int ret;
	int tm;
	int errmsg = 0;
	bool fNoConfirm = false;
	CLogger logger;

	if (!mcGetNextItem(0)) return -1;

	SetProcessPriorityBySetting(GetCurrentProcess());

	mcinst.item = mcGetNextItem(0);

	memset(&inst.stats, 0, sizeof(TASK_STATS));
	mcinst.fAudioDecodable = false;
	mcinst.ovaction = OA_ASK;
	mcinst.commonPartLen=0;
	if (pref.GetValueBool(PI_KEEP_DIR)) {
		TCHAR *p;
		T_ITEM* item = itemqueue.next;
		if (!strstr(item->filename, "://")) {
			p=strrchr(item->filename,'\\');
			if (p) {
				p++;
				mcinst.commonPartLen=p - item->filename;
			}
		}

		T_ITEM* prev = item;
		//find out the common part of paths
		for (item = mcinst.item; item && mcinst.commonPartLen>0; prev = item, item = item->next) {
			int i;
			if (strstr(item->filename, "://")) {
				mcinst.commonPartLen = 0;
				break;
			}
			for (i = ( p[1] ==':' ? 2 : 0); prev->filename[i] == item->filename[i] && i<mcinst.commonPartLen; i++);
			mcinst.commonPartLen=i;
		}
	}

	inst.state = STATE_ENCODING;
	inst.stats.dwEncStartTime = NOW;
	inst.stats.percent = 0;
	for (; inst.state == STATE_ENCODING; mcinst.item = mcinst.item->next) {
		tm = NOW;
		inst.stats.dwTaskStartTime = tm;
		logger.Log(IDS_START_JOB, mcinst.item->filename);
		inst.stats.fileprocessed++;
		pfnEvent(MC_JOB_START, mcinst.item);
		mcinst.destfile = "";

		if (mcinst.item->flags & IF_DONE) {
			inst.state = STATE_SKIPPING;
			goto skip;
		}

		mcGetMediaInfo(mcinst.item);

		if (!(mcinst.item->flags & IF_PRIVATE_FORMAT)) {
			mcApplyParam();
		}
		audioenc = AudioEncoders[mcinst.item->target.audioformat];
		videoenc = VideoEncoders[mcinst.item->target.videoenc];
		container = Containers[mcinst.item->target.container];

		memset(&inst.vstats, 0, sizeof(MP_STATS));
		mcinst.fAudioEncoding = (mcinst.item->target.videoformat == VC_NONE || mcinst.item->info.filetype->type == CT_AUDIO);

		if (!fNoConfirm && !pref.GetValueBool(PI_QUIET) && 
			((mcinst.fAudioEncoding && !mcinst.item->info.duration) ||
			!mcinst.fAudioEncoding && (!mcinst.item->info.video.codec || !mcinst.item->info.video.width || !mcinst.item->info.video.height))) {
			int ret;
			CString msg;
			msg.Format(IDS_INVALID_FILE, mcinst.item->filename);
			ret = pfnEvent(MC_MSG_YESNO, msg.GetBuffer());
			msg.Format(IDS_OVERWRITE_KEEP);
			if (pfnEvent(MC_MSG_YESNO, msg.GetBuffer()) == IDYES) fNoConfirm = true;
			if (ret == IDNO) {
				inst.state = STATE_SKIPPING;
				goto skip;
			}
		}

		{
			mcinst.fNeedWrap = false;
			if (mcinst.item->target.container != CF_DEFAULT) {
				if (!videoenc->CheckContainerSupport(mcinst.item->target.container)) {
					if (audioenc->flags & FLAG_NO_ENCODER) {
						logger.Log(IDS_AUDIO_STREAM_UNSUPPORTED);
						inst.state = STATE_ERROR;
					} else {
						mcinst.fNeedWrap = true;
					}
				}
			}
		}
		if (!container->CheckAudioSupport(mcinst.item->target.audioformat) || !container->CheckVideoSupport(mcinst.item->target.videoformat)) {
			PopupMsg(IDS_ERROR_MUX);
			inst.state = STATE_ERROR;
		}
		if (inst.state!=STATE_ENCODING) goto skip;

		// retrieve file tags
		if (!mcinst.item->info.tags.valid)
			ReadFileTags(mcinst.item);

		// check for destination file existance
		inst.stage = ST_MUXING;
		mcinst.destfile = "";
		GetVariable("DestFile", mcinst.destfile);
		switch (CheckFileExistence(mcinst.destfile.GetBuffer(), &inst.stats)) {
		case 1:
			inst.state = STATE_SKIPPING;
			goto skip;
		case -1:
			inst.state = STATE_ERROR;
			goto skip;
		}
		inst.stats.dwClipStartTime = NOW;

		tm = inst.stats.dwClipStartTime - tm;
		inst.stats.dwEncStartTime += tm;
		inst.stats.dwTaskStartTime += tm;

		// encode audio
		if (mcinst.fAudioEncoding && (audioenc->flags & FLAG_NO_ENCODER)) {
			logger.Log(IDS_NOT_STANDALONE_ENCODER, 0);
			inst.state = STATE_ERROR;
		} else if (mcinst.item->target.audioformat == AC_COPY) {
			mcContainerAddFile(CT_AUDIO, mcinst.item->filename, DUMMY_FLAG);
		} else if ((mcinst.fAudioEncoding || mcinst.fNeedWrap) && mcinst.item->target.audioformat != AC_NONE) {
			pfnEvent(MC_UPDATE_TITLE, "audio transcoding in progress");
			mcinst.audiochannel = pref.GetValueInt(PI_START_AID);
			for(;;) {
				inst.stats.dwClipStartTime = GetTickCount();
				ret = TranscodeAudio();
				if (!ret) {
					mcContainerAddFile(CT_AUDIO, mcinst.destfile.GetBuffer(), DUMMY_FLAG);
					if (inst.state == STATE_ENCODING && pref.GetValueBool(PI_ALL_AUDIO)) {
						mcinst.audiochannel++;
						if (pref.GetValueInt(PI_DOWNMIX)) {
							if (mcinst.audiochannel == 2) break;
							inst.stats.dwClipStartTime = GetTickCount();
							switch (mcinst.audiochannel) {
							case 1:
								mcinst.audiofilter->pfnAudioDsp = dspRightChannelMono;
								break;
							}
						}
						continue;
					}
				} else {
					if (!pref.GetValueBool(PI_ALL_AUDIO)) {
						logger.Log(IDS_ERROR_AUDIO);
					}
				}
				break;
			}
			if (pref.GetValueBool(PI_ALL_AUDIO)) pref.SetValue(PI_AID, 0);
		} else if (!videoenc->CheckAudioSupport(mcinst.item->target.audioformat)) {
			logger.Log(IDS_CANNOT_MUX_AUDIO);
		}
		pfnEvent(MC_UPDATE_STATS, 0);
		if (inst.state != STATE_ENCODING) goto skip;

		// encode video
		timerlevel = 2;
		inst.stats.dwClipStartTime = GetTickCount();
		if (mcinst.item->target.videoformat == VC_COPY && container->IsExternalMuxer()) {
			mcContainerAddFile(CT_VIDEO, mcinst.item->filename, DUMMY_FLAG);
		} else if (!mcinst.fAudioEncoding) {
			if (TranscodeVideo()) {
				logger.Log(IDS_ERROR_VIDEO_TRANSCODE);
				inst.state=STATE_ERROR;
			}
		}
		mcinst.fNeedWrap=false;
		if (inst.state!=STATE_ENCODING) goto skip;
		// mux into container
		if (container->IsExternalMuxer()) {
			inst.stage=ST_MUXING;
			pfnEvent(MC_UPDATE_TITLE, "muxing tracks into container");
			pfnEvent(MC_UPDATE_STATE, (void*)inst.state);
			mcinst.destfile = "";
			GetVariable("DestFile",mcinst.destfile);
			Sleep(500);
			mcContainerMuxFiles(mcinst.destfile.GetBuffer());
		}
		skip:
		timerlevel = 0;
		logger.Log(IDS_FINISH_JOB, mcinst.item->filename);
		// clean up
		mcContainerPurge((pref.GetValueBool(PI_KEEP_TEMP) || mcinst.item->target.container == CF_DEFAULT) ? 0 : 1);
		str.Format("%smcpass%d.log",pref.GetValueStr(PI_TEMP_DIR),GetCurrentProcessId());
		DeleteFile(str.GetBuffer());
		str += ".temp";
		DeleteFile(str.GetBuffer());
		if (inst.state==STATE_ENCODING) {
			if (FileExist(mcinst.destfile.GetBuffer())) {
				int size = RetrieveFileSize(mcinst.destfile.GetBuffer());
				if (size >= 0) {
					inst.stats.resultKB += size;
					if (pref.GetValueBool(PI_DEL_ORIGINAL)) DeleteFile(mcinst.item->filename);
					inst.stats.encodedFiles++;
					inst.stats.originalKB += (unsigned int)(mcinst.item->info.filesize);
				}
				mcinst.item->flags |= IF_DONE;
				if (mcinst.item->destfile) free(mcinst.item->destfile);
				mcinst.item->destfile = strdup(mcinst.destfile.GetBuffer());
			} else {
				inst.state = STATE_ERROR;
			}
		}
		pfnEvent(MC_JOB_FINISH, mcinst.item);
		if (inst.state ==STATE_SKIPPING) {
			inst.stats.skippedFiles++;
			inst.state=STATE_ENCODING;
		} else if (inst.state !=STATE_ENCODING) {
			if (!pref.GetValueBool(PI_KEEP_UNFINISH)) {
				DeleteFile(mcinst.destfile);
			}
			switch (inst.state) {
			case STATE_ERROR:
				inst.stats.errorFiles++;
				inst.state=STATE_ENCODING;
				break;
			case STATE_STOPPING:
				inst.stats.skippedFiles++;
				break;
			}
		}
		pfnEvent(MC_UPDATE_STATE, (void*)inst.state);
		mcinst.fAudioEncoding=0;
		if (!mcinst.item->next) break;
	}

	mcinst.item = &itemqueue;
	mcApplyParam();
	inst.astats.percent = 100;
	pfnEvent(MC_UPDATE_TITLE, 0);
	pfnEvent(MC_UPDATE_PROGRESS, (void*)100);

	if (pref.GetValueBool(PI_POWEROFF)) {
		inst.state = STATE_SHUTDOWN;
		return 0;
	}
	switch (inst.state) {
	case STATE_ENCODING:
		mcInvalidiateStatsData();
		if (!mcGetPrefValueInt(PI_POWEROFF) && inst.stats.encodedFiles>0) {
			float ratio = inst.stats.resultKB ? (float)inst.stats.originalKB / inst.stats.resultKB : 0;
			CString result;
			result.Format(IDS_TRANSCODE_DONE, inst.stats.encodedFiles, inst.stats.originalKB, inst.stats.resultKB, (NOW - inst.stats.dwEncStartTime) / 1000, ratio);
			pfnEvent(MC_MSG_POPUP, result.GetBuffer());
		} else {
			PopupMsg(IDS_TRANSCODE_FAIL);
		}
		break;
	case STATE_STOPPING:
		PopupMsg(IDS_TRANSCODE_INTERRUPTED);
		logger.Log(IDS_TRANSCODE_INTERRUPTED);
		break;
	}
	inst.state = STATE_IDLE;
	SetEvent(mcinst.hEvent);
	return 0;
}

STATES mcGetState()
{
	return inst.state;
}

void mcSetState(STATES state)
{
	inst.state = state;
}

void mcInvalidiateStatsData()
{
	inst.stats.dwElapsed = GetTickCount() - inst.stats.dwClipStartTime;
	if (inst.state != STATE_ENCODING) return;
	switch (inst.stage) {
	case ST_PROCESSING_AUDIO:
		if (audiosrc) {
			inst.astats.pos = audiosrc->GetPos();
			inst.astats.bytes = audiosrc->bytescount;
			if (inst.astats.pos > 0 && inst.astats.length > 0) {
				if (inst.astats.pos > inst.astats.length) {
					inst.astats.percent = 99;
				} else {
					inst.astats.percent = (float)inst.astats.pos * 100 / inst.astats.length;
				}
			}
			if (inst.stats.dwElapsed) inst.astats.speed = inst.astats.pos * 10 / inst.stats.dwElapsed;
		}
		inst.stats.percent = inst.astats.percent;
		break;
	case ST_PROCESSING_VIDEO:
		if (inst.vstats) {
			inst.stats.percent = inst.vstats->percent;
		}
		break;
	}
}

extern PARAM_DESC prefInfo[];

int mcGetPrefProperty(int id, PARAM_PROPERTY property, void* pval)
{
	PARAM_DATA* entry = pref.GetEntry(id);
	switch (property) {
	case PP_KEY: *(char**)pval = entry->pchKeyName; break;
	case PP_TYPE: *(int*)pval = entry->type; break;
	case PP_FLAG: *(int*)pval = entry->flags; break;
	case PP_VALMIN: *(int*)pval = entry->min; break;
	case PP_VALMAX: *(int*)pval = entry->max; break;
	case PP_CAPTION: *(char**)pval = entry->pchParamName; break;
	case PP_DESC:
		for (int i = 0; prefInfo[i].desc; i++) {
			if (prefInfo[i].id == id) {
				*(char**)pval = prefInfo[i].desc;
				break;
			}
		}
		break;
	default:
		return -1;
	}
	return 0;
}

int mcGetPrefIdByKey(char* key)
{
	return pref.GetIdByKey(key);
}

char* mcGetPrefKeyById(int id)
{
	return pref.GetKeyById(id);
}

void* mcGetPrefValue(int id)
{
	return pref.GetValue(id);
}

int mcGetPrefValueInt(int id)
{
	return pref.GetValueInt(id);
}

TCHAR* mcGetPrefValueStr(int id)
{
	return pref.GetValueStr(id);
}

TCHAR* mcGetPrefValueEnum(int id)
{
	return pref.GetValueEnum(id);
}

TCHAR* mcGetPrefValueEnumDefs(int id, int index)
{
	TCHAR *p=(TCHAR*)prefParams[id].pchEnums;
	for (int i = 0; i < index && *p; i++) {
		while (*(p++));
	}
	return p;
}

float mcGetPrefValueFloat(int id)
{
	return pref.GetValueFloat(id);
}

void* mcSetPrefValue(int id, void* value)
{
	return pref.SetValue(id, value);
}

float mcSetPrefValueFloat(int id, float value)
{
	return pref.SetValue(id, value);
}

int mcSetPrefValueInt(int id, int value)
{
	return pref.SetValue(id, value);
}

TCHAR* mcGetToolName(MC_TOOLS type, int nametype, int id)
{
	switch (type) {
	case TL_AUDIO_ENCODER:
		return nametype == 0 ? AudioEncoders[id]->GetName() : AudioEncoders[id]->GetFullName();
	case TL_VIDEO_CODEC:
		return pref.GetValueEnum(PI_VCODEC);
	case TL_VIDEO_ENCODER:
		return VideoEncoders[id]->GetName();
	case TL_CONTAINER:
		return Containers[id]->GetName();
	}
	return NULL;
}

TCHAR* mcGetToolVersion(MC_TOOLS type, int id)
{
	switch (type) {
	case TL_AUDIO_ENCODER:
		return AudioEncoders[id]->GetVersionInfo();
	case TL_VIDEO_ENCODER:
		return VideoEncoders[id]->GetVersionInfo();
	case TL_CONTAINER:
		return Containers[id]->GetVersionInfo();
	}
	return NULL;
}

TCHAR* mcGetToolCommand(MC_TOOLS type, int pass)
{
	switch (type) {
	case TL_AUDIO_ENCODER:
		AudioEncoders[pref.GetValueInt(PI_AENC)]->UpdateSettings();
		return AudioEncoders[pref.GetValueInt(PI_AENC)]->cl.GetBuffer();
	case TL_VIDEO_CODEC:
		if (pref.GetValueInt(PI_VCODEC) != VC_NONE) {
			if (VideoEncoders[pref.GetValueInt(PI_VENC)]->UpdateSettings() == 0) {
				if (pass == 0) PopupMsg(IDS_UNSUPPORTED_MODE);
			} else {
				return VideoEncoders[pref.GetValueInt(PI_VENC)]->GetCommand(pass);
			}
		}
		break;
	case TL_CONTAINER:
		//FIXME
		//return Containers[pref.GetValueInt(PI_CONTAINER)]->cmd.GetBuffer();
		break;
	}
	return 0;	
}

TCHAR* mcRunMediaTool(MEDIA_TOOLS type, int action, T_ITEM* item)
{
	static CString result;
	int ret;
	CMediaTool *mt = (CMediaTool*)mcCreateObject(TL_TOOL, type);
	if (!mt) return 0;
	ret = mt->Run(action, item);
	result = mt->result;
    mcDestroyObject(TL_TOOL, type, mt);
	return ret ? 0 : result.GetBuffer();
}

int StartPlayer(T_ITEM* item, HWND hWndParent, HWND* phVideoWnd, char* opts)
{
	int ret;
	mcGetMediaInfo(item);
	player.mppath = mcGetPrefValueStr(PI_MP_PATH);
	player.flags = 0;
	if (hWndParent) {
		player.flags |= MF_NATIVE_WINDOW;
	}
	player.options = opts ? opts : "";
	ret = player.Play(item->filename, hWndParent);
	if (phVideoWnd) *phVideoWnd = player.hwnd;
	return ret;
}

int mcPlayerStart(T_ITEM* item, HWND hWndParent, HWND* phVideoWnd, BOOL fNoFilter, char* extraopts)
{
	CMediaInfo mi;
	CString opts;
	CString tmp;
	int i;
	if (!item) {
		if (phVideoWnd) *phVideoWnd = 0;
		return -1;
	}
	GenMplayerOptions(opts);
	GenCommonOptions(opts, item);
	if (!fNoFilter) {
		GenAudioFilterOptions(opts, 0);
		GenVideoFilterOptions(opts);
	}
	opts.AppendFormat(" -monitoraspect %d:%d", mcinst.scrw, mcinst.scrh);
	if ((i = pref.GetValueInt(PI_STARTPOS))) {
		opts.AppendFormat(" -ss %d", i);
	}
	if (extraopts) {
		opts += ' ';
		opts += extraopts;
	}
	return StartPlayer(item, hWndParent, phVideoWnd, opts.GetBuffer());
}

int mcPlayerStop()
{
	if (player.state == STATE_PLAYING) {
		if (player.hwnd) 
			player.Close();
		else
			player.Stop();
	}
	return 0;
}

int mcPlayerCommand(char* cmd)
{
	return player.Command(cmd);
}

int mcPreviewStart(T_ITEM* item, HWND hWndParent, HWND* phVideoWnd, char* extraopts)
{
	int ret;
	TCHAR* file;
	if (!item) {
		file = mcinst.destfile.GetBuffer();
	} else {
		if (!item->destfile || !item->destfile) return -1;
		file = item->destfile;
	}
	T_ITEM* itemtmp = mcMakeItem(file);
	ret =  StartPlayer(itemtmp, hWndParent, phVideoWnd, extraopts);
	mcFreeItem(itemtmp);
	return ret;
}

int mcShowConfigDialog(CONFIG_DIALOG type, PARAM_NOTIFY notify)
{
	HINSTANCE hInst = 0;
#ifdef _USRDLL
	hInst = hDllInst;
#endif
	switch (type) {
	case CD_PREF: {
		CParamList pl;
		return pl.open(hInst, prefParams, notify, prefInfo, 0, mcinst.hwndMain);
		} break;
	case CD_CONTAINER: {
		CParamList pl;
		int prefID[] = {PI_CONTAINER_OPTS, PI_CONTAINER_OPTS, PI_MKV, PI_MP4BOX, PI_MPG, PI_LAVF};
		return pl.open(hInst, prefParams, notify, prefInfo, prefID[pref.GetValueInt(PI_CONTAINER)], mcinst.hwndMain);
		} break;
	}
	return 0;
}

void mcLogInfo(int id, TCHAR* arg)
{
	static CLogger applog;
	applog.Log(id, arg);
	pfnEvent(MC_UPDATE_LOG, 0);
}

int mcGetBuildInfo(TCHAR** ptrdate)
{
	if (ptrdate) *ptrdate = BUILD_DATE;
	return BUILD_NO;
}

int mcGetMediaInfo(T_ITEM* item)
{
	switch (item->info.filetype->exttype) {
	case FT_UNKNOWN:
		return -1;
	}
	if (item->flags & IF_INFO_RETRIEVED) return 0;
	{
		CMediaInfo mi;
		mi.Run(1, item);
		if (item->info.duration) {
			if ((item->info.filetype->type == CT_AUDIO && (item->info.audio.codec || item->info.audio.samplerate)) || 
					(item->info.video.width && item->info.video.height)) {
				item->flags |= IF_INFO_RETRIEVED;
				return 0;
			}
		}
	}
	{
		CMPParser mpp;
		mpp.Run(0, item);
		item->flags |= IF_INFO_RETRIEVED;
	}
	return item->info.duration ? 0 : -1;
}

int mcGetTargetParam(TARGET_PARAM type)
{
	switch (type) {
	case TP_AUDIO_BITRATE:
		return AudioEncoders[mcinst.item->target.audioformat]->GetTargetBitRate();
		break;
	default:
		return -1;
	}
}

int mcSetTargetParam(TARGET_PARAM type, int val)
{
	return -1;
}
