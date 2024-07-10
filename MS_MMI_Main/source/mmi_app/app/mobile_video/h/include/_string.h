//
//	_String.h
//
#ifndef	__STRING_API_H
#define	__STRING_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

extern _INT _strlen(_CONST _CHAR* string);
extern _INT _wcslen(_CONST _WCHAR* string);

extern _CHAR* _strchr(_CONST _CHAR* string, _CHAR c);
extern _WCHAR* _wcschr(_CONST _WCHAR *string, _WCHAR c);

extern _CHAR* _strstr(_CONST _CHAR* string, _CONST _CHAR* strCharSet);
extern _WCHAR* _wcsstr(_CONST _WCHAR *string, _CONST _WCHAR* strCharSet);

extern _CHAR* _strcpy(_CHAR* strDestination, _CONST _CHAR* strSource);
extern _WCHAR* _wcscpy(_WCHAR* strDestination, _CONST _WCHAR* strSource);

extern _CHAR* _strncpy(_CHAR* strDestination, _CONST _CHAR* strSource, _INT nCount);
extern _WCHAR* _wcsncpy(_WCHAR* strDestination, _CONST _WCHAR* strSource, _INT nCount);

extern _CHAR* _strcat(_CHAR* strDestination, _CONST _CHAR* strSource);
extern _WCHAR* _wcscat(_WCHAR* strDestination, _CONST _WCHAR* strSource);

extern _CHAR* _strdump(_CONST _CHAR* strSource);
extern _WCHAR* _wcsdump(_CONST _WCHAR* strSource);

extern _INT _strcmp(_CONST _CHAR* string1, _CONST _CHAR* string2);
extern _INT _wcscmp(_CONST _WCHAR* string1, _CONST _WCHAR* string2);
extern _INT _strncmp(_CONST _CHAR* string1, _CONST _CHAR* string2, _INT n);
extern _INT _wcsncmp(_CONST _WCHAR* string1, _CONST _WCHAR* string2, _INT n);

extern _INT _strcmpnocase(_CONST _CHAR* string1, _CONST _CHAR* string2);
extern _INT _wcscmpnocase(_CONST _WCHAR* string1, _CONST _WCHAR* string2);

extern _CHAR* _strlwr(_CHAR* string);
extern _WCHAR* _wcslwr(_WCHAR* string);
extern _CHAR* _strupr(_CHAR* string);
extern _WCHAR* _wcsupr(_WCHAR* string);

extern _CHAR _chrlwr(_CHAR c);
extern _WCHAR _wchrlwr(_WCHAR c);
extern _CHAR _chrupr(_CHAR c);
extern _WCHAR _wchrupr(_WCHAR c);

extern _CHAR* _strtrim(_CHAR* string);
extern _CHAR* _strlefttrim(_CHAR* string);
extern _CHAR* _strrighttrim(_CHAR* string);
extern _WCHAR* _wcstrim(_WCHAR* string);
extern _WCHAR* _wcslefttrim(_WCHAR* string);
extern _WCHAR* _wcsrighttrim(_WCHAR* string);


extern _BOOLEAN _isdigit(_CHAR c);
extern _BOOLEAN _iswdigit(_WCHAR c);

extern _INT _strtol(_CONST _CHAR* nptr, _CHAR** endptr, _INT base);
extern _INT _wcstol(_CONST _WCHAR* nptr, _WCHAR** endptr, _INT base);
extern _INT _atol(_CONST _CHAR* str);
extern _INT _watol(_CONST _WCHAR* str);

extern _CHAR* _ltostring(_INT value, _CHAR* string, _INT radix);
extern _WCHAR* _ltowstring(_INT value, _WCHAR* string, _INT radix);

extern _INT _MultiBytesToWideChar(_UINT, _CHAR*, _INT, _WCHAR*, _INT);
extern _INT _WideCharToMultiBytes(_UINT, _WCHAR*, _INT, _CHAR*, _INT);


#ifdef	__cplusplus
}
#endif

#endif
