/* $ID:DeltaMMS.h  V1.0 2003/05/20 */

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
 *        COPYRIGHT CORETEK CORPORATION 2003
 *      LICENSED MATERIAL - PROGRAM PROPERTY OF CORETEK
 ************************************************************************/

/************************************************************************
 *
 *  FILE: DeltaMMS.h
 *
 *  MODULE: MMS
 *
 *  PURPOSE: Define the macro and struct used by the mms ui designer .
 *
 *  AUTHOR(S): XiaoLin Xiao
 *
 *  GROUP: PDA
 *
 *  DATE CREATED:2003/05/20
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name     		Description
 *  2003/05/20    XiaoLin Xiao          Create this file
 *************************************************************************/

#ifndef _DELTAMMS_H
#define _DELTAMMS_H





#ifdef __cplusplus
extern "C" {
#endif	/*end of __cplusplus*/

/* mms config struct   */
typedef struct T_MMS_SysCfgs
{
	int Expiry;   /* 过期时间，秒数 */
	unsigned char ReadNow;	/*  立即取或者延时 */
	unsigned char ReadAnonymity;   /* 阅读匿名信息  */
	unsigned char ReadAdvert;		/*  接收广告 */	
	unsigned char ReadReport;	  /* 阅读报告  */	
	unsigned char DeliverReport;  /* 传送报告    */
	unsigned char Boundary; /*字节对齐 补充位 */
	char mmsc[50];
	char GateWay[16];
}T_MMS_SysCfgs;


/*===================================================================
 *   below is the plug and callback functions which used in the mms
 *	 kernel,these functions must be realize when program the mms's 
 *	 user Agent.
 *===================================================================*/

/**************************************************************
 * FUNCTION: fnMMS_SaveMmsPushMessage
 *
 * PURPOST: Save push message come from sms,the push message may
 *			be mms notification or delivery/read report,when receive
 *          these information,agent must do some thing to indicate it
 *          to the user.
 * PARAMETER
 * 		InPut :  
 *			unsigned char *pData -- push information's data content.
 *			unsigned int Length -- the length of the notify data .
 *		OutPut: none
 *
 *		InOut : none
 *	
 * Return Value: if success return 1 ,else return 0. 
 *
 * Reentrant: none
 **************************************************************/
T_BOOL  fnMMS_SaveMmsPushMessage(unsigned char *pData,unsigned int Length);





#ifdef __cplusplus
}
#endif  //end of #ifdef __cplusplus 


#endif
