#ifndef _MV_FS_INTERFACE_H
#define _MV_FS_INTERFACE_H

#include "_Types.h"
#include "_IO.h"
#define  MAX_FILENAME_LEN 256

typedef enum{
	modeRead = 0x00,
	modeWrite = 0x01,
	modeReadWrite = 0x02,
	modeCreate = 0x04
}FILE_OPEN_MODE_E;

enum{
	fromBegin,
	fromEnd,
	fromCurrent
};



_HANDLE CM_File_Open(_WCHAR* wstrFilename, _UINT32 dwMode);

_VOID CM_File_Close(_HANDLE hFile);

_INT CM_File_Read(_HANDLE hFile, _VOID* pBuffer, _INT nCount);

_INT CM_File_Write(_HANDLE hFile,_VOID* pBuffer,_INT nCount);

_VOID CM_File_Flush(_HANDLE hFile);

_INT CM_File_GetLength(_HANDLE hFile);

_BOOLEAN CM_File_Seek(_HANDLE hFile, _INT nOffset,_INT nFrom);

_INT CM_File_GetPosition(_HANDLE hFile);

_BOOLEAN CM_FileExists(_WCHAR* wstrFileName);

_BOOLEAN CM_MakeFolder(_CONST _WCHAR* wstrFolderName);

_BOOLEAN CM_DeleteFolder(_CONST _WCHAR* wstrFolderName);

_BOOLEAN CM_MoveFile(_CONST _WCHAR* wstrExistingFileName,_CONST _WCHAR* wstrNewFileName );

_HANDLE CM_FindFirstFile(const _WCHAR* wstrFilter, FINDFILE_DATA* pFindData);

_BOOLEAN CM_FindNextFile(_HANDLE hFind,FINDFILE_DATA* pFindData);

_BOOLEAN CM_FindClose(_HANDLE bFind);

_INT CM_File_Time(_WCHAR* wstrFilename ,_TIME* fileTime);

_BOOLEAN CM_DeleteFile(_CONST _WCHAR* wstrFileName);

_INT CM_GetCurFolder(_WCHAR* wstrBuffer, _INT nBufferSize);

_BOOLEAN CM_SetCurFolder(_WCHAR* wstrBuffer);

_BOOLEAN  CM_ReName(_CONST _WCHAR* wstrExistingFileName, _CONST _WCHAR* wstrNewFileName);

_BOOLEAN CM_IsTCardExist();
#endif