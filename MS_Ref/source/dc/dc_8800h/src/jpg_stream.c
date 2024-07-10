#include "JPG_Codec.h"
#include "os_api.h"

uint16 jstream_op_mode;	
uint8  *jstream_buf;
uint8  inter_buf[INPUT_BUF_SIZE];
uint32 read_bytes;
uint32 bytes_in_buf;
uint32 jstream_len;
uint32 jstream_current_pos;
uint32 jstream_words;
uint16 jremain_bit_num;

uint32 bit_put_ops;
uint32 bit_get_ops;

uint32 ESC_MODE = 1;

uint32 DO_FREE = 0;

#define ZERO_STREAM_REC \
	if ((jstream_buf)&&(DO_FREE))\
{ \
	   free(jstream_buf); \
}\
	jstream_buf = 0; \
	jstream_op_mode = 0; \
	jstream_len = 0; \
	jstream_current_pos = 0; \
	jstream_words = 0; \
	jremain_bit_num = 0; \
	bytes_in_buf = 0;\
	read_bytes = 0

uint32 InitStreamForRW (uint8 * buf, uint32 len, uint8 ReadOrWrite)
{
	ZERO_STREAM_REC;
	
	switch(ReadOrWrite)
	{
	case READ_OP:
		jstream_buf = buf;
		jstream_len = len;
		break;
	case WRITE_OP:
		jstream_buf = buf;
		jstream_op_mode = WRITE_OP;
		jremain_bit_num = 32;
		break;
	default:
		return JPEG_FAILED;
	}
	return JPEG_SUCCESS;
}

void JPEG_Fill_Buffer(void)
{
	uint32 nbytes;
	nbytes = MIN(INPUT_BUF_SIZE, jstream_len - read_bytes);
	SCI_MEMCPY(inter_buf, jstream_buf + read_bytes, nbytes);
	read_bytes += nbytes;
	bytes_in_buf = nbytes;
	jstream_current_pos = 0;
}

void JPEG_Fill_Bit_Buffer(void)
{
	register uint8 tmp;
	while(jremain_bit_num < 15)
	{
		//read 4096 bytes into the inter_buf[];
		if(0 == bytes_in_buf)
		{
			JPEG_Fill_Buffer();
		}
		bytes_in_buf--;
		tmp = inter_buf[jstream_current_pos++];
		if (ESC_MODE && tmp == 0xFF)
		{
			do 
			{
				if (0 == bytes_in_buf)
				{
					JPEG_Fill_Buffer();
				}
				bytes_in_buf--;
				tmp = inter_buf[jstream_current_pos++];
			} while (tmp == 0xFF);

			if (tmp == 0x00)
			{
				//JPEG_TRACE("JPEG: the value after 0xFF, is 0x%2x\n", jstream_buf[jstream_current_pos]);
				tmp = 0xFF;
				jstream_words = (jstream_words << 8) | tmp;
				jremain_bit_num +=8;
			}
			else
			{
				bytes_in_buf += 2;
				jstream_current_pos -= 2;
				return;
				//	
			}
		}else
		{
			jstream_words = (jstream_words << 8) | tmp;
			jremain_bit_num +=8;
		}
	}
}

void JPEG_PutC(uint8 ch)
{
//	JPEG_ASSERT(jremain_bit_num == 32);
//	JPEG_ASSERT(jstream_current_pos < JSTREAM_BUF_SIZE);
	jstream_buf[jstream_current_pos++] = ch;
	jstream_len++;
}

void JPEG_PutW(uint16 w)
{
//	JPEG_ASSERT(jremain_bit_num == 32);
//	JPEG_ASSERT(jstream_current_pos < JSTREAM_BUF_SIZE-2);
	jstream_buf[jstream_current_pos++] = (w>>8);
	jstream_buf[jstream_current_pos++] = w&0xFF;
	jstream_len += 2;
}

void JPEG_PutBits(uint16 bits, uint16 size)
{
	uint32 mask;
	uint16 tmp;
	uint8 i, k, tmp1;

	if (!size)
	{
		//JPEG_ASSERT(0);
		return;
	}

	mask = (1<<size) - 1;
	bits =(uint16)( mask & bits );
	
	if (jremain_bit_num > size)
	{
		jstream_words = (jstream_words<<size)| bits;
		jremain_bit_num -= size;
	}
	else
	{
//		JPEG_ASSERT(jstream_current_pos < JSTREAM_BUF_SIZE-4);

		tmp = size - jremain_bit_num;
		jstream_words = (jstream_words<<jremain_bit_num)|(bits>>(tmp));

		/*write to buffer*/
		for (i=0, k=24; i<4; i++, k-=8)
		{
			tmp1 = (uint8)(jstream_words>>k);
			jstream_buf[jstream_current_pos++] = tmp1;
			
			if (ESC_MODE)
			{
				if (tmp1 == 0xFF)
				{
					jstream_buf[jstream_current_pos++] = 0x00;
					jstream_len++;
				}
			}
		}

		if (tmp)
		{
			jstream_words = ((1<<tmp)-1)&bits;
			jremain_bit_num = 32 - tmp;
		}
		else
		{
			jstream_words = 0;
			jremain_bit_num = 32;
		}
		jstream_len += 4;
	}

}

void JPEG_Flush(uint32 value)
{
	uint32 mask;
	uint16 i, k;
	uint8 tmp;

	if (jstream_op_mode == WRITE_OP)
	{
		if (jremain_bit_num == 32)
		{
			return;
		}

		mask = (1<<jremain_bit_num ) - 1;
		value &=  mask;
		jstream_words = (jstream_words<<jremain_bit_num)|value;
		i = (32 - jremain_bit_num + 7)>>3;
		k = 24;
		jstream_len += i;
		while(i)
		{
			tmp = (uint8)(jstream_words>>k);
			jstream_buf[jstream_current_pos++] = tmp;
			
			if (ESC_MODE)
			{
				if (tmp == 0xFF)
				{
					jstream_buf[jstream_current_pos++] = 0x00;
					jstream_len++;
				}
			}
			k -= 8;
			i--;
		}

		jstream_words = 0;
		jremain_bit_num = 32;
	}
	else
	{
		i = jremain_bit_num>>3;
		jstream_current_pos -= i;
		jremain_bit_num = 0;
		jstream_words = 0;
	}

}

uint8 JPEG_GetC()
{
	uint8 ret;
	if(bytes_in_buf == 0)
	{
		JPEG_Fill_Buffer();
	}
	ret = jstream_buf[jstream_current_pos++];
	bytes_in_buf --;
	return ret;
}

uint16 JPEG_GetW()
{
	uint16 ret;
	if(bytes_in_buf == 0)
	{
		JPEG_Fill_Buffer();
	}

	ret = inter_buf[jstream_current_pos++];
	ret = (ret<<8) | inter_buf[jstream_current_pos++];
	bytes_in_buf -= 2;
	return ret;
}