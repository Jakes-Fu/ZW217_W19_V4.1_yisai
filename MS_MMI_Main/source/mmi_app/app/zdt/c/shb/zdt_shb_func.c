#include "zdt_app.h"
#ifndef WIN32
#include "mbedtls/pem.h"
#include "mbedtls/base64.h"
#include "mbedtls/des.h"
#include "mbedtls/aes.h"
#include "mbedtls/md5.h"
#include "mbedtls/cipher.h"
#endif

#ifdef ZDT_PLAT_SHB_SUPPORT
void SHB_Func_MD5_Generate(uint8 * input, unsigned int input_len,char out_str[32])
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


static uint8 * SHB_Func_AES_Encrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
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

    SCI_MEMCPY(iv_val,SHB_AES_IV_STR,strlen(SHB_AES_IV_STR));
    SCI_MEMCPY(key_val,SHB_AES_KEY_STR,strlen(SHB_AES_KEY_STR));
    ZDT_LOG("iv_val=%s,key_val=%s",iv_val,key_val);
    
    mbedtls_aes_setkey_enc( &aes_ctx, key_val, 128);

    mbedtls_aes_crypt_cbc( &aes_ctx, MBEDTLS_AES_ENCRYPT, encrypt_data_len, iv_val, encrypt_data, encrypt_data);

    mbedtls_aes_free( &aes_ctx );
#endif
    return encrypt_data;
}

static uint8 * SHB_Func_AES_Decrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
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
    mbedtls_aes_init( &aes_ctx );

    SCI_MEMCPY(iv_val,SHB_AES_IV_STR,strlen(SHB_AES_IV_STR));
    SCI_MEMCPY(key_val,SHB_AES_KEY_STR,strlen(SHB_AES_KEY_STR));
    ZDT_LOG("iv_val=%s,key_val=%s",iv_val,key_val);

    mbedtls_aes_setkey_dec( &aes_ctx, key_val, 128);

    mbedtls_aes_crypt_cbc( &aes_ctx, MBEDTLS_AES_DECRYPT, encrypt_data_len, iv_val, encrypt_data, encrypt_data);

    mbedtls_aes_free( &aes_ctx );
#endif
    return encrypt_data;
}

uint8 * SHB_Func_Encrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
{
    uint8 * aes_data = PNULL;
    uint32  aes_len = 0;
    uint32  base64_max_len = 0;
    uint32  base64_rel_len = 0;
    uint8 * encrypt_data = PNULL;
    ZDT_CryptTrace(data,data_len);
    aes_data = SHB_Func_AES_Encrypt(data,data_len,&aes_len);
    if(aes_data == PNULL)
    {
        return PNULL;
    }
    ZDT_HexTraceU8(aes_data,aes_len);
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

uint8 * SHB_Func_Decrypt(uint8 *data, uint32 data_len,uint32 * pOutlen)
{
    uint8 * aes_data = PNULL;
    uint32  aes_len = 0;
    uint32  base64_max_len = 0;
    uint32  base64_rel_len = 0;
    uint8 * decrypt_data = PNULL;
    ZDT_CryptTrace(data,data_len);
    base64_max_len = data_len;
    decrypt_data = SCI_ALLOC_APPZ(base64_max_len);
    if (decrypt_data  == NULL)
    {
        return PNULL;
    }
#ifndef WIN32
    mbedtls_base64_decode(decrypt_data, base64_max_len, &base64_rel_len, data, data_len);
#endif
    ZDT_HexTraceU8(decrypt_data,base64_rel_len);
    aes_data = SHB_Func_AES_Decrypt(decrypt_data,base64_rel_len,&aes_len);
    if(aes_data == PNULL)
    {
        SCI_FREE(decrypt_data);
        return PNULL;
    }
    ZDT_CryptTrace(aes_data,aes_len);
    *pOutlen = aes_len;
    SCI_FREE(decrypt_data);
    return aes_data;
}

uint8 SHB_Func_HexCharToInt(uint8 hexchar)
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

uint8 SHB_Func_IntToHexChar(uint8 bNibble)
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

uint8 SHB_Func_IntToHexChar_One(uint8 data,uint8 * dest_buf)
{
    uint8 low = 0;
    uint8 high = 0;
    high = data>>4;
    dest_buf[0] = (char)SHB_Func_IntToHexChar(high);
    low = data & 0x0F;
    dest_buf[1] = (char)SHB_Func_IntToHexChar(low);
    return 2;
}

int  SHB_Func_GetCmdPosPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos)
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
            if(*(*pCmd + i) == '*')
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
        if(*(*pCmd + i) == '*')
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

int  SHB_Func_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == '|')
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

int  SHB_Func_GetIPNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
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

int  SHB_Func_GetSubNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ',')
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
int  SHB_Func_GetPosPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos)
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

uint32 SHB_Func_IntToHexChar_Array(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
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
        dest_buf[j++] = (char)SHB_Func_IntToHexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)SHB_Func_IntToHexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

int  SHB_Func_ClockGetNextPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen)
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

int  SHB_Func_TimerGetNextPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen)
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
uint8 SHB_Func_GetDate(uint8 * pData)
{
    SCI_DATE_T          date;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    
    sprintf(tem_buf,"%04d%02d%02d,",date.year,date.mon,date.mday);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len] = 0;
    return len;
}

uint8 SHB_Func_GetTime(uint8 * pData)
{
    SCI_TIME_T          time;
    char tem_buf[50] = {0};
    uint16 len = 0; 

    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%02d%02d%02d",time.hour,time.min,time.sec);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len] = 0;
    return len;
}

uint8 SHB_Func_GetDateTime(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    char tem_buf[100] = {0};
    uint16 len = 0; 

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    sprintf(tem_buf,"%04d%02d%02d%02d%02d%02d",date.year,date.mon,date.mday,time.hour,time.min,time.sec);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len] = 0;
    return len;
}

uint32 SHB_Func_StrToDateTime(uint8 * pData)
{
    uint32   second = 0;
    char year[5] = {0};
    char mon[3] = {0};
    char day[3] = {0};
    char hour[3] = {0};
    char min[3] = {0};
    char sec[3] = {0};
    uint32 tm_year = 0;
    uint32 tm_mon = 0;
    uint32 tm_mday = 0;
    uint32 tm_hour = 0;
    uint32 tm_min = 0;
    uint32 tm_sec = 0;
    
    SCI_MEMCPY(year,&pData[0],4);
    SCI_MEMCPY(mon,&pData[4],2);
    SCI_MEMCPY(day,&pData[6],2);
    SCI_MEMCPY(hour,&pData[8],2);
    SCI_MEMCPY(min,&pData[10],2);
    SCI_MEMCPY(sec,&pData[12],2);

    tm_year = atoi(year);
    tm_mon = atoi(mon);
    tm_mday = atoi(day);
    tm_hour = atoi(hour);
    tm_min = atoi(min);
    tm_sec = atoi(sec);
    
    second = MMIAPICOM_Tm2Second(tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year);
    return second;
}

uint8 SHB_Func_GetTimeDateTime(uint8 * pData)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    SHB_Func_IntToHexChar_One((uint8)time.hour,pData);
    SHB_Func_IntToHexChar_One((uint8)time.min,pData+2);
    SHB_Func_IntToHexChar_One((uint8)(time.sec),pData+4);
    pData[6] = ',';
    
    SHB_Func_IntToHexChar_One((uint8)date.mday,pData+7);
    SHB_Func_IntToHexChar_One((uint8)date.mon,pData+9);
    SHB_Func_IntToHexChar_One((uint8)(date.year%2000),pData+11);
    pData[13] = ',';
    
    SHB_Func_IntToHexChar_One((uint8)time.hour,pData+14);
    SHB_Func_IntToHexChar_One((uint8)time.min,pData+16);
    SHB_Func_IntToHexChar_One((uint8)time.sec,pData+18);
    pData[20] = ',';
    return 21;
}

uint8 SHB_Func_GetBatPercent(uint8 * pData)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    uint8 percent = 20;
    char tem_buf[100] = {0};
    uint16 len = 0; 
    if(p_chgmng_info != PNULL)
    {
        percent = p_chgmng_info->bat_remain_cap;
    }
    sprintf(tem_buf,"%d",percent);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len] = 0;
    return len;
}


uint8 SHB_Func_GetRssiPercent(uint8 * pData)
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

uint8 SHB_Func_GetJPNum(uint8 * pData)
{
    uint32 jp_sum = 0;
    char tem_buf[20] = {0};
    uint16 len = 0; 
#ifdef ZDT_GSENSOR_SUPPORT
    jp_sum = ZDT_GSensor_GetStep();
#endif
    sprintf(tem_buf,"%d",jp_sum);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 SHB_Func_GetFGNum(uint8 * pData)
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

uint8 SHB_Func_GetStatus(uint8 * pData,uint16 type)
{
    char tmp_buf[20] = {0};
    uint16 alert = 0;
    uint16 status = 0;
    if(ZDT_GetIsBatLow())
    {
        status |= 0x0001;
    }
    sprintf(tmp_buf,"%04X%04X,",alert,status);
    SCI_MEMCPY(pData,tmp_buf,9);
    return 9;
}

uint16 SHB_Func_GetWIFIInfo(uint8 * pData,uint8 loc_ok)
{
    uint16 len = 0;
    uint8 wifi_num = 0;
#ifdef ZDT_WIFI_SUPPORT
    ZDT_WIFI_DATA_T last_wifi = {0};
#endif
    char temp_buf[500] = {0};
    char item_buf[100] = {0};
    uint16 i =0;
#ifdef ZDT_WIFI_SUPPORT
    //WIFI
    if(loc_ok & 0x02)
    {
        wifi_num = ZDT_WIFI_Get_DataNum();
        if(wifi_num > 3)
        {
            wifi_num = 3;
        }
    }
    else
    {
        wifi_num = 0;
    }
    if(wifi_num > 0)
    {
        for(i = 0; i < 3; i++)
        {
            if(ZDT_WIFI_Get_Data(i,&last_wifi))
            {
                SCI_MEMSET(item_buf,0,100);
                sprintf(item_buf,"|%02x:%02x:%02x:%02x:%02x:%02x|%d",last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5],last_wifi.rssi);
                strcat(temp_buf,item_buf);
            }
            else
            {
                strcat(temp_buf,"|0|0");
            }
        }
        len = strlen(temp_buf);
        SCI_MEMCPY(pData,temp_buf,len);
    }
    else
#endif
    {
        sprintf(temp_buf,"|0|0|0|0|0|0");
        len = strlen(temp_buf);
        SCI_MEMCPY(pData,temp_buf,len);
    }

    return len;
}

uint8 SHB_Func_GetGPSLat(uint8 * pData,uint32 Latitude,uint32 Lat_m,uint8 Lat_Dir)
{
    uint32 d = 0;
    uint32 d_m = 0;
    double tmp = Lat_m/0.6;
    d_m = tmp;
    d = Latitude/180000;
    sprintf((char *)pData,"%02ld.%06ld",d,d_m);
    return 8;
}

uint8 SHB_Func_GetGPSLong(uint8 * pData,uint32 Longitude,uint8 long_Dir)
{
    double tmp = 0;
    uint32 d = 0;
    uint32 d_m = 0;
    d = Longitude/180000;
    tmp = (Longitude - d*180000)/0.18;
    d_m = tmp;
    sprintf((char *)pData,"%03ld.%06ld",d,d_m);
    return 9;
}

uint8 SHB_Func_GetGPSSateNum(uint8 * pData,uint8 sate_num)
{
    char tem_buf[20] = {0};
    uint16 len = 0; 
    sprintf(tem_buf,"%d",sate_num);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    pData[len++] = ',';
    return len;
}

uint8 SHB_Func_GetGPSSpeed(uint8 * pData,uint32 speed)
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

uint8 SHB_Func_GetGPSCog(uint8 * pData,uint16 cog)
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

uint8 SHB_Func_GetGPSAlt(uint8 * pData,uint32 alt, uint8 alt_dir)
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

uint16 SHB_Func_GetSSInfo(uint8 * pData)
{
    char tem_buf[200] = {0};
    char item_buf[100] = {0};
    uint16 len = 0;
    uint16 i = 0;
    uint16 j = 0;

    MMIZDT_Station_Update();
    
    ZDT_LOG("SHB_Func_GetSSInfo rssiLevel=%d, mcc=%d,,mnc=%d,lac=%d,cell_id=%d",\
    g_zdt_station_data.s_cell.rssiLevel,g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id);

    if(g_zdt_station_data.s_cell.is_valid)
    {
        sprintf(tem_buf,"%d|%d|%d|%d|%d", \
            g_zdt_station_data.s_cell.mcc,g_zdt_station_data.s_cell.mnc,g_zdt_station_data.s_cell.lac,g_zdt_station_data.s_cell.cell_id,g_zdt_station_data.s_cell.rssiLevel);
        
        for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
        {
            if(g_zdt_station_data.n_cell[i].is_valid)
            {
                memset(item_buf,0,100);
                sprintf(item_buf,"|%d|%d|%d",g_zdt_station_data.s_cell.lac,g_zdt_station_data.n_cell[i].cell_id,g_zdt_station_data.n_cell[i].rssiLevel);
                strcat(tem_buf,item_buf);
            }
            else
            {
                memset(item_buf,0,100);
                sprintf(item_buf,"|0|0|0");
                strcat(tem_buf,item_buf);
            }
        }
    }
    else
    {
        sprintf(tem_buf,"0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0");
    }
    len = strlen(tem_buf);
    memcpy(pData,tem_buf,len);
    return len;
}


// 字符全部转换为大写
char* SHB_Func_strupr(char* src)
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
char* SHB_Func_strlwr(char* src)
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

uint16 SHB_Func_GetFileName(char *pFullPath,char *pFileName)
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

#endif
