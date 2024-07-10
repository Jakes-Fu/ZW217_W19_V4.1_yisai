#include <stdlib.h>
#include <stdio.h>
#include "rs_datatype.h"
#include "rs_flash_operate.h"
#include "rs_dev.h"
#include "rs_error.h"
#include "rs_checksum.h"
#include "rs_param.h"
#include "rs_debug.h"

// 6531���ͷ�ļ�
#include "os_api.h"
#include "nv_item_id.h"
#include "mn_type.h"
#include "nvitem.h"
#include "version.h"
#include "sci_service.h"
#include "mn_events.h"
#include "sig_code.h"
#include "sci_api.h"



/***************************��������*****************************************/


/***************************�ֲ�����*****************************************/


/***************************flash����ʵ��************************************/
/**
����˵����flash��ʼ�� 
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_init()
{
	BOOLEAN ret = FALSE;
	
	ret = FLASH_Init();

	if(ret != TRUE) {
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash init failed\n");
		return RS_ERR_FLASH_INIT;
	}
	
	return RS_ERR_OK;
}

/**
����˵������ȡ��ǰflash��һ��page�Ĵ�С 
����˵������
����ֵ���ɹ�����page��size��ʧ�ܷ���-1 
*/
rs_s32 rs_flash_getPageSize()
{
	return PAGE_SIZE;
}

/**
����˵����flash��ʼ�� 
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_getBlockSize()
{
	return BLOCK_SIZE;
}

#if (!defined(RS_SUPPORT_UPDATE_INFO_FS) || !defined(RS_SUPPORT_UPDATE_DATA_FS))

/**
����˵��������block 
����˵����addr��Ҫ������block�ĵ�ַ
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_eraseBlock(rs_u32 addr)
{
	BOOLEAN ret = FALSE;
	
	ret = FLASH_Erase(addr);
	if (ret != TRUE)
		return RS_ERR_FLASH_WRITE;

	return RS_ERR_OK;
}


/**
����˵��������page��ȡ���� 
����˵����addr ��Ҫ��ȡ�ĵ�ַ
		 buffer ���Ҫ��ȡ���ݵĻ�����
		 len ��Ҫд��buffer���ݵĳ��ȣ�����Ϊһ��page�ĳ���
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_readPage(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{
	BOOLEAN ret = FALSE;
	rs_u32 pageSize = rs_flash_getPageSize();
	rs_u32 vAddress = addr;
	rs_u8 pageBuf[PAGE_SIZE];
	
	if (buffer == RS_NULL || len > pageSize)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read, buffer == null or len > pageSize\n");
		return RS_ERR_INVALID_PARAM;
	}
	
		// check page aligned or not
	if (vAddress & (pageSize - 1)) {
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read, address is not aligned by page\n");
		return RS_ERR_INVALID_PARAM;
	}
	
	ret = FLASH_Read(addr, pageBuf, PAGE_SIZE);
	if (ret != TRUE)
		return RS_ERR_FLASH_READ;
	rs_memcpy(buffer, pageBuf, len);
		
	return RS_ERR_OK;
}


/**
����˵��������pageд������ 
����˵����addr ��Ҫд��ĵ�ַ
		 Buffer ��Ҫд���buffer����
		 Len ��Ҫд������ݵĳ��ȣ�����Ϊһ��page�ĳ���
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_writePage(rs_u32 addr, const rs_u8* buffer, rs_s32 len)
{
	rs_u32 pageSize = rs_flash_getPageSize();
	BOOLEAN ret = FALSE;
	rs_u32 vAddress = addr;
	rs_u8 pageBuf[PAGE_SIZE];
	
	if (buffer == RS_NULL || len > pageSize)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash write, buffer == null or len > pageSize\n");
		return RS_ERR_INVALID_PARAM;
	}
	
		// check page aligned or not
	if (vAddress & (pageSize-1)) {
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash write, address is not aligned by page\n");
		return RS_ERR_INVALID_PARAM;
	}
	
	rs_memset(pageBuf, 0xff, PAGE_SIZE);
	rs_memcpy(pageBuf, buffer, len);
	ret = FLASH_Write(addr, buffer, len);
	if (ret != TRUE)
		return RS_ERR_FLASH_WRITE;
		
	return RS_ERR_OK;
}






/**
����˵��������block��ȡ���� 
����˵����addr ��Ҫ��ȡ�ĵ�ַ,block����
		 Buffer ��Ҫ��ȡ��buffer����
		 Len ��Ҫ��ȡ�����ݵĳ��ȣ�����ΪС��block�ĳ���
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_readLessBlock(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{
	rs_u32 j, vSizeCount, vSizeRemain;
	rs_u32 vAddress = addr;
	rs_u8 *p = buffer;
	rs_u8 data[PAGE_SIZE];
	rs_u32 blockSize = rs_flash_getBlockSize();
	rs_u32 pageSize = rs_flash_getPageSize();

	if (buffer == RS_NULL)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read2,  buffer == null\n");
		return RS_ERR_INVALID_PARAM;
	}

	if ((vAddress & (blockSize - 1)) != 0)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read2, address is not an aligned by BLOCK\n");
		return RS_ERR_INVALID_PARAM;
	}
	
	if (len < 0 || len > blockSize)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read2 need read len = %d\n", len);
		return RS_ERR_INVALID_PARAM;		
	}
	
	if (pageSize != PAGE_SIZE)
	{
		return RS_ERR_INVALID_PARAM;		
	}
	
	vSizeCount = (len / pageSize);
	vSizeRemain = (len % pageSize);

	for (j = 0 ; j < vSizeCount; j++)
	{
		// read data
		rs_s32 readRet = rs_flash_readPage(vAddress + j*pageSize, data, pageSize);
		if (readRet != RS_ERR_OK)
			return readRet;

		rs_memcpy(p, data, pageSize);
		
		// next page
		p += pageSize;

		rs_memset(data, pageSize, 0);
	}

	if (vSizeRemain != 0)
	{
		// read data	
		rs_s32 readRet = rs_flash_readPage(vAddress + j*pageSize, data, pageSize);
		if (readRet != RS_ERR_OK)
			return readRet;
		
		// copy remain data to destination ptr
		rs_memcpy(p, data, vSizeRemain);
	}
	
	return RS_ERR_OK;
}

/*************************************��չ�ӿ�*************************************/

/**
����˵���������ݺ�������ݶ�Ӧ��checkSumд�뵽ĳ��block��
����˵����addr ��Ҫд��ĵ�ַ,block����
		buffer ��Ҫд������ݵĻ�����
		len buffer���ݵĻ�����
		checkSum ������ݵ�У��ֵ
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_writeDataWithCSToBlock(rs_u32 addr, const rs_u8* buffer, rs_s32 len, rs_u32 checkSum)
{
	rs_u8 page_buf[PAGE_SIZE];
	rs_s32 writeLen=0;
	rs_u32 w_addr = addr;
	rs_s32 needWriteLen = len;

	if(buffer == RS_NULL || len == 0)
	{
		return RS_ERR_WRITE_DATA_FAILE;
	}

	rs_flash_eraseBlock(addr);
	while(needWriteLen >= PAGE_SIZE)
	{
		if(rs_flash_writePage(w_addr, buffer+writeLen, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
		w_addr += PAGE_SIZE;
		writeLen += PAGE_SIZE;
		needWriteLen -= PAGE_SIZE;
	}

	if (needWriteLen + sizeof(rs_u32) > PAGE_SIZE)
	{
		rs_memset(page_buf, 0xFF, PAGE_SIZE);
		rs_memcpy(page_buf, buffer+writeLen, needWriteLen);
		rs_memcpy(page_buf+needWriteLen, &checkSum, PAGE_SIZE-needWriteLen);
		if(rs_flash_writePage(w_addr, page_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}

		rs_memset(page_buf, 0xFF, PAGE_SIZE);
		rs_memcpy(page_buf, &checkSum + (PAGE_SIZE-needWriteLen), sizeof(rs_u32) - (PAGE_SIZE-needWriteLen));
		if(rs_flash_writePage(w_addr, page_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
	}
	else
	{
		rs_memset(page_buf, 0xFF, PAGE_SIZE);
		rs_memcpy(page_buf, buffer+writeLen, needWriteLen);
		rs_memcpy(page_buf+needWriteLen, &checkSum, sizeof(rs_u32));
		if(rs_flash_writePage(w_addr, page_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
	}

	return RS_ERR_OK;
}

/**
����˵������ָ����ַ��ȡ����unsigned long�͵�����ֵ
����˵����addr ��Ҫ��ȡ�ĵ�ַ
����ֵ�����ض���������ֵ
*/
rs_u32 rs_flash_readU32(rs_u32 addr)
{
	rs_u8 read_buf[PAGE_SIZE];
	rs_s32 page_offset = 0;
	rs_u32 checkSum = 0;
	rs_bool readTwoPage = RS_FALSE;
	rs_u32 blockAddr = (addr / BLOCK_SIZE) * BLOCK_SIZE;
	rs_u32 offset = addr - blockAddr;

	if ((offset/PAGE_SIZE) != ((offset + sizeof(rs_u32) - 1)/PAGE_SIZE))
	{
		readTwoPage = RS_TRUE;
	}

	if (readTwoPage == RS_TRUE)
	{
		rs_s32 alreadyRead = 0;
		rs_u8 checkSumBuf[10] = {0};
		rs_u8* p = checkSumBuf;

		rs_memset(read_buf, 0, PAGE_SIZE);
		page_offset = (offset/PAGE_SIZE)*PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		alreadyRead = PAGE_SIZE - (offset%PAGE_SIZE);
		rs_memcpy(p, read_buf+(offset%PAGE_SIZE), alreadyRead);

		rs_memset(read_buf, 0, PAGE_SIZE);
		page_offset += PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		rs_memcpy(p + alreadyRead, read_buf, sizeof(rs_u32) - alreadyRead);

		rs_memcpy(&checkSum, checkSumBuf, 4);
	}
	else
	{
		rs_memset(read_buf, 0, PAGE_SIZE);
		page_offset = (offset/PAGE_SIZE)*PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		rs_memcpy(&checkSum, read_buf+(offset%PAGE_SIZE), sizeof(rs_u32));
	}
	
	return checkSum;
}

/*
����˵�������ݵ�ַ�����ȣ�����ö����ݵ�checksumֵ
����˵����addr ��ʼ��ַ��page����
			len ���ݳ���
����ֵ��ʧ��Ϊ-1���ɹ�ΪУ��ֵ
*/
rs_u32 rs_flash_calculatCSByAddr(rs_u32 addr, rs_s32 len)
{
	rs_u32 checksum = 0;
	rs_u8 buff[PAGE_SIZE];
	rs_s32 pagecount;
	rs_s32 lessPageSize = len & (PAGE_SIZE - 1); //��page���볤�����ݴ�С
	rs_s32 i;

	if((addr & (PAGE_SIZE - 1)) != 0)
	{
		return -1;
	}

 	if (rs_open_checksum() != 1)
	{
		return -1;
	}
	pagecount = len/PAGE_SIZE ;
	if(lessPageSize > 0)
	{
		pagecount++;
	}

	for(i=0; i<pagecount; i++)
	{
		if(rs_flash_readPage(addr + i*PAGE_SIZE, buff, PAGE_SIZE) != RS_ERR_OK)
		{
			return -1;
		}

		if(i + 1 == pagecount)
		{
			if (lessPageSize != 0)
			{
				checksum = rs_get_checksum(checksum, buff, lessPageSize);
			}
			else
			{
				checksum = rs_get_checksum(checksum, buff, PAGE_SIZE);
			}
		}
		else
		{
			checksum = rs_get_checksum(checksum, buff, PAGE_SIZE);
		}
	}
	rs_close_checksum();

	return checksum;
}

#endif