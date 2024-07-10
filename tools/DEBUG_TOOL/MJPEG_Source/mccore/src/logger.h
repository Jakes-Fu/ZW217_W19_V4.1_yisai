class CLogger
{
public:
	CLogger(TCHAR* modulename) { modname = strdup(modulename); }
	CLogger():modname(0) {}
	~CLogger() { if (modname) free(modname); }
	void SetModuleName(TCHAR* modulename);
	void Log(TCHAR* msg);
	void Log(CString& msg);
	void Log(int id);
	void Log(int id, TCHAR* who);
	void Log(int id, int val);
	static TCHAR* GetLog(int maxsize);
	static void SetLog(TCHAR* logtext);
	static void Init();
	static void Init(TCHAR* logfile);
	static void Uninit();
private:
	void WriteLog(TCHAR* content);
	TCHAR* modname;
};