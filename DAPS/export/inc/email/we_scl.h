/*=====================================================================================

    HEADER NAME : we_scl.h
    MODULE NAME : WE Framework SCL(Similar C Library)

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file can be included by all WE Programmer.In this file all scl interfaces and 
    related macro is defined.
        These Info is  aptotic and is not changed by switching Project.

    TECHFAITH Wireless Confidential Proprietary
    (c) Copyright 2006 by TechSoft. All Rights Reserved.
=======================================================================================
    Revision History
   
    Modification                 Tracking
    Date           Author         Number        Description of changes
    ----------   ------------    ---------   --------------------------------------------
    2006-05-28   HiroWang				       create this file

    Self-documenting Code
    Describe/explain low-level design, especially things in header files, of this module and/or group 
    of functions and/or specific function that are hard to understand by reading code and thus requires 
    detail description.
    Free format !

=====================================================================================*/
/*--------------------------START-----------------------------*/


/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef WE_SCL_H
#define WE_SCL_H
 
#ifdef WIN32
#define  WIN32_DEBUG
#endif
/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/
#include "we_def.h"
#include "tf3rd_for_HZ.h"


/*Include Module head file*/
//#define WE_DEBUG_INFO
/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/

/*The macro of the base64 decode table*/
#define _UC    0x1u            /* Upper case: A-Z */
#define _LC    0x2u            /* Lower case: a-z */
#define _D     0x4u            /* Digit: 0-9 */
#define _X     0x8u            /* Hex digit: a-f, A-F */
#define _M     0x10u           /* Mark: -_.!~*'() */
#define _SRES  0x20u           /* Sometimes reserved: :@&=+$, */
#define _ARES  0x40u           /* Always reserved: ;/? */
#define _DL    0x80u           /* Delimiters: <>#%" */
#define _UW    0x100u          /* Unwise: {}|\^[]` */
#define _WS    0x200u          /* Whitespace: HT, VT, CR, LF, SP, FF */
#define _SP    0x400u          /* Space: ' ' */
#define _B     0x800u          /* Blank: SP TAB */
#define _C     0x1000u         /* Ctrl character: 0x0-0x1f, 0xff */
#define _T     0x2000u         /* Tspecials: ()<>@,;:\"/[]?={}SPHT */
#define _ESC   0x4000u         /* Escape character: % */
#define _XWS   0x8000u         /* XML whitespace character: HT, CR, LF, SP */

/*The error return value*/
#define STR_HASH_ERROR                           0xFFFFFFFF

/* Different search methods for finding an integer in a string table. */
#define WE_TABLE_SEARCH_INDEXING                 1
#define WE_TABLE_SEARCH_BINARY                   2
#define WE_TABLE_SEARCH_LINEAR                   3

/*Define Constant Macro end*/


/*Define Macro Function start*/
/*The macro to get the value from the table*/
#define CT_ISASCII              ct_isascii
#define CT_ISALPHA              ct_isalpha           
#define CT_ISALPHANUM           ct_isalphanum        
#define CT_ISLOWER              ct_islower           
#define CT_ISUPPER              ct_isupper           
#define CT_ISDIGIT              ct_isdigit        
#define CT_ISHEX                ct_ishex           
#define CT_ISHEXLETTER          ct_ishexletter       
#define CT_ISBLANK              ct_isblank   
#define CT_ISWHITESPACE         ct_iswhitespace
#define CT_ISXMLWHITESPACE      ct_isxmlwhitespace
#define CT_ISCTRL               ct_isctrl
#define CT_ISESCAPE             ct_isescape
#define CT_ISRESERVED           ct_isreserved
#define CT_ISEXCLUDED           ct_isexcluded
#define CT_ISDELIMITER          ct_isdelimiter
#define CT_ISUNWISE             ct_isunwise
#define CT_ISTSPECIAL           ct_istspecial
#define CT_ISSPECIAL            ct_isspecial
#define CT_ISPCHAR              ct_ispchar
#define CT_ISURIC               ct_isuric

#define ct_isascii(c)           ((WE_UCHAR)(c) < 0x80)
#define ct_isalpha(c)           ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_UC | _LC))
#define ct_isalphanum(c)        ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_UC | _LC | _D))
#define ct_islower(c)           ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _LC)
#define ct_isupper(c)           ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _UC)
#define ct_isdigit(c)           ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _D)
#define ct_ishex(c)             ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_D | _X))
#define ct_ishexletter(c)       ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _X)
#define ct_isblank(c)           ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_B))
#define ct_iswhitespace(c)      ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _WS)
#define ct_isxmlwhitespace(c)   ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _XWS)
#define ct_isctrl(c)            ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _C)
#define ct_isescape(c)          ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _ESC)
#define ct_isreserved(c)        ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_SRES | _ARES))
#define ct_isexcluded(c)        ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_C | _SP | _DL))
#define ct_isdelimiter(c)       ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _DL)
#define ct_isunwise(c)          ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _UW)
#define ct_istspecial(c)        ((WeSCL_GetCharType((WE_UCHAR)(c)))  & _T)
#define ct_isspecial(c)         ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_C | _SP | _SRES | _ARES | _UW | _DL))
#define ct_ispchar(c)           ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_UC | _LC | _D | _M | _SRES))
#define ct_isuric(c)            ((WeSCL_GetCharType((WE_UCHAR)(c)))  & (_SRES | _ARES | _UC | _LC | _D | _M))

/*Define Macro Function end*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*Define base type start*/
/*Define base type end*/

/*Define Enum type start*/
/*Define Enum type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/

/* A string table is an array of elements of this type: */
typedef struct tagSt_WeSCLStrTableEntry
{
    WE_CHAR         *pcName;
    WE_INT32        iValue;
} St_WeSCLStrTableEntry, *P_St_WeSCLStrTableEntry;

/* A string table should be complemented with a hash table (automatically generated), 
and all the information about the tables is collected in a structure of this kind: */
typedef struct tagSt_WeSCLStrtableInfo
{
    St_WeSCLStrTableEntry           *pstStringTable;
    WE_UCHAR                        *pucHashTable;
    WE_INT32                        iHashTableSize; 
    WE_INT32                        iStringTableSize;
    WE_INT16                        sSearchMethod;
    WE_CHAR                         acReserved[2];
} St_WeSCLStrtableInfo, *P_St_WeSCLStrtableInfo;

/*The duration structure*/
typedef struct tagSt_WeSCLDuration
{
    WE_UINT32          uiYears;
    WE_UINT32          uiMonths;
    WE_UINT32          uiDays;
    WE_UINT32          uiHours;
    WE_UINT32          uiMinutes;
    WE_UINT32          uiSeconds;
} St_WeSCLDuration, *P_St_WeSCLDuration;

/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

/****String Operater Start****/
/*====================================================================================
FUNCTION: WeSCL_GetCharType
              
CREATE DATE: 2006-06-02
AUTHOR: Yang Min
        
DESCRIPTION:
    Get the character type value by the character input.
              
ARGUMENTS PASSED:
    None.          
RETURN VALUE:
    Return the character type value if success, else return USER_ERORR.      

USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to get the correspond value in the character type table.
IMPORTANT NOTES:
    When function call the macro function to get the character type from the character type
    table, it will call this function indirectly.       
=======================================================================================*/
#define  WE_SCL_GETCHARTYPE   WeSCL_GetCharType
WE_UINT16 WeSCL_GetCharType
(
   WE_UINT16 usCharacter
);

/*=====================================================================================
FUNCTION: 
    WeSCL_StrDup
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Duplicate string.
ARGUMENTS PASSED:
    pcStrSource: a string is duplicated by this function.
RETURN VALUE:
    Each of these functions returns a pointer to the storage location for the copied 
    string or NULL if storage cannot be allocated.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need duplicate a string.
IMPORTANT NOTES:
    The WeSCL_StrDup function calls MALLOC to allocate storage space for a copy of 
    pcStrSource and then copies pcStrSource to the allocated space.
    Because WeSCL_StrDup calls malloc to allocate storage space for the copy of 
    pcStrSource,it is good practice always to release this memory by calling the 
    FREE routine on the pointer returned by the call to WeSCL_StrDup.      
  
=====================================================================================*/
#define WE_SCL_STRDUP        TF3RD_STRDUP

/*=====================================================================================
FUNCTION: 
    WeSCL_StrnDup
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Duplicate string.
ARGUMENTS PASSED:
    pcStrSource: a string is duplicated by this function.
    iLen:Length of string needing be copied.
RETURN VALUE:
    Each of these functions returns a pointer to the storage location for the copied 
    string or NULL if storage cannot be allocated.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need duplicate a string.
IMPORTANT NOTES:
    The WeSCL_Strndup function calls MALLOC to allocate storage space for a copy of 
    pcStrSource and then copies specify length pcStrSource to the allocated space.
    Because WeSCL_Strndup calls MALLOC to allocate storage space for the copy of 
    pcStrSource,it is good practice always to release this memory by calling the 
    FREE routine on the pointer returned by the call to WeSCL_Strndup.      
=====================================================================================*/
#define WE_SCL_STRNDUP  WeSCL_StrnDup
WE_CHAR * WeSCL_StrnDup   /*need writed*/
(
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
);

/*=====================================================================================
FUNCTION: 
    WeSCL_StrCat
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Connect two string and Create a new string.
ARGUMENTS PASSED:
    pcStrSource1: First string is connected by this function.
    pcStrSource2: Second string is connected by this function.
RETURN VALUE:
    Each of these functions returns a pointer to the storage location for the connected 
    two string or NULL if storage cannot be allocated.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need connect two string.
IMPORTANT NOTES:
    The WeSCL_Strcat function calls MALLOC to allocate storage space for a connective 
    of pcStrSource1&pcStrSource2 .
    Because WeSCL_Strcat calls MALLOC to allocate storage space for the connective 
    of pcStrSource1&pcStrSource2,it is good practice always to release this memory 
    by calling the FREE routine on the pointer returned by the call to WeSCL_Strcat. 
  
=====================================================================================*/
#define WE_SCL_STRCAT  TF3RD_STRCAT
WE_CHAR * WeSCL_StrCat
(
const WE_CHAR *pcStrSource1,
const WE_CHAR *pcStrSource2
);

/*=====================================================================================
FUNCTION: 
    WeSCL_StrnCat
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Connect two string and Create a new string.
ARGUMENTS PASSED:
    pcStrSource1: First string is connected by this function.
    pcStrSource2: Second string is connected by this function.
    iLen2:Length of chars needed connect in pcStrSource2.
RETURN VALUE:
    Each of these functions returns a pointer to the storage location for the connected 
    two string or NULL if storage cannot be allocated.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need connect two string.
IMPORTANT NOTES:
    The WeSCL_StrnCat function calls MALLOC to allocate storage space for a connective 
    of pcStrSource1&pcStrSource2 .
    Because WeSCL_StrnCat calls MALLOC to allocate storage space for the connective 
    of pcStrSource1&pcStrSource2,it is good practice always to release this memory 
    by calling the FREE routine on the pointer returned by the call to WeSCL_StrnCat. 
  
=====================================================================================*/
#define WE_SCL_STRNCAT  WeSCL_StrnCat
WE_CHAR * WeSCL_StrnCat
(
    const WE_CHAR *pcStrSource1, 
    const WE_CHAR *pcStrSource2, 
    WE_INT32 iLen2
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Str3Cat
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Connect three strings and Create a new string.
ARGUMENTS PASSED:
    pcStrSource1: First string is connected by this function.
    pcStrSource2: Second string is connected by this function.
    pcStrSource2: Third string is connected by this function.
RETURN VALUE:
    Each of these functions returns a pointer to the storage location for the connected 
    three string or NULL if storage cannot be allocated.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need connect three string.
IMPORTANT NOTES:
    The WeSCL_Str3Cat function calls MALLOC to allocate storage space for a connective 
    of pcStrSource1&pcStrSource2&pcStrSource3.
    Because WeSCL_Str3Cat calls MALLOC to allocate storage space for the connective 
    of pcStrSource1&pcStrSource2&pcStrSource3,it is good practice always to release 
    this memory by calling the FREE routine on the pointer returned by the call to 
    WeSCL_Str3Cat. 
  
=====================================================================================*/
#define WE_SCL_STR3CAT  WeSCL_Str3Cat
WE_CHAR * WeSCL_Str3Cat
(
    const WE_CHAR *pcStrSource1, 
    const WE_CHAR *pcStrSource2, 
    const WE_CHAR *pcStrSource3
);

/*=====================================================================================
FUNCTION: 
    WeSCL_StrnChr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Find a character in a string at first occurrence position.
ARGUMENTS PASSED:
    pcStrSource: Specify string found char.
    cCharFound: Specify char found in a string.
    iLen: Find char in a string after at most 'iLen' characters.
RETURN VALUE:
    Return a pointer to the first occurrence of the character 'cCharFound'
    in the string 'pcStrSources'. The search stops at a null-byte or after
    at most 'iLen' characters.If not found return NULL.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need find char in a string.
IMPORTANT NOTES:
    None. 
  
=====================================================================================*/
#define WE_SCL_STRNCHR  WeSCL_StrnChr
WE_CHAR * WeSCL_StrnChr
(
    const WE_CHAR *pcStrSource,
    WE_CHAR cCharFound,
    WE_INT32 iLen
);

/*=====================================================================================
FUNCTION: 
	WeSCL_StrCpyLc
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Bill Huang
DESCRIPTION:
	Copy the null-terminated string from source buffer to destination buffer, while changing all 
	uppercase letters to lowercase letters.
ARGUMENTS PASSED:
	pcStrSource : Source string where the data is read from.
	pcStrDest   : Destination string where the data is passed into.
RETURN VALUE:
	void
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES:
	None
CALL BY:
	All program need copy null-terminated string from one buffer to a specific buffer
	IMPORTANT NOTES:
Non-ASCII characters are not affected with this function.
	None
=====================================================================================*/
#define WE_SCL_STRCPYLC  WeSCL_StrCpyLc
void WeSCL_StrCpyLc
(
	WE_CHAR *pcStrDest,
	const WE_CHAR *pcStrSource
);

 /*=====================================================================================
 FUNCTION: 
	WeSCL_StrnCpyLc
 CREATE DATE: 
	2006-05-30
 AUTHOR: 
	Bill Huang
 DESCRIPTION:
	 Copy a null-terminated string from source buffer to destination buffer with specific string
	length, while changing all uppercase letters to lowercase.
 ARGUMENTS PASSED:
	pcStrSource : Source string where the data is read from.
	pcStrDest   : Destination string where the data is passed into.
	iLen        : Length of string needing be copied.
 RETURN VALUE:
	void
 USED GLOBAL VARIABLES:
	None
 USED STATIC VARIABLES:
	None
 CALL BY:
	All program need copy null-terminated string with specific length from one buffer to a specific buffer
 IMPORTANT NOTES:
	Non-ASCII characters are not affected with this function.
 
=====================================================================================*/
#define WE_SCL_STRNCPYLC  WeSCL_StrnCpyLc
void WeSCL_StrnCpyLc
(
	WE_CHAR *pcStrDest,
	const WE_CHAR *pcStrSource,
	WE_INT32 iLen
);

 /*=====================================================================================
 FUNCTION: 
	WeSCL_StrnCpy
 CREATE DATE: 
	2006-05-30
 AUTHOR: 
	Bill Huang
 DESCRIPTION:
	 Copy a null-terminated string from source buffer to destination buffer with specific string
	length, while changing all uppercase letters to lowercase.
 ARGUMENTS PASSED:
	pcStrSource : Source string where the data is read from.
	pcStrDest   : Destination string where the data is passed into.
	iLen        : Length of string needing be copied.
 RETURN VALUE:
	void
 USED GLOBAL VARIABLES:
	None
 USED STATIC VARIABLES:
	None
 CALL BY:
	All program need copy null-terminated string with specific length from one buffer to a specific buffer
 IMPORTANT NOTES:
	Non-ASCII characters are not affected with this function.
 
=====================================================================================*/
#define WE_SCL_STRNCPY  TF3RD_STRNCPY

/*=====================================================================================
FUNCTION: 
	WeSCL_ChrLc
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Bill Huang
DESCRIPTION:
	Change a character to lowercase.
ARGUMENTS PASSED:
	cChr : the character which will be changed to lowercase.  
RETURN VALUE:
	he lowercase version of a character
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES:
	None
CALL BY:
	All program need to change a uppercase character to a lowercase character.
IMPORTANT NOTES:
	Non-ASCII characters are not affected with this function.

=====================================================================================*/
#define WE_SCL_CHRLC  WeSCL_ChrLc
WE_CHAR WeSCL_ChrLc
(
    WE_CHAR cChr
);

/*=====================================================================================
FUNCTION: 
	WeSCL_StrCmpNc
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Bill Huang
DESCRIPTION:
	Perform a string comparison between two strings, but without changing the uppercase to lowercase.
ARGUMENTS PASSED:
	pcStrSource1 :  First string which is compared.
	pcStrSource2 :  Second String which is compared.
RETURN VALUE:
	The first different character's ASCII value's difference in two strings.
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES:
	None
CALL BY:
	All program need to compare two strings
IMPORTANT NOTES:
	only upper and lower case versions of ASCII characters are treated as equal!

=====================================================================================*/
#define WE_SCL_STRCMPNC  WeSCL_StrCmpNc
WE_INT32 WeSCL_StrCmpNc
(
    const WE_CHAR *pcStrSource1,
    const WE_CHAR *pcStrSource2
);

/*=====================================================================================
FUNCTION: 
    WeSCL_StrnCmpNc
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Bill Huang
DESCRIPTION:
	Perform a string comparison between two strings with specific length, but without 
	changing the uppercase to lowercase.
ARGUMENTS PASSED:
	pcStrSource1 :  First string which is compared.
	pcStrSource2 :  Second String which is compared.
	iLen         :  Length of string needs to compare.
RETURN VALUE:
	The first different character's ASCII value's difference in two strings.
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES:
	None
CALL BY:
	All program need to compare two strings with specific string length.
IMPORTANT NOTES:
	only upper and lower case versions of ASCII characters are treated as equal!

=====================================================================================*/
#define WE_SCL_STRNCMPNC  WeSCL_StrnCmpNc
WE_INT32 WeSCL_StrnCmpNc
(
    const WE_CHAR *pcStrSource1,
    const WE_CHAR *pcStrSource2,
    WE_INT32 iLen
);

/*=====================================================================================
FUNCTION: 
	WeSCL_Byte2Hex
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Bill Huang
DESCRIPTION:
	Change a binary value representation character to a hex value representation character. 
ARGUMENTS PASSED:
	ucByte        :  The character which will be changed.
	pucHexLocation  :  The location that the changed character written into.
RETURN VALUE:
	void
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES: 
	None
CALL BY:
	All program need to change a binary value representation character to a hex value 
	representation character.
IMPORTANT NOTES:  
	None
=====================================================================================*/
#define WE_SCL_BYTE2HEX  WeSCL_Byte2Hex
void WeSCL_Byte2Hex
(
    WE_UINT8 ucByte,
    WE_CHAR *pcStrSource
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Hex2Byte
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Bill Huang
DESCRIPTION:
    Change hex value representation string to binary value representation string. 
ARGUMENTS PASSED:
    pcHexLocation  :  The location that store the hex value representation string.  
    pucByteLocation   :  The location that store the binary value representation string.
RETURN VALUE:
    Returns FALSE if the string 'pcHexSource' does not start with two 
    hexadecimal digits, and TRUE otherwise.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES: 
    None
CALL BY:
    All program need to change hex value representation string to binary value representation string. 
IMPORTANT NOTES: 
    None
=====================================================================================*/
#define WE_SCL_HEX2BYTE  WeSCL_Hex2Byte
WE_BOOL WeSCL_Hex2Byte
(
    const WE_CHAR *pcHexSource,
    WE_UCHAR *pucByteDest
);

/*=====================================================================================
FUNCTION: 
	WeSCL_Hex2Int
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Bill Huang
DESCRIPTION:
	Change hex value representation string to integer value representation string. 
ARGUMENTS PASSED:
	pcHexLocation  :  The location that store the hex value representation string.  
RETURN VALUE:
	Return the integer value representation string.
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES: 
	None
CALL BY:
	All program need to change hex value representation string to integer value representation string. 
IMPORTANT NOTES:  
	None
=====================================================================================*/
#define WE_SCL_HEX2INT  WeSCL_Hex2Int
WE_INT32 WeSCL_Hex2Int
(
    const WE_CHAR *pcHexLocation
);
 /*=====================================================================================
 FUNCTION: 
	WeSCL_Hex2Long
 CREATE DATE: 
	2006-05-30
 AUTHOR: 
	Bill Huang
 DESCRIPTION:
	Change hex value representation string to long integer value representation string. 
 ARGUMENTS PASSED:
	pcHexLocation  :  The location that store the hex value representation string.  
 RETURN VALUE:
	Return the long integer value representation string.
 USED GLOBAL VARIABLES:
	None
 USED STATIC VARIABLES: 
	None
 CALL BY:
	All program need to change hex value representation string to long integer value representation string. 
 IMPORTANT NOTES:
	None
=====================================================================================*/
#define WE_SCL_HEX2LONG  WeSCL_Hex2Long
WE_LONG WeSCL_Hex2Long
(
    const WE_CHAR* pcHexLocation
);


 /*=====================================================================================
 FUNCTION: 
	WeSCL_Utf8StrLen
 CREATE DATE: 
	2006-05-30
 AUTHOR: 
	Eva zhang
 DESCRIPTION:
	Given a UTF-8 encoded null-terminated string,return the number of Unicode characters
	in the string.
 ARGUMENTS PASSED:
	pcString: a pointer to a const character string.
 RETURN VALUE:
	iCount: the number of Unicode  characters in the string.
 USED GLOBAL VARIABLES:
	None
 USED STATIC VARIABLES:
	None
 CALL BY:
	All program need return the number of Unicode characters in the string.
 IMPORTANT NOTES:
 None 
=====================================================================================*/
#define WE_SCL_UTF8STRLEN    WeSCL_Utf8StrLen
WE_INT32 WeSCL_Utf8StrLen
(
   const WE_CHAR *pcString
);

/*=====================================================================================
FUNCTION: 
	WeSCL_Utf8StrnLen
CREATE DATE: 
	2006-05-30
AUTHOR: 
	Eva Zhang
DESCRIPTION:
	Given a UTF-8 encoded string of length 'iLen' bytes,return he tnumber of Unicode 
	characters in the string.
ARGUMENTS PASSED:
	pcStrSource: a pointer to a const character string.
iLen: the length of the string.
RETURN VALUE:
	iCount: the number of Unicode characters in the string
USED GLOBAL VARIABLES:
	None
USED STATIC VARIABLES:
	None
CALL BY:
	All program need the number of Unicode characters in the string that its length is
	'iLen' bytes.
IMPORTANT NOTES:
	None
=====================================================================================*/
#define WE_SCL_UTF8STRNLEN  WeSCL_Utf8StrnLen
WE_INT32 WeSCL_Utf8StrnLen  
(
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Utf8CharLen 
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Eva Zhang
DESCRIPTION:
    Given a UTF-8 encoded string, return the number of bytes in the first Unicode character.
ARGUMENTS PASSED:
    pcStrSource: a pointer to a const character string.
RETURN VALUE:
    Return the number of bytes in the first Unicode character.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need return the number of bytes in the first Unicode character.
IMPORTANT NOTES: 
  
=====================================================================================*/
#define WE_SCL_UTF8CHARLEN   WeSCL_Utf8CharLen 
WE_INT32  WeSCL_Utf8CharLen (const WE_CHAR *pcStrSource);

/*=====================================================================================
FUNCTION: 
    WeSCL_Utf8StrIncr
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Eva Zhang
DESCRIPTION:
    Given a UTF-8 encoded null-terminated string, step past 'iSteps' Unicode characters 
  and return a pointer to the new position.
ARGUMENTS PASSED:
    pcStrSource: a pointer to a const character string.
    iSteps: the number of bytes that you want to step past Unicode charaters.
RETURN VALUE:
    Return a pointer to the new position in the string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need return a pointer to the new position in the string.
IMPORTANT NOTES:
    None
=====================================================================================*/
#define WE_SCL_UTF8STRINCR  WeSCL_Utf8StrIncr
WE_CHAR * WeSCL_Utf8StrIncr
(
    const WE_CHAR *pcStrSource, 
    WE_INT32 iSteps
);

/*=====================================================================================
FUNCTION: 
    WeSCL_SkipBlanks
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Eva Zhang
DESCRIPTION:
    Return a pointer to the first non-blank character in the string 'pcStrSource'. A blank 
  character is either SP or TAB.
ARGUMENTS PASSED:
    pcStrSource: a pointer to a const character string.
RETURN VALUE:
    Return a pointer to the first non-blank character in the string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need return a pointer to the first non-blank character in the string.
IMPORTANT NOTES:
	None
=====================================================================================*/
#define WE_SCL_SKIPBLANKS WeSCL_SkipBlanks
WE_CHAR* WeSCL_SkipBlanks(const WE_CHAR *pcStrSource);

/*=====================================================================================
FUNCTION: 
    WeSCL_SkipField
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Eva Zhang
DESCRIPTION:
    Return a pointer to the first white space character in the string 'pcStrSource', or the 
    NULL character terminating the string.A white space character is either SP, TAB, CR, 
    or LF.
ARGUMENTS PASSED:
    pcStrSource: a pointer to a const character string.
RETURN VALUE:
    Return a pointer to the first white space character in the string 'pcStrSource', or the 
    NULL character terminating the string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need return a pointer to the first white space character in the 
    string 'pcString', or the NULL character terminating the string.
IMPORTANT NOTES:
	None
=====================================================================================*/
#define WE_SCL_SKIPFIELD WeSCL_SkipField
WE_CHAR* WeSCL_SkipField(const WE_CHAR *pcStrSource);

/*=====================================================================================
FUNCTION: 
     WeSCL_SKipTailingBlanks
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Eva Zhang
DESCRIPTION:
    Given a string 'pcStrSource' and its length, return the length of that string if trailing 
  white space were omitted. A white space character is either SP, TAB, CR, or LF.
ARGUMENTS PASSED:
    pcStrSource: a pointer to a const character string.
    iLen: the length of the string.
RETURN VALUE:
    Return the length of the string if trailing white space were omitted.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need the length of the string if trailing white space were omitted.
IMPORTANT NOTES:
	None
=====================================================================================*/
#define WE_SCL_SKIPTAILINGBLANKS WeSCL_SKipTailingBlanks
WE_INT32 WeSCL_SKipTailingBlanks
(
    const WE_CHAR* pcStrSource, 
    WE_INT32 iLen
);



/*=====================================================================================
FUNCTION: 
    WeSCL_Base64DecodeChar
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Venly Feng
DESCRIPTION:
    translates the character into its 6-bit value.
ARGUMENTS PASSED:
    ucCHAR: the character wants to translate.
RETURN VALUE:
    Each of these functions returns the result of translated or -1 if translated false.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need decode character.
IMPORTANT NOTES:
    The WeSCL_Base64DecodeChar function translates the character into its 6-bit value 
    for each input character in a base64.
    Because WeSCL_Base64DecodeChar function translates the character into its 6-bit value 
    for each input character in a base64,it is good practice always to decode the character. 
  
=====================================================================================*/
#ifndef WE_SCL_BASE64DECODECHAR
#define WE_SCL_BASE64DECODECHAR  WeSCL_Base64DecodeChar
WE_INT32 WeSCL_Base64DecodeChar (WE_UCHAR ucCHAR);
#endif

/*=====================================================================================
FUNCTION: 
    WeSCL_Base64DecodeLen
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Venly Feng
DESCRIPTION:
    calculate the number of characters needed to decode.
ARGUMENTS PASSED:
    pcStr: the string needed to decode
    iLen: the length of the string
RETURN VALUE:
    Each of these functions returns the result of calculation, the number of 
    character needed to decode.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need decode character.
IMPORTANT NOTES:
    The WeSCL_Base64DecodeLen function calculate the number of character needed to 
    decode the string 'pcStr' of length 'iLen'.
    Because WeSCL_Base64DecodeLen function calculate the number of character needed to 
    decode the string 'pcStr' of length 'iLen',it is good practice always to complete
    decode the character. 
  
=====================================================================================*/
#define WE_SCL_BASE64DECODELEN  WeSCL_Base64DecodeLen
WE_INT32 WeSCL_Base64DecodeLen
(
    const WE_CHAR *pcStr,
    WE_INT32 iLen
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Base64Decode
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Venly Feng
DESCRIPTION:
    decode a string of base64-encoded data and place it in the output buffer.
ARGUMENTS PASSED:
    pcInBuf:  the buffer that storage input character 
    iInLen:   the length of input character
    pcOutBuf: the buffer that storage output character
    piOutLen: the length of the character that had decoded. On input, 'piOutLen' should 
               hold the maximum size of the output buffer, on output, it will hold the
               number of bytes placed in the output buffer .
RETURN VALUE:
    Returns 0 if the decode succeed, and -1 on false.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need decode character.
IMPORTANT NOTES:
    The WeSCL_Base64Decode function decode a string of base64-encoded data and place
    it in the output buffer .
    Because WeSCL_Base64Decode calls function decode a string of base64-encoded data and place
    it in the output buffer,it is good practice always to handle the input character . 
  
=====================================================================================*/
#define WE_SCL_BASE64_DECODE  WeSCL_Base64Decode
WE_INT32 WeSCL_Base64Decode 
(
    const WE_CHAR *pcInBuf, 
    WE_INT32 iInLen, 
    WE_CHAR *pcOutBuf, 
    WE_INT32 *piOutLen
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Base64EncodeLen
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Venly Feng
DESCRIPTION:
    calculate the number of characters needed to encode.
ARGUMENTS PASSED:
    iLen: the length of the characters needed to encode.
RETURN VALUE:
    return the number of characters needed to encode a string of length 'len'
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need connect two string.
IMPORTANT NOTES:
    The WeSCL_Base64EncodeLen function calculate the number of characters needed 
    to encode .
    Because WeSCL_Base64EncodeLen function calculate the number of characters needed 
    to encode,it is good practice always to encode the character. 

=====================================================================================*/
#define  WE_SCL_BASE64ENCODELEN  WeSCL_Base64EncodeLen

#define  WeSCL_Base64EncodeLen(c)  ((c == 0) ? 0 : ((c + 2) / 3) * 4)

/*=====================================================================================
FUNCTION: 
    WeSCL_Base64Encode
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Venly Feng
DESCRIPTION:
    create a base-64 encoding of said data, and place it in the output buffer.
ARGUMENTS PASSED:
    pcInBuf: the buffer that storage input character 
    iInLen:  the length of the input character
    pcOutBuf:the buffer that storage output character
    piOutLen:the length of output character had encoded.On input, 'piOutLen' should
                  hold the maximum size of the output buffer,on output, it will hold
             the length of the string written there.
RETURN VALUE:
    Each of these functions returns 0 if the decode succeed, and -1 on false.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need encode character.
IMPORTANT NOTES:
    The WeSCL_Base64Encode function create a base-64 encoding of said data, and 
    place it in the output buffer.
    Because WeSCL_Base64Encode function create a base-64 encoding of said data, and 
    place it in the output buffer,it is good practice always to handle character. 
  
=====================================================================================*/
#define WE_SCL_BASE64ENCODE  WeSCL_Base64Encode
WE_INT32 WeSCL_Base64Encode
(
    const WE_CHAR *pcInBuf, 
    WE_INT32 iInLen, 
    WE_CHAR *pcOutBuf, 
    WE_INT32 *piOutLen
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Month2Int
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
    find  the number of the month in the string .
ARGUMENTS PASSED:
    ppcStrSource: the string which want to find the month.
RETURN VALUE:
    Return the number of the month found in the string *ppcStrSource,
    and update the pointer 'ppcStrSource' to point beyond the month name.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need find the month in string.
IMPORTANT NOTES:
    None. 
  
=====================================================================================*/
#define WE_SCL_MONTH2INT  WeSCL_Month2Int
WE_SCHAR WeSCL_Month2Int(WE_CHAR **ppcStrSource);


/*=====================================================================================
FUNCTION: 
    WeSCL_Str2Time
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
    Converts a string (e.g. "Thu, 15 Nov 2001 08:12:31 GMT") into a
    Unix 31-bit time value (seconds since 1/1 1970, GMT).
ARGUMENTS PASSED:
    pcStrSource: the string which want to convert.
    puiSecond:	 the second which the string convert to.
RETURN VALUE:
    Return the result of the operation (FALSE or TRUE) 
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need convert string to time.
IMPORTANT NOTES:
    NONE. 
  
=====================================================================================*/
#define WE_SCL_STR2TIME  WeSCL_Str2Time
WE_BOOL WeSCL_Str2Time
(
    const WE_CHAR *pcStrSource,
    WE_UINT32 *puiSecond
);


/*=====================================================================================
FUNCTION: 
    WeSCL_Time2Str
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
    Format a Unix 31-bit time value (seconds since 1/1 1970, GMT)
    as a string in the format: "Thu, 15 Nov 2001 08:12:31 GMT"
    (as specified in RFC 1123).
ARGUMENTS PASSED:
    uiSecond: the given seconds which is start from 1/1 1970, GMT.
    pcStrSource: the formatted string.
RETURN VALUE:
    The formatted string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to convert time to string.
IMPORTANT NOTES:
    The string pcStrSource must have room for at least 30 bytes. 
  
=====================================================================================*/
#define WE_SCL_TIME2STR  WeSCL_Time2Str
void WeSCL_Time2Str
(
    WE_UINT32 uiSecond,
    WE_CHAR *pcStrSource
);



/*=====================================================================================
FUNCTION: 
    WeSCL_ParseDuration
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
    Parses a "duration" string (e.g. "PnYnMnDTnHnMnS") as defined in 
    "XML Schema Part 2: Data types. W3C Recommendation 2 May 2001" 
ARGUMENTS PASSED:
    pcStrSource: the string need to be parsed.
    pstDuration: the returned struct containing the parsed string  .
RETURN VALUE:
    Return the "duration" struct
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to parses a "duration" string.
IMPORTANT NOTES:
    None. 
  
=====================================================================================*/
#define WE_SCL_PARSEDURATION  WeSCL_ParseDuration
WE_BOOL WeSCL_ParseDuration
(
    const WE_CHAR* pcStrSource,
    St_WeSCLDuration *pstDuration
);


/*=====================================================================================
FUNCTION: 
    WeSCL_Str2Int 
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
	Look up a string's integer value in the supplied hash table, 
ARGUMENTS PASSED:
    pcStrSource: the string which need to be found integer value.
	iLength: the length of the string
	pstStrInfo: the returned struct containing the string table information.
RETURN VALUE:
	Returns -1 if the string was not found,otherwise return the string's integer value
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to find a string's integer value.
IMPORTANT NOTES:
    NONE. 
  
=====================================================================================*/
#define WE_SCL_STR2INT  WeSCL_Str2Int 
WE_INT32 WeSCL_Str2Int
(
    const WE_CHAR* pcStrSource,
    WE_INT32 iLength,
    const St_WeSCLStrtableInfo *pstStrInfo
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Str2IntLc 
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
	Look up a string's integer value in the supplied hash table  
ARGUMENTS PASSED:
    pcStrSource: the string need to be find integer value.
	iLength: the length of the string
	pstStrInfo: the returned struct containing the string table information,
	            the ASCII of the string is lower case.
RETURN VALUE:
	Returns -1 if the string was not found,otherwise return the string's integer value
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to find a string's lower case integer value.
IMPORTANT NOTES:
	This is lower-case version which means when hashing and matching, all upper-
	case ASCII characters are regarded as their lower-case
    counterparts. Consequently the 'info' string table can
    only contain lower-case strings. 
  
=====================================================================================*/
#define WE_SCL_STR2INTLC  WeSCL_Str2IntLc
WE_INT32 WeSCL_Str2IntLc
(
    const WE_CHAR* pcStrSource,
    WE_INT32 iLength,
    const St_WeSCLStrtableInfo *pstStrInfo
);



/*=====================================================================================
FUNCTION: 
    WeSCL_Int2Str 
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
	Look up  integer value in the supplied hash table, 
ARGUMENTS PASSED:
    iValue: integer value need to be find.
	pstStrInfo:  struct containing the string table information.
RETURN VALUE:
	Returns -1 if the integer value was not found,otherwise return the  its associated string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to find  a integer value's associated string.
IMPORTANT NOTES:
	The caller must not deallocate the returned string. 
  
=====================================================================================*/
#define WE_SCL_INT2STR  WeSCL_Int2Str
const WE_CHAR* WeSCL_Int2Str
(
    WE_INT32 iValue,
    const St_WeSCLStrtableInfo *pstStrInfo
);


/*=====================================================================================
FUNCTION: 
    WeSCL_StrHash 
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
	This hash function returns a 32-bit unsigned integer calculated over
	all positions in the given string.
ARGUMENTS PASSED:
    pStrSource: the given string .
	iLength:  the length of string need to be transformed, this number mustn't larger than length of string and smaller than 0
RETURN VALUE:
	Returns STR_HASH_ERROR(0XFFFFFFFF) if the parameters is invalidation,otherwise return the hash value of the string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to calculate a hash value of a string.
IMPORTANT NOTES:
	None. 
  
=====================================================================================*/
#define WE_SCL_STRHASH  WeSCL_StrHash
WE_UINT32 WeSCL_StrHash
(
    const WE_CHAR* pcStrSource,
    WE_INT32 iLength
);




/*=====================================================================================
FUNCTION: 
    WeSCL_StrHashLc 
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
	This hash function returns a 32-bit unsigned integer calculated over
	all positions in the given string ,all upper string is regards as lower.
ARGUMENTS PASSED:
    pStrSource: the given string .
	iLength:  the length of string need to be transformed, this number mustn't larger than length of string and smaller than 0
RETURN VALUE:
	Returns STR_HASH_ERROR(0XFFFFFFFF) if the parameters is invalidation,otherwise return the hash value of the string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to calculate a hash value of a string.
IMPORTANT NOTES:
	None. 
  
=====================================================================================*/
#define WE_SCL_STRHASHLC  WeSCL_StrHashLc
WE_UINT32 WeSCL_StrHashLc
(
    const WE_CHAR* pcStrSource,
    WE_INT32 iLength
);


/*=====================================================================================
FUNCTION: 
    WeSCL_CheckIPAddr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    check whether IP address is valid.
ARGUMENTS PASSED:
    pcIPAddress: IP Address.
RETURN VALUE:
    Each of these functions returns TRUE if the IP Address is valid, otherwise,
    returns FALSE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need check the IP Address.
IMPORTANT NOTES:
    The WeSCL_CheckIPAddr function check whether IP address is valid.
    Because WeSCL_CheckIPAddr function check whether IP address is valid, it is good
    practice always to the program run non-terminate. 
  
=====================================================================================*/
#define WE_SCL_CHECKIPADDR  WeSCL_CheckIPAddr
WE_BOOL  WeSCL_CheckIPAddr
(
    WE_CHAR *pcIPAddress
);


/*=====================================================================================
FUNCTION: 
    WeSCL_CheckProxyPort
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    check whether proxy port value is valid.
ARGUMENTS PASSED:
    pcPort: the proxy port
RETURN VALUE:
    Each of these functions returns TRUE if the proxy port is valid, otherwise,
    returns FALSE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need connect two string.
IMPORTANT NOTES:
    The WeSCL_CheckProxyPort function check whether proxy port value is valid.
    Because WeSCL_CheckProxyPort function check whether proxy port value is valid,
    it is good practice always to the program run non-terminate. Max number of proxy 
    port is 65535
  
=====================================================================================*/
#define WE_SCL_CHECKPROXYPORT  WeSCL_CheckProxyPort
WE_BOOL  WeSCL_CheckProxyPort
(
   WE_CHAR *pcPort
);


/*=====================================================================================
FUNCTION: 
    WeSCL_StrToLower
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Song yi
DESCRIPTION:
	Converts source string to a lower string.
ARGUMENTS PASSED:
    pcStrSource: the source string which need to convert.
	pcStrDest:	 the  destination string which the string convert to.
RETURN VALUE:
	Return the result of the operation (FALSE or TRUE),and the destination string.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need to convert string to lower string.
IMPORTANT NOTES:
    The length of the string to be converted can not be longer than 4. 
=====================================================================================*/
#define WE_SCL_STRTOLOWER  WeSCL_StrToLower
WE_BOOL WeSCL_StrToLower
(
    WE_CHAR *pcStrDest,
    WE_CHAR *pcStrSource
);

/*=====================================================================================
FUNCTION: 
    WeSCL_Sprintf
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Print a array of string to destination.
ARGUMENTS PASSED:
    pcBuffer: Storage location for output.
    pcFormat:Format-control string.
RETURN VALUE:
    Returns the number of bytes stored in buffer, not counting the 
    terminating null character
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need print strings, character or integer to the destination string.
IMPORTANT NOTES:
    The WeSCL_Sprintf function calls SPRINTF to  print the string.      
=====================================================================================*/
#define     WE_SCL_SPRINTF       TF3RD_SPRINTF

/*=====================================================================================
FUNCTION: 
    WeSCL_Sprintf
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Get the length of the string.
ARGUMENTS PASSED:
    pcBuffer: The string to be calculate the length.
RETURN VALUE:
    Each of these functions returns the number of characters in string, excluding the 
    terminal NULL. No return value is reserved to indicate an error.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need get the length of a string.
IMPORTANT NOTES:
    Each of these functions returns the number of characters in string, not including 
    the terminating null character. wcslen is a wide-character version of strlen; the
    argument of wcslen is a wide-character string. wcslen and strlen behave identically
    otherwise.       
=====================================================================================*/
#define     WE_SCL_STRLEN        TF3RD_STRLEN

/*=====================================================================================
FUNCTION: 
    WeSCL_StrCmp
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    pcString1: Null-terminated strings to compare.
    pcString2: Null-terminated strings to compare.
RETURN VALUE:
    The return value for each of these functions indicates the lexicographic relation of
    string1 to string2.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRCMP   TF3RD_STRCMP

/*=====================================================================================
FUNCTION: 
    WeSCL_StrNCmp
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    pcString1: Null-terminated strings to compare.
    pcString2: Null-terminated strings to compare.
RETURN VALUE:
    The return value for each of these functions indicates the lexicographic relation of
    string1 to string2.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRNCMP   TF3RD_STRNCMP

/*=====================================================================================
FUNCTION: 
    WeSCL_StrCpy
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    copy the source string to the destination string.
ARGUMENTS PASSED:
    pcDestination: The destination string to be copied to.
    pcSource: The source string to be copied.
RETURN VALUE:
    Each of these functions returns the destination string. No return value is reserved
    to indicate an error.

USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need copy one string to another.
IMPORTANT NOTES:
    The strcpy function copies strSource, including the terminating null character, to the 
    location specified by strDestination. No overflow checking is performed when strings 
    are copied or appended. The behavior of strcpy is undefined if the source and destination
    strings overlap.      
=====================================================================================*/
#define WE_SCL_STRCPY   TF3RD_STRCPY

/*=====================================================================================
FUNCTION: 
    WeScl_Wstrlen
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    This function gets the length of the given null terminated wide string 
    (i.e. string comprised of AECHAR characters) 
ARGUMENTS PASSED:
    pusString: Null terminated string whose length is to be determined.
RETURN VALUE:
    Returns the number of AECHAR characters in string, excluding the 
    terminal NULL.

USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need get length of wide string.
IMPORTANT NOTES:
    None.     
=====================================================================================*/
#define WE_SCL_WSTRLEN  TF3RD_WSTRLEN

/*=====================================================================================
FUNCTION: 
    WeScl_WstrToStr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    This function converts a wide string into a single-byte string . 
ARGUMENTS PASSED:
    pusSource: Pointer to null terminated wide string that must be converted to single-byte
    character string.
    pcDestination: Pointer to destination buffer to receive the single-byte string 
    iSize : Size (in bytes) of pDest buffer. If this is 0, this function does not do any 
    conversion. It returns pszDest as is without any changes. Negative values of nSize are invalid. 
RETURN VALUE:
    Returns the destination string.

USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need get length of wide string.
IMPORTANT NOTES:
    None.     
=====================================================================================*/
#define WE_SCL_WSTRTOSTR    WeScl_WstrToStr
/*WE_CHAR   *WeScl_WstrToStr(AECHAR *pusSource, WE_CHAR *pcDestination, WE_INT32 iSize);*/
#define WeScl_WstrToStr     //WSTRTOSTR

#define WE_SCL_WSTRCAT  TF3RD_WSTRCAT
/*=====================================================================================
FUNCTION: 
    WeScl_Free
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    pcPointer: Pointer to the memory to be freed.
RETURN VALUE:
    The return value for each of these functions indicates the lexicographic relation of
    string1 to string2.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_FREE   TF3RD_MEMFREE

/*=====================================================================================
FUNCTION: 
    WeScl_FreeIf
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    pcPointer: Pointer to the memory to be freed.
RETURN VALUE:
    The return value for each of these functions indicates the lexicographic relation of
    string1 to string2.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_FREEIF(p)    if(p){WE_SCL_FREE((void*)p);(p)=0;}

/*=====================================================================================
FUNCTION: 
    WeScl_MemMov
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MEMMOVE   TF3RD_MEMMOVE

/*=====================================================================================
FUNCTION: 
    WeScl_Malloc
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MALLOC   TF3RD_MALLOCMEMORY

/*=====================================================================================
FUNCTION: 
    WeScl_Strstr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRSTR   TF3RD_STRSTR

/*=====================================================================================
FUNCTION: 
    WeScl_Strstr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Compare the two string.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRCHR   TF3RD_STRCHR

/*=====================================================================================
FUNCTION: 
    WeScl_Memcpy
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Copy the memory by the size.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MEMCPY   TF3RD_MEMCPY

/*=====================================================================================
FUNCTION: 
    WeScl_Memset
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MEMSET   TF3RD_MEMSET

/*=====================================================================================
FUNCTION: 
    WeScl_Memcmp
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MEMCMP   TF3RD_MEMCMP

/*=====================================================================================
FUNCTION: 
    WeScl_Atoi
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_ATOI   TF3RD_STRATOI

/*=====================================================================================
FUNCTION: 
    WeScl_Strchr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRCHR   TF3RD_STRCHR

/*=====================================================================================
FUNCTION: 
    WeScl_MemChr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MEMCHR   TF3RD_MEMCHR

/*=====================================================================================
FUNCTION: 
    WeScl_MemRChr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MEMRCHR(a,b,c)   TF3RD_MEMRCHR(a,b,c)

/*=====================================================================================
FUNCTION: 
    WeScl_GetRand
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_GETRAND   TF3RD_RAND

/*=====================================================================================
FUNCTION: 
    WeScl_GetTimes
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_GETTIMES   TF3RD_TICKCOUNT

/*=====================================================================================
FUNCTION: 
    WeScl_GetSeconds
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_GETSECONDS   TF3RD_TIME

/*=====================================================================================
FUNCTION: 
    WeScl_StrrChr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRRCHR   TF3RD_STRRCHR

/*=====================================================================================
FUNCTION: 
    WeScl_Printf
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    print the log as to printf.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_PRINTF   WeScl_Printf
#ifdef WE_DEBUG_INFO
	#ifdef WIN32_DEBUG
	#define WeScl_Printf(x)    printf x
	#else
	#define WeScl_Printf(x)    TF3RD_DBGPRINTF x
	#endif
#else
    #define WeScl_Printf(x)
#endif

/*=====================================================================================
FUNCTION: 
    WeScl_StrNiCmp
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRNICMP   TF3RD_STRNICMP

/*=====================================================================================
FUNCTION: 
    WeScl_StriCmp
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRICMP   TF3RD_STRICMP

/*=====================================================================================
FUNCTION: 
    WeScl_StrToUl
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRTOUL   TF3RD_STRTOUL

/*=====================================================================================
FUNCTION: 
    WeScl_Min
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MIN(a, b)   ((a < b) ? a : b)

/*=====================================================================================
FUNCTION: 
    WeScl_Max
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_MAX(a, b)   ((a > b) ? a : b)

/*=====================================================================================
FUNCTION: 
    WE_SCL_BASE64LEN
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/

#define WE_SCL_BASE64LEN(x)            (((((x) + 2) / 3) * 4) + 1)

#define WE_SCL_BASE64_DECODE_LEN(x)    ((((x) - 1) * 3) / 4 - 2)

/*=====================================================================================
FUNCTION: 
    WeScl_StriStr
CREATE DATE: 
    2006-05-29
AUTHOR: 
    Yang Min
DESCRIPTION:
    Set the value of the memory.
ARGUMENTS PASSED:
    uiSize:    size of the memory to be get.
RETURN VALUE:
    The pointer to the memory!
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    All program need Compare two string.
IMPORTANT NOTES:
    The WeSCL_StrCmp function compares string1 and string2 lexicographically and returns a 
    value indicating their relationship.       
=====================================================================================*/
#define WE_SCL_STRISTR  TF3RD_STRISTR

/*--------------------------END-----------------------------*/


#endif/*WE_SCL_H*/