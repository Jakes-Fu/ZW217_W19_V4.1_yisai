
#include "ai_common.h"









BOOLEAN           g_ai_is_charge = FALSE;
uint8                 g_ai_is_bat_low = 0;
uint8                 g_ai_is_bat_full = 0;
BOOLEAN           g_ai_is_auto_answer = FALSE;
BOOLEAN           g_ai_bt_is_on = FALSE;


#define AI_POWER10or16_GRADE  8

const uint32 g_dwaAiPower10[AI_POWER10or16_GRADE] = 
{
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000
};

// Power of 16
const uint32 g_dwaAiPower16[AI_POWER10or16_GRADE] = 
{
    1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456
};

static BOOLEAN is_all_number(char* data, uint16 len)
{
	char * temp=data;
	uint16 index = 0;
	while ((*temp != 0) && (index <len))
	{
		index++;
		switch(*temp)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				temp++;
				break;
			default:
				return FALSE;
		}
	}
	return TRUE;
}

//MEID十进制转换用
static uint16 MEID_Hex2Dec_2(uint8* ch)
{
	uint16 dec =0;
	uint16 dec1=0;
	uint16 dec2=0;
	uint16 dec3=0;
	uint16 dec4=0;
	uint8   tmp[2]={0};
	
	if ((ch[0]>='0')&&(ch[0]<='9'))
	{
		dec1 = (ch[0]-0x30);
	}
	else return 0;

	if ((ch[1]>='0')&&(ch[1]<='9'))
	{
		dec2 = (ch[1]-0x30)*2;
	}
	else return 0;

       dec3 = dec2/10;
       dec4 = dec2%10;
       	
	dec = dec1 +dec3 + dec4;
	return dec;
}

//MEID十六进制转换用
static uint16 MEID_Hex2Dec(uint8* ch)
{
	uint16 dec =0;
	uint16 dec1=0;
	uint16 dec2=0;
	uint16 dec3=0;
	uint16 dec4=0;
	uint8   tmp[2]={0};
	
	if ((ch[0]>='0')&&(ch[0]<='9'))
	{
		dec1 = (ch[0]-0x30);
	}
	else if((ch[0]>='A')&&(ch[0]<='F'))
	{
		dec1 = (ch[0]-'A'+10);
	}
	else if((ch[0]>='a')&&(ch[0]<='f'))
	{
		dec1 = (ch[0]-'a'+10);
	}
	else return 0;

	if ((ch[1]>='0')&&(ch[1]<='9'))
	{
		dec2 = (ch[1]-0x30)*2;
	}
	else if((ch[1]>='A')&&(ch[1]<='F'))
	{
		dec2 = (ch[1]-'A'+10)*2;
	}
	else if((ch[1]>='a')&&(ch[1]<='f'))
	{
		dec2 = (ch[1]-'a'+10)*2;
	}
	else return 0;

       dec3 = dec2/16;
       dec4 = dec2%16;
       	
	dec = dec1 +dec3 + dec4;
	return dec;
}

uint8 AI_Nibble2HexChar(uint8 bNibble)
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

uint8 AI_HexChar2Nibble(uint8 bHexChar)
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

uint8 AI_GetNextPara(T_ATP_GET_PARA_TYPE tType, uint8** pCmd, uint16* pLen, uint32* pNum, uint8* pStr, uint16 wMaxStrLen)
{
    uint8  bRtn = 1, c, bHaveNextComma = FALSE, bDoubleQuotes = 0, MaxNumLen = AI_POWER10or16_GRADE;
    int    i, L = 0, EndIndex = 0;

    for(i=0; i < *pLen; i++)
    {
        if(tType == ATP_GET_STR_WITH_DQ)
        {
            if(bDoubleQuotes < 2)
            {
                if(*(*pCmd + i) == '"')
                    bDoubleQuotes ++;
                continue;
            }
        }
        if( *(*pCmd + i) == ',')    // Find next comma, that is, delimiter
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
    
    EndIndex = i;    // maybe EndIndex = *pLen, that is, end of command string and no comma any more

    if(EndIndex)
    {
        if(tType == ATP_GET_NUM_DEC)  // Get decimal number
        {
            *pNum = 0;
            for(i=EndIndex-1; i>=0; i--)  // Scan from the last digit
            {
                c = *(*pCmd + i);
                if( c >= '0' && c <= '9')
                {
                    if(MaxNumLen)
                    {
                        *pNum += ( (c - '0') * g_dwaAiPower10[L++] );
                        MaxNumLen --;
                    }
                }
                else
                {   // Invalid digit
                    bRtn = 0;
                    break;
                }
            }
        }
        else if(tType == ATP_GET_NUM_HEX)  // Get hexadecimal number
        {
            *pNum = 0;
            for(i=EndIndex-1; i>=0; i--)  // Scan from the last digit
            {
                c = *(*pCmd + i);
                if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                {
                    if(MaxNumLen)
                    {
                        *pNum += ( AI_HexChar2Nibble(c) * g_dwaAiPower16[L++] );
                        MaxNumLen --;
                    }
                }
                else
                {   // Invalid digit
                    bRtn = 0;
                    break;
                }
            }
        }
        else    // Get string
        {
            bDoubleQuotes = 0;
            if(!wMaxStrLen)  // No limit if NULL
                wMaxStrLen = 512;
            for(i=0; i<EndIndex; i++)
            {
                if( i == 0 || i == EndIndex-1 )    // Check double quotes at 1st and last one
                {
                    if(tType==ATP_GET_STR_WITH_DQ && *(*pCmd + i)=='"')
                    {
                        bDoubleQuotes ++;
                        continue;
                    }
                }
                if(wMaxStrLen)
                {
                    *(pStr + L) = *(*pCmd + i);
                    L ++;
                    wMaxStrLen --;
                }
            }
            if(tType==ATP_GET_STR_WITH_DQ && bDoubleQuotes!=2)
            {   // Without a pair of double quotes
                bRtn = 0;
            }
        }
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

uint16  AI_IsAllNumber(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len //[IN]  源String的长度，byte数
                             )
{
    uint16 i = 0;
    for(i = 0; i < src_len; i++)
    {
        if( src_ptr[i] < '0' || src_ptr[i] > '9')
        {
            return i;
        }
    }
    return i;
}

uint16 AI_Str16_to_Int(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint8*  dest_ptr //[OUT] String的目标指针
                           )
{
    uint16  i,  j;
    uint8 high = 0;
    uint8 low = 0;
    uint16 len = 0;
    if(src_len == 0)
    {
        return 0;
    }
    
    // 转换成ucs2
    for(i = 0; i < src_len; i++)
    {
        j = i%2;
        if(j == 0)
        {
            if(src_ptr[i] >= 0x30 &&  src_ptr[i] < 0x3a)
            {
                high = (src_ptr[i] - 0x30) << 4;
            }
            else if(src_ptr[i] >= 0x41 && src_ptr[i] <= 0x46)
            {
                high = (src_ptr[i] - 0x41 + 10) << 4;
            }
            else if(src_ptr[i] >= 0x61 && src_ptr[i] <= 0x66)
            {
                high = (src_ptr[i] - 0x61 + 10) << 4;
            }
        }
        else
        {
            if(src_ptr[i] >= 0x30 &&  src_ptr[i] < 0x3a)
            {
                low = src_ptr[i] - 0x30;
            }
            else if(src_ptr[i] >= 0x41 && src_ptr[i] <= 0x46)
            {
                low = src_ptr[i] - 0x41 + 10;
            }
            else if(src_ptr[i] >= 0x61 && src_ptr[i] <= 0x66)
            {
                low = src_ptr[i] - 0x61 + 10;
            }
            dest_ptr[i/2] = high + low;
        }
        
    }
    
    return src_len/2;
}





uint16 AI_UCS_Str16_to_uint16(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint16*  dest_ptr, //[OUT] String的目标指针
                           uint16 dest_max_len
                           )
{
    uint16 i = 0;
    uint16 len = 0;
    uint8 * tmp_buf = NULL;
    uint16 tmp;
    tmp_buf = SCI_ALLOC_APPZ(src_len);
    SCI_MEMSET(tmp_buf,0,src_len);
    len = AI_Str16_to_Int(src_ptr,src_len,tmp_buf);
    len = len/2;
    for(i = 0; i < len; i++)
    {
        if(i < dest_max_len)
        {
            tmp = tmp_buf[i*2];
            tmp = tmp << 8;
            tmp += tmp_buf[i*2+1];
            dest_ptr[i] = tmp;
        }
        else
        {
            break;
        }
    }
    SCI_FREE(tmp_buf);
}

uint32  AI_Get_DayTimeSecond(void)
{
    uint32 daysecond;
    SCI_TIME_T          time;
    TM_GetSysTime(&time);
    daysecond = (time.hour* 3600) + (time.min* 60) + time.sec;
    return daysecond;
}

uint32  AI_Get_TimeSecond(void)
{
    SCI_TM_T sys_time = {0};
    uint32   second = 0;

    TM_GetTime(&sys_time);
    second = MMIAPICOM_Tm2Second(sys_time.tm_sec, sys_time.tm_min, sys_time.tm_hour, sys_time.tm_mday, sys_time.tm_mon, sys_time.tm_year);

    return second;
}

BOOLEAN AI_GetIsCharge(void)
{
    return g_ai_is_charge;
}

void AI_SetIsCharge(BOOLEAN is_charge)
{
    if(g_ai_is_charge != is_charge)
    {
        g_ai_is_charge = is_charge;
    }
	return;
}

uint8 AI_GetIsBatLow(void)
{
    return g_ai_is_bat_low;
}

void AI_SetIsBatLow(uint8 is_low)
{
    if(g_ai_is_bat_low != is_low)
    {
        g_ai_is_bat_low = is_low;
#ifdef AI_PLAT_YX_SUPPORT
        YX_API_BatStatus_Post(1,10);
#endif
    }
    return;
}

uint8 AI_GetIsBatFull(void)
{
    return g_ai_is_bat_full;
}

void AI_SetIsBatFull(uint8 is_full)
{
    if(g_ai_is_bat_full != is_full)
    {
        g_ai_is_bat_full = is_full;
    }
    return;
}

BOOLEAN AI_GetIsAutoAnswer(void)
{
    return g_ai_is_auto_answer;
}

void AI_SetIsAutoAnswer(BOOLEAN is_auto)
{
    g_ai_is_auto_answer = is_auto;
    return;
}

BOOLEAN AI_GetIsBTOn(void)
{
    return g_ai_bt_is_on;
}

void AI_SetIsBTOn(BOOLEAN is_on)
{
    g_ai_bt_is_on = is_on;
    return;
}

BOOLEAN AI_SIM_Exsit(void)
{
    if(MMIPHONE_IsSimOk(MN_DUAL_SYS_1) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}

#if 1
BOOLEAN  AI_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
         MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
        
	file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
	if(SFS_INVALID_HANDLE != file_handle)
	{
		file_ret = MMIFILE_WriteFile(
			file_handle, 
			data_ptr, 
			data_len, 
			&transbyte, 
			NULL
			);
        
		if(SFS_ERROR_NONE == file_ret)
		{
			ret = TRUE;
		}
		MMIFILE_CloseFile(file_handle);
	}

    return ret;
	
}

BOOLEAN  AI_DBFile_Exist(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     
     return MMIFILE_IsFileExist(full_path,full_path_len);	
}

uint32  AI_DBFile_GetSize(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;	
}

BOOLEAN  AI_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read)
{
    BOOLEAN ret = FALSE;
    FFS_ERROR_E error = FFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
	  
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }
        
    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
    full_path_len += tmp_len;
        
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
    }
    else
    {
        return FALSE;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
    	file_ret = MMIFILE_ReadFile(
                              file_handle, 
                              buffer, 
                              bytes_to_read, 
                              bytes_read, 
                              NULL
                             );
        
        if(SFS_ERROR_NONE != file_ret)
        {
            error = FFS_ERROR_INVALID_PARAM;
        }
        else
        {
            ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN  AI_DBFile_RemoveFile(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
     return TRUE;	
}

BOOLEAN  AI_DBFile_RemoveDir(char * path	)
{
    BOOLEAN ret = FALSE;
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    SFS_ERROR_E		result              = SFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;

    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
	  
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }

    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return TRUE;
    }

    do
    {
        uint32 uNameLen = 0;
        uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
        SCI_MEMCPY(&full_path[full_path_len], sfsfind.name, MIN(uNameLen*2+2, MMIFILE_FULL_PATH_MAX_LEN));
        full_path[full_path_len+uNameLen] = '\0';

        // 是目录
        if( sfsfind.attr & SFS_ATTR_DIR )
        {
        }
        else
        {
            //删除文件
            result = MMIFILE_DeleteFile(full_path, PNULL);

            if( SFS_NO_ERROR != result )
            {
                SFS_FindClose(hFind);
                return FALSE;
            }
        }

        result = SFS_FindNextFile(hFind, &sfsfind);

    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);

    return TRUE;
}
#endif


BOOLEAN AI_File_MemFull(BOOLEAN is_tf)
{
    uint32          free_space_high     = 0;
    uint32          free_space_low      = 102400;
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       return TRUE;
    }
    
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                            &cur_free_space_high,
                                            &cur_free_space_low);

        if (file_ret == SFS_ERROR_NONE)
        {
            if (cur_free_space_high > free_space_high //高位大于肯定大于
                    || ( (cur_free_space_high == free_space_high) //高位相等，低位大于
                         && (cur_free_space_low > free_space_low))
               )
            {
                return FALSE;
            }
        }
    }
        
    return TRUE;
}

BOOLEAN AI_File_Save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;


    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(is_add)
    {
        if(MMIFILE_IsFileExist(full_path, full_path_len))
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);
        }
        else
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
        }
    }
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }
    
    if(SFS_INVALID_HANDLE != file_handle)
    {
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);

        if(SFS_ERROR_NONE == file_ret)
        {
        	ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
	
}

void AI_Save_Mp3(uint8* data, uint32 data_len)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16  tmp_len = 0;
    BOOLEAN ret = FALSE;
    MMIFILE_HANDLE file_handle;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)AI_VOICE_RCV_DIR, SCI_STRLEN(AI_VOICE_RCV_DIR));
    full_path_len = tmp_len;
    if(!MMIFILE_IsFileExist(full_path, full_path_len))
    {
        ret = MMIFILE_CreateDir(full_path, full_path_len);
    }
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)AI_VOICE_RCV_FULL_PATH, SCI_STRLEN(AI_VOICE_RCV_FULL_PATH));
    full_path_len = tmp_len;
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
        
    }
    file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);

    //file_handle = AI_File_Open(full_path, FALSE);
    if(file_handle == SFS_INVALID_HANDLE)
    {
        AI_LOG("AI AI_Save_Mp3 create file failed");
        return ;
    }

    if(AI_File_Write(file_handle, data, data_len)==0)
    {
        AI_LOG("AI AI_Save_Mp3 file handle error AEE_ENOMEMORY");
        AI_File_Close(file_handle);
        return ;
    }
    AI_File_Close(file_handle);

}


BOOLEAN AI_File_Remove(const char * file_name)
{	
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
       
    if(MMIFILE_IsFileExist(full_path,full_path_len))
    {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
        return TRUE;
    }

    return FALSE;
}

BOOLEAN AI_File_Read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len)
{
    BOOLEAN ret = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;
    
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
    }
    else
    {
        return FALSE;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
    	file_ret = MMIFILE_ReadFile(
                              file_handle, 
                              data_ptr, 
                              data_len, 
                              read_len, 
                              PNULL
                             );
        
    	if(SFS_ERROR_NONE == file_ret)
    	{
    		ret = TRUE;
    	}
    	MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN AI_File_Exsit(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    
    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        return TRUE;
    }
    
    return FALSE;
}

BOOLEAN AI_Dir_Exsit(const char * dir_path)
{
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    BOOLEAN		result              = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    BOOLEAN ret = FALSE;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return FALSE;
    }

    SFS_FindClose(hFind);

    return TRUE;
}

BOOLEAN AI_File_RemoveDirFile(const char * dir_path)
{
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    BOOLEAN		result              = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    uint16 i = 0;
    BOOLEAN ret = FALSE;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)dir_path, SCI_STRLEN(dir_path));
    full_path_len = tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return TRUE;
    }

    do
    {
        uint32 uNameLen = 0;
        uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
        SCI_MEMCPY(&full_path[full_path_len], sfsfind.name, MIN(uNameLen*2+2, MMIFILE_FULL_PATH_MAX_LEN));
        full_path[full_path_len+uNameLen] = '\0';

        // 是目录
        if( sfsfind.attr & SFS_ATTR_DIR )
        {
        }
        else
        {
            //删除文件
            result = MMIFILE_DeleteFile(full_path, PNULL);

            if( SFS_NO_ERROR != result )
            {
                SFS_FindClose(hFind);
                return FALSE;
            }
        }

        result = SFS_FindNextFile(hFind, &sfsfind);

    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);

    return TRUE;
}

MMIFILE_HANDLE  AI_File_Open(const char * file_name,BOOLEAN is_read)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    if(is_read)
    {
        if(MMIFILE_IsFileExist(full_path, full_path_len))
        {
            file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
        }
        else
        {
            return SFS_INVALID_HANDLE;
        }
    }
    else
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }

    return file_handle;
	
}

uint32  AI_File_Write(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len)
{
    uint16 j = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    uint32 transbyte = 0;
    uint32 all_transbyte = 0;
    BOOLEAN ret = TRUE;
    if(file_handle == SFS_INVALID_HANDLE)
    {
        return 0;
    }

    do
    {
        transbyte = 0;
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);
        
        if(SFS_ERROR_NONE != file_ret)
        {
            ret = FALSE;
            break;
        }
        all_transbyte += transbyte;
    }while(transbyte != 0 && all_transbyte < data_len);
    

    return all_transbyte;
	
}

void  AI_File_Close(MMIFILE_HANDLE file_handle)
{
    if(file_handle != SFS_INVALID_HANDLE)
    {
        MMIFILE_CloseFile(file_handle);
    }
    return;
	
}

uint32  AI_File_GetSize(const char * file_name)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    uint16   tmp_len = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    
     tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
     full_path_len = tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;
}

BOOLEAN  AI_File_Rename(const char * file_name, const char * new_filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar   new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  new_full_path_len = 0;
    uint16   tmp_len = 0;

    tmp_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    full_path_len = tmp_len;

    tmp_len = GUI_GBToWstr(new_full_path, (const uint8*)new_filename, SCI_STRLEN(new_filename));
    new_full_path_len = tmp_len;

    return MMIAPIFMM_RenameFile(full_path,full_path_len,new_full_path,new_full_path_len);
}
