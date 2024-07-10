#pragma once
#include "stdafx.h"
#include "HostProtocol.h"

CHostProtocol::CHostProtocol(void)
{
	m_nLastErrNo = RSP_ACK;

	m_strFile.Empty();
	m_pFile = NULL;
	m_nFileSizes = 0;

}

CHostProtocol::~CHostProtocol(void)
{

	m_strFile.Empty();

	if(m_pFile)
	{
		delete []m_pFile;
		m_pFile = NULL;
	}
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
* @param value int          <p> 缓冲区中的数据大小 </p>
*/
int CHostProtocol::Assemble_Patket_Connect(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;
	int nSizes = 0;
	int nDataLen = 0;

	/// 完整数据包的大小
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

        nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Patket_Connect: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
    PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType  = CMD_CONNECT;
	PktConnect.PkgPdl.dwDataAddr = 0x00;
	PktConnect.PkgPdl.dwDataSize = 0;
	PktConnect.PkgPdl.pData      = NULL;
	PktConnect.PkgPdl.nPackSize  = nDataLen;

	/// 组装数据
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}


/** 
* @brief                  <p> 组装一个数据包 </p>
* @param int nPacketNo    <p> 数据包序号 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @return value int       <p> 数据包的大小 </p>
*/
int CHostProtocol::Assemble_Packet_Data(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen)
{
	T_Packet PktConnect;
	int nSizes = 0;
	int nDataLen = 0;

	LPBYTE pData = new BYTE[MAX_PACKET_SIZE];         //< 存放包中的数据

	if(!pData)
	{
		::AfxMessageBox(_T("Assemble_Packet_Data: memory allocation failure!"));
		return nSizes;
	}
	
	/// 获取该包中的数据及大小
    nDataLen = Get_Packet_FDL1(nPacketNo, pData, MAX_PACKET_SIZE);

	/// 完整数据包的大小
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nDestBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nDestBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Packet_Data: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG; 
	PktConnect.PkgHeader.nDataSize = nDataLen + PACKET_DATAHEAD_SIZE;   //< 数据大小
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType  = CMD_MID_DATA;                        //< 中间数据包
	PktConnect.PkgPdl.dwDataAddr = nPacketNo;                           //< 数据包的序号
	PktConnect.PkgPdl.dwDataSize = nDataLen;
	PktConnect.PkgPdl.pData      = pData;                               //< 包中数据存放的缓冲区
	PktConnect.PkgPdl.nPackSize  = nDataLen;                            //< 包中数据的大小

	/// 组装数据
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	/// 释放内存
	delete []pData;
	pData = NULL;

	return nSizes;
}



/** 
* @brief                  <p> 组装开始数据包 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @return value int       <p> 数据包的大小 </p>
*/
int CHostProtocol::Assemble_Patket_Start(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;

	char FileFlag[5] = {0x50, 0x44, 0x4c, 0x31, 0x00};      //< "PDL1"
	int nDataLen = 5;
    int nSizes = 0;
	
	/// 完整数据包的大小
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Packet_Data: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType    = CMD_START_DATA;             //< START_DATA
	PktConnect.PkgPdl.dwDataAddr   = (DWORD)FDL1_ADDRESS;        //< FDL1 文件的烧写地址
	PktConnect.PkgPdl.dwDataSize   = m_nFileSizes;               //< FDL1文件的大小
	PktConnect.PkgPdl.pData        = (BYTE *)&FileFlag;          //< 数据缓冲区
	PktConnect.PkgPdl.nPackSize    = nDataLen;                   //< 数据大小

	/// 组装数据
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}


/** 
* @brief                  <p> 组装结束数据包 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @return value int       <p> 数据包的大小 </p>
*/
int CHostProtocol::Assemble_Patket_End(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;

	char Data[4] = {0x1c, 0x3c, 0x6e, 0x06};
	int nDataLen = 4;
	int nSizes = 0;

	/// 完整数据包的大小
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Patket_End: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType    = CMD_END_DATA;               //< END_DATA
	PktConnect.PkgPdl.dwDataAddr   = 0;                          //< 烧写地址
	PktConnect.PkgPdl.dwDataSize   = 0;                          //< FDL1文件的大小
	PktConnect.PkgPdl.pData        = (BYTE *)&Data;              //< 数据缓冲区
	PktConnect.PkgPdl.nPackSize    = nDataLen;                   //< 数据大小

	/// 组装数据
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}


/** 
* @brief                  <p> 组装执行数据包 </p>
* @param LPBYTE pDestBuff <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen <p> 目标缓冲区的大小 </p>
* @return value int       <p> 数据包的大小 </p>
*/
int CHostProtocol::Assemble_Patket_Execute(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;

	int nDataLen = 0;
	int nSizes = 0;

	/// 完整数据包的大小
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Patket_Execute: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType    = CMD_EXEC_DATA;              //< END_DATA
	PktConnect.PkgPdl.dwDataAddr   = 0;                          //< 烧写地址
	PktConnect.PkgPdl.dwDataSize   = 0;                          //< FDL1文件的大小
	PktConnect.PkgPdl.pData        = NULL;                       //< 数据缓冲区
	PktConnect.PkgPdl.nPackSize    = nDataLen;                   //< 数据大小

	/// 组装数据
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}

/** 
* @brief                     <p> 组装数字包 </p>
* @param T_Packet& sPacket   <p> 数据包的基本信息 </p>
* @param LPBYTE pDestBuff    <p> 存放数据的目标缓冲区 </p>
* @param int nBuffLen        <p> 数据包的大小 </p>
* @return value int          <p> 数据包的大小 </p> 
*/
int CHostProtocol::Assemble_Patket(T_Packet& sPacket, LPBYTE pDestBuff, int nBuffLen)
{
    LPBYTE pDst = pDestBuff;

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.ucTag, sizeof(sPacket.PkgHeader.ucTag));
	pDst += sizeof(sPacket.PkgHeader.ucTag);

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.nDataSize, sizeof(sPacket.PkgHeader.nDataSize));
	pDst += sizeof(sPacket.PkgHeader.nDataSize);

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.ucFlowID, sizeof(sPacket.PkgHeader.ucFlowID));
	pDst += sizeof(sPacket.PkgHeader.ucFlowID);

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.wReserved, sizeof(sPacket.PkgHeader.wReserved));
	pDst += sizeof(sPacket.PkgHeader.wReserved);

	memcpy(pDst,(BYTE *)&sPacket.PkgPdl.dwCmdType, sizeof(sPacket.PkgPdl.dwCmdType));
	pDst += sizeof(sPacket.PkgPdl.dwCmdType);

	memcpy(pDst,(BYTE *)&sPacket.PkgPdl.dwDataAddr, sizeof(sPacket.PkgPdl.dwDataAddr));
	pDst += sizeof(sPacket.PkgPdl.dwDataAddr);

	memcpy(pDst,(BYTE *)&sPacket.PkgPdl.dwDataSize, sizeof(sPacket.PkgPdl.dwDataSize));
	pDst += sizeof(sPacket.PkgPdl.dwDataSize);

	if(sPacket.PkgPdl.pData)
	{
		memcpy(pDst, sPacket.PkgPdl.pData, sPacket.PkgPdl.nPackSize);
		pDst += sPacket.PkgPdl.nPackSize;
	}

	return nBuffLen;
}

/** 
* @brief                    <p> 解码命令应答结果 </p>
* @param LPBYTE pDataBuff   <p> 源数据缓冲区 </p>
* @param int nDataSize      <p> 缓冲区中的数据大小 </p>
* @param value BOOL         <p> 应答OK，返回TRUE，否则，返回FALSE </p>
*/
BOOL CHostProtocol:: Decode_Response(LPBYTE pDataBuff, int nDataSize)
{
	if(nDataSize < PACKET_MIN_SIEZ)
	{
		return FALSE;
	}

    T_Packet sPacket;

    sPacket.PkgHeader.ucTag     = *pDataBuff;
	sPacket.PkgHeader.nDataSize = *((int *)(pDataBuff + 1));
	sPacket.PkgHeader.ucFlowID  = *(pDataBuff + 5);
	sPacket.PkgHeader.wReserved = *((WORD *)(pDataBuff + 6));

	sPacket.PkgPdl.dwCmdType = *((DWORD *)(pDataBuff + 8));

    if(sPacket.PkgHeader.ucTag == PACKET_FLAG_TAG \
		&& sPacket.PkgHeader.ucFlowID == PACKET_FLAG_FLOWID \
		&& sPacket.PkgPdl.dwCmdType == RSP_ACK)
	{
		return TRUE;
	}

	m_nLastErrNo = sPacket.PkgPdl.dwCmdType;

	return FALSE;
}


/** 
* @brief      <p> 最后一次的错误信息 </p>
*/
CString CHostProtocol::GetLastErrInfo()
{
	int nErrCount = sizeof(ErrInfo) / sizeof(TCHAR *);

	if(m_nLastErrNo < nErrCount)
	{
        return ErrInfo[m_nLastErrNo];
	}

	return _T("");
}


/** 
* @brief   <p> 设置FDL1文件路径名 </p>
*/
void CHostProtocol::SetFDL1Path(CString strFDL1File)
{
	m_strFile = strFDL1File;
}


/** 
* @brief      <p> 计算发送的数据包数 </p>
*/
int CHostProtocol::CountPackets()
{
	int nPacketCount = 0;

	if(m_strFile.IsEmpty())
	{
		return nPacketCount;
	}

	CFile sFile;

	if( !sFile.Open(m_strFile,CFile::modeRead) )
	{
		::AfxMessageBox(_T("Open file Fail"));
		return nPacketCount;
	}

	if(m_pFile)
	{
		delete []m_pFile;
		m_pFile = NULL;

		m_nFileSizes = 0;
	}

	m_nFileSizes = (int)sFile.GetLength();

    m_pFile = new BYTE[m_nFileSizes + 1];

	if(m_pFile)
	{
		sFile.Read(m_pFile, m_nFileSizes);
		nPacketCount = (m_nFileSizes - 1) / MAX_PACKET_SIZE + 1;
	}
	else
	{
		::AfxMessageBox(_T("Open file Fail!"));
	}

	sFile.Close();


	return nPacketCount;
}


/** 
* @brief                      <p> 获取一个数据包的数据 <p>
* @param int nPacketNo        <p> 数据包的序号 </p>
* @param LPBYTE pDestBuff     <p> 存放数据包的目标缓冲区 </p>
* @param int nDestBuffLen     <p> 目标缓冲区的大小 </p>
* @return value int           <p> 数据包的大小 </p>
*/
int CHostProtocol::Get_Packet_FDL1(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen)
{
	int nPackSizes = 0;                                     //< 数据包的大小
	int nPactetOffset = 0;                                  //< 数据的开始偏移
	int nPacketNums = CountPackets();                       //< 发送的总包数

	if(nPacketNums == 0)
	{
		return nPackSizes;
	}

	if(nPacketNo < 0 || nPacketNo >= nPacketNums)
	{
        return nPackSizes;
	}

	/// 计算数据包的大小
	if(nPacketNo < nPacketNums - 1)
	{
		nPackSizes = MAX_PACKET_SIZE;
	}
	else
	{
		nPackSizes = m_nFileSizes - nPacketNo * MAX_PACKET_SIZE; 
	}

	/// 包的数据FDL1总的便宜地址
    nPactetOffset = nPacketNo * MAX_PACKET_SIZE; 

	if(nDestBuffLen < nPackSizes)
	{
		delete []pDestBuff;
		pDestBuff = NULL;
	}

	if(!pDestBuff)
	{
		pDestBuff = new BYTE[nPackSizes + 1];
	}

	memcpy(pDestBuff, (LPBYTE)(m_pFile + nPactetOffset), nPackSizes);

	return nPackSizes;
}


