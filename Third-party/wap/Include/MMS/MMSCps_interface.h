/* $ID:MMSCps_Interface.h  V1.0 2004/05/17 */

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
 *  FILE: MMSCps_interface.h
 *
 *  MODULE: MMS
 *
 *  PURPOSE: Define the macro,struct and apis used by the mms UI designer.
 *
 *  AUTHOR(S): XiaoLin Xiao
 *
 *  GROUP: TERMINAL
 *
 *  DATE CREATED:2004/05/18
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name     		Description
 *  2004/05/18    XiaoLin Xiao          Create this file
 *************************************************************************/

#ifndef _MMSCPS_INTERFACE_H
#define _MMSCPS_INTERFACE_H

#include "sysTypes.h"

/* Error number */

#define COMPOSER_SUCCESS					0		/* �����ɹ� */
#define COMPOSER_MEMERY_ALLOC_FAIL		-1		/* �ڴ����ʧ�� */
#define COMPOSER_MODULE_IS_USING			-2		/* ģ�������ڱ༭ģʽ */
#define COMPOSER_MEDIA_HAS_EXIST			-3		/* ��Դ�Ѿ����� */
#define COMPOSER_MEDIA_EXIST_VIDEO		-4		/* �Ѿ�������Ƶ */
#define COMPOSER_MEDIA_EXIST_IMG_AUD	-5		/* �Ѿ�����������ͼ��*/
#define COMPOSER_REACH_MAX_SLIDE			-6		/* �Ѿ��ﵽ�����slide */
#define COMPOSER_JUST_ONE_EMPTY_SLIDE	-7		/* ��ǰֻʣΨһ�հ�ҳ */
#define COMPOSER_MEDIA_NOT_EXIST			-8		/* ��Դ������ */
#define COMPOSER_GOTO_THE_END			-9		/* �ѵ������ҳ��  */
#define COMPOSER_GOTO_THE_FIRST			-10		/* �ѵ���һ��ҳ�� */
#define COMPOSER_GLOBE_VAR_ERROR			-11		/* �ڲ�ȫ�ֱ������� */
#define COMPOSER_INPUT_PARAM_ERROR		-12		/* ������������ */
#define COMPOSER_CURRENT_SLIDE_NOT_EXIST	-13	/* ��ǰslide������ */
#define COMPOSER_UNKNOWN_ERROR			-16		/* ��ȷ������ */


/*  image content type macros   */
#define COMPOSER_IMAGE_GIF	"image/gif"
#define COMPOSER_IMAGE_JPEG	"image/jpeg"
#define COMPOSER_IMAGE_PNG	"image/png"
#define COMPOSER_IMAGE_WBMP	"image/vnd.wap.wbmp"

/* audio content type macros  */

#define COMPOSER_AUDIO_WAV		"audio/wav"
#define COMPOSER_AUDIO_IMELODY	"audio/imelody"
#define COMPOSER_AUDIO_MIDI		"audio/midi"
#define COMPOSER_AUDIO_AMR		"audio/amr"
#define COMPOSER_AUDIO_MP3		"audio/mp3"

/*  video content type macros */
#define COMPOSER_VIDEO_MPEG		"video/mpeg"







#ifdef __cplusplus
extern "C" {
#endif	/*end of __cplusplus*/

/* ��ʾ��������ṹ  */
typedef struct{ /*record display position */
	T_WORD x; /*x start coordinate of screen*/
	T_WORD y;/*y start coordinate of screen*/
	T_WORD width;	/*width of display position*/
	T_WORD height;	/*height of display postion*/
}T_Composer_Rectangle;


/*����ͼƬ����Դ��Ϣ*/
typedef struct T_Composer_Image{
	T_UBYTE* ImgBuf;	/*image data*/
	T_CHAR*  Type;		/*image type*/
	T_WORD	begin;		/*begin time of image display*/
	T_WORD	end;		/*end time of image display*/
	T_WORD size;		/*size of image data*/
	T_Composer_Rectangle region;  /* image's region   */
}T_Composer_Image;

/*�����ı�����Դ��Ϣ*/
typedef struct T_Composer_Text{
	T_UBYTE* TxtBuf; 	/*text data*/
	T_WORD	begin;		/*begin time of text show*/
	T_WORD	end;		/*end time of text show*/
	T_WORD size;		/* the size of the text content */
	T_Composer_Rectangle region;  /* text's region  */
}T_Composer_Text;


/*����������Դ��Ϣ*/
typedef struct T_Composer_Audio{
	T_UBYTE* AudBuf;	/*audio data buffer*/
	T_CHAR *Type;		/* audio type  */
	T_WORD	begin;		/*begin time of audio play*/
	T_WORD	end;		/*end time of audio play*/
	T_WORD size;		/*size of audio data*/
}T_Composer_Audio;

/*����������Դ��Ϣ*/
typedef struct T_Composer_Video{
	T_UBYTE* VidBuf;	/*video data buffer*/
	T_CHAR *Type;		/* video type  */
	T_WORD	begin;		/*begin time of video play*/
	T_WORD	end;		/*end time of video play*/
	T_WORD size;		/*size of video data*/
	T_Composer_Rectangle region;  /* video's region  */
}T_Composer_Video;



/*=============================================================================
 *     API Defines
 *=============================================================================*/
/* �½����� */
T_WORD fnCPS_NewMms(T_VOID);

/* �ٱ༭���� */
T_WORD fnCPS_ReEditMms(T_UBYTE*Pdu,T_WORD Size);

/* �˳��༭ģ�� */
T_VOID fnCPS_Quit (T_VOID);

/* ��鵱ǰҳ���Ƿ����ͼ�� */
T_BOOL fnCPS_CurImageExist(T_VOID);

/* ��鵱ǰҳ���Ƿ�����ı� */
T_BOOL fnCPS_CurTextExist(T_VOID);

/* ��鵱ǰҳ���Ƿ�������� */
T_BOOL fnCPS_CurAudioExist(T_VOID);

/* ��鵱ǰҳ���Ƿ������Ƶ */
T_BOOL fnCPS_CurVideoExist(T_VOID);

/* �ڵ�ǰҳ���ͼ�� */
T_WORD fnCPS_AddImage(T_Composer_Image*Image);

/* �ڵ�ǰҳ����ı� */
T_WORD fnCPS_AddText(T_Composer_Text*Text);

/* �ڵ�ǰҳ������� */
T_WORD fnCPS_AddAudio(T_Composer_Audio*Audio);

/* �ڵ�ǰҳ�����Ƶ */
T_WORD fnCPS_AddVideo(T_Composer_Video*Video);

/* �ڵ�ǰҳ��������µĿհ�ҳ�� */
T_WORD fnCPS_AddSlide (T_VOID);

/* ���õ�ǰҳ�����ʱ */
T_WORD fnCPS_SetCurSlideDuration(T_WORD delay);

/* ��õ�ǰҳ�����ʱ */
T_WORD fnCPS_GetCurSlideDuration(T_VOID);

/* �ӵ�ǰҳɾ��ͼ�� */
T_WORD fnCPS_DeleteCurImage(T_VOID);

/* �ӵ�ǰҳɾ���ı� */
T_WORD fnCPS_DeleteCurText(T_VOID);

/* �ӵ�ǰҳɾ������ */
T_WORD fnCPS_DeleteCurAudio(T_VOID);

/* �ӵ�ǰҳɾ����Ƶ */
T_WORD fnCPS_DeleteCurVideo(T_VOID);

/* ɾ����ǰҳ�棬������ҳ����Դ */
T_WORD fnCPS_DeleteCurSlide(T_VOID);

/* �Ա༭ģ�������գ���������ɵı༭���� */
T_VOID fnCPS_Empty(T_VOID);

/*  �ƶ����¸�ҳ�� */
T_WORD fnCPS_GoNextSlide (T_VOID);

/*  �ƶ����ϸ�ҳ�� */
T_WORD fnCPS_GoPreSlide (T_VOID);

/*  �õ���ǰҳ��ͼ����Ϣ */
T_WORD fnCPS_GetCurImage(T_Composer_Image*Image);

/* �õ���ǰҳ���ı���Ϣ */
T_WORD fnCPS_GetCurText(T_Composer_Text*Text);

/* �õ���ǰҳ��������Ϣ */
T_WORD fnCPS_GetCurAudio(T_Composer_Audio*Audio);

/* �õ���ǰҳ����Ƶ��Ϣ */
T_WORD fnCPS_GetCurVideo(T_Composer_Video*Video);

/* �õ���ǰҳ������ */
T_WORD fnCPS_GetCurSlideIndex(T_VOID);

/*�õ���ҳ��*/
T_WORD fnCPS_GetTotalSlides(T_VOID);

/* �õ���ǰ�༭���ŵ���Դ��С */
T_WORD fnCPS_GetTotalSize(T_VOID);

/* �õ���ǰ�༭���ŵ�Body PDU */
T_UBYTE* fnCPS_GetMmsContentPdu(T_WORD *Size);




#ifdef __cplusplus
}
#endif  //end of #ifdef __cplusplus 


#endif
