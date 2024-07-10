/*
云芯平台
*/
#include "wldt_app.h"


#ifdef WLDT_PLAT_CY_SUPPORT

uint8 CY_Func_HexCharToInt(uint8 hexchar)
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

uint8 CY_Func_IntToHexChar(uint8 bNibble)
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

uint8 CY_Func_IntToHexChar_One(uint8 data,uint8 * dest_buf)
{
    uint8 low = 0;
    uint8 high = 0;
    high = data>>4;
    dest_buf[0] = (char)CY_Func_IntToHexChar(high);
    low = data & 0x0F;
    dest_buf[1] = (char)CY_Func_IntToHexChar(low);
    return 2;
}

int  CY_Func_GetCmdPosPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos)
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

int  CY_Func_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen)
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
// 字符全部转换为大写
char* CY_Func_strupr(char* src)
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
char* CY_Func_strlwr(char* src)
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

#endif
