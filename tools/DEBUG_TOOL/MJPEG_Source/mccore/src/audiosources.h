/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#define AUDIO_SRC_COUNT 4

#define MS2BYTE(MS, BPS) (((BPS % 100 == 0) ? ((long long)MS * (BPS / 100) / 10) : ((long long)MS * (BPS / 10) / 100)) & (-8))
#define BYTE2MS(BYTES, BPS) ( BYTES * 10 / (BPS / 100) )

enum {
	AS_MPLAYER = 0,
	AS_WAVE,
	AS_COUNT,
};

class CAudioSource
{
public:
	virtual int open(T_ITEM* item, int startpos) = 0;
	virtual int extract(TCHAR* filename, TCHAR* outfile, int startpos) { return -1; }
	virtual int close() = 0;
	virtual int seek(int startpos) {return -1;}
	virtual int read(char* buffer, int bufsize);
	virtual int stop() {return -1;}
	virtual int GetPos();
	virtual TCHAR* GetName() = 0;
	virtual int prepare();
	CAudioSource();
	int state;
	unsigned long duration;
	unsigned long long bytescount;
	unsigned long long totalbytes;
	RIFF_HEADER hdr;
	int samplerate;
	int framesize;
	int prefBase;
protected:
	CLogger logger;
	HANDLE hInput;
};

class CWaveFileSource:public CAudioSource
{
public:
	CWaveFileSource();
	int open(T_ITEM* item, int startpos);
	int close();
	int seek(int startpos);
	TCHAR* GetName() { return "Waveform/PCM File"; }
};

class CAmrDecoder:public CAudioSource
{
public:
	CAmrDecoder();
	int open(T_ITEM* item, int startpos);
	int close();
	int read(char* buffer, int bytes);
	TCHAR* GetName() { return "AMR Decoder"; }
	int frames;
private:
	CDllHelper* dll;
	void* pfnDecode;
	FILE *fp;
	void * destate;
};

#define MP_DLL "mplayer.dll"
#define MP_EXE "mplayer.exe"

class CMplayerSource:public CAudioSource
{
public:
	CMplayerSource():sp(0),dll(0) { 
		prefBase = PI_MP; 
		logger.SetModuleName(GetName());
	}
	//int read(char* buffer, int bufsize);
	int extract(TCHAR* filename, TCHAR* outfile, int startpos);
	int open(T_ITEM* item, int startpos);
	int close();
	TCHAR* GetName() { return "MPlayer"; }
private:
	CDllHelper* dll;
	SHELL_PARAM* sp;
};

#endif
