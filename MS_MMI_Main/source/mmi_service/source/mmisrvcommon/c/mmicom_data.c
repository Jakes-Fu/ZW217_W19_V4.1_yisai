/****************************************************************************
** File Name:      mmicom_data.c                                             *
** Author:         jian.ma                                                   *
** Date:           03/04/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008        jian.ma            Create
** 
****************************************************************************/
#define MMICOM_DATA_C


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
//#ifdef WIN32
//#include "std_header.h"
//#endif
#include "mmicom_data.h"
#include "mmk_type.h"
#include "os_api.h"
#include "mmk_app.h"
//#include "mmk_authen.h"
//#include "guicommon.h"
#include "mmisrv_adapt.h"
#include "mmi_string.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define MMICOM_STD_TRUE                1
#define FLOAT_PRECISION         1000
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

static const int sin_0_90_table[91] = 
{
  0,
 17, 34, 52, 69, 87,104,121,139,156,173,
190,207,224,241,258,275,292,309,325,342,
358,374,390,406,422,438,453,469,484,499,
515,529,544,559,573,587,601,615,629,642,
656,669,681,694,707,719,731,743,754,766,
777,788,798,809,819,829,838,848,857,866,
874,882,891,898,906,913,920,927,933,939,
945,951,956,961,965,970,974,978,981,984,
987,990,992,994,996,997,998,999,999,1000,
};

/*****************************************************************************/
//  Description : ssc string valid check
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: valid ssc string
//          FALSE:invalid ssc string
/*****************************************************************************/
LOCAL BOOLEAN check_ssc_string(char *pStr);

/*****************************************************************************/
//  Description : tel number string valid check
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: valid tel number
//          FALSE:invalid tel number
/*****************************************************************************/
LOCAL BOOLEAN check_tel_string(char *pStr);

PUBLIC void MMIAPICOM_GetBcdLen(
                   uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                   uint8 *pBcd,  // BCD码的头指针
                   uint8 *pLen   // 返回BCD码的长度
                  )
{
    int32 i = 0;
    uint8 bcdLen = 0;

    /* BCD format is defined in applayer.h                    */
    /* NOTE: LSB first 1234 = 0x21 0x43, bcdLen = 4           */
    /*                 123  = 0x21 0xf3, bcdLen = 3           */
    /*       MSB first 1234 = 0x12 0x34, bcdLen = 4           */
    /*                 123  = 0x12 0x3f, bcdLen = 3           */
    /*       unpacked  1234 = 0x01 0x02 0x03 0x04, bcdLen = 4 */

    switch(bcdFmt)
    {
        case PACKED_LSB_FIRST:
            bcdLen = 0;
            for(i=0;MMICOM_STD_TRUE;i++)/*lint !e506 */
            {
                if(((pBcd[i/2]>>(((i+1)&1)?0:4))&0x0f) == 0x0f)
                {                  /* NOTE: BCD is end by 0x0f */
                   break;
                }
                bcdLen ++;
            }
            bcdLen = i;
            break;
        case PACKED_MSB_FIRST:
            bcdLen = 0;
            for(i=0;MMICOM_STD_TRUE;i++)/*lint !e506 */
            {
                if(((pBcd[i/2]>>((i&1)?0:4))&0x0f) == 0x0f)
                {
                   break;
                }
                bcdLen ++;
            }
            bcdLen = i;
            break;
        case UNPACKED        :
            bcdLen = 0;
            for(i=0;pBcd[i] != 0xff && pBcd[i] != 0x0f;i++) ;
            bcdLen = i;
            break;
        default:
            // DevFail("wrong bcd format");
            break;
    }
    *pLen = bcdLen;
}   /* End Of mmi_GetBcdLen() */

/*****************************************************************************/
//  Description : convert the bcd code to string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdToStr(
                  uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                  uint8 *pBcd,  // BCD码的头指针
                  uint8 bcdLen, // BCD码的长度
                  char* str     // 转换后String的头指针
                 )
{
    /* BCD format is defined in applayer.h                    */
    /* NOTE: LSB first 1234 = 0x21 0x43, bcdLen = 4           */
    /*                 123  = 0x21 0xf3, bcdLen = 3           */
    /*       MSB first 1234 = 0x12 0x34, bcdLen = 4           */
    /*                 123  = 0x12 0x3f, bcdLen = 3           */
    /*       unpacked  1234 = 0x01 0x02 0x03 0x04, bcdLen = 4 */
    switch(bcdFmt)
    {
        case PACKED_LSB_FIRST:
            MMIAPICOM_BcdLfToStr(pBcd,bcdLen,str);
            break;
        case PACKED_MSB_FIRST:
            MMIAPICOM_BcdMfToStr(pBcd,bcdLen,str);
            break;
        case UNPACKED        :
            MMIAPICOM_BcdUpToStr(pBcd,bcdLen,str);
            break;
        default:
            // DevFail("wrong bcd format");
            break;
    }
}   /* End Of mmi_BcdToStr() */

/*****************************************************************************/
//  Description : convert the bcd code to string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdToStr(
                  uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                  uint8 *pBcd,  // BCD码的头指针
                  uint8 bcdLen, // BCD码的长度
                  char* str     // 转换后String的头指针
                 )
{
    /* BCD format is defined in applayer.h                    */
    /* NOTE: LSB first 1234 = 0x21 0x43, bcdLen = 4           */
    /*                 123  = 0x21 0xf3, bcdLen = 3           */
    /*       MSB first 1234 = 0x12 0x34, bcdLen = 4           */
    /*                 123  = 0x12 0x3f, bcdLen = 3           */
    /*       unpacked  1234 = 0x01 0x02 0x03 0x04, bcdLen = 4 */
    switch(bcdFmt)
    {
        case PACKED_LSB_FIRST:
            MMIAPICOM_NetBcdLfToStr(pBcd,bcdLen,str);
            break;
        case PACKED_MSB_FIRST:
            MMIAPICOM_NetBcdMfToStr(pBcd,bcdLen,str);
            break;
        case UNPACKED        :
            MMIAPICOM_NetBcdUpToStr(pBcd,bcdLen,str);
            break;
        default:
            // DevFail("wrong bcd format");
            break;
    }
}   /* End Of mmi_BcdToStr() */
/*****************************************************************************/
//  Description : convert the bcd code(PACKED_LSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdLfToStr(
                    uint8 *pBcd,  // BCD码的头指针
                    uint8 bcdLen, // BCD码的长度
                    char* str     // 转换后String的头指针
                   )
{   /*BCD format - LSB first (1234 = 0x21, 0x43)*/
    int32 i = 0;
    uint8 bcdCode = 0;
    uint8 ascCode = 0;

    for(i = 0;i < bcdLen; i++)
    {
        bcdCode = (pBcd[i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;
        if(bcdCode == DIALBCD_FILLER)
        {
            break;
        }
        ascCode = (bcdCode == DIALBCD_STAR) ? '*':
                  (bcdCode == DIALBCD_HASH) ? '#':
                  (bcdCode == DIALBCD_PAUSE)? 'P':
                  (bcdCode == DIALBCD_WILD) ? 'W': 
                  (bcdCode == DIALBCD_EXPANSION)?'+':
                  (bcdCode + '0');


        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
              ascCode == 'W'||ascCode == 'w'||ascCode == '+'))
           ascCode = 0;

        str[i] = ascCode;
    }

    str[i] = 0;
}  /*-- End of MMI_BcdLfToStr( ) --*/

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_LSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdLfToStr(
                    uint8 *pBcd,  // BCD码的头指针
                    uint8 bcdLen, // BCD码的长度
                    char* str     // 转换后String的头指针
                   )
{   /*BCD format - LSB first (1234 = 0x21, 0x43)*/
    int32 i = 0;
    uint8 bcdCode = 0;
    uint8 ascCode = 0;

    for(i = 0;i < bcdLen; i++)
    {
        bcdCode = (pBcd[i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;
        if(bcdCode == DIALBCD_FILLER)
        {
            break;
        }
        ascCode = (bcdCode == DIALBCD_STAR) ? '*':
                  (bcdCode == DIALBCD_HASH) ? '#':
                  (bcdCode == DIALBCD_PAUSE)? 'a':
                  (bcdCode == DIALBCD_WILD) ? 'b': 
                  (bcdCode == DIALBCD_EXPANSION) ? 'c':(bcdCode + '0');


        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'a'||ascCode == 'b')||
              ascCode == 'c'))
           ascCode = 0;

        str[i] = ascCode;
    }

    str[i] = 0;
}  /*-- End of MMI_BcdLfToStr( ) --*/

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_MSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdMfToStr(
                    uint8 *pBcd,  // BCD码的头指针
                    uint8 bcdLen, // BCD码的长度
                    char* str     // 转换后String的头指针
                   )
{   /*BCD format - MSB first (1234 = 0x12 0x34)*/
    int32 i = 0;
    uint8 bcdCode = 0;
    uint8 ascCode = 0;

    for(i = 0;i < bcdLen; i++)
    {
        bcdCode = (pBcd[i/2] >> ((i & 1) ? 0 : 4)) & 0x0F;
        if(bcdCode == 0x0f) break;
        ascCode = (bcdCode == DIALBCD_STAR) ? '*':
                  (bcdCode == DIALBCD_HASH) ? '#':
                  (bcdCode == DIALBCD_PAUSE)? 'P':
                  (bcdCode == DIALBCD_WILD) ? 'w': (bcdCode + '0');

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
              ascCode == 'W'||ascCode == 'w'))
           ascCode = 0;

        str[i] = ascCode;
    }

    str[i] = 0;
}  /*-- End of MMI_BcdMfToStr( ) --*/

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_MSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdMfToStr(
                    uint8 *pBcd,  // BCD码的头指针
                    uint8 bcdLen, // BCD码的长度
                    char* str     // 转换后String的头指针
                   )
{   /*BCD format - MSB first (1234 = 0x12 0x34)*/
    int32 i = 0;
    uint8 bcdCode = 0;
    uint8 ascCode = 0;

    for(i = 0;i < bcdLen; i++)
    {
        bcdCode = (pBcd[i/2] >> ((i & 1) ? 0 : 4)) & 0x0F;
        if(bcdCode == 0x0f) break;
        ascCode = (bcdCode == DIALBCD_STAR) ? '*':
                  (bcdCode == DIALBCD_HASH) ? '#':
                  (bcdCode == DIALBCD_PAUSE)? 'a':
                  (bcdCode == DIALBCD_WILD) ? 'b': 
                  (bcdCode == DIALBCD_EXPANSION) ? 'c':(bcdCode + '0');

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'a'||ascCode == 'b')||
              ascCode == 'c'))
           ascCode = 0;

        str[i] = ascCode;
    }

    str[i] = 0;
}  /*-- End of MMI_BcdMfToStr( ) --*/

/*****************************************************************************/
//  Description : convert the bcd code(UNPACKED) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdUpToStr(
                    uint8 *pBcd,  // BCD码的头指针
                    uint8 bcdLen, // BCD码的长度
                    char *str     // 转换后String的头指针
                   )
{   /*BCD format - unpacked (1 digit per byte)*/
    int32 i = 0;
    uint8 bcdCode = 0;
    uint8 ascCode = 0;

    for(i = 0;i < bcdLen; i++)
    {
        bcdCode = pBcd[i];
        ascCode = (bcdCode == DIALBCD_STAR) ? '*':
                  (bcdCode == DIALBCD_HASH) ? '#':
                  (bcdCode == DIALBCD_PAUSE)? 'P':
                  (bcdCode == DIALBCD_WILD) ? 'w': (bcdCode + '0');

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
              ascCode == 'W'||ascCode == 'w'))
           ascCode = 0;

        str[i] = ascCode;
    }

    str[i] = 0;
}  /*-- End of MMI_BcdUpToStr( ) --*/

/*****************************************************************************/
//  Description : convert the bcd code(UNPACKED) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdUpToStr(
                    uint8 *pBcd,  // BCD码的头指针
                    uint8 bcdLen, // BCD码的长度
                    char *str     // 转换后String的头指针
                   )
{   /*BCD format - unpacked (1 digit per byte)*/
    int32 i = 0;
    uint8 bcdCode = 0;
    uint8 ascCode = 0;

    for(i = 0;i < bcdLen; i++)
    {
        bcdCode = pBcd[i];
        ascCode = (bcdCode == DIALBCD_STAR) ? '*':
                  (bcdCode == DIALBCD_HASH) ? '#':
                  (bcdCode == DIALBCD_PAUSE)? 'a':
                  (bcdCode == DIALBCD_WILD) ? 'b': 
                  (bcdCode == DIALBCD_EXPANSION) ? 'c':(bcdCode + '0');

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'a'||ascCode == 'b')||
              ascCode == 'c'))
           ascCode = 0;

        str[i] = ascCode;
    }

    str[i] = 0;
}  /*-- End of MMI_BcdUpToStr( ) --*/

/*****************************************************************************/
//  Description : convert the string to the bcd code
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcd(
                  uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                  char *pStr,   // String的头指针
                  uint8 *pBcd   // 转换后BCD码的头指针
                 )
{
    uint8 strLen = 0;

    strLen = strlen(pStr);

    switch(bcdFmt)
    {
        case PACKED_LSB_FIRST:
            MMIAPICOM_StrToBcdLf(pStr,strLen,pBcd);
            break;
        case PACKED_MSB_FIRST:
            MMIAPICOM_StrToBcdMf(pStr,strLen,pBcd);
            break;
        case UNPACKED        :
            MMIAPICOM_StrToBcdUp(pStr,strLen,pBcd);
            break;
        default:
            // DevFail("Wrong bcd format");
            break;
    }
}   /* End Of mmi_StrToBcd() */

/*****************************************************************************/
//  Description : check this the sting can be convert to bcd number
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: the pStr can be convert to bcd number
//          FALSE:the pStr include invalid string, can not to be convert to bcd number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsValidNumberStringEx(MMI_STRING_T *num_string)
{
    BOOLEAN ret = TRUE;
    uint8 *num_str = PNULL;

    if (PNULL == num_string || PNULL == num_string->wstr_ptr || 0 == num_string->wstr_len)
    {
        return FALSE;
    }

    num_str = SCI_ALLOCA(num_string->wstr_len + 1);

    if (PNULL == num_str)
    {
        return FALSE;
    }

    SCI_MEMSET(num_str, 0x00, (num_string->wstr_len + 1));

    MMI_WSTRNTOSTR(num_str, num_string->wstr_len, num_string->wstr_ptr, num_string->wstr_len, num_string->wstr_len);

    ret = MMIAPICOM_IsValidNumberString((char *)num_str, num_string->wstr_len);

    SCI_FREE(num_str);

    return ret;
}

/*****************************************************************************/
//  Description : check this the sting can be convert to bcd number
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: the pStr can be convert to bcd number
//          FALSE:the pStr include invalid string, can not to be convert to bcd number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsValidNumberString(
                      char *pStr,   // String的头指针
                      uint8 strLen  // String的长度
                     )
{
    BOOLEAN ret = TRUE;
    int32 i = 0;

    if (PNULL == pStr || 0 == strLen || pStr[0] == 0)
    {
        ret = FALSE;

        return ret;
    }

	//1.号码中只能有‘0’ - '9', +, *, #, P, W；
    for(i = 0; i < strLen; i++)
    {
        if(!MMIAPICOM_IsTelephoneNum(pStr[i]))
        {
            ret = FALSE;

            break;
        }
    }
	if(ret)
	{
		//P，W开头的无效
		if(pStr[0] != '+' && (pStr[0]=='p' || pStr[0]== 'w' ||pStr[0]=='P' || pStr[0]== 'W'))
			
		{
			return FALSE;
		}
		//+P，+W开头的无效
		if(pStr[0] == '+' && (pStr[1]=='p' || pStr[1]== 'w' ||pStr[1]=='P' || pStr[1]== 'W'))
		{
			return FALSE;
		}

#if 0
		//以#,*,+#,+*开头的号码是ssc号码
		if(!strncmp(pStr, "*", strlen("*")) 
			||!strncmp(pStr, "+*", strlen("+*")) 
			||!strncmp(pStr, "#", strlen("#"))
			||!strncmp(pStr, "+#", strlen("+#")) 
			)
		{
			//ss code string
			ret = check_ssc_string(pStr);
		}
		else
		{
			//telephone string
			ret = check_tel_string(pStr);	
		}
#endif

        /*1. 如果是以“*”“#”“**” “*#”或者“##”开始，以"#"结尾的，或者形似“7X”，X为0-9之间的一个数字，认为是ssc号码，进行ssc号码有效性判断（70--79就是ussd字串；前面的需要继续进行判断）；
		  2. 否则认为是普通号码，进行普通号码的有效性判断；*/
		//以*,#,**,*#,##开头的，且以"#"结尾的号码是ssc号码
		if(((!strncmp(pStr, "*", strlen("*")) 
			||!strncmp(pStr, "#", strlen("#")) 
			||!strncmp(pStr, "**", strlen("**"))
			||!strncmp(pStr, "*#", strlen("*#")) 
			||!strncmp(pStr, "##", strlen("##")))
			&&(pStr[strLen - 1] == '#'))
			|| (!strncmp(pStr, "7", strlen("7")) && (pStr[1] <= 0x39) && (pStr[1] >= 0x30) && (2 == strLen))
			)
        {
            //ss code string
            ret = check_ssc_string(pStr);
        }
        else
        {
            //telephone string
            ret = check_tel_string(pStr);
        }   
			
	}
    return ret; 
}


/*****************************************************************************/
//  Description : ssc string valid check
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: valid ssc string
//          FALSE:invalid ssc string
// ssc's regulation
/*一：号码有效性判断过程：
1. 如果是以"*""#""**" "*#"或者"##"开始，以"#"结尾的，或者形似"7X",X为0-9之间的一个数字，认为是ssc号码，进行ssc号码有效性判断；
2. 否则认为是普通号码，进行普通号码的有效性判断；

二：ssc号码的有效性判断规则
1. 号码中只能有"0"-"9", +, *, #, P, W;
2. 如果以 "+p"或者 "+w"开头则认为无效；----------(去掉这句话)
3. 如果是在号码中间出现 "*", "#"认为无效；   //该条规则 后来是否修改过？？？？？？？
4. "*", "#"开头不是以"#"结尾的无效；P, W开头的无效；
5. 否则是有效的ssc。

三；普通号码的有效性判断规则
1. 号码中只能有"0"-"9", +, *, #, P, W;
2. 如果以 "+p"或者 "+w"开头则认为无效；
3. 开头多余1个"+"认为无效；
4. 如果整个号码是一个"+"认为无效；
5. "P", "W"开头的无效； "+"出现在非首位是无效的。
*/
/*****************************************************************************/
LOCAL BOOLEAN check_ssc_string(char *pStr)
{
    uint8 i; //j, plus_index;
	uint8 len = 0;
    	if(pStr == PNULL)
	{
		return FALSE;
	}
	len = strlen((char*)pStr);
/*    
	if(pStr)
	{
        i = 0;
		//跳过前面的+,#, *
		if(pStr[i] == '+')
		{
			i++;
		}
		while ( (pStr[i] == '*' || pStr[i] == '#') )
		{
			i++;
		}
		while (pStr[i] >=  '0' && pStr[i] <= '9' )
		{       
			i++;
		}
		if (pStr[i] != '*' && pStr[i] != '#' && pStr[i] != '\0')
		{
			//不是以#,*，数字结尾的无效，#,*出现中间无效
			return FALSE;
		}
		if( pStr[i] == '\0')
		{
			//后面没有其他字符是有效号码
			return TRUE;
		}
		i++;
		//如果*,# 后面还有其他字符，后面只能紧跟一个+
		plus_index = i;
		for (j=i ; pStr[j] != '\0'; j++)
		{
			if (pStr[j] == '+')
			{
				plus_index = j;        
			}
		}
		
		if (plus_index != i)
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
*/

	{
        i = 0;
		//跳过前面的#, *
		while ( (pStr[i] == '*' || pStr[i] == '#') )
		{
			i++;
		}
		while (((pStr[i] >=  '0' && pStr[i] <= '9' ) 
                        || pStr[i] == 'p'
                        || pStr[i] == 'w' 
                        || pStr[i] == 'P' 
                        || pStr[i] == 'W'
                        || pStr[i] == '+'
                        || pStr[i] == '*'
                        )
                        && i < (strlen((char*)pStr ) - 1)
                        )
		{       
			i++;
		}
        
		//#出现中间无效
		if ( pStr[i] == '#')
		{
			i++;
			if( (i < len) && (pStr[i++] != '\0') )
			{
				return FALSE;
			}
		}
		
		//不是以#结尾的无效
		/*if (pStr[--len] != '#')
		{
			return FALSE;
		}*/
		return TRUE;
	}
}

/*****************************************************************************/
//  Description : tel number string valid check
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: valid tel number
//          FALSE:invalid tel number
/*****************************************************************************/
LOCAL BOOLEAN check_tel_string(char *pStr)
{
    uint8 i, len;
    int8 plus_index;

	if(pStr == PNULL)
	{
		return FALSE;
	}
    len = strlen((char*)pStr);

    //以"+p""+w""+P""+W""p""w""P""W"开头的无效
    if((!strncmp(pStr, "+p", strlen("+p")))
       ||(!strncmp(pStr, "+w", strlen("+w")))
       ||(!strncmp(pStr, "+P", strlen("+P")))
       ||(!strncmp(pStr, "+W", strlen("+W")))
       ||(!strncmp(pStr, "p", strlen("p")))
       ||(!strncmp(pStr, "w", strlen("w")))
       ||(!strncmp(pStr, "P", strlen("P")))
       ||(!strncmp(pStr, "W", strlen("W")))
       )
    {
        return FALSE;
    }

    plus_index = -1;
	//检查开头+的个数
    for ( i=0; i<len ; i++)
    {
        if (pStr[i] == '+' )
        {
            plus_index = i;
        }
    }

    // 若"+"出现在非首位是无效的
    if ((plus_index > 0)/*&&(plus_index < (len - 1))*/)
	{
        return FALSE;
	}
    //这个号码是“+”无效
    else if (plus_index == 0 && len == 1)
	{
        return FALSE;
	}

    // * 和 # 前面只有+或者以* 和 #开头，无效
    //本条规则放宽，去掉
    /*plus_index++;
    if(pStr[plus_index] == '#' || pStr[plus_index] == '*')
	{
        return FALSE;
	}*/
    
    return TRUE;
}
/*****************************************************************************/
//  Description : convert the string to the bcd code(PACKED_LSB_FIRST)
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcdLf(
                    char *pStr,   // String的头指针
                    uint8 strLen, // String的长度
                    uint8 *pBcd   // 转换后BCD码的头指针
                   )
{
    uint8 bcdCode = 0;
    uint8 ascCode = 0;
    int32 i = 0;

    /*memset(pBcd,0xFF,(strLen&1) ? (strLen+1)/2 : strLen/2+1);*/

    for(i=0;i < strLen;i++)
    {
        ascCode = pStr[i];
        /*bcdCode = 0xFF;*/

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
              ascCode == 'W'||ascCode == 'w'||ascCode=='+'))
            break;
        /*
        bcdCode = ( ascCode == '*' ) ? DIALBCD_STAR:
                  ( ascCode == '#' ) ? DIALBCD_HASH:
                  ( ascCode == 'P'||ascCode == 'p') ? DIALBCD_PAUSE:
                  ( ascCode == 'W' ) ? DIALBCD_WILD:(ascCode - '0');
        */
        switch( ascCode )
        {
            case '*':
                bcdCode = DIALBCD_STAR;
                break;
            case '#':
                bcdCode = DIALBCD_HASH;
                break;
            case 'P':
            case 'p':
                bcdCode = DIALBCD_PAUSE;
                break;
            case 'W':
            case 'w':
                bcdCode = DIALBCD_WILD;
                break;
            case '+':
		        bcdCode=DIALBCD_EXPANSION;
		        break;
            default:
                bcdCode = ascCode - '0';
                break;
        }
        pBcd[i/2] = ((i & 1) ? pBcd[i/2] : 0) +
                    (bcdCode << ((i & 1) ? 4 : 0));
    }
    if(i&1)
    {
       pBcd[(i-1)/2] |= 0xf0;
    }

}   /* End Of MMI_StrToBcdLf() */

/*****************************************************************************/
//  Description : convert the string to the bcd code(PACKED_MSB_FIRST)
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcdMf(
                    char *pStr,   // String的头指针
                    uint8 strLen, // String的长度
                    uint8 *pBcd   // 转换后BCD码的头指针
                   )
{
    uint8 bcdCode = 0;
    uint8 ascCode = 0;
    int32 i = 0;

  /*memset(pBcd,0xFF,(strLen&1) ? (strLen+1)/2 : strLen/2+1); */

    for(i=0;i < strLen;i++)
    {
        ascCode = pStr[i];
        /*bcdCode = 0xFF;*/

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
              ascCode == 'W'||ascCode == 'w'))
            break;
        /*
        bcdCode = ( ascCode == '*' ) ? DIALBCD_STAR:
                  ( ascCode == '#' ) ? DIALBCD_HASH:
                  ( ascCode == 'P'||ascCode == 'p') ? DIALBCD_PAUSE:
                  ( ascCode == 'W' ) ? DIALBCD_WILD:(ascCode - '0');
        */
        switch( ascCode )
        {
            case '*':
                bcdCode = DIALBCD_STAR;
                break;
            case '#':
                bcdCode = DIALBCD_HASH;
                break;
            case 'P':
            case 'p':
                bcdCode = DIALBCD_PAUSE;
                break;
            case 'W':
            case 'w':
                bcdCode = DIALBCD_WILD;
                break;
            default:
                bcdCode = ascCode - '0';
                break;
        }
        pBcd[i/2] = ((i & 1) ? pBcd[i/2] : 0) +
                    (bcdCode << ((i+1 & 1) ? 4 : 0));
    }
    if(i&1)
    {
       pBcd[(i-1)/2] |= 0x0f;
    }

}   /* End Of MMI_StrToBcdMf() */

/*****************************************************************************/
//  Description : convert the string to the bcd code(UNPACKED)
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcdUp(
                    char *pStr,   // String的头指针
                    uint8 strLen, // String的长度
                    uint8 *pBcd   // 转换后BCD码的头指针
                   )
{
    uint8 bcdCode = 0;
    uint8 ascCode = 0;
    int32 i = 0;

    /*memset(pBcd,0xFF,strLen+1);*/

    for(i=0;i < strLen;i++)
    {
        ascCode = pStr[i];

        if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
              ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
              ascCode == 'W'||ascCode == 'w'))
            break;
        /*
        bcdCode = ( ascCode == '*' ) ? DIALBCD_STAR:
                  ( ascCode == '#' ) ? DIALBCD_HASH:
                  ( ascCode == 'P'||ascCode == 'p') ? DIALBCD_PAUSE:
                  ( ascCode == '+' ) ? DIALBCD_WILD:(ascCode - '0');
        */

        switch( ascCode )
        {
            case '*':
                bcdCode = DIALBCD_STAR;
                break;
            case '#':
                bcdCode = DIALBCD_HASH;
                break;
            case 'P':
            case 'p':
                bcdCode = DIALBCD_PAUSE;
                break;
            case 'W':
            case 'w':
                bcdCode = DIALBCD_WILD;
                break;
            default:
                bcdCode = ascCode - '0';
                break;
        }
        pBcd[i] = bcdCode;
    }
}   /* End Of MMI_StrToBcdUp() */

/*****************************************************************************/
//  Description : 得到a除以b的商
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Divide(
              uint16 a,
              uint16 b
              ) 
{
    return GUI_Divide(a, b);
}

/*****************************************************************************/
//  Description : 得到a除以b的余数
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Remainder(
                 uint16 a,
                 uint16 b
                 )
{
    if (0 == b)
    {
        return 0;
        //SCI_PASSERT(FALSE,("Remainder: divide 0 error!"));  /*assert verified*/
    }

    return (uint16)(a % b);
}

/******************************************************************************/
//  Description : draw softkey and clock
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
PUBLIC int MMIAPICOM_SIN_S(int angle)
{
    angle %= 360;

    if(angle <= 90)
    {
        return sin_0_90_table[angle];
    }
    else if (angle <= 180)
    {
        return sin_0_90_table[180 - angle];
    }
    else if (angle <= 270)
    {
        return -sin_0_90_table[angle - 180];
    }
    else if (angle <= 360)
    {
        return -sin_0_90_table[360 - angle];
    }
    else 
    {
        return 0;
    }
}
/******************************************************************************/
//  Description : draw softkey and clock
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
PUBLIC int MMIAPICOM_COS_S(int angle)
{
    angle %= 360;

    if (angle <= 90)
    {
        return sin_0_90_table[90 - angle];
    }
    else if (angle <= 180)
    {
        return -sin_0_90_table[angle - 90];
    }
    else if (angle <= 270)
    {
        return -sin_0_90_table[270 - angle];
    }
    else if (angle <= 360)
    {
        return sin_0_90_table[angle - 270];
    }
    else
    {
        return 0;
    }
    
}

/*****************************************************************************/
//  Description : to get quadrantalpha
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_GetQuadrantAlpha(int angle)
{
    return ((360 + 90 - angle) % 360);
}
#if 0
/*****************************************************************************/
//  Description : 计算电子书名的哈希数
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:用于详情文件中电子书的索引表
//  Return:16位的哈希数
/*****************************************************************************/                      
PUBLIC uint16 MMIAPICOM_MakeTxtNameHash(
                             wchar* name,//IN: 书名
                             uint16 name_len//IN: 书名字节数
                             )
{
  
    uint16 name_hash = 0;
    uint32 des[4]={0};
    uint8 i=8;	
    MD5_CTX context ={0};
    uint16 * pBuf=(uint16 *)des;

    MD5Init (&context);
    MD5Update (&context, (unsigned char *)name, (unsigned int)(name_len<<2));
    MD5Final ((uint8*)des, &context);
    for(;i>0;i--)
    {
        name_hash+=pBuf[i-1]<<3;
	
    }	
    
    return name_hash;
}
#endif
/*****************************************************************************/
//  Description : to Count the ceiling of a value
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_CeilValue(                       //RETURN:
                      uint16    total_len,  //IN:
                      uint8     split_len   //IN:
                      )
{
    uint8   ret_val = 0;
    
    if (0 == split_len)
    {
        MMI_ASSERT_WARNING("0 != split_len");   /*assert verified*/
        return 0;
    }
    
    ret_val = ( 0 != ( total_len % split_len ) ) ? 1 : 0;
    
    ret_val += ( total_len / split_len );
    
    return (ret_val);
}

/*****************************************************************************/
//  Description : simulate float divid
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_FloatDivid(
                    uint32  num1, 
                    uint32  num2
                    )

{
    uint32 result = 0;

    result = num1 * FLOAT_PRECISION / num2;
    return result;
}

/*****************************************************************************/
//  Description : simulate float multiple
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_FloatMultiple(
                      uint32    num1, 
                      uint32   num2
                      )
{
    uint32 result = 0;

    result = (uint32)((num1 * num2) / FLOAT_PRECISION);
    return result;
}

/*****************************************************************************/
//  Description : This functions trace data,
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_TraceData(unsigned char *buf, int len)
{
    int i = 0;
    int n = 0;
    while(i<len)
    {
        n = len -i;
        if (n>=8)
        {
            //SCI_TRACE_LOW:"0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_DATA_1015_112_2_18_1_59_6_78,(uint8*)"dddddddd", buf[i],buf[i+1],buf[i+2],buf[i+3],buf[i+4],buf[i+5],buf[i+6],buf[i+7]);
            i+=8;
            SCI_Sleep(100);
        }
        else
        {
            while(n)
            {
                n = len -i ;
                //SCI_TRACE_LOW:"0x%02x,"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_DATA_1024_112_2_18_1_59_6_79,(uint8*)"d", buf[i]);
                i++;
            }
            //SCI_TRACE_LOW:"\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_DATA_1027_112_2_18_1_59_6_80,(uint8*)"");
            SCI_Sleep(100);
        }
    }
}

/*****************************************************************************/
//  Description : This functions switch binary and hex char
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BinaryToHexChar(uint8* src_ptr, uint16 src_len, uint8* dest_ptr, uint16* dest_len)
{
    uint8 szValue[ 4 ] = { 0 };
    int32 i = 0;    

    if (PNULL == src_ptr ||src_len == 0 || PNULL == dest_ptr || PNULL == dest_len)
    {
        return;
    }
    
    for(i = 0; i < src_len; ++i)
    {
        sprintf((char*)szValue, "%02x", *src_ptr);
        *dest_ptr = szValue[0];
        dest_ptr++;
        *dest_ptr = szValue[1];
        dest_ptr++;
        src_ptr++;
    }

    *dest_ptr = '\0';
    *dest_len = src_len * 2;
}

/*****************************************************************************/
//  Description : This functions switch hex and binary
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/ 
PUBLIC void MMIAPICOM_HexCharToBinary(uint8* src_ptr, int src_len, uint8* dest_ptr, uint16* dest_len)
{
    uint8 btValue = 0;
    uint16 nDesIdx = 0;
    int32 i = 0;

    if(PNULL == src_ptr || 0 >=src_len)
    {
        return ;
    }
    for( i = 0; i < src_len; ++i, ++nDesIdx)
    {
        btValue = MMIAPICOM_HexCharToByte( *(src_ptr + i) );
        *(dest_ptr + nDesIdx) = btValue << 4;
        ++i;
        *(dest_ptr + nDesIdx) |= MMIAPICOM_HexCharToByte( *(src_ptr + i) );          
    }
    *dest_len = src_len / 2;
}

 
/*****************************************************************************/
//  Description : This functions is Hex char to Byte
//  Global resource dependence : none
//  Author:      
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_HexCharToByte(uint8 cValue)
{
    uint8 btValue = 0;
    if( cValue >= '0' && cValue <= '9' )
    {
        btValue = cValue - '0';
    }
    else if( cValue >= 'a' && cValue <= 'f' )
    {
        btValue = cValue - 'a' + 10;
    }
    else if( cValue >= 'A' && cValue <= 'F' )
    {
        btValue = cValue - 'A' + 10;
    }
    else
    {
        //SCI_TRACE_LOW:"invalid cValue"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_DATA_1107_112_2_18_1_59_6_81,(uint8*)"");
    }
    return btValue;
}


/*****************************************************************************/
//  Description : This functions judge if the char is in the telephone number
//  Global resource dependence : none
//  Author:      
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsTelephoneNum(uint8 cNumber)
{    
    if (((cNumber  <= 0x39) && (cNumber >= 0x30))
        || cNumber == 0x2A  || cNumber == 0x23
        || cNumber == 0x50  || cNumber == 0x70
        || cNumber == 0x57  || cNumber == 0x77
        || cNumber == 0x4E  || cNumber == 0x6E
        || cNumber == 0x2B)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetAlign4Offset(uint32 x)
{
    uint32 z = 0;
    uint32 offset = 0;

    z = x % 4;
    if (z == 0) //do not need align
    {
        offset = 0;
    }
    else
    {
        offset = 4 - z;  //test: if x==7, offset = 4 - 3 = 1
    }

    return offset;
}

/*****************************************************************************/
//  Description : align 8
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetAlign8Offset(uint32 x)
{
    uint32 z = 0;
    uint32 offset = 0;

    z = x % 8;
    if (z == 0) //do not need align
    {
        offset = 0;
    }
    else
    {
        offset = 8 - z;  //test: if x==7, offset = 4 - 3 = 1
    }

    return offset;
}

#ifdef _SW_ZDT_PRODUCT_
/*****************************************************************************/
//  Description : string to int
//  Global resource dependence : 
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint64 MMIAPICOM_strToint(uint8 *str, uint8 len)
{
    uint64 ret_value = 0;
    uint64 dec = 1;
	uint8  j = 0;
	uint8  i = 0;

    while(len != 0)
	{
		for (i = 1; i < len; i++)
		{
			dec = dec * 10;
		}

		str[j] = str[j] - 48;

		if (str[j] != 0)
		{
			ret_value += str[j] * dec;
		}

		j++;
		len--;
		dec = 1;
	}

    return ret_value;
}

PUBLIC void MMIAPICOM_BcdICCIDToStr
(
    uint8*    pBcd,    // BCD码的头指针
    uint8    bcdLen,    // BCD码的长度
    char*    str        // 转换后String的头指针
)
{
    // BCD format - LSB first (1234 = 0x21, 0x43)
    int32 i = 0;
    // 对非法字符的号码，重新计算length，如Test、空字符"",将bcd Code按UNPACKED格式进行转换
    if ((pBcd[0] == 0xff) && (bcdLen >= 1))
    {
        uint8 upBcdLen = 0;
        
        upBcdLen = bcdLen >> 1;
        for (i = 1;i < upBcdLen;i++)
        {
            str[i - 1] = pBcd[i];
        }
        str[i - 1] = 0;
        return;
    }    
    
    for (i = 0;i < bcdLen;i++)
    {
        uint8    bcdCode = 0;
        uint8    ascCode = 0;    
    
        bcdCode = (pBcd[i / 2] >> (((i + 1) & 1) ? 0 : 4)) & 0x0F;
        
        SCI_TRACE_LOW("MMIAPICOM_BcdICCIDToStr: i = %d, bcdCode = %d", i, bcdCode);

        // 处理字符 A~F
        ascCode = (bcdCode == DIALBCD_STAR) ? 'A':
                  (bcdCode == DIALBCD_HASH) ? 'B':
                  (bcdCode == DIALBCD_PAUSE)? 'C':
                  (bcdCode == DIALBCD_WILD) ? 'D': 
                  (bcdCode == DIALBCD_EXPANSION) ? 'E':
                  (bcdCode == DIALBCD_FILLER) ? 'F':
                  (bcdCode + '0');

        if (!((ascCode >= '0' && ascCode <= '9') || 
            (ascCode >= 'A' && ascCode <= 'F') || 
            (ascCode >= 'a' && ascCode <= 'f')))
        {
            ascCode = 0;
        }
        str[i] = ascCode;
        
        SCI_TRACE_LOW("MMIAPICOM_BcdICCIDToStr: i = %d, ascCode = %d", i, ascCode);
    }

    str[i] = 0;
}
#endif


