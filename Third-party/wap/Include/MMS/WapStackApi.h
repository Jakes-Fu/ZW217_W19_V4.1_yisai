/* $id:WapStackApi.h  V1.0 2002/09/20 */

/******************************************************************************
 *  This source code has been made available to you by CORETEK  on
 *  AS-IS.Anyone receiving this source is licensed under
 *  CORETEK copyrights to use it in any way he or she deems fit,including
 *  copying it,modifying it,compiling it,and redistributing it either with
 *  or without modifictions.
 *
 *
 *  Any person who transfers this source code or any derivative work must
 *  include the CORETEK copyright notice, this paragraph,and the preceding
 *  two paragraphs in the transferred software.
 *
 *
 *       COPYRIGHT CORETEK CORPORATION 2001
 *     LICENSED MATERIAL - PROGRAM PROPERTY OF CORETEK
 *****************************************************************************/

/******************************************************************************
 *
 *  FILE: WapStackApi.h
 *
 *  MODULE: wap stack
 *
 *  PURPOSE: Define the wap socket api.
 *
 *  AUTHOR(S):XXLIN
 *
 *  GROUP:TERMINAL 
 *
 *  DATE CREATED:2002/09/20
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name       Description
 *  2002/09/20    XXLIN           Create this file
 *********************************************************************************/


#ifndef _WAPSTACKAPI_H_
#define _WAPSTACKAPI_H_

#include "sysTypes.h"

T_EXTERN T_HWORD wap_session;

#ifdef __cplusplus
T_EXTERN "C"{
#endif

T_UBYTE wap_begin(T_VOID);
T_HWORD BeginToLoad(T_CHAR * url,T_BYTE *Header[],T_HWORD Header_len);
T_HWORD Dwap_Send(T_BYTE *Url,
	T_UBYTE * Body,T_WORD Body_len,
	T_BYTE *Header[],T_HWORD Header_len,
	T_UBYTE *ContentType);
T_UHWORD wap_is_connected(T_VOID);
T_VOID  wap_exit(T_VOID);
T_HWORD Wap_Ok(void);
T_HWORD  Wap_Peek_Ok(void);
T_VOID	wap_setparam(T_CONST T_CHAR * wapgateway,T_WORD wapport,T_WORD MaxCsdu,T_WORD MaxSsdu);



#ifdef __cplusplus
}
#endif


#endif 
