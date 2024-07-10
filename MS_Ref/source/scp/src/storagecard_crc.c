/******************************************************************************
 ** File Name:      StorageCard_crc.h                                                 *
 ** Author:         Zhemin.Lin                                                *
 ** DATE:           11/11/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    define crc16 and crc7 operation interface                 *
 ******************************************************************************/

#include "sci_types.h"
#include "StorageCard_crc.h"

//crc7 polynomial
#define CRC7_POLYNOMIAL 0x89            /* x^7 + x^3 + 1 */
//crc16 plynomial
#define CRC16_POLYNOMIAL 0x1021

//crc table
static uint8 crc7_table[256];
static uint16 crc16_table[256];

/*
 * Generate a table of CRC-7 for each possible input byte
 */
LOCAL void _GenCrc7Table (void)
{
	uint32 i, j, crc;
  
	for (i = 0;  i < 256;  ++i)
	{
		crc = ((i & 0x80) != 0)? i ^ CRC7_POLYNOMIAL : i;
		for (j = 0;  j < 7;  ++j)
		{
			if (((crc <<= 1) & 0x80) != 0)
			{
				crc ^= CRC7_POLYNOMIAL;
			}
		}
		crc7_table[i] = (uint8) crc;
	}
}

/*
 * Generate the CRC remainders table for all possible bytes
 */
LOCAL void _GenCrc16Table()
{
    uint16 i, j;
    uint16 crc_accum;
    uint16 *pcrctable;

	pcrctable = crc16_table;
	
    for(i = 0; i < 256; i++)
    {
    	crc_accum = i << 8;
		for(j = 0; j < 8; j++ )
		{
			if(crc_accum & 0x8000)
			{
				crc_accum = (crc_accum << 1) ^ CRC16_POLYNOMIAL;
			}
			else
			{
				crc_accum = (crc_accum << 1);
			}
		}
		
		*pcrctable++ = crc_accum;
	}

	return;
}


LOCAL void _GenCrcTable()
{
	static uint32 table_is_ready = 0;
	if(!table_is_ready)
	{
		_GenCrc16Table();
		_GenCrc7Table();
		table_is_ready = 1;
	}
}
/*
 * get the CRC-7 
 */
PUBLIC uint8 SC_GetCrc7(uint8 *pdata, uint32 len)
{
	uint8 crc7_accum = 0;
	uint32 i;

	_GenCrcTable();

	for (i = 0;  i < len;  ++i)
	{
		crc7_accum =
			crc7_table[(crc7_accum << 1) ^ pdata[i]];
	}
	
	return (crc7_accum&0x7F);
}


/*
 *  CRC16 Calculation function
 */
PUBLIC uint16 SC_GetCrc16(uint8* pdata, uint32 len)
{
	uint16 crc_accum = 0;
	uint16 i;

	_GenCrcTable();
        
	while(len--){
		i =( (int) (crc_accum >> 8) ^ *pdata++ ) & 0xff;
		crc_accum = (crc_accum << 8) ^ crc16_table[i];
	}
	
	return crc_accum;
}


