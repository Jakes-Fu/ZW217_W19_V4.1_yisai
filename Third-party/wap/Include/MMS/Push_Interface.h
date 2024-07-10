 /* $id:Push_Interface.h  V1.0 2004/05/31 */

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
 *       COPYRIGHT CORETEK CORPORATION 2004
 *     LICENSED MATERIAL - PROGRAM PROPERTY OF CORETEK
 *****************************************************************************/

/******************************************************************************
 *
 *  FILE: Push_Interface.h
 *
 *  MODULE: Push module
 *
 *  PURPOSE: define the external api and struct.
 *
 *  AUTHOR(S):XXLIN
 *
 *  GROUP:TERMINAL
 *
 *  DATE CREATED:2004/05/31
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name       Description
 *  2004/05/31    XXLIN           Create this file
 *********************************************************************************/




#ifndef _PUSH_INTERFACE_H
#define _PUSH_INTERFACE_H

#include "sysTypes.h"
#include "mn_type.h"

#define CONSMS_IS_MMS_NOTIFY		0	/* 是新到彩信push消息并且处理成功  */
#define CONSMS_IS_MMS_DELIVERY_IND	-1	/* 是彩信传送报告　*/
#define CONSMS_IS_DEAL_ERROR		-2	/* 是彩信相关push消息但是处理错误  */
#define CONSMS_IS_CONSMS_NOT_PUSH	-3	/* 不是push消息 */
#define CONSMS_IS_OTHER_PUSH		-4	/* 其他的未处理类型的Push消息 */
#define CONSMS_EXCEED_MAX_SEQUENCE	-5	/* 超过最大分片 */


#ifdef __cplusplus
T_EXTERN "C"{
#endif


typedef struct T_Push_Date_Struct{
	T_UWORD year;
	T_UWORD month;
	T_UWORD date;
	T_UWORD hour;
	T_UWORD minute;
	T_UWORD second;
}T_Push_Date;


/*  push sic struct defines  */
typedef struct T_Push_SIC
{
	T_UBYTE *href;		/* URL address  */
	T_UBYTE *si_id;		/* push si content's id */
	T_UBYTE *NotifyText;	/* text information of the push si  */
	T_WORD CharSet;		/* charset used in this push si  */
	T_Push_Date  created;		/*  create time of the push si */
	T_Push_Date  si_expired;	/* expired time */
	T_UBYTE action;		/*  action tag field */
	T_UBYTE Boundary[3];	/*  4-bit boundary */
}T_Push_SIC;



/* api used to dispatch push message  */
T_BYTE fnPush_OTADispach(T_UBYTE *Buf, T_WORD DataLength);

T_BYTE fnPush_RecvSms(MN_SMS_USER_DATA_T *pSms);


/* APIs used to parse push content,these functions may be used independently  */
T_Push_SIC*  fnPush_DecodeSic(T_UBYTE *Buf,T_WORD DataLength);
T_VOID fnPush_FreeSIC(T_Push_SIC* pSic);






#ifdef __cplusplus
}
#endif

#endif

