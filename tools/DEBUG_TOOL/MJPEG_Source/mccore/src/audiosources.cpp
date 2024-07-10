/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include "config.h"
#include <windows.h>
#include <stdio.h>
#include "transcode.h"	
#include "processpil.h"
#include "datatypes.h"
#include "paramlist.h"
#include "preference.h"
#include "resource.h"
#include "common.h"
#include "audiosources.h"
#include "audioencoders.h"

//extern CAudioEncoder* AudioEncoders[];
extern CAudioSource* AudioSources[];
extern MC_STATE* pinst;
extern PFN_EVENT_HANDLER pfnEvent;

CAudioSource::CAudioSource():state(0),prefBase(0),duration(0),framesize(0),samplerate(0)
{
	InitRiffHeader(&hdr);
}

int CAudioSource::GetPos()
{ 
	return (int)BYTE2MS(bytescount, hdr.fmt.dwAvgBytesPerSec);
}

int CAudioSource::prepare()
{
	bytescount = 0;
	if (duration)
		totalbytes = MS2BYTE(duration, hdr.fmt.dwAvgBytesPerSec);
	else
		totalbytes = -1;
	return 0;
}

int CAudioSource::read(char* buffer, int bufsize)
{
	DWORD bytes = 0;
	if (!ReadFile(hInput, buffer, bufsize, &bytes, 0) || bytes == 0)
		return -1;
	bytescount += bytes;
	return bytes;
}

////////////////////////////////////////////////////////////////////////////////////
// Wave File Source
////////////////////////////////////////////////////////////////////////////////////
CWaveFileSource::CWaveFileSource()
{
	prefBase = PI_WAVE;
	logger.SetModuleName(GetName());
}

int CWaveFileSource::open(T_ITEM* item, int startpos)
{
	DWORD bytes;
	hInput = CreateFile(item->filename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if (hInput == INVALID_HANDLE_VALUE) return -1;
	ReadFile(hInput,&hdr,sizeof(RIFF_HEADER),&bytes,0);
	if (hdr.desc.chunkID=='FFIR' && hdr.desc.format=='EVAW' && hdr.fmt.chunkID==' tmf' && hdr.data.chunkID=='atad') {
		totalbytes = hdr.data.chunkSize;
	} else {
		SetFilePointer(hInput, 0, 0, FILE_BEGIN);
		InitRiffHeader(&hdr);
		*((LPDWORD)&totalbytes) = GetFileSize(hInput,(LPDWORD)&totalbytes + 1);
	}
	if (prepare()) return -1;
	seek(startpos);
	return 0;
}

int CWaveFileSource::close()
{
	CloseHandle(hInput);
	return 0;
}

int CWaveFileSource::seek(int startpos)
{
	if (startpos) {
		long long bytepos = MS2BYTE(startpos, hdr.fmt.dwAvgBytesPerSec) + sizeof(RIFF_HEADER);
		SetFilePointer(hInput, (DWORD)bytepos, (PLONG)&bytepos + 1, FILE_BEGIN);
	}
	return startpos;
}

////////////////////////////////////////////////////////////////////////////////////
// AMR Decoder
////////////////////////////////////////////////////////////////////////////////////
#if 0
CAmrDecoder::CAmrDecoder()
{
	logger.SetModuleName(GetName());
}

int CAmrDecoder::open(T_ITEM* item, int startpos)
{
	char buf[8];
	fp = fopen(item->filename,"rb");
	if (!fp) return -1;
	fread(buf, sizeof( char ), sizeof( AMRNB_MAGIC_NUMBER ) - 1, fp);
	if (memcmp(buf, AMRNB_MAGIC_NUMBER, sizeof(AMRNB_MAGIC_NUMBER) - 1)) {
		fclose(fp);
		return -1;
	}
	hdr.fmt.dwSamplesPerSec = 8000;
	hdr.fmt.wBitsPerSample = 16;
	hdr.fmt.wChannels = 1;
	frames = 0;
	if (prepare()) return -1;

	dll = new CDllHelper(pref.GetValueStr(PI_AMR_PATH));
	if (!dll->hDll) {
		delete dll;
		logger.Log(IDS_FILE_NOT_FOUND, pref.GetValueStr(PI_AMR_PATH));
		fclose(fp);
		return -1;
	}
	void* pfnAmrInit = dll->GetAddr("Decoder_Interface_init");
	pfnDecode = dll->GetAddr("Decoder_Interface_Decode");
	if (!pfnAmrInit || !pfnDecode) {
		delete dll;
		logger.Log(IDS_INCORRECT_VERSION, GetName());
		fclose(fp);
		return -1;
	}
	destate = (*(pfn_amrnb_dec_init)pfnAmrInit)();
	return 0;
}

int CAmrDecoder::close()
{
	pfn_amrnb_dec_exit pfnAmrExit;
	logger.Log(IDS_UNLOADING_DLL);
	pfnAmrExit = (pfn_amr_exit)dll->GetAddr("Dncoder_Interface_exit");
	if (pfnAmrExit) (*pfnAmrExit)(destate);
	fclose(fp);
	delete dll;
	return 0;
}

int CAmrDecoder::read(char* buffer, int bytes)
{
	unsigned char analysis[32];
	static int block_size[16]={ 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
	int dec_mode;
	
	if (bytes < AMRNB_FRAME_SIZE * sizeof(short)) return -1;
	if (fread(analysis, sizeof (unsigned char), 1, fp ) <= 0) return -1;
	dec_mode = (analysis[0] >> 3) & 0x000F;
	fread(&analysis[1], sizeof (char), block_size[dec_mode], fp );
	frames ++;
	(*(pfn_amrnb_decode)pfnDecode)(destate, analysis, (short*)buffer, 0);
	bytescount += AMRNB_FRAME_SIZE * sizeof(short);
	return AMRNB_FRAME_SIZE * sizeof(short);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// MPlayer Source
////////////////////////////////////////////////////////////////////////////////////
#define CONSOLE_BUFFER_SIZE 4096
/*
TCHAR* strrstr(TCHAR* str, TCHAR* substr)
{
	TCHAR *p,*s=NULL;
	p=str;
	for(;;) {
		p=strstr(p,substr);
		if (!p) break;
		s=p++;
	}
	return s;
}
*/

typedef struct ao_data_s
{
  int samplerate;
  int channels;
  int format;
  int bps;
  int outburst; 	  
  int buffersize;         
  int pts;
} ao_data_t;

int CMplayerSource::open(T_ITEM* item, int startpos)
{
	CString cmd;
	DWORD bytes;
	HANDLE hPipeWrite;
	ao_data_t ao_data;
	if (!CreatePipe(&hInput, &hPipeWrite, 0, AUDIO_BLOCK_SIZE)) return -1;
	SetHandleInheritance(&hPipeWrite, TRUE);
	cmd.Format("\"%s%s\" \"%s\" %s -vo null -ao pipe:fd=%d:fdtype=1",
		pref.GetValueStr(PI_MP_PATH),
		pref.GetValueBool(PI_MP_DLL) ? MP_DLL : MP_EXE,
		item->filename,
		pref.GetValueBool(PI_QUIET) ? "-really-quiet" : "-quiet",
		hPipeWrite);
	GenMplayerOptions(cmd);
	GenAudioFilterOptions(cmd, samplerate);
	GenCommonOptions(cmd, mcinst.item);
	if (startpos) {
		cmd.AppendFormat(" -vc null -ss %d", startpos / 1000);
	} else {
		cmd += pref.GetValueBool(PI_MP_BYPASS) ? " -vc dummy" : " -vc null";
	}
	if (mcinst.audiochannel && !pref.GetValueBool(PI_DOWNMIX)) {
		cmd.AppendFormat(" -aid %d", mcinst.audiochannel);
	}
	if (pref.GetValueBool(PI_MP_DLL)) {
		dll = new CDllHelper;
		if (!dll->LoadDLL(pref.GetValueStr(PI_MP_PATH), MP_DLL)) {
			logger.Log(IDS_FILE_NOT_FOUND, "MPlayer DLL");
			close();
			return -1;
		}
		if (dll->StartMain(cmd.GetBuffer())) {
			logger.Log(IDS_ERROR_DECODE_AUDIO, GetName());
			close();
			return -1;
		}
		SetThreadPriorityBySetting(dll->hThread);
	} else {
		sp = (SHELL_PARAM*)calloc(1, sizeof(SHELL_PARAM));
		sp->pchCommandLine = cmd.GetBuffer();
		if (ShellExec(sp)) {
			CloseHandle(hInput);
			return -1;
		}
	}
	bytes = 0;

	for (int i = 0; (!PeekNamedPipe(hInput,0,0,0,&bytes,0) || bytes == 0) && i < 300; i++) {
		if (dll) {
			if (dll->Wait(50) != WAIT_TIMEOUT) {
				// MPlayer exited unexpectedly
				close();
				return -1;
			}
		} else {
			if (ShellWait(sp, 50)) {
				// MPlayer exited unexpectedly
				close();
				return -1;
			}
		}
	}
	if (bytes == 0) {
		// not data received from info pipe
		close();
		if (false && !pref.GetValueBool(PI_QUIET)) {
			CString str;
			str.Format(IDS_MP_NEED_UPDATE);
			pfnEvent(MC_MSG_ALERT, str.GetBuffer());
		}
		return -1;
	}
	if (!ReadFile(hInput, &ao_data, sizeof(ao_data), &bytes, 0) || bytes != sizeof(ao_data)) {
		close();
		return -1;
	}
	hdr.fmt.dwSamplesPerSec = ao_data.samplerate;
	hdr.fmt.wChannels = ao_data.channels;
	prepare();

	if (!pref.GetValueBool(PI_MP_DLL))
		CloseHandle(hPipeWrite);

	return 0;
}

/*
int CMplayerSource::read(char* buffer, int bufsize)
{
	DWORD bytes = 0;
	PeekNamedPipe(hInput,0,0,0,&bytes,0);
	if (bytes == 0 && WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0) {
		// end of stream
		return -1;
	}
	if (!ReadFile(hInput, buffer, bufsize, &bytes, 0))
		return -1;
	bytescount += bytes;
	return bytes;
}
*/

int CMplayerSource::extract(TCHAR* filename, TCHAR* destfile, int startpos)
{
	CString cmd;
	cmd.Format("\"%smplayer.exe\" \"%s\" -really-quiet -vo null", pref.GetValueStr(PI_MP_PATH), filename);
	GenMplayerOptions(cmd);
	GenAudioFilterOptions(cmd, 0);
	GenCommonOptions(cmd, mcinst.item);
	if (startpos) {
		cmd.AppendFormat(" -ss %d", startpos);
	} else {
		cmd += " -vc dummy";
	}

	cmd += " -ao pcm:file=";
	cmd += destfile;
	sp = (SHELL_PARAM*)calloc(1, sizeof(SHELL_PARAM));
	sp->pchCommandLine = cmd.GetBuffer();
	pfnEvent(MC_MSG_CMD, sp->pchCommandLine);
	if (ShellExec(sp)) return -1;
	while (!ShellWait(sp, 1000) && inst.state==STATE_ENCODING);
	ShellClean(sp);
	free(sp);
	return FileExist(destfile) ? 0 : -1;
}

int CMplayerSource::close()
{
	if (dll) {
		dll->Stop();
		delete dll;
		dll = 0;
	} else {
		ShellTerminate(sp);
		ShellClean(sp);
		free(sp);
		sp = 0;
	}
	CloseHandle(hInput);
	state=0;
	return 0;
}
