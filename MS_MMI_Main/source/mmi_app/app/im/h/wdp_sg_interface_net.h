#ifndef SG_INTERFACE_H
#define SG_INTERFACE_H

#include "sci_types.h"
#include "wdp_SG_string.h"
//#define  WINDOW32  //移植到各平台时，请关闭此宏，并将文件下方，对应的函数实现。
#ifdef WIN32
typedef SG_INT   FILE_HANDLE;
//#include "WTYPES.h"
// #include "WINSOCK2.h"
//typedef HANDLE   FILE_HANDLE;
#elif defined(MTK)
typedef SG_INT   FILE_HANDLE;
#else
#ifndef FILE_HANDLE
typedef SG_INT   FILE_HANDLE;
#endif
#define NULL	0
#endif

//////【文件操作】
typedef enum{
	EOPEN_FOR_READ_WRITE = 0x00000000L,
	EOPEN_FOR_READ = 0x00000100L,
	EOPEN_FOR_WRITE = 0x00000001L,
	EOPEN_FOR_CREATE_ALWAYS = 0x00020000L
}SG_FILE_OPEN_TYPE;
#ifndef WIN32
PUBLIC SG_INT SG_FS_Open(const SG_UINT16 * FileName, SG_UINT Flag);
PUBLIC SG_INT SG_FS_Close(FILE_HANDLE FileHandle);
PUBLIC SG_INT SG_FS_Read(FILE_HANDLE FileHandle, void * DataPtr, SG_UINT Length, SG_UINT * Read);
PUBLIC SG_INT SG_FS_Write(FILE_HANDLE FileHandle, void * DataPtr, SG_UINT Length, SG_UINT * Written);
PUBLIC SG_INT SG_FS_Seek(FILE_HANDLE FileHandle, SG_INT Offset, SG_UINT Whence);
PUBLIC SG_INT SG_FS_Delete(const SG_UINT16 * FileName);
PUBLIC SG_INT SG_FS_Copy(const SG_UINT16 * SrcFullPath, const SG_UINT16 * DstFullPath);
PUBLIC SG_INT SG_FS_Move(const SG_UINT16 * SrcFullPath, const SG_UINT16 * DstFullPath);
PUBLIC SG_INT SG_FS_FindNext(SG_INT FileHandle, SG_INT *IsFile, SG_UINT16 *FileName, SG_UINT MaxLength);
PUBLIC SG_INT SG_FS_FindFirst(const SG_UINT16 *NamePattern, SG_INT *IsFile, SG_UINT16 *FileName, SG_UINT MaxLength);
PUBLIC SG_INT SG_FS_FindClose(SG_INT FileHandle);
PUBLIC void	SG_IME_Init_Save_Net_Word(void);
/******************************************************************************/
// Description: assert实现
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void SG_Assert(SG_INT express);

/******************************************************************************/
// Description: 获取tick count
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC SG_UINT SG_GetTickCount(void);
#endif

PUBLIC void SG_soc_excuteTimer(void);

/////【内存管理】
PUBLIC void *SGMallocMem(SG_UINT size);
PUBLIC void SGFreeMem(void *ptr);

#define   SG_MALLOC     SGMallocMem
#define   SG_FREE       SGFreeMem
#define   sg_sprintf    sprintf


//////【加密接口】
PUBLIC void SG_che_init(void* che_context, SG_UINT type);
PUBLIC void SG_che_deinit(void* che_context);

typedef struct  SG_processInput_t
{
	void* che_context;
	SG_UINT type;// type用来表示不同的加密算法
	SG_UINT mode;
	SG_UINT act;
	SG_UINT source;
	SG_UINT dest;
	SG_UINT source_length;
	SG_UINT final;
}SG_processInput;

PUBLIC SG_UINT8 che_process(SG_processInput* processInput);

#define SG_NET
//////////【socket接口】

/***********************************************************************************************
******************** hwx test for net work *****************************************************
***********************************************************************************************/

typedef enum
{
	ESG_EVENT_SOC_NONE    = 0,
	ESG_EVENT_SOC_READ    = 0x01,  													/* Notify for read */
	ESG_EVENT_SOC_WRITE   = 0x02,  													/* Notify for write */
	ESG_EVENT_SOC_ACCEPT  = 0x04, 													/* Notify for accept */
	ESG_EVENT_SOC_CONNECT = 0x08, 												 	/* Notify for connect */
	ESG_EVENT_SOC_CLOSE   = 0x10, 													/* Notify for close */
	ESG_EVENT_SOC_END   = 0xFFFFFFF,
}ESG_HTTP_SOCEVENT;

typedef void (*SGEventCallBack)(SG_INT socketID,ESG_HTTP_SOCEVENT event);	
extern void SG_soc_setEventCallBack(SGEventCallBack callback);

typedef struct SG_SetOptSocketInput_t 
{
	SG_INT			s;

}SG_SetOptSocketInput;

PUBLIC SG_INT SG_soc_getConfig(SG_UINT8 *ip, SG_UINT *port, SG_INT *accountID);
PUBLIC void SG_soc_setopt(SG_SetOptSocketInput * setOptSocketInput);

/*******************************        hwx test end ********************************************************/      

typedef struct SG_CreateSocketInput_t 
{
	SG_UINT		domain;
	SG_UINT		type;
	SG_UINT		protocol;
	SG_UINT       mod_id;
	SG_UINT		nwk_account_id;
}SG_CreateSocketInput;

PUBLIC SG_INT SG_soc_create(SG_CreateSocketInput* createSocketInput);

//=========================================================================


typedef struct SG_CloseSocketInput_t 
{
	SG_INT			s;

}SG_CloseSocketInput;

PUBLIC SG_INT SG_soc_close(SG_CloseSocketInput* closeSocketInput);

//==============================================================================

typedef struct SG_sockaddr_struct_t 
{
	SG_UINT8*		ipAddr;//ip地址
	SG_UINT		point;//端口号
}SG_sockaddr_struct;


//===============================================================================
typedef struct SG_BindSocketInput_t 
{
	SG_INT			s;
	SG_sockaddr_struct	*addr;
	SG_INT			addrlen;
	
}SG_BindSocketInput;

PUBLIC SG_INT SG_soc_bind(SG_BindSocketInput* bindSocketInput);

//================================================================


typedef struct SG_ListenSocketInput_t 
{
	SG_INT			s;
	SG_INT		backlog;
	
}SG_ListenSocketInput;

PUBLIC SG_INT SG_soc_listen(SG_ListenSocketInput* listenSocketInput);

//============================================================================

typedef struct SG_AcceptSocketInput_t 
{
	SG_INT			s;
	SG_sockaddr_struct *addr;
	SG_INT		addrlen;
	
}SG_AcceptSocketInput;

PUBLIC SG_INT SG_soc_accept(SG_AcceptSocketInput* acceptSocketInput);
//============================================================================

typedef struct SG_ConnectSocketInput_t 
{
	SG_INT			s;
	SG_sockaddr_struct *addr;
	SG_INT		addrlen;
	
}SG_ConnectSocketInput;

PUBLIC SG_INT SG_soc_connect(SG_ConnectSocketInput* connectSocketInput);


//===========================================================================

typedef struct SG_SendToSocketInput_t 
{
	SG_INT		s;
	const void		*buf;
	SG_INT	    len;
	SG_INT 	    flags;
	SG_sockaddr_struct *addr;
	SG_INT	    tolen;
	
}SG_SendToSocketInput;

PUBLIC SG_INT SG_soc_sendto(SG_SendToSocketInput* sendToSocketInput);

//========================================================================

typedef struct SG_SendSocketInput_t 
{
	SG_INT		s;
	const void		*buf;
	SG_INT	    len;
	SG_INT 	    flags;
}SG_SendSocketInput;

PUBLIC SG_INT SG_soc_send(SG_SendSocketInput* sendSocketInput);

//===========================================================================

typedef struct SG_RecvFromSocketInput_t 
{
	SG_INT		s;
	const void		*buf;
	SG_INT	    len;
	SG_INT 	    flags;
	SG_sockaddr_struct *fromaddr;
	SG_INT	    fromlen;
}SG_RecvFromSocketInput;

PUBLIC SG_INT SG_soc_recvfrom(SG_RecvFromSocketInput* recvFromSocketInput);

//===============================================================================

typedef struct SG_RecvSocketInput_t 
{
	SG_INT		s;
	const void		*buf;
	SG_INT	    len;
	SG_INT	    flags;
}SG_RecvSocketInput;

PUBLIC SG_INT SG_soc_recv(SG_RecvSocketInput*  recvSocketInput);

//==========================================================================

typedef struct SG_ShutDownSocketInput_t 
{
	SG_INT		s;
	SG_INT 	    how;
}SG_ShutDownSocketInput;

PUBLIC SG_INT8 SG_soc_shutdown(SG_ShutDownSocketInput* shutDownSocketInput);
PUBLIC int SG_Check_D_Space_Is_Enough(void);
PUBLIC int SG_Check_E_Space_Is_Enough(void);
PUBLIC void SG_Check_Build_Time(void);//6_19_1

//===================================================================================

#endif
