#include <time.h>
#include "atlstr.h"
#include "logger.h"

#define CRLF "\r\n"
#ifdef LOG_ENABLE_YEAR
#define LOGFMT "[%04d-%02d-%02d %02d:%02d:%02d] "
#else
#define LOGFMT "[%02d-%02d %02d:%02d:%02d] "
#endif

#define MAX_LOG_SIZE (16 * 1024)
static CString logger;
static FILE* fplog = 0;

void CLogger::WriteLog(TCHAR* content)
{
	struct tm *btm;
	time_t now = time(0);
	if (logger.GetLength() > MAX_LOG_SIZE) {
		int i = 0;
		while (i >= 0 && i < (MAX_LOG_SIZE / 4 * 3)) {
			i = logger.Find('\n', i + 1);
		}
		if (i < 0) {
			logger = "";
		} else {
			logger.Delete(0, i + 1);
		}
	}
	btm=localtime(&now);
	int len = logger.GetLength();
	logger.AppendFormat(LOGFMT,
#ifdef LOG_ENABLE_YEAR
		1900+btm->tm_year,
#endif
		btm->tm_mon,
		btm->tm_mday,
		btm->tm_hour,
		btm->tm_min,
		btm->tm_sec);

	if (modname) {
		logger.AppendFormat("[%s] ", modname);
	}
	logger += content;
	logger += CRLF;
	// write to file
	if (fplog) {
		fprintf(fplog, "%s", logger.Mid(len));
	}
}

void CLogger::SetModuleName(TCHAR* modulename)
{
	if (modname) free(modname);
	modname = strdup(modulename);
}

void CLogger::Log(CString& msg)
{
	WriteLog(msg.GetBuffer());
}

void CLogger::Log(TCHAR* msg)
{
	WriteLog(msg);
}

void CLogger::Log(int id)
{
	CString msg;
	msg.Format(id);
	WriteLog(msg.GetBuffer());
}

void CLogger::Log(int id, TCHAR* who)
{
	CString msg;
	msg.Format(id, who);
	WriteLog(msg.GetBuffer());
}

void CLogger::Log(int id, int val)
{
	CString msg;
	msg.Format(id, val);
	WriteLog(msg.GetBuffer());
}

TCHAR* CLogger::GetLog(int maxsize)
{
	return logger.GetBuffer();
}

void CLogger::SetLog(TCHAR* logtext)
{
	logger = logtext;
}

void CLogger::Init()
{
}

void CLogger::Init(TCHAR* logfile)
{
	fplog = fopen(logfile, "a");
}

void CLogger::Uninit()
{
	if (fplog) fclose(fplog);
}
