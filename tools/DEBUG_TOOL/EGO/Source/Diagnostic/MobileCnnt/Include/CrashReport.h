// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CRASHREPORT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CRASHREPORT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CRASHREPORT_EXPORTS
#define CRASHREPORT_API __declspec(dllexport)
#else
#define CRASHREPORT_API __declspec(dllimport)
#endif

typedef int ( *HOOKCALLBACK)(_EXCEPTION_POINTERS*);

enum
{
	CR_ERROR_SUCCESS_HOOK,				// Success
	CR_ERROR_INVALID_PARAMERTER,		// Invalid parameter
	CR_ERROR_UNICODE_CONVERT,			// Can not convert to unicode version
	CR_ERROR_ASSERT_HOOK,				// Can not install a assert hook
	CR_ERROR_CRASH_HOOK,				// Can not install a crash hook
	CR_ERROR_CREATE_DUMP_FILE,			// Can not open file for dump
	CR_ERROR_ALREADY_HOOKED,			// Already call hook function
	CR_ERROR_UNKNOWN,
};

#define CRCB_SUCCESS			0
// The call back function has its own msg box,the default msg box will not be displayed
#define CRCB_NEED_NO_PROCESS	1		

// Unicode version
extern "C"
CRASHREPORT_API DWORD InitCrashReportW( PWSTR lpszVersion,bool bHookAssert,PWSTR lpszDumpFile,HOOKCALLBACK lpCallback );

// Ansi version
extern "C"
CRASHREPORT_API DWORD InitCrashReportA( PSTR lpszVersion,bool bHookAssert,PSTR lpszDumpFile,HOOKCALLBACK lpCallback );

// Unicode version for application build with vc6
extern "C"
CRASHREPORT_API DWORD InitCrashReportOldW( PWSTR lpszVersion,bool bHookAssert,PWSTR lpszDumpFile,HOOKCALLBACK lpCallback );

// Ansi version for application build with vc6
extern "C"
CRASHREPORT_API DWORD InitCrashReportOldA( PSTR lpszVersion,bool bHookAssert,PSTR lpszDumpFile,HOOKCALLBACK lpCallback );


#ifdef _UNICODE
#if _MSC_VER > 1200
#define InitCrashReport InitCrashReportW
#else
#define InitCrashReport InitCrashReportOldW
#endif // _MSC_VER
#else
#if _MSC_VER > 1200
#define InitCrashReport InitCrashReportA
#else
#define InitCrashReport InitCrashReportOldA
#endif // _MSC_VER
#endif // _UNICODE