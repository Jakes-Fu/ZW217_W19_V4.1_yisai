/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef AUDIOCODEC_H
#define AUDIOCODEC_H

// audio encoder flags
#define FLAG_RAW_PCM 0x1
#define FLAG_STANDALONE 0x4
#define FLAG_READ_CONSOLE 0x8
#define FLAG_PIPE 0x10
#define FLAG_TO_TAG 0x20
#define FLAG_NO_ENCODER 0x40
#define FLAG_QUIET 0x80

#include "enc_if.h"
#include "MACDll.h"

class CDllHelper;

class CAudioEncoder : public CEncoder
{
public:
	/* encoder methods ***********************************************************/
	virtual int OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr);
	virtual int Encode(char *data, int bytes);
	virtual int CloseEncoder(RIFF_HEADER *wavhdr);
	virtual int GetProgress();
	int flags;
	int prefBase;
	/* encoder properties ********************************************************/
	virtual int GetLowestSampleRate() { return 8000; }
	virtual int GetHighestSampleRate() { return 48000; }
	virtual int GetMinChannel() { return 1; }
	virtual int GetMaxChannel() { return 2; }
	virtual int GetTargetBitRate() { return 0; }
	int framesize;
	/*****************************************************************************/
	CAudioEncoder():flags(0),prefBase(0),percent(0),framesize(0) {}
	CString cl;
protected:
	CLogger logger;
	SHELL_PARAM sp;
	int percent;
};

typedef AudioCoder* (*CREATEAUDIO3TYPE)(int nch, int srate, int bps, unsigned int srct, unsigned int *outt, TCHAR *configfile);
typedef unsigned int (*GETAUDIOTYPES3TYPE)(int idx, TCHAR *desc);

class CWinampEnc
{
public:
	int open(CString& encplugfile, CString& destfile, CString& conffile, RIFF_HEADER* wavhdr);
	int encode(char* data, int bytes);
	int close();
	CWinampEnc():encoder(0),outtype(1346584897) { logger.SetModuleName("WAENC"); }
	unsigned int outtype;
private:
	CLogger logger;
	HINSTANCE hEncPlugin;
	CREATEAUDIO3TYPE CreateAudio3;
	GETAUDIOTYPES3TYPE GetAudioTypes3;
	AudioCoder *(*finishAudio3)(TCHAR *fn, AudioCoder *c);
	void (*PrepareToFinish)(const TCHAR *filename, AudioCoder *coder);
	AudioCoder* encoder;
	FILE* fp;
	CString outfile;
};

class CLameMP3:public CAudioEncoder
{
public:
	CLameMP3();
	TCHAR* GetName() {return "Lame";}
	TCHAR* GetInExts() {return "wav";}
	TCHAR* GetFullName() { return "Lame MP3"; }
	TCHAR* GetOutExt() {return "mp3";}
	TCHAR* GetVersionInfo();
	int GetTargetBitRate();
	int UpdateSettings();
};

class CAACPlus:public CAudioEncoder
{
public:
	CAACPlus();
	TCHAR* GetName() {return "CT AAC+";}
	TCHAR* GetFullName() { return "CT AAC+ V2"; }
	TCHAR* GetOutExt() {return "aac";}
	int OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr);
	int Encode(char *buffer, int bytes);
	int CloseEncoder(RIFF_HEADER *wavhdr);
	int GetLowestSampleRate() { return 32000; }
	int GetTargetBitRate();
private:
	CWinampEnc waenc;
};

typedef int (*pfnAacPlusEncode)(char *data, int bytes);
typedef int (*pfnAacPlusInit)(char *outfile, int bitrate, int useParametric, FORMAT_CHUNK *fmt);
typedef int (*pfnAacPlusFinish)();

#define MODE_AUTO 0
#define MODE_STEREO 1
#define MODE_STEREO_NOPARAM 2
#define MODE_MONO 3

class C3gppAacPlus:public CAudioEncoder
{
public:
	C3gppAacPlus();
	TCHAR* GetName() {return "3GPP AAC+";}
	TCHAR* GetFullName() {return "3GPP AAC+ V2";}
	TCHAR* GetOutExt() {return "m4a";}
	int OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr);
	int Encode(char *buffer, int bytes);
	int CloseEncoder(RIFF_HEADER *wavhdr);
	int UpdateSettings();
	int GetLowestSampleRate() { return 32000; }
	int GetTargetBitRate();
private:
	CDllHelper* dll;
	pfnAacPlusEncode AacPlusEncode;
	pfnAacPlusFinish AacPlusFinish;
};

class CFaac:public CAudioEncoder
{
public:
	CFaac();
	TCHAR* GetName() {return "AAC";}
	TCHAR* GetOutExt() {return "m4a";}
	TCHAR* GetVersionInfo();
	int GetTargetBitRate();
	int UpdateSettings();
};

class CHelix:public CAudioEncoder
{
public:
	CHelix();
	TCHAR* GetName() {return "Helix";}
	TCHAR* GetFullName() { return "Helix Producer"; }
	TCHAR* GetOutExt();
	TCHAR* GetInExts() {return "wav";}
	TCHAR* GetVersionInfo();
	int UpdateSettings();
	int GetProgress();
};

class CAmr:public CAudioEncoder
{
public:
	CAmr();
	TCHAR* GetName() {return "AMR";}
	TCHAR* GetFullName() { return "AMR"; }
	TCHAR* GetOutExt() {return "amr";}
	int UpdateSettings();
	int GetMaxChannel() { return 1; }
	int GetLowestSampleRate();
	int GetHighestSampleRate();
};

class CWma:public CAudioEncoder
{
public:
	CWma();
	TCHAR* GetName() {return "WMA";}
	TCHAR* GetFullName() { return "Windows Media Audio"; }
	TCHAR* GetOutExt() {return "wma";}
	TCHAR* GetInExts() {return "mp3;wav;mpg;avi";}
	int GetTargetBitRate();
	int UpdateSettings();
};

class CGeneric:public CAudioEncoder
{
public:
	CGeneric();
	TCHAR* GetName() {return "CLI";}
	TCHAR* GetFullName() { return "Generic CLI Encoder"; }
	TCHAR* GetOutExt();
	int UpdateSettings();
};

class CWavPack:public CAudioEncoder
{
public:
	CWavPack();
	TCHAR* GetName() {return "WavPack";}
	TCHAR* GetOutExt() {return "wv";}
	TCHAR* GetVersionInfo();
	int UpdateSettings();
};

class CFlac:public CAudioEncoder
{
public:
	CFlac();
	TCHAR* GetName() {return "FLAC";}
	TCHAR* GetFullName() {return "Free Lossless Audio Codec";}
	TCHAR* GetOutExt() {return "flac";}
	TCHAR* GetVersionInfo();
	int UpdateSettings();
};

////////////////////////////////////////////////////////////////////////////////////
// Monkey's Audio Encoder 
////////////////////////////////////////////////////////////////////////////////////

// MAC_DLL structure (holds function pointers)
typedef struct {
	// APECompress functions
	proc_APECompress_Create			Create;
	proc_APECompress_Destroy		Destroy;
	proc_APECompress_Start			Start;
	proc_APECompress_AddData		AddData;
	proc_APECompress_Finish			Finish;
} MAC_COMPRESS_DLL;

class CApe:public CAudioEncoder
{
public:
	CApe();
	int OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr);
	int Encode(char *data, int bytes);
	int CloseEncoder(RIFF_HEADER *wavhdr);
	TCHAR* GetName() {return "APE";}
	TCHAR* GetFullName() { return "Monkey's Audio"; }
	TCHAR* GetOutExt() {return "ape";}
private:
	int GetFunctions(HMODULE hMACDll);
	int VersionCheckInterface(HMODULE hMACDll);
	HMODULE hMACDll;
	MAC_COMPRESS_DLL MACDll;
	APE_COMPRESS_HANDLE hAPECompress;
};

class CVorbis:public CAudioEncoder
{
public:
	CVorbis();
	TCHAR* GetName() {return "Vorbis";}
	TCHAR* GetFullName() { return "Ogg/Vorbis"; }
	TCHAR* GetOutExt() {return "ogg";}
	TCHAR* GetVersionInfo();
	int GetTargetBitRate();
	int UpdateSettings();
};

class CPcm:public CAudioEncoder
{
public:
	CPcm();
	int OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr);
	int CloseEncoder(RIFF_HEADER *wavhdr);
	int Encode(char *buffer, int bytes);
	TCHAR* GetName() {return "PCM";}
	TCHAR* GetFullName() { return "Waveform/PCM"; }
	TCHAR* GetOutExt() {return (flags&FLAG_RAW_PCM)?"pcm":"wav";}
	int UpdateSettings();
private:
	HANDLE hFile;
};

class CMusePack:public CAudioEncoder
{
public:
	CMusePack();
	TCHAR* GetName() {return "MusePack";}
	TCHAR* GetOutExt() {return "mpc";}
	TCHAR* GetVersionInfo();
	int UpdateSettings();
};

class COptimFrog:public CAudioEncoder
{
public:
	COptimFrog();
	TCHAR* GetName() {return "OptimFrog";}
	TCHAR* GetInExts() {return "wav";}
	TCHAR* GetOutExt() {return "ofr";}
	TCHAR* GetVersionInfo();
	int UpdateSettings();
};

class CItunesEnc:public CAudioEncoder
{
public:
	CItunesEnc();
	TCHAR* GetName() {return "iTunesEncode";}
	TCHAR* GetInExts() {return "wav;mp3;m4a;wma;aiff";}
	TCHAR* GetOutExt() {return "m4a";}
	int UpdateSettings();
};

class CHelixMP3:public CAudioEncoder
{
public:
	CHelixMP3();
	TCHAR* GetName() {return "HelixMP3";}
	TCHAR* GetFullName() {return "Helix MP3 Encoder";}
	TCHAR* GetInExts() {return "wav";}
	TCHAR* GetOutExt() {return "mp3";}
	TCHAR* GetVersionInfo();
	int GetTargetBitRate();
	int UpdateSettings();
};

class CNeroAAC:public CAudioEncoder
{
public:
	CNeroAAC();
	int OpenEncoder(CString& cmd, CString& destfile, RIFF_HEADER *wavhdr);
	int Encode(char *data, int bytes);
	int CloseEncoder(RIFF_HEADER *wavhdr);
	TCHAR* GetName() {return "Nero AAC";}
	TCHAR* GetFullName() {return "Nero AAC Encoder";}
	TCHAR* GetOutExt() {return "m4a";}
	int GetDLLDir(TCHAR* buf, int bufsize);
private:
	int SetOptions();
	HMODULE hAacEncDLL;
	HMODULE hAacDLL;
};

class CFFMpegAudio:public CAudioEncoder
{
public:
	CFFMpegAudio();
	TCHAR* GetName() {return "FFMPEG";}
	TCHAR* GetFullName() { return "FFMPEG Audio Codecs"; }
	TCHAR* GetOutExt();
	TCHAR* GetVersionInfo();
	int GetTargetBitRate();
	int UpdateSettings();
	TCHAR* GetCodecName();
};

class CAudioCopy:public CAudioEncoder
{
public:
	TCHAR* GetName() {return "Copy";}
};

class CAudioNone:public CAudioEncoder
{
public:
	//@richardguan
	//TCHAR* GetName() {return "No Audio";}
	TCHAR* GetName() {return "≤ª ‰≥ˆ“Ù∆µ";}
};


#endif
