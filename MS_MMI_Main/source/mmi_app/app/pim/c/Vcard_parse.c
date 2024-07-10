/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : Vcard_parse.c
*
*Description: 
*
*Notes        :N/A
*
*---------------------------------------------------------------------------
*Change History: 
*----------------------------------------------------------------------------
*
*2009-7-7 ,wangxiaolin,create originally.
*
*******************************************************************************/

//add by wxl 2007.04.26
#include "mmi_app_pim_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "vCard.h"
#include "vCal.h"
#include "Vcard_parse.h"
#include "mmipb_export.h"
#include "mmk_type.h"
//#include "guifont.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "guicommon.h"

#include "ffs.h"

#include "mmifmm_export.h"

#include "mmipim.h"
#include "pimlibmem.h"
#include "PIMbase64.h"
#include "../h/list.h" //add  by wangxiaolin  2009.12.20 
#include "guifont.h" //add  by wangxiaolin  2009.12.20 
//================================================
#define MAX_CHAR 512
#define VCARD_FILENAME_LEN				100
#define READ_BUFFER_SIZE 4096// 4K buffer..
#define MAX_VCARD_LEN2 21
 
//char wrBuf[MAX_CHAR];
//char tempBuf[MAX_CHAR];
 static  BOOL is_CHN = FALSE;
//queyuan add for error code 
#define VACAR_EMAIL_LEN 41
//static int g_vcard_len=0;

//add over
//char parseData[READ_BUFFER_SIZE];//为大对象解析的时候预留，改为动态分东
VCARD_TYPE Vcardfile;

static const char AdrType[TOTAL_ADR][10]=
    {"DOM",
     "INTL",
     "POSTAL",
     "PARCEL",
     "HOME",
     "WORK"
    };

static const char TelType[TOTAL_TEL][10]=
    {"PREF",
     "WORK",
     "HOME",
     "VOICE",
     "FAX",
     "MSG",
     "CELL",
     "PAGER",
     "BBS",
     "MODEM",
     "CAR",
     "ISDN",
     "VIDEO"
    };

static const char EmailType[TOTAL_EMAIL][12]=
    {"AOL",
     "APPLELINK",
     "ATTMAIL",
     "CIS",
     "EWORLD",
     "INTERNET",
     "IBMMAIL",
     "MCIMAIL",
     "POWERSHARE",
     "PRODIGY",
     "TLX",
     "X400"
    };


static const char ImageType[TOTAL_IMAGE_TYPE][10] =
    {"GIF",
     "CGM",
     "WMF",
     "BMP",
     "MET",
     "PMG",
     "DIB",
     "PICT",
     "TIFF",
     "PS",
     "PDF",
     "JPEG",
     "MPEG",
     "MPEG2",
     "AVI",
     "QTIME"
    };


static const char SoundType[TOTAL_SOUND][10]=
    {"WAVE",
     "PCM",
     "AIFF"
    };


static const char KeyType[TOTAL_KEY][10]=
    {"X509",
     "PGP"
    };
//add by wxl 2007.04.26
static const uint8 bytesFromUTF8[256] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /*32 each line*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5};

static const uint32 offsetsFromUTF8[6] ={
0x00000000UL,/* min value which 0xxx xxxx can state,*/ 
0x00003080UL,/* 110x xxxx 10xx xxxx, 1100 000x is illegal, that is to say, should >=0x80*/ 
0x000E2080UL,/* 1110 xxxx 10xx xxxx 10xx xxxx,*/
0x03C82080UL,/* 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx,*/ 
0xFA082080UL,/* 1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx,*/  
0x82082080UL /* 1111 110x 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx,*/
			};

static uint16 Uint8StrToUint16Ucs2(
    uint16* dest_ucs2_ptr, //[OUT] ucs2 code
    uint16  dest_ucs2_len, //[IN]  ucs2 length (word)
    uint8*  src_str_ptr,   //[IN]  string
    uint8  src_str_len  //[IN]  string length (byte)
);

static unsigned char HexToChr(char chr);

static char *StrUpr(char *inPtr);

//add by wxl 2007.04.26






static void Str2Wstr
(
    uint16       *out_len_ptr,       //OUT:
    wchar       *out_alpha_ptr,     //OUT:
    uint16      out_max_len,        //IN:
    BOOLEAN     is_ucs2,            //IN:
    uint16       in_len,             //IN:
    const  char       *in_alpha_ptr       //IN:
)
{
	uint16 len=0;
#if 0//del by wangxiaolin  2009.10.16
    SCI_ASSERT(PNULL != out_len_ptr);
    SCI_ASSERT(PNULL != out_alpha_ptr);
#else
if((PNULL == out_len_ptr)||(PNULL == out_alpha_ptr))
{
    return;
}
#endif
    
    if (0 < in_len)
    {
#if 0//del by wangxiaolin  2009.10.16
        SCI_ASSERT(PNULL != in_alpha_ptr);
#else
        if(PNULL == in_alpha_ptr)
        {
            return;
        }
#endif
        if (is_ucs2)
        {
            len=in_len/sizeof(wchar);
            *out_len_ptr = MIN(len, out_max_len);
            MMIAPICOM_Wstrncpy(out_alpha_ptr, (wchar *)in_alpha_ptr, *out_len_ptr);

        }
        else
        {
            *out_len_ptr = MIN(in_len, out_max_len);
            MMI_STRNTOWSTR(out_alpha_ptr, out_max_len, in_alpha_ptr, out_max_len, *out_len_ptr);/*lint !e64*/
        }
    }
    else
    {
        *out_len_ptr = 0;
        hs_smlLibMemset( out_alpha_ptr,
                    0XFF,
                    (out_max_len*sizeof(wchar))
                    );
    }
    //syncml_vcard_parse_message:"mmipim in_str=%s,len=%d,out_max_len=%d,out_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_224_112_2_18_2_47_24_309,(uint8*)"sddd",in_alpha_ptr,in_len,out_max_len,*out_len_ptr);

}

/*convert the utf8 bytes stream to Unicode string*/
void	VmByteToUnicode (const char *UTF8Start, uint32 UTF8Size,
			uint16 *UCS2Start, uint32 UCS2Size)
{
	uint8* UTF8Buffer = (uint8 *)UTF8Start;
	uint16	*UCS2Buffer = UCS2Start;
	uint16	*UCS2End = UCS2Buffer + UCS2Size;
	uint8* UTF8End = UTF8Buffer + UTF8Size;

	while (UTF8Buffer < UTF8End) {
		uint32 wch = 0;
		uint16 extraBytesToWrite = bytesFromUTF8[*UTF8Buffer];
		if (UTF8Buffer + extraBytesToWrite > UTF8End) {
			break;
		}
		switch(extraBytesToWrite) {	/* note: code falls through cases! */
			case 5:	wch += *UTF8Buffer++; wch <<= 6;/*lint !e825*/
			case 4:	wch += *UTF8Buffer++; wch <<= 6;/*lint !e825*/
			case 3:	wch += *UTF8Buffer++; wch <<= 6;/*lint !e825*/
			case 2:	wch += *UTF8Buffer++; wch <<= 6;/*lint !e825*/
			case 1:	wch += *UTF8Buffer++; wch <<= 6;/*lint !e825*/
			case 0:	wch += *UTF8Buffer++;/*lint !e825*/
                         break;
            default:/*lint !e825*/
                break;
		}
		wch -= (uint16)offsetsFromUTF8[extraBytesToWrite];

		if (UCS2Buffer >= UCS2End) {
			break;
		}
		*UCS2Buffer++ = (uint16)wch;
	}
}


//add by wxl 2007.04.26
/* Convert wide character string (Unicode) to UTF8 byte stream. */
void VmUnicodeToByte(const uint16 *UCS2Start, uint32 UCS2Size,
			uint8 *UTF8Start, uint32 UTF8Size)
{
#define REPLACEMENT_CHAR		0x0000FFFD
#define MAX_UCS2 			0x0000FFFF
#define UTF_MASK			0xBF
#define UTF_FLAG			0x80

	uint16 MSBFlag[] = {0x00, 0x00, 0xC0, 0xE0};
	uint16	m_wchar;
	uint16	UTF8Bytes = 0;
	uint16	*UCS2Buffer = (uint16*)UCS2Start;
	uint8	*UTF8Buffer = UTF8Start;
	uint16	*UCS2End = UCS2Buffer + UCS2Size;
	uint8	*UTF8End = UTF8Buffer + UTF8Size;

	while (UCS2Buffer < UCS2End) 
	{
		m_wchar = *UCS2Buffer++;
		if (m_wchar <= 0x7F) 
		{
			UTF8Bytes = 1;
		} 
		else if (m_wchar <= 0x07FF) 
		{
			UTF8Bytes = 2;
		}
#if 0//del by wangxiaolin   2009.12.20
		else if (m_wchar <= 0xFFFF) 
		{	
			UTF8Bytes = 3;
		} 
		else 
		{						
			UTF8Bytes = 2;
			m_wchar = REPLACEMENT_CHAR;
		} 
#else
             else
		{	
			UTF8Bytes = 3;
		} 
#endif
		
		UTF8Buffer += UTF8Bytes;
		if (UTF8Buffer > UTF8End) 
		{
			UTF8Buffer -= UTF8Bytes; 
			break;
		}
		switch (UTF8Bytes) 
		{	/* note: no break; in cases */
			case 3:	
				*--UTF8Buffer =(uint8)((m_wchar | UTF_FLAG) & UTF_MASK); 
				m_wchar >>= 6;
			case 2:	 /*lint !e616 !e825*/
				*--UTF8Buffer = (uint8)((m_wchar | UTF_FLAG) & UTF_MASK); 
				m_wchar >>= 6;
			case 1:	/*lint !e616 !e825*/
				*--UTF8Buffer =  (uint8)(m_wchar | MSBFlag[UTF8Bytes]);
                         break;
            default:/*lint !e616 !e825*/
                break;
		}
		UTF8Buffer += UTF8Bytes;
	}
}
int pim_unicode_strlen(unsigned short *src)
{
    int count = 0;

    while (*src){
        count++;
        src++;
	}

    //syncml_vcard_parse_message:"bt_vcard_unicode_strlen() return  %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_340_112_2_18_2_47_24_310,(uint8*)"d", count);

    return count;
}
static int bt_vcard_unicode_strlen(uint8 *src)
{
    int count = 0;

    while (!((*src==0x00)&&(*(src+1)==0x00))){
        count=count+2;
        src=src+2;
	}

    //syncml_vcard_parse_message:"bt_vcard_unicode_strlen() return  %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_353_112_2_18_2_47_24_311,(uint8*)"d", count);

    return count;
}

void uim_tk_test_print_data(uint8* data,uint8 data_len)
{
     uint8 i=0;
     for (i=0;i<data_len;i++)
     {
          //syncml_vcard_parse_message:"[vcardName] data[%d] = 0x%02x"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_363_112_2_18_2_47_25_312,(uint8*)"dd",i,data[i]);
     }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static char *StrUpr(char *inPtr)
{
    unsigned int i;

    if (!inPtr)
        return NULL;

    for (i=0; i<strlen(inPtr); i++)
    {
        /* a..z ==> A..Z */
        if ((inPtr[i] >= 'a') && (inPtr[i] <= 'z'))
            inPtr[i] -= 0x20;
    }

    return inPtr;
}

BOOLEAN FileIsVcard(char *inFile)
{
    if ( (strstr(inFile, BEGIN_VCARD_DELIMITER)) && (strstr(inFile, END_VCARD_DELIMITER)) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

char *IdentifySemicolon(char *inFile, char *found)
{
    char *lfcr;
    char *semicolon;

    lfcr = strstr(inFile, LFCR);
    semicolon = strstr(inFile, SEMICOLON);

    if (semicolon < lfcr)/*lint !e613*/
    {
        *found = 1;
        return semicolon;
    }
    else
    {
        *found = 0;
        return NULL;
    }
}

signed int IdentifySemiColonOrDelimiter(char *inFile)
{
    if (*inFile == ':')
        return 1;
    else if (*inFile == ';')
        return 2;
    else if (*inFile == '/')
        return 3;
    else
        return -1;
}

char *IdentifyDelimiters(char *inFile, char *eol)
{
    char *lfcr;
    char *delimiters;
#if 0 // chenxiang 20090214 
    char *backSlash;
#endif

    lfcr = strstr(inFile, LFCR);
    delimiters = strstr(inFile, DELIMITERS);
#if 0 // chenxiang 20090214 
    backSlash = strstr(inFile, "\\");
#endif

    if (!lfcr)
        return NULL;
#if 0 // chenxiang 20090214 
    if ( (backSlash) && ( (delimiters>backSlash) || (!delimiters) ) )
    {
        *eol = 1;
        return backSlash;
    }
    else if ( (lfcr) && (!delimiters) )
#else
    if (!delimiters) 
#endif
    {
        *eol = 1;
        return lfcr;
    }

    if (delimiters < lfcr)
    {
        *eol = 0;
        return delimiters;
    }
    else
    {
        *eol = 1;
        return lfcr;
    }
}

signed int IdentifyEncoding(char *inFile, IRMC_ENCODING_TYPE *encoding)
{
    char *foundStr;
    char *headPtr;
    unsigned int i, searchLen;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
    foundStr = strstr(upperCasePtr, ENCODING);

    if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
    {
        // could it be encoding with ENCODING=
        // check if
        foundStr = strstr(upperCasePtr, QUOTED_PRINTABLE_ENCODING);
        if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
        {
            foundStr = strstr(upperCasePtr, BASE_64_ENCODING);
            if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
            {
                foundStr = strstr(upperCasePtr, EIGHT_BIT_ENCODING);
                if (foundStr)
                {
                    encoding->eightBit = 1;
                }
                else
                {
                     return -1;
                }
           #if 0//del by wangxiaolin   2009.12.20  for del warning 
                else if ( (!foundStr) || (delimiters <foundStr) )/*lint !e774*/
                    return -1;
           #endif
            }
            else if (delimiters > foundStr)/*lint !e613*/
                encoding->base64 = 1;
        }
        else
            encoding->quotedPrintable = 1;

        return 0;
    }

    searchLen = delimiters - foundStr - (unsigned long)strlen(ENCODING);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(ENCODING);

    for (i=0; i<=searchLen; i++)
    {
        if (*(upperCasePtr+i) == ';')
            break;
        else if (*(upperCasePtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -1;
    }

    foundStr = strstr(upperCasePtr, QUOTED_PRINTABLE_ENCODING);
    if (!foundStr)
    {
        foundStr = strstr(upperCasePtr, BASE_64_ENCODING);
        if (!foundStr)
        {
            foundStr = strstr(upperCasePtr, EIGHT_BIT_ENCODING);
            if (foundStr)
                encoding->eightBit = 1;
        }
        else
            encoding->base64 = 1;
    }
    else
        encoding->quotedPrintable = 1;

    return 0;
}

signed int IdentifyCharSet(char *inFile, char **charSet)
{
    char *foundStr;
    unsigned int i, searchLen;
    char *headPtr;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;
    //syncml_vcard_parse_message:"IdentifyCharSet "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_573_112_2_18_2_47_25_313,(uint8*)"");

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
	if(delimiters)
	{
    	//syncml_vcard_parse_message:"IdentifyCharSet  delimiters %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_582_112_2_18_2_47_25_314,(uint8*)"d",*delimiters);
	}

    foundStr = strstr(upperCasePtr, CHARSET);
   // syncml_vcard_parse_message(("IdentifyCharSet  foundStr %d dis %d",*foundStr,delimiters - foundStr));

    if (!foundStr)
        return -1;
    else if (delimiters < foundStr)/*lint !e613*/
        return -2;

    searchLen = delimiters - foundStr - (unsigned long)strlen(CHARSET);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(CHARSET);

    for (i=0; i<=searchLen; i++)
    {
        if (*(headPtr+i) == ';')
            break;
        else if (*(headPtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -1;
    }
    //syncml_vcard_parse_message:"IdentifyCharSet char= %d,len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_609_112_2_18_2_47_25_315,(uint8*)"dd",foundStr[0],i);
    *charSet = (char *)SCI_ALLOC(i+1);
    memcpy(*charSet, foundStr+strlen(CHARSET), i);
    (*charSet)[i] = '\0';
    return 0;
}

signed int IdentifyLanguage(char *inFile, char **language)
{
    char *foundStr;
    unsigned int i, searchLen;
    char *headPtr;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);

    foundStr = strstr(upperCasePtr, LANGUAGE);
    if (!foundStr)
        return -1;
    else if (delimiters < foundStr)/*lint !e613*/
        return -2;

    searchLen = delimiters - foundStr - (unsigned long)strlen(LANGUAGE);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(LANGUAGE);

    for (i=0; i<=searchLen; i++)
    {
        if (*(headPtr+i) == ';')
            break;
        else if (*(headPtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -1;
    }

    *language = (char *)SCI_ALLOC(i+1);
    memcpy(*language, foundStr+strlen(LANGUAGE), i);
    (*language)[i] = '\0';
    return 0;
}

signed int IdentifyValueLocation(char *inFile, IRMC_VALUE_LOCATION_TYPE *valueLocation)
{
    char *foundStr;
    char *headPtr;
    unsigned int i, searchLen;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
    foundStr = strstr(upperCasePtr, VALUE);

    if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
    {
        // could it be encoding with VALUE=
        // check if
        foundStr = strstr(upperCasePtr, CONTENT_ID_VL);
        if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
        {
            foundStr = strstr(upperCasePtr, URL_VL);
            if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
            {
                return -1;
            }
            else if (delimiters > foundStr)/*lint !e613*/
                valueLocation->url = 1;
        }
        else
            valueLocation->cid = 1;

        return 0;
    }

    searchLen = delimiters - foundStr - (unsigned long)strlen(VALUE);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(VALUE);

    for (i=0; i<=searchLen; i++)
    {
        if (*(upperCasePtr+i) == ';')
            break;
        else if (*(upperCasePtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -1;
    }

    foundStr = strstr(upperCasePtr, CONTENT_ID_VL);
    if (!foundStr)
    {
        foundStr = strstr(upperCasePtr, URL_VL);
        if (foundStr)
            valueLocation->url = 1;
    }
    else
        valueLocation->cid = 1;

    return 0;
}

signed int IdentifyImageType(char *inFile, VCARD_IMAGE_CONSTANT *imageType)
{
    char *foundStr;
    char *headPtr;
    unsigned int i, searchLen;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
    foundStr = strstr(upperCasePtr, TYPE);

    if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
    {
        // may be it is not with TYPE=
        // check if
        for (i=IMAGE_GIF_TYPE; i<TOTAL_IMAGE_TYPE; i++)
        {
            foundStr = strstr(upperCasePtr, ImageType[i]);
            if (foundStr)
                break;
        }

        if (i==TOTAL_IMAGE_TYPE)
            return -2;

        *imageType = (VCARD_IMAGE_CONSTANT)i;

        return 0;
    }

    searchLen = delimiters - foundStr - (unsigned long)strlen(TYPE);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(TYPE);

    for (i=0; i<=searchLen; i++)
    {
        if (*(upperCasePtr+i) == ';')
            break;
        else if (*(upperCasePtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -3;
    }

    for (i=IMAGE_GIF_TYPE; i<TOTAL_IMAGE_TYPE; i++)
    {
        foundStr = strstr(upperCasePtr, ImageType[i]);
        if (foundStr)
            break;
    }

    if (i==TOTAL_IMAGE_TYPE)
        return -2;

    *imageType = (VCARD_IMAGE_CONSTANT)i;

    return 0;
}

signed int IdentifyAdrType(char *inFile, VCARD_ADR_TYPE *adrType)
{
    char *headPtr;
    char upperCase[MAX_CHAR];
    char *semicolon;
    char *upperCasePtr=upperCase;

    semicolon = strstr(inFile, SEMICOLON);
    if (!semicolon)
        return -1;
    else if (semicolon == inFile)
        return 1;	// there is no parameters

    memcpy(upperCase, inFile, (semicolon-inFile)+1);
    upperCase[(semicolon-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    semicolon = strstr(upperCasePtr, SEMICOLON);

    // Property parameters starts from here
    headPtr = upperCasePtr+1;

    if (strstr(upperCasePtr, AdrType[0]))
        adrType->domType = 1;
    if (strstr(upperCasePtr, AdrType[1]))
        adrType->intlType = 1;
    if (strstr(upperCasePtr, AdrType[2]))
        adrType->postalType = 1;
    if (strstr(upperCasePtr, AdrType[3]))
        adrType->parcelType = 1;
    if (strstr(upperCasePtr, AdrType[4]))
        adrType->homeType = 1;
    if (strstr(upperCasePtr, AdrType[5]))
        adrType->workType = 1;

    return 1;
}

signed int IdentifyTelType(char *inFile, VCARD_TEL_TYPE *telType)
{
    char *headPtr;
    char upperCase[MAX_CHAR];
    char *semicolon;
    char *upperCasePtr=upperCase;

    semicolon = strstr(inFile, SEMICOLON);
    if (!semicolon)
        return -1;
    else if (semicolon == inFile)
        return 1;	// there is no parameters

    memcpy(upperCase, inFile, (semicolon-inFile)+1);
    upperCase[(semicolon-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    semicolon = strstr(upperCasePtr, SEMICOLON);

    // Property parameters starts from here
    headPtr = upperCasePtr+1;

    if (strstr(upperCasePtr, TelType[0]))
        telType->prefType = 1;
    if (strstr(upperCasePtr, TelType[1]))
        telType->workType = 1;
    if (strstr(upperCasePtr, TelType[2]))
        telType->homeType = 1;
    if (strstr(upperCasePtr, TelType[3]))
        telType->voiceType = 1;
    if (strstr(upperCasePtr, TelType[4]))
        telType->faxType = 1;
    if (strstr(upperCasePtr, TelType[5]))
        telType->msgType = 1;
    if (strstr(upperCasePtr, TelType[6]))
        telType->cellType = 1;
    if (strstr(upperCasePtr, TelType[7]))
        telType->pagerType = 1;
    if (strstr(upperCasePtr, TelType[8]))
        telType->bbsType = 1;
    if (strstr(upperCasePtr, TelType[9]))
        telType->modemType = 1;
    if (strstr(upperCasePtr, TelType[10]))
        telType->carType = 1;
    if (strstr(upperCasePtr, TelType[11]))
        telType->isdnType = 1;
    if (strstr(upperCasePtr, TelType[12]))
        telType->videoType = 1;

    return 1;
}

signed int IdentifyEmailType(char *inFile, VCARD_EMAIL_CONSTANT *emailType)
{
    char *foundStr;
    char *headPtr;
    unsigned int i, searchLen;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
    foundStr = strstr(upperCasePtr, TYPE);

    if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
    {
        // may be it is not with TYPE=
        // check if
        for (i=EMAIL_AOL; i<TOTAL_EMAIL; i++)
        {
            foundStr = strstr(upperCasePtr, EmailType[i]);
            if (foundStr)
                break;
        }

        if (i==TOTAL_EMAIL)
            return -2;

        *emailType = (VCARD_EMAIL_CONSTANT)i;

        return 0;
    }

    searchLen = delimiters - foundStr - (unsigned long)strlen(TYPE);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(TYPE);

    for (i=0; i<=searchLen; i++)
    {
        if (*(upperCasePtr+i) == ';')
            break;
        else if (*(upperCasePtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -3;
    }

    for (i=EMAIL_AOL; i<TOTAL_EMAIL; i++)
    {
        foundStr = strstr(upperCasePtr, EmailType[i]);
        if (foundStr)
            break;
    }

    if (i==TOTAL_EMAIL)
        return -2;

    *emailType = (VCARD_EMAIL_CONSTANT)i;

    return 0;

}

signed int IdentifyAdrValue(char *inFile, VCARD_ADR_PROPERTY_TYPE *adr)
{
    char *headPtr;
    char *lfcr;
    char *semicolon;
    char *delimiters;
    char eol;
    signed int valueLen;
    char i;

    semicolon = strstr(inFile, SEMICOLON);
    if (!semicolon)
        return -1;

    lfcr = strstr(inFile, LFCR);

    // Property value starts from here
    headPtr = semicolon+1;

    i = 0;

    while (i<7)
    {
        // there are total 7 fields
        delimiters = IdentifyDelimiters(headPtr, &eol);

        if (!delimiters)
            return 1;

        // first field
        valueLen = delimiters - headPtr;
        if (valueLen > 0)
        {
            adr->adrField[i] = (char *)SCI_ALLOC(valueLen+1);
            if (!adr->adrField[i])
                return -2;

            memcpy(adr->adrField[i], headPtr, valueLen);
            adr->adrField[i][valueLen] = '\0';
        }

        if (eol)
            return 1;

        i++;
        headPtr = delimiters + 1;
    }

    return 1;
}

signed int IdentifyLabelValue(char *inFile, VCARD_LABEL_PROPERTY_TYPE *label)
{
    char *headPtr;
    char *lfcr;
    char *semicolon;
    char *backSlash;
    signed int valueLen=0;

    semicolon = strstr(inFile, SEMICOLON);
    if (!semicolon)
        return -1;

    backSlash = strstr(inFile, "\\");
    lfcr = strstr(inFile, LFCR);

    // Property value starts from here
    headPtr = semicolon+1;

    if ((backSlash) && (backSlash<lfcr) )	/*lint !e613*/// terminated by backSlash
        valueLen = backSlash - headPtr;
    else if ((backSlash) && (backSlash>lfcr))/*lint !e613*/
        valueLen = lfcr - headPtr;/*lint !e613*/
    else if (!backSlash)
        valueLen = lfcr - headPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        label->label = (char *)SCI_ALLOC(valueLen+1);
        if (!label->label)
            return -2;
        memcpy(label->label, headPtr, valueLen);
        label->label[valueLen] = '\0';
    }

    return 1;
}

signed int IdentifyTelValue(char *inFile, VCARD_TEL_PROPERTY_TYPE *tel)
{
    char *headPtr;
    char *lfcr;
    char *semicolon;
    signed int valueLen=0;
    char *backSlash;

    semicolon = strstr(inFile, SEMICOLON);
    if (!semicolon)
        return -1;

    backSlash = strstr(inFile, "\\");
    lfcr = strstr(inFile, LFCR);

    // Property value starts from here
    headPtr = semicolon+1;

    if ((backSlash) && (backSlash<lfcr) )	/*lint !e613*/// terminated by backSlash
        valueLen = backSlash - headPtr;/*lint !e613*/
    else if ((backSlash) && (backSlash>lfcr))/*lint !e613*/
        valueLen = lfcr - headPtr;/*lint !e613*/
    else if (!backSlash)
        valueLen = lfcr - headPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        tel->tel = (char *)SCI_ALLOC(valueLen+1);
        if (!tel->tel)
            return -2;
        memcpy(tel->tel, headPtr, valueLen);
        tel->tel[valueLen] = '\0';
    }

    return 1;
}

signed int IdentifyEmailValue(char *inFile, VCARD_EMAIL_PROPERTY_TYPE *email)
{
    char *headPtr;
    char *lfcr;
    char *semicolon;
    signed int valueLen=0;
    char *backSlash;

    semicolon = strstr(inFile, SEMICOLON);
    if (!semicolon)
        return -1;

    backSlash = strstr(inFile, "\\");
    lfcr = strstr(inFile, LFCR);

    // Property value starts from here
    headPtr = semicolon+1;

    if ((backSlash) && (backSlash<lfcr) )	/*lint !e613*/// terminated by backSlash
        valueLen = backSlash - headPtr;
    else if ((backSlash) && (backSlash>lfcr))/*lint !e613*/
        valueLen = lfcr - headPtr;/*lint !e613*/
    else if (!backSlash)
        valueLen = lfcr - headPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        email->email = (char *)SCI_ALLOC(valueLen+1);
        if (!email->email)
            return -2;
        memcpy(email->email, headPtr, valueLen);
        email->email[valueLen] = '\0';
    }

    return 1;
}

signed int IdentifySoundType(char *inFile, VCARD_SOUND_CONSTANT *soundType)
{
    char *foundStr;
    char *headPtr;
    unsigned int i, searchLen;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
    foundStr = strstr(upperCasePtr, TYPE);

    if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
    {
        // may be it is not with TYPE=
        // check if
        for (i=SOUND_WAVE; i<TOTAL_SOUND; i++)
        {
            foundStr = strstr(upperCasePtr, SoundType[i]);
            if (foundStr)
                break;
        }

        if (i==TOTAL_SOUND)
            return -2;

        *soundType = (VCARD_SOUND_CONSTANT)i;

        return 0;
    }

    searchLen = delimiters - foundStr - (unsigned long)strlen(TYPE);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(TYPE);

    for (i=0; i<=searchLen; i++)
    {
        if (*(upperCasePtr+i) == ';')
            break;
        else if (*(upperCasePtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -3;
    }

    for (i=SOUND_WAVE; i<TOTAL_SOUND; i++)
    {
        foundStr = strstr(upperCasePtr, SoundType[i]);
        if (foundStr)
            break;
    }

    if (i==TOTAL_SOUND)
        return -2;

    *soundType = (VCARD_SOUND_CONSTANT)i;

    return 0;
}

signed int IdentifyKeyType(char *inFile, VCARD_KEY_CONSTANT *keyType)
{
    char *foundStr;
    char *headPtr;
    unsigned int i, searchLen;
    char upperCase[MAX_CHAR];
    char *delimiters;
    char *upperCasePtr=upperCase;

    delimiters = strstr(inFile, SEMICOLON);
    if (!delimiters)
        return -1;

    memcpy(upperCase, inFile, (delimiters-inFile)+1);
    upperCase[(delimiters-inFile)+1] = '\0';
    upperCasePtr = StrUpr(upperCase);

    delimiters = strstr(upperCasePtr, SEMICOLON);
    foundStr = strstr(upperCasePtr, TYPE);

    if ( (!foundStr) || (delimiters < foundStr) )/*lint !e613*/
    {
        // may be it is not with TYPE=
        // check if
        for (i=KEY_X509; i<TOTAL_KEY; i++)
        {
            foundStr = strstr(upperCasePtr, KeyType[i]);
            if (foundStr)
                break;
        }

        if (i==TOTAL_KEY)
            return -2;

        *keyType = (VCARD_KEY_CONSTANT)i;

        return 0;
    }

    searchLen = delimiters - foundStr - (unsigned long)strlen(TYPE);/*lint !e737 !e613*/
    headPtr = foundStr+strlen(TYPE);

    for (i=0; i<=searchLen; i++)
    {
        if (*(upperCasePtr+i) == ';')
            break;
        else if (*(upperCasePtr+i) == ':')
            break;
    }

    if (i > searchLen)
    {
        // not found
        return -3;
    }

    for (i=KEY_X509; i<TOTAL_KEY; i++)
    {
        foundStr = strstr(upperCasePtr, KeyType[i]);
        if (foundStr)
            break;
    }

    if (i==TOTAL_KEY)
        return -2;

    *keyType = (VCARD_KEY_CONSTANT)i;

    return 0;
}

signed int ExtractVersion(const char *inFile, IRMC_VERSION_PORPERTY_TYPE *irmcVer)
{
    char *lfcr;
    char *headPtr;
    char *tailPtr;
    signed int valueLen;
    char foundSemicolon;
    char *propertyPtr;

    if ((!inFile) || (!irmcVer))
        return -1;
    headPtr = strstr(inFile, VERSION_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(VERSION_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &irmcVer->property.encoding);
    IdentifyCharSet(headPtr, &irmcVer->property.charSet);
    IdentifyLanguage(headPtr, &irmcVer->property.language);
    IdentifyValueLocation(headPtr, &irmcVer->property.valueLocation);

    lfcr = strstr(headPtr, LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        irmcVer->verNo = (char *)SCI_ALLOC(valueLen+1);
        if (!irmcVer->verNo)
        {
            //syncml_vcard_parse_message:"SCI_ALLOC FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1312_112_2_18_2_47_27_316,(uint8*)"d", (valueLen+1));
            return -2;
        }

        memcpy(irmcVer->verNo, propertyPtr, valueLen);
        irmcVer->verNo[valueLen] = '\0';

        return 1;
    }

    return -1;
}

signed int 	IdentifyPropertyValue(char *headPtr, char *tailPtr, char **propertyValue)
{
    char *backSlash;
    char *lfcr;
    signed int	valueLen=0;
    char *lfcrlwsp;
    char *tempPtr;
    signed int index=0;

    backSlash = strstr(headPtr, "\\");
    lfcr = strstr(headPtr, LFCR);
    lfcrlwsp = strstr(headPtr, LFCRLWSP);

    if (lfcr == lfcrlwsp)
    {
        // search for the size of the property value
        tempPtr = tailPtr;
        while (1)/*lint !e716*/
        {
            lfcr = strstr(tempPtr, "\r\n");
            if (!lfcr)
            {
                tempPtr = tailPtr;	// error
                break;
            }
            else if (*(lfcr+2) != ' ')	// end of property value
            {
                tempPtr = lfcr;
                break;
            }

            tempPtr = lfcr+2;
        }

        if ((tempPtr-tailPtr) <= 0)
            return -4;

        valueLen = tempPtr - tailPtr;
        *propertyValue = (char *)SCI_ALLOC(valueLen+1);
        if (!(*propertyValue))
            return -5;

        headPtr = tailPtr + 1;
        lfcr = strstr(headPtr, LFCR);
        index = 0;

        valueLen = lfcr - headPtr;/*lint !e613*/

        if (lfcr != headPtr)
        {
            memcpy(*propertyValue, headPtr, valueLen);
            index += valueLen;

        }

        while (*(lfcr+2) == ' ')/*lint !e613*/
        {
            tempPtr = lfcr+2;/*lint !e613*/
            while (*(tempPtr++) == ' ')
                ;

            tempPtr--;
            lfcr = strstr(tempPtr, LFCR);

            memcpy((*propertyValue)+index, tempPtr, (lfcr-tempPtr));/*lint !e613*/

            index += (lfcr-tempPtr);/*lint !e613*/
        }

        (*propertyValue)[index] = '\0';

        return (index);
    }

    // Property value starts from here
    headPtr = tailPtr+1;

#if 0//del by wangxiaolin  2009.11.3  \之后的字符都被截取饿了，当前我们不用\对；进行转义
    if ((backSlash) && (backSlash<lfcr) )	// terminated by backSlash
        valueLen = backSlash - headPtr;
    else if ((backSlash) && (backSlash>lfcr))
        valueLen = lfcr - headPtr;
    else if (!backSlash)
#endif
        valueLen = lfcr - headPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        *propertyValue = (char *)SCI_ALLOC(valueLen+1);
        if (!(*propertyValue))
            return -2;
        memcpy(*propertyValue, headPtr, valueLen);
        (*propertyValue)[valueLen] = '\0';
    }

    return (valueLen);
}

signed int ExtractNameFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *lfcr;
    char *headPtr;
    char *tailPtr;
    signed int valueLen;
    char eol;
    char foundSemicolon;
   
    char i, j,k=0;
 
    //int dataLength =0;
	
    uint8 utf8_ptr[MAX_CHAR] = {0};
    uint8 ucs2_ptr[2*MAX_CHAR] = {0};
    int utf8_len = 0;
    uint16 ucs2_len = 0;

    if ((!inFile) || (!vcard))
        return -1;
    //syncml_vcard_parse_message:"ExtractNameFrVcard "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1443_112_2_18_2_47_27_317,(uint8*)"");

    headPtr = strstr(inFile, NAME_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(NAME_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;


    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardName.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardName.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardName.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardName.property.valueLocation);
    //syncml_vcard_parse_message:"ExtractNameFrVcard 22"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1461_112_2_18_2_47_27_318,(uint8*)"");

    lfcr = strstr(headPtr, LFCR);
    if (!lfcr)
        return -4;

    i = 0;
    if((vcard->vCardName.property.encoding.quotedPrintable == 1)
	&&(strstr(vcard->vCardName.property.charSet, "UTF-8")))
    	{
    while (1)/*lint !e716*/
    {
    	if (i > 4) 
        {
            return 1;
        }
		
        headPtr = tailPtr+1;

        tailPtr = IdentifyDelimiters((char *)headPtr, &eol);
        if (!tailPtr)
            return -5;

        valueLen = tailPtr - headPtr;
        //syncml_vcard_parse_message:"ExtractNameFrVcard valueLen = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1485_112_2_18_2_47_27_319,(uint8*)"d",valueLen);
        
        //重新置0，否则会将前一个字段的值带入后面字段
        hs_smlLibMemset(utf8_ptr, 0, MAX_CHAR);
        hs_smlLibMemset(ucs2_ptr, 0, 2*MAX_CHAR);

/*Start of  wangxiaolin 2009.7.17 for 对数据进行保护，防止内存覆盖 */
        valueLen=MIN(MAX_CHAR,valueLen);
/*End   of  wangxiaolin 2009.7.17 for 对数据进行保护，防止内存覆盖 */

       
        if (valueLen>0)
        {
	     //uint8 k=0;
            //qp->utf-8
            utf8_len = DecodeQuoted(headPtr, utf8_ptr,  valueLen);
			//syncml_vcard_parse_message:"utf8_len=%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1499_112_2_18_2_47_27_320,(uint8*)"d",utf8_len);
			is_CHN = FALSE;
            for(j = 0;j<utf8_len;j++)
            {
              // syncml_vcard_parse_message((" utf8_ptr[%d] = %x", j,utf8_ptr[j]));
    			if(0x80 <= utf8_ptr[j])
    				is_CHN = TRUE;
        	}

			//syncml_vcard_parse_message:"ExtractNameFrVcard headPtr  is_CHN =%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1508_112_2_18_2_47_27_321,(uint8*)"d",is_CHN);
            if(is_CHN)
            {
                vcard->vCardName.nameField[i].is_ucs2 = TRUE; // chenxiang_vcard 20071207
	            //utf8->ucs2
	            VmByteToUnicode((const char*)utf8_ptr, utf8_len, (uint16*)ucs2_ptr, sizeof(ucs2_ptr));
#ifdef WIN32  //add by wangxiaolin 2007.12.05 解决大小端
	            for(j = 0;j<pim_unicode_strlen((unsigned short *)ucs2_ptr);j++){
					k = ucs2_ptr[2*j];
					ucs2_ptr[2*j] = ucs2_ptr[2*j+1];
					ucs2_ptr[2*j+1] = k;
				}
#endif
	            ucs2_len =(uint16)( 2 * pim_unicode_strlen((unsigned short *)ucs2_ptr));
     
                  //g_vcard_len=ucs2_len;
	            //syncml_vcard_parse_message:"que_ucs2_len=%d"
	            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1524_112_2_18_2_47_27_322,(uint8*)"d",ucs2_len);
		      for(k=0;k<ucs2_len;k++)
			{
                    //syncml_vcard_parse_message(("QUE_ucs2_ptr[%d]=%x",k,ucs2_ptr[k]));
		      }
	
	            //chenxiang_vcard 20071207
	            vcard->vCardName.nameField[i].str_ptr = (uint8 *)SCI_ALLOC(ucs2_len+2);
                
	            if (!vcard->vCardName.nameField[i].str_ptr)
                {
                    return -6;
                }

                   hs_smlLibMemset(vcard->vCardName.nameField[i].str_ptr, 0, ucs2_len+2);
                   
	            hs_smlLibMemcpy(vcard->vCardName.nameField[i].str_ptr, (char *)ucs2_ptr, ucs2_len);
	            vcard->vCardName.nameField[i].str_ptr[ucs2_len] = '\0';
	            vcard->vCardName.nameField[i].str_ptr[ucs2_len + 1] = '\0';
                vcard->vCardName.nameField[i].str_len = ucs2_len;
			}
			else
           {
                vcard->vCardName.nameField[i].is_ucs2 = FALSE;
	            vcard->vCardName.nameField[i].str_ptr = (uint8 *)SCI_ALLOC(utf8_len+1);
	            if (!vcard->vCardName.nameField[i].str_ptr)
                {
                    return -6;
                }
                
                   hs_smlLibMemset(vcard->vCardName.nameField[i].str_ptr, 0, utf8_len+1);
                   
	            hs_smlLibMemcpy(vcard->vCardName.nameField[i].str_ptr, (char *)utf8_ptr, utf8_len);
	            vcard->vCardName.nameField[i].str_ptr[utf8_len] = '\0';
                vcard->vCardName.nameField[i].str_len = (uint16)utf8_len;
			}
        }

        if (eol)
            return 1;

        i++;
    }
}
else // chenxiang_vcard 20071207
{
	is_CHN = FALSE;
    j = 0;
    while (!is_CHN && headPtr[j] != '\r')
    {
		if(0x80 <= (uint8)headPtr[j])
        {
        	is_CHN = TRUE;
        }
        j++;
	}

    //g_vcard_len = 0;
    while (1)/*lint !e716*/
    {
    	if (i > 4) 
        {
            return 1;
        }
        headPtr = tailPtr+1;

        tailPtr = IdentifyDelimiters((char *)headPtr, &eol);
        if (!tailPtr)
            return -5;

        valueLen = tailPtr - headPtr;
        //syncml_vcard_parse_message:"ExtractNameFrVcard valueLen = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_1595_112_2_18_2_47_27_323,(uint8*)"d",valueLen);
        
        if (valueLen>0)
        {
            if (is_CHN) //有汉字
            {
                uint16  unicode_char;
                vcard->vCardName.nameField[i].is_ucs2 = TRUE;
                vcard->vCardName.nameField[i].str_ptr = (uint8 *)SCI_ALLOC((valueLen << 1) + 2);
                if (!vcard->vCardName.nameField[i].str_ptr)
                    return -6;
                j = 0;
                k = 0;
                while ( j < valueLen)
                {
                    if ((uint8)headPtr[j] > 0x80) // 汉字
                    {
                        GUI_GBToWstr(&unicode_char, (uint8 *)&headPtr[j], 2);
#ifdef WIN32
                        vcard->vCardName.nameField[i].str_ptr[k] = (char)(unicode_char & 0x00ff );
                        vcard->vCardName.nameField[i].str_ptr[k + 1] = (char)((unicode_char & 0xff00) >> 8);
#else
                        vcard->vCardName.nameField[i].str_ptr[k] = (char)((unicode_char >> 8) & 0x00ff );
                        vcard->vCardName.nameField[i].str_ptr[k + 1] = (char)(unicode_char & 0x00ff);
#endif
                        j += 2;
                        k += 2;
                    }
                    else // 英文
                    {
#ifdef WIN32
                        vcard->vCardName.nameField[i].str_ptr[k] = headPtr[j];
                        vcard->vCardName.nameField[i].str_ptr[k + 1] = 0;
#else
                        vcard->vCardName.nameField[i].str_ptr[k] = 0;
                        vcard->vCardName.nameField[i].str_ptr[k + 1] = headPtr[j];
#endif
            	        j++;
                        k += 2;
                    }
                }
                vcard->vCardName.nameField[i].str_ptr[k] = '\0';
                vcard->vCardName.nameField[i].str_ptr[k + 1] = '\0';
                vcard->vCardName.nameField[i].str_len = k;
               // g_vcard_len += k;
            }
            else //全是英文字符
            {
                vcard->vCardName.nameField[i].is_ucs2 = FALSE;
                vcard->vCardName.nameField[i].str_ptr = (uint8 *)SCI_ALLOC(valueLen + 1);
                if (!vcard->vCardName.nameField[i].str_ptr)
                    return -6;
                
	            hs_smlLibMemcpy(vcard->vCardName.nameField[i].str_ptr, (char *)headPtr, valueLen);
	            vcard->vCardName.nameField[i].str_ptr[valueLen] = '\0';
                vcard->vCardName.nameField[i].str_len =(uint16)valueLen;
            }
        }

        if (eol)
            return 1;

        i++;
    }
	syncml_vcard_parse_message(("ExtractNameFrVcard not utf8 or qp  "));/*lint !e527*/
}

    return 1;
}

signed int ExtractFNFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    headPtr = strstr(inFile, FN_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(FN_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardFn.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardFn.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardFn.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardFn.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardFn.fn);

    return 1;
}

signed int ExtractPhotoFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    headPtr = strstr(inFile, PHOTO_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(PHOTO_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardPhoto.property.encoding);
    IdentifyValueLocation(headPtr, &vcard->vCardPhoto.property.valueLocation);
    IdentifyImageType(headPtr, &vcard->vCardPhoto.formatType);

    vcard->vCardPhoto.photoLen = IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardPhoto.photo);

    return 1;
}

signed int ExtractBirthDateFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    headPtr = strstr(inFile, BDAY_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(BDAY_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardBDate.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardBDate.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardBDate.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardBDate.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardBDate.birthDate);

    return 1;
}

signed int ExtractAdrFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;
    char *propertyPtr;
    signed int adrCount=0;
    VCARD_ADR_PROPERTY_TYPE *adrQueue;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    while (1)/*lint !e716*/
    {
        headPtr = strstr(headPtr, ADR_PROPERTY);
        if (!headPtr)
            break;

        adrCount++;
        headPtr += strlen(ADR_PROPERTY);
    }

    if (!adrCount)
        return 1;

    headPtr = (char *)inFile;

    while (adrCount)
    {
        adrQueue = (VCARD_ADR_PROPERTY_TYPE *)SCI_ALLOC(sizeof(VCARD_ADR_PROPERTY_TYPE));
        if (!adrQueue)
        {
            // remove the previous list
            return -3;
        }

        hs_smlLibMemset(adrQueue, 0, sizeof(VCARD_ADR_PROPERTY_TYPE));

        headPtr = strstr(headPtr, ADR_PROPERTY);

        if (!headPtr)
        {
            SCI_FREE(adrQueue);
            return -2;
        }

        headPtr += strlen(ADR_PROPERTY);
        tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

        if (!( (tailPtr) && (foundSemicolon) ))
        {
            SCI_FREE(adrQueue);
            return -4;
        }

        // Property Value starts from here
        propertyPtr = tailPtr+1;

        // check if the Encodings exist
        IdentifyEncoding(headPtr, &adrQueue->property.encoding);
        IdentifyCharSet(headPtr, &adrQueue->property.charSet);
        IdentifyLanguage(headPtr, &adrQueue->property.language);
        IdentifyValueLocation(headPtr, &adrQueue->property.valueLocation);

        IdentifyAdrType(headPtr, &adrQueue->adrType);

        IdentifyAdrValue(headPtr, adrQueue);

        adrCount--;

        LstAdd(&vcard->vCardAdr, (PNodeType)adrQueue);

        headPtr = propertyPtr;
    }

    return 1;
}

signed int ExtractLabelFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;
    char *propertyPtr;
    signed int labelCount=0;
    VCARD_LABEL_PROPERTY_TYPE *labelQueue;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    while (1)/*lint !e716*/
    {
        headPtr = strstr(headPtr, LABEL_PROPERTY);
        if (!headPtr)
            break;

        labelCount++;
        headPtr += strlen(LABEL_PROPERTY);
    }

    if (!labelCount)
        return 1;

    headPtr = (char *)inFile;

    while (labelCount)
    {
        labelQueue = (VCARD_LABEL_PROPERTY_TYPE *)SCI_ALLOC(sizeof(VCARD_LABEL_PROPERTY_TYPE));
        if (!labelQueue)
        {
            // remove the previous list
            return -3;
        }

        hs_smlLibMemset(labelQueue, 0, sizeof(VCARD_LABEL_PROPERTY_TYPE));

        headPtr = strstr(headPtr, LABEL_PROPERTY);
        if (!headPtr)
        {
            SCI_FREE(labelQueue);
            return -2;
        }

        headPtr += strlen(LABEL_PROPERTY);
        tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

        if (!( (tailPtr) && (foundSemicolon) ))
        {
            SCI_FREE(labelQueue);
            return -4;
        }

        // Property Value starts from here
        propertyPtr = tailPtr+1;

        // check if the Encodings exist
        IdentifyEncoding(headPtr, &labelQueue->property.encoding);
        IdentifyCharSet(headPtr, &labelQueue->property.charSet);
        IdentifyLanguage(headPtr, &labelQueue->property.language);
        IdentifyValueLocation(headPtr, &labelQueue->property.valueLocation);

        IdentifyAdrType(headPtr, &labelQueue->labelType);

        IdentifyPropertyValue(headPtr, tailPtr, &labelQueue->label);

        labelCount--;

        LstAdd(&vcard->vCardLabel, (PNodeType)labelQueue);/*lint !e26*/

        headPtr = propertyPtr;
    }

    return 1;
}

signed int ExtractTelFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;
    char *propertyPtr;
    signed int telCount=0;
    VCARD_TEL_PROPERTY_TYPE *telQueue;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    while (1)/*lint !e716*/
    {
        headPtr = strstr(headPtr, TEL_PROPERTY);
        if (!headPtr)
            break;

        telCount++;
        headPtr += strlen(TEL_PROPERTY);
    }

    if (!telCount)
        return 1;

    headPtr = (char *)inFile;

    while (telCount)
    {
        telQueue = (VCARD_TEL_PROPERTY_TYPE *)SCI_ALLOC(sizeof(VCARD_TEL_PROPERTY_TYPE));
        if (!telQueue)
        {
            // remove the previous list
            return -3;
        }

        hs_smlLibMemset(telQueue, 0, sizeof(VCARD_TEL_PROPERTY_TYPE));

        headPtr = strstr(headPtr, TEL_PROPERTY);
        if (!headPtr)
        {
            SCI_FREE(telQueue);
            return -2;
        }

        headPtr += strlen(TEL_PROPERTY);
        tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

        if (!( (tailPtr) && (foundSemicolon) ))
        {
            SCI_FREE(telQueue);
            return -4;
        }

        // Property Value starts from here
        propertyPtr = tailPtr+1;

        // check if the Encodings exist
        IdentifyEncoding(headPtr, &telQueue->property.encoding);
        IdentifyCharSet(headPtr, &telQueue->property.charSet);
        IdentifyLanguage(headPtr, &telQueue->property.language);
        IdentifyValueLocation(headPtr, &telQueue->property.valueLocation);

        IdentifyTelType(headPtr, &telQueue->telType);

        IdentifyPropertyValue(headPtr, tailPtr, &telQueue->tel);

        telCount--;

        LstAdd(&vcard->vCardTel, (PNodeType)telQueue);/*lint !e26*/

        headPtr = propertyPtr;
    }

    return 1;
}

signed int ExtractEmailFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;
    char *propertyPtr;
    signed int emailCount=0;
    VCARD_EMAIL_PROPERTY_TYPE *emailQueue;

    
    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    while (1)/*lint !e716*/
    {
        headPtr = strstr(headPtr, EMAIL_PROPERTY);
        if (!headPtr)
            break;

        emailCount++;
        headPtr += strlen(EMAIL_PROPERTY);
    }

    if (!emailCount)
        return 1;

    headPtr = (char *)inFile;

    while (emailCount)
    {
        emailQueue = (VCARD_EMAIL_PROPERTY_TYPE *)SCI_ALLOC(sizeof(VCARD_EMAIL_PROPERTY_TYPE));
        if (!emailQueue)
        {
            // remove the previous list
            return -3;
        }

        hs_smlLibMemset(emailQueue, 0, sizeof(VCARD_EMAIL_PROPERTY_TYPE));

        headPtr = strstr(headPtr, EMAIL_PROPERTY);
        if (!headPtr)
        {
            SCI_FREE(emailQueue);
            return -2;
        }

        headPtr += strlen(EMAIL_PROPERTY);
        tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

        if (!( (tailPtr) && (foundSemicolon) ))
        {
            SCI_FREE(emailQueue);
            return -4;
        }

        // Property Value starts from here
        propertyPtr = tailPtr+1;

        // check if the Encodings exist
        IdentifyEncoding(headPtr, &emailQueue->property.encoding);
        IdentifyCharSet(headPtr, &emailQueue->property.charSet);
        IdentifyLanguage(headPtr, &emailQueue->property.language);
        IdentifyValueLocation(headPtr, &emailQueue->property.valueLocation);

        IdentifyEmailType(headPtr, &emailQueue->formatType);

        IdentifyPropertyValue(headPtr, tailPtr, &emailQueue->email);

        emailCount--;

        LstAdd(&vcard->vCardEmail, (PNodeType)emailQueue);/*lint !e26*/

        headPtr = propertyPtr;
    }

    return 1;
}

signed int ExtractMailerFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, MAILER_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(MAILER_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardMailer.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardMailer.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardMailer.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardMailer.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardMailer.mailer);

    return 1;
}

signed int ExtractTimezone(const char *inFile, IRMC_TIME_ZONE_PROPERTY_TYPE *timezone)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!timezone))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, TIMEZONE_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(TIMEZONE_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &timezone->property.encoding);
    IdentifyCharSet(headPtr, &timezone->property.charSet);
    IdentifyLanguage(headPtr, &timezone->property.language);
    IdentifyValueLocation(headPtr, &timezone->property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &timezone->timeZone);

    return 1;
}

signed int ExtractGeographic(const char *inFile, IRMC_GEOGRAPHIC_PROPERTY_TYPE *geographic)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!geographic))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, GEOGRAPHIC_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(GEOGRAPHIC_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &geographic->property.encoding);
    IdentifyCharSet(headPtr, &geographic->property.charSet);
    IdentifyLanguage(headPtr, &geographic->property.language);
    IdentifyValueLocation(headPtr, &geographic->property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &geographic->geographic);

    return 1;
}

signed int ExtractTitleFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, TITLE_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(TITLE_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardTitle.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardTitle.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardTitle.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardTitle.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardTitle.title);

    return 1;
}

signed int ExtractRoleFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, ROLE_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(ROLE_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardRole.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardRole.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardRole.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardRole.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardRole.role);

    return 1;
}

signed int ExtractLogoFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    headPtr = strstr(inFile, LOGO_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(LOGO_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardLogo.property.encoding);
    IdentifyValueLocation(headPtr, &vcard->vCardLogo.property.valueLocation);
    IdentifyImageType(headPtr, &vcard->vCardLogo.formatType);

    vcard->vCardLogo.logoLen = IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardLogo.logo);

    return 1;
}

signed int ExtractOrgFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;
    char *lfcr;
    signed int	valueLen=0;
    char i;
    char eol;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, ORG_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(ORG_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardOrg.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardOrg.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardOrg.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardOrg.property.valueLocation);

    lfcr = strstr(headPtr, LFCR);
    if (!lfcr)
        return -4;

    i = 0;
    while (1)/*lint !e716*/
    {
        headPtr = tailPtr+1;

        tailPtr = IdentifyDelimiters((char *)headPtr, &eol);
        if (!tailPtr)
            return -5;

        valueLen = tailPtr - headPtr;
        if (valueLen>0)
        {
            vcard->vCardOrg.org[i] = (char *)SCI_ALLOC(valueLen+1);
            if (!vcard->vCardOrg.org[i])
                return -6;

            memcpy(vcard->vCardOrg.org[i], headPtr, valueLen);
            vcard->vCardOrg.org[i][valueLen] = '\0';
        }

        if (eol)
            return 1;

        i++;
    }

    //return 1;  //del by wangxiaolin 2009.12.20 for del warning
}

signed int ExtractNoteFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, NOTE_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(NOTE_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardNote.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardNote.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardNote.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardNote.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardNote.note);

    return 1;
}

signed int ExtractLastRevFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, LAST_REV_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(LAST_REV_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardLastRev.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardLastRev.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardLastRev.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardLastRev.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardLastRev.lastRev);

    return 1;
}

signed int ExtractSoundFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    headPtr = strstr(inFile, SOUND_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(SOUND_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardSound.property.encoding);
    IdentifyValueLocation(headPtr, &vcard->vCardSound.property.valueLocation);
    IdentifySoundType(headPtr, &vcard->vCardSound.formatType);

    vcard->vCardSound.soundLen = IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardSound.sound);

    return 1;
}

signed int ExtractUrlFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, URL_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(URL_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardUrl.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardUrl.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardUrl.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardUrl.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardUrl.url);

    return 1;
}

signed int ExtractUidFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    // check how many Adr in the vcard
    headPtr = (char *)inFile;

    headPtr = strstr(headPtr, UID_PROPERTY);
    if (!headPtr)
        return -2;

    headPtr += strlen(UID_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -4;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardUid.property.encoding);
    IdentifyCharSet(headPtr, &vcard->vCardUid.property.charSet);
    IdentifyLanguage(headPtr, &vcard->vCardUid.property.language);
    IdentifyValueLocation(headPtr, &vcard->vCardUid.property.valueLocation);

    IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardUid.uid);

    return 1;
}

signed int ExtractKeyFrVcard(const char *inFile, VCARD_TYPE *vcard)
{
    char *headPtr;
    char *tailPtr;
    char foundSemicolon;

    if ((!inFile) || (!vcard))
        return -1;

    headPtr = strstr(inFile, KEY_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return -2;

    headPtr += strlen(KEY_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return -3;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcard->vCardKey.property.encoding);
    IdentifyValueLocation(headPtr, &vcard->vCardKey.property.valueLocation);
    IdentifyKeyType(headPtr, &vcard->vCardKey.formatType);

    vcard->vCardKey.keyLen = IdentifyPropertyValue(headPtr, tailPtr, &vcard->vCardKey.key);

    return 1;
}


signed int ReleaseVcardMem(VCARD_TYPE *vcard)
{
    signed int i;
    VCARD_ADR_PROPERTY_TYPE *adrQueue;
    VCARD_LABEL_PROPERTY_TYPE *labelQueue;
    VCARD_TEL_PROPERTY_TYPE *telQueue;
    VCARD_EMAIL_PROPERTY_TYPE *emailQueue;

    // Vcard version
    if (vcard->vCardVer.property.charSet)
        SCI_FREE(vcard->vCardVer.property.charSet);
    if (vcard->vCardVer.property.language)
        SCI_FREE(vcard->vCardVer.property.charSet);
    if (vcard->vCardVer.verNo)
        SCI_FREE(vcard->vCardVer.verNo);

    // Name
    if (vcard->vCardName.property.charSet)
        SCI_FREE(vcard->vCardName.property.charSet);
    if (vcard->vCardName.property.language)
        SCI_FREE(vcard->vCardName.property.language);

    for (i=0; i<5; i++)
    {
        if (vcard->vCardName.nameField[i].str_ptr) // chenxiang_vcard 20071207
            SCI_FREE(vcard->vCardName.nameField[i].str_ptr); // chenxiang_vcard 20071207
    }

    // FN
    if (vcard->vCardFn.property.charSet)
        SCI_FREE(vcard->vCardFn.property.charSet);
    if (vcard->vCardFn.property.language)
        SCI_FREE(vcard->vCardFn.property.language);
    if (vcard->vCardFn.fn)
        SCI_FREE(vcard->vCardFn.fn);

    // Photo
    if (vcard->vCardPhoto.property.charSet)
        SCI_FREE(vcard->vCardPhoto.property.charSet);
    if (vcard->vCardPhoto.property.language)
        SCI_FREE(vcard->vCardPhoto.property.language);
    if (vcard->vCardPhoto.photo)
        SCI_FREE(vcard->vCardPhoto.photo);

    // BirthDay
    if (vcard->vCardBDate.property.charSet)
        SCI_FREE(vcard->vCardBDate.property.charSet);
    if (vcard->vCardBDate.property.language)
        SCI_FREE(vcard->vCardBDate.property.language);
    if (vcard->vCardBDate.birthDate)
        SCI_FREE(vcard->vCardBDate.birthDate);

    // ADR
    while (LstCount(&vcard->vCardAdr))
    {
        adrQueue = (VCARD_ADR_PROPERTY_TYPE *)LstGet(&vcard->vCardAdr);
        if (!adrQueue)
            break;

        if (adrQueue->property.charSet)
            SCI_FREE(adrQueue->property.charSet);
        if (adrQueue->property.language)
            SCI_FREE(adrQueue->property.language);
        for (i=0; i<7; i++)
        {
            if (adrQueue->adrField[i])
                SCI_FREE(adrQueue->adrField[i]);
        }
    }

    // LABEL
    while (LstCount(&vcard->vCardLabel))
    {
        labelQueue = (VCARD_LABEL_PROPERTY_TYPE *)LstGet(&vcard->vCardLabel);
        if (!labelQueue)
            break;

        if (labelQueue->property.charSet)
            SCI_FREE(labelQueue->property.charSet);
        if (labelQueue->property.language)
            SCI_FREE(labelQueue->property.language);

        if (labelQueue->label)
            SCI_FREE(labelQueue->label);
    }

    // TEL
    while (LstCount(&vcard->vCardTel))
    {
        telQueue = (VCARD_TEL_PROPERTY_TYPE *)LstGet(&vcard->vCardTel);
        if (!telQueue)
            break;

        if (telQueue->property.charSet)
            SCI_FREE(telQueue->property.charSet);
        if (telQueue->property.language)
            SCI_FREE(telQueue->property.language);

        if (telQueue->tel)
            SCI_FREE(telQueue->tel);
    }

    // EMAIL
    while (LstCount(&vcard->vCardEmail))
    {
        emailQueue = (VCARD_EMAIL_PROPERTY_TYPE *)LstGet(&vcard->vCardEmail);
        if (!emailQueue)
            break;

        if (emailQueue->property.charSet)
            SCI_FREE(emailQueue->property.charSet);
        if (emailQueue->property.language)
            SCI_FREE(emailQueue->property.language);

        if (emailQueue->email)
            SCI_FREE(emailQueue->email);
    }

    // MAILER
    if (vcard->vCardMailer.property.charSet)
        SCI_FREE(vcard->vCardMailer.property.charSet);
    if (vcard->vCardMailer.property.language)
        SCI_FREE(vcard->vCardMailer.property.language);
    if (vcard->vCardMailer.mailer)
        SCI_FREE(vcard->vCardMailer.mailer);

    // TIMEZOE
    if (vcard->vCardTimeZone.property.charSet)
        SCI_FREE(vcard->vCardTimeZone.property.charSet);
    if (vcard->vCardTimeZone.property.language)
        SCI_FREE(vcard->vCardTimeZone.property.language);
    if (vcard->vCardTimeZone.timeZone)
        SCI_FREE(vcard->vCardTimeZone.timeZone);

    // GEOGRAPHIC
    if (vcard->vCardGeographic.property.charSet)
        SCI_FREE(vcard->vCardGeographic.property.charSet);
    if (vcard->vCardGeographic.property.language)
        SCI_FREE(vcard->vCardGeographic.property.language);
    if (vcard->vCardGeographic.geographic)
        SCI_FREE(vcard->vCardGeographic.geographic);

    // TITLE
    if (vcard->vCardTitle.property.charSet)
        SCI_FREE(vcard->vCardTitle.property.charSet);
    if (vcard->vCardTitle.property.language)
        SCI_FREE(vcard->vCardTitle.property.language);
    if (vcard->vCardTitle.title)
        SCI_FREE(vcard->vCardTitle.title);

    // ROLE
    if (vcard->vCardRole.property.charSet)
        SCI_FREE(vcard->vCardRole.property.charSet);
    if (vcard->vCardRole.property.language)
        SCI_FREE(vcard->vCardRole.property.language);
    if (vcard->vCardRole.role)
        SCI_FREE(vcard->vCardRole.role);

    // LOGO
    if (vcard->vCardLogo.property.charSet)
        SCI_FREE(vcard->vCardLogo.property.charSet);
    if (vcard->vCardLogo.property.language)
        SCI_FREE(vcard->vCardLogo.property.language);
    if (vcard->vCardLogo.logo)
        SCI_FREE(vcard->vCardLogo.logo);

    // ORG
    if (vcard->vCardOrg.property.charSet)
        SCI_FREE(vcard->vCardOrg.property.charSet);
    if (vcard->vCardOrg.property.language)
        SCI_FREE(vcard->vCardOrg.property.language);
    for (i=0; i<3; i++)
    {
        if (vcard->vCardOrg.org[i])
            SCI_FREE(vcard->vCardOrg.org[i]);
    }

    // NOTE
    if (vcard->vCardNote.property.charSet)
        SCI_FREE(vcard->vCardNote.property.charSet);
    if (vcard->vCardNote.property.language)
        SCI_FREE(vcard->vCardNote.property.language);
    if (vcard->vCardNote.note)
        SCI_FREE(vcard->vCardNote.note);

    // SOUND
    if (vcard->vCardSound.property.charSet)
        SCI_FREE(vcard->vCardSound.property.charSet);
    if (vcard->vCardSound.property.language)
        SCI_FREE(vcard->vCardSound.property.language);
    if (vcard->vCardSound.sound)
        SCI_FREE(vcard->vCardSound.sound);

    // URL
    if (vcard->vCardUrl.property.charSet)
        SCI_FREE(vcard->vCardUrl.property.charSet);
    if (vcard->vCardUrl.property.language)
        SCI_FREE(vcard->vCardUrl.property.language);
    if (vcard->vCardUrl.url)
        SCI_FREE(vcard->vCardUrl.url);

    // UID
    if (vcard->vCardUid.property.charSet)
        SCI_FREE(vcard->vCardUid.property.charSet);
    if (vcard->vCardUid.property.language)
        SCI_FREE(vcard->vCardUid.property.language);
    if (vcard->vCardUid.uid)
        SCI_FREE(vcard->vCardUid.uid);

    // KEY
    if (vcard->vCardKey.property.charSet)
        SCI_FREE(vcard->vCardKey.property.charSet);
    if (vcard->vCardKey.property.language)
        SCI_FREE(vcard->vCardKey.property.language);
    if (vcard->vCardKey.key)
        SCI_FREE(vcard->vCardKey.key);

    return 1;
}


signed int VcardParser(char *dataPtr, VCARD_TYPE *vcard,MMIPB_CONTACT_T*pb_entry)
{
    uint16 i = 0;
/*Start of  wangxiaolin 2009.11.2 for add by chenxiang */
    uint16 j = 0, k = 0;
/*End   of  wangxiaolin 2009.11.2 for add by chenxiang */
    //VCARD_ADR_PROPERTY_TYPE *adrQueue;
   // VCARD_LABEL_PROPERTY_TYPE *labelQueue;
    VCARD_TEL_PROPERTY_TYPE *telQueue;
    VCARD_EMAIL_PROPERTY_TYPE *emailQueue;
    char msg[100];
    //int dataLength =0;
    MMI_PARTY_NUMBER_T  party_num;
    T_VCARD_NUMBER_TYPE numberType = VCARD_NUM_TYPE_MAX;
    //wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0}; 
    
    #ifdef HS_PIM_SUPPORT_ALL_FIELD  //Added by wangxiaolin  2009.7.9 for 电话簿其他字段
    uint32 number_type_flag=0;
    //uint32 email_type_index=0;
    #endif

    //syncml_vcard_parse_message:" enter VcardParser "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2765_112_2_18_2_47_30_324,(uint8*)"");
	hs_smlLibMemset(pb_entry->name.wstr, 0x00, sizeof(pb_entry->name.wstr));

    if (!FileIsVcard(dataPtr))
    {
        //syncml_vcard_parse_message:"File is not vCard\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2770_112_2_18_2_47_30_325,(uint8*)"");
        return -1;
    }

    hs_smlLibMemset(vcard, 0, sizeof(VCARD_TYPE));

    if (ExtractNameFrVcard((const char *)dataPtr, vcard) > 0)
    {

        if (vcard->vCardName.property.encoding.quotedPrintable)
            //syncml_vcard_parse_message:"tvCard Name [encoding=QUOTED-PRINTABLE]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2780_112_2_18_2_47_30_326,(uint8*)"");
        else if (vcard->vCardName.property.encoding.eightBit)
            //syncml_vcard_parse_message:"tvCard Name [encoding=8BIT]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2782_112_2_18_2_47_30_327,(uint8*)"");
        else if (vcard->vCardName.property.encoding.base64)
            //syncml_vcard_parse_message:"tvCard Name [encoding=BASE64]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2784_112_2_18_2_47_30_328,(uint8*)"");

        if (vcard->vCardName.property.charSet)
        {
            //syncml_vcard_parse_message:"tvCard Name [charset=%s]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2788_112_2_18_2_47_30_329,(uint8*)"s", vcard->vCardName.property.charSet);
        }

        if (vcard->vCardName.property.language)
        {
            //syncml_vcard_parse_message:"\tvCard Name [language=%s]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2793_112_2_18_2_47_30_330,(uint8*)"s", vcard->vCardName.property.language);
        }

        if (vcard->vCardName.nameField[0].str_ptr)	// family name
        {
            //syncml_vcard_parse_message:"field 0, is_ucs2=%d,Name =%s,len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2798_112_2_18_2_47_30_331,(uint8*)"dsd",vcard->vCardName.nameField[0].is_ucs2, vcard->vCardName.nameField[0].str_ptr,vcard->vCardName.nameField[0].str_len);

            Str2Wstr(&(pb_entry->name.wstr_len),
                  pb_entry->name.wstr,
                  MMIPB_NAME_MAX_LEN,
                  vcard->vCardName.nameField[0].is_ucs2,
                  vcard->vCardName.nameField[0].str_len,
                  (const char *)vcard->vCardName.nameField[0].str_ptr
                 );
        }

        if (vcard->vCardName.nameField[1].str_ptr)	// given name
        {
            uint8 first_field_len=0;
            //syncml_vcard_parse_message:" field1,is_ucs2=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2812_112_2_18_2_47_30_332,(uint8*)"d",vcard->vCardName.nameField[1].is_ucs2);
            
         /***********************************************************************************
         中国移动PIM接口规范3.0版本要求，服务器下发数据要放在N字段
         的第一个域中。但当前商用服务器不符合规范，姓字段在第一个
         域下发，名在第二个域下发，此处将两个域合并。         
        **************************************************************************************/
        first_field_len=pb_entry->name.wstr_len;
         
        Str2Wstr(&(pb_entry->name.wstr_len),
              &(pb_entry->name.wstr[first_field_len]),
              MMIPB_NAME_MAX_LEN-first_field_len,
              vcard->vCardName.nameField[1].is_ucs2,
              vcard->vCardName.nameField[1].str_len,
              (const char *)vcard->vCardName.nameField[1].str_ptr
             );
         pb_entry->name.wstr_len=pb_entry->name.wstr_len+first_field_len;
         
        }

        if (vcard->vCardName.nameField[2].str_ptr)	// middle name
        {
            //syncml_vcard_parse_message:"field2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2834_112_2_18_2_47_30_333,(uint8*)"");
        }

        if (vcard->vCardName.nameField[3].str_ptr)	// prefix name
        {
            //syncml_vcard_parse_message:"Prefix Name: %s\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2839_112_2_18_2_47_30_334,(uint8*)"s", vcard->vCardName.nameField[3].str_ptr);
        }

        if (vcard->vCardName.nameField[4].str_ptr)	// suffix name
        {
            //syncml_vcard_parse_message:"Suffix Name: %s\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2844_112_2_18_2_47_30_335,(uint8*)"s", vcard->vCardName.nameField[4].str_ptr);
        }


	    //syncml_vcard_parse_message:"alpha_len = %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2848_112_2_18_2_47_30_336,(uint8*)"d", pb_entry->name.wstr_len);
        

    }

    // Name
    if (vcard->vCardName.property.charSet)
        SCI_FREE(vcard->vCardName.property.charSet);
    if (vcard->vCardName.property.language)
        SCI_FREE(vcard->vCardName.property.language);

    for (i=0; i<5; i++)
    {
        if (vcard->vCardName.nameField[i].str_ptr)
            SCI_FREE(vcard->vCardName.nameField[i].str_ptr);
    }


    /********* Telecommunications Addressing Properties **********/
    LstInit(&vcard->vCardTel);
    ExtractTelFrVcard((const char *)dataPtr, vcard);

    telQueue = (VCARD_TEL_PROPERTY_TYPE *)LstFirst(&vcard->vCardTel);

    while (telQueue)
    {
        
        number_type_flag = 0;
        numberType=VCARD_NUM_TYPE_MAX;
        
        if (telQueue->telType.prefType)
        {
            number_type_flag=number_type_flag|1;
        }
        if (telQueue->telType.workType)
        {    
            number_type_flag=number_type_flag|(1<<1);
        }
        if (telQueue->telType.homeType)//HOME
        {
            number_type_flag=number_type_flag|(1<<2);
        }
        if (telQueue->telType.voiceType)
	 {
            number_type_flag=number_type_flag|(1<<3);
        }
        
        if (telQueue->telType.faxType)//FAX
        {
            number_type_flag=number_type_flag|(1<<4);
        }
        if (telQueue->telType.msgType)
		{
            number_type_flag=number_type_flag|(1<<5);
        }

        if (telQueue->telType.cellType)
        {
            number_type_flag=number_type_flag|(1<<6);
        }
        if (telQueue->telType.pagerType)
        {
            number_type_flag=number_type_flag|(1<<7);
        }
        if (telQueue->telType.bbsType)
        {
            number_type_flag=number_type_flag|(1<<8);
        }
        if (telQueue->telType.modemType)
        {
            number_type_flag=number_type_flag|(1<<9);
        }
        if (telQueue->telType.carType)
        {
            number_type_flag=number_type_flag|(1<<10);
        }
        if (telQueue->telType.isdnType)
        {
            number_type_flag=number_type_flag|(1<<11);
        }
        if (telQueue->telType.videoType)
        {
            number_type_flag=number_type_flag|(1<<12);
        }
        //syncml_message:"mmipim tel,type_flag=%x,workType=%d,homeType=%d,voiceType=%d,faxType=%d,cellType=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_2934_112_2_18_2_47_30_337,(uint8*)"dddddd",number_type_flag,telQueue->telType.workType,telQueue->telType.homeType,telQueue->telType.voiceType,telQueue->telType.faxType,telQueue->telType.cellType);
        //需要根据当前type类型，拷贝到电话簿结构对应域内
        if (telQueue->tel)
        {
            //按照中国移动3.0规范，添加下面所有关于号码的处理
            if(0==number_type_flag)//tel
            {
                numberType = VCARD_NUM_TYPE_NUM;
            }
            
            if(0x0001==number_type_flag)//pref
            {
                
            }
            if(0x0040==number_type_flag)//tel.cell   
            {
                numberType = VCARD_NUM_TYPE_MOBILE;
            }
            if(0x0004==number_type_flag)//tel.home
            {
                numberType = VCARD_NUM_TYPE_HOME;
            }
            if(0x0002==number_type_flag)//tel.work
            {
                numberType = VCARD_NUM_TYPE_OFFICE;
            }
            if(0x0008==number_type_flag)//tel.voice
            {
                
            }
            if(0x0010==number_type_flag)//tel.fax
            {
                 numberType = VCARD_NUM_TYPE_FAX;
            }
            if(0x0042==number_type_flag)//tel.cell.work
            {

            }
            if(0x0044==number_type_flag)//tel.cell.home
            {

            }
            if(0x0012==number_type_flag)//tel.fax.work
            {

            }
        	if(0x0014==number_type_flag)//tel.fax.home
            {

            }
            if(0x1000==number_type_flag)//tel.video
            {

            }
        	if(0x1004==number_type_flag)//tel.video.home
            {

            }
            if(0x1002==number_type_flag)//tel.video.work
            {

            }
        	if(0x1040==number_type_flag)//tel.video.cell
            {

            }

            j=0;//add by wangxiaolin 
            sprintf(msg,"%s", telQueue->tel);
/*Start of  wangxiaolin 2009.11.2 for add by chenxiang */
            syncml_vcard_parse_message(("MMIPIM chenxiang VcardParser tel 1 msg = %s", msg));
            while('\0' != msg[j])
            {
                if(!((msg[j] >= '0' && msg[j] <= '9')||msg[j] == '*'||
                    msg[j] == '#'||(msg[j] == 'P'||msg[j] == 'p')||
                    msg[j] == 'W'||msg[j] == 'w' || msg[j] == '+'))
                {
                    uint16 count  = strlen(msg);
                    for (k = j; k < count; k++)
                    {
                        msg[k] = msg[k+1];
                    }
                }
                else
                {
                    j++;
                }
            }
            //syncml_vcard_parse_message:"MMIPIM chenxiang VcardParser tel 2 msg = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3021_112_2_18_2_47_30_338,(uint8*)"s", msg);
/*End   of  wangxiaolin 2009.11.2 for add by chenxiang */
            if((numberType != VCARD_NUM_TYPE_MAX) &&(numberType<MMIPB_MAX_NV_PHONE_NUM))
            {
                if (MMIAPICOM_GenPartyNumber( (uint8 *)msg, strlen(msg), &party_num) )
                {
                     pb_entry->number[numberType].npi_ton = (uint8)(party_num.number_type << 0X04);
                     //syncml_vcard_parse_message:"tel  npi_ton = %d"
                     SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3027_112_2_18_2_47_30_339,(uint8*)"d",pb_entry->number[numberType].npi_ton);
                    pb_entry->number[numberType].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                     hs_smlLibMemcpy( pb_entry->number[numberType].number, party_num.bcd_num, pb_entry->number[numberType].number_len);
                }
                else
                {
                    //syncml_vcard_parse_message:"tel  false"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3033_112_2_18_2_47_30_340,(uint8*)"");
                }
            }
        }

        telQueue = (VCARD_TEL_PROPERTY_TYPE *)LstNext((PNodeType)telQueue);
    }

    // TEL
    while (LstCount(&vcard->vCardTel))
    {
        telQueue = (VCARD_TEL_PROPERTY_TYPE *)LstGet(&vcard->vCardTel);
        if (!telQueue)
            break;

        if (telQueue->property.charSet)
            SCI_FREE(telQueue->property.charSet);
        if (telQueue->property.language)
            SCI_FREE(telQueue->property.language);

        if (telQueue->tel)
            SCI_FREE(telQueue->tel);
        
        //if (telQueue) //del by wangxiaolin 2009.12.20 for del warning
            SCI_FREE(telQueue);
    }

    /********* E-mail Addressing Properties **********/

    LstInit(&vcard->vCardEmail);
    ExtractEmailFrVcard((const char *)dataPtr, vcard);

    emailQueue = (VCARD_EMAIL_PROPERTY_TYPE *)LstFirst(&vcard->vCardEmail);

    while (emailQueue)
    {
        //syncml_vcard_parse_message:"EMAIL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3069_112_2_18_2_47_30_341,(uint8*)"");

        if (emailQueue->property.encoding.quotedPrintable)
            //syncml_vcard_parse_message:"\t[ENCODING=QUOTED-PRINTABLE]\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3072_112_2_18_2_47_30_342,(uint8*)"");
        else if (emailQueue->property.encoding.base64)
            //syncml_vcard_parse_message:"\t[ENCODING=BASE64]\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3074_112_2_18_2_47_30_343,(uint8*)"");
        else if (emailQueue->property.encoding.eightBit)
            //syncml_vcard_parse_message:"\n[ENCODING=8BIT]\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3076_112_2_18_2_47_30_344,(uint8*)"");


        if (emailQueue->property.valueLocation.cid)
            //syncml_vcard_parse_message:"\t[VALUE=CONTENT-ID]\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3080_112_2_18_2_47_30_345,(uint8*)"");
        else if (emailQueue->property.valueLocation.url)
            //syncml_vcard_parse_message:"\t[VALUE=URL]\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3082_112_2_18_2_47_30_346,(uint8*)"");

        //syncml_vcard_parse_message:"\t[Type=%s]\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3084_112_2_18_2_47_30_347,(uint8*)"s", EmailType[emailQueue->formatType]);

        if (emailQueue->email)
        {



#if 1//add  by chenxiang  2009.11.2
            uint8 utf8_ptr[MAX_CHAR] = {0};
            int utf8_len = 0;
            //signed int valueLen = 0;
            //syncml_vcard_parse_message:"MMIPIM chenxiang VcardParser email valueLen = %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3095_112_2_18_2_47_30_348,(uint8*)"d",(uint16)strlen(emailQueue->email));
            //syncml_vcard_parse_message:"MMIPIM chenxiang VcardParser email 1 utf8_ptr = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3096_112_2_18_2_47_30_349,(uint8*)"s", utf8_ptr);
            utf8_len = DecodeQuoted(emailQueue->email, utf8_ptr, (uint16)strlen(emailQueue->email));
            //syncml_vcard_parse_message:"MMIPIM chenxiang VcardParser email utf8_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3098_112_2_18_2_47_30_350,(uint8*)"d",utf8_len);
            //syncml_vcard_parse_message:"MMIPIM chenxiang VcardParser email 1 utf8_ptr = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3099_112_2_18_2_47_30_351,(uint8*)"s", utf8_ptr);
            j=0;//add by wangxiaolin
            while('\0' != utf8_ptr[j])
            {
                if(utf8_ptr[j]>=0x80 )
                {
                    uint16 count  = strlen((char*)utf8_ptr);
                    for (k = j; k < count; k++)
                    {
                        utf8_ptr[k] = utf8_ptr[k+1];
                    }
                }
                else
                {
                    j++;
                }
            }

            //syncml_vcard_parse_message:"MMIPIM chenxiang VcardParser email 2 utf8_ptr = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3117_112_2_18_2_47_30_352,(uint8*)"s", utf8_ptr);
#ifdef MMIPB_MAIL_SUPPORT
            Str2Wstr(&(pb_entry->mail.wstr_len),
                      pb_entry->mail.wstr,
                      MMIPB_MAX_MAIL_LEN,
                      FALSE,
                      utf8_len,
                      (const char *)utf8_ptr
                     );
#endif
#else //add  by chenxiang  2009.11.2
            Str2Wstr(&(pb_entry->mail.wstr_len),
                      pb_entry->mail.wstr,
                      MMIPB_MAX_MAIL_LEN,
                      FALSE,
                      (uint16)strlen(emailQueue->email),
                      (const char *)emailQueue->email
                     );
#endif //end of add  by chenxiang  2009.11.2

		//add over
        }

        emailQueue = (VCARD_EMAIL_PROPERTY_TYPE *)LstNext((PNodeType)emailQueue);/*lint !e26*/
    }


    // EMAIL
    while (LstCount(&vcard->vCardEmail))
    {
        emailQueue = (VCARD_EMAIL_PROPERTY_TYPE *)LstGet(&vcard->vCardEmail);
        if (!emailQueue)
            break;

        if (emailQueue->property.charSet)
            SCI_FREE(emailQueue->property.charSet);
        if (emailQueue->property.language)
            SCI_FREE(emailQueue->property.language);

        if (emailQueue->email)
            SCI_FREE(emailQueue->email);

        //if (emailQueue)  //del by wangxiaolin 2009.12.20 for del warning
            SCI_FREE(emailQueue);
    }

    #ifdef HS_PIM_SUPPORT_ALL_FIELD  //Added by wangxiaolin  2009.7.9 for 电话簿其他字段
    //Note
    ExtractNoteFrVcard((const char *)dataPtr, vcard);
    
    if (vcard->vCardNote.note)	
    {
        int valueLen=0;
        //syncml_vcard_parse_message:"mmipim , parse note!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3169_112_2_18_2_47_30_353,(uint8*)"");
        
        valueLen=strlen(vcard->vCardNote.note);
        valueLen=MIN(MAX_CHAR-1,valueLen);
        
        if((vcard->vCardNote.property.encoding.quotedPrintable == 1)
        	  &&(strstr(vcard->vCardNote.property.charSet, "UTF-8")))
        {

                uint8 utf8_ptr[MAX_CHAR] = {0};
                uint8 ucs2_ptr[2*MAX_CHAR] = {0};
                int utf8_len = 0;
                uint16 ucs2_len = 0;
                //syncml_vcard_parse_message:"mmipim , parse note!qp ,utf8"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3182_112_2_18_2_47_30_354,(uint8*)"");
               
                if (valueLen>0)
                {
                      //qp->utf-8
                      utf8_len = DecodeQuoted(vcard->vCardNote.note, utf8_ptr,  valueLen);
                      //syncml_vcard_parse_message:"utf8_len=%d"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3188_112_2_18_2_47_30_355,(uint8*)"d",utf8_len);
 
                      //utf8->ucs2
                      VmByteToUnicode((const char*)utf8_ptr, utf8_len, (uint16*)ucs2_ptr, sizeof(ucs2_ptr));
                      ucs2_len =(uint16)( 2 * pim_unicode_strlen((unsigned short *)ucs2_ptr));
                      ucs2_ptr[ucs2_len] = '\0';
                      ucs2_ptr[ucs2_len + 1] = '\0';

                      Str2Wstr(&(pb_entry->memo.wstr_len),
                                    pb_entry->memo.wstr,
                                    MMIPB_MAX_CUSTOM_TEXT_LEN,
                                    TRUE,
                                    ucs2_len,
                                    (const char *)ucs2_ptr
                                    );

              }
         }
        else
        {
              Str2Wstr(&(pb_entry->memo.wstr_len),
                            pb_entry->memo.wstr,
                            MMIPB_MAX_CUSTOM_TEXT_LEN,
                            FALSE,
                            valueLen,
                            (const char *)vcard->vCardNote.note
                            );
        }

    }


    if (vcard->vCardNote.property.charSet)
        SCI_FREE(vcard->vCardNote.property.charSet);
    
    if (vcard->vCardNote.property.language)
        SCI_FREE(vcard->vCardNote.property.language);
    
    if (vcard->vCardNote.note)
        SCI_FREE(vcard->vCardNote.note);
#if 0
    //提取职务  
    ExtractTitleFrVcard((const char *)dataPtr, vcard);
    
    if (vcard->vCardTitle.title)	
    {
        int valueLen=0;
        //syncml_vcard_parse_message:"mmipim , parse title!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3235_112_2_18_2_47_31_356,(uint8*)"");
        
        valueLen=strlen(vcard->vCardTitle.title);
        valueLen=MIN(MAX_CHAR-1,valueLen);
        
        if((vcard->vCardTitle.property.encoding.quotedPrintable == 1)
        	  &&(strstr(vcard->vCardTitle.property.charSet, "UTF-8")))
        {

                uint8 utf8_ptr[MAX_CHAR] = {0};
                uint8 ucs2_ptr[2*MAX_CHAR] = {0};
                int utf8_len = 0;
                uint16 ucs2_len = 0;
                //syncml_vcard_parse_message:"mmipim , parse title!qp ,utf8"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3248_112_2_18_2_47_31_357,(uint8*)"");
               
                if (valueLen>0)
                {
                      //qp->utf-8
                      utf8_len = DecodeQuoted(vcard->vCardTitle.title, utf8_ptr,  valueLen);
                      //syncml_vcard_parse_message:"utf8_len=%d"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3254_112_2_18_2_47_31_358,(uint8*)"d",utf8_len);
 
                      //utf8->ucs2
                      VmByteToUnicode((const char*)utf8_ptr, utf8_len, (uint16*)ucs2_ptr, sizeof(ucs2_ptr));
                      ucs2_len =(uint16)( 2 * pim_unicode_strlen((unsigned short *)ucs2_ptr));
                      ucs2_ptr[ucs2_len] = '\0';
                      ucs2_ptr[ucs2_len + 1] = '\0';

                      Str2Wstr(&(pb_entry->title.wstr_len),
                                    pb_entry->title.wstr,
                                    MMIPB_MAX_TITLE_LEN,
                                    TRUE,
                                    ucs2_len,
                                    (const char *)ucs2_ptr
                                    );

              }
         }
        else
        {
              Str2Wstr(&(pb_entry->title.wstr_len),
                            pb_entry->title.wstr,
                            MMIPB_MAX_TITLE_LEN,
                            FALSE,
                            valueLen,
                            (const char *)vcard->vCardTitle.title
                            );
        }

    }


    if (vcard->vCardTitle.property.charSet)
        SCI_FREE(vcard->vCardTitle.property.charSet);
    
    if (vcard->vCardTitle.property.language)
        SCI_FREE(vcard->vCardTitle.property.language);
    
    if (vcard->vCardTitle.title)
        SCI_FREE(vcard->vCardTitle.title);

 #endif
#if 0
    //Org
    ExtractOrgFrVcard((const char *)dataPtr, vcard);

   
    if (vcard->vCardOrg.org[0])	
    {
        int valueLen=0;
        //syncml_vcard_parse_message:"mmipim , parse org!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3303_112_2_18_2_47_31_359,(uint8*)"");
        
        valueLen=strlen(vcard->vCardOrg.org[0]);
        valueLen=MIN(MAX_CHAR-1,valueLen);
        
        if((vcard->vCardOrg.property.encoding.quotedPrintable == 1)
        	  &&(strstr(vcard->vCardOrg.property.charSet, "UTF-8")))
        {

                uint8 utf8_ptr[MAX_CHAR] = {0};
                uint8 ucs2_ptr[2*MAX_CHAR] = {0};
                int utf8_len = 0;
                uint16 ucs2_len = 0;
                //syncml_vcard_parse_message:"mmipim , parse org!qp ,utf8"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3316_112_2_18_2_47_31_360,(uint8*)"");
               
                if (valueLen>0)
                {
                      //qp->utf-8
                      utf8_len = DecodeQuoted(vcard->vCardOrg.org[0], utf8_ptr,  valueLen);
                      //syncml_vcard_parse_message:"utf8_len=%d"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3322_112_2_18_2_47_31_361,(uint8*)"d",utf8_len);
 
                      //utf8->ucs2
                      VmByteToUnicode((const char*)utf8_ptr, utf8_len, (uint16*)ucs2_ptr, sizeof(ucs2_ptr));
                      ucs2_len =(uint16)( 2 * pim_unicode_strlen((unsigned short *)ucs2_ptr));
                      ucs2_ptr[ucs2_len] = '\0';
                      ucs2_ptr[ucs2_len + 1] = '\0';

                      Str2Wstr(&(pb_entry->company.wstr_len),
                                    pb_entry->company.wstr,
                                    MMIPB_MAX_ORG_LEN,
                                    TRUE,
                                    ucs2_len,
                                    (const char *)ucs2_ptr
                                    );

              }
         }
        else
        {
              Str2Wstr(&(pb_entry->company.wstr_len),
                            pb_entry->company.wstr,
                            MMIPB_MAX_ORG_LEN,
                            FALSE,
                            valueLen,
                            (const char *)vcard->vCardOrg.org[0]
                            );
        }

    }
    if (vcard->vCardOrg.org[1])	//部门信息
    {
#if 0//del by wangxiaolin  2009.8.28  //暂时删除
        int valueLen=0;
        uint8 first_field_len=0;
        
        //syncml_vcard_parse_message:" mmipim vcard_org-unit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3358_112_2_18_2_47_31_362,(uint8*)"");
        first_field_len=pb_entry->org.wstr_len;

        valueLen=strlen(vcard->vCardOrg.org[1]);
        valueLen=MIN(MAX_CHAR-1,valueLen);
        
        if((vcard->vCardOrg.property.encoding.quotedPrintable == 1)
        	  &&(strstr(vcard->vCardOrg.property.charSet, "UTF-8")))
        {

                uint8 utf8_ptr[MAX_CHAR] = {0};
                uint8 ucs2_ptr[2*MAX_CHAR] = {0};
                int utf8_len = 0;
                uint16 ucs2_len = 0;
                //syncml_vcard_parse_message:"mmipim , parse org!qp ,utf8"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3372_112_2_18_2_47_31_363,(uint8*)"");
               
                if (valueLen>0)
                {
                      //qp->utf-8
                      utf8_len = DecodeQuoted(vcard->vCardOrg.org[1], utf8_ptr,  valueLen);
                      //syncml_vcard_parse_message:"utf8_len=%d"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3378_112_2_18_2_47_31_364,(uint8*)"d",utf8_len);
 
                      //utf8->ucs2
                      VmByteToUnicode((const char*)utf8_ptr, utf8_len, (uint16*)ucs2_ptr, sizeof(ucs2_ptr));
                      ucs2_len =(uint16)( 2 * pim_unicode_strlen((unsigned short *)ucs2_ptr));
                      ucs2_ptr[ucs2_len] = '\0';
                      ucs2_ptr[ucs2_len + 1] = '\0';

                      Str2Wstr(&(pb_entry->org.wstr_len),
                                    &(pb_entry->org.wstr[first_field_len]),
                                    MMIPB_MAX_ORG_LEN-first_field_len,
                                    TRUE,
                                    ucs2_len,
                                    (const char *)ucs2_ptr
                                    );
                       pb_entry->org.wstr_len=pb_entry->org.wstr_len+first_field_len;


              }
         }
        else
        {
              Str2Wstr(&(pb_entry->org.wstr_len),
                            &(pb_entry->org.wstr[first_field_len]),
                            MMIPB_MAX_ORG_LEN-first_field_len,
                            FALSE,
                            valueLen,
                            (const char *)vcard->vCardOrg.org[1]
                            );

               pb_entry->org.wstr_len=pb_entry->org.wstr_len+first_field_len;
        }
#endif
    }
    
     if (vcard->vCardOrg.org[2])	
    {
      // TODO: 组织其他字段的处理 /*-wangxiaolin 2009.8.25 -*/
    }
     
    if (vcard->vCardOrg.property.charSet)
        SCI_FREE(vcard->vCardOrg.property.charSet);
    
    if (vcard->vCardOrg.property.language)
        SCI_FREE(vcard->vCardOrg.property.language);

     for (i=0; i<3; i++)
    {
        if (vcard->vCardOrg.org[i])
            SCI_FREE(vcard->vCardOrg.org[i]);
    }
#endif
 #if 0//去掉对地址字段的支持   
    //Addr
    LstInit(&vcard->vCardAdr);
    ExtractAdrFrVcard((const char *)dataPtr, vcard);

    adrQueue = (VCARD_ADR_PROPERTY_TYPE *)LstFirst(&vcard->vCardAdr);

    while (adrQueue)
    {
        //syncml_vcard_parse_message:"ADDR "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3438_112_2_18_2_47_31_365,(uint8*)"");

        //Addr 可以有home,work等参数修饰，每个addr可以有7个域
        //因为展讯当前仅支持公司地址，且只有一个域，所以，如果支持
        //也只能对应支持一个域(但，当前测试服务器上添加的地址是全字段)

        //展讯只支持公司地址
        if(adrQueue->adrType.workType==1)
        {
            for (i=0; i<2; i++)
            {
                if (adrQueue->adrField[i])
                {
                    //解码，然后添加到对应的电话簿结构中去
                    
                    int valueLen=0;
                    //syncml_vcard_parse_message:"mmipim , parse addr!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3454_112_2_18_2_47_31_366,(uint8*)"");
                    
                    valueLen=strlen(adrQueue->adrField[i]);
                    valueLen=MIN(MAX_CHAR-1,valueLen);
                    
                    if((adrQueue->property.encoding.quotedPrintable == 1)
                    	  &&(strstr(adrQueue->property.charSet, "UTF-8")))
                    {

                            uint8 utf8_ptr[MAX_CHAR] = {0};
                            uint8 ucs2_ptr[2*MAX_CHAR] = {0};
                            int utf8_len = 0;
                            uint16 ucs2_len = 0;
                            //syncml_vcard_parse_message:"mmipim , parse org!qp ,utf8"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3467_112_2_18_2_47_31_367,(uint8*)"");
                           
                            if (valueLen>0)
                            {
                                  //qp->utf-8
                                  utf8_len = DecodeQuoted(adrQueue->adrField[i], utf8_ptr,  valueLen);
                                  //syncml_vcard_parse_message:"utf8_len=%d"
                                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3473_112_2_18_2_47_31_368,(uint8*)"d",utf8_len);
             
                                  //utf8->ucs2
                                  VmByteToUnicode((const char*)utf8_ptr, utf8_len, (uint16*)ucs2_ptr, sizeof(ucs2_ptr));
                                  ucs2_len =(uint16)( 2 * pim_unicode_strlen((unsigned short *)ucs2_ptr));
                                  ucs2_ptr[ucs2_len] = '\0';
                                  ucs2_ptr[ucs2_len + 1] = '\0';

                                  Str2Wstr(&(pb_entry->work_adr.wstr_len),
                                                pb_entry->work_adr.wstr,
                                                MMIPB_MAX_WORKADR_LEN,
                                                TRUE,
                                                ucs2_len,
                                                (const char *)ucs2_ptr
                                                );

                          }
                     }
                    else
                    {
                          Str2Wstr(&(pb_entry->work_adr.wstr_len),
                                        pb_entry->work_adr.wstr,
                                        MMIPB_MAX_WORKADR_LEN,
                                        FALSE,
                                        valueLen,
                                        (const char *)vcard->vCardOrg.org[0]
                                        );
                    }
                }
            }
        }
        adrQueue = (VCARD_ADR_PROPERTY_TYPE *)LstNext((PNodeType)adrQueue);/*lint !e64 !e26*/
    }


    // FREE ADDR
    while (LstCount(&vcard->vCardAdr))
    {
        adrQueue = (VCARD_ADR_PROPERTY_TYPE *)LstGet(&vcard->vCardAdr);/*lint !e64 */
        if (!adrQueue)
            break;

        if (adrQueue->property.charSet)
            SCI_FREE(adrQueue->property.charSet);
        
        if (adrQueue->property.language)
            SCI_FREE(adrQueue->property.language);
        
        for (i=0; i<7; i++)
        {
            if (adrQueue->adrField[i])
                SCI_FREE(adrQueue->adrField[i]);
        }

       // if (adrQueue) //del by wangxiaolin 2009.12.20 for del warning 
            SCI_FREE(adrQueue);

    }
#endif
  #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.6.22  
  
  #endif
    #endif //end of HS_PIM_SUPPORT_ALL_FIELD
    return 1;
}


/*****************************************************************************/
// 	Description : String To UCS2
//	Global resource dependence :
//  Author: huhengyi
//	Note:
/*****************************************************************************/
static uint16 Uint8StrToUint16Ucs2(
    uint16* dest_ucs2_ptr, //[OUT] ucs2 code
    uint16  dest_ucs2_len, //[IN]  ucs2 length (word)
    uint8*  src_str_ptr,   //[IN]  string
    uint8  src_str_len  //[IN]  string length (byte)
)
{
    // 允许参数为空为0
    uint16 i = 0;
    uint16 for_limit = 0;

    if (   (PNULL == dest_ucs2_ptr)
            || (0     == dest_ucs2_len)
            || (PNULL == src_str_ptr)
            || (0     == src_str_len)   )
    {
        return 0;
    }

    for_limit = (uint16)(src_str_len>>1);

    SCI_PASSERT(dest_ucs2_len >= for_limit, ("StringToUint16Ucs2: 1 error!"));/*assert verified*/

    for(i = 0; i < for_limit; i++)
    {
        dest_ucs2_ptr[i] = *src_str_ptr;
        src_str_ptr++;
        dest_ucs2_ptr[i] = (uint16)((dest_ucs2_ptr[i]<< 8) + *src_str_ptr);
        src_str_ptr++;
    }
    return for_limit;
}

// Quoted-Printable是MIME邮件中常用的编码方式之一
//同Base64一样，它也将输入的字符串或数据编
//码成全是ASCII码的可打印字符串。

//Quoted-Printable编码的基本方法是：输入数据在33-60、62-126
//范围内的，直接输出；其它的需编码为“=”加两个字节
//的HEX码(大写)。为保证输出行不超过规定长度，可在行尾加“=\r\n”
//序列作为软回车。

int EncodeQuoted(const unsigned char* pSrc, char* pDst, int nSrcLen )
{
    int nDstLen;        // 输出的字符计数
    //   int nLineLen;       // 输出的行长度计数
    int i = 0;
    //    nLineLen = 0;
    char cFirst ;
    char cSecond ;
    nDstLen = 0;

    for ( i = 0; i < nSrcLen; i++, pSrc++)
    {
        // ASCII 33-60, 62-126原样输出，其余的需编码
#if 0//del by wangxiaolin  2009.11.3
        if ((*pSrc >= '!') && (*pSrc <= '~') && (*pSrc != '='))
#else
        if ((*pSrc >= '!') && (*pSrc <= '~') && (*pSrc != '=')
        && (*pSrc != '&')&& (*pSrc != '>')&& (*pSrc != '<')&& (*pSrc != '|')&& (*pSrc != '%')&& (*pSrc != ';'))
#endif
        {
            *pDst++ = (char)*pSrc;
            nDstLen++;
            //            nLineLen++;
        }
        else
        {
            //            sprintf(pDst, "=%02X", *pSrc);
            cFirst =(char)( 0x0F & (*pSrc >> 4));
            cSecond = (char)(0x0F & *pSrc);
            //syncml_vcard_parse_message:"cFirst = %d ,cSecond = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3615_112_2_18_2_47_31_369,(uint8*)"dd",cFirst,cSecond);

            *pDst++ = '=';
            *pDst++ = (char)((cFirst < 0xA) ? (cFirst + 48):(cFirst + 55));
            *pDst++ = (char)((cSecond < 0xA) ? (cSecond + 48):(cSecond + 55));
            //syncml_vcard_parse_message:"pDst = %d , %d,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3620_112_2_18_2_47_31_370,(uint8*)"ddd",pDst[nDstLen],pDst[nDstLen+1],pDst[nDstLen+2]);
            //           pDst += 3;
            nDstLen += 3;
            //            nLineLen += 3;
        }
      //  syncml_vcard_parse_message(("pSrc = %d , %02X",*pSrc,*pSrc));
#if 0
        // 输出换行？
        if (nLineLen >= nMaxLineLen - 3)
        {
            sprintf(pDst, "=\r\n");
            pDst += 3;
            nDstLen += 3;
            nLineLen = 0;
        }
#endif

    }

    // 输出加个结束符
    *pDst = '\0';
    return nDstLen;
}

//Quoted-Printable解码很简单，将编码过程反过来就行了。

int DecodeQuoted(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
    int nDstLen;        // 输出的字符计数
    int i;
    unsigned char TempChr1, TempChr2;
    int equal_mark_num = 0;
    
    i = 0;
    nDstLen = 0;

    while (i < nSrcLen)
    {
     //   syncml_vcard_parse_message(("pSrc = %d",*pSrc));
        if (strncmp(pSrc, "=\r\n", 3) == 0)        // 软回车，跳过
        {
            pSrc += 3;
            i += 3;
        }
        else
        {
            if (*pSrc == '=')        // 是编码字节
            {
                //                sscanf(pSrc, "=%02X", pDst);
                equal_mark_num++;
                TempChr1 = HexToChr(*++pSrc);
                TempChr2 = HexToChr(*++pSrc);
                //syncml_vcard_parse_message:"TempChr1 = %d TempChr2 = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3672_112_2_18_2_47_31_371,(uint8*)"dd",TempChr1,TempChr2);
                if (TempChr1 == 0xff || TempChr2 == 0xff)
                {
                    //容错处理，如果出现超长的字符(可能为中文，英文和数字的混合)，此时我们截下的长度内
                    //最后一个字符可能不是一个 完整的汉字，就会出现(TempChr1 == 0xff || TempChr2 == 0xff)的情况，           
                    //如果遇到这种情况，则直接结束解码过程；
                    //每个中文字符的编码包括三个等号，例如"你"的编码是=AE=B0=E0; "你A1"的编码是=AE=B0=E0A1;
                    //"A1你"的编码是A1=AE=B0=E0;
                    //如果不做下列if处理,则最后一个字符可能是错误的字符;
                    // SCI_TRACE_LOW("DecodeQuoted,equal_mark_num=%d",equal_mark_num);
                    if ((equal_mark_num % 3) == 0) 
                    {
                        //当第三个等号出错时，前面两个等号的处理无效，然后结束该解码过程；
                        nDstLen = nDstLen - 2;
                        pDst = pDst - 2;
                    }
                    else if ((equal_mark_num % 3) == 2)
                    {
                        //当第二个等号出错时，前面一个等号的处理无效，然后结束该解码过程；
                        nDstLen = nDstLen - 1;
                        pDst = pDst - 1;
                    }
                    else if ((equal_mark_num % 3) == 1)
                    {
                        //当第一个等号出错时，直接结束该解码流程；
                    }
                     // 输出加个结束符
                    *pDst = '\0';
                    return nDstLen;  
                            
                }
                else
                {
                    *pDst++ = (unsigned char)(TempChr1 << 4 | TempChr2);
                    pSrc++;
                }
                //syncml_vcard_parse_message:"pDst = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3708_112_2_18_2_47_32_372,(uint8*)"d",*(pDst-1));
                i += 3;
            }
            else        // 非编码字节
            {
                *pDst++ = (unsigned char)*pSrc++;
                i++;
            }

            nDstLen++;
        }
    }

    // 输出加个结束符
    *pDst = '\0';

    return nDstLen;
}

static unsigned char HexToChr(char chr)
{
    unsigned char TempChr;
    if ( chr >= 'a' && chr <= 'f' )
    {
        TempChr =(unsigned char)(chr - 'a' + 10);
        return TempChr;
    }
    if ( chr >= '0' && chr <= '9' )
    {
        TempChr = (unsigned char)(chr - '0');
        return TempChr;
    }
    if ( chr >= 'A' && chr <= 'F' )
    {
        TempChr =(unsigned char)( chr - 'A' +10 );
        return TempChr;
    }
    else
        return 0xff;
}

#ifdef HS_PIM_SUPPORT_ALL_FIELD  //Added by wangxiaolin  2009.7.9 for 电话簿其他字段
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
/**********************************************************************************
 *Function: PhotoFile2Base64String
 *
 * Purpose: 
 * Relation:<describing the name, version and position of protocols involved by this function> 
 * Params: 
 *
 *   Name                Type            In/Out          Description
 * --------              ----            ------          ----------- 
 *  spFileName                                           文件名字符串
 * Return: <comment on the returned values> ,编码后的字符串地址
 * Notes: <the limitations to use this function or other comments>
 **********************************************************************************/
int PhotoFile2Base64String(const wchar* spFileName,char * base64_buffer)
{

    MMIFILE_HANDLE	file_handle = PNULL;
    MMIFILE_ERROR_E ffs_error = SFS_NO_ERROR;
    uint32 opt_mode=0;
    uint32 file_size = 0;
    uint32 charsread = 0;
    char *file_stream_buffer=NULL;
    
    //syncml_message:"MMIPIM wangxiaolin PhotoFile2Base64String"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3773_112_2_18_2_47_32_373,(uint8*)"");
    

    if(spFileName==NULL)
    {
       return 0;   
    }

    if(NULL==base64_buffer)
    {
        return 0;
    }
    
    if (!MMIAPIFMM_IsFileExist((wchar *)spFileName, (uint16)pim_unicode_strlen(spFileName)))/*lint !e605*/
    {
        return 0;
    }
    else
    {          
            opt_mode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;/*lint !e655*/
            file_handle=   MMIAPIFMM_CreateFile((uint16 *)spFileName, 
                                       				opt_mode, 
                                       				PNULL, 
                                       				PNULL
                                       				);
            file_size = MMIAPIFMM_GetFileSize(file_handle);
            if (0 == file_size)
            {
                MMIAPIFMM_CloseFile(file_handle);
                
                return 0;
            }
            else
            {



                file_stream_buffer= hs_smlLibMalloc(__FILE__, __LINE__, PHOTO_FILE_LEN);
                if(NULL==file_stream_buffer)
                {
                    return 0;
                }
                
                hs_smlLibMemset(file_stream_buffer,0,PHOTO_FILE_LEN);
                
                ffs_error = MMIAPIFMM_ReadFile(file_handle,file_stream_buffer,PHOTO_FILE_LEN,&charsread,PNULL);
                if (0 < charsread && SFS_NO_ERROR == ffs_error)
                {
                       char abSave[4];
                       int cbCopied=0;
                       unsigned long source_str_len = file_size;
                       unsigned long cbOffset = 0; 
                       abSave [0] = abSave [1] = abSave [2] = abSave [3] = '\0';
                       
                       //将二进制码流转换成BASE64字符串
                       cbCopied = hs_base64Encode ((unsigned char*)base64_buffer, BASE64_BUF_LEN,
                                        (unsigned char*) file_stream_buffer,
                                        &source_str_len, &cbOffset, 1, (unsigned char *)abSave);
                       base64_buffer[cbCopied] = '\0';
                       MMIAPIFMM_CloseFile(file_handle);    
                       
                       hs_smlLibFree(file_stream_buffer);
                       return   cbCopied;
                 }
                 else
                 {
                    hs_smlLibFree(file_stream_buffer);
                     return 0;
                 }
             }
      }
}

/**********************************************************************************
 *Function: Base64String2PhotoFile
 *
 * Purpose: 
 * Relation:<describing the name, version and position of protocols involved by this function> 
 * Params: 
 *
 *   Name                Type            In/Out          Description
 * --------              ----            ------          ----------- 
 *
 * Return: <comment on the returned values>
 * Notes: <the limitations to use this function or other comments>
 **********************************************************************************/
BOOL Base64String2PhotoFile(char* base64_str_ptr,const wchar* spFileName)
{

    unsigned long cbDataLength=0;
    unsigned long decoded_str_len=0;
    
    MMIFILE_HANDLE	file_handle = PNULL;
    uint32 opt_mode=0;
    uint32 charswrite = 0;
     char *file_stream_buffer=NULL;

    //syncml_message:"MMIPIM wangxiaolin Base64String2PhotoFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCARD_PARSE_3869_112_2_18_2_47_32_374,(uint8*)"");
    if((NULL==base64_str_ptr)||(NULL==spFileName))
    {
        return FALSE;
    }
    
    file_stream_buffer= hs_smlLibMalloc(__FILE__, __LINE__, PHOTO_FILE_LEN);
    if(NULL==file_stream_buffer)
    {
        return FALSE;
    }
    hs_smlLibMemset(file_stream_buffer,0,PHOTO_FILE_LEN);
    cbDataLength=strlen(base64_str_ptr);/*lint !e64 */
    
    //base64解码
    decoded_str_len=hs_base64Decode((unsigned char*)file_stream_buffer,PHOTO_FILE_LEN,
                                                    (unsigned char*)base64_str_ptr,&cbDataLength);/*lint !e64 */
    file_stream_buffer[decoded_str_len]='\0';

    //解码成功，将码流写入文件
    if(decoded_str_len>0)
    {
        if (MMIAPIFMM_IsFileExist((uint16 *)spFileName, pim_unicode_strlen(spFileName)))/*lint !e605*/
        {
            MMIAPIFMM_DeleteFile((uint16 *)spFileName, NULL);
        }

        opt_mode = SFS_MODE_CREATE_NEW | SFS_MODE_WRITE;/*lint !e655*/
        file_handle=   MMIAPIFMM_CreateFile((uint16 *)spFileName, 
        					opt_mode, 
        					PNULL, 
        					PNULL
        					);
        MMIAPIFMM_WriteFile(file_handle, file_stream_buffer, decoded_str_len+1, &charswrite, NULL);
        MMIAPIFMM_CloseFile(file_handle);
        
        hs_smlLibFree(file_stream_buffer);      

        return TRUE ;
    }
    else
    {
       hs_smlLibFree(file_stream_buffer);
        return FALSE;
    }
}
#endif
#endif //HS_PIM_SUPPORT_ALL_FIELD

