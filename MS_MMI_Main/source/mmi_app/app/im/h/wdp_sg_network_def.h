#ifndef SG_NETWORK_DEF_H
#define SG_NETWORK_DEF_H

#include "wdp_SG_Def.h"
#include "wdp_SG_Interface_net.h"

#define		PROJECT_IMEI_LEN				30
#define		PROJECT_ISEI_LEN				30
#define 	PROJECT_RESOLUTION				20
#define		PROJECT_MODE_ID					12
#define 	PROJECT_HTTP_TIME_SIZE			16
#define 	PROJECT_REST_DATA_SIZE			30
#define		DISPLAY_TO_USER_STRING_LEN		51
#define 	PROJECT_PLATFORM				25
#define		MAX_SAVE_FILE_PATH_LENGTH		100
#define 	MAX_FILE_NAME_SIZE				MAX_SAVE_FILE_PATH_LENGTH
#define		INT_NEGATIVE_FLAG				0x80000000


typedef void (*SGReadContentCB)(SG_UINT totalSize,SG_UINT curSize); /*hwx add for mtk interface called by out*/

//*********************初始化开始*************************************

typedef struct SG_InitInputStruct_t 
{
	SGReadContentCB readContentCb; /*hwx add for mtk interface called by out*/
	SG_UINT8  sg_imei[PROJECT_IMEI_LEN];
	SG_UINT8  sg_imsi[PROJECT_ISEI_LEN];
	SG_UINT8  resolution[PROJECT_RESOLUTION];
	SG_UINT8  mode_id[PROJECT_MODE_ID];
	SG_UINT8  current_time[PROJECT_HTTP_TIME_SIZE];
}SG_InitInputStruct;

typedef enum{
	SG_HTTP_GET_KEY_UPDATE = 0,
	SG_HTTP_GET_SKIN_BIN,
	SG_HTTP_GET_SKIN,
	SG_HTTP_GET_TYPEMAX,		
	SG_HTT_GET_END = 0xFFFFFFF, 
}SGHttpGetType;

typedef struct{
	SG_INT initFalg;
	SG_INT8		imsi[PROJECT_ISEI_LEN];
	SG_INT8		imei[PROJECT_IMEI_LEN];
	SG_INT8		resolution[PROJECT_RESOLUTION];
	SG_INT8		current_time[PROJECT_HTTP_TIME_SIZE];
	SG_INT8		mode_id[PROJECT_MODE_ID];
	SG_INT8		platform[PROJECT_PLATFORM];
}T_HTTPUserInfor;

#define HEAD_BUFFER_LEN 2048
#define POST_BUFFER_LEN 2048
#define NO_CONTENT_LEN_MAX_SIZE (100*1024)
#define HEAD_NO_ANASIZE 50
#define TOTAL_REV_BUFFER_SIZE (100*1024)
#define PACKET_MAX_PACKET 2048												/*test*/
#define SG_SOC_WOULDBLOCK -2
#define SG_MIN_HHEAD_LEN 10
typedef enum																/*SG network state*/
{
	SG_HTTP_STATE_IDLE = 0, 
	SG_HTTP_STATE_INIT,
	SG_HTTP_STATE_CREATE,
	SG_HTTP_STATE_CONNECT,
	SG_HTTP_STATE_SEND,
	SG_HTTP_STATE_GET_HEAD,
	SG_HTTP_STATE_GET_CONTENT,
	SG_HTTP_STATE_FINISHED,
	SG_HTTP_STATE_END = 0xFFFFFFF
}SG_HTTP_STATE;

typedef enum
{
	SG_HTTP_OK = 0,
	SG_HTTP_TIMEOUT,
	SG_HTTP_CREETE_ERROR,
	SG_HTTP_CONNECT_ERROR,
	SG_HTTP_SEND_ERROR,
	SG_HTTP_READHEAD_ERROR,
	SG_HTTP_READCONTENT_ERROR,
	SG_HTTP_CLOSE_ERROR,
	SG_HTTP_ERROR_NONE,
	SG_HTTP_MEM_ERROR,
	SG_HTTP_ERROR_END = 0xFFFFFFF
}SG_HTTP_ERROR;


typedef enum
{
	SG_EVENT_SOC_NONE    = 0,
	SG_EVENT_SOC_READ    = 0x01,  													/* Notify for read */
	SG_EVENT_SOC_WRITE   = 0x02,  													/* Notify for write */
	SG_EVENT_SOC_ACCEPT  = 0x04, 													/* Notify for accept */
	SG_EVENT_SOC_CONNECT = 0x08, 												 	/* Notify for connect */
	SG_EVENT_SOC_CLOSE   = 0x10,  													/* Notify for close */
	SG_EVENT_SOC_END = 0xFFFFFFF
}SG_HTTP_SOCEVENT;

typedef void (*SG_FUNCPTR)(void);
typedef void (*SGFinishCallback)(SG_HTTP_ERROR);						/* infact easy to change to callback */
typedef void (*SGStartReadCallback)(SG_UINT);							
typedef void (*SGReadContentCallback)(SG_UINT8 *,SG_UINT size,SG_UINT curSize);
					/* call back */

typedef struct{
	SG_INT socketId;
	SG_HTTP_STATE httpState;
	SG_HTTP_ERROR HttpError;
	SG_UINT postLen;
	SG_UINT postSize;
	SG_UINT curheadSize;
	SG_UINT totalSize;
	SG_UINT curSize;
	SG_INT8 postContent[POST_BUFFER_LEN];
	SG_INT8 headContent[HEAD_BUFFER_LEN];
	SG_INT8 RecContent[PACKET_MAX_PACKET];											/* temp mem statis */
	SGFinishCallback finishCallback;
	SGStartReadCallback startReadCallback;
	SGReadContentCallback readContentCallback;
}SG_HTTP_CONFIG;

typedef struct{
	SG_INT8 *pPostContent;
	SGFinishCallback finishCallback;
	SGStartReadCallback startReadCallback;
	SGReadContentCallback readContentCallback;
}SG_HTTP_REQUEST;

/**************************hwx add end ***********************************/





//********************更新主接口开始**************************************

typedef struct SG_getDataInputStruct_t 
{
	SGHttpGetType type;
	SG_UINT16 *uploadfileName;
	SG_UINT16 *destfileName;
	SG_INT8 *urldata;
	SG_UINT8  sg;	
}SG_getDataInputStruct;


typedef struct SG_getDataOutputStruct_t 
{
	SG_UINT16 infileName[MAX_FILE_NAME_SIZE];
	SG_UINT16 uploadfileName[MAX_FILE_NAME_SIZE];
	SG_UINT totalSentSection;
	SG_UINT nowSentSection;
	SG_HTTP_STATE httpState;
	SGHttpGetType type;	
	SG_HTTP_ERROR errorCode;
	SG_UINT revlen;
	SG_UINT totlalLen;
	SG_UINT16  displayStr[DISPLAY_TO_USER_STRING_LEN+2];	
}SG_getDataOutputStruct;



//===============================================================
//		此函数为网络初始化函数
//
//		传入网络连接过程中需要的参数
//
//		URL的生成，网络需要空间的申请等等，都在此函数中完成
//
//		初始化成功返回0，不成功返回错误号
//================================================================
SG_INT SG_InterNet_Init(SG_InitInputStruct* sg_initInput);


//**********************初始化结束****************************************

//========================================================================================================
//		更新主接口函数，调用此函数的更新到词库或者皮肤
//
//		传入更新过程中需要的参数（SG_getDataInputStruct）
//
//		将结果放入到SG_getDataInputStruct中返回
//========================================================================================================

SG_INT SG_InterNet_Get_Data(SG_getDataInputStruct* sg_getDataInput,SG_getDataOutputStruct* sg_getDataOutput);

//========================================================================================================
//		取消联网接口
//
//		传入更新过程中需要的参数（SG_getDataInputStruct）
//
//		将结果放入到SG_getDataInputStruct中返回
//========================================================================================================
SG_INT SG_InterNet_Cancell_Data(SG_getDataInputStruct* sg_getDataInput,SG_getDataOutputStruct* sg_getDataOutput);
//*********************更新主接口结束****************************************************************************




//*****************网络退出开始*************************************************************

//========================================================================
//		退出网络函数，在更新结束，或者更新失败，或者用户主动取消更新
//
//		调用此函数，释放当前占用的空间
//
//========================================================================
SG_INT SG_InterNet_Exit(void);

//*****************网络退出结束*************************************************************





//*****************获取网络参数开始*************************************************************

//========================================================================
//		获取网络参数函数，可以得到更新周期等参数
//
//
//========================================================================
SG_UINT8 WD_GetUFByIndex(SG_UINT8 index);
SG_UINT8 WD_GetDftUFIndex(void);
SG_UINT8 WD_GetDftUTIndex(SG_UINT8 *p);

//*****************获取网络参数结束*************************************************************


#endif
