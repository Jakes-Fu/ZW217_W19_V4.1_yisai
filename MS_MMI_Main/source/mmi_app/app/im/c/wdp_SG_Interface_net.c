#ifndef WDP_SG_INTERFACE_NET_C
#define WDP_SG_INTERFACE_NET_C
#include "mmi_app_im_trc.h"
#include "mmk_app.h"
#include "wdp_SG_NetWork_Def.h"
#include "WD_Def.h"//6_12_2
#include "os_api.h"
#include "mmifmm_export.h"
#include "stdarg.h"
#include "version.h"//6_19_1
#include "wdp_update_wintab.h"//6_19_1
#define SERVER_ADDRESS 0x3D87B506 // "61.135.181.6"

#define SG_SUCCESS        0
#define SG_FAILED		 -1
#define TRACE_MAX_LENGTH     256
extern uint8 g_sg_save_position;//0:D盘,1:E盘,2:非D非E
extern BOOLEAN g_sg_IsWap;//6_12_3
LOCAL SG_INT8 g_sg_TraceStr[TRACE_MAX_LENGTH];

#define SG_UPDATE_WORD_MINIMIZE_FREESPACE   (270 * 1024)

#ifdef MAINLCD_SIZE_240X320
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (11 * 1024)
#elif defined MAINLCD_SIZE_128X160
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (8 * 1024)
#elif defined MAINLCD_SIZE_176X220
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (9 * 1024)
#elif defined MAINLCD_SIZE_240X400
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (11 * 1024)
#elif defined MAINLCD_SIZE_320X480
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (14 * 1024)
#else
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (14 * 1024)
#endif

#ifndef WIN32
#include "socket_types.h"
#include "wdp_update.h"
//#include "WDP_Customer.h"

/***********************************************************************
					内存管理
*************************************************************************/
PUBLIC void *SGMallocMem(SG_UINT size)
{
	return SCI_ALLOCA(size);
}

PUBLIC void SGFreeMem(void *ptr)
{
	SCI_FREE(ptr);/*lint !e438*/
}

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
/***********************************************************************
					网络部分
*************************************************************************/
LOCAL SGEventCallBack s_timercb = NULL;
PUBLIC void SG_soc_setEventCallBack(SGEventCallBack callback)
{
    s_timercb = callback;
}

/***********************************************************
                   window的定时器模拟
************************************************************/
PUBLIC void SG_soc_excuteTimer(void)
{
	if (s_timercb != NULL)
    {
        s_timercb(-1, 0);
    }
}

/***********************************************************
funtion name : SG_soc_setopt	  设置回调函数
intput param
		SG_SetOptSocketInput * setOptSocketInput

output param

		一定要设置成非阻塞模式
************************************************************/
PUBLIC void SG_soc_setopt(SG_SetOptSocketInput * setOptSocketInput)
{
#ifdef GPRS_SUPPORT
	if (-1 == sci_sock_setsockopt(setOptSocketInput->s, SO_NBIO, NULL))
#else
	if (1)
#endif
	{
		//error
		//SCI_TRACE_LOW:"wdp_SG_Interface_net.c SG_soc_setopt: error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_SG_INTERFACE_NET_70_112_2_18_2_29_30_299,(uint8*)"");
	}
}

/***********************************************************
funtion name : SG_soc_getConfig	  设置回调函数
intput param
		SG_UINT8 *ip, 
		SG_UINT *port, 
		SG_INT *accountID

output param

		获取Ip,accountID,端口
		系统不需要 accountID的换传0
************************************************************/
PUBLIC SG_INT SG_soc_getConfig(SG_UINT8 *ip, SG_UINT *port, SG_INT *accountID)
{
	*port = 80;
	ip[0] = 10;
	ip[1] = 0;
	ip[2] = 0;
	ip[3] = 172;
	*accountID = 0;//cbm_encode_data_account_id(14, 0, 0, 0); // according flatform  is different	

	return 0;
}

/***********************************************************
funtion name : SG_soc_create	  创建socket
intput param
		SG_CreateSocketInput* createSocketInput
output param
************************************************************/
PUBLIC SG_INT SG_soc_create(SG_CreateSocketInput* createSocketInput)
{
#ifdef GPRS_SUPPORT
    TCPIP_SOCKET_T socket_id = sci_sock_socket(AF_INET, SOCK_STREAM, 0, MMIAPIIM_SGGetNetId());

    //SCI_TRACE_LOW:"wdp_sg_interface_net.c SG_soc_create socket id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_SG_INTERFACE_NET_107_112_2_18_2_29_31_300,(uint8*)"d", socket_id);
	if (-1 == socket_id)  /*lint !e838*/
    {
        return SG_FAILED;
    }
    else
    {
		return (SG_INT)socket_id;
    }
#else
    return SG_FAILED;
#endif
}

/***********************************************************
funtion name : SG_soc_close	  关闭socket
intput param
		SG_CloseSocketInput* closeSocketInput
output param
************************************************************/
PUBLIC SG_INT SG_soc_close(SG_CloseSocketInput* closeSocketInput)
{
#ifdef GPRS_SUPPORT
	return sci_sock_socketclose(closeSocketInput->s); /*lint !e838*/
#else
	return 0;
#endif
}

/***********************************************************
funtion name : SG_soc_connect	 连接socket
intput param
		SG_ConnectSocketInput* connectSocketInput
output param
		阻塞为-2
************************************************************/
PUBLIC SG_INT SG_soc_connect(SG_ConnectSocketInput* connectSocketInput)
{
	int conn_err;
	struct sci_sockaddr sa;
	int socket_ret = 0;

    sa.family = AF_INET;
    sa.port = htons(80); /*lint !e845*/
	if (0 == MMIAPIIM_SGGetServerAddress())
	{
		SCI_TRACE_LOW("[MMIIM] SG_soc_connect 1 g_sg_IsWap=%d",g_sg_IsWap);//6_12_3
		if(g_sg_IsWap)//6_12_3
		{
			sa.ip_addr = htonl((uint32)SG_SOCKET_WAP_GATEWAY);//6_12_3
		}
		else//6_12_3
			sa.ip_addr = htonl((uint32)SG_SKINS_ADDR);//SG_SKINS_ADDR          0x3D87B506
	}
	else
	{
		sa.ip_addr = htonl((uint32)MMIAPIIM_SGGetServerAddress());/*lint !e666*/ //SG_SKINS_ADDR          0x3D87B506
	}

#ifdef GPRS_SUPPORT
	socket_ret=sci_sock_connect(connectSocketInput->s, &sa,sizeof(struct sci_sockaddr));  /*lint !e838*/
#endif
	if (0 > socket_ret) 
	{
		//error
#ifdef GPRS_SUPPORT
		conn_err = sci_sock_errno(connectSocketInput->s);
#endif
		if((EINPROGRESS == conn_err)||(0 == conn_err))//失败重传
		{
			return -2;
		}
		else
		{
			return -1;
		}
	}
	else
    {
		return 0;
    }
}

/***********************************************************
funtion name : SG_soc_recv	 接受数据函数
intput param
		SG_ShutDownSocketInput* shutDownSocketInput
output param
		阻塞为-2
************************************************************/
PUBLIC SG_INT SG_soc_send(SG_SendSocketInput* sendSocketInput)
{
	int32 send_len = 0;
	int conn_err;

#ifdef GPRS_SUPPORT
	send_len = sci_sock_send(sendSocketInput->s,(char*)sendSocketInput->buf,sendSocketInput->len,0);  /*lint !e838*/
#endif

	SCI_TRACE_LOW("[MMIIM] SG_soc_send 2 sendSocketInput->buf = %s , send_len = %d", sendSocketInput->buf , send_len);
    if (send_len < 0)
	{
#ifdef GPRS_SUPPORT
		conn_err = sci_sock_errno(sendSocketInput->s);
#endif
		//SCI_TraceLow:"SG_soc_send sci_sock_send err  conn_err=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_SG_INTERFACE_NET_202_112_2_18_2_29_31_301,(uint8*)"d",conn_err);
		if (EWOULDBLOCK != conn_err && ENOBUFS != conn_err)
		{
			return -1;
		}
		else
        {
			return -2;
        }
	}
	else
	{
		return send_len;
	}
}

/***********************************************************
funtion name : SG_soc_recv	 接受数据函数
intput param
		SG_ShutDownSocketInput* shutDownSocketInput
output param
		阻塞为-2
************************************************************/
PUBLIC SG_INT SG_soc_recv(SG_RecvSocketInput*  recvSocketInput)
{
	int32 receive_len = 0;
	int32 conn_err;

#ifdef GPRS_SUPPORT
	receive_len = sci_sock_recv(recvSocketInput->s, (char*)recvSocketInput->buf,recvSocketInput->len,0);  /*lint !e838*/
#endif
	SCI_TRACE_LOW("[MMIIM] SG_soc_recv 2 recvSocketInput->buf= %s,receive_len= %d" , recvSocketInput->buf , receive_len);
	//SCI_TraceLow:"SG_soc_recv recvSocketInput->s=%d,receive_len=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_SG_INTERFACE_NET_232_112_2_18_2_29_31_302,(uint8*)"dd",recvSocketInput->s,receive_len);
	if (receive_len < 0)
	{
#ifdef GPRS_SUPPORT
		conn_err = sci_sock_errno(recvSocketInput->s);
#endif
		if (EWOULDBLOCK != conn_err && ENOBUFS != conn_err)
		{
			return -1;
		}
		else
        {
			return -2;
        }
	}
	else
	{
		return receive_len;
	}
}
#endif // #if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#endif // #ifndef WIN32

/***********************************************************
funtion name : sg_trace trace 函数
intput param
output param
************************************************************/
PUBLIC void sg_trace(const SG_INT8 *fmt, ...)
{
    va_list arg_ptr; 
    
    SCI_MEMSET(g_sg_TraceStr, 0, sizeof(g_sg_TraceStr));
    va_start(arg_ptr, fmt); /*lint !e718 !e64 !e746 !e516 -esym(628,__va_start)*/
    vsprintf(g_sg_TraceStr, fmt,arg_ptr);
    va_end(arg_ptr); 
    //SCI_TraceLow:"Sogou Core trace: %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_SG_INTERFACE_NET_267_112_2_18_2_29_31_303,(uint8*)"s", g_sg_TraceStr);
}

/******************************************************************************
	功能：打开一个文件或目录
	输入参数：Flag包含以下参数及其组合，可按或组合
		EOPEN_FOR_READ_WRITE    	=	0x00000000L,读写模式
		EOPEN_FOR_READ          	=	0x00000100L,只读模式
		EOPEN_FOR_WRITE			=	0x00000001L,只写模式
		EOPEN_FOR_CREATE_ALWAYS 	=	0x00020000L,创建模式，永远创建
	输入参数：Fullname 文件路径 绝对路径
	其他值：返回（-1）失败
	返回值：
	正确打开返回 文件句柄  打开失败返回 -1

******************************************************************************/
PUBLIC SG_INT SG_FS_Open(const SG_UINT16 *FileName, SG_UINT Flag)
{
    SFS_HANDLE  file_handle = NULL;
	wchar full_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
	uint16 full_path_len = MMIFILE_FILE_NAME_MAX_LEN;
	if(2==g_sg_save_position)
	{
		SCI_TraceLow("SG_FS_Open g_sg_save_position==2 return");//6_4
		return SG_FAILED;
	}
    full_path_len = MMIAPICOM_Wstrlen((const wchar*)FileName);
	SCI_MEMSET(full_path,0,sizeof(full_path));
	SCI_MEMCPY(full_path,FileName,sizeof(WD_UINT16)*full_path_len);

    if((Flag&0x00020000) > 0)
	{
#ifndef WIN32
        file_handle = SFS_CreateFile((wchar*)FileName,SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS,NULL,NULL);
#else
		file_handle = SFS_CreateFile((wchar*)FileName,SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS,NULL,NULL);
		if(file_handle == NULL)
			return -1;
		SFS_CloseFile(file_handle);
		file_handle = SFS_CreateFile((wchar*)FileName,SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING,NULL,NULL);
#endif
	}
	else
	{
		file_handle = SFS_CreateFile((wchar*)FileName,SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING,NULL,NULL);
	}

    if (file_handle == NULL)
    {
		return SG_FAILED;
    }
	else
    {
		return file_handle;
    }
}

/********************************************************************************
	功能:  向指定文件读取数据
	输入参数: FileHandle：文件句柄
	输入参数: DataPtr：缓冲区，存储读入的内容
	输入参数: Length：预读入的内容
	输出参数: Written：实际读入长度
	返回值: 成功返回 实际读入长度 失败返回 -1 

**********************************************************************************/
PUBLIC SG_INT SG_FS_Read(FILE_HANDLE FileHandle, void * DataPtr, SG_UINT Length, SG_UINT *Read)
{
	SFS_ERROR_E sfs_err = SFS_ReadFile(FileHandle,DataPtr,Length, (uint32*)Read, PNULL);

	if (sfs_err == SFS_NO_ERROR)
	{
		return *Read;
	}
	else
	{
		return SG_FAILED;
	}
}

/********************************************************************************
	功能:  向指定文件写入指定内容
	输入参数: FileHandle：文件句柄
	输入参数：DataPtr：缓冲区，存储写入的内容
	输入参数：Length：预写入的长度
	输出参数：Written：实际写入长度
	返回值: 成功返回 实际写入长度 失败返回 -1 

**********************************************************************************/
PUBLIC SG_INT SG_FS_Write(FILE_HANDLE FileHandle, void * DataPtr, SG_UINT Length, SG_UINT * Written)
{
	SFS_ERROR_E sfs_err = SFS_WriteFile(FileHandle, DataPtr, Length, (uint32*)Written, NULL);

	if (sfs_err == SFS_NO_ERROR)
	{
		return *Written;
	}
	else
	{
		return SG_FAILED;
	}
}

/********************************************************************************	
	功能:  关闭文件
	输入参数: FileHandle：文件句柄
	返回值: 成功返回0 失败返回-1 	
**********************************************************************************/
PUBLIC SG_INT SG_FS_Close(FILE_HANDLE FileHandle)
{
	SFS_ERROR_E sfs_err = SFS_CloseFile(FileHandle);

	if (sfs_err == SFS_NO_ERROR)
	{
		return SG_SUCCESS;
	}
	else
	{
		return SG_FAILED;
	}
}

/********************************************************************************
funtion name : SG_FS_Seek      移动游标
intput param
		FILE_HANDLE FileHandle
		SG_INT Offset 	移动数量
		SG_UINT Whence  起点//0 起点,1当前,2终点
output param 
**********************************************************************************/
PUBLIC SG_INT SG_FS_Seek(FILE_HANDLE FileHandle, SG_INT Offset, SG_UINT Whence)
{
	SFS_ERROR_E sfs_err;				

	switch (Whence)
	{
  	case 0:
		sfs_err = SFS_SetFilePointer((SFS_HANDLE)FileHandle, (__int64)Offset,SFS_SEEK_BEGIN);
		break;

   	case 1:
		sfs_err = SFS_SetFilePointer((SFS_HANDLE)FileHandle, (__int64)Offset,SFS_SEEK_CUR);
		break;

   	case 2:
		sfs_err = SFS_SetFilePointer((SFS_HANDLE)FileHandle, (__int64)Offset,SFS_SEEK_END);
		break;

	default:
		return SG_FAILED;
	}

	if (sfs_err == SFS_NO_ERROR)
	{
		return SG_SUCCESS;
	}
	else
	{
		return SG_FAILED;
	}
}

/********************************************************************************
funtion name : SG_FS_Delete    删除文件
intput param
		const SG_UINT16 * FileName
output param
**********************************************************************************/
PUBLIC SG_INT SG_FS_Delete(const SG_UINT16 * FileName)
{
	SFS_ERROR_E sfs_err = SFS_DeleteFile(FileName,PNULL);

	if (sfs_err == SFS_NO_ERROR)
	{
		return SG_SUCCESS;
	}
	else
	{
		return SG_FAILED;
	}
}

/********************************************************************************
funtion name : SG_FS_GetLength      得到文件长度
intput param
		FILE_HANDLE FileHandle
output param
		SG_INT	file length
**********************************************************************************/
PUBLIC SG_UINT SG_FS_GetLength(FILE_HANDLE FileHandle)
{
	SFS_ERROR_E sfs_err;				
    uint32 file_size = 0;

    sfs_err = SFS_GetFileSize(FileHandle, &file_size);
	if (sfs_err == SFS_NO_ERROR)
    {
		return file_size;
    }
	else
    {
		return (SG_UINT)(SG_FAILED);
    }
}

/**********************************************************************	
	功能：移动或重命名文件
	输入参数：SrcFullPath 复制源地址
	输入参数：DstFullPath 复制目的地址
	返回值：复制成功 返回 0 失败返回 - 1
**********************************************************************/
PUBLIC SG_INT SG_FS_Move(const SG_UINT16 * SrcFullPath, const SG_UINT16 * DstFullPath)
{
    // 文件已经存在的时候，RENAME操作有时会不成功
    SFS_DeleteFile((const uint16*)DstFullPath, PNULL);
    if (SFS_RenameFile((const uint16*)SrcFullPath, (const uint16*)DstFullPath, PNULL) == SFS_ERROR_NONE)
    {
    	return SG_SUCCESS;
    }
    else
    {
        return SG_FAILED;
    }
}

/*********************************************************************
	功能: 查找符合条件的文件
	输入参数: NamePattern: 文件搜索通配,一般为"目录/ *"
	输出参数：IsFile: 返回是否为文件true是false否
	输出参数：FileName: 找到的文件或目录名称
	输入参数：MaxLength: 文件或目录名称最大长度
	返回值: 返回搜索句柄，失败返回-1
**********************************************************************/
PUBLIC SG_INT SG_FS_FindFirst(const SG_UINT16 *NamePattern, SG_INT *IsFile, SG_UINT16 *FileName, SG_UINT MaxLength)
{
	/*SG_INT ret = 0;
	FS_DOSDirEntry  FileInfo = {0}	;
    ret = FS_FindFirst(NamePattern, 0, 0,&FileInfo, FileName, MaxLength * ENCODE_BYTE);
	if(FileInfo.Attributes & FS_ATTR_DIR)
		*IsFile = 1;
	else
		*IsFile = 0;
	return(ret);*/
	return SG_SUCCESS;
}

/**********************************************************************
	功能: 查找下一个符合条件的文件
	输入参数: FileHandle: 搜索句柄
	输出参数：IsFile: 返回是否为文件true是false否
	输出参数：FileName: 找到的文件或目录名称（非全路径）
	输入参数：MaxLength: 文件或目录名称最大长度
	返回值: 返回搜索成功0，失败返回-1
**********************************************************************/
PUBLIC SG_INT SG_FS_FindNext(SG_INT FileHandle, SG_INT *IsFile, SG_UINT16 *FileName, SG_UINT MaxLength)
{
	/*FS_DOSDirEntry  FileInfo = {0}	;
	SG_INT ret = 0;
	ret = FS_FindNext(FileHandle, &FileInfo, FileName, MaxLength);
	if(FileInfo.Attributes & FS_ATTR_DIR)
		*IsFile = 1;
	else
		*IsFile = 0;
	return(ret);*/
	return SG_SUCCESS;
}

/***********************************************************************
	功能:  关闭搜索句柄
	输入参数: handle: 文件句柄
	返回值: 成功返回0 失败返回-1
*************************************************************************/
PUBLIC SG_INT SG_FS_FindClose(SG_INT FileHandle)
{
	//return(FS_FindClose(FileHandle));
	return SG_SUCCESS;
}

/***********************************************************
funtion name : SG_soc_shutdown	  关闭射频，不需要的可空实现
intput param
		SG_ShutDownSocketInput* shutDownSocketInput
output param
************************************************************/
PUBLIC SG_INT8 SG_soc_shutdown(SG_ShutDownSocketInput* shutDownSocketInput)
{
	return 0;
}

/***********************************************************************
功能:  创建文件夹
输入参数: filePath: 创建文件夹的名称
返回值: 成功返回0 失败返回-1
 *************************************************************************/
PUBLIC SG_INT SG_FS_CreateDir(SG_UINT16* FilePath)
{
     return 0;
}

/******************************************************************************/
// Description: malloc接口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void *SG_Malloc(SG_UINT size)
{
    return malloc(size);
}

/******************************************************************************/
// Description: Free接口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void SG_Free(void *ptr)
{
    free(ptr);
}

/******************************************************************************/
// Description: malloc接口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void *SoGou_Malloc(SG_UINT size)
{
    return malloc(size);
}

/******************************************************************************/
// Description: Free接口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void SoGou_FREE(void *ptr)
{
    free(ptr);
}

/******************************************************************************/
// Description: assert实现
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void SG_Assert(SG_INT express)
{
    if (express == 0)
    {
	    SCI_TRACE_LOW ("[MMIIM] SG_Assert : assertion failed!");
        SCI_ASSERT(express); /*assert verified*/
    }
}

/******************************************************************************/
// Description: 获取tick count
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC SG_UINT SG_GetTickCount(void)
{
	return SCI_GetTickCount();
}

#define SG_MINIMIZE_CHINESE_WORD_SPACE  (260 * 1024) //最小中文自造词文件大小
//返回1 说明空间足够
//返回0 说明空间不够
PUBLIC int SG_Check_D_Space_Is_Enough(void)
{
    BOOLEAN is_space_enough = FALSE;
    uint32 free_high_word = 0;
    uint32 free_low_word  = 0;
	if (MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK),
			MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK),
								   &free_high_word, &free_low_word))
	{
		if (!(0 == free_high_word && SG_MINIMIZE_CHINESE_WORD_SPACE > free_low_word))
		{
			is_space_enough = TRUE;
		}
	}
	if(is_space_enough)
		return 1;
	else
		return 0;

}

//返回1 说明空间足够
//返回0 说明空间不够
PUBLIC int SG_Check_E_Space_Is_Enough(void)
{
    BOOLEAN is_space_enough = FALSE;
    uint32 free_high_word = 0;
    uint32 free_low_word  = 0;
	if (MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
			MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
								   &free_high_word, &free_low_word))
	{
		if (!(0 == free_high_word && (SG_UPDATE_WORD_MINIMIZE_FREESPACE+SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE) > free_low_word))	/*lint !e574 !e737*/
		{
			is_space_enough = TRUE;
		}
	}
	if(is_space_enough)
		return 1;
	else
		return 0;

}



/******************************************************************************/
// Description: 检测SG目录的文件时间，因在T卡测试时，如果down的事一个8000的单词版本，
// 				稍候继续down一个1000的版本测试，则会引起死机问题
//			通过检测当前版本的编译时间和T卡中存在的sogou目录文件时间来决定是否删除已经存在的文件
//			sogou文件创建时，以编译时间命名	
// Global resource dependence: NONE
// Author: Ming.song
// Note: 
// Modify:去掉D,E盘检测相关功能  yaoguang.chen
/******************************************************************************/
PUBLIC void SG_Check_Build_Time (void)//6_19_1
{
    char*          now_build_time = PNULL;
    char build_time_filename[30+1] = { 0 };
    MMIFILE_HANDLE filehandle;
    uint32 readed = 0;
    uint32  write_size  = 0;
#ifndef _WIN32
	now_build_time = VERSION_GetInfo(BUILD_TIME);	/*lint !e605*/
#else
	now_build_time = "2012-06-19";
#endif	

	SCI_TraceLow("[MMIIM]SG_Check_Build_Time now_build_time=%s",now_build_time);

	//创建编译时间文件，如果已经存在，则直接使用该文件，得到该文件句柄
	if(0==g_sg_save_position)
	{
		filehandle = MMIAPIFMM_CreateFile((const wchar*)SG_D_BUILD_TIME, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
	}
	else if(1==g_sg_save_position)
	{
		filehandle = MMIAPIFMM_CreateFile((const wchar*)SG_E_BUILD_TIME, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
	}
	else
		return;
	SCI_TraceLow("[MMIIM]SG_Check_Build_Time filehandle=0x%04x",filehandle);
	if (filehandle)		//得到编译文件时间信息
	{
		SCI_MEMSET (build_time_filename, 0, sizeof(build_time_filename));
		MMIAPIFMM_ReadFile(filehandle, (void*)build_time_filename, 30, &readed, PNULL);
		SCI_TraceLow("[MMIIM]SG_Check_Build_Time build_time_filename=%s", build_time_filename);
	}
	
     //编译文件时间比系统编译时间早(如果buildtime文件不存在，默认清除bin文件)//6_19_1(2012_12_21)
    if ((readed > 0 && strcmp(build_time_filename, now_build_time) < 0) || readed == 0)//6_19_1(2012_12_21)
	{
		if(0==g_sg_save_position)
		{
			SCI_TraceLow("SG_Check_Build_Time del 1");
        	SFS_DeleteFile(L"D:\\@sogou\\pyuser.bin", PNULL);
        	SFS_DeleteFile(L"D:\\@sogou\\bhuser.bin", PNULL);
		}
		else if(1==g_sg_save_position)
		{
			SCI_TraceLow("SG_Check_Build_Time del 2");
        	SFS_DeleteFile(L"E:\\@sogou\\pyuser.bin", PNULL);
        	SFS_DeleteFile(L"E:\\@sogou\\bhuser.bin", PNULL);
		}
		if(0==g_sg_save_position)
		{
			SCI_TraceLow("SG_Check_Build_Time del 3");
			SFS_DeleteFile(SG_D_BUILD_TIME, PNULL);
		}
		else if(1==g_sg_save_position)
		{
			SCI_TraceLow("SG_Check_Build_Time del 4");
			SFS_DeleteFile(SG_E_BUILD_TIME, PNULL);
		}
		SFS_CloseFile(filehandle);
		filehandle=0;
	}
	
	if(0 == filehandle)		//前头如果编译文件时间比系统时间早，则会删除编译时间文件，重新创建
	{
		if(0==g_sg_save_position)
		{
			filehandle = SFS_CreateFile(SG_D_BUILD_TIME, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
		}
		else if(1==g_sg_save_position)
		{
			filehandle = SFS_CreateFile(SG_E_BUILD_TIME, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
		}
	}
    SFS_WriteFile(filehandle, now_build_time, strlen(now_build_time), &write_size, NULL);
	SCI_TraceLow("SG_Check_Build_Time SFS_WriteFile filehandle=0x%04x,write_size=%d",filehandle,write_size);
    SFS_CloseFile(filehandle);
}

#ifdef IME_Initialize//6_12_2
#undef IME_Initialize
#endif
#ifdef SG_Quit//6_12_2
#undef SG_Quit
#endif
int g_sg_imeFlag = 0;//6_12_2
WD_UINT8 IME_Initialize_ext(WD_UINT8* pImeTypeList,  WD_UINT8 defaultIme,
						WD_UINT8* pFlashData,WD_imeResult*	pResult)//6_12_2
{
	g_sg_imeFlag = 1;
	return(IME_Initialize(pImeTypeList,defaultIme,pFlashData,pResult));		/*lint !e718 !e746 !e628*/
}

void SG_Quit_ext(void)//6_12_2
{
	if(g_sg_imeFlag == 0)
		return;
	g_sg_imeFlag = 0;
	SG_Quit();		/*lint !e718 !e746 !e628*/
}
#endif //WDP_SG_INTERFACE_NET_C
