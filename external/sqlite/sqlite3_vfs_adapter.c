/*****************************************************************************
** File Name:      sqlite3_adapter.c                                         *
** Author:                                                                   *
** Date:             04/11/2011                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file array module           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2011       Cheng.Luo          Create                                 *
******************************************************************************/

#if SQLITE_OS_OTHER

#include "external_trc.h"
#ifdef MAX
# undef MAX
#endif
#ifdef MIN
# undef MIN
#endif

#include "os_api.h"
#include "sfs.h"
#include "dal_time.h"
#include "gui_ucs2b_converter.h"

#define  FILE_MAX_PATH	SFS_MAX_PATH
#define  OSTRACE        SCI_TRACE_LOW
#define OpenCounter(X)

typedef struct sqliteAdapterFile sqliteAdapterFile;
struct sqliteAdapterFile {
	const sqlite3_io_methods *pMethod;  /* Always the first entry */
	SFS_HANDLE   h;                  /* Handle for accessing the file */
};

/*
** Close a file.
*/
static int sqliteAdapterClose( sqlite3_file *id )
{
	sqliteAdapterFile *pFile = (sqliteAdapterFile*)id;
	if( pFile )
	{
		if(SFS_ERROR_NONE != SFS_CloseFile(pFile->h))
		{
			return SQLITE_IOERR_CLOSE;
		}
		
		//OSTRACE:"CLOSE   %-3d\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_47_112_2_18_0_26_10_3,(uint8*)"d", pFile->h);
		OpenCounter(-1);
		
		memset(pFile, 0, sizeof(sqliteAdapterFile));
	}
	return SQLITE_OK;
}
/*
** Read data from a file into a buffer.  Return SQLITE_OK if all
** bytes were read successfully and SQLITE_IOERR if anything goes
** wrong.
*/
static int sqliteAdapterRead(
							 sqlite3_file *id,               /* File to read from */
							 void *pBuf,                     /* Write content into this buffer */
							 int amt,                        /* Number of bytes to read */
							 sqlite3_int64 offset            /* Begin reading at this offset */
							 )
{
	uint32 got = 0;
	sqliteAdapterFile *pFile = (sqliteAdapterFile*)id;
	assert( id!=0 );
	
	//OSTRACE:"READ %d \n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_69_112_2_18_0_26_10_4,(uint8*)"d", pFile->h);
	if (SFS_ERROR_NONE != SFS_SetFilePointer(pFile->h, offset, SFS_SEEK_BEGIN))
	{
		return SQLITE_IOERR;
	}
	
	if (SFS_ERROR_NONE != SFS_ReadFile(pFile->h, pBuf,amt, &got, NULL))
	{
		return SQLITE_IOERR_READ;
	}
	
	if( got == (uint32)amt )
		return SQLITE_OK;
	else 
	{
		/* Unread portions of the input buffer must be zero-filled */
		memset(&((char*)pBuf)[got], 0, amt-got);
		return SQLITE_IOERR_SHORT_READ;
	}
}
/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
static int sqliteAdapterWrite(
							  sqlite3_file *id,               /* File to write into */
							  const void *pBuf,               /* The bytes to be written */
							  int amt,                        /* Number of bytes to write */
							  sqlite3_int64 offset            /* Offset into the file to begin writing at */
							  )
{
	int rc =  0;
	uint32 wrote = 0;
	sqliteAdapterFile *pFile = (sqliteAdapterFile*)id;
	
	assert( id!=0 );
	assert( amt>0);
	
	//OSTRACE:"WRITE %d \n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_106_112_2_18_0_26_11_5,(uint8*)"d", pFile->h);
	if (SFS_ERROR_NONE != SFS_SetFilePointer(pFile->h, offset, SFS_SEEK_BEGIN))
	{
		return SQLITE_IOERR;
	}
	
	while (amt > 0 &&
		SFS_ERROR_NONE !=(rc = SFS_WriteFile(pFile->h, pBuf,amt, &wrote, NULL))
		&& wrote > 0)	 
	{
		amt -= wrote;
		pBuf = &((char*)pBuf)[wrote];
	}
	
	return ( rc != 0 || amt > (int)wrote ) ? SQLITE_FULL : SQLITE_OK;
}
/*
** Truncate an open file to a specified size
*/
static int sqliteAdapterTruncate( sqlite3_file *id, i64 nByte )
{
	int rc = 0;
	sqliteAdapterFile *pFile = (sqliteAdapterFile*)id;
	
	//OSTRACE:"TRUNCATE %d %lld\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_129_112_2_18_0_26_11_6,(uint8*)"dd", pFile->h, nByte );
	rc = SFS_SetFileSize(pFile->h, (uint32)nByte);
	
	return rc == 0 ? SQLITE_OK : SQLITE_IOERR_TRUNCATE;
}
/*
** Make sure all writes to a particular file are committed to disk.
*/
static int sqliteAdapterSync( sqlite3_file *id, int flags )
{
#if !defined(NDEBUG) || !defined(SQLITE_NO_SYNC) || defined(SQLITE_DEBUG)
	sqliteAdapterFile *pFile = (sqliteAdapterFile*)id;
#else
	UNUSED_PARAMETER(id);
#endif
	assert( pFile );
	/* Check that one of SQLITE_SYNC_NORMAL or FULL was passed */
	assert((flags&0x0F)==SQLITE_SYNC_NORMAL
		|| (flags&0x0F)==SQLITE_SYNC_FULL
		);
	
	//OSTRACE:"SYNC %d \n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_149_112_2_18_0_26_11_7,(uint8*)"d", pFile->h);
	
#ifndef SQLITE_TEST
	UNUSED_PARAMETER(flags);
#else
	if( flags & SQLITE_SYNC_FULL ){
		sqlite3_fullsync_count++;
	}
	sqlite3_sync_count++;
#endif
	
	/* If we compiled with the SQLITE_NO_SYNC flag, then syncing is a
	** no-op
	*/
#ifdef SQLITE_NO_SYNC
	return SQLITE_OK;
#else
	if(SFS_ERROR_NONE == SFS_FlushFile(pFile->h, 0))
	{
		return SQLITE_OK;
	}
	else
	{
		return SQLITE_IOERR;
	}
#endif
}
/*
** Determine the current size of a file in bytes
*/
static int sqliteAdapterFileSize( sqlite3_file *id, sqlite3_int64 *pSize )
{
	uint32 size = 0;
	sqliteAdapterFile *pFile = (sqliteAdapterFile*)id;
	
	if( SFS_ERROR_NONE == SFS_GetFileSize(pFile->h, &size))
	{
		*pSize = (sqlite3_int64)size;
		return SQLITE_OK;
	}
	else
	{
		return SQLITE_IOERR_FSTAT;
	}
}

static int sqliteAdapterLock( sqlite3_file *id, int locktype )
{
	return SQLITE_OK;
}

static int sqliteAdapterUnlock(sqlite3_file *id, int locktype)
{
	return SQLITE_OK;
}

static int sqliteAdapterCheckReservedLock(sqlite3_file *id, int *pResOut)
{
	*pResOut = 0;
	return SQLITE_OK;
}

/*
** Control and query of the open file handle.
*/
static int sqliteAdapterFileControl(sqlite3_file *id, int op, void *pArg)
{
	return SQLITE_NOTFOUND;
}

/*
** Return the sector size in bytes of the underlying block device for
** the specified file. This is almost always 512 bytes, but may be
** larger for some devices.
*/
static int sqliteAdapterSectorSize(sqlite3_file *id)
{
	return SQLITE_DEFAULT_SECTOR_SIZE;
}

/*
** Return a vector of device characteristics.
*/
static int sqliteAdapterDeviceCharacteristics(sqlite3_file *id)
{
	return 0;
}

static const sqlite3_io_methods sqliteAdapterIoMethod = {
	1,                        /* iVersion */
		sqliteAdapterClose,
		sqliteAdapterRead,
		sqliteAdapterWrite,
		sqliteAdapterTruncate,
		sqliteAdapterSync,
		sqliteAdapterFileSize,
		sqliteAdapterLock,
		sqliteAdapterUnlock,
		sqliteAdapterCheckReservedLock,
		sqliteAdapterFileControl,
		sqliteAdapterSectorSize,
		sqliteAdapterDeviceCharacteristics
};

///////////////////////////////////
static int GetTempname(int nBuf, char *zBuf)
{
	static const unsigned char zChars[] =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
	unsigned int i, j;
	uint16 attr = 0;
	const char *zDir = "D:\\";
	
	/* Check that the output buffer is large enough for the temporary file 
	** name. If it is not, return SQLITE_ERROR.
	*/
	if( (strlen(zDir) + strlen(SQLITE_TEMP_FILE_PREFIX) + 17) >= (size_t)nBuf )
	{
		return SQLITE_ERROR;
	}
	
	do{
		sqlite3_snprintf(nBuf-17, zBuf, "%s/"SQLITE_TEMP_FILE_PREFIX, zDir);
		j = (int)strlen(zBuf);
		sqlite3_randomness(15, &zBuf[j]);
		for(i=0; i<15; i++, j++){
			zBuf[j] = (char)zChars[ ((unsigned char)zBuf[j])%(sizeof(zChars)-1) ];
		}
		zBuf[j] = 0;
	}while( SFS_ERROR_NONE == SFS_GetAttr(zBuf,&attr));
	
	return SQLITE_OK;
}

static int sqliteAdapterOpen(
							 sqlite3_vfs *pVfs,        /* Not used */
							 const char *zName,        /* Name of the file (UTF-8) */
							 sqlite3_file *id,         /* Write the SQLite file handle here */
							 int flags,                /* Open mode flags */
							 int *pOutFlags            /* Status return flags */		
							 )
{
	sqliteAdapterFile *pFile = (sqliteAdapterFile *)id;
	int openFlags = 0;             /* Flags to pass to open() */
	int openMode = 0;
	int rc = 0;					   /* Function Return Code */
	uint16 name_wstr_len    = 0;
	wchar *name_wstr        = PNULL;
	int name_utf8_Len		= 0;
	SFS_HANDLE   h = 0;
	char zTmpname[FILE_MAX_PATH+1] = {0};    /* Buffer to hold name of temp file */
	
											 /* If the second argument to this function is NULL, generate a 
											 ** temporary file name to use 
	*/
	if( !zName )
	{
		int rc = GetTempname(FILE_MAX_PATH+1, zTmpname);
		if( rc!=SQLITE_OK )
		{
			return rc;
		}
		zName = zTmpname;
	}
	
	memset( pFile, 0, sizeof(*pFile) );
	
	//OSTRACE:"OPEN want %d\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_314_112_2_18_0_26_11_8,(uint8*)"d", flags);
	
	if( flags & SQLITE_OPEN_READWRITE )
	{
		openMode |= SFS_MODE_READ|SFS_MODE_WRITE;
		//OSTRACE:"OPEN read/write\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_319_112_2_18_0_26_11_9,(uint8*)"");
	}
	else
	{
		openMode |= SFS_MODE_READ;
		//OSTRACE:"OPEN read only\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_324_112_2_18_0_26_11_10,(uint8*)"");
	}
	
	if( flags & SQLITE_OPEN_CREATE ){
		openMode |= SFS_MODE_OPEN_ALWAYS;
		//OSTRACE:"OPEN open new/create\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_329_112_2_18_0_26_11_11,(uint8*)"" );
	}else{
		openMode |= SFS_MODE_OPEN_EXISTING;
		//OSTRACE:"OPEN open existing\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_332_112_2_18_0_26_11_12,(uint8*)"" );
	}
	
	if( flags & SQLITE_OPEN_MAIN_DB ){
		//openMode |= SFS_MODE_SHARE_READ|SFS_MODE_SHARE_WRITE;
		//OSTRACE:"OPEN share read/write\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_337_112_2_18_0_26_11_13,(uint8*)"" );
	}else{
		//openMode |= SFS_MODE_SHARE_READ;
		//OSTRACE:"OPEN share read only\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_340_112_2_18_0_26_11_14,(uint8*)"" );
	}
	
	if( flags & SQLITE_OPEN_DELETEONCLOSE )
	{
		//OSTRACE:"OPEN hidden/delete on close file attributes\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_345_112_2_18_0_26_11_15,(uint8*)"" );
	}
	else
	{
		//OSTRACE:"OPEN normal file attribute\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_349_112_2_18_0_26_11_16,(uint8*)"" );
	}
	
	name_utf8_Len = strlen(zName);
	name_wstr = (wchar *)SCI_ALLOCA((name_utf8_Len + 1) * sizeof(wchar));
	if (PNULL == name_wstr)
	{
		return SQLITE_NOMEM;
	}
	SCI_MEMSET(name_wstr, 0, ((name_utf8_Len + 1) * sizeof(wchar)));
	name_wstr_len = GUI_UTF8ToWstr(name_wstr, name_utf8_Len, zName, name_utf8_Len);
	
	h = SFS_CreateFile(name_wstr, openMode, 0, 0);	
	SCI_FREE( name_wstr );
	
	if(0 == h)
	{
		//OSTRACE:"OPEN Invalid handle rc=%d: zName=%s,Mode=%#lx\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_366_112_2_18_0_26_11_17,(uint8*)"dsd",rc, zName,openMode);
		if( flags & SQLITE_OPEN_READWRITE )
		{
			//OSTRACE:"OPEN %d Invalid handlen"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_370_112_2_18_0_26_11_18,(uint8*)"d",((flags | SQLITE_OPEN_READONLY) & ~SQLITE_OPEN_READWRITE));
			return sqliteAdapterOpen( pVfs, zName, id,
				((flags | SQLITE_OPEN_READONLY) & ~SQLITE_OPEN_READWRITE),
				pOutFlags );
		}
		else
		{
			return SQLITE_CANTOPEN;
		}
	}
	
	if( pOutFlags )
	{
		*pOutFlags = flags & SQLITE_OPEN_READWRITE ? SQLITE_OPEN_READWRITE : SQLITE_OPEN_READONLY;
	}
	
	pFile->pMethod = &sqliteAdapterIoMethod;
	pFile->h = h;
	OpenCounter(+1);
	//OSTRACE:"OPEN %d pOutFlags=%d\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_389_112_2_18_0_26_11_19,(uint8*)"dd", pFile->h, pOutFlags);
	return SQLITE_OK;
}

static int sqliteAdapterDelete(
							   sqlite3_vfs *pVfs,                     /* Not used on os2 */
							   const char *zFilename,                 /* Name of file to delete */
							   int syncDir                            /* Not used on os2 */
							   )
{
	int ret					= 0;
	uint16 name_wstr_len    = 0;
	wchar *name_wstr        = PNULL;
	int name_utf8_Len		= 0;
	UNUSED_PARAMETER(pVfs);
	
	if (NULL == zFilename)
	{
		return SQLITE_ERROR;
	}
	
	name_utf8_Len = strlen(zFilename);
	name_wstr = (wchar *)SCI_ALLOCA((name_utf8_Len + 1) * sizeof(wchar));
	if (PNULL == name_wstr)
	{
		return SQLITE_NOMEM;
	}
	SCI_MEMSET(name_wstr, 0, ((name_utf8_Len + 1) * sizeof(wchar)));
	name_wstr_len = GUI_UTF8ToWstr(name_wstr, name_utf8_Len, zFilename, name_utf8_Len);
	
	ret = SFS_DeleteFile(name_wstr, 0);
	SCI_FREE( name_wstr );
	//OSTRACE:"DELETE \"%s\"\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SQLITE3_VFS_ADAPTER_421_112_2_18_0_26_11_20,(uint8*)"s", name_wstr);
	
	return ret == 0 ? SQLITE_OK : SQLITE_IOERR_DELETE;
}

static int sqliteAdapterAccess(
							   sqlite3_vfs *pVfs,         /* Not used on win32 */
							   const char *zFilename,     /* Name of file to check */
							   int flags,                 /* Type of test to make on this file */
							   int *pResOut               /* OUT: Result */
							   )
{
	uint16 attr				= 0;
	int		rc				= 0;
	uint16 name_wstr_len    = 0;
	wchar *name_wstr        = PNULL;
	int name_utf8_Len		= 0;
	UNUSED_PARAMETER(pVfs);
	
	if (NULL == zFilename)
	{
		return SQLITE_ERROR;
	}
	
	name_utf8_Len = strlen(zFilename);
	name_wstr = (wchar *)SCI_ALLOCA((name_utf8_Len + 1) * sizeof(wchar));
	if (PNULL == name_wstr)
	{
		return SQLITE_NOMEM;
	}
	SCI_MEMSET(name_wstr, 0, ((name_utf8_Len + 1) * sizeof(wchar)));
	name_wstr_len = GUI_UTF8ToWstr(name_wstr, name_utf8_Len, zFilename, name_utf8_Len);
	
	if (SFS_ERROR_NONE != SFS_GetAttr(name_wstr, &attr))
	{
		attr = 0xffff;
	}
	
	SCI_FREE( name_wstr );
	
	switch( flags )
	{
	case SQLITE_ACCESS_READ:
	case SQLITE_ACCESS_EXISTS:
		rc = (attr!= 0xffff);
		break;
	case SQLITE_ACCESS_READWRITE:
		rc = (attr & SFS_ATTR_RO != SFS_ATTR_RO);
		break;
	default:
		assert(!"Invalid flags argument");
	}
	*pResOut = rc;
	
	return SQLITE_OK;
}

static int sqliteAdapterFullPathname(
									 sqlite3_vfs *pVfs,            /* Pointer to vfs object */
									 const char *zRelative,        /* Possibly relative input path */
									 int nFull,                    /* Size of output buffer in bytes */
									 char *zFull                   /* Output buffer */
									 )
{
	UNUSED_PARAMETER(pVfs);
	if (NULL == zFull)
	{
		return SQLITE_ERROR;
	}
	
	sqlite3_snprintf(nFull, zFull, "%s", zRelative);
	
	return SQLITE_OK;
}

static int sqliteAdapterRandomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf)
{
	int n = 0;
	UNUSED_PARAMETER(pVfs);
	
	if (NULL == zBuf)
	{
		return SQLITE_ERROR;
	}
	
#if defined(SQLITE_TEST)
	n = nBuf;
	memset(zBuf, 0, nBuf);
#else
	if( sizeof(SCI_TM_T)<=nBuf-n ){
		SCI_TM_T x;
		TM_GetTime(&x);
		memcpy(&zBuf[n], &x, sizeof(x));
		n += sizeof(x);
	}
	if( sizeof(uint32)<=nBuf-n ){
		uint32 id = SCI_IdentifyThread();
		memcpy(&zBuf[n], &id, sizeof(id));
		n += sizeof(id);
	}
	if( sizeof(uint32)<=nBuf-n ){
		uint32 cnt = SCI_GetTickCount();
		memcpy(&zBuf[n], &cnt, sizeof(cnt));
		n += sizeof(cnt);
	}
#endif
	return n;
}

static int sqliteAdapterSleep(sqlite3_vfs *pVfs, int microsec)
{
	UNUSED_PARAMETER(pVfs);
	SCI_Sleep((microsec+999)/1000);	
	
	return ((microsec+999)/1000)*1000;
}

int sqliteAdapterCurrentTime( sqlite3_vfs *pVfs, double *prNow )
{
	double now;
	SCI_TM_T cur_time={0};
	
	TM_GetTime(&cur_time);
	
	/* Calculate the Julian days */
	now = cur_time.tm_mday - 32076 +
		1461*(cur_time.tm_year + 4800 + (cur_time.tm_mon - 14)/12)/4 +
		367*(cur_time.tm_mon - 2 - (cur_time.tm_mon - 14)/12*12)/12 -
		3*((cur_time.tm_year + 4900 + (cur_time.tm_mon - 14)/12)/100)/4;
	
	/* Add the fractional hours, mins and seconds */
	now += (cur_time.tm_hour + 12.0)/24.0;
	now += cur_time.tm_min/1440.0;
	now += cur_time.tm_sec/86400.0;
	*prNow = now;
	
	return 0;
}

static int sqliteAdapterGetLastError(sqlite3_vfs *pVfs, int nBuf, char *zBuf)
{
	return 0;
}

/*
** Initialize and deinitialize the operating system interface.
*/
SQLITE_API int sqlite3_os_init(void)
{
	static sqlite3_vfs sqliteAdapterVfs = {
		1,                 /* iVersion */
			sizeof(sqliteAdapterVfs),   /* szOsFile */
			FILE_MAX_PATH,      /* mxPathname */
			0,					/* pNext */
			"sqliteAdapter",    /* zName */
			0,					/* pAppData */
			sqliteAdapterOpen,           /* xOpen */
			sqliteAdapterDelete,         /* xDelete */
			sqliteAdapterAccess,         /* xAccess */
			sqliteAdapterFullPathname,   /* xFullPathname */
			0,        /* xDlOpen */
			0,        /* xDlError */
			0,        /* xDlSym */
			0,        /* xDlClose */
			sqliteAdapterRandomness,     /* xRandomness */
			sqliteAdapterSleep,          /* xSleep */
			sqliteAdapterCurrentTime,    /* xCurrentTime */
			sqliteAdapterGetLastError,   /* xGetLastError */
	};
	
	sqlite3_vfs_register(&sqliteAdapterVfs, 1);
	
	return SQLITE_OK; 
}

SQLITE_API int sqlite3_os_end(void)
{ 
	return SQLITE_OK;
}
#endif  /*SQLITE_OS_OTHER*/
