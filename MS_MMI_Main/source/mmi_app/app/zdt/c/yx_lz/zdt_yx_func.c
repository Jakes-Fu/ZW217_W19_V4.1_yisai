/*
云芯平台
*/
#include "zdt_app.h"
#ifndef WIN32
#include "mbedtls/pem.h"
#include "mbedtls/base64.h"
#include "mbedtls/des.h"
#include "mbedtls/aes.h"
#include "mbedtls/md5.h"
#include "mbedtls/cipher.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifndef MIN
   #define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif

#define ISUNRESERVED(c) ('*'==(c)||'-'==(c)||'.'==(c)||'_'==(c))

/*
  needs expansion
 */
#define ISQUOTE(c)  (!ISALNUM((c)) && !ISUNRESERVED((c)) && (' ' != (c)))

#define ISDIGIT(c)  ( (unsigned) ((c) - '0') < 10)
#define ISALPHA(c)  ( (unsigned) ( ((c)|32) - 'a') < 26 )
#define ISALNUM(c)  ( ISDIGIT(c) || ISALPHA(c) )
#define ISHEXDIGIT(c) ( ISDIGIT((c)) || (((unsigned)(((c)|32) - 'a')) < 6) )

static char gszHexDigits[] = "0123456789ABCDEF";
#define HEXDIGIT(n) gszHexDigits[(n)&0x0f]
/* will overflow, though */
#define HEXVAL(c) (ISDIGIT(c) ? ((c) - '0') : (((c)|32) - 'a' + 10))


#define XX 127


/**
  || Function
  || --------
  || static char *UrlEncode(const char *pcIn, int *pnInLen, 
  ||                        char *pcOut, int *pnOutLen)
  ||
  || Description
  || -----------
  || encodes a string for safe inclusion in a URL, % escapes any "reserved" 
  ||  characters
  ||
  || Parameters
  || ----------
  || const char *pcIn: pointer to string to be encoded
  || int *pnInLen: (in/out) pointer to inLen, set on return to number of bytes
  ||                 successfully encoded
  || char *pcOut: (out) where to write encoded bytes, may be NULL, for counting
  || int *pnOutLen: (in/out) for input, is sizeof pcOut, if pcOut is NULL, 
  ||                pnOutLen is set to desired buffer size, if pcOut is 
  ||                non-null, pnOutLen is set to length of encoded data, 
  ||                which may be less than sizeof(pcOut) if a full encoded
  ||                character won't fit, or if an encoded pcIn is shorter
  ||                than sizeof(pcOut)
  ||
  || Returns
  || -------
  || pcOut
  ||
  || Remarks
  || -------
  ||
  */
char *YX_Func_UrlEncode(const char *pcIn, int *pnInLen, 
                       char *pcOut, int *pnOutLen)
{
   int   nOutLen;
   int   nInLen = *pnInLen;
   const char *pc;

   /* how much space do we need? */
   for (pc = (pcIn + nInLen - 1), nOutLen = 0; pc >= pcIn; nOutLen++, pc--) {
      if (ISQUOTE(*pc)) {
         nOutLen += 2; /* two more */
      }
   }
  
   /* no output buffer, just return space needed */
   if ((char *)0 == pcOut) {
      goto done;
   }

   /* more space or more data? */
   nOutLen = MIN(nOutLen,*pnOutLen);
   
   {
      int i;
      const char *pEnd = pcIn + nInLen;
      /* how much can we encode, assuming nOutLen... */
      for (i = nOutLen, pc = pcIn; i > 0 && pc < pEnd; pc++) {
         if (ISQUOTE(*pc)) {
            if (i >= 3) {
               i -= 3;
            } else {
               break;
            }
         } else {
            i--;
         }
      }

      /* only encode this much...*/
      nInLen = MIN(nInLen, pc - pcIn);
   }
   /* return how much we encoded */
   *pnInLen = nInLen;

   /* to encode in place */
   
   memmove(pcOut + nOutLen - nInLen, pcIn, nInLen);

   /* finally do work */
   for (pc = pcOut + nOutLen - nInLen, nOutLen = 0; nInLen > 0;
        pc++, nInLen--) {
      unsigned char c = *pc;
      if (ISQUOTE(c)) {
         pcOut[nOutLen++] = '%';
         pcOut[nOutLen++] = HEXDIGIT(c>>4);
         pcOut[nOutLen++] = HEXDIGIT(c);
      } else {
         if (' ' == c) {
            c = '+';
         }
         pcOut[nOutLen++] = c;
      }
   }

 done:
   *pnOutLen = nOutLen;

   return pcOut;
}

/**
  || Function
  || --------
  || static char *UrlDecode(const char *pcIn, int *pnInLen,
  ||                        char *pcOut, int *pnOutLen)
  ||
  || Description
  || -----------
  || decodes a url-encoded string (% escaped)
  ||
  || Parameters
  || ----------
  || const char *pcIn: pointer to bytes to decode
  || int *pnInLen: (in/out) sizeof pcIn, on return set to number of bytes 
  ||                consumed
  || char *pcOut: where to decode to
  || int *pnOutLen: (in/out) for input, is sizeof pcOut, if pcOut is NULL, 
  ||                pnOutLen is set to desired buffer size, if pcOut is 
  ||                non-null, pnOutLen is set to length of decoded data, 
  ||                which may be less than sizeof(pcOut) if a decoded pcIn is 
  ||                shorter than sizeof(pcOut)
  ||
  || Returns
  || -------
  || pcOut
  ||
  || Remarks
  || -------
  ||
  */
char * YX_Func_UrlDecode(const char *pcIn, int *pnInLen,
                       char *pcOut, int *pnOutLen)
{
   int32       nWrote;
   int32       nOutLen;
   const char *pc;
   const char *pcEnd;

   if ((char *)0 == pcOut) {
      nOutLen = 0x7ffffff; /* MAXINT */
   } else {
      nOutLen = *pnOutLen;
   }

   pcEnd = pcIn + *pnInLen;
   for (nWrote = 0, pc = pcIn; (nWrote < nOutLen) && (pc < pcEnd); 
        pc++, nWrote++) {
      char c = *pc;

      if ('%' == c) {
         /* make sure we have the whole thing quoted char */
         if (pc < pcEnd - 2) {
            char c1 = pc[1];
            char c2 = pc[2];
               
            /* make sure it's really a quoted char */
            if (ISHEXDIGIT(c1) && ISHEXDIGIT(c2)) {
               c = (char)((HEXVAL(c1) << 4) + HEXVAL(c2));
               pc += 2;
            }
         } else {
            break; /* don't decode partials, bail */
         }

      } else if ('+' == c) {
         c = ' ';
      }
      
      if ((char *)0 != pcOut) {
         pcOut[nWrote] = c;
      }
   }

   *pnInLen  = pc - pcIn;
   *pnOutLen = nWrote;

   return pcOut;
}

#ifdef ZDT_PLAT_YX_SUPPORT
uint8 g_yx_al_repeat_num = 0;

void YX_Func_MD5_Generate(uint8 * input, unsigned int input_len,char out_str[32])
{
#ifndef WIN32
    mbedtls_md5_context md5_ctx = {0};
    uint8 digest[16] = {0};
    uint16 i = 0;
    mbedtls_md5_init( &md5_ctx );
    mbedtls_md5_starts( &md5_ctx );
    mbedtls_md5_update( &md5_ctx, input, input_len);
    mbedtls_md5_finish( &md5_ctx, digest);
    mbedtls_md5_free( &md5_ctx );
    
   for(i = 0; i < 16; i++)
   {
        sprintf(out_str+(i*2),"%02x",digest[i]);
   }
#endif
}

static uint8 Hex2Dec(uint8 bHexChar)
{
    uint8  bRtn = 0;

    if(bHexChar >= '0' && bHexChar <= '9')
        bRtn = bHexChar - '0';
    else
    {
        switch(bHexChar)
        {
            case 'A':
            case 'a':
                bRtn = 10;
                break;
            case 'B':
            case 'b':
                bRtn = 11;
                break;
            case 'C':
            case 'c':
                bRtn = 12;
                break;
            case 'D':
            case 'd':
                bRtn = 13;
                break;
            case 'E':
            case 'e':
                bRtn = 14;
                break;
            case 'F':
            case 'f':
                bRtn = 15;
                break;
        }
    }
    return(bRtn);
}

static uint32 decodeHex(uint8 * pSrcData,uint32 SrcDatalen, uint8* pDestData )
{
    uint32 i = 0;
    uint32 j = 0;
    uint8 f  = 0;
    uint8 t = 0;
    uint32 len = SrcDatalen;
    
    if(pSrcData == PNULL || SrcDatalen == 0 || pDestData == PNULL)
    {
        return 0;
    }

    for (i = 0, j = 0; j < len; i++)
    {
        t = Hex2Dec(pSrcData[j++]);
        f = t << 4;
        t = Hex2Dec(pSrcData[j++]);
        f = f | t;
        pDestData[i] = f;
    }
    
    return i;
}

static uint8 * YX_Func_AES_Encrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
{
    uint8 * encrypt_data = PNULL;
#ifndef WIN32
    mbedtls_aes_context aes_ctx = {0};
    uint8 iv_val[17] = {0};
    uint8 key_val[17] = {0};
    uint32 encrypt_data_len = 0;
    uint8 temp_len = 0;
    uint8 tmp_val = 0;
    uint32 i = 0;
    if(data == NULL || data_len == 0)
    {
        return PNULL;
    }
    temp_len = 16-(data_len%16);
    encrypt_data_len = data_len+temp_len;
    encrypt_data = SCI_ALLOC_APPZ(encrypt_data_len);
    if(encrypt_data == NULL)
    {
        return PNULL;
    }
    SCI_MEMCPY(encrypt_data,data,data_len);
    for(i = data_len; i < (data_len+temp_len); i++)
    {
        encrypt_data[i] = temp_len;
    }
    *pOutlen = encrypt_data_len;
    
    mbedtls_aes_init( &aes_ctx );

    decodeHex(YX_AES_IV_STR,strlen(YX_AES_IV_STR),iv_val);
    decodeHex(YX_AES_KEY_STR,strlen(YX_AES_KEY_STR),key_val);
    
    mbedtls_aes_setkey_enc( &aes_ctx, key_val, 128);

    mbedtls_aes_crypt_cbc( &aes_ctx, MBEDTLS_AES_ENCRYPT, encrypt_data_len, iv_val, encrypt_data, encrypt_data);

    mbedtls_aes_free( &aes_ctx );
#endif
    return encrypt_data;
}

static uint8 * YX_Func_AES_Decrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
{
    uint8 * encrypt_data = PNULL;
#ifndef WIN32
    mbedtls_aes_context aes_ctx = {0};
    uint8 iv_val[17] = {0};
    uint8 key_val[17] = {0};
    uint32 encrypt_data_len = 0;
    uint32 temp_len = 0;
    uint32 i = 0;
    if(data == NULL || data_len == 0)
    {
        return PNULL;
    }
    temp_len = data_len/16;
    encrypt_data_len = temp_len*16;
    encrypt_data = SCI_ALLOC_APPZ(encrypt_data_len+1);
    if(encrypt_data == NULL)
    {
        return PNULL;
    }
    SCI_MEMCPY(encrypt_data,data,encrypt_data_len);
    *pOutlen = encrypt_data_len;
    mbedtls_aes_init(&aes_ctx);

    decodeHex(YX_AES_IV_STR,strlen(YX_AES_IV_STR),iv_val);
    decodeHex(YX_AES_KEY_STR,strlen(YX_AES_KEY_STR),key_val);

    mbedtls_aes_setkey_dec( &aes_ctx, key_val, 128);

    mbedtls_aes_crypt_cbc( &aes_ctx, MBEDTLS_AES_DECRYPT, encrypt_data_len, iv_val, encrypt_data, encrypt_data);

    mbedtls_aes_free( &aes_ctx );
#endif
    return encrypt_data;
}

uint8 * YX_Func_Encrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
{
    uint8 * aes_data = PNULL;
    uint32  aes_len = 0;
    uint32  base64_max_len = 0;
    uint32  base64_rel_len = 0;
    uint8 * encrypt_data = PNULL;
    if(data == PNULL || data_len == 0)
    {
        return PNULL;
    }
    ZDT_CryptTrace(data,data_len);
    aes_data = YX_Func_AES_Encrypt(data,data_len,&aes_len);
    if(aes_data == PNULL)
    {
        return PNULL;
    }
    //ZDT_HexTraceU8(aes_data,aes_len);
    base64_max_len = (aes_len / 3 + 2) * 4+1;
    encrypt_data = SCI_ALLOC_APPZ(base64_max_len+1);
    if (encrypt_data  == NULL)
    {
        SCI_FREE(aes_data);
        return PNULL;
    }
#ifndef WIN32
    mbedtls_base64_encode(encrypt_data, base64_max_len, &base64_rel_len, aes_data, aes_len);
#endif
    *pOutlen = base64_rel_len;
    encrypt_data[base64_rel_len] = 0;
    ZDT_CryptTrace(encrypt_data,base64_rel_len);
    SCI_FREE(aes_data);
    return encrypt_data;
}

uint8 * YX_Func_Decrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
{
    uint8 * aes_data = PNULL;
    uint8  left_len = 0;
    uint32  aes_len = 0;
    uint32  aes_rel_len = 0;
    uint32  base64_max_len = 0;
    uint32  base64_rel_len = 0;
    uint8 * decrypt_data = PNULL;
    if(data == PNULL || data_len == 0)
    {
        return PNULL;
    }
    ZDT_DecryptTrace(data,data_len);
    base64_max_len = data_len;
    decrypt_data = SCI_ALLOC_APPZ(base64_max_len);
    if (decrypt_data  == NULL)
    {
        return PNULL;
    }
#ifndef WIN32
    mbedtls_base64_decode(decrypt_data, base64_max_len, &base64_rel_len, data, data_len);
#endif
    
    //ZDT_HexTraceU8(decrypt_data,base64_rel_len);
    aes_data = YX_Func_AES_Decrypt(decrypt_data,base64_rel_len,&aes_len);
    if(aes_data == PNULL)
    {
        SCI_FREE(decrypt_data);
        return PNULL;
    }
    left_len = aes_data[aes_len-1];
    aes_rel_len = aes_len - left_len;
    ZDT_DecryptTrace(aes_data,aes_rel_len);
    *pOutlen = aes_rel_len;
    SCI_FREE(decrypt_data);
    return aes_data;
}

void YX_Func_TestCrypt(void)
{
    uint8 * pOutData = PNULL;
    uint32 OutDataLen = 0;

    ZDT_LOG("YX_Func_TestCrypt [868560040058335,89860317947552295702,202202231403130022,GET_SERVER_INFO,3,20220223140313,1,1]");
    
    pOutData = YX_Func_Encrypt("[868560040058335,89860317947552295702,202202231403130022,GET_SERVER_INFO,3,20220223140313,1,1]",strlen("[868560040058335,89860317947552295702,202202231403130022,GET_SERVER_INFO,3,20220223140313,1,1]"),&OutDataLen);

    if(pOutData != PNULL)
    {
        uint8 * pDecData = PNULL;
        uint32 DecDataLen = 0;
        pDecData = YX_Func_Decrypt(pOutData,OutDataLen,&DecDataLen);
        if(pDecData != PNULL)
        {
            SCI_FREE(pDecData);
        }
        SCI_FREE(pOutData);
    }
    return;
}

uint8 YX_Func_HexCharToInt(uint8 hexchar)
{
	if (hexchar>=0x30 && hexchar <=0x39)
	{
		return (hexchar - 0x30);
	}
	if (hexchar>= 'a' && hexchar <='f')
	{
		return (hexchar - 'a' +10);
	}
	if (hexchar>= 'A' && hexchar <='F')
	{
		return (hexchar - 'A' +10);
	}
	return 15;
}

uint8 YX_Func_IntToHexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

uint8 YX_Func_IntToHexChar_One(uint8 data,uint8 * dest_buf)
{
    uint8 low = 0;
    uint8 high = 0;
    high = data>>4;
    dest_buf[0] = (char)YX_Func_IntToHexChar(high);
    low = data & 0x0F;
    dest_buf[1] = (char)YX_Func_IntToHexChar(low);
    return 2;
}

int  YX_Func_GetCmdPosPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    int StartIndex = 0;
    int p = pos;
    
    if(*pLen == 0)
    {
        return -1;
    }
    if(p > 0)
    {
        for(i=0; i < (*pLen); i++)
        {
            if(*(*pCmd + i) == ',')
            {
                p--;
                if(p == 0)
                {
                    StartIndex = i+1;
                    break;
                }
            }
        }
        
        if(p != 0)
        {
            return -2;
        }
    }
    
    for(i=StartIndex; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ',')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if((EndIndex-StartIndex) > 0)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=StartIndex; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex - StartIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '@')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaW(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '#')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaM(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ':')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaF(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '!')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaJ(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '+')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaD(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '=')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaStar(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '*')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetNextParaHush(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '#')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_GetPosPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    int StartIndex = 0;
    int p = pos;
    
    if(*pLen == 0)
    {
        return -1;
    }
    if(p > 0)
    {
        for(i=0; i < (*pLen); i++)
        {
            if(*(*pCmd + i) == ',')
            {
                p--;
                if(p == 0)
                {
                    StartIndex = i+1;
                    break;
                }
            }
        }
        
        if(p != 0)
        {
            return -2;
        }
    }
    
    for(i=StartIndex; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ',')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if((EndIndex-StartIndex) > 0)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=StartIndex; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex - StartIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

uint32 YX_Func_IntToHexChar_Array(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)YX_Func_IntToHexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)YX_Func_IntToHexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

int  YX_Func_ClockGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ':' || *(*pCmd + i) == '-' )
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_RepeatGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ';')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}
int  YX_Func_TimerGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ':' || *(*pCmd + i) == '-')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}

int  YX_Func_CR_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    int  is_cr = FALSE;
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == 0x0D || *(*pCmd + i) == 0x0A)
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
    
    if( (i+1) < *pLen && bHaveNextComma && *(*pCmd + i) == 0x0D && *(*pCmd + i + 1) == 0x0A)
    {
        is_cr = TRUE;
    }
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)
        {
            wMaxStrLen = 100;
        }
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            if(is_cr)
            {
                *pCmd += (EndIndex + 2);
                *pLen -= (EndIndex + 2);
            }
            else
            {
                *pCmd += (EndIndex + 1);
                *pLen -= (EndIndex + 1);
            }
        }
    }
    return(bRtn);
}
#if 1
uint8 YX_Func_GetIndexWater(uint8 * pData)
{
    static uint16 s_water_index = 0;
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%04d%02d%02d%02d%02d%02d%04d",date.year,date.mon,date.mday,time.hour,time.min,time.sec,s_water_index);
    s_water_index++;
    if(s_water_index > 9999)
    {
        s_water_index = 0;
    }
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetICCID(uint8 * pData)
{
    char tem_buf[50] = {0};
    uint16 len = 0; 

    SCI_MEMCPY(tem_buf,g_zdt_sim_iccid,ZDT_ICCID_LEN);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetAlertType(uint8 * pData,uint16 alert_type)
{
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d%",alert_type);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

//是否接入市电
uint8 YX_Func_Get220VStatus(uint8 * pData)
{
    if(CHGMNG_IsChargeConnect())
    {
        pData[0] = '0';
    }
    else
    {
        pData[0] = '1';
    }
    pData[1] = ',';
    return 2;
}

uint8 YX_Func_GetGPSStatus(uint8 * pData)
{
    pData[0] = '0';
    pData[1] = ',';
    return 2;
}

uint8 YX_Func_GetDeviceType(uint8 * pData)
{
    SCI_MEMCPY(pData,"002,",4);
    return 4;
}

uint8 YX_Func_GetNetType(uint8 * pData)
{
    SCI_MEMCPY(pData,"4G,",3);
    return 3;
}

uint8 YX_Func_GetDeviceModel(uint8 * pData)
{
    SCI_MEMCPY(pData,"Z293,",5);
    return 5;
}

uint8 YX_Func_GetDeviceVer(uint8 * pData)
{
    SCI_MEMCPY(pData,"503,",4);
    return 4;
}

uint8 YX_Func_GetDeviceModelVer(uint8 * pData)
{
    SCI_MEMCPY(pData,"Z293|503,",9);
    return 9;
}

uint8 YX_Func_GetFirstLandTime(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    char tem_buf[100] = {0};
    uint16 len = SCI_STRLEN((char *)yx_DB_Set_Rec.first_time); 

    if(len > 0)
    {
        SCI_MEMCPY(pData,yx_DB_Set_Rec.first_time,len);
    }
    else
    {
        TM_GetSysDate(&date);
        TM_GetSysTime(&time);
        
        sprintf(tem_buf,"%04d-%02d-%02d %02d:%02d:%02d",date.year,date.mon,date.mday,time.hour,time.min,time.sec);
        len = SCI_STRLEN(tem_buf);
        SCI_MEMCPY(yx_DB_Set_Rec.first_time,tem_buf,len);
        YX_DB_SET_ListModify();
        SCI_MEMCPY(pData,tem_buf,len);
    }
    
    return len;
}

uint16 YX_Func_GetMD5Str(uint8 * pData)
{
    uint8 in_buf[100] = {0};
    char time_s_buf[30] = {0};
    char out_buf[100] = {0};
    int in_buf_len = 0;
    uint32 time_s = 0;
    uint16 len = 0;
    uint8 digest[16] = {0};
    uint16 i = 0;
    
#ifndef WIN32
    mbedtls_md5_context md5_ctx = {0};
    
    time_s = ZDT_Get_TimeSecond();
    
    sprintf(time_s_buf,"%d",time_s/300);
    
    in_buf_len = YX_Func_GetFirstLandTime(&in_buf);
    strcat(in_buf,time_s_buf);
    in_buf_len = strlen((char *)in_buf);
    mbedtls_md5_init( &md5_ctx );
    mbedtls_md5_starts( &md5_ctx );
    mbedtls_md5_update( &md5_ctx, in_buf, in_buf_len);
    mbedtls_md5_finish( &md5_ctx, digest);
    mbedtls_md5_free( &md5_ctx );
    for(i = 0; i < 16; i++)
    {
        sprintf(out_buf+(i*2),"%02x",digest[i]);
    }
    len = strlen(out_buf);
    SCI_MEMCPY(pData,out_buf,len);
#endif
    return len;
}



uint8 YX_Func_SetRepeatNum(uint8 rpt_num)
{
    g_yx_al_repeat_num = rpt_num;
    return 0;
}

uint8 YX_Func_GetRepeatNum(uint8 * pData)
{
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d%",g_yx_al_repeat_num);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint16 YX_Func_GetSSInfo(uint8 * pData)
{
    char tem_buf[200] = {0};
    char item_buf[100] = {0};
    uint16 len = 0;
    uint16 i = 0;
    uint16 j = 0;
    int nbr_count = 0;

    MMIZDT_Station_Update();
    #if 0
    for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
    {
        if(g_zdt_station_data.n_cell[i].is_valid)
        {
            j++;
        }
    }
    nbr_count = j;
    #endif
    ZDT_LOG("YX_Func_GetSSInfo rssiLevel=%d, mcc=%d,,mnc=%d,lac=%d,cell_id=%d",\
    g_zdt_station_data.s_cell.rssiLevel,g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id);

    if(g_zdt_station_data.s_cell.is_valid)
    {
        sprintf(tem_buf,"%d!%d!%d!%d!%d",\
            g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id,g_zdt_station_data.s_cell.rssiLevel);
        #if 0
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            if(g_zdt_station_data.n_cell[i].is_valid)
            {
                memset(item_buf,0,100);
                sprintf(item_buf,"@%d-%d-%d",g_zdt_station_data.s_cell.lac,g_zdt_station_data.n_cell[i].cell_id,g_zdt_station_data.n_cell[i].rssiLevel);
                strcat(tem_buf,item_buf);
            }
        }
        #endif
    }
    else
    {
        sprintf(tem_buf,"0!0!0!0!0");
    }
    len = strlen(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint16 YX_Func_GetSSInfoExt(uint8 * pData)
{
    char tem_buf[200] = {0};
    char item_buf[100] = {0};
    uint16 len = 0;
    uint16 i = 0;
    uint16 j = 0;
    int nbr_count = 0;

    MMIZDT_Station_Update();
    for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
    {
        if(g_zdt_station_data.n_cell[i].is_valid)
        {
            j++;
        }
    }
    nbr_count = j;
    ZDT_LOG("YX_Func_GetSSInfoExt rssiLevel=%d, mcc=%d,,mnc=%d,lac=%d,cell_id=%d",\
    g_zdt_station_data.s_cell.rssiLevel,g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id);

    if(g_zdt_station_data.s_cell.is_valid)
    {
        sprintf(tem_buf,"cellinfo=%d,0,%d,%d,%d,%d,%d",\
            nbr_count,g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id,g_zdt_station_data.s_cell.rssiLevel);
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            if(g_zdt_station_data.n_cell[i].is_valid)
            {
                memset(item_buf,0,100);
                sprintf(item_buf,",%d,%d,%d",g_zdt_station_data.s_cell.lac,g_zdt_station_data.n_cell[i].cell_id,g_zdt_station_data.n_cell[i].rssiLevel);
                strcat(tem_buf,item_buf);
            }
        }
    }
    else
    {
        sprintf(tem_buf,"cellinfo=1,0,%d,%d,%d,%d,%d",\
            g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id,g_zdt_station_data.s_cell.rssiLevel);
    }
    len = strlen(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}
uint8 YX_Func_GetBatPercent(uint8 * pData)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    uint8 percent = 0;
    char tem_buf[20] = {0};
    uint16 len = 0; 
    if(p_chgmng_info != PNULL)
    {
        percent = p_chgmng_info->bat_remain_cap;
    }
    
    sprintf(tem_buf,"%d%%",percent);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetDateTime(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%04d%02d%02d%02d%02d%02d",date.year,date.mon,date.mday,time.hour,time.min,time.sec);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetNoteMsgID(uint8 * pData,uint16 msg_id)
{
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d",msg_id);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetMemFree(uint8 * pData)
{
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d",ZDT_File_GetMemFree(FALSE));
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetTemper(uint8 * pData)
{
    float temp = 36.2;
    char tem_buf[100] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%1.3f",temp);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetTemper_ex(uint8 * pData,double temp)
{
	char tem_buf[100] = {0};
	uint16 len = 0; 
	sprintf(tem_buf,"%.1lf",temp);
	len = SCI_STRLEN(tem_buf);
	SCI_MEMCPY(pData,tem_buf,len);
	return len;
}

#endif

uint8 YX_Func_GetDate(uint8 * pData)
{
    SCI_DATE_T          date;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    
    sprintf(tem_buf,"%02d%02d%02d,",(date.year%2000),date.mon,date.mday);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetTime(uint8 * pData)
{
    SCI_TIME_T          time;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%02d%02d%02d,",time.hour,time.min,time.sec);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetDateTimeEx(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%02d%02d%02d%02d%02d%02d,",(date.year%2000),date.mon,date.mday,time.hour,time.min,time.sec);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}
uint8 YX_Func_GetTimeDateTime(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    YX_Func_IntToHexChar_One((uint8)time.hour,pData);
    YX_Func_IntToHexChar_One((uint8)time.min,pData+2);
    YX_Func_IntToHexChar_One((uint8)(time.sec),pData+4);
    pData[6] = ',';
    
    YX_Func_IntToHexChar_One((uint8)date.mday,pData+7);
    YX_Func_IntToHexChar_One((uint8)date.mon,pData+9);
    YX_Func_IntToHexChar_One((uint8)(date.year%2000),pData+11);
    pData[13] = ',';
    
    YX_Func_IntToHexChar_One((uint8)time.hour,pData+14);
    YX_Func_IntToHexChar_One((uint8)time.min,pData+16);
    YX_Func_IntToHexChar_One((uint8)time.sec,pData+18);
    pData[20] = ',';
    return 21;
}

uint8 YX_Func_GetRssiPercent(uint8 * pData)
{
    uint8	i = 5;
    uint16 rssi = g_zdt_s_station_data.rssiLevel;
    uint8 percent = 0;
    char tem_buf[20] = {0};
    uint16 len = 0; 
    //设置信强度,根据参考手机sagem得到的数据，将idle界面下的信号强度划分成5级
    // 1：1-7； 2：8-11； 3：12-15； 4：16-19； 5：20-63
    if (20 <= rssi)
    {
        i = 5;
    }
    else if (16 <= rssi)
    {
        i = 4;
    }
    else if (12 <= rssi)
    {
        i = 3;
    }
    else if (8 <= rssi)
    {
        i = 2;
    }
    else if (1 <= rssi)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    percent = i *20;
    if(percent > 100)
    {
        percent = 100;
    }
    sprintf(tem_buf,"%d",percent);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetJPNum(uint8 * pData)
{
    uint32 jp_sum = 0;
    uint32 one_today_jp_sum = 0;
    char tem_buf[20] = {0};
    uint16 len = 0; 
#ifdef ZDT_GSENSOR_SUPPORT
    jp_sum = ZDT_GSensor_GetStep();
    #ifndef WIN32
    one_today_jp_sum=ZDT_GSensor_GetStepOneDay();
    #endif
#endif
    sprintf(tem_buf,"@%d#%d",jp_sum,one_today_jp_sum);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetFGNum(uint8 * pData)
{
    uint32 fg_sum = 0;
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d",fg_sum);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetStatus(uint8 * pData,uint16 type)
{
    char tmp_buf[20] = {0};
    uint16 alert = 0;
    uint16 status = 0;
    if(ZDT_GetIsBatLow())
    {
        status |= 0x0001;
    }
    #ifdef FEATURE_ALSENSOR_SUPPORT
    if(alsensor_is_far())
    {
        status |= 0x0008;
    }
    #endif
    #ifdef ZDT_GSENSOR_SUPPORT
    if(ZDT_GSensor_GetStill())
    {
        status |= 0x0010;
    }
    #endif
    if(type == YX_NET_ALERT_TYPE_SOS)
    {
        alert |= 0x0001;
    }
    if(type == YX_NET_ALERT_TYPE_LOWBAT)
    {
        alert |= 0x0002;
    }
    if(type == YX_NET_ALERT_TYPE_LOSE)
    {
        alert |= 0x0010;
    }
    if(type == YX_NET_ALERT_TYPE_FALLDOWN)
    {
        alert |= 0x0020;
    }
    sprintf(tmp_buf,"%04X%04X,",alert,status);
    SCI_MEMCPY(pData,tmp_buf,9);
    return 9;
}

uint16 YX_Func_GetWIFIInfo(uint8 * pData,uint8 loc_ok)
{
    uint16 len = 0;
#ifdef ZDT_WIFI_SUPPORT
    uint8 wifi_num = 0;
    ZDT_WIFI_DATA_T last_wifi = {0};
    char temp_buf[500] = {0};
    char item_buf[100] = {0};
    uint16 i =0;
    //WIFI
    if(loc_ok & 0x02)
    {
        wifi_num = ZDT_WIFI_Get_DataNum();
        if(wifi_num > 5)
        {
            wifi_num = 5;
        }
    }
    else
    {
        wifi_num = 0;
    }
    if(wifi_num > 0)
    {
        for(i = 0; i < wifi_num; i++)
        {
            ZDT_WIFI_Get_Data(i,&last_wifi);
            if(last_wifi.rssi < 0)
            {
                last_wifi.rssi = 0 - last_wifi.rssi;
            }
            if(i == 0)
            {
                sprintf(temp_buf,"%s!%02x:%02x:%02x:%02x:%02x:%02x!%d#",last_wifi.ssid,last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5],last_wifi.rssi);
            }
            else
            {
                SCI_MEMSET(item_buf,0,100);
                sprintf(item_buf,"%s!%02x:%02x:%02x:%02x:%02x:%02x!%d#",last_wifi.ssid,last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5],last_wifi.rssi);
                strcat(temp_buf,item_buf);
            }
        }
        len = strlen(temp_buf);
        SCI_MEMCPY(pData,temp_buf,len);
    }
    else
#endif
    {
        len = 0;
    }

    return len;
}

uint16 YX_Func_GetWIFIInfoExt(uint8 * pData,uint8 loc_ok)
{
    uint16 len = 0;
#ifdef ZDT_WIFI_SUPPORT
    uint8 wifi_num = 0;
    ZDT_WIFI_DATA_T last_wifi = {0};
    char temp_buf[500] = {0};
    char item_buf[100] = {0};
    uint16 i =0;
    //WIFI
    if(loc_ok & 0x02)
    {
        wifi_num = ZDT_WIFI_Get_DataNum();
        if(wifi_num > 5)
        {
            wifi_num = 5;
        }
    }
    else
    {
        wifi_num = 0;
    }
    if(wifi_num > 0)
    {
        for(i = 0; i < wifi_num; i++)
        {
            ZDT_WIFI_Get_Data(i,&last_wifi);
            if(last_wifi.rssi < 0)
            {
                last_wifi.rssi = 0 - last_wifi.rssi;
            }
            if(i == 0)
            {
                SCI_MEMSET(item_buf,0,100);
                sprintf(item_buf,"wifiinfo=%d,,%02x:%02x:%02x:%02x:%02x:%02x,%d",wifi_num,last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5],last_wifi.rssi);
                strcat(temp_buf,item_buf);
            }
            else
            {
                SCI_MEMSET(item_buf,0,100);
                sprintf(item_buf,",,%02x:%02x:%02x:%02x:%02x:%02x,%d",last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5],last_wifi.rssi);
                strcat(temp_buf,item_buf);
            }
        }
        len = strlen(temp_buf);
        SCI_MEMCPY(pData,temp_buf,len);
    }
    else
#endif
    {
        len = 0;
    }

    return len;
}

uint8 YX_Func_GetGPSLat(uint8 * pData,uint32 Latitude,uint32 Lat_m,uint8 Lat_Dir)
{
    char tem_buf[50] = {0};
    uint16 len = 0; 
    uint32 d = 0;
    uint32 d_m = 0;
    double tmp = Lat_m/0.6;
    d_m = tmp;
    d = Latitude/180000;
    sprintf((char *)tem_buf,"%C0%02ld.%06ld",Lat_Dir,d,d_m);
    len = strlen(tem_buf);
    ZDT_LOG("YX_Func_GetGPSLat %s",tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetGPSLong(uint8 * pData,uint32 Longitude,uint8 long_Dir)
{
    char tem_buf[50] = {0};
    uint16 len = 0; 
    double tmp = 0;
    uint32 d = 0;
    uint32 d_m = 0;
    d = Longitude/180000;
    tmp = (Longitude - d*180000)/0.18;
    d_m = tmp;
    sprintf((char *)tem_buf,"%C%03ld.%06ld",long_Dir,d,d_m);
    len = strlen(tem_buf);
    ZDT_LOG("YX_Func_GetGPSLong %s",tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}

uint8 YX_Func_GetGPSSateNum(uint8 * pData,uint8 sate_num)
{
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d",sate_num);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetGPSSpeed(uint8 * pData,uint32 speed)
{
    char tem_buf[20] = {0};
    uint32 sp_kilo = 0;
    uint32 sp_kilo_d = 0;
    uint16 len = 0; 
    sp_kilo = speed/1000;
    sp_kilo_d = (speed - sp_kilo*1000)/10;
    sprintf(tem_buf,"%ld.%02ld",sp_kilo,sp_kilo_d);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetGPSCog(uint8 * pData,uint16 cog)
{
    char tem_buf[20] = {0};
    uint16 cur_cog = cog/10;
    uint16 len = 0; 
    sprintf(tem_buf,"%d",cur_cog);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 YX_Func_GetGPSAlt(uint8 * pData,uint32 alt, uint8 alt_dir)
{
    char tem_buf[20] = {0};
    uint32 cur_alt = alt/10;
    uint16 len = 0;
    if(alt_dir > 0)
    {
        sprintf(tem_buf,"%d",cur_alt);
    }
    else
    {
        sprintf(tem_buf,"-%d",cur_alt);
    }
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint16 YX_Func_GetGPSTime(uint8 * pData,uint32 gps_date,uint32 gps_time)
{
    uint16 yy = 0;
    uint8 mm = 0;
    uint8 dd = 0;
    uint8 hour = 0;
    uint8 min = 0;
    uint8 sec = 0;
    uint32 date = 0;
    uint32 time = 0;
    char time_buf[100] = {0};
    uint16 time_len = 0;
    MMI_TM_T sys_time = {0};
    uint32   second = 0;

    date = gps_date;
    time = gps_time;
    yy = date >> 16;
    yy += 2000;
    mm = (date&0x0000FF00) >> 8;
    dd = (date&0x000000FF);
    hour = time/3600000;
    min = (time%3600000)/60000;
    sec = (time%60000)/1000;
    
    second = MMIAPICOM_Tm2Second(sec, min, hour, dd, mm, yy);
    second += 28800; 
    sys_time = MMIAPICOM_Second2Tm(second);
    sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d",sys_time.tm_year,sys_time.tm_mon,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);
    time_len = strlen(time_buf);
    SCI_MEMCPY(pData,time_buf,time_len);

    return time_len;
}

uint16 YX_Func_GetGPSInfo(uint8 * pData,uint8 loc_ok)
{
    uint16 len = 0;
    uint16 yy = 0;
    uint8 mm = 0;
    uint8 dd = 0;
    uint8 hour = 0;
    uint8 min = 0;
    uint8 sec = 0;
    uint32 date = 0;
    uint32 time = 0;
#ifdef ZDT_GPS_SUPPORT
    uint8 gps_num = 0;
    GPS_DATA_T last_gps= {0};
    char temp_buf[500] = {0};
    char time_buf[100] = {0};
    uint16 time_len = 0;
    uint16 i =0;
    //GPS
    if(loc_ok & 0x01)
    {
        GPS_API_GetLastPos(&last_gps);
        temp_buf[len++] = '0';
        len += YX_Func_GetGPSLong(temp_buf+len,last_gps.Longitude,last_gps.Long_Dir);
        len += YX_Func_GetGPSLat(temp_buf+len,last_gps.Latitude,last_gps.Lat_m,last_gps.Lat_Dir);
        
        date = last_gps.date;
        time = last_gps.time;
        yy = date >> 16;
        yy += 2000;
        mm = (date&0x0000FF00) >> 8;
        dd = (date&0x000000FF);
        hour = time/3600000;
        min = (time%3600000)/60000;
        sec = (time%60000)/1000;
        sprintf(time_buf,"T%04d%02d%02d%02d%02d%02d",yy,mm,dd,hour,min,sec);
        time_len = strlen(time_buf);
        SCI_MEMCPY(temp_buf+len,time_buf,time_len);
        len += time_len;
        SCI_MEMCPY(pData,temp_buf,len);
    }
    else
    {
        SCI_TM_T sys_time = {0};
        
        temp_buf[len++] = '1';
        SCI_MEMCPY(temp_buf+len,"E000.000000N000.000000",strlen("E000.000000N000.000000"));
        len += strlen("E000.000000N000.000000");
        
        TM_GetTime(&sys_time);
        sprintf(time_buf,"T%04d%02d%02d%02d%02d%02d",sys_time.tm_year,sys_time.tm_mon,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);
        time_len = strlen(time_buf);
        SCI_MEMCPY(temp_buf+len,time_buf,time_len);
        len += time_len;
        SCI_MEMCPY(pData,temp_buf,len);
    }
#else
    {
        SCI_TM_T sys_time = {0};
        temp_buf[len++] = '1';
        SCI_MEMCPY(temp_buf+len,"E0.000000N0.000000",strlen("E0.000000N0.000000"));
        len += strlen("E0.000000N0.000000");
        
        TM_GetTime(&sys_time);
        sprintf(time_buf,"T%04d%02d%02d%02d%02d%02d",sys_time.tm_year,sys_time.tm_mon,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);
        time_len = strlen(time_buf);
        SCI_MEMCPY(temp_buf+len,time_buf,time_buf,time_len);
        len += time_len;
        SCI_MEMCPY(pData,temp_buf,len);
    }
#endif
    return len;
}

uint16 YX_Func_GetGPSInfoExt(uint8 * pData,double latitude,double longitude)
{
    uint16 len = 0;
#ifdef ZDT_GPS_SUPPORT
    uint8 gps_num = 0;
    char temp_buf[500] = {0};
    char lat_buf[100] = {0};
    char long_buf[100] = {0};
    char time_buf[100] = {0};
    uint16 time_len = 0;
    uint16 i =0;
    //GPS
    SCI_TM_T sys_time = {0};
    
    temp_buf[len++] = '0';
    if(longitude > 0)
    {
        temp_buf[len++] = 'E';
    }
    else
    {
        temp_buf[len++] = 'W';
        longitude = 0 - longitude;
    }
    sprintf(long_buf,"%010lf",longitude);
    SCI_MEMCPY(temp_buf+len,long_buf,10);
    len += 10;
    if(latitude > 0)
    {
        temp_buf[len++] = 'N';
    }
    else
    {
        temp_buf[len++] = 'S';
        latitude = 0 - latitude;
    }
    sprintf(lat_buf,"%010lf",latitude);
    SCI_MEMCPY(temp_buf+len,lat_buf,10);
    len += 10;
    
    TM_GetTime(&sys_time);
    sprintf(time_buf,"T%04d%02d%02d%02d%02d%02d",sys_time.tm_year,sys_time.tm_mon,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);
    time_len = strlen(time_buf);
    SCI_MEMCPY(temp_buf+len,time_buf,time_len);
    len += time_len;
    SCI_MEMCPY(pData,temp_buf,len);
#else
    {
        SCI_TM_T sys_time = {0};
        temp_buf[len++] = '1';
        SCI_MEMCPY(temp_buf+len,"E000.000000N000.000000",strlen("E000.000000N000.000000"));
        len += strlen("E000.000000N000.000000");
        
        TM_GetTime(&sys_time);
        sprintf(time_buf,"T%04d%02d%02d%02d%02d%02d",sys_time.tm_year,sys_time.tm_mon,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);
        time_len = strlen(time_buf);
        SCI_MEMCPY(temp_buf+len,time_buf,time_buf,time_len);
        len += time_len;
        SCI_MEMCPY(pData,temp_buf,len);
    }
#endif
    return len;
}
// 字符全部转换为大写
char* YX_Func_strupr(char* src)
{
	while (*src != '\0')
	{
		if (*src >= 'a' && *src <= 'z')
			//在ASCII表里大写字符的值比对应小写字符的值小32.
			//*p -= 0x20; // 0x20的十进制就是32
			*src -= 32;
		src++;
	}
	return src;
}

// 字符全部转换为小写
char* YX_Func_strlwr(char* src)
{
	while (*src != '\0')
	{
		if (*src > 'A' && *src <= 'Z'){
			//*src += 0x20; 
			*src += 32;
		}
		src++;
	}
	return src;
}

uint16 YX_Func_GetFileName(char *pFullPath,char *pFileName)
{
    uint16 len = 0;
    char* p;
    
    if(pFullPath == NULL || pFileName == NULL)
    {
        return 0;
    }
    
    len = SCI_STRLEN(pFullPath);
    if(len == 0)
    {
        return 0;
    }
    
    
    p = strrchr(pFullPath, '\\');
    if(p == NULL)
    {
        return 0;
    }
    SCI_STRCPY(pFileName,p+1);
    len = strlen(pFileName);
    return len;
}

uint32 YX_Func_StrTimeToInt(char *time_str,uint32 time_len)
{
    int ret = 0;
    uint32 u_time = 0;
    uint16 clock_hour = 0;
    uint8 clock_min = 0;
    uint8 clock_sec = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[10] = {0};
    
    if(time_str == NULL || time_len == 0)
    {
        return 0;
    }
    
    pTime = (uint8 *)time_str;
    con_len = time_len;
    ret = YX_Func_ClockGetNextPara(&pTime,&con_len,tmp_str,6);
    if(ret > 0)
    {
        clock_hour = atoi(tmp_str);
    }
    ret = YX_Func_ClockGetNextPara(&pTime,&con_len,tmp_str,4);
    if(ret > 0)
    {
        clock_min = atoi(tmp_str);
    }
    
    ret = YX_Func_ClockGetNextPara(&pTime,&con_len,tmp_str,4);
    if(ret > 0)
    {
        clock_sec = atoi(tmp_str);
    }
    u_time = (clock_hour* 3600) + (clock_min* 60) + clock_sec;
    return u_time;
}

uint32 YX_Func_StrDateToInt(char *date_str,uint32 date_len)
{
    int ret = 0;
    uint32 u_date = 0;
    uint16 clock_y = 0;
    uint8 clock_m = 0;
    uint8 clock_d = 0;
    uint8 * pTime = NULL;
    uint32 con_len = 0;
    char tmp_str[10] = {0};
    
    if(date_str == NULL || date_len == 0)
    {
        return 0;
    }
    
    pTime = (uint8 *)date_str;
    con_len = date_len;
    ret = YX_Func_ClockGetNextPara(&pTime,&con_len,tmp_str,6);
    if(ret > 0)
    {
        clock_y = atoi(tmp_str);
    }
    ret = YX_Func_ClockGetNextPara(&pTime,&con_len,tmp_str,4);
    if(ret > 0)
    {
        clock_m = atoi(tmp_str);
    }
    
    ret = YX_Func_ClockGetNextPara(&pTime,&con_len,tmp_str,4);
    if(ret > 0)
    {
        clock_d = atoi(tmp_str);
    }
    u_date = clock_y;
    u_date = u_date << 8;
    u_date += clock_m;
    u_date = u_date << 8;
    u_date += clock_d;
    return u_date;
}


/* 原理: 将测试点的Y坐标与多边形的每一个点进行比较，
**		 会得到测试点所在的行与多边形边的所有交点。
**		 如果测试点的两边点的个数都是奇数个，
**		 则该测试点在多边形内，否则在多边形外。 */
  
/* 函数功能: 判断点(x, y)是否在有ploy_sides个顶点的多边形内 */
/* 参数: poly_sides	测试多边形的顶点数
**		 poly_x	测试多边形的各个顶点的X轴坐标
**		 poly_y	测试多边形的各个顶点的Y轴坐标
**		 x	测试点的X轴坐标
**		 Y 	测试点的Y轴坐标 */
/* 返回值: 返回0 表示不在多边形内部，返回1 表示在多边形内部 */
/* 说明: 在多边形各边上的点默认不在多边形内部 */
static int inOrNot(int poly_sides, double *poly_X, double *poly_Y, double x, double y) 
{
    int i, j;
    int res = 0;
    j = poly_sides-1;

    for (i = 0; i<poly_sides; i++) 
    {
        if((poly_Y[i]<y && poly_Y[j]>=y || poly_Y[j]<y && poly_Y[i]>=y) && (poly_X[i]<=x || poly_X[j]<=x))
        {
            res ^= ((poly_X[i] + (y-poly_Y[i])/(poly_Y[j]-poly_Y[i])*(poly_X[j]-poly_X[i])) < x);
        }
        j=i;
    }
 
    return res; 
}
static double getDistance(double longitude1, double latitude1, double longitude2, double latitude2)
{
    double a = 0;
    double b = 0;
    double distance = 0;
    double EARTH_RADIUS = 6371.004; // 地球半径系数6370.996
    double PI = 3.14159265358979323846;
    double radLat1 = latitude1 * PI / 180.0;
    double radLat2 = latitude2 * PI / 180.0;

    double radLng1 = longitude1 * PI / 180.0;
    double radLng2 = longitude2 * PI /180.0;
 
    a = radLat1 - radLat2;
    b = radLng1 - radLng2;
 
  distance = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1) * cos(radLat2) * pow(sin(b/2),2)));
  distance = distance * EARTH_RADIUS * 1000;
 
  return distance;
 
}
 
BOOLEAN YX_Func_Regional_IsInPoly(double x, double y, int poly_sides,double *poly_X,double *poly_Y)
{
    BOOLEAN res = FALSE;
    char tmp_buf[200] = {0};
    int ret;
    ret = inOrNot(poly_sides, poly_X, poly_Y, x, y);
    if(1 == ret)
    {
        res = TRUE;
    }
    sprintf(tmp_buf,"Old long:%lf, lati:%lf ,ret=%d",x,y,ret);
    SCI_TRACE_LOW("YX_Func_Regional_IsInPoly(%d): %s",res,tmp_buf);
    return res;
}

static double msqrt( double n, double* ret )   
{
	double xn0,xn1,diff;
	xn0=n/2;   
	xn1=(xn0+n/xn0)/2;
	do   
	{   
		xn0=xn1;   
		xn1=(xn0+n/xn0)/2;
		diff = xn0-xn1;
		if( diff < 0 )
			diff = -diff;
	}   
	while(diff>=1e-5);
	*ret = xn1;
        
	return xn1;
}

// 参数double x 为角度, 计算时应换算为弧度
// 换算为弧度公式: 360 = 2PI
// y = 2PI*x/360 = PI*x/180 = 0.01745329251994330f *x
static double mcos(double x, double* ret)
{
	double j,f,c,c0;
	int32 i, h;
	double diff;

	c=1;c0=0;f=1;
	h=1;i=0;j=1;
	//x = 0.01745329251994330f * x;
	do
	{
		c0=c;
		f=f*x*x;
		j=j*(i+1)*(i+2);
		i=i+2;
		h=-h;
		c=c+h*f/j;
		diff = c - c0;
		if( diff <0 )
			diff = -diff;
	}while(diff>1e-6);
	*ret = c;
	return c;
}

uint32 g_yx_wl_distance = 0;
const double yx_pi = 3.14159265358979324;
const double yx_a = 6378245.0;
const double yx_ee = 0.00669342162296594323;
const double yx_x_pi = 3.14159265358979324 * 3000.0 / 180.0;
 
#define port_abs(x)     ((x > 0) ? (x) : ((x) * -1))
 
BOOLEAN outOfChina(double lat, double lon)
{
    if (lon < 72.004 || lon > 137.8347)
        return TRUE;
    if (lat < 0.8293 || lat > 55.8271)
        return TRUE;
    return FALSE;
}
 
double transformLat(double x, double y)
{
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(port_abs(x));
    ret += (20.0 * sin(6.0 * x * yx_pi) + 20.0 * sin(2.0 * x * yx_pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(y * yx_pi) + 40.0 * sin(y / 3.0 * yx_pi)) * 2.0 / 3.0;
    ret += (160.0 * sin(y / 12.0 * yx_pi) + 320 * sin(y * yx_pi / 30.0)) * 2.0 / 3.0;
    return ret;
}
 
double transformLon(double x, double y)
{
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(port_abs(x));
    ret += (20.0 * sin(6.0 * x * yx_pi) + 20.0 * sin(2.0 * x * yx_pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(x * yx_pi) + 40.0 * sin(x / 3.0 * yx_pi)) * 2.0 / 3.0;
    ret += (150.0 * sin(x / 12.0 * yx_pi) + 300.0 * sin(x / 30.0 * yx_pi)) * 2.0 / 3.0;
    return ret;
}
 
/* 地球坐标转换为火星坐标，即WGS84（国际通用）转为GCJ02坐标系适用于腾讯地图、高德（阿里）地图或谷歌地图 */
void WGS84toGCJ02(double wgLat, double wgLon, double *gcjLat, double *gcjLon)
{
    double dLat = 0;
    double dLon = 0;
    double radLat = 0;
    double magic = 0;
    double sqrtMagic = 0;
    if (outOfChina(wgLat, wgLon)) {
        *gcjLat = wgLat;
        *gcjLon = wgLon;
        return;
    }
    dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
    dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
    radLat = wgLat / 180.0 * yx_pi;
    magic = sin(radLat);
    magic = 1 - yx_ee * magic * magic;
    sqrtMagic = sqrt(magic);
    dLat = (dLat * 180.0) / ((yx_a * (1 - yx_ee)) / (magic * sqrtMagic) * yx_pi);
    dLon = (dLon * 180.0) / (yx_a / sqrtMagic * cos(radLat) * yx_pi);
    *gcjLat = wgLat + dLat;
    *gcjLon = wgLon + dLon;
}
 
/* 地球坐标转换为百度坐标，即WGS84（国际通用）坐标系转为BD09坐标系适用于百度地图 */
void WGS84toBD09(double wgLat, double wgLon, double *bdLat, double *bdLon)
{
    double z = 0;
    double theta = 0;
    double gcjLat, gcjLon;
    WGS84toGCJ02(wgLat, wgLon, &gcjLat, &gcjLon);
 
    z = sqrt(gcjLon * gcjLon + gcjLat * gcjLat) + 0.00002 * sin(gcjLat * yx_x_pi);
    theta = atan2(gcjLat, gcjLon) + 0.000003 * cos(gcjLon * yx_x_pi);
    *bdLat = z * sin(theta) + 0.006;    // #0.006     #0.01205
    *bdLon = z * cos(theta) + 0.0062;   // #0.0065    #0.00370
}
 
/* 火星坐标转换为百度坐标 */
void GCJ02toBD09(double gcjLat, double gcjLon, double *bdLat, double *bdLon)
{
    double x = gcjLon, y = gcjLat;
    double z = sqrt(x * x + y * y) + 0.00002 * sin(y * yx_x_pi);
    double theta = atan2(y, x) + 0.000003 * cos(x * yx_x_pi);
    *bdLon = z * cos(theta) + 0.0065;
    *bdLat = z * sin(theta) + 0.006;
}
 
/* 百度转火星 */
void BD09toGCJ02(double bdLat, double bdLon, double *gcjLat, double *gcjLon)
{
    double x = bdLon - 0.0065, y = bdLat - 0.006;
    double z = sqrt(x * x + y * y) - 0.00002 * sin(y * yx_x_pi);
    double theta = atan2(y, x) - 0.000003 * cos(x * yx_x_pi);
    *gcjLon = z * cos(theta);
    *gcjLat = z * sin(theta);
}
 

#if 0
/*
GS84坐标系	121.469522,31.231905
GCJ02坐标系 121.47405857079794,31.229974621114046
BD09坐标系  121.48055338931275,31.235941661557085
*/
int main(void)
{
    double wgsLat = 31.231905;
    double wgsLon = 121.469522;
 
    double gcjLat = 0, gcjLon = 0;
    double bdLat = 0, bdLon = 0;
 
    WGS84toGCJ02(wgsLat, wgsLon, &gcjLat, &gcjLon);
    printf("WGS84toGCJ02:%lf-%lf >>> %lf-%lf\n", wgsLon, wgsLat, gcjLon, gcjLat);
 
    WGS84toBD09(wgsLat, wgsLon, &bdLat, &bdLon);
    printf("WGS84toBD09:%lf-%lf >>> %lf-%lf\n", wgsLon, wgsLat, bdLon, bdLat);
 
    GCJ02toBD09(gcjLat, gcjLon, &bdLat, &bdLon);
    printf("GCJ02toBD09:%lf-%lf >>> %lf-%lf\n", gcjLon, gcjLat, bdLon, bdLat);
 
    BD09toGCJ02(bdLat, bdLon, &gcjLat, &gcjLon);
    printf("BD09toGCJ02:%lf-%lf >>> %lf-%lf\n", bdLon, bdLat, gcjLon, gcjLat);
 
    return 0;
}
#endif

BOOLEAN YX_Func_Regional_IsInRound(double src_longitude, double src_latitude, uint16 radius,double new_longitude,double new_latitude)
{
    double diff_longitude=0;
    double diff_latitude=0;
    double new_distance = 0;
    uint32 distance;
    double long_z = 0;
    double lat_z = 0;
    double dTmp = 0;
    char tmp_buf[200] = {0};
    BOOLEAN res = FALSE;
    if(src_longitude == new_longitude
        && src_latitude== new_latitude)
    {
        new_distance = 1;
    }
    else
    {
        #if 1
            new_distance = getDistance(src_longitude,src_latitude,new_longitude,new_latitude);
        #else
            diff_longitude = new_longitude - src_longitude;
            mcos(new_latitude/58, &long_z);
            long_z = long_z*diff_longitude*111303;
            diff_latitude = new_latitude - src_latitude;
            lat_z = diff_latitude*110946;
            msqrt(long_z*long_z + lat_z*lat_z, &new_distance);
        #endif
    }

    dTmp = radius;
    if(new_distance <= dTmp)
    {
        res = TRUE;  
    }
    distance = new_distance;
    g_yx_wl_distance = distance;
    if(ZDT_Factory_IsTesting_WL())
    {
        YX_API_PlayDistance(distance);
    }
    sprintf(tmp_buf,"Old long:%lf, lati:%lf ,new long:%lf, lati:%lf , new_distance=%lf",src_longitude,src_latitude, new_longitude, new_latitude,new_distance);
    SCI_TRACE_LOW("YX_Func_Regional_IsInRound(%d): %s",res,tmp_buf);
    return res;
}
#endif
