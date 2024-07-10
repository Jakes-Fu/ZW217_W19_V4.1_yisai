class CArgs
{
public:
	void Set(char* string);
	CArgs():argv(0),data(0) {}
	~CArgs()
	{
		if (argv) free(argv);
		if (data) free(data);
	}
	char** argv;
	int argc;
private:
	char* data;
};

typedef int (*PFNMAIN)(int argc, char* argv[]);

class CDllHelper 
{
public:
	CDllHelper() { init(); }
	CDllHelper(TCHAR* file) { init(); LoadDLL(file); }
	CDllHelper(TCHAR* path, TCHAR* file) { init(); LoadDLL(path, file); }
	~CDllHelper() {	
		Unload();
		if (args) delete args;
	}
	HMODULE LoadDLL(TCHAR* path, TCHAR* file);
	HMODULE LoadDLL(TCHAR* file);
	void Unload();
	static DWORD WINAPI MainThread(void* ptrthis);
	int StartMain(char* cmdline);
	int Stop();
	void* GetAddr(char* funcname);
	bool IsThreadAlive();
	DWORD Wait(int timeout) { return WaitForSingleObject(hThread, timeout); }
	HMODULE hDll;
	HANDLE hThread;
	PFNMAIN pfnmain;
private:
	void init() {
		hDll = 0;
		pfnmain = 0;
		args = 0;
		hThread = 0;
	}
	CArgs* args;
};
