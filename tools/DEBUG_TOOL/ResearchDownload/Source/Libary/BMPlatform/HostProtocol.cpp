#pragma once
#include "stdafx.h"
#include "HostProtocol.h"
#include <exception>

CHostProtocol::CHostProtocol(void)
{
	m_pFileBuf = NULL;
	m_dwFileSize = 0;
	m_nDataSize = 0;
	m_dwMaxLength = MAX_HOST_DATA_PACKET_SIZE;
	m_pPackageBuf = (LPBYTE)GetMemory(MAX_HOST_PACKAGE_LEN);
	m_pRingBuf = (LPBYTE)GetMemory(MAX_HOST_PACKAGE_LEN);
}

CHostProtocol::~CHostProtocol(void)
{
	FreeMemory(m_pFileBuf);
	FreeMemory(m_pPackageBuf);
	FreeMemory(m_pRingBuf);
}

LPVOID	CHostProtocol::GetMemory( DWORD dwSize)
{
	if( dwSize == 0)
	{
		return NULL;
	}

	LPBYTE lpBuf = NULL;
	try
	{
		lpBuf = new BYTE[dwSize];
	}
	catch (const std::bad_alloc& /*e*/)
	{
		lpBuf = NULL;
	}

	return lpBuf;

}

void 	CHostProtocol::FreeMemory( LPBYTE& lpMemBlock )
{
	if(lpMemBlock != NULL)
	{
		delete[] lpMemBlock;
		lpMemBlock = NULL;
	}
}

BOOL CHostProtocol::CheckPackageMemory()
{
	if (NULL==m_pPackageBuf)
	{
		m_pPackageBuf = (LPBYTE)GetMemory(MAX_HOST_PACKAGE_LEN);
	}
	return m_pPackageBuf ? TRUE : FALSE;
}

/** 
* @brief               <p> 计算一个包的完整大小 </p>
* @param int nDataLen  <p> 包中的数据大小 </p>
* @return value int    <p> 包的大小 </p>
*/
int CHostProtocol::Calc_Packet_Size(int nDataLen)
{
	int nSizes = 0;
	nSizes += PACKET_PACKETHEAD_SZIE;
	nSizes += PACKET_DATAHEAD_SIZE;
	nSizes += nDataLen;
	return nSizes;
}

/** 
* @brief                    <p> 组装connect命令 </p>
* @param LPBYTE pDestBuff   <p> 目标缓冲区 </p>
* @param int nBuffLen       <p> 目标缓冲区的大小 </p>
* @param value BOOL         <p> TRUE/FALSE </p>
*/
BOOL CHostProtocol::Assemble_Packet_Connect(LPBYTE& pDestBuff, int &nBuffLen)
{
	m_nDataSize = 0;
	int nPkgSizes = Calc_Packet_Size(0);
	if (!CheckPackageMemory() || nPkgSizes > MAX_HOST_PACKAGE_LEN)
	{
		return FALSE;
	}

	T_Packet PktConnect;
	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));
	DeclareHostHeader(PktConnect.PkgHeader,0);
	DeclareHostData(PktConnect.PkgPdl,CMD_CONNECT,0x00,0,NULL);

	/// 组装数据
	Assemble_Packet(PktConnect, m_pPackageBuf, 0);
	pDestBuff = m_pPackageBuf;
	nBuffLen = nPkgSizes;
	return TRUE;
}


/** 
* @brief                  <p> 组装一个数据包 </p>
* @param int nPacketNo    <p> 数据包序号 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @param value BOOL         <p> TRUE/FALSE </p>
*/
BOOL CHostProtocol::Assemble_Packet_Data(int nPacketNo, LPBYTE& pDestBuff, int& nDestBuffLen)
{
    m_nDataSize = 0;
	LPBYTE pDataBuf = NULL; //m_pFileBuf中的偏移
	int nDataLen = 0;
	
	/// 获取该包中的数据缓存及大小
    if ( !Get_Packet_FDL1(nPacketNo, pDataBuf, nDataLen))
    {
		return FALSE;
    }

	/// 完整数据包的大小
	int nPkgSizes = Calc_Packet_Size(nDataLen);
	if (!CheckPackageMemory() || nPkgSizes > MAX_HOST_PACKAGE_LEN)
	{
		return FALSE;
	}
	T_Packet PktConnect;
	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));
	DeclareHostHeader(PktConnect.PkgHeader,nDataLen);
	DeclareHostData(PktConnect.PkgPdl,CMD_MID_DATA,nPacketNo,nDataLen,pDataBuf);

	/// 组装数据
	Assemble_Packet(PktConnect, m_pPackageBuf, nDataLen);
	pDestBuff = m_pPackageBuf;
	nDestBuffLen = nPkgSizes;
	return TRUE;
}

/** 
* @brief                  <p> 组装开始数据包 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @param value BOOL         <p> TRUE/FALSE </p>
*/
BOOL CHostProtocol::Assemble_Packet_Start(LPBYTE& pDestBuff, int &nBuffLen)
{
    m_nDataSize = 0;

#define MAX_START_FLAG_LEN (5)
	BYTE StartFlag[MAX_START_FLAG_LEN] = {0x50, 0x44, 0x4c, 0x31, 0x00};      //< "PDL1"
	int nPkgSizes = Calc_Packet_Size(MAX_START_FLAG_LEN);
	if (!CheckPackageMemory() || nPkgSizes > MAX_HOST_PACKAGE_LEN)
	{
		return FALSE;
	}

	T_Packet PktConnect;
	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));
	DeclareHostHeader(PktConnect.PkgHeader,MAX_START_FLAG_LEN);
	DeclareHostData(PktConnect.PkgPdl,CMD_START_DATA,m_dwAddress,m_dwFileSize,StartFlag);

	/// 组装数据
	Assemble_Packet(PktConnect, m_pPackageBuf, MAX_START_FLAG_LEN);
	pDestBuff = m_pPackageBuf;
	nBuffLen = nPkgSizes;
	return TRUE;
}

/** 
* @brief                  <p> 组装结束数据包 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @param value BOOL         <p> TRUE/FALSE </p>
*/
BOOL CHostProtocol::Assemble_Packet_End(LPBYTE& pDestBuff, int &nBuffLen)
{
    m_nDataSize = 0;

#define MAX_END_FLAG_LEN (4)
	BYTE EndFlag[MAX_END_FLAG_LEN] = {0x1c, 0x3c, 0x6e, 0x06};
	int nPkgSizes = Calc_Packet_Size(MAX_END_FLAG_LEN);
	if (!CheckPackageMemory() || nPkgSizes > MAX_HOST_PACKAGE_LEN)
	{
		return FALSE;
	}

	T_Packet PktConnect;
	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));
	DeclareHostHeader(PktConnect.PkgHeader,MAX_END_FLAG_LEN);
	DeclareHostData(PktConnect.PkgPdl,CMD_END_DATA,0,0,EndFlag);

	/// 组装数据
	Assemble_Packet(PktConnect, m_pPackageBuf, MAX_END_FLAG_LEN);
	pDestBuff = m_pPackageBuf;
	nBuffLen = nPkgSizes;
	return TRUE;
}


/** 
* @brief                  <p> 组装执行数据包 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @param value BOOL         <p> TRUE/FALSE </p>
*/
BOOL CHostProtocol::Assemble_Packet_Execute(LPBYTE& pDestBuff, int &nBuffLen)
{
	m_nDataSize = 0;
	int nPkgSizes = Calc_Packet_Size(0);
	if (!CheckPackageMemory() || nPkgSizes > MAX_HOST_PACKAGE_LEN)
	{
		return FALSE;
	}

	T_Packet PktConnect;
	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));
	DeclareHostHeader(PktConnect.PkgHeader,0);
	DeclareHostData(PktConnect.PkgPdl,CMD_EXEC_DATA,0x00,0,NULL);

	/// 组装数据
	Assemble_Packet(PktConnect, m_pPackageBuf, 0);
	pDestBuff = m_pPackageBuf;
	nBuffLen = nPkgSizes;
	return TRUE;
}

/** 
* @brief                     <p> 组装数字包 </p>
* @param T_Packet& sPacket   <p> 数据包的基本信息 </p>
* @param LPBYTE pDestBuff    <p> 存放数据的目标缓冲区 </p>
* @param int nBuffLen        <p> 数据包的大小 </p>
*/
void CHostProtocol::Assemble_Packet(const T_Packet& sPacket, LPBYTE pDestBuff, int nBuffLen)
{
    LPBYTE pDst = pDestBuff;
	int nCpyLen = sizeof(T_Packet)-4;
	memcpy(pDst,&sPacket,nCpyLen);
	pDst += nCpyLen;
	if(sPacket.PkgPdl.pData && nBuffLen)
	{
		memcpy(pDst, sPacket.PkgPdl.pData, nBuffLen);
	}
}

/** 
* @brief                    <p> 解码命令应答结果 </p>
* @param LPBYTE pDataBuff   <p> 源数据缓冲区 </p>
* @param int nDataSize      <p> 缓冲区中的数据大小 </p>
* @param value BOOL         <p> 应答OK，返回TRUE，否则，返回FALSE </p>
*/
BOOL CHostProtocol:: Decode_Response(LPBYTE pDataBuff, int nDataSize)
{
	//[2019-03-22 18:24:17:957] <<- 12(0x0000000c)/12(0x0000000c) Bytes
	//00000000h: AE 04 00 00 00 FF 00 00 00 00 00 00             ; ............
	
	if (NULL == m_pRingBuf)
	{
		return FALSE;
	}
	if (m_nDataSize+nDataSize > MAX_HOST_PACKAGE_LEN)
	{
		m_nDataSize = 0;
	}
	memcpy(m_pRingBuf+m_nDataSize,pDataBuff,nDataSize);
	m_nDataSize += nDataSize;
	if (m_nDataSize<PACKET_PACKETHEAD_SZIE)
	{
		return FALSE;
	}
	
	BOOL bOK = FALSE;
    int nOffset = 0;
	int nLeft = m_nDataSize;
	while(nLeft > 0)
	{
		if (nLeft < PACKET_PACKETHEAD_SZIE)
		{
			break;
		}
		T_Packet_Header   PkgHeader;
		memcpy(&PkgHeader,m_pRingBuf+nOffset,sizeof(T_Packet_Header));
		if (
			PkgHeader.ucTag == PACKET_FLAG_TAG && 
			PkgHeader.ucFlowID == PACKET_FLAG_FLOWID &&
			PkgHeader.wReserved == 0x00
			)
		{
			nOffset += PACKET_PACKETHEAD_SZIE;
			nLeft -= PACKET_PACKETHEAD_SZIE;
			if(nLeft < 4)
			{
				break;
			}
			if (RSP_ACK == *((DWORD *)(m_pRingBuf + nOffset)))
			{
				bOK = TRUE;
			}
			nOffset += 4;
			nLeft -= 4;
			memmove(m_pRingBuf,m_pRingBuf+nOffset,nLeft);
			break;

		}
		else
		{
			++nOffset;
			--nLeft;
		}
		
	}
	return bOK;

}

BOOL CHostProtocol::SetFdlFileInfo(const BMFileInfo* pBMFileInfo)
{
	BOOL bOK = FALSE;
	do 
	{
		if(NULL == pBMFileInfo || NULL == pBMFileInfo->hFDLCode || 0 == pBMFileInfo->llCodeSize )
		{
			break;
		}

		LARGE_INTEGER liOffset;
		liOffset.QuadPart = pBMFileInfo->llCodeOffset;
		if (::SetFilePointer(pBMFileInfo->hFDLCode,liOffset.LowPart, &liOffset.HighPart,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			break;
		}

		FreeMemory(m_pFileBuf);
		DWORD dwTmpSize = (DWORD)(pBMFileInfo->llCodeSize);
		m_pFileBuf = (LPBYTE)GetMemory(dwTmpSize);
		if(NULL == m_pFileBuf)
		{
			break;
		}

		DWORD dwReaded = 0;
		if(!::ReadFile(pBMFileInfo->hFDLCode,m_pFileBuf,dwTmpSize,&dwReaded,NULL) || dwReaded != dwTmpSize)
		{
			FreeMemory(m_pFileBuf);
			break;
		}

		m_dwFileSize = (DWORD)(pBMFileInfo->llCodeSize);
		m_dwAddress = (DWORD)(pBMFileInfo->llBase);
		m_dwMaxLength = pBMFileInfo->dwMaxLength ?  pBMFileInfo->dwMaxLength : MAX_HOST_DATA_PACKET_SIZE;
		bOK = TRUE;

	} while (0);
	return bOK;
}

/** 
* @brief      <p> 计算发送的数据包数 </p>
*/
int CHostProtocol::CountPackets()
{
	int nPacketCount = 0;

	if(m_dwFileSize == 0 || NULL == m_pFileBuf || 0 == m_dwMaxLength)
	{
		return nPacketCount;
	}

	nPacketCount = ((m_dwFileSize)% m_dwMaxLength ) ? ((m_dwFileSize)/m_dwMaxLength + 1) : ((m_dwFileSize)/m_dwMaxLength) ;

	return nPacketCount;
}


/** 
* @brief                      <p> 获取一个数据包的数据 <p>
* @param int nPacketNo        <p> 数据包的序号 </p>
* @param LPBYTE pDestBuff     <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen     <p> 目标缓冲区的大小 </p>
* @return value int           <p> 数据包的大小 </p>
*/
BOOL CHostProtocol::Get_Packet_FDL1(int nPacketNo, LPBYTE& pDestBuff, int& nDestBuffLen)
{
	int nPacketNums = CountPackets();                       //< 发送的总包数
	if(nPacketNums == 0 || nPacketNo < 0 || nPacketNo >= nPacketNums || 0 == m_dwMaxLength)
	{
		return FALSE;
	}

	/// 计算数据包的大小
	if(nPacketNo < nPacketNums - 1)
	{
		nDestBuffLen = m_dwMaxLength;
	}
	else
	{
		nDestBuffLen = m_dwFileSize - nPacketNo * m_dwMaxLength; 
	}
	/// 数据包在FDL1中的偏移地址
	pDestBuff = m_pFileBuf + nPacketNo * m_dwMaxLength;

	return TRUE;
}


