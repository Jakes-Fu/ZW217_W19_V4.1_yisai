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

#define COMPOSER_SUCCESS					0		/* 操作成功 */
#define COMPOSER_MEMERY_ALLOC_FAIL		-1		/* 内存分配失败 */
#define COMPOSER_MODULE_IS_USING			-2		/* 模块正处于编辑模式 */
#define COMPOSER_MEDIA_HAS_EXIST			-3		/* 资源已经存在 */
#define COMPOSER_MEDIA_EXIST_VIDEO		-4		/* 已经存在视频 */
#define COMPOSER_MEDIA_EXIST_IMG_AUD	-5		/* 已经存在声音和图像*/
#define COMPOSER_REACH_MAX_SLIDE			-6		/* 已经达到了最大slide */
#define COMPOSER_JUST_ONE_EMPTY_SLIDE	-7		/* 当前只剩唯一空白页 */
#define COMPOSER_MEDIA_NOT_EXIST			-8		/* 资源不存在 */
#define COMPOSER_GOTO_THE_END			-9		/* 已到了最后页面  */
#define COMPOSER_GOTO_THE_FIRST			-10		/* 已到第一个页面 */
#define COMPOSER_GLOBE_VAR_ERROR			-11		/* 内部全局变量错误 */
#define COMPOSER_INPUT_PARAM_ERROR		-12		/* 函数参数错误 */
#define COMPOSER_CURRENT_SLIDE_NOT_EXIST	-13	/* 当前slide不存在 */
#define COMPOSER_UNKNOWN_ERROR			-16		/* 不确定错误 */


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

/* 显示矩形区域结构  */
typedef struct{ /*record display position */
	T_WORD x; /*x start coordinate of screen*/
	T_WORD y;/*y start coordinate of screen*/
	T_WORD width;	/*width of display position*/
	T_WORD height;	/*height of display postion*/
}T_Composer_Rectangle;


/*插入图片的资源信息*/
typedef struct T_Composer_Image{
	T_UBYTE* ImgBuf;	/*image data*/
	T_CHAR*  Type;		/*image type*/
	T_WORD	begin;		/*begin time of image display*/
	T_WORD	end;		/*end time of image display*/
	T_WORD size;		/*size of image data*/
	T_Composer_Rectangle region;  /* image's region   */
}T_Composer_Image;

/*插入文本的资源信息*/
typedef struct T_Composer_Text{
	T_UBYTE* TxtBuf; 	/*text data*/
	T_WORD	begin;		/*begin time of text show*/
	T_WORD	end;		/*end time of text show*/
	T_WORD size;		/* the size of the text content */
	T_Composer_Rectangle region;  /* text's region  */
}T_Composer_Text;


/*插入声音资源信息*/
typedef struct T_Composer_Audio{
	T_UBYTE* AudBuf;	/*audio data buffer*/
	T_CHAR *Type;		/* audio type  */
	T_WORD	begin;		/*begin time of audio play*/
	T_WORD	end;		/*end time of audio play*/
	T_WORD size;		/*size of audio data*/
}T_Composer_Audio;

/*插入声音资源信息*/
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
/* 新建彩信 */
T_WORD fnCPS_NewMms(T_VOID);

/* 再编辑彩信 */
T_WORD fnCPS_ReEditMms(T_UBYTE*Pdu,T_WORD Size);

/* 退出编辑模块 */
T_VOID fnCPS_Quit (T_VOID);

/* 检查当前页面是否包含图像 */
T_BOOL fnCPS_CurImageExist(T_VOID);

/* 检查当前页面是否包含文本 */
T_BOOL fnCPS_CurTextExist(T_VOID);

/* 检查当前页面是否包含音乐 */
T_BOOL fnCPS_CurAudioExist(T_VOID);

/* 检查当前页面是否包含视频 */
T_BOOL fnCPS_CurVideoExist(T_VOID);

/* 在当前页添加图像 */
T_WORD fnCPS_AddImage(T_Composer_Image*Image);

/* 在当前页添加文本 */
T_WORD fnCPS_AddText(T_Composer_Text*Text);

/* 在当前页添加音乐 */
T_WORD fnCPS_AddAudio(T_Composer_Audio*Audio);

/* 在当前页添加视频 */
T_WORD fnCPS_AddVideo(T_Composer_Video*Video);

/* 在当前页后面添加新的空白页面 */
T_WORD fnCPS_AddSlide (T_VOID);

/* 设置当前页面的延时 */
T_WORD fnCPS_SetCurSlideDuration(T_WORD delay);

/* 获得当前页面的延时 */
T_WORD fnCPS_GetCurSlideDuration(T_VOID);

/* 从当前页删除图像 */
T_WORD fnCPS_DeleteCurImage(T_VOID);

/* 从当前页删除文本 */
T_WORD fnCPS_DeleteCurText(T_VOID);

/* 从当前页删除音乐 */
T_WORD fnCPS_DeleteCurAudio(T_VOID);

/* 从当前页删除视频 */
T_WORD fnCPS_DeleteCurVideo(T_VOID);

/* 删除当前页面，包括该页的资源 */
T_WORD fnCPS_DeleteCurSlide(T_VOID);

/* 对编辑模块进行清空，放弃已完成的编辑工作 */
T_VOID fnCPS_Empty(T_VOID);

/*  移动到下个页面 */
T_WORD fnCPS_GoNextSlide (T_VOID);

/*  移动到上个页面 */
T_WORD fnCPS_GoPreSlide (T_VOID);

/*  得到当前页的图像信息 */
T_WORD fnCPS_GetCurImage(T_Composer_Image*Image);

/* 得到当前页的文本信息 */
T_WORD fnCPS_GetCurText(T_Composer_Text*Text);

/* 得到当前页的音乐信息 */
T_WORD fnCPS_GetCurAudio(T_Composer_Audio*Audio);

/* 得到当前页的视频信息 */
T_WORD fnCPS_GetCurVideo(T_Composer_Video*Video);

/* 得到当前页的索引 */
T_WORD fnCPS_GetCurSlideIndex(T_VOID);

/*得到总页数*/
T_WORD fnCPS_GetTotalSlides(T_VOID);

/* 得到当前编辑彩信的资源大小 */
T_WORD fnCPS_GetTotalSize(T_VOID);

/* 得到当前编辑彩信的Body PDU */
T_UBYTE* fnCPS_GetMmsContentPdu(T_WORD *Size);




#ifdef __cplusplus
}
#endif  //end of #ifdef __cplusplus 


#endif
