/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include <tchar.h>
#include "atlstr.h"
#include "paramlistdef.h"
#include "logger.h"
#include "mccore.h"
#include "mchelper.h"

#define INIFILE "mc.ini"
#define OUTPIPENAME "outpipe"

#define AUDIO_BLOCK_SIZE (16 * 1024)

extern FILE_TYPE filetypes[];
extern PFN_EVENT_HANDLER pfnEvent;

typedef struct {
	TCHAR* data;
	int bytes;
	unsigned int totalbytes;
	int length;
	unsigned short int count;
} AUDIO_ENCODE_DATA;

typedef enum {
	OA_ASK = 0,
	OA_OVERWRITE,
	OA_BACKUP,
	OA_SKIP,
	OA_NOTEXIST,
} OV_ACTION;

typedef struct {
	T_ITEM* item;
	CString destfile;
	CString apppath;
	bool fAudioDecodable;
	bool fAudioEncoding;
	bool fUseWaveFile;
	bool fNeedWrap;
	int commonPartLen;
	int audiochannel;
	TCHAR* filterbuf;
	TCHAR* audiodata;
	MC_AUDIO_FILTER *audiofilter;
	HWND hwndMain;
	int scrw;
	int scrh;
	int audioDelay;
	int audioFrameSize;
	RIFF_HEADER wavhdr;
	int bitrate;
	OV_ACTION ovaction;
	HANDLE hEvent;
} MC_INST_INTERNAL;

class CEncoder {
public:
	/* encoder properties ********************************************************/
	virtual TCHAR* GetName() = 0;
	virtual TCHAR* GetFullName() { return GetName(); }
	virtual TCHAR* GetOutExt() {return "out";}
	virtual TCHAR* GetInExts() {return 0;}
	virtual TCHAR* GetVersionInfo() { return 0; }
	virtual int UpdateSettings() { return 0; }
	virtual int GetProgress() { return -1; }
};

int EncodeAudioData(char* buf, int len);
int ReadFileTags(T_ITEM* item);
int WriteFileTags(TCHAR* destfile, T_ITEM* item);
void PurgeTags(TAGS* tags);
void QualifyVariables(CString& cmd, TCHAR *in);
void GetVariable(TCHAR* varNameString, CString& value);
void InitRiffHeader(RIFF_HEADER *hdr);
void SetThreadPriorityBySetting(HANDLE hThread);
void SetProcessPriorityBySetting(HANDLE hProcess);
TCHAR* GetEncoderVersionInfo(TCHAR* path, TCHAR* options, TCHAR* delimiter);
void GetOptionFPS(CString& str);
void GenAudioFilterOptions(CString& str, int srate);
void GenVideoFilterOptions(CString& str);
void GenCommonOptions(CString& str, T_ITEM* item);
void GenMplayerOptions(CString& str);

extern MC_STATE inst;
extern MC_INST_INTERNAL mcinst;
