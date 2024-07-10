#ifndef IFLYTEK_VOICE_MAGICVOICE_2_IVMAGICVOICE_H
#define IFLYTEK_VOICE_MAGICVOICE_2_IVMAGICVOICE_H

#include "ivDefine.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
*	COMMON DEFINES
*/
#define ivMV_SUGGEST_HEAPSIZE	(16*1024)		/* 完整版推荐分配的堆大小16k */
#define ivMV_4KSUGGEST_HEAPSIZE (3*1024 + 512)	/* 4k裁剪版推荐分配的堆大小3.5k */
#define ivMV_ProSize			64				/* ivMV_RunStep一次处理的samples个数 */
#define ivMV_MAXPACKNUM			10				/* 背景资源包的最大数目 */
/* 
* param id
 */
#define ivMV_PARAM_CHANGE           0		/* 音色 */
#define	ivMV_PARAM_ROLE				1		/* 角色 */
#define	ivMV_PARAM_EFFECT			2		/* 音效 */
#define ivMV_PARAM_EQID             3		/* 均衡器 */	
#define ivMV_PARAM_BGID				4		/* 背景声 */
#define ivMV_PARAM_BGVolume			5		/* 背景声音量 */
#define ivMV_PARAM_MVolume          6		/* 主音量 */
 
/*
* change value
*/
#define ivMV_CHANGE_NONE			0		
#define ivMV_CHANGE_WOMAN			1		/* 女性化 */
#define ivMV_CHANGE_MAN				2		/* 男性化 */
#define ivMV_CHANGE_CHILD			3		/* 儿童化 */
#define ivMV_CHANGE_OLD				4		/* 老年化 */
#define ivMV_CHANGE_CARTOON			5		/* 卡通化 */
#define ivMV_CHANGE_YOUNG			6		/* 年轻化 */
/*
* role value
*/
#define	ivMV_ROLE_NONE				0
#define	ivMV_ROLE_OPTIMUS			1		/* 擎天柱 */
#define	ivMV_ROLE_DDUCK				2		/* 唐老鸭 */
#define	ivMV_ROLE_ROBOT				3		/* 机器人 */
#define	ivMV_ROLE_EVA				4		/* eva */	
#define	ivMV_ROLE_WALLE				5		/* walle */
#define	ivMV_ROLE_RAPMAN			6		
#define	ivMV_ROLE_RAPWOMAN			7

/* 
* effect value
*/
#define ivMV_VE_NONE				0
#define ivMV_VE_METAL				1		/* 金属 */
#define ivMV_VE_ECHO				2		/* 回声 */
#define ivMV_VE_WANDER				3		/* 忽远忽近 */
#define ivMV_VE_REVERB				4		/* 混响 */
#define ivMV_VE_CHORUS				5		/* 合唱 */
#define ivMV_VE_UWATER				6		/* 水下 */
/*
* BG value
*/
#define ivMV_BGID_NONE				0
#define ivMV_BGID_WIND				1		/* 风 */
#define ivMV_BGID_RAIN				2		/* 雨 */
#define ivMV_BGID_BIRD				3		/* 鸟 */
#define ivMV_BGID_CRICKET			4		/* 蟋蟀 */	
/*
* EQ value
*/
#define ivMV_EQID_NONE				0		
#define ivMV_EQID_SUGGEST			1		/* 推荐 */
#define ivMV_EQID_POP				2		/* 流行 */
#define ivMV_EQID_ROCK				3		/* 摇滚 */
#define ivMV_EQID_METAL				4		/* 金属 */
#define ivMV_EQID_DANCE				5		/* 舞曲 */
#define ivMV_EQID_COUNTRY			6		/* 乡村音乐 */
#define ivMV_EQID_JAZZ				7		/* 爵士 */
#define ivMV_EQID_CLASSICAL			8		/* 古典 */
#define ivMV_EQID_BLUES				9		/* 布鲁斯 */
#define ivMV_EQID_OPERA				10		/* 歌剧 */
#define ivMV_EQID_SPEECH1			11		/* 语音1 */
#define ivMV_EQID_SPEECH2			12		/* 语音2 */	
/*
* return value
*/
#define ivMV_OK						0		
#define ivMV_INVARG					1
#define ivMV_INVALID_PARAM			2
#define ivMV_INVALID_HANDLE			3
#define ivMV_BUFFFER_FULL			4
#define ivMV_BUFFFER_EMPTY			5
#define ivMV_DATA_EMPTY				6
#define ivMV_DATA_FULL				7
#define ivMV_NOT_SUPPORT			8 
/*
* BG 独立模块宏
*/
#define ivMV_BGHEAP_TOO_SMALL		9
#define ivMV_BG_INVPARAM       		10
#define ivMV_INVRES            		11
#define ivMV_ENGINE_ERROR			12
#define ivMVBGParam_Volume          0x0001		
#define ivMVBGParam_SoundID         0x0003

typedef ivPointer ivHMV; /* 魔音实例 */
typedef ivPointer ivHBG; /* 背景声实例 */
typedef ivUInt16 ivStatus; 
/* 资源包读取 */
typedef struct tagMVResPackDesc ivTMVResPackDesc;
typedef struct tagMVResPackDesc ivPtr ivPMVResPackDesc;
struct tagMVResPackDesc
{
	ivPointer		pCBParam;		/* 回调参数 */
	ivCBReadBGRes	pfnRead;		/* 回调函数 */
};
/* 用户系统信息 */
typedef struct tagMVUserSys ivTMVUserSys;
typedef struct tagMVUserSys ivPtr ivPMVUserSys;
struct tagMVUserSys 
{
	ivPointer			lpHeap;			/* 堆地址 */
	ivSize				nHeapSize;	 	/* 堆大小 */
	ivPointer			pLogCBParam;	/* log回调参数 */
	ivCBWriteLog		pLogWrite;		/* log回调函数 */
	ivPMVResPackDesc    pMVResPackDesc; /* 资源包数组地址 */
	ivUInt32			nMVResPackDesc; /* 资源包个数 */
};
/*
*	FUNCTION ENTRIES
 */
ivStatus ivCall ivMV_Init(	ivHMV ivPtr 	lphMV,
							ivPMVUserSys      pUserSys);

ivStatus ivCall ivMV_AppendData(ivHMV			pMVKernel,
								ivPCInt16		pInData,
								ivUInt32		nSample);

ivStatus ivCall ivMV_GetData(ivHMV			pMVKernel,
							ivPInt16		pOutData,
							ivPUInt32		pnSample);

ivStatus ivCall ivMV_RunStep(ivHMV		pMVKernel);
 
ivStatus ivCall ivMV_SetParam(ivHMV			pMVKernel, 
							  ivInt32		nMVParamId,
							  ivInt32		nMVParamValue);


/* 独立的背景API */

/* 用户系统信息 */
typedef struct tagBGUserSys ivTBGUserSys;
typedef struct tagBGUserSys ivPtr ivPBGUserSys;
struct tagBGUserSys 
{
	ivSize				nHeapSize;	 	/* 堆大小 */
	ivPMVResPackDesc    pMVResPackDesc; /* 资源包数组地址 */
	ivUInt32			nMVResPackDesc; /* 资源包个数 */
};

ivStatus ivCall ivMV_Background_Init(ivHBG		 pBGHeap,		/* 背景音对象的内存首地址（必须4字节对齐） */
									 ivPBGUserSys     pUserSys
									 );

ivStatus ivCall ivMV_Background_SetParam(ivHBG		 pBGHeap,		/* 背景音对象的内存首地址（必须4字节对齐） */
										 ivInt32     nMVParam,		/* 需要设定的属性 */
										 ivInt32     nMVParamValue	/* 属性的值 */
										 );

ivStatus ivCall ivMV_Background_Process(ivHBG		pBGHeap,		/* 背景音对象的内存首地址（必须4字节对齐） */
										ivPInt16    pOutBuff,       /* 输出缓冲（可以和输入缓冲重叠） */
										ivPInt16    pInBuff,        /* 输入缓冲 */
										ivSize      nSize           /* 输入数据字节数 */
											);

			

#ifdef __cplusplus
}
#endif

#endif
