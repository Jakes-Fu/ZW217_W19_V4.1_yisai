// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SPARCE2RAW_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SPARCE2RAW_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SPARCE2RAW_EXPORTS
#define SPARCE2RAW_API __declspec(dllexport)
#else
#define SPARCE2RAW_API __declspec(dllimport)
#endif


typedef enum
{
	S2R_SUCCESS = 0,
	S2R_INVALID_PARAMETER = 1,
	S2R_OPEN_SPARCEFILE_FAILED,
	S2R_CREATE_RAWFILE_FAILED,
	S2R_READ_FILE_FAILED,
	S2R_BAD_MAGIC,
	S2R_UNKNOWN_MAJOR_VERSION,
	S2R_CHUNK_TOTAL_SIZE_ERROR,
	S2R_TOTAL_BLOCK_ERROR,
	S2R_WRITE_FILE_FAILED,
	S2R_CRC_CHECK_FAILED,
	S2R_SET_FILE_POINTER_FAILED,
	S2R_MAX
}S2R_RESULT;

typedef int (*WritePtr)(HANDLE, VOID*, UINT32);

// This class is exported from the Sparce2Raw.dll
class SPARCE2RAW_API CSparce2Raw {
public:
	CSparce2Raw(void);
	~CSparce2Raw();
	// TODO: add your methods here.
	static int ImgWrite(HANDLE hWrite, VOID* pBuf, UINT32 nLen);
	S2R_RESULT Simg2Img(HANDLE hRead, HANDLE hWrite);
	UINT64 GetImgSize(HANDLE hRead);

	void SetWritePtr(WritePtr ptrWrite)
	{
		m_ptrWrite = ptrWrite;
	}

	static int ImgWriteNull(HANDLE, VOID*, UINT32) {return 1;};

private:
	int ProcessRawChunk(HANDLE hRead, HANDLE hWrite, UINT32 blocks, UINT32 blk_sz, UINT32 *crc32);
	int ProcessFillChunk(HANDLE hRead, HANDLE hWrite, UINT32 blocks, UINT32 blk_sz, UINT32 *crc32);
	int ProcessSkipChunk(HANDLE hWrite, UINT32 blocks, UINT32 blk_sz, UINT32 *crc32);
	S2R_RESULT ProcessCrcChunk(HANDLE hRead, UINT32 crc32);

	HANDLE m_hRead;
	HANDLE m_hWrite;
	CHAR*	m_pszCopyBuf;
	WritePtr m_ptrWrite;
	UINT64	m_nTotalSize;
};
