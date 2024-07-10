#ifndef  _UTIL_H_
#define  _UTIL_H_

/* little_endian and big_endian convert */
DWORD ConvertEndian(DWORD  t);

CString	GetModuleDir(BOOL bPostfix = TRUE);
BOOL CreateDir( LPCTSTR lpPathName);
BOOL CopyDirectory(LPCTSTR lpSrcPath,LPCTSTR lpDestPath);
BOOL MoveDirectory(LPCTSTR lpSrcPath,LPCTSTR lpDestPath);
#endif //_UTIL_H_