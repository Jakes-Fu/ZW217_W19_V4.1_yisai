

#if !defined(__TRACEEX_H__)
#define __TRACEEX_H__

// TRACE信息输出至文件
#define TRACE_INFO_TO_FILE
// TRACE信息中包含文件名
//#define TRACE_CONTAIN_FILE_NAME

#ifdef TRACE_INFO_TO_FILE

#include <crtdbg.h>

#ifdef TRACE
	#undef TRACE   // 不使用
#endif // TRACE

#ifdef TRACE0
	#undef TRACE0
#endif // TRACE0

#ifdef TRACE_CONTAIN_FILE_NAME
	#define TRACE0(sz) _RPTF0(_CRT_WARN, _T(sz))
#else
	#define TRACE0(sz) _RPT0(_CRT_WARN, _T(sz))
#endif // TRACE_CONTAIN_FILE_NAME

#ifdef TRACE1
	#undef TRACE1
#endif // TRACE1

#ifdef TRACE_CONTAIN_FILE_NAME
	#define TRACE1(sz, p1) _RPTF1(_CRT_WARN, _T(sz), p1)
#else
	#define TRACE1(sz, p1) _RPT1(_CRT_WARN, _T(sz), p1)
#endif // TRACE_CONTAIN_FILE_NAME

#ifdef TRACE2
#undef TRACE2
#endif // TRACE02

#ifdef TRACE_CONTAIN_FILE_NAME
	#define TRACE2(sz, p1, p2) _RPTF2(_CRT_WARN, _T(sz), p1, p2)
#else
	#define TRACE2(sz, p1, p2) _RPT2(_CRT_WARN, _T(sz), p1, p2)
#endif // TRACE_CONTAIN_FILE_NAME

#ifdef TRACE3
#undef TRACE3
#endif // TRACE3

#ifdef TRACE_CONTAIN_FILE_NAME
	#define TRACE3(sz, p1, p2, p3) _RPTF3(_CRT_WARN, _T(sz), p1, p2, p3)
#else
	#define TRACE3(sz, p1, p2, p3) _RPT3(_CRT_WARN, _T(sz), p1, p2, p3)
#endif // TRACE_CONTAIN_FILE_NAME

#endif // TRACE_INFO_TO_FILE

class CTraceEx
{
public:
	CTraceEx();
	~CTraceEx();

public:
	static LPCTSTR GetTraceFileDir(void);

	static BOOL Init( LPCTSTR pTraceFileName );
	static void Release(void);

private:
	
private:
	static HANDLE m_hFile;
	static int    m_nCount;
};

//HANDLE CTraceEx::m_hFile = NULL;

#endif // __TRACEEX_H__