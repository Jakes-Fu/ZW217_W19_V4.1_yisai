// stdafx.cpp : source file that includes just the standard includes
//	DspKernel.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
std::string Format(const char *lpszFmt, ...)
{
	const	size_t MAX_SIZE		= 1024;
	char	szString[MAX_SIZE]	= {0};
    va_list	args;
    va_start(args, lpszFmt);
    _vsnprintf(szString, sizeof(szString), lpszFmt, args);
    va_end(args);

    return std::string(szString);
}


std::string  GetLocalTime(void) 
{
	SYSTEMTIME systime;
	::GetLocalTime(&systime);
	char szTime[MAX_PATH] = {0};
	sprintf(szTime, "%04d-%02d-%02d-%02d-%02d-%02d-%03d", 
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);
	
	return std::string(szTime);
};