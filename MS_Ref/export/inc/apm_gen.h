/******************************************************************************
 ** File Name:      apm_gen.h                                                 *
 ** Author:         shujing.dong                                              *
 ** DATE:           11/15/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the general type and macro define       * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 **  -----------------------------------------------------------------------  *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/15/2005     shujing.dong     Create.                                   *
 *****************************************************************************/
  /*! \file apm_gen.h
  *  \author Shujing.Dong 
  *  \date Nov 15, 2005
  *  \brief This file defines the basic operation interfaces and data structure 
  *          of audio plugger manage.
  */
#ifndef _APM_GEN_H
#define _APM_GEN_H
/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "os_api.h"
#include "audio_api.h"
/**---------------------------------------------------------------------------**
 **                        Debugging Flag                                     **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
/*! \def APM_MALLOC
 *  malloc in apm
 */
#define APM_MALLOC SCI_ALLOC_APP
/*! \def APM_FREE
 *  free in apm
 */
#define APM_FREE   SCI_FREE
/*! \def APM_STRCPY
 *  strcpy in apm
 */
#define APM_STRCPY SCI_MEMCPY
/*! \def APM_SIZEOF
 *  sizeof in apm, equal to in ANSI C
 */
#define APM_SIZEOF sizeof
/*! \def APM_MAX_LEN_NAME
 *  max len of name, eg:name of codec, name of express
 */
#define APM_MAX_LEN_NAME          96 // 32
/*! \def APM_MAX_LEN_DESCRIPTION
 *  max len of description, eg:description of codec,
 *                  description of codec extOpe
 */
#define APM_MAX_LEN_DESCRIPTION   800 
   

/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Function Prototypes                               **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn uint32 AUDIO_PM_GetStrLength(                      
//!    const uint16 *srcString,       
//!    uint32 maxlen  
//!    )
//! \param srcString source string
//! \param maxlen is the value permitted return in bytes
//!                              no matter how long of srcString
//! \return return the length in bytes of the string  \sa uint32
//! \brief  Description: This function is to get the length of string.
//! \author Author: Shujing.Dong
//! \note   Note:the length isn't include the end flag--two '\0'           
/*****************************************************************************/  
uint32 AUDIO_PM_GetStrLength(//return the length of the string                        
    const uint16 *srcString, //source string
    uint32 maxlen            //maxlen is the value permitted return in bytes
                             //no matter how long of srcString
    );
                       
/*****************************************************************************/
//! \fn AUDIO_RESULT_E AUDIO_PM_StrCpy16(                     
//!    uint16 *destString,
//!    const uint16 *srcString,         
//!    uint32 len  
//!    )
//! \param destString dest string, must not be SCI_NULL   
//! \param srcString source string, must not be SCI_NULL
//! \param len copy the former len bytes data of srcString to destString
//! \return return the result of copy  \sa AUDIO_RESULT_E
//! \brief  Description: This function is to copy a string 
//! \author Author: Shujing.Dong
//! \note   Note:Nothing.           
/*****************************************************************************/ 
AUDIO_RESULT_E AUDIO_PM_StrCpy16(    //return the result of copy
    uint16 *destString,     //dest string, must not be SCI_NULL                        
    const uint16 *srcString,//source string, must not be SCI_NULL
    uint32 len              //copy the former len bytes data of 
                            //srcString to destString
    ); 

/*****************************************************************************/
//! \fn uint32 AUDIO_PM_StrCmp16(                     
//!    const uint16 *string1,                    
//!    const uint16 *string2   
//!    )
//! \param string1 string1 
//! \param string2 string2
//! \return if equal, return 0, else, return 1  \sa uint32
//! \brief  Description: This function is to compare two strings if equal.
//! \author Author: Shujing.Dong
//! \note   Note: only compare the front 0xffff bytes, if lengths of both strings
//!      are much  than oxffff, this func will not compare the remainder chars.
/*****************************************************************************/ 
uint32 AUDIO_PM_StrCmp16(     //if equal, return 0, else, return 1
    const uint16 *string1,    //string1                        
    const uint16 *string2     //string2
    );                          
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif

#endif  // _APM_GEN_H

// End of apm_gen.h
