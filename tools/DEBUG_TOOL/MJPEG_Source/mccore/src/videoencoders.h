#define MAX_PASS_COUNT 3

class CVideoEncoder : public CEncoder
{
public:
	virtual int OpenEncoder(TCHAR* sourcefile, TCHAR* destfile) { return -1; }
	virtual int Encode(CString& cmd) { return -1; }
	virtual int CloseEncoder() { return -1; }
	virtual int UpdateSettings() { return 0; }
	virtual int GetPassCount() { return passcount; }
	virtual TCHAR* GetCommand(int pass) {
		return pass < passcount ? cmds[pass].GetBuffer() : 0;
	}
	virtual bool CheckAudioSupport(int id) { return true; }
	virtual bool CheckContainerSupport(int id) { return true; }
	virtual TCHAR* GetVersionInfo() { return 0; }
	CVideoEncoder():passcount(0),prefBase(0),stats(0) {}
	MP_STATS* stats;
protected:
	CString cmds[MAX_PASS_COUNT];
	int passcount;
	int prefBase;
private:
};

#define ME_DLL "mencoder.dll"
#define ME_EXE "mencoder.exe"

class CDllHelper;

class CMencoder : public CVideoEncoder
{	
public:
	CMencoder():dll(0) {}
	int OpenEncoder(TCHAR* sourcefile, TCHAR* destfile);
	int Encode(CString& cmd);
	int CloseEncoder();
	int GetProgress();
	TCHAR* GetName() { return "MEncoder"; }
	TCHAR* GetOutExt();
	int UpdateSettings();
	bool CheckAudioSupport(int id);
	bool CheckContainerSupport(int id);
	TCHAR* GetVersionInfo();
private:
	void GetAudioOptions(CString& str);
	void ParseConsole(char* consoleText, int consoleBytes);
	bool fLoadDll;
	CDllHelper* dll;
	SHELL_PARAM* sp;
};

class Cffmpeg : public CVideoEncoder
{	
public:
	int OpenEncoder(TCHAR* sourcefile, TCHAR* destfile);
	int Encode(CString& cmd);
	int CloseEncoder();
	TCHAR* GetName() { return "FFMPEG"; }
	TCHAR* GetOutExt() { return "avi"; }
	int GetProgress();
	int UpdateSettings();
	bool CheckAudioSupport(int id);
	bool CheckContainerSupport(int id);
private:
	SHELL_PARAM sp;
};

class Cx264 : public CVideoEncoder
{	
public:
	TCHAR* GetName() { return "X264"; }
	TCHAR* GetOutExt() { return "264"; }
};