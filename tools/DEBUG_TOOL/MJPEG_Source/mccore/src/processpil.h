/*******************************************************************
* Process Platform Independent Layer
* Copyright (c) 2005 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#define SHELL_ALLOC 0x1
#define SHELL_SHOW_WINDOW 0x2
#define SHELL_REDIRECT_STDIN 0x4
#define SHELL_REDIRECT_STDOUT 0x8
#define SHELL_REDIRECT_STDERR 0x10
#define SHELL_READ_STDOUT_ALL 0x20
#define SHELL_CONVERT_LF 0x40

typedef struct {
#ifdef WIN32
	TCHAR *pchCommandLine;
	TCHAR *pchCurDir;
	TCHAR *pchPath;
	HANDLE fdRead;
	HANDLE fdWrite;
	PROCESS_INFORMATION piProcInfo;
#else
	char *pchCommandLine;
	char *pchCurDir;
	char *pchPath;
	int fdRead;
	int fdWrite;
	int pid;
#endif
	char *buffer;
	int iBufferSize;
	int iDelimiter;
	unsigned int flags;
}SHELL_PARAM;

#ifdef __cplusplus
extern "C" {
#endif
int ShellRead(SHELL_PARAM* param, int timeout);
void ShellClean(SHELL_PARAM* param);
int ShellWait(SHELL_PARAM* param, int iTimeout);
int ShellExec(SHELL_PARAM* param);
int ShellTerminate(SHELL_PARAM* param);
int ShellSetPriority(SHELL_PARAM* param, int iPriority);
#ifdef __cplusplus
}
#endif

