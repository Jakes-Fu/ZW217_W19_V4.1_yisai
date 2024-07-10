/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include "config.h"
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <mmreg.h>
#include "resource.h"

#include "processpil.h"
#include "windowhandle.h"
#include "datatypes.h"
#include "transcode.h"
#include "audioencoders.h"
#include "paramlist.h"
#include "preference.h"
#include "common.h"

#include "all.h"

int CAudioEncoder::OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr)
{
	memset(&sp,0,sizeof(SHELL_PARAM));
	OVERLAPPED ov;
	if (flags & FLAG_PIPE) {
		CString pipename;
		memset(&ov,0,sizeof(ov));
		pipename.Format("\\\\.\\pipe\\%s%d.wav", OUTPIPENAME, GetCurrentProcessId());
		ov.hEvent = CreateEvent(0, TRUE, TRUE, 0);
		sp.fdWrite = CreateNamedPipe(pipename.GetBuffer(),PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED,PIPE_WAIT|PIPE_READMODE_MESSAGE|PIPE_TYPE_MESSAGE,1,4096,4096,1000,0);
		if (sp.fdWrite == INVALID_HANDLE_VALUE) {
			sp.fdWrite = 0;
			logger.Log(IDS_ERROR_CREATE_PIPE);
			return -1;
		}
		ConnectNamedPipe(sp.fdWrite, &ov);
	}
	if (!(flags & FLAG_PIPE) && !(flags & FLAG_STANDALONE)) {
		sp.flags = SHELL_REDIRECT_STDIN;
	}
	if ((flags & FLAG_READ_CONSOLE) && !pref.GetValueBool(PI_SHOW_CONSOLE)) {
		sp.flags |= SHELL_REDIRECT_STDOUT;
		sp.buffer=(TCHAR*)malloc(4096);
		sp.iBufferSize=4096;
	}
	if (pref.GetValueBool(PI_SHOW_CONSOLE)) sp.flags|=SHELL_SHOW_WINDOW;
	sp.pchCommandLine = cmd.GetBuffer();;
	pfnEvent(MC_MSG_CMD, sp.pchCommandLine);
	if (ShellExec(&sp)) {
		if (flags & FLAG_PIPE) CloseHandle(sp.fdWrite);
		logger.Log(IDS_FAIL_START_FE);
		return -1;
	}
	if (flags & FLAG_PIPE) {
		WaitForSingleObject(ov.hEvent, 3000);
		CloseHandle(ov.hEvent);
	}
	SetProcessPriorityBySetting(sp.piProcInfo.hProcess);
	percent = 0;
	return 0;
}

int CAudioEncoder::CloseEncoder(RIFF_HEADER *wavhdr) {
	if (sp.piProcInfo.dwProcessId) {
		if (flags & FLAG_READ_CONSOLE) {
			CloseHandle(sp.fdRead);
			free(sp.buffer);
		}
		if (sp.fdWrite) CloseHandle(sp.fdWrite);
		logger.Log(IDS_WAIT_PROCESS);
		ShellWait(&sp, 1000);
		ShellTerminate(&sp);
		ShellClean(&sp);
		percent = 0;
	}
	return 0;
}

int CAudioEncoder::Encode(char *data, int bytes) {
	DWORD bytesWritten=0;
	if (!WriteFile(sp.fdWrite,data,bytes,&bytesWritten,NULL))
		return -1;
	return bytesWritten;
}

int CAudioEncoder::GetProgress()
{
	TCHAR *p;
	if (!(sp.flags & SHELL_REDIRECT_STDOUT)) {
		return ShellWait(&sp, 1000)?-1:0;
	}
	if (ShellRead(&sp, 1000)<=0) {
		percent = 0;
		return -1;
	}
	if ((p=strchr(sp.buffer,'%'))) {
		for (;;) {
			TCHAR ch = *(--p);
			if (!(ch >= '0' && ch <='9') && ch != '.') break;
		}
		percent = atoi(p + 1);
	}
	return percent;
}

/*******************************************************************************
* FAAC encoder
********************************************************************************/

CFaac::CFaac()
{
	prefBase=PI_FAAC;
	flags=FLAG_RAW_PCM;
	logger.SetModuleName(GetName());
}

int CFaac::UpdateSettings()
{
	static const TCHAR *objtypes[]={"LC","Main","SSR","LTP"};
	logger.Log(IDS_APPLY_SETTINGS);
	cl.Format("\"%s\" -w -P -X -%c %d --mpeg-vers %c --obj-type %s -R $(SampleRate) -B $(SampleBits) -C $(ChannelNum)",
		pref.GetValueStr(PI_FAAC_PATH),
		pref.GetValueInt(PI_FAAC_MODE)?'b':'q',
		pref.GetValueInt(PI_FAAC_MODE)?pref.GetValueInt(PI_FAAC_BITRATE):pref.GetValueInt(PI_FAAC_QUALITY),
		pref.GetValueInt(PI_FAAC_MPEGVER)?'4':'2',
		objtypes[pref.GetValueInt(PI_FAAC_OBJTYPE)]);
	if (pref.GetValueBool(PI_TAG) && mcinst.item->info.tags.valid)
		cl.AppendFormat(" --title \"$(Title)\" --artist \"$(Artist)\" --album \"$(Album)\" --track \"$(Track)\" --year \"$(Year)\" --genre \"$(Genre)\" --comment \"$(Comment)\"");
	if (pref.GetValueBool(PI_FAAC_TNS)) cl += " --tns";
	if (!pref.GetValueBool(PI_FAAC_MIDSIDE)) cl += " --no-midside";
	if (pref.GetValueInt(PI_FAAC_SHORTCTL)) cl.AppendFormat(" --shortctl %d", pref.GetValueInt(PI_FAAC_SHORTCTL));
	cl += " -o \"$(DestFile)\" \"$(SourceFile)\"";
	return 0;
}

TCHAR* CFaac::GetVersionInfo()
{
	return GetEncoderVersionInfo(pref.GetValueStr(PI_FAAC_PATH), 0, 0);
}

int CFaac::GetTargetBitRate()
{
	return (pref.GetValueInt(PI_FAAC_MODE) == 0) ? 0 : pref.GetValueInt(PI_FAAC_BITRATE);
}

////////////////////////////////////////////////////////////////////////////////////
// AMR Encoder
////////////////////////////////////////////////////////////////////////////////////

static int getWBBitrateMode(int bitrate)
{
    static unsigned char rates[]={ 8, //6.6k
                         10,//8.85
                         13,//12.65
                         15,//14.25
                         17,//15.85
                         18,//18.25
                         22,//19.85
                         23,//23.05
                         24,//23.85
	};
    int i;
    for (i=0;i<9;i++)  {
        if(bitrate <= rates[i]) return(i);
    }
    /*Return highest possible*/
    return(8);
}

CAmr::CAmr()
{
	flags=FLAG_RAW_PCM;
	prefBase=PI_AMR;
	logger.SetModuleName(GetName());
}

int CAmr::GetLowestSampleRate()
{ 
	return pref.GetValueInt(PI_AMR_CODEC) == 0 ? 8000 : 16000;
}

int CAmr::GetHighestSampleRate()
{
	return pref.GetValueInt(PI_AMR_CODEC) == 0 ? 8000 : 16000;	
}

int CAmr::UpdateSettings()
{
	cl.Format("codecs\\amrnb-encoder.exe MR122 fd=0 \"$(DestFile)\"");
	return 0;
}

/*******************************************************************************
* PCM file writer
********************************************************************************/
CPcm::CPcm()
{
	hFile=NULL;
	prefBase=PI_PCM;
	logger.SetModuleName(GetName());
}

int CPcm::OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr)
{
	hFile=CreateFile(destfile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		hFile = 0;
		return -1;
	} else {
		return 0;
	}
}

int CPcm::CloseEncoder(RIFF_HEADER *wavhdr)
{
	DWORD bytes;
	if (hFile) {
		SetFilePointer(hFile,0,NULL,FILE_BEGIN);
		WriteFile(hFile,wavhdr,sizeof(RIFF_HEADER),&bytes,NULL);
		CloseHandle(hFile);
		hFile = 0;
	}
	return 0;
}

int CPcm::Encode(char *data, int bytes)
{
	DWORD bytesWritten;
	WriteFile(hFile,data,bytes,&bytesWritten,NULL);
	return (int)bytesWritten;
}

int CPcm::UpdateSettings()
{
	logger.Log(IDS_APPLY_SETTINGS);
	if (pref.GetValueInt(PI_PCM_FORMAT) == 0)
		flags&=~FLAG_RAW_PCM;
	else
		flags|=FLAG_RAW_PCM;
	return 0;
}
