// Sparce2Raw.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Sparce2Raw.h"
extern "C"
{
#include "sparse_format.h"
#include "sparse_crc32.h"
};

#pragma warning(disable:4244)

#define COPY_BUF_SIZE 1*1024*1024*sizeof(CHAR)	// 1M


// This is the constructor of a class that has been exported.
// see Sparce2Raw.h for the class definition
CSparce2Raw::CSparce2Raw()
	: m_hRead(INVALID_HANDLE_VALUE)
	, m_hWrite(INVALID_HANDLE_VALUE)
	, m_nTotalSize(0)
{
	m_pszCopyBuf = new CHAR[COPY_BUF_SIZE];
	m_ptrWrite = ImgWrite;
}

CSparce2Raw::~CSparce2Raw()
{
	if (INVALID_HANDLE_VALUE != m_hRead)
	{
		CloseHandle(m_hRead);
	}

	if (INVALID_HANDLE_VALUE != m_hWrite)
	{
		CloseHandle(m_hWrite);
	}

	if (NULL != m_pszCopyBuf)
	{
		delete[] m_pszCopyBuf;
		m_pszCopyBuf = NULL;
	}

}

int CSparce2Raw::ProcessRawChunk(HANDLE hRead, HANDLE hWrite, UINT32 blocks, UINT32 blk_sz, UINT32 *crc32)
{
	u64	len = (u64)blocks * (u64)blk_sz;
	int ret;
	int chunk;
	DWORD dwReadBytes = 0;
	m_nTotalSize += len;
	while(len)
	{
		chunk = (len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : len;
		ret = ReadFile(hRead, m_pszCopyBuf, chunk, &dwReadBytes, NULL);
		if (!ret)
		{
			return S2R_READ_FILE_FAILED;
		}

		*crc32 = sparse_crc32(*crc32, (u8*)m_pszCopyBuf, chunk);

		ret = m_ptrWrite(hWrite, m_pszCopyBuf, chunk);
		if (!ret)
		{
			return S2R_WRITE_FILE_FAILED;
		}
		len -= chunk;
	}

	return blocks;
}

int CSparce2Raw::ProcessFillChunk(HANDLE hRead, HANDLE hWrite, UINT32 blocks, UINT32 blk_sz, UINT32 *crc32)
{
	u64 len = (u64)blocks * blk_sz;
	int ret;
	int chunk;
	u32 fill_val;
	u32 *fillbuf;
	unsigned int i;
	m_nTotalSize += len;
	DWORD dwReadBytes = 0;

	/* Fill copy_buf with the fill value */
	ret = ReadFile(hRead, &fill_val, sizeof(fill_val), &dwReadBytes, NULL);
	if (!ret)
	{
		return S2R_READ_FILE_FAILED;
	}
	fillbuf = (u32 *)m_pszCopyBuf;
	for (i = 0; i < (COPY_BUF_SIZE / sizeof(fill_val)); i++)
	{	
		fillbuf[i] = fill_val;	
	}

	while (len)
	{
		chunk = (len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : len;
		*crc32 = sparse_crc32(*crc32, (u8*)m_pszCopyBuf, chunk);
		ret = m_ptrWrite(hWrite, m_pszCopyBuf, chunk);
		if (!ret)
		{
			return S2R_WRITE_FILE_FAILED;
		}
		len -= chunk;
	}
	return blocks;
}

int CSparce2Raw::ProcessSkipChunk(HANDLE hWrite, UINT32 blocks, UINT32 blk_sz, UINT32 *crc32)
{
	/* len needs to be 64 bits, as the sparse file specifies the skip amount
	 * as a 32 bit value of blocks.
	 */
	u64 len = (u64)(blocks * blk_sz);
	memset(m_pszCopyBuf, 0, COPY_BUF_SIZE);
	m_nTotalSize += len;

	while (len) 
	{
		int chunk = min(len, COPY_BUF_SIZE);
		*crc32 = sparse_crc32(*crc32, (u8*)m_pszCopyBuf, chunk);
		BOOL bRet = m_ptrWrite(hWrite, m_pszCopyBuf, chunk);
		if (!bRet)
		{
			return S2R_WRITE_FILE_FAILED;
		}
		len -= chunk;
	}
	
	return blocks;
}

int CSparce2Raw::ImgWrite(HANDLE hWrite, VOID* pBuf, UINT32 nLen)
{
	if (INVALID_HANDLE_VALUE == hWrite)
	{
		return S2R_INVALID_PARAMETER;
	}
	DWORD dwWriteBytes = 0;
	return WriteFile(hWrite, pBuf, nLen, &dwWriteBytes, NULL);
}

S2R_RESULT CSparce2Raw::ProcessCrcChunk(HANDLE hRead, UINT32 crc32)
{
	u32 file_crc32;
	int ret;

	DWORD dwReadBytes = 0;
	ret = ReadFile(hRead, &file_crc32, 4, &dwReadBytes, NULL);
	if (!ret)
	{
		return S2R_READ_FILE_FAILED;
	}

	if (file_crc32 != crc32)
	{
		return S2R_CRC_CHECK_FAILED;
	}

	return S2R_SUCCESS;
}

S2R_RESULT CSparce2Raw::Simg2Img(HANDLE hRead, HANDLE hWrite)
{
	if (INVALID_HANDLE_VALUE == hRead)
	{
		return S2R_INVALID_PARAMETER;
	}

	sparse_header_t sHeader;
	DWORD dwBytesRead;
	BOOL bRet = ReadFile(hRead, &sHeader, sizeof(sHeader), &dwBytesRead, NULL);
	if (!bRet)
	{
		return S2R_READ_FILE_FAILED;
	}

	if (sHeader.magic != SPARSE_HEADER_MAGIC) {
		return S2R_BAD_MAGIC;
	}

	if (sHeader.major_version != SPARSE_HEADER_MAJOR_VER) {
		return S2R_UNKNOWN_MAJOR_VERSION;
	}

	if (sHeader.file_hdr_sz > SPARSE_HEADER_LEN)
	{
		/* Skip the remaining bytes in a header that is longer than
		 * we expected.
		 */
		if (HFILE_ERROR == SetFilePointer(hRead, sHeader.file_hdr_sz-SPARSE_HEADER_LEN, 0, FILE_CURRENT))
		{
			return S2R_SET_FILE_POINTER_FAILED;
		}
	}

	chunk_header_t chunkHeader;
	UINT32 nTotalBlocks = 0;
	UINT32 nCrc32 = 0;

	for (UINT32 i = 0; i < sHeader.total_chunks; i++)
	{
		bRet = ReadFile(hRead, &chunkHeader, sizeof(chunk_header), &dwBytesRead, NULL);
		if (!bRet)
		{
			return S2R_READ_FILE_FAILED;
		}		
		
		if (sHeader.chunk_hdr_sz > CHUNK_HEADER_LEN)
		{
			/* Skip the remaining bytes in a header that is longer than
			 * we expected.
			 */
			
			if (HFILE_ERROR == SetFilePointer(hRead, sHeader.chunk_hdr_sz - CHUNK_HEADER_LEN, 0, FILE_CURRENT))
			{
				return S2R_SET_FILE_POINTER_FAILED;
			}
		}

		switch (chunkHeader.chunk_type)
		{
			case CHUNK_TYPE_RAW:
				if(chunkHeader.total_sz != (sHeader.chunk_hdr_sz + (chunkHeader.chunk_sz * sHeader.blk_sz)))
				{
					return S2R_CHUNK_TOTAL_SIZE_ERROR;
				}
				nTotalBlocks +=	ProcessRawChunk(hRead, hWrite, chunkHeader.chunk_sz, sHeader.blk_sz, &nCrc32);
				break;

			case CHUNK_TYPE_FILL:
				if (chunkHeader.total_sz != (sHeader.chunk_hdr_sz + sizeof(UINT32)))
				{
					return S2R_CHUNK_TOTAL_SIZE_ERROR;
				}
				nTotalBlocks += ProcessFillChunk(hRead, hWrite, chunkHeader.chunk_sz, sHeader.blk_sz, &nCrc32);
				break;

			case CHUNK_TYPE_DONT_CARE:
				if (chunkHeader.total_sz != sHeader.chunk_hdr_sz)
				{
					return S2R_CHUNK_TOTAL_SIZE_ERROR;
				}
				nTotalBlocks += ProcessSkipChunk(hWrite, chunkHeader.chunk_sz, sHeader.blk_sz, &nCrc32);
				break;

			case CHUNK_TYPE_CRC32:
				{
					S2R_RESULT rlt = ProcessCrcChunk(m_hRead, nCrc32);
					if (S2R_SUCCESS != rlt)
					{
						return rlt;
					}
				}				

				break;

			default:
				break;
		}
	}

	if (sHeader.total_blks != nTotalBlocks) 
	{
		return S2R_TOTAL_BLOCK_ERROR;
	}

	return S2R_SUCCESS;
}

UINT64 CSparce2Raw::GetImgSize(HANDLE hRead)
{
	m_nTotalSize = 0;

	SetWritePtr(ImgWriteNull);

	if (S2R_SUCCESS != Simg2Img(hRead, NULL))
	{
		return 0;
	}

	return m_nTotalSize;
}

