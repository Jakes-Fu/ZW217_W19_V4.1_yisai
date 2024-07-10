

#ifndef __Aplfs_BUFFER_H__
#define __Aplfs_BUFFER_H__

#include "OEMOS.h"

// typedef enum 
// {
// 	FALSE,
// 	TRUE
// } boolean;

typedef unsigned int FS_HANDLE;

// #define OEMOS_dbgprintf DBGPRINTF
// #define APPLE_FILE_LOG


   #define Aplfs_VERBOSE 0

	#define MED_STAT_Aplfs_START 0
   #define Aplfs_PLATFORM_KAL 1
   #define Aplfs_PLATFORM_WIN32 0
   #define Aplfs_PLATFORM_MEMORY 0
#define mem_cpy(a,b,c) memcpy(a,b,c)


typedef enum {
   Aplfs_OK = MED_STAT_Aplfs_START,
   Aplfs_OPEN_ERROR,
   Aplfs_READ_ERROR,
   Aplfs_WRITE_ERROR,
   Aplfs_SEEK_ERROR,
   Aplfs_CLOSE_ERROR,
   Aplfs_INVALID_ARGUMENT,
   /// The field of the structure is invalid. Possibly caused by memory corruption.
   Aplfs_MEMORY_CORRUPTION,
   Aplfs_FATAL_ERROR,
   Aplfs_DEVICE_BUSY,
   Aplfs_READ_OVER_EOF,
   Aplfs_SEEK_OVER_EOF
} Aplfs_Status;



typedef enum {
   /// Open for reading.
   Aplfs_READ = 0,
   /// Open for reading and writing.
   /// The file is created if it is not existed, otherwise it is truncated.
   Aplfs_WRITE,
   /// Open for reading and appending.
   /// The file is created if it is not existed.
   Aplfs_APPEND,
   /// Open a ROM file for reading.
   /// Aplfs_Direct_SetRamFileSize shall be called before calling Aplfs_Open.
   Aplfs_ROMFILE,
   ///
   Aplfs_READ_SHARED,
   Aplfs_READWRITE,
   Aplfs_NONBLOCKING = 0x80
} Aplfs_FileMode;


typedef enum {
   Aplfs_BIG_ENDIAN = 0,
   Aplfs_LITTLE_ENDIAN   
} Aplfs_ByteOrder;

/**
 * The memory space of the Aplfs structure is prepared by the caller.
 * Accesses to each file requires a separate instance of each Aplfs structure.
 * The content of the structuer will be initialized via the Aplfs_Open
 * function call.
 * And the pointer to the structure need to be passed as the first argument
 * to each Aplfs function invocations.
 */
typedef struct {
   /* file system abstration layer*/
   FS_HANDLE hFile;
   /// error code from file system
   int32 iFSErrorCode;

   /// For ROM file. Point to the start memory address of the file.
   uint8 *pbFile;
   uint32 uRamFileSize;
   uint32 uRamFileOffset;
   uint32 uFileSize;
   uint32 uFileOffset;
   boolean bBuffering;
   uint8 *pbBuf;
   uint32 uBufSize;
   uint32 uCachedBlock;
   boolean bDirty; /* whether the cache has been written to */
   uint32 uLastReadCount;
   uint32 uDRMpermission;

   Aplfs_ByteOrder uByteOrder;
} STAplfs;

/* ------ Private Macros ------ */

#define Aplfs_CHECK_ARG(exp) \
   if (!(exp)) \
      return Aplfs_INVALID_ARGUMENT

#define Aplfs_ASSERT(exp) \
   if (!(exp)) \
      return Aplfs_FATAL_ERROR

#define Aplfs_ASSERT_NO_RET_VAL(exp) \
   if (!(exp)) \
      return

/* ------ public functions ------ */

/**
 * Open the file.
 *
 * @param pstAplfs pointer to Aplfs structure prepared by caller.
 * Aplfs will initialize the structure.
 * @param szFile pointer to a string contained the file name when opened in Aplfs_READ,
 * Aplfs_WRITE, or Aplfs_APPEND mode; pointer to the start memory address of the file
 * when opened in Aplfs_ROMFILE mode.
 * @param eMode file operation mode.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Open(STAplfs *pstAplfs, void *szFile, Aplfs_FileMode eMode);

/**
 * Open the file which has been opened via file I/O API with its file handle.
 * Please do not call Aplfs_Close for this handle.
 *
 * @param pstAplfs pointer to Aplfs structure prepared by caller.
 * Aplfs will initialize the structure.
 * @param pFileHandle file handle of the underlying file I/O API.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Open_WithHandle(STAplfs *pstAplfs, void *pFileHandle);

/**
 * Open an existing file. When using this interface, you need to consider
 * about the issues of multiple access to the same file.
 * Please do not call Aplfs_Close for this handle.
 *
 * @param pstAplfs pointer to Aplfs structure prepared by caller.
 * Aplfs will initialize the structure.
 * @param pstAplfs_Existing pointer to an existing Aplfs structure,
 * Aplfs will access file opened via the file handle in that structure.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Open_Attach(STAplfs *pstAplfs, STAplfs *pstAplfs_Existing);

/**
 * Client prepares its own buffer and call this function
 * to set buffer for Aplfs cache.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param uBufferSize size of the buffer in unit of byte.
 * @param pbBuf memory location of the buffer
 *
 * @return None.
 */
void Aplfs_SetBuffer(STAplfs *pstAplfs, uint32 uBufferSize, uint8 *pbBuf);

/**
 * Flush Aplfs cache and close the file.
 *
 * @param pstAplfs pointer to Aplfs structure.
 *
 * @return None.
 */
Aplfs_Status Aplfs_Close(STAplfs *pstAplfs);

/* ------ buffering functions (Aplfs_buffer.c) ------ */

/**
 * Read data from the file.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbBuf memory location of the data.
 * @param uSize number of bytes to read. uSize must be larger than 0.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Read(STAplfs *pstAplfs, uint8* pbBuf, uint32 uSize);

/**
 * Write data to the file.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbBuf memory location of the data.
 * @param uSize number of bytes to write. uSize must be larger than 0.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Write(STAplfs *pstAplfs, uint8 *pbBuf, uint32 uSize);

/**
 * Change file offset.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param uOffset offsets in unit of bytes.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Seek(STAplfs *pstAplfs, uint32 uOffset);

/**
 * Flush Aplfs cache to the underlying file system.
 *
 * @param pstAplfs pointer to Aplfs structure.
 *
 * @return None.
 */
Aplfs_Status Aplfs_CacheFlush(STAplfs *pstAplfs);

/**
 * Get the current file position.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puPosition memory location for retrieving the file position.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_GetCurPos(STAplfs *pstAplfs, uint32 *puPosition);

/**
 * Get the file size.
 * The current file position remains the same after invocation.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puFileSize memory location for retrieving the file size.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_GetFileSize(STAplfs *pstAplfs, uint32 *puFileSize);



/* ------ bits/bytes read access functions (Aplfs_read.c) ------ */

/**
 * Read n bytes.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbData memory location for storing the result bytes
 * @param uLen number of bytes to read
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Read_Bytes(STAplfs *pstAplfs, uint8 *pbData, uint32 uLen);

/**
 * Read n bits.
 * When calling this function for the first time, *puBitCnt must be 0.
 * *puBitCnt will be updated after each invocation.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puBitCnt memory location for storing the bit offset.
 * @param pbValue memory location for storing the result bits.
 * @param uBitLen number of bits to read (1~8).
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Read_Bits(STAplfs *pstAplfs, uint32 *puBitCnt, uint8 *pbValue, uint32 uBitLen);

/**
 * Read 4 bytes representing an unsigned integer.
 * The content stored on the file is assumed to be big-endian.
 * This function will handle the endian-conversion issue.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puValue memory location for storing the result unsigned integer
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Read_UINT(STAplfs *pstAplfs, uint32 *puValue);

/**
 * Read 3 bytes representing an unsigned integer.
 * The content stored on the file is assumed to be big-endian.
 * This function will handle the endian-conversion issue.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puValue memory location for storing the result unsigned integer
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Read_UINT24(STAplfs *pstAplfs, uint32 *puValue);

/**
 * Read 2 bytes representing an unsigned integer.
 * The content stored on the file is assumed to be big-endian.
 * This function will handle the endian-conversion issue.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pwValue memory location for storing the result unsigned integer
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Read_UINT16(STAplfs *pstAplfs, uint16 *pwValue);

/**
 * Skip n bytes.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param uLen number of bytes to skip
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Skip_Bytes(STAplfs *pstAplfs, uint32 uLen);

/**
 * Get next n bytes. The file offset does not change after invocation.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbValue memory location for storing the result bytes
 * @param uLen number of bytes to peek
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Peek_Bytes(STAplfs *pstAplfs, uint8 *pbValue, uint32 uLen);

/**
 * Get next unsinged integer. The file offset does not change after invocation.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puValue memory location for storing the result unsigned integer
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Peek_UINT(STAplfs *pstAplfs, uint32 *puValue);


/* ------ bits/bytes write access functions (Aplfs_write.c) ------ */

/**
 * Write n bytes.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbData memory location of the bytes
 * @param uLen number of bytes to write
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Write_Bytes(STAplfs *pstAplfs, uint8 *pbData, uint32 uLen);

/**
 * Write 4 bytes representing an unsigned integer.
 * The value written to the file will be stored as big-endian.
 * This function will handle the endian-conversion issue.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param uValue The value to be written to the file.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Write_UINT(STAplfs *pstAplfs, uint32 uValue);

/**
 * Write 2 bytes representing an unsigned integer.
 * The value written to the file will be stored as big-endian.
 * This function will handle the endian-conversion issue.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param wValue The value to be written to the file.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Write_UINT16(STAplfs* pstAplfs, uint16 wValue);

/**
 * Write 1 byte representing an unsigned character.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param bValue The value to be written to the file.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Write_UINT8(STAplfs* pstAplfs, uint8 bValue);

/**
 * Write 4 bytes representing an unsigned integer at a given file offset.
 * File position remains the same after the invocation.
 * The value written to the file will be stored as big-endian.
 * This function will handle the endian-conversion issue.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param uValue The value to be written to the file.
 * @param uOffset The file offset to write to.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Write_UINT_AT(STAplfs* pstAplfs, uint32 uValue, uint32 uOffset);

/* ------ direct access functions (bypass buffering mechanism) ------ */

/**
 * Open the file. (Internal Use by Aplfs Only)
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param szFile pointer to a string contained the file name.
 * @param eMode file operation mode.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_Open(STAplfs *pstAplfs, void *szFile, Aplfs_FileMode eMode);

/**
 * Close the file. (Internal Use by Aplfs Only)
 *
 * @param pstAplfs pointer to Aplfs structure.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_Close(STAplfs *pstAplfs);

/**
 * Read data from the file directly.
 *
 * Use with caution. Becuase the file offset will be out-of-sync
 * between Aplfs and underlying file system.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbBuf memory location of the data.
 * @param uSize number of bytes to read. uSize must be larger than 0.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_Read(STAplfs *pstAplfs, uint8* pbBuf, uint32 uSize);

/**
 * Write data to the file directly.
 *
 * Use with caution. Becuase the file offset will be out-of-sync
 * between Aplfs and underlying file system.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param pbBuf memory location of the data.
 * @param uSize number of bytes to write. uSize must be larger than 0.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_Write(STAplfs *pstAplfs, uint8* pbBuf, uint32 uSize);

/**
 * Change file offset of the underlying file system directly.
 *
 * Use with caution. Becuase the file offset will be out-of-sync
 * between Aplfs and underlying file system.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param uOffset offsets in unit of bytes.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_Seek(STAplfs *pstAplfs, uint32 uOffset);

/**
 * Get the current file position of the underlying file system directly.
 *
 * Use with caution. Becuase the file offset may be out-of-sync
 * between Aplfs and underlying file system.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puPosition memory location for retrieving the file position.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_GetCurPos(STAplfs *pstAplfs, uint32* puPosition);

/**
 * Get the file size of the underlying file system directly.
 * The current file position remains the same after invocation.
 *
 * Use with caution. Becuase the file size may be out-of-sync
 * between Aplfs and underlying file system.
 *
 * @param pstAplfs pointer to Aplfs structure.
 * @param puFileSize memory location for retrieving the file size.
 *
 * @return If the function succeeds, the return value is Aplfs_OK.
 * Otherwise, an error code is returned.
 */
Aplfs_Status Aplfs_Direct_GetFileSize(STAplfs *pstAplfs, uint32 *puFileSize);

/**
 * When using ram file, this funtion shall be called before calling Aplfs_Open.
 */
void Aplfs_Direct_SetRamFileSize(STAplfs *pstAplfs, uint32 uSize);

/**
 * When writing to ram file, if after a write operation, the file offset exceeds
 * the specified ram file size, an assertion will be issued in order to detect
 * memory corruption.
 */
void Aplfs_Direct_SetMaxRamFileSize(STAplfs *pstAplfs, uint32 uSize);

/**
 * Get the last file system error.
 */
int Aplfs_GetLastError(STAplfs *pstAplfs);

/**
 * If the pstAplfs is a handle of ram file, return TRUE; otherwise return FALSE.
 */
boolean Aplfs_IsRamFile(STAplfs *pstAplfs);

/**
 * If the pstAplfs is a handle of ram file, return pointer to the memory block; otherwise return NULL.
 */
uint8* Aplfs_GetRamFilePointer(STAplfs *pstAplfs);

/**
 * Get the data count (in bytes) read in the previous read operation.
 * Generally, call this function if the return value of the previous read operation if Aplfs_READ_OVER_EOF.
 */
int Aplfs_GetLastReadCount(STAplfs *pstAplfs);


Aplfs_Status Aplfs_SetByteOrder(STAplfs *pstAplfs, Aplfs_ByteOrder byteOrder);

#endif

