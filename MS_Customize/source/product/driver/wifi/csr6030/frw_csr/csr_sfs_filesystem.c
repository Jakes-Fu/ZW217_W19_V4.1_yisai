/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "sfs.h"

#include "csr_file.h"
#include "csr_dir.h"
#include "csr_util.h"
#include "csr_unicode.h"
#include "csr_pmem.h"

/*lint -e835*/
#define SFS_INVALID_HANDLE 0

extern void CsrDebugTraceHigh(const char* const format, ...);

static int returnMappedFlags(CsrFileOpenFlags mode)
{
    CsrFileOpenFlags ret = 0;

    ret |= (mode & CSR_FILE_OPEN_FLAGS_APPEND)       ? SFS_MODE_APPEND : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_READ_ONLY)    ? SFS_MODE_READ : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_READ_WRITE)   ? SFS_MODE_WRITE |  SFS_MODE_READ  : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_WRITE_ONLY)   ? SFS_MODE_WRITE : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_CREATE)       ? SFS_MODE_CREATE_NEW  : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_TRUNCATE)     ? SFS_MODE_CREATE_ALWAYS  : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_EXCL)         ? SFS_MODE_CREATE_NEW   : 0;

    return ret;
}

typedef struct
{
    SFS_HANDLE handle;
} csrFileHandle;


#define MAP_FILE_OPEN(result) (result != SFS_INVALID_HANDLE ? CSR_RESULT_SUCCESS : CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileOpen(CsrFileHandle **handle, const CsrUtf8String * fileName, CsrFileOpenFlags flags, CsrFilePerms perms)
{
    int linFlags = returnMappedFlags(flags);
    csrFileHandle *fsalHandle = CsrPmemAlloc(sizeof(csrFileHandle));
    SFS_HANDLE fd;
    CsrUtf16String *unicodeName;

    unicodeName = CsrUtf82Utf16String(fileName);
    fd = SFS_CreateFile((uint16 *)fileName, linFlags, NULL, NULL);
    CsrPmemFree(unicodeName);

    if (fd != SFS_INVALID_HANDLE)
    {
        fsalHandle->handle = fd;
        *handle = fsalHandle;
    }
    else
    {
        CsrPmemFree(fsalHandle);
        *handle = NULL;
    }

    return MAP_FILE_OPEN(fd);
}

CsrResult CsrFileClose(CsrFileHandle *handle)
{
    csrFileHandle *fsalHandle = handle;
	
    SFS_CloseFile(fsalHandle->handle);
    CsrPmemFree(handle);
    return CSR_RESULT_SUCCESS;
}

static CsrResult mapSFSResult2CSR(SFS_ERROR_E result)
{
    CsrResult ret = CSR_FILE_RESULT_FAILURE;
    
	switch (result)
	{
		case SFS_ERROR_NONE:
		{
			ret = CSR_RESULT_SUCCESS;
			break;
		}

		case SFS_ERROR_DEVICE:
		{
			ret = CSR_FILE_RESULT_FAILURE;
			break;
		}

		case SFS_ERROR_SYSTEM:
		{
			ret = CSR_FILE_RESULT_FAILURE;
			break;
		}
			
		case SFS_ERROR_INVALID_PARAM:
		{
			ret = CSR_FILE_RESULT_NOT_ALLOWED;
			break;
		}
		case SFS_ERROR_NO_ENOUGH_RESOURCE:
		{
			ret = CSR_FILE_RESULT_NO_SPACE;
			break;
		}

		case SFS_ERROR_IO_PENDING:
		{
			ret = CSR_FILE_RESULT_NOT_ALLOWED;
			break;
		}
		case SFS_ERROR_ACCESS:
		{
			ret = CSR_FILE_RESULT_NOT_ALLOWED;
			break;
		}

		case SFS_ERROR_NOT_EXIST:
		{
			ret = CSR_FILE_RESULT_NOT_EXIST;
			break;
		}
		case SFS_ERROR_HAS_EXIST:
		{
			ret = CSR_FILE_RESULT_ALREAD_EXISTS;
			break;
		}
		default:
		{
			ret = CSR_FILE_RESULT_FAILURE;			
			break;
		}
	}

	return ret;
}

CsrResult CsrFileWrite(const void *buffer, CsrSize bytesToWrite, 
                  CsrFileHandle *handle, CsrSize *written)
{
    SFS_ERROR_E result;
    csrFileHandle *fsalHandle = handle;

    result = SFS_WriteFile(fsalHandle->handle, buffer, bytesToWrite, written, NULL);

    return mapSFSResult2CSR(result);
}

CsrResult CsrFileRead(void *buffer, CsrSize bytesToRead, 
                 CsrFileHandle *handle, CsrSize *bytesRead)
{
    SFS_ERROR_E result;
    csrFileHandle *fsalHandle = handle;

    result = SFS_ReadFile(fsalHandle->handle, buffer, bytesToRead, bytesRead, NULL);

    if (result == SFS_ERROR_NONE && *bytesRead > 0)
    {
        return CSR_RESULT_SUCCESS;
    }
    return mapSFSResult2CSR(result);
}

CsrResult CsrFileSeek(CsrFileHandle *handle, CsrInt32 offset, CsrInt32 relativeOffset)
{
    csrFileHandle *fsalHandle = handle;
    int o;
    SFS_ERROR_E result;

    switch(relativeOffset)
    {
        case CSR_SEEK_SET:
            o=SFS_SEEK_BEGIN;
            break;

        case CSR_SEEK_CUR:
            o=SFS_SEEK_CUR;
            break;
            
        case CSR_SEEK_END:
            o=SFS_SEEK_END;
            break;
            
        default:
            o=SFS_SEEK_BEGIN;
            break;
    }

    result = SFS_SetFilePointer(fsalHandle->handle, (__int64)offset, o); /*lint !e747*/
    return mapSFSResult2CSR(result);
}

CsrResult CsrFileFlush(CsrFileHandle *handle)
{
    CsrDebugTraceHigh("CsrFileFlush NOT IMPLEMENTED");
    return CSR_FILE_RESULT_FAILURE;
}

CsrResult CsrFileSetEndOfFile(CsrFileHandle *handle)
{
    CsrDebugTraceHigh("CsrFileSetEndOfFile NOT IMPLEMENTED");
    return CSR_FILE_RESULT_FAILURE;
}

CsrResult CsrFileSetPerms(const CsrUtf8String * name, CsrFilePerms perms) /* NB: May also be used on directories */
{

    CsrDebugTraceHigh("CsrFileSetPerms NOT IMPLEMENTED");
    return CSR_FILE_RESULT_FAILURE;
}

CsrResult CsrFileRemove(const CsrUtf8String *fileName)
{
    SFS_ERROR_E result;
    CsrUtf16String *unicodeName;

    unicodeName = CsrUtf82Utf16String(fileName);

    result = SFS_DeleteFile((uint16 *) unicodeName, NULL);
    CsrPmemFree(unicodeName);
    return mapSFSResult2CSR(result);
}

CsrResult CsrFileRename(const CsrUtf8String *oldName, const CsrUtf8String *newName) /* NB: May also be used on directories */
{
    SFS_ERROR_E result;
    CsrUtf16String *oldUnicodeName;
    CsrUtf16String *newUnicodeName;

    oldUnicodeName = CsrUtf82Utf16String(oldName);
    newUnicodeName = CsrUtf82Utf16String(newName);

    result = SFS_RenameFile((uint16 *)oldUnicodeName, (uint16 *)newUnicodeName, NULL);
    CsrPmemFree(oldUnicodeName);
    CsrPmemFree(newUnicodeName);

    return mapSFSResult2CSR(result);
}

CsrResult CsrFileTell(CsrFileHandle *handle, CsrUint32 *position)
{    
    CsrDebugTraceHigh("CsrFileTell NOT IMPLEMENTED");
    return CSR_FILE_RESULT_FAILURE;
}

/************************ NOTE Csr Dir API has Not been implemented **************************************/


CsrResult CsrDirGetCurrentWorkingDir(CsrUtf8String **dirName)
{
    CsrDebugTraceHigh("CsrDirGetCurrentWorkingDir NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}

CsrResult CsrDirStat(const CsrUtf8String * path, CsrDirEntryStat * fileStat)
{
    CsrDebugTraceHigh("CsrDirStat NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}
CsrResult CsrDirMake(const CsrUtf8String * dirName)
{
    CsrDebugTraceHigh("CsrDirMake NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}

CsrResult CsrDirRemove(const CsrUtf8String * dirName)
{
    CsrDebugTraceHigh("CsrDirRemove NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}

CsrResult CsrDirChange(const CsrUtf8String * dirName)
{
    CsrDebugTraceHigh("CsrDirChange NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}

CsrDirHandle *CsrDirFindFirst(const CsrUtf8String * searchPattern, CsrDirFindStat * dirStat)
{
    CsrDebugTraceHigh("CsrDirFindFirst NOT IMPLEMENTED");
    return NULL /*CSR_DIR_INVALID_HANDLE*/; 
}

CsrResult CsrDirFindNext(CsrDirHandle *handle, CsrDirFindStat * dirStat)
{
    CsrDebugTraceHigh("CsrDirFindNext NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}

CsrResult CsrDirFindClose(CsrDirHandle *handle)
{
    CsrDebugTraceHigh("CsrDirFindClose NOT IMPLEMENTED");
    return CSR_DIR_RESULT_FAILURE;	
}

