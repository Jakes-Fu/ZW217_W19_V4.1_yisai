/*****************************************************************************
** File Name:     cfl_char.h                                                 *
** Author:        li.li                                                      *  
** Date:          2007/11/14                                                 *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:   char and string operation functions for Ascii chars        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/11/14     li.li            Create									 *
** 2008/05/08     yifeng.wang      Amend                                     *
******************************************************************************/

#ifndef _CFL_CHAR_H_
#define _CFL_CHAR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif		
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CFLCHAR_MIN(_X, _Y)		(((_X)<(_Y))? (_X):(_Y))

#define CFLCHAR_MEMCPY(_DEST_PTR, _DEST_LEN, _SRC_PTR, _SRC_LEN, _SIZE) \
        SCI_MEMCPY((_DEST_PTR), (_SRC_PTR), \
	    ((_DEST_LEN) > (_SRC_LEN))? CFLCHAR_MIN((_SRC_LEN),(_SIZE)) : CFLCHAR_MIN((_DEST_LEN),(_SIZE)))
	    
// Ascii string function macro
#define CFLCHAR_StrCpy   strcpy
#define CFLCHAR_StrNCmp  strncmp
#define CFLCHAR_StrCmp   strcmp
#define CFLCHAR_StrCat   strcat
#define CFLCHAR_StrStr   strstr
#define CFLCHAR_StrLen(_STR_PTR)    (PNULL==_STR_PTR)? (0):(strlen(_STR_PTR))


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: check if an input char is Ascii code
//	Global resource dependence:
//  Author: li.li
//	Note: same as stdc::isascii    
/*****************************************************************************/
PUBLIC int32 CFLCHAR_IsAscii( uint32 c );

/*****************************************************************************/
// 	Description: Convert an character to uppercase
//	Global resource dependence: 
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC int32 CFLCHAR_ToUpper( uint32 c );

/*****************************************************************************/
// 	Description : Convert character to lowercase
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC int32 CFLCHAR_ToLower( uint32 c );

/*****************************************************************************/
// 	Description : check if the input is an alphabetic character
//	Global resource dependence :
//  Author: li.li
//	Note:    
/*****************************************************************************/
PUBLIC int32 CFLCHAR_IsAlpha( uint32 c );

/*****************************************************************************/
// 	Description : check if the input is an alphabetic or a num character
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC int32 CFLCHAR_IsAlNum( uint32 c );

/****************************************************************************/
// 	Description : strncpy
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	It is different for strncpy in stdlib, because it always append one null in dest string.
/****************************************************************************/
PUBLIC char* CFLCHAR_StrNCpy(
			char            *dest_ptr,  //[IN] source pointer
            const char      *src_ptr,   //[IN] destination pointer
            uint32          len         //[IN] max length
			);

/*****************************************************************************/
//  Description: string compare with case-insensitive
//  Global resource dependence : 
//  Author: li.li
//  Note: same as stricmp
/*****************************************************************************/
PUBLIC int32 CFLCHAR_StrICmp( char* s1, char* s2 );

/*****************************************************************************/
//  Description: string compare with case-insensitive 
//  Global resource dependence : 
//  Author: li.li
//  Note: same as strnicmp
/*****************************************************************************/
PUBLIC int32 CFLCHAR_StrNICmp(const char* s1, const char* s2, uint32 len );
	
/*****************************************************************************/
//  Description: snprintf
//  Global resource dependence : 
//  Author: li.li
//  Note: target_buf_len is the full length in byte of target_buf_ptr memory (include '\0'). 
/*****************************************************************************/
PUBLIC int32 CFL_Snprintf(char* target_buf_ptr, size_t target_buf_len, const char* format_ptr, ...);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* _CFL_CHAR_H_ */