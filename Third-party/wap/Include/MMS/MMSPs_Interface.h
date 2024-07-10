/* $ID:MMSPs_Interface.h  V1.0 2003/08/28 */

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
 *  FILE: MMSPs_Interface.h
 *
 *  MODULE: MMS Parser
 *
 *  PURPOSE: define the external interface of the parser module.
 *
 *  AUTHOR(S): xxlin
 *
 *  GROUP: Terminal
 *
 *  DATE CREATED:2003/08/28
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name     		Description
 *  2003/08/30    xxlin             Create this file
 *************************************************************************/

#ifndef _MMSPS_INTERFACE_H_
#define _MMSPS_INTERFACE_H_

#include "sysTypes.h"


#ifdef __cplusplus
T_EXTERN "C" {
#endif	/*end of __cplusplus*/


typedef struct{ /*record display position of MMS element such as image and text*/
	T_WORD xco; /*x start coordinate of screen*/
	T_WORD yco;/*y start coordinate of screen*/
	T_WORD width;	/*width of display position*/
	T_WORD height;	/*height of display postion*/
}T_SMIL_PosTp;



typedef struct T_SMIL_ImgStruct{	/*record image info of MMS display which is in process*/
	T_SMIL_PosTp  graphPos; /*image display position ,may be needless*/
	T_UBYTE* ImgBuf;	/*image data*/
	T_CHAR*  Type;		/*image type*/
	T_WORD	begin;		/*begin time of image display*/
	T_WORD	end;		/*end time of image display*/
	T_WORD size;		/*size of image data*/
}T_SMIL_ImgStruct;

typedef struct T_SMIL_VideoStruct{	/*record video info of MMS display which is in process*/
	T_SMIL_PosTp  VideoPos; /* video display position ,may be needless*/
	T_UBYTE* VideoBuf;	/*video data*/
	T_CHAR*  Type;		/*video type*/
	T_WORD	begin;		/*begin time of video display*/
	T_WORD	end;		/*end time of video display*/
	T_WORD size;		/*size of video data*/
}T_SMIL_VideoStruct;


typedef struct T_SMIL_TextStruct{
	T_SMIL_PosTp  txtPos;	/*text show position ,may be need less*/
	T_BYTE* TxtBuf; 	/*text data*/
	T_WORD	begin;		/*begin time of text show*/
	T_WORD	end;		/*end time of text show*/
}T_SMIL_TextStruct;

typedef struct T_SMIL_AudStruct{
	T_UBYTE* AudBuf;	/*audio data buffer*/
	T_CHAR *Type;		/* audio type  */
	T_WORD	begin;		/*begin time of audio play*/
	T_WORD	end;		/*end time of audio play*/
	T_WORD size;		/*size of audio data*/
}T_SMIL_AudStruct;


typedef enum{	/*enumeration image type*/
	SMIL_IMAGE_ERR, /*error image type*/
	SMIL_IMAGE_GIF, /*GIF image type*/
	SMIL_IMAGE_JPEG,	/*JPEG image type*/
	SMIL_IMAGE_WBMP /*WBMP image type*/
}T_MMS_ImgType;


/*This function is the entry of MMS parser module.It creates MMS 
 *			  Playing window and initializes MMS parser module.
 */
T_BOOL fnPAR_SmilParserEntry(T_UBYTE* pBodyPdu,T_WORD length);	/* parse the mms content */
T_VOID fnPAR_SmilFree(T_VOID); /*free the memory which is used by smil*/
T_VOID fnPAR_InitCurPar(T_VOID); /*init current page of MMS data which will be previewed*/
T_BOOL fnPAR_ProceedNextPar(T_VOID); /*go to  next slide of MMS data which will be previewed*/
T_BOOL fnPAR_ProceedPrePar(T_VOID); /*go to  preceding slide of MMS data which will be previewed*/
T_BOOL fnPAR_GetCurParImg(T_SMIL_ImgStruct * CurImg);	/*get image data from current page of MMS*/
T_BOOL fnPAR_GetCurParText(T_SMIL_TextStruct *CurTxt); /*get text data from current slide of MMS*/
T_BOOL fnPAR_GetCurParAudio(T_SMIL_AudStruct *CurAud); /*get audio data from current slide of MMS*/
T_BOOL fnPAR_GetCurParVideo(T_SMIL_VideoStruct *CurVideo);/* get video data form current slide of mms */
T_WORD fnPAR_GetCurParDur(T_VOID);	/*get time length during one page of MMS preview*/
T_BOOL fnPAR_IsParEmpty(T_VOID); /*judge if the current slide of MMS preview is empty*/
T_WORD fnPAR_GetTotalSlideNum(T_VOID); /* get the total slide number  */

#ifdef __cplusplus
}
#endif  //end of #ifdef __cplusplus 


#endif  //end of #ifndef _MMSPS_INTERFACE_H_


