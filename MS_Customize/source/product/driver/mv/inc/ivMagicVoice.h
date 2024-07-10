#ifndef IFLYTEK_VOICE_MAGICVOICE_2_IVMAGICVOICE_H
#define IFLYTEK_VOICE_MAGICVOICE_2_IVMAGICVOICE_H

#include "ivDefine.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
*	COMMON DEFINES
*/
#define ivMV_SUGGEST_HEAPSIZE	(16*1024)		/* �������Ƽ�����ĶѴ�С16k */
#define ivMV_4KSUGGEST_HEAPSIZE (3*1024 + 512)	/* 4k�ü����Ƽ�����ĶѴ�С3.5k */
#define ivMV_ProSize			64				/* ivMV_RunStepһ�δ����samples���� */
#define ivMV_MAXPACKNUM			10				/* ������Դ���������Ŀ */
/* 
* param id
 */
#define ivMV_PARAM_CHANGE           0		/* ��ɫ */
#define	ivMV_PARAM_ROLE				1		/* ��ɫ */
#define	ivMV_PARAM_EFFECT			2		/* ��Ч */
#define ivMV_PARAM_EQID             3		/* ������ */	
#define ivMV_PARAM_BGID				4		/* ������ */
#define ivMV_PARAM_BGVolume			5		/* ���������� */
#define ivMV_PARAM_MVolume          6		/* ������ */
 
/*
* change value
*/
#define ivMV_CHANGE_NONE			0		
#define ivMV_CHANGE_WOMAN			1		/* Ů�Ի� */
#define ivMV_CHANGE_MAN				2		/* ���Ի� */
#define ivMV_CHANGE_CHILD			3		/* ��ͯ�� */
#define ivMV_CHANGE_OLD				4		/* ���껯 */
#define ivMV_CHANGE_CARTOON			5		/* ��ͨ�� */
#define ivMV_CHANGE_YOUNG			6		/* ���ữ */
/*
* role value
*/
#define	ivMV_ROLE_NONE				0
#define	ivMV_ROLE_OPTIMUS			1		/* ������ */
#define	ivMV_ROLE_DDUCK				2		/* ����Ѽ */
#define	ivMV_ROLE_ROBOT				3		/* ������ */
#define	ivMV_ROLE_EVA				4		/* eva */	
#define	ivMV_ROLE_WALLE				5		/* walle */
#define	ivMV_ROLE_RAPMAN			6		
#define	ivMV_ROLE_RAPWOMAN			7

/* 
* effect value
*/
#define ivMV_VE_NONE				0
#define ivMV_VE_METAL				1		/* ���� */
#define ivMV_VE_ECHO				2		/* ���� */
#define ivMV_VE_WANDER				3		/* ��Զ���� */
#define ivMV_VE_REVERB				4		/* ���� */
#define ivMV_VE_CHORUS				5		/* �ϳ� */
#define ivMV_VE_UWATER				6		/* ˮ�� */
/*
* BG value
*/
#define ivMV_BGID_NONE				0
#define ivMV_BGID_WIND				1		/* �� */
#define ivMV_BGID_RAIN				2		/* �� */
#define ivMV_BGID_BIRD				3		/* �� */
#define ivMV_BGID_CRICKET			4		/* �� */	
/*
* EQ value
*/
#define ivMV_EQID_NONE				0		
#define ivMV_EQID_SUGGEST			1		/* �Ƽ� */
#define ivMV_EQID_POP				2		/* ���� */
#define ivMV_EQID_ROCK				3		/* ҡ�� */
#define ivMV_EQID_METAL				4		/* ���� */
#define ivMV_EQID_DANCE				5		/* ���� */
#define ivMV_EQID_COUNTRY			6		/* ������� */
#define ivMV_EQID_JAZZ				7		/* ��ʿ */
#define ivMV_EQID_CLASSICAL			8		/* �ŵ� */
#define ivMV_EQID_BLUES				9		/* ��³˹ */
#define ivMV_EQID_OPERA				10		/* ��� */
#define ivMV_EQID_SPEECH1			11		/* ����1 */
#define ivMV_EQID_SPEECH2			12		/* ����2 */	
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
* BG ����ģ���
*/
#define ivMV_BGHEAP_TOO_SMALL		9
#define ivMV_BG_INVPARAM       		10
#define ivMV_INVRES            		11
#define ivMV_ENGINE_ERROR			12
#define ivMVBGParam_Volume          0x0001		
#define ivMVBGParam_SoundID         0x0003

typedef ivPointer ivHMV; /* ħ��ʵ�� */
typedef ivPointer ivHBG; /* ������ʵ�� */
typedef ivUInt16 ivStatus; 
/* ��Դ����ȡ */
typedef struct tagMVResPackDesc ivTMVResPackDesc;
typedef struct tagMVResPackDesc ivPtr ivPMVResPackDesc;
struct tagMVResPackDesc
{
	ivPointer		pCBParam;		/* �ص����� */
	ivCBReadBGRes	pfnRead;		/* �ص����� */
};
/* �û�ϵͳ��Ϣ */
typedef struct tagMVUserSys ivTMVUserSys;
typedef struct tagMVUserSys ivPtr ivPMVUserSys;
struct tagMVUserSys 
{
	ivPointer			lpHeap;			/* �ѵ�ַ */
	ivSize				nHeapSize;	 	/* �Ѵ�С */
	ivPointer			pLogCBParam;	/* log�ص����� */
	ivCBWriteLog		pLogWrite;		/* log�ص����� */
	ivPMVResPackDesc    pMVResPackDesc; /* ��Դ�������ַ */
	ivUInt32			nMVResPackDesc; /* ��Դ������ */
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


/* �����ı���API */

/* �û�ϵͳ��Ϣ */
typedef struct tagBGUserSys ivTBGUserSys;
typedef struct tagBGUserSys ivPtr ivPBGUserSys;
struct tagBGUserSys 
{
	ivSize				nHeapSize;	 	/* �Ѵ�С */
	ivPMVResPackDesc    pMVResPackDesc; /* ��Դ�������ַ */
	ivUInt32			nMVResPackDesc; /* ��Դ������ */
};

ivStatus ivCall ivMV_Background_Init(ivHBG		 pBGHeap,		/* ������������ڴ��׵�ַ������4�ֽڶ��룩 */
									 ivPBGUserSys     pUserSys
									 );

ivStatus ivCall ivMV_Background_SetParam(ivHBG		 pBGHeap,		/* ������������ڴ��׵�ַ������4�ֽڶ��룩 */
										 ivInt32     nMVParam,		/* ��Ҫ�趨������ */
										 ivInt32     nMVParamValue	/* ���Ե�ֵ */
										 );

ivStatus ivCall ivMV_Background_Process(ivHBG		pBGHeap,		/* ������������ڴ��׵�ַ������4�ֽڶ��룩 */
										ivPInt16    pOutBuff,       /* ������壨���Ժ����뻺���ص��� */
										ivPInt16    pInBuff,        /* ���뻺�� */
										ivSize      nSize           /* ���������ֽ��� */
											);

			

#ifdef __cplusplus
}
#endif

#endif
