/* $ID:CharSet_TranscodeApi.h  V1.0 2004/05/21 */

/***********************************************************************
 *   This source code has been made available to you by CORETEK on
 *   AS-IS.Anyone receiving this source is licensed under
 *   CORETEK copyrights to use it in any way he or she deems fit,including
 *   copying it,modifying it,compiling it,and redistributing it either with
 *   or without modifictions.
 *
 *
 *   Any person who transfers this source code or any derivative work must
 *   include the CORETEK copyright notice, this paragraph,and the preceding
 *   two paragraphs in the transferred software.
 *
 *
 *        COPYRIGHT CORETEK CORPORATION 2004
 *      LICENSED MATERIAL - PROGRAM PROPERTY OF CORETEK
 ************************************************************************/

/************************************************************************
 *
 *  FILE: CharSet_TranscodeApi.h
 *
 *  MODULE: Charset module
 *
 *  PURPOSE: Define the api used to transcode charset.
 *
 *  AUTHOR(S): XiaoLin Xiao
 *
 *  GROUP: TERMINAL
 *
 *  DATE CREATED:2004/05/21
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name     		Description
 *  2004/05/21    XiaoLin Xiao          Create this file
 *************************************************************************/

#ifndef _CHARSET_TRANSCODEAPI_H
#define _CHARSET_TRANSCODEAPI_H


#define ISO_10646_UCS_2		0x03E8	/* ucs2  */
#define GB2312				0x07E9	/* gb2312 */
#define BIG5				0x07EA	/* big4  */
#define UTF8				0x6A	/* UTF-8  */


#ifdef __cplusplus
extern "C" {
#endif	/*end of __cplusplus*/







/*========================================================
 *      API defines
 *========================================================*/
unsigned char *fnCharSet_DecodeString2Local(int CharSet,unsigned char *Text,int *len);
unsigned char *fnCharSet_EncodeLocalString2UTF8(unsigned char *Text,int *len);

void fnCharSet_FreeStringBuf(void *pbuf);







#ifdef __cplusplus
}
#endif  //end of #ifdef __cplusplus 


#endif
