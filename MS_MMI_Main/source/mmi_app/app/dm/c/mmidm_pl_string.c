/*************************************************************************
 ** File Name:      mmidm_pl_string.c                                    *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/03                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/03     jian.ma          Create.                              *
*************************************************************************/
#define _MMIDM_PL_STRING_C_
//#include "std_header.h"
#include "mmi_app_dm_trc.h"
#ifdef DM_SUPPORT
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mmi_nv.h"
#include "vdm_pl_types.h"
#include "vdm_pl_stdlib.h"
#include "vdm_pl_string.h"
#include "vdm_pl_string_utils.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_IU32_DIV_TEN	( (uint32)0xffffffff / 10 )
#define MAX_IU32_MOD_TEN	( (uint32)0xffffffff % 10 )
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

//This for the part of vdm_pl_stdlib
/*!
*******************************************************************************
//  Description : 
* Convert a NULL terminated byte character string to an integer. This function
* is only required to handle positive integer decimal or hexadecimal strings.
* A sign character ('+') is considered to be an illegal character.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string		The source string
 *
 * \param	radix		The base to use for conversion (10 decimal, 16 hex)
 *
 * \retval	success		Where to store a success/failure flag. If this
 *						parameter is NULL, success/failure is not returned.
 *
 * \return	The IU32 value
*******************************************************************************
 */
IU32 VDM_PL_atoIU32(const char *string, IU8 radix, IBOOL *success)
{
	uint32	myVal = 0;
	char	maxLastDigit = (char)('0' + MAX_IU32_MOD_TEN);
	int16   digitsParsed = 0;
	char	nextChr = 0;
	IBOOL	result = FALSE;
	
	//SCI_TRACE_LOW("MMIDM: RDM_atoIU32");

    if (PNULL == string)
    {
        return result;
    }
	if (10 == radix)
	{
		/*
		 * This code will allow the following to be considered legal.
		 * '0003' parses to 3
		 * '0000000000000000018' parses to 18
		 * '+0' is illegal
		 * '-0' is illegal
		 */
		nextChr = *string++;

		while((nextChr >= '0') && (nextChr <= '9'))
		{
			if ( (myVal > MAX_IU32_DIV_TEN) ||
				 ((myVal == MAX_IU32_DIV_TEN) && (nextChr > maxLastDigit)) )
			{
				/* Value too large */
				break;
			}
			myVal *= 10;
			myVal += (nextChr - (char)'0');/*lint !e737 */
			nextChr = *string++;
			++digitsParsed;
		}

		/* Check that
		 * - We DID parse at least 1 digit
		 * - We've reached the NUL. If we haven't, there are either invalid
		 *	 characters following the value, or the value is too large to fit
		 *	 into an IU32.
		 */
		if (( '\0' == nextChr ) && (0 < digitsParsed))
		{
			result = TRUE; /* Success */
		}
	}
	else
	{
		uint8		cVal;

		/* Any other radix is treated as 16 */

		if (('0' == *string) && (('x' == *(string+1)) || ('X' == *(string+1))))
			string += 2;

		cVal = (uint8)*string++;

		while ('\0' != cVal)
		{
			if (('0' <= cVal) && (cVal <= '9'))
			{
				myVal = (myVal << 4) | (cVal - '0');
			}
			else if (('A' <= cVal) && (cVal <= 'F'))
			{
				myVal = (myVal << 4) | (cVal - 'A' + 10);
			}
			else  if (('a' <= cVal) && (cVal <= 'f'))
			{
				myVal = (myVal << 4) | (cVal - 'a' + 10);
			}
			else
				break;

			if (digitsParsed++ > 8)
				break;

			cVal = *string++;
		}

		/*
		 * Parsing a hexadecimal value is made stricter than a decimal value
		 * Maximum of 8 digits,
		 * All the characters must be valid hexadecimal digits
		 * Allows the value to start '0x' or '0X'
		 * No signs allowed
		 */
		if ( (0 < digitsParsed) && (digitsParsed < 9) && ('\0' == cVal) )
		{
			result = TRUE; /* Succesfully parsed the value */
		}
	}

	/* Return a success/fail indication */
	if (success)
    {
        *success = result;
    }
	return myVal;
}



/*!
*******************************************************************************
//  Description : 
 * Convert an integer value into a decimal or hexadecimal string
 * representation. The supplied buffer should be large enough to receive the
 * generated string (11 bytes for decimal, 9 bytes for hexadecimal). Note that
 * this function does NOT prepend the value with '0x' for hexadecimal values.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	value		The integer value to convert
 *
 * \param	radix		The base to use for conversion (10 decimal, 16 hex)
 *
 * \retval	string		Pointer to a buffer to receive the ascii
 *						representation.
*******************************************************************************
 */
void VDM_PL_IU32toa(IU32 value, char *string, IU8 radix)
{
	int16		i = 0;
	uint32	v = value;

    if (PNULL == string)
    {
        //SCI_TRACE_LOW:"MMIDM VDM_PL_IU32toa PNULL == string"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_STRING_196_112_2_18_2_14_24_823,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW("MMIDM VDM_PL_IU32toa");
	if (16 == radix) {

		do
		{
			v = v >> 4;
			++i;
		} while(v != 0);

		/*
		 * Add NUL terminator.
		 */
		string[i] = '\0';

		/*
		 * Write the digits in reverse order.
		 */
		for ( --i; i >= 0; --i )
		{
			char ch = (char)('0' + (value & 0xF));
			if ( ch > '9' )
			{
				ch = (char)(ch + 'A' - ('9' + 1));
			}

			string[i] = ch;
			value = value >> 4;
		}

	} else {
		/*
		 * Any other radix will be treated as decimal
		 */

		/*
		 * Work out the number of digits.
		 */
		do
		{
			i++;
			v /= 10;
		}
		while ( v != 0 );

		/*
		 * Add NUL terminator.
		 */
		string[i] = 0;

		/*
		 * Write the digits in reverse order.
		 */
		for ( --i; i >= 0; --i )
		{
			string[i] = (char)('0' + value % 10);
			value /= 10;
		}
	}
}



//This for the part of vdm_pl_string
/*!
 *******************************************************************************
//  Description : Calculates the length of a null terminated byte character string.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string		The string.
 *
 * \return	The length of the string in bytes.
 *******************************************************************************
 */
IU32 VDM_PL_strlen(const char *string)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strlen len = %d",strlen(string));
	return strlen(string);
}

/*!
 *******************************************************************************
//  Description : Search for a character in a null terminated byte character string.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string		The string.
 * \param	ch			The character.
 *
 * \return	A pointer to the first occurrence of the character in the string,
 *			or NULL if the character is not found.
 *******************************************************************************
*/
char *VDM_PL_strchr(const char *string, char ch)
{
	return strchr(string, ch);/*lint !e605*/
}

/*!
 *******************************************************************************
//  Description : Reverse search for a character in a null terminated byte character string.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string		The string.
 * \param	ch			The character.
 *
 * \return	A pointer to the last occurrence of the character in the string,
 *			or NULL if the character is not found.
 *******************************************************************************
 */
char *VDM_PL_strrchr(const char *string, char ch)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strrchr");
	return strrchr(string, ch);/*lint !e605*/
}


/*!
 *******************************************************************************
//  Description : Compares two null terminated byte character strings.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string1		A string.
 * \param	string2		A string.
 *
 * \retval	  0		if the \a string1 == \a string2
 * \retval	< 0		if \a string1 is lexically less than \a stri < ng2
 * \retval	> 0		if \a string1 is lexically greater than \a string2
 *******************************************************************************
 */
IS32 VDM_PL_strcmp(const char *string1, const char *string2)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strcmp");
	return strcmp(string1, string2);
}


/*!
 *******************************************************************************
//  Description : 
 * Compares up to \a len characters between the array pointed to by \a string1
 * and the array pointed to by \a string2. Characters that follow a null
 * character are not compared.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string1		A string.
 * \param	string2		A string.
 * \param	len			Number of characters to compare
 *
 * \retval	  0		if the \a string1 == \a string2
 * \retval	< 0		if \a string1 is lexically less than \a string2
 * \retval	> 0		if \a string1 is lexically greater than \a string2
 *******************************************************************************
 */
IS32 VDM_PL_strncmp(const char *string1, const char *string2, IU32 len)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strncmp");
	return strncmp(string1, string2, len);
}


/*!
 *******************************************************************************
//  Description : Copy a null terminated byte character string to the specified destination.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	dest		The destination location.
 * \param	src			The src string.
 *
 * \return	A pointer to the destination null terminated byte character string.
 *******************************************************************************
 */
char *VDM_PL_strcpy(char *dest, const char *src)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strcpy");
	return strcpy(dest, src);
}


/*!
 *******************************************************************************
//  Description :
 * Copy \a len characters from a null terminated byte character string to the
 * specified destination.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	dest		The destination location.
 * \param	src			The src string.
 * \param	len			The number of characters to copy
 *
 * \return	A pointer to the destination character string.
 *******************************************************************************
 */
char *VDM_PL_strncpy(char *dest, const char *src, IU32 len)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strncpy");
	return strncpy(dest, src, len);
}


/*!
 *******************************************************************************
//  Description :
 * Concatenate a null terminated byte character string to the specified null
 * terminated byte character string.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	dest		The destination string.
 * \param	src			The src string to concatenate.
 *
 * \return	A pointer to the destination null terminated byte character string.
 *******************************************************************************
 */
char *VDM_PL_strcat(char *dest, const char *src)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strcat");
	return strcat(dest, src);
}



/*!
 *******************************************************************************
//  Description : Compare two null terminated byte character strings ignoring case.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string1		A string.
 * \param	string2		A string.
 *
 * \return	0 if string1 == string2
 *			< 0 if string1 < string2
 *			> 0 if string1 > string2
 *******************************************************************************
 */
IS32 VDM_PL_strcasecmp(const char *string1, const char *string2)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strcasecmp");
	return strcmp(string1, string2);
}

/*!
 *******************************************************************************
//  Description :
 * Compare len characters of two null terminated byte character strings ignoring
 * case.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string1		A string.
 * \param	string2		A string.
 * \param	len			Number of characters to compare
 *
 * \return	0 if string1 == string2
 *			< 0 if string1 < string2
 *			> 0 if string1 > string2
 *******************************************************************************
 */
IS32 VDM_PL_strncasecmp(const char *string1, const char *string2, IU32 len)
{
    IU32 ii=0;
    //SCI_TRACE_LOW("MMIDM==> VDM_PL_strncasecmp len =%d",len);
    for(;ii<len;ii++)
    {
        if( tolower(string1[ii]) < tolower(string2[ii]))/*lint !e718 */
        {
          return -1;  
        }
        else if( tolower(string1[ii]) > tolower(string2[ii]))/*lint !e718 */
        {
            return 1;  
        }
        
    }    
	return 0;
}

/*!
 *******************************************************************************
//  Description : Find substring
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	haystack	A string.
 * \param	needle		A string.
 *
 * \return	pointer to needle in haystack, or 0 if not found 
 *******************************************************************************
 */
char *VDM_PL_strstr(const char *haystack, const char *needle)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strstr");
	return strstr(haystack, needle);/*lint !e605*/
}


/*!
 *******************************************************************************
//  Description : Convert string to lower case
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
 * \param	string		A string.
 *
 * \return	pointer to converted string
 *******************************************************************************
 */
char *VDM_PL_strlwr(const char *string)
{
    //SCI_TRACE_LOW("MMIDM VDM_PL_strstr");
    return MMIAPICOM_Strlower((char*)string);
}

//This for the part of vdm_pl_string_utils
/*!
*******************************************************************************
//  Description : Write formatted data to a string.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
* \param	inBuffer	Result buffer.
* \param	inCount		Maximum number of characters to  produce.
* \param	inFormat	Format-control string.
* \param	...			Argument list.
*
* If inCount is too short then the result string is truncated.
*
* \return	The number of characters written if the number of characters to 
*			write is less than or equal to inCount; if the number of characters 
*			to write is greater than inCount, return -1 indicating that output
*			has been truncated. 
*
* The return value does not include the terminating null, if one is written.
*******************************************************************************
*/
IS32 VDM_PL_snprintf(char *inBuffer, IS32 inCount, char *inFormat, ...)
{
	int32 result = 0;
	va_list argp;

	va_start(argp, inFormat);/*lint !e718 !e64 */

	result = _vsnprintf(inBuffer, inCount, inFormat, argp);

	va_end(argp);

	return result;
}


/*!
*******************************************************************************
//  Description : Write formatted data to a string.
* Joins a number of strings together to form one long string, 
* with the optional separator inserted between each of them.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
* \param	separator	A string to insert between each of the strings, 
*						or NULL.
* \param	...			A NULL-terminated list of strings to join
*
* \return	A newly-allocated string containing all of the strings joined
*			together with separator between them.
*
* example:	char *str = VDM_PL_strjoin(",", "Hello", " world!", (char *)NULL);
*******************************************************************************
*/
char* VDM_PL_strjoin(char* separator, ...)
{
	size_t len = 0;
	size_t separator_len = 0;
	char *retbuf = PNULL;
	va_list argp;
	char *p;

    //SCI_TRACE_LOW("MMIDM VDM_PL_strstr");
	if(separator == NULL)
    {
        return separator = "";
    }
	separator_len = strlen(separator);
	va_start(argp, separator);/*lint !e718 !e64 !e746 !e628*/
	p = va_arg(argp, char *);/*lint !e718 !e64 !e746 !e628*/
	if (p)
	{
		/* calculate total length of result string*/

		len = 1 + strlen(p); /* +1 for trailing \0 */

		while((p = va_arg(argp, char *)) != NULL)/*lint !e718 !e64 !e746 !e628*/
		{
			len += separator_len + strlen(p);
		}
		va_end(argp);

		/* now build the result string */
		retbuf = SCI_ALLOC_APP(len);
		if(retbuf == NULL)
        {
            return NULL;		/* error */
        }
			

		/* copy first string */
		va_start(argp, separator);/*lint !e718 !e64 !e746 !e628*/
		p = va_arg(argp, char *);/*lint !e718 !e64 !e746 !e628*/
		strcpy(retbuf, p);

		/* concatenate the rest of the strings 
		   separated by the separator */
		while((p = va_arg(argp, char *)) != NULL)/*lint !e718 !e64 !e746 !e628*/
		{
			strcat(retbuf,separator);
			strcat(retbuf, p);
		}
	}
	else
	{
        retbuf = SCI_ALLOC_APP(1);
        memset(retbuf,0x00,1);
	}
	va_end(argp);

	return retbuf;
}
#endif
