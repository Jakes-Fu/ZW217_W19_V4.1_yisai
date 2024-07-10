#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"
#include "abup_typedef.h"
#include "abup_stdlib.h"

//for time stamp
#define  ABUP_DT_UTC_BASE_YEAR  1970
#define  ABUP_DT_MONTH_PER_YEAR    12
#define  ABUP_DT_DAY_PER_YEAR         365
#define  ABUP_DT_SEC_PER_DAY    86400
#define  ABUP_DT_SEC_PER_HOUR      3600
#define  ABUP_DT_SEC_PER_MIN    60
const abup_uint8 s_abup_dt_day_per_mon[ABUP_DT_MONTH_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define PNULL 0

abup_int32 abup_sprintf(abup_char * buffer, const abup_char *format, ...)
{
	abup_int32 iReturn;
	va_list Args;
	va_start(Args,format);
	iReturn = vsprintf(buffer,format,Args);
	va_end(Args);
	return iReturn;
}


abup_int32 abup_snprintf(abup_char * buffer, abup_int32 size,const abup_char *format, ...)
{
	abup_int32 iReturn;
	va_list Args;
	va_start(Args,format);
#ifndef WIN32	
	iReturn = vsnprintf(buffer,size,format,Args);
#else
	iReturn = _vsnprintf(buffer,size,format,Args);
#endif
	va_end(Args);
	return iReturn;
}


void * abup_memcpy(void * dest, const void * src, abup_uint32 count)
{
   if((dest!=NULL)&&(src!=NULL)&&(count!=0))
	return memcpy(dest,src,count);
   else
   	return NULL;
}

void * abup_memset(void * dest, char c, abup_uint32 count )
{
   if((dest!=NULL)&&(count!=0))
	return memset(dest,c,count);
   else
   	return NULL;
}

abup_int32 abup_memcmp(void * buf1, void * buf2, abup_uint32 count)
{
   if((buf1!=NULL)&&(buf2!=NULL)&&(count!=0))
	return memcmp((const void *)buf1,(const void *)buf2,count);
   else
   	return 0;
}


abup_char * abup_strcat(abup_char * strDestination, const abup_char * strSource )
{
   if((strDestination!=NULL)&&(strSource!=NULL))
	return strcat(strDestination,strSource);
   else
   	return NULL;
}


abup_char * abup_strncat(abup_char * strDestination, const abup_char * strSource,abup_uint32 len)
{
   if((strDestination!=NULL)&&(strSource!=NULL)&&(len!=0))
	return strncat(strDestination,strSource,len);
   else
   	return NULL;
}


abup_uint32 abup_strlen(const abup_char * str)
{
   if(str!=NULL)
	return (abup_uint32)strlen(str);
   else
   	return 0;
}

abup_int32 abup_strncmp(const abup_char * string1, abup_char * string2, abup_uint32 count)
{
	return strncmp(string1,string2,count);

}

abup_int32 abup_strcmp(const abup_char * string1, const abup_char * string2 )
{
	return strcmp(string1,string2); 	
}

abup_int32 abup_wstrcmp(const abup_wchar * string1, const abup_wchar * string2)
{
    abup_int32 ret = 0 ;
    
    if( string1 == PNULL && string2 == PNULL )
    {
        return 0;
    }
    else if( string1 == PNULL && string2 != PNULL )
    {
        return -1;
    }
    else if( string1 != PNULL && string2 == PNULL )
    {
        return 1;
    }
    else
    {
/*lint -save -e613 */
        ret = *(abup_wchar *)string1 - *(abup_wchar *)string2;/*lint !e613 */

        while( (!ret) && *string2 && *string1)/*lint !e613 */
        {
            ++string1;/*lint !e613 */
            ++string2;
            ret = *(abup_wchar *)string1 - *(abup_wchar *)string2;
        }
/*lint -restore */
        return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) ); 
    }
}


abup_char * abup_strcpy(abup_char * strDestination, const abup_char * strSource)
{
   if((strDestination!=NULL)&&(strSource!=NULL))
	return strcpy(strDestination, strSource);
   else
   	return NULL;
}

abup_char * abup_strncpy(abup_char * strDest, const abup_char * strSource, abup_uint32 count)
{
   if((strDest!=NULL)&&(strSource!=NULL)&&(count!=0))
	return strncpy(strDest,strSource,count);
   else
   	return NULL;
}


abup_char * abup_strstr(const abup_char * str, const abup_char * strSearch)
{
   if(str!=NULL)
	return strstr(str,strSearch);
   else
   	return NULL;
}

abup_char * abup_strchr(const abup_char * str, abup_int32 ch)
{
   if(str!=NULL)
	return strchr(str,ch);
   else
   	return NULL;
}



void abup_itoa(abup_uint32 i, abup_char *buf, abup_uint8 base)
{
	abup_char *s;
	#define ABUP_ITOA_LEN	20
	abup_int32 rem;
	static abup_char rev[ABUP_ITOA_LEN+1];

	rev[ABUP_ITOA_LEN] = 0;
	if (i == 0)
		{
		buf[0] = '0';
		++(buf);
		return;
		}
	s = &rev[ABUP_ITOA_LEN];
	while (i)
		{
		rem = i % base;
		if (rem < 10)
			*--s = rem + '0';
		else if (base == 16)
			*--s = "abcdef"[rem - 10];
		i /= base;
		}
	while (*s)
		{
		buf[0] = *s++;
		++(buf);
		}
}

abup_uint32 abup_atoi(abup_char * str)
{
 	if(str!=NULL)
		return (abup_uint32)atoi(str);
	else
		return 0;
}


static abup_uint8 abup_dt_is_leap_year(abup_uint16 year)
{
    if ((year % 400) == 0)
    {
        return 1;
    }
    else if ((year % 100) == 0)
    {
        return 0;
    }
    else if ((year % 4) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static abup_uint8 abup_dt_last_day_of_mon(abup_uint8 month, abup_uint16 year)
{
    if ((month == 0) || (month > 12))
    {
		return s_abup_dt_day_per_mon[1] + abup_dt_is_leap_year(year);
  	}
  	
    if (month != 2)
    {
        return s_abup_dt_day_per_mon[month - 1];
    }
    else
    {
        return s_abup_dt_day_per_mon[1] + abup_dt_is_leap_year(year);
    }
}

abup_uint32 abup_dt_curtime2utcsec(abup_time_struct *currTime)
{
    abup_uint16 i;
    abup_uint32 no_of_days = 0;
    abup_uint32 utc_time;
    abup_uint8 dst;

    if (!currTime || (currTime->nYear<ABUP_DT_UTC_BASE_YEAR))
    {
        return 0;
    }

    /* year */
    for (i = ABUP_DT_UTC_BASE_YEAR; i < currTime->nYear; i++)
    {
        no_of_days += (365 + abup_dt_is_leap_year(i));
    }

    /* month */
    for (i = 1; i < currTime->nMonth; i++)
    {
        no_of_days += abup_dt_last_day_of_mon((abup_uint8) i, currTime->nYear);
    }

    /* day */
    no_of_days += (currTime->nDay - 1);

    /* sec */
    utc_time = (abup_uint32) no_of_days *ABUP_DT_SEC_PER_DAY + \
    				(abup_uint32) (currTime->nHour * ABUP_DT_SEC_PER_HOUR +
                     currTime->nMin * ABUP_DT_SEC_PER_MIN + currTime->nSec);
	
	if(currTime->nYear < 2002)
		utc_time += 0x30000000;

    return utc_time;
}

