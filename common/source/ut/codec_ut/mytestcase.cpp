// MyTestCase.cpp: implementation of the CMyTestCase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "string.h"
#include "time.h"

#include "codec_ut.h"
#include "mytestcase.h"

#include "..\..\sci_codec.c"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(MyTestCase);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MyTestCase::MyTestCase()
{

}

MyTestCase::~MyTestCase()
{

}

// UT loops
#define MAX_LOOP 10

// for trans 7bits data to 8 bits data, old function can not support an array longer than 160
// so if need compare with old function, please define COMPARE_WITH_OLD_FUNC

#define COMPARE_WITH_OLD_FUNC

#ifdef COMPARE_WITH_OLD_FUNC
#define MAX_LEN 160
#else
#define MAX_LEN 4000
#endif




typedef struct _MN_SMS_USER_VALID_DATA_T
{	 
	uint8 length;                       // the length
	uint8 user_valid_data_arr[MAX_LEN]; // the data buf 
} MN_SMS_USER_VALID_DATA_T;

#define COMPRESSED_CHAR_BITS_LEN	7       // a byte compress into 7 bits
#define NOMAL_CHAR_BITS_LEN			8       // normal char bits len


void CompressCharToBuf(
			uint8	compress_octet,	// the data compressed into buffer
			uint8	*bit_offset,	// the current offset in bits list of a byte
			uint8	*octet_offset,	// the current offset in byte list
			uint8	*output_str_ptr	// pointer of memory contain compressed data
			)
{
	uint8 left_bits;
	
	assert(bit_offset		!= NULL);
	assert(octet_offset		!= NULL);
	assert(output_str_ptr	!= NULL);
	assert(*bit_offset <= COMPRESSED_CHAR_BITS_LEN);
	
	left_bits	= (uint8)(NOMAL_CHAR_BITS_LEN - *bit_offset);
	if(left_bits < NOMAL_CHAR_BITS_LEN)
	{
		// write left-bits length bits into current byte(high bits)
		output_str_ptr[*octet_offset] = (uint8)(output_str_ptr[*octet_offset] + (compress_octet << *bit_offset));
		(*octet_offset)++;
		// write left bits of compress-octet into the later byte
		*bit_offset = (uint8)(COMPRESSED_CHAR_BITS_LEN - left_bits);
		output_str_ptr[*octet_offset] = (uint8)(compress_octet >> left_bits);
	}
	else
	{
		// write compress charater into current byte
		output_str_ptr[*octet_offset] = (uint8)(output_str_ptr[*octet_offset] + (compress_octet << *bit_offset));
		*bit_offset = (uint8)(*bit_offset + COMPRESSED_CHAR_BITS_LEN);
		SCI_ASSERT(*bit_offset <= NOMAL_CHAR_BITS_LEN);
		if(NOMAL_CHAR_BITS_LEN == *bit_offset)
		{
			// 8 bits of current byte is written
			*bit_offset = 0;
			*octet_offset++;
		}    
	}    
}


void Transfer8Bitsto7Bits(
				MN_SMS_USER_VALID_DATA_T	*user_data_ptr,
				uint8						*data_ptr,        // out--the message structure
				uint8                       bit_off
                )
{
	uint8	i				= 0,
			bit_offset		= bit_off,
			octet_offset	= 0;

	for(i=0; i<user_data_ptr->length; i++)
	{
		CompressCharToBuf(user_data_ptr->user_valid_data_arr[i], &bit_offset, &octet_offset, data_ptr);   	
	}
}



void MyTestCase::TestCase7bitsTo8bits()
{
	unsigned char u7BitsData[MAX_LEN];
	unsigned char u8BitsData[MAX_LEN];
	unsigned char u8BitsData1[MAX_LEN];

	short i7BitsDataLen;
	short i8BitsDataLen;
	unsigned char i7BitsDataOffsetBegin, i7BitsDataOffsetEnd;

	unsigned short len;

	unsigned char newStart[] = {0xff, 0xff};

	MN_SMS_USER_VALID_DATA_T EightBitsData;
	unsigned char SevenBitsData[MAX_LEN];
	

	TRY
	{
		CFile cFileSource("d:\\eightBitsData.dat", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CFile cFileTarget("d:\\sevenBitsData.dat", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CFile cFileTargetSunsome("d:\\sevenBitsDataSunsome.dat", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CFile cFileTarget1("d:\\eightBitsData1.dat", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		for (int i = 0; i < MAX_LOOP; i++)
		{
			srand( (unsigned)time( NULL ) );

			len = rand() % (MAX_LEN + 1);

			for (int j = 0; j < len; j++)
			{
				u8BitsData[j] = rand()%128;
				EightBitsData.user_valid_data_arr[j] = u8BitsData[j];
			}

			EightBitsData.length = len;

			i7BitsDataLen = MAX_LEN;
			i7BitsDataOffsetBegin = rand() % 8;

			i7BitsDataOffsetEnd = i7BitsDataOffsetBegin;

			cFileSource.Write(u8BitsData, len);
			cFileSource.Write(newStart, 2);

			CPPUNIT_ASSERT(SCI_SUCCESS == SCI_Pack8bitsTo7bits(u8BitsData, len, u7BitsData, &i7BitsDataLen, &i7BitsDataOffsetEnd));

			cFileTarget.Write(u7BitsData, i7BitsDataLen);
			cFileTarget.Write(newStart, 2);
			cFileTarget.Write(&i7BitsDataOffsetBegin, 1);

#ifdef COMPARE_WITH_OLD_FUNC

			SCI_MEMSET(SevenBitsData, 0, MAX_LEN);
			Transfer8Bitsto7Bits(&EightBitsData, SevenBitsData, i7BitsDataOffsetBegin);

			cFileTargetSunsome.Write(SevenBitsData, i7BitsDataLen);
			cFileTargetSunsome.Write(newStart, 2);
			cFileTargetSunsome.Write(&i7BitsDataOffsetBegin, 1);
#endif

			i8BitsDataLen = MAX_LEN;
			CPPUNIT_ASSERT(SCI_SUCCESS == SCI_Unpack7bitsTo8bits(u7BitsData, i7BitsDataLen, i7BitsDataOffsetBegin, u8BitsData1, &i8BitsDataLen));

			cFileTarget1.Write(u8BitsData1, i8BitsDataLen);
			cFileTarget1.Write(newStart, 2);

			for (short k = 0; k < len; k++)
			{
				CPPUNIT_ASSERT(((u8BitsData[k])&0x7f) == ((u8BitsData1[k])&0x7f));
			}

#ifdef COMPARE_WITH_OLD_FUNC

			// no compare the first byte for the remain part of the first byte is random data
			for (k = 1; k < i7BitsDataLen; k++ )
			{
				CPPUNIT_ASSERT(u7BitsData[k] == SevenBitsData[k]);
			}
#endif


			Sleep(1000);
		}

		cFileSource.Close();
		cFileTarget.Close();
		cFileTarget1.Close();
		cFileTargetSunsome.Close();
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	END_CATCH

}



// first get ucs2 array from file d:\ucs2Str.txt
// then trans it to a gb2312 one
// then trans back to a ucs2 one
void MyTestCase::TestCaseGbToUcs2()
{
	unsigned char ucs2Str[MAX_LEN * 2];
	unsigned char ucs2Str1[MAX_LEN * 2];
	unsigned char gb2312Str[MAX_LEN * 2];

	unsigned char temp;

	short ucs2Len;
	short gb2312Len;

	short nBytesReaded;

	TRY
	{
		CFile cFileUcs2Source("d:\\ucs2Str.txt", CFile::modeRead|CFile::typeBinary);
		CFile cFileUcs2Target("d:\\ucs2Str1.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		CFile cFileGbTarget("d:\\gb2312Str1.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		nBytesReaded = cFileUcs2Source.Read(ucs2Str, MAX_LEN * 2);

		// little endian to big endian
		// the first and second byte of a ucs2 file is 0xff and 0xfe
		for (short loop = 1; loop < nBytesReaded / 2; loop++)
		{
			temp = ucs2Str[2 * loop + 1];
			ucs2Str[2 * loop + 1] = ucs2Str[2 * loop];
			ucs2Str[2 * loop] = temp;
		}

		gb2312Len = MAX_LEN * 2;
		CPPUNIT_ASSERT(SCI_SUCCESS == SCI_TransUCS2StrToGB2312Str(ucs2Str+2, nBytesReaded-2, gb2312Str, &gb2312Len));

		cFileGbTarget.Write(gb2312Str, gb2312Len);

		ucs2Str1[0] = 0xff;
		ucs2Str1[1] = 0xfe;

		ucs2Len = MAX_LEN * 2 - 2;

		CPPUNIT_ASSERT(SCI_SUCCESS == SCI_TransGB2312StrToUCS2Str(gb2312Str, gb2312Len, ucs2Str1 + 2, &ucs2Len));

		cFileUcs2Target.Write(ucs2Str1, ucs2Len + 2);

		CPPUNIT_ASSERT(nBytesReaded == ucs2Len + 2);
		for (short i = 0; i < nBytesReaded; i++)
		{
			CPPUNIT_ASSERT(ucs2Str[i] == ucs2Str1[i]);
		}

		cFileUcs2Source.Close();
		cFileUcs2Target.Close();
		cFileGbTarget.Close();
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	END_CATCH


}

void MyTestCase::TestCaseHexStrToBin()
{
	char HEX_CHAR_ARRAY[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	char strHex[MAX_LEN - 1];
	char strHex1[MAX_LEN - 1];

	unsigned char ubBin[MAX_LEN - 2];
	short ubBinLen;

	unsigned short len;

	char chEnter[] = {0xd, 0xa};
	unsigned char newStart[] = {0xff, 0xff};

	TRY
	{
		CFile cFileSource("d:\\HexStr.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CFile cFileTarget("d:\\HexBin.dat", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		for (int i = 0; i < MAX_LOOP; i++)
		{
			srand( (unsigned)time( NULL ) );

			len = rand() % MAX_LEN;
			len = (len / 2) * 2;
			for (int j = 0; j < len; j++)
			{
				strHex[j] = HEX_CHAR_ARRAY[rand()%16];
			}
			strHex[len] = '\0';

			ubBinLen = MAX_LEN - 2;
			CPPUNIT_ASSERT(SCI_SUCCESS == SCI_TransHexStrToBin(strHex, ubBin, &ubBinLen));
			CPPUNIT_ASSERT(SCI_SUCCESS == SCI_TransBinToHexStr(ubBin, ubBinLen, strHex1, MAX_LEN));

			CPPUNIT_ASSERT(0 == strcmp(strHex, strHex1));

			cFileSource.Write(strHex, len);
			cFileSource.Write(chEnter, 2);
			cFileTarget.Write(ubBin, ubBinLen);
			cFileTarget.Write(newStart, 2);

			CPPUNIT_ASSERT(0 == strcmp(strHex, strHex1));

			Sleep(1000);
		}

		cFileSource.Close();
		cFileTarget.Close();
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	END_CATCH

}

void MyTestCase::TestCaseBCDToBin()
{
	char BCD_CHAR_ARRAY[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '#', 'a', 'b', 'c'};
	char strBCD[MAX_LEN];
	char strBCD1[MAX_LEN];

	unsigned char ubBin[MAX_LEN - 1];
	short ubBinLen;

	unsigned short len;

	char chEnter[] = {0xd, 0xa};
	unsigned char newStart[] = {0xff, 0xff};

	TRY
	{
		CFile cFileSource("d:\\BCDStr.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CFile cFileTarget("d:\\BCDBin.dat", CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		for (int i = 0; i < MAX_LOOP; i++)
		{
			srand( (unsigned)time( NULL ) );

			len = rand() % MAX_LEN;
			for (int j = 0; j < len; j++)
			{
				strBCD[j] = BCD_CHAR_ARRAY[rand()%15];
			}
			strBCD[len] = '\0';

			ubBinLen = MAX_LEN - 1;
			CPPUNIT_ASSERT(SCI_SUCCESS == SCI_TransStrToBCD(strBCD, ubBin, &ubBinLen));
			CPPUNIT_ASSERT(SCI_SUCCESS == SCI_TransBCDToStr(ubBin, ubBinLen, strBCD1, MAX_LEN));

			cFileSource.Write(strBCD, len);
			cFileSource.Write(chEnter, 2);
			cFileTarget.Write(ubBin, ubBinLen);
			cFileTarget.Write(newStart, 2);

			CPPUNIT_ASSERT(0 == strcmp(strBCD, strBCD1));

			Sleep(1000);
		}

		cFileSource.Close();
		cFileTarget.Close();
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	END_CATCH

}