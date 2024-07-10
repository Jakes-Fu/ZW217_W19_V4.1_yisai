#define MAX_TRACKS 16

class CContainer : public CEncoder
{
public:
	virtual TCHAR* UpdateSettings(TCHAR* destfile, CONTAINER_TRACKS tracks[]) { return 0; }
	virtual int Mux() { return 0; }
	virtual bool CheckAudioSupport(int id) { return true; }
	virtual bool CheckVideoSupport(int id) { return true; }
	virtual bool IsExternalMuxer() { return false; }
	virtual int ExtractTrack(TCHAR* srcfile, CString& destfile, int trackno) { return 0; }
private:
};

class CDefContainer : public CContainer
{
public:
	TCHAR* GetOutExt();
	TCHAR* GetName() { return "Default"; }
};

class CMp4box : public CContainer
{
public:
	int Mux();
	TCHAR* UpdateSettings(TCHAR* destfile, CONTAINER_TRACKS tracks[]);
	TCHAR* GetOutExt();
	TCHAR* GetName() { return "MP4"; }
	TCHAR* GetVersionInfo();
	int ExtractTrack(TCHAR* srcfile, CString& destfile, int trackno);
	bool IsExternalMuxer() { return true; }
	int GetProgress();

protected:
    char* destfile;
    CString cmd;
    SHELL_PARAM sp;
	char* videofile;
};

// container for gpconverter
class CSpMp4 : public CMp4box
{
public:
	int Mux();
	int GetProgress();
};